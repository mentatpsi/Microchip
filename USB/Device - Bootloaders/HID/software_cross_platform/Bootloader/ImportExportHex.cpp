/************************************************************************
* Copyright (c) 2005-2010,  Microchip Technology Inc.
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
* E. Schlunder  2009/04/29  0.01  Code ported from PicKit2 pk2cmd source code.
* F. Schlunder  2011/06/13  2.90  Some changes for USB HID Bootloader use.
* F. Schlunder  2011/07/06  2.90a Updated ImportHexFile() function so it can
*                                 support importing of hex files with
*                                 "non-monotonic" line address ordering.
*************************************************************************/

#include <QFile>
#include "ImportExportHex.h"
#include "Device.h"


HexImporter::HexImporter(void)
{
}

HexImporter::~HexImporter(void)
{
}


/*
    PIC16Fxx parts use only one address for each FLASH program word. Address 0 has 14 bits of data, Address 1 has
    14 bits of data, etc. However, the PIC16Fxx HEX file addresses each byte of data with a unique address number.
    As a result, you basically have to take the HEX file address and divide by 2 to figure out the actual
    PIC16Fxx FLASH memory address that the byte belongs to.

    Example: PIC16F886 has 8K program words, word addressed as 0 to 0x2000.
        A full HEX file for this part would have 16Kbytes of FLASH data. The HEX file bytes would
        be addressed from 0 to 0x4000.

    This presents a predicament for EEPROM data. Instead of starting from HEX file address 0x2100 as
    the EDC device database might indicate, the HEX file has to start EEPROM data at 0x2000 + 0x2100 = 0x4100,
    to avoid overlapping with the HEX file's FLASH addresses.
*/

//This function reads in Intel 32-bit .hex file formatted firmware image files and stores the
//parsed data into buffers in the PC system RAM, so that it is in a format more suitable for directly
//programming into the target microcontroller.
HexImporter::ErrorCode HexImporter::ImportHexFile(QString fileName, DeviceData* pData, Device* device)
{
    QFile hexfile(fileName);

    hasEndOfFileRecord = false;
    fileExceedsFlash = false;

    //Open the user specified .hex file.
    if (!hexfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return CouldNotOpenFile;
    }

    bool ok;
    bool includedInProgrammableRange;
    bool addressWasEndofRange;
    unsigned int segmentAddress = 0;
    unsigned int byteCount;
    unsigned int lineAddress;
    unsigned int deviceAddress;
    unsigned int i;
    unsigned int endDeviceAddressofRegion;
    unsigned int bytesPerAddressAndType;

    unsigned char* pPCRAMBuffer = 0;
    bool importedAtLeastOneByte = false;

    unsigned char type;

    HEX32_RECORD recordType;

    QString hexByte;
    unsigned int wordByte;
    hasConfigBits = false;


    //Parse the entire hex file, line by line.
    while (!hexfile.atEnd())
    {
        //Fetch a line of ASCII text from the .hex file.
        QByteArray line = hexfile.readLine();

        //Do some error checking on the .hex file contents, to make sure the file is
        //formatted like a legitimate Intel 32-bit formatted .hex file.
        if ((line[0] != ':') || (line.size() < 11))
        {
            //Something is wrong if hex line entry, is not minimum length or does not have leading colon (ex: ":BBAAAATTCC")
            //If an error is detected in the hex file formatting, the safest approach is to
            //abort the operation and force the user to supply a properly formatted hex file.
            if(hexfile.isOpen())
                hexfile.close();
            return ErrorInHexFile;
        }

        //Extract the info prefix fields from the hex file line data.
        //Example Intel 32-bit hex file line format is as follows (Note: spaces added to separate fields, actual line does not have spaces in it):
        //: 10 0100 00 214601360121470136007EFE09D21901 40
        //Leading ":" is always present on each line from the .hex file.
        //Next two chars (10) are the byte count of the data payload on this hex file line. (ex: 10 = 0x10 = 16 bytes)
        //Next four chars (0100) are the 16 bit address (needs to be combined with the extended linear address to generate a 32-bit address).
        //Next two chars (00) are the "record type".  "00" means it is a "data" record, which means it contains programmable payload data bytes.
        //Next 2n characters are the data payload bytes (where n is the number of bytes specified in the first two numbers (10 in this example))
        //Last two characters on the line are the two complement of the byte checksum computed on the other bytes in the line.
        //For more details on Intel 32-bit hex file formatting see: http://en.wikipedia.org/wiki/Intel_HEX
        byteCount = line.mid(1, 2).toInt(&ok, 16);                      //Convert the two ASCII chars corresponding to the byte count into a binary encoded byte
        lineAddress = segmentAddress + line.mid(3, 4).toInt(&ok, 16);   //Convert the four ASCII chars that correspond to the line address into a 16-bit binary encoded word
        recordType = (HEX32_RECORD)line.mid(7, 2).toInt(&ok, 16);       //Convert the two ASCII chars corresponding to the record type into a binary encoded byte

        //Error check: Verify checksum byte at the end of the .hex file line is valid.  Note,
        //this is not the same checksum as MPLAB(R) IDE uses/computes for the entire hex file.
        //This is only the mini-checksum at the end of each line in the .hex file.
        unsigned int hexLineChecksum = 0;
        for(i = 0; i < (byteCount+4); i++)  //+4 correction is for byte count, 16-bit address, and record type bytes
        {
            hexByte = line.mid(1 + (2 * i), 2); //Fetch two adjacent ASCII bytes from the .hex file
            wordByte = hexByte.toInt(&ok, 16);  //Re-format the above two ASCII bytes into a single binary encoded byte (0x00-0xFF)
            //Add the newly fetched byte to the running checksum
            hexLineChecksum += (unsigned char)wordByte;
        }
        //Now get the two's complement of the hexLineChecksum.
        hexLineChecksum = 0 - hexLineChecksum;
        hexLineChecksum &= 0xFF;    //Truncate to a single byte.  We now have our computed checksum.  This should match the .hex file.
        //Fetch checksum byte from the .hex file
        hexByte = line.mid(1 + (2 * i), 2);     //Fetch the two ASCII bytes that correspond to the checksum byte
        wordByte = hexByte.toInt(&ok, 16);      //Re-format the above two ASCII bytes into a single binary encoded byte (0x00-0xFF)
        wordByte &= 0xFF;
        //Now check if the checksum we computed matches the one at the end of the line in the hex file.
        if(hexLineChecksum != wordByte)
        {
            //Checksum in the hex file doesn't match the line contents.  This implies a corrupted hex file.
            //If an error is detected in the hex file formatting, the safest approach is to
            //abort the operation and force the user to supply a properly formatted hex file.
            if(hexfile.isOpen())
                hexfile.close();
            return ErrorInHexFile;
        }



        //Check the record type of the hex line, to determine how to continue parsing the data.
        if (recordType == END_OF_FILE)                        // end of file record
        {
            hasEndOfFileRecord = true;
            break;
        }
        else if ((recordType == EXTENDED_SEGMENT_ADDR) || (recordType == EXTENDED_LINEAR_ADDR)) // Segment address
        {
            //Error check: Make sure the line contains the correct number of bytes for the specified record type
            if((unsigned int)line.size() >= (11 + (2 * byteCount)))
            {
                //Fetch the payload, which is the upper 4 or 16-bits of the 20-bit or 32-bit hex file address
                segmentAddress = line.mid(9, 4).toInt(&ok, 16);

                //Load the upper bits of the address
                if (recordType == EXTENDED_SEGMENT_ADDR)
                {
                    segmentAddress <<= 4;
                }
                else
                {
                    segmentAddress <<= 16;
                }

                //Update the line address, now that we know the upper bits are something new.
                lineAddress = segmentAddress + line.mid(3, 4).toInt(&ok, 16);
            }
            else
            {
                //Length appears to be wrong in hex line entry.
                //If an error is detected in the hex file formatting, the safest approach is to
                //abort the operation and force the user to supply a properly formatted hex file.
                if(hexfile.isOpen())
                    hexfile.close();
                return ErrorInHexFile;
            }

        } // end if ((recordType == EXTENDED_SEGMENT_ADDR) || (recordType == EXTENDED_LINEAR_ADDR)) // Segment address
        else if (recordType == DATA)                        // Data Record
        {
            //Error check to make sure line is long enough to be consistent with the specified record type
            if ((unsigned int)line.size() < (11 + (2 * byteCount)))
            {
                //If an error is detected in the hex file formatting, the safest approach is to
                //abort the operation and force the user to supply a proper hex file.
                if(hexfile.isOpen())
                    hexfile.close();
                return ErrorInHexFile;
            }


            //For each data payload byte we find in the hex file line, check if it is contained within
            //a progammable region inside the microcontroller.  If so save it.  If not, discard it.
            for(i = 0; i < byteCount; i++)
            {
                //Use the hex file linear byte address, to compute other imformation about the
                //byte/location.  The GetDeviceAddressFromHexAddress() function gives us a pointer to
                //the PC RAM buffer byte that will get programmed into the microcontroller, which corresponds
                //to the specified .hex file extended address.
                //The function also returns a boolean letting us know if the address is part of a programmable memory region on the device.
                deviceAddress = device->GetDeviceAddressFromHexAddress(lineAddress + i, pData, type, includedInProgrammableRange, addressWasEndofRange, bytesPerAddressAndType, endDeviceAddressofRegion, pPCRAMBuffer);
                //Check if the just parsed hex byte was included in one of the microcontroller reported programmable memory regions.
                //If so, save the byte into the proper location in the PC RAM buffer, so it can be programmed later.
                if((includedInProgrammableRange == true) && (pPCRAMBuffer != 0)) //Make sure pPCRAMBuffer pointer is valid before using it.
                {
                    //Print debug output text to debug window
                    if(i == 0)
                    {
                        qDebug(QString("Importing .hex file line with device address: 0x" + QString::number(deviceAddress, 16)).toLatin1());
                    }

                    //Fetch ASCII encoded payload byte from .hex file and save the byte to our temporary RAM buffer.
                    hexByte = line.mid(9 + (2 * i), 2);  //Fetch two ASCII data payload bytes from the .hex file
                    wordByte = hexByte.toInt(&ok, 16);   //Re-format the above two ASCII bytes into a single binary encoded byte (0x00-0xFF)

                    *pPCRAMBuffer = (unsigned char)wordByte;    //Save the .hex file data byte into the PC RAM buffer that holds the data to be programmed
                    importedAtLeastOneByte = true;       //Set flag so we know we imported something successfully.

                    //Check if we just parsed a config bit byte.  If so, set flag so the user is no longer locked out
                    //of programming the config bits section.
                    if(type == CONFIG_MEMORY)
                    {
                        hasConfigBits = true;
                    }
                }
                else if((includedInProgrammableRange == true) && (pPCRAMBuffer == 0))
                {
                    //Previous memory allocation must have failed, or otherwise pPCRAMBuffer would not be = 0.
                    //Since the memory allocation failed, we should bug out and let the user know.
                    if(hexfile.isOpen())
                        hexfile.close();
                    return InsufficientMemory;
                }
            }//for(i = 0; i < byteCount; i++)
        } // end else if (recordType == DATA)
    }//while (!hexfile.atEnd())

    //If we get to here, that means we reached the end of the hex file, or we found a END_OF_FILE record in the .hex file.
    if(hexfile.isOpen())
    {
        hexfile.close();
    }

    //Check if we imported any data from the .hex file.
    if(importedAtLeastOneByte == true)
    {
        qDebug(QString("Hex File imported successfully.").toLatin1());
        return Success;
    }
    else
    {
        //If we get to here, we didn't import anything.  The hex file must have been empty or otherwise didn't
        //contain any data that overlaps a device programmable region.  We should let the user know they should
        //supply a better hex file designed for their device.
        return NoneInRange;
    }
}





