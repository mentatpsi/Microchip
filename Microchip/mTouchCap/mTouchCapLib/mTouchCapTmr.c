/******************************************************************************
* File Name:       mTouchCapTmr.c
* Includes:
*   void mTouchCapTmr_IgnoreCurrentData(BOOL)
*   void mTouchCapTmr_InitTimer4(void)
*   void mTouchCapTmr_SetPeriodTimer4(unsigned int time)
*   void __attribute__((interrupt, auto_psv)) _T4Interrupt (void)
*   void __ISR(_TIMER_4_VECTOR, IPL1SOFT) mTouchCapTmr_Tmr4ISR(void)
*
* Dependencies:    None
* Processor:       PIC32MX795F512H
* Compiler:        C32
* Company:         Microchip Technology, Inc.
* Software License Agreement
*
* Copyright © 2011 Microchip Technology Inc.
* Microchip licenses this software to you solely for use with Microchip
* products, according to the terms of the accompanying click-wrap software
* license. Microchip and its licensors retain all right, title and interest in
* and to the software.  All rights reserved. This software and any accompanying
* information is for suggestion only. It shall not be deemed to modify
* Microchip’s standard warranty for its products.  It is your responsibility to
* ensure that this software meets your requirements.
*
* SOFTWARE IS PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR
* IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
* NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL
* MICROCHIP OR ITS LICENSORS BE LIABLE FOR ANY DIRECT OR INDIRECT DAMAGES OR
* EXPENSES INCLUDING BUT NOT LIMITED TO INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
* OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
* SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, OR ANY CLAIMS BY THIRD PARTIES
* (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
* The aggregate and cumulative liability of Microchip and its licensors for
* damages related to the use of the software will in no event exceed $1000 or
* the amount you paid Microchip for the software, whichever is greater.
*
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
* TERMS AND THE TERMS OF THE ACCOMPANYING CLICK-WRAP SOFTWARE LICENSE.
*
*
* Author    Date    Comments
*-------------------------------------------------------------------------
* BDB    26-Jun-2008 First release
* NK     24-Apr-2009 Porting for 18F46J50 Eval Board
* MC     22-Ian-2010 First release of TMRDriver.c, TMRDriver.h
*                    Porting for PIC32MX795F512H
* MWM    06-Sep-2011 Moved Callback to separate file.
******************************************************************************/
#include "mTouchConfig.h"
#include "mTouchCapAPI.h"
#include "mTouchCapTmr.h"


// From  mTouchCapAPI.c
extern volatile UINT8 EmTouchDataReady;      //global indicating the reading of all channels finished

/* timer 4 callback ISR local - init in Initialization function */
static void *T4CallBackISR;
volatile BOOL IgnoreCurrentDataFlag = FALSE; // Flag set by application to signal when mTouch
                                             // should ignore data being measured.

void TIMERCALLBACKFUNC(void); //Defined in mTouchConfig.h

void mTouchCapTmr_IgnoreCurrentData( BOOL Flag )
{
    IgnoreCurrentDataFlag = Flag;
}

/******************************************************************************
 * Function:       void InitTimer(void)
 *
 * PreCondition:    None
 *
 * Input:           unsigned int time
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Set the period for timer 4 depending on input param time
 *
 * Note:            Stops and restarts the timer when setting the new period
 *****************************************************************************/
void mTouchCapTmr_InitTimer4(void)
{

    // set up the timer interrupt with a priority of 1
    Set_ScanTimer_Priority(1);
    // interrupt subpriority is 0
    Set_ScanTimer_Subpriority(0);
    // clear the interrupt flag
    Set_ScanTimer_IF_Bit_State(0);

    // initialize the callback pointer with the value passed as argument
    T4CallBackISR = TIMERCALLBACKFUNC;

    /* enable the interrupt */
    Set_ScanTimer_IE_Bit_State(1);

    TMR4 = 0;
    PR4 = TIMER4_PERIOD;
    // restart the timer
    Set_ScanTimer_ON_Bit_State(1);

    return;
}

/******************************************************************************
 * Function:       void SetPeriodTimer4(unsigned int time)
 *
 * PreCondition:    None
 *
 * Input:           unsigned int time
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Set the period for timer 4 depending on input param time
 *
 * Note:            Stops and restarts the timer when setting the new period
 *****************************************************************************/
void mTouchCapTmr_SetPeriodTimer4(unsigned int time)
{
    // stop the timer
    Set_ScanTimer_ON_Bit_State(0);
    /* load the time to higher and lower byte of the timer register */
    TMR4 = 0;
    PR4  = time;
    // restart the timer
    Set_ScanTimer_ON_Bit_State(1);

}

#if defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
void __attribute__((interrupt, auto_psv)) _T4Interrupt (void)
#else
void __ISR(_TIMER_4_VECTOR, IPL1SOFT) mTouchCapTmr_Tmr4ISR(void)
#endif
{
    void (*ptr_fcn)(void);

    /* init local wiht the callback function ptr */
    ptr_fcn = (void (*) ())T4CallBackISR;
    if (ptr_fcn != 0)
    {
        // call the callback isr
        (*ptr_fcn)();
    }

    Set_ScanTimer_IF_Bit_State(DISABLE);    //Clear interrupt flag
    if ( EmTouchDataReady == TRUE )
    {
        Set_ScanTimer_IE_Bit_State(DISABLE);    // Disable interrupt
        Set_ScanTimer_ON_Bit_State(DISABLE);    // Stop timer 4
    }

}
