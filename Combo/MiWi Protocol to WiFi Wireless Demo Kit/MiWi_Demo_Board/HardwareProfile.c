/*****************************************************************************
 *
 *              HardwareProfile.c -- Hardware Profile
 *
 *****************************************************************************
 * FileName:        HardwareProfile.c
 * Dependencies:
 * Processor:       PIC18, PIC24, PIC32, dsPIC30, dsPIC33
 * Compiler:        C18 02.20.00 or higher
 *                  C30 02.03.00 or higher
 *                  C32 01.00.02 or higher
 * Linker:          MPLINK 03.40.00 or higher
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2007-2010 Microchip Technology Inc.  All rights reserved.
 *
 * Microchip licenses to you the right to use, modify, copy and distribute 
 * Software only when embedded on a Microchip microcontroller or digital 
 * signal controller and used with a Microchip radio frequency transceiver, 
 * which are integrated into your product or third party product (pursuant 
 * to the terms in the accompanying license agreement).   
 *
 * You should refer to the license agreement accompanying this Software for 
 * additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY 
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
 * PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
 * LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
 * CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
 * DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
 * ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
 * LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
 * TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
 * NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *****************************************************************************
 * File Description:
 *
 *   This file provides configuration and basic hardware functionality 
 *   based on chosen hardware demo boards.
 *
 * Change History:
 *  Rev   Date         Description
 *  1.0   2/02/2011    MiWi CARD Demo
 *****************************************************************************/
#include "SystemProfile.h"
#include "Compiler.h"
#include "Console.h"
#include "LCD_ST7032.h"
#include "TimeDelay.h"
#include "HardwareProfile.h"

// Config Bit Settings
#pragma config OSC = INTOSCPLL, WDTEN = OFF, XINST = ON, WDTPS = 2048, PLLDIV = 2, CPUDIV = OSC3_PLL3


#define DEBOUNCE_TIME 0x00002FFF
#define SWITCH_NOT_PRESSED  0
#define SWITCH0_PRESSED     1
#define SWITCH1_PRESSED     2

BOOL switch0Pressed;
TICK switch0PressTime;
BOOL switch1Pressed;
TICK switch1PressTime;

/*********************************************************************
 * Function:        void BoardInit( void )
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    Board is initialized for P2P usage
 *
 * Overview:        This function configures the board 
 *
 * Note:            This routine needs to be called before the function 
 *                  to initialize stack or any other function that
 *                  operates on the stack
 ********************************************************************/
void BoardInit(void)
{
        // primary internal oscillator      
        OSCCON = 0x78;
		OSCTUNEbits.PLLEN = 1;
        WDTCONbits.SWDTEN = 0;

        EECON2 = 0x55;
        EECON2 = 0xAA;
        PPSCONbits.IOLOCK = 0;
        
        #if defined(MRF49XA)
            RPINR3 = 6;
        #endif
        
        #if defined(MRF24J40)
            RPINR1 = 4;
        #endif

        #if defined(MRF89XA)
			RPINR3 = 6;
		#endif

        #if !defined(SENSOR_PORT_UART)
            // use LCD
            RPINR21 = 19;			//Mapping SDI2 to RD2
        	RPOR23 = 9;				//Mapping SDO2 to RD6
        	RPOR21 = 10;			//Mapping SCK2 to RD4
    	#endif
    	
    	#if defined(SENSOR_PORT_UART)
    	    // use UART
    	    RPINR16 = 19;
    	    RPOR17 = 5;
    	#endif
    	
        EECON2 = 0x55;
        EECON2 = 0xAA;
        PPSCONbits.IOLOCK = 1;

        ANCON0 = 0xFF;
        ANCON1 = 0x3F;
		//ANCON1 |= 0x04;					//RB1 is digital pin

										
        INTCON2bits.RBPU = 0;
      
        TRISA = 0xFF;
        TRISB = 0xFF;
        TRISC = 0xFF;
        TRISD = 0xFF;
        TRISE = 0xFF;
        
        LATA = 0;
        LATB = 0;
        //LATC = 0;
        LATD = 0;
        LATE = 0;

	    TRISAbits.TRISA0 = 0;		//Configure the Temp sensor port
        // Enable AN1 as input
        TRISAbits.TRISA1 = 1;
    										
    	TRISDbits.TRISD2 = 0;
    	LATDbits.LATD2 = 1;
    	TRISCbits.TRISC6 = 0;
    	LATCbits.LATC6 = 0;
    	       
        // set I/O ports
        SW0_TRIS = 1;
        SW1_TRIS = 1;
        LED0_TRIS = 0;
        LED1_TRIS = 0;
        
		#if !defined(MRF24J40)
			LED2_TRIS = 0;
        #endif
		#if defined(MRF24J40) || defined(MRF49XA)        
        	PHY_CS_TRIS = 0;
        	PHY_CS = 1;
        	PHY_RESETn_TRIS = 0;
        	PHY_RESETn = 1;
		#endif
        RF_INT_TRIS = 1;
        
        SDI_TRIS = 1;
        SDO_TRIS = 0;
        SCK_TRIS = 0;
        
        #if defined(HARDWARE_SPI)
            SSP1STAT = 0xC0;
			#if defined(MRF24J40)
            	SSP1CON1 = 0x20;
			#else
				SSP1CON1 = 0x21;
        #else
            SPI_SDO = 0;        
            SPI_SCK = 0;    
        #endif   
        	RF_EEnCS_TRIS = 0;
			RF_EEnCS = 1;
        #if defined(MRF49XA)
            nFSEL_TRIS = 0;
            FINT_TRIS = 1;
            nFSEL = 1;          // nFSEL inactive
            
            // enable INT effective edge
            INTCON2bits.INTEDG3 = 0;
        #endif
        
        #if defined(MRF24J40)
            PHY_WAKE_TRIS = 0;
            PHY_WAKE = 1;
            
            // enable INT effective edge
            INTCON2bits.INTEDG1 = 0;
        #endif
        
		#if defined(MRF89XA)
			Data_nCS_TRIS = 0;
			Config_nCS_TRIS = 0;
			Data_nCS = 1;
			Config_nCS = 1;
			PHY_IRQ0_TRIS = 1;

			INTCON2bits.INTEDG3 = 1;
		#endif

        #if defined(ENABLE_NVM)

            EE_nCS_TRIS = 0;
            EE_nCS = 1;
        
        #endif
        
        INTCONbits.GIEH = 1;
        
        RFIF = 0;
        RFIE = 1;

        #if defined(SENSOR_PORT_LCD)
            LCDInit();
        #endif
}


/*********************************************************************
 * Function:        void LCDDisplay(char *text, BYTE value, BOOL delay)
 *
 * PreCondition:    LCD has been initialized
 *
 * Input:           text - text message to be displayed on LCD
 *                  value - the text message allows up to one byte 
 *                          of variable in the message
 *                  delay - whether need to display the message for
 *                          2 second without change
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function display the text message on the LCD, 
 *                  including up to one BYTE variable, if applicable.
 *
 * Note:            This routine is only effective if Explorer16 or
 *                  PIC18 Explorer demo boards are used
 ********************************************************************/
void LCDDisplay(char *text, BYTE value, BOOL delay)
{
        LCDErase();
        sprintf((char *)LCDText, (far rom char*)text, value); 
        LCDUpdate();
        
        // display the message for 2 seconds
        if( delay )
        {
            BYTE i;
            for(i = 0; i < 8; i++)
            {
                DelayMs(250);
            }
        }
}


/*********************************************************************
 * Function:        BYTE ButtonPressed(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Byte to indicate which button has been pressed. 
 *                  Return 0 if no button pressed.
 *
 * Side Effects:    
 *
 * Overview:        This function check if any button has been pressed
 *
 * Note:            
 ********************************************************************/
BYTE ButtonPressed(void)
{
    TICK tickDifference;
    TICK currentTick;
    BYTE result = SWITCH_NOT_PRESSED;
  
        
    if(SW0_PORT == 0)
    {
        //if the button was previously not pressed
        if(switch0Pressed == FALSE)
        {
            switch0Pressed = TRUE;
            switch0PressTime = TickGet();
        }
    }else
    {
        switch0Pressed = FALSE;
        switch0PressTime = TickGet();
    }    
    
    if(SW1_PORT == 0)
    {
        //if the button was previously not pressed
        if(switch1Pressed == FALSE)
        {
            switch1Pressed = TRUE;
            switch1PressTime = TickGet();
        }
    }else
    {
        switch1Pressed = FALSE;
        switch1PressTime = TickGet();
    }    
    
    currentTick = TickGet();
    
    if(switch0Pressed)
    {
        tickDifference.Val = TickGetDiff(currentTick, switch0PressTime);

        if(tickDifference.Val > DEBOUNCE_TIME)
        {
            result |= SWITCH0_PRESSED;
        }    
    }
        
    if(switch1Pressed)
    {
        tickDifference.Val = TickGetDiff(currentTick, switch1PressTime);

        if(tickDifference.Val > DEBOUNCE_TIME)
        {
            result |= SWITCH1_PRESSED;
        }    
    }
    
    return result;
}


void I2CTask(void);
  

void UserInterruptHandler(void)
{
    if( PIR3bits.SSP2IF )
    {
        PIR3bits.SSP2IF = 0;
        I2CTask();
        DelayMs(5);
    }   
}
