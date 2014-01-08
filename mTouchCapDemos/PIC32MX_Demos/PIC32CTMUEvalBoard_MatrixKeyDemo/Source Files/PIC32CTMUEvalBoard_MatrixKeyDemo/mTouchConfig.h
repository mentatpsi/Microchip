/******************************************************************************

  PIC232 MTOUCH CVD Eval Board configuration file (Header File)

Description:
    This file defines variables of interest to the way the touch sense
    keys operate.

******************************************************************************/

/*****************************************************************************

* FileName:        mTouchConfig.h
* Dependencies:    None.
* Processor:       PIC32MX795F512H
* Compiler:        C32
* Company:         Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

Change History:
Author          Date       Comments
-------------------------------------------------------------------------------
BDB          26-Jun-2008 Initial release
SB           22-Oct-2008
NMS/NK       10-Feb-2009 Folder/Files restructuring
MC           22-Ian-2009 Porting for PIC32MX795F512
MWM          28 Mar 2011 Moved key performance #define's up to this file
******************************************************************************/
#ifndef __MTOUCH_CONFIG_H
#define __MTOUCH_CONFIG_H

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~    Includes    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    #include "Compiler.h"
    #include "GenericTypeDefs.h"
    #include "HardwareProfile.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~    Demo Chosen ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#undef  USE_SLIDER_2CHNL
#undef  USE_SLIDER_4CHNL
#define USE_MATRIX_KEYS
#undef  USE_DIRECT_KEYS

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~    Timer Call Back  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//#define TIMERCALLBACKFUNC TimerCallbackFunc1 // single or differential
//#define TIMERCALLBACKFUNC TimerCallbackFunc2 // single or differential with pull-ups
//#define TIMERCALLBACKFUNC TimerCallbackFunc3 // differential only wo pull-ups
  #define TIMERCALLBACKFUNC TimerCallbackFunc4 // Using CTMU

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~    Constants / Macros ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define TICKLE_LED1 //for diagnostic signal on LED 1
#undef  TICKLE_LED1 //for diagnostic signal on LED 1

/* Debug count for key press */
#define DEBOUNCECOUNT            3   // Defines How Many consecutive reads for

#define TIMER4_PERIOD         3000  // Timer4 clock ticks between button scans

#define NUM_HF_READS            32   // number of reads in CVD Channels
                                    // that are averaged to produce "current" value

#define ALPHA_INVERSE           32  // Weight in updating average based on current value
#define LOG2_ALPHA_INVERSE       5  // Equivalent bit shift to averaging weight

#define KEYTRIPDIV              40
#define HYSTERESIS_VALUE        50

#define DEFAULT_TRIP_VALUE      100  // Default trip value for all channels, in adjusted ADC counts
                                     // Only used to initialize array tripValue[ ]
/*
    "Average" data is calculated using the algorithm

        Average[new] = (1 - alpha)*Average[old] + alpha*CurFiltdData[new]

    implemented in C code such as

        averageData[iChannel] = averageData[iChannel]
                                - (averageData[iChannel]>>Log2AlphaInverse)
                                + (CurFiltdData[iChannel]>>Log2AlphaInverse);

    The "current" raw data value is based on ADC measurements taken in
    the Timer 4 ISR call back function, "TimerCallbackFuncN" (N=1,2,3,4), found in
    TimerCallbackFuncN.c  (N=1,2,3,4), with additional filtering and averaging done by
    "mTouchCapPhy_UpdateData" in mTouchCapPhy.c .

    Trip value and hysteris value are defined by
        tripValue[iChannel] = averageData[iChannel] / KEYTRIPDIV;
        hystValue[iChannel] = tripValue[iChannel] / HYSTERESIS_VALUE;

    A button is "pressed" if
        CurFiltdData[iChannel] < averageData[iChannel] - tripValue[iChannel]

    A button is "released" if
        CurFiltdData[iChannel] > averageData[iChannel] - tripValue[iChannel] + hystValue[iChannel]
 */

/* Allow Enough Time to initialize all channels in mTouchCapPhy_StablizeChannelData */
// Startup Count: Just loop through meassurements and update averages using slow averaging
#define STABILIZE_STARTUP_COUNT  (4*ALPHA_INVERSE)


/*
  Charge/discharge cycles for Rev 1 and Rev 2

  Multiple cycles needed because ADC's sample and hold (SAH)
  capacitor is much smaller than typical button capacitor.

 */
// Also used for Rev 3, Vpositive
#define CHARGE_DISCHARGE_CYCLE_1
#define CHARGE_DISCHARGE_CYCLE_2
#define CHARGE_DISCHARGE_CYCLE_3
#define CHARGE_DISCHARGE_CYCLE_4

//Button charge cycles for Rev 3, Vnegative
#define BUTTON_CHARGE_CYCLE_1
#define BUTTON_CHARGE_CYCLE_2
#define BUTTON_CHARGE_CYCLE_3
//#define BUTTON_CHARGE_CYCLE_4

//Button discharge cycles for Rev 3, Vnegative
#define BUTTON_DISCHARGE_CYCLE_1
#define BUTTON_DISCHARGE_CYCLE_2
#define BUTTON_DISCHARGE_CYCLE_3
#define BUTTON_DISCHARGE_CYCLE_4


// CTMU charge timing for Rev 4
#if   defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD )


#elif defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )


#elif defined( PIC32MC764F128_EXPLORER_16_BOARD ) || \
      defined( PIC32MX795_CAP_TOUCH_EVAL_BOARD  )
    #define ChargeDelay()  asm("NOP");asm("NOP");asm("NOP");asm("NOP") // 50 ns


#elif defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD ) || \
      defined( PIC32MX220_STARTER_KIT_BOARD    )
    #define ChargeDelay()  asm("NOP");asm("NOP");asm("NOP");asm("NOP") // 100 ns

    // Eleven ChargeDelay's brings unasserted button voltage to half scale
    #define CTMU_CHARGE_DELAYS  ChargeDelay(); ChargeDelay(); \
                                ChargeDelay(); ChargeDelay(); \
                                ChargeDelay(); ChargeDelay(); \
                                ChargeDelay(); ChargeDelay(); \
                                ChargeDelay(); ChargeDelay(); \
                                ChargeDelay();

#endif


//Button assert decode methods:
// { DECODE_METHOD_MOST_PRESSED | DECODE_METHOD_PRESS_ASSERT }
#define DEFAULT_DECODE_METHOD   DECODE_METHOD_PRESS_ASSERT

//Average button voltage filtering methods:
// { FILTER_METHOD_SLOWAVERAGE | FILTER_METHOD_GATEDAVERAGE | FILTER_METHOD_FASTAVERAGE }
//#define DEFAULT_FILTER_METHOD   FILTER_METHOD_SLOWAVERAGE
#define DEFAULT_FILTER_METHOD   FILTER_METHOD_GATEDAVERAGE

// Limit change between consecutive voltage measurements to +/- 1 ADC LSB ?
#define LIMIT_SLEW_RATE
//#undef LIMIT_SLEW_RATE

// Use differential instead of single-ended measurements ?
#define  USE_DIFFERENTIAL_MEASUREMENTS
#undef USE_DIFFERENTIAL_MEASUREMENTS

// Dump mTouch diagnostics out UART?
#define DUMP_DIAGNOSTICS_VIA_UART
//#undef  DUMP_DIAGNOSTICS_VIA_UART

#if defined( DUMP_DIAGNOSTICS_VIA_UART )
// Dump raw ADC counts out UART?
#define UART_DUMP_RAW_COUNTS
#undef  UART_DUMP_RAW_COUNTS

#ifndef UART_DUMP_RAW_COUNTS
#   define UART_DUMP_ALL_COUNTS // Dump Vpos, Vneg, Vmeas
#    undef UART_DUMP_ALL_COUNTS
#endif

#endif//defined( DUMP_DIAGNOSTICS_VIA_UART )

#endif // _CONFIG_H
