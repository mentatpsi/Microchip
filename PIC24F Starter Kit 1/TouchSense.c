/******************************************************************************

    Capacitive Touch Sense

Description:
    This file contains the capacitive touch sensing routines for the PIC24F
    Starter Kit.  The CTMU (Charge Time Measurement Unit) is used for the
    capacitive touch switches.  The five switches are simply copper pads on the
    top layer of the printed circuit board.  The CTMU measures the relative
    capacitance of the switches in conjunction with the A/D converter.  The
    CTMU contains a constant current source, which linearly charges the
    capacitive switch with respect to time.  The amount of time the switch is
    charged is fixed, so that the voltage on the switch is constant as long as
    the switch remains unchanged.  When the switch is pressed, the capacitance
    of the human body is added in parallel with the capacitance of the switch.
    Since the charging time is fixed, the new larger capacitance results in a
    lower voltage read by the A/D.  Additional averaging of the A/D readings
    is done to smooth out the readings.

    Each switch reading is comprised of first draining all charge from the
    circuit by grounding the switch through a grounding function of the CTMU.
    This assures the charge on the circuit starts at 0 Volts.  The CTMU then
    charges the switch for a fixed amount of time, stops, and signals the A/D
    converter to make a conversion.  The voltage read is stored for use by the
    routine checking for button presses.  The switch is then discharged again,
    to make sure no leakage occurs to other switches located nearby.

Summary:
    This file contains the capacitive touch sensing routines for the PIC24F
    Starter Kit.

Remarks:
    None
*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

* File Name:       TouchSense.c
* Dependencies:    See the #include files below
* Processor:       PIC24F
* Compiler:        C30 v3.02c
* Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the “Company”) for its PICmicro® Microcontroller is intended and
supplied to you, the Company’s customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
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

Author          Date    Comments
--------------------------------------------------------------------------------
KO          14-Feb-2008 First release

*******************************************************************************/
//DOM-IGNORE-END

#include <p24Fxxxx.h>
#include "PIC24F Starter Kit.h"

//******************************************************************************
//******************************************************************************
// Constants and Enumerations
//******************************************************************************
//******************************************************************************

// CTMU Constants

#define CTMU_OFF                        0x0000
#define CTMU_CONTINUE_IN_IDLE           0x0000
#define CTMU_EDGE_DELAY_DISABLED        0x0000
#define CTMU_EDGES_BLOCKED              0x0000
#define CTMU_NO_EDGE_SEQUENCE           0x0000
#define CTMU_CURRENT_NOT_GROUNDED       0x0000
#define CTMU_TRIGGER_OUT_DISABLED       0x0000
#define CTMU_EDGE2_NEGATIVE             0x0000
#define CTMU_EDGE2_CTED1                0x0060
#define CTMU_EDGE2_CTED2                0x0040
#define CTMU_EDGE1_POSITIVE             0x0010
#define CTMU_EDGE1_CTED1                0x000C
#define CTMU_EDGE1_CTED2                0x0008

#define CTMU_EDGE_MASK                  0x0003
#define CTMU_EDGE2                      0x0002
#define CTMU_EDGE1                      0x0001

#define AVG_DELAY                       64 //1 
#if defined TOUCHPAD_DIRECT_TOUCH
	#define CHARGE_TIME_COUNT           90 //34 // If optimized, this value must change.
#else
	#define CHARGE_TIME_COUNT           6 //34 // If optimized, this value must change.
	#define NUM_CTMU_READS				16
#endif


typedef enum {
    TOUCHPAD1 = 0,
    TOUCHPAD2,
    TOUCHPAD3,
    TOUCHPAD4,
    TOUCHPAD5,
    TOUCHPAD_NONE
} TOUCHPAD;

typedef enum {
    TOUCHPAD_NONE_MASK    = 0,
    TOUCHPAD1_MASK        = 0x01,
    TOUCHPAD2_MASK        = 0x02,
    TOUCHPAD3_MASK        = 0x04,
    TOUCHPAD4_MASK        = 0x08,
    TOUCHPAD5_MASK        = 0x10
} TOUCHPAD_MASK;

#define TOUCHPAD_ALL_BUTTONS_MASK   (TOUCHPAD1_MASK | TOUCHPAD2_MASK | TOUCHPAD3_MASK | TOUCHPAD4_MASK | TOUCHPAD5_MASK )

typedef enum {
    STATE_START = 0,
    STATE_FIRST_PRESS,
    STATE_SEND_RELEASED_REPEAT,
    STATE_SEND_PRESSED,
    STATE_NEXT_PRESS_WAIT,
    STATE_SEND_RELEASED,
} TOUCHPAD_STATES;


//******************************************************************************
//******************************************************************************
// Data Structures
//******************************************************************************
//******************************************************************************

typedef union
{
    struct
    {
        unsigned    BTN1:1;
        unsigned    BTN2:1;
        unsigned    BTN3:1;
        unsigned    BTN4:1;
        unsigned    BTN5:1;
    };
    WORD            val;
} TOUCHPADS;


//******************************************************************************
//******************************************************************************
// Variables
//******************************************************************************
//******************************************************************************

TOUCHPADS                   buttons;
TOUCHPADS                   buttonsPrevious;
extern SCREEN_STATES        screenState;

unsigned int                rawCTMU[NUM_TOUCHPADS];       //raw A/D value
unsigned int                average[NUM_TOUCHPADS];       //averaged A/D value
unsigned int                trip   [NUM_TOUCHPADS];       //trip point for TOUCHPAD
unsigned int                hyst   [NUM_TOUCHPADS];       //hysterisis for TOUCHPAD

unsigned char               first;                  // first variable to 'discard' first N samples
unsigned char               buttonIndex;                    // Index of what TOUCHPAD is being checked.

unsigned int                value;                  // current button value
unsigned int                bigVal;                 // current button bigval
unsigned int                smallAvg;               // current button smallavg
unsigned int                AvgIndex;


//******************************************************************************
//******************************************************************************
// Local Prototypes
//******************************************************************************
//******************************************************************************

void SetNextChannel(void);

/****************************************************************************
  Function:
    void CTMUInit( void )

  Description:
    This routine sets up the CTMU for capacitive touch sensing.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/

void CTMUInit( void )
{
    int     i;

    #if defined USE_GFX_DISPLAY_CONTROLLER_SH1101A
        TRISB    = 0x1F01;     //RB0, RB8, RB9, RB10, RB11, RB12 Tri-state - used by A/D for CTMU/POT
        AD1PCFGL &= ~0x1F01;
    #elif defined USE_GFX_DISPLAY_CONTROLLER_SSD1303
        TRISB    = 0x0F01;     //RB0, RB9, RB10, RB11, RB12 Tri-state - used by A/D for CTMU/POT
        AD1PCFGL &= ~0x0F01;
    #endif

    // Set up the CTMU

    CTMUCON =   CTMU_OFF                    |
                CTMU_CONTINUE_IN_IDLE       |
                CTMU_EDGE_DELAY_DISABLED    |
                CTMU_EDGES_BLOCKED          |
                CTMU_NO_EDGE_SEQUENCE       |
                CTMU_CURRENT_NOT_GROUNDED   |
                CTMU_TRIGGER_OUT_DISABLED   |
                CTMU_EDGE2_NEGATIVE         |
                CTMU_EDGE2_CTED1            |
                CTMU_EDGE1_POSITIVE         |
                CTMU_EDGE1_CTED1;

    #if defined TOUCHPAD_DIRECT_TOUCH
        CTMUICONbits.IRNG   = 2;        // 5.5uA
    #else
        CTMUICONbits.IRNG   = 3;        // 55uA
    #endif
    CTMUICONbits.ITRIM  = 0;        // 0%

    // Set up the ADC

////    AD1PCFG             = 0x0000;
    AD1CON1             = 0x0000;
    AD1CHS              = STARTING_ADC_CHANNEL; // Select the starting analog channel
    AD1CSSL             = 0x0000;
    AD1CON1bits.FORM    = 0x0;                  // Unsigned integer format
    AD1CON3             = 0x0002;
    AD1CON2             = 0x0000;
    AD1CON1bits.ADON    = 1;                    // Start A/D in continuous mode

    // Enable the CTMU

    CTMUCONbits.CTMUEN  = 1;

    // Initialize Capacitive Sensing

    for (i = 0; i < NUM_TOUCHPADS; i++ )
    {
        trip[i]         = TRIP_VALUE;
        hyst[i]         = HYSTERESIS_VALUE;
        // rawCTMU[] and average[] arrays are initialized to 0 by definition
    }

    buttonIndex         = 0;
    first               = 160;          // First pass flag to reach steady state average before enabling detection
}


/****************************************************************************
  Function:
    void ReadCTMU( void )

  Description:
    Thi is the capacitive touch sensing service routine for CTMU.  It takes
    the measurement; determines if the button under test is pressed or not,
    setting a flag accordingly; and averages the new reading appropriately.
    This is done for all buttons sequentially.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None

  Remarks:
    To keep all A/D manipulation together, the Starter Kit's potentiometer is
    also read here if we are graphing or capturing.
  ***************************************************************************/

void ReadCTMU( void )
{
    int                     current_ipl;
    int                     i;
    int                     j;
#if defined TOUCHPAD_USE_OVERLAY
	int						k;
#endif
    volatile unsigned int   tempADch;
	extern SCREEN_STATES 	screenState;


    tempADch            = AD1CHS;      //store the current A/D mux channel selected

    AD1CON1             = 0x0000;       // Unsigned integer format
    AD1CSSL             = 0x0000;
    AD1CON3             = 0x0002;
    AD1CON2             = 0x0000;
    AD1CON1bits.ADON    = 1;            // Start A/D in continuous mode

    for(i = 0; i < NUM_TOUCHPADS; i++)
    {
        #if defined TOUCHPAD_USE_OVERLAY		
		for(k = 0; k < NUM_CTMU_READS; k++)
        #endif
		{
            // Get the raw sensor reading.
            AD1CHS                  = STARTING_ADC_CHANNEL + buttonIndex; //select A/D channel
    
            // Make sure touch circuit is completely discharged
            IFS0bits.AD1IF          = 0;
            AD1CON1bits.DONE        = 0;
            AD1CON1bits.SAMP        = 1;        // Manually start sampling
            // Wait for the A/D converter to begin sampling
            Nop();    Nop();    Nop();    Nop();    Nop();    Nop();    Nop();    Nop();
            CTMUCONbits.IDISSEN     = 1;        // Drain any charge on the circuit
            Nop();    Nop();    Nop();    Nop();    Nop();
            CTMUCONbits.IDISSEN     = 0;
            Nop();    Nop();    Nop();    Nop();    Nop();
            IFS0bits.AD1IF          = 0;
            AD1CON1bits.SAMP        = 0;		//Manually start conversion
            while(!IFS0bits.AD1IF);             // Wait for the A/D conversion to finish
                                                // Note: This A/D conversion not used.
                                                // A/D mux must connect to the channel
                                                // for CTMU to drain charge
    
            // Charge touch circuit
    
            // Since the charge is time dependent, set the CPU priority such
            // that we will not be interrupted during the read.
            SET_AND_SAVE_CPU_IPL( current_ipl, 7 );
    
            IFS0bits.AD1IF          = 0;
    
            AD1CON1bits.SAMP        = 1;        // Manually start sampling
            CTMUCONbits.EDG2STAT    = 0;        // Make sure edge2 is 0
            CTMUCONbits.EDG1STAT    = 1;        // Set edge1 - Start Charge
    
            for (j = 0; j < CHARGE_TIME_COUNT; j++);    // Delay for CTMU charge time
    
            CTMUCONbits.EDG1STAT    = 0;        // Clear edge1 - Stop Charge
    
            // Re-enable interrupts.
            RESTORE_CPU_IPL( current_ipl );
    
            IFS0bits.AD1IF          = 0;
            AD1CON1bits.SAMP        = 0;
            while(!IFS0bits.AD1IF);             // Wait for the A/D conversion to finish
            #if defined TOUCHPAD_USE_OVERLAY
            	    value = value + ADC1BUF0; 			// Read the value from the A/D conversion
            #else
            		value = ADC1BUF0;
            #endif
    
            //Discharge the touch circuit
            IFS0bits.AD1IF          = 0;
            AD1CON1bits.SAMP        = 1;        // Manually start sampling
    
            // Wait for A/D conversion to begin
            Nop();    Nop();    Nop();    Nop();    Nop();    Nop();    Nop();    Nop();
            CTMUCONbits.IDISSEN     = 1;        // Drain any charge on the circuit
            Nop();    Nop();    Nop();    Nop();    Nop();
            CTMUCONbits.IDISSEN     = 0;        // End charge drain
            Nop();    Nop();    Nop();    Nop();
            IFS0bits.AD1IF          = 0;
            AD1CON1bits.SAMP        = 0;        // Perform conversion
            while(!IFS0bits.AD1IF);             // Wait for the A/D conversion to finish
            IFS0bits.AD1IF          = 0;
            AD1CON1bits.DONE        = 0;        // Note: The A/D conversion not used
                                                // A/D mux must connect to the channel
                                                // for CTMU to drain charge
            //End of CTMU read
		}
        #if defined TOUCHPAD_USE_OVERLAY
		    value = value/NUM_CTMU_READS;
        #endif

        #if defined TOUCHPAD_LO_PASS
		if (screenState == SCREEN_BLASTER)
        #endif
		{
    		// Allow greater sensitivity for the game Blaster.
		    bigVal                  = value  * 16; 
		}
        #if defined TOUCHPAD_LO_PASS
		else               
		{
    		// Integrate the raw values for better stability.
		    bigVal					= ((rawCTMU[buttonIndex]>>4)*7+value)<<1; 
		}    
        #endif  
        
        smallAvg                = average[buttonIndex]  / 16;   // smallAvg is the current average right shifted 4 bits
        rawCTMU[buttonIndex]    = bigVal;                       // raw array holds the most recent bigVal values

        // On power-up, reach steady-state readings

        if (first > 0)
        {
            first--;
            average[buttonIndex] = bigVal;
            SetNextChannel();
            break;
        }

        // Is a keypad button pressed or released?

        if (bigVal < (average[buttonIndex] - trip[buttonIndex]))
        {
            // Pressed
            switch(buttonIndex)
            {
                case TOUCHPAD1:    buttons.BTN1  = 1;  break;
                case TOUCHPAD2:    buttons.BTN2  = 1;  break;
                case TOUCHPAD3:    buttons.BTN3  = 1;  break;
                case TOUCHPAD4:    buttons.BTN4  = 1;  break;
                case TOUCHPAD5:    buttons.BTN5  = 1;  break;
            }

        }
        else if (bigVal > (average[buttonIndex] - trip[buttonIndex] + hyst[buttonIndex]))
        {
            // Released
            switch(buttonIndex)
            {
                case TOUCHPAD1:    buttons.BTN1  = 0;  break;
                case TOUCHPAD2:    buttons.BTN2  = 0;  break;
                case TOUCHPAD3:    buttons.BTN3  = 0;  break;
                case TOUCHPAD4:    buttons.BTN4  = 0;  break;
                case TOUCHPAD5:    buttons.BTN5  = 0;  break;
            }
        }

        // Implement quick-release for a released button

        if (bigVal  > average[buttonIndex])
        {
            average[buttonIndex] = bigVal;                                      // If raw is above Average, reset to high average.
        }

        // Average in the new value.  Always Average (all buttons)
        // Counting 0..8 has effect of every 9th count cycling the next button.
        // Counting 0..4 will average faster and also can use 0..4*m, m=0,1,2,3..
	        if(buttonIndex == 0)
        {
            if (AvgIndex < AVG_DELAY)
            {
                AvgIndex++;
            }
            else
            {
                AvgIndex = 0;
            }
        }

        if (AvgIndex == AVG_DELAY)
        {
            // Average in raw value.
			
            average[buttonIndex] = average[buttonIndex] + (value - smallAvg);
        }

        // Determine next sensor to test.

        SetNextChannel();

    }

    if((screenState == SCREEN_GRAPH) || (screenState == SCREEN_CAPTURE))
    {
        // Read the potentiometer and store it for the demo.
        GraphReadPotentiometer();
    }

    AD1CHS = tempADch;  //restore A/D channel select
}


/****************************************************************************
  Function:
    void SetNextChannel( void )

  Description:
    This function sets the next channel in sequence to be scanned based off
    of the current index value.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/

void SetNextChannel( void )
{
    buttonIndex++;
    if (buttonIndex == NUM_TOUCHPADS)
    {
        buttonIndex = 0;
    }
}


/****************************************************************************
  Function:
    void TouchSenseButtonsMsg(GOL_MSG* msg)

  Description:
    This routine creates approprite messages for button presses & releases.

  Precondition:
    None

  Parameters:
    GOL_MSG *msg    - The new system message

  Returns:
    None

  Remarks:
    Look for a change in the state of a button.  If there
    has been a state change, we send a message.  Note that this means that
    repetative messages will not be generated if a touchpad is held for a
    long period of time.
  ***************************************************************************/

void TouchSenseButtonsMsg(GOL_MSG* msg)
{
    msg->uiEvent    = EVENT_INVALID;
    msg->type       = TYPE_KEYBOARD;

    if (buttons.BTN1 != buttonsPrevious.BTN1)
    {
        msg->uiEvent = EVENT_KEYSCAN;
        msg->param1  = ID_TOUCH_PAD;            //ID_TOUCH_BUTTON_01;
        if (buttons.BTN1)
        {
            msg->param2  = SCAN_UP_PRESSED;     //SCAN_CR_PRESSED;
        }
        else
        {
            msg->param2  = SCAN_UP_RELEASED;    //SCAN_CR_RELEASED;
        }
        buttonsPrevious.BTN1 = buttons.BTN1;
        return;
    }

    if (buttons.BTN2 != buttonsPrevious.BTN2)
    {
        msg->uiEvent = EVENT_KEYSCAN;
        msg->param1  = ID_TOUCH_PAD;            //ID_TOUCH_BUTTON_02;
        if (buttons.BTN2)
        {
            msg->param2  = SCAN_RIGHT_PRESSED;  //SCAN_CR_PRESSED;
        }
        else
        {
            msg->param2  = SCAN_RIGHT_RELEASED; //SCAN_CR_RELEASED;
        }
        buttonsPrevious.BTN2 = buttons.BTN2;
        return;
    }

    if (buttons.BTN3 != buttonsPrevious.BTN3)
    {
        msg->uiEvent = EVENT_KEYSCAN;
        msg->param1  = ID_TOUCH_PAD;            //ID_TOUCH_BUTTON_03;
        if (buttons.BTN3)
        {
            msg->param2  = SCAN_DOWN_PRESSED;   //SCAN_CR_PRESSED;
        }
        else
        {
            msg->param2  = SCAN_DOWN_RELEASED;  //SCAN_CR_RELEASED;
        }
        buttonsPrevious.BTN3 = buttons.BTN3;
        return;
    }

    if (buttons.BTN4 != buttonsPrevious.BTN4)
    {
        msg->uiEvent = EVENT_KEYSCAN;
        msg->param1  = ID_TOUCH_PAD;            //ID_TOUCH_BUTTON_04;
        if (buttons.BTN4)
        {
            msg->param2  = SCAN_LEFT_PRESSED;   //SCAN_CR_PRESSED;
        }
        else
        {
            msg->param2  = SCAN_LEFT_RELEASED;  //SCAN_CR_RELEASED;
        }
        buttonsPrevious.BTN4 = buttons.BTN4;
        return;
    }

    if (buttons.BTN5 != buttonsPrevious.BTN5)
    {
        msg->uiEvent = EVENT_KEYSCAN;
        msg->param1  = ID_TOUCH_PAD;            //ID_TOUCH_BUTTON_05;
        if (buttons.BTN5)
        {
            msg->param2  = SCAN_CR_PRESSED;
        }
        else
        {
            msg->param2  = SCAN_CR_RELEASED;
        }
        buttonsPrevious.BTN5 = buttons.BTN5;
        return;
    }
}



