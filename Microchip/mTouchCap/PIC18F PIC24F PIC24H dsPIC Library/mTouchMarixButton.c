/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                        
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouchMatrixButton.c
 *  Dependencies:    mTouch.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch matrix button module.
 *                   This module provides a group of functions for
 *                   setting up and decoding matrix buttons.
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

///////////////////////////////
// INCLUDES 
///////////////////////////////
#include "mTouch.h"

#if (MTOUCH_MATRIXBUTTONS_NUMBER > 0)

///////////////////////////////
// GLOBAL VARIABLES
///////////////////////////////
MTOUCHMATRIXBUTTON matrixButtons[MTOUCH_MATRIXBUTTONS_NUMBER];


///////////////////////////////
// FUNCTIONS
///////////////////////////////
#define MTouchGetMatrixButtonDelta(pButton)  (pButton->pCh1Sensor->delta+pButton->pCh2Sensor->delta)


void MTouchSetMatrixButton(UINT8 buttonNumber, UINT8 ch1SensorNumber, UINT8 ch2SensorNumber, UINT8 decode)
{
MTOUCHMATRIXBUTTON* pButton;

     pButton = (MTOUCHMATRIXBUTTON*)matrixButtons + buttonNumber;

     pButton->pCh1Sensor = (SENSOR*)sensors + ch1SensorNumber;
     pButton->pCh2Sensor = (SENSOR*)sensors + ch2SensorNumber;

     pButton->state = CONTROL_RELEASED|CONTROL_IDLE;
     pButton->curState = CONTROL_RELEASED;
     pButton->decode = decode;
     pButton->pressCounter = MCONTROL_REPEAT_INITIAL_DELAY;
     pButton->prevSensorState = SENSOR_RELEASED;
}

void MTouchDecodeMatrixButtons(void)
{
MTOUCHMATRIXBUTTON* pButton = (MTOUCHMATRIXBUTTON*)matrixButtons;

MTOUCHMATRIXBUTTON* pMostPressedButton = NULL;

UINT8         ch1SensorState;

UINT8         ch2SensorState;

UINT8         i;

UINT8         sensorState;

UINT8         state;

UINT8         decode;

for(i=0; i<MTOUCH_MATRIXBUTTONS_NUMBER; i++)
{

    ch1SensorState = pButton->pCh1Sensor->state;
    ch2SensorState = pButton->pCh2Sensor->state;
  
	if( (ch1SensorState == SENSOR_PRESSED) && (ch2SensorState == SENSOR_PRESSED) )
	{
	    sensorState = SENSOR_PRESSED;
	}else{
        if( (ch1SensorState == SENSOR_INITIALIZING) || (ch2SensorState == SENSOR_INITIALIZING) )
		{
	        sensorState = SENSOR_INITIALIZING;
		}else{
	        sensorState = SENSOR_RELEASED;
		}
	}

    if(sensorState != SENSOR_INITIALIZING)
    {

        decode = pButton->decode;

        state = pButton->curState;

        if(decode&DECODE_PRESS_REPEAT)
        {

            if(state == CONTROL_RELEASED)
            {
                pButton->pressCounter = MCONTROL_REPEAT_INITIAL_DELAY;

            }else{
               
                if(pButton->pressCounter > 0)
                {
                    state = CONTROL_PRESSED;
                }else{
                    state = CONTROL_RELEASED;
                }                   

                if(pButton->pressCounter < -MCONTROL_REPEAT_DELAY)
                {
                    pButton->pressCounter = MCONTROL_REPEAT_DELAY;
                }                   

                pButton->pressCounter--;
            }
        }


        if(decode&DECODE_ONE_EVENT)
        {
            if(pButton->state&state)
            {
                state |= CONTROL_IDLE;   
            }
        }

        pButton->state = state;    

        
        if(decode&DECODE_TOGGLE)
        {
            if(sensorState != pButton->prevSensorState)
            {
                if(sensorState == SENSOR_PRESSED)
                {
                    if(pButton->curState == CONTROL_PRESSED)
                    {
                        pButton->curState = CONTROL_RELEASED;   
                    }else{
                        pButton->curState = CONTROL_PRESSED;   
                    }
    
                }
            }
            pButton->prevSensorState = sensorState;
        }else

        if(decode&DECODE_MOST_PRESSED)
        {
            pButton->curState = CONTROL_RELEASED;
            if(sensorState == SENSOR_PRESSED)
            {
                if(pMostPressedButton == NULL)
                {
                    pMostPressedButton = pButton;
                }else{
                   if( MTouchGetMatrixButtonDelta(pButton) > MTouchGetMatrixButtonDelta(pMostPressedButton))
                   {                          
                       pMostPressedButton = pButton;
                   }
                }
            }
        }else{

            if(sensorState == SENSOR_PRESSED)
            {
                pButton->curState = CONTROL_PRESSED;   
            }else{
                pButton->curState = CONTROL_RELEASED;
            }

        }

    } // end of if

    pButton++; // move to the next button

} //end of for

if(pMostPressedButton != NULL)
{
    pMostPressedButton->curState = CONTROL_PRESSED;
}

}

#endif
