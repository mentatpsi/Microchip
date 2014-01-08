#pragma once
/********************************************************************
 FileName:		Form1.h
 Dependencies:	Windows SDK during development.  
				Also need WDK 6001.18002 (see includes section for 
				additional details).  When compiled, needs .NET 
				framework 3.5 redistributable to run.  
 Hardware:		Need a free USB port to connect USB peripheral device
				programming with appropriate WinUSB firmware.  VID and
				PID in firmware must match the VID and PID in this
				program.
 Compiler:  	Microsoft Visual C++ 2008 Express Edition (or better)
 Company:		Microchip Technology, Inc.

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


********************************************************************
NOTE:	All user made code contained in this project is in the Form1.h file.
		All other code and files were generated automatically by either the
		new project wizard, or by the development environment (ex: code is
		automatically generated if you create a new button on the form, and
		then double click on it, which creates a click event handler
		function).  
********************************************************************/


//Includes
//-----------------------------------------------------------------------------
#include <Windows.h>	//Definitions for various common and not so common types like DWORD, PCHAR, HANDLE, etc.
#include <setupapi.h>	//From Windows SDK.  Setupapi.h provides definitions needed for the SetupDixxx() 
						//functions, which we use to find our plug and play device.  
						//If getting build errors when trying to compile this project, make sure the
						//Windows SDK is correctly installed, and that it has been correctly integrated into 
						//the VC++ development environment.  
						//Also see the below comments on how to add directories to the include path.

						//IMPORTANT: READ THIS BEFORE TRYING TO BUILD THIS CODE
						//-----------------------------------------------------
#include <Winusb.h>		//Winusb.h comes as a part of the Windows Driver Kit (WDK) build 6001.18002 (and later versions).
						//The WDK is currently a free download from http://msdn.microsoft.com/en-us/library/windows/hardware/gg487428.aspx
						//You will need the WDK build 6001.18002 (or later) installed on
						//your computer to use this source code.  Once you have it installed, you will also need to add
						//the include path directories from the WDK to your VC++ IDE.  This can be done by clicking
						//Tools-->Options-->+Projects and Solutions-->VC++ Directories-->Show Directories for: "Include files"
						//Then click the Folder icon (new line) and then the "..." button and add these directories (to the 
						//bottom of the list!  The order of the list is important, and the project will not build correctly
						//if the order is wrong.):
						//C:\WINDDK\7600.16385.1\inc\ddk		
						//C:\WINDDK\7600.16385.1\inc\api		
						//The above directory locations assume the default location for the WDK, build 7600.16385.1.
						//Change the directory if installed in a different location, or if a different version of the WDK
						//was used.
						//If the above procedure is not followed correctly, a variety of build errors looking for various
						//files such as winusbio.h, usb200.h, usb100.h, usb.h, etc. will occur.

						//Special note for Microsoft Visual C++ 2010: The "Tools-->Options-->+Projects and Solutions-->VC++ Directories"
						//menu area is deprecated in MS VC++ 2010.  The included paths can instead be added to the project by
						//Clicking: Project-->[project name] Properties...-->+Configuration Properties-->VC++ Directories.			
							//NOTE: if the "Project" menu only shows "Properties", this means you have a file (ex: Form1.h) selected 
							//in the solution explorer, and you are trying to edit the properties specific to that file.  Therefore, 
							//make sure you deselect the file before clicking "Project" (you can do this by clicking somewhere else, 
							//ex: in this editor text area).  Only then will the Project --> "[project name] Properties..." option 
							//appear in the menu.
						//In the "Configuration" drop down box, select "All Configurations".
						//Then in the "Include Directories" entry, add the [wdk install dir]\inc\ddk and [wdk install dir]\inc\api path 
						//entries at the END OF THE EXISTING LIST (must be the end of the list, the order is important).
						//If using the WDK 7600, the Include Directories list would look something (or maybe exactly) like this:
						//$(VCInstallDir)include;$(VCInstallDir)atlmfc\include;$(WindowsSdkDir)include;$(FrameworkSDKDir)\include;C:\WinDDK\7600.16385.1\inc\ddk;C:\WinDDK\7600.16385.1\inc\api						


#include <Dbt.h>		//Need this for definitions of WM_DEVICECHANGE messages


//Modify this value to match the VID and PID in your USB device descriptor.
//Use the formatting: "Vid_xxxx&Pid_xxxx" where xxxx is a 16-bit hexadecimal number.
#define MY_DEVICE_ID  "Vid_04d8&Pid_0053"	//Change this number (along with the corresponding VID/PID in the 
											//microcontroller firmware, and in the driver installation .INF 
											//file) before moving the design into production.


namespace WinUSBDemo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;	
	using namespace System::Runtime::InteropServices;  //Need this to support "unmanaged" code.

	#pragma region DLL Imports
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

	//WinUsb_Initialize() needs to be called before the application can begin sending/receiving data with the USB device.
	[DllImport("winusb.dll" , CharSet = CharSet::Seeifdef, EntryPoint="WinUsb_Initialize", CallingConvention=CallingConvention::Winapi)]
	extern "C" BOOL WinUsb_Initialize(
		HANDLE	DeviceHandle,
		PWINUSB_INTERFACE_HANDLE InterfaceHandle);

	//WinUsb_WritePipe() is the basic function used to write data to the USB device (sends data to OUT endpoints on the device)
	[DllImport("winusb.dll" , CharSet = CharSet::Seeifdef, EntryPoint="WinUsb_WritePipe", CallingConvention=CallingConvention::Winapi)]
	extern "C" BOOL WinUsb_WritePipe(
		WINUSB_INTERFACE_HANDLE InterfaceHandle,
		UCHAR PipeID,
		PUCHAR Buffer,
		ULONG BufferLength,
		PULONG LengthTransferred,
		LPOVERLAPPED Overlapped);

	//WinUsb_ReadPipe() is the basic function used to read data from the USB device (polls for and obtains data from
	//IN endpoints on the device)
	[DllImport("winusb.dll" , CharSet = CharSet::Seeifdef, EntryPoint="WinUsb_ReadPipe", CallingConvention=CallingConvention::Winapi)]
	extern "C" BOOL WinUsb_ReadPipe(
		WINUSB_INTERFACE_HANDLE InterfaceHandle,
		UCHAR PipeID,
		PUCHAR Buffer,
		ULONG BufferLength,
		PULONG LengthTransferred,
		LPOVERLAPPED Overlapped);

	//WinUsb_SetPipePolicy() can be used to configure the behavior of the WinUSB use of the specified endpoint
	[DllImport("winusb.dll" , CharSet = CharSet::Seeifdef, EntryPoint="WinUsb_SetPipePolicy", CallingConvention=CallingConvention::Winapi)]
	extern "C" BOOL WinUsb_SetPipePolicy(
		WINUSB_INTERFACE_HANDLE  InterfaceHandle,
		UCHAR  PipeID,
		ULONG  PolicyType,
		ULONG  ValueLength,
		PVOID  Value);

	//WinUsb_Free() is used to free up resources/close the handle that was returned when calling WinUsb_Initialize()
	[DllImport("winusb.dll" , CharSet = CharSet::Seeifdef, EntryPoint="WinUsb_Free", CallingConvention=CallingConvention::Winapi)]
	extern "C" BOOL WinUsb_Free(WINUSB_INTERFACE_HANDLE InterfaceHandle);

	//WinUsb_FlushPipe() is used to discard any data that may be "cached in a pipe".
	[DllImport("winusb.dll" , CharSet = CharSet::Seeifdef, EntryPoint="WinUsb_FlushPipe", CallingConvention=CallingConvention::Winapi)]
	extern "C" BOOL WinUsb_FlushPipe(WINUSB_INTERFACE_HANDLE  InterfaceHandle, UCHAR  PipeID);

	//Note: WinUSB supports quite a few more functions that aren't being used in this application, and aren't
	//shown here.  See the WinUSB client support routines documentation in MSDN (found in the WDK documentation
	//relating to WinUSB).

	//Need this function for receiving all of the WM_DEVICECHANGE messages.  See MSDN documentation for
	//description of what this function does/how to use it. Note: name is remapped "RegisterDeviceNotificationUM" to
	//avoid possible build error conflicts.
	[DllImport("user32.dll" , CharSet = CharSet::Seeifdef, EntryPoint="RegisterDeviceNotification", CallingConvention=CallingConvention::Winapi)]					
	extern "C" HDEVNOTIFY WINAPI RegisterDeviceNotificationUM(
		HANDLE hRecipient,
		LPVOID NotificationFilter,
		DWORD Flags);

	#pragma endregion



	//----------------------------------------------------------------
	//Application variables that need to have wide scope.
	//----------------------------------------------------------------
	volatile BOOL AttachedState = FALSE;							//Need to keep track of the USB device attachment status for proper plug and play operation.
	PSP_DEVICE_INTERFACE_DETAIL_DATA DetailedInterfaceDataStructure = new SP_DEVICE_INTERFACE_DETAIL_DATA;	//Global
	HANDLE MyDeviceHandle = INVALID_HANDLE_VALUE;		//First need to get the Device handle
	WINUSB_INTERFACE_HANDLE MyWinUSBInterfaceHandle;	//And then can call WinUsb_Initialize() to get the interface handle
														//which is needed for doing other operations with the device (like
														//reading and writing to the USB device).
	
	volatile unsigned char PushbuttonStateByte = 0xFF;	//atomic variable, used for sharing small amount of data between threads.
										//If larger data sets need to be shared across thread boundaries, use
										//a "Mutex" instead.
	volatile bool ThreadCancellationPending = false;




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
		Form1(void)	//Constructed function that gets executed when the Form is created.
		{
			InitializeComponent();

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
			if(CheckIfPresentAndGetUSBDevicePath())	//Check and make sure at least one device with matching VID/PID is attached
			{
				//We now have the proper device path, and we can finally open a device handle to the device.
				//WinUSB requires the device handle to be opened with the FILE_FLAG_OVERLAPPED attribute.
				MyDeviceHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
				DWORD ErrorStatus = GetLastError();
				if(ErrorStatus == ERROR_SUCCESS)
				{
					//Now get the WinUSB interface handle by calling WinUsb_Initialize() and providing the device handle.
					BOOL BoolStatus = WinUsb_Initialize(MyDeviceHandle, &MyWinUSBInterfaceHandle);
					if(BoolStatus == TRUE)
					{
						AttachedState = TRUE;
						StatusBox_txtbx->Text = "Device Found: AttachedState = TRUE";
						pushbuttonState_lbl->Text = "Pushbutton State: Unknown";
						pushbuttonState_lbl->Enabled = TRUE;
						toggleLEDs_btn->Enabled = TRUE;
					}
				}
			}
			else	//Device must not be connected (or not programmed with correct firmware)
			{
				AttachedState = FALSE;
				StatusBox_txtbx->Text = "Device Not Detected: Verify Connection/Correct Firmware";
				pushbuttonState_lbl->Text = "Pushbutton State: Unknown";
				pushbuttonState_lbl->Enabled = FALSE;
				toggleLEDs_btn->Enabled = FALSE;
			}

			//Start the "pushbuttonStatePollerThread" thread.  It will begin 
			//running in the background once the below line executes.
			ThreadCancellationPending = false;
			pushbuttonStatePollerThread->RunWorkerAsync();

		}//Form1(void)

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()	//Desctructor function.  Executes when the form is being closed.
		{
			//Try to gracefully stop the pushbuttonStatePollerThread, by
			//setting a flag letting it know it should shutdown.
			int i = 100;
			ThreadCancellationPending = true;
			pushbuttonStatePollerThread->CancelAsync();
			//Allow up to ~1 second for the thread to shutdown itself down 
			//gracefully, before non-gracefully exiting anyway.  This is
			//done as it is not ideal/preferred/recommended to close the
			//WinUSB interface handle while a blocking I/O operation may
			//still be using it.
			while((ThreadCancellationPending != false) && (i > 0))	//set to false in the thread once it has reached a safe state
			{
				i--;
				Sleep(10);
			}
			delete pushbuttonStatePollerThread;
			
			//Recommend explicitly closing these handles when exiting the application.  
			//Failure to do so may contribute to reduced system stability.
			if(AttachedState == TRUE)
			{
				WinUsb_Free(MyWinUSBInterfaceHandle);
				CloseHandle(MyDeviceHandle);
			}

			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  StatusBox_lbl;
	protected: 
	private: System::Windows::Forms::TextBox^  StatusBox_txtbx;
	private: System::Windows::Forms::Button^  toggleLEDs_btn;
	private: System::Windows::Forms::Label^  pushbuttonState_lbl;
	private: System::Windows::Forms::Timer^  GUIUpdateTimer;
	private: System::ComponentModel::BackgroundWorker^  pushbuttonStatePollerThread;
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
			this->StatusBox_lbl = (gcnew System::Windows::Forms::Label());
			this->StatusBox_txtbx = (gcnew System::Windows::Forms::TextBox());
			this->toggleLEDs_btn = (gcnew System::Windows::Forms::Button());
			this->pushbuttonState_lbl = (gcnew System::Windows::Forms::Label());
			this->GUIUpdateTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->pushbuttonStatePollerThread = (gcnew System::ComponentModel::BackgroundWorker());
			this->SuspendLayout();
			// 
			// StatusBox_lbl
			// 
			this->StatusBox_lbl->AutoSize = true;
			this->StatusBox_lbl->Location = System::Drawing::Point(316, 15);
			this->StatusBox_lbl->Name = L"StatusBox_lbl";
			this->StatusBox_lbl->Size = System::Drawing::Size(37, 13);
			this->StatusBox_lbl->TabIndex = 19;
			this->StatusBox_lbl->Text = L"Status";
			// 
			// StatusBox_txtbx
			// 
			this->StatusBox_txtbx->BackColor = System::Drawing::SystemColors::Window;
			this->StatusBox_txtbx->Location = System::Drawing::Point(12, 12);
			this->StatusBox_txtbx->Name = L"StatusBox_txtbx";
			this->StatusBox_txtbx->ReadOnly = true;
			this->StatusBox_txtbx->Size = System::Drawing::Size(298, 20);
			this->StatusBox_txtbx->TabIndex = 18;
			// 
			// toggleLEDs_btn
			// 
			this->toggleLEDs_btn->Enabled = false;
			this->toggleLEDs_btn->Location = System::Drawing::Point(12, 55);
			this->toggleLEDs_btn->Name = L"toggleLEDs_btn";
			this->toggleLEDs_btn->Size = System::Drawing::Size(97, 23);
			this->toggleLEDs_btn->TabIndex = 20;
			this->toggleLEDs_btn->Text = L"Toggle LED(s)";
			this->toggleLEDs_btn->UseVisualStyleBackColor = true;
			this->toggleLEDs_btn->Click += gcnew System::EventHandler(this, &Form1::toggleLEDs_btn_Click);
			// 
			// pushbuttonState_lbl
			// 
			this->pushbuttonState_lbl->AutoSize = true;
			this->pushbuttonState_lbl->Enabled = false;
			this->pushbuttonState_lbl->Location = System::Drawing::Point(148, 60);
			this->pushbuttonState_lbl->Name = L"pushbuttonState_lbl";
			this->pushbuttonState_lbl->Size = System::Drawing::Size(141, 13);
			this->pushbuttonState_lbl->TabIndex = 21;
			this->pushbuttonState_lbl->Text = L"Pushbutton State: Unknown";
			// 
			// GUIUpdateTimer
			// 
			this->GUIUpdateTimer->Enabled = true;
			this->GUIUpdateTimer->Interval = 5;
			this->GUIUpdateTimer->Tick += gcnew System::EventHandler(this, &Form1::GUIUpdateTimer_Tick);
			// 
			// pushbuttonStatePollerThread
			// 
			this->pushbuttonStatePollerThread->WorkerSupportsCancellation = true;
			this->pushbuttonStatePollerThread->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::pushbuttonStatePollerThread_DoWork);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(380, 107);
			this->Controls->Add(this->pushbuttonState_lbl);
			this->Controls->Add(this->toggleLEDs_btn);
			this->Controls->Add(this->StatusBox_lbl);
			this->Controls->Add(this->StatusBox_txtbx);
			this->Name = L"Form1";
			this->Text = L"WinUSB Demo";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


BOOL	CheckIfPresentAndGetUSBDevicePath(void)
{
		/* 
		Before we can "connect" our application to our USB embedded device, we must first find the device.
		A USB bus can have many devices simultaneously connected, so somehow we have to find our device, and only
		our device.  This is done with the Vendor ID (VID) and Product ID (PID).  Each USB product line should have
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

		The ultimate objective of the following code is to call WinUsb_Initialize(), which gets an interface handle
		to a specific interface on a USB device.  This handle will be needed later when calling WinUsb_WritePipe()
		and WinUsb_ReadPipe().  These functions are used to actually send and receive application related data 
		to/from the USB peripheral device.

		However, in order to call WinUsb_Initialize(), we first need to get the device path for the USB device
		with the correct VID and PID.  Getting the device path is a multi-step round about process, which
		requires calling several of the SetupDixxx() functions provided by setupapi.dll.
		*/


		//Globally Unique Identifier (GUID). Windows uses GUIDs to identify things.  This GUID needs to match
		//the GUID that is used in the .INF file used to install the WinUSB driver onto the system.
		//The INF file creates a register entry which associates a GUID with the WinUSB device.  In order for
		//a user mode application (such as this one) to find the USB device on the bus, it needs to known the
		//correct GUID that got put into the registry.
		GUID InterfaceClassGuid = {0x58D07210, 0x27C1, 0x11DD, 0xBD, 0x0B, 0x08, 0x00, 0x20, 0x0C, 0x9A, 0x66}; 

		HDEVINFO DeviceInfoTable = INVALID_HANDLE_VALUE;
		PSP_DEVICE_INTERFACE_DATA InterfaceDataStructure = new SP_DEVICE_INTERFACE_DATA;
//		PSP_DEVICE_INTERFACE_DETAIL_DATA DetailedInterfaceDataStructure = new SP_DEVICE_INTERFACE_DETAIL_DATA;	//Global
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

				if(CheckIfPresentAndGetUSBDevicePath())	//Check and make sure at least one device with matching VID/PID is attached
				{
					if(AttachedState == FALSE)
					{
						//We now have the proper device path, and we can finally open a device handle to the device.
						//WinUSB requires the device handle to be opened with the FILE_FLAG_OVERLAPPED attribute.
						MyDeviceHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
						DWORD ErrorStatus = GetLastError();
						if(ErrorStatus == ERROR_SUCCESS)
						{
							//Now get the WinUSB interface handle by calling WinUsb_Initialize() and providing the device handle.
							BOOL BoolStatus = WinUsb_Initialize(MyDeviceHandle, &MyWinUSBInterfaceHandle);
							if(BoolStatus == TRUE)
							{
								AttachedState = TRUE;
								StatusBox_txtbx->Text = "Device Found: AttachedState = TRUE";
								pushbuttonState_lbl->Text = "Pushbutton State: Unknown";
								pushbuttonState_lbl->Enabled = TRUE;
								toggleLEDs_btn->Enabled = TRUE;
							}
						}
					}
				}
				else	//Device must not be connected (or not programmed with correct firmware)
				{
					if(AttachedState == TRUE)		//If it is currently set to TRUE, that means the device was just now disconnected, and the open handles become invalid.
					{
						WinUsb_Free(MyWinUSBInterfaceHandle);
						CloseHandle(MyDeviceHandle);
					}
					AttachedState = FALSE;
					StatusBox_txtbx->Text = "Device Not Detected: Verify Connection/Correct Firmware";
					pushbuttonState_lbl->Text = "Pushbutton State: Unknown";
					pushbuttonState_lbl->Enabled = FALSE;
					toggleLEDs_btn->Enabled = FALSE;

				}				 
			 }//if(((int)m.WParam == DBT_DEVICEARRIVAL) || ((int)m.WParam == DBT_DEVICEREMOVEPENDING) || ((int)m.WParam == DBT_DEVICEREMOVECOMPLETE) || ((int)m.WParam == DBT_CONFIGCHANGED) )
		 }//if(m.Msg == WM_DEVICECHANGE)

		 //Now call the regular windows message handler
		 Form::WndProc( m );
	}//virtual void WndProc( Message% m ) override{




   //This is the button click event callback handler function that gets called when 
   //the user clicks the Toggle LED pushbutton on the Windows Form.  When the user
   //clicks on this button, we will send a USB OUT data packet to the device
   //firmware, telling it to toggle the LED(s) on the demo board.  The command
   //byte we will use for sending this information has been arbitarily selected
   //to be "0x80" in the first byte of the packet that gets sent.  Upon receiving
   //a USB OUT data packet from the host, the device firmware will check the first
   //byte to determine the host's command, and will perform the requested action
   //(toggle the LED(s) on the demo board, in this case).
	private: System::Void toggleLEDs_btn_Click(System::Object^  sender, System::EventArgs^  e) {

				 unsigned char OUTBuffer[64];
				 ULONG LengthTransferred;

				 //Prepare a USB OUT data packet to send to the device firmware
				 OUTBuffer[0] = 0x80;	//0x80 in byte 0 position is the "Toggle LED" command in the firmware

				 //Now send the USB packet data in the OUTBuffer[] to the device firmware.
				 WinUsb_WritePipe(MyWinUSBInterfaceHandle, 0x01, &OUTBuffer[0], 1, &LengthTransferred, NULL);

			 }//private: System::Void toggleLEDs_btn_Click(System::Object^  sender, System::EventArgs^  e) {





			 //This is a separate thread, which is used to perform periodic USB firmware
			 //pushbutton state polling.  This code periodically sends a request to
			 //the firmware (through a USB OUT data packet) requesting if the pushbutton
			 //on the demo board is pressed or not.  The firmware responds to this request
			 //by sending an IN data packet, consisting of the pushbutton state information.
			 //This polling is done in a independent thread, rather than the main GUI thread,
			 //since synchronous I/O operations are blocking and can take an indefinite amount
			 //of time to complete, if the device firmware never sends the expected data. 
			 //Therefore, it is preferrable to perform the USB I/O operations in a separate
			 //thread, so that the main GUI is not blocked from updating and accepting new
			 //user operations.  			 
private: System::Void pushbuttonStatePollerThread_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			 unsigned char OUTBuffer[64];
			 unsigned char INBuffer[64];
			 ULONG BytesTransferred;
			 BOOL Status;

			 //Infinite polling loop while this thread is still active
			 while(ThreadCancellationPending == false)
			 {
				 //Make sure the USB device is attached before we try to communicate with it.
				 if((AttachedState == TRUE) && (MyWinUSBInterfaceHandle != NULL))
				 {

					 //First, prepare an OUT packet to send to the USB device, which commands it to
					 //send a packet back IN to us, with the pushbutton state information
					 OUTBuffer[0] = 0x81;	//0x81 is the command byte value that the firmware is
											//written to recognize as the "get pushbutton state" request

					 //Now send the OUT packet to the USB device firwmare.
					 Status = WinUsb_WritePipe(MyWinUSBInterfaceHandle, 0x01, &OUTBuffer[0], 1, &BytesTransferred, NULL);

					 //Do error case checking to verify that the packet was successfully sent
					 if(Status == TRUE)
					 {
						 //We successfully sent the request to the firmware, it is now time to
						 //try to read the response IN packet from the device.
						 Status = WinUsb_ReadPipe(MyWinUSBInterfaceHandle, 0x81, &INBuffer[0], 64, &BytesTransferred, NULL);

						 //Now that we have the data in the INBuffer[], we should copy the pushbutton
						 //state byte into a global shadow byte in an atomic fashion.  The GUIUpdateTimer
						 //will periodically read this global shadow copy to update the Pushbutton State label
						 //on the GUI form.  This extra step is required since this thread cannot directly
						 //manipulate GUI objects in a separate thread.
						 if(Status == TRUE)
						 {
							 //Verify we got the echo back byte, of the command that the firmware responded to.
							 if(INBuffer[0] == 0x81) //0x81 is the get pushbutton state command
							 {
								 PushbuttonStateByte = INBuffer[1];	//Byte 1 contains the pushbutton state.  Byte 0 is 
																	//an echo back of the command byte the firmware 
																	//was responding to.  Bytes 2-63 were pad bytes sent by the firmware.
							 }
						 }
						 else
						 {
							 //The USB read failed for some reason.  In this case, we can't trust the pushbutton
							 //state, so lets force the PushbuttonStateByte to an invalid value,
							 //so the GUIUpdateTimer can portray the pushbutton state as unknown.
							 PushbuttonStateByte = 0xFF;	//Not valid pressed/not pressed value
						 }
					 }//if(Status == TRUE)
					 else
					 {
						 //The USB write failed for some reason.  In this case, we can't trust the pushbutton
						 //state, so lets force the PushbuttonStateByte to an invalid value,
						 //so the GUIUpdateTimer can portray the pushbutton state as unknown.
						 PushbuttonStateByte = 0xFF;	//Not valid pressed or not pressed value.  Value "unknown"
					 }
				 }//if((AttachedState == TRUE) && (MyWinUSBInterfaceHandle != NULL))

				 //Sleep for a little while.  Since this thread contains an infinite
				 //while(1) loop, we should Sleep() this thread periodically to let other
				 //threads get CPU time and avoid using 100% CPU utilization (of a single core).
				 Sleep(3);	//3ms

			 }//while(ThreadCancellationPending == false)

			 //We are about to finish executing the thread.  Let the Form destructor know that
			 //we are now done performing blocking I/O operations and it is safe to
			 //close the WinUSB interface handle.
			 ThreadCancellationPending = false;
		 }//private: System::Void pushbuttonStatePollerThread_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)



		 //This is a callback function that periodically gets called by a timer.
		 //It is used to update items on the GUI Form, based on information gathered
		 //in a separate thread (which can't directly update the GUI).
private: System::Void GUIUpdateTimer_Tick(System::Object^  sender, System::EventArgs^  e) {

			 //Peridically update the Pushbutton State label text with the latest info.

			 //Check if the device is currently attached.  If the device is unplugged, then
			 //we obviously can't know the pushbutton status.
			 if(AttachedState == FALSE)
			 {
				 pushbuttonState_lbl->Text = "Pushbutton State: Unknown";
			 }
			 else
			 {
				//The device must be attached
				if(PushbuttonStateByte == 0x00)
				{
					//The pushbutton was pressed
					pushbuttonState_lbl->Text = "Pushbutton State: Pressed";
				}
				else if(PushbuttonStateByte == 0x01)
				{
					//The pushbutton was NOT pressed
					pushbuttonState_lbl->Text = "Pushbutton State: Not Pressed";
				}
				else
				{
					//The firmware responded with an unexpected value
					pushbuttonState_lbl->Text = "Pushbutton State: Unknown";
				}
			 }
		 }//private: System::Void GUIUpdateTimer_Tick(System::Object^  sender, System::EventArgs^  e) 


}; //public ref class Form1 : public System::Windows::Forms::Form
}//namespace WinUSBDemo

