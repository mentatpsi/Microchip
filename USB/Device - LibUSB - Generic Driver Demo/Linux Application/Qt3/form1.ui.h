//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//Please see the folder for a .pdf version of this license.

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/


/********************************************************************
 FileName:		Form1.h
 Dependencies:
 Hardware:	Need a free USB port to connect USB peripheral device
		programming with appropriate Libusb firmware.  VID and
		PID in firmware must match the VID and PID in this program.
 Compiler:  	Gcc
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0

********************************************************************
NOTE:	All user made code contained in this project is in the Form1.h file.
		All other code and files were generated automatically by either the
		new project wizard, or by the development environment (ex: code is
		automatically generated if you create a new button on the form, and
		then double click on it, which creates a click event handler
		function).  All user made code is contained in clearly marked cut and
		paste blocks.
********************************************************************/
// Includes
#include <usb.h>
#include <qmessagebox.h>

//Modify this value to match the VID and PID in your USB device descriptor.//Modify this value to match the VID and PID in your USB device descriptor.
#define MY_VID 0x04D8
#define MY_PID 0x0204

/* the device's endpoints */
#define EP_IN 0x81
#define EP_OUT 0x01

#define CONNECTED  0x01

// Global Variables
usb_dev_handle *MyLibusbDeviceHandle = NULL;  /* the device handle */

/* The Below Function Searches for a device with given VID and PID.
It return a hanlde to the device if the device is found in the Bus.*/
usb_dev_handle *open_dev(void)
{
    struct usb_bus *bus;
    struct usb_device *dev;

    for(bus = usb_get_busses(); bus; bus = bus->next)
    {
	for(dev = bus->devices; dev; dev = dev->next)
	{
	    if(dev->descriptor.idVendor == MY_VID
	       && dev->descriptor.idProduct == MY_PID)
	    {
		return usb_open(dev);  //Opens a USB device
	    }
	}
    }
    return NULL;
}


/******************************************************************************
 * Function:        void Connect_Device(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The Function connects the application to the device. This function gets called when the user
                            hits the 'Connect' button on the GUI application
 *
 * Note:            None
 *******************************************************************/
void Form1::Connect_Device(void)
{
    /*  Before we can "connect" our application to our USB embedded device, we must first find the device.
          A USB bus can have many devices simultaneously connected, so somehow we have to find our device, and only
          our device.  This is done with the Vendor ID (VID) and Product ID (PID).  Each USB product line should have
          a unique combination of VID and PID. */

    if ( Connection_Status != CONNECTED)  // if not connected already
    {
	usb_init(); /* initialize the library */
	usb_find_busses(); /* find all busses */
	usb_find_devices(); /* find all connected devices */
	if(!(MyLibusbDeviceHandle = open_dev()))
	{
	    QMessageBox::warning(this, "Connect", " Open Failed", "OK", 0, 0, 1);
	    return;
	}
	if(usb_set_configuration(MyLibusbDeviceHandle, 1) < 0) // Sets the Active configuration of the device
	{
	    QMessageBox::warning(this, "Connect", " Config Failed", "OK", 0, 0, 1);
	    usb_close(MyLibusbDeviceHandle);
	    return;
	}
	if(usb_claim_interface(MyLibusbDeviceHandle, 0) < 0)  //claims the interface with the Operating System
	{
	    QMessageBox::warning(this, "Connect", " Claim Failed", "OK", 0, 0, 1);
	    //Closes a device opened since the claim interface is failed.
	    usb_close(MyLibusbDeviceHandle);
	    return ;
	}
	ToggleLed->setEnabled(true);	 //Make button no longer greyed out
	PushButtonStatus->setEnabled(true); //Make button no longer greyed out
	StateLabel->setEnabled(true);	  //Make label no longer greyed out
	Connection_Status = CONNECTED;	   // Everything went well. Now connection status is CONNECTED
    }
}


/******************************************************************************
 * Function:        void Toggle_LED(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function toggles LED[s] on the Libusb Demo  Device.  This function gets called when the user
                            hits the 'Toggle LED' button on the GUI.
 *
 * Note:            None
 *******************************************************************/

void Form1::Toggle_LED()
{
    char OutputPacketBuffer[64]; //Allocate a memory buffer which will contain data to send to the USB device
    if ( Connection_Status == CONNECTED)
    {
	OutputPacketBuffer[0] = 0x80;  //0x80 is the "Toggle LED(s)" command in the firmware
	 //For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
	//data in it if you like.
	//Writes data to a bulk endpoint. The Function call will send out 64 to the USB Device.
	if(usb_bulk_write(MyLibusbDeviceHandle,  EP_OUT, &OutputPacketBuffer[0], 64, 5000) != 64)
	{
	    QMessageBox::warning(this, "Toggle LED", " Write Failed", "OK", 0, 0, 1);
	    return;
	}
    }
}

/******************************************************************************
 * Function:        void Get_Push_Button_Status(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The Function gets the status of the pushbutton and displays on the GUI. This function gets called when the user
                            hits the 'Get Push Button Status'  button on the GUI.
 *
 * Note:            None
 *******************************************************************/

void Form1::Get_Push_Button_Status()
{
    if ( Connection_Status  == CONNECTED)
    {
	 char OutputPacketBuffer[64]; //Allocate a memory buffer which will contain data to send to the USB device
	 char InputPacketBuffer[64];  //Allocate a memory buffer for the data which we will read from the USB device
	OutputPacketBuffer[0] = 0x81; //0x81 is the "Get Pushbutton State" command in the firmware
	//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real 				//data in it if you like.

	 //To get the pushbutton state, first, we send a packet with our "Get Pushbutton State" command in it.
	//The following call to usb_bulk_write() sends 64 bytes of data to the USB device.
	if(usb_bulk_write(MyLibusbDeviceHandle, EP_OUT, &OutputPacketBuffer[0], 64, 5000)
	!= 64)
	{
	    QMessageBox::warning(this, "Push Button Status", " Write Failed", "OK", 0, 0, 1);
	    return;
	}

	//Now get the response packet from the firmware.
	//The following call to usb_bulk_read() retrieves 64 bytes of data from the USB device.
	if(usb_bulk_read(MyLibusbDeviceHandle, EP_IN, &InputPacketBuffer[0], 64, 5000)
	!= 64)
	{
	    QMessageBox::warning(this, "Push Button Status", " Read Failed", "OK", 0, 0, 1);
	    return;
	}

	//InputPacketBuffer[0] is an echo back of the command.
	//InputPacketBuffer[1] contains the I/O port pin value for the pushbutton.
	if (InputPacketBuffer[1] == 0x01)
	{
	    StateLabel->setText("State: Not Pressed") ;
	}
	else
	{
	    StateLabel->setText("State: Pressed");
	}
    }
}

/******************************************************************************
 * Function:        void Close(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The Function releases the interface... This function gets called when the user
                           closes the GUI application.
 *
 * Note:            None
 *******************************************************************/

void Form1::close()
{
    if ( Connection_Status == CONNECTED)
    {
	//The following functiom releases a previously claimed interface
	usb_release_interface(MyLibusbDeviceHandle, 0);
	//closes a device opened
	usb_close(MyLibusbDeviceHandle);
    }
}


