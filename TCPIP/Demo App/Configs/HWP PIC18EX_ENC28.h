 /*********************************************************************
 *
 *	Hardware specific definitions for:
 *    - PIC18 Explorer
 *    - PIC18F8722 and other PIC18s
 *    - Ethernet PICtail (ENC28J60)
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
#define PIC18_EXPLORER

// Set configuration fuses (but only in MainDemo.c where THIS_IS_STACK_APPLICATION is defined)
#if defined(THIS_IS_STACK_APPLICATION)
	#if defined(__18F8722)
		#pragma config OSC=HSPLL, FCMEN=OFF, IESO=OFF, PWRT=OFF, WDT=OFF, LVP=OFF
	#elif defined(__18F87J10)
		#pragma config WDTEN=OFF, FOSC2=ON, FOSC=HSPLL
	#elif defined(__18F87J11)
		#pragma config WDTEN=OFF, FOSC=HSPLL
	#elif defined(__18F87J50)
		#pragma config WDTEN=OFF, FOSC=HSPLL, PLLDIV=3, CPUDIV=OSC1
	#endif

	// Automatically set Extended Instruction Set fuse based on compiler setting
	#if defined(__EXTENDED18__)
		#pragma config XINST=ON
	#else
		#pragma config XINST=OFF
	#endif
#endif


// Clock frequency values
// These directly influence timed events using the Tick module.  They also are used for UART and SPI baud rate generation.
#if defined(__18F87J50) || defined(_18F87J50)
	#define GetSystemClock()	(48000000ul)			// PIC18F87J50 USB PIM has it's own 12MHz crystal on it
#else	// PIC18F8722, PIC18F87J11, other ordinary PIMs	// Uses 10MHz crystal on PIC18 Explorer
	#define GetSystemClock()	(40000000ul)			// Hz
#endif
#define GetInstructionClock()	(GetSystemClock()/4)	// Should be GetSystemClock()/4 for PIC18
#define GetPeripheralClock()	(GetSystemClock()/4)	// Should be GetSystemClock()/4 for PIC18


// Hardware I/O pin mappings

// LEDs
#define LED0_TRIS			(TRISDbits.TRISD0)	// Ref D1
#define LED0_IO				(LATDbits.LATD0)
#define LED1_TRIS			(TRISDbits.TRISD1)	// Ref D2
#define LED1_IO				(LATDbits.LATD1)
#define LED2_TRIS			(TRISDbits.TRISD2)	// Ref D3
#define LED2_IO				(LATDbits.LATD2)
#define LED3_TRIS			(TRISDbits.TRISD3)	// Ref D4
#define LED3_IO				(LATDbits.LATD3)
#define LED4_TRIS			(TRISDbits.TRISD4)	// Ref D5
#define LED4_IO				(LATDbits.LATD4)
#define LED5_TRIS			(TRISDbits.TRISD5)	// Ref D6
#define LED5_IO				(LATDbits.LATD5)
#define LED6_TRIS			(TRISDbits.TRISD6)	// Ref D7
#define LED6_IO				(LATDbits.LATD6)
#define LED7_TRIS			(TRISDbits.TRISD7)	// Ref D8
#define LED7_IO				(LATDbits.LATD7)
#define LED_GET()			(LATD)
#define LED_PUT(a)			(LATD = (a))

// Momentary push buttons
#define BUTTON0_TRIS		(TRISAbits.TRISA5)
#define	BUTTON0_IO			(PORTAbits.RA5)
#define BUTTON1_TRIS		(TRISBbits.TRISB0)
#define	BUTTON1_IO			(PORTBbits.RB0)
#define BUTTON2_TRIS		(TRISBbits.TRISB0)	// No Button2 on this board
#define	BUTTON2_IO			(1u)
#define BUTTON3_TRIS		(TRISBbits.TRISB0)	// No Button3 on this board
#define	BUTTON3_IO			(1u)

//// ENC424J600/624J600 Fast 100Mbps Ethernet PICtail Plus defines
//#define ENC100_INTERFACE_MODE			0
//
//// ENC100_MDIX, ENC100_POR, and ENC100_INT are all optional.  Simply leave 
//// them commented out if you don't have such a hardware feature on your 
//// board.
//#define ENC100_MDIX_TRIS				(TRISBbits.TRISB4)
//#define ENC100_MDIX_IO					(LATBbits.LATB4)
////#define ENC100_POR_TRIS					(TRISBbits.TRISB5)
////#define ENC100_POR_IO					(LATBbits.LATB5)
////#define ENC100_INT_TRIS					(TRISBbits.TRISB2)
////#define ENC100_INT_IO					(PORTBbits.RB2)
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
//#define ENC100_SPI_IF			(PIR1bits.SSPIF)
//#define ENC100_SSPBUF			(SSP1BUF)
//#define ENC100_SPISTAT			(SSP1STAT)
//#define ENC100_SPISTATbits		(SSP1STATbits)
//#define ENC100_SPICON1			(SSP1CON1)
//#define ENC100_SPICON1bits		(SSP1CON1bits)
//#define ENC100_SPICON2			(SSP1CON2)

// ENC28J60 I/O pins
#define ENC_RST_TRIS		(TRISBbits.TRISB5)
#define ENC_RST_IO			(LATBbits.LATB5)
#define ENC_CS_TRIS			(TRISBbits.TRISB3)
#define ENC_CS_IO			(LATBbits.LATB3)
#define ENC_SCK_TRIS		(TRISCbits.TRISC3)
#define ENC_SDI_TRIS		(TRISCbits.TRISC4)
#define ENC_SDO_TRIS		(TRISCbits.TRISC5)
#define ENC_SPI_IF			(PIR1bits.SSPIF)
#define ENC_SSPBUF			(SSP1BUF)
#define ENC_SPISTAT			(SSP1STAT)
#define ENC_SPISTATbits		(SSP1STATbits)
#define ENC_SPICON1			(SSP1CON1)
#define ENC_SPICON1bits		(SSP1CON1bits)
#define ENC_SPICON2			(SSP1CON2)

//// MRF24WB0M I/O pins
//#define WF_CS_TRIS			(TRISCbits.TRISC2)
//#define WF_SDI_TRIS			(TRISCbits.TRISC4)
//#define WF_SCK_TRIS			(TRISCbits.TRISC3)
//#define WF_SDO_TRIS			(TRISCbits.TRISC5)
//#define WF_RESET_TRIS		(TRISBbits.TRISB1)
//#define WF_RESET_IO			(LATBbits.LATB1)
//#define WF_INT_TRIS	    	(TRISBbits.TRISB0)
//#define WF_INT_IO			(PORTBbits.RB0)
//#define WF_CS_IO			(LATCbits.LATC2)
//#define WF_HIBERNATE_TRIS   (TRISBbits.TRISB2)
//#define	WF_HIBERNATE_IO 	(PORTBbits.RB2)
//#define WF_INT_EDGE		    (INTCON2bits.INTEDG0)
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
#define EEPROM_CS_TRIS		(TRISAbits.TRISA3)
#define EEPROM_CS_IO		(LATAbits.LATA3)
#define EEPROM_SCK_TRIS		(TRISCbits.TRISC3)
#define EEPROM_SDI_TRIS		(TRISCbits.TRISC4)
#define EEPROM_SDO_TRIS		(TRISCbits.TRISC5)
#define EEPROM_SPI_IF		(PIR1bits.SSPIF)
#define EEPROM_SSPBUF		(SSP1BUF)
#define EEPROM_SPICON1		(SSP1CON1)
#define EEPROM_SPICON1bits	(SSP1CON1bits)
#define EEPROM_SPICON2		(SSP1CON2)
#define EEPROM_SPISTAT		(SSP1STAT)
#define EEPROM_SPISTATbits	(SSP1STATbits)


// Register name fix up for certain processors
#define SPBRGH			SPBRGH1
#if defined(__18F87J50) || defined(_18F87J50) || defined(__18F87J11) || defined(_18F87J11)
	#define ADCON2		ADCON1
#endif

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
