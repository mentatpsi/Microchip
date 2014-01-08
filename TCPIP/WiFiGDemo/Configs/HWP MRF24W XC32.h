 /*********************************************************************
 *
 *	Hardware specific definitions for:
 *    -  WiFi G Demo Board
 *          ~ PIC32MX695F512H
 *          ~ MRF24WG0MA 
 *
 *********************************************************************
 * FileName:           HWP MRF24W XC32.h
 * Dependencies:    Compiler.h
 * Processor:          PIC32MX695F512H
 * Compiler:           Microchip  XC32 Compiler
 * Company:          Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2013 Microchip Technology Inc.  All rights
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
 * Amy Ong                         10/22/2012    Created for WiFi G Demo Board to conform to existing MLA file structures
 ********************************************************************/

#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

// Set configuration fuses (but only in MainDemo.c where THIS_IS_STACK_APPLICATION is defined)
#if defined(THIS_IS_STACK_APPLICATION)
	// DEVCFG0.CP              Code-protect bit
	// DEVCFG1.FNOSC        Oscillator selection bits
	// DEVCFG1.FSOSCEN    Secondary oscillator enable bit (Disabled)
	// DEVCFG1.POSCMOD   Primary oscillator configuration bits (Disabled)
	// DEVCFG1.FPBDIV       Peripheral bus clock divisor 
	// DEVCFG1.FWDTEN      Watchdog timer enable bit
	// DEVCFG2.FPLLIDIV     PLL input divider bits
	// DEVCFG2.FPLLMUL      PLL multiplier bits
	// DEVCFG2.FPLLODIV    Default postscalar for PLL bits
	#pragma config FNOSC = FRCPLL, FPLLIDIV = DIV_2, FPLLMUL = MUL_20, FPLLODIV = DIV_2, FPBDIV = DIV_1, FWDTEN = OFF, POSCMOD = OFF, FSOSCEN = OFF, CP = OFF
#endif

#define GetSystemClock()		(40000000ul)                // 8MHz/FPLLIDIV*FPLLMUL/FPLLODIV 
#define GetInstructionClock()	(GetSystemClock()/1)
#define GetPeripheralClock()	(GetInstructionClock()/1)	// Set your divider according to your Peripheral Bus Frequency configuration fuse setting


// ====================================================
//   Hardware mappings
// ====================================================


//----------------------------
// LED and Button I/O pins
//----------------------------
#define LED0_TRIS			(TRISEbits.TRISE0)	// Ref D10 Green
#define LED0_IO				(LATEbits.LATE0)
#define LED1_TRIS			(TRISFbits.TRISF1)	// Ref D9 Yellow
#define LED1_IO				(LATFbits.LATF1)
#define LED2_TRIS			(TRISFbits.TRISF0)	// Ref D8 Red
#define LED2_IO				(LATFbits.LATF0)

#define LEDS_ON()           {LED0_ON(); LED1_ON(); LED2_ON();}
#define LEDS_OFF()          {LED0_OFF(); LED1_OFF(); LED2_OFF();}
#define LED0_ON()           LATESET = BIT_0;
#define LED0_OFF()          LATECLR = BIT_0;
#define LED0_INV()          LATEINV = BIT_0;

#define LED1_ON()           LATFSET = BIT_1;
#define LED1_OFF()          LATFCLR = BIT_1;
#define LED1_INV()          LATFINV = BIT_1;

#define LED2_ON()           LATFSET = BIT_0;
#define LED2_OFF()          LATFCLR = BIT_0;
#define LED2_INV()          LATFINV = BIT_0;

#define SW0_TRIS            (TRISDbits.TRISD9)
#define	SW0_IO              (PORTDbits.RD9)

#define VBAT_TRIS           (TRISBbits.TRISB0)     // Battery level ADC input

// Added to support EZ_CONFIG_STORE  
// 25LC256 I/O pins
//#define EEPROM_CS_TRIS		(TRISDbits.TRISD12)
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
// MRF24WG0MA/B WiFi I/O pins
//----------------------------

#define WF_CS_TRIS			(TRISGbits.TRISG9)
#define WF_CS_IO			(LATGbits.LATG9)
#define WF_SDI_TRIS			(TRISGbits.TRISG7)
#define WF_SCK_TRIS			(TRISGbits.TRISG6)
#define WF_SDO_TRIS			(TRISGbits.TRISG8)
#define WF_RESET_TRIS		(TRISDbits.TRISD1)
#define WF_RESET_IO			(LATDbits.LATD1)
        
// NOTE: 
//   Wifi comm demo declares  WF_INT_VECTOR as   _EXTERNAL_1_VECTOR and used in WF_Eint.c.  
//   Wifi G demo adopts generic approach taken by demo/console/ezconfig by defining MRF24W_IN_SPI1.
//     _EXTERNAL_1_VECTOR  = MRF24W_IN_SPI1
#define MRF24W_IN_SPI1

#define WF_INT_TRIS	        (TRISDbits.TRISD8)  // INT1
#define WF_INT_IO		    (PORTDbits.RD8)

#define WF_HIBERNATE_TRIS	(TRISEbits.TRISE4)
#define	WF_HIBERNATE_IO		(PORTEbits.RE4)

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

//----------------------------
// UART to Telnet Mapping
//----------------------------
#define BusyUART()          (TelnetOutFree() ? 0 : (StackTask(), 1))
#define putcUART            putchar
#define putrsUART(a)        fputs((const char*)a,(FILE *)stdout)
#define putsUART(a)         fputs((const char*)a,(FILE *)stdout)
#define DataRdyUART()       TelnetInChars()
#define ReadUART()          TelnetGet()

//----------------------------
// Sensor Port Mapping
//----------------------------
#define SENSOR_UART         UART2

#endif // #ifndef HARDWARE_PROFILE_H
