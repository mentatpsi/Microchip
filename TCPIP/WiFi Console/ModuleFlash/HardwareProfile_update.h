/*********************************************************************
 *
 *	Hardware specific definitions
 *
 *********************************************************************
 * FileName:        HardwareProfile.h
 * Dependencies:    None
 * Processor:       PIC32
 * Compiler:        Microchip C32 v1.11b or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2012 Microchip Technology Inc.  All rights
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
 * Ken Hesky            01/xx/10    Added MRF24WB0M-specific features
 ********************************************************************/
#ifndef __HARDWARE_PROFILE_H
#define __HARDWARE_PROFILE_H

#include "Compiler_update.h"

// Choose which hardware profile to compile for here.  See 
// the hardware profiles below for meaning of various options.  
#define EXPLORER_16				// PIC24FJ128GA010, PIC24HJ256GP610, dsPIC33FJ256GP710, PIC32MX360F512L, PIC32MX460F512L, PIC32MX795F512L, etc. PIMs


// Clock frequency value.
// This value is used to calculate Tick Counter value
// PIC32MX processor
#define GetSystemClock()		(80000000ul)      // Hz
#define GetInstructionClock()	(GetSystemClock()/1)
#define GetPeripheralClock()	(GetInstructionClock()/1)	// Set your divider according to your Peripheral Bus Frequency configuration fuse setting

#define TICKS_PER_SECOND		((GetPeripheralClock()+128ull)/256ull)
#define TICK_SECOND				((QWORD)TICKS_PER_SECOND)

#if defined(EXPLORER_16)
// Explorer 16 + PIC32MX460F512L/PIC32MX360F512L/PIC32MX795F512L PIM + 
//               Fast 100Mbps Ethernet PICtail Plus or Ethernet PICtail Plus or MRF24WB0M WiFi PICtail Plus

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
	#define LED7_TRIS			(TRISAbits.TRISA7)	// Ref D10	// Note: This is multiplexed with BUTTON1
	#define LED7_IO				(LATAbits.LATA7)
	#define LED_GET()			(*((volatile unsigned char*)(&LATA)))
	#define LED_PUT(a)			(*((volatile unsigned char*)(&LATA)) = (a))


	#define BUTTON0_TRIS		(TRISDbits.TRISD13)	// Ref S4
	#define	BUTTON0_IO			(PORTDbits.RD13)
	#define BUTTON1_TRIS		(TRISAbits.TRISA7)	// Ref S5	// Note: This is multiplexed with LED7
	#define	BUTTON1_IO			(PORTAbits.RA7)
	#define BUTTON2_TRIS		(TRISDbits.TRISD7)	// Ref S6
	#define	BUTTON2_IO			(PORTDbits.RD7)
	#define BUTTON3_TRIS		(TRISDbits.TRISD6)	// Ref S3
	#define	BUTTON3_IO			(PORTDbits.RD6)

	#define UARTTX_TRIS			(TRISFbits.TRISF5)
	#define UARTTX_IO			(PORTFbits.RF5)
	#define UARTRX_TRIS			(TRISFbits.TRISF4)
	#define UARTRX_IO			(PORTFbits.RF4)

    #define UBRG                U2BRG
    #define UMODE				U2MODE
    #define USTA				U2STA
    #define BusyUART()			BusyUART2()
    #define CloseUART()			CloseUART2()
    #define ConfigIntUART(a)	ConfigIntUART2(a)
    #define DataRdyUART()		DataRdyUART2()
    #define OpenUART(a,b,c)		OpenUART2(a,b,c)
    #define ReadUART()			ReadUART2()
    #define WriteUART(a)		WriteUART2(a)
    #define getsUART(a,b,c)		getsUART2(a,b,c)
    #define putsUART(a)         putsUART2(a)
    #define getcUART()			getcUART2()
    #define putcUART(a)			do{while(BusyUART()); WriteUART(a); while(BusyUART()); }while(0)
    #define putrsUART(a)		putrsUART2(a)

    #define EEPROM_CS_TRIS		(TRISDbits.TRISD12)
    #define EEPROM_CS_IO		(LATDbits.LATD12)
	#define EEPROM_SCK_TRIS		(TRISGbits.TRISG6)
	#define EEPROM_SDI_TRIS		(TRISGbits.TRISG7)
	#define EEPROM_SDO_TRIS		(TRISGbits.TRISG8)

    #define EEPROM_SPI_IF		(IFS1bits.SPI2RXIF)
    #define EEPROM_SSPBUF		(SPI2BUF)
    #define EEPROM_SPICON1		(SPI2CON)
    #define EEPROM_SPICON1bits	(SPI2CONbits)
    #define EEPROM_SPIBRG		(SPI2BRG)
    #define EEPROM_SPISTAT		(SPI2STAT)
    #define EEPROM_SPISTATbits	(SPI2STATbits)

    //----------------------------
    // MRF24WB0M WiFi I/O pins
    //----------------------------
	// If you have a MRF24WB0M WiFi PICtail, you must uncomment one of 
	// these two lines to use it.  SPI1 is the top-most slot in the Explorer 16 
	// (closer to the LCD and prototyping area) while SPI2 corresponds to 
	// insertion of the PICtail into the middle of the side edge connector slot.
	#define MRF24WB0M_IN_SPI1
	//#define MRF24WB0M_IN_SPI2

    #if defined( MRF24WB0M_IN_SPI1 ) && !defined(__32MX460F512L__) && !defined(__32MX795F512L__) && !defined(__PIC24FJ256GA110__)
        // MRF24WB0M in SPI1 slot
        #define WF_CS_TRIS			(TRISBbits.TRISB2)
    	#define WF_CS_IO			(LATBbits.LATB2)
    	#define WF_SDI_TRIS			(TRISFbits.TRISF7)
    	#define WF_SCK_TRIS			(TRISFbits.TRISF6)
    	#define WF_SDO_TRIS			(TRISFbits.TRISF8)
      	#define WF_RESET_TRIS		(TRISFbits.TRISF0)
    	#define WF_RESET_IO			(LATFbits.LATF0)
  
        #define WF_INT_TRIS	    (TRISEbits.TRISE8)  // INT1
        #define WF_INT_IO		(PORTEbits.RE8)

    	#define WF_HIBERNATE_TRIS	(TRISFbits.TRISF1)
    	#define	WF_HIBERNATE_IO		(PORTFbits.RF1)
        #define WF_INT_EDGE		    (INTCONbits.INT1EP)
        #define WF_INT_IE			(IEC0bits.INT1IE)
        #define WF_INT_IF			(IFS0bits.INT1IF)
        #define WF_INT_IE_CLEAR     IEC0CLR
        #define WF_INT_IF_CLEAR     IFS0CLR
        #define WF_INT_IE_SET       IEC0SET
        #define WF_INT_IF_SET       IFS0SET
        #define WF_INT_BIT          0x00000080
        #define WF_INT_IPCSET       IPC1SET
        #define WF_INT_IPCCLR       IPC1CLR
        #define WF_INT_IPC_MASK     0xFF000000
        #define WF_INT_IPC_VALUE    0x0C000000

    	#define WF_SSPBUF			(SPI1BUF)
    	#define WF_SPISTAT			(SPI1STAT)
    	#define WF_SPISTATbits		(SPI1STATbits)

        #define WF_SPICON1			(SPI1CON)
        #define WF_SPICON1bits		(SPI1CONbits)
        #define WF_SPI_IE_CLEAR     IEC0CLR
        #define WF_SPI_IF_CLEAR     IFS0CLR
        #define WF_SPI_INT_BITS     0x03800000
        #define WF_SPI_BRG		    (SPI1BRG)
        #define WF_MAX_SPI_FREQ     (10000000ul)	// Hz

    #elif defined( MRF24WB0M_IN_SPI2 ) && !defined(__32MX460F512L__) && !defined(__32MX795F512L__)
        // MRF24WB0M in SPI2 slot
        #define WF_CS_TRIS			(TRISGbits.TRISG9)
    	#define WF_CS_IO			(LATGbits.LATG9)
    	#define WF_SDI_TRIS			(TRISGbits.TRISG7)
    	#define WF_SCK_TRIS			(TRISGbits.TRISG6)
    	#define WF_SDO_TRIS			(TRISGbits.TRISG8)
      	#define WF_RESET_TRIS		(TRISGbits.TRISG0)
    	#define WF_RESET_IO			(LATGbits.LATG0)

        #define WF_INT_TRIS		    (TRISAbits.TRISA14)	// INT3
        #define WF_INT_IO			(PORTAbits.RA14)

    	#define WF_HIBERNATE_TRIS	    (TRISGbits.TRISG1)
    	#define	WF_HIBERNATE_IO			(PORTGbits.RG1)

        #define WF_INT_EDGE		    (INTCONbits.INT3EP)
        #define WF_INT_IE			(IEC0bits.INT3IE)
        #define WF_INT_IF			(IFS0bits.INT3IF)
        #define WF_INT_IE_CLEAR     IEC0CLR
        #define WF_INT_IF_CLEAR     IFS0CLR
        #define WF_INT_IE_SET       IEC0SET
        #define WF_INT_IF_SET       IFS0SET
        #define WF_INT_BIT          0x00008000
        #define WF_INT_IPCSET       IPC3SET
        #define WF_INT_IPCCLR       IPC3CLR
        #define WF_INT_IPC_MASK     0xFF000000
        #define WF_INT_IPC_VALUE    0x0C000000

    	#define WF_SSPBUF			(SPI2BUF)
    	#define WF_SPISTAT			(SPI2STAT)
    	#define WF_SPISTATbits		(SPI2STATbits)

        #define WF_SPICON1			(SPI2CON)
        #define WF_SPICON1bits		(SPI2CONbits)
        #define WF_SPI_IE_CLEAR     IEC1CLR
        #define WF_SPI_IF_CLEAR     IFS1CLR
        #define WF_SPI_INT_BITS     0x000000e0
        #define WF_SPI_BRG		    (SPI2BRG)
        #define WF_MAX_SPI_FREQ     (10000000ul)	// Hz

	#elif defined( MRF24WB0M_IN_SPI1 ) && (defined(__32MX460F512L__) || defined(__32MX795F512L__))
        // MRF24WB0M in SPI1 slot
        #define WF_CS_TRIS			(TRISDbits.TRISD9)
    	#define WF_CS_IO			(LATDbits.LATD9)
    	#define WF_SDI_TRIS			(TRISCbits.TRISC4)
    	#define WF_SCK_TRIS			(TRISDbits.TRISD10)
    	#define WF_SDO_TRIS			(TRISDbits.TRISD0)
      	#define WF_RESET_TRIS		(TRISFbits.TRISF0)
    	#define WF_RESET_IO			(LATFbits.LATF0)
    	#define WF_INT_TRIS		    (TRISEbits.TRISE8)  // INT1
    	#define WF_INT_IO			(PORTEbits.RE8)
    	#define WF_HIBERNATE_TRIS	(TRISFbits.TRISF1)
    	#define WF_HIBERNATE_IO		(PORTFbits.RF1)

        #define WF_INT_EDGE		    (INTCONbits.INT1EP)
        #define WF_INT_IE			(IEC0bits.INT1IE)
        #define WF_INT_IF			(IFS0bits.INT1IF)
        #define WF_INT_IE_CLEAR     IEC0CLR
        #define WF_INT_IF_CLEAR     IFS0CLR
        #define WF_INT_IE_SET       IEC0SET
        #define WF_INT_IF_SET       IFS0SET
        #define WF_INT_BIT          0x00000080
        #define WF_INT_IPCSET       IPC1SET
        #define WF_INT_IPCCLR       IPC1CLR
        #define WF_INT_IPC_MASK     0xFF000000
        #define WF_INT_IPC_VALUE    0x0C000000

    	#define WF_SSPBUF			(SPI1BUF)
    	#define WF_SPISTAT			(SPI1STAT)
    	#define WF_SPISTATbits		(SPI1STATbits)

        #define WF_SPICON1			(SPI1CON)
        #define WF_SPICON1bits		(SPI1CONbits)
        #define WF_SPI_IE_CLEAR     IEC0CLR
        #define WF_SPI_IF_CLEAR     IFS0CLR
        #define WF_SPI_INT_BITS     0x03800000
        #define WF_SPI_BRG		    (SPI1BRG)
        #define WF_MAX_SPI_FREQ     (10000000ul)	// Hz

    #elif defined( MRF24WB0M_IN_SPI2 ) && (defined(__32MX460F512L__) || defined(__32MX795F512L__))
    	#error "/RST and /CE are on RG2 and RG3 which are multiplexed with USB D+ and D-."
    #endif

#else
	#error "Hardware profile not defined.  See available profiles in HardwareProfile.h and modify or create one."
#endif

#endif
