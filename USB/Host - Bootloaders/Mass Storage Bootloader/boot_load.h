/*
*******************************************************************************
                                                                                
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
                                                                                
*******************************************************************************
*/
/* "Loader" Interface
*******************************************************************************
The Loader interface is used by the media layer to decode and program the 
image file to memory.
*******************************************************************************
*/

// Check for definitions of required build parameters
#ifndef PROGRAM_FLASH_BASE
    #error "PROGRAM_FLASH_BASE must be defined in boot_config.h"
#endif

#ifndef PROGRAM_FLASH_LENGTH
    #error "PROGRAM_FLASH_LENGTH must be defined in boot_config.h"
#endif

#ifndef FLASH_BLOCK_SIZE
    #error "FLASH_BLOCK_SIZE must be defined in boot_config.h"
#endif

#ifndef BLOCK_FILL_DEFAULT
    #define BLOCK_FILL_DEFAULT          0xFF // default value of program memory
#endif

// Synonyms and calculated values for Flash parameters
#define MIN_FLASH_ADDRESS           (PROGRAM_FLASH_BASE)
#define MAX_FLASH_ADDRESS           (PROGRAM_FLASH_BASE + PROGRAM_FLASH_LENGTH)
#define NUMBER_OF_FLASH_BLOCKS      (PROGRAM_FLASH_LENGTH / FLASH_BLOCK_SIZE)
#define BLOCK_ALIGNMENT_MASK        (~(FLASH_BLOCK_SIZE-1))

// Data structure used to manage and buffer data for a block (or page) of Flash
typedef struct
{
    // 32bit address for start of data in the record (even if offset is non zero)
	unsigned long int                           address;

    // State variable indicating if block is full, partial, or empty ("new")
    unsigned char                               blockState;

    // Buffer for block data (must be word aligned)
	unsigned char __attribute__ ((aligned(4)))  data[FLASH_BLOCK_SIZE];

} FLASH_BLOCK; 


// blockState values
#define BLOCK_STATE_NEW           0
#define BLOCK_STATE_PARTIAL       1
#define BLOCK_STATE_READY         2


/****************************************************************************
  Function:
    void Loader_Initialize ( FLASH_BLOCK *pFlashBlock )

  Description:
    This routine initializes the loader by initializing the FLASH_BLOCK 
    structure, so that it can begin accumulating data, and initializing the
    Flash page erasure tracking array.

  Precondition:
    None

  Parameters:
    pFlashBlock     - Pointer to a FLASH_BLOCK structure

  Returns:
    None
    
  Remarks:
    Initializes the Flash buffer space to all 1's (0xFFFFFFFF)
***************************************************************************/

void Loader_Initialize ( FLASH_BLOCK *pFlashBlock );


/****************************************************************************
  Function:
    unsigned int Loader_GetFlashBlock ( FLASH_BLOCK  *pFlashBlock, 
                                        unsigned int *pRawSize, 
                                        BYTE         *pRawBuffer  )

  Description:
    This routine gets raw data from the input buffer (RawBuffer), translates 
    it from Hex Record ASCII format into binary data, and copies it to the 
    Flash block buffer (FLASH_BLOCK).  In the process, it updates the state
    variables in the FLASH_BLOCK structure as well as the count (pRawSize)
    of bytes in the raw buffer.

  Precondition:
    The FlashBlock must have been initialized.  (It may already be partially
    filled.)  Also, the raw buffer must contain *pRawSize bytes of data.

  Parameters:
    pFlashBlock - Pointer to a structure to receive data to program to Flash
                  along with information about where to program it

    pRawSize    - Pointer to a variable providing the number of bytes of data
                  pointed to by pRawBuffer.
                  
    pRawBuffer  - Pointer to the data to translate and place into the 
                  FLASH_BLOCK buffer

  Returns:
    LOADER_BLOCK_READY      - Block is ready to program to Flash
    LOADER_NEED_DATA        - Need additional data to form block
    LOADER_DECODE_ERROR     - Error decoding raw data

  Remarks:
    This routine may leave residual data in either the Flash block or the
    in the raw buffer.  It will update the state of the Flash block and
    *pRawSize count appropriately.
***************************************************************************/


#define LOADER_BLOCK_READY      0   // Block is ready to program to Flash
#define LOADER_BLOCK_NOT_READY  1   // Block is not yet ready to program
#define LOADER_NEED_DATA        2   // Need additional data to form block
#define LOADER_EOF              3   // End of the image file reached
#define LOADER_DECODE_ERROR     4   // Error decoding raw data
#define LOADER_ADDRESS_ERROR    5   // Block address error

unsigned int Loader_GetFlashBlock ( FLASH_BLOCK  *pFlashBlock, 
                                    unsigned int *pRawSize, 
                                    BYTE         *pRawBuffer  );

BOOL Loader_ProgramFlashBlock( FLASH_BLOCK *pFlashBlock );

// Optional Record Type Support (Necessary if EXTENDED_HEXFILE_SUPPORT is defined)
#ifndef Loader_ValidateSerialNumber
    #define Loader_ValidateSerialNumber(d,l)    RECORD_NON_DATA
#endif

#ifndef Loader_ValidateRevisionNumber
    #define Loader_ValidateRevisionNumber(d,l)  RECORD_NON_DATA
#endif

#ifndef Loader_CheckErrorDetection
    #define Loader_CheckErrorDetection(d,l)     RECORD_NON_DATA
#endif


/****************************************************************************
  Function:
    BOOL PermitProgramming(void)

  Description:
    Determines if flash can be written based on user defined extended hexfile
    commands 

  Precondition:
    None

  Parameters:
    None
    
  Returns:
    TRUE    - If memory should be written
    FALSE   - If memory should not be written
    
  Remarks:
    Optional
***************************************************************************/

#ifndef PermitProgramming
    #define PermitProgramming()         TRUE
#endif


/*
*******************************************************************************
EOF
*******************************************************************************
*/

