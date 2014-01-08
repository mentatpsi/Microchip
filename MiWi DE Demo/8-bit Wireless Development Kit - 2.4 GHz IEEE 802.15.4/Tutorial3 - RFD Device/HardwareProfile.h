/********************************************************************
* FileName:		HardwareProfile.h
* Dependencies:    
* Processor:	PIC18, PIC24, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher	
*               Microchip C32 v1.02 or higher	
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
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
*
*********************************************************************
* File Description:
*
*  This file defines functions used for demo board hardware
*
* Change History:
*  Rev   Date         Author    Description
*  1.0   2/15/2009    yfy       Initial revision
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
********************************************************************/
#ifndef _HARDWARE_PROFILE_H
    #define _HARDWARE_PROFILE_H

    #include "GenericTypeDefs.h"
    #include "ConfigApp.h"
    

    #define EIGHT_BIT_WIRELESS_BOARD    
    
    #define CLOCK_FREQ  8000000
    #define USE_EXTERNAL_EEPROM

// Define EEPROM_SHARE_SPI if external EEPROM shares the same
// SPI with transceiver
    #define EEPROM_SHARE_SPI

// 8-bit wireless demo board can use the sensor port to
// connect to either a LCD or a RS232 serial port
    #define SENSOR_PORT_LCD
    #define SENSOR_PORT_UART
    #define ENABLE_MANUALBACKLIGHT

    #if defined(MRF49XA)
        #define RFIF            INTCON3bits.INT3IF
        #define RFIE            INTCON3bits.INT3IE
        #define PHY_CS          LATCbits.LATC2
        #define PHY_CS_TRIS     TRISCbits.TRISC2
        #define RF_INT_PIN      PORTBbits.RB3
        #define RF_INT_TRIS     TRISBbits.TRISB3
        #define nFSEL           LATEbits.LATE1
        #define nFSEL_TRIS      TRISEbits.TRISE1
        #define FINT            PORTDbits.RD3
        #define FINT_TRIS       TRISDbits.TRISD3
        #define RF_EEnCS_TRIS   TRISEbits.TRISE2
        #define RF_EEnCS        LATEbits.LATE2
    #elif defined(MRF24J40)
        #define RFIF            INTCON3bits.INT1IF
        #define RFIE            INTCON3bits.INT1IE
        #define PHY_CS          LATBbits.LATB3
        #define PHY_CS_TRIS     TRISBbits.TRISB3
        #define RF_INT_PIN      PORTBbits.RB1
        #define RF_INT_TRIS     TRISBbits.TRISB1
        #define PHY_WAKE        LATDbits.LATD3
        #define PHY_WAKE_TRIS   TRISDbits.TRISD3
        #define RF_EEnCS_TRIS   TRISCbits.TRISC2
        #define RF_EEnCS        LATCbits.LATC2
    #elif defined(MRF89XA)
		#define IRQ1_INT_PIN	PORTBbits.RB3
		#define IRQ1_INT_TRIS	TRISBbits.TRISB3
		#define PHY_IRQ1        INTCON3bits.INT3IF
		#define PHY_IRQ1_En     INTCON3bits.INT3IE
		#define Config_nCS      LATEbits.LATE1
	    #define Config_nCS_TRIS TRISEbits.TRISE1
		#define Data_nCS        LATCbits.LATC2
		#define Data_nCS_TRIS   TRISCbits.TRISC2
		#define IRQ0_INT_PIN    PORTDbits.RD3
		#define IRQ0_INT_TRIS   TRISDbits.TRISD3
		#define RF_EEnCS_TRIS	TRISEbits.TRISE0
		#define RF_EEnCS		LATEbits.LATE0
		#define PHY_RESETn      LATDbits.LATB7
		#define PHY_RESETn_TRIS TRISDbits.TRISD7
		//If using both IRQ0 and IRQ1 then uncomment the following lines
		//Also define the PHY_IRQ0 connections
		//#define PHY_IRQ0		
		//#define PHY_IRQ0_En		
		//#define USE_IRQ0_AS_INTERRUPT in ConfigMRF89XA.h
    #endif
    
    #define SPI_SDI     PORTBbits.RB5
    #define SDI_TRIS    TRISBbits.TRISB5
    #define SPI_SDO     LATCbits.LATC7
    #define SDO_TRIS    TRISCbits.TRISC7
    #define SPI_SCK     LATBbits.LATB4
    #define SCK_TRIS    TRISBbits.TRISB4

    #if defined(MRF24J40) || defined(MRF49XA)
        #define PHY_RESETn      LATDbits.LATD7
        #define PHY_RESETn_TRIS TRISDbits.TRISD7
    #endif
    
    #define PUSH_BUTTON_RB0       PORTBbits.RB0
    #define BUTTON_RB0_TRIS       TRISBbits.TRISB0
    #define PUSH_BUTTON_RB2       PORTBbits.RB2
    #define BUTTON_RB2_TRIS       TRISBbits.TRISB2

    #define LED_1           LATAbits.LATA2
    #define LED_1_TRIS      TRISAbits.TRISA2
    #define LED_2           LATAbits.LATA3
    #define LED_2_TRIS      TRISAbits.TRISA3

    #define EE_nCS_TRIS     TRISDbits.TRISD5
    #define EE_nCS          LATDbits.LATD5
    #define TMRL            TMR0L

    #define SUPPORT_TWO_SPI
    #define SPInCS_TRIS TRISCbits.TRISC6
    #define SPInCS_LAT  LATCbits.LATC6
    #define SPISSPSTAT  SSP2STAT
    #define SPISSPCON   SSP2CON1
    #define SPISSPBUF   SSP2BUF
    #define SPIWCOL     SSP2CON1bits.WCOL
    #define SPISSPIE    PIE3bits.SSP2IE
    #define SPISSPIF    PIR3bits.SSP2IF
    

 // Following definition is for delay functionality
  
    #define GetInstructionClock()	(CLOCK_FREQ/4)
    

void    BoardInit(void);
void    LCDDisplay(char *, BYTE, BOOL);
void    ReadMacAddress(void);
void    ConfigADC(void);
void    NetworkSetup(void);
void    Read_VBGVoltage(void);
void    ReadTempSensor(void);
void    TransmitMessage(void);   
void    ProcessRxMessage(void);
void    PrintTempLCD(void);
#endif
