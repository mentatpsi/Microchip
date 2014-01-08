/*****************************************************************************
 *
 * Beeper.
 *
 *****************************************************************************
 * FileName:        Beep.c
 * Dependencies:    Beep.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Date        	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 07/10/07		...
 * 02/07/08    	PIC32 support
 *****************************************************************************/
#include "Beep.h"

#if defined (USE_GFX_PICTAIL_V2_BEEPER)

volatile WORD   pulseCounter;

/*********************************************************************
* Function: Timer2 ISR
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: counts beeper pulses and turns off it after BEEP_TIME
*
* Note: none
*
********************************************************************/
#ifdef __PIC32MX__
    #define __T2_ISR    __ISR(_TIMER_2_VECTOR, ipl1)
#else
    #define __T2_ISR    __attribute__((interrupt, shadow, auto_psv))
#endif

/* */
void __T2_ISR _T2Interrupt(void)
{
    BEEP_LAT_BIT ^= 1;
    if(BEEP_TIME < pulseCounter++)
    {

        // clear pulse counter
        pulseCounter = 0;

        // stop counter
        T2CONbits.TON = 0;

        // clear timer
        TMR2 = 0;
        PR2 = TIMER_BASE;
        BEEP_LAT_BIT = 0;
    }

    PR2 += 1;

    //clear flag
    #ifdef __PIC32MX
    mT2ClearIntFlag();
    #else
    IFS0bits.T2IF = 0;
    #endif
}

/*********************************************************************
* Function:  void BeepInit(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: initializes beeper
*
* Note: none
*
********************************************************************/
void BeepInit(void)
{
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
    RPOR2 = 0b0000000000010010;
    #endif

    // clear pulse counter
    pulseCounter = 0;

    // init Timer2
    TMR2 = 0;
    PR2 = TIMER_BASE;

    // set prescale to 1:8
    T2CONbits.TCKPS = 1;

    #ifdef __PIC32MX__
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
    #else
    IFS0bits.T2IF = 0;  //clear flag
    IEC0bits.T2IE = 1;  //enable interrupt
    #endif
    BEEP_LAT_BIT = 0;
    BEEP_TRIS_BIT = 0;
}
#endif // #if defined (GFX_PICTAIL_VER_2)

