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
 * Version:         1.2.7
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
#define FS_MAX_FILES_OPEN 	3
/************************************************************************/

// The size of a sector
// Must be 512, 1024, 2048, or 4096
// 512 bytes is the value used by most cards
#define MEDIA_SECTOR_SIZE 		512
/************************************************************************/



/* *******************************************************************************************************/
/************** Compiler options to enable/Disable Features based on user's application ******************/
/* *******************************************************************************************************/


// Uncomment this to use the FindFirst, FindNext, and FindPrev
#define ALLOW_FILESEARCH
/************************************************************************/
/************************************************************************/

// Comment this line out if you don't intend to write data to the card
#define ALLOW_WRITES
/************************************************************************/

// Comment this line out if you don't intend to format your card
// Writes must be enabled to use the format function
#define ALLOW_FORMATS
/************************************************************************/

// Uncomment this definition if you're using directories
// Writes must be enabled to use directories
#define ALLOW_DIRS
/************************************************************************/

// Allows the use of FSfopenpgm, FSremovepgm, etc with PIC18
#if defined(__18CXX)
    #define ALLOW_PGMFUNCTIONS
    #define USE_PIC18
#endif
/************************************************************************/

// Allows the use of the FSfprintf function
// Writes must be enabled to use the FSprintf function
#define ALLOW_FSFPRINTF
/************************************************************************/

// If FAT32 support required then uncomment the following
#define SUPPORT_FAT32
/* ******************************************************************************************************* */




// Select how you want the timestamps to be updated
// Use the Real-time clock peripheral to set the clock
// You must configure the RTC in your application code
//#define USEREALTIMECLOCK
// The user will update the timing variables manually using the SetClockVars function
// The user should set the clock before they create a file or directory (Create time),
// and before they close a file (last access time, last modified time)
//#define USERDEFINEDCLOCK
// Just increment the time- this will not produce accurate times and dates
#define INCREMENTTIMESTAMP


#ifdef USE_PIC18
	#ifdef USEREALTIMECLOCK
		#error Some PIC18 devices do not have a Real-time clock and calander module
	#endif
#endif

#ifdef ALLOW_PGMFUNCTIONS
	#ifndef USE_PIC18
		#error The pgm functions are unneccessary when not using PIC18
	#endif
#endif

#ifndef USEREALTIMECLOCK
    #ifndef USERDEFINEDCLOCK
        #ifndef INCREMENTTIMESTAMP
            #error Please enable USEREALTIMECLOCK, USERDEFINEDCLOCK, or INCREMENTTIMESTAMP
        #endif
    #endif
#endif

/************************************************************************/
// Define FS_DYNAMIC_MEM to use malloc for allocating
// FILE structure space.  uncomment all three lines
/************************************************************************/
#if 0
	#define FS_DYNAMIC_MEM
	#ifdef USE_PIC18
		#define FS_malloc	SRAMalloc
		#define FS_free		SRAMfree
	#else
		#define FS_malloc	malloc
		#define FS_free		free
	#endif
#endif

#define USE_INTERNAL_FLASH
// Function definitions
// Associate the physical layer functions with the correct physical layer
#ifdef USE_SD_INTERFACE_WITH_SPI       // SD-SPI.c and .h

    #define MDD_MediaInitialize     MDD_SDSPI_MediaInitialize
    #define MDD_MediaDetect         MDD_SDSPI_MediaDetect
    #define MDD_SectorRead          MDD_SDSPI_SectorRead
    #define MDD_SectorWrite         MDD_SDSPI_SectorWrite
    #define MDD_InitIO              MDD_SDSPI_InitIO
    #define MDD_ShutdownMedia       MDD_SDSPI_ShutdownMedia
    #define MDD_WriteProtectState   MDD_SDSPI_WriteProtectState
    #define MDD_ReadSectorSize      MDD_SDSPI_ReadSectorSize
    #define MDD_ReadCapacity        MDD_SDSPI_ReadCapacity

#elif defined USE_CF_INTERFACE_WITH_PMP       // CF-PMP.c and .h

    #define MDD_MediaInitialize     MDD_CFPMP_MediaInitialize
    #define MDD_MediaDetect         MDD_CFPMP_MediaDetect
    #define MDD_SectorRead          MDD_CFPMP_SectorRead
    #define MDD_SectorWrite         MDD_CFPMP_SectorWrite
    #define MDD_InitIO              MDD_CFPMP_InitIO
    #define MDD_ShutdownMedia       MDD_CFPMP_ShutdownMedia
    #define MDD_WriteProtectState   MDD_CFPMP_WriteProtectState
    #define MDD_CFwait              MDD_CFPMP_CFwait
    #define MDD_CFwrite             MDD_CFPMP_CFwrite
    #define MDD_CFread              MDD_CFPMP_CFread

#elif defined USE_MANUAL_CF_INTERFACE         // CF-Bit transaction.c and .h

    #define MDD_MediaInitialize     MDD_CFBT_MediaInitialize
    #define MDD_MediaDetect         MDD_CFBT_MediaDetect
    #define MDD_SectorRead          MDD_CFBT_SectorRead
    #define MDD_SectorWrite         MDD_CFBT_SectorWrite
    #define MDD_InitIO              MDD_CFBT_InitIO
    #define MDD_ShutdownMedia       MDD_CFBT_ShutdownMedia
    #define MDD_WriteProtectState   MDD_CFBT_WriteProtectState
    #define MDD_CFwait              MDD_CFBT_CFwait
    #define MDD_CFwrite             MDD_CFBT_CFwrite
    #define MDD_CFread              MDD_CFBT_CFread

#elif defined USE_USB_INTERFACE               // USB host MSD library

    #define MDD_MediaInitialize     USBHostMSDSCSIMediaInitialize
    #define MDD_MediaDetect         USBHostMSDSCSIMediaDetect
    #define MDD_SectorRead          USBHostMSDSCSISectorRead
    #define MDD_SectorWrite         USBHostMSDSCSISectorWrite
    #define MDD_InitIO();              
    #define MDD_ShutdownMedia       USBHostMSDSCSIMediaReset
    #define MDD_WriteProtectState   USBHostMSDSCSIWriteProtectState

#elif defined USE_INTERNAL_FLASH
    #define MDD_MediaInitialize     MDD_IntFlash_MediaInitialize
    #define MDD_MediaDetect         MDD_IntFlash_MediaDetect
    #define MDD_SectorRead          MDD_IntFlash_SectorRead
    #define MDD_SectorWrite         MDD_IntFlash_SectorWrite
    #define MDD_InitIO              MDD_IntFlash_InitIO
    #define MDD_ShutdownMedia       MDD_IntFlash_ShutdownMedia
    #define MDD_WriteProtectState   MDD_IntFlash_WriteProtectState
    #define MDD_ReadSectorSize      MDD_IntFlash_ReadSectorSize
    #define MDD_ReadCapacity        MDD_IntFlash_ReadCapacity

    //---------------------------------------------------------------------------------------
    //The size (in number of sectors) of the desired usable data portion of the MSD volume
    //---------------------------------------------------------------------------------------
    //Note: Windows 7 appears to require a minimum capacity of at least 13 sectors.
    //Note2: Windows will not be able to format a drive if it is too small.  The reason
    //for this, is that Windows will try to put a "heavyweight" (comparatively) filesystem
    //on the drive, which will consume ~18kB of overhead for the filesystem.  If the total
    //drive size is too small to fit the filesystem, then Windows will give an error.    
    //This also means that formatting the drive will "shrink" the usuable data storage
    //area, since the default FAT12 filesystem implemented in the Files.c data tables is very
    //lightweight, with very low overhead.
    //Note3: It is important to make sure that no part of the MSD volume shares a flash
    //erase page with the firmware program memory.  This can be done by using a custom
    //modified linker script, or by carefully selecting the starting address and the 
    //total size of the MSD volume.  See also below code comments.
    //Note4: It is also important to make sure that no part of the MSD volume shares
    //an erase page with the erase page that contains the microcontroller's configuration
    //bits (for microcontrollers that use flash for storing the configuration bits, 
    //see device datasheet). This can be accomplished by using a modified linker script,
    //which protects the flash page with the configuration bits (if applicable), or,
    //by carefully choosing the FILES_ADDRESS and MDD_INTERNAL_FLASH_DRIVE_CAPACITY,
    //to make sure the MSD volume does extend into the erase page with the configuration
    //bits.
    #define MDD_INTERNAL_FLASH_DRIVE_CAPACITY 14


    //--------------------------------------------------------------------------
    //Starting Address of the MSD Volume.
    //--------------------------------------------------------------------------
    //Note: Make certain that this starting address is aligned with the start
    //of a flash erase block.  It is important to make certain that no part of
    //the MSD volume overlaps any portion of a flash erase page which is used
    //for storing firmware program code.  When the host writes a sector to the 
    //MSD volume, the firmware must erase an entire page of flash in order to
    //do the write.  If the sector being written happened to share a flash erase
    //page with this firmware, unpredictable results would occur, since part of
    //firmware would also end up getting erased during the write to the MSD volume.
    #define FILES_ADDRESS 0x2000


    //--------------------------------------------------------------------------
    //Maximum files supported
    //--------------------------------------------------------------------------
    //MDD_INTERNAL_FLASH_MAX_NUM_FILES_IN_ROOT must be a multiple of 16
    //Note: Even if MDD_INTERNAL_FLASH_MAX_NUM_FILES_IN_ROOT is 16, this does not
    //necessarily mean the drive will always work with 16 files.  The drive will
    //suppport "up to" 16 files, but other limits could be hit first, even before
    //the drive is full.  The RootDirectory0[] sector could get full with less
    //files, especially if the files are using long filenames.  
    #define MDD_INTERNAL_FLASH_MAX_NUM_FILES_IN_ROOT 16
 


    //name and extern definition for the master boot record
    extern ROM BYTE MasterBootRecord[512];
    #define MASTER_BOOT_RECORD_ADDRESS &MasterBootRecord[0]

    //#define INTERNAL_FLASH_WRITE_PROTECT
#endif

#endif
