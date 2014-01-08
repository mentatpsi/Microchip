
/*****************************************************************************
 * FileName:        	mTouch_Keys.c
 * Dependencies:
 * Processor:       	PIC18, PIC24
 * Compiler:       		C18, C30
 * Linker:          	MPLINK,MPLAB LINK30
 * Company:         	Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright � 2009 Microchip Technology Inc.  
 * Microchip licenses this software to you solely for use with Microchip products, according to the terms of the  
 * accompanying click-wrap software license.  Microchip and its licensors retain all right, title and interest in 
 * and to the software.  All rights reserved. This software and any accompanying information is for suggestion only.  
 * It shall not be deemed to modify Microchip�s standard warranty for its products.  It is your responsibility to ensure 
 * that this software meets your requirements.

 * SOFTWARE IS PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS LICENSORS BE LIABLE 
 * FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO INCIDENTAL, 
 * SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, 
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, OR ANY CLAIMS BY THIRD PARTIES 
 * (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.  
 * The aggregate and cumulative liability of Microchip and its licensors for damages related to the use of the software 
 * will in no event exceed $1000 or the amount you paid Microchip for the software, whichever is greater.

 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS AND 
 * THE TERMS OF THE ACCOMPANYING CLICK-WRAP SOFTWARE LICENSE.
 * 
 *
 * Author               		Date        		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 *****************************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~	Includes	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  	*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "mTouchCap/PIC18F PIC24F PIC24H dsPIC Library/mTouch.h"
#include "mTouchKeys.h"
#include "RF4CEController.h"
#include "led.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~	Function Definitions  ~~~~~~~~~~~~~~~~~~~~~~~~~~~  	*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/********************************************************************
 * Function			:   void mTouch_CreateKeys(void);
 *
 * PreCondition		:   None
 *
 * Input			:   None
 *
 * Output			:   None
 *
 * Side Effects		:   None
 *
 * Overview			:   This function will setup the channels and other related properties 
 *				  	 	that are associated with the corresonding Direct key.
 *
 *
 * Note				:
 *******************************************************************/
 void mTouch_CreateKeys(void)
 {
    // mTouch Initialization
    MTouchInit();

    // Store info about sensors
    // sensor VOL_PLUS is connected to RB11/AN11 pin
    MTouchSetSensor(
                   VOL_PLUS, // sensor number 
                   &TRISB, // port B
                   &LATB,
                   11, // bit number
                   11, // A2D channel number
                   -1, // press detection threshold
                   -1, // oversampling factor
                   -1  // CTMU charge delay 
                   );
  
    MTouchSetSensor(VOL_MINUS, &TRISB, &LATB, 10, 10, -1, -1, -1);  
    MTouchSetSensor(CH_PLUS,   &TRISC, &LATC, 4,  16, -1, -1, -1);  
    MTouchSetSensor(CH_MINUS,  &TRISG, &LATG, 6,  17, -1, -1, -1);  
    MTouchSetSensor(UP,        &TRISB, &LATB, 6,   6, -1, -1, -1);  
    MTouchSetSensor(DOWN,      &TRISE, &LATE, 9,  21, -1, -1, -1);  
    MTouchSetSensor(LEFT,      &TRISB, &LATB, 7,   7, -1, -1, -1);  
    MTouchSetSensor(RIGHT,     &TRISG, &LATG, 8,  19, -1, -1, -1);  
    MTouchSetSensor(OK,        &TRISB, &LATB, 5,   5, -1, -1, -1);  
    MTouchSetSensor(MUTE,      &TRISG, &LATG, 9,  20, -1, -1, -1);  
    MTouchSetSensor(BACK,      &TRISG, &LATG, 7,  18, -1, -1, -1);  

    // Store info about buttons
    MTouchSetButton(
                   VOL_PLUS, // button number 
                   VOL_PLUS, // sensor number
                   DECODE_PRESS_REPEAT|DECODE_PRESS_REPEAT|DECODE_ONE_EVENT // decoding method
                   );
  
    MTouchSetButton(VOL_MINUS, VOL_MINUS, DECODE_MOST_PRESSED|DECODE_PRESS_REPEAT|DECODE_ONE_EVENT);  
    MTouchSetButton(CH_PLUS,   CH_PLUS,   DECODE_MOST_PRESSED|DECODE_PRESS_REPEAT|DECODE_ONE_EVENT);  
    MTouchSetButton(CH_MINUS,  CH_MINUS,  DECODE_MOST_PRESSED|DECODE_PRESS_REPEAT|DECODE_ONE_EVENT);  
    MTouchSetButton(UP,        UP,        DECODE_MOST_PRESSED|DECODE_ONE_EVENT);  
    MTouchSetButton(DOWN,      DOWN,      DECODE_MOST_PRESSED|DECODE_ONE_EVENT);  
    MTouchSetButton(LEFT,      LEFT,      DECODE_MOST_PRESSED|DECODE_ONE_EVENT);  
    MTouchSetButton(RIGHT,     RIGHT,     DECODE_MOST_PRESSED|DECODE_ONE_EVENT);  
    MTouchSetButton(OK,        OK,        DECODE_MOST_PRESSED|DECODE_ONE_EVENT);  
    MTouchSetButton(MUTE,      MUTE,      DECODE_MOST_PRESSED|DECODE_ONE_EVENT);  
    MTouchSetButton(BACK,      BACK,      DECODE_MOST_PRESSED|DECODE_ONE_EVENT);

}

/********************************************************************
 * Function			:   void mTouch_ProcessKeys(void)
 *
 * PreCondition		:   None
 *
 * Input			:   None
 *
 * Output			:   None
 *
 * Side Effects		:   None
 *
 * Overview			:   This function will send a RF command of the pressed key.
 *
 *
 * Note				:
 *******************************************************************/

void mTouch_ProcessKeys(void)
{
    MTouchDecode();

    if(MTouchGetButtonState(VOL_PLUS) == CONTROL_PRESSED)
    {
       LEDPulse(GREEN1);
       RF4CEControllerVolumnUp();
           }
    else if(MTouchGetButtonState(VOL_MINUS) == CONTROL_PRESSED)
    {
       LEDPulse(GREEN1);
       RF4CEControllerVolumnDown();
           }
    else  if(MTouchGetButtonState(CH_PLUS) == CONTROL_PRESSED)
    {
       LEDPulse(GREEN1);
       RF4CEControllerChannelUp();
           }
    else  if(MTouchGetButtonState(CH_MINUS) == CONTROL_PRESSED)
    {
       LEDPulse(GREEN1);
       RF4CEControllerChannelDown();
           }
    else  if(MTouchGetButtonState(UP) == CONTROL_PRESSED)
    {
       LEDPulse(GREEN1);
       RF4CEControllerUp();
           }
    else  if(MTouchGetButtonState(DOWN) == CONTROL_PRESSED)
    {
       LEDPulse(GREEN1);
       RF4CEControllerDown();
           }  
    else  if(MTouchGetButtonState(LEFT) == CONTROL_PRESSED)
    {
       LEDPulse(GREEN1);
       RF4CEControllerLeft();
           }
    else  if(MTouchGetButtonState(RIGHT) == CONTROL_PRESSED)
    {
       LEDPulse(GREEN1);
       RF4CEControllerRight();
           } 
    else  if(MTouchGetButtonState(OK) == CONTROL_PRESSED)
    {
       LEDPulse(GREEN1);
       RF4CEControllerSelect();
           }   
    else  if(MTouchGetButtonState(MUTE) == CONTROL_PRESSED)
    {
       LEDPulse(GREEN1);
       RF4CEControllerMute();
           }   
    else  if(MTouchGetButtonState(BACK) == CONTROL_PRESSED)
    {
       LEDPulse(GREEN1);
       RF4CEControllerPreviousChannel();
           }          
    else
    {
    }
}


