/********************************************************************
  File Information:
    FileName:     	Form1.h
    Dependencies:	Requires .NET framework redistributable 2.0 or later
					to run executable.  During development, also needs setupapi.h 
					header file.  This is included in the Windows Driver Kit (WDK).
					Please download and install the WDK, and add the appropriate
					header include paths to your VC++ IDE, if encountering
					build errors.
    Processor:		x86, x64
    Hardware:		Personal Computer
    Complier:  	    Microsoft Visual C++ 2008 Express edition (or higher/better)
    Company:		Microchip Technology, Inc.
    
    Software License Agreement:
    
    The software supplied herewith by Microchip Technology Incorporated
    (the “Company”) for its PIC® Microcontroller is intended and
    supplied to you, the Company’s customer, for use solely and
    exclusively on Microchip PIC Microcontroller products. The
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



  Description:
    Example PC application demo showing how to connect to and communicate
	with a USB virtual COM port.  This demo uses dynamic concepts to
	find the COMx number based on the USB VID/PID, thus simplying the
	usage for the end user (human doesn't need to know the COMx number
	that was assigned for the device).


********************************************************************
 File Description:

 Change History:
  Rev    Description
  ----   -----------
  2.9	 Initial creation of this demo.  Started code from the 
		 "VC++ 2005 Simple CDC Demo" in the MCHPFSUSB Framework v2.8.
********************************************************************/


//Here are some useful articles when creating new PC applications for COM ports:
//(links valid as of June 3, 2008)
//
//"SerialPort Class"
//http://msdn.microsoft.com/en-us/library/system.io.ports.serialport.aspx
//
//"SerialPort Members"
//http://msdn.microsoft.com/en-us/library/system.io.ports.serialport_members.aspx
//
//"SerialPort.DataReceived Event"
//http://msdn.microsoft.com/en-us/library/system.io.ports.serialport.datareceived.aspx
//
//"How to: Make Thread-Safe Calls to Windows Forms Controls"
//http://msdn.microsoft.com/en-us/library/ms171728(VS.80).aspx

#pragma once


//-----------------------------------------------------------------------------------------------------------------------
//Modify this value to match the VID and PID in your USB device descriptor (in the USB firmware on the microcontroller).
//This value is needed to "find" the COMx number provided by the USB device.
//If the human user already knows the proper COMx number, then it is not required to know
//the VID/PID.  If the human user doesn't know the COMx number, then it is possible for
//this software to find the COMx number dynamically, by using the VID/PID of the USB device.

//Use the formatting: "Vid_xxxx&Pid_xxxx" where xxxx is a 16-bit hexadecimal number.
#define MY_DEVICE_ID  "Vid_04d8&Pid_000a"
//-----------------------------------------------------------------------------------------------------------------------

#include <windows.h>	//Defines datatypes and other things needed for using the setupapi.dll functions.
#include <setupapi.h>	//Definitions useful for using the setupapi.dll functions.



namespace VCCDC {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Runtime::InteropServices;  //Need this to support "unmanaged" code.

	#ifdef UNICODE
	#define	Seeifdef	Unicode
	#else
	#define Seeifdef	Ansi
	#endif

	//Returns a HDEVINFO type for a device information set (USB devices in
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

	//Note: This function is only supported in Windows Server 2003 and later (not implemented in XP).
	//It is uncertain if this function could be used to re-initialize a "Status: Attached but broken." device.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiRestartDevices", CallingConvention=CallingConvention::Winapi)]					
	extern "C" BOOL WINAPI SetupDiRestartDevices(
    HDEVINFO  DeviceInfoSet,
    PSP_DEVINFO_DATA  DeviceInfoData);


	//State tracking variables.
	BOOL DeviceAttachedState = FALSE;
	BOOL OldDeviceAttachedState = FALSE;



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
		//Create a delegate function for this thread that will take
        //  in a string and will write it to the txtDataReceived textbox

		delegate void SetTextCallback(String^ text);

		public:
		/****************************************************************************
			Function:
				public Form1()

			Summary:
				The main contructor for the Form1 class.

			Description:
				The main contructor for the Form1 class.  This function creates and
				initializes all of the form objects.

			Precondition:
				None

			Parameters:
				None

			Return Values:
				None

			Remarks:
				None
		***************************************************************************/
		Form1(void)
		{
			InitializeComponent();
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

	protected: 
	private: System::IO::Ports::SerialPort^  serialPort1;

	private: System::Windows::Forms::Button^  btnSendData;
	private: System::Windows::Forms::TextBox^  txtSendData;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::TextBox^  txtDataReceived;
	private: System::Windows::Forms::Label^  COMStatus_lbl;

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
			this->txtDataReceived = (gcnew System::Windows::Forms::TextBox());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->btnSendData = (gcnew System::Windows::Forms::Button());
			this->txtSendData = (gcnew System::Windows::Forms::TextBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->COMStatus_lbl = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// txtDataReceived
			// 
			this->txtDataReceived->Location = System::Drawing::Point(12, 66);
			this->txtDataReceived->Multiline = true;
			this->txtDataReceived->Name = L"txtDataReceived";
			this->txtDataReceived->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtDataReceived->Size = System::Drawing::Size(522, 155);
			this->txtDataReceived->TabIndex = 0;
			// 
			// serialPort1
			// 
			this->serialPort1->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &Form1::serialPort1_DataReceived);
			// 
			// btnSendData
			// 
			this->btnSendData->Location = System::Drawing::Point(12, 39);
			this->btnSendData->Name = L"btnSendData";
			this->btnSendData->Size = System::Drawing::Size(76, 21);
			this->btnSendData->TabIndex = 2;
			this->btnSendData->Text = L"Send Data";
			this->btnSendData->UseVisualStyleBackColor = true;
			this->btnSendData->Click += gcnew System::EventHandler(this, &Form1::btnSendData_Click);
			// 
			// txtSendData
			// 
			this->txtSendData->Location = System::Drawing::Point(94, 40);
			this->txtSendData->Name = L"txtSendData";
			this->txtSendData->Size = System::Drawing::Size(440, 20);
			this->txtSendData->TabIndex = 3;
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 30;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// COMStatus_lbl
			// 
			this->COMStatus_lbl->AutoSize = true;
			this->COMStatus_lbl->Location = System::Drawing::Point(12, 9);
			this->COMStatus_lbl->Name = L"COMStatus_lbl";
			this->COMStatus_lbl->Size = System::Drawing::Size(167, 13);
			this->COMStatus_lbl->TabIndex = 6;
			this->COMStatus_lbl->Text = L"Status: Unknown or Not Attached";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(546, 236);
			this->Controls->Add(this->COMStatus_lbl);
			this->Controls->Add(this->txtSendData);
			this->Controls->Add(this->btnSendData);
			this->Controls->Add(this->txtDataReceived);
			this->Name = L"Form1";
			this->Text = L"Dynamic COM port example";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


		/****************************************************************************
			Function:
				private void timer1_Tick(object sender, EventArgs e)

			Summary:
				This function periodically runs and checks if the USB virtual COM port
				devices attached.  If so, it connects to it.  If not, it closes
				the COM port.

			Description:
				This function periodically runs and checks if the USB virtual COM port
				devices attached.  If so, it connects to it.  If not, it closes
				the COM port.

			Precondition:
				None

			Parameters:
				object sender     - Sender of the event (this form)
				EventArgs e       - The event arguments

			Return Values:
				None

			Remarks:
				None
		***************************************************************************/
		private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
		{
			String^ COMNumberString = "";

			COMNumberString = CheckIfPresentAndGetCOMString(MY_DEVICE_ID);
			if(COMNumberString == "")
			{
				DeviceAttachedState = FALSE;
				COMStatus_lbl->Text = "Status: Unknown or Not Attached";
			}
			else
			{
				DeviceAttachedState = TRUE;
			}

			//Check if we are switching from "was not attached" to "is now attached" state.
			if((OldDeviceAttachedState != DeviceAttachedState) && (DeviceAttachedState == TRUE))
			{
				try
				{
					//Get the port name from the application list box.
					//  the PortName attribute is a string name of the COM
					//  port (e.g. - "COM1").
					//serialPort1->PortName = lstCOMPorts->Items[lstCOMPorts->SelectedIndex]->ToString();
					serialPort1->PortName = COMNumberString;

					//Open the COM port.
					serialPort1->Open();

					//Enable user interface to the device.
					btnSendData->Enabled = TRUE;
					COMStatus_lbl->Text = "Status: Attached as " + COMNumberString;
				}
				catch(...)
				{
					//If there was an exception, then close the handle to 
					//  the device and assume that the device was removed
					//btnClose_Click(this, gcnew EventArgs());
					OldDeviceAttachedState = TRUE;
					DeviceAttachedState = FALSE;
					COMStatus_lbl->Text = "Status: Attached but broken.  Please unplug device, wait a few seconds, and re-attach device.";
				}
			}
			
			//Check if we are switching from "was attached" to "not attached anymore" state.
			if((OldDeviceAttachedState != DeviceAttachedState) && (DeviceAttachedState == FALSE))
			{
				//Reset the state of the application objects
				btnSendData->Enabled = FALSE;


				//This section of code will try to close the COM port.
				//  Please note that it is important to use a try/catch
				//  statement when closing the COM port.  If a USB virtual
				//  COM port is removed and the PC software tries to close
				//  the COM port before it detects its removal then
				//  an exeception is thrown.  If the execption is not in a
				//  try/catch statement this could result in the application
				//  crashing.
				try
				{
					//Dispose the In and Out buffers;
					serialPort1->DiscardInBuffer();
					serialPort1->DiscardOutBuffer();
				}
				//If there was an exeception then there isn't much we can
				//  do.  The port is no longer available.
				catch(...){}

				try
				{
					//Close the COM port
					serialPort1->Close();
				}
				catch(...){}
			}

			OldDeviceAttachedState = DeviceAttachedState;
		}




		/****************************************************************************
			Function:
				String^	CheckIfPresentAndGetCOMString(String^ DeviceIDString)

			Summary:
				This function checks if a USB CDC device is currently plugged in with 
				a matching USB VID and PID string.  If a matching device is found, it
				returns the COMx number string associated with the USB device.

			Description:
				This function checks if a USB CDC device is currently plugged in with 
				a matching USB VID and PID string.  If a matching device is found, it
				returns the COMx number string associated with the USB device.

			Precondition:
				None

			Parameters:
				String^ DeviceIDString  - Takes in a Device ID string, containing the VID/PID 
											of the USB device to be found.  Should be formatted 
											like: "Vid_04d8&Pid_000a"

			Return Values:
				String^ containing the "COMx" string number (useful when trying to open the COM port).
						If no USB CDC COMx ports are currently provided by a USB with matching VID/PID,
						then the return value is: ""

			Remarks:
				This function is passive and does not attempt to open/close any COMx ports.  All it
				does is search for attached USB devices, that are currently providing COMx ports.
				
				This function is currently written to work only with a single instance of the USB device
				attached to the computer, with the given VID/PID.  If more than one device of matching
				VID/PID is simultaneously attached to the machine, then more than one COMx port number
				will exist (one for each CDC device).  This function will only return the first COMx
				number that it finds for the matching VID/PID.  If more than one exists, the additional
				COMx numbers will remain unknown.  To expand the functionality of this function, it would
				be possible to remove the "return COMString;" line of code, and instead keep looping
				until the "if(ErrorStatus == ERROR_NO_MORE_ITEMS)" exit condition is hit instead.
				In this case, the loop will find multiple COMx numbers and instances, and these
				could be stored and returned in an array of strings instead.
		***************************************************************************/
String^	CheckIfPresentAndGetCOMString(String^ DeviceIDString)
{
		//Globally Unique Identifier (GUID) for COM port devices.  Windows uses GUIDs to identify things.
		GUID InterfaceClassGuid = {0xa5dcbf10, 0x6530, 0x11d2, 0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed};

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
		DWORD LoopCounter = 0;

		String^ DeviceIDToFind = DeviceIDString;
		String^ FriendlyNameString = "";
		int IndexOfStartofCOMChars;
		int IndexOfEndOfCOMChar = 0;
		String^ COMString = "";

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
					return "";		
				}
			}
			else	//Else some other kind of unknown error ocurred...
			{
				ErrorStatus = GetLastError();
				SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
				return "";	
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
				return "";		
			}

			//Retrieve the hardware IDs for the current device we are looking at.  PropertyValueBuffer gets filled with a 
			//REG_MULTI_SZ (array of null terminated strings).  To find a device, we only care about the very first string in the
			//buffer, which will be the "device ID".  The device ID is a string which contains the VID and PID, in the example 
			//format "Vid_04d8&Pid_000a".
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
				//Device must have been found.  Now we should read the "FriendlyName".  The FriendlyName is the text
				//that appears in the device manager entry for the device.  The friendlyname for the virtual com port
				//is composed of the text from the .inf file used to install the driver, combined with the COMx number
				//assigned to the device.  The COMx number also appears in the registry "PortName" entry for the device.
				//First call the function to determine the size of the required buffer.
				SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_FRIENDLYNAME, &dwRegType, NULL, 0, &dwRegSize);
				//Allocate a buffer for the hardware ID.
				PropertyValueBuffer = (BYTE *) malloc (dwRegSize);
				if(PropertyValueBuffer == NULL)	//if null, error, couldn't allocate enough memory
				{	//Can't really recover from this situation, just exit instead.
					SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
					return "";		
				}

				//Retrieve the FriendlyName string containing the COMx number.
				SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_FRIENDLYNAME, &dwRegType, PropertyValueBuffer, dwRegSize, NULL);

				//Now copy the resulting string into a managed type for easier manipulation with
				//managed functions.
				#ifdef UNICODE
					FriendlyNameString = gcnew String((wchar_t *)PropertyValueBuffer);
				#else
					FriendlyNameString = gcnew String((char *)PropertyValueBuffer);
				#endif
				free(PropertyValueBuffer);		//No longer need the PropertyValueBuffer, free the memory to prevent potential memory leaks


				//Check if it contains the COMx string like expected, and if so, extract it, so it can
				//be returned in the formatting used when opening a COMx port.
				if((FriendlyNameString->Contains("(COM")) && (FriendlyNameString->Contains(")")))
				{
					IndexOfStartofCOMChars = FriendlyNameString->IndexOf("COM");
					IndexOfEndOfCOMChar = FriendlyNameString->IndexOf(")");
					if((IndexOfEndOfCOMChar > IndexOfStartofCOMChars) && (IndexOfEndOfCOMChar <= FriendlyNameString->Length))
					{
						//Copy the "COMxx" string value from the FriendlyNameString to the COMString.
						//The COMString is formatted in the exact format needed when setting a serial port object's COM number.
						//COMString = FriendlyNameString->Substring(IndexOfStartofCOMChars, IndexOfEndOfCOMChar - IndexOfStartofCOMChars);
						COMString = FriendlyNameString->Substring(IndexOfStartofCOMChars, IndexOfEndOfCOMChar - IndexOfStartofCOMChars);
					}
				}
				 
				//We now (most likely) found the COMx number and should return.
				SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
				return COMString;	//May contain "" if we couldn't find the device COMxx number.  Otherwise, returns "COMxxx" where xxx is the COM number (could be only one or two chars, depending upon COM number)
			}

			InterfaceIndex++;	
			//Keep looping until we either find a device with matching VID and PID, or until we run out of devices to check.
			//However, just in case some unexpected error occurs, keep track of the number of loops executed.
			//If the number of loops exceeds a very large number, exit anyway, to prevent inadvertent infinite looping.
			LoopCounter++;
			if(LoopCounter == 10000000)	//Surely there aren't more than 10 million devices attached to any forseeable PC...
			{
				return "";
			}
		}//end of while(true)
}



		/****************************************************************************
			Function:
				private void serialPort1_DataReceived(  object sender, 
														SerialDataReceivedEventArgs e)

			Summary:
				This function prints any data received on the COM port.

			Description:
				This function is called when the data is received on the COM port.  This
				function attempts to write that data to the txtDataReceived textbox.  If
				an exception occurs the btnClose_Click() function is called in order to
				close the COM port that caused the exception.

			Precondition:
				None

			Parameters:
				object sender     - Sender of the event (this form)
				SerialDataReceivedEventArgs e       - The event arguments

			Return Values:
				None

			Remarks:
				None
		***************************************************************************/
		private: System::Void serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) 
		{
			//The ReadExisting() function will read all of the data that
			//  is currently available in the COM port buffer.  In this 
			//  example we are sending all of the available COM port data
			//  to the SetText() function.
			//
			//  NOTE: the <SerialPort>_DataReceived() function is launched
			//  in a seperate thread from the rest of the application.  A
			//  delegate function is required in order to properly access
			//  any managed objects inside of the other thread.  Since we
			//  will be writing to a textBox (a managed object) the delegate
			//  function is required.  Please see the SetText() function for 
			//  more information about delegate functions and how to use them.
			try
			{
				SetText(serialPort1->ReadExisting());
			}
			catch(...)
			{
			}
		}

		/****************************************************************************
			Function:
				private void SetText(string text)

			Summary:
				This function prints the input text to the txtDataReceived textbox.

			Description:
				This function prints the input text to the txtDataReceived textbox.  If
				the calling thread is the same as the thread that owns the textbox, then
				the AppendText() method is called directly.  If a thread other than the
				main thread calls this function, then an instance of the delegate function
				is created so that the function runs again in the main thread.

			Precondition:
				None

			Parameters:
				string text     - Text that needs to be printed to the textbox

			Return Values:
				None

			Remarks:
				None
		***************************************************************************/
		private: void SetText(String^ text)
		{
			//InvokeRequired required compares the thread ID of the
			//  calling thread to the thread ID of the creating thread.
			//  If these threads are different, it returns true.  We can
			//  use this attribute to determine if we can append text
			//  directly to the textbox or if we must launch an a delegate
			//  function instance to write to the textbox.
			if (this->txtDataReceived->InvokeRequired)
			{
				//InvokeRequired returned TRUE meaning that this function
				//  was called from a thread different than the current
				//  thread.  We must launch a deleage function.

				//Create an instance of the SetTextCallback delegate and
				//  assign the delegate function to be this function.  This
				//  effectively causes this same SetText() function to be
				//  called within the main thread instead of the second
				//  thread.
				SetTextCallback^ d = gcnew SetTextCallback(this,&VCCDC::Form1::SetText);

				//Invoke the new delegate sending the same text to the
				//  delegate that was passed into this function from the
				//  other thread.
				this->Invoke(d,gcnew String(text));
			}
			else
			{
				//If this function was called from the same thread that 
				//  holds the required objects then just add the text.
				txtDataReceived->AppendText(text);
			}
		}

		/****************************************************************************
			Function:
				private void btnSendData_Click(object sender, EventArgs e)

			Summary:
				This function will attempt to send the contents of txtData over the COM port

			Description:
				This function is called when the btnSendData button is clicked.  It will 
				attempt to send the contents of txtData over the COM port.  If the attempt
				is unsuccessful this function will call the btnClose_Click() in order to
				close the COM port that just failed.

			Precondition:
				None

			Parameters:
				object sender     - Sender of the event (this form)
				EventArgs e       - The event arguments

			Return Values:
				None

			Remarks:
				None
		***************************************************************************/
		private: System::Void btnSendData_Click(System::Object^  sender, System::EventArgs^  e)
		{
			//This section of code will try to write to the COM port.
			//  Please note that it is important to use a try/catch
			//  statement when writing to the COM port.  If a USB virtual
			//  COM port is removed and the PC software tries to write
			//  to the COM port before it detects its removal then
			//  an exeception is thrown.  If the execption is not in a
			//  try/catch statement this could result in the application
			//  crashing.

			try
			{
				//Write the data in the text box to the open serial port
				serialPort1->Write(txtSendData->Text);
			}
			catch(...)
			{
			}
		}
};
}

