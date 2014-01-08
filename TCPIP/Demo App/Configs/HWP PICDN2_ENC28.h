 /*********************************************************************
 *
 *	Hardware specific definitions for:
 *    - PICDEM.net 2
 *    - PIC18F97J60
 *    - Onboard ENC28J60 (J2 jack)
 *
 *********************************************************************
 * FileName:        HardwareProfile.h
 * Dependencies:    Compiler.h
 * Processor:       PIC18
 * Compiler:        Microchip C18 v3.36 or higher
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
 * Howard Schlunder		09/16/2010	Regenerated for specific boards
 ********************************************************************/
#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

#include "Compiler.h"

// Define a macro describing this hardware set up (used in other files)
#define PICDEMNET2

// Set configuration fuses (but only in MainDemo.c where THIS_IS_STACK_APPLICATION is defined)
#if defined(THIS_IS_STACK_APPLICATION)
	#pragma config WDT=OFF, FOSC2=ON, FOSC=HSPLL, ETHLED=ON

	// Automatically set Extended Instruction Set fuse based on compiler setting
	#if defined(__EXTENDED18__)
		#pragma config XINST=ON
	#else
		#pragma config XINST=OFF
	#endif
#endif


// Clock frequency values
// These directly influence timed events using the Tick module.  They also are used for UART and SPI baud rate generation.
#define GetSystemClock()		(41666667ul)			// Hz
#define GetInstructionClock()	(GetSystemClock()/4)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Might need changing if using Doze modes.
#define GetPeripheralClock()	(GetSystemClock()/4)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Divisor may be different if using a PIC32 since it's configurable.


// Hardware I/O pin mappings

// LEDs
#define LED0_TRIS			(TRISJbits.TRISJ0)	// Ref D8
#define LED0_IO				(LATJbits.LATJ0)
#define LED1_TRIS			(TRISJbits.TRISJ1)	// Ref D7
#define LED1_IO				(LATJbits.LATJ1)
#define LED2_TRIS			(TRISJbits.TRISJ2)	// Ref D6
#define LED2_IO				(LATJbits.LATJ2)
#define LED3_TRIS			(TRISJbits.TRISJ3)	// Ref D5
#define LED3_IO				(LATJbits.LATJ3)
#define LED4_TRIS			(TRISJbits.TRISJ4)	// Ref D4
#define LED4_IO				(LATJbits.LATJ4)
#define LED5_TRIS			(TRISJbits.TRISJ5)	// Ref D3
#define LED5_IO				(LATJbits.LATJ5)
#define LED6_TRIS			(TRISJbits.TRISJ6)	// Ref D2
#define LED6_IO				(LATJbits.LATJ6)
#define LED7_TRIS			(TRISJbits.TRISJ7)	// Ref D1
#define LED7_IO				(LATJbits.LATJ7)
#define LED_GET()			(LATJ)
#define LED_PUT(a)			(LATJ = (a))

// Momentary push buttons
#define BUTTON0_TRIS		(TRISBbits.TRISB3)	// Ref S5
#define	BUTTON0_IO			(PORTBbits.RB3)
#define BUTTON1_TRIS		(TRISBbits.TRISB2)	// Ref S4
#define	BUTTON1_IO			(PORTBbits.RB2)
#define BUTTON2_TRIS		(TRISBbits.TRISB1)	// Ref S3
#define	BUTTON2_IO			(PORTBbits.RB1)
#define BUTTON3_TRIS		(TRISBbits.TRISB0)	// Ref S2
#define	BUTTON3_IO			(PORTBbits.RB0)

// Ethernet TPIN+/- polarity swap circuitry (PICDEM.net 2 Rev 6)
#define ETH_RX_POLARITY_SWAP_TRIS	(TRISGbits.TRISG0)
#define ETH_RX_POLARITY_SWAP_IO		(LATGbits.LATG0)

// ENC28J60 I/O pins
#define ENC_RST_TRIS		(TRISDbits.TRISD2)	// Not connected by default
#define ENC_RST_IO			(LATDbits.LATD2)
#define ENC_CS_TRIS			(TRISDbits.TRISD3)
#define ENC_CS_IO			(LATDbits.LATD3)
#define ENC_SCK_TRIS		(TRISCbits.TRISC3)
#define ENC_SDI_TRIS		(TRISCbits.TRISC4)
#define ENC_SDO_TRIS		(TRISCbits.TRISC5)
#define ENC_SPI_IF			(PIR1bits.SSP1IF)
#define ENC_SSPBUF			(SSP1BUF)
#define ENC_SPISTAT			(SSP1STAT)
#define ENC_SPISTATbits		(SSP1STATbits)
#define ENC_SPICON1			(SSP1CON1)
#define ENC_SPICON1bits		(SSP1CON1bits)
#define ENC_SPICON2			(SSP1CON2)

//// ENC424J600/624J600 Fast 100Mbps Ethernet PICtail Plus defines
//#define ENC100_INTERFACE_MODE			0	// Uncomment this to use the ENC424J600/624J600 Ethernet controller
//
//// ENC424J600/624J600 Fast 100Mbps Ethernet PICtail Plus I/O pins
//#define ENC100_MDIX_TRIS				(TRISBbits.TRISB4)
//#define ENC100_MDIX_IO					(LATBbits.LATB4)
//#define ENC100_POR_TRIS					(TRISBbits.TRISB5)
//#define ENC100_POR_IO					(LATBbits.LATB5)
//#define ENC100_INT_TRIS					(TRISBbits.TRISB2)
//#define ENC100_INT_IO					(PORTBbits.RB2)
//
//// ENC424J600/624J600 SPI pinout
//#define ENC100_CS_TRIS					(TRISBbits.TRISB3)
//#define ENC100_CS_IO					(LATBbits.LATB3)
//#define ENC100_SO_WR_B0SEL_EN_TRIS		(TRISCbits.TRISC4)	// NOTE: SO is ENC624J600 Serial Out, which needs to connect to the PIC SDI pin for SPI mode
//#define ENC100_SO_WR_B0SEL_EN_IO		(PORTCbits.RC4)
//#define ENC100_SI_RD_RW_TRIS			(TRISCbits.TRISC5)	// NOTE: SI is ENC624J600 Serial In, which needs to connect to the PIC SDO pin for SPI mode
//#define ENC100_SI_RD_RW_IO				(LATCbits.LATC5)
//#define ENC100_SCK_AL_TRIS				(TRISCbits.TRISC3)
//#define ENC100_SCK_AL_IO				(PORTCbits.RC3)		// NOTE: This must be the PORT, not the LATch like it is for the PSP interface.
//
//// ENC424J600/624J600 SPI SFR register selection (controls which SPI 
//// peripheral to use on PICs with multiple SPI peripherals).
////#define ENC100_ISR_ENABLE		(INTCON3bits.INT2IE)
////#define ENC100_ISR_FLAG			(INTCON3bits.INT2IF)
////#define ENC100_ISR_POLARITY		(INTCON2bits.INTEDG2)
////#define ENC100_ISR_PRIORITY		(INTCON3bits.INT2IP)
//#define ENC100_SPI_ENABLE		(ENC100_SPISTATbits.SPIEN)
//#define ENC100_SPI_IF			(PIR1bits.SSP1IF)
//#define ENC100_SSPBUF			(SSP1BUF)
//#define ENC100_SPISTAT			(SSP1STAT)
//#define ENC100_SPISTATbits		(SSP1STATbits)
//#define ENC100_SPICON1			(SSP1CON1)
//#define ENC100_SPICON1bits		(SSP1CON1bits)
//#define ENC100_SPICON2			(SSP1CON2)

//// MRF24WB0M Wi-Fi PICtail I/O pins
//#define WF_CS_TRIS			(TRISCbits.TRISC2)
//#define WF_SDI_TRIS			(TRISCbits.TRISC4)
//#define WF_SCK_TRIS			(TRISCbits.TRISC3)
//#define WF_SDO_TRIS			(TRISCbits.TRISC5)
//#define WF_RESET_TRIS		(TRISBbits.TRISB1)
//#define WF_RESET_IO			(LATBbits.LATB1)
//#define WF_INT_TRIS			(TRISBbits.TRISB0)
//#define WF_INT_IO			(PORTBbits.RB0)
//#define WF_CS_IO			(LATCbits.LATC2)
//#define WF_HIBERNATE_TRIS	(TRISBbits.TRISB2)
//#define	WF_HIBERNATE_IO		(PORTBbits.RB2)
//#define WF_INT_EDGE			(INTCON2bits.INTEDG0)
//#define WF_INT_IE			(INTCONbits.INT0IE)
//#define WF_INT_IF			(INTCONbits.INT0IF)
//#define WF_SPI_IF			(PIR1bits.SSPIF)
//#define WF_SSPBUF			(SSP1BUF)
//#define WF_SPISTAT			(SSP1STAT)
//#define WF_SPISTATbits		(SSP1STATbits)
//#define WF_SPICON1			(SSP1CON1)
//#define WF_SPICON1bits		(SSP1CON1bits)
//#define WF_SPICON2			(SSP1CON2)
//#define WF_SPI_IE			(PIE1bits.SSPIE)
//#define WF_SPI_IP			(IPR1bits.SSPIP)

// 25LC256 I/O pins
#define EEPROM_CS_TRIS		(TRISDbits.TRISD7)
#define EEPROM_CS_IO		(LATDbits.LATD7)
#define EEPROM_SCK_TRIS		(TRISCbits.TRISC3)
#define EEPROM_SDI_TRIS		(TRISCbits.TRISC4)
#define EEPROM_SDO_TRIS		(TRISCbits.TRISC5)
#define EEPROM_SPI_IF		(PIR1bits.SSP1IF)
#define EEPROM_SSPBUF		(SSP1BUF)
#define EEPROM_SPICON1		(SSP1CON1)
#define EEPROM_SPICON1bits	(SSP1CON1bits)
#define EEPROM_SPICON2		(SSP1CON2)
#define EEPROM_SPISTAT		(SSP1STAT)
#define EEPROM_SPISTATbits	(SSP1STATbits)

// LCD I/O pins
#define LCD_DATA_TRIS		(TRISE)
#define LCD_DATA_IO			(LATE)
#define LCD_RD_WR_TRIS		(TRISHbits.TRISH1)
#define LCD_RD_WR_IO		(LATHbits.LATH1)
#define LCD_RS_TRIS			(TRISHbits.TRISH2)
#define LCD_RS_IO			(LATHbits.LATH2)
#define LCD_E_TRIS			(TRISHbits.TRISH0)
#define LCD_E_IO			(LATHbits.LATH0)

// UART mapping functions for consistent API names across 8-bit and 16 or 
// 32 bit compilers.  For simplicity, everything will use "UART" instead 
// of USART/EUSART/etc.
#define BusyUART()			BusyUSART()
#define CloseUART()			CloseUSART()
#define ConfigIntUART(a)	ConfigIntUSART(a)
#define DataRdyUART()		DataRdyUSART()
#define OpenUART(a,b,c)		OpenUSART(a,b,c)
#define ReadUART()			ReadUSART()
#define WriteUART(a)		WriteUSART(a)
#define getsUART(a,b,c)		getsUSART(b,a)
#define putsUART(a)			putsUSART(a)
#define getcUART()			ReadUSART()
#define putcUART(a)			WriteUSART(a)
#define putrsUART(a)		putrsUSART((far rom char*)a)

#endif // #ifndef HARDWARE_PROFILE_H
