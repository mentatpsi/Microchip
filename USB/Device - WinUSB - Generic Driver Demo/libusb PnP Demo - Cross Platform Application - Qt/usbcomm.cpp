/********************************************************************
 FileName:      usbcomm.cpp
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


#include <QtDebug>
#include <QFuture>
#include <QtConcurrentRun>
#include <QThread>
#include "usbcomm.h"


//-------------------------------------------------------------------------------
//VID/PID Definitions: These must match the VID/PID in the USB device descriptor,
//in the device firmware, as well as in the driver package .inf file, 
//in order for this application to find and connect to the USB device.
#define USB_VID_TO_FIND 0x04D8
#define USB_PID_TO_FIND 0x0053
//-------------------------------------------------------------------------------


//Wide scope variables
libusb_device_handle* libUSBDeviceHandle;
bool libusbInitialized = false;
QMutex* mutex;


//So we can gain access to the static/private msleep() function.
class Sleeper: public QThread {
public:
    static void msleep(int ms)
    {
        QThread::msleep(ms);
    }
};


//Constructor function.  Executes when we initialize a new instance of this class.
Usbcomm::Usbcomm(QObject *parent) : QObject(parent)
{
    //Initialize state tracking and other variables.
    libusbInitialized = false;
    isConnected = false;
    libUSBDeviceHandle = NULL;
    isPushbuttonPressed = false;
    mutex = new QMutex;
}


//Destructor function
Usbcomm::~Usbcomm() {
    //Don't need to do much here.  The important
    //shutdown code is located in the PollUSBConnection()
    //thread.  Make sure to set the *pThreadAbortFlag = true
    //so as to gracefully shutdown the thread and libusb
    //before exiting the application.
}




//This function is intended to be executed as a separate thread.  It periodically
//checks the system for the presence of the USB device with matching VID and PID.
//If a device is found, it automatically connects up to it and notifies the application.
//Similarly, this function can also detect detach events and will also alert the application.
void Usbcomm::PollUSBConnection(volatile bool* pThreadAbortFlag)
{
    int status;
    libusb_device** list;
    ssize_t numberOfDevices;
    libusb_device_descriptor deviceDescriptor;
    ssize_t i;
    bool deviceFound;
    Sleeper SleepThread;
    libusb_device_handle* localLibUSBDeviceHandle = NULL;
    bool oldIsConnected = true;


    //Infinite polling loop, until the calling thread tries to signal abort.
    while(1)
    {
        //Check if the calling thread it trying to tell us to abort/quit this thread
        if(*pThreadAbortFlag == true)
        {
            //Clean things up and exit
            if(localLibUSBDeviceHandle != NULL)
            {
                //libusb_release_interface(); //Normally supposed to call this prior to calling lubusb_close(), but since
                //the USB device is presumably detached right now, this is rather extraneous and should error out with LIBUSB_ERROR_NO_DEVICE
                libusb_close(localLibUSBDeviceHandle);   //
                localLibUSBDeviceHandle = NULL;
            }
            mutex->lock();
            libUSBDeviceHandle = localLibUSBDeviceHandle;
            mutex->unlock();
            isConnected = false;
            emit(comm_update_connection(isConnected));
            return;
        }

        //Check if the libusb library has been initialized yet.
        if(libusbInitialized == false)
        {
            //The library wasn't already initialized.  Try to initialize it now.
            status = libusb_init(NULL); //Must be called prior to calling any other libusb functions
            if(status == 0)
            {
                //The init function worked.  Keep track of this so we know it is safe to call other libusb functions.
                libusbInitialized = true;
            }
            else
            {
                //Couldn't open the libusb library correctly for some unknown reason.
                //Bug out for now, will try again next time the while loop executes.
                //Sleep for awhile in the while(1) loop, so we don't monopolize the CPU.
                SleepThread.msleep(50);
                continue;
            }
        }//if(libusbInitialized == false)


        //Check if the device was last known to be not connected, if so, check
        //and see if it has recently become connected.
        if(isConnected == false)
        {
            //Check to see if there is a "dead" handle (to a device that has subsequently been
            //unplugged) still consuming resources.  If so, destroy it before trying to make a new one.
            if(localLibUSBDeviceHandle != NULL)
            {
                //libusb_release_interface(); //Normally supposed to call this prior to calling lubusb_close(), but since
                //the USB device is presumably detached right now, this is rather extraneous and should error out with LIBUSB_ERROR_NO_DEVICE
                libusb_close(localLibUSBDeviceHandle);   //
                localLibUSBDeviceHandle = NULL;
            }

            //Try to find if a USB device of matching VID/PID is attached to the system.
            //First, we need to get a list of devices attached to the system.
            numberOfDevices = libusb_get_device_list(NULL, &list);
            //Now search through every item in the list, to see if any of them match
            //our device with matching VID/PID
            for(i = 0; i < numberOfDevices; i++)
            {
                //Fetch the device descriptor for a device in the list
                libusb_get_device_descriptor(list[i], &deviceDescriptor);
                //Check if the VID/PID in the device descriptor match the one we are looking for
                if((deviceDescriptor.idVendor == USB_VID_TO_FIND) && (deviceDescriptor.idProduct == USB_PID_TO_FIND))
                {
                    //Now try to open the device.
                    if(libusb_open(list[i], &localLibUSBDeviceHandle) == 0)
                    {
                        //We successfully opened the device! The LibUSBDeviceHandle
                        //will have a valid non-null value.
                        break;  //Exit for() loop.  We found the device of interest.
                    }
                    else
                    {
                        //Something went wrong if we get to here...
                        libusb_close(localLibUSBDeviceHandle);
                        localLibUSBDeviceHandle = NULL;
                    }
                }
            }

            //Free up the resources associated with the device list, now that we are done using it
            libusb_free_device_list(list, 1);

            //Check if we successfully opened the device.
            if(localLibUSBDeviceHandle != NULL)
            {
                //A device with matching VID/PID was found.  Now try to "claim"
                //the USB interface #0 on the device (should normally be interface 0
                //unless we are connecting to a composite device with multiple interfaces).
                status = libusb_claim_interface(localLibUSBDeviceHandle, 0);
                if(status == 0)
                {
                    //We found and connected (to interface 0) successfully on the first device with matching VID/PID
                    isConnected = true;
                    oldIsConnected = false; //intentionally opposite of isConnected, so as to send alert to app
                }
            }
            else
            {
                //No device with matching VID/PID was attached to the system
                isConnected = false;
            }
        }//if(isConnected == false)
        else
        {
            //The device has previously been connected.  Re-check the connection
            //status to make sure the user hasn't recently unplugged the device.
            //First, we need to get a list of devices attached to the system.
            numberOfDevices = libusb_get_device_list(NULL, &list);
            //Now search through every item in the list, to see if any of them match
            //our device with matching VID/PID
            deviceFound = false;
            for(i = 0; i < numberOfDevices; i++)
            {
                //Fetch the device descriptor for a device in the list
                libusb_get_device_descriptor(list[i], &deviceDescriptor);
                //Check if the VID/PID in the device descriptor match the one we are looking for
                if((deviceDescriptor.idVendor == 0x04D8) && (deviceDescriptor.idProduct == 0x0053))
                {
                    deviceFound = true;
                    break;
                }
            }

            //Free up the resources associated with the device list, now that we are done using it
            libusb_free_device_list(list, 1);

            //Verify if we found the device or not.
            if(deviceFound == false)
            {
                //The device must have recently been unplugged.
                isConnected = false;
            }
        }

        //Update the broad scope libUSBDeviceHandle to match our local, more up to date copy.
        //We are using a mutex to make sure other thread(s) are not currently using the
        //libUSBDeviceHandle while we are writing to it.  Note: The libUSBDeviceHandle is
        //a pointer, and in theory, assignment of a pointer should normally be an atomic
        //opertion on an x86 CPU.  However, since this is a cross platform application, it is
        //probably best not to make this assumption.  Some CPU types, especially embedded
        //CPUs, can have an ALU data width that is shorter than the address bus width, whereby
        //pointer assignments typically would not be atomic.  Therefore, to be safe, we will
        //protect usage/access to the libUSBDeviceHandle variable with a mutex.
        mutex->lock();
        libUSBDeviceHandle = localLibUSBDeviceHandle;
        mutex->unlock();

        //If the connection status has recently changed, alert the application
        //of the new status (so it can execute callbacks/do other things,
        //such as disabling buttons, etc., based on USB connection status).
        if(oldIsConnected != isConnected)
        {
            oldIsConnected = isConnected;   //Update for next time we check
            //Let the application know of the recently changed status
            emit(comm_update_connection(isConnected));
        }

        //Sleep for awhile in the while() loop, so we don't monopolize the CPU.
        SleepThread.msleep(50);
    }//while(1)
}//void Usbcomm::PollUSBConnection(void)



//Method that may be called to send the Toggle LED(s) command to the
//device firmware using the application data endpoint.
void Usbcomm::SendToggleLEDs(void) {
    int status;
    unsigned char OUTDataBuffer[64];
    int NumBytesSent;

    //Prepare an OUT data packet to send to the device
    OUTDataBuffer[0] = 0x80;    //0x80 in the byte 0 position is command to toggle LEDs in the USB device firmware...

    //Verify the USB device and software is in a proper state where it is legal to
    //use the read/write APIs, prior to actually calling them.
    mutex->lock();   //Get a mutex lock so as to be certain the libUSBDeviceHandle can't change while we are using it.
    if((libUSBDeviceHandle == NULL) || (isConnected == false) || (libusbInitialized == false))
    {
        //Device status isn't ready to communicate for whatever reason.  Therefore
        //we won't try to call the read/write APIs.
        mutex->unlock();  //Done using libUSBDeviceHandle, release our lock on it.
        return;
    }

    //Send the OUT data packet that we just prepared to the USB device.
    status = libusb_bulk_transfer(libUSBDeviceHandle, 0x01, &OUTDataBuffer[0], 1, &NumBytesSent, 10000);  //Send a 1 byte bulk data transfer to EP1 OUT
    mutex->unlock();  //Done using libUSBDeviceHandle, release our lock on it.
    if(status == 0)
    {
        //We successfully sent all of the data if the NumBytesSent variable
        //now contains '1' (which was our intended transfer size).
    }
    else if((status == LIBUSB_ERROR_NO_DEVICE) || (status == LIBUSB_ERROR_IO))
    {
        //The device may have been detached...  Notify the app.
        isConnected = false;

        //Let the application know of the detached status
        emit(comm_update_connection(isConnected));
    }
}



//Method that may be called to send the "get pushbutton status" command to the
//device firmware, and subsequently request the response packet with the state
//data.
void Usbcomm::PollPushbuttonState(void)
{
    int status;
    unsigned char OUTDataBuffer[64];
    unsigned char INDataBuffer[64];
    int NumBytesTransferred;

    //Prepare an OUT USB data packet to send to the device
    OUTDataBuffer[0] = 0x81;    //0x81 in the byte 0 position is command to get the current pushbutton state from the firmware.


    //Verify the USB device and software is in a proper state where it is legal to
    //use the read/write APIs, prior to actually calling them.
    mutex->lock();   //Get a mutex lock so as to be certain the libUSBDeviceHandle can't change while we are using it.
    if((libUSBDeviceHandle == NULL) || (isConnected == false) || (libusbInitialized == false))
    {
        //Device status isn't ready to communicate for whatever reason.  Therefore
        //we won't try to call the read/write APIs.
        mutex->unlock();  //Done using libUSBDeviceHandle, release our lock on it.
        return;
    }

    //Now send the OUT data packet that we just prepared to the USB device.

    //----------------------------------------------------------------------------------
    //Add code here: Add code to send a 1 byte packet to the USB OUT endpoint 1 on
    //the device, consisting of the OUTDataBuffer[] contents.  This data byte will
    //let the microcontroller firmware know that we want to know the demo board
    //pushbutton state.
    //Hint 1: One of the libusb_...() API functions described in the
    //        "\Labs\Lab Instructions and Reference\libusb_1.0_APIs_Synchronous_IO.pdf"
    //        document may be useful for this purpose.
    //Hint 2: If a "libusb_device_handle*" happens to be needed, one has already been
    //        opened for you and is called "libUSBDeviceHandle".
    //Hint 3: Only one line of code is needed, but make sure to save the status returned
    //        by the function in the "status" variable.
    //Hint 4: Some existing code in the SendToggleLEDs() function (above) may be useful
    //        to look at as a reference.
    //----------------------------------------------------------------------------------


    status = libusb_bulk_transfer(libUSBDeviceHandle, 0x01, &OUTDataBuffer[0], 1, &NumBytesTransferred, 10000);  //Send a 1 byte bulk data transfer to EP1 OUT


    //----------------------------------------------------------------------------------
    mutex->unlock();  //Done using libUSBDeviceHandle, release our lock on it.
    //Check the status of the last I/O request to make sure it worked correctly
    if(status == 0)
    {
        //We successfully sent the request.  Now try to fetch the response from the device.
        mutex->lock();   //Get a mutex lock so as to be certain the libUSBDeviceHandle can't change while we are using it.

        //----------------------------------------------------------------------------------
        //Add code here: Add code to receive a 64 byte IN data packet from the USB IN endpoint 1 on
        //the device, which will contain the demo board pushbutton status information.  Place the
        //received data in the INDataBuffer[] buffer.
        //Hint 1: One of the libusb_...() API functions described in the
        //        "\Labs\Lab Instructions and Reference\libusb_1.0_APIs_Synchronous_IO.pdf"
        //        document may be useful for this purpose.
        //Hint 2: If a "libusb_device_handle*" happens to be needed, one has already been
        //        opened for you and is called "libUSBDeviceHandle".
        //Hint 3: Only one line of code is needed, but make sure to save the status returned
        //        by the function in the "status" variable.
        //Hint 4: The endpoint 1 IN endpoint/PipeID is encoded as "0x81" (consists of endpoint
        //        number [1] ORed with 0x80, which signifies an IN endpoint).
        //----------------------------------------------------------------------------------

        status = libusb_bulk_transfer(libUSBDeviceHandle, 0x81, &INDataBuffer[0], 64, &NumBytesTransferred, 10000);  //Request to read 64 bytes of bulk data from EP1 IN



        //----------------------------------------------------------------------------------
        mutex->unlock();  //Done using libUSBDeviceHandle, release our lock on it.
        //Check the status of the last I/O request to make sure it worked correctly
        if(status == 0)
        {
            //We successfully received the data.  The pushbutton state should be stored in byte index 1.
            //Byte index 0 should be an echo of the command that the firmware was responding to.
            if(INDataBuffer[0] == 0x81)
            {
                if(INDataBuffer[1] == 0x00)
                {
                    //The pushbutton is pressed.
                    isPushbuttonPressed = true;
                }
                else
                {
                    isPushbuttonPressed = false;
                }
                emit(comm_update_pushbutton(isPushbuttonPressed));
            }
            else
            {
                //The firmware sent us some unexpected data packet for some reason.
                //Double check the firmware if you get here.
            }
        }
        else if((status == LIBUSB_ERROR_NO_DEVICE) || (status == LIBUSB_ERROR_IO))
        {
            //The device may have been detached...  Notify the app.
            isConnected = false;

            //Let the application know of the detached status
            emit(comm_update_connection(isConnected));
        }


    }
    else if((status == LIBUSB_ERROR_NO_DEVICE) || (status == LIBUSB_ERROR_IO))
    {
        //The device may have been detached...  Notify the app.
        isConnected = false;

        //Let the application know of the detached status
        emit(comm_update_connection(isConnected));
    }

}




