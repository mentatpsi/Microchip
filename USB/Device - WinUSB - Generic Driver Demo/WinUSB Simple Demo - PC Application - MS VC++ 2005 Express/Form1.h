#pragma once
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
/********************************************************************
 FileName:		Form1.h
 Dependencies:	Windows Server 2003 Platform SDK during development.  
				Also need WDK 6001.18001 (see includes section for 
				additional details).  When compiled, needs .NET 
				framework 2.0 redistributable to run.  May also
				need MS VC++ redistributable to run.
 Hardware:		Need a free USB port to connect USB peripheral device
				programming with appropriate WinUSB firmware.  VID and
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
  1.0   05/21/2008   Initial release
  2.3	08/11/2008	 Slight update for improved robustness
  2.7a	08/29/2010	 Adding explicit calling conventions to the DllImports.
					 This is needed for Visual Studio 2010 compatibility.
					 No functional changes to the code.  Backwards compatibility
					 should be retained.  Also modified the #include <Winusb.h>
					 inline comment, as it also contains information relevant
					 when building this project with Visual Studio 2010.
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
						//Also see the below comments on how to add directories to the include path.


						//IMPORTANT: READ THIS BEFORE TRYING TO BUILD THIS CODE
						//----------------------------------------------------
#include <Winusb.h>		//Winusb.h comes as a part of the Windows Driver Kit (WDK) build 6001.18002 (and later versions).
						//The WDK is a free download from http://connect.microsoft.com (must create an account
						//and then sign up for the WDK connection).
						//You will need the WDK build 6001.18002 (or later) installed on
						//your computer to use this source code.  Once you have it installed, you will also need to add
						//the include path directories from the WDK to your VC++ IDE.  This can be done by clicking
						//Tools-->Options-->+Projects and Solutions-->VC++ Directories-->Show Directories for: "Include files"
						//Then click the Folder icon (new line) and then the "..." button and add these directories (to the 
						//bottom of the list!  The order of the list is important, and the project will not build correctly
						//if the order is wrong.):
						//C:\WINDDK\6001.18002\inc\ddk		
						//C:\WINDDK\6001.18002\inc\api		
						//The above directory locations assume the default location for the WDK, build 6001.18002.
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
						//$(VCInstallDir)include;$(VCInstallDir)atlmfc\include;$(WindowsSdkDir)include;$(FrameworkSDKDir)\include;C:\WinDDK\7600.16385.0\inc\ddk;C:\WinDDK\7600.16385.0\inc\api						




//Modify this value to match the VID and PID in your USB device descriptor.
//Use the formatting: "Vid_xxxx&Pid_xxxx" where xxxx is a 16-bit hexadecimal number.
#define MY_DEVICE_ID  "Vid_04d8&Pid_0053"	
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


namespace SimpleWinUSBDemo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
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


//  Variables that need to have wide scope.
	HANDLE MyDeviceHandle = INVALID_HANDLE_VALUE;		//First need to get the Device handle
	WINUSB_INTERFACE_HANDLE MyWinUSBInterfaceHandle;	//And then can call WinUsb_Initialize() to get the interface handle
														//which is needed for doing other operations with the device (like
														//reading and writing to the USB device).
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
	protected: 
	private: System::Windows::Forms::Button^  ToggleLED_btn;
	private: System::Windows::Forms::Button^  GetPushbuttonState_btn;
	private: System::Windows::Forms::Label^  StateLabel;

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
			this->Connect_btn->Location = System::Drawing::Point(12, 12);
			this->Connect_btn->Name = L"Connect_btn";
			this->Connect_btn->Size = System::Drawing::Size(75, 23);
			this->Connect_btn->TabIndex = 1;
			this->Connect_btn->Text = L"Connect";
			this->Connect_btn->UseVisualStyleBackColor = true;
			this->Connect_btn->Click += gcnew System::EventHandler(this, &Form1::Connect_btn_Click);
			// 
			// ToggleLED_btn
			// 
			this->ToggleLED_btn->Enabled = false;
			this->ToggleLED_btn->Location = System::Drawing::Point(93, 12);
			this->ToggleLED_btn->Name = L"ToggleLED_btn";
			this->ToggleLED_btn->Size = System::Drawing::Size(125, 23);
			this->ToggleLED_btn->TabIndex = 2;
			this->ToggleLED_btn->Text = L"Toggle LED(s)";
			this->ToggleLED_btn->UseVisualStyleBackColor = true;
			this->ToggleLED_btn->Click += gcnew System::EventHandler(this, &Form1::ToggleLED_btn_Click);
			// 
			// GetPushbuttonState_btn
			// 
			this->GetPushbuttonState_btn->Enabled = false;
			this->GetPushbuttonState_btn->Location = System::Drawing::Point(93, 41);
			this->GetPushbuttonState_btn->Name = L"GetPushbuttonState_btn";
			this->GetPushbuttonState_btn->Size = System::Drawing::Size(125, 23);
			this->GetPushbuttonState_btn->TabIndex = 3;
			this->GetPushbuttonState_btn->Text = L"Get Pushbutton State";
			this->GetPushbuttonState_btn->UseVisualStyleBackColor = true;
			this->GetPushbuttonState_btn->Click += gcnew System::EventHandler(this, &Form1::GetPushbuttonState_btn_Click);
			// 
			// StateLabel
			// 
			this->StateLabel->AutoSize = true;
			this->StateLabel->Enabled = false;
			this->StateLabel->Location = System::Drawing::Point(224, 51);
			this->StateLabel->Name = L"StateLabel";
			this->StateLabel->Size = System::Drawing::Size(84, 13);
			this->StateLabel->TabIndex = 4;
			this->StateLabel->Text = L"State: Unknown";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(333, 96);
			this->Controls->Add(this->StateLabel);
			this->Controls->Add(this->GetPushbuttonState_btn);
			this->Controls->Add(this->ToggleLED_btn);
			this->Controls->Add(this->Connect_btn);
			this->Name = L"Form1";
			this->Text = L"Simple WinUSB Demo";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Connect_btn_Click(System::Object^  sender, System::EventArgs^  e) {
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
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
		PSP_DEVICE_INTERFACE_DETAIL_DATA DetailedInterfaceDataStructure = new SP_DEVICE_INTERFACE_DETAIL_DATA;
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
				if(ERROR_NO_MORE_ITEMS == ErrorStatus)	//Did we reach the end of the list of matching devices in the DeviceInfoTable?
				{	//Cound not find the device.  Must not have been attached.
					SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
					return;		
				}
			}
			else	//Else some other kind of unknown error ocurred...
			{
				ErrorStatus = GetLastError();
				SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
				return;	
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
				return;		
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
					return;		
				}
				DetailedInterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
				 //Now call SetupDiGetDeviceInterfaceDetail() a second time to receive the goods.  
				SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, DetailedInterfaceDataStructure, StructureSize, NULL, NULL); 

				//We now have the proper device path, and we can finally open a device handle to the device.
				//WinUSB requires the device handle to be opened with the FILE_FLAG_OVERLAPPED attribute.
				MyDeviceHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);

				ErrorStatus = GetLastError();
				if(ErrorStatus == ERROR_SUCCESS)
				{
					//Now get the WinUSB interface handle by calling WinUsb_Initialize() and providing the device handle.
					BoolStatus = WinUsb_Initialize(MyDeviceHandle, &MyWinUSBInterfaceHandle);
					if(BoolStatus == TRUE)
					{
						//If gets here, the "MyWinUSBInterfaceHandle" was initialized successfully.
						//May begin using the MyWinUSBInterfaceHandle handle in WinUsb_WritePipe() and
						//WinUsb_ReadPipe() function calls now.  Those are the functions for writing/reading to
						//the USB device's endpoints.

						ToggleLED_btn->Enabled = true;				//Make button no longer greyed out
						GetPushbuttonState_btn->Enabled = true;		//Make button no longer greyed out
						StateLabel->Enabled = true;					//Make label no longer greyed out
					}
				}

				SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
				return;
			}

			InterfaceIndex++;	
			//Keep looping until we either find a device with matching VID and PID, or until we run out of items.
		}//end of while(true)	
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
			 }
private: System::Void ToggleLED_btn_Click(System::Object^  sender, System::EventArgs^  e) {
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
		 ULONG BytesWritten = 0;
		 unsigned char OutputPacketBuffer[64];	//Allocate a memory buffer which will contain data to send to the USB device

		 OutputPacketBuffer[0] = 0x80;		//0x80 is the "Toggle LED(s)" command in the firmware
											//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
											//data in it if you like.

		 //The winusb.dll file exposes a variety of useful functions, the most important of which is WinUsb_WritePipe() and
		 //WinUsb_ReadPipe().  These are the actual functions that can be used by a user mode application (such as this one)
		 //to send/receive application related data to specific endpoints on the USB device.
		 //Before we can use the WinUsb_WritePipe() and WinUsb_ReadPipe() functions, we will have to have already obtained
		 //a WINUSB_INTERFACE_HANDLE for the USB device.  This is done by the code that gets executed after clicking the
		 //Connect button.

		 //The following function call will send out 64 bytes (starting from OutputPacketBuffer[0]) to the USB device.  The
		 //data will arrive on "PipeID" 0x01, which is the endpoint 1 OUT endpoint.
		
		 //Endpoint PipeID: bottom 7 bits is the endpoint number (0-15), the MSb is the direction.  MSb = 0 for OUT endpoints, MSb = 1 for IN
		 //endpoints.  Ex: Endpoint 7 IN is "0x87", while endpoint 3 OUT is "0x03", while endpoint 1 OUT is 0x01.
		 //Note: The below function call is a blocking function until it completes, since it is not being called with 
		 //an "OVERLAPPED" structure specified.  See MSDN documentation regarding overlapped structures and asynchronous
		 //I/O operations.
		 WinUsb_WritePipe(MyWinUSBInterfaceHandle, 0x01, &OutputPacketBuffer[0], 64, &BytesWritten, NULL);  
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		 }
private: System::Void GetPushbuttonState_btn_Click(System::Object^  sender, System::EventArgs^  e) {
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
		 ULONG BytesWritten = 0;
		 ULONG BytesRead = 0;
		 unsigned char OutputPacketBuffer[64];	//Allocate a memory buffer which will contain data to send to the USB device
		 unsigned char InputPacketBuffer[64];	//Allocate a memory buffer for the data which we will read from the USB device

		 OutputPacketBuffer[0] = 0x81;			//0x81 is the "Get Pushbutton State" command in the firmware
												//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
												//data in it if you like.

		 //The winusb.dll functions: WinUsb_WritePipe() and WinUsb_ReadPipe() can be used to write and read from
		 //specific endpoints on the USB device.  See the WDK documentation (also containined in MSDN) for other 
		 //"WinUSB User-Mode Client Support Routines" (ie: other useful functions for interacting with your USB device).

		 //To get the pushbutton state, first, we send a packet with our "Get Pushbutton State" command in it.
		 //The following call to WinUsb_WritePipe() sends 64 bytes of data to the USB device.
		 WinUsb_WritePipe(MyWinUSBInterfaceHandle, 0x01, &OutputPacketBuffer[0], 64, &BytesWritten, NULL);  
		 //Now get the response packet from the firmware.
		 //The following call to WinUsb_ReadPipe() retrieves 64 bytes of data from the USB device.
		 WinUsb_ReadPipe(MyWinUSBInterfaceHandle, 0x81, &InputPacketBuffer[0], 64, &BytesRead, NULL);  

		 //InputPacketBuffer[0] is an echo back of the command.
		 //InputPacketBuffer[1] contains the I/O port pin value for the pushbutton.  
		 if(InputPacketBuffer[1] == 0x01)	
		 {
			 StateLabel->Text = "State: Not Pressed";		//Update the pushbutton state text label on the form, so the user can see the result 
		 }
		 else //InputPacketBuffer[1] must have been == 0x00 instead
		 {
			 StateLabel->Text = "State: Pressed";			//Update the pushbutton state text label on the form, so the user can see the result 
		 }
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		 }
};
}

