/********************************************************************
 FileName:      mainwindow.cpp
 Dependencies:	Needs Qt core framework DLL files  
                Needs .NET framework 3.5 redistributable
                Needs libusb-1.0 library (or API equivalent newer version)
 Hardware:      Need a free USB port to connect USB peripheral device
                programming with appropriate WinUSB firmware.  VID and
                PID in firmware must match the VID and PID in this
                program.
 Compiler:      Qt SDK 1.2.1 (or better)
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC® Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively with Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
********************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QtConcurrentRun>
#include "usbcomm.h"


//Wide scope variables
QTimer* pushbuttonPollingTimer;
volatile bool threadAbortFlag = false;  //flag used to tell PollUSBConnection() concurrent thread to shutdown gracefully
QFuture<void> connectionPoller;


//Constructor function, executed when launching the form.
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Initialize the items painted on the GUI form.
    ui->setupUi(this);
    ui->connectionStatus_txtbx->setText("Searching for device...");

    //Send out a debugger message
    qDebug() << "Hello world, from the main window constructor.";

    //Create and initialize an instance of the Usbcomm class.
    usbcomm = new Usbcomm();

    //Execute PollUSBConnection() in a separate thread, so that
    //it can periodically check for USB attach/detach events in the
    //background, without blocking the user interface of this GUI app.
    connectionPoller = QtConcurrent::run(usbcomm, &Usbcomm::PollUSBConnection, &threadAbortFlag);

    //Connect some sigals to some slots (ex: connect things up so events
    //can trigger the appropriate callback functions to execute).
    connect(this, SIGNAL(Toggle_LEDs_Button_Pressed()), usbcomm, SLOT(SendToggleLEDs()));
    connect(usbcomm, SIGNAL(comm_update_connection(bool)), this, SLOT(Update_GUI_Connection(bool)));
    connect(usbcomm, SIGNAL(comm_update_pushbutton(bool)), this, SLOT(Update_GUI_PushbuttonLabel(bool)));

    //Create and intialize a timer for USB pushbutton status polling.
    //When the timeout signal is generated, make the
    //PollPushbuttonState() function/slot execute.
    pushbuttonPollingTimer = new QTimer();
    connect(pushbuttonPollingTimer, SIGNAL(timeout()), usbcomm, SLOT(PollPushbuttonState()));
    pushbuttonPollingTimer->start(10);  //Send request for pushbutton status every 10ms
}


//Destructor function, executed when closing the form.
MainWindow::~MainWindow()
{
    //Tell the PollUSBConnection() to halt gracefully
    threadAbortFlag = true;
    connectionPoller.waitForFinished(); //Wait until the thread has shut itself down
    delete ui;
}


//Public Slot (callback function) for updating the USB device connection status
//on the GUI form.  Also enables/disables Toggle LED(s) pushbutton based
//connection status.
void MainWindow::Update_GUI_Connection(bool isConnected)
{
    if(isConnected)
    {
        ui->pushButton->setEnabled(true);
        ui->pushButtonState_lbl->setEnabled(true);
        ui->connectionStatus_txtbx->setText("Device Found: AttachedState = TRUE");
    }
    else
    {
        ui->pushButton->setEnabled(false);
        ui->pushButtonState_lbl->setEnabled(false);
        ui->connectionStatus_txtbx->setText("Device Not Detected: Verify Connection/Correct Firmware");
        ui->pushButtonState_lbl->setText("Pushbutton State: Unknown");
    }
}

//Public Slot (callback function) for updating the user pushbutton status
//label on the GUI form, with the state of the demo board pushbutton
void MainWindow::Update_GUI_PushbuttonLabel(bool isPressed)
{
    if(isPressed)
    {
        ui->pushButtonState_lbl->setText("Pushbutton State: Pressed");
    }
    else
    {
        ui->pushButtonState_lbl->setText("Pushbutton State: Not Pressed");
    }
}


//This function gets called when the user clicks on the Toggle LED(s) pushbutton on the form.
void MainWindow::on_pushButton_clicked()
{
    //Send out a signal, so that the connected slot/callback function gets to execute.
    emit Toggle_LEDs_Button_Pressed();
}
