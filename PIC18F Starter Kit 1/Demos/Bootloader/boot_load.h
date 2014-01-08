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

#ifndef PROGRAM_FLASH_END
    #error "PROGRAM_FLASH_END must be defined in boot_config.h"
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

// blockState values
#define BLOCK_STATE_NEW           0
#define BLOCK_STATE_PARTIAL       1
#define BLOCK_STATE_READY         2


#define LOADER_BLOCK_READY      0   // Block is ready to program to Flash
#define LOADER_BLOCK_NOT_READY  1   // Block is not yet ready to program
#define LOADER_NEED_DATA        2   // Need additional data to form block
#define LOADER_EOF              3   // End of the image file reached
#define LOADER_DECODE_ERROR     4   // Error decoding raw data
#define LOADER_ADDRESS_ERROR    5   // Block address error

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

/*
*******************************************************************************
EOF
*******************************************************************************
*/

