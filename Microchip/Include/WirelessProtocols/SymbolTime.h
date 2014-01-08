/********************************************************************
* FileName:		SymbolTime.h
* Dependencies: ConfigApp.h
* Processor:	PIC18, PIC24F, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Hardware:		PICDEM Z, Explorer 16, PIC18 Explorer
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
*  This file provides access to all of the time managment functions
*   as well as calculating the timer scaling settings required for
*   accurate symbol time measurement
*
* Change History:
*  Rev   Date         Author    Description
*  0.1   11/09/2006   yfy       Initial revision
*  1.0   01/09/2007   yfy       Initial release
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
*  2.1   06/20/2009   yfy       Add LCD support
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/

#ifndef __SYMBOL_TIME_H_
#define __SYMBOL_TIME_H_

/************************ HEADERS **********************************/

#include "Compiler.h"
#include "GenericTypeDefs.h"

/************************ DEFINITIONS ******************************/

#if defined(__18CXX)
    /* this section is based on the Timer 0 module of the PIC18 family */

    #if(CLOCK_FREQ <= 250000)
        #define CLOCK_DIVIDER 1
        #define CLOCK_DIVIDER_SETTING 0x08 /* no prescalar */
        #define SYMBOL_TO_TICK_RATE 250000
    #elif(CLOCK_FREQ <= 500000)
        #define CLOCK_DIVIDER 2
        #define CLOCK_DIVIDER_SETTING 0x00
        #define SYMBOL_TO_TICK_RATE 500000
    #elif(CLOCK_FREQ <= 1000000)
        #define CLOCK_DIVIDER 4
        #define CLOCK_DIVIDER_SETTING 0x01
        #define SYMBOL_TO_TICK_RATE 1000000
    #elif(CLOCK_FREQ <= 2000000)
        #define CLOCK_DIVIDER 8
        #define CLOCK_DIVIDER_SETTING 0x02
        #define SYMBOL_TO_TICK_RATE 2000000
    #elif(CLOCK_FREQ <= 4000000)
        #define CLOCK_DIVIDER 16
        #define CLOCK_DIVIDER_SETTING 0x03
        #define SYMBOL_TO_TICK_RATE 4000000
    #elif(CLOCK_FREQ <= 8000000)
        #define CLOCK_DIVIDER 32
        #define CLOCK_DIVIDER_SETTING 0x04
        #define SYMBOL_TO_TICK_RATE 8000000
    #elif(CLOCK_FREQ <= 16000000)
        #define CLOCK_DIVIDER 64
        #define CLOCK_DIVIDER_SETTING 0x05
        #define SYMBOL_TO_TICK_RATE 16000000
    #elif(CLOCK_FREQ <= 32000000)
        #define CLOCK_DIVIDER 128
        #define CLOCK_DIVIDER_SETTING 0x06
        #define SYMBOL_TO_TICK_RATE 32000000
    #else
        #define CLOCK_DIVIDER 256
        #define CLOCK_DIVIDER_SETTING 0x07
        #define SYMBOL_TO_TICK_RATE 32000000
    #endif

    #define ONE_SECOND (((DWORD)CLOCK_FREQ/1000 * 62500) / (SYMBOL_TO_TICK_RATE / 1000))
    /* SYMBOLS_TO_TICKS to only be used with input (a) as a constant, otherwise you will blow up the code */
    #define SYMBOLS_TO_TICKS(a) (((DWORD)CLOCK_FREQ/100000) * a / ((DWORD)SYMBOL_TO_TICK_RATE/100000))
    #define TICKS_TO_SYMBOLS(a) (((DWORD)SYMBOL_TO_TICK_RATE/100000) * a / ((DWORD)CLOCK_FREQ/100000))


    #define TMR_IF          INTCONbits.TMR0IF
    #define TMR_IE          INTCONbits.TMR0IE
    #define TMR_IP          INTCON2bits.TMR0IP
    #define TMR_ON          T0CONbits.TMR0ON
    #define TMR_CON         T0CON
    #define TMR_L           TMR0L
    #define TMR_H           TMR0H
    
    
#elif defined(__dsPIC30F__) || defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC24H__)
    /* this section is based on the Timer 2/3 module of the dsPIC33/PIC24 family */
    #if(CLOCK_FREQ <= 125000)
        #define CLOCK_DIVIDER 1
        #define CLOCK_DIVIDER_SETTING 0x0000 /* no prescalar */
        #define SYMBOL_TO_TICK_RATE 125000
    #elif(CLOCK_FREQ <= 1000000)
        #define CLOCK_DIVIDER 8
        #define CLOCK_DIVIDER_SETTING 0x0010
        #define SYMBOL_TO_TICK_RATE 1000000
    #elif(CLOCK_FREQ <= 8000000)
        #define CLOCK_DIVIDER 64
        #define CLOCK_DIVIDER_SETTING 0x0020
        #define SYMBOL_TO_TICK_RATE 8000000
    #else
        #define CLOCK_DIVIDER 256
        #define CLOCK_DIVIDER_SETTING 0x0030
        #define SYMBOL_TO_TICK_RATE 32000000
    #endif

    #define ONE_SECOND (((DWORD)CLOCK_FREQ/1000 * 62500) / ((DWORD)SYMBOL_TO_TICK_RATE / 1000))
    /* SYMBOLS_TO_TICKS to only be used with input (a) as a constant, otherwise you will blow up the code */
    #define SYMBOLS_TO_TICKS(a) (((DWORD)CLOCK_FREQ/10000 * a ) / ((DWORD)SYMBOL_TO_TICK_RATE / 10000))
    #define TICKS_TO_SYMBOLS(a) (((DWORD)SYMBOL_TO_TICK_RATE/10000) * a / ((DWORD)CLOCK_FREQ/10000))
#elif defined(__PIC32MX__)
    /* this section is based on the Timer 2/3 module of the PIC32MX family */
    #define INSTR_FREQ  (CLOCK_FREQ/4)
    #if(INSTR_FREQ <= 125000)
        #define CLOCK_DIVIDER 1
        #define CLOCK_DIVIDER_SETTING 0x0000 /* no prescalar */
        #define SYMBOL_TO_TICK_RATE 125000
    #elif(INSTR_FREQ <= 1000000)
        #define CLOCK_DIVIDER 8
        #define CLOCK_DIVIDER_SETTING 0x0030
        #define SYMBOL_TO_TICK_RATE 1000000
    #elif(INSTR_FREQ <= 8000000)
        #define CLOCK_DIVIDER 64
        #define CLOCK_DIVIDER_SETTING 0x0060
        #define SYMBOL_TO_TICK_RATE 8000000
    #elif(INSTR_FREQ <= 16000000)
        #define CLOCK_DIVIDER 256
        #define CLOCK_DIVIDER_SETTING 0x0070
        #define SYMBOL_TO_TICK_RATE INSTR_FREQ
    #else
        #define CLOCK_DIVIDER 256
        #define CLOCK_DIVIDER_SETTING 0x70
        #define SYMBOL_TO_TICK_RATE INSTR_FREQ
    #endif

    #define ONE_SECOND (((DWORD)INSTR_FREQ/1000 * 62500) / (SYMBOL_TO_TICK_RATE / 1000))
    /* SYMBOLS_TO_TICKS to only be used with input (a) as a constant, otherwise you will blow up the code */
    #define SYMBOLS_TO_TICKS(a) (((DWORD)(INSTR_FREQ/100000) * a) / (SYMBOL_TO_TICK_RATE / 100000))
    #define TICKS_TO_SYMBOLS(a) (((DWORD)SYMBOL_TO_TICK_RATE/100000) * a / ((DWORD)CLOCK_FREQ/100000))
#else
    #error "Unsupported processor.  New timing definitions required for proper operation"
#endif

#define ONE_MILI_SECOND     (ONE_SECOND/1000)
#define HUNDRED_MILI_SECOND (ONE_SECOND/10)
#define FORTY_MILI_SECOND   (ONE_SECOND/25)
#define TWENTY_MILI_SECOND  (ONE_SECOND/50)
#define TEN_MILI_SECOND     (ONE_SECOND/100)
#define FIVE_MILI_SECOND    (ONE_SECOND/200)
#define TWO_MILI_SECOND     (ONE_SECOND/500)
#define ONE_MINUTE          (ONE_SECOND*60)
#define ONE_HOUR            (ONE_MINUTE*60)

#define MiWi_TickGetDiff(a,b) (a.Val - b.Val)

/************************ DATA TYPES *******************************/


/******************************************************************
 // Time unit defined based on IEEE 802.15.4 specification.
 // One tick is equal to one symbol time, or 16us. The Tick structure
 // is four bytes in length and is capable of represent time up to
 // about 19 hours.
 *****************************************************************/
typedef union _MIWI_TICK
{
    DWORD Val;
    struct _MIWI_TICK_bytes
    {
        BYTE b0;
        BYTE b1;
        BYTE b2;
        BYTE b3;
    } byte;
    BYTE v[4];
    struct _MIWI_TICK_words
    {
        WORD w0;
        WORD w1;
    } word;
} MIWI_TICK;

void InitSymbolTimer(void);
MIWI_TICK MiWi_TickGet(void);

/************************ VARIABLES ********************************/

extern volatile BYTE timerExtension1,timerExtension2;
#endif
