#pragma once
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
/********************************************************************
 FileName:		Form1.h
 Dependencies:	Platform SDK during development.  When compiled, needs
				.NET framework 2.0 redistributable to run
 Processor:		x86 
 Hardware:		Need a free USB port to connect USB peripheral device
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
#include <setupapi.h>	//From Platform SDK. Definitions needed for the SetupDixxx() functions, which we use to
						//find our plug and play device.

//Modify this value to match the VID and PID in your USB device descriptor.
//Use the formatting: "Vid_xxxx&Pid_xxxx" where xxxx is a 16-bit hexadecimal number.
#define MY_DEVICE_ID  "Vid_04d8&Pid_003F"	
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


namespace GenericHIDSimpleDemo {

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

	//Returns a HDEVINFO type for a device information set (USB HID devices in
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


//  Variables that need to have wide scope.
	HANDLE WriteHandle = INVALID_HANDLE_VALUE;	//Need to get a write "handle" to our device before we can write to it.
	HANDLE ReadHandle = INVALID_HANDLE_VALUE;	//Need to get a read "handle" to our device before we can read from it.
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
	private: System::Windows::Forms::Button^  ToggleLED_btn;
	protected: 

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
			this->Connect_btn->Location = System::Drawing::Point(13, 13);
			this->Connect_btn->Name = L"Connect_btn";
			this->Connect_btn->Size = System::Drawing::Size(75, 23);
			this->Connect_btn->TabIndex = 0;
			this->Connect_btn->Text = L"Connect";
			this->Connect_btn->UseVisualStyleBackColor = true;
			this->Connect_btn->Click += gcnew System::EventHandler(this, &Form1::Connect_btn_Click);
			// 
			// ToggleLED_btn
			// 
			this->ToggleLED_btn->Enabled = false;
			this->ToggleLED_btn->Location = System::Drawing::Point(95, 13);
			this->ToggleLED_btn->Name = L"ToggleLED_btn";
			this->ToggleLED_btn->Size = System::Drawing::Size(125, 23);
			this->ToggleLED_btn->TabIndex = 1;
			this->ToggleLED_btn->Text = L"Toggle LED(s)";
			this->ToggleLED_btn->UseVisualStyleBackColor = true;
			this->ToggleLED_btn->Click += gcnew System::EventHandler(this, &Form1::ToggleLED_btn_Click);
			// 
			// GetPushbuttonState_btn
			// 
			this->GetPushbuttonState_btn->Enabled = false;
			this->GetPushbuttonState_btn->Location = System::Drawing::Point(95, 43);
			this->GetPushbuttonState_btn->Name = L"GetPushbuttonState_btn";
			this->GetPushbuttonState_btn->Size = System::Drawing::Size(125, 23);
			this->GetPushbuttonState_btn->TabIndex = 2;
			this->GetPushbuttonState_btn->Text = L"Get Pushbutton State";
			this->GetPushbuttonState_btn->UseVisualStyleBackColor = true;
			this->GetPushbuttonState_btn->Click += gcnew System::EventHandler(this, &Form1::GetPushbuttonState_btn_Click);
			// 
			// StateLabel
			// 
			this->StateLabel->AutoSize = true;
			this->StateLabel->Enabled = false;
			this->StateLabel->Location = System::Drawing::Point(227, 52);
			this->StateLabel->Name = L"StateLabel";
			this->StateLabel->Size = System::Drawing::Size(84, 13);
			this->StateLabel->TabIndex = 3;
			this->StateLabel->Text = L"State: Unknown";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(346, 117);
			this->Controls->Add(this->StateLabel);
			this->Controls->Add(this->GetPushbuttonState_btn);
			this->Controls->Add(this->ToggleLED_btn);
			this->Controls->Add(this->Connect_btn);
			this->Name = L"Form1";
			this->Text = L"Form1";
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

		The ultimate objective of the following code is to call CreateFile(), which opens a communications
		pipe to a specific device (such as a HID class USB device endpoint).  CreateFile() returns a "handle" 
		which is needed later when calling ReadFile() or WriteFile().  These functions are used to actually 
		send and receive application related data to/from the USB peripheral device.

		However, in order to call CreateFile(), we first need to get the device path for the USB device
		with the correct VID and PID.  Getting the device path is a multi-step round about process, which
		requires calling several of the SetupDixxx() functions provided by setupapi.dll.
		*/


		//Globally Unique Identifier (GUID) for HID class devices.  Windows uses GUIDs to identify things.
		GUID InterfaceClassGuid = {0x4d1e55b2, 0xf16f, 0x11cf, 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30}; 

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
				//Device must have been found.  Open read and write handles.  In order to do this, we will need the actual device path first.
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

				//We now have the proper device path, and we can finally open read and write handles to the device.
				//We store the handles in the global variables "WriteHandle" and "ReadHandle", which we will use later to actually communicate.
				WriteHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

				ErrorStatus = GetLastError();
				if(ErrorStatus == ERROR_SUCCESS)
					ToggleLED_btn->Enabled = true;				//Make button no longer greyed out
				ReadHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
				ErrorStatus = GetLastError();
				if(ErrorStatus == ERROR_SUCCESS)
				{
					GetPushbuttonState_btn->Enabled = true;		//Make button no longer greyed out
					StateLabel->Enabled = true;					//Make label no longer greyed out
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
		 DWORD BytesWritten = 0;
		 unsigned char OutputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1

		 OutputPacketBuffer[0] = 0;			//The first byte is the "Report ID".  This number is used by the USB driver, but does not
											//get tranmitted accross the USB bus.  The custom HID class firmware is only configured for
											//one type of report, therefore, we must always initializate this byte to "0" before sending
											//a data packet to the device.

		 OutputPacketBuffer[1] = 0x80;		//0x80 is the "Toggle LED(s)" command in the firmware
											//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
											//data in it if you like.

		 //The basic Windows I/O functions WriteFile() and ReadFile() can be used to read and write to HID class USB devices.
		 //Note that we need the handle which we got earlier when we called CreateFile() (when we hit the connect button).
		 //The following function call will send out 64 bytes (starting from OutputPacketBuffer[1]) to the USB device.  The data will
		 //arrive on the OUT interrupt endpoint.
		 WriteFile(WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		 }


private: System::Void GetPushbuttonState_btn_Click(System::Object^  sender, System::EventArgs^  e) {
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
		 DWORD BytesWritten = 0;
		 DWORD BytesRead = 0;
		 unsigned char OutputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1
		 unsigned char InputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1

		 InputPacketBuffer[0] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
		 OutputPacketBuffer[0] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.

		 OutputPacketBuffer[1] = 0x81;			//0x81 is the "Get Pushbutton State" command in the firmware
												//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
												//data in it if you like.

		 //The basic Windows I/O functions WriteFile() and ReadFile() can be used to read and write to HID class USB devices 
		 //(once we have the read and write handles to the device, which are obtained with CreateFile()).

		 //To get the pushbutton state, first, we send a packet with our "Get Pushbutton State" command in it.
		 //The following call to WriteFile() sends 64 bytes of data to the USB device.
		 WriteFile(WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
		 //Now get the response packet from the firmware.
		 //The following call to ReadFIle() retrieves 64 bytes of data from the USB device.
		 ReadFile(ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used	

		 //InputPacketBuffer[0] is the report ID, which we don't care about.
		 //InputPacketBuffer[1] is an echo back of the command.
		 //InputPacketBuffer[2] contains the I/O port pin value for the pushbutton.  
		 if(InputPacketBuffer[2] == 0x01)	
		 {
			 StateLabel->Text = "State: Not Pressed";		//Update the pushbutton state text label on the form, so the user can see the result 
		 }
		 else //InputPacketBuffer[2] must have been == 0x00 instead
		 {
			 StateLabel->Text = "State: Pressed";			//Update the pushbutton state text label on the form, so the user can see the result 
		 }
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

		 }
};
}

