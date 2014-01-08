/*****************************************************************************
 *
 *              BSP_8BitWirelessBd.c -- Hardware Profile
 *
 *****************************************************************************
 * FileName:        BSP_MiWiCardDemoBd.c
 * Dependencies:
 * Processor:       PIC18
 * Compiler:        C18 02.20.00 or higher
 * Linker:          MPLINK 03.40.00 or higher
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2007-2011 Microchip Technology Inc.  All rights reserved.
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
*  Rev   Date         Author    Description
*  1.0   2/02/2011    ccs       Initial revision
********************************************************************/
#include "SystemProfile.h"
#include "Compiler.h"
#include "WirelessProtocols/Console.h"
#include "WirelessProtocols/SymbolTime.h"
#include "LCD_ST7032.h"
#include "HardwareProfile.h"

// Config Bit Settings to get 16 MHz: Internal 8 MHz / 2 = 4 * 12 = 48 / 3 = 16
#pragma config OSC = INTOSCPLL, WDTEN = OFF, XINST = ON, WDTPS = 2048, PLLDIV = 2, CPUDIV = OSC3_PLL3


#define DEBOUNCE_TIME       0x00001FFF
#define SWITCH_NOT_PRESSED  0
#define SWITCH0_PRESSED     1
#define SWITCH1_PRESSED     2

BOOL switch0Pressed;
MIWI_TICK switch0PressTime;
BOOL switch1Pressed;
MIWI_TICK switch1PressTime;


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
    /*******************************************************************/
    // Primary Internal Oscillator
    /*******************************************************************/
    OSCCON = 0x78;
	OSCTUNEbits.PLLEN = 1;
    WDTCONbits.SWDTEN = 0;

    /*******************************************************************/
    // Configure PPS Related Pins
    /*******************************************************************/
    // Unlock to config PPS
    EECON2 = 0x55;
    EECON2 = 0xAA;
    PPSCONbits.IOLOCK = 0;
    
    RPINR1 = 17;            // Mapping IRQ1 to RC6(RP17) * Used for MRF89XA
    RPINR21 = 23;			// Mapping SDI2 to RD6(RP23)
    RPOR21 = 10;			// Mapping SCK2 to RD4(RP21)
    RPOR19 = 9;			    // Mapping SDO2 to RD2(RP19)
	
	// Lock System
    EECON2 = 0x55;
    EECON2 = 0xAA;
    PPSCONbits.IOLOCK = 1;

    /*******************************************************************/
    // AN0 & AN1 Analog Pins others Digital Pins
    /*******************************************************************/
    ANCON0 = 0xFC;
    ANCON1 = 0x1F;
    
    /*******************************************************************/
    // Configure Switch and LED I/O Ports
    /*******************************************************************/
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED0_TRIS = 0;
    LED1_TRIS = 0;
    LED2_TRIS = 0;
            
    SW1_TRIS = 1;
    SW2_TRIS = 1;      
								
    INTCON2bits.RBPU = 0;   // Enable PORTB Pull-ups for Switches

    /*******************************************************************/
    // Configure the Temp Sensor and VBat port
    /*******************************************************************/	
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA0 = 1;
    
    /*******************************************************************/
	// Config RF Radio
	/*******************************************************************/
#if defined(MRF24J40)
    /*******************************************************************/
	// Config MRF24J40 Pins
	/*******************************************************************/
	PHY_CS = 1;
	PHY_RESETn = 1;    
    PHY_WAKE = 1;
    
    PHY_CS_TRIS = 0;
    PHY_RESETn_TRIS = 0;        
    PHY_WAKE_TRIS = 0;
    RF_INT_TRIS = 1;
             
    // Config INT0 Edge = Falling
    INTCON2bits.INTEDG0 = 0;
    
    RFIF = 0;
    RFIE = 1;
    
    /*******************************************************************/
    // Confiure SPI1
    /*******************************************************************/     
    SDI_TRIS = 1;
    SDO_TRIS = 0;
    SCK_TRIS = 0;
    
    SSP1STAT = 0xC0;
    SSP1CON1 = 0x20;
   
#elif defined(MRF89XA)
    /*******************************************************************/
	// Config MRF89XA Pins
	/*******************************************************************/ 
    Data_nCS = 1;
    Config_nCS = 1;
    
    Data_nCS_TRIS = 0; 
    Config_nCS_TRIS = 0;   
    
    IRQ0_INT_TRIS = 1;
    IRQ1_INT_TRIS = 1;
    
    // Config IRQ1 Edge = Rising
    INTCON2bits.INTEDG1 = 1;
    
    // Config IRQ0 Edge = Falling
    INTCON2bits.INTEDG0 = 1;
    
    PHY_IRQ0 = 0;           // MRF89XA
    PHY_IRQ0_En = 1;        // MRF89XA  
    PHY_IRQ1 = 0;           // MRF89XA
    PHY_IRQ1_En = 1;        // MRF89XA    
    /*******************************************************************/
    // Confiure SPI1
    /*******************************************************************/   
    SDI_TRIS = 1;
    SDO_TRIS = 0;
    SCK_TRIS = 0;
    
    SSP1STAT = 0xC0;
    SSP1CON1 = 0x21;

    PHY_RESETn = 0;
    PHY_RESETn_TRIS = 0;

#endif
    
    /*******************************************************************/    
    // Configure EEProm Pins
    /*******************************************************************/
    RF_EEnCS = 1;
    RF_EEnCS_TRIS = 0;
    EE_nCS = 1;
    EE_nCS_TRIS = 0;

    
    /*******************************************************************/
    // Configure LCD Pins
    /*******************************************************************/
    LCD_BKLT = 0;
    LCD_CS = 1;
    LCD_RS = 0;
    LCD_RESET = 0;
    LCD_BKLT_TRIS = 0;
    LCD_CS_TRIS = 0;
    LCD_RS_TRIS = 0;
    LCD_RESET_TRIS = 0;
    
    /*******************************************************************/
    // Configure SPI2
    /*******************************************************************/   
    SDI2_TRIS = 1;
    SDO2_TRIS = 0;
    SCK2_TRIS = 0;
    
    SSP2STAT = 0x00;
    SSP2CON1 = 0x31;
    
    PIR3bits.SSP2IF = 0; 

    /*******************************************************************/
    // Enable System Interupts
    /*******************************************************************/
    INTCONbits.GIEH = 1;

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
    BYTE result = SWITCH_NOT_PRESSED;
    MIWI_TICK tickDifference, t;


    if (SW1_PORT == 0)
    {
        switch0PressTime = MiWi_TickGet();
        if(SW1_PORT == 0)
        {
            while(SW1_PORT == 0);
             //get the current time
            t = MiWi_TickGet();

             //if the button has been pressed long enough
            tickDifference.Val = MiWi_TickGetDiff(t,switch0PressTime);
            if(tickDifference.Val > DEBOUNCE_TIME)
                result = SWITCH0_PRESSED;
        }    
        else
            result = SWITCH_NOT_PRESSED;
    }
    
    if (SW2_PORT == 0)
    {
        switch1PressTime = MiWi_TickGet();
        if(SW2_PORT == 0)
        {
            while(SW2_PORT == 0);
             //get the current time
            t = MiWi_TickGet();

             //if the button has been pressed long enough
            tickDifference.Val = MiWi_TickGetDiff(t,switch1PressTime);
            if(tickDifference.Val > DEBOUNCE_TIME)
                result = SWITCH1_PRESSED;
        }    
        else
            result = SWITCH_NOT_PRESSED;
    }
    
    return result;
}
  

void UserInterruptHandler(void)
{
/*    if( PIR3bits.SSP2IF )
    {
        PIR3bits.SSP2IF = 0;
        DelayMs(5);
    }  */ 
}
