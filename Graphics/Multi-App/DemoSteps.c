/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * Support code for the ESC demo. This runs the demo in automatic mode.
*****************************************************************************
 * FileName:        DemoSteps.c
 * Dependencies:    DemoSteps.h
 * Processor:       PIC24, PIC32
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Paolo A. Tamayo		09/15/08	... 
 *****************************************************************************/
#include "MainDemo.h"

#ifdef ENABLE_DEMO_MODE
WORD            demoCount;          // demo event counter
BOOL            gEnableDemoFlag;    // flag to indicate if the demo is enabled
DWORD           prevDemoTime;       // keeps the record of the previous time, used to delay

// the execution of each predefined event
extern DWORD    tick;               // external tick counter

// the predefined events of the automatic demo mode	
const DEMO_MSG  DemoPoints[] =  {   // dummy event
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,    0},

    // event to move to page 1 of the main screen
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // ********************************************
    // RGB Demo
    // ********************************************
    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   ONESECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS, IconLeft(0)+2, IconTop(0)+2,    HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, IconLeft(0)+2, IconTop(0)+2,  HALFSECDELAY},

    // inside the RGB demo
    // set fade mode
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWENTYSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // use preset color 1
    {TYPE_TOUCHSCREEN, EVENT_PRESS, 0, 0, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // use preset color 2
    {TYPE_TOUCHSCREEN, EVENT_PRESS, 0, 0, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // use preset color 1
    {TYPE_TOUCHSCREEN, EVENT_PRESS, 0, 0, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // use preset color 2
    {TYPE_TOUCHSCREEN, EVENT_PRESS, 0, 0, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // emulate the touch on the palette
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+16, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+32, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+64, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+66, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+68, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+76, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+86, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+90, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+100, PLTYPOS+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+140, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+150, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+140, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+100, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+80, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+86, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+76, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+68, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+66, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+64, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+32, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+16, PLTYPOS+2, FIFTHSECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+2, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+4, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+8, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+16, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+32, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+64, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+66, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+68, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+76, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+86, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+90, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+120, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+90, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+86, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+76, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+68, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+66, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+64, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+32, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+16, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+8, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+4, PLTYPOS+2, FIFTHSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+120+2, PLTYPOS+2, FIFTHSECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTWIDTH-(PLTWIDTH>>2), PLTYPOS+2, FIFTHSECDELAY},

    // change the colors of R, G and B using the left and right buttons
    // change the colors R
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   BTNUPXPOS+2, BTN1UPYPOS+2,  HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, BTNUPXPOS+2, BTN1UPYPOS+2,  THREESECDELAY},

    // change the colors G
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   BTNDNXPOS+2, BTN2DNYPOS+2,  HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, BTNDNXPOS+2, BTN2DNYPOS+2,  TWOSECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS,   BTNUPXPOS+2, BTN2UPYPOS+2,  HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, BTNUPXPOS+2, BTN2UPYPOS+2,  TWOSECDELAY},

    // change the colors B
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   BTNDNXPOS+2, BTN3DNYPOS+2,  HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, BTNDNXPOS+2, BTN3DNYPOS+2,  THREESECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS,   BTNUPXPOS+2, BTN3UPYPOS+2,  HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, BTNUPXPOS+2, BTN3UPYPOS+2,  TWOSECDELAY},

    // preset color 1
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, ONESECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS, PLTXPOS+10, PLTYPOS+2, FIFTHSECDELAY},

    // change the colors G
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   BTNDNXPOS+2, BTN2DNYPOS+2,  HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, BTNDNXPOS+2, BTN2DNYPOS+2,  ONESECDELAY},

    // change the colors R
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   BTNDNXPOS+2, BTN1DNYPOS+2,  HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, BTNDNXPOS+2, BTN1DNYPOS+2,  ONESECDELAY},

    // preset color 2
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2, ONESECDELAY},

    // exit the RGB demo
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(0)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(0)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // ********************************************
    // Chart Demo
    // ********************************************
    // event to move to page 2 of the main screen,
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, FIFTHSECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS,   IconLeft(1)+2, IconTop(0)+2,    HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, IconLeft(1)+2, IconTop(0)+2,   FIFTHSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   TWOSECDELAY},

    // select Bar charts
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, FIFTHSECDELAY},

    // select Bar charts
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, FIFTHSECDELAY},

    // select Multi Bar charts
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(2)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2, FIFTHSECDELAY},

    // select Bar charts
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, FIFTHSECDELAY},

    // select Bar charts
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, FIFTHSECDELAY},

    // select Bar charts
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, FIFTHSECDELAY},

    // select Pie charts
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(1)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, FIFTHSECDELAY},

    // select pie charts
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, FIFTHSECDELAY},

    // select pie charts
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, FIFTHSECDELAY},

    // select pie charts
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, FIFTHSECDELAY},

    // select pie charts
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, FIFTHSECDELAY},

    // exit the chart demo
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(0)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(0)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // event to move to page 1 of the main screen,
    //{TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},
    //{TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2,	HALFSECDELAY},
    // ********************************************
    // External Flash Demo
    // ********************************************
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   IconLeft(2)+2, IconTop(0)+2,    HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, IconLeft(2)+2, IconTop(0)+2,   HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   TWOSECDELAY},

    // select picture
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // select picture
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // select picture
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(2)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // select picture
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // select picture
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // select picture
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // select picture
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // exit the External Flash demo
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(0)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(0)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // event to move to page 1 of the main screen,
    //{TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},
    //{TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2,	HALFSECDELAY},
    // ********************************************
    // Animation Demo
    // ********************************************
    // event to move to page 1 of the main screen,
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},


    {TYPE_TOUCHSCREEN, EVENT_PRESS,   IconLeft(0)+2, IconTop(1)+2,    HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, IconLeft(0)+2, IconTop(1)+2,   HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   TWOSECDELAY},

    // change the scale
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(1)+2, CtrlBtnTop()+2, FASTDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(1)+2, CtrlBtnTop()+2, FASTDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(1)+2, CtrlBtnTop()+2, FASTDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // decrease the speed
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // decrease the speed
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // decrease the speed
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(2)+2, CtrlBtnTop()+2, FASTDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   ONESECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // increase the speed
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, FASTDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // change the scale
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(1)+2, CtrlBtnTop()+2, FASTDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // exit the Animation demo
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(0)+2, CtrlBtnTop()+2, TWOSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(0)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // ********************************************
    // Language Demo
    // ********************************************
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   IconLeft(1)+2, IconTop(0)+2,    HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, IconLeft(1)+2, IconTop(0)+2,   HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   TWOSECDELAY},

    // change the text
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   THREESECDELAY},

    // change the text
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   THREESECDELAY},

    // change the text
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   THREESECDELAY},

    // change the text
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   THREESECDELAY},

    // change the text
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   THREESECDELAY},

    // change the text
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   THREESECDELAY},

    // change the text
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   THREESECDELAY},

    // change the text
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   TWOSECDELAY},

    // change the text
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   THREESECDELAY},

    // exit the Language demo
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(0)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(0)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // ********************************************
    // Time Demo
    // ********************************************
    // event to move to page 2 of the main screen, always assumes that the page when
    // this demo is entered is page 1.
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},


    {TYPE_TOUCHSCREEN, EVENT_PRESS,   IconLeft(0)+2, IconTop(0)+2,    HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, IconLeft(0)+2, IconTop(0)+2,   HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   THREESECDELAY},

    // set the time
    // set the Hour
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(2)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // set the Minute
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // skip the Seconds
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // set the Month
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // set the month day
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},

    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(3)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // exit setting
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(1)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // dummy
    {TYPE_TOUCHSCREEN, EVENT_INVALID, 0, 0,   TWOSECDELAY},

    // exit the time demo
    {TYPE_TOUCHSCREEN, EVENT_PRESS,   CtrlBtnLeft(0)+2, CtrlBtnTop()+2, HALFSECDELAY},
    {TYPE_TOUCHSCREEN, EVENT_RELEASE, CtrlBtnLeft(0)+2, CtrlBtnTop()+2, HALFSECDELAY},

    // demo loop back marker
    {0xCC, 0x55, 0xFF, 0xFF,    0},

};

/* */

void InitDemoMode(void)
{
    gEnableDemoFlag = FALSE;
    prevDemoTime = 0;
    demoCount = 0;
}

/* */
void UpdateDemoMode(GOL_MSG *pMsg)
{
    if(pMsg->uiEvent == EVENT_PRESS)
    {
        if(gEnableDemoFlag == TRUE)
        {
            gEnableDemoFlag = FALSE;
            demoCount = 0;
            return;
        }
    }

    if(gEnableDemoFlag == TRUE)
    {
        if((tick - prevDemoTime) > (DemoPoints[demoCount].time))
        {
            pMsg->type = DemoPoints[demoCount].type;
            pMsg->uiEvent = DemoPoints[demoCount].uiEvent;
            pMsg->param1 = DemoPoints[demoCount].param1;
            pMsg->param2 = DemoPoints[demoCount].param2;

            // check if the demo loop back marker is reached
            if(DemoPoints[demoCount].type == 0xCC)
            {
                demoCount = 0;
            }
            else
            {
                demoCount++;
            }

            prevDemoTime = tick;
        }
    }
}

#endif //ENABLE_DEMO_MODE
