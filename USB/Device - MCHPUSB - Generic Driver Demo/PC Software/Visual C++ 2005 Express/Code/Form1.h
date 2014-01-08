#pragma once
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
/********************************************************************
 FileName:		Form1.h
 Dependencies:	Windows Server 2003 R2 Platform SDK during development.  
				When compiled, needs .NET framework 2.0 redistributable
				to run (under Microsoft Windows XP, Vista comes with 
				.NET 2.0 redistributable package already installed)
				May also need Microsoft Visual C++ redistributable to run.
 Hardware:		Need a free USB port to connect USB peripheral device
				programmed with appropriate MCHPUSB demo firmware.  VID and
				PID in firmware must match the VID and PID in this
				program.
 Compiler:  	Microsoft Visual C++ 2005 Express Edition (or newer: 2008 
				and 2010 versions will also work, simply upgrade the 
				solution files using the automatic conversion wizard)
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
  1.0   06/15/2008   Initial release
  2.7a	08/29/2010	 Adding explicit calling conventions to the DllImports.
					 This is needed for Visual Studio 2010 compatibility.
					 No functional changes to the code.  Backwards compatibility
					 should be retained.
********************************************************************
NOTE:	All user made code contained in this project is in the Form1.h file.
		All other code and files were generated automatically by either the
		new project wizard, or by the development environment (ex: code is
		automatically generated if you create a new button on the form, and
		then double click on it, which creates a click event handler
		function).  All user made code is contained in clearly marked cut and
		paste blocks.

NOTE2:  Need Windows Server 2003 R2 Platform SDK.  Untested with Windows SDK.  
		If getting build errors when trying to compile this project, make sure the
		platform SDK is correctly installed, and that it has been integrated into the VC++ development
		environment.  In other words, make sure the include paths have been setup correctly in this IDE.
		Microsoft created some small tutorial videos showing how to do this.
		These may be located at:
		http://msdn.microsoft.com/en-us/visualc/aa336415.aspx 
		The link is functional as of 21 May 2008, but could move.  If the above link does not work,
		try searching for "Video1_PSDK.wmv", which may be associated with:
		"Using Visual C++ Express Edition and the Platform SDK"		
********************************************************************/


//Includes
#include <windows.h>	//Definitions for various common and not so common types like DWORD, PCHAR, HANDLE, etc.
#include <Dbt.h>		//Need this for definitions of WM_DEVICECHANGE messages
#include "mpusbapi.h"	//Make sure this header file is located in your project directory.

//When modifying the firmware and changing the Vendor and Device ID's, make
//sure to update the PC application as well.
//Use the formatting: "Vid_xxxx&Pid_xxxx" where xxxx is a 16-bit hexadecimal number.
//This is the USB device that this program will look for:
#define DeviceVID_PID "vid_04d8&pid_000c"
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


namespace MCHPUSBPnPDemo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
	using namespace System::Threading;	
	using namespace System::Runtime::InteropServices;  //Need this to support "unmanaged" code.

	/*
	In order to use these unmanaged functions from within the managed .NET environment, we need
	to explicitly import the functions which we will be using from other .DLL file(s).  Simply
	including the appropriate header files is not enough. 

	Note: In order to avoid potential name conflicts in the header files (which we still use),
	I have renamed the functions by adding "UM" (unmanaged) onto the end of them.  To find 
	documentation for the functions in MSDN, search for the function name without the extra 
	"UM" attached.
	Note2: In the header files, normally the function names are 
	remapped, depending upon if UNICODE is defined or not.  For example, two versions of the
	function SetupDiGetDeviceInterfaceDetail() exist.  One for UNICODE, and one for ANSI.  
	If the wrong version of the function is called, things won't work correctly.  Therefore,
	in order to make sure the correct one gets called (based on your compiler settings, which
	may or may not define "UNICODE"), it is useful to explicity specify the CharSet when doing
	the DLL import.
	*/

	#ifdef UNICODE
	#define	Seeifdef	Unicode
	#else
	#define Seeifdef	Ansi
	#endif

	//See the mpusbapi.dll source code (_mpusbapi.cpp) for API related documentation for these functions.
	//The source code is in the MCHPFSUSB vX.X distributions.
	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBGetDLLVersion", CallingConvention=CallingConvention::Cdecl)] 
	extern "C" DWORD MPUSBGetDLLVersion(void);
	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBGetDeviceCount", CallingConvention=CallingConvention::Cdecl)] 
	extern "C" DWORD MPUSBGetDeviceCount(PCHAR pVID_PID);
	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBOpen", CallingConvention=CallingConvention::Cdecl)]
	extern "C" HANDLE MPUSBOpen(DWORD instance,	//  Input
										PCHAR pVID_PID,	// Input
										PCHAR pEP,		// Input
										DWORD dwDir,	// Input
										DWORD dwReserved);// Input

	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBClose", CallingConvention=CallingConvention::Cdecl)] 
	extern "C" BOOL MPUSBClose(HANDLE handle);	//Input
	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBRead", CallingConvention=CallingConvention::Cdecl)] 
	extern "C" DWORD MPUSBRead(HANDLE handle,	// Input
										PVOID pData,	// Output
										DWORD dwLen,	// Input
										PDWORD pLength,	// Output
										DWORD dwMilliseconds);// Input

	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBWrite", CallingConvention=CallingConvention::Cdecl)] 
	extern "C" DWORD MPUSBWrite(HANDLE handle,	// Input
										PVOID pData,	// Output
										DWORD dwLen,	// Input
										PDWORD pLength,	// Output
										DWORD dwMilliseconds);// Input
	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBReadInt", CallingConvention=CallingConvention::Cdecl)] 
	extern "C" DWORD MPUSBReadInt(HANDLE handle,	// Input
										PVOID pData,	// Output
										DWORD dwLen,	// Input
										PDWORD pLength,	// Output
										DWORD dwMilliseconds);// Input

	//Need this function for receiving all of the WM_DEVICECHANGE messages.  See MSDN documentation for
	//description of what this function does/how to use it. Note: name is remapped "RegisterDeviceNotificationUM" to
	//avoid possible build error conflicts.
	[DllImport("user32.dll" , CharSet = CharSet::Seeifdef, EntryPoint="RegisterDeviceNotification", CallingConvention=CallingConvention::Winapi)]					
	extern "C" HDEVNOTIFY WINAPI RegisterDeviceNotificationUM(
		HANDLE hRecipient,
		LPVOID NotificationFilter,
		DWORD Flags);




	//----------------Global variables used in this application--------------------------------
	HANDLE  EP1INHandle = INVALID_HANDLE_VALUE;
	HANDLE  EP1OUTHandle = INVALID_HANDLE_VALUE;
	unsigned int ADCValue = 0;
	BOOL AttachedState = FALSE;		//Need to keep track of the USB device attachment status for proper plug and play operation.
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------



	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
			//Additional constructor code

			//Globally Unique Identifier (GUID). Windows uses GUIDs to identify things.  
			GUID InterfaceClassGuid = {0xa5dcbf10, 0x6530, 0x11d2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED}; //Globally Unique Identifier (GUID) for USB peripheral devices

			//Register for WM_DEVICECHANGE notifications:
			DEV_BROADCAST_DEVICEINTERFACE MyDeviceBroadcastHeader;// = new DEV_BROADCAST_HDR;
			MyDeviceBroadcastHeader.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
			MyDeviceBroadcastHeader.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
			MyDeviceBroadcastHeader.dbcc_reserved = 0;	//Reserved says not to use...
			MyDeviceBroadcastHeader.dbcc_classguid = InterfaceClassGuid;
			RegisterDeviceNotificationUM((HANDLE)this->Handle, &MyDeviceBroadcastHeader, DEVICE_NOTIFY_WINDOW_HANDLE);

			//Now perform an initial start up check of the device state (attached or not attached), since we would not have
			//received a WM_DEVICECHANGE notification.
			if(MPUSBGetDeviceCount(DeviceVID_PID))	//Check and make sure at least one device with matching VID/PID is attached
			{
				EP1OUTHandle = MPUSBOpen(0, DeviceVID_PID, "\\MCHP_EP1", MP_WRITE, 0);
				EP1INHandle = MPUSBOpen(0, DeviceVID_PID, "\\MCHP_EP1", MP_READ, 0);

				AttachedState = TRUE;
				StatusBox_txtbx->Text = "Device Found: AttachedState = TRUE";
				label2->Enabled = true;	//Make the label no longer greyed out
			}
			else	//Device must not be connected (or not programmed with correct firmware)
			{
				StatusBox_txtbx->Text = "Device Not Detected: Verify Connection/Correct Firmware";
				AttachedState = FALSE;
				label2->Enabled = false;	//Make the label greyed out
			}


			ReadWriteThread->RunWorkerAsync();	//Recommend performing USB read/write operations in a separate thread.  Otherwise,
												//the Read/Write operations are effectively blocking functions and can lock up the
												//user interface if the I/O operations take a long time to complete.
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
			//Make sure to close any open handles, before exiting the application
			if (EP1OUTHandle != INVALID_HANDLE_VALUE)
				MPUSBClose (EP1OUTHandle);
			if (EP1INHandle != INVALID_HANDLE_VALUE)
				MPUSBClose (EP1INHandle);
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::ProgressBar^  progressBar1;
	protected: 
	private: System::ComponentModel::BackgroundWorker^  ReadWriteThread;
	private: System::Windows::Forms::Timer^  timer1;

	private: System::Windows::Forms::TextBox^  StatusBox_txtbx;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;


	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->ReadWriteThread = (gcnew System::ComponentModel::BackgroundWorker());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->StatusBox_txtbx = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// progressBar1
			// 
			this->progressBar1->BackColor = System::Drawing::Color::White;
			this->progressBar1->ForeColor = System::Drawing::Color::Red;
			this->progressBar1->Location = System::Drawing::Point(13, 122);
			this->progressBar1->Maximum = 1024;
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(298, 23);
			this->progressBar1->Step = 1;
			this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBar1->TabIndex = 9;
			// 
			// ReadWriteThread
			// 
			this->ReadWriteThread->WorkerReportsProgress = true;
			this->ReadWriteThread->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::ReadWriteThread_DoWork);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 8;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// StatusBox_txtbx
			// 
			this->StatusBox_txtbx->Location = System::Drawing::Point(13, 13);
			this->StatusBox_txtbx->Multiline = true;
			this->StatusBox_txtbx->Name = L"StatusBox_txtbx";
			this->StatusBox_txtbx->Size = System::Drawing::Size(298, 20);
			this->StatusBox_txtbx->TabIndex = 11;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(317, 16);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(58, 13);
			this->label1->TabIndex = 12;
			this->label1->Text = L"Status Box";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Enabled = false;
			this->label2->Location = System::Drawing::Point(123, 106);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(93, 13);
			this->label2->TabIndex = 13;
			this->label2->Text = L"ANx/POT Voltage";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(386, 175);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->StatusBox_txtbx);
			this->Controls->Add(this->progressBar1);
			this->Name = L"Form1";
			this->Text = L"MCHPUSB PnP Demo";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion




//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------

private: System::Void ReadWriteThread_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			//This thread does the actual USB read/write operations (but only when AttachedState == TRUE).
			//Since this is a separate thread, this code below executes asynchronously from the reset of the
			//code in this application.
			unsigned char Buffer [64];
			DWORD ActualLength;

			while(true)
			{
				Buffer[0] = 0x37;			//READ_POT command (see the firmware source code), gets 10-bit ADC Value
				if(AttachedState == TRUE)	//Do not try and send USB traffic unless the device is actually attached and enumerated/configured.
				{
					if(MPUSBWrite(EP1OUTHandle, Buffer, 1, &ActualLength, 1000))	//Send the command now over USB
					{
						if(MPUSBRead(EP1INHandle, Buffer, 3, &ActualLength, 1000))	//Receive the answer from the device firmware through USB
						{
							ADCValue = (Buffer[2] << 8) + Buffer[1];
						}
					}
				}
				else
				{
					ADCValue = 0;
				}
				Sleep(4);	//Add a small delay.  Otherwise, this while(true) loop executes at warp speed and can cause 
							//tangible CPU utilization increase, with no particular benefit to the application.
			}
		}



private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
			//When the timer goes off on the main form, update the user inferface with the new data obtained from the thread.
			 if(ADCValue > (unsigned int)progressBar1->Maximum)
			 {
				 ADCValue = progressBar1->Maximum;
			 }

			 progressBar1->Value = ADCValue;

		 }


protected: virtual void WndProc( Message% m ) override{
		 //This is a callback function that gets called when a Windows message is received by the form.

		 // Listen for Windows messages.  We will receive various different types of messages, but the ones we really want to use are the WM_DEVICECHANGE messages.
		 if(m.Msg == WM_DEVICECHANGE)
		 {
			 if(((int)m.WParam == DBT_DEVICEARRIVAL) || ((int)m.WParam == DBT_DEVICEREMOVEPENDING) || ((int)m.WParam == DBT_DEVICEREMOVECOMPLETE) || ((int)m.WParam == DBT_CONFIGCHANGED) )
			 {

				//WM_DEVICECHANGE messages by themselves are quite generic, and can be caused by a number of different
				//sources, not just your USB hardware device.  Therefore, must check to find out if any changes relavant
				//to your device (with known VID/PID) took place before doing any kind of opening or closing of endpoints.
				//(the message could have been totally unrelated to your application/USB device)
				if(MPUSBGetDeviceCount(DeviceVID_PID))	//Check and make sure at least one device with matching VID/PID is attached
				{
					if(AttachedState == FALSE)
					{
						EP1OUTHandle = MPUSBOpen(0, DeviceVID_PID, "\\MCHP_EP1", MP_WRITE, 0);
						EP1INHandle = MPUSBOpen(0, DeviceVID_PID, "\\MCHP_EP1", MP_READ, 0);

						AttachedState = TRUE;
						StatusBox_txtbx->Text = "Device Found: AttachedState = TRUE";
						label2->Enabled = true;	//Make the label no longer greyed out
					}
					//else AttachedState == TRUE, in this case, do not try to re-open already open and functioning endpoints.  Doing
					//so will break them and make them no longer work.
				}
				else	//No devices attached, WM_DEVICECHANGE message must have been caused by your USB device falling off the bus (IE: user unplugged/powered down)
				{
					if(MPUSBClose(EP1OUTHandle))
						EP1OUTHandle = INVALID_HANDLE_VALUE;
					if(MPUSBClose(EP1INHandle))
						EP1INHandle = INVALID_HANDLE_VALUE;

					AttachedState = FALSE;
					StatusBox_txtbx->Text = "Device Not Detected: Verify Connection/Correct Firmware";
					label2->Enabled = false;	//Make the label greyed out

				}
			 }
		 }

		 Form::WndProc( m );
	}
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


};
}

