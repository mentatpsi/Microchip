/*********************************************************************
 *
 *	Hardware specific definitions
 *
 *********************************************************************
 * FileName:        HardwareProfile.h
 * Dependencies:    None
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.10 or higher
 *					Microchip C30 v3.12 or higher
 *					Microchip C18 v3.34 or higher
 *					HI-TECH PICC-18 PRO 9.63PL2 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2010 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product ("Device") which is
 *      integrated into Licensee's product; or
 * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
 *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device
 *		used in conjunction with a Microchip ethernet controller for
 *		the sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this
 * Software for additional information regarding your rights and
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Chris Smith		09/30/11	Original
 ********************************************************************/
#ifndef __HARDWARE_PROFILE_H
#define __HARDWARE_PROFILE_H

#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "ConfigApp.h"


#define CLOCK_FREQ      40000000ul

// PIC32MX processor
#define GetSystemClock()		(CLOCK_FREQ)      // Hz
#define GetInstructionClock()	(GetSystemClock()/1)
#define GetPeripheralClock()	(GetInstructionClock()/1)	// Set your divider according to your Peripheral Bus Frequency configuration fuse setting


// Hardware mappings
//----------------------------
// LED and Button I/O pins
//----------------------------
#define LED0                LATEbits.LATE5
#define LED0_TRIS           TRISEbits.TRISE5
#define LED1                LATEbits.LATE6
#define LED1_TRIS           TRISEbits.TRISE6
#define LED2                LATEbits.LATE7
#define LED2_TRIS           TRISEbits.TRISE7

#define LEDS_OFF()          {LED0_OFF(); LED1_OFF(); LED2_OFF();}
#define LEDS_ON()           {LED0_ON(); LED1_ON(); LED2_ON();}

#define LED0_OFF()          {LATECLR = BIT_5;}
#define LED0_ON()           {LATESET = BIT_5;}
#define LED0_INV()          LATEINV = BIT_5;
#define LED1_OFF()          {LATECLR = BIT_6;}
#define LED1_ON()           {LATESET = BIT_6;}
#define LED1_INV()          LATEINV = BIT_6;
#define LED2_OFF()          {LATECLR = BIT_7;}
#define LED2_ON()           {LATESET = BIT_7;}
#define LED2_INV()          LATEINV = BIT_7;

#define BUTTON0_TRIS        (TRISDbits.TRISD6)	// Ref SW0
#define	BUTTON0_IO          (PORTDbits.RD6)
#define BUTTON1_TRIS        (TRISDbits.TRISD5)	// Ref SW1
#define	BUTTON1_IO          (PORTDbits.RD5)
#define BUTTON2_TRIS        (TRISDbits.TRISD4)	// Ref SW2
#define	BUTTON2_IO          (PORTDbits.RD4)

// LCD Pin Definitions
#define LCD_CS_TRIS         TRISEbits.TRISE0
#define LCD_CS              LATEbits.LATE0
#define LCD_RS_TRIS         TRISEbits.TRISE1
#define LCD_RS              LATEbits.LATE1
#define LCD_RESET_TRIS      TRISEbits.TRISE2
#define LCD_RESET           LATEbits.LATE2
#define LCD_BKLT_TRIS       TRISEbits.TRISE3
#define LCD_BKLT            LATEbits.LATE3

#define LCD_SPI_BRG         (SPI3BRG)
#define LCD_SPICON1         (SPI3CON)
#define LCD_SPICON1bits     (SPI3CONbits)
#define LCD_MAX_SPI_FREQ    (100000ul)	// Hz
// 25LC256 I/O pins
#define EEPROM_CS_TRIS		(TRISBbits.TRISB14)
#define EEPROM_CS_IO		(LATBbits.LATB14)
#define EEPROM_SCK_TRIS		(TRISDbits.TRISD10)
#define EEPROM_SDI_TRIS		(TRISCbits.TRISC4)
#define EEPROM_SDO_TRIS		(TRISDbits.TRISD0)
#define EEPROM_SPI_IF		(IFS0bits.SPI1RXIF)
#define EEPROM_SSPBUF		(SPI1BUF)
#define EEPROM_SPICON1		(SPI1CON)
#define EEPROM_SPICON1bits	(SPI1CONbits)
#define EEPROM_SPIBRG		(SPI1BRG)
#define EEPROM_SPISTAT		(SPI1STAT)
#define EEPROM_SPISTATbits	(SPI1STATbits)

//Analog POTENTIOMETER
#define ANALOG_POT_TRIS      (TRISBbits.TRISB11)
#define ANALOG_POT_IO        (LATBbits.LATB11)

//Temp Sensor
#define ANALOG_TEMP_TRIS    (TRISBbits.TRISB10)
#define ANALOG_TEMP_IO      (LATBbits.LATB10)

//SPI Flash
#define SPIFLASH_CS_TRIS       (TRISDbits.TRISD14)
#define SPIFLASH_CS_IO         (LATDbits.LATD14)
#define SPIFLASH_SCK_TRIS      (TRISDbits.TRISD15)
#define SPIFLASH_SDI_TRIS      (TRISFbits.TRISF2)
#define SPIFLASH_SDO_TRIS      (TRISFbits.TRISF8)
#define SPIFLASH_SPI_IF        (IFS0bits.SPI3RXIF)
#define SPIFLASH_SSPBUF        (SPI3BUF)
#define SPIFLASH_SPICON1       (SPI3CON)
#define SPIFLASH_SPICON1bits   (SPI3CONbits)
#define SPIFLASH_SPIBRG        (SPI3BRG)
#define SPIFLASH_SPISTAT       (SPI3STAT)
#define SPIFLASH_SPISTATbits   (SPI3STATbits)
#define FLASH_MAX_SPI_FREQ        (10000000ul)	// Hz
//----------------------------
// MRF24WB0M WiFi I/O pins
//----------------------------
#define MRF24W_IN_SPI1
//#define MRF24W_IN_SPI2

//-----------------------------
//UART I/0
//-----------------------------
#define TX_UART  UART2

// MRF24WG0M in SPI2
#define WF_CS_TRIS		(TRISGbits.TRISG9)
#define WF_CS_IO		(LATGbits.LATG9)
#define WF_SDI_TRIS		(TRISGbits.TRISG7)
#define WF_SCK_TRIS		(TRISGbits.TRISG6)
#define WF_SDO_TRIS		(TRISGbits.TRISG8)
#define WF_RESET_TRIS		(TRISAbits.TRISA4)
#define WF_RESET_IO		(LATAbits.LATA4)
        
#define WF_INT_TRIS	        (TRISEbits.TRISE8)  // INT0
#define WF_INT_IO		(PORTEbits.RE8)

#define WF_HIBERNATE_TRIS	(TRISAbits.TRISA5)
#define	WF_HIBERNATE_IO		(PORTAbits.RA5)

#define WF_INT_EDGE		(INTCONbits.INT1EP)
#define WF_INT_IE		(IEC0bits.INT1IE)
#define WF_INT_IF		(IFS0bits.INT1IF)
#define WF_INT_IE_CLEAR         IEC0CLR
#define WF_INT_IF_CLEAR         IFS0CLR
#define WF_INT_IE_SET           IEC0SET
#define WF_INT_IF_SET           IFS0SET
#define WF_INT_BIT              0x00000080
#define WF_INT_IPCSET           IPC1SET
#define WF_INT_IPCCLR           IPC1CLR
#define WF_INT_IPC_MASK         0xFF000000
#define WF_INT_IPC_VALUE        0x0C000000

#define WF_SSPBUF		(SPI2BUF)
#define WF_SPISTAT		(SPI2STAT)
#define WF_SPISTATbits		(SPI2STATbits)


#define WF_SPICON1		(SPI2CON)
#define WF_SPICON1bits		(SPI2CONbits)
#define WF_SPI_IE_CLEAR         IEC1CLR
#define WF_SPI_IF_CLEAR         IFS1CLR
#define WF_SPI_INT_BITS         0x000000e0
#define WF_SPI_BRG		(SPI2BRG)
#define WF_MAX_SPI_FREQ         (10000000ul)	// Hz

//----------------------------
// MIWI I/O Pins
//----------------------------
// There are three ways to use NVM to store data: External EPROM, Data EEPROM and
// programming space, with following definitions:
//  #define USE_EXTERNAL_EEPROM
//  #define USE_DATA_EEPROM
//  #define USE_PROGRAMMING_SPACE
// Each demo board has defined the method of using NVM, as
// required by Network Freezer feature.
#if !defined(MRF89XA)
    #define RFIF            IFS0bits.INT2IF
    #define RFIE            IEC0bits.INT2IE
#endif

#if defined(MRF89XA)
    #define IRQ1_INT_PIN      PORTEbits.RE9
    #define IRQ1_INT_TRIS     TRISEbits.TRISE9
    #define IRQ0_INT_PIN      PORTAbits.RA14
    #define IRQ0_INT_TRIS     TRISAbits.TRISA14
#else
    #define RF_INT_PIN      PORTEbits.RE9
    #define RF_INT_TRIS     TRISEbits.TRISE9
#endif

//#define USE_EXTERNAL_EEPROM

// Transceiver Configuration
#if (defined(MRF24J40) || defined(MRF49XA))
    #define PHY_CS              LATDbits.LATD9
    #define PHY_CS_TRIS         TRISDbits.TRISD9
    #define PHY_RESETn          LATDbits.LATD11
    #define PHY_RESETn_TRIS     TRISDbits.TRISD11
#endif

#define SPI_SDI             PORTCbits.RC4
#define SDI_TRIS            TRISCbits.TRISC4
#define SPI_SDO             LATDbits.LATD0
#define SDO_TRIS            TRISDbits.TRISD0
#define SPI_SCK             LATDbits.LATD10
#define SCK_TRIS            TRISDbits.TRISD10

#if defined(MRF49XA)
    #define nFSEL           LATBbits.LATB1
    #define nFSEL_TRIS      TRISBbits.TRISB1
    #define FINT		    PORTEbits.RE9
    #define FINT_TRIS       TRISEbits.TRISE9
#elif defined(MRF24J40)
    #define PHY_WAKE        LATBbits.LATB9
    #define PHY_WAKE_TRIS   TRISBbits.TRISB9
#elif defined(MRF89XA)
    #define PHY_IRQ1        IFS0bits.INT3IF
    #define PHY_IRQ1_En     IEC0bits.INT3IE
    #define PHY_IRQ0		IFS0bits.INT2IF
    #define PHY_IRQ0_En		IEC0bits.INT2IE
    #define PHY_IRQ1_TRIS   TRISAbits.TRISA14
    #define Config_nCS      LATBbits.LATB1
    #define Config_nCS_TRIS TRISBbits.TRISB1
    #define Data_nCS        LATBbits.LATB2
    #define Data_nCS_TRIS   TRISBbits.TRISB2
    #define PHY_RESETn      LATGbits.LATG2
    #define PHY_RESETn_TRIS TRISGbits.TRISG2
#endif

// Define SUPPORT_TWO_SPI if external EEPROM use the second SPI
// port alone, not sharing SPI port with the transceiver
#define SUPPORT_TWO_SPI

// External EEPROM SPI chip select pin definition
#define EE_nCS_TRIS         TRISBbits.TRISB14
#define EE_nCS              LATBbits.LATB14

#define TMRL                TMR2

//----------------------------
// UART I/O pins
//----------------------------
// Select which UART the STACK_USE_UART and STACK_USE_UART2TCP_BRIDGE 
// options will use.  You can change these to U1BRG, U1MODE, etc. if you 
// want to use the UART1 module instead of UART2.
#define UBRG					U2BRG
#define UMODE					U2MODE
#define USTA					U2STA
#define BusyUART()				BusyUART2()
#define CloseUART()				CloseUART2()
#define ConfigIntUART(a)		ConfigIntUART2(a)
#define DataRdyUART()			DataRdyUART2()
#define OpenUART(a,b,c)			OpenUART2(a,b,c)
#define ReadUART()				ReadUART2()
#define WriteUART(a)			WriteUART2(a)
#define getsUART(a,b,c)			getsUART2(a,b,c)

#define putsUART(a)			    Nop()
#define getcUART()				getcUART2()
#define putcUART(a)				do{while(BusyUART()); WriteUART(a); while(BusyUART()); }while(0)
#define putrsUART(a)			Nop()

//#define UARTTX_TRIS			(TRISFbits.TRISF5)
//#define UARTTX_IO			(PORTFbits.RF5)
//#define UARTRX_TRIS			(TRISFbits.TRISF4)
//#define UARTRX_IO			(PORTFbits.RF4)

#endif
