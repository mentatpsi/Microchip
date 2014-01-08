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
 * Copyright (C) 2002-2009 Microchip Technology Inc.  All rights
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
 * Howard Schlunder		10/03/06	Original, copied from Compiler.h
 * Ken Hesky            07/01/08    Added ZG2100-specific features
 ********************************************************************/
#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

#include "GenericTypeDefs.h"
#include "Compiler.h"




#if defined(__PIC24FJ128GA010__) 
	#define USE_SD_INTERFACE_WITH_SPI

#elif defined(__PIC24FJ256GB110__)
	#define USE_USB_INTERFACE

#else
    #error Processor not supported.
#endif

#define EXPLORER_16				// PIC24FJ128GA010, PIC24FJ256GB110


// Set configuration fuses (but only once)
#if defined(THIS_IS_STACK_APPLICATION)

	#if defined(__PIC24F__)
        #if defined(__PIC24FJ256GB110__)
            _CONFIG2(FNOSC_PRIPLL & POSCMOD_HS & PLL_96MHZ_ON & PLLDIV_DIV2) // Primary HS OSC with PLL, USBPLL /2
            _CONFIG1(JTAGEN_OFF & FWDTEN_OFF & ICS_PGx2)   // JTAG off, watchdog timer off
 
        #else
        // Explorer 16 board
        _CONFIG2(FNOSC_PRIPLL & POSCMOD_XT)     // Primary XT OSC with 4x PLL
        _CONFIG1(JTAGEN_OFF & FWDTEN_OFF)       // JTAG off, watchdog timer off
        #endif
   
	#endif
#endif // Prevent more than one set of config fuse definitions

// Clock frequency value.
// This value is used to calculate Tick Counter value
#if defined(__PIC24F__)	
	// PIC24F processor
	#define GetSystemClock()		(32000000ul)      // Hz
	#define GetInstructionClock()	(GetSystemClock()/2)
	#define GetPeripheralClock()	GetInstructionClock()

#endif

// Hardware mappings
#if defined(EXPLORER_16)
// Explorer 16 + PIC24FJ128GA010

	#define LED0_TRIS			(TRISAbits.TRISA0)	// Ref D3
	#define LED0_IO				(LATAbits.LATA0)	
	#define LED1_TRIS			(TRISAbits.TRISA1)	// Ref D4
	#define LED1_IO				(LATAbits.LATA1)
	#define LED2_TRIS			(TRISAbits.TRISA2)	// Ref D5
	#define LED2_IO				(LATAbits.LATA2)
	#define LED3_TRIS			(TRISAbits.TRISA3)	// Ref D6
	#define LED3_IO				(LATAbits.LATA3)
	#define LED4_TRIS			(TRISAbits.TRISA4)	// Ref D7
	#define LED4_IO				(LATAbits.LATA4)
	#define LED5_TRIS			(TRISAbits.TRISA5)	// Ref D8
	#define LED5_IO				(LATAbits.LATA5)
	#define LED6_TRIS			(TRISAbits.TRISA6)	// Ref D9
	#define LED6_IO				(LATAbits.LATA6)
	#define LED7_TRIS			(LATAbits.LATA7)	// Ref D10;  Note: This is multiplexed with BUTTON1, so this LED can't be used.  However, it will glow very dimmly due to a weak pull up resistor.
	#define LED7_IO				(LATAbits.LATA7)
	#define LED_GET()			(*((volatile unsigned char*)(&LATA)))
	#define LED_PUT(a)			(*((volatile unsigned char*)(&LATA)) = (a))


	#define BUTTON0_TRIS		(TRISDbits.TRISD13)	// Ref S4
	#define	BUTTON0_IO			(PORTDbits.RD13)
	#define BUTTON1_TRIS		(TRISAbits.TRISA7)	// Ref S5;  Note: This is multiplexed with LED7
	#define	BUTTON1_IO			(PORTAbits.RA7)
	#define BUTTON2_TRIS		(TRISDbits.TRISD7)	// Ref S6
	#define	BUTTON2_IO			(PORTDbits.RD7)
	#define BUTTON3_TRIS		(TRISDbits.TRISD6)	// Ref S3
	#define	BUTTON3_IO			(PORTDbits.RD6)

	#define UARTTX_TRIS			(TRISFbits.TRISF5)
	#define UARTTX_IO			(PORTFbits.RF5)
	#define UARTRX_TRIS			(TRISFbits.TRISF4)
	#define UARTRX_IO			(PORTFbits.RF4)

	// ENC28J60 I/O pins
	#define ENC_CS_TRIS			(TRISDbits.TRISD14)	// Comment this line out if you are using the ENC424J600/624J600, MRF24WB0M, or other network controller.
	#define ENC_CS_IO			(LATDbits.LATD14)
	//#define ENC_RST_TRIS		(TRISDbits.TRISD15)	// Not connected by default.  It is okay to leave this pin completely unconnected, in which case this macro should simply be left undefined.
	//#define ENC_RST_IO			(PORTDbits.RD15)
	// SPI SCK, SDI, SDO pins are automatically controlled by the 
	// PIC24/dsPIC/PIC32 SPI module 
	#if defined(__C30__)	// PIC24F, PIC24H, dsPIC30, dsPIC33
		#define ENC_SPI_IF			(IFS0bits.SPI1IF)
		#define ENC_SSPBUF			(SPI1BUF)
		#define ENC_SPISTAT			(SPI1STAT)
		#define ENC_SPISTATbits		(SPI1STATbits)
		#define ENC_SPICON1			(SPI1CON1)
		#define ENC_SPICON1bits		(SPI1CON1bits)
		#define ENC_SPICON2			(SPI1CON2)
	#endif



	
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
	#define putsUART(a)				putsUART2((unsigned int*)a)
	#define getcUART()				getcUART2()
	#define putcUART(a)				do{while(BusyUART()); WriteUART(a); while(BusyUART()); }while(0)
	#define putrsUART(a)			putrsUART2(a)

		// LCD Module I/O pins.  NOTE: On the Explorer 16, the LCD is wired to the 
	// same PMP lines required to communicate with an ENCX24J600 in parallel 
	// mode.  Since the LCD does not have a chip select wire, if you are using 
	// the ENC424J600/624J600 in parallel mode, the LCD cannot be used.
	#if !defined(ENC100_INTERFACE_MODE) || (ENC100_INTERFACE_MODE == 0)	// SPI only
		#define LCD_DATA_TRIS		(*((volatile BYTE*)&TRISE))
		#define LCD_DATA_IO			(*((volatile BYTE*)&LATE))
		#define LCD_RD_WR_TRIS		(TRISDbits.TRISD5)
		#define LCD_RD_WR_IO		(LATDbits.LATD5)
		#define LCD_RS_TRIS			(TRISBbits.TRISB15)
		#define LCD_RS_IO			(LATBbits.LATB15)
		#define LCD_E_TRIS			(TRISDbits.TRISD4)
		#define LCD_E_IO			(LATDbits.LATD4)
	#endif


#endif

/*********************************************************************/
/******************* Pin and Register Definitions ********************/
/*********************************************************************/

/* SD Card definitions: Change these to fit your application when using
   an SD-card-based physical layer                                   */

#ifdef USE_SD_INTERFACE_WITH_SPI
     
    #if defined __PIC24F__

				// Description: SD-SPI Chip Select Output bit
        #define SD_CS               PORTBbits.RB9
				// Description: SD-SPI Chip Select TRIS bit
        #define SD_CS_TRIS          TRISBbits.TRISB9
				
				// Description: SD-SPI Card Detect Input bit
        #define SD_CD               PORTGbits.RG0
				// Description: SD-SPI Card Detect TRIS bit
        #define SD_CD_TRIS          TRISGbits.TRISG0
				
				// Description: SD-SPI Write Protect Check Input bit
        #define SD_WE               PORTGbits.RG1
				// Description: SD-SPI Write Protect Check TRIS bit
        #define SD_WE_TRIS          TRISGbits.TRISG1
		
				// Registers for the SPI module you want to use
		
				// Description: The main SPI control register
        #define SPICON1             SPI2CON1
				// Description: The SPI status register
        #define SPISTAT             SPI2STAT
				// Description: The SPI Buffer
        #define SPIBUF              SPI2BUF
				// Description: The receive buffer full bit in the SPI status register
        #define SPISTAT_RBF         SPI2STATbits.SPIRBF
				// Description: The bitwise define for the SPI control register (i.e. _____bits)
        #define SPICON1bits         SPI2CON1bits
				// Description: The bitwise define for the SPI status register (i.e. _____bits)
        #define SPISTATbits         SPI2STATbits
				// Description: The enable bit for the SPI module
        #define SPIENABLE           SPISTATbits.SPIEN
		
				// Tris pins for SCK/SDI/SDO lines
		
				// Description: The TRIS bit for the SCK pin
        #define SPICLOCK            TRISGbits.TRISG6
				// Description: The TRIS bit for the SDI pin
        #define SPIIN               TRISGbits.TRISG7
				// Description: The TRIS bit for the SDO pin
        #define SPIOUT              TRISGbits.TRISG8



        // Will generate an error if the clock speed is too low to interface to the card
        #if (GetSystemClock() < 100000)
            #error Clock speed must exceed 100 kHz
        #endif    
     
    
    #endif

#endif



// Macros for input and output TRIS bits
#define INPUT_PIN   1
#define OUTPUT_PIN  0

#endif


