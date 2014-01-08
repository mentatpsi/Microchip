#pragma once
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
/********************************************************************
 FileName:		Form1.h
 Dependencies:	Platform SDK during development.  When compiled, needs
				.NET framework 2.0 redistributable to run
 Processor:		x86 
 Hardware:		Need free USB port to connect USB peripheral device(s)
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
  1.0   04/08/2008   Initial release
  2.3	08/11/2008	 Slight update for improved robustness
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
#include <setupapi.h>	//From Platform SDK. Definitions needed for the SetupDixxx() functions, which we use to
						//find our plug and play device
#include <Dbt.h>		//Needed for WM_DEVICECHANGE messages.  Defines the constants such as "DBT_DEVICEARRIVAL".
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace WM_DEVICECHANGE_Demo {

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

	//Returns a HDEVINFO type for a device information set (USB devices in
	//our case).  We will need the HDEVINFO as in input parameter for calling many of
	//the other SetupDixxx() functions.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetClassDevs")]		
	extern "C" HDEVINFO  SetupDiGetClassDevsUM(
		LPGUID  ClassGuid,				//Input: Supply the class GUID here. 
		PCTSTR  Enumerator,				//Input: Use NULL here, not important for our purposes
		HWND  hwndParent,				//Input: Use NULL here, not important for our purposes
		DWORD  Flags);					//Input: Flags describing what kind of filtering to use.

	//Gives us "PSP_DEVICE_INTERFACE_DATA" which contains the Interface specific GUID (different
	//from class GUID).
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiEnumDeviceInterfaces")]				
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiEnumDeviceInterfacesUM(
		HDEVINFO  DeviceInfoSet,				//Input: Give it the HDEVINFO we got from SetupDiGetClassDevs()
		PSP_DEVINFO_DATA  DeviceInfoData,		//Input (optional) 
		LPGUID  InterfaceClassGuid,				//Input 
		DWORD  MemberIndex,						//Input: Specify the "index" of the device you are interested in.  The index is zero based (starts at 0, and counts up 1, 2, 3, etc.).
		PSP_DEVICE_INTERFACE_DATA  DeviceInterfaceData);//Output: Give it a pointer to a "SP_DEVICE_INTERFACE_DATA" structure.  This function fills in the data structure.

	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiEnumDeviceInfo")]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiEnumDeviceInfoUM(
		HDEVINFO  DeviceInfoSet,
		DWORD  MemberIndex,
		PSP_DEVINFO_DATA  DeviceInfoData);

	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiDestroyDeviceInfoList")]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiDestroyDeviceInfoListUM(			
		HDEVINFO  DeviceInfoSet);				//Input: Give it a handle to a device info list to deallocate from RAM.

	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetDeviceRegistryProperty")]
	extern "C"	WINSETUPAPI BOOL WINAPI  SetupDiGetDeviceRegistryPropertyUM(
		HDEVINFO  DeviceInfoSet,
		PSP_DEVINFO_DATA  DeviceInfoData,
		DWORD  Property,
		PDWORD  PropertyRegDataType,
		PBYTE  PropertyBuffer,   
		DWORD  PropertyBufferSize,  
		PDWORD  RequiredSize);

	[DllImport("user32.dll" , EntryPoint="RegisterDeviceNotification")]					
	extern "C" HDEVNOTIFY WINAPI RegisterDeviceNotificationUM(
		HANDLE hRecipient,
		LPVOID NotificationFilter,
		DWORD Flags);


//  Stuff that needs to have wide scope.
	GUID InterfaceClassGuid = {0xa5dcbf10, 0x6530, 0x11d2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED}; //Globally Unique Identifier (GUID) for USB peripheral devices
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
			//Register for WM_DEVICECHANGE notifications.  Even without registering for anything additional messages, we will
			//likely receive some anyway.  By registering however, we can receive more messages, such as DBT_DEVICEARRIVAL and DBT_DEVICEREMOVECOMPLETE
			DEV_BROADCAST_DEVICEINTERFACE MyDeviceBroadcastHeader;
			MyDeviceBroadcastHeader.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
			MyDeviceBroadcastHeader.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
			MyDeviceBroadcastHeader.dbcc_reserved = 0;
			MyDeviceBroadcastHeader.dbcc_classguid = InterfaceClassGuid;
			RegisterDeviceNotificationUM((HANDLE)this->Handle, &MyDeviceBroadcastHeader, DEVICE_NOTIFY_WINDOW_HANDLE);


			String ^ deviceName;
			unsigned char i;

			deviceName = gcnew String("Vid_");
			for(i=txtbx_Vid->Text->Length;i<4;i++)
			{
				deviceName = String::Concat(deviceName,"0");
			}
			deviceName = String::Concat(deviceName,txtbx_Vid->Text);

			deviceName = String::Concat(deviceName,"&Pid_");
			for(i=txtbx_Pid->Text->Length;i<4;i++)
			{
				deviceName = String::Concat(deviceName,"0");
			}
			deviceName = String::Concat(deviceName,txtbx_Pid->Text);

			//Check once manually if your device is present.  If the device was plugged in before launching the application, we want to detect that too.  
			//We will not have received a WM_DEVICECHANGE message in this case, so we must manually check for our device once at application startup.
			if(TryToFindDeviceFromVIDPID(deviceName) == true)
				StatusBox->AppendText("Device: " + deviceName + " is CONNECTED.\r\n\r\n");
			else
				StatusBox->AppendText("Device: " + deviceName + " is NOT connected.\r\n\r\n");
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

	private: System::Windows::Forms::TextBox^  txtbx_Pid;

	private: System::Windows::Forms::TextBox^  txtbx_Vid;


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  StatusBox;
	private: System::Windows::Forms::Label^  label3;
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
			this->txtbx_Pid = (gcnew System::Windows::Forms::TextBox());
			this->txtbx_Vid = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->StatusBox = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// txtbx_Pid
			// 
			this->txtbx_Pid->Location = System::Drawing::Point(173, 12);
			this->txtbx_Pid->Name = L"txtbx_Pid";
			this->txtbx_Pid->Size = System::Drawing::Size(61, 20);
			this->txtbx_Pid->TabIndex = 1;
			this->txtbx_Pid->Text = L"003f";
			// 
			// txtbx_Vid
			// 
			this->txtbx_Vid->Location = System::Drawing::Point(50, 13);
			this->txtbx_Vid->Name = L"txtbx_Vid";
			this->txtbx_Vid->Size = System::Drawing::Size(61, 20);
			this->txtbx_Vid->TabIndex = 2;
			this->txtbx_Vid->Text = L"04d8";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(142, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(25, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"PID";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(19, 12);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(25, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"VID";
			// 
			// StatusBox
			// 
			this->StatusBox->Location = System::Drawing::Point(12, 52);
			this->StatusBox->Multiline = true;
			this->StatusBox->Name = L"StatusBox";
			this->StatusBox->Size = System::Drawing::Size(429, 602);
			this->StatusBox->TabIndex = 5;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(19, 36);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(227, 13);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Enter the VID and PID of the device to monitor";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(453, 666);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->StatusBox);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->txtbx_Vid);
			this->Controls->Add(this->txtbx_Pid);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
//Callback function which runs when a Windows message is received.  This function gets called more often than just once per 
//WM_DEVICECHANGE event, due to other Windows messages.
protected: virtual void WndProc( Message% m ) override{

		 // We will receive various different types of messages, but the ones we really want to use are the WM_DEVICECHANGE messages.
		 // These let us know if plug and play events are taking place.
		 if(m.Msg == WM_DEVICECHANGE)
		 {
			 MyCallBackOnWM_DEVICECHANGE(m);
		 }
         Form::WndProc( m );
   }


//private: System::Void MyCallBackOnWM_DEVICECHANGE(Message% m){
void MyCallBackOnWM_DEVICECHANGE(Message% m){
			String ^ deviceName;
			unsigned char i;

			deviceName = gcnew String("Vid_");
			for(i=txtbx_Vid->Text->Length;i<4;i++)
			{
				deviceName = String::Concat(deviceName,"0");
			}
			deviceName = String::Concat(deviceName,txtbx_Vid->Text);

			deviceName = String::Concat(deviceName,"&Pid_");
			for(i=txtbx_Pid->Text->Length;i<4;i++)
			{
				deviceName = String::Concat(deviceName,"0");
			}
			deviceName = String::Concat(deviceName,txtbx_Pid->Text);

			if((int)m.WParam == DBT_CONFIGCHANGECANCELED)
				StatusBox->AppendText("WM_DEVICECHANGE: DBT_CONFIGCHANGECANCELED\r\n");
			if((int)m.WParam == DBT_CONFIGCHANGED)
				StatusBox->AppendText("WM_DEVICECHANGE: DBT_CONFIGCHANGED\r\n");
			if((int)m.WParam == DBT_CUSTOMEVENT)
				StatusBox->AppendText("WM_DEVICECHANGE: DBT_CUSTOMEVENT\r\n");
			if((int)m.WParam == DBT_DEVICEARRIVAL)
				StatusBox->AppendText("WM_DEVICECHANGE: DBT_DEVICEARRIVAL\r\n");
			if((int)m.WParam == DBT_DEVICEQUERYREMOVE)
				StatusBox->AppendText("WM_DEVICECHANGE: DBT_DEVICEQUERYREMOVE\r\n");
			if((int)m.WParam == DBT_DEVICEQUERYREMOVEFAILED)
				StatusBox->AppendText("WM_DEVICECHANGE: DBT_DEVICEQUERYREMOVEFAILED\r\n");
			if((int)m.WParam == DBT_DEVICEREMOVECOMPLETE)
				StatusBox->AppendText("WM_DEVICECHANGE: DBT_DEVICEREMOVECOMPLETE\r\n");
			if((int)m.WParam == DBT_DEVICEREMOVEPENDING)
				StatusBox->AppendText("WM_DEVICECHANGE: DBT_DEVICEREMOVEPENDING\r\n");
			if((int)m.WParam == DBT_DEVICETYPESPECIFIC)
				StatusBox->AppendText("WM_DEVICECHANGE: DBT_DEVICETYPESPECIFIC\r\n");
			if((int)m.WParam == DBT_DEVNODES_CHANGED)
				StatusBox->AppendText("WM_DEVICECHANGE: DBT_DEVNODES_CHANGED\r\n");
			if((int)m.WParam == DBT_QUERYCHANGECONFIG)
				StatusBox->AppendText("WM_DEVICECHANGE: DBT_QUERYCHANGECONFIG\r\n");
			if((int)m.WParam == DBT_USERDEFINED)
				StatusBox->AppendText("WM_DEVICECHANGE: DBT_USERDEFINED\r\n");

			if(TryToFindDeviceFromVIDPID(deviceName) == true)
				StatusBox->AppendText("Device: " + deviceName + " is CONNECTED.\r\n\r\n");
			else
				StatusBox->AppendText("Device: " + deviceName + " is NOT connected.\r\n\r\n");
		}


bool TryToFindDeviceFromVIDPID(String^ DeviceIDToFind)
{
		/* 
		Before we can "connect" our application to our USB embedded device, we must first find the device.
		A USB bus can have many devices simultaneously connected, so somehow we have to find our device, and only
		our device.  This is done with the Vendor ID (VID) and Product ID (PID).  Each USB product line should have
		a unique combination of VID and PID.  

		Microsoft has created a number of functions which are useful for finding plug and play devices.  Documentation
		for each function used can be found in the MSDN library.  We will be using the following functions:

		SetupDiGetClassDevs()				//provided by setupapi.dll, which comes with Windows
		SetupDiEnumDeviceInterfaces()		//provided by setupapi.dll, which comes with Windows
		GetLastError()						//provided by kernel32.dll, which comes with Windows
		SetupDiDestroyDeviceInfoList()		//provided by setupapi.dll, which comes with Windows
		SetupDiGetDeviceRegistryProperty()	//provided by setupapi.dll, which comes with Windows
		malloc()							//part of? (something basic everyone should have)

		We will also be using the following unusual data types and structures.  Documentation can also be found in
		the MSDN library:

		PSP_DEVICE_INTERFACE_DATA
		SP_DEVINFO_DATA
		HDEVINFO
		GUID
		*/

		HDEVINFO DeviceInfoTable = INVALID_HANDLE_VALUE;
		PSP_DEVICE_INTERFACE_DATA InterfaceDataStructure = new SP_DEVICE_INTERFACE_DATA;
		SP_DEVINFO_DATA DevInfoData;

		DWORD InterfaceIndex = 0;
		DWORD StatusLastError = 0;
		DWORD dwRegType;
		DWORD dwRegSize;
		DWORD StructureSize = 0;
		PBYTE PropertyValueBuffer;
		bool MatchFound = false;
		DWORD ErrorStatus;

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
					return false;		
				}
			}
			else	//Else some other kind of unknown error ocurred...
			{
				ErrorStatus = GetLastError();
				SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
				return false;	
			}

			// Now retrieve the hardware ID from the registry.  The hardware ID contains the VID and PID, which we will then check to see if our device is attached or not.

			// Initialize an appropriate SP_DEVINFO_DATA structure.  We need this structure for SetupDiGetDeviceRegistryProperty().
			DevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
			SetupDiEnumDeviceInfoUM(DeviceInfoTable, InterfaceIndex, &DevInfoData);

			// First query for the size of the hardware ID, so we can know how big a buffer to allocate for the data.
			SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, NULL, 0, &dwRegSize);

			// Allocate a buffer for the hardware ID.
			PropertyValueBuffer = (BYTE *) malloc (dwRegSize);// * sizeof(TCHAR));

			// Retrieve the hardware IDs for the current device we are looking at.  PropertyValueBuffer gets filled with a REG_MULTI_SZ (array of null terminated strings).
			// To find a device, we only care about the very first string in the buffer, which will be the "device ID".  The device ID is a string which contains the USB
			// VID and PID, in the example format "Vid_04d8&Pid_000c".
			SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, PropertyValueBuffer, dwRegSize, NULL);

			//Now check if the first string in the hardware ID matches the device ID of my USB device.
			#ifdef UNICODE
			String^ DeviceIDFromRegistry = gcnew String((wchar_t *)PropertyValueBuffer);
			#else
			String^ DeviceIDFromRegistry = gcnew String((char *)PropertyValueBuffer);
			#endif

			free(PropertyValueBuffer);		//No longer need the PropertyValueBuffer, free the memory to prevent potential memory leaks

			DeviceIDFromRegistry = DeviceIDFromRegistry->ToLowerInvariant();	//Convert both strings to lower case.  This makes the code more robust/portable accross OS Versions
			DeviceIDToFind = DeviceIDToFind->ToLowerInvariant();				//Convert both strings to lower case.  This makes the code more robust/portable accross OS Versions
			MatchFound = DeviceIDFromRegistry->Contains(DeviceIDToFind);		//Check if the hardware ID we are looking at contains the correct VID/PID
			if(MatchFound == true)
			{
				//Device must have been found.  Insert additional application specific code here.  For example, if you wanted to set a state variable
				//"DeviceAttachedState = true;" here would be a good place for doing this.  Additionally, now would also be a good place where you would call
				//some kind of "MPUSBOpen()" or similar type of function to get read/write handles to your device.  

				SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);		//Clean up the old structure we no longer need.
				return true;
			}

			InterfaceIndex++;	//Keep looping until we either find our device with matching VID and PID, or until we have checked all possible items in the device class and still couldn't find it.
		 }//end of while(true)	
}
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


	};
}

