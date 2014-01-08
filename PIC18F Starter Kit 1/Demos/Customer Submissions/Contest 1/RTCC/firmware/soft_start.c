/********************************************************************
  File Information:
    FileName:     	soft_start.c
    Dependencies:   See INCLUDES section
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

/******** Include files **********************/
#include "GenericTypeDefs.h"
#include "Compiler.h"
/*********************************************/


/******** Configuration Options **************/
#define APP_POWER_PWM_START 0x00
#define APP_POWER_PWM_END 0x40
#define APP_POWER_INCREMENT_PERIOD 0x60
#define APP_POWER_PWM_PERIOD 0x80

#define APP_POWER_PIN_TRIS TRISCbits.TRISC0
#define APP_POWER_PIN_LAT  LATCbits.LATC0
#define APP_POWER_PIN_PORT PORTCbits.RC0
#define APP_POWER_RP_REGISTER RPOR11

#define APP_POWER_INCREMENT_POSTSCALE 16

#define APP_POWER_AUTO_ENABLE
/*********************************************/


/************ Type Definitions ***************/
typedef union _app_power_status_bits
{
    struct
    {
        BYTE power_ready :1;
        BYTE ramping_power :1;
    };
    BYTE Val;
} APP_POWER_STATUS_BITS;
/********************************************/


/*************** Variables ******************/
#pragma idata
APP_POWER_STATUS_BITS app_power_status = 0;
#pragma udata
/********************************************/


/*************** Functions ******************/
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
void AppPowerEnable(void)
{
    //Check to see if the power is already available, if so then
    //  don't do anything
    if(app_power_status.power_ready == 0)
    {
        //If the power isn't ready then let's indicate that we are
        //  in the process of ramping the power
        app_power_status.ramping_power = 1;

        //Set the FET to full off to start off with.
        APP_POWER_PIN_LAT = 1;
        APP_POWER_PIN_TRIS = 0;

	//  UnLock Registers
		EECON2 = 0x55;
		EECON2 = 0xAA;
		PPSCONbits.IOLOCK = 0;
	//  Unlock ends

        //Set the CCP1 module to the correct RP pin
        APP_POWER_RP_REGISTER = 14;    //CCP1 output to RP11

	// 	Lock Registers
		EECON2 = 0x55;
		EECON2 = 0xAA;
		PPSCONbits.IOLOCK = 1;
	//  Lock Registers ends

        //Configure the CCP1 module for PWM mode with the starting PWM values
        CCP1CON = 0b00001111; //single output, PWM
        CCP1CONbits.DC1B = 0b00;
        CCPR1L = APP_POWER_PWM_START;

        //Configure the TMR2 module for the starting period value
        TCLKCON = 0;
        PR2 = APP_POWER_PWM_PERIOD;
        T2CON = 0b00000100;

        //Configure the TMR4 module for the time between decreases in the
        //  positive width (duty cycle) of TMR2 (and thus the PWM)
        PIR3bits.TMR4IF = 0;
        PR4 = APP_POWER_INCREMENT_PERIOD;
        T4CON = 0b00000111 | ((APP_POWER_INCREMENT_POSTSCALE - 1)<<3);
    }
}

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
void AppPowerDisable(void)
{
    //Clear the internal variables to reflect that we are not powered nor 
    //  in the ramping process
    app_power_status.power_ready = 0;
    app_power_status.ramping_power = 0;

    //Set the PFET to full off, thus disabling the APP_VDD on the board
    APP_POWER_PIN_LAT = 1;
    APP_POWER_PIN_TRIS = 0;
}

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
BOOL AppPowerReady(void)
{
    //Check to see if we think that we are already powered
    if(app_power_status.power_ready == 1)
    {
        //If we think we are already powered then see if we have the PFET
        //  set to full on yet.
        if(APP_POWER_PIN_LAT == 0 && APP_POWER_PIN_TRIS == 0)
        {
            //If so then we are truly full on.  Return TRUE indicating that 
            //  power is ready
            return TRUE;
        }
        else
        {
            //If we are not full on then something went wrong in the power up
            //  sequence.  Clear the ready indication to show that we are
            //  no longer ready.
            app_power_status.power_ready = 0;

            //If the AUTO_ENABLE feature is set then start the power up process
            //  otherwise just return FALSE to show that power is not ready
            #if defined(APP_POWER_AUTO_ENABLE)
                AppPowerEnable();
            #else
                return FALSE;
            #endif
        }
    }

    //Check to see if we are in the process of ramping the voltage
    if(app_power_status.ramping_power == 0)
    {
        //If not then,
        //  if the AUTO_ENABLE feature is set then start the power up process
        //  otherwise just return FALSE to show that power is not ready
        #if defined(APP_POWER_AUTO_ENABLE)
            AppPowerEnable();
        #else
            return FALSE;
        #endif
    }

    //If timer 4 has rolled over, this means that we need to decrease the 
    //  duty cycle of the PWM.
    if(PIR3bits.TMR4IF)
    {
        //Clear the interrupt flag for the next event
        PIR3bits.TMR4IF = 0;

        //If the DC1B bits are ready to roll over (the LSBs of the duty cycle)
        if(CCP1CONbits.DC1B == 0b11)
        {
            //If the total period has reached the ending point
            if(CCPR1L == APP_POWER_PWM_END)
            {
                //Set the output to full on
                APP_POWER_PIN_LAT = 0;
                APP_POWER_PIN_TRIS = 0;

                //Turn off the PWM
                CCP1CON = 0;

                //Return all of the other registers to their reset value
                T2CON = 0;
                APP_POWER_RP_REGISTER = 0;
                TCLKCON = 0;
                PR2 = 0xFF;
                T4CON = 0;
                PR4 = 0xFF;
                PIR3bits.TMR4IF = 0;

                //Indicate that the power is ready and that we are no longer
                //  in the ramping process.
                app_power_status.power_ready = 1;
                app_power_status.ramping_power = 0;

                //return TRUE to indicate that the application is now powered
                return TRUE;
            }

            //If we haven't reached the ending duty cycle value then move to
            //  the next value
            CCPR1L++;
            CCP1CONbits.DC1B = 0;
        }
        else
        {
            //If the DC1B bits are not at a roll over point yet then go to the
            //  next value.
            CCP1CONbits.DC1B++;
        }
    }  

    //If we haven't returned TRUE already then that means we are still in the 
    //  ramping process.  Return FALSE indicating that the power isn't ready
    //  yet.
    return FALSE;  
} 

/** EOF *************************************/
