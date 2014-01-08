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
*/

#include "boot_config.h"
#include "boot_io.h"
#include "boot_load.h"
#include "boot_media.h"

/*
*******************************************************************************
Common Boot Loader Definitions
*******************************************************************************
*/

/* Boot Loader Call Out Interfaces
*******************************************************************************
The following interface "functions" are called from the boot loader to 
customize its behavior.
*******************************************************************************
*/

/******************************************************************************
  Function:
    BOOL BL_ApplicationIsValid ( void )

  Description:
    This function identifies if the application image currently in program
    Flash is valid and ready to be booted.

  Precondition:
    None

  Parameters:
    None

  Returns:
    TRUE    - If the application image in Flash is valid
    FALSE   - If the application image in Flash is invalid

  Remarks:
    This function can be implemented to validate the image.  If not 
    implemented, then this must always return TRUE or no image will be 
    booted.
******************************************************************************/

#ifndef BL_ApplicationIsValid
    #define BL_ApplicationIsValid()            TRUE
#endif


/******************************************************************************
  Function:
    void BL_ApplicationFailedToLaunch ( void )

  Description:
    This function will be called after the attempt to boot the application,
    only if the application fails to boot and "returns" to the boot loader.
    It will not be called if the application "boots" successfully.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    This function can be implemented to provide any desired action or 
    status reporting if the application fails to boot and returns to the
    boot loader.
******************************************************************************/

#ifndef BL_ApplicationFailedToLaunch
    #define BL_ApplicationFailedToLaunch()
#endif


/*
*******************************************************************************
EOF
*******************************************************************************
*/

