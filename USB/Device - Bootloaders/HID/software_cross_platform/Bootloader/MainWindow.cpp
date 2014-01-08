/************************************************************************
* Copyright (c) 2009-2011,  Microchip Technology Inc.
*
* Microchip licenses this software to you solely for use with Microchip
* products.  The software is owned by Microchip and its licensors, and
* is protected under applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED "AS IS."  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT
* NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL
* MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
* CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR
* EQUIPMENT, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY
* OR SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED
* TO ANY DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION,
* OR OTHER SIMILAR COSTS.
*
* To the fullest extent allowed by law, Microchip and its licensors
* liability shall not exceed the amount of fees, if any, that you
* have paid directly to Microchip to use this software.
*
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
* OF THESE TERMS.
*
* Author        Date        Ver   Comment
*************************************************************************
* E. Schlunder  2009/04/14  0.01  Initial code ported from VB app.
* T. Lawrence   2011/01/14  2.90  Initial implementation of USB version of this
*                                 bootloader application.
* F. Schlunder  2011/07/06  2.90a Small update to support importing of hex files
*                                 with "non-monotonic" line address ordering.
************************************************************************/

#include <QTextStream>
#include <QByteArray>
#include <QList>
#include <QTime>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QSettings>
#include <QtWidgets/QDesktopWidget>
#include <QtConcurrent/QtConcurrentRun>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Settings.h"

#include "../version.h"




//Surely the micro doesn't have a programmable memory region greater than 268 Megabytes...
//Value used for error checking device reponse values.
#define MAXIMUM_PROGRAMMABLE_MEMORY_SEGMENT_SIZE 0x0FFFFFFF

bool deviceFirmwareIsAtLeast101 = false;
Comm::ExtendedQueryInfo extendedBootInfo;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    int i;
    hexOpen = false;
    fileWatcher = NULL;
    timer = new QTimer();

    ui->setupUi(this);
    setWindowTitle(APPLICATION + QString(" v") + VERSION);

    QSettings settings;
    settings.beginGroup("MainWindow");
    fileName = settings.value("fileName").toString();

    for(i = 0; i < MAX_RECENT_FILES; i++)
    {
        recentFiles[i] = new QAction(this);
        connect(recentFiles[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
        recentFiles[i]->setVisible(false);
        ui->menuFile->insertAction(ui->actionExit, recentFiles[i]);
    }
    ui->menuFile->insertSeparator(ui->actionExit);

    settings.endGroup();

    settings.beginGroup("WriteOptions");
    writeFlash = settings.value("writeFlash", true).toBool();
    //writeConfig = settings.value("writeConfig", false).toBool();
    writeConfig = false; //Force user to manually re-enable it every time they re-launch the application.  Safer that way.
    writeEeprom = settings.value("writeEeprom", false).toBool();
    eraseDuringWrite = true;
    settings.endGroup();

    comm = new Comm();
    deviceData = new DeviceData();
    hexData = new DeviceData();

    device = new Device(deviceData);

    qRegisterMetaType<Comm::ErrorCode>("Comm::ErrorCode");

    connect(timer, SIGNAL(timeout()), this, SLOT(Connection()));
    connect(this, SIGNAL(IoWithDeviceCompleted(QString,Comm::ErrorCode,double)), this, SLOT(IoWithDeviceComplete(QString,Comm::ErrorCode,double)));
    connect(this, SIGNAL(IoWithDeviceStarted(QString)), this, SLOT(IoWithDeviceStart(QString)));
    connect(this, SIGNAL(AppendString(QString)), this, SLOT(AppendStringToTextbox(QString)));
    connect(this, SIGNAL(SetProgressBar(int)), this, SLOT(UpdateProgressBar(int)));
    connect(comm, SIGNAL(SetProgressBar(int)), this, SLOT(UpdateProgressBar(int)));


    this->statusBar()->addPermanentWidget(&deviceLabel);
    deviceLabel.setText("Disconnected");

    //Make initial check to see if the USB device is attached
    comm->PollUSB();
    if(comm->isConnected())
    {
        qWarning("Attempting to open device...");
        comm->open();
        ui->plainTextEdit->setPlainText("Device Attached.");
        ui->plainTextEdit->appendPlainText("Connecting...");
        GetQuery();
    }
    else
    {
        ui->plainTextEdit->appendPlainText("Device not detected.  Verify device is attached and in firmware update mode.");
        deviceLabel.setText("Disconnected");
        hexOpen = false;
        setBootloadEnabled(false);
        emit SetProgressBar(0);
    }

    //Update the file list in the File-->[import files list] area, so the user can quickly re-load a previously used .hex file.
    UpdateRecentFileList();


    timer->start(1000); //Check for future USB connection status changes every 1000 milliseconds.
}

MainWindow::~MainWindow()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("fileName", fileName);
    settings.endGroup();

    settings.beginGroup("WriteOptions");
    settings.setValue("writeFlash", writeFlash);
    settings.setValue("writeConfig", writeConfig);
    settings.setValue("writeEeprom", writeEeprom);
    settings.endGroup();

    comm->close();
    setBootloadEnabled(false);

    delete timer;
    delete ui;
    delete comm;
    delete deviceData;
    delete hexData;
    delete device;
}

void MainWindow::Connection(void)
{
    bool currStatus = comm->isConnected();
    Comm::ErrorCode result;

    comm->PollUSB();

    if(currStatus != comm->isConnected())
    {
        UpdateRecentFileList();

        if(comm->isConnected())
        {
            qWarning("Attempting to open device...");
            comm->open();
            ui->plainTextEdit->setPlainText("Device Attached.");
            ui->plainTextEdit->appendPlainText("Connecting...");
            if(writeConfig)
            {
                //ui->plainTextEdit->appendPlainText("Disabling Erase button to prevent accidental erasing of the configuration words without reprogramming them\n");
                writeConfig = true;
                result = comm->LockUnlockConfig(false);
                if(result == Comm::Success)
                {
                    ui->plainTextEdit->appendPlainText("Unlocked Configuration bits successfully\n");
                }
            }
            else
            {
                writeConfig = false;
            }
            GetQuery();
        }
        else
        {
            qWarning("Closing device.");
            comm->close();
            deviceLabel.setText("Disconnected");
            ui->plainTextEdit->setPlainText("Device Detached.");
            hexOpen = false;
            setBootloadEnabled(false);
            emit SetProgressBar(0);
        }
    }
}

void MainWindow::setBootloadEnabled(bool enable)
{
    ui->action_Settings->setEnabled(enable);
    ui->actionErase_Device->setEnabled(enable && !writeConfig);
    ui->actionWrite_Device->setEnabled(enable && hexOpen);
    ui->actionExit->setEnabled(enable);
    ui->action_Verify_Device->setEnabled(enable && hexOpen);
    ui->actionOpen->setEnabled(enable);
    ui->actionBlank_Check->setEnabled(enable && !writeConfig);
    ui->actionReset_Device->setEnabled(enable);
}

void MainWindow::setBootloadBusy(bool busy)
{
    if(busy)
    {
        QApplication::setOverrideCursor(Qt::BusyCursor);
        timer->stop();
    }
    else
    {
        QApplication::restoreOverrideCursor();
        timer->start(1000);
    }

    ui->action_Settings->setEnabled(!busy);
    ui->actionErase_Device->setEnabled(!busy && !writeConfig);
    ui->actionWrite_Device->setEnabled(!busy && hexOpen);
    ui->actionExit->setEnabled(!busy);
    ui->action_Verify_Device->setEnabled(!busy && hexOpen);
    ui->actionOpen->setEnabled(!busy);
    ui->action_Settings->setEnabled(!busy);
    ui->actionBlank_Check->setEnabled(!busy && !writeConfig);
    ui->actionReset_Device->setEnabled(!busy);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::IoWithDeviceStart(QString msg)
{
    ui->plainTextEdit->appendPlainText(msg);
    setBootloadBusy(true);
}


//Useful for adding lines of text to the main window from other threads.
void MainWindow::AppendStringToTextbox(QString msg)
{
    ui->plainTextEdit->appendPlainText(msg);
}

void MainWindow::UpdateProgressBar(int newValue)
{
    ui->progressBar->setValue(newValue);
}



void MainWindow::IoWithDeviceComplete(QString msg, Comm::ErrorCode result, double time)
{
    QTextStream ss(&msg);

    switch(result)
    {
        case Comm::Success:
            ss << " Complete (" << time << "s)\n";
            setBootloadBusy(false);
            break;
        case Comm::NotConnected:
            ss << " Failed. Device not connected.\n";
            setBootloadBusy(false);
            break;
        case Comm::Fail:
            ss << " Failed.\n";
            setBootloadBusy(false);
            break;
        case Comm::IncorrectCommand:
            ss << " Failed. Unable to communicate with device.\n";
            setBootloadBusy(false);
            break;
        case Comm::Timeout:
            ss << " Timed out waiting for device (" << time << "s)\n";
            setBootloadBusy(false);
            break;
        default:
            break;
    }

    ui->plainTextEdit->appendPlainText(msg);
}

void MainWindow::on_action_Verify_Device_triggered()
{
    future = QtConcurrent::run(this, &MainWindow::VerifyDevice);
}


//Routine that verifies the contents of the non-voltaile memory regions in the device, after an erase/programming cycle.
//This function requests the memory contents of the device, then compares it against the parsed .hex file data to make sure
//The locations that got programmed properly match.
void MainWindow::VerifyDevice()
{
    Comm::ErrorCode result;
    DeviceData::MemoryRange deviceRange, hexRange;
    QTime elapsed;
    unsigned int i, j;
    unsigned int arrayIndex;
    bool failureDetected = false;
    unsigned char flashData[MAX_ERASE_BLOCK_SIZE];
    unsigned char hexEraseBlockData[MAX_ERASE_BLOCK_SIZE];
    unsigned long int startOfEraseBlock;

    //Initialize an erase block sized buffer with 0xFF.
    //Used later for post SIGN_FLASH verify operation.
    memset(&hexEraseBlockData[0], 0xFF, MAX_ERASE_BLOCK_SIZE);

    emit IoWithDeviceStarted("Verifying Device...");
    foreach(deviceRange, deviceData->ranges)
    {
        if(writeFlash && (deviceRange.type == PROGRAM_MEMORY))
        {
            elapsed.start();
            //emit IoWithDeviceStarted("Verifying Device's Program Memory...");

            result = comm->GetData(deviceRange.start,
                                   device->bytesPerPacket,
                                   device->bytesPerAddressFLASH,
                                   device->bytesPerWordFLASH,
                                   deviceRange.end,
                                   deviceRange.pDataBuffer);

            if(result != Comm::Success)
            {
                failureDetected = true;
                qWarning("Error reading device.");
                //emit IoWithDeviceCompleted("Verifying Device's Program Memory", result, ((double)elapsed.elapsed()) / 1000);
            }

            //Search through all of the programmable memory regions from the parsed .hex file data.
            //For each of the programmable memory regions found, if the region also overlaps a region
            //that was included in the device programmed area (which just got read back with GetData()),
            //then verify both the parsed hex contents and read back data match.
            foreach(hexRange, hexData->ranges)
            {
                if(deviceRange.start == hexRange.start)
                {
                    //For this entire programmable memory address range, check to see if the data read from the device exactly
                    //matches what was in the hex file.
                    for(i = deviceRange.start; i < deviceRange.end; i++)
                    {
                        //For each byte of each device address (1 on PIC18, 2 on PIC24, since flash memory is 16-bit WORD array)
                        for(j = 0; j < device->bytesPerAddressFLASH; j++)
                        {
                            //Check if the device response data matches the data we parsed from the original input .hex file.
                            if(deviceRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j] != hexRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j])
                            {
                                //A mismatch was detected.

                                //Check if this is a PIC24 device and we are looking at the "phantom byte"
                                //(upper byte [j = 1] of odd address [i%2 == 1] 16-bit flash words).  If the hex data doesn't match
                                //the device (which should be = 0x00 for these locations), this isn't a real verify
                                //failure, since value is a don't care anyway.  This could occur if the hex file imported
                                //doesn't contain all locations, and we "filled" the region with pure 0xFFFFFFFF, instead of 0x00FFFFFF
                                //when parsing the hex file.
                                if((device->family == Device::PIC24) && ((i % 2) == 1) && (j == 1))
                                {
                                    //Not a real verify failure, phantom byte is unimplemented and is a don't care.
                                }
                                else
                                {
                                    //If the data wasn't a match, and this wasn't a PIC24 phantom byte, then if we get
                                    //here this means we found a true verify failure.
                                    failureDetected = true;
                                    if(device->family == Device::PIC24)
                                    {
                                        qWarning("Device: 0x%x Hex: 0x%x", *(unsigned short int*)&deviceRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j], *(unsigned short int*)&hexRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j]);
                                    }
                                    else
                                    {
                                        qWarning("Device: 0x%x Hex: 0x%x", deviceRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j], hexRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j]);
                                    }
                                    qWarning("Failed verify at address 0x%x", i);
                                    emit IoWithDeviceCompleted("Verify", Comm::Fail, ((double)elapsed.elapsed()) / 1000);
                                    return;
                                }
                            }//if(deviceRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j] != hexRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j])
                        }//for(j = 0; j < device->bytesPerAddressFLASH; j++)
                    }//for(i = deviceRange.start; i < deviceRange.end; i++)
                }//if(deviceRange.start == hexRange.start)
            }//foreach(hexRange, hexData->ranges)
            //emit IoWithDeviceCompleted("Verify", Comm::Success, ((double)elapsed.elapsed()) / 1000);
        }//if(writeFlash && (deviceRange.type == PROGRAM_MEMORY))
        else if(writeEeprom && (deviceRange.type == EEPROM_MEMORY))
        {
            elapsed.start();
            //emit IoWithDeviceStarted("Verifying Device's EEPROM Memory...");

            result = comm->GetData(deviceRange.start,
                                   device->bytesPerPacket,
                                   device->bytesPerAddressEEPROM,
                                   device->bytesPerWordEEPROM,
                                   deviceRange.end,
                                   deviceRange.pDataBuffer);

            if(result != Comm::Success)
            {
                failureDetected = true;
                qWarning("Error reading device.");
                //emit IoWithDeviceCompleted("Verifying Device's EEPROM Memory", result, ((double)elapsed.elapsed()) / 1000);
            }


            //Search through all of the programmable memory regions from the parsed .hex file data.
            //For each of the programmable memory regions found, if the region also overlaps a region
            //that was included in the device programmed area (which just got read back with GetData()),
            //then verify both the parsed hex contents and read back data match.
            foreach(hexRange, hexData->ranges)
            {
                if(deviceRange.start == hexRange.start)
                {
                    //For this entire programmable memory address range, check to see if the data read from the device exactly
                    //matches what was in the hex file.
                    for(i = deviceRange.start; i < deviceRange.end; i++)
                    {
                        //For each byte of each device address (only 1 for EEPROM byte arrays, presumably 2 for EEPROM WORD arrays)
                        for(j = 0; j < device->bytesPerAddressEEPROM; j++)
                        {
                            //Check if the device response data matches the data we parsed from the original input .hex file.
                            if(deviceRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressEEPROM)+j] != hexRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressEEPROM)+j])
                            {
                                //A mismatch was detected.
                                failureDetected = true;
                                qWarning("Device: 0x%x Hex: 0x%x", deviceRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j], hexRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j]);
                                qWarning("Failed verify at address 0x%x", i);
                                emit IoWithDeviceCompleted("Verify EEPROM Memory", Comm::Fail, ((double)elapsed.elapsed()) / 1000);
                                return;
                            }
                        }
                    }
                }
            }//foreach(hexRange, hexData->ranges)
            //emit IoWithDeviceCompleted("Verifying", Comm::Success, ((double)elapsed.elapsed()) / 1000);
        }//else if(writeEeprom && (deviceRange.type == EEPROM_MEMORY))
        else if(writeConfig && (deviceRange.type == CONFIG_MEMORY))
        {
            elapsed.start();
            //emit IoWithDeviceStarted("Verifying Device's Config Words...");

            result = comm->GetData(deviceRange.start,
                                   device->bytesPerPacket,
                                   device->bytesPerAddressConfig,
                                   device->bytesPerWordConfig,
                                   deviceRange.end,
                                   deviceRange.pDataBuffer);

            if(result != Comm::Success)
            {
                failureDetected = true;
                qWarning("Error reading device.");
                //emit IoWithDeviceCompleted("Verifying Device's Config Words", result, ((double)elapsed.elapsed()) / 1000);
            }

            //Search through all of the programmable memory regions from the parsed .hex file data.
            //For each of the programmable memory regions found, if the region also overlaps a region
            //that was included in the device programmed area (which just got read back with GetData()),
            //then verify both the parsed hex contents and read back data match.
            foreach(hexRange, hexData->ranges)
            {
                if(deviceRange.start == hexRange.start)
                {
                    //For this entire programmable memory address range, check to see if the data read from the device exactly
                    //matches what was in the hex file.
                    for(i = deviceRange.start; i < deviceRange.end; i++)
                    {
                        //For each byte of each device address (1 on PIC18, 2 on PIC24, since flash memory is 16-bit WORD array)
                        for(j = 0; j < device->bytesPerAddressConfig; j++)
                        {
                            //Compute an index into the device and hex data arrays, based on the current i and j values.
                            arrayIndex = ((i - deviceRange.start) * device->bytesPerAddressConfig)+j;

                            //Check if the device response data matches the data we parsed from the original input .hex file.
                            if(deviceRange.pDataBuffer[arrayIndex] != hexRange.pDataBuffer[arrayIndex])
                            {
                                //A mismatch was detected.  Perform additional checks to make sure it was a real/unexpected verify failure.

                                //Check if this is a PIC24 device and we are looking at the "phantom byte"
                                //(upper byte [j = 1] of odd address [i%2 == 1] 16-bit flash words).  If the hex data doesn't match
                                //the device (which should be = 0x00 for these locations), this isn't a real verify
                                //failure, since value is a don't care anyway.  This could occur if the hex file imported
                                //doesn't contain all locations, and we "filled" the region with pure 0xFFFFFFFF, instead of 0x00FFFFFF
                                //when parsing the hex file.
                                if((device->family == Device::PIC24) && ((i % 2) == 1) && (j == 1))
                                {
                                    //Not a real verify failure, phantom byte is unimplemented and is a don't care.
                                }//Make further special checks for PIC18 non-J devices
                                else if((device->family == Device::PIC18) && (deviceRange.start == 0x300000) && ((i == 0x300004) || (i == 0x300007)))
                                {
                                     //The "CONFIG3L" and "CONFIG4H" locations (0x300004 and 0x300007) on PIC18 non-J USB devices
                                     //are unimplemented and should be masked out from the verify operation.
                                }
                                else
                                {
                                    //If the data wasn't a match, and this wasn't a PIC24 phantom byte, then if we get
                                    //here this means we found a true verify failure.
                                    failureDetected = true;
                                    if(device->family == Device::PIC24)
                                    {
                                        qWarning("Device: 0x%x Hex: 0x%x", *(unsigned short int*)&deviceRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressConfig)+j], *(unsigned short int*)&hexRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressConfig)+j]);
                                    }
                                    else
                                    {
                                        qWarning("Device: 0x%x Hex: 0x%x", deviceRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressConfig)+j], hexRange.pDataBuffer[((i - deviceRange.start) * device->bytesPerAddressConfig)+j]);
                                    }
                                    qWarning("Failed verify at address 0x%x", i);
                                    emit IoWithDeviceCompleted("Verify Config Bit Memory", Comm::Fail, ((double)elapsed.elapsed()) / 1000);
                                    return;
                                }
                            }
                        }
                    }
                }
            }//foreach(hexRange, hexData->ranges)
            //emit IoWithDeviceCompleted("Verifying", Comm::Success, ((double)elapsed.elapsed()) / 1000);
        }//else if(writeConfig && (deviceRange.type == CONFIG_MEMORY))
        else
        {
            continue;
        }
    }//foreach(deviceRange, deviceData->ranges)

    if(failureDetected == false)
    {
        //Successfully verified all regions without error.
        //If this is a v1.01 or later device, we now need to issue the SIGN_FLASH
        //command, and then re-verify the first erase page worth of flash memory
        //(but with the exclusion of the signature WORD address from the verify,
        //since the bootloader firmware will have changed it to the new/magic
        //value (probably 0x600D, or "good" in leet speak).
        if(deviceFirmwareIsAtLeast101 == true)
        {
            comm->SignFlash();
            //Now re-verify the first erase page of flash memory.
            if(device->family == Device::PIC18)
            {
                startOfEraseBlock = extendedBootInfo.PIC18.signatureAddress - (extendedBootInfo.PIC18.signatureAddress % extendedBootInfo.PIC18.erasePageSize);
                result = comm->GetData(startOfEraseBlock,
                                       device->bytesPerPacket,
                                       device->bytesPerAddressFLASH,
                                       device->bytesPerWordFLASH,
                                       (startOfEraseBlock + extendedBootInfo.PIC18.erasePageSize),
                                       &flashData[0]);
                if(result != Comm::Success)
                {
                    failureDetected = true;
                    qWarning("Error reading, post signing, flash data block.");
                }

                //Search through all of the programmable memory regions from the parsed .hex file data.
                //For each of the programmable memory regions found, if the region also overlaps a region
                //that is part of the erase block, copy out bytes into the hexEraseBlockData[] buffer,
                //for re-verification.
                foreach(hexRange, hexData->ranges)
                {
                    //Check if any portion of the range is within the erase block of interest in the device.
                    if((hexRange.start <= startOfEraseBlock) && (hexRange.end > startOfEraseBlock))
                    {
                        unsigned int rangeSize = hexRange.end - hexRange.start;
                        unsigned int address = hexRange.start;
                        unsigned int k = 0;

                        //Check every byte in the hex file range, to see if it is inside the erase block of interest
                        for(i = 0; i < rangeSize; i++)
                        {
                            //Check if the current byte we are looking at is inside the erase block of interst
                            if(((address+i) >= startOfEraseBlock) && ((address+i) < (startOfEraseBlock + extendedBootInfo.PIC18.erasePageSize)))
                            {
                                //The byte is in the erase block of interst.  Copy it out into a new buffer.
                                hexEraseBlockData[k] = *(hexRange.pDataBuffer + i);
                                //Check if this is a signature byte.  If so, replace the value in the buffer
                                //with the post-signing expected signature value, since this is now the expected
                                //value from the device, rather than the value from the hex file...
                                if((address+i) == extendedBootInfo.PIC18.signatureAddress)
                                {
                                    hexEraseBlockData[k] = (unsigned char)extendedBootInfo.PIC18.signatureValue;    //Write LSB of signature into buffer
                                }
                                if((address+i) == (extendedBootInfo.PIC18.signatureAddress + 1))
                                {
                                    hexEraseBlockData[k] = (unsigned char)(extendedBootInfo.PIC18.signatureValue >> 8); //Write MSB into buffer
                                }
                                k++;
                            }
                            if((k >= extendedBootInfo.PIC18.erasePageSize) || (k >= sizeof(hexEraseBlockData)))
                                break;
                        }
                    }
                }//foreach(hexRange, hexData->ranges)

                //We now have both the hex data and the post signing flash erase block data
                //in two RAM buffers.  Compare them to each other to perform post-signing
                //verify.
                for(i = 0; i < extendedBootInfo.PIC18.erasePageSize; i++)
                {
                    if(flashData[i] != hexEraseBlockData[i])
                    {
                        failureDetected = true;
                        qWarning("Post signing verify failure.");
                        EraseDevice();  //Send an erase command, to forcibly
                        //remove the signature (which might be valid), since
                        //there was a verify error and we can't trust the application
                        //firmware image integrity.  This ensures the device jumps
                        //back into bootloader mode always.
                    }
                }
            }

        }//if(deviceFirmwareIsAtLeast101 == true)

    }//if(failureDetected == false)

    if(failureDetected == true)
    {
        emit AppendString("Operation aborted due to error encountered during verify operation.");
        emit AppendString("Please try the erase/program/verify sequence again.");
        emit AppendString("If repeated failures are encountered, this may indicate the flash");
        emit AppendString("memory has worn out, that the device has been damaged, or that");
        emit AppendString("there is some other unidentified problem.");
    }
    else
    {
        emit IoWithDeviceCompleted("Verify", Comm::Success, ((double)elapsed.elapsed()) / 1000);
        emit AppendString("Erase/Program/Verify sequence completed successfully.");
        emit AppendString("You may now unplug or reset the device.");
    }

    emit SetProgressBar(100);   //Set progress bar to 100%
}//void MainWindow::VerifyDevice()


//Gets called when the user clicks to program button in the GUI.
void MainWindow::on_actionWrite_Device_triggered()
{
    future = QtConcurrent::run(this, &MainWindow::WriteDevice);
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText("Starting Erase/Program/Verify Sequence.");
    ui->plainTextEdit->appendPlainText("Do not unplug device or disconnect power until the operation is fully complete.");
    ui->plainTextEdit->appendPlainText(" ");
}


//This thread programs previously parsed .hex file data into the device's programmable memory regions.
void MainWindow::WriteDevice(void)
{
    QTime elapsed;
    Comm::ErrorCode result;
    DeviceData::MemoryRange hexRange;

    //Update the progress bar so the user knows things are happening.
    emit SetProgressBar(3);
    //First erase the entire device.
    EraseDevice();

    //Now being re-programming each section based on the info we obtained when
    //we parsed the user's .hex file.

    emit IoWithDeviceStarted("Writing Device...");
    foreach(hexRange, hexData->ranges)
    {
        if(writeFlash && (hexRange.type == PROGRAM_MEMORY))
        {
            //emit IoWithDeviceStarted("Writing Device Program Memory...");
            elapsed.start();

            result = comm->Program(hexRange.start,
                                   device->bytesPerPacket,
                                   device->bytesPerAddressFLASH,
                                   device->bytesPerWordFLASH,
                                   device->family,
                                   hexRange.end,
                                   hexRange.pDataBuffer);
        }
        else if(writeEeprom && (hexRange.type ==  EEPROM_MEMORY))
        {
                //emit IoWithDeviceStarted("Writing Device EEPROM Memory...");
                elapsed.start();

                result = comm->Program(hexRange.start,
                                       device->bytesPerPacket,
                                       device->bytesPerAddressEEPROM,
                                       device->bytesPerWordEEPROM,
                                       device->family,
                                       hexRange.end,
                                       hexRange.pDataBuffer);
        }
        else if(writeConfig && (hexRange.type == CONFIG_MEMORY))
        {
            //emit IoWithDeviceStarted("Writing Device Config Words...");
            elapsed.start();

            result = comm->Program(hexRange.start,
                                   device->bytesPerPacket,
                                   device->bytesPerAddressConfig,
                                   device->bytesPerWordConfig,
                                   device->family,
                                   hexRange.end,
                                   hexRange.pDataBuffer);
        }
        else
        {
            continue;
        }

        //emit IoWithDeviceCompleted("Writing", result, ((double)elapsed.elapsed()) / 1000);

        if(result != Comm::Success)
        {
            qWarning("Programming failed");
            return;
        }
    }

    emit IoWithDeviceCompleted("Write", result, ((double)elapsed.elapsed()) / 1000);

    VerifyDevice();
}

void MainWindow::on_actionBlank_Check_triggered()
{
    future = QtConcurrent::run(this, &MainWindow::BlankCheckDevice);
}

void MainWindow::BlankCheckDevice(void)
{
    QTime elapsed;
    Comm::ErrorCode result;
    DeviceData::MemoryRange deviceRange;

    elapsed.start();

    foreach(deviceRange, deviceData->ranges)
    {
        if(writeFlash && (deviceRange.type == PROGRAM_MEMORY))
        {
            emit IoWithDeviceStarted("Blank Checking Device's Program Memory...");

            result = comm->GetData(deviceRange.start,
                                   device->bytesPerPacket,
                                   device->bytesPerAddressFLASH,
                                   device->bytesPerWordFLASH,
                                   deviceRange.end,
                                   deviceRange.pDataBuffer);


            if(result != Comm::Success)
            {
                qWarning("Blank Check failed");
                emit IoWithDeviceCompleted("Blank Checking Program Memory", result, ((double)elapsed.elapsed()) / 1000);
                return;
            }

            for(unsigned int i = 0; i < ((deviceRange.end - deviceRange.start) * device->bytesPerAddressFLASH); i++)
            {
                if((deviceRange.pDataBuffer[i] != 0xFF) && !((device->family == Device::PIC24) && ((i % 4) == 3)))
                {
                    qWarning("Failed blank check at address 0x%x", deviceRange.start + i);
                    qWarning("The value was 0x%x", deviceRange.pDataBuffer[i]);
                    emit IoWithDeviceCompleted("Blank Check", Comm::Fail, ((double)elapsed.elapsed()) / 1000);
                    return;
                }
            }
            emit IoWithDeviceCompleted("Blank Checking Program Memory", Comm::Success, ((double)elapsed.elapsed()) / 1000);
        }
        else if(writeEeprom && deviceRange.type == EEPROM_MEMORY)
        {
            emit IoWithDeviceStarted("Blank Checking Device's EEPROM Memory...");

            result = comm->GetData(deviceRange.start,
                                   device->bytesPerPacket,
                                   device->bytesPerAddressEEPROM,
                                   device->bytesPerWordEEPROM,
                                   deviceRange.end,
                                   deviceRange.pDataBuffer);


            if(result != Comm::Success)
            {
                qWarning("Blank Check failed");
                emit IoWithDeviceCompleted("Blank Checking EEPROM Memory", result, ((double)elapsed.elapsed()) / 1000);
                return;
            }

            for(unsigned int i = 0; i < ((deviceRange.end - deviceRange.start) * device->bytesPerWordEEPROM); i++)
            {
                if(deviceRange.pDataBuffer[i] != 0xFF)
                {
                    qWarning("Failed blank check at address 0x%x + 0x%x", deviceRange.start, i);
                    qWarning("The value was 0x%x", deviceRange.pDataBuffer[i]);
                    emit IoWithDeviceCompleted("Blank Check", Comm::Fail, ((double)elapsed.elapsed()) / 1000);
                    return;
                }
            }
            emit IoWithDeviceCompleted("Blank Checking EEPROM Memory", Comm::Success, ((double)elapsed.elapsed()) / 1000);
        }
        else
        {
            continue;
        }
    }
}

void MainWindow::on_actionErase_Device_triggered()
{
    future = QtConcurrent::run(this, &MainWindow::EraseDevice);
}

void MainWindow::EraseDevice(void)
{
    QTime elapsed;
    Comm::ErrorCode result;
    Comm::BootInfo bootInfo;


    //if(writeFlash || writeEeprom)
    {
        emit IoWithDeviceStarted("Erasing Device... (no status update until complete, may take several seconds)");
        elapsed.start();

        result = comm->Erase();
        if(result != Comm::Success)
        {
            emit IoWithDeviceCompleted("Erase", result, ((double)elapsed.elapsed()) / 1000);
            return;
        }

        result = comm->ReadBootloaderInfo(&bootInfo);

        emit IoWithDeviceCompleted("Erase", result, ((double)elapsed.elapsed()) / 1000);
    }
}

//Executes when the user clicks the open hex file button on the main form.
void MainWindow::on_actionOpen_triggered()
{
    QString msg, newFileName;
    QTextStream stream(&msg);

    //Create an open file dialog box, so the user can select a .hex file.
    newFileName =
        QFileDialog::getOpenFileName(this, "Open Hex File", fileName, "Hex Files (*.hex *.ehx)");

    if(newFileName.isEmpty())
    {
        return;
    }

    LoadFile(newFileName);
}

void MainWindow::LoadFile(QString newFileName)
{
    QString msg;
    QTextStream stream(&msg);
    QFileInfo nfi(newFileName);

    QApplication::setOverrideCursor(Qt::BusyCursor);

    HexImporter import;
    HexImporter::ErrorCode result;
    Comm::ErrorCode commResultCode;

    hexData->ranges.clear();

    //Print some debug info to the debug window.
    qDebug(QString("Total Programmable Regions Reported by Device: " + QString::number(deviceData->ranges.count(), 10)).toLatin1());

    //First duplicate the deviceData programmable region list and
    //allocate some RAM buffers to hold the hex data that we are about to import.
    foreach(DeviceData::MemoryRange range, deviceData->ranges)
    {
        //Allocate some RAM for the hex file data we are about to import.
        //Initialize all bytes of the buffer to 0xFF, the default unprogrammed memory value,
        //which is also the "assumed" value, if a value is missing inside the .hex file, but
        //is still included in a programmable memory region.
        range.pDataBuffer = new unsigned char[range.dataBufferLength];
        memset(range.pDataBuffer, 0xFF, range.dataBufferLength);
        hexData->ranges.append(range);

        //Print info regarding the programmable memory region to the debug window.
        qDebug(QString("Device Programmable Region: [" + QString::number(range.start, 16).toUpper() + " - " +
                   QString::number(range.end, 16).toUpper() +")").toLatin1());
    }

    //Import the hex file data into the hexData->ranges[].pDataBuffer buffers.
    result = import.ImportHexFile(newFileName, hexData, device);
    //Based on the result of the hex file import operation, decide how to proceed.
    switch(result)
    {
        case HexImporter::Success:
            break;

        case HexImporter::CouldNotOpenFile:
            QApplication::restoreOverrideCursor();
            stream << "Error: Could not open file " << nfi.fileName() << "\n";
            ui->plainTextEdit->appendPlainText(msg);
            return;

        case HexImporter::NoneInRange:
            QApplication::restoreOverrideCursor();
            stream << "No address within range in file: " << nfi.fileName() << ".  Verify the correct firmware image was specified and is designed for your device.\n";
            ui->plainTextEdit->appendPlainText(msg);
            return;

        case HexImporter::ErrorInHexFile:
            QApplication::restoreOverrideCursor();
            stream << "Error in hex file.  Please make sure the firmware image supplied was designed for the device to be programmed. \n";
            ui->plainTextEdit->appendPlainText(msg);
            return;
        case HexImporter::InsufficientMemory:
            QApplication::restoreOverrideCursor();
            stream << "Memory allocation failed.  Please close other applications to free up system RAM and try again. \n";
            ui->plainTextEdit->appendPlainText(msg);
            return;

        default:
            QApplication::restoreOverrideCursor();
            stream << "Failed to import: " << result << "\n";
            ui->plainTextEdit->appendPlainText(msg);
            return;
    }

    //Check if the user has imported a .hex file that doesn't contain config bits in it,
    //even though the user is planning on re-programming the config bits section.
    if(writeConfig && (import.hasConfigBits == false) && device->hasConfig())
    {
        //The user had config bit reprogramming selected, but the hex file opened didn't have config bit
        //data in it.  We should automatically prevent config bit programming, to avoid leaving the device
        //in a broken state following the programming cycle.
        commResultCode = comm->LockUnlockConfig(true); //Lock the config bits.
        if(commResultCode != Comm::Success)
        {
            ui->plainTextEdit->appendPlainText("Unexpected internal error encountered.  Recommend restarting the application to avoid ""bricking"" the device.\n");
        }

        QMessageBox::warning(this, "Warning!", "This HEX file does not contain config bit information.\n\nAutomatically disabling config bit reprogramming to avoid leaving the device in a state that could prevent further bootloading.", QMessageBox::AcceptRole, QMessageBox::AcceptRole);
        writeConfig = false;
    }

    fileName = newFileName;
    watchFileName = newFileName;

    QSettings settings;
    settings.beginGroup("MainWindow");

    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while(files.size() > MAX_RECENT_FILES)
    {
        files.removeLast();
    }
    settings.setValue("recentFileList", files);
    UpdateRecentFileList();

    stream.setIntegerBase(10);

    msg.clear();
    QFileInfo fi(fileName);
    QString name = fi.fileName();
    stream << "Opened: " << name << "\n";
    ui->plainTextEdit->appendPlainText(msg);
    hexOpen = true;
    setBootloadEnabled(true);
    QApplication::restoreOverrideCursor();

    return;
}

void MainWindow::openRecentFile(void)
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        LoadFile(action->data().toString());
    }
}

void MainWindow::UpdateRecentFileList(void)
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    QStringList files;

    files = settings.value("recentFileList").toStringList();

    int recentFileCount = qMin(files.size(), MAX_RECENT_FILES);
    QString text;
    int i;

    for(i = 0; i < recentFileCount; i++)
    {
        text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());

        recentFiles[i]->setText(text);
        recentFiles[i]->setData(files[i]);
        recentFiles[i]->setVisible(comm->isConnected());
    }

    for(; i < MAX_RECENT_FILES; i++)
    {
        recentFiles[i]->setVisible(false);
    }
}

void MainWindow::on_action_About_triggered()
{
    QString msg;
    QTextStream stream(&msg);

    stream << "USB HID Bootloader v" << VERSION << "\n";
    stream << "Copyright " << (char)Qt::Key_copyright << " 2011-2013,  Microchip Technology Inc.\n\n";

    stream << "Microchip licenses this software to you solely for use with\n";
    stream << "Microchip products. The software is owned by Microchip and\n";
    stream << "its licensors, and is protected under applicable copyright\n";
    stream << "laws. All rights reserved.\n\n";

    stream << "SOFTWARE IS PROVIDED \"AS IS.\"  MICROCHIP EXPRESSLY\n";
    stream << "DISCLAIMS ANY WARRANTY OF ANY KIND, WHETHER EXPRESS\n";
    stream << "OR IMPLIED, INCLUDING BUT NOT LIMITED TO, THE IMPLIED\n";
    stream << "WARRANTIES OF MERCHANTABILITY, FITNESS FOR A\n";
    stream << "PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  IN NO EVENT\n";
    stream << "SHALL MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL,\n";
    stream << "INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR\n";
    stream << "LOST DATA, HARM TO YOUR EQUIPMENT, COST OF\n";
    stream << "PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR\n";
    stream << "SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT\n";
    stream << "NOT LIMITED TO ANY DEFENSE THEREOF), ANY CLAIMS FOR\n";
    stream << "INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.\n\n";

    stream << "To the fullest extent allowed by law, Microchip and its\n";
    stream << "licensors liability shall not exceed the amount of fees, if any,\n";
    stream << "that you have paid directly to Microchip to use this software.\n\n";

    stream << "MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON\n";
    stream << "YOUR ACCEPTANCE OF THESE TERMS.";

    QMessageBox::about(this, "About", msg);
}

void MainWindow::GetQuery()
{
    QTime totalTime;
    Comm::BootInfo bootInfo;
    DeviceData::MemoryRange range;
    QString connectMsg;
    QTextStream ss(&connectMsg);


    qDebug("Executing GetQuery() command.");

    totalTime.start();

    if(!comm->isConnected())
    {
        qWarning("Query not sent, device not connected");
        return;
    }

    //Send the Query command to the device over USB, and check the result status.
    switch(comm->ReadBootloaderInfo(&bootInfo))
    {
        case Comm::Fail:
        case Comm::IncorrectCommand:
            ui->plainTextEdit->appendPlainText("Unable to communicate with device\n");
            return;
        case Comm::Timeout:
            ss << "Operation timed out";
            break;
        case Comm::Success:
            wasBootloaderMode = true;
            ss << "Device Ready";
            deviceLabel.setText("Connected");
            break;
        default:
            return;
    }

    ss << " (" << (double)totalTime.elapsed() / 1000 << "s)\n";
    ui->plainTextEdit->appendPlainText(connectMsg);
    deviceData->ranges.clear();

    //Now start parsing the bootInfo packet to learn more about the device.  The bootInfo packet contains
    //contains the query response data from the USB device.  We will save these values into globabl variables
    //so other parts of the application can use the info when deciding how to do things.
    device->family = (Device::Families) bootInfo.deviceFamily;
    device->bytesPerPacket = bootInfo.bytesPerPacket;

    //Set some processor family specific global variables that will be used elsewhere (ex: during program/verify operations).
    switch(device->family)
    {
        case Device::PIC18:
            device->bytesPerWordFLASH = 2;
            device->bytesPerAddressFLASH = 1;
            break;
        case Device::PIC24:
            device->bytesPerWordFLASH = 4;
            device->bytesPerAddressFLASH = 2;
            device->bytesPerWordConfig = 4;
            device->bytesPerAddressConfig = 2;
            break;
        case Device::PIC32:
            device->bytesPerWordFLASH = 4;
            device->bytesPerAddressFLASH = 1;
            break;
        case Device::PIC16:
            device->bytesPerWordFLASH = 2;
            device->bytesPerAddressFLASH = 2;
        default:
            device->bytesPerWordFLASH = 2;
            device->bytesPerAddressFLASH = 1;
            break;
    }

    //Initialize the deviceData buffers and length variables, with the regions that the firmware claims are
    //reprogrammable.  We will need this information later, to decide what part(s) of the .hex file we
    //should look at/try to program into the device.  Data sections in the .hex file that are not included
    //in these regions should be ignored.
    for(int i = 0; i < MAX_DATA_REGIONS; i++)
    {
        if(bootInfo.memoryRegions[i].type == END_OF_TYPES_LIST)
        {
            //Before we quit, check the special versionFlag byte,
            //to see if the bootloader firmware is at least version 1.01.
            //If it is, then it will support the extended query command.
            //If the device is based on v1.00 bootloader firmware, it will have
            //loaded the versionFlag location with 0x00, which was a pad byte.
            if(bootInfo.versionFlag == BOOTLOADER_V1_01_OR_NEWER_FLAG)
            {
                deviceFirmwareIsAtLeast101 = true;
                qDebug("Device bootloader firmware is v1.01 or newer and supports Extended Query.");
                //Now fetch the extended query information packet from the USB firmware.
                comm->ReadExtendedQueryInfo(&extendedBootInfo);
                qDebug("Device bootloader firmware version is: " + extendedBootInfo.PIC18.bootloaderVersion);
            }
            else
            {
                deviceFirmwareIsAtLeast101 = false;
            }
            break;
        }

        //Error check: Check the firmware's reported size to make sure it is sensible.  This ensures
        //we don't try to allocate ourselves a massive amount of RAM (capable of crashing this PC app)
        //if the firmware claimed an improper value.
        if(bootInfo.memoryRegions[i].size > MAXIMUM_PROGRAMMABLE_MEMORY_SEGMENT_SIZE)
        {
            bootInfo.memoryRegions[i].size = MAXIMUM_PROGRAMMABLE_MEMORY_SEGMENT_SIZE;
        }

        //Parse the bootInfo response packet and allocate ourselves some RAM to hold the eventual data to program.
        if(bootInfo.memoryRegions[i].type == PROGRAM_MEMORY)
        {
            range.type = PROGRAM_MEMORY;
            range.dataBufferLength = bootInfo.memoryRegions[i].size * device->bytesPerAddressFLASH;
            range.pDataBuffer = new unsigned char[range.dataBufferLength];
            memset(&range.pDataBuffer[0], 0xFF, range.dataBufferLength);
        }
        else if(bootInfo.memoryRegions[i].type == EEPROM_MEMORY)
        {
            range.type = EEPROM_MEMORY;
            range.dataBufferLength = bootInfo.memoryRegions[i].size * device->bytesPerAddressEEPROM;
            range.pDataBuffer = new unsigned char[range.dataBufferLength];
            memset(&range.pDataBuffer[0], 0xFF, range.dataBufferLength);
        }
        else if(bootInfo.memoryRegions[i].type == CONFIG_MEMORY)
        {
            range.type = CONFIG_MEMORY;
            range.dataBufferLength = bootInfo.memoryRegions[i].size * device->bytesPerAddressConfig;
            range.pDataBuffer = new unsigned char[range.dataBufferLength];
            memset(&range.pDataBuffer[0], 0xFF, range.dataBufferLength);
        }

        //Notes regarding range.start and range.end: The range.start is defined as the starting address inside
        //the USB device that will get programmed.  For example, if the bootloader occupies 0x000-0xFFF flash
        //memory addresses (ex: on a PIC18), then the starting bootloader programmable address would typically
        //be = 0x1000 (ex: range.start = 0x1000).
        //The range.end is defined as the last address that actually gets programmed, plus one, in this programmable
        //region.  For example, for a 64kB PIC18 microcontroller, the last implemented flash memory address
        //is 0xFFFF.  If the last 1024 bytes are reserved by the bootloader (since that last page contains the config
        //bits for instance), then the bootloader firmware may only allow the last address to be programmed to
        //be = 0xFBFF.  In this scenario, the range.end value would be = 0xFBFF + 1 = 0xFC00.
        //When this application uses the range.end value, it should be aware that the actual address limit of
        //range.end does not actually get programmed into the device, but the address just below it does.
        //In this example, the programmed region would end up being 0x1000-0xFBFF (even though range.end = 0xFC00).
        //The proper code to program this would basically be something like this:
        //for(i = range.start; i < range.end; i++)
        //{
        //    //Insert code here that progams one device address.  Note: for PIC18 this will be one byte for flash memory.
        //    //For PIC24 this is actually 2 bytes, since the flash memory is addressed as a 16-bit word array.
        //}
        //In the above example, the for() loop exits just before the actual range.end value itself is programmed.

        range.start = bootInfo.memoryRegions[i].address;
        range.end = bootInfo.memoryRegions[i].address + bootInfo.memoryRegions[i].size;
        //Add the new structure+buffer to the list
        deviceData->ranges.append(range);
    }


    //Make sure user has allowed at least one region to be programmed
    if(!(writeFlash || writeEeprom || writeConfig))
    {
        setBootloadEnabled(false);
        ui->action_Settings->setEnabled(true);
    }
    else
        setBootloadEnabled(true);
}




void MainWindow::on_action_Settings_triggered()
{
    Comm::ErrorCode result;
    Settings* dlg = new Settings(this);

    dlg->enableEepromBox(device->hasEeprom());

    dlg->setWriteFlash(writeFlash);
    dlg->setWriteConfig(writeConfig);
    dlg->setWriteEeprom(writeEeprom);

    if(dlg->exec() == QDialog::Accepted)
    {
        writeFlash = dlg->writeFlash;
        writeEeprom = dlg->writeEeprom;

        if(!writeConfig && dlg->writeConfig)
        {
            ui->plainTextEdit->appendPlainText("Disabling Erase button to prevent accidental erasing of the configuration words without reprogramming them\n");
            writeConfig = true;
            hexOpen = false;
            result = comm->LockUnlockConfig(false);
            if(result == Comm::Success)
            {
                ui->plainTextEdit->appendPlainText("Unlocked Configuration bits successfully\n");
                GetQuery();
            }
        }
        else if(writeConfig && !dlg->writeConfig)
        {
            writeConfig = false;
            hexOpen = false;
            result = comm->LockUnlockConfig(true);
            if(result == Comm::Success)
            {
                ui->plainTextEdit->appendPlainText("Locked Configuration bits successfully\n");
                GetQuery();
            }
        }

        if(!(writeFlash || writeEeprom || writeConfig))
        {
            setBootloadEnabled(false);
            ui->action_Settings->setEnabled(true);
        }
        else
        {
            setBootloadEnabled(true);
        }
    }

    delete dlg;
}

void MainWindow::on_actionReset_Device_triggered()
{
    if(!comm->isConnected())
    {
        failed = -1;
        qWarning("Reset not sent, device not connected");
        return;
    }

    ui->plainTextEdit->appendPlainText("Resetting...");
    comm->Reset();
}
