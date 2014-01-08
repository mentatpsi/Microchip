/******************************************************************************

    mTouchCapLED (Header File)

Description:
    This file contains function prototypes that are used in display.c
*******************************************************************************/

/*******************************************************************************
* FileName:         mTouchCap_Display
* Dependencies:    See included files, below.
* Processor:       PIC32MX795F512H
* Compiler:        C32
* Company:         Microchip Technology, Inc.
*
* FileName:         mTouchCap_PIC32MX_DirectKeyDemo.c
* Dependencies:
* Processor:        PIC32
* Compiler:         C32
* Linker:           MPLAB LINK32
* Company:          Microchip Technology Incorporated
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
* Change History:
* Author Date       Comments
*--------------------------------------------------------------------------
* BDB    26-Jun-2008 Initial release
* NK     24-Apr-2009 Porting for 18F46J50 Eval Board
* MC     22-Ian-2010 Porting for PIC32MX795F512H
* MWM    13 Sep 2011 Changed function prefix from mTouchCapAPI to mTouchCapLED
*******************************************************************************/
#ifndef __MTOUCHCAP_DISPLAY_H
#define __MTOUCHCAP_DISPLAY_H

#include "mTouchConfig.h"
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "mTouchCapPhy.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~    Function Prototypes  ~~~~~~~~~~~~~~~~~~~~~~~~~~     */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#if defined(USE_SLIDER_4CHNL)
void mTouchCapLED_4ChDisplay(WORD Percent_Display);
#endif//defined(USE_SLIDER_4CHNL)

#if defined( USE_MATRIX_KEYS )
void mTouchCapLED_MatrixKeyDisplay(SHORT input);
#endif//defined( USE_MATRIX_KEYS )

void mTouchCapLED_PowerUpSequence(void);

void mTouchCapLED_SetSliderLEDs(SHORT);

void mTouchCapLED_SetLED(WORD LedNum);

enum{
    LED_1=0,
    LED_2,
    LED_3,
    LED_4,
    LED_5,
    LED_6,
    LED_7,
    LED_8,
    LED_9,
    LED_10,
    LED_11,
    LED_12,
    LED_13,
    LED_14,
    LED_15,
    LED_16,
    LED_NIL
};

#endif  //end of #ifndef __DISPLAY_H
