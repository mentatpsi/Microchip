/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        system.c
 *  Dependencies:    system.h
 *  Processor:       PIC18
 *  Compiler:        HI-TECH, MPLAB C18 and XC8
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This files contains all hardware depended functions.
 *************************************************************************/
/**************************************************************************
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software, and 
 * any derivatives created by any person or entity by or on your behalf, 
 * exclusively with Microchip's products in accordance with applicable
 * software license terms and conditions, a copy of which is provided for
 * your referencein accompanying documentation. Microchip and its licensors 
 * retain all ownership and intellectual property rights in the 
 * accompanying software and in all derivatives hereto. 
 * 
 * This software and any accompanying information is for suggestion only. 
 * It does not modify Microchip's standard warranty for its products. You 
 * agree that you are solely responsible for testing the software and 
 * determining its suitability. Microchip has no obligation to modify, 
 * test, certify, or support the software. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH 
 * MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY 
 * APPLICATION. 
 * 
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, 
 * TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT 
 * LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, 
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, 
 * FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, 
 * HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY 
 * OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS 
 * SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID 
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE. 
 * 
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF 
 * THESE TERMS. 
 *************************************************************************/
#ifndef _SYSTEM_C_
#define _SYSTEM_C_

#include "system.h"

#ifdef MTOUCH_DEBUG

#if (CLOCK_FREQUENCY == 48000000)
void SysLogInit(void)
{
     EECON2 = 0x55; EECON2 = 0xAA; PPSCONbits.IOLOCK = 0;
     RPOR11 = 5;
     EECON2 = 0x55; EECON2 = 0xAA; PPSCONbits.IOLOCK = 1;
     TXSTA2bits.BRGH = 1;
     SPBRG2 = 25;
     TXSTA2bits.TXEN = 1;
     RCSTA2bits.SPEN = 1;
}

void LogChar(char ch)
{
    while(TXSTA2bits.TRMT == 0) Nop();
    TXREG2 = ch;
}
#else

#error Debug UART works only for 48MHz. Change frequency or comment MTOUCH_DEBUG in mTouchConfig.h

#endif
#endif

void SysDelayMs(UINT16 delay)
{
    while(delay--)
    {
       SysDelay1Ms();
    }
}

void SysBoardInit(void)
{
    OSCTUNEbits.PLLEN = 1;
    INTCONbits.PEIE = 1; INTCONbits.GIE = 1;
    SysLedsInit();
    SysLedsChaser();
}

#if defined(__PICC18__)
void interrupt ISRTimer2(void)
#else
void ISRTimer2(void);
#pragma code HIGH_INTERRUPT_VECTOR = 0x08
void High_ISR (void)
{
	_asm goto ISRTimer2 _endasm
}
#pragma interrupt ISRTimer2
void ISRTimer2(void)
#endif
{

   SysTimerInterrupt();

   TMR2 = 0;
   PIR1bits.TMR2IF = 0;
}

#endif
