#pragma once
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
/********************************************************************
 FileName:		Form1.h
 Dependencies:	Windows Server 2003 R2 Platform SDK during development.  
				When compiled, needs .NET framework 2.0 redistributable to run.
				May also need MS VC++ redistributable to run.
 Hardware:		Need a free USB port to connect USB peripheral device
				programmed with appropriate Generic HID firmware.  VID and
				PID in firmware must match the VID and PID in this
				program.
 Compiler:  	Microsoft Visual C++ 2005 Express Edition (or better)
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
  2.3	08/28/2008	 Initial Release
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
********************************************************************/

//Includes
#include <Windows.h>	//Definitions for various common and not so common types like DWORD, PCHAR, HANDLE, etc.
#include <Dbt.h>		//Need this for definitions of WM_DEVICECHANGE messages
#include <setupapi.h>	//From Windows Server 2003 R2 Platform SDK.  Untested with Windows SDK.  Setupapi.h provides
						//definitions needed for the SetupDixxx() functions, which we use to find our plug and 
						//play device.  If getting build errors when trying to compile this project, make sure the
						//platform SDK is correctly installed, and that it has been integrated into the VC++ development
						//environment.  In other words, make sure the include paths have been setup correctly in this IDE.
						//Microsoft created some small tutorial videos showing how to do this.
						//These may be located at:
						//http://msdn.microsoft.com/en-us/visualc/aa336415.aspx 
						//The link is functional as of 21 May 2008, but could move.  If the above link does not work,
						//try searching for "Video1_PSDK.wmv", which may be associated with:
						//"Using Visual C++ Express Edition and the Platform SDK"

//Modify this value to match the VID and PID in your USB device descriptor (in the USB firmware on the microcontroller).
//Use the formatting: "Vid_xxxx&Pid_xxxx" where xxxx is a 16-bit hexadecimal number.
#define MY_DEVICE_ID  "Vid_04d8&Pid_003f"	
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


namespace HIDPnPDemo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
#pragma region DLL Imports
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
	Note2: In the header files (such as setupapi.h), normally the function names are 
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

	//Returns a HDEVINFO type for a device information set (WinUSB devices in
	//our case).  We will need the HDEVINFO as in input parameter for calling many of
	//the other SetupDixxx() functions.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetClassDevs", CallingConvention=CallingConvention::Winapi)]		
	extern "C" HDEVINFO  SetupDiGetClassDevsUM(
		LPGUID  ClassGuid,					//Input: Supply the class GUID here. 
		PCTSTR  Enumerator,					//Input: Use NULL here, not important for our purposes
		HWND  hwndParent,					//Input: Use NULL here, not important for our purposes
		DWORD  Flags);						//Input: Flags describing what kind of filtering to use.

	//Gives us "PSP_DEVICE_INTERFACE_DATA" which contains the Interface specific GUID (different
	//from class GUID).  We need the interface GUID to get the device path.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiEnumDeviceInterfaces", CallingConvention=CallingConvention::Winapi)]				
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiEnumDeviceInterfacesUM(
		HDEVINFO  DeviceInfoSet,			//Input: Give it the HDEVINFO we got from SetupDiGetClassDevs()
		PSP_DEVINFO_DATA  DeviceInfoData,	//Input (optional)
		LPGUID  InterfaceClassGuid,			//Input 
		DWORD  MemberIndex,					//Input: "Index" of the device you are interested in getting the path for.
		PSP_DEVICE_INTERFACE_DATA  DeviceInterfaceData);//Output: This function fills in an "SP_DEVICE_INTERFACE_DATA" structure.

	//SetupDiDestroyDeviceInfoList() frees up memory by destroying a DeviceInfoList
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiDestroyDeviceInfoList", CallingConvention=CallingConvention::Winapi)]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiDestroyDeviceInfoListUM(			
		HDEVINFO  DeviceInfoSet);			//Input: Give it a handle to a device info list to deallocate from RAM.

	//SetupDiEnumDeviceInfo() fills in an "SP_DEVINFO_DATA" structure, which we need for SetupDiGetDeviceRegistryProperty()
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiEnumDeviceInfo", CallingConvention=CallingConvention::Winapi)]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiEnumDeviceInfoUM(
		HDEVINFO  DeviceInfoSet,
		DWORD  MemberIndex,
		PSP_DEVINFO_DATA  DeviceInfoData);

	//SetupDiGetDeviceRegistryProperty() gives us the hardware ID, which we use to check to see if it has matching VID/PID
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetDeviceRegistryProperty", CallingConvention=CallingConvention::Winapi)]
	extern "C"	WINSETUPAPI BOOL WINAPI  SetupDiGetDeviceRegistryPropertyUM(
		HDEVINFO  DeviceInfoSet,
		PSP_DEVINFO_DATA  DeviceInfoData,
		DWORD  Property,
		PDWORD  PropertyRegDataType,
		PBYTE  PropertyBuffer,   
		DWORD  PropertyBufferSize,  
		PDWORD  RequiredSize);

	//SetupDiGetDeviceInterfaceDetail() gives us a device path, which is needed before CreateFile() can be used.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetDeviceInterfaceDetail", CallingConvention=CallingConvention::Winapi)]
	extern "C" BOOL SetupDiGetDeviceInterfaceDetailUM(
		HDEVINFO DeviceInfoSet,										//Input: Wants HDEVINFO which can be obtained from SetupDiGetClassDevs()
		PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData,				//Input: Pointer to an structure which defines the device interface.  
		PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData,	//Output: Pointer to a strucutre, which will contain the device path.
		DWORD DeviceInterfaceDetailDataSize,						//Input: Number of bytes to retrieve.
		PDWORD RequiredSize,										//Output (optional): Te number of bytes needed to hold the entire struct 
		PSP_DEVINFO_DATA DeviceInfoData);							//Output

	//Need this function for receiving all of the WM_DEVICECHANGE messages.  See MSDN documentation for
	//description of what this function does/how to use it. Note: name is remapped "RegisterDeviceNotificationUM" to
	//avoid possible build error conflicts.
	[DllImport("user32.dll" , CharSet = CharSet::Seeifdef, EntryPoint="RegisterDeviceNotification", CallingConvention=CallingConvention::Winapi)]					
	extern "C" HDEVNOTIFY WINAPI RegisterDeviceNotificationUM(
		HANDLE hRecipient,
		LPVOID NotificationFilter,
		DWORD Flags);

#pragma endregion

#pragma region Global Variables
	/*** This section is all of the global variables related to this namespace ***/

	//Globally Unique Identifier (GUID) for HID class devices.  Windows uses GUIDs to identify things.
	GUID InterfaceClassGuid = {0x4d1e55b2, 0xf16f, 0x11cf, 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30}; 

	//USB related variables that need to have wide scope.
	BOOL AttachedState = FALSE;						//Need to keep track of the USB device attachment status for proper plug and play operation.
	BOOL AttachedButBroken = FALSE;					
	PSP_DEVICE_INTERFACE_DETAIL_DATA DetailedInterfaceDataStructure = new SP_DEVICE_INTERFACE_DETAIL_DATA;	//Global
	HANDLE WriteHandleToUSBDevice = INVALID_HANDLE_VALUE;
	HANDLE ReadHandleToUSBDevice = INVALID_HANDLE_VALUE;

	//Variables used by the application/form updates.
	BOOL PushbuttonPressed = FALSE;		//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
	BOOL ToggleLEDsPending = FALSE;		//Updated by ToggleLED(s) button click event handler, used by ReadWriteThread (needs to be atomic)
	unsigned int ADCValue = 0;			//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)

#pragma endregion
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------




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
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
			//Additional constructor code

			//Initialize tool tips, to provide pop up help when the mouse cursor is moved over objects on the form.
			ANxVoltageToolTip->SetToolTip(this->ANxVoltage_lbl, "If using a board/PIM without a potentiometer, apply an adjustable voltage to the I/O pin.");
			ANxVoltageToolTip->SetToolTip(this->progressBar1, "If using a board/PIM without a potentiometer, apply an adjustable voltage to the I/O pin.");
			ToggleLEDToolTip->SetToolTip(this->ToggleLEDs_btn, "Sends a packet of data to the USB device.");
			PushbuttonStateTooltip->SetToolTip(this->PushbuttonState_lbl, "Try pressing pushbuttons on the USB demo board/PIM.");

			//Register for WM_DEVICECHANGE notifications.  This code uses these messages to detect plug and play connection/disconnection events for USB devices
			DEV_BROADCAST_DEVICEINTERFACE MyDeviceBroadcastHeader;// = new DEV_BROADCAST_HDR;
			MyDeviceBroadcastHeader.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
			MyDeviceBroadcastHeader.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
			MyDeviceBroadcastHeader.dbcc_reserved = 0;	//Reserved says not to use...
			MyDeviceBroadcastHeader.dbcc_classguid = InterfaceClassGuid;
			RegisterDeviceNotificationUM((HANDLE)this->Handle, &MyDeviceBroadcastHeader, DEVICE_NOTIFY_WINDOW_HANDLE);

			//Now make an initial attempt to find the USB device, if it was already connected to the PC and enumerated prior to launching the application.
			//If it is connected and present, we should open read and write handles to the device so we can communicate with it later.
			//If it was not connected, we will have to wait until the user plugs the device in, and the WM_DEVICECHANGE callback function can process
			//the message and again search for the device.
			if(CheckIfPresentAndGetUSBDevicePath())	//Check and make sure at least one device with matching VID/PID is attached
			{
				DWORD ErrorStatusWrite;
				DWORD ErrorStatusRead;

				//We now have the proper device path, and we can finally open read and write handles to the device.
				WriteHandleToUSBDevice = CreateFile(DetailedInterfaceDataStructure->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
				ErrorStatusWrite = GetLastError();
				ReadHandleToUSBDevice = CreateFile(DetailedInterfaceDataStructure->DevicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
				ErrorStatusRead = GetLastError();

				if((ErrorStatusWrite == ERROR_SUCCESS) && (ErrorStatusRead == ERROR_SUCCESS))
				{
					AttachedState = TRUE;		//Let the rest of the PC application know the USB device is connected, and it is safe to read/write to it
					AttachedButBroken = FALSE;
				}
				else //for some reason the device was physically plugged in, but one or both of the read/write handles didn't open successfully...
				{
					AttachedState = FALSE;		//Let the rest of this application known not to read/write to the device.
					AttachedButBroken = TRUE;	//Flag so that next time a WM_DEVICECHANGE message occurs, can retry to re-open read/write pipes
					if(ErrorStatusWrite == ERROR_SUCCESS)
						CloseHandle(WriteHandleToUSBDevice);
					if(ErrorStatusRead == ERROR_SUCCESS)
						CloseHandle(ReadHandleToUSBDevice);
				}
			}
			else	//Device must not be connected (or not programmed with correct firmware)
			{
				AttachedState = FALSE;
				AttachedButBroken = FALSE;
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
			//Close the read/write handles, if they are currently open.
			if(AttachedState == TRUE)
			{
				CloseHandle(WriteHandleToUSBDevice);
				CloseHandle(ReadHandleToUSBDevice);
			}
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  ANxVoltage_lbl;
	private: System::Windows::Forms::Label^  StatusBox_lbl;
	protected: 

	protected: 

	private: System::Windows::Forms::TextBox^  StatusBox_txtbx;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::ComponentModel::BackgroundWorker^  ReadWriteThread;
	private: System::Windows::Forms::Timer^  FormUpdateTimer;

	private: System::Windows::Forms::Button^  ToggleLEDs_btn;
	private: System::Windows::Forms::Label^  PushbuttonState_lbl;
	private: System::Windows::Forms::ToolTip^  ANxVoltageToolTip;
private: System::Windows::Forms::ToolTip^  ToggleLEDToolTip;
private: System::Windows::Forms::ToolTip^  PushbuttonStateTooltip;




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
			this->ANxVoltage_lbl = (gcnew System::Windows::Forms::Label());
			this->StatusBox_lbl = (gcnew System::Windows::Forms::Label());
			this->StatusBox_txtbx = (gcnew System::Windows::Forms::TextBox());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->ReadWriteThread = (gcnew System::ComponentModel::BackgroundWorker());
			this->FormUpdateTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->ToggleLEDs_btn = (gcnew System::Windows::Forms::Button());
			this->PushbuttonState_lbl = (gcnew System::Windows::Forms::Label());
			this->ANxVoltageToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->ToggleLEDToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->PushbuttonStateTooltip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->SuspendLayout();
			// 
			// ANxVoltage_lbl
			// 
			this->ANxVoltage_lbl->AutoSize = true;
			this->ANxVoltage_lbl->Enabled = false;
			this->ANxVoltage_lbl->Location = System::Drawing::Point(122, 105);
			this->ANxVoltage_lbl->Name = L"ANxVoltage_lbl";
			this->ANxVoltage_lbl->Size = System::Drawing::Size(93, 13);
			this->ANxVoltage_lbl->TabIndex = 17;
			this->ANxVoltage_lbl->Text = L"ANx/POT Voltage";
			// 
			// StatusBox_lbl
			// 
			this->StatusBox_lbl->AutoSize = true;
			this->StatusBox_lbl->Location = System::Drawing::Point(316, 15);
			this->StatusBox_lbl->Name = L"StatusBox_lbl";
			this->StatusBox_lbl->Size = System::Drawing::Size(37, 13);
			this->StatusBox_lbl->TabIndex = 16;
			this->StatusBox_lbl->Text = L"Status";
			// 
			// StatusBox_txtbx
			// 
			this->StatusBox_txtbx->BackColor = System::Drawing::SystemColors::Window;
			this->StatusBox_txtbx->Location = System::Drawing::Point(12, 12);
			this->StatusBox_txtbx->Name = L"StatusBox_txtbx";
			this->StatusBox_txtbx->ReadOnly = true;
			this->StatusBox_txtbx->Size = System::Drawing::Size(298, 20);
			this->StatusBox_txtbx->TabIndex = 15;
			// 
			// progressBar1
			// 
			this->progressBar1->BackColor = System::Drawing::Color::White;
			this->progressBar1->ForeColor = System::Drawing::Color::Red;
			this->progressBar1->Location = System::Drawing::Point(12, 121);
			this->progressBar1->Maximum = 1024;
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(298, 23);
			this->progressBar1->Step = 1;
			this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBar1->TabIndex = 14;
			// 
			// ReadWriteThread
			// 
			this->ReadWriteThread->WorkerReportsProgress = true;
			this->ReadWriteThread->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::ReadWriteThread_DoWork);
			// 
			// FormUpdateTimer
			// 
			this->FormUpdateTimer->Enabled = true;
			this->FormUpdateTimer->Interval = 6;
			this->FormUpdateTimer->Tick += gcnew System::EventHandler(this, &Form1::FormUpdateTimer_Tick);
			// 
			// ToggleLEDs_btn
			// 
			this->ToggleLEDs_btn->Enabled = false;
			this->ToggleLEDs_btn->Location = System::Drawing::Point(12, 60);
			this->ToggleLEDs_btn->Name = L"ToggleLEDs_btn";
			this->ToggleLEDs_btn->Size = System::Drawing::Size(96, 23);
			this->ToggleLEDs_btn->TabIndex = 18;
			this->ToggleLEDs_btn->Text = L"ToggleLED(s)";
			this->ToggleLEDs_btn->UseVisualStyleBackColor = true;
			this->ToggleLEDs_btn->Click += gcnew System::EventHandler(this, &Form1::ToggleLEDs_btn_Click);
			// 
			// PushbuttonState_lbl
			// 
			this->PushbuttonState_lbl->AutoSize = true;
			this->PushbuttonState_lbl->Enabled = false;
			this->PushbuttonState_lbl->Location = System::Drawing::Point(131, 65);
			this->PushbuttonState_lbl->Name = L"PushbuttonState_lbl";
			this->PushbuttonState_lbl->Size = System::Drawing::Size(141, 13);
			this->PushbuttonState_lbl->TabIndex = 19;
			this->PushbuttonState_lbl->Text = L"Pushbutton State: Unknown";
			// 
			// ANxVoltageToolTip
			// 
			this->ANxVoltageToolTip->AutomaticDelay = 20;
			this->ANxVoltageToolTip->AutoPopDelay = 20000;
			this->ANxVoltageToolTip->InitialDelay = 15;
			this->ANxVoltageToolTip->ReshowDelay = 15;
			// 
			// ToggleLEDToolTip
			// 
			this->ToggleLEDToolTip->AutomaticDelay = 2000;
			this->ToggleLEDToolTip->AutoPopDelay = 20000;
			this->ToggleLEDToolTip->InitialDelay = 15;
			this->ToggleLEDToolTip->ReshowDelay = 15;
			// 
			// PushbuttonStateTooltip
			// 
			this->PushbuttonStateTooltip->AutomaticDelay = 20;
			this->PushbuttonStateTooltip->AutoPopDelay = 20000;
			this->PushbuttonStateTooltip->InitialDelay = 15;
			this->PushbuttonStateTooltip->ReshowDelay = 15;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(356, 158);
			this->Controls->Add(this->PushbuttonState_lbl);
			this->Controls->Add(this->ToggleLEDs_btn);
			this->Controls->Add(this->ANxVoltage_lbl);
			this->Controls->Add(this->StatusBox_lbl);
			this->Controls->Add(this->StatusBox_txtbx);
			this->Controls->Add(this->progressBar1);
			this->Name = L"Form1";
			this->Text = L"Generic HID PnP Demo";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------

//FUNCTION:	CheckIfPresentAndGetUSBDevicePath()
//PURPOSE:	Check if a USB device is currently plugged in with a matching VID and PID
//INPUT:	Uses globally declared "DetailedInterfaceDataStructure" structure, globally declared GUID, and the MY_DEVICE_ID constant.
//OUTPUT:	Returns BOOL.  TRUE when device with matching VID/PID found.  FALSE if device with VID/PID could not be found.
//			When returns TRUE, the globally accessable "DetailedInterfaceDataStructure" will contain the device path
//			to the USB device with the matching VID/PID.

BOOL	CheckIfPresentAndGetUSBDevicePath(void)
{
		/* 
		Before we can "connect" our application to our USB embedded device, we must first find the device.
		A USB bus can have many devices simultaneously connected, so somehow we have to find our device only.
		This is done with the Vendor ID (VID) and Product ID (PID).  Each USB product line should have
		a unique combination of VID and PID.  

		Microsoft has created a number of functions which are useful for finding plug and play devices.  Documentation
		for each function used can be found in the MSDN library.  We will be using the following functions:

		SetupDiGetClassDevs()					//provided by setupapi.dll, which comes with Windows
		SetupDiEnumDeviceInterfaces()			//provided by setupapi.dll, which comes with Windows
		GetLastError()							//provided by kernel32.dll, which comes with Windows
		SetupDiDestroyDeviceInfoList()			//provided by setupapi.dll, which comes with Windows
		SetupDiGetDeviceInterfaceDetail()		//provided by setupapi.dll, which comes with Windows
		SetupDiGetDeviceRegistryProperty()		//provided by setupapi.dll, which comes with Windows
		malloc()								//part of C runtime library, msvcrt.dll?
		CreateFile()							//provided by kernel32.dll, which comes with Windows

		We will also be using the following unusual data types and structures.  Documentation can also be found in
		the MSDN library:

		PSP_DEVICE_INTERFACE_DATA
		PSP_DEVICE_INTERFACE_DETAIL_DATA
		SP_DEVINFO_DATA
		HDEVINFO
		HANDLE
		GUID

		The ultimate objective of the following code is to get the device path, which will be used elsewhere for getting
		read and write handles to the USB device.  Once the read/write handles are opened, only then can this
		PC application begin reading/writing to the USB device using the WriteFile() and ReadFile() functions.

		Getting the device path is a multi-step round about process, which requires calling several of the
		SetupDixxx() functions provided by setupapi.dll.
		*/

		HDEVINFO DeviceInfoTable = INVALID_HANDLE_VALUE;
		PSP_DEVICE_INTERFACE_DATA InterfaceDataStructure = new SP_DEVICE_INTERFACE_DATA;
//		PSP_DEVICE_INTERFACE_DETAIL_DATA DetailedInterfaceDataStructure = new SP_DEVICE_INTERFACE_DETAIL_DATA;	//Globally declared instead
		SP_DEVINFO_DATA DevInfoData;

		DWORD InterfaceIndex = 0;
		DWORD StatusLastError = 0;
		DWORD dwRegType;
		DWORD dwRegSize;
		DWORD StructureSize = 0;
		PBYTE PropertyValueBuffer;
		bool MatchFound = false;
		DWORD ErrorStatus;
		BOOL BoolStatus = FALSE;
		DWORD LoopCounter = 0;

		String^ DeviceIDToFind = MY_DEVICE_ID;

		//First populate a list of plugged in devices (by specifying "DIGCF_PRESENT"), which are of the specified class GUID. 
		DeviceInfoTable = SetupDiGetClassDevsUM(&InterfaceClassGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

		//Now look through the list we just populated.  We are trying to see if any of them match our device. 
		while(true)
		{
			InterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
			if(SetupDiEnumDeviceInterfacesUM(DeviceInfoTable, NULL, &InterfaceClassGuid, InterfaceIndex, InterfaceDataStructure))
			{
				ErrorStatus = GetLastError();
				if(ErrorStatus == ERROR_NO_MORE_ITEMS)	//Did we reach the end of the list of matching devices in the DeviceInfoTable?
				{	//Cound not find the device.  Must not have been attached.
					SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
					return FALSE;		
				}
			}
			else	//Else some other kind of unknown error ocurred...
			{
				ErrorStatus = GetLastError();
				SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
				return FALSE;	
			}

			//Now retrieve the hardware ID from the registry.  The hardware ID contains the VID and PID, which we will then 
			//check to see if it is the correct device or not.

			//Initialize an appropriate SP_DEVINFO_DATA structure.  We need this structure for SetupDiGetDeviceRegistryProperty().
			DevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
			SetupDiEnumDeviceInfoUM(DeviceInfoTable, InterfaceIndex, &DevInfoData);

			//First query for the size of the hardware ID, so we can know how big a buffer to allocate for the data.
			SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, NULL, 0, &dwRegSize);

			//Allocate a buffer for the hardware ID.
			PropertyValueBuffer = (BYTE *) malloc (dwRegSize);
			if(PropertyValueBuffer == NULL)	//if null, error, couldn't allocate enough memory
			{	//Can't really recover from this situation, just exit instead.
				SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
				return FALSE;		
			}

			//Retrieve the hardware IDs for the current device we are looking at.  PropertyValueBuffer gets filled with a 
			//REG_MULTI_SZ (array of null terminated strings).  To find a device, we only care about the very first string in the
			//buffer, which will be the "device ID".  The device ID is a string which contains the VID and PID, in the example 
			//format "Vid_04d8&Pid_003f".
			SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, PropertyValueBuffer, dwRegSize, NULL);

			//Now check if the first string in the hardware ID matches the device ID of my USB device.
			#ifdef UNICODE
			String^ DeviceIDFromRegistry = gcnew String((wchar_t *)PropertyValueBuffer);
			#else
			String^ DeviceIDFromRegistry = gcnew String((char *)PropertyValueBuffer);
			#endif

			free(PropertyValueBuffer);		//No longer need the PropertyValueBuffer, free the memory to prevent potential memory leaks

			//Convert both strings to lower case.  This makes the code more robust/portable accross OS Versions
			DeviceIDFromRegistry = DeviceIDFromRegistry->ToLowerInvariant();	
			DeviceIDToFind = DeviceIDToFind->ToLowerInvariant();				
			//Now check if the hardware ID we are looking at contains the correct VID/PID
			MatchFound = DeviceIDFromRegistry->Contains(DeviceIDToFind);		
			if(MatchFound == true)
			{
				//Device must have been found.  Open WinUSB interface handle now.  In order to do this, we will need the actual device path first.
				//We can get the path by calling SetupDiGetDeviceInterfaceDetail(), however, we have to call this function twice:  The first
				//time to get the size of the required structure/buffer to hold the detailed interface data, then a second time to actually 
				//get the structure (after we have allocated enough memory for the structure.)
				DetailedInterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
				//First call populates "StructureSize" with the correct value
				SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, NULL, NULL, &StructureSize, NULL);	
				DetailedInterfaceDataStructure = (PSP_DEVICE_INTERFACE_DETAIL_DATA)(malloc(StructureSize));		//Allocate enough memory
				if(DetailedInterfaceDataStructure == NULL)	//if null, error, couldn't allocate enough memory
				{	//Can't really recover from this situation, just exit instead.
					SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
					return FALSE;		
				}
				DetailedInterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
				 //Now call SetupDiGetDeviceInterfaceDetail() a second time to receive the goods.  
				SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, DetailedInterfaceDataStructure, StructureSize, NULL, NULL); 

				//We now have the proper device path, and we can finally open a device handle to the device.
				//WinUSB requires the device handle to be opened with the FILE_FLAG_OVERLAPPED attribute.
				SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
				return TRUE;
			}

			InterfaceIndex++;	
			//Keep looping until we either find a device with matching VID and PID, or until we run out of devices to check.
			//However, just in case some unexpected error occurs, keep track of the number of loops executed.
			//If the number of loops exceeds a very large number, exit anyway, to prevent inadvertent infinite looping.
			LoopCounter++;
			if(LoopCounter == 10000000)	//Surely there aren't more than 10 million devices attached to any forseeable PC...
			{
				return FALSE;
			}
		}//end of while(true)
}
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------

//This is a callback function that gets called when a Windows message is received by the form.
//We will receive various different types of messages, but the ones we really want to use are the WM_DEVICECHANGE messages.
protected: virtual void WndProc( Message% m ) override{
		 if(m.Msg == WM_DEVICECHANGE)
		 {
			 if(((int)m.WParam == DBT_DEVICEARRIVAL) || ((int)m.WParam == DBT_DEVICEREMOVEPENDING) || ((int)m.WParam == DBT_DEVICEREMOVECOMPLETE) || ((int)m.WParam == DBT_CONFIGCHANGED) )
			 {
				//WM_DEVICECHANGE messages by themselves are quite generic, and can be caused by a number of different
				//sources, not just your USB hardware device.  Therefore, must check to find out if any changes relavant
				//to your device (with known VID/PID) took place before doing any kind of opening or closing of handles/endpoints.
				//(the message could have been totally unrelated to your application/USB device)

				if(CheckIfPresentAndGetUSBDevicePath())	//Check and make sure at least one device with matching VID/PID is attached
				{
					//If executes to here, this means the device is currently attached and was found.
					//This code needs to decide however what to do, based on whether or not the device was previously known to be
					//attached or not.
					if((AttachedState == FALSE) || (AttachedButBroken == TRUE))	//Check the previous attachment state
					{
						DWORD ErrorStatusWrite;
						DWORD ErrorStatusRead;

						//We obtained the proper device path (from CheckIfPresentAndGetUSBDevicePath() function call), and it
						//is now possible to open read and write handles to the device.
						WriteHandleToUSBDevice = CreateFile(DetailedInterfaceDataStructure->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
						ErrorStatusWrite = GetLastError();
						ReadHandleToUSBDevice = CreateFile(DetailedInterfaceDataStructure->DevicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
						ErrorStatusRead = GetLastError();

						if((ErrorStatusWrite == ERROR_SUCCESS) && (ErrorStatusRead == ERROR_SUCCESS))
						{
							AttachedState = TRUE;
							AttachedButBroken = FALSE;
						}
						else //For some reason one or both of the read/write pipes couldn't be opened, even though the device was physically attached.  This could 
						{	 //happen if some other application already opened handles to the device, but did not allow read or write sharing.
							AttachedState = FALSE;		//Let the rest of this application know not to try reading/writing to the USB device
							AttachedButBroken = TRUE;	//Let this WM_DEVICECHANGE handler know to retry to open handles the next time a WM_DEVICECHANGE message occurs.
							if(ErrorStatusWrite == ERROR_SUCCESS)
								CloseHandle(WriteHandleToUSBDevice);
							if(ErrorStatusRead == ERROR_SUCCESS)
								CloseHandle(ReadHandleToUSBDevice);
						}
					}
					//else we did find the device, but AttachedState was already TRUE.  In this case, don't do anything to the read/write handles,
					//since the WM_DEVICECHANGE message presumably wasn't caused by our USB device.  
				}
				else	//Device must not be connected (or not programmed with correct firmware)
				{
					if(AttachedState == TRUE)		//If it is currently set to TRUE, that means the device was just now disconnected
					{
						AttachedState = FALSE;
						CloseHandle(WriteHandleToUSBDevice);
						CloseHandle(ReadHandleToUSBDevice);
					}
					AttachedState = FALSE;
					AttachedButBroken = FALSE;
				}				 
			 }
		 } //end of: if(m.Msg == WM_DEVICECHANGE)

		 Form::WndProc( m );
	} //end of: WndProc() function
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------







	private: System::Void FormUpdateTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
	//This timer tick event handler function is used to update the user interface on the form, based on data
	//obtained asynchronously by the ReadWriteThread and the WM_DEVICECHANGE event handler functions.

	//Check if user interface on the form should be enabled or not, based on the attachment state of the USB device.
	if(AttachedState == TRUE)
	{
		//Device is connected and ready to communicate, enable user interface on the form 
		StatusBox_txtbx->Text = "Device Found: AttachedState = TRUE";
		PushbuttonState_lbl->Enabled = true;	//Make the label no longer greyed out
		ANxVoltage_lbl->Enabled = true;
		ToggleLEDs_btn->Enabled = true;
	}
	if((AttachedState == FALSE) || (AttachedButBroken == TRUE))
	{
		//Device not available to communicate. Disable user interface on the form.
		StatusBox_txtbx->Text = "Device Not Detected: Verify Connection/Correct Firmware";
		PushbuttonState_lbl->Enabled = false;	//Make the label no longer greyed out
		ANxVoltage_lbl->Enabled = false;
		ToggleLEDs_btn->Enabled = false;

		PushbuttonState_lbl->Text = "Pushbutton State: Unknown";
		ADCValue = 0;
		progressBar1->Value = 0;
	}

	//Update the various status indicators on the form with the latest info obtained from the ReadWriteThread()
	if(AttachedState == TRUE)
	{
		//Update the pushbutton state label.
		if(PushbuttonPressed == FALSE)	
			PushbuttonState_lbl->Text = "Pushbutton State: Not Pressed";		//Update the pushbutton state text label on the form, so the user can see the result 
		else
			PushbuttonState_lbl->Text = "Pushbutton State: Pressed";			//Update the pushbutton state text label on the form, so the user can see the result 
		
		//Update the ANxx/POT Voltage indicator value (progressbar)
		progressBar1->Value = ADCValue;
	}
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
			 }

private: System::Void ReadWriteThread_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------

		/*This thread does the actual USB read/write operations (but only when AttachedState == TRUE) to the USB device.
		It is generally preferrable to write applications so that read and write operations are handled in a separate
		thread from the main form.  This makes it so that the main form can remain responsive, even if the I/O operations
		take a very long time to complete.

		Since this is a separate thread, this code below executes independently from the rest of the
		code in this application.  All this thread does is read and write to the USB device.  It does not update
		the form directly with the new information it obtains (such as the ANxx/POT Voltage or pushbutton state).
		The information that this thread obtains is stored in atomic global variables.
		Form updates are handled by the FormUpdateTimer Tick event handler function.

		This application sends packets to the endpoint buffer on the USB device by using the "WriteFile()" function.
		This application receives packets from the endpoint buffer on the USB device by using the "ReadFile()" function.
		Both of these functions are documented in the MSDN library.

		All ReadFile() and WriteFile() operations in this example project are synchronous.  They are blocking function
		calls and only return when they are complete, or if they fail because of some event, such as the user unplugging
		the device.  It is possible to call these functions with "overlapped" structures, and use them as non-blocking
		asynchronous I/O function calls.  

		Note:  This code may perform differently on some machines when the USB device is plugged into the host through a
		USB 2.0 hub, as opposed to a direct connection to a root port on the PC.  In some cases the data rate may be slower
		when the device is connected through a USB 2.0 hub.  This performance difference is believed to be caused by
		the issue described in Microsoft knowledge base article 940021:
		http://support.microsoft.com/kb/940021/en-us 

		Higher effective bandwidth (up to the maximum offered by interrupt endpoints), both when connected
		directly and through a USB 2.0 hub, can generally be achieved by queuing up multiple pending read and/or
		write requests simultaneously.  This can be done when using	asynchronous I/O operations (calling ReadFile() and
		WriteFile()	with overlapped structures).  The Microchip	HID USB Bootloader application uses asynchronous I/O
		for some USB operations and the source code can be used	as an example.*/


		unsigned char OUTBuffer[65];	//Allocate a memory buffer equal to the OUT endpoint size + 1
		unsigned char INBuffer[65];		//Allocate a memory buffer equal to the IN endpoint size + 1
		DWORD BytesWritten = 0;
		DWORD BytesRead = 0;

		while(true)
		{
			if(AttachedState == TRUE)	//Do not try to use the read/write handles unless the USB device is attached and ready
			{
				//Get ANxx/POT Voltage value from the microcontroller firmware.  Note: some demo boards may not have a pot
				//on them.  In this case, the firmware may be configured to read an ANxx I/O pin voltage with the ADC
				//instead.  If this is the case, apply a proper voltage to the pin.  See the firmware for exact implementation.
				OUTBuffer[0] = 0;			//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
				OUTBuffer[1] = 0x37;	//READ_POT command (see the firmware source code), gets 10-bit ADC Value
				//Initialize the rest of the 64-byte packet to "0xFF".  Binary '1' bits do not use as much power, and do not cause as much EMI
				//when they move across the USB cable.  USB traffic is "NRZI" encoded, where '1' bits do not cause the D+/D- signals to toggle states.
				//This initialization is not strictly necessary however.
				for(unsigned char i = 2; i <65; i++)
					OUTBuffer[i] = 0xFF;

				//To get the ADCValue, first, we send a packet with our "READ_POT" command in it.
				if(WriteFile(WriteHandleToUSBDevice, &OUTBuffer, 65, &BytesWritten, 0))	//Blocking function, unless an "overlapped" structure is used
				{
					INBuffer[0] = 0;
					//Now get the response packet from the firmware.
					if(ReadFile(ReadHandleToUSBDevice, &INBuffer, 65, &BytesRead, 0))		//Blocking function, unless an "overlapped" structure is used	
					{
						//INBuffer[0] is the report ID, which we don't care about.
						//INBuffer[1] is an echo back of the command (see microcontroller firmware).
						//INBuffer[2] and INBuffer[3] contains the ADC value (see microcontroller firmware).  
						if(INBuffer[1] == 0x37)
						{
							ADCValue = (INBuffer[3] << 8) + INBuffer[2];	//Need to reformat the data from two unsigned chars into one unsigned int.
						}
					}
				}



				//Get the pushbutton state from the microcontroller firmware.
				OUTBuffer[0] = 0;			//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
				OUTBuffer[1] = 0x81;		//0x81 is the "Get Pushbutton State" command in the firmware
				for(unsigned char i = 2; i <65; i++)	//This loop is not strictly necessary.  Simply initializes unused bytes to
					OUTBuffer[i] = 0xFF;				//0xFF for lower EMI and power consumption when driving the USB cable.

				//To get the pushbutton state, first, we send a packet with our "Get Pushbutton State" command in it.
				if(WriteFile(WriteHandleToUSBDevice, &OUTBuffer, 65, &BytesWritten, 0))	//Blocking function, unless an "overlapped" structure is used
				{
					//Now get the response packet from the firmware.
					INBuffer[0] = 0;
					if(ReadFile(ReadHandleToUSBDevice, &INBuffer, 65, &BytesRead, 0))	//Blocking function, unless an "overlapped" structure is used	
					{
						//INBuffer[0] is the report ID, which we don't care about.
						//INBuffer[1] is an echo back of the command (see microcontroller firmware).
						//INBuffer[2] contains the I/O port pin value for the pushbutton (see microcontroller firmware).  
						if((INBuffer[1] == 0x81) && (INBuffer[2] == 0x01))	
						{
							PushbuttonPressed = FALSE;
						}
						if((INBuffer[1] == 0x81) && (INBuffer[2] == 0x00))	
						{
							PushbuttonPressed = TRUE;
						}
					}
				}



				//Check if this thread should send a Toggle LED(s) command to the firmware.  ToggleLEDsPending will get set
				//by the ToggleLEDs_btn click event handler function if the user presses the button on the form.
				if(ToggleLEDsPending == TRUE)
				{
					OUTBuffer[0] = 0;					//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
					OUTBuffer[1] = 0x80;			//0x80 is the "Toggle LED(s)" command in the firmware
					for(unsigned char i = 2; i <65; i++)	//This loop is not strictly necessary.  Simply initializes unused bytes to
						OUTBuffer[i] = 0xFF;				//0xFF for lower EMI and power consumption when driving the USB cable.
					//Now send the packet to the USB firmware on the microcontroller
					WriteFile(WriteHandleToUSBDevice, &OUTBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
					ToggleLEDsPending = FALSE;
				}
			} //end of: if(AttachedState == TRUE)
			else
			{
				Sleep(5);	//Add a small delay.  Otherwise, this while(true) loop can execute very fast and cause 
							//high CPU utilization, with no particular benefit to the application.
			}

		} //end of while(true) loop
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		 }

private: System::Void ToggleLEDs_btn_Click(System::Object^  sender, System::EventArgs^  e) {
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
			ToggleLEDsPending = TRUE;	//Will get used asynchronously by the ReadWriteThread
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		 }
};
}

