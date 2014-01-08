/******************************************************************************
 *
 *                Microchip Memory Disk Drive File System
 *
 ******************************************************************************
 * FileName:        FSconfig.h
 * Dependencies:    None
 * Processor:       PIC18/PIC24/dsPIC30/dsPIC33
 * Compiler:        C18/C30
 * Company:         Microchip Technology, Inc.
 * Version:         1.0.0
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
*****************************************************************************/


#ifndef _FS_DEF_


#include "HardwareProfile.h"

/***************************************************************************/
/*   Note:  There are likely pin definitions present in the header file    */
/*          for your device (SP-SPI.h, CF-PMP.h, etc).  You may wish to    */
/*          specify these as well                                          */
/***************************************************************************/

// The FS_MAX_FILES_OPEN #define is only applicable when Dynamic
// memeory allocation is not used (FS_DYNAMIC_MEM not defined).
// Defines how many concurent open files can exist at the same time.
// Takes up static memory. If you do not need to open more than one
// file at the same time, then you should set this to 1 to reduce
// memory usage
#define FS_MAX_FILES_OPEN 	1
/************************************************************************/

// The size of a sector
// Must be 512, 1024, 2048, or 4096
// 512 bytes is the value used by most cards
#define MEDIA_SECTOR_SIZE 		512
/************************************************************************/

// Determines processor type
#define USE_PIC32

// Defines the device type
#include "USB/usb_host_msd_scsi.h"

/*************************************************************************/
/*Compiler options to enable/Disable Features based on user's application*/
/*************************************************************************/

// Uncomment this to use the FindFirst, FindNext, and FindPrev
#define ALLOW_FILESEARCH
/************************************************************************/
/************************************************************************/

// Comment this line out if you don't intend to write data to the card
//#define ALLOW_WRITES
/************************************************************************/

// Comment this line out if you don't intend to format your card
// Writes must be enabled to use the format function
//#define ALLOW_FORMATS
/************************************************************************/

// Uncomment this definition if you're using directories
// Writes must be enabled to use directories
//#define ALLOW_DIRS
/************************************************************************/

// Allows the use of FSfopenpgm, FSremovepgm, etc with PIC18
//#define ALLOW_PGMFUNCTIONS
/************************************************************************/

// Allows the use of the FSfprintf function
// Writes must be enabled to use the FSprintf function
//#define ALLOW_FSFPRINTF
/************************************************************************/

// If FAT32 support required then uncomment the following
#define SUPPORT_FAT32
/* **********************************************************************/


// Select how you want the timestamps to be updated
// Use the Real-time clock peripheral to set the clock
// You must configure the RTC in your application code
//#define USEREALTIMECLOCK
// The user will update the timing variables manually using the SetClockVars function
// The user should set the clock before they create a file or directory (Create time),
// and before they close a file (last access time, last modified time)
#define USERDEFINEDCLOCK
// Just increment the time- this will not produce accurate times and dates
//#define INCREMENTTIMESTAMP

// Warnings
#ifndef USEREALTIMECLOCK
    #ifndef USERDEFINEDCLOCK
        #ifndef INCREMENTTIMESTAMP
            #error Please enable USEREALTIMECLOCK, USERDEFINEDCLOCK, or INCREMENTTIMESTAMP
        #endif
    #endif
#endif


// Function definitions
#define MDD_MediaInitialize     USBHostMSDSCSIMediaInitialize
#define MDD_MediaDetect         USBHostMSDSCSIMediaDetect
#define MDD_SectorRead          USBHostMSDSCSISectorRead
#define MDD_SectorWrite         USBHostMSDSCSISectorWrite
#define MDD_InitIO();              
#define MDD_ShutdownMedia       USBHostMSDSCSIMediaReset
#define MDD_WriteProtectState   USBHostMSDSCSIWriteProtectState

#endif
