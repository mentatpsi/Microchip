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
*/

#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "boot.h"
#include "MDD File System/FSIO.h"
#include "Compiler.h"


//******************************************************************************
//******************************************************************************
// Constants
//******************************************************************************
//******************************************************************************

// Record status codes
#define RECORD_DATA                     0   // Data record found
#define RECORD_FOUND          RECORD_DATA   // Alias for RECORD_DATA
#define RECORD_NON_DATA                 1   // Identified non-data record
#define RECORD_EOF                      2   // Last record in the file
#define RECORD_NOT_FOUND               10   // No record found
#define RECORD_UNSUPPORTED             11   // Unable to identify record type
#define RECORD_CHECKSUM_ERR            12   // Record was corrupted

// Record buffer array size
#define MAX_RECORD_LENGTH             16   // Max Hex-Record Length (converted)


//******************************************************************************
//******************************************************************************
// Global Data
//******************************************************************************
//******************************************************************************


// This structure holds the translated version of the hex record
typedef struct
{
    unsigned char       RecordLength;   // Length record data payload (adjusted 
                                        //   if Offset is non-zero).
    unsigned int        LoadOffset;     // 16-bit offset to which the data will 
                                        //   be loaded
    unsigned char       RecordType;     // Type of data in the record
    unsigned char       ValidIndex;     // Indicates first valid byte in a record

    unsigned char       data[MAX_RECORD_LENGTH];      // Record data buffer

} RECORD_STRUCT; // hexadecimal format data for transfer to aggregator

// Buffer used to build a translated record.
RECORD_STRUCT   recordBuffer;

// Base address of current section being written to Flash.
unsigned long int baseAddress = 0;

// Flash block erasure tracking
//
// Each bit represents one Flash block.  Assumes an unsigned long int has 32 bits.
unsigned long int flashPageStatus[1];


//******************************************************************************
//******************************************************************************
// Local Routines
//******************************************************************************
//******************************************************************************

/****************************************************************************
  Function:
    void TrackPageEraseInit(void)

  Description:
    Initializes array used to track pages that have been erased.
    
  Precondition:
    None

  Parameters:
    None
    
  Returns:
    None
        
  Remarks:
    None
***************************************************************************/
void TrackPageEraseInit(void)
{
    // Clear all bits in the tracking array.
    memset(flashPageStatus, 0, sizeof(flashPageStatus));

} //TrackPageEraseInit


/****************************************************************************
  Function:
    unsigned char TrackPageEraseTest(unsigned long int currentAddress)

  Description:
    Checks to see if a given page has been erased (or if it needs to be 
    erased). 
    
  Precondition:
    None

  Parameters:
    PageAddress         - Address of page to test for prior erasure

  Returns:
    TRUE                - Page has aready been erased
    FALSE               - Page has not yet been erased
    
  Remarks:
    None
***************************************************************************/

BOOL TrackPageEraseTest(unsigned long int PageAddress)
{
    unsigned long int PageNumber;

    // Calculate page address
    PageNumber = (PageAddress - PROGRAM_FLASH_BASE) / FLASH_BLOCK_SIZE;
    
    // Check the bit that tracks that page
    //
    // Note:  Each bit tracks 1 page so each word tracks 32 pages
    //   Word position = PageNumber / 32
    //   Bit  position = PageNumber % 32
    if ( flashPageStatus[PageNumber/32] & (1 << (PageNumber%32)) )
    {
        // page has been erased
        return TRUE;
    }
    else
    {
        // page has not been erased yet
        return FALSE;
    }

} //TrackPageEraseTest
    

/****************************************************************************
  Function:
    void TrackPageErase(unsigned long int PageAddress, BOOL PageErased)
    
  Description:
    This routine sets (or clears) a bit in a tracking array to track if a
    Flash page has been erased or if it needs to be erased.

  Precondition:
    None, but the tracking array will not contain correct data about the
    other Flash pages if it has not been properly initialized.

  Parameters:
    PageAddress     - Address of the page being erased
    PageErased      - Flag indicating if the page has been erased or if it
                      needs to be erased.

  Returns:
    None
    
  Remarks:
    None
    
***************************************************************************/

// Parameter flags definitions
#define PAGE_ERASED         TRUE
#define PAGE_NOT_ERASED     FALSE

void TrackPageErase(unsigned long int PageAddress, BOOL PageErased)
{
    unsigned long int PageNumber;

    // Identify which page was just erased.
    PageNumber = (PageAddress - PROGRAM_FLASH_BASE) / FLASH_BLOCK_SIZE;
    
    // Update the bit that tracks that page
    //
    // Note:  Each bit tracks 1 page so each word tracks 32 pages
    //   Word position = PageNumber / 32
    //   Bit  position = PageNumber % 32
    if (PageErased)
    {
        // Set the bit to indicate the page has been erased
        flashPageStatus[PageNumber/32] |=  ( 1 << (PageNumber % 32) );
    }
    else
    {
        // Clear the bit to indicate the page needs to be erased
        flashPageStatus[PageNumber/32] &= ~( 1 << (PageNumber % 32) );
    }

} //TrackPageErase


/****************************************************************************
  Function:
    unsigned char AsciiToHexNibble(unsigned char data)
    
  Description:
    Converts an ASCII byte in the range of "0 - 9", "A - F", or "a - f" to a 
    hex nibble
    
  Precondition:
    None

  Parameters:
    data            - char containing the data to convert
    
  Returns:
    hex value represented by the input ASCII character (0x00 - 0x0F)
    0 for input values out of range.

  Remarks:
     Example:         value = AsciiToHexNibble('A'); // value = 0x0A
***************************************************************************/

unsigned char AsciiToHexNibble(unsigned char data)
{
    if (data < '0')                     // return 0 for an invalid characters
    {
        return 0;
    }
    else if (data <= '9')               // handle numbers
    {
        return ( data - '0' );
    }
    else if (data < 'A')
    {
        return 0;
    }
    else if (data <= 'F')               // handle uppercase letters
    {
        return ( data - 'A' + 10 );
    }
    else if (data < 'a')
    {
        return 0;
    }
    else if (data <= 'f')               // handle lowercase letters
    {
        return ( data - 'a' + 10 );
    }
    else
    {
        return 0;
    }

} // AsciiToHexNibble


/****************************************************************************
  Function:
    unsigned char AsciiToHexByte ( unsigned char m, unsigned char, l )

  Description:
    Converts two ASCII characters representing hex nibbles into a single
    hex (or binary) byte.
    
  Precondition:
    None

  Parameters:
    m           - Most significant nibble (in ASCII-hex representation)
    l           - Least significant nibble (in ASCII-hex representation)

  Returns:
    1-byte hex (binary) value represented by the two ASCII nibbles

  Remarks:
    Implemented as a macro
***************************************************************************/

#define AsciiToHexByte(m,l) ( (AsciiToHexNibble(m) << 4 ) | AsciiToHexNibble(l) )


/****************************************************************************
  Function:
    unsigned int FindHexRecord ( char *pBufferData, unsigned int nSize )

  Description:
    Scans the specified buffer from start to end seeking the start of 
    record charactor (':') and provides the position of the first byte of 
    the record (past the start character).
    
  Precondition:
    None

  Parameters:
    pBufferData     - Pointer to buffer
    nSize           - Size of the buffer in bytes

  Returns:
    Postion starting position record (next byte after the start of record
    character ':').  If the return value is greater or equal to the size of
    the given buffer, no record was found.

  Remarks:
    None
***************************************************************************/

unsigned int FindHexRecord ( char *pBufferData, unsigned int nSize )
{
    unsigned int position;  // Position of record in buffer
    
    // Scan the buffer for the start-of-record character
    for (position = 0; position < nSize; position++)
    {
        if ( pBufferData[position] == ':' )
        {
            // Skip the start-of-record character
            position++;
            break;
        }
    }
    return (position);

} // FindRecord


/****************************************************************************
  Function:
    void RecordInitialize( RECORD_STRUCT *pRecord )

  Description:
    Initializes a Record structure
    
  Precondition:
    None

  Parameters:
    pRecord     - Pointer to the record data structure

  Returns:
    None

  Remarks:
    Implemented as a macro
***************************************************************************/

#define RecordInitialize(r)    ((r)->RecordLength = 0,(r)->ValidIndex = 0)


/****************************************************************************
  Function:
    unsigned char ProcessRecord( RECORD_STRUCT *pRecord )

  Description:
    Executes the command for each hexfile record type.
    
  Precondition:
    pRecord must contain valid record data

  Parameters:
    pRecord               - Pointer to the record

  Returns:
    RECORD_DATA           - Data record found
    RECORD_NON_DATA       - Identified non-data record
    RECORD_EOF            - Last record in the file
    RECORD_NOT_FOUND      - No record found
    RECORD_UNSUPPORTED    - Unable to identify record type
    RECORD_CHECKSUM_ERR   - Record was corrupted

  Remarks:
    None
***************************************************************************/

unsigned char ProcessRecord( RECORD_STRUCT *pRecord )
{
    // executes command from the hex file line
    unsigned char   returnVal = 0;
        
    switch (pRecord->RecordType)
    {
    
    case 0x00: // data entry
        returnVal = RECORD_DATA;
        break;
    
    case 0x01: // end of file
        // this was the last record in the hexfile
        RecordInitialize(pRecord);
        returnVal = RECORD_EOF; 
        break;
    
    case 0x02: // Extended segment address record
        RecordInitialize(pRecord);
        returnVal = RECORD_UNSUPPORTED;
        break;
    
    case 0x03: // Start Segment Address record
        RecordInitialize(pRecord);
        returnVal = RECORD_UNSUPPORTED;
        break;
    
    case 0x04: // Extended Linear Address record
        // set upper 16bits to value from record, lower 16bits are forced to 0 ** update to point to ULBA
        baseAddress = (pRecord->data[0] << 8) | pRecord->data[1];
        baseAddress = baseAddress << 16;
        RecordInitialize(pRecord);
        returnVal   = RECORD_NON_DATA;
        break;
    
    case 0x05: // Start Linear Address record
        RecordInitialize(pRecord);
        returnVal = RECORD_UNSUPPORTED;
        break;

    #ifdef EXTENDED_HEXFILE_SUPPORT // non-Intel commands
                
    // If this mode is not enabled extened commands will generate 
    // unsupported hexfile command errors(messages), they will be ignored

    case 0x10: // serial number
        // Only allow programming if the stored SN matches the one stored in the file
        returnVal = Loader_ValidateSerialNumber(pRecord->data, pRecord->RecordLength);
        RecordInitialize(pRecord);
        break;

    case 0x11: // revision number
        // Intended to  only allow programming if current revision is 
        // lower than the one contained in the file
        returnVal = Loader_ValidateRevisionNumber(pRecord->data, pRecord->RecordLength);
        RecordInitialize(pRecord);
        break;

    case 0x12: // Check Error Detection Accumulator (csum, CRC, MD-5, etc)
        returnVal = Loader_CheckErrorDetection(pRecord->data, pRecord->RecordLength);
        RecordInitialize(pRecord);
        break;
            
    #endif
    
    default:
        RecordInitialize(pRecord);
        returnVal = RECORD_UNSUPPORTED;
        break;

    } // switch
    
    return (returnVal);

} //DecodeHexFileCommand()


/****************************************************************************
  Function:
    unsigned int GetRecord( RECORD_STRUCT *pOutputBuffer,  
                            unsigned int  *nInput, 
                            BYTE          *pInputBuffer ) 

  Description:
    Takes data from raw hex buffer and translates it into a record structure
    
  Precondition:
    The pInputBuffer must point to nInput bytes of hex-record format data.

  Parameters:
    pOutputBuffer       - Pointer to a record structure
    nInput              - Number of bytes of raw data available (updated)
    pInputBuffer        - Pointer to input data buffer

  Returns:
    RECORD_FOUND        - Record found, translated, placed in output buffer,
                          and "removed" from input buffer
    RECORD_NOT_FOUND    - No record found, no data removed from input buffer
    RECORD_CHECKSUM_ERR - Record was corrupted
    
  Note:  
    If RECORD_FOUND is returned, *nInput will be reduced by the number of 
    ASCII bytes that made up the record (plus any preceeding bytes).
  
  Record format:
    +--------------------------------------------------------------+
    | ':' | RecLen | Load Offset | RecType | Data Payload | ChkSum |
    +--------------------------------------------------------------+
      -1  | 0   1  | 2  3  4  5  | 6   7   | 0 ...  n*2  | +0  +1  |
      
***************************************************************************/

unsigned int GetRecord( RECORD_STRUCT *pOutputBuffer,  unsigned int *nInput, BYTE *pInputBuffer ) 
{
    unsigned char   Sum = 0;                // Accumulated sum of translated bytes
    unsigned char   Temp;                   // Used for MSB of multi-byte values
    unsigned char   iData;                  // Data payload index variable
    unsigned int    nUsed;                  // Number of input buffer data bytes used
    unsigned char   RecordLength;           // Length record data payload
    unsigned int    LoadOffset;             // 16-bit offset to which the data will be loaded
    unsigned char   RecordType;             // Type of data in the record
    unsigned char   Checksum;               // Checksum given in the record

    // Check to make sure the record buffer is empty
    if ( pOutputBuffer->RecordLength - pOutputBuffer->ValidIndex > 0 )
    {
        return RECORD_FOUND;
    }

    // Make sure we have input data
    if (*nInput > 0)
    {
        // Scan for a start of record
        nUsed         = FindHexRecord((char*)pInputBuffer, *nInput);
        pInputBuffer += nUsed;

        // Check to see of we have enough data for a record (10 bytes overhead)
        if ( nUsed + 8  < *nInput )
        {
            //
            // Translate record and accumulate checksum:
            //

            // Extract the record length
            RecordLength  = AsciiToHexByte(pInputBuffer[0], pInputBuffer[1]);
            Sum          += RecordLength;

            // Extract the load offset
            Temp          = AsciiToHexByte(pInputBuffer[2], pInputBuffer[3]);
            Sum          += Temp;
            LoadOffset    = AsciiToHexByte(pInputBuffer[4], pInputBuffer[5]);
            Sum          += LoadOffset;
            LoadOffset   += Temp << 8;

            // Extract the record type
            RecordType    = AsciiToHexByte(pInputBuffer[6], pInputBuffer[7]);
            Sum          += RecordType;

            pInputBuffer += 8;


            // Make sure the input buffer contains enough data for the payload and checksum
            nUsed += (2 * RecordLength) + 10;
            if ( nUsed > *nInput )
            {
                // Insufficient data to form a record
                return RECORD_NOT_FOUND;
            }

            // Extract the data payload
            for (iData=0; iData/2 < RecordLength; iData+=2)
            {
                pOutputBuffer->data[iData/2]  = AsciiToHexByte(pInputBuffer[iData+0], pInputBuffer[iData+1]);
                Sum                          += pOutputBuffer->data[iData/2];
            }

            pInputBuffer += iData;


            // Extract and verify check sum
            Checksum  = AsciiToHexByte(pInputBuffer[0], pInputBuffer[1]);
            Sum      += Checksum;
            if (Sum != 0 )
            {
                BLIO_ReportBootStatus(LOADER_CHECKSUM_ERR, "BL Loader: Error - Hex record checksum mismatch\r\n");
                return RECORD_CHECKSUM_ERR;
            }

            // Checksum valid, "remove" data from input buffer
            *nInput -= nUsed;

            // Fill in the record parameters
            pOutputBuffer->RecordLength = RecordLength;
            pOutputBuffer->LoadOffset   = LoadOffset;
            pOutputBuffer->RecordType   = RecordType;
            pOutputBuffer->ValidIndex   = 0;

            return RECORD_FOUND;
        }
    }
    
    // Insufficient data to form a record
    return RECORD_NOT_FOUND;

} // GetRecord


/*
*******************************************************************************
EOF
*******************************************************************************
*/

