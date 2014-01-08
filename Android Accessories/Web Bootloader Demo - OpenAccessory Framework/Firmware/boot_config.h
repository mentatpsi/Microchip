/*
*******************************************************************************
                                                                                
Software License Agreement                                                      
                                                                                
Copyright © 2007-2008 Microchip Technology Inc. and its licensors.  All         
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

The boot loader uses three interfaces to define and customize its behavior.

Boot Loader IO Interface:

The boot loader IO interface definitions provide control inputs and status
outputs that the boot loader can use to determine when it should load a new
application image and how it should report its status.

Boot Loader Media Interface:

The boot loader media interface definitions provide access to the boot image 
medium and the ability to read the file.

Boot Loader Interface

The Loader interface definitions provide any data translation and Flash 
programming necessary to load the boot image into program Flash memory.

These interface routines can be implemented via macros or functions as 
required.
*******************************************************************************

 Change History:
  Rev   Description
  ----  -----------------------------------------
  1.0   Modified from USB Host MSD bootloader example

*******************************************************************************
*/

/*
*******************************************************************************
Boot Loader size Definitions
*******************************************************************************
*/
#define NUM_BOOT_REGIONS            1
#define SIZE_OF_USER_HEADER_SIZE    4
#define VALIDATION_CODE_SIZE        4

/*
*******************************************************************************
Boot Loader Validation Methods
*******************************************************************************
*/
#define CHECK_SUM   // For now, only check sum support


/*
*******************************************************************************
Boot Loader IO Configuration
*******************************************************************************
*/
#define BLIO_LoaderEnabled()    (sw1||sw2||sw3||sw4)


/* Boot Loader Media Interface Call Outs
*******************************************************************************
The following interface routines are called from the boot loader to access new
application image file and program it to Flash.
*******************************************************************************
*/

#define BLMedia_InitializeTransport()       USBInitialize(0)

#if defined(__PIC32__)
#define BLMedia_DeinitializeTransport()     (USBHostShutdown(),IFS1CLR = 0x02000000)
#else
#define BLMedia_DeinitializeTransport()     (USBHostShutdown())
#endif


/* "Loader" Interface Call Outs
*******************************************************************************
The following interface is used by the media layer to decode and program the 
image file to memory.
*******************************************************************************
*/

// These macros define the maximum size of a Flash block.

//512KB PIC32 variants
#if defined(__C32__)

    #define FLASH_BLOCK_SIZE            (1024 * 4)          // Size in bytes
    #define APPLICATION_ADDRESS         0x9D00F490// + VALIDATION_CODE_SIZE

//245KB PIC24F variants
#elif   defined(__C30__) || defined __XC16__
		#define APPLICATION_ADDRESS         0x3000ul + VALIDATION_CODE_SIZE
		#define FLASH_BLOCK_SIZE            (1024)          // Size in bytes
#else
    #error "Device currently not supported"
#endif


/*
*******************************************************************************
EOF
*******************************************************************************
*/

