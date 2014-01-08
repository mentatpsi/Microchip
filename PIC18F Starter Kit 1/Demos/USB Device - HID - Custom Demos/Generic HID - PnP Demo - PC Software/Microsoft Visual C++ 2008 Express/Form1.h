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
#define MY_DEVICE_ID  "Vid_04d8&Pid_005e"	
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
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetClassDevs")]		
	extern "C" HDEVINFO  SetupDiGetClassDevsUM(
		LPGUID  ClassGuid,					//Input: Supply the class GUID here. 
		PCTSTR  Enumerator,					//Input: Use NULL here, not important for our purposes
		HWND  hwndParent,					//Input: Use NULL here, not important for our purposes
		DWORD  Flags);						//Input: Flags describing what kind of filtering to use.

	//Gives us "PSP_DEVICE_INTERFACE_DATA" which contains the Interface specific GUID (different
	//from class GUID).  We need the interface GUID to get the device path.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiEnumDeviceInterfaces")]				
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiEnumDeviceInterfacesUM(
		HDEVINFO  DeviceInfoSet,			//Input: Give it the HDEVINFO we got from SetupDiGetClassDevs()
		PSP_DEVINFO_DATA  DeviceInfoData,	//Input (optional)
		LPGUID  InterfaceClassGuid,			//Input 
		DWORD  MemberIndex,					//Input: "Index" of the device you are interested in getting the path for.
		PSP_DEVICE_INTERFACE_DATA  DeviceInterfaceData);//Output: This function fills in an "SP_DEVICE_INTERFACE_DATA" structure.

	//SetupDiDestroyDeviceInfoList() frees up memory by destroying a DeviceInfoList
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiDestroyDeviceInfoList")]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiDestroyDeviceInfoListUM(			
		HDEVINFO  DeviceInfoSet);			//Input: Give it a handle to a device info list to deallocate from RAM.

	//SetupDiEnumDeviceInfo() fills in an "SP_DEVINFO_DATA" structure, which we need for SetupDiGetDeviceRegistryProperty()
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiEnumDeviceInfo")]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiEnumDeviceInfoUM(
		HDEVINFO  DeviceInfoSet,
		DWORD  MemberIndex,
		PSP_DEVINFO_DATA  DeviceInfoData);

	//SetupDiGetDeviceRegistryProperty() gives us the hardware ID, which we use to check to see if it has matching VID/PID
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetDeviceRegistryProperty")]
	extern "C"	WINSETUPAPI BOOL WINAPI  SetupDiGetDeviceRegistryPropertyUM(
		HDEVINFO  DeviceInfoSet,
		PSP_DEVINFO_DATA  DeviceInfoData,
		DWORD  Property,
		PDWORD  PropertyRegDataType,
		PBYTE  PropertyBuffer,   
		DWORD  PropertyBufferSize,  
		PDWORD  RequiredSize);

	//SetupDiGetDeviceInterfaceDetail() gives us a device path, which is needed before CreateFile() can be used.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetDeviceInterfaceDetail")]
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
	[DllImport("user32.dll" , CharSet = CharSet::Seeifdef, EntryPoint="RegisterDeviceNotification")]					
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
	BOOL PushbuttonPressed = FALSE;				//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
	BOOL mTouchCalibratePending = FALSE;		//Updated by ToggleLED(s) button click event handler, used by ReadWriteThread (needs to be atomic)
	unsigned int ADCValueL = 0;					//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
	unsigned int ADCValueR = 0;					//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
	unsigned int ADCValueSliderTop = 0;			//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
	unsigned int ADCValueSliderBottom = 0;		//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)

	signed char ACCValueX = 0;					//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
	signed char ACCValueY = 0;					//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
	signed char ACCValueZ = 0;					//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
	unsigned char ACCValueT = 0;				//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)

	unsigned int ADCValuePot = 0;				//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)

	unsigned int maxADCValueL = 0;					//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
	unsigned int maxADCValueR = 0;					//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
	unsigned int maxADCValueSliderTop = 0;			//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
	unsigned int maxADCValueSliderBottom = 0;		//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)

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


	protected: 

	protected: 

	private: System::Windows::Forms::TextBox^  StatusBox_txtbx;
	private: System::Windows::Forms::ProgressBar^  progressBarL;




	private: System::ComponentModel::BackgroundWorker^  ReadWriteThread;
	private: System::Windows::Forms::Timer^  FormUpdateTimer;






private: System::Windows::Forms::ProgressBar^  progressBarR;
private: System::Windows::Forms::ProgressBar^  progressBarSliderBottom;

private: System::Windows::Forms::ProgressBar^  progressBarSliderTop;
private: System::Windows::Forms::TabControl^  tabControl1;
private: System::Windows::Forms::TabPage^  Graphical;

private: System::Windows::Forms::TabPage^  tabPage2;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::NumericUpDown^  numericUpDown4;
private: System::Windows::Forms::NumericUpDown^  numericUpDown3;
private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
private: System::Windows::Forms::Button^  button1;
private: System::Windows::Forms::PictureBox^  pictureBox1;
private: System::Windows::Forms::PictureBox^  pictureBox2;
private: System::Windows::Forms::NumericUpDown^  numericUpDown7;
private: System::Windows::Forms::NumericUpDown^  numericUpDown6;
private: System::Windows::Forms::NumericUpDown^  numericUpDown5;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::NumericUpDown^  numericUpDown8;
private: System::Windows::Forms::Label^  label9;
private: System::Windows::Forms::NumericUpDown^  numericUpDown9;



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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->StatusBox_txtbx = (gcnew System::Windows::Forms::TextBox());
			this->progressBarL = (gcnew System::Windows::Forms::ProgressBar());
			this->ReadWriteThread = (gcnew System::ComponentModel::BackgroundWorker());
			this->FormUpdateTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->progressBarR = (gcnew System::Windows::Forms::ProgressBar());
			this->progressBarSliderBottom = (gcnew System::Windows::Forms::ProgressBar());
			this->progressBarSliderTop = (gcnew System::Windows::Forms::ProgressBar());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->Graphical = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown8 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown7 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown6 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown5 = (gcnew System::Windows::Forms::NumericUpDown());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown4 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown9 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->tabControl1->SuspendLayout();
			this->Graphical->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->tabPage2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown8))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown9))->BeginInit();
			this->SuspendLayout();
			// 
			// StatusBox_txtbx
			// 
			this->StatusBox_txtbx->BackColor = System::Drawing::SystemColors::Window;
			this->StatusBox_txtbx->Location = System::Drawing::Point(12, 10);
			this->StatusBox_txtbx->Multiline = true;
			this->StatusBox_txtbx->Name = L"StatusBox_txtbx";
			this->StatusBox_txtbx->ReadOnly = true;
			this->StatusBox_txtbx->Size = System::Drawing::Size(193, 43);
			this->StatusBox_txtbx->TabIndex = 15;
			this->StatusBox_txtbx->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// progressBarL
			// 
			this->progressBarL->BackColor = System::Drawing::Color::White;
			this->progressBarL->ForeColor = System::Drawing::Color::Red;
			this->progressBarL->Location = System::Drawing::Point(22, 20);
			this->progressBarL->Maximum = 1024;
			this->progressBarL->Name = L"progressBarL";
			this->progressBarL->Size = System::Drawing::Size(60, 60);
			this->progressBarL->Step = 1;
			this->progressBarL->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBarL->TabIndex = 14;
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
			// progressBarR
			// 
			this->progressBarR->BackColor = System::Drawing::Color::White;
			this->progressBarR->ForeColor = System::Drawing::Color::Red;
			this->progressBarR->Location = System::Drawing::Point(102, 20);
			this->progressBarR->Maximum = 1024;
			this->progressBarR->Name = L"progressBarR";
			this->progressBarR->Size = System::Drawing::Size(60, 60);
			this->progressBarR->Step = 1;
			this->progressBarR->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBarR->TabIndex = 14;
			// 
			// progressBarSliderBottom
			// 
			this->progressBarSliderBottom->BackColor = System::Drawing::Color::White;
			this->progressBarSliderBottom->ForeColor = System::Drawing::Color::Red;
			this->progressBarSliderBottom->Location = System::Drawing::Point(22, 100);
			this->progressBarSliderBottom->Maximum = 1024;
			this->progressBarSliderBottom->Name = L"progressBarSliderBottom";
			this->progressBarSliderBottom->Size = System::Drawing::Size(140, 20);
			this->progressBarSliderBottom->Step = 1;
			this->progressBarSliderBottom->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBarSliderBottom->TabIndex = 14;
			// 
			// progressBarSliderTop
			// 
			this->progressBarSliderTop->BackColor = System::Drawing::Color::White;
			this->progressBarSliderTop->ForeColor = System::Drawing::Color::Red;
			this->progressBarSliderTop->Location = System::Drawing::Point(22, 130);
			this->progressBarSliderTop->Maximum = 1024;
			this->progressBarSliderTop->Name = L"progressBarSliderTop";
			this->progressBarSliderTop->Size = System::Drawing::Size(140, 20);
			this->progressBarSliderTop->Step = 1;
			this->progressBarSliderTop->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBarSliderTop->TabIndex = 14;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->Graphical);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(12, 59);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(193, 427);
			this->tabControl1->TabIndex = 16;
			// 
			// Graphical
			// 
			this->Graphical->Controls->Add(this->pictureBox1);
			this->Graphical->Controls->Add(this->progressBarL);
			this->Graphical->Controls->Add(this->progressBarSliderBottom);
			this->Graphical->Controls->Add(this->progressBarR);
			this->Graphical->Controls->Add(this->progressBarSliderTop);
			this->Graphical->Location = System::Drawing::Point(4, 22);
			this->Graphical->Name = L"Graphical";
			this->Graphical->Padding = System::Windows::Forms::Padding(3);
			this->Graphical->Size = System::Drawing::Size(185, 401);
			this->Graphical->TabIndex = 0;
			this->Graphical->Text = L"Visual";
			this->Graphical->UseVisualStyleBackColor = true;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(6, 346);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(174, 49);
			this->pictureBox1->TabIndex = 15;
			this->pictureBox1->TabStop = false;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->label9);
			this->tabPage2->Controls->Add(this->label8);
			this->tabPage2->Controls->Add(this->label7);
			this->tabPage2->Controls->Add(this->label6);
			this->tabPage2->Controls->Add(this->label5);
			this->tabPage2->Controls->Add(this->numericUpDown9);
			this->tabPage2->Controls->Add(this->numericUpDown8);
			this->tabPage2->Controls->Add(this->numericUpDown7);
			this->tabPage2->Controls->Add(this->numericUpDown6);
			this->tabPage2->Controls->Add(this->numericUpDown5);
			this->tabPage2->Controls->Add(this->pictureBox2);
			this->tabPage2->Controls->Add(this->button1);
			this->tabPage2->Controls->Add(this->label4);
			this->tabPage2->Controls->Add(this->label3);
			this->tabPage2->Controls->Add(this->label2);
			this->tabPage2->Controls->Add(this->label1);
			this->tabPage2->Controls->Add(this->numericUpDown4);
			this->tabPage2->Controls->Add(this->numericUpDown3);
			this->tabPage2->Controls->Add(this->numericUpDown2);
			this->tabPage2->Controls->Add(this->numericUpDown1);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(185, 401);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Numeric";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(93, 273);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(87, 13);
			this->label8->TabIndex = 18;
			this->label8->Text = L"Temperature (°C)";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(93, 233);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(36, 13);
			this->label7->TabIndex = 18;
			this->label7->Text = L"Acc Z";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(93, 193);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(36, 13);
			this->label6->TabIndex = 18;
			this->label6->Text = L"Acc Y";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(93, 153);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(36, 13);
			this->label5->TabIndex = 18;
			this->label5->Text = L"Acc X";
			// 
			// numericUpDown8
			// 
			this->numericUpDown8->Location = System::Drawing::Point(20, 270);
			this->numericUpDown8->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, System::Int32::MinValue});
			this->numericUpDown8->Name = L"numericUpDown8";
			this->numericUpDown8->Size = System::Drawing::Size(69, 20);
			this->numericUpDown8->TabIndex = 17;
			this->numericUpDown8->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, System::Int32::MinValue});
			// 
			// numericUpDown7
			// 
			this->numericUpDown7->Location = System::Drawing::Point(20, 230);
			this->numericUpDown7->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {256, 0, 0, 0});
			this->numericUpDown7->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {256, 0, 0, System::Int32::MinValue});
			this->numericUpDown7->Name = L"numericUpDown7";
			this->numericUpDown7->Size = System::Drawing::Size(69, 20);
			this->numericUpDown7->TabIndex = 17;
			// 
			// numericUpDown6
			// 
			this->numericUpDown6->Location = System::Drawing::Point(20, 190);
			this->numericUpDown6->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {256, 0, 0, 0});
			this->numericUpDown6->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {256, 0, 0, System::Int32::MinValue});
			this->numericUpDown6->Name = L"numericUpDown6";
			this->numericUpDown6->Size = System::Drawing::Size(69, 20);
			this->numericUpDown6->TabIndex = 17;
			// 
			// numericUpDown5
			// 
			this->numericUpDown5->Location = System::Drawing::Point(20, 150);
			this->numericUpDown5->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {256, 0, 0, 0});
			this->numericUpDown5->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {256, 0, 0, System::Int32::MinValue});
			this->numericUpDown5->Name = L"numericUpDown5";
			this->numericUpDown5->Size = System::Drawing::Size(69, 20);
			this->numericUpDown5->TabIndex = 17;
			// 
			// pictureBox2
			// 
			this->pictureBox2->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox2.Image")));
			this->pictureBox2->Location = System::Drawing::Point(6, 346);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(174, 49);
			this->pictureBox2->TabIndex = 16;
			this->pictureBox2->TabStop = false;
			// 
			// button1
			// 
			this->button1->ForeColor = System::Drawing::Color::Red;
			this->button1->Location = System::Drawing::Point(20, 110);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(146, 20);
			this->button1->TabIndex = 2;
			this->button1->Text = L"mTouchCalibrate";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(100, 55);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(69, 13);
			this->label4->TabIndex = 1;
			this->label4->Text = L"Bottom Slider";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(20, 55);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(55, 13);
			this->label3->TabIndex = 1;
			this->label3->Text = L"Top Slider";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(100, 5);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(66, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Right Button";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(20, 5);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(59, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Left Button";
			// 
			// numericUpDown4
			// 
			this->numericUpDown4->Location = System::Drawing::Point(100, 70);
			this->numericUpDown4->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1024, 0, 0, 0});
			this->numericUpDown4->Name = L"numericUpDown4";
			this->numericUpDown4->Size = System::Drawing::Size(69, 20);
			this->numericUpDown4->TabIndex = 0;
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->Location = System::Drawing::Point(20, 70);
			this->numericUpDown3->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1024, 0, 0, 0});
			this->numericUpDown3->Name = L"numericUpDown3";
			this->numericUpDown3->Size = System::Drawing::Size(69, 20);
			this->numericUpDown3->TabIndex = 0;
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Location = System::Drawing::Point(100, 20);
			this->numericUpDown2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1024, 0, 0, 0});
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(69, 20);
			this->numericUpDown2->TabIndex = 0;
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(20, 20);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1024, 0, 0, 0});
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(69, 20);
			this->numericUpDown1->TabIndex = 0;
			// 
			// numericUpDown9
			// 
			this->numericUpDown9->Location = System::Drawing::Point(20, 310);
			this->numericUpDown9->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1024, 0, 0, 0});
			this->numericUpDown9->Name = L"numericUpDown9";
			this->numericUpDown9->Size = System::Drawing::Size(69, 20);
			this->numericUpDown9->TabIndex = 17;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(93, 313);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(72, 13);
			this->label9->TabIndex = 18;
			this->label9->Text = L"Potentiometer";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(218, 498);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->StatusBox_txtbx);
			this->Name = L"Form1";
			this->Text = L"Generic HID PnP Demo";
			this->tabControl1->ResumeLayout(false);
			this->Graphical->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown8))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown9))->EndInit();
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
				if(ERROR_NO_MORE_ITEMS == GetLastError())	//Did we reach the end of the list of matching devices in the DeviceInfoTable?
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
	}
	if((AttachedState == FALSE) || (AttachedButBroken == TRUE))
	{
		//Device not available to communicate. Disable user interface on the form.
		StatusBox_txtbx->Text = "Device Not Detected: Verify Connection/Correct Firmware";

		ADCValueL = 0;
		ADCValueR = 0;
		ADCValueSliderTop = 0;
		ADCValueSliderBottom = 0;

		maxADCValueL = 0;
		maxADCValueR = 0;
		maxADCValueSliderTop = 0;
		maxADCValueSliderBottom = 0;

		progressBarL->Value = 0;
		progressBarR->Value = 0;
		progressBarSliderTop->Value = 0;
		progressBarSliderBottom->Value = 0;

		numericUpDown1->Value = 0;
		numericUpDown2->Value = 0;
		numericUpDown3->Value = 0;
		numericUpDown4->Value = 0;

		numericUpDown5->Value = 0;
		numericUpDown6->Value = 0;
		numericUpDown7->Value = 0;
		numericUpDown8->Value = 0;

		numericUpDown9->Value = 0;
	}

	//Update the various status indicators on the form with the latest info obtained from the ReadWriteThread()
	if(AttachedState == TRUE)
	{

		if (ADCValueL > maxADCValueL)
		{
			maxADCValueL = ADCValueL;
		}
		if (ADCValueR > maxADCValueR)
		{
			maxADCValueR = ADCValueR;
		}				
		if (ADCValueSliderTop > maxADCValueSliderTop)
		{
			maxADCValueSliderTop = ADCValueSliderTop;
		}				
		if (ADCValueSliderBottom > maxADCValueSliderBottom)
		{
			maxADCValueSliderBottom = ADCValueSliderBottom;
		}				

		/* Update the maximum value of the all the progress bar*/
		progressBarL->Maximum = maxADCValueL;
		progressBarR->Maximum = maxADCValueR;
		progressBarSliderTop->Maximum = maxADCValueSliderTop;
		progressBarSliderBottom->Maximum = maxADCValueSliderBottom;

		progressBarL->Value = ADCValueL;
		progressBarR->Value = ADCValueR;
		progressBarSliderTop->Value = ADCValueSliderTop;
		progressBarSliderBottom->Value = ADCValueSliderBottom;

		numericUpDown1->Value = ADCValueL;
		numericUpDown2->Value = ADCValueR;
		numericUpDown3->Value = ADCValueSliderTop;
		numericUpDown4->Value = ADCValueSliderBottom;

		numericUpDown5->Value = ACCValueX;
		numericUpDown6->Value = ACCValueY;
		numericUpDown7->Value = ACCValueZ;
		numericUpDown8->Value = ACCValueT/2 - 30;

		numericUpDown9->Value = ADCValuePot;
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
				OUTBuffer[1] = 0x20;	//READ_POT command (see the firmware source code), gets 10-bit ADC Value
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
						if(INBuffer[1] == 0x20)
						{
							ADCValuePot = (INBuffer[3] << 8) + INBuffer[2];	//Need to reformat the data from two unsigned chars into one unsigned int.
						}
					}
				}


				OUTBuffer[0] = 0;			//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
				OUTBuffer[1] = 0x30;	//READ_POT command (see the firmware source code), gets 10-bit ADC Value
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
						if(INBuffer[1] == 0x30)
						{
							ADCValueR = (INBuffer[3] << 8) + INBuffer[2];	//Need to reformat the data from two unsigned chars into one unsigned int.
						}
					}
				}


				OUTBuffer[0] = 0;			//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
				OUTBuffer[1] = 0x31;	//READ_POT command (see the firmware source code), gets 10-bit ADC Value
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
						if(INBuffer[1] == 0x31)
						{
							ADCValueSliderTop = (INBuffer[3] << 8) + INBuffer[2];	//Need to reformat the data from two unsigned chars into one unsigned int.
						}
					}
				}


				OUTBuffer[0] = 0;			//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
				OUTBuffer[1] = 0x32;	//READ_POT command (see the firmware source code), gets 10-bit ADC Value
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
						if(INBuffer[1] == 0x32)
						{
							ADCValueSliderBottom = (INBuffer[3] << 8) + INBuffer[2];	//Need to reformat the data from two unsigned chars into one unsigned int.
						}
					}
				}


				OUTBuffer[0] = 0;			//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
				OUTBuffer[1] = 0x33;	//READ_POT command (see the firmware source code), gets 10-bit ADC Value
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
						if(INBuffer[1] == 0x33)
						{
							ADCValueL = (INBuffer[3] << 8) + INBuffer[2];	//Need to reformat the data from two unsigned chars into one unsigned int.
						}
					}
				}

				OUTBuffer[0] = 0;			//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
				OUTBuffer[1] = 0x40;	//READ_POT command (see the firmware source code), gets 10-bit ADC Value
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
						if(INBuffer[1] == 0x40)
						{
							ACCValueX= (INBuffer[3] << 2) + INBuffer[2];;
						}
					}
				}

				OUTBuffer[0] = 0;			//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
				OUTBuffer[1] = 0x41;	//READ_POT command (see the firmware source code), gets 10-bit ADC Value
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
						if(INBuffer[1] == 0x41)
						{
							ACCValueY= (INBuffer[3] << 2) + INBuffer[2];
						}
					}
				}

				OUTBuffer[0] = 0;			//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
				OUTBuffer[1] = 0x42;	//READ_POT command (see the firmware source code), gets 10-bit ADC Value
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
						if(INBuffer[1] == 0x42)
						{
							ACCValueZ= (INBuffer[3] << 2) + INBuffer[2];
						}
					}
				}

				OUTBuffer[0] = 0;			//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
				OUTBuffer[1] = 0x43;	//READ_POT command (see the firmware source code), gets 10-bit ADC Value
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
						if(INBuffer[1] == 0x43)
						{
							ACCValueT= INBuffer[2];
						}
					}
				}

				if(mTouchCalibratePending == TRUE)
				{
					OUTBuffer[0] = 0;					//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
					OUTBuffer[1] = 0x80;			//0x80 is the "Toggle LED(s)" command in the firmware
					for(unsigned char i = 2; i <65; i++)	//This loop is not strictly necessary.  Simply initializes unused bytes to
						OUTBuffer[i] = 0xFF;				//0xFF for lower EMI and power consumption when driving the USB cable.
					//Now send the packet to the USB firmware on the microcontroller
					WriteFile(WriteHandleToUSBDevice, &OUTBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
					mTouchCalibratePending = FALSE;
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

private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			mTouchCalibratePending = TRUE;
			maxADCValueL = 0;
			maxADCValueR = 0;
			maxADCValueSliderTop = 0;
			maxADCValueSliderBottom = 0;
		 }
};
}

