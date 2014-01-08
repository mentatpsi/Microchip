/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        system.c
 *  Dependencies:    system.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
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

// Implement this function to log data from mTouch library.
void LogChar(char ch)
{
}

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
#if !defined (__C30__)

#if defined(FAMILY_PIC18XXK50)
    OSCCON2bits.PLLEN = 1;
#elif  defined(FAMILY_PIC18XXJ94)
    OSCCON4bits.PLLEN = 1;
#else
    OSCTUNEbits.PLLEN = 1;
#endif

    INTCONbits.PEIE = 1; INTCONbits.GIE = 1;
#endif
}


#if defined(__C30__)

void __attribute__((interrupt, shadow, auto_psv)) _T2Interrupt(void)
{
   SysTimerInterrupt();

   TMR2 = 0;

   _T2IF=0;
}

#else

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

#endif
