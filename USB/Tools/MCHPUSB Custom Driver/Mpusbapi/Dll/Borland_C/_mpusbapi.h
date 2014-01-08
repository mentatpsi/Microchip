/*********************************************************************
 *
 *                  MPUSBAPI Library
 *
 *********************************************************************
 * FileName:        _mpusbapi.h
 * Dependencies:    None
 * Compiler:        Borland C++ Builder 6
 * Company:         Copyright (C) 2004 by Microchip Technology, Inc.
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
 ********************************************************************/

#ifndef _MPUSBAPI_H_
#define _MPUSBAPI_H_

#define	MPUSB_FAIL                  0
#define MPUSB_SUCCESS               1

#define MP_WRITE                    0
#define MP_READ                     1

// MAX_NUM_MPUSB_DEV is an abstract limitation.
// It is very unlikely that a computer system will have more
// then 127 USB devices attached to it. (single or multiple USB hosts)
#define MAX_NUM_MPUSB_DEV           127


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
extern "C" __declspec(dllexport)
DWORD MPUSBGetDLLVersion(void);

///////////////////////////////////////////////////////////////////////////////
//	MPUSBGetDeviceCount : Returns the number of devices with matching VID & PID
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
extern "C" __declspec(dllexport)
DWORD MPUSBGetDeviceCount(PCHAR pVID_PID);

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
extern "C" __declspec(dllexport)
HANDLE MPUSBOpen(DWORD instance,    // Input
                 PCHAR pVID_PID,    // Input
                 PCHAR pEP,         // Input
                 DWORD dwDir,       // Input
                 DWORD dwReserved); // Input <Future Use>

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
extern "C" __declspec(dllexport)
DWORD MPUSBRead(HANDLE handle,              // Input
                PVOID pData,                // Output
                DWORD dwLen,                // Input
                PDWORD pLength,             // Output
                DWORD dwMilliseconds);      // Input

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
extern "C" __declspec(dllexport)
DWORD MPUSBWrite(HANDLE handle,             // Input
                 PVOID pData,               // Input
                 DWORD dwLen,               // Input
                 PDWORD pLength,            // Output
                 DWORD dwMilliseconds);     // Input

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
extern "C" __declspec(dllexport)
DWORD MPUSBReadInt(HANDLE handle,           // Input
                   PVOID pData,             // Output
                   DWORD dwLen,             // Input
                   PDWORD pLength,          // Output
                   DWORD dwMilliseconds);   // Input

///////////////////////////////////////////////////////////////////////////////
//	MPUSBClose : closes a given handle.
//
//	Note that "input" and "output" refer to the parameter designations in calls
//	to this function, which are the opposite of common sense from the
//	perspective of an application making the calls.
//
extern "C" __declspec(dllexport)
BOOL MPUSBClose(HANDLE handle);

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
extern "C" __declspec(dllexport)
DWORD MPUSBGetDeviceDescriptor(HANDLE handle,       // Input
                               PVOID pDevDsc,       // Output
                               DWORD dwLen,         // Input
                               PDWORD pLength);     // Output

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
extern "C" __declspec(dllexport)
DWORD MPUSBGetConfigurationDescriptor(HANDLE handle,       // Input
                               UCHAR bIndex,        // Input
                               PVOID pDevDsc,       // Output
                               DWORD dwLen,         // Input
                               PDWORD pLength);     // Output

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
extern "C" __declspec(dllexport)
DWORD MPUSBGetStringDescriptor(HANDLE handle,       // Input
                               UCHAR bIndex,        // Input
                               USHORT wLangId,      // Input
                               PVOID pDevDsc,       // Output
                               DWORD dwLen,         // Input
                               PDWORD pLength);     // Output

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
extern "C" __declspec(dllexport)
DWORD MPUSBSetConfiguration(HANDLE handle,         // Input
                         USHORT bConfigSetting);           // Input
#endif
