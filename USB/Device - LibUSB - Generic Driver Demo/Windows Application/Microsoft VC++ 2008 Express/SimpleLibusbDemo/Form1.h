#pragma once
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
/********************************************************************
 FileName:		Form1.h
 Dependencies:
 Hardware:		Need a free USB port to connect USB peripheral device
				programming with appropriate Libusb firmware.  VID and
				PID in firmware must match the VID and PID in this
				program.
 Compiler:  	Microsoft Visual C++ 2008 Express Edition (or better)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively with Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
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
//Includes
#include <windows.h>
#include <errno.h>
#include "usb.h"

/*In order to build the application, 
  1) Copy the file <libusb-win32 unzipped folder>\ libusb-win32-device-bin-0.1.12.1\lib\msvc\libusb.lib and paste to ‘lib’ folder of the VC++. 
  2) Copy the file <libusb-win32 unzipped folder>\ libusb-win32-device-bin-0.1.12.1\include\usb.h and paste to the “<Install Directory>\USB Device - Libusb - Generic Driver Demo\Windows Application\Microsoft VC++ 2008 Express\SimpleLibusbDemo’ folder.*/ 

//Modify this value to match the VID and PID in your USB device descriptor.
#define MY_VID	0x04D8 //0x04D8
#define MY_PID	0x0204   //0x0204

#define CONNECTED 1

// Global Variables
unsigned char Connection_Status;
usb_dev_handle *MyLibusbDeviceHandle = NULL; /* the device handle */


//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace SimpleLibusbDemo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  Connect_btn;
	private: System::Windows::Forms::Button^  ToggleLED_btn;
	private: System::Windows::Forms::Button^  GetPushbuttonState_btn;
	private: System::Windows::Forms::Label^  StateLabel;
	protected:

	protected:




	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->Connect_btn = (gcnew System::Windows::Forms::Button());
			this->ToggleLED_btn = (gcnew System::Windows::Forms::Button());
			this->GetPushbuttonState_btn = (gcnew System::Windows::Forms::Button());
			this->StateLabel = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			//
			// Connect_btn
			//
			this->Connect_btn->Location = System::Drawing::Point(12, 14);
			this->Connect_btn->Name = L"Connect_btn";
			this->Connect_btn->Size = System::Drawing::Size(66, 27);
			this->Connect_btn->TabIndex = 0;
			this->Connect_btn->Text = L"Connect";
			this->Connect_btn->UseVisualStyleBackColor = true;
			this->Connect_btn->Click += gcnew System::EventHandler(this, &Form1::Connect_btn_Click);
			//
			// ToggleLED_btn
			//
			this->ToggleLED_btn->Enabled = false;
			this->ToggleLED_btn->Location = System::Drawing::Point(106, 15);
			this->ToggleLED_btn->Name = L"ToggleLED_btn";
			this->ToggleLED_btn->Size = System::Drawing::Size(119, 26);
			this->ToggleLED_btn->TabIndex = 1;
			this->ToggleLED_btn->Text = L"Toggle LED(s)";
			this->ToggleLED_btn->UseVisualStyleBackColor = true;
			this->ToggleLED_btn->Click += gcnew System::EventHandler(this, &Form1::ToggleLED_btn_Click);
			//
			// GetPushbuttonState_btn
			//
			this->GetPushbuttonState_btn->Enabled = false;
			this->GetPushbuttonState_btn->Location = System::Drawing::Point(106, 53);
			this->GetPushbuttonState_btn->Name = L"GetPushbuttonState_btn";
			this->GetPushbuttonState_btn->Size = System::Drawing::Size(119, 27);
			this->GetPushbuttonState_btn->TabIndex = 2;
			this->GetPushbuttonState_btn->Text = L"Get Pushbutton State";
			this->GetPushbuttonState_btn->UseVisualStyleBackColor = true;
			this->GetPushbuttonState_btn->Click += gcnew System::EventHandler(this, &Form1::GetPushbuttonState_btn_Click);
			//
			// StateLabel
			//
			this->StateLabel->AutoSize = true;
			this->StateLabel->Enabled = false;
			this->StateLabel->Location = System::Drawing::Point(247, 67);
			this->StateLabel->Name = L"StateLabel";
			this->StateLabel->Size = System::Drawing::Size(84, 13);
			this->StateLabel->TabIndex = 3;
			this->StateLabel->Text = L"State: Unknown";
			//
			// Form1
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(343, 101);
			this->Controls->Add(this->StateLabel);
			this->Controls->Add(this->GetPushbuttonState_btn);
			this->Controls->Add(this->ToggleLED_btn);
			this->Controls->Add(this->Connect_btn);
			this->Name = L"Form1";
			this->Text = L"Simple Libusb Demo";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &Form1::Form1_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void Connect_btn_Click(System::Object^  sender, System::EventArgs^  e)
		 {

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
		/*
		Before we can "connect" our application to our USB embedded device, we must first find the device.
		A USB bus can have many devices simultaneously connected, so somehow we have to find our device, and only
		our device.  This is done with the Vendor ID (VID) and Product ID (PID).  Each USB product line should have
		a unique combination of VID and PID. */

			 if ( Connection_Status != CONNECTED)  // if not connected already
			 {
				 usb_init(); /* initialize the library */
				 usb_find_busses(); /* find all busses */
				 usb_find_devices(); /* find all connected devices */

				 //Now look through the list that we just populated.  We are trying to see if any of them match our device.
				 struct usb_bus *bus;
				 struct usb_device *dev;
				 for(bus = usb_get_busses(); bus; bus = bus->next)
				 {
					for(dev = bus->devices; dev; dev = dev->next)
					{
							if(dev->descriptor.idVendor == MY_VID
								&& dev->descriptor.idProduct == MY_PID)
							{
								MyLibusbDeviceHandle = usb_open(dev); //Opens a USB device
								break;
							}
					 }
				  }
				 if(!MyLibusbDeviceHandle)
				 {
					 return;
				 }
				 if(usb_set_configuration(MyLibusbDeviceHandle, 1) < 0) // Sets the Active configuration of the device
				 {
					 usb_close(MyLibusbDeviceHandle);
					 return;
				 }

				 if(usb_claim_interface(MyLibusbDeviceHandle, 0) < 0)  //claims the interface with the Operating System
				 {
					 //Closes a device opened since the claim interface is failed.
					 usb_close(MyLibusbDeviceHandle);
					 return ;
				 }
				 ToggleLED_btn->Enabled = true;				//Make button no longer greyed out
				 GetPushbuttonState_btn->Enabled = true;	//Make button no longer greyed out
				 StateLabel->Enabled = true;	            //Make label no longer greyed out
				 Connection_Status = CONNECTED;	            //Now status is connected
			 }
	     //-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
		 //-------------------------------------------------------------------------------------------------------------------------------------------------------------------

		}


private: System::Void ToggleLED_btn_Click(System::Object^  sender, System::EventArgs^  e)
		 {
		 //-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		 //-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------

			 char OutputPacketBuffer[64]; //Allocate a memory buffer which will contain data to send to the USB device
			 OutputPacketBuffer[0] = 0x80;		   //0x80 is the "Toggle LED(s)" command in the firmware
											       //For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
											       //data in it if you like.
			 //Writes data to a bulk endpoint. The Function call will send out 64 bytes to the USB Device.
			 if(usb_bulk_write(MyLibusbDeviceHandle, 0x01, &OutputPacketBuffer[0], 64, 5000) != 64)
			 {
				 return;
			 }
		 //-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
		 //-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		 }


private: System::Void GetPushbuttonState_btn_Click(System::Object^  sender, System::EventArgs^  e)
		 {

		 //-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
		 //-------------------------------------------------------------------------------------------------------------------------------------------------------------------
			 char OutputPacketBuffer[64]; //Allocate a memory buffer which will contain data to send to the USB device
			 char InputPacketBuffer[64];  //Allocate a memory buffer for the data which we will read from the USB device
			 OutputPacketBuffer[0] = 0x81; //0x81 is the "Get Pushbutton State" command in the firmware
												//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
												//data in it if you like.

			//To get the pushbutton state, first, we send a packet with our "Get Pushbutton State" command in it.
			//The following call to usb_bulk_write() sends 64 bytes of data to the USB device.
			 if(usb_bulk_write(MyLibusbDeviceHandle, 0x01, &OutputPacketBuffer[0], 64, 5000) != 64)
			 {
				 return;
			 }

			 //Now get the response packet from the firmware.
			 //The following call to usb_bulk_read() retrieves 64 bytes of data from the USB device.
			 if(usb_bulk_read(MyLibusbDeviceHandle, 0x81, &InputPacketBuffer[0], 64, 5000) != 64)
			 {
				 return;
			 }

			 //InputPacketBuffer[0] is an echo back of the command.
			 //InputPacketBuffer[1] contains the I/O port pin value for the pushbutton.
			 if (InputPacketBuffer[1] == 0x01)
			 {
				 StateLabel->Text = "State: Not Pressed";
			 }
			 else
			 {
				 StateLabel->Text = "State: Pressed";
			 }
		 //-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
		 //-------------------------------------------------------------------------------------------------------------------------------------------------------------------

		 }
private: System::Void Form1_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {

		 //-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
		 //-------------------------------------------------------------------------------------------------------------------------------------------------------------------
			if ( Connection_Status == CONNECTED)
			{
				//The following functiom releases a previously claimed interface
				usb_release_interface(MyLibusbDeviceHandle, 0);

				//closes a device opened
				usb_close(MyLibusbDeviceHandle);
			}
			return;
		 //-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
		 //-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		 }

};
}

//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/* The below portion of the code is used link dynamically to the libusb0.dll. The run time linking
	    is done only to the functions which are used by the demo. If the user wants to use all the
		functinalties of the liusb, then delete the below portion of the code and add "libusb_dyn.c"
		file (distributed along with libusb-win32 project) to this application. Also change the file name
		to "libusb_dyn.cpp" before building.*/



typedef usb_dev_handle * (*usb_open_t)(struct usb_device *dev);
typedef int (*usb_close_t)(usb_dev_handle *dev);
typedef int (*usb_bulk_write_t)(usb_dev_handle *dev, int ep, char *bytes,
                                int size, int timeout);
typedef int (*usb_bulk_read_t)(usb_dev_handle *dev, int ep, char *bytes,
                               int size, int timeout);
typedef int (*usb_set_configuration_t)(usb_dev_handle *dev, int configuration);
typedef int (*usb_claim_interface_t)(usb_dev_handle *dev, int interface);
typedef int (*usb_release_interface_t)(usb_dev_handle *dev, int interface);
typedef void (*usb_init_t)(void);
typedef int (*usb_find_busses_t)(void);
typedef int (*usb_find_devices_t)(void);
typedef struct usb_device * (*usb_device_t)(usb_dev_handle *dev);
typedef struct usb_bus * (*usb_get_busses_t)(void);



static usb_open_t _usb_open = NULL;
static usb_close_t _usb_close = NULL;
static usb_bulk_write_t _usb_bulk_write = NULL;
static usb_bulk_read_t _usb_bulk_read = NULL;
static usb_set_configuration_t _usb_set_configuration = NULL;
static usb_claim_interface_t _usb_claim_interface = NULL;
static usb_release_interface_t _usb_release_interface = NULL;
static usb_init_t _usb_init = NULL;
static usb_find_busses_t _usb_find_busses = NULL;
static usb_find_devices_t _usb_find_devices = NULL;
static usb_device_t _usb_device = NULL;
static usb_get_busses_t _usb_get_busses = NULL;



void usb_init(void)
{

  HINSTANCE libusb_dll  = LoadLibrary("libusb0.dll");

  if(!libusb_dll)
    return;

  _usb_open = (usb_open_t)
    GetProcAddress(libusb_dll, "usb_open");
  _usb_close = (usb_close_t)
    GetProcAddress(libusb_dll, "usb_close");
  _usb_bulk_write = (usb_bulk_write_t)
    GetProcAddress(libusb_dll, "usb_bulk_write");
  _usb_bulk_read = (usb_bulk_read_t)
    GetProcAddress(libusb_dll, "usb_bulk_read");
  _usb_set_configuration = (usb_set_configuration_t)
    GetProcAddress(libusb_dll, "usb_set_configuration");
  _usb_claim_interface = (usb_claim_interface_t)
    GetProcAddress(libusb_dll, "usb_claim_interface");
  _usb_release_interface = (usb_release_interface_t)
    GetProcAddress(libusb_dll, "usb_release_interface");
  _usb_init = (usb_init_t)
    GetProcAddress(libusb_dll, "usb_init");
  _usb_find_busses = (usb_find_busses_t)
    GetProcAddress(libusb_dll, "usb_find_busses");
  _usb_find_devices = (usb_find_devices_t)
    GetProcAddress(libusb_dll, "usb_find_devices");
  _usb_device = (usb_device_t)
    GetProcAddress(libusb_dll, "usb_device");
  _usb_get_busses = (usb_get_busses_t)
    GetProcAddress(libusb_dll, "usb_get_busses");


  if(_usb_init)
    _usb_init();
}

usb_dev_handle *usb_open(struct usb_device *dev)
{
  if(_usb_open)
    return _usb_open(dev);
  else
    return NULL;
}

int usb_close(usb_dev_handle *dev)
{
  if(_usb_close)
    return _usb_close(dev);
  else
    return -ENFILE;
}

int usb_bulk_write(usb_dev_handle *dev, int ep, char *bytes, int size,
                   int timeout)
{
  if(_usb_bulk_write)
    return _usb_bulk_write(dev, ep, bytes, size, timeout);
  else
    return -ENFILE;
}

int usb_bulk_read(usb_dev_handle *dev, int ep, char *bytes, int size,
                  int timeout)
{
  if(_usb_bulk_read)
    return _usb_bulk_read(dev, ep, bytes, size, timeout);
  else
    return -ENFILE;
}
int usb_set_configuration(usb_dev_handle *dev, int configuration)
{
  if(_usb_set_configuration)
    return _usb_set_configuration(dev, configuration);
  else
    return -ENFILE;
}

int usb_claim_interface(usb_dev_handle *dev, int interface)
{
  if(_usb_claim_interface)
    return _usb_claim_interface(dev, interface);
  else
    return -ENFILE;
}

int usb_release_interface(usb_dev_handle *dev, int interface)
{
  if(_usb_release_interface)
    return _usb_release_interface(dev, interface);
  else
    return -ENFILE;
}

int usb_find_busses(void)
{
  if(_usb_find_busses)
    return _usb_find_busses();
  else
    return -ENFILE;
}

int usb_find_devices(void)
{
  if(_usb_find_devices)
    return _usb_find_devices();
  else
    return -ENFILE;
}

struct usb_device *usb_device(usb_dev_handle *dev)
{
  if(_usb_device)
    return _usb_device(dev);
  else
    return NULL;
}

struct usb_bus *usb_get_busses(void)
{
  if(_usb_get_busses)
    return _usb_get_busses();
  else
    return NULL;
}

 //-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
 //-------------------------------------------------------------------------------------------------------------------------------------------------------------------