/*********************************************************************
 *
 *                Example 02 - Run-time Linking
 *
 *********************************************************************
 * FileName:        console.cpp
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rawin Rojvanit       11/19/04
 ********************************************************************/

//---------------------------------------------------------------------------

#pragma hdrstop

#include <stdio.h>
#include "windows.h"
#include "mpusbapi.h"                   // MPUSBAPI Header File

//---------------------------------------------------------------------------
#pragma argsused

// Global Vars
char vid_pid[]= "vid_04d8&pid_000c";    // Default Demo Application Firmware
char out_pipe[]= "\\MCHP_EP1";
char in_pipe[]= "\\MCHP_EP1";

DWORD temp;

HINSTANCE libHandle;
HANDLE myOutPipe;
HANDLE myInPipe;

//---------------------------------------------------------------------------
// Prototypes
void GetSummary(void);
void LoadDLL(void);
void GetUSBDemoFWVersion(void);
DWORD SendReceivePacket(BYTE *SendData, DWORD SendLength, BYTE *ReceiveData,
                    DWORD *ReceiveLength, UINT SendDelay, UINT ReceiveDelay);
void CheckInvalidHandle(void);
void SetConfigurationExample(void);
void GetDescritorExample(void);

//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    BOOLEAN bQuit;
    DWORD selection;
    bQuit = false;

    // Load DLL when it is necessary, i.e. on start-up!
    LoadDLL();

    // Always a good idea to initialize the handles
    myOutPipe = myInPipe = INVALID_HANDLE_VALUE;

    printf("Microchip Technology Inc., 2004\r\n");
    printf("===============================\r\n");
    while(!bQuit)
    {
        printf("Select an option\r\n");
        printf("[1] Get MPUSBAPI Version\r\n");
        printf("[2] Summarize Instances\r\n");
        printf("[3] Set Configuration\r\n");
        printf("[4] Get Descriptor Information\r\n");
        printf("[5] Quit\r\n>>");
        scanf("%d",&selection);

        switch(selection)
        {
            case 1:
                temp = MPUSBGetDLLVersion();
                printf("MPUSBAPI Version: %d.%d\r\n",HIWORD(temp),LOWORD(temp));
                break;
            case 2:
                GetSummary();
                break;
            case 3:
                SetConfigurationExample();
                break;
            case 4:
                GetDescritorExample();
                break;
            case 5:
                bQuit = true;
                break;
            default:
                break;
        }// end switch

        fflush(stdin);printf("\r\n");
    }//end while

    // Always check to close all handles before exiting!
    if (myOutPipe != INVALID_HANDLE_VALUE) MPUSBClose(myOutPipe);
    if (myInPipe != INVALID_HANDLE_VALUE) MPUSBClose(myInPipe);
    myOutPipe = myInPipe = INVALID_HANDLE_VALUE;

    // Always check to close the library too.
    if (libHandle != NULL) FreeLibrary(libHandle);

    return 0;
}//end main

//---------------------------------------------------------------------------

void SetConfigurationExample(void)
{
    HANDLE tempPipe = INVALID_HANDLE_VALUE;
    DWORD selection1;
    DWORD selection2;
    DWORD results;

    printf("\r\nEnter device instance number:\r\n>>");
    scanf("%d",&selection1);

    printf("\r\nEnter the configuration number:\r\n>>");
    scanf("%d",&selection2);

    tempPipe = MPUSBOpen(selection1,vid_pid,NULL,MP_WRITE,0);

    if(tempPipe != INVALID_HANDLE_VALUE)
    {
        results = MPUSBSetConfiguration(tempPipe,selection2);

        if(results != MPUSB_SUCCESS)
        {
            printf("\r\nUnable to set configuration.\r\n");
        }
        else
        {
            printf("\r\nSet configuration successful.\r\n");
        }
        MPUSBClose(tempPipe);
    }
    else
    {
        printf("\r\nInstance number %d is not valid.\r\n");
    }
}

void GetDescritorExample(void)
{
    HANDLE tempPipe = INVALID_HANDLE_VALUE;
    DWORD selection1;
    DWORD selection2;
    DWORD selection3;
    BYTE descriptor[64];
    DWORD resultingLength;
    DWORD results;

    results = MPUSBGetDeviceCount(vid_pid);

    selection1 = 0;
    if(results == 0)
    {
        printf("\r\nNo attached device.\r\n");
        return;
    }
    if(results > 1)
    {
        printf("\r\nEnter device instance number:\r\n>>");
        scanf("%d",&selection1);
    }


    printf("\r\nSelect Descriptor type:\r\n");
    printf("[1] Device Descriptor\r\n");
    printf("[2] Configuration Descriptor\r\n");
    printf("[3] String Descriptor\r\n>>");
    scanf("%d",&selection2);

    tempPipe = MPUSBOpen(selection1,vid_pid,NULL,MP_READ,0);

    if(tempPipe == INVALID_HANDLE_VALUE)
    {
        printf("\r\nInstance number %d is not valid.\r\n");
        return;
    }

    #define DEVICE_DESCRIPTOR_TYPE 1
    #define CONFIGURATION_DESCRIPTOR_TYPE 2
    #define STRING_DESCRIPTOR_TYPE 3
    switch(selection2)
    {
        case DEVICE_DESCRIPTOR_TYPE:
            //Print the device descriptor
            results = MPUSBGetDeviceDescriptor
            (
                tempPipe,
                &descriptor[0],
                sizeof(descriptor),
                &resultingLength
            );

            if(results == MPUSB_SUCCESS)
            {
                printf("\r\nDevice Descriptor:\r\n");

                for(DWORD j = 0; j < resultingLength; j++)
                {
                    printf("%02x ",descriptor[j]);

                    if((j+1)%8 == 0)
                    {
                        printf("\r\n");
                    }
                }
            }
            break;

        case CONFIGURATION_DESCRIPTOR_TYPE:
            printf("\r\nSelect configuration number:\r\n>>");
            scanf("%d",&selection3);

            //Print the configuration descriptor
            results = MPUSBGetConfigurationDescriptor
            (
                tempPipe,
                selection3,  //Index
                &descriptor[0],
                sizeof(descriptor),
                &resultingLength
            );
            if(results == MPUSB_SUCCESS)
            {
                printf("\r\n\r\nConfiguration Descriptor:\r\n");

                for(DWORD j = 0; j < resultingLength; j++)
                {
                    printf("%02x ",descriptor[j]);
                    if((j+1)%8 == 0)
                    {
                        printf("\r\n");
                    }
                }
            }
            break;

        case STRING_DESCRIPTOR_TYPE:
            printf("\r\nFixed LangId: 0x0409\r\n");
            printf("\r\nSelect string index:\r\n>>");
            scanf("%d",&selection3);

            results = MPUSBGetStringDescriptor
            (
                tempPipe,
                selection3,  //Index
                0x0409,  //LangId
                &descriptor[0],
                sizeof(descriptor),
                &resultingLength
            );

            if(results == MPUSB_SUCCESS)
            {
                printf("\r\nString Descriptor Index %d:\r\n",selection3);

                if(selection3 == 0)
                {
                    for(DWORD j = 0; j < resultingLength; j++)
                    {
                        printf("%02x ",descriptor[j]);
                        if((j+1)%8 == 0)
                        {
                            printf("\r\n");
                        }
                    }
                }
                else
                {
                    //Print out every other character starting from byte 2.
                    //  the first two bytes are the size and STRING descriptor
                    //  constant.  The actual data starts at byte 2.
                    for(DWORD j = 2; j < resultingLength; j++)
                    {
                        if((j%2) == 0)
                        {
                            printf("%c",descriptor[j]);
                        }
                    }
                }
            }
            break;
    }
    printf("\r\n");
}

void GetSummary(void)
{
    HANDLE tempPipe = INVALID_HANDLE_VALUE;
    DWORD count = 0;
    DWORD max_count;

    max_count = MPUSBGetDeviceCount(vid_pid);

    printf("\r\n%d device(s) with %s currently attached\r\n",max_count,vid_pid);

    // Note:
    // The total number of devices using the generic driver could be
    // bigger than max_count. They could have different vid & pid numbers.
    // This means if max_count is 2, the valid instance index do not
    // necessary have to be '0' and '1'.
    //
    // Below is a sample code for searching for all valid instance indexes.
    // MAX_NUM_MPUSB_DEV is defined in _mpusbapi.h

    count = 0;
    for(int i = 0; i < MAX_NUM_MPUSB_DEV; i++)
    {
        tempPipe = MPUSBOpen(i,vid_pid,NULL,MP_READ,0);
        if(tempPipe != INVALID_HANDLE_VALUE)
        {
            printf("Instance Index # %d\r\n",i);
            MPUSBClose(tempPipe);
            count++;
        }
        if(count == max_count) break;
    }//end for
    printf("\r\n");
}//end GetSummary

//---------------------------------------------------------------------------

void LoadDLL(void)
{
    libHandle = NULL;
    libHandle = LoadLibrary("mpusbapi");
    if(libHandle == NULL)
    {
        printf("Error loading mpusbapi.dll\r\n");
    }
    else
    {
        MPUSBGetDLLVersion=(DWORD(*)(void))\
                    GetProcAddress(libHandle,"_MPUSBGetDLLVersion");
        MPUSBGetDeviceCount=(DWORD(*)(PCHAR))\
                    GetProcAddress(libHandle,"_MPUSBGetDeviceCount");
        MPUSBOpen=(HANDLE(*)(DWORD,PCHAR,PCHAR,DWORD,DWORD))\
                    GetProcAddress(libHandle,"_MPUSBOpen");
        MPUSBWrite=(DWORD(*)(HANDLE,PVOID,DWORD,PDWORD,DWORD))\
                    GetProcAddress(libHandle,"_MPUSBWrite");
        MPUSBRead=(DWORD(*)(HANDLE,PVOID,DWORD,PDWORD,DWORD))\
                    GetProcAddress(libHandle,"_MPUSBRead");
        MPUSBReadInt=(DWORD(*)(HANDLE,PVOID,DWORD,PDWORD,DWORD))\
                    GetProcAddress(libHandle,"_MPUSBReadInt");
        MPUSBClose=(BOOL(*)(HANDLE))GetProcAddress(libHandle,"_MPUSBClose");
        MPUSBSetConfiguration=(DWORD(*)(HANDLE,USHORT))\
                    GetProcAddress(libHandle,"_MPUSBSetConfiguration");
        MPUSBGetStringDescriptor = \
                    (DWORD(*)(HANDLE,UCHAR,USHORT,PVOID,DWORD,PDWORD))\
                    GetProcAddress(libHandle,"_MPUSBGetStringDescriptor");
        MPUSBGetConfigurationDescriptor = \
                   (DWORD(*)(HANDLE,UCHAR,PVOID,DWORD,PDWORD))\
                   GetProcAddress(libHandle,"_MPUSBGetConfigurationDescriptor");
        MPUSBGetDeviceDescriptor = (DWORD(*)(HANDLE,PVOID,DWORD,PDWORD))\
                   GetProcAddress(libHandle,"_MPUSBGetDeviceDescriptor");

        if((MPUSBGetDeviceCount == NULL) || (MPUSBOpen == NULL) ||
            (MPUSBWrite == NULL) || (MPUSBRead == NULL) ||
            (MPUSBClose == NULL) || (MPUSBGetDLLVersion == NULL) ||
            (MPUSBReadInt == NULL) || (MPUSBSetConfiguration == NULL) ||
            (MPUSBGetStringDescriptor == NULL) ||
            (MPUSBGetConfigurationDescriptor == NULL) ||
            (MPUSBGetDeviceDescriptor == NULL))
            printf("GetProcAddress Error\r\n");
    }//end if else
}//end LoadDLL

//---------------------------------------------------------------------------

void GetUSBDemoFWVersion(void)
{
    // First we need to open data pipes...
    DWORD selection;
    fflush(stdin);
    printf("Enter a valid instance index to open a USB connection: ");
    scanf("%d",&selection);

    myOutPipe = MPUSBOpen(selection,vid_pid,out_pipe,MP_WRITE,0);
    myInPipe = MPUSBOpen(selection,vid_pid,out_pipe,MP_READ,0);
    if(myOutPipe == INVALID_HANDLE_VALUE || myInPipe == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open data pipes.\r\n");
        return;
    }//end if


    // The default demo firmware application has a defined application
    // level protocol.
    // To read the firmware version, the host must send the READ_VERSION
    // command which is defined as 0x00, follows by the length of the
    // expected result, in this case is 2 bytes, major version, and minor
    // version.
    // i.e. <READ_VERSION><0x02>
    //
    // The response packet from the board has the following format:
    // <READ_VERSION><0x02><MINOR_VER><MAJOR_VER>

    // The receive buffer size must be equal to or larger than the maximum
    // endpoint size it is communicating with. In this case, 64 bytes.

    BYTE send_buf[64],receive_buf[64];
    DWORD RecvLength=4;

    #define READ_VERSION    0
    send_buf[0] = READ_VERSION;      // Command
    send_buf[1] = 0x02;              // Expected length of the result

    if(SendReceivePacket(send_buf,2,receive_buf,&RecvLength,1000,1000) == 1)
    {
        if(RecvLength == 4 && receive_buf[0] == READ_VERSION &&
            receive_buf[1] == 0x02)
        {
            printf("\nUSB Demo Firmware Version %d.%d\r\n",receive_buf[3],
                    receive_buf[2]);
        }
    }
    else
        printf("USB Operation Failed\r\n");

    // Let's close the data pipes since we have nothing left to do..
    MPUSBClose(myOutPipe);
    MPUSBClose(myInPipe);
    myOutPipe = myInPipe = INVALID_HANDLE_VALUE;
    
}//end GetUSBDemoFWVersion

//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//
// A typical application would send a command to the target device and expect
// a response.
// SendReceivePacket is a wrapper function that facilitates the
// send command / read response paradigm
//
// SendData - pointer to data to be sent
// SendLength - length of data to be sent
// ReceiveData - Points to the buffer that receives the data read from the call
// ReceiveLength - Points to the number of bytes read
// SendDelay - time-out value for MPUSBWrite operation in milliseconds
// ReceiveDelay - time-out value for MPUSBRead operation in milliseconds
//

DWORD SendReceivePacket(BYTE *SendData, DWORD SendLength, BYTE *ReceiveData,
                    DWORD *ReceiveLength, UINT SendDelay, UINT ReceiveDelay)
{
    DWORD SentDataLength;
    DWORD ExpectedReceiveLength = *ReceiveLength;

    if(myOutPipe != INVALID_HANDLE_VALUE && myInPipe != INVALID_HANDLE_VALUE)
    {
        if(MPUSBWrite(myOutPipe,SendData,SendLength,&SentDataLength,SendDelay))
        {

            if(MPUSBRead(myInPipe,ReceiveData, ExpectedReceiveLength,
                        ReceiveLength,ReceiveDelay))
            {
                if(*ReceiveLength == ExpectedReceiveLength)
                {
                    return 1;   // Success!
                }
                else if(*ReceiveLength < ExpectedReceiveLength)
                {
                    return 2;   // Partially failed, incorrect receive length
                }//end if else
            }
            else
                CheckInvalidHandle();
        }
        else
            CheckInvalidHandle();
    }//end if

    return 0;  // Operation Failed
}//end SendReceivePacket

//---------------------------------------------------------------------------

void CheckInvalidHandle(void)
{
    if(GetLastError() == ERROR_INVALID_HANDLE)
    {
        // Most likely cause of the error is the board was disconnected.
        MPUSBClose(myOutPipe);
        MPUSBClose(myInPipe);
        myOutPipe = myInPipe = INVALID_HANDLE_VALUE;
    }//end if
    else
        printf("Error Code \r\n",GetLastError());
}//end CheckInvalidHandle

//---------------------------------------------------------------------------
