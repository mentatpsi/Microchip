#pragma once
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
/********************************************************************
 FileName:		Form1.h
 Dependencies:	Windows Server 2003 Platform SDK during development.  
				Also need WDK 6001.18002 (see includes section for 
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
  2.5   05/29/2009   Initial release
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
						//The link is functional as of 21 May 2009, but could move.  If the above link does not work,
						//try searching for "Video1_PSDK.wmv", which may be associated with:
						//"Using Visual C++ Express Edition and the Platform SDK"
						//Also see the below comments on how to add directories to the include path.

						//If this project will be built with Microsoft Visual C++ 2008 (express or other edition),
						//it will be necessary to use the "Windows SDK" instead of the "Platform SDK".


						//IMPORTANT: READ THIS BEFORE TRYING TO BUILD THIS CODE
						//----------------------------------------------------
#include <Winusb.h>		//Winusb.h comes as a part of the Windows Driver Kit (WDK) build 6001.18002 (and presumably later versions).
						//The WDK is currently a free download from http://connect.microsoft.com
						//You will need the WDK build 6001.18002 (the latest full official release as of 22 May 2009) installed on
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


#include <Dbt.h>		//Need this for definitions of WM_DEVICECHANGE messages


//Modify this value to match the VID and PID in your USB device descriptor.
//Use the formatting: "Vid_xxxx&Pid_xxxx" where xxxx is a 16-bit hexadecimal number.
#define MY_DEVICE_ID  "Vid_04d8&Pid_0052"	//Microchip High bandwidth WinUSB Demo.  Change this number (along with the 
											//corresponding VID/PID in the microcontroller firmware, and in the driver 
											//installation .INF file) before moving the design into production.


//----------------------------------------------------------------------------------------------------------------
//This demo is meant to show some methods that can be used to maximize USB full speed bandwidth over bulk endpoints
//using the Microsoft WinUSB general purpose vendor class USB driver.  Although this example uses the WinUSB driver,
//the concepts shown (using multiple bulk endpoints, calling the USB read/write functions with large data buffers)
//are generally applicable for other USB drivers which support bulk endpoints as well.

//For a full speed USB bus, the 12Mbits/s bit rate allows for up to 1,216,000 Bytes/second of application
//data, after accounting for USB protocol overhead (handshake packets, CRC fields, token packets, SOF 
//packets, etc.).  See the USB 2.0 specifications section 5.8.4 for details on how this figure is obtained. 

//All currently existing Microchip USB microcontrollers use a dedicated DMA interface for
//transferring large amounts of USB data directly into and out of main microcontroller
//SRAM.  This allows for large data transfer rates, with minimal microcontroller
//firmware intervention.  The USB module is therefore capable of obtaining the entire
//1,216,000 Bytes/second theoretical maximum application bandwidth of full speed bulk
//endpoints, on any currently existing PIC18/24/32 USB microcontroller.

//In thoery, a single bulk endpoint is capable of saturating all of the available ~1.2MB/s bandwidth
//of a full speed bus.  In practice, this often does not occur, as all USB components in the system:
//host conroller, host drivers, USB 2.0 hubs, hub drivers, USB device firmware, USB device driver, 
//and any other USB related drivers that may be applicable (ex: composite driver), etc., must all
//be operating at perfect efficiency to obtain the full ~1.2MB/s bandwidth.  

//Since USB is a token based protocol, a USB device cannot simply begin driving the D+/D- lines if 
//it wants to send data to the host.  The host is responsible for initiating all communications on the
//USB bus by sending appropriate token packets (ex: IN, OUT, SETUP).  Therefore, it is important
//that the host scheduler schedule traffic on the bus optimally in order to obtain high USB bandwidth
//utilization approaching the theoretical maximum.

//In practice, common USB hosts do not always schedule USB traffic perfectly, and therefore,
//the effective practical bandwidth observed over a single bulk endpoint is often less than the
//theoretical maximum (ex: several hundred kB/s, but noticesably less than the theortical 
//maximum 1.2MB/s).  

//For some additional details and tips on how to maximize
//bandwidth, see:
//www.usb.org/developers/presentations/pres0598/bulkperf.ppt

//As can be seen from the above referred to presentation, higher effective bandwidth, approaching
//the theoretical maximum 1.2MB/s can potentially be achieved by keeping several best practices
//in mind when developing both the PC application, and the USB device firmware.

//In order to achieve maximum practical bandwidth, it is critical to write the PC application to use
//large data buffers when calling the USB read/write API functions endpoints.

//For example, for maximum bandwidth when sending 64,000 bytes of OUT data to the device, do not 
//do something like this:
/*----------------------------------------------------------------------------------------------
unsigned char DataBuffer[64000];
ULONG BytesWritten;
for(i = 0; i < 1000; i++)
{
	WinUsb_WritePipe(WinUSBInterfaceHandle, 0x01, &DataBuffer[i*64], 64, &BytesWritten, NULL);
}
----------------------------------------------------------------------------------------------*/

//Instead, the below method of sending the 64,000 bytes of OUT data will allow the host to 
//schedule the USB traffic much more efficiently:
/*----------------------------------------------------------------------------------------------
unsigned char DataBuffer[64000];
ULONG BytesWritten;
WinUsb_WritePipe(WinUSBInterfaceHandle, 0x01, &DataBuffer, 64000, &BytesWritten, NULL);
----------------------------------------------------------------------------------------------*/

//When attempting to send or receive more bulk data than the size of the endpoint, the USB driver
//automatically splits up the data and sends/receives it over multiple transactions as needed.
//The PC application software does not have to manually do this (and doing so will cause a 
//performance penalty).  However, if the data size is extremely large, there can still be some
//limitations in the USB driver that require the data to be split over multiple calls to the
//respective read or write function.  See the USB driver documentation for details regarding
//limitations of the driver.  Using larger and larger data buffers has dimishing returns,
//so it is reasonable to simply limit the maximum data size for each WinUsb_WritePipe() or
//WinUsb_ReadPipe() function calls, even if not required explicitly by the driver.  If huge 
//amounts of data need to be sent or received, use a large data buffer (ex: 64000 bytes), 
//but call the read or write function in a loop to transfer the full amount of data desired.

//This demo is meant to show a funtioning implementation of the above key methods needed to obtain
//high practical USB bulk bandwidth.  This demo shows how a single bulk endpoint can be used,
//as well as showing how three bulk endpoints can be used simultaneously, to demonstrate the
//differences in behavior.

//Note: Connecting the USB device to the host through a USB 2.0 hub(s) can also have a 
//noticeable performance impact.  USB 2.0 hubs include one or more "transaction translator(s)."
//If a USB full speed (12Mbps) or low speed (1.5Mbps) device is connected to a USB 2.0 hub, the hub
//will commmunicate downstream (to the low or full speed device) at low or full speed, but upsteam
//(to the host), it may communicate at 480Mbps USB high speed rate (assuming no USB 1.1 hubs are upstream
//of the USB 2.0 hub).  The hub can perform a speed translation which can greatly help to improve 
//USB 2.0 root port bandwidth allocation/utilization when multiple low or full speed USB devices are 
//connected through USB 2.0 hub(s) up to a single USB high speed (480Mbps) capable root port.

//In some cases the USB 2.0 transaction translation process can improve effective bandwidth for a single
//device, but in some cases it can lead to different transaction timing, which depending upon other aspects
//of the USB system design, can sometimes lead to less effective bandwidth for a single device, using a 
//single endpoint.  When developing a new application, it is always recommended
//to thoroughly test the design on a wide variety of host and hub combinations to ensure that the design 
//functions correctly and meets bandwdith expectations.


//In order to maximize effective USB application bandwidth, in addition to optimizing the PC
//application, it is also important to optimize the USB device firmware to minimize "NAK"
//handshake packets from being sent from the device to the host.  This is especially true for
//bulk OUT endpoints, although it is also important for IN endpoints as well.

//For bulk OUT endpoints, a normal OUT transaction consists of the following:
//1.  Host sends OUT token packet.
//2.  Host sends DATAx packet (this contains up to 64 bytes of data payload)
//3.  USB device sends either and ACK or a NAK handshake to the host.

//If the host receives an ACK handshake from the device, it knows that the data was correctly received.
//If the host receives a NAK handshake, this means that the device was busy, and that the host needs
//to re-transmit the same data again.  A USB device is allowed to NAK bulk traffic at its discretion,
//and it may continously do so many times if it stays busy for a long time.  In this case, the
//host must continuously re-attempt the transaction again and again by sending the OUT token packet
//and DATAx data payload packet as many times as needed.

//For example, if the host wants to send 128 bytes of data to a bulk endpoint of 64 byte maximum size,
//The following USB bus sequence may occur:

//1. Host sends OUT token packet
//2. Host sends DATAx payload packet (containing bytes 0-63)
//3. Device sends back an ACK packet
//4. (small inter-transaction delay, bus is idle for a few microseconds)
//5. Host sends OUT token packet
//6. Host sends DATAx payload packet (containing bytes 64-127)
//7. Device sends back a NAK packet
//8. (small inter-transaction delay, bus is idle for a few microseconds)
//9. Host sends OUT token packet
//10. Host sends DATAx payload packet (containing bytes 64-127, again)
//11. Device sends back a ACK packet

//In the above scenario, all of the USB bus bandwidth that was needed for steps 5, 6, 7, are purely
//"wasted", as application data did not successfully get received, as the USB device was busy.
//This wasted bus bandwidth significantly degrades overall effective application data transfer
//rates.  In the above example scenario ~1/3 of the total USB bandwidth is wasted, which has
//the effect of reducing the maximum theoretical data rate of 1.2MB/s down to ~790kB/s.


//Therefore, if high application data rates are required, it is necessary to implement
//optimum USB firmware.  All currently existing Microchip full speed USB microcontrollers 
//implement a "ping-pong" buffering feature, that can be used to greatly reduce NAK handshakes.
//The ping-pong buffering feature is described in the individual USB device datasheets, but
//essentially, ping-pong buffers is equivalent to a 2-entry "DMA FIFO".

//For each successful USB transaction to take place, it is necessary for the firmware to
//"arm" the endpoint that the data will be sent/received on.  The firmware does this by
//initializing the "BDT" (buffer descriptor table) entry for that endpoint.  The BDT
//entry consists of a small set of special function registers (SFRs) that are used to control
//the USB DMA transaction (ex: number of bytes to transmit, pointer to where the data should be 
//deposited, or read from, and a "UOWN" bit, which indicates that the buffer is initialized 
//and ready for the DMA transaction to occur).

//When ping-pong buffering is enabled, there are two sets of DMA control SFRs (BDT entries) 
//for each endpoint.  For example, if ping-pong buffering is enabled for "endpoint 1", 
//the "Endpoint 1 OUT" endpoint will have two BDT entries for controlling it, and the
//"Endpoint 1 IN" endpoint will also have two separate BDT entries.  

//By having more than one BDT entry (set of DMA control SFRs) for each endpoint, it is 
//possible to simultaneously "queue up" more than one pending transaction at a time, on
//the same endpoint.  This is effectively a "DMA FIFO", and it allows the firmware to greatly
//reduce the amount of NAK handshake packets that can get sent to the host on bulk endpoints.

//To take maximum advantage of the ping-pong buffering feature, it is necessary to write
//the application firmware to keep track of and maintain two buffers for each endpoint.
//Since this adds application complexity, many of the default demos in the MCHPFSUSB framework
//package may not make full use of, and obtain the full benefits of, ping-pong buffering.
//The demos may however be modified to make use of this feature.


//On the author's PC (1.7 GHz Pentium M based laptop, Intel EHCI USB 2.0 host controller,
//Microsoft Windows XP with SP3) this demo achieves the following results:

//USB Device: Direct connection to root port.
//Single bulk OUT on EP1 only: 1,065,482 Bytes/second
//Multi bulk OUT on EP1, EP2, EP3 Simultaneously:  1,141,498 Bytes/second

//USB Device: Connected to host through a USB 2.0 hub.
//Single bulk OUT on EP1 only: 766,773 Bytes/second
//Multi bulk OUT on EP1, EP2, EP3 Simultaneously:  1,155,234 Bytes/second

//The above numbers were measured with a PIC18F46J50 running at 48MHz, while
//using full ping pong buffering.  When the USB traffic is observed using a hardware 
//based USB protocol analyzer, there are no or almost no NAK packets on the bus for any of
//the above four measurements.  In the single endpoint USB 2.0 hub case, there is fairly 
//signficant bus idle time (~27us typ) between each transaction which is reducing the achieved 
//bandwidth.  This host scheduling inefficiency is believed to be caused primarily due to
//Microsoft Knowledge Base article 940021:
//http://support.microsoft.com/kb/940021


//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


namespace HighBandwidthWinUSB {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

#pragma region DLL Imports

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
	[DllImport("winusb.dll" , CharSet = CharSet::Seeifdef, EntryPoint="WinUsb_Free", CallingConvention=CallingConvention::Winapi)]
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

//  Variables that need to have wide scope.
	BOOL AttachedState = FALSE;							//Need to keep track of the USB device attachment status for proper plug and play operation.
	PSP_DEVICE_INTERFACE_DETAIL_DATA DetailedInterfaceDataStructure = new SP_DEVICE_INTERFACE_DETAIL_DATA;	//Global
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
						SendMultiOUT_btn->Enabled = TRUE;
						BulkOut_btn->Enabled = TRUE;
						Bandwidth_lbl->Enabled = TRUE;
						ElapsedTimer_lbl->Enabled = TRUE;
						DataXfer_chkbx->Enabled = TRUE;
						StatusBox_txtbx->Text = "Device Found: AttachedState = TRUE";
					}
				}
			}
			else	//Device must not be connected (or not programmed with correct firmware)
			{
				AttachedState = FALSE;
				SendMultiOUT_btn->Enabled = FALSE;
				BulkOut_btn->Enabled = FALSE;
				Bandwidth_lbl->Enabled = FALSE;
				ElapsedTimer_lbl->Enabled = FALSE;
				DataXfer_chkbx->Enabled = FALSE;
				Bandwidth_txtbx->Text = "";
				ElapsedTime_txtbx->Text = "";
				StatusBox_txtbx->Text = "Device Not Detected: Verify Connection/Correct Firmware";
			}

//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------



		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			//Recommend explicitly closing these handles when exiting the application.  Failure to do so may contribute to
			//reduced system stability.
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
	private: System::Windows::Forms::TextBox^  StatusBox_txtbx;
	protected: 
	private: System::Windows::Forms::Label^  StatusBox_lbl;
	private: System::Windows::Forms::CheckBox^  DataXfer_chkbx;
	private: System::Windows::Forms::Label^  Bandwidth_lbl;
	private: System::Windows::Forms::TextBox^  Bandwidth_txtbx;
	private: System::Windows::Forms::Label^  ElapsedTimer_lbl;
	private: System::Windows::Forms::TextBox^  ElapsedTime_txtbx;
	private: System::Windows::Forms::Button^  BulkOut_btn;
	private: System::Windows::Forms::Button^  SendMultiOUT_btn;

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
			this->StatusBox_txtbx = (gcnew System::Windows::Forms::TextBox());
			this->StatusBox_lbl = (gcnew System::Windows::Forms::Label());
			this->DataXfer_chkbx = (gcnew System::Windows::Forms::CheckBox());
			this->Bandwidth_lbl = (gcnew System::Windows::Forms::Label());
			this->Bandwidth_txtbx = (gcnew System::Windows::Forms::TextBox());
			this->ElapsedTimer_lbl = (gcnew System::Windows::Forms::Label());
			this->ElapsedTime_txtbx = (gcnew System::Windows::Forms::TextBox());
			this->BulkOut_btn = (gcnew System::Windows::Forms::Button());
			this->SendMultiOUT_btn = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// StatusBox_txtbx
			// 
			this->StatusBox_txtbx->BackColor = System::Drawing::SystemColors::Window;
			this->StatusBox_txtbx->Location = System::Drawing::Point(12, 12);
			this->StatusBox_txtbx->Name = L"StatusBox_txtbx";
			this->StatusBox_txtbx->ReadOnly = true;
			this->StatusBox_txtbx->Size = System::Drawing::Size(298, 20);
			this->StatusBox_txtbx->TabIndex = 16;
			// 
			// StatusBox_lbl
			// 
			this->StatusBox_lbl->AutoSize = true;
			this->StatusBox_lbl->Location = System::Drawing::Point(316, 15);
			this->StatusBox_lbl->Name = L"StatusBox_lbl";
			this->StatusBox_lbl->Size = System::Drawing::Size(37, 13);
			this->StatusBox_lbl->TabIndex = 17;
			this->StatusBox_lbl->Text = L"Status";
			// 
			// DataXfer_chkbx
			// 
			this->DataXfer_chkbx->AutoCheck = false;
			this->DataXfer_chkbx->AutoSize = true;
			this->DataXfer_chkbx->Enabled = false;
			this->DataXfer_chkbx->Location = System::Drawing::Point(12, 123);
			this->DataXfer_chkbx->Name = L"DataXfer_chkbx";
			this->DataXfer_chkbx->Size = System::Drawing::Size(147, 17);
			this->DataXfer_chkbx->TabIndex = 18;
			this->DataXfer_chkbx->Text = L"Data Transfer In Progress";
			this->DataXfer_chkbx->UseVisualStyleBackColor = true;
			// 
			// Bandwidth_lbl
			// 
			this->Bandwidth_lbl->AutoSize = true;
			this->Bandwidth_lbl->Enabled = false;
			this->Bandwidth_lbl->Location = System::Drawing::Point(99, 176);
			this->Bandwidth_lbl->Name = L"Bandwidth_lbl";
			this->Bandwidth_lbl->Size = System::Drawing::Size(102, 13);
			this->Bandwidth_lbl->TabIndex = 22;
			this->Bandwidth_lbl->Text = L"Bandwidth (Bytes/s)";
			// 
			// Bandwidth_txtbx
			// 
			this->Bandwidth_txtbx->BackColor = System::Drawing::SystemColors::Window;
			this->Bandwidth_txtbx->Location = System::Drawing::Point(12, 173);
			this->Bandwidth_txtbx->Name = L"Bandwidth_txtbx";
			this->Bandwidth_txtbx->ReadOnly = true;
			this->Bandwidth_txtbx->Size = System::Drawing::Size(81, 20);
			this->Bandwidth_txtbx->TabIndex = 21;
			// 
			// ElapsedTimer_lbl
			// 
			this->ElapsedTimer_lbl->AutoSize = true;
			this->ElapsedTimer_lbl->Enabled = false;
			this->ElapsedTimer_lbl->Location = System::Drawing::Point(99, 149);
			this->ElapsedTimer_lbl->Name = L"ElapsedTimer_lbl";
			this->ElapsedTimer_lbl->Size = System::Drawing::Size(93, 13);
			this->ElapsedTimer_lbl->TabIndex = 20;
			this->ElapsedTimer_lbl->Text = L"Elapsed Time (ms)";
			// 
			// ElapsedTime_txtbx
			// 
			this->ElapsedTime_txtbx->BackColor = System::Drawing::SystemColors::Window;
			this->ElapsedTime_txtbx->Location = System::Drawing::Point(12, 146);
			this->ElapsedTime_txtbx->Name = L"ElapsedTime_txtbx";
			this->ElapsedTime_txtbx->ReadOnly = true;
			this->ElapsedTime_txtbx->Size = System::Drawing::Size(81, 20);
			this->ElapsedTime_txtbx->TabIndex = 19;
			// 
			// BulkOut_btn
			// 
			this->BulkOut_btn->Location = System::Drawing::Point(12, 38);
			this->BulkOut_btn->Name = L"BulkOut_btn";
			this->BulkOut_btn->Size = System::Drawing::Size(172, 54);
			this->BulkOut_btn->TabIndex = 23;
			this->BulkOut_btn->Text = L"Send Bulk OUT Packets \r\n(960,000 Bytes)\r\nEP1 Only\r\n";
			this->BulkOut_btn->UseVisualStyleBackColor = true;
			this->BulkOut_btn->Click += gcnew System::EventHandler(this, &Form1::BulkOut_btn_Click);
			// 
			// SendMultiOUT_btn
			// 
			this->SendMultiOUT_btn->Location = System::Drawing::Point(190, 38);
			this->SendMultiOUT_btn->Name = L"SendMultiOUT_btn";
			this->SendMultiOUT_btn->Size = System::Drawing::Size(172, 54);
			this->SendMultiOUT_btn->TabIndex = 24;
			this->SendMultiOUT_btn->Text = L"Send Bulk OUT Packets\r\n(960,000 Bytes)\r\nEP1, EP2, EP3 Simultaneously";
			this->SendMultiOUT_btn->UseVisualStyleBackColor = true;
			this->SendMultiOUT_btn->Click += gcnew System::EventHandler(this, &Form1::SendMultiOUT_btn_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(430, 200);
			this->Controls->Add(this->SendMultiOUT_btn);
			this->Controls->Add(this->BulkOut_btn);
			this->Controls->Add(this->Bandwidth_lbl);
			this->Controls->Add(this->Bandwidth_txtbx);
			this->Controls->Add(this->ElapsedTimer_lbl);
			this->Controls->Add(this->ElapsedTime_txtbx);
			this->Controls->Add(this->DataXfer_chkbx);
			this->Controls->Add(this->StatusBox_lbl);
			this->Controls->Add(this->StatusBox_txtbx);
			this->Name = L"Form1";
			this->Text = L"High Bandwidth WinUSB Example (VID: 0x04D8, PID: 0x0052)";
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
								SendMultiOUT_btn->Enabled = TRUE;
								BulkOut_btn->Enabled = TRUE;
								Bandwidth_lbl->Enabled = TRUE;
								ElapsedTimer_lbl->Enabled = TRUE;
								DataXfer_chkbx->Enabled = TRUE;
								StatusBox_txtbx->Text = "Device Found: AttachedState = TRUE";
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
					SendMultiOUT_btn->Enabled = FALSE;
					BulkOut_btn->Enabled = FALSE;
					Bandwidth_lbl->Enabled = FALSE;
					ElapsedTimer_lbl->Enabled = FALSE;
					DataXfer_chkbx->Enabled = FALSE;
					Bandwidth_txtbx->Text = "";
					ElapsedTime_txtbx->Text = "";
					StatusBox_txtbx->Text = "Device Not Detected: Verify Connection/Correct Firmware";
				}				 
				 

			 }
		 }

		 Form::WndProc( m );
	}
//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

	private: System::Void SendMultiOUT_btn_Click(System::Object^  sender, System::EventArgs^  e) {
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
		 //The winusb.dll file exposes a variety of useful functions, the most important of which is WinUsb_WritePipe() and
		 //WinUsb_ReadPipe().  These are the actual functions that can be used by a user mode application (such as this one)
		 //to send/receive application related data to specific endpoints on the USB device.
		 //Before we can use the WinUsb_WritePipe() and WinUsb_ReadPipe() functions, we will have to have already obtained
		 //a WINUSB_INTERFACE_HANDLE for the USB device.  This is done by the CheckIfPresentAndGetUSBDevicePath() function.

		 //Endpoint PipeID: bottom 7 bits is the endpoint number (0-15), the MSb is the direction.  MSb = 0 for OUT endpoints, MSb = 1 for IN
		 //endpoints.  Ex: Endpoint 7 IN is "0x87", while endpoint 3 OUT is "0x03", while endpoint 1 OUT is 0x01.
		 
		 //Note: To achieve maximum bandwidth utilization through bulk transfers, it is necessary to send the data through more
		 //than one endpoint.  In theory, a single bulk endpoint can use all of the bandwidth, but in practice, the host
		 //will not schedule the traffic optimally for a single endpoint, and much of the available USB bus bandwidth
		 //can get wasted as bus idle time instead.

		 //The below code uses three bulk endpoints EP1 OUT, EP2 OUT, EP3 OUT, to simultaneously send a large
		 //block of data, to achieve high bandwidth approaching the theoretical maximum for full speed USB busses.
		 //For a full speed USB bus, the 12Mbits/s bit rate allows for up to 1.2Mbytes/second of application
		 //data, after accounting for USB protocol overhead (handshake packets, CRC fields, token packets, SOF packets, etc.).

		 //To achieve three simultaneous transfers over three separate endpoints, without using multiple threads, this
		 //demo uses "asynchronous" I/O operations, which makes the WinUsb_WritePipe() function non-blocking.  When
		 //used in synchronous I/O mode (no OVERLAPPED structures used), the WinUsb_WritePipe() function is blocking,
		 //and does not return until the I/O operation has completed.
			
		 //See MSDN documentation regarding "OVERLAPPED" structures and asynchronous I/O operations.

		SendMultiOUT_btn->Enabled = FALSE;	//Grey the button out until the operation completes.
		BulkOut_btn->Enabled = FALSE;		//Grey the button out until the operation completes.

		DWORD TimeTickerStartValue;
		DWORD TimeTickerStopValue;
		DWORD CumulativeBytesXferred = 0;
		ULONG BytesWritten = 0;
		ULONG BytesWritten2 = 0;
		ULONG BytesWritten3 = 0;
		UCHAR BooleanVar = TRUE;
		unsigned char OutputPacketBuffer[64000];	//Allocate a memory buffer which will contain data to send to the USB device
		unsigned char OutputPacketBuffer2[64000];
		unsigned char OutputPacketBuffer3[64000];

		//Initialize the buffers with some incrementing dummy data.  
		//A real application would want to fill these buffers with useful information instead.
		for(unsigned int i = 0; i < 64000; i++)	
		{
			OutputPacketBuffer[i] = (unsigned char)i;
			OutputPacketBuffer2[i] = (unsigned char)i;
			OutputPacketBuffer3[i] = (unsigned char)i;
		}

		//OVERLAPPED structures are used for asynchronous I/O operations.  See MSDN.
		OVERLAPPED OverlappedWriteStructure;
		OVERLAPPED OverlappedWriteStructure2;
		OVERLAPPED OverlappedWriteStructure3;
		HANDLE IOEvent = CreateEvent(NULL, TRUE, FALSE, NULL);	//Manual reset, initially non-signalled state
		HANDLE IOEvent2 = CreateEvent(NULL, TRUE, FALSE, NULL);	//Manual reset, initially non-signalled state
		HANDLE IOEvent3 = CreateEvent(NULL, TRUE, FALSE, NULL);	//Manual reset, initially non-signalled state

		DataXfer_chkbx->Checked = TRUE;

		//Note: This method of time tracking isn't perfect.  If this thread is interrupted for a long time, 
		//the timer keeps ticking, but USB traffic may not have started yet (if the interrupt occured between
		//reading the GetTickCount(), and when the I/O operations start).  This would have the effect of 
		//increasing the reported time, decreasing the apparent bandwidth calculation.  Therefore, the "actual"
		//bandwidth, during the time when the transmission is actually occuring, may actually be a bit
		//better than what this form reports.
		TimeTickerStartValue = GetTickCount();	

		for(unsigned int i = 0; i < 5; i++)
		{
			//Initialize the events and overlapped structures to their proper states before we use them.
			ResetEvent(IOEvent);
			ResetEvent(IOEvent2);
			ResetEvent(IOEvent3);

			OverlappedWriteStructure.Internal = 0;
			OverlappedWriteStructure.InternalHigh = 0;
			OverlappedWriteStructure.Offset = 0;
			OverlappedWriteStructure.OffsetHigh = 0;
			OverlappedWriteStructure.Pointer = 0;
			OverlappedWriteStructure.hEvent = IOEvent;

			OverlappedWriteStructure2.Internal = 0;
			OverlappedWriteStructure2.InternalHigh = 0;
			OverlappedWriteStructure2.Offset = 0;
			OverlappedWriteStructure2.OffsetHigh = 0;
			OverlappedWriteStructure2.Pointer = 0;
			OverlappedWriteStructure2.hEvent = IOEvent2;

			OverlappedWriteStructure3.Internal = 0;
			OverlappedWriteStructure3.InternalHigh = 0;
			OverlappedWriteStructure3.Offset = 0;
			OverlappedWriteStructure3.OffsetHigh = 0;
			OverlappedWriteStructure3.Pointer = 0;
			OverlappedWriteStructure3.hEvent = IOEvent3;

			//Actually start sending the data.
			HighBandwidthWinUSB::WinUsb_WritePipe(MyWinUSBInterfaceHandle, 0x01, &OutputPacketBuffer[0], 64000, &BytesWritten, &OverlappedWriteStructure);		//Send data OUT (of the host) on endpoint 1 (pipe ID = 0x01)
			HighBandwidthWinUSB::WinUsb_WritePipe(MyWinUSBInterfaceHandle, 0x02, &OutputPacketBuffer2[0], 64000, &BytesWritten2, &OverlappedWriteStructure2);  //Send data OUT (of the host) on endpoint 2 (pipe ID = 0x02)
			HighBandwidthWinUSB::WinUsb_WritePipe(MyWinUSBInterfaceHandle, 0x03, &OutputPacketBuffer3[0], 64000, &BytesWritten3, &OverlappedWriteStructure3);  //Send data OUT (of the host) on endpoint 3 (pipe ID = 0x03)
			
			//Wait for the data to finish being transferred.
			WaitForSingleObject(IOEvent, 9000);		//Blocking function until complete or timeout occurs.
			WaitForSingleObject(IOEvent2, 9000);	//Blocking function until complete or timeout occurs.
			WaitForSingleObject(IOEvent3, 9000);	//Blocking function until complete or timeout occurs.

			//Alternative method of waiting until the data is finished being transferred.
			//while(!HasOverlappedIoCompleted(&OverlappedWriteStructure) || !HasOverlappedIoCompleted(&OverlappedWriteStructure2) || !HasOverlappedIoCompleted(&OverlappedWriteStructure3));

			//At this point, all of the data was sent (assuming no error conditions, ex: user unplugged 
			//the USB cable during the transmission).  A complete application should ideally use error case 
			//checking/handling code to ensure a smooth user experience, even in the event of the unexpected.

			CumulativeBytesXferred += OverlappedWriteStructure.InternalHigh;
			CumulativeBytesXferred += OverlappedWriteStructure2.InternalHigh;
			CumulativeBytesXferred += OverlappedWriteStructure3.InternalHigh;

			BytesWritten = 0;	
			BytesWritten2 = 0;	
			BytesWritten3 = 0;	
		}		
		TimeTickerStopValue = GetTickCount();
		DataXfer_chkbx->Checked = FALSE;

		TimeTickerStopValue = TimeTickerStopValue - TimeTickerStartValue;

		ElapsedTime_txtbx->Text = TimeTickerStopValue.ToString();

		DWORD	BytesPerSecond;
		if(TimeTickerStopValue == 0)
			TimeTickerStopValue = 1;
		BytesPerSecond = (DWORD)(((CumulativeBytesXferred * 1000)/TimeTickerStopValue));
		Bandwidth_txtbx->Text = BytesPerSecond.ToString();

		CloseHandle(IOEvent);
		CloseHandle(IOEvent2);
		CloseHandle(IOEvent3);

		BulkOut_btn->Enabled = TRUE;		//Un-grey out the button.
		SendMultiOUT_btn->Enabled = TRUE;	//Un-grey out the button.


//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

			 }
private: System::Void BulkOut_btn_Click(System::Object^  sender, System::EventArgs^  e) {
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------

			//Send the same amount of total data as the SendMultiOUT_btn_Click() event handler function,
			//but in this case, only send the data through the EP1 OUT endpoint only.  In theory, a single
			//bulk endpoint can saturate all of the bus bandwidth.  In practice, this does not typically happen,
			//mainly due to host scheduling limitations.  
			
			 
			BulkOut_btn->Enabled = FALSE;			//Grey out the button temporarily.
			SendMultiOUT_btn->Enabled = FALSE;		//Grey out the button temporarily.

			ULONG BytesWritten = 0;				

			DWORD TimeTickerStartValue;
			DWORD TimeTickerStopValue;
			DWORD CumulativeBytesXferred = 0;
			unsigned char OutputPacketBuffer[64000];	//Allocate a memory buffer which will contain data to send to the USB device

			//Initialize the buffer with some incrementing dummy data.  
			//A real application would want to fill this buffer with useful information instead.
			for(unsigned int i = 0; i < 64000; i++)	
			{
				OutputPacketBuffer[i] = (unsigned char)i;
			}

			DataXfer_chkbx->Checked = TRUE;

			//Note: This method of time tracking isn't perfect.  If this thread is interrupted for a long time, 
			//the timer keeps ticking, but USB traffic may not have started yet (if the interrupt occured between
			//reading the GetTickCount(), and when the I/O operations start).  This would have the effect of 
			//increasing the reported time, decreasing the apparent bandwidth calculation.  Therefore, the "actual"
			//bandwidth, during the time when the transmission is actually occuring, may actually be a bit
			//better than what this form reports.
			TimeTickerStartValue = GetTickCount();	

			for(unsigned int i = 0; i < 15; i++)
			{
				if(WinUsb_WritePipe(MyWinUSBInterfaceHandle, 0x01, &OutputPacketBuffer[0], 64000, &BytesWritten, NULL))		//Send data OUT (of the host) on endpoint 1 (pipe ID = 0x01).  Synchronous I/O operation.  This is a blocking function until the I/O operation completes.
				{
					CumulativeBytesXferred += BytesWritten;
					BytesWritten = 0;
				}
				else
				{
					//Some kind of unexpected error occurred (ex: user unplugged USB cable during the transmission).
					//For smooth user interface behavior, consider using application specific error case handling code.
				}
			}		
			TimeTickerStopValue = GetTickCount();
			DataXfer_chkbx->Checked = FALSE;

			TimeTickerStopValue = TimeTickerStopValue - TimeTickerStartValue;
			ElapsedTime_txtbx->Text = TimeTickerStopValue.ToString();

			DWORD BytesPerSecond;
			if(TimeTickerStopValue == 0)
				TimeTickerStopValue = 1;
			BytesPerSecond = (DWORD)(((CumulativeBytesXferred * 1000)/TimeTickerStopValue));
			Bandwidth_txtbx->Text = BytesPerSecond.ToString();

			SendMultiOUT_btn->Enabled = TRUE;	//Un-grey out the button.
			BulkOut_btn->Enabled = TRUE;		//Un-grey out the button.

//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		 }
};
}

