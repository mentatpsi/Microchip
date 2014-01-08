/******************************************************************************

    USB Host Mass Storage Class SCSI Interface Driver

This file provides the interface between the file system and the USB Host Mass
Storage class.  It translates the file system funtionality requirements to the
appropriate SCSI commands, and sends the SCSI commands via the USB Mass
Storage class.

The functions in this file are designed to interface the Microchip Memory Disk
Drive File System library (see Application Note AN1045) to the USB Host Mass
Storage Class, allowing a PIC application to utilize mass storage devices such
as USB flash drives.  For ease of integration, this file contains macros to
allow the File System code to reference the functions in this file.

Currently, the file system layer above this interface layer is limited to one
LUN (Logical Unit Number) on a single mass storage device.  This layer accepts
and stores the max LUN from the USB MSD layer, but all sector reads and writes
are hard-coded to LUN 0, since the layer above does not specify a LUN in the
sector read and write commands.  Also, to interface with the existing file
system code, only one attached device is allowed.

FileName:        usb_host_msd_scsi.c
Dependencies:    Microchip Memory Disk Drive File System v1.01
Processor:       PIC24/dsPIC30/dsPIC33/PIC32MX
Compiler:        C30/C32
Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the �Company�) for its PICmicro� Microcontroller is intended and
supplied to you, the Company�s customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
software is owned by the Company and/or its supplier, and is
protected under applicable copyright laws. All rights are reserved.
Any use in violation of the foregoing restrictions may subject the
user to criminal sanctions under applicable laws, as well as to
civil liability for the breach of the terms and conditions of this
license.

THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

Change History:
  Rev         Description
  ----------  ----------------------------------------------------------
  2.6 - 2.7a  No change

*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "FSconfig.h"
#include "MDD File System/FSDefs.h"
#include "MDD File System/FSIO.h"
#include "USB/usb.h"
#include "USB/usb_host_msd.h"
#include "USB/usb_host_msd_scsi.h"

//#define DEBUG_MODE
#if defined(DEBUG_MODE)
    #include "uart2.h"
#endif

#define PERFORM_TEST_UNIT_READY

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

#define FAT_GOOD_SIGN_0             0x55        // The value in the Signature0 field of a valid partition.
#define FAT_GOOD_SIGN_1             0xAA        // The value in the Signature1 field of a valid partition.
#define FO_MBR                      0L          // Master Boot Record sector LBA
#define FUA_ALLOW_CACHE             0x00        // Force Unit Access, allow cache use
#define INITIALIZATION_ATTEMPTS     100         // How many times to try to initialize the media before failing
#define RDPROTECT_NORMAL            0x00        // Normal Read Protect behavior.
#define WRPROTECT_NORMAL            0x00        // Normal Write Protect behavior.


//******************************************************************************
//******************************************************************************
// Section: Local Prototypes and Macros
//******************************************************************************
//******************************************************************************

#if !defined( USBTasks )
    #define USBTasks()                  \
        {                               \
            USBHostTasks();             \
            USBHostMSDTasks();          \
        }
#endif

#if defined( PERFORM_TEST_UNIT_READY )
    BOOL    _USBHostMSDSCSI_TestUnitReady( void );
#endif


//******************************************************************************
//******************************************************************************
// Section: SCSI MSD Host Global Variables
//******************************************************************************
//******************************************************************************

//******************************************************************************
// Section: Internal Global Variables
//******************************************************************************

static BYTE                deviceAddress = 0;  // USB address of the attached device.
static MEDIA_INFORMATION   mediaInformation;   // Information about the attached media.

// *****************************************************************************
// *****************************************************************************
// Section: MSD Host Stack Callback Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BOOL USBHostMSDSCSIInitialize( BYTE address, DWORD flags, BYTE clientDriverID )

  Description:
    This function is called when a USB Mass Storage device is being
    enumerated.

  Precondition:
    None

  Parameters:
    BYTE address    -   Address of the new device
    DWORD flags     -   Initialization flags
    BYTE clientDriverID - ID for this layer.  Not used by the media interface layer.

  Return Values:
    TRUE    -   We can support the device.
    FALSE   -   We cannot support the device.

  Remarks:
    None
  ***************************************************************************/

BOOL USBHostMSDSCSIInitialize( BYTE address, DWORD flags, BYTE clientDriverID )
{
    #ifdef DEBUG_MODE
        UART2PrintString( "SCSI: Device attached.\r\n" );
    #endif

    if (deviceAddress == 0)
    {
        // Save the address of the new device.
        deviceAddress = address;
        return TRUE;
    }
    else
    {
        // We can currently only handle one device.
        return FALSE;
    }
}


/****************************************************************************
  Function:
    BOOL USBHostMSDSCSIEventHandler( BYTE address, USB_EVENT event,
                        void *data, DWORD size )

  Description:
    This function is called when various events occur in the USB Host Mass
    Storage client driver.

  Precondition:
    The device has been initialized.

  Parameters:
    BYTE address    -   Address of the device
    USB_EVENT event -   Event that has occurred
    void *data      -   Pointer to data pertinent to the event
    DWORD size      -   Size of the data

  Return Values:
    TRUE    -   Event was handled
    FALSE   -   Event was not handled

  Remarks:
    None
  ***************************************************************************/

BOOL USBHostMSDSCSIEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    if (deviceAddress == address)
    {
        switch( (INT) event )
        {
            case EVENT_MSD_NONE:
            case EVENT_MSD_TRANSFER:                 // A MSD transfer has completed
                return TRUE;
                break;

            case EVENT_MSD_RESET:
                #ifdef DEBUG_MODE
                    UART2PrintString( "SCSI: MSD Reset performed.\r\n" );
                #endif
                return TRUE;
                break;

            case EVENT_MSD_MAX_LUN:
                #ifdef DEBUG_MODE
                    UART2PrintString( "SCSI: Max LUN set.\r\n" );
                #endif
                mediaInformation.maxLUN                     = *((BYTE *)data);
                mediaInformation.validityFlags.bits.maxLUN  = 1;
                return TRUE;
                break;

            case EVENT_DETACH:                      // USB cable has been detached (data: BYTE, address of device)
                #ifdef DEBUG_MODE
                    UART2PrintString( "SCSI: Device detached.\r\n" );
                #endif
                deviceAddress                           = 0;
                mediaInformation.validityFlags.value    = 0;
                return TRUE;
                break;

            default:
                return FALSE;
                break;
        }
    }
    return FALSE;
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Callable Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BYTE USBHostMSDSCSIMediaDetect( void )

  Description:
    This function determines if a mass storage device is attached and ready
    to use.

  Precondition:
    None

  Parameters:
    None - None

  Return Values:
    TRUE    -   MSD present and ready
    FALSE   -   MSD not present or not ready

  Remarks:
    Since this will often be called in a loop while waiting for a device,
    we need to make sure that USB tasks are executed.
  ***************************************************************************/

BYTE USBHostMSDSCSIMediaDetect( void )
{
    if (USBHostMSDDeviceStatus( deviceAddress ) == USB_MSD_NORMAL_RUNNING)
    {
        return TRUE;
    }

    return FALSE;
}


/****************************************************************************
  Function:
    MEDIA_INFORMATION * USBHostMSDSCSIMediaInitialize( void )

  Description:
    This function initializes the media.

  Precondition:
    None

  Parameters:
    None - None

  Returns:
    The function returns a pointer to the MEDIA_INFORMATION structure.  The
    errorCode member may contain the following values:
        * MEDIA_NO_ERROR - The media initialized successfully, and the
                sector size should be valid (confirm using the validityFlags
                bit).
        * MEDIA_DEVICE_NOT_PRESENT - The requested device is not attached.
        * MEDIA_CANNOT_INITIALIZE - Cannot initialize the media.

  Remarks:
    This function performs the following SCSI commands:
                        * READ CAPACITY 10
                        * REQUEST SENSE

    The READ CAPACITY 10 command block is as follows:

    <code>
        Byte/Bit    7       6       5       4       3       2       1       0
           0                    Operation Code (0x25)
           1        [                      Reserved                         ]
           2        [ (MSB)
           3                        Logical Block Address
           4
           5                                                          (LSB) ]
           6        [                      Reserved
           7                                                                ]
           8        [                      Reserved                 ] [ PMI ]
           9        [                    Control                            ]
    </code>

    The REQUEST SENSE command block is as follows:

    <code>
        Byte/Bit    7       6       5       4       3       2       1       0
           0                    Operation Code (0x02)
           1        [                      Reserved                 ] [ DESC]
           2        [                      Reserved
           3                                                                ]
           4        [                  Allocation Length                    ]
           5        [                    Control                            ]
    </code>
  ***************************************************************************/

MEDIA_INFORMATION * USBHostMSDSCSIMediaInitialize( void )
{
    BYTE        attempts;
    DWORD       byteCount;
    BYTE        commandBlock[10];
    BYTE        errorCode;
    BYTE        inquiryData[36];

    // Make sure the device is still attached.
    if (deviceAddress == 0)
    {
        mediaInformation.errorCode = MEDIA_DEVICE_NOT_PRESENT;
        return &mediaInformation;
    }

    attempts = INITIALIZATION_ATTEMPTS;
    while (attempts != 0)
    {
        attempts --;

        #ifdef DEBUG_MODE
            UART2PrintString( "SCSI: READ CAPACITY 10..." );
        #endif

        // Fill in the command block with the READ CAPACITY 10 parameters.
        commandBlock[0] = 0x25;     // Operation Code
        commandBlock[1] = 0;        //
        commandBlock[2] = 0;        //
        commandBlock[3] = 0;        //
        commandBlock[4] = 0;        //
        commandBlock[5] = 0;        //
        commandBlock[6] = 0;        //
        commandBlock[7] = 0;        //
        commandBlock[8] = 0;        //
        commandBlock[9] = 0x00;     // Control

        errorCode = USBHostMSDRead( deviceAddress, 0, commandBlock, 10, inquiryData, 8 );
        #ifdef DEBUG_MODE
            UART2PutHex( errorCode ) ;
            UART2PutChar( ' ' );
        #endif

        if (!errorCode)
        {
            while (!USBHostMSDTransferIsComplete( deviceAddress, &errorCode, &byteCount ))
            {
                USBTasks();
            }
        }

        if (!errorCode)
        {
            #ifdef DEBUG_MODE
                UART2PutHex( errorCode ) ;
                UART2PrintString( "\r\n" );
            #endif

            // Determine sector size.
            #ifdef DEBUG_MODE
                UART2PrintString( "SCSI: Sector size:" );
                UART2PutChar( inquiryData[7] + '0' );
                UART2PutChar( inquiryData[6] + '0' );
                UART2PutChar( inquiryData[5] + '0' );
                UART2PutChar( inquiryData[4] + '0' );
                UART2PrintString( "\r\n" );
            #endif
            mediaInformation.sectorSize                     = (inquiryData[7] << 12) + (inquiryData[6] << 8) + (inquiryData[5] << 4) + (inquiryData[4]);
            mediaInformation.validityFlags.bits.sectorSize  = 1;

            mediaInformation.errorCode = MEDIA_NO_ERROR;
            return &mediaInformation;
        }
        else
        {
            // Perform a Request Sense to try to clear the stall.
            #ifdef DEBUG_MODE
                UART2PrintString( "SCSI: REQUEST SENSE..." );
            #endif

            // Fill in the command block with the REQUEST SENSE parameters.
            commandBlock[0] = 0x03;     // Operation Code
            commandBlock[1] = 0;        //
            commandBlock[2] = 0;        //
            commandBlock[3] = 0;        //
            commandBlock[4] = 18;       // Allocation length
            commandBlock[5] = 0;        // Control

            errorCode = USBHostMSDRead( deviceAddress, 0, commandBlock, 6, inquiryData, 18 );
            #ifdef DEBUG_MODE
                UART2PutHex( errorCode ) ;
                UART2PutChar( ' ' );
            #endif

            if (!errorCode)
            {
                while (!USBHostMSDTransferIsComplete( deviceAddress, &errorCode, &byteCount ))
                {
                    USBTasks();
                }
            }
        }
    }

    attempts = INITIALIZATION_ATTEMPTS;
    while (attempts != 0)
    {
        attempts --;

        // Perform a Request Sense to try to clear the stall.
        #ifdef DEBUG_MODE
            UART2PrintString( "SCSI: REQUEST SENSE..." );
        #endif

        // Fill in the command block with the REQUEST SENSE parameters.
        commandBlock[0] = 0x03;     // Operation Code
        commandBlock[1] = 0;        //
        commandBlock[2] = 0;        //
        commandBlock[3] = 0;        //
        commandBlock[4] = 18;       // Allocation length
        commandBlock[5] = 0;        // Control

        errorCode = USBHostMSDRead( deviceAddress, 0, commandBlock, 6, inquiryData, 18 );
        #ifdef DEBUG_MODE
            UART2PutHex( errorCode ) ;
            UART2PutChar( ' ' );
        #endif

        if (!errorCode)
        {
            while (!USBHostMSDTransferIsComplete( deviceAddress, &errorCode, &byteCount ))
            {
                USBTasks();
            }
        }


        _USBHostMSDSCSI_TestUnitReady();


        // Perform a Request Sense to try to clear the stall.
        #ifdef DEBUG_MODE
            UART2PrintString( "SCSI: REQUEST SENSE..." );
        #endif

        // Fill in the command block with the REQUEST SENSE parameters.
        commandBlock[0] = 0x03;     // Operation Code
        commandBlock[1] = 0;        //
        commandBlock[2] = 0;        //
        commandBlock[3] = 0;        //
        commandBlock[4] = 18;       // Allocation length
        commandBlock[5] = 0;        // Control

        errorCode = USBHostMSDRead( deviceAddress, 0, commandBlock, 6, inquiryData, 18 );
        #ifdef DEBUG_MODE
            UART2PutHex( errorCode ) ;
            UART2PutChar( ' ' );
        #endif

        if (!errorCode)
        {
            while (!USBHostMSDTransferIsComplete( deviceAddress, &errorCode, &byteCount ))
            {
                USBTasks();
            }
        }

        #ifdef DEBUG_MODE
            UART2PrintString( "SCSI: READ CAPACITY 10..." );
        #endif

        // Fill in the command block with the READ CAPACITY 10 parameters.
        commandBlock[0] = 0x25;     // Operation Code
        commandBlock[1] = 0;        //
        commandBlock[2] = 0;        //
        commandBlock[3] = 0;        //
        commandBlock[4] = 0;        //
        commandBlock[5] = 0;        //
        commandBlock[6] = 0;        //
        commandBlock[7] = 0;        //
        commandBlock[8] = 0;        //
        commandBlock[9] = 0x00;     // Control

        errorCode = USBHostMSDRead( deviceAddress, 0, commandBlock, 10, inquiryData, 8 );
        #ifdef DEBUG_MODE
            UART2PutHex( errorCode ) ;
            UART2PutChar( ' ' );
        #endif

        if (!errorCode)
        {
            while (!USBHostMSDTransferIsComplete( deviceAddress, &errorCode, &byteCount ))
            {
                USBTasks();
            }
        }

        if (!errorCode)
        {
            #ifdef DEBUG_MODE
                UART2PutHex( errorCode ) ;
                UART2PrintString( "\r\n" );
            #endif

            // Determine sector size.
            #ifdef DEBUG_MODE
                UART2PrintString( "SCSI: Sector size:" );
                UART2PutChar( inquiryData[7] + '0' );
                UART2PutChar( inquiryData[6] + '0' );
                UART2PutChar( inquiryData[5] + '0' );
                UART2PutChar( inquiryData[4] + '0' );
                UART2PrintString( "\r\n" );
            #endif
            mediaInformation.sectorSize                     = (inquiryData[7] << 12) + (inquiryData[6] << 8) + (inquiryData[5] << 4) + (inquiryData[4]);
            mediaInformation.validityFlags.bits.sectorSize  = 1;

            mediaInformation.errorCode = MEDIA_NO_ERROR;
            return &mediaInformation;
        }
        else
        {
            // Perform a Request Sense to try to clear the stall.
            #ifdef DEBUG_MODE
                UART2PrintString( "SCSI: REQUEST SENSE..." );
            #endif

            // Fill in the command block with the REQUEST SENSE parameters.
            commandBlock[0] = 0x03;     // Operation Code
            commandBlock[1] = 0;        //
            commandBlock[2] = 0;        //
            commandBlock[3] = 0;        //
            commandBlock[4] = 18;       // Allocation length
            commandBlock[5] = 0;        // Control

            errorCode = USBHostMSDRead( deviceAddress, 0, commandBlock, 6, inquiryData, 18 );
            #ifdef DEBUG_MODE
                UART2PutHex( errorCode ) ;
                UART2PutChar( ' ' );
            #endif

            if (!errorCode)
            {
                while (!USBHostMSDTransferIsComplete( deviceAddress, &errorCode, &byteCount ))
                {
                    USBTasks();
                }
            }
        }
    }

    mediaInformation.errorCode = MEDIA_CANNOT_INITIALIZE;
    return &mediaInformation;

}


/****************************************************************************
  Function:
    BOOL USBHostMSDSCSIMediaReset( void  )

  Summary:
    This function resets the media.

  Description:
    This function resets the media.  It is called if an operation returns an
    error.  Or the application can call it.

  Precondition:
    None

  Parameters:
    None - None

  Return Values:
    USB_SUCCESS                 - Reset successful
    USB_MSD_DEVICE_NOT_FOUND    - No device with specified address
    USB_ILLEGAL_REQUEST         - Device is in an illegal USB state
                                  for reset

  Remarks:
    None
  ***************************************************************************/

BYTE USBHostMSDSCSIMediaReset( void  )
{
    DWORD   byteCount;
    BYTE    errorCode;

    errorCode = USBHostMSDResetDevice( deviceAddress );
    if (errorCode)
    {
        return errorCode;
    }

    do
    {
        USBTasks();
        errorCode = USBHostMSDDeviceStatus( deviceAddress );
    } while (errorCode == USB_MSD_RESETTING_DEVICE);


    if (USBHostMSDTransferIsComplete( deviceAddress, &errorCode, &byteCount ))
    {
        return errorCode;
    }

    return USB_MSD_RESET_ERROR;
}


/****************************************************************************
  Function:
    BYTE USBHostMSDSCSISectorRead( DWORD sectorAddress, BYTE *dataBuffer)

  Summary:
    This function reads one sector.

  Description:
    This function uses the SCSI command READ10 to read one sector.  The size
    of the sector was determined in the USBHostMSDSCSIMediaInitialize()
    function.  The data is stored in the application buffer.

  Precondition:
    None

  Parameters:
    DWORD   sectorAddress   - address of sector to read
    BYTE    *dataBuffer     - buffer to store data

  Return Values:
    TRUE    - read performed successfully
    FALSE   - read was not successful

  Remarks:
    The READ10 command block is as follows:

    <code>
        Byte/Bit    7       6       5       4       3       2       1       0
           0                    Operation Code (0x28)
           1        [    RDPROTECT      ]  DPO     FUA      -     FUA_NV    -
           2        [ (MSB)
           3                        Logical Block Address
           4
           5                                                          (LSB) ]
           6        [         -         ][          Group Number            ]
           7        [ (MSB)         Transfer Length
           8                                                          (LSB) ]
           9        [                    Control                            ]
    </code>
  ***************************************************************************/

BYTE USBHostMSDSCSISectorRead( DWORD sectorAddress, BYTE *dataBuffer )
{
    DWORD   byteCount;
    BYTE    commandBlock[10];
    BYTE    errorCode;

    #ifdef DEBUG_MODE
        UART2PrintString( "SCSI: Reading sector " );
        UART2PutHex(sectorAddress >> 24);
        UART2PutHex(sectorAddress >> 16);
        UART2PutHex(sectorAddress >> 8);
        UART2PutHex(sectorAddress);
        UART2PrintString( " Device " );
        UART2PutHex(deviceAddress);
        UART2PrintString( "\r\n" );
    #endif

    if (deviceAddress == 0)
    {
        return FALSE;       // USB_MSD_DEVICE_NOT_FOUND;
    }

    // Fill in the command block with the READ10 parameters.
    commandBlock[0] = 0x28;     // Operation code
    commandBlock[1] = RDPROTECT_NORMAL | FUA_ALLOW_CACHE;
    commandBlock[2] = (BYTE) (sectorAddress >> 24);     // Big endian!
    commandBlock[3] = (BYTE) (sectorAddress >> 16);
    commandBlock[4] = (BYTE) (sectorAddress >> 8);
    commandBlock[5] = (BYTE) (sectorAddress);
    commandBlock[6] = 0x00;     // Group Number
    commandBlock[7] = 0x00;     // Number of blocks - Big endian!
    commandBlock[8] = 0x01;
    commandBlock[9] = 0x00;     // Control

    // Currently using LUN=0.  When the File System supports multiple LUN's, this will change.
    errorCode = USBHostMSDRead( deviceAddress, 0, commandBlock, 10, dataBuffer, mediaInformation.sectorSize );
    #ifdef DEBUG_MODE
        UART2PrintString( "SCSI: Read sector init error " );
        UART2PutHex( errorCode );
        UART2PrintString( "\r\n" );
    #endif

    if (!errorCode)
    {
        while (!USBHostMSDTransferIsComplete( deviceAddress, &errorCode, &byteCount ))
        {
            USBTasks();
        }
    }

    #ifdef DEBUG_MODE
        UART2PrintString( "SCSI: Read sector error " );
        UART2PutHex( errorCode );
        UART2PrintString( "\r\n" );
    #endif

    if (!errorCode)
    {
        return TRUE;
    }
    else
    {
//        USBHostMSDSCSIMediaReset();
        return FALSE;
    }
}

/****************************************************************************
  Function:
    BYTE USBHostMSDSCSISectorWrite( DWORD sectorAddress, BYTE *dataBuffer, BYTE allowWriteToZero )

  Summary:
    This function writes one sector.

  Description:
    This function uses the SCSI command WRITE10 to write one sector.  The size
    of the sector was determined in the USBHostMSDSCSIMediaInitialize()
    function.  The data is read from the application buffer.

  Precondition:
    None

  Parameters:
    DWORD   sectorAddress   - address of sector to write
    BYTE    *dataBuffer     - buffer with application data
    BYTE    allowWriteToZero- If a write to sector 0 is allowed.

  Return Values:
    TRUE    - write performed successfully
    FALSE   - write was not successful

  Remarks:
    To follow convention, this function blocks until the write is complete.

    The WRITE10 command block is as follows:

    <code>
        Byte/Bit    7       6       5       4       3       2       1       0
           0                    Operation Code (0x2A)
           1        [    WRPROTECT      ]  DPO     FUA      -     FUA_NV    -
           2        [ (MSB)
           3                        Logical Block Address
           4
           5                                                          (LSB) ]
           6        [         -         ][          Group Number            ]
           7        [ (MSB)         Transfer Length
           8                                                          (LSB) ]
           9        [                    Control                            ]
    </code>
  ***************************************************************************/

BYTE USBHostMSDSCSISectorWrite( DWORD sectorAddress, BYTE *dataBuffer, BYTE allowWriteToZero )
{
    DWORD   byteCount;
    BYTE    commandBlock[10];
    BYTE    errorCode;

    #ifdef DEBUG_MODE
        UART2PrintString( "SCSI: Writing sector " );
        UART2PutHex(sectorAddress >> 24);
        UART2PutHex(sectorAddress >> 16);
        UART2PutHex(sectorAddress >> 8);
        UART2PutHex(sectorAddress);
        UART2PrintString( " Device " );
        UART2PutHex(deviceAddress);
        UART2PrintString( "\r\n" );
    #endif

    if (deviceAddress == 0)
    {
        return FALSE;   //USB_MSD_DEVICE_NOT_FOUND;
    }

    if ((sectorAddress == 0) && (allowWriteToZero == FALSE))
    {
        return FALSE;
    }

    // Fill in the command block with the WRITE 10 parameters.
    commandBlock[0] = 0x2A;     // Operation code
    commandBlock[1] = WRPROTECT_NORMAL | FUA_ALLOW_CACHE;
    commandBlock[2] = (BYTE) (sectorAddress >> 24);     // Big endian!
    commandBlock[3] = (BYTE) (sectorAddress >> 16);
    commandBlock[4] = (BYTE) (sectorAddress >> 8);
    commandBlock[5] = (BYTE) (sectorAddress);
    commandBlock[6] = 0x00;     // Group Number
    commandBlock[7] = 0x00;     // Number of blocks - Big endian!
    commandBlock[8] = 0x01;
    commandBlock[9] = 0x00;     // Control

    // Currently using LUN=0.  When the File System supports multiple LUN's, this will change.
    errorCode = USBHostMSDWrite( deviceAddress, 0, commandBlock, 10, dataBuffer, mediaInformation.sectorSize );
    #ifdef DEBUG_MODE
        UART2PrintString( "SCSI: Write sector init error " );
        UART2PutHex( errorCode );
        UART2PrintString( "\r\n" );
    #endif

    if (!errorCode)
    {
        while (!USBHostMSDTransferIsComplete( deviceAddress, &errorCode, &byteCount ))
        {
            USBTasks();
        }
    }

    #ifdef DEBUG_MODE
        UART2PrintString( "SCSI: Write sector error " );
        UART2PutHex( errorCode );
        UART2PrintString( "\r\n" );
    #endif

    if (!errorCode)
    {
        return TRUE;
    }
    else
    {
//        USBHostMSDSCSIMediaReset();
        return FALSE;
    }
}


/****************************************************************************
  Function:
    BYTE USBHostMSDSCSIWriteProtectState( void )

  Description:
    This function returns the write protect status of the device.

  Precondition:
    None

  Parameters:
    None - None

  Return Values:
    0 - not write protected


  Remarks:
    None
  ***************************************************************************/

BYTE    USBHostMSDSCSIWriteProtectState( void )
{
    return 0;
}


// *****************************************************************************
// *****************************************************************************
// Section: Internal Functions
// *****************************************************************************
// *****************************************************************************


/*******************************************************************************
  Function:
    BOOL _USBHostMSDSCSI_TestUnitReady( void )

  Precondition:
    None

  Overview:
    This function sends the TEST UNIT READY SCSI command

  Parameters:
    None - None

  Return Values:
    TRUE    - Command completed without error
    FALSE   - Error while performing command

  Remarks:
    The format of the TEST UNIT READY command is as follows:

    <code>
        Byte/Bit    7       6       5       4       3       2       1       0
           0                    Operation Code (0x00)
           1        [
           2                               Reserved
           3
           4                                                                ]
           5        [                    Control                            ]
    </code>
  ***************************************************************************/

#ifdef PERFORM_TEST_UNIT_READY
BOOL _USBHostMSDSCSI_TestUnitReady( void )
{
    DWORD       byteCount;
    BYTE        commandBlock[10];
    BYTE        errorCode;
    BYTE        inquiryData[36];
    BYTE        unitReadyCount;

    // Issue a TEST UNIT READY
    #ifdef DEBUG_MODE
        UART2PrintString( "SCSI: TEST UNIT READY..." );
    #endif

    unitReadyCount = 0;
    while (unitReadyCount < 5)
    {
        unitReadyCount ++;

        // Fill in the command block with the TEST UNIT READY parameters.
        commandBlock[0] = 0x00;     // Operation Code
        commandBlock[1] = 0;        // Reserved
        commandBlock[2] = 0;        // Reserved
        commandBlock[3] = 0;        // Reserved
        commandBlock[4] = 0;        // Reserved
        commandBlock[5] = 0x00;     // Control

        errorCode = USBHostMSDRead( deviceAddress, 0, commandBlock, 6, inquiryData, 0 );
        #ifdef DEBUG_MODE
            UART2PutHex( errorCode ) ;
            UART2PutChar( ' ' );
        #endif

        if (!errorCode)
        {
            while (!USBHostMSDTransferIsComplete( deviceAddress, &errorCode, &byteCount ))
            {
                USBTasks();
            }
        }
        #ifdef DEBUG_MODE
            UART2PutHex( errorCode ) ;
            UART2PrintString( "\r\n" );
        #endif
        if (!errorCode)
        {
            return TRUE;
        }
    }

    return FALSE;
}
#endif

