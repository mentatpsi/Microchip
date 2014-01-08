/********************************************************************
  File Information:
    FileName:     	soft_start.h
    Dependencies:   None
    Processor:      PIC18F46J50
    Hardware:       PIC18F Starter Kit
    Complier:  	    Microchip C18 (for PIC18)
    Company:        Microchip Technology, Inc.
    
    Software License Agreement:
    
    The software supplied herewith by Microchip Technology Incorporated
    (the “Company”) for its PIC® Microcontroller is intended and
    supplied to you, the Company’s customer, for use solely and
    exclusively on Microchip PIC Microcontroller products. The
    software is owned by the Company and/or its supplier, and is
    protected under applicable copyright laws. All rights are reserved.
    Any use in violation of the foregoing restrictions may subject the
    user to criminal sanctions under applicable laws, as well as to
    civil liability for the breach of the terms and conditions of this
    license.
    
    THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
    TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
    IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

  File Description:
    
    Change History:
     Rev   Date         Description
     1.0                Initial release

  Summary:
    This file contains all of functions, macros, definitions, variables,
    datatypes, etc. that are required to implement the soft start of the
    application power on the PIC18F Starter Kit.

  Description:
    This file contains all of functions, macros, definitions, variables,
    datatypes, etc. that are required to implement the soft start of the
    application power on the PIC18F Starter Kit.

    Please refer to the PIC18F Starter Kit documentations for a full
    description of how this code corrisponds with the circuit and how this
    is used to achieve a USB compliant soft start.

********************************************************************/

/********************************************************************
	Function:
		void AppPowerEnable(void)
		
 	Summary:
 		This function starts the soft start process for the PIC18F starter kit.

 	Description:
 		This function starts the soft start process for the PIC18F starter kit.
        This function is optional if APP_POWER_AUTO_ENABLE is defined. 	
		
	PreCondition:
		Timer 2, Timer 4, and CCP1 are available.
		
	Parameters:
		None
		
	Return Values:
		None
		
	Remarks:
        This function uses Timer 2, Timer 4, CCP1, and one I/O pin.  When the
        AppPowerReady() function returns TRUE, the timers and CCP1 have been 
        reset to their reset values and are no longer in use and can be used by 
        the user application.  The I/O pin should not be manipulated.
 
 *******************************************************************/
void AppPowerEnable(void);

/********************************************************************
	Function:
		void AppPowerDisable(void)
		
 	Summary:
        Disables the application power for the PIC18F Starter Kit.

 	Description:
        Disables the application power for the PIC18F Starter Kit. 
		
	PreCondition:
		None
		
	Parameters:
		None
		
	Return Values:
		None
		
	Remarks:
		Not that to power up the application power again, the Timer and CCP 
        resources used by AppPowerEnable() or AppPowerReady() will be used again.
 
 *******************************************************************/
void AppPowerDisable(void);

/********************************************************************
	Function:
		void AppPowerReady(void)
		
 	Summary:
       This function determines if the soft start process is complete or not.

 	Description:
        This function determines if the soft start process is complete or not.
        if the APP_POWER_AUTO_ENABLE feature is defined then calling this
        function will also start the soft start process without having to call
        the AppPowerEnable() function.
		
	PreCondition:
		None
		
	Parameters:
		None
		
	Return Values:
		TRUE - the application power is fully ramped and ready.  This also
        indicates that the timer and CCP resources are now available for
        application use (see Remarks).
        FALSE - the applicatoin power is not ready.  See Remarks section for
        details about the auto-start option.
		
	Remarks:
		This function uses Timer 2, Timer 4, CCP1, and one I/O pin.  When this
        function returns TRUE, the timers and CCP1 have been reset to their
        reset values and are no longer in use and can be used by the user
        application.  The I/O pin should not be manipulated.

        If APP_POWER_AUTO_ENABLE is defined, then this function will
        automatically start the application power if it is not already started.
        If APP_POWER_AUTO_ENABLE is not defined then AppPowerEnable() must
        be called to start the application power.
 
 *******************************************************************/
BOOL AppPowerReady(void);
