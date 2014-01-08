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

Boot Loader Common Interface:

The common boot loader interface definitions provide basic boot loader 
functionality or common items used by other interfaces and the boot loader in 
general.

Boot Loader IO Interface:

The boot loader IO interface definitions provide control inputs and status
outputs that the boot loader can use to determine when it should load a new
application image and how it should report its status.

Boot Loader Media Interface:

The boot loader media interface definitions provide access to the boot image 
medium and the ability to read the file.

These interface routines can be implemented via macros or functions as 
required.
*******************************************************************************
*/

//*****************************************************************************
//*****************************************************************************
// Boot Loader Media Interface Definitions
//*****************************************************************************
//*****************************************************************************

// Application Image File Name
#ifndef BOOT_FILE_NAME
    #define BOOT_FILE_NAME          "image.hex"
#endif

// Max number of times to retry locating the image file.
#ifndef MAX_LOCATE_RETRYS
    #define MAX_LOCATE_RETRYS            3
#endif


/* Boot Loader Media Interface Call Outs
*******************************************************************************
The following interface routines are called from the boot loader to access new
application image file and program it to Flash.
*******************************************************************************
*/

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

#ifndef BLMedia_InitializeTransport
    #define "BLMedia_InitializeTransport() must be defined in boot_config.h"
#endif


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

#ifndef BLMedia_DeinitializeTransport
    #define "BLMedia_DeinitializeTransport() must be defined in boot_config.h"
#endif


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

BOOL BLMedia_MonitorMedia ( void );


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

#ifndef BLMedia_MediumAttached
    #define "BLMedia_MediumAttached() must be defined in boot_config.h"
#endif


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

BOOL BLMedia_LocateFile ( char *file_name );


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

BOOL BLMedia_LoadFile (  char *file_name );


/*
*******************************************************************************
EOF
*******************************************************************************
*/

