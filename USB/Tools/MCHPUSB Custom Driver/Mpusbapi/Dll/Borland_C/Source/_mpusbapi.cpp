/*********************************************************************
 *
 *                  MPUSBAPI Library
 *
 *********************************************************************
 * FileName:        _mpusbapi.cpp
 * Dependencies:    See #include section below.
 * Compiler:        Borland C++ Builder 6
 * Company:         Copyright (C) 2010 by Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * revision             Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * v0.0.0.0             9/2/04      Implemented MPUSBGetDeviceLink()
 * v1.0.0.0             11/19/04    Original version 1.00 completed
 * v1.0.1.0             03/24/08    Slight update to fix issue which
 *                                  was requiring admin run level to
 *                                  work correctly.
 * v1.1.0.0             05/22/08    Added the support for the
 *                                  following functions:
 *                                   MPUSBSetConfiguration()
 *                                   MPUSBGetDeviceDescriptor()
 *                                   MPUSBGetConfigurationDescriptor()
 *                                   MPUSBGetStringDescriptor()
 *                                  Modified MPUSBGetDLLVersion return
 *                                   format.
 * v1.2.0.0             02/02/09    Fixed bugs in MPUSBRead() function
 * v1.2.1.0             08/18/09    Added MPUSBSetPowerState() function
 * v6.0.0.0             14 Oct 2010 Implemented WinUSB + mchpusb driver capability.
 *                                  Should retain backwards compatibility with v1.2.1.0,
 *                                  while also adding the ability to use WinUSB driver instead of mchpusb,
 *                                  without having to rebuild the target PC application source code using this DLL.
 *                                  This potentially allows for seamless migration between the two drivers.
 ********************************************************************/

#include <stdio.h>
#include <windows.h>    //Definitions for various common and not so common types like DWORD, PCHAR, HANDLE, etc.
#include <string.h>
#include <setupapi.h>   //From Windows Server 2003 R2 Platform SDK.  Untested with Windows SDK.  Setupapi.h provides
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
#include <initguid.h>
#include <winioctl.h>
#include "ioctls.h"     //This file is modified compared to mchpusb version.  Contains GUID for the WinUSB device, instead of the mchpusb GUID device...
#include "_mpusbapi.h"
						//IMPORTANT: READ THIS BEFORE TRYING TO BUILD THIS CODE
						//----------------------------------------------------
#include "Winusb_modified.h"		        //Winusb.h comes as a part of the Windows Driver Kit (WDK) build 6001.18001 (and later versions).
                                                //This project is using a modified version of Winusb.h however, since the original has function
                                                //prototypes in a format this compiler doesn't like.
						//The WDK is currently a free download from http://connect.microsoft.com.  Please follow these steps in order
						//  to find the WDK.
						//  1) go to http://connect.microsoft.com
						//  2) click the sign-in button.  If you already have a Windows Live ID then sign in with that.
						//		If not then create one now.
						//	3) Once signed in it should bring you to your dashboard.  Click on "CONNECTION DIRECTORY"
						//  4) find the WDK on the list.
						//  5) Click "Apply Now".  This should bring you to the WDK home page
						//  6) Click "Downloads" on the left menu bar
						//  7) Click on "WDK for Server 2008"
						//You will need the WDK build 7600.16385.0 (or later) installed on
						//your computer to use this source code.  Once you have it installed, you will also need to add
						//the include path directories from the WDK to your C++ IDE.
						//C:\WINDDK\7600.16385.0\inc\ddk
						//C:\WINDDK\7600.16385.0\inc\api
						//The above directory locations assume the default location for the WDK.
						//If the above procedure is not followed correctly, a variety of build errors looking for various
						//files such as winusbio.h, usb200.h, usb100.h, usb.h, etc. will occur.



#define MPUSBAPI_VERSION            0x06000000L     //revision 6.0.0.0 This version merges WinUSB + Microchip custom class driver functionality.  This version of mpusbapi.dll will work with either driver installed.




#define	MPUSB_DEV_NO_INFO           2
#define	MPUSB_DEV_INVALID_INST      3
#define	MPUSB_DEV_VIDPID_NOT_FOUND  4
#define MPUSB_DEV_NOT_ATTACHED      5

DEFINE_GUID(GUID_DEVINTERFACE_MCHPUSB, 0x5354fa28L, 0x6d14, 0x4e35, 0xa1, 0xf5, 0x75, 0xbb, 0x54, 0xe6, 0x03, 0x0f);    //GUID for the mchpusb.sys devices.
DEFINE_GUID(GUID_DEVINTERFACE_WINUSB, 0x58d07210, 0x27c1, 0x11dd, 0xbd, 0x0b, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66);      //This is the guid used by the WinUSB installer inf file for this device instead.

BOOL UsingWinUSBDriver = FALSE;
BOOL WinUSBLibraryLoaded = FALSE;
HANDLE GlobalWinUSBDeviceHandle = INVALID_HANDLE_VALUE;
HANDLE GlobalWinUSBInterfaceHandle = INVALID_HANDLE_VALUE;
BOOL WinUSBInterfaceInitialized = FALSE;
UINT ApparentOpenPipes = 0;
HINSTANCE libHandle;

typedef BOOL __stdcall (*WinUsb_init)(HANDLE, PWINUSB_INTERFACE_HANDLE);
WinUsb_init WinUsbDLL_Initialize;
typedef BOOL __stdcall (*WinUsb_readpipe)(WINUSB_INTERFACE_HANDLE, UCHAR, PUCHAR, ULONG, PULONG, LPOVERLAPPED);
WinUsb_readpipe WinUsbDLL_ReadPipe;
typedef BOOL __stdcall (*WinUsb_writepipe)(WINUSB_INTERFACE_HANDLE, UCHAR, PUCHAR, ULONG, PULONG, LPOVERLAPPED);
WinUsb_writepipe WinUsbDLL_WritePipe;
typedef BOOL __stdcall (*WinUsb_abortpipe)(WINUSB_INTERFACE_HANDLE, UCHAR);
WinUsb_abortpipe WinUsbDLL_AbortPipe;
typedef BOOL __stdcall (*WinUsb_free)(WINUSB_INTERFACE_HANDLE);
WinUsb_free WinUsbDLL_Free;
typedef BOOL __stdcall (*WinUsb_setpowerpolicy)(WINUSB_INTERFACE_HANDLE, ULONG, ULONG, PVOID);
WinUsb_setpowerpolicy WinUsbDLL_SetPowerPolicy;
typedef BOOL __stdcall  (*WinUsb_GetDescriptor)(WINUSB_INTERFACE_HANDLE, UCHAR, UCHAR, USHORT, PUCHAR, ULONG, PULONG);
WinUsb_GetDescriptor WinUsbDLL_GetDescriptor;



//Private protoypes
HANDLE WinUSB_MPUSBOpen(DWORD instance, PCHAR pVID_PID, PCHAR pEP, DWORD dwDir, DWORD dwReserved);
DWORD WinUSB_MPUSBGetDescriptor(HANDLE, PVOID, DWORD, PVOID, DWORD, PDWORD);
DWORD WinUSB_MPUSBGetDeviceDescriptor(HANDLE, PVOID, DWORD, PDWORD);
DWORD WinUSB_MPUSBGetConfigurationDescriptor(HANDLE, UCHAR, PVOID, DWORD, PDWORD);
DWORD WinUSB_MPUSBGetStringDescriptor(HANDLE, UCHAR, USHORT, PVOID, DWORD, PDWORD);
DWORD WinUSB_MPUSBGetConfigInfoSize(HANDLE);
DWORD WinUSB_MPUSBGetConfigInfo(HANDLE, PVOID, DWORD);
DWORD WinUSB_MPUSBSetConfiguration(HANDLE, USHORT);
DWORD WinUSB_MPUSBRead(HANDLE, PVOID, DWORD, PDWORD, DWORD);
DWORD WinUSB_MPUSBWrite(HANDLE, PVOID, DWORD, PDWORD, DWORD);
DWORD WinUSB_MPUSBReadInt(HANDLE, PVOID, DWORD, PDWORD, DWORD);
BOOL WinUSB_MPUSBClose(HANDLE);


#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, DWORD reason, void* lpReserved)
{
    if(reason == DLL_PROCESS_ATTACH) //Don't want to execute the below again in the case of other reasons like "DLL_THREAD_ATTACH"
    {
        UsingWinUSBDriver = FALSE;  //Assume we will be using mchpusb driver, unless we later determine we are actually using WinUSB driver
        WinUSBLibraryLoaded = FALSE;
        GlobalWinUSBDeviceHandle = INVALID_HANDLE_VALUE;
        GlobalWinUSBInterfaceHandle = INVALID_HANDLE_VALUE;
        WinUSBInterfaceInitialized = FALSE;
        ApparentOpenPipes = 0;

        libHandle = NULL;
        libHandle = LoadLibrary("winusb.dll");
        if(libHandle == NULL)
        {
            //Couldn't load winusb.dll in this case...
            UsingWinUSBDriver = FALSE;  //Assume we are using mchpusb driver instead in this case.
            WinUSBLibraryLoaded = FALSE;
            return 1;     //return 1 anyway.  May still be able to use mchpusb.sys driver if it is running instead.
        }
        else
        {
            WinUsbDLL_Initialize=(WinUsb_init)GetProcAddress(libHandle,"WinUsb_Initialize");
            WinUsbDLL_ReadPipe=(WinUsb_readpipe)GetProcAddress(libHandle,"WinUsb_ReadPipe");
            WinUsbDLL_WritePipe=(WinUsb_writepipe)GetProcAddress(libHandle,"WinUsb_WritePipe");
            WinUsbDLL_AbortPipe=(WinUsb_abortpipe)GetProcAddress(libHandle,"WinUsb_AbortPipe");
            WinUsbDLL_Free=(WinUsb_free)GetProcAddress(libHandle,"WinUsb_Free");
            WinUsbDLL_SetPowerPolicy=(WinUsb_setpowerpolicy)GetProcAddress(libHandle,"WinUsb_SetPowerPolicy");      //WDK documentation (6001.18001 is wrong.  It claims there is a "WinUsb_SetInterfacePowerPolicy" function, but dumpbin does not show such a function.
            WinUsbDLL_GetDescriptor=(WinUsb_GetDescriptor)GetProcAddress(libHandle,"WinUsb_GetDescriptor");
        }//end if else

        //Now check to make we successfully loaded the function address entry points.
        //To avoid application errors during runtime, we should make sure not to try and call
        //a function if we didn't successfully load the function entry point addresses.
        if((WinUsbDLL_Initialize == NULL) || (WinUsbDLL_ReadPipe == NULL) || (WinUsbDLL_WritePipe == NULL) || (WinUsbDLL_AbortPipe == NULL) || (WinUsbDLL_Free == NULL) || (WinUsbDLL_GetDescriptor == NULL))
        {
            WinUSBLibraryLoaded = FALSE;
            UsingWinUSBDriver = FALSE;  //Assume we are using mchpusb driver instead in this case.
            return 1;
        }
        else
        {
            WinUSBLibraryLoaded = TRUE;
        }
    }

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
//	MPUSBGetDLLVersion : get mpusbapi.dll revision level
//
//	Input:
//		None
//	Output:
//		32-bit revision level MMmmddii
//                 MM - Major release
//                 mm - Minor release
//                 dd - dot release or minor fix
//                 ii - test release revisions
//      Note:
//              The formatting changed between revisions v1.0.1.0 of the
//                driver and v1.1.0.0.  The output of this function was
//                previously MMMMmmmm and did not match the .DLL file
//                numbering format.  The format of this fuction was changed to
//                match how the .DLL file number generation works.
//

DWORD MPUSBGetDLLVersion(void)
{
    return MPUSBAPI_VERSION;
}//end MPUSBGetDLLVersion


///////////////////////////////////////////////////////////////////////////////
//	MPUSBIsVidPidEqual : Compares the pVID_PID string against the DeviceInstance
//  string retrieved from the registry using the DevicePath subkey.
//  This function should be called only from MPUSBGetDevicePath().
//
//  Note:
//  All Windows version has the DeviceClasses information stored in:
//  HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\DeviceClasses\\
//  {GUID_DEVINTERFACE_MCHPUSB}\\<DevicePath>
//  Win98SE,ME have different DevicePath string format from 2K,XP.
//  It does not contain vid&pid information in the DevicePath.
//  Thus necessitating the needs to check the DeviceInstance string in the
//  registry.
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
DWORD MPUSBIsVidPidEqual(PCHAR pDevicePath, PCHAR pVID_PID)
{
    DWORD dwResult = MPUSB_FAIL;
    char lszValue[255];
    char lpSubKey[512];
    char LowerCaseVIDPID[17] = "vid_XXXX&pid_YYYY";     //Intermediate space, so as to avoid modifying the pVID_PID string.  It is only an input to this function.

    HKEY hKey;
    LONG returnStatus;
    DWORD dwType=REG_SZ;
    DWORD dwSize=255;

    //First check for MCHPUSB based GUID.
    GUID guid = GUID_DEVINTERFACE_MCHPUSB;

    if(strlen(pVID_PID) < 50)  //Make sure it isn't too long.  Should only be (17+Null terminator) characters long.
    {
       strcpy(LowerCaseVIDPID, pVID_PID);
       strlwr(LowerCaseVIDPID);
    }

    /* Modify DevicePath to use registry format */
    pDevicePath[0] = '#';
    pDevicePath[1] = '#';
    pDevicePath[3] = '#';

    /* Form SubKey */
    wsprintf(lpSubKey,
    "SYSTEM\\CURRENTCONTROLSET\\CONTROL\\DEVICECLASSES\\{%4.2x-%2.2x-%2.2x-%.2x%.2x-%.2x%.2x%.2x%.2x%.2x%.2x}\\%s",
    guid.Data1,guid.Data2,guid.Data3,guid.Data4[0],guid.Data4[1],guid.Data4[2],
    guid.Data4[3],guid.Data4[4],guid.Data4[5],guid.Data4[6],guid.Data4[7],pDevicePath);

    /* Open Key */
    returnStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                lpSubKey,
                                0L,
                                KEY_QUERY_VALUE,        //Changed from v1.0.  Used to be "KEY_ALL_ACCESS" which requires administrator logon (and application administrator execution level to work in Vista).
                                &hKey);
    if(returnStatus == ERROR_SUCCESS)
    {
        returnStatus = RegQueryValueEx(hKey,
                                       "DeviceInstance",
                                       NULL,
                                       &dwType,
                                       (LPBYTE)&lszValue,
                                       &dwSize);
        if(returnStatus == ERROR_SUCCESS)
        {
            /*
             * The string info stored in 'DeviceInstance' is the same
             * across all Windows platforms: 98SE, ME, 2K, and XP.
             * Upper-case in 98SE,ME.
             * Converts all to lower-case anyway.
             */
            strlwr(lszValue);
            if(strstr(lszValue,LowerCaseVIDPID) != NULL)
            {
                dwResult = MPUSB_SUCCESS;
            }
        }
    }
    RegCloseKey(hKey);

    /* Modify DevicePath to use the original format */
    pDevicePath[0] = '\\';
    pDevicePath[1] = '\\';
    pDevicePath[3] = '\\';

    if(dwResult == MPUSB_SUCCESS)
    {
        UsingWinUSBDriver = FALSE;
        return dwResult;
    }

    //If we get to here, we didn't find the mchpusb interface attached.  However,
    //there still might be a WinUSB interface attached instead.  Need to check for
    //the WinUSB interface now.
    guid = GUID_DEVINTERFACE_WINUSB;

    if(strlen(pVID_PID) < 50)  //Make sure it isn't too long.  Should only be (17+Null terminator) characters long.
    {
       strcpy(LowerCaseVIDPID, pVID_PID);
       strlwr(LowerCaseVIDPID);
    }

    /* Modify DevicePath to use registry format */
    pDevicePath[0] = '#';
    pDevicePath[1] = '#';
    pDevicePath[3] = '#';

    /* Form SubKey */
    wsprintf(lpSubKey,
    "SYSTEM\\CURRENTCONTROLSET\\CONTROL\\DEVICECLASSES\\{%4.2x-%2.2x-%2.2x-%.2x%.2x-%.2x%.2x%.2x%.2x%.2x%.2x}\\%s",
    guid.Data1,guid.Data2,guid.Data3,guid.Data4[0],guid.Data4[1],guid.Data4[2],
    guid.Data4[3],guid.Data4[4],guid.Data4[5],guid.Data4[6],guid.Data4[7],pDevicePath);

    /* Open Key */
    returnStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                lpSubKey,
                                0L,
                                KEY_QUERY_VALUE,        //Changed from v1.0.  Used to be "KEY_ALL_ACCESS" which requires administrator logon (and application administrator execution level to work in Vista).
                                &hKey);
    if(returnStatus == ERROR_SUCCESS)
    {
        returnStatus = RegQueryValueEx(hKey,
                                       "DeviceInstance",
                                       NULL,
                                       &dwType,
                                       (LPBYTE)&lszValue,
                                       &dwSize);
        if(returnStatus == ERROR_SUCCESS)
        {
            /*
             * The string info stored in 'DeviceInstance' is the same
             * across all Windows platforms: 98SE, ME, 2K, and XP.
             * Upper-case in 98SE,ME.
             * Converts all to lower-case anyway.
             */
            strlwr(lszValue);
            if(strstr(lszValue,LowerCaseVIDPID) != NULL)
            {
                dwResult = MPUSB_SUCCESS;
            }
        }
    }
    RegCloseKey(hKey);

    /* Modify DevicePath to use the original format */
    pDevicePath[0] = '\\';
    pDevicePath[1] = '\\';
    pDevicePath[3] = '\\';

    if(dwResult == MPUSB_SUCCESS)
    {
        UsingWinUSBDriver = TRUE;
    }

    return dwResult;

}//end

///////////////////////////////////////////////////////////////////////////////
//	MPUSBGetDeviceLink : Returns the path to device hardware with a given
//  instance number.
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
DWORD MPUSBGetDeviceLink(DWORD instance,    // Input
                         PCHAR pVID_PID,    // Input
                         PCHAR pPath,       // Output
                         DWORD dwLen,       // Input
                         PDWORD pLength)    // Output
{
    if(pLength != NULL)*pLength = 0;        // Initialization

    HDEVINFO info = SetupDiGetClassDevs((LPGUID)&GUID_DEVINTERFACE_MCHPUSB, NULL, NULL, DIGCF_PRESENT|DIGCF_DEVICEINTERFACE);
    if(info==INVALID_HANDLE_VALUE)
    {
        SetupDiDestroyDeviceInfoList(info);
    	return MPUSB_DEV_NO_INFO;
    }// end if

	// Get interface data for the requested instance
	SP_DEVICE_INTERFACE_DATA intf_data;
	intf_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	if(!SetupDiEnumDeviceInterfaces(info, NULL, (LPGUID)&GUID_DEVINTERFACE_MCHPUSB, instance, &intf_data))
	{
                //Failed to find a match.
		SetupDiDestroyDeviceInfoList(info);
                //Now check for a matching WinUSB device instead.
                info = SetupDiGetClassDevs((LPGUID)&GUID_DEVINTERFACE_WINUSB, NULL, NULL, DIGCF_PRESENT|DIGCF_DEVICEINTERFACE);
                if(info==INVALID_HANDLE_VALUE)
                {
                        SetupDiDestroyDeviceInfoList(info);
    	                return MPUSB_DEV_NO_INFO;
                }// end if

        	// Get interface data for the requested instance
        	intf_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

                if(!SetupDiEnumDeviceInterfaces(info, NULL, (LPGUID)&GUID_DEVINTERFACE_WINUSB, instance, &intf_data))
                {
                        //Failed to find a match of either mchpusb or winusb.
        		SetupDiDestroyDeviceInfoList(info);
        		return MPUSB_DEV_INVALID_INST;
                }

	}// end if

	// Get size of symbolic link
	DWORD ReqLen;
	SetupDiGetDeviceInterfaceDetail(info, &intf_data, NULL, 0, &ReqLen, NULL);

	PSP_DEVICE_INTERFACE_DETAIL_DATA intf_detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)(new char[ReqLen]);

	if( intf_detail == NULL)
	{
		SetupDiDestroyDeviceInfoList(info);
                delete intf_detail;
		return MPUSB_DEV_NO_INFO;
	}// end if

    // Get symbolic link name
	intf_detail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
    // sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) should equals 5.
    // In C++ Builder, go to Project/Options/Advanced Compiler/Data Alignment
    // and select "byte" align.

	if(!SetupDiGetDeviceInterfaceDetail(info, &intf_data, intf_detail, ReqLen, NULL, NULL))
	{
		SetupDiDestroyDeviceInfoList(info);
		delete intf_detail;
		return MPUSB_DEV_NO_INFO;
	}// end if

    // Check for a valid VID&PID - if argument is not null)
    if(pVID_PID != NULL)
    {
        if(MPUSBIsVidPidEqual(intf_detail->DevicePath, pVID_PID) == NULL)
        {
            SetupDiDestroyDeviceInfoList(info);
		    delete intf_detail;
		    return MPUSB_DEV_VIDPID_NOT_FOUND;
        }// end if
    }// end if

	// Set the length of the path string
	if(pLength != NULL)
        *pLength = (DWORD)strlen(intf_detail->DevicePath);

    // Copy output string path to buffer pointed to by pPath
    if(pPath != NULL)
    {
        // Check that input buffer has enough room...
        // Use > not >= because strlen does not include null
        if(dwLen > strlen(intf_detail->DevicePath))
	        strcpy(pPath, intf_detail->DevicePath);
        else
        {
            SetupDiDestroyDeviceInfoList(info);
            delete intf_detail;
            return MPUSB_FAIL;
        }// end if
    }// end if

	// Clean up
	SetupDiDestroyDeviceInfoList(info);
	delete intf_detail;
	return MPUSB_SUCCESS;

}// end MPUSBGetDeviceLink

///////////////////////////////////////////////////////////////////////////////
//	MPUSBGetDeviceCount : Returns the number of devices with matching VID & PID
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
DWORD MPUSBGetDeviceCount(PCHAR pVID_PID)
{
    DWORD count;        // Number of USB device with matching VID & PID
    count = 0;          // Initialization

    for(int i = 0; i < MAX_NUM_MPUSB_DEV; i++)
    {
        if(MPUSBGetDeviceLink(i,pVID_PID,NULL,NULL,NULL) == MPUSB_SUCCESS)
            count++;
    }//end for
    return count;
}//end MPUSBGetDeviceCount

///////////////////////////////////////////////////////////////////////////////
//	MPUSBOpen : Returns the handle to the endpoint pipe with matching VID & PID
//
//  All pipes are opened with the FILE_FLAG_OVERLAPPED attribute.
//  This allows MPUSBRead,MPUSBWrite, and MPUSBReadInt to have a time-out value.
//
//  Note: Time-out value has no meaning for Isochronous pipes.
//
//  instance - An instance number of the device to open.
//             Typical usage is to call MPUSBGetDeviceCount first to find out
//             how many instances there are.
//             It is important to understand that the driver is shared among
//             different devices. The number of devices returned by
//             MPUSBGetDeviceCount could be equal to or less than the number
//             of all the devices that are currently connected & using the
//             generic driver.
//
//             Example:
//             if there are 3 device with the following PID&VID connected:
//             Device Instance 0, VID 0x04d8, PID 0x0001
//             Device Instance 1, VID 0x04d8, PID 0x0002
//             Device Instance 2, VID 0x04d8, PID 0x0001
//
//             If the device of interest has VID = 0x04d8 and PID = 0x0002
//             Then MPUSBGetDeviceCount will only return '1'.
//             The calling function should have a mechanism that attempts
//             to call MPUSBOpen up to the absolute maximum of MAX_NUM_MPUSB_DEV
//             (MAX_NUM_MPUSB_DEV is defined in _mpusbapi.h).
//             It should also keep track of the number of successful calls
//             to MPUSBOpen(). Once the number of successes equals the
//             number returned by MPUSBGetDeviceCount, the attempts should
//             be aborted because there will no more devices with
//             a matching vid&pid left.
//
//  pVID_PID - A string containing the PID&VID value of the target device.
//             The format is "vid_xxxx&pid_yyyy". Where xxxx is the VID value
//             in hex and yyyy is the PID value in hex.
//             Example: If a device has the VID value of 0x04d8 and PID value
//                      of 0x000b, then the input string should be:
//                      "vid_04d8&pid_000b"
//
//  pEP      - A string of the endpoint number on the target endpoint to open.
//             The format is "\\MCHP_EPz". Where z is the endpoint number in
//             decimal.
//             Example: "\\MCHP_EP1"
//
//             This arguement can be NULL. A NULL value should be used to
//             create a handles for non-specific endpoint functions.
//             MPUSBRead, MPUSBWrite, MPUSBReadInt are endpoint specific
//             functions.
//             All others are not.
//             Non-specific endpoint functions will become available in the
//             next release of the DLL.
//
//             Note: To use MPUSBReadInt(), the format of pEP has to be
//                   "\\MCHP_EPz_ASYNC". This option is only available for
//                   an IN interrupt endpoint. A data pipe opened with the
//                   "_ASYNC" keyword would buffer the data at the interval
//                   specified in the endpoint descriptor upto the maximum of
//                   100 data sets. Any data received after the driver buffer
//                   is full will be ignored.
//                   The user application should call MPUSBReadInt() often
//                   enough so that the maximum limit of 100 is never reached.
//
//  dwDir    - Specifies the direction of the endpoint.
//             Use MP_READ for MPUSBRead, MPSUBReadInt
//             Use MP_WRITE for MPUSBWrite
//
//  dwReserved Future Use
//
//  Summary of transfer type usage:
//  ============================================================================
//  Transfer Type       Functions                       Time-Out Applicable?
//  ============================================================================
//  Interrupt - IN      MPUSBRead, MPUSBReadInt         Yes
//  Interrupt - OUT     MPUSBWrite                      Yes
//  Bulk - IN           MPUSBRead                       Yes
//  Bulk - OUT          MPUSBWrite                      Yes
//  Isochronous - IN    MPUSBRead                       No
//  Isochronous - OUT   MPUSBWrite                      No
//  ============================================================================
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
///////////////////////////////////////////////////////////////////////////////
//Entrypoint for any MPUSBOpen() call.  Also implements the MCHPUSB based version of MPUSBOpen().
HANDLE MPUSBOpen(DWORD instance,    // Input
                 PCHAR pVID_PID,    // Input
                 PCHAR pEP,         // Input
                 DWORD dwDir,       // Input
                 DWORD dwReserved)  // Input <Future Use>
{
    HANDLE WinUSBReturnHandle;

    //If WinUSB library was successfully loaded, make an initial attempt to connect to the
    //device using the WinUSB API.  If this succeeds, this means the device is attached
    //and the WinUSB driver has been loaded for the device.  If this fails, this could
    //mean either the device isn't connected at all, or that the device is using the mchpusb
    //driver instead.  In this case, we need to make another attempt to connect to it,
    //using the method appropriate for the mchpusb driver instead.
    if(WinUSBLibraryLoaded == TRUE)
    {
        WinUSBReturnHandle = WinUSB_MPUSBOpen(instance, pVID_PID, pEP, dwDir, dwReserved);
        if(UsingWinUSBDriver == TRUE)
        {
                return WinUSBReturnHandle;
        }
    }

    char path[MAX_PATH];
    DWORD dwReqLen;

    HANDLE handle;
    handle = INVALID_HANDLE_VALUE;

    // Check arguments first
    if((pVID_PID != NULL) && ((dwDir == MP_WRITE) || (dwDir == MP_READ)))
    {
        if(MPUSBGetDeviceLink(instance,pVID_PID,path,MAX_PATH,&dwReqLen)==\
            MPUSB_SUCCESS)
        {
            char path_io[MAX_PATH];
            strcpy(path_io,path);
            if(pEP != NULL) strcat(path_io,pEP);

            if(dwDir == MP_READ)
            {
                handle = CreateFile(path_io,
                                    GENERIC_READ,
                                    0,
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
                                    NULL);
            }
            else
            {
                handle = CreateFile(path_io,
                                    GENERIC_WRITE,
                                    0,
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
                                    NULL);
            }//end if
        }//end if
    }//end if
    return handle;
}//end MPUSBOpen(...)


///////////////////////////////////////////////////////////////////////////////
//Function: WinUSB_MPUSBOpen()
//Note: API equivalent of MPUSBOpen().  Used for when using the WinUSB driver instead of mchpusb.
//
//      NOTE: This modified version (using WinUSB) of MPUSBOpen does not actually return a HANDLE.
//      Due to differences in the native API of WinUSB and MPUSBAPI, we return a value that
//      contains the info for the endpoint that will be used instead.  This will be used by the MPUSBRead() and
//      MPUSBWrite() functions to determine which pipe to send/receive the data to/from.
///////////////////////////////////////////////////////////////////////////////
HANDLE WinUSB_MPUSBOpen(DWORD instance,    // Input
                 PCHAR pVID_PID,    // Input
                 PCHAR pEP,         // Input
                 DWORD dwDir,       // Input
                 DWORD dwReserved)  // Input <Future Use>
{
    char path[MAX_PATH];
    DWORD dwReqLen;
    BOOL BoolStatus;
    DWORD ErrorStatus;
    HANDLE EPNumNotRealHandle = INVALID_HANDLE_VALUE;
    ULONG ValueLength = 1;      //Used by WinUsbDLL_SetInterfacePowerPolicy()
    UCHAR InterfacePowerPolicyValue;

    if(pEP != NULL) //Make sure pointer is valid before using it.
    {
            if((strstr(pEP, "\\MCHP_EP0") != NULL) || (strstr(pEP, "\\mchp_ep0") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x80;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x00;
            }

            if((strcmp(pEP, "\\MCHP_EP1") == 0) || (strcmp(pEP, "\\MCHP_EP1_ASYNC") == 0))  //Make sure the string is exactly the right length, otherwise it might be a "\\MCHP_EP11" or other EP1x endpoint instead of EP1
            {
                if((strstr(pEP, "\\MCHP_EP1") != NULL) || (strstr(pEP, "\\mchp_ep1") != NULL))
                    {
                      if(dwDir == MP_READ)
                              (int)EPNumNotRealHandle = 0x81;
                      else if(dwDir == MP_WRITE)
                              (int)EPNumNotRealHandle = 0x01;
                    }
            }

            if((strstr(pEP, "\\MCHP_EP2") != NULL) || (strstr(pEP, "\\mchp_ep2") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x82;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x02;
            }
            if((strstr(pEP, "\\MCHP_EP3") != NULL) || (strstr(pEP, "\\mchp_ep3") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x83;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x03;
            }
            if((strstr(pEP, "\\MCHP_EP4") != NULL) || (strstr(pEP, "\\mchp_ep4") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x84;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x04;
            }
            if((strstr(pEP, "\\MCHP_EP5") != NULL) || (strstr(pEP, "\\mchp_ep5") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x85;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x05;
            }
            if((strstr(pEP, "\\MCHP_EP6") != NULL) || (strstr(pEP, "\\mchp_ep6") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x86;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x06;
            }
            if((strstr(pEP, "\\MCHP_EP7") != NULL) || (strstr(pEP, "\\mchp_ep7") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x87;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x07;
            }
            if((strstr(pEP, "\\MCHP_EP8") != NULL) || (strstr(pEP, "\\mchp_ep8") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x88;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x08;
            }
            if((strstr(pEP, "\\MCHP_EP9") != NULL) || (strstr(pEP, "\\mchp_ep9") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x89;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x09;
            }
            if((strstr(pEP, "\\MCHP_EP10") != NULL) || (strstr(pEP, "\\mchp_ep10") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x8A;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x0A;
            }
            if((strstr(pEP, "\\MCHP_EP11") != NULL) || (strstr(pEP, "\\mchp_ep11") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x8B;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x0B;
            }
            if((strstr(pEP, "\\MCHP_EP12") != NULL) || (strstr(pEP, "\\mchp_ep12") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x8C;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x0C;
            }
            if((strstr(pEP, "\\MCHP_EP13") != NULL) || (strstr(pEP, "\\mchp_ep13") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x8D;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x0D;
            }
            if((strstr(pEP, "\\MCHP_EP14") != NULL) || (strstr(pEP, "\\mchp_ep14") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x8E;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x0E;
            }
            if((strstr(pEP, "\\MCHP_EP15") != NULL) || (strstr(pEP, "\\mchp_ep15") != NULL))
            {
              if(dwDir == MP_READ)
                      (int)EPNumNotRealHandle = 0x8F;
              else if(dwDir == MP_WRITE)
                      (int)EPNumNotRealHandle = 0x0F;
            }
    } //end if(pEP != NULL)


    // Check arguments first
    if((pVID_PID != NULL) && ((dwDir == MP_WRITE) || (dwDir == MP_READ)))
    {
        if(MPUSBGetDeviceLink(instance,pVID_PID,path,MAX_PATH,&dwReqLen) == MPUSB_SUCCESS)
        {
             char path_io[MAX_PATH];
             strcpy(path_io,path);

             //We now have the proper device path, and we can finally open a device handle to the device.
	     //WinUSB requires the device handle to be opened with the FILE_FLAG_OVERLAPPED attribute.
             //This MPUSBOpen function may have already been called.  In this case, the WinUSB device and interface handles may have already been opened.
             //in this case, don't want to try to open them again.
             if((GlobalWinUSBDeviceHandle == INVALID_HANDLE_VALUE) || (WinUSBInterfaceInitialized == FALSE))
             {
	        GlobalWinUSBDeviceHandle = CreateFile(path_io, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
        	if(GetLastError() == ERROR_SUCCESS)
                {
        	        //Now get the WinUSB interface handle by calling WinUsb_Initialize() and providing the device handle.
        		BoolStatus = WinUsbDLL_Initialize(GlobalWinUSBDeviceHandle, &GlobalWinUSBInterfaceHandle);
        		if(BoolStatus == TRUE)  //Successfully intialized the WinUSB interface
        		{
                                ApparentOpenPipes++;
                                InterfacePowerPolicyValue = FALSE;      //Value for the AUTO_SUSPEND setting.  The default is enabled, but to be more compatible, it is preferrable to disable AUTO_SUSPEND by default.
                                if(WinUsbDLL_SetPowerPolicy != NULL) //Not done earlier.  Some uncertainty if this will always work since the WDK documentation (6001.18001) is incorrect describing this function.  Try to make this fail safe if it doesn't work.
                                {
                                   WinUsbDLL_SetPowerPolicy(GlobalWinUSBInterfaceHandle, AUTO_SUSPEND, ValueLength, &InterfacePowerPolicyValue);
                                   SetLastError(ERROR_SUCCESS);
                                }
                                WinUSBInterfaceInitialized = TRUE;
                                if(pEP == NULL)    //If MPUSBOpen() was called with pEP == NULL, then the mchpusb version of mpusbapi.dll will still return a valid interface handle, instead of returning INVALID_HANDLE_VALUE.
                                {
                                    if(dwDir == MP_READ)
                                        (int)EPNumNotRealHandle = 0x80;
                                    else if(dwDir == MP_WRITE)
                                        (int)EPNumNotRealHandle = 0x00;
                                }
                                UsingWinUSBDriver = TRUE;
                                return EPNumNotRealHandle;
        		        //If gets here, the "MyWinUSBInterfaceHandle" was initialized successfully.
        			//May begin using the MyWinUSBInterfaceHandle handle in WinUsb_WritePipe() and
        			//WinUsb_ReadPipe() function calls now.  Those are the functions for writing/reading to
        			//the USB device's endpoints.
                        }
                        else
                        {
                                 WinUSBInterfaceInitialized = FALSE;
                                 CloseHandle(GlobalWinUSBDeviceHandle);
                                 GlobalWinUSBInterfaceHandle = INVALID_HANDLE_VALUE;
                                 UsingWinUSBDriver = FALSE;
                                 return INVALID_HANDLE_VALUE;
                        }
        	}
                else
                {
                    WinUSBInterfaceInitialized = FALSE;
                    GlobalWinUSBInterfaceHandle = INVALID_HANDLE_VALUE;
                    UsingWinUSBDriver = FALSE;
                    return INVALID_HANDLE_VALUE;
                }
             }
             else  //else the WinUSB interface was already initialized from a prior call.  don't need to re-initialize it in this case.
             {
                ApparentOpenPipes++;
                SetLastError(ERROR_SUCCESS);
                if(pEP == NULL)    //If MPUSBOpen() was called with pEP == NULL, then the mchpusb version of mpusbapi.dll will still return a valid interface handle, instead of returning INVALID_HANDLE_VALUE.
                {
                        if(dwDir == MP_READ)
                                (int)EPNumNotRealHandle = 0x80;
                        else if(dwDir == MP_WRITE)
                                (int)EPNumNotRealHandle = 0x00;
                }
                return EPNumNotRealHandle;
             }
        }//end if(MPUSBGetDeviceLink ...)
        else //else couldn't even find a device that was attached!
        {
                if(WinUSBInterfaceInitialized == TRUE)
                {
                     WinUSBInterfaceInitialized = FALSE;
                }
        }
    }//end if
    SetLastError(ERROR_INVALID_DATA);
    return EPNumNotRealHandle;
}//end WinUSB_MPUSBOpen(...)






///////////////////////////////////////////////////////////////////////////////
//	MPUSBGetDescriptor : Returns the Requested Descriptor Data
//
//  handle  - Identifies the endpoint pipe to be read. The pipe handle must
//            have been created with MP_READ access attribute.
//  pDscParam
//          - pointer to a descriptor parameters object to be sent down to the
//            driver.  This object specifies the details about the descriptor
//            request.
//  dscLen  - the size of the pDscParam object
//  pDevDsc - pointer to where the resulting descriptor should be copied.
//  dwLen   - the available data in the pDevDsc buffer
//  pLength - a pointer to a DWORD that will be updated with the amount of data
//            actually written to the pDevDsc buffer.  This number will be
//            less than or equal to dwLen.
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
///////////////////////////////////////////////////////////////////////////////
DWORD MPUSBGetDescriptor(HANDLE handle,       // Input
                               PVOID pDscParam,     // Input
                               DWORD dscLen,        // Input
                               PVOID pDevDsc,       // Output
                               DWORD dwLen,         // Input
                               PDWORD pLength)      // Output
{
    //If WinUSB was detected, use the WinUSB flavor of this function instead.
    if(UsingWinUSBDriver == TRUE)
    {
        return WinUSB_MPUSBGetDescriptor(handle, pDscParam, dscLen, pDevDsc, dwLen, pLength);
    }

    if(pLength != NULL)*pLength = 0;
    if(pDevDsc == NULL) return MPUSB_FAIL;
    if(pDscParam == NULL) return MPUSB_FAIL;

    if(!DeviceIoControl(handle,
                        IOCTL_MCHPUSB_GET_DESCRIPTOR,
                        pDscParam,
                        dscLen,
                        pDevDsc,
                        dwLen,
                        pLength,
                        NULL))
    {
        printf("Get dsc error: %d",GetLastError());
        return MPUSB_FAIL;
    }//end if
    return MPUSB_SUCCESS;
}// MPUSBGetDescriptor()

///////////////////////////////////////////////////////////////////////////////
//Function: WinUSB_MPUSBGetDescriptor()
//Note: API equivalent of MPUSBGetDescriptor().  Used for when using the WinUSB driver instead of mchpusb.
///////////////////////////////////////////////////////////////////////////////
DWORD WinUSB_MPUSBGetDescriptor(HANDLE handle,       // Input
                               PVOID pDscParam,     // Input
                               DWORD dscLen,        // Input
                               PVOID pDevDsc,       // Output
                               DWORD dwLen,         // Input
                               PDWORD pLength)      // Output
{
        if(pLength != NULL)
        {
                *pLength = 0;
        }

        if(pDevDsc == NULL)
        {
                SetLastError(ERROR_INVALID_PARAMETER);
                return MPUSB_FAIL;
        }
        if(pDscParam == NULL)
        {
                SetLastError(ERROR_INVALID_PARAMETER);
                return MPUSB_FAIL;
        }

        UCHAR DescriptorType = ((_GET_DESCRIPTOR_PARAMETER*)pDscParam)->bType;  //This is the bDescriptorType value (see USB 2.0 specs for values)
        UCHAR Index = ((_GET_DESCRIPTOR_PARAMETER*)pDscParam)->bIndex;
        USHORT LanguageID = ((_GET_DESCRIPTOR_PARAMETER*)pDscParam)->wLangid;

        if(WinUsbDLL_GetDescriptor(GlobalWinUSBInterfaceHandle, DescriptorType, Index, LanguageID, (PUCHAR)pDevDsc, dwLen, pLength) == TRUE)
        {
                return MPUSB_SUCCESS;
        }
        else
        {
                return MPUSB_FAIL;
        }
}// WinUSB_MPUSBGetDescriptor()


///////////////////////////////////////////////////////////////////////////////
//	MPUSBGetDeviceDescriptor : Returns the Device Descriptor Data
//
//  handle  - Identifies the endpoint pipe to be read. The pipe handle must
//            have been created with MP_READ access attribute.
//  pDevDsc - pointer to where the resulting descriptor should be copied.
//  dwLen   - the available data in the pDevDsc buffer
//  pLength - a pointer to a DWORD that will be updated with the amount of data
//            actually written to the pDevDsc buffer.  This number will be
//            less than or equal to dwLen.
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
DWORD MPUSBGetDeviceDescriptor(HANDLE handle,       // Input
                               PVOID pDevDsc,       // Output
                               DWORD dwLen,         // Input
                               PDWORD pLength)      // Output
{
    //If WinUSB was detected, use the WinUSB flavor of this function instead.
    if(UsingWinUSBDriver == TRUE)
    {
        return WinUSB_MPUSBGetDeviceDescriptor(handle, pDevDsc, dwLen, pLength);
    }

    GET_DESCRIPTOR_PARAMETER DscParam;

    if(pLength != NULL)*pLength = 0;
    if(pDevDsc == NULL) return MPUSB_FAIL;

    DscParam.bType = USB_DEVICE_DESCRIPTOR_TYPE;
    DscParam.bIndex = 0;
    DscParam.wLangid = 0;

    if(!DeviceIoControl(handle,
                        IOCTL_MCHPUSB_GET_DESCRIPTOR,
                        &DscParam,
                        sizeof(DscParam),
                        pDevDsc,
                        dwLen,
                        pLength,
                        NULL))
    {
        printf("Get dsc error: %d",GetLastError());
        return MPUSB_FAIL;
    }//end if
    return MPUSB_SUCCESS;
}// MPUSBGetDeviceDescriptor

///////////////////////////////////////////////////////////////////////////////
//Function: WinUSB_MPUSBGetDeviceDescriptor()
//Note: API equivalent of MPUSBGetDeviceDescriptor().  Used for when using the WinUSB driver instead of mchpusb.
///////////////////////////////////////////////////////////////////////////////
DWORD WinUSB_MPUSBGetDeviceDescriptor(HANDLE handle,       // Input
                               PVOID pDevDsc,       // Output
                               DWORD dwLen,         // Input
                               PDWORD pLength)      // Output
{
        if(pLength != NULL)
        {
                *pLength = 0;
        }

        if(pDevDsc == NULL)
        {
                SetLastError(ERROR_INVALID_PARAMETER);
                return MPUSB_FAIL;
        }

        UCHAR DescriptorType = USB_DEVICE_DESCRIPTOR_TYPE;  //This is the bDescriptorType value (see USB 2.0 specs for values, 0x01 is device descriptor)
        UCHAR Index = 0x00;
        USHORT LanguageID = 0x00;  //LangID is 0 for non-string descriptors

        if(WinUsbDLL_GetDescriptor(GlobalWinUSBInterfaceHandle, DescriptorType, Index, LanguageID, (PUCHAR)pDevDsc, dwLen, pLength) == TRUE)
        {
                return MPUSB_SUCCESS;
        }
        else
        {
                return MPUSB_FAIL;
        }
}// WinUSB_MPUSBGetDeviceDescriptor


///////////////////////////////////////////////////////////////////////////////
//	MPUSBGetConfigurationDescriptor : Returns the Configuration Descriptor
//
//  handle  - Identifies the endpoint pipe to be read. The pipe handle must
//            have been created with MP_READ access attribute.
//  bIndex  - the index of the configuration descriptor desired.  Valid input
//            range is 1 - 255.
//  pDevDsc - pointer to where the resulting descriptor should be copied.
//  dwLen   - the available data in the pDevDsc buffer
//  pLength - a pointer to a DWORD that will be updated with the amount of data
//            actually written to the pDevDsc buffer.  This number will be
//            less than or equal to dwLen.
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
DWORD MPUSBGetConfigurationDescriptor(HANDLE handle,       // Input
                               UCHAR bIndex,        // Input
                               PVOID pDevDsc,       // Output
                               DWORD dwLen,         // Input
                               PDWORD pLength)      // Output
{
    //If WinUSB was detected, use the WinUSB flavor of this function instead.
    if(UsingWinUSBDriver == TRUE)
    {
        return WinUSB_MPUSBGetConfigurationDescriptor(handle, bIndex, pDevDsc, dwLen, pLength);
    }

    GET_DESCRIPTOR_PARAMETER DscParam;

    if(pLength != NULL)*pLength = 0;
    if(pDevDsc == NULL) return MPUSB_FAIL;
    if(bIndex == 0) return MPUSB_FAIL;

    DscParam.bType = USB_CONFIGURATION_DESCRIPTOR_TYPE;
    DscParam.bIndex = bIndex - 1;
    DscParam.wLangid = 0;

    if(!DeviceIoControl(handle,
                        IOCTL_MCHPUSB_GET_DESCRIPTOR,
                        &DscParam,
                        sizeof(DscParam),
                        pDevDsc,
                        dwLen,
                        pLength,
                        NULL))
    {
        printf("Get dsc error: %d",GetLastError());
        return MPUSB_FAIL;
    }//end if
    return MPUSB_SUCCESS;
}// MPUSBGetConfigurationDescriptor


DWORD WinUSB_MPUSBGetConfigurationDescriptor(HANDLE handle,       // Input
                               UCHAR bIndex,        // Input
                               PVOID pDevDsc,       // Output
                               DWORD dwLen,         // Input
                               PDWORD pLength)      // Output
{
        if(pLength != NULL)
            *pLength = 0;

        if(pDevDsc == NULL)
        {
            SetLastError(ERROR_INVALID_PARAMETER);
            return MPUSB_FAIL;
        }

        UCHAR DescriptorType = USB_CONFIGURATION_DESCRIPTOR_TYPE;  //This is the bDescriptorType value (see USB 2.0 specs for values)
        USHORT LanguageID = 0x00;  //LangID is 0 for non-string descriptors

        if(WinUsbDLL_GetDescriptor(GlobalWinUSBInterfaceHandle, DescriptorType, bIndex, LanguageID, (PUCHAR)pDevDsc, dwLen, pLength) == TRUE)
        {
                return MPUSB_SUCCESS;
        }
        else
        {
                return MPUSB_FAIL;
        }
}// WinUSB_MPUSBGetConfigurationDescriptor

///////////////////////////////////////////////////////////////////////////////
//	MPUSBGetStringDescriptor : Returns the requested string descriptor
//
//  handle  - Identifies the endpoint pipe to be read. The pipe handle must
//            have been created with MP_READ access attribute.
//  bIndex  - the index of the configuration descriptor desired.  Valid input
//            range is 0 - 255.
//  wLangId - the language ID of the string that needs to be read
//  pDevDsc - pointer to where the resulting descriptor should be copied.
//  dwLen   - the available data in the pDevDsc buffer
//  pLength - a pointer to a DWORD that will be updated with the amount of data
//            actually written to the pDevDsc buffer.  This number will be
//            less than or equal to dwLen.
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
DWORD MPUSBGetStringDescriptor(HANDLE handle,       // Input
                               UCHAR bIndex,        // Input
                               USHORT wLangId,      // Input
                               PVOID pDevDsc,       // Output
                               DWORD dwLen,         // Input
                               PDWORD pLength)      // Output
{
    //If WinUSB was detected, use the WinUSB flavor of this function instead.
    if(UsingWinUSBDriver == TRUE)
    {
        return WinUSB_MPUSBGetStringDescriptor(handle, bIndex, wLangId, pDevDsc, dwLen, pLength);
    }

    GET_DESCRIPTOR_PARAMETER DscParam;

    if(pLength != NULL)*pLength = 0;
    if(pDevDsc == NULL) return MPUSB_FAIL;

    DscParam.bType = USB_STRING_DESCRIPTOR_TYPE;
    DscParam.bIndex = bIndex;
    DscParam.wLangid = wLangId;

    if(!DeviceIoControl(handle,
                        IOCTL_MCHPUSB_GET_DESCRIPTOR,
                        &DscParam,
                        sizeof(DscParam),
                        pDevDsc,
                        dwLen,
                        pLength,
                        NULL))
    {
        printf("Get dsc error: %d",GetLastError());
        return MPUSB_FAIL;
    }//end if
    return MPUSB_SUCCESS;
}// MPUSBGetConfigurationDescriptor


DWORD WinUSB_MPUSBGetStringDescriptor(HANDLE handle,       // Input
                               UCHAR bIndex,        // Input
                               USHORT wLangId,      // Input
                               PVOID pDevDsc,       // Output
                               DWORD dwLen,         // Input
                               PDWORD pLength)      // Output
{
        if(pLength != NULL)
        {
                *pLength = 0;
        }

        if(pDevDsc == NULL)
        {
                SetLastError(ERROR_INVALID_PARAMETER);
                return MPUSB_FAIL;
        }


        UCHAR DescriptorType = USB_STRING_DESCRIPTOR_TYPE;  //This is the bDescriptorType value (see USB 2.0 specs for values)

        if(WinUsbDLL_GetDescriptor(GlobalWinUSBInterfaceHandle, DescriptorType, bIndex, wLangId, (PUCHAR)pDevDsc, dwLen, pLength) == TRUE)
        {
                return MPUSB_SUCCESS;
        }
        else
        {
                return MPUSB_FAIL;
        }
}// WinUSB_MPUSBGetStringDescriptor


//  **** INCOMPLETE ****
DWORD MPUSBGetConfigInfoSize(HANDLE handle)
{
    //If WinUSB was detected, use the WinUSB flavor of this function instead.
    if(UsingWinUSBDriver == TRUE)
    {
        return WinUSB_MPUSBGetConfigInfoSize(handle);
    }

    DWORD config_size;
    DWORD dwReqLen;
    DeviceIoControl(handle,
                    IOCTL_MCHPUSB_GET_CONFIGURATION_INFO,
                    NULL,
                    0,
                    &config_size,
                    sizeof(DWORD),
                    &dwReqLen,
                    NULL);
    return config_size;
}//end MPUSBGetConfigInfoSize

DWORD WinUSB_MPUSBGetConfigInfoSize(HANDLE handle)
{
//  **** INCOMPLETE ****
        SetLastError(ERROR_NOT_SUPPORTED);
        return MPUSB_FAIL;
}//end MPUSBGetConfigInfoSize



//  **** INCOMPLETE ****
DWORD MPUSBGetConfigInfo(HANDLE handle,         // Input
                         PVOID pData,           // Output
                         DWORD dwLen)           // Input
{
    //If WinUSB was detected, use the WinUSB flavor of this function instead.
    if(UsingWinUSBDriver == TRUE)
    {
        return WinUSB_MPUSBGetConfigInfo(handle, pData, dwLen);
    }

    DWORD dwReqLen;
    if(!DeviceIoControl(handle,
                        IOCTL_MCHPUSB_GET_CONFIGURATION_INFO,
                        NULL,
                        0,
                        pData,
                        dwLen,
                        &dwReqLen,
                        NULL))
    {
        printf("Get config error: %d",GetLastError());
        return MPUSB_FAIL;
    }//end if

    return MPUSB_SUCCESS;
}//end MPUSBGetConfigInfo

DWORD WinUSB_MPUSBGetConfigInfo(HANDLE handle,         // Input
                         PVOID pData,           // Output
                         DWORD dwLen)           // Input
{
//  **** INCOMPLETE ****
        SetLastError(ERROR_NOT_SUPPORTED);
        return MPUSB_FAIL;
}//end WinUSB_MPUSBGetConfigInfo



//  **** INCOMPLETE ****
DWORD MPUSBSendControl(HANDLE handle,         // Input
                         PVOID pData,           // Input
                         DWORD dwLen)           // Input
{
    SetLastError(ERROR_NOT_SUPPORTED);
    return MPUSB_FAIL;
}

//  **** INCOMPLETE ****
DWORD MPUSBGetControl(HANDLE handle,         // Input
                         PVOID pData,           // Ouput
                         DWORD dwLen)           // Input
{
    SetLastError(ERROR_NOT_SUPPORTED);
    return MPUSB_FAIL;
}

///////////////////////////////////////////////////////////////////////////////
//	MPUSBSetConfiguration : Sets the device configuration through a USB
//            SET_CONFIGURATION command.
//
//  handle  - Identifies the endpoint pipe to be written. The pipe handle must
//            have been created with MP_WRITE access attribute.
//
//  bConfigSetting
//          - Denotes the configuration number that needs to be set.  If this
//            number does not fall in the devices allowed configurations then
//            this function will return with MP_FAIL
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
DWORD MPUSBSetConfiguration(HANDLE handle,         // Input
                            USHORT bConfigSetting) // Input
{
    //If WinUSB was detected, use the WinUSB flavor of this function instead.
    if(UsingWinUSBDriver == TRUE)
    {
        return WinUSB_MPUSBSetConfiguration(handle, bConfigSetting);
    }

    SET_CONFIGURATION_PARAMETER  cfgParam;
    DWORD dwReturnData;
    DWORD dwReqLen;

    cfgParam.bConfigurationValue = bConfigSetting;

    if(!DeviceIoControl(handle,
                        IOCTL_MCHPUSB_SET_CONFIGURATION,
                        &cfgParam,
                        sizeof(cfgParam),
                        &dwReturnData,
                        sizeof(dwReturnData),
                        &dwReqLen,
                        NULL))
    {
        printf("Set configuration error: %d",GetLastError());
        return MPUSB_FAIL;
    }//end if

    return MPUSB_SUCCESS;
}

DWORD WinUSB_MPUSBSetConfiguration(HANDLE handle,         // Input
                            USHORT bConfigSetting) // Input
{
        //Unfortunately there doesn't appear to be any directly equivalent function
        //provided by winusb.dll.  It might be possible to partially emulate
        //this request using the WinUsb_ControlTransfer() function, but this might
        //not be fully feasible...
        SetLastError(ERROR_NOT_SUPPORTED);
        return MPUSB_FAIL;
}

///////////////////////////////////////////////////////////////////////////////
//	MPUSBRead :
//
//  handle  - Identifies the endpoint pipe to be read. The pipe handle must
//            have been created with MP_READ access attribute.
//
//  pData   - Points to the buffer that receives the data read from the pipe.
//
//  dwLen   - Specifies the number of bytes to be read from the pipe.
//
//  pLength - Points to the number of bytes read. MPUSBRead sets this value to
//            zero before doing any work or error checking.
//
//  dwMilliseconds
//          - Specifies the time-out interval, in milliseconds. The function
//            returns if the interval elapses, even if the operation is
//            incomplete. If dwMilliseconds is zero, the function tests the
//            data pipe and returns immediately. If dwMilliseconds is INFINITE,
//            the function's time-out interval never elapses.
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
DWORD MPUSBRead(HANDLE handle,          // Input
                PVOID pData,            // Output
                DWORD dwLen,            // Input
                PDWORD pLength,         // Output
                DWORD dwMilliseconds)   // Input
{
    //If WinUSB was detected, use the WinUSB flavor of this function instead.
    if(UsingWinUSBDriver == TRUE)
    {
        return WinUSB_MPUSBRead(handle, pData, dwLen, pLength, dwMilliseconds);
    }

    BOOL bResult;
    DWORD nBytesRead;
    OVERLAPPED gOverlapped;
    DWORD dwResult;

    dwResult = MPUSB_FAIL;

    // set up overlapped structure fields
    gOverlapped.Internal     = 0;
    gOverlapped.InternalHigh = 0;
    gOverlapped.Offset       = 0;
    gOverlapped.OffsetHigh   = 0;
    gOverlapped.hEvent       = CreateEvent(NULL, FALSE, FALSE, NULL);

    if(pLength != NULL)*pLength = 0;

    // attempt an asynchronous read operation
    bResult = ReadFile(handle,pData,dwLen,&nBytesRead,&gOverlapped);

    if(!bResult)
    {
        // deal with the error code
        switch (GetLastError())
        {
            case ERROR_HANDLE_EOF:
            {
                // we have reached the end of the file
                // during the call to ReadFile
                break;
            }
            case ERROR_IO_PENDING:
            {
                // asynchronous i/o is still in progress
                switch(WaitForSingleObject(gOverlapped.hEvent, dwMilliseconds))
                {
                    case WAIT_OBJECT_0:
                        // check on the results of the asynchronous read
                        // and update the nBytesRead...
                        bResult = GetOverlappedResult(handle, &gOverlapped,
                                                      &nBytesRead, FALSE);
                        if(!bResult)
                        {
                            printf("Error: %d", GetLastError());
                        }
                        else
                        {
                            if(pLength != NULL)
                                *pLength = nBytesRead;
                            dwResult = MPUSB_SUCCESS;
                        }//end if else
                        break;
                    case WAIT_TIMEOUT:
                        CancelIo(handle);
                        break;
                    default: //switch(WaitForSingleObject(gOverlapped.hEvent, dwMilliseconds))
                        CancelIo(handle);
                        break;
                }//end switch
                break;  //switch (GetLastError()) case ERROR_IO_PENDING
            }//end case
            default:    //switch (GetLastError())
                CancelIo(handle);
                break;
        }//end switch
    }
    else
    {
        if(pLength != NULL)
            *pLength = nBytesRead;
        dwResult = MPUSB_SUCCESS;
    }//end if else

    ResetEvent(gOverlapped.hEvent);
    CloseHandle(gOverlapped.hEvent);

    return dwResult;
}//end MPUSBRead


DWORD WinUSB_MPUSBRead(HANDLE pEPInfo,          // Input, NOTE: pEPInfo is the pEP that got passed to the MPUSBOpen() call.  It is not a real HANDLE, it originates as a PCHAR
                PVOID pData,            // Output
                DWORD dwLen,            // Input
                PDWORD pLength,         // Output
                DWORD dwMilliseconds)   // Input
{
    BOOL bResult;
    DWORD nBytesRead = 0;
    OVERLAPPED gOverlapped;
    DWORD dwResult;
    UCHAR PipeID;
    dwResult = MPUSB_FAIL;

    if(pLength != NULL)*pLength = 0;

    //Make sure pData pointer and pEPInfo "handle" are valid before doing anything
    if((pData == NULL) || (pEPInfo == INVALID_HANDLE_VALUE))
    {
        SetLastError(ERROR_INVALID_DATA);
        return MPUSB_FAIL;
    }

     //First, figure out the PipeID (used by WinUsb_ReadPipe()) by using the data in the pEPInfo. (which is interpreted in the MPUSBOpen() function)
    PipeID = (UCHAR)pEPInfo;

    // set up overlapped structure fields
    gOverlapped.Internal     = 0;
    gOverlapped.InternalHigh = 0;
    gOverlapped.Offset       = 0;
    gOverlapped.OffsetHigh   = 0;
    gOverlapped.hEvent       = CreateEvent(NULL, FALSE, FALSE, NULL);

    // attempt an asynchronous read operation
    bResult = WinUsbDLL_ReadPipe((WINUSB_INTERFACE_HANDLE)GlobalWinUSBInterfaceHandle, PipeID, (PUCHAR)pData, dwLen, &nBytesRead, &gOverlapped);

    if(!bResult)
    {
        // deal with the error code
        switch (GetLastError())
        {
            case ERROR_HANDLE_EOF:
            {
                // we have reached the end of the file
                // during the call to ReadFile
                break;
            }
            case ERROR_IO_PENDING:
            {
                // asynchronous i/o is still in progress
                switch(WaitForSingleObject(gOverlapped.hEvent, dwMilliseconds))
                {
                    case WAIT_OBJECT_0:
                        // check on the results of the asynchronous read
                        // and update the nBytesRead...
                        bResult = GetOverlappedResult(GlobalWinUSBInterfaceHandle, &gOverlapped,
                                                      &nBytesRead, FALSE);
                        if(!bResult)
                        {
                            printf("Error: %d", GetLastError());
                        }
                        else
                        {
                            if(pLength != NULL)
                                *pLength = nBytesRead;
                            dwResult = MPUSB_SUCCESS;
                        }//end if else
                        break;
                    case WAIT_TIMEOUT:
                        WinUsbDLL_AbortPipe(GlobalWinUSBInterfaceHandle, PipeID);
                        break;
                    default:
                        WinUsbDLL_AbortPipe(GlobalWinUSBInterfaceHandle, PipeID);
                        break;
                }//end switch
            }//end case
            default:
                WinUsbDLL_AbortPipe(GlobalWinUSBInterfaceHandle, PipeID);
                break;
        }//end switch
    }
    else
    {
        if(pLength != NULL)
            *pLength = nBytesRead;
        dwResult = MPUSB_SUCCESS;
    }//end if else

    ResetEvent(gOverlapped.hEvent);
    CloseHandle(gOverlapped.hEvent);

    return dwResult;
}//end WinUSB_MPUSBRead

///////////////////////////////////////////////////////////////////////////////
//	MPUSBWrite :
//
//  handle  - Identifies the endpoint pipe to be written to. The pipe handle
//            must have been created with MP_WRITE access attribute.
//
//  pData   - Points to the buffer containing the data to be written to the pipe.
//
//  dwLen   - Specifies the number of bytes to write to the pipe.
//
//  pLength - Points to the number of bytes written by this function call.
//            MPUSBWrite sets this value to zero before doing any work or
//            error checking.
//
//  dwMilliseconds
//          - Specifies the time-out interval, in milliseconds. The function
//            returns if the interval elapses, even if the operation is
//            incomplete. If dwMilliseconds is zero, the function tests the
//            data pipe and returns immediately. If dwMilliseconds is INFINITE,
//            the function's time-out interval never elapses.
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
DWORD MPUSBWrite(HANDLE handle,         // Input
                 PVOID pData,           // Input
                 DWORD dwLen,           // Input
                 PDWORD pLength,        // Output
                 DWORD dwMilliseconds)  // Input
{
    //If WinUSB was detected, use the WinUSB flavor of this function instead.
    if(UsingWinUSBDriver == TRUE)
    {
        return WinUSB_MPUSBWrite(handle, pData, dwLen, pLength, dwMilliseconds);
    }

    BOOL bResult;
    DWORD nBytesWritten;
    OVERLAPPED gOverlapped;
    DWORD dwResult;

    dwResult = MPUSB_FAIL;

    // set up overlapped structure fields
    gOverlapped.Internal     = 0;
    gOverlapped.InternalHigh = 0;
    gOverlapped.Offset       = 0;
    gOverlapped.OffsetHigh   = 0;
    gOverlapped.hEvent       = CreateEvent(NULL, FALSE, FALSE, NULL);

    if(pLength != NULL)*pLength = 0;

    // attempt an asynchronous read operation
    bResult = WriteFile(handle,pData,dwLen,&nBytesWritten,&gOverlapped);

    if(!bResult)
    {
        // deal with the error code
        switch (GetLastError())
        {
            case ERROR_HANDLE_EOF:
            {
                // we have reached the end of the file
                // during the call to ReadFile
                break;
            }
            case ERROR_IO_PENDING:
            {
                // asynchronous i/o is still in progress
                switch(WaitForSingleObject(gOverlapped.hEvent, dwMilliseconds))
                {
                    case WAIT_OBJECT_0:
                        // check on the results of the asynchronous read
                        // and update the nBytesWritten...
                        bResult = GetOverlappedResult(handle, &gOverlapped,
                                                      &nBytesWritten, FALSE);
                        if(!bResult)
                        {
                            printf("Error: %d", GetLastError());
                        }
                        else
                        {
                            if(pLength != NULL)
                                *pLength = nBytesWritten;
                            dwResult = MPUSB_SUCCESS;
                        }//end if else
                        break;
                    case WAIT_TIMEOUT:
                        CancelIo(handle);
                        break;
                    default:
                        CancelIo(handle);
                        break;
                }//end switch
                break;
            }//end case
            default:
                CancelIo(handle);
                break;
        }//end switch
    }
    else
    {
        if(pLength != NULL)
            *pLength = nBytesWritten;
        dwResult = MPUSB_SUCCESS;
    }//end if else

    ResetEvent(gOverlapped.hEvent);
    CloseHandle(gOverlapped.hEvent);

    return dwResult;

}//end MPUSBWrite


DWORD WinUSB_MPUSBWrite(HANDLE pEPInfo,         // Input, NOTE: This is not a real "HANDLE", it is a uchar that contains the endpoint number returned by the MPUSBOpen() function.
                 PVOID pData,           // Input
                 DWORD dwLen,           // Input
                 PDWORD pLength,        // Output
                 DWORD dwMilliseconds)  // Input
{
    BOOL bResult;
    ULONG nBytesWritten = 0;
    OVERLAPPED gOverlapped;
    DWORD dwResult;
    dwResult = MPUSB_FAIL;
    UCHAR PipeID;

    if(pLength != NULL)
        *pLength = (DWORD)0;

    //Make sure pData pointer and pEPInfo "handle" are valid before doing anything
    if((pData == NULL) || (pEPInfo == INVALID_HANDLE_VALUE))
    {
        SetLastError(ERROR_INVALID_DATA);
        return MPUSB_FAIL;
    }

    //First, figure out the PipeID (used by WinUsb_WritePipe()) by using the data in the pEPInfo. (which is interpreted in the MPUSBOpen() function)
    PipeID = (UCHAR)pEPInfo;

    // set up overlapped structure fields
    gOverlapped.Internal     = 0;
    gOverlapped.InternalHigh = 0;
    gOverlapped.Offset       = 0;
    gOverlapped.OffsetHigh   = 0;
    gOverlapped.hEvent       = CreateEvent(NULL, FALSE, FALSE, NULL);




    // attempt an asynchronous write operation
    bResult = WinUsbDLL_WritePipe(GlobalWinUSBInterfaceHandle, PipeID, (PUCHAR)pData, (ULONG)dwLen, &nBytesWritten, &gOverlapped);
    if(!bResult)
    {
        // deal with the error code
        switch (GetLastError())
        {
            case ERROR_HANDLE_EOF:
            {
                // we have reached the end of the file
                // during the call to ReadFile
                break;
            }
            case ERROR_IO_PENDING:
            {
                // asynchronous i/o is still in progress
                switch(WaitForSingleObject(gOverlapped.hEvent, dwMilliseconds))
                {
                    case WAIT_OBJECT_0:
                        // check on the results of the asynchronous read
                        // and update the nBytesWritten...
                        bResult = GetOverlappedResult(GlobalWinUSBInterfaceHandle, &gOverlapped,
                                                      &nBytesWritten, FALSE);
                        if(!bResult)
                        {
                            printf("Error: %d", GetLastError());
                        }
                        else
                        {
                            if(pLength != NULL)
                                *pLength = (DWORD)nBytesWritten;
                            dwResult = MPUSB_SUCCESS;
                        }//end if else
                        break;
                    case WAIT_TIMEOUT:
                        WinUsbDLL_AbortPipe(GlobalWinUSBInterfaceHandle, PipeID);
                        break;
                    default:
                        WinUsbDLL_AbortPipe(GlobalWinUSBInterfaceHandle, PipeID);
                        break;
                }//end switch
            }//end case
            default:
                WinUsbDLL_AbortPipe(GlobalWinUSBInterfaceHandle, PipeID);
                break;
        }//end switch
    }
    else
    {
        if(pLength != NULL)
            *pLength = (DWORD)nBytesWritten;
        dwResult = MPUSB_SUCCESS;
    }//end if else

    ResetEvent(gOverlapped.hEvent);
    CloseHandle(gOverlapped.hEvent);

    return dwResult;

}//end WinUSB_MPUSBWrite

///////////////////////////////////////////////////////////////////////////////
//	MPUSBReadInt :
//
//  handle  - Identifies the endpoint pipe to be read. The pipe handle must
//            have been created with MP_READ access attribute.
//
//  pData   - Points to the buffer that receives the data read from the pipe.
//
//  dwLen   - Specifies the number of bytes to be read from the pipe.
//
//  pLength - Points to the number of bytes read. MPUSBRead sets this value to
//            zero before doing any work or error checking.
//
//  dwMilliseconds
//          - Specifies the time-out interval, in milliseconds. The function
//            returns if the interval elapses, even if the operation is
//            incomplete. If dwMilliseconds is zero, the function tests the
//            data pipe and returns immediately. If dwMilliseconds is INFINITE,
//            the function's time-out interval never elapses.
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
DWORD MPUSBReadInt(HANDLE handle,       // Input
                   PVOID pData,         // Output
                   DWORD dwLen,         // Input
                   PDWORD pLength,      // Output
                   DWORD dwMilliseconds)// Input
{
    //If WinUSB was detected, use the WinUSB flavor of this function instead.
    if(UsingWinUSBDriver == TRUE)
    {
        return WinUSB_MPUSBReadInt(handle, pData, dwLen, pLength, dwMilliseconds);
    }

    BOOL bResult;
    DWORD nBytesRead;
    OVERLAPPED gOverlapped;
    DWORD dwResult;

    dwResult = MPUSB_FAIL;

    // set up overlapped structure fields
    gOverlapped.Internal     = 0;
    gOverlapped.InternalHigh = 0;
    gOverlapped.Offset       = 0;
    gOverlapped.OffsetHigh   = 0;
    gOverlapped.hEvent       = CreateEvent(NULL, FALSE, FALSE, NULL);

    if(pLength != NULL)*pLength = 0;
    if(pData == NULL)
    {
        SetLastError(ERROR_NOACCESS);
        return dwResult;
    }//end if

    // attempt an asynchronous read operation
    bResult = DeviceIoControl(handle,
                              IOCTL_MCHPUSB_WAIT_INTERRUPT,
                              NULL,
                              0,
                              pData,
                              dwLen,
                              &nBytesRead,
                              &gOverlapped);
    if(!bResult)
    {
        // deal with the error code
        switch (GetLastError())
        {
            case ERROR_HANDLE_EOF:
            {
                // we have reached the end of the file
                // during the call to ReadFile
                break;
            }
            case ERROR_IO_PENDING:
            {
                // asynchronous i/o is still in progress
                switch(WaitForSingleObject(gOverlapped.hEvent, dwMilliseconds))
                {
                    case WAIT_OBJECT_0:
                        // check on the results of the asynchronous read
                        // and update the nBytesRead...
                        bResult = GetOverlappedResult(handle, &gOverlapped,
                                                      &nBytesRead, FALSE);
                        if(!bResult)
                        {
                            printf("Error: %d", GetLastError());
                        }
                        else
                        {
                            if(pLength != NULL)
                                *pLength = nBytesRead;
                            dwResult = MPUSB_SUCCESS;
                        }//end if else
                        break;
                    case WAIT_TIMEOUT:
                        CancelIo(handle);
                        break;
                    default:
                        CancelIo(handle);
                        break;
                }//end switch
                break;
            }//end case
            default:
                CancelIo(handle);
                break;
        }//end switch
    }
    else
    {
        if(pLength != NULL)
            *pLength = nBytesRead;
        dwResult = MPUSB_SUCCESS;
    }//end if else

    ResetEvent(gOverlapped.hEvent);
    CloseHandle(gOverlapped.hEvent);

    return dwResult;
}//end MPUSBReadInt


DWORD WinUSB_MPUSBReadInt(HANDLE pEPInfo,       // Input, NOTE: Not a real HANDLE.  A UCHAR from the MPUSBOpen() command, which contains the PipeID.
                   PVOID pData,         // Output
                   DWORD dwLen,         // Input
                   PDWORD pLength,      // Output
                   DWORD dwMilliseconds)// Input
{
    //NOTE: This function may not be 100% compatible with the mchpusb.sys based MPUSBReadInt() function.
    //This is essentially the same as the MPUSBRead() code.

    BOOL bResult;
    DWORD nBytesRead;
    OVERLAPPED gOverlapped;
    DWORD dwResult;
    UCHAR PipeID;
    dwResult = MPUSB_FAIL;

    if(pLength != NULL)*pLength = 0;

    //Make sure pData pointer and pEPInfo "handle" are valid before doing anything
    if((pData == NULL) || (pEPInfo == INVALID_HANDLE_VALUE))
    {
        SetLastError(ERROR_NOACCESS);
        return MPUSB_FAIL;
    }

     //First, figure out the PipeID (used by WinUsb_ReadPipe()) by using the data in the pEPInfo. (which is interpreted in the MPUSBOpen() function)
    PipeID = (UCHAR)pEPInfo;

    // set up overlapped structure fields
    gOverlapped.Internal     = 0;
    gOverlapped.InternalHigh = 0;
    gOverlapped.Offset       = 0;
    gOverlapped.OffsetHigh   = 0;
    gOverlapped.hEvent       = CreateEvent(NULL, FALSE, FALSE, NULL);

    // attempt an asynchronous read operation
    bResult = WinUsbDLL_ReadPipe((WINUSB_INTERFACE_HANDLE)GlobalWinUSBInterfaceHandle, PipeID, (PUCHAR)pData, dwLen, pLength, &gOverlapped);

    if(!bResult)
    {
        // deal with the error code
        switch (GetLastError())
        {
            case ERROR_HANDLE_EOF:
            {
                // we have reached the end of the file
                // during the call to ReadFile
                break;
            }
            case ERROR_IO_PENDING:
            {
                // asynchronous i/o is still in progress
                switch(WaitForSingleObject(gOverlapped.hEvent, dwMilliseconds))
                {
                    case WAIT_OBJECT_0:
                        // check on the results of the asynchronous read
                        // and update the nBytesRead...
                        bResult = GetOverlappedResult(GlobalWinUSBInterfaceHandle, &gOverlapped,
                                                      &nBytesRead, FALSE);
                        if(!bResult)
                        {
                            printf("Error: %d", GetLastError());
                        }
                        else
                        {
                            if(pLength != NULL)
                                *pLength = nBytesRead;
                            dwResult = MPUSB_SUCCESS;
                        }//end if else
                        break;
                    case WAIT_TIMEOUT:
                        WinUsbDLL_AbortPipe(GlobalWinUSBInterfaceHandle, PipeID);
                        break;
                    default:
                        WinUsbDLL_AbortPipe(GlobalWinUSBInterfaceHandle, PipeID);
                        break;
                }//end switch
            }//end case
            default:
                WinUsbDLL_AbortPipe(GlobalWinUSBInterfaceHandle, PipeID);
                break;
        }//end switch
    }
    else
    {
        if(pLength != NULL)
            *pLength = nBytesRead;
        dwResult = MPUSB_SUCCESS;
    }//end if else

    ResetEvent(gOverlapped.hEvent);
    CloseHandle(gOverlapped.hEvent);

    return dwResult;
}//end WinUSB_MPUSBReadInt


///////////////////////////////////////////////////////////////////////////////
//	MPUSBClose : closes a given handle.
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
BOOL MPUSBClose(HANDLE handle)
{
    //If WinUSB was detected, use the WinUSB flavor of this function instead.
    if(UsingWinUSBDriver == TRUE)
    {
        return WinUSB_MPUSBClose(handle);
    }

    BOOL toReturn;
    toReturn = true;

    if(handle != INVALID_HANDLE_VALUE)
        toReturn = CloseHandle(handle);
	return toReturn;
}//end MPUSBClose

BOOL WinUSB_MPUSBClose(HANDLE handle)
{
    if((ApparentOpenPipes == 0) || ((ApparentOpenPipes - 1) == 0))
    {
            ApparentOpenPipes = 0;
            if(WinUSBInterfaceInitialized == TRUE)
            {
                    WinUSBInterfaceInitialized = FALSE;
                    if(WinUsbDLL_Free(GlobalWinUSBInterfaceHandle))
                    {
                        GlobalWinUSBInterfaceHandle == INVALID_HANDLE_VALUE;
                        if(CloseHandle(GlobalWinUSBDeviceHandle))
                        {
                            GlobalWinUSBDeviceHandle == INVALID_HANDLE_VALUE;
                            return true;    //successfully closed both device and interface handles
                        }
                    }
             }
             return false; //For some reason couldn't properly close the WinUSB interface handle...
    }

    ApparentOpenPipes--;
    return true;

}//end WinUSB_MPUSBClose

//---------------------------------------------------------------------------
