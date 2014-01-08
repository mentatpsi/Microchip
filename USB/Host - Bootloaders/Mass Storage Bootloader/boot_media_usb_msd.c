/*
********************************************************************************
                                                                                
Software License Agreement                                                      
                                                                                
Copyright © 2008 Microchip Technology Inc. and its licensors.  All         
rights reserved.                                                                
                                                                                
Microchip licenses to you the right to: (1) install Software on a single        
computer and use the Software with Microchip 16-bit microcontrollers and        
16-bit digital signal controllers ("Microchip Product"); and (2) at your        
own discretion and risk, use, modify, copy and distribute the device            
driver files of the Software that are provided to you in Source Code;           
provided that such Device Drivers are only used with Microchip Products         
and that no open source or free software is incorporated into the Device        
Drivers without Microchip's prior written consent in each instance.             
                                                                                
You should refer to the license agreement accompanying this Software for        
additional information regarding your rights and obligations.                   
                                                                                
SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY         
KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY              
WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A          
PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE             
LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY,               
CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY           
DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY         
INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR         
LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY,                 
SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY         
DEFENSE THEREOF), OR OTHER SIMILAR COSTS.                                       
                                                                                
********************************************************************************

 Change History:
  Rev   Description
  ----  -----------------------------------------
  2.7   Major rework of the BLMedia_LoadFile() to remove processor specific
        implementation details.

        Formatting changes to make the processing flow more readable.

********************************************************************************
*/


#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "boot.h"
#include "MDD File System/FSIO.h"
#include "USB/usb.h"
#include "USB/usb_host_msd.h"
#include "USB/usb_host_msd_scsi.h"
//*****************************************************************************
//*****************************************************************************
// Boot Loader TypeDefs and defintions
//*****************************************************************************
//*****************************************************************************

typedef enum
{
    RECORD_START_TOKEN = 0,
    RECORD_BYTE_COUNT_NIBBLE_1,
    RECORD_BYTE_COUNT_NIBBLE_0,
    RECORD_ADDRESS_NIBBLE_3,
    RECORD_ADDRESS_NIBBLE_2,
    RECORD_ADDRESS_NIBBLE_1,
    RECORD_ADDRESS_NIBBLE_0,
    RECORD_TYPE_NIBBLE_1,
    RECORD_TYPE_NIBBLE_0,
    RECORD_DATA,
    RECORD_CHECKSUM_NIBBLE_1,
    RECORD_CHECKSUM_NIBBLE_0 
} RECORD_STATE;

//*****************************************************************************
//*****************************************************************************
// Boot Loader Media Interface Definitions
//*****************************************************************************
//*****************************************************************************

// Check for definitions of required build parameters

// Maximum number of retries to mount the boot medium
#ifndef MAX_NUM_MOUNT_RETRIES
    #define MAX_NUM_MOUNT_RETRIES       10
#endif

// Defines the size of the buffer used to read the boot image file.
#ifndef BL_READ_BUFFER_SIZE
    #define BL_READ_BUFFER_SIZE        512
#endif


//******************************************************************************
//******************************************************************************
// Global Data
//******************************************************************************
//******************************************************************************

// Flag indicating media attached
BOOL            MediaPresent;

// Flag indicating file system volume valid
BOOL            VolumeValid;

// Buffer for reading image file data
BYTE            ReadBuffer[BL_READ_BUFFER_SIZE];

// Stores data and base address of data for use by the programming routine
FLASH_BLOCK     FlashBlock;

// Stores the information about the current record
RECORD_STRUCT   current_record;

//******************************************************************************
//******************************************************************************
// USB Support Functions
//******************************************************************************
//******************************************************************************

/****************************************************************************
  Function:
    BOOL USB_ThumbDriveEventHandler( BYTE address, USB_EVENT event,
                void *data, DWORD size )

  Summary:
    This is the boot loader event handler.  It is called when the USB stack 
    has an event that needs to be handled by the boot loader rather than by
    the USB client driver.

  Precondition:
    The USB stack must have been initialized.

  Parameters:
    address - Address of device where event occurred
    event   - Identifies the event that occured
    data    - Pointer to event-specific data
    size    - Size of the event-specific data

  Return Values:
    TRUE    - The event was handled
    FALSE   - The event was not handled

  Remarks:
    None
  ***************************************************************************/

BOOL USB_ThumbDriveEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    switch( event )
    {        
    case EVENT_VBUS_REQUEST_POWER:
        // The data pointer points to a byte that represents the amount of power
        // requested in mA, divided by two.  If the device wants too much power,
        // we reject it.
        if (((USB_VBUS_POWER_EVENT_DATA*)data)->current <= (MAX_ALLOWED_CURRENT / 2))
        {
            return TRUE;
        }
        else
        {
            MediaPresent = FALSE;
            BLIO_ReportBootStatus( BL_USB_OC_ERR, "BL: USB Error - device requires too much current\r\n" );
        }
        break;

    case EVENT_VBUS_RELEASE_POWER:
        // The Explorer 16 cannot turn off Vbus through software.
        BLIO_ReportBootStatus( BL_MEDIUM_DETACH, "BL: USB - Boot medium detached\r\n" );
        return TRUE;

    case EVENT_HUB_ATTACH:
        BLIO_ReportBootStatus( BL_USB_HUB_ERR, "BL: USB Error - hubs are not supported\r\n" );
        return TRUE;

    case EVENT_UNSUPPORTED_DEVICE:
        BLIO_ReportBootStatus( BL_BAD_DEV_ERR, "BL: USB Error - device is not supported\r\n" );
        return TRUE;

    case EVENT_CANNOT_ENUMERATE:
        BLIO_ReportBootStatus( USB_ENUM_ERR,  "BL: USB Error - cannot enumerate device\r\n" );
        return TRUE;

    case EVENT_CLIENT_INIT_ERROR:
        BLIO_ReportBootStatus( USB_MSD_INIT_ERR, "BL: USB Error - msd client driver initialization error\r\n" );
        return TRUE;

    case EVENT_OUT_OF_MEMORY:
        BLIO_ReportBootStatus( OUT_OF_MEMORY_ERR, "BL: Error - out of heap memory\r\n" );
        return TRUE;

    case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
        BLIO_ReportBootStatus( USB_ERR, "BL: USB Error - unspecified\r\n" );
        return TRUE;
    default:
        break;
    }

    return FALSE;

} // USB_ThumbDriveEventHandler


//******************************************************************************
//******************************************************************************
// Boot Loader Media Interface Functions
//******************************************************************************
//******************************************************************************

/****************************************************************************
  Function:
    BOOL BLMedia_InitializeTransport ( void )

  Description:
    This routine is called to initalize the media transport layer used to 
    access the boot medium.

  Precondition:
    None

  Parameters:
    None

  Returns:
    TRUE    - If able to initialize the boot medium's transport layer
    FALSE   - If not able to initialize the boot medium's transport layer

  Remarks:
    This routine can be implemented to inialize any desired boot medium.
***************************************************************************/

// Implemented as a macro to call USBInitialize()


/****************************************************************************
  Function:
    void BLMedia_DeinitializeTransport ( void )

  Description:
    This routine disables the media transport layer that was initialized by
    BLMedia_InitializeTransport.


  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    This routine must disable and clear any interrupts that were enabled
    by BLMedia_InitializeTransport.  Any stray pending interrupts (once the
    application boots) may cause the system to trigger an un-handled (or 
    incorrectly handled) interrupt.
***************************************************************************/

// Implemented as a macro to call USBShutdown()


/****************************************************************************
  Function:
    BOOL BLMedia_MonitorMedia( void )

  Description:
    This function calls the background tasks necessary to support USB Host
    operation.  Upon initial insertion of the media, it initializes the file
    system support.

  Precondition:
    The USB transport must have been initialized.

  Parameters:
    None

  Returns:
    TRUE    - As long as the media interface is active
    FALSE   - If for any reason the media interface needs to be 
              re-initialized.

  Remarks:
    Calls USBTasks() to maintain the USB MSD and FAT FS stack.
  ***************************************************************************/

BOOL BLMedia_MonitorMedia ( void )
{
    BYTE            mediaPresentNow;
    BYTE            mountTries;

    USBTasks();

    mediaPresentNow = USBHostMSDSCSIMediaDetect();
    if (mediaPresentNow != MediaPresent)
    {
        if (mediaPresentNow)
        {
            mountTries = MAX_NUM_MOUNT_RETRIES;
            while(!FSInit() && mountTries--);
            if (!mountTries)
            {
                BLIO_ReportBootStatus(BL_FS_INIT_ERR, "BL: USB Error - Could not initialize file system\r\n" );
                MediaPresent = FALSE;
            }
            else
            {
                MediaPresent = TRUE;
            }
        }
        else
        {
            MediaPresent    = FALSE;
            VolumeValid     = FALSE;
        }
    }

    return TRUE;

} // MonitorMedia


/****************************************************************************
  Function:
    BOOL BLMedia_MediumAttached ( void )

  Description:
    This routine determines if the boot medium is currently attached to the
    system.

  Precondition:
    BLMedia_InitializeTransport must have been called and the medium muust
    be maintained as needed.

  Parameters:
    None

  Returns:
    TRUE    - If the boot medium is currently attached and accessible
    FALSE   - If the boot medium is not currently attached

  Remarks:
    None
***************************************************************************/

// Implemented as a macro to call USBHostMSDSCSIMediaDetect()


/****************************************************************************
  Function:
    BOOL BLMedia_LocateFile ( char *file_name )

  Description:
    Searches the thumb drive's root directory and identifies if 
    the given file is present.

  Precondition:
    The USB transport must have been initialized.

  Parameters:
    file_name - Pointer to a null-terminated character string giving the 
                name of the application's image file.

  Returns:
    TRUE      - If the application's image file named by the file_name string
                is able to be found on the boot medium.
    FALSE     - If the file cannot be found.

  Remarks:
    None
  ***************************************************************************/

BOOL BLMedia_LocateFile ( char *file_name )
{
    //BOOL        fileFound;
    SearchRec   searchRecord;

    if (FindFirst( file_name, 
                   ATTR_DIRECTORY | ATTR_ARCHIVE | ATTR_READ_ONLY | ATTR_HIDDEN, 
                   &searchRecord ) == 0 )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

} // BLMedia_LocateFile


/****************************************************************************
  Function:
    BOOL BLMedia_LoadFile (  char *file_name )

  Description:
    Accesses the boot image file, reads it over the transport, and programs
    it to Flash.

  Precondition:
    The boot medium and file system must have been initialized and
    maintained as needed.

  Parameters:
    file_name - Pointer to a null-terminated character string giving the 
                name of the application's image file.

  Returns:
    TRUE      - If a valid application image was read from the thumb drive
                and programmed into Flash at the APPLICATION_ADDRESS.
    FALSE     - If the application image was not valid or if unable to 
                program a valid application to Flash for any reason.

  Remarks:
    This routine calls the loader layer to translate and program the boot
    image file.
    
    This routine can be modified to account for differences in how the medium
    and file format must be processed.
  ***************************************************************************/
#define AsciiToHexByte(m,l) ( (AsciiToHexNibble(m) << 4 ) | AsciiToHexNibble(l) )
unsigned char AsciiToHexNibble(unsigned char data);

typedef enum
{
    RECORD_TYPE_DATA_RECORD = 0x00,
    RECORD_TYPE_EOF = 0x01,
    RECORD_TYPE_EXTENDED_ADDRESS = 0x04
} RECORD_TYPE;

BOOL BLMedia_LoadFile (  char *file_name )
{
    FSFILE         *fp;             // File pointer
    RECORD_STATE    record_state;   // This field specifies which part of the
                                    //   record is currently being read.
    unsigned int    nRemaining;     // Number of bytes remaining to decode

    BYTE            *p_file_data;

    WORD_VAL        byteCountASCII;
    DWORD_VAL       addressASCII;
    WORD_VAL        recordTypeASCII;
    WORD_VAL        checksumASCII;
    WORD_VAL        dataByteASCII;

    DWORD_VAL       totalAddress;
    WORD_VAL        extendedAddress;

    BYTE            calculated_checksum;

    BYTE            recordDataCounter;
    BYTE            byteEvenVsOdd;

    //Generic loop index.  Needs to be a WORD since a record could be 255 bytes
    //  and in this case written 4 bytes at a time.  Thus the counts for that loop
    //  would be 252 and 256.  Since 256 can't be represented in a byte, a loop
    //  counter of a byte could only count up to 252 bytes.
    WORD            i;

    WORD            numSectorsRead;
    DWORD           numRecordsProcessed;

    // Attempt to open the file
    if ( (fp=FSfopen( file_name, "r" )) == NULL )
    {
        BLIO_ReportBootStatus(BL_FILE_ERR, "BL: USB Error - Unable to open file\r\n" );
        return FALSE;
    }

    //Now that we have successfully openned the file that we want to program,
    //  let's erase all of the existing application data.
    EraseAllApplicationMemory();

    // Read the file and program it to Flash
    nRemaining  = 0;
    record_state = RECORD_START_TOKEN;
    numSectorsRead = 0;
    numRecordsProcessed = 0;

    //Continue processing the file until we reach the end of the file.
    while(FSfeof(fp) != EOF)
    {
        //Read in a BL_READ_BUFFER_SIZE block of data into the ReadBuffer.  This
        //  data will be processed in the following loop.  Record the number of
        //  bytes actually read into the nRemaining variable.  This variable will
        //  track the number of bytes remaining in the ReadBuffer.
        nRemaining = FSfread(&ReadBuffer[0], 1, BL_READ_BUFFER_SIZE, fp );

        if(nRemaining == 0)
        {
            //We weren't able to read any data from the file for some reason
            //  even though the file stream indicated that it wasn't at the end
            //  of the file.
            BLIO_ReportBootStatus(BL_FILE_ERR, "BL: File error - unable to read data from file even though it was not reported as EOF.\r\n" );
            FSfclose( fp );
            return FALSE; 
        }

        //point to the data read from the file
        p_file_data = (BYTE*) &ReadBuffer[0];

        //process all of the data read so far...
        while(nRemaining)
        {
            switch(record_state)
            {
                case RECORD_START_TOKEN: //start code
                    if(*p_file_data == ':')
                    {
                        //move to the first state of the byte count
                        record_state = RECORD_BYTE_COUNT_NIBBLE_1;
                    }
                    else
                    {
                        //If we didn't see a start code ":" where we expected it, then
                        //ignore line feeds and line returns
                        if((*p_file_data != 0x0D) && (*p_file_data != 0x0A))
                        {
                            //If there was anything other than a line feed or line return
                            //  then there was an error with the hex file.  Abort the
                            //  loading operation.
                            BLIO_ReportBootStatus(LOADER_MISSING_START, "BL: Start code expected but not found.\r\n" );
                            FSfclose( fp );
                            return FALSE; 
                        }
                    } 
    
                    //byte read from the buffer.  Advance to the next position.
                    p_file_data++; 
                    break;
    
                case RECORD_BYTE_COUNT_NIBBLE_1: //byte count byte 1
                    byteCountASCII.v[1] = *p_file_data++;

                    //move to the next state of the byte count
                    record_state = RECORD_BYTE_COUNT_NIBBLE_0;
                    break;
    
                case RECORD_BYTE_COUNT_NIBBLE_0: //byte count byte 2
                    byteCountASCII.v[0] = *p_file_data++;
                    current_record.RecordLength = AsciiToHexByte(byteCountASCII.v[1],byteCountASCII.v[0]);
                    byteEvenVsOdd = 0;
                    recordDataCounter = 0;

                    //move to the first state of the address
                    record_state = RECORD_ADDRESS_NIBBLE_3;
                    break;
    
                case RECORD_ADDRESS_NIBBLE_3: //address byte 1
                    addressASCII.v[3] = *p_file_data++;

                    //move to the next state of the address
                    record_state = RECORD_ADDRESS_NIBBLE_2;
                    break;
    
                case RECORD_ADDRESS_NIBBLE_2: //address byte 2
                    addressASCII.v[2] = *p_file_data++;

                    //move to the next state of the address
                    record_state = RECORD_ADDRESS_NIBBLE_1;
                    break;
    
                case RECORD_ADDRESS_NIBBLE_1: //address byte 3
                    addressASCII.v[1] = *p_file_data++;

                    //move to the next state of the address
                    record_state = RECORD_ADDRESS_NIBBLE_0;
                    break;
    
                case RECORD_ADDRESS_NIBBLE_0: //address byte 4
                    addressASCII.v[0] = *p_file_data++;
                    current_record.LoadOffset = ((AsciiToHexByte(addressASCII.v[3],addressASCII.v[2]))<<8) + AsciiToHexByte(addressASCII.v[1],addressASCII.v[0]);

                    //move to the first state of the type
                    record_state = RECORD_TYPE_NIBBLE_1;
                    break;
    
                case RECORD_TYPE_NIBBLE_1: //record type byte 1
                    recordTypeASCII.v[1] = *p_file_data++;

                    //move to the next state of the type
                    record_state = RECORD_TYPE_NIBBLE_0;
                    break;
    
                case RECORD_TYPE_NIBBLE_0: //record type byte 2
                    recordTypeASCII.v[0] = *p_file_data++;
                    current_record.RecordType = AsciiToHexByte(recordTypeASCII.v[1],recordTypeASCII.v[0]);

                    if(current_record.RecordLength == 0)
                    {
                        //There is no data stage for this record.  Proceed to the
                        //  checksum.
                        record_state = RECORD_CHECKSUM_NIBBLE_1;
                    }
                    else
                    {
                        //move to the data state
                        record_state = RECORD_DATA;
                    }
                    break;
    
                case RECORD_DATA: //data
                    if(byteEvenVsOdd == 0)
                    {
                        dataByteASCII.v[1] = *p_file_data++;
                        byteEvenVsOdd = 1;
                    }
                    else
                    {
                        dataByteASCII.v[0] = *p_file_data++;
                        current_record.data[recordDataCounter++] = AsciiToHexByte(dataByteASCII.v[1],dataByteASCII.v[0]);
                        byteEvenVsOdd = 0;
                    }

                    //If we have read all of the data, then move to the next state
                    if(recordDataCounter == current_record.RecordLength)
                    {
                        record_state = RECORD_CHECKSUM_NIBBLE_1;
                    }

                    break;
    
                case RECORD_CHECKSUM_NIBBLE_1: //checksum byte 1
                    checksumASCII.v[1] = *p_file_data++;

                    //move to the next state of the checksum
                    record_state = RECORD_CHECKSUM_NIBBLE_0;
                    break;
    
                case RECORD_CHECKSUM_NIBBLE_0: //checksum byte 2
                    checksumASCII.v[0] = *p_file_data++;
                    current_record.Checksum = AsciiToHexByte(checksumASCII.v[1],checksumASCII.v[0]);
                    numRecordsProcessed++;

                    //Calculate the checksum of the data
                    calculated_checksum =   current_record.RecordLength +
                                            (current_record.LoadOffset&0xFF) +
                                            ((current_record.LoadOffset>>8)&0xFF) +
                                            current_record.RecordType;

                    for(i=0;i<current_record.RecordLength;i++)
                    {
                        calculated_checksum += current_record.data[i];
                    }

                    //take the 2s compliment of the result
                    calculated_checksum ^= 0xFF;
                    calculated_checksum += 1;

                    if(calculated_checksum != current_record.Checksum)
                    {
                        //The calculated checksum of the data didn't match the
                        //  value of the checksum held in the data record.
                        BLIO_ReportBootStatus(LOADER_CHECKSUM_ERR, "BL: Calculated checksum didn't match the hex record checksum.\r\n" );
                        FSfclose( fp );
                        return FALSE;
                    }
    
                    switch(current_record.RecordType)
                    {
                        case RECORD_TYPE_DATA_RECORD:
                            //check the address here
                            totalAddress.word.HW = extendedAddress.Val;
                            totalAddress.word.LW = current_record.LoadOffset;
    
                            #if defined(__C30__) || defined __XC16__
                                //divide by 2 since the C30 hex file addresses are 2x the actual physical address
                                totalAddress.Val >>= 1; 
                            #endif

                            if(totalAddress.Val < PROGRAM_FLASH_BASE)
                            {
                                //invalid address - below base - don't program the requested address
                                break;
                            }
    
                            if(totalAddress.Val >= MAX_FLASH_ADDRESS)
                            {
                                //invalid address - above max - don't program the requested address
                                break;
                            }

                            //Program the current data record into program memory
                            if(ProgramHexRecord(&current_record, extendedAddress.Val) == FALSE)
                            {
                                //There was an error programming the data.  The
                                //  error status is already set by the ProgramHexRecord function
                                //  so we just need to close the file and exit here.
                                FSfclose( fp );
                                return FALSE;
                            }
        
                            break;
    
                        case RECORD_TYPE_EOF:
                            FSfclose( fp );
                            return TRUE;
                            break;
    
                        case RECORD_TYPE_EXTENDED_ADDRESS:
                            extendedAddress.v[1] = current_record.data[0];
                            extendedAddress.v[0] = current_record.data[1];
                            break;
                    }

                    //Move to the start token phase.  This record is complete.
                    //  Start looking for the next record.
                    record_state = RECORD_START_TOKEN;
                    break;

                default:
                    //If for some reason we are not in one of the predetermined
                    //  states, then exit.
                    BLIO_ReportBootStatus(LOADER_ERR, "BL: Loader state machine in an unknown state.\r\n" );
                    FSfclose( fp );
                    return FALSE;
            }

            //We have finished processing this byte.  There is now one less byte
            //  in the ReadBuffer.
            nRemaining--;
        }
    }

    //Unexpectedly reached the end of the file.  This could be caused by a
    //  corruption in the hex file.  It may be unknown if the entire contents of
    //  memory was programmed as expected.
    BLIO_ReportBootStatus(LOADER_EOF_REC_MISSING, "BL: Reached the end of the file stream without finding an EOF record.  This is a hex file format error.\r\n" );
    FSfclose( fp );
    return FALSE;

} // BLMedia_LoadFile


/*
*******************************************************************************
EOF
*******************************************************************************
*/

