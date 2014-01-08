/*****************************************************************************
* FileName:         mTouchCapLED.c
* Includes:
*   void mTouchCapLED_4ChDisplay(WORD Percent_Display)
*
*   void mTouchCapLED_PowerUpSequence(void)
*
*   static void mTouchCapLED_SetSliderLEDs_OnEvalBoard(SHORT input)
*   void mTouchCapLED_SetSliderLEDs(SHORT input)
*
*   static void mTouchCapLED_SetLED_OnEvalBoard(WORD LedNum)
*   void mTouchCapLED_SetLED(WORD LedNum)
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
*----------------------------------------------------------------------
* BDB    26-Jun-2008 First release
* SB     22-Oct-2008
* NK     24-Apr-2009 Porting for 18F46J50 Eval Board
* MC     22-Ian-2010 Porting for PIC32MX795F512H
* MWM       Mar-2011 Added comments, rearranged order of routines
* MWM    13 Sep 2011 Changed function prefix from mTouchCapAPI to mTouchCapLED
*******************************************************************************/
#include    "HardwareProfile.h"
#include    "mTouchConfig.h"
#include    "mTouchCapLED.h"
#include    "mTouchCapAPI.h"
#include    "TimeDelay.h"


#if defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
    static unsigned int LED_ARRAY1_IO[16] =
        {
         0x0001,0x0003,0x0007,0x000F,    //Green
         0x001F,0x003F,0x007F,0x00FF,    //Yellow
         0x0002,0x0003,0x4003,0x5003,    //Orange
         0x7003,0xF003,0x0002,0x0006,    //Red
         };
    #if defined  (USE_DIRECT_KEYS) || defined (USE_MATRIX_KEYS)
    static unsigned int LED_ARRAY2_IO[16] =
        {
         0x0001,0x0002,0x0004,0x0008,
         0x0010,0x0020,0x0040,0x0080,
         0x0002,0x0001,0x4000,0x1000,
         0x2000,0x8000,0x0002,0x0004,
         };
    #endif

#elif !defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD ) && \
      !defined( PIC32MX220_STARTER_KIT_BOARD ) && \
      !defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
    static unsigned int LED_ARRAY1_IO[16] =
        {
         0x0001,0x0003,0x0007,0x000F,    //Green
         0x001F,0x003F,0x007F,0x00FF,    //Yellow
         0x0001,0x0003,0x0007,0x000F,    //Orange
         0x001F,0x003F,0x007F,0x00FF,    //Red
         };
#endif//!defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )...

#if     !defined( PIC32MX220_STARTER_KIT_BOARD )
#  if     defined (USE_DIRECT_KEYS) || defined(USE_MATRIX_KEYS)
    static SHORT PreviousLED = 0xFF; //storage of the last state of the
#  endif
#endif//!defined( PIC32MX220_STARTER_KIT_BOARD )

/********************************************************************
 * Function         :    void mTouchCapLED_4ChDisplay(WORD Percent_Display)
 *
 * PreCondition     :    None
 *
 * Input            :    Percent_Display - Percentage level of touch
 *
 * Output           :    None
 *
 * Side Effects     :    None
 *
 * Overview         :   The function will average out the samples and will
 *                      send the filtered value of the finger position on the
 *                      2 channel slider.
 *
 *
 * Note             :
 *******************************************************************/
#ifdef USE_SLIDER_4CHNL
#include "mTouchCapApp_FourChanSlider.h"

// From mTouchCapApp_FourChanSlider.c
extern SHORT   pct_output_4Chl;
extern long    pct_sum_4Chl;
extern long    pct_array_4Chl[6];

void mTouchCapLED_4ChDisplay(WORD Percent_Display)
{
    WORD Index;
    pct_output_4Chl = (SHORT)Percent_Display;
    pct_sum_4Chl = pct_array_4Chl[0];           // Establish sum initially with first
    for (Index=1; Index<6; Index++)
    {
        pct_sum_4Chl += pct_array_4Chl[Index];      // Calculate sum over iterations
        pct_array_4Chl[Index-1] = pct_array_4Chl[Index];// Shift values
    }
    pct_array_4Chl[5] = pct_output_4Chl;

    pct_output_4Chl = pct_sum_4Chl/6;


    if(pct_output_4Chl < 1)
    {
        mTouchCapLED_SetSliderLEDs(1);
    }
    else
    {
        mTouchCapLED_SetSliderLEDs(pct_output_4Chl);
    }
}
#endif


/********************************************************************
 * Function         :    void mTouchCapLED_PowerUpSequence(void)
 *
 * PreCondition     :    None
 *
 * Input            :    None
 *
 * Output           :    None
 *
 * Side Effects     :    None
 *
 * Overview         :   This function will turn ON the LEDs in the chaser sequence
 *
 * Note             :
 *******************************************************************/
#if defined( PIC32MX220_STARTER_KIT_BOARD ) || \
    defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD ) // No LEDs for power up
void mTouchCapLED_PowerUpSequence(void)
{
    //Do Nothing
}
#else // Eval board has LEDs
void mTouchCapLED_PowerUpSequence(void)
{
    INT16 DirFlag=+1;
    WORD  ChaserCount=0;
    WORD  runFlag = TRUE;

    while(runFlag)
    {
        DelayMs(20);

        if ( DirFlag == +1 )
        {
            ChaserCount++;
            if(ChaserCount==100)
            {
                DirFlag = -1;
            }
        }
        else
        {
            ChaserCount--;
            if(ChaserCount == 0)
            {
                runFlag = FALSE;
            }
        }

        mTouchCapLED_SetSliderLEDs(ChaserCount);

        if(runFlag == FALSE)
        {
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_NIL | PORTB_COL_NIL;
            PORTC = PORTC_ROW_NIL | PORTC_COL_NIL;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD )|| \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD=0x00;
            PORTG=0x00;
            PORTC=0x00;
#          else
            PORTD=0x0000;
            PORTE=0x0000;
#          endif
        }

    }

}
#endif

#if !defined( PIC32MX220_STARTER_KIT_BOARD ) && \
    !defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
static void mTouchCapLED_SetSliderLEDs_OnEvalBoard(SHORT input)
{

    switch (input)
    {
        // Turn on LED1
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
#          if    defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_1 | PORTB_COL_1;
            PORTC = PORTC_ROW_1 | PORTC_COL_1;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[0]; Nop();
            PORTG = 0;
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[0];
            PORTE = 0;
#          endif
            break;

        // Turn on LEDs 1-2
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_1 | PORTB_COL_2;
            PORTC = PORTC_ROW_1 | PORTC_COL_2;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[1]; Nop();
            PORTG = 0;
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[1];
            PORTE = 0;
#          endif
            break;

        // Turn on LEDs 1-3
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_1 | PORTB_COL_3;
            PORTC = PORTC_ROW_1 | PORTC_COL_3;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[2]; Nop();
            PORTG = 0;
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[2];
            PORTE = 0;
#          endif
            break;

        // Turn on LEDs 1-4
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_1 | PORTB_COL_4;
            PORTC = PORTC_ROW_1 | PORTC_COL_4;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[3]; Nop();
            PORTG = 0;
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[3];
            PORTE = 0;
#          endif
            break;

        // Turn on LEDs 1-5
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_2 | PORTB_COL_1;
            PORTC = PORTC_ROW_2 | PORTC_COL_1;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[4]; Nop();
            PORTG = 0;
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[4];
            PORTE = 0;
#          endif
            break;

        // Turn on LEDs 1-6
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_2 | PORTB_COL_2;
            PORTC = PORTC_ROW_2 | PORTC_COL_2;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[5]; Nop();
            PORTG = 0;
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[5];
            PORTE = 0;
#          endif
            break;

        // Turn on LEDs 1-7
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_2 | PORTB_COL_3;
            PORTC = PORTC_ROW_2 | PORTC_COL_3;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[6]; Nop();
            PORTG = 0;
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[6];
            PORTE = 0;
#          endif
            break;

        // Turn on LEDs 1-8
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_2 | PORTB_COL_4;
            PORTC = PORTC_ROW_2 | PORTC_COL_4;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[7]; Nop();
            PORTG = 0;
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[7];
            PORTE = 0;
#          endif
            break;

        // Turn on LEDs 1-9
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_3 | PORTB_COL_1;
            PORTC = PORTC_ROW_3 | PORTC_COL_1;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[7]; Nop();
            PORTG = LED_ARRAY1_IO[8]; Nop();
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[7];
            PORTE = LED_ARRAY1_IO[8];
#          endif
            break;

        // Turn on LEDs 1-10
        case 55:
        case 56:
        case 57:
        case 58:
        case 59:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_3 | PORTB_COL_2;
            PORTC = PORTC_ROW_3 | PORTC_COL_2;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[7]; Nop();
            PORTG = LED_ARRAY1_IO[9]; Nop();
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[7];
            PORTE = LED_ARRAY1_IO[9];
#          endif
            break;

        // Turn on LEDs 1-11
        case 60:
        case 61:
        case 62:
        case 63:
        case 64:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_3 | PORTB_COL_3;
            PORTC = PORTC_ROW_3 | PORTC_COL_3;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[7];  Nop();
            PORTG = LED_ARRAY1_IO[10]; Nop();
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[7];
            PORTE = LED_ARRAY1_IO[10];
#          endif
            break;

        // Turn on LEDs 1-12
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_3 | PORTB_COL_4;
            PORTC = PORTC_ROW_3 | PORTC_COL_4;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[7];  Nop();
            PORTG = LED_ARRAY1_IO[11]; Nop();
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[7];
            PORTE = LED_ARRAY1_IO[11];
#          endif
            break;

        // Turn on LEDs 1-13
        case 70:
        case 71:
        case 72:
        case 73:
        case 74:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_4 | PORTB_COL_1;
            PORTC = PORTC_ROW_4 | PORTC_COL_1;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[7];  Nop();
            PORTG = LED_ARRAY1_IO[12]; Nop();
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[7];
            PORTE = LED_ARRAY1_IO[12];
#          endif
            break;

        // Turn on LEDs 1-14
        case 75:
        case 76:
        case 77:
        case 78:
        case 79:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_4 | PORTB_COL_2;
            PORTC = PORTC_ROW_4 | PORTC_COL_2;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[7];  Nop();
            PORTG = LED_ARRAY1_IO[13]; Nop();
            PORTC = 0;
#          else
            PORTD = LED_ARRAY1_IO[7];
            PORTE = LED_ARRAY1_IO[13];
#          endif
            break;

        // Turn on LEDs 1-15
        case 80:
        case 81:
        case 82:
        case 83:
        case 84:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_4 | PORTB_COL_3;
            PORTC = PORTC_ROW_4 | PORTC_COL_3;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[7];  Nop();
            PORTG = LED_ARRAY1_IO[13]; Nop();
            PORTC = LED_ARRAY1_IO[14]; Nop();
#          else
            PORTD = LED_ARRAY1_IO[7];
            PORTE = LED_ARRAY1_IO[14];
#          endif
            break;

        // Turn on LEDs 1-16
        case 85:
        case 86:
        case 87:
        case 88:
        case 89:
        case 90:
        case 91:
        case 92:
        case 93:
        case 94:
        case 95:
        case 96:
        case 97:
        case 98:
        case 99:
        case 100:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_4 | PORTB_COL_4;
            PORTC = PORTC_ROW_4 | PORTC_COL_4;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = LED_ARRAY1_IO[7];  Nop();
            PORTG = LED_ARRAY1_IO[13]; Nop();
            PORTC = LED_ARRAY1_IO[15]; Nop();
#          else
            PORTD = LED_ARRAY1_IO[7];
            PORTE = LED_ARRAY1_IO[15];
#          endif
            break;

        default:
#          if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
            PORTB = PORTB_ROW_NIL | PORTB_COL_NIL;
            PORTC = PORTC_ROW_NIL | PORTC_COL_NIL;
#          elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
            PORTD = 0;
            PORTG = 0;
            PORTC = 0;
#          else
            PORTD = 0;
            PORTE = 0;
#          endif
            break;

    }

}
#endif//!defined( PIC32MX220_STARTER_KIT_BOARD )...

/********************************************************************
 * Function         :    void mTouchCapLED_SetSliderLEDs(SHORT input)
 *
 * PreCondition     :    None
 *
 * Input            :    input - Position of the finger on the slider in terms of percentage(0-100)
 *
 * Output           :    None
 *
 * Side Effects     :    None
 *
 * Overview         :    Invokes the appropriate LED display function based on PIC controller selected.
 *
 *
 *
 * Note             :
 *******************************************************************/
void mTouchCapLED_SetSliderLEDs(SHORT input)
{
#if !defined( PIC32MX220_STARTER_KIT_BOARD ) && \
    !defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
    /* Call PIC32MX LED display function */
    mTouchCapLED_SetSliderLEDs_OnEvalBoard(input);
#else
    //Do Nothing - starter kit board doesn't have LEDs.
#endif
}


/********************************************************************
 * Function         :    void mTouchCapLED_SetLED_OnEvalBoard(WORD LedNum)
 *
 * PreCondition     :    None
 *
 * Input            :    The LED number that has to be displayed.
 *
 * Output           :    None
 *
 * Side Effects     :    None
 *
 * Overview         :    This function will turn on the LED in the PIC32 CVD Eval Board
 *                       based on the input that is passed to the function
 *
 *
 * Note             :
 *******************************************************************/
#if !defined( PIC32MX220_STARTER_KIT_BOARD ) && \
    defined (USE_DIRECT_KEYS) || defined(USE_MATRIX_KEYS)
static void mTouchCapLED_SetLED_OnEvalBoard(WORD LedNum)
{

    if(PreviousLED != LedNum)      //check for new value to prevent flicker of LEDS
    {
        switch(LedNum)
        {
            case LED_1:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_1 | PORTB_COL_1;
                PORTC = PORTC_ROW_1 | PORTC_COL_1;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = LED_ARRAY2_IO[0]; Nop();
                PORTG = 0;
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                PORTC = 1<<2; // Left Arrow
                PORTD = 1<<7; // General Purpose LED Array
#              else
                PORTD=0x0001;
                PORTE=0x0000;
#              endif
                break;

            case LED_2:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_1 | PORTB_COL_2;
                PORTC = PORTC_ROW_1 | PORTC_COL_2;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = LED_ARRAY2_IO[1]; Nop();
                PORTG = 0;
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                PORTC = 1<<4; // Down Arrow
                PORTD = 1<<8; // General Purpose LED Array
#              else
                PORTD=0x0002;
                PORTE=0x0000;
#              endif
                break;

            case LED_3:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_1 | PORTB_COL_3;
                PORTC = PORTC_ROW_1 | PORTC_COL_3;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = LED_ARRAY2_IO[2]; Nop();
                PORTG = 0;
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                PORTC = 0;    // Enter (Central) Key
                PORTD = 1<<9; // General Purpose LED Array
#              else
                PORTD=0x0004;
                PORTE=0x0000;
#              endif
                break;

            case LED_4:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_1 | PORTB_COL_4;
                PORTC = PORTC_ROW_1 | PORTC_COL_4;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = LED_ARRAY2_IO[3]; Nop();
                PORTG = 0;
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                PORTC = 1<<3;  // Right Arrow
                PORTD = 1<<10; // General Purpose LED Array
#              else
                PORTD=0x0008;
                PORTE=0x0000;
#              endif
                break;

            case LED_5:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_2 | PORTB_COL_1;
                PORTC = PORTC_ROW_2 | PORTC_COL_1;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = LED_ARRAY2_IO[4]; Nop();
                PORTG = 0;
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                PORTC = 1<<1;     // Up  Arrow
                PORTD = 1<<11; // General Purpose LED Array
#              else
                PORTD=0x0010;
                PORTE=0x0000;
#              endif
                break;

            case LED_6:
#             if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_2 | PORTB_COL_2;
                PORTC = PORTC_ROW_2 | PORTC_COL_2;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = LED_ARRAY2_IO[5]; Nop();
                PORTG = 0;
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                PORTC = 0;
                PORTD = 1<<12; // General Purpose LED Array
#              else
                PORTD=0x0020;
                PORTE=0x0000;
#              endif
                break;

            case LED_7:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_2 | PORTB_COL_3;
                PORTC = PORTC_ROW_2 | PORTC_COL_3;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = LED_ARRAY2_IO[6]; Nop();
                PORTG = 0;
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                PORTC = 0;
                PORTD = 1<<5; // General Purpose LED Array
#              else
                PORTD=0x0040;
                PORTE=0x0000;
#              endif
                break;

            case LED_8:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_2 | PORTB_COL_4;
                PORTC = PORTC_ROW_2 | PORTC_COL_4;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = LED_ARRAY2_IO[7];
                PORTG = 0;
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                PORTC = 0;
                PORTD = 1<<6; // General Purpose LED Array
#              else
                PORTD=0x0080;
                PORTE=0x0000;
#              endif
                break;

            case LED_9:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_3 | PORTB_COL_1;
                PORTC = PORTC_ROW_3 | PORTC_COL_1;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = 0;
                PORTG = LED_ARRAY2_IO[8];
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                // Do Nothing
#              else
                PORTD=0x0000;
                PORTE=0x0001;
#              endif
                break;

            case LED_10:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_3 | PORTB_COL_2;
                PORTC = PORTC_ROW_3 | PORTC_COL_2;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = 0;
                PORTG = LED_ARRAY2_IO[9];
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                // Do Nothing
#              else
                PORTD=0x0000;
                PORTE=0x0002;
#              endif
                break;

            case LED_11:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_3 | PORTB_COL_3;
                PORTC = PORTC_ROW_3 | PORTC_COL_3;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = 0;
                PORTG = LED_ARRAY2_IO[10];
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                // Do Nothing
#              else
                PORTD=0x0000;
                PORTE=0x0004;
#              endif
                break;

            case LED_12:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_3 | PORTB_COL_4;
                PORTC = PORTC_ROW_3 | PORTC_COL_4;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = 0;
                PORTG = LED_ARRAY2_IO[11];
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                // Do Nothing
#              else
                PORTD=0x0000;
                PORTE=0x0008;
#              endif
                break;

            case LED_13:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_4 | PORTB_COL_1;
                PORTC = PORTC_ROW_4 | PORTC_COL_1;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = 0;
                PORTG = LED_ARRAY2_IO[12];
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                // Do Nothing
#              else
                PORTD=0x0000;
                PORTE=0x0010;
#              endif
                break;

            case LED_14:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_4 | PORTB_COL_2;
                PORTC = PORTC_ROW_4 | PORTC_COL_2;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = 0;
                PORTG = LED_ARRAY2_IO[13];
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                // Do Nothing
#              else
                PORTD=0x0000;
                PORTE=0x0020;
#              endif
                break;

            case LED_15:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_4 | PORTB_COL_3;
                PORTC = PORTC_ROW_4 | PORTC_COL_3;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = 0;
                PORTG = 0;
                PORTC = LED_ARRAY2_IO[14];
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                // Do Nothing
#              else
                PORTD=0x0000;
                PORTE=0x0040;
#              endif
                break;

            case LED_16:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_4 | PORTB_COL_4;
                PORTC = PORTC_ROW_4 | PORTC_COL_4;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = 0;
                PORTG = 0;
                PORTC = LED_ARRAY2_IO[15];
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                // Do Nothing
#              else
                PORTD=0x0000;
                PORTE=0x0080;
#              endif
                break;

            default:
#              if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
                PORTB = PORTB_ROW_NIL | PORTB_COL_NIL;
                PORTC = PORTC_ROW_NIL | PORTC_COL_NIL;
#              elif defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
                    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
                PORTD = 0;
                PORTG = 0;
                PORTC = 0;
#              elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
                PORTC = 0; // 5 Key Star
                PORTD = 0; // General Purpose LED Array
#              else
                PORTD=0x0000;
                PORTE=0x0000;
#              endif
                break;

        }

        /* Back it up  */
        PreviousLED = LedNum;

    }//if(PreviousLED != LedNum)
}
#endif//!defined( PIC32MX220_STARTER_KIT_BOARD )


/********************************************************************
 * Function         :    void mTouchCapLED_SetLED(WORD LedNum)
 *
 * PreCondition     :    None
 *
 * Input            :    The LED number that has to be displayed.
 *
 * Output           :    None
 *
 * Side Effects     :    None
 *
 * Overview         : Invokes the appropriate display function based on PIC controller selected.
 *
 *
 * Note             :
 *******************************************************************/
#if defined (USE_DIRECT_KEYS) || defined(USE_MATRIX_KEYS)
void mTouchCapLED_SetLED(WORD LedNum)
{
#  if defined( PIC32MX220_STARTER_KIT_BOARD )
    //Do nothing - board doesn't have LEDs
#  else
    mTouchCapLED_SetLED_OnEvalBoard(LedNum);
#  endif
}
#endif
