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

// Selects the switch (port A only) pressed on Explorer 16 to load a new image.
#ifndef BOOT_SWITCH
    #error "BOOT_SWITCH must be defined in boot_config.h"
#endif


//*****************************************************************************
//*****************************************************************************
// Boot Loader IO Definitions
//*****************************************************************************
//*****************************************************************************

/****************************************************************************
  Function:
    void BLIO_InitializeIO ( void )

  Description:
    This routine initializes the status LED and boot switch ports.  It also 
    initializes the core timer so it can be used to flash the status LED.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
***************************************************************************/

void BLIO_InitializeIO ( void );


/****************************************************************************
  Function:
    void BLIO_DeinitializeIO ( void )

  Description:
    This routine de-initializes the IO pins and disables and clears the core
    timer interrupt.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    Any stray pending interrupts (once the application boots) may cause
    the system to trigger an un-handled (or incorrectly handled) interrupt.
***************************************************************************/

void BLIO_DeinitializeIO ( void );


/****************************************************************************
  Function:
    BOOL BLIO_AbortLoad ( void )

  Description:
    This routine is called to determine if the user needs to abort the load 
    of the new application after BLIO_LoaderEnabled has returned TRUE and the
    search for a new application image has began.  It is used to provides a 
    way out of the main loader loop if no image is available.

  Precondition:
    BLIO_InitializeIO must have been called.

  Parameters:
    None

  Returns:
    TRUE    - If the load attempt should be aborted
    FALSE   - If the load attempt should continue

  Remarks:
    Will not interrupt a load once it's actually started.
***************************************************************************/

BOOL BLIO_AbortLoad ( void );

/****************************************************************************
  Function:
    BOOL BLIO_LoaderEnabled ( void )

  Description:
    This routine is called to determine if the boot loader should attempt to
    load a new application.

  Precondition:
    BLIO_InitializeIO must have been called.

  Parameters:
    None

  Returns:
      TRUE  - If a new application should be loaded
      FALSE - If the existing application should be booted


  Remarks:
    This function must be implemented.
***************************************************************************/
BOOL BLIO_LoaderEnabled ( void );

/****************************************************************************
  Function:
    void BLIO_ReportBootStatus ( BOOT_STATUS status, char *message )

  Description:
    This function can be used to display the current status of the boot 
    loader and possibly a with a descriptive message.

  Precondition:
    BLIO_InitializeIO must have been called.

  Parameters:
    status      - A code indicating the current boot loader status
    message     - A text string describing the current status

  Returns:
    None

  Remarks:
    This function can be implemented in any way that is appropriate for the 
    device.  Not all of the information needs to be used.  A macro
    definition can be used to "throw away" the string to reduce the Flash 
    footprint of the boot loader.
***************************************************************************/

#ifndef BLIO_ReportBootStatus
    #error "BLIO_ReportBootStatus(s,z) must be defined in boot_config.h"
#endif


/*
*******************************************************************************
EOF
*******************************************************************************
*/

