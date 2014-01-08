/******************************************************************************

  mTouchCapTmr.h (Header File)

Description:
    This file contains function prototypes that are used in TMRDriver.c
******************************************************************************/

/******************************************************************************
* FileName:        TMRDriver.h
* Dependencies:    See included files, below.
* Processor:       PIC32MX795F512H
* Compiler:        C32
* Company:         Microchip Technology, Inc.
*
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
* Change History:
* Author          Date       Comments
*------------------------------------------------------------------------------
* BDB           26-Jun-2008 Initial release
* NK            24-Apr-2009 Porting for 18F46J50 Eval Board
* MC            22-Ian-2010 First release of TMRDriver.c, TMRDriver.h
*                           Porting for PIC32MX795F512H
*
*******************************************************************************/
#ifndef __MTOUCH_CAP_TIMER_H
#define __MTOUCH_CAP_TIMER_H

#include "HardwareProfile.h"

//==========================================================
//--- PROTOTYPES
//==============================
void mTouchCapTmr_IgnoreCurrentData(BOOL);
void mTouchCapTmr_InitTimer4(void);
void mTouchCapTmr_SetPeriodTimer4(unsigned int time);

/************************************************************************
Macro       : Set_ScanTimer_IF_Bit_State(State)
Overview    : setting or clearing T4 Interrupt Flag Bit
Parameters  : ENABLE '1' or DISABLE '0'
Remarks     : None.
**************************************************************************/
#if defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
    #define Set_ScanTimer_IF_Bit_State(State)  (IFS1bits.T4IF = State)
#else
    #define Set_ScanTimer_IF_Bit_State(State)  (IFS0bits.T4IF = State)
#endif

/************************************************************************
Macro       : Set_ScanTimer_IE_Bit_State(State)
Overview    : setting or clearing T4 Interrupt Enable Bit
Parameters  : ENABLE '1' or DISABLE '0'
Remarks     : None.
**************************************************************************/
#if defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
    #define Set_ScanTimer_IE_Bit_State(State)  (IEC1bits.T4IE = State)
#else
    #define Set_ScanTimer_IE_Bit_State(State)  (IEC0bits.T4IE = State)
#endif

/************************************************************************
Macro       : Set_ScanTimer_ON_Bit_State(State
Overview    : setting or clearing T4 ON bit
Parameters  : ENABLE '1' or DISABLE '0'
Remarks     : None.
**************************************************************************/
#if defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
    #define Set_ScanTimer_ON_Bit_State(State)  (T4CONbits.TON = State)
#else
    #define Set_ScanTimer_ON_Bit_State(State)  (T4CONbits.TON = State)
#endif

/************************************************************************
Macro       : Set_ScanTimer_Priority(State)
Overview    : setting or clearing T4 ON bit
Parameters  : ENABLE '1' or DISABLE '0'
Remarks     : None.
**************************************************************************/
#if defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
    #define Set_ScanTimer_Priority(State)  (IPC6bits.T4IP = State)
#else
    #define Set_ScanTimer_Priority(State)  (IPC4bits.T4IP = State)
#endif

/************************************************************************
Macro       : Set_ScanTimer_Subriority(State)
Overview    : setting or clearing T4 ON bit
Parameters  : ENABLE '1' or DISABLE '0'
Remarks     : None.
**************************************************************************/
#if defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
    #define Set_ScanTimer_Subpriority(State)  // No subpriority available
#else
    #define Set_ScanTimer_Subpriority(State)  (IPC4bits.T4IS = State)
#endif

#endif //end of __MTOUCH_CAP_TIMER_H
