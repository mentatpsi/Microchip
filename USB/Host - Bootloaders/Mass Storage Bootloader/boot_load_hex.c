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
  2.7   Initial revision

********************************************************************************
*/

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "boot.h"
#include "MDD File System/FSIO.h"


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


//******************************************************************************
//******************************************************************************
// Global Data
//******************************************************************************
//******************************************************************************

// Base address of current section being written to Flash.
unsigned long int baseAddress = 0;



//******************************************************************************
//******************************************************************************
// Local Routines
//******************************************************************************
//******************************************************************************




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



/*
*******************************************************************************
EOF
*******************************************************************************
*/

