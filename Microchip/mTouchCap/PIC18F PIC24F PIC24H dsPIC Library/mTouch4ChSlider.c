/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                  
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouch4ChSlider.c
 *  Dependencies:    mTouch.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This file contains all variables and functions
 *                   related to 4 channels slider.
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

#if (MTOUCH_4CHSLIDERS_NUMBER > 0)

///////////////////////////////
// GLOBAL VARIABLES
///////////////////////////////
MTOUCH4CHSLIDER  sliders4ch[MTOUCH_4CHSLIDERS_NUMBER];

///////////////////////////////
// FUNCTIONS
/////////////////////////////// 
void MTouchSet4ChSlider(UINT8 sliderNumber, UINT8 ch1SensorNumber, UINT8 ch2SensorNumber, UINT8 ch3SensorNumber, UINT8 ch4SensorNumber)
{
MTOUCH4CHSLIDER* pSlider;

     pSlider = (MTOUCH4CHSLIDER*)sliders4ch + sliderNumber;

     pSlider->state = CONTROL_RELEASED;
     pSlider->value = 0;
     pSlider->pCh1Sensor = (SENSOR*)sensors + ch1SensorNumber;
     pSlider->pCh2Sensor = (SENSOR*)sensors + ch2SensorNumber;
     pSlider->pCh3Sensor = (SENSOR*)sensors + ch3SensorNumber;
     pSlider->pCh4Sensor = (SENSOR*)sensors + ch4SensorNumber;
}


void MTouchDecode4ChSliders(void)
{
MTOUCH4CHSLIDER* pSlider = (MTOUCH4CHSLIDER*)sliders4ch;

UINT8         ch1SensorState;
UINT8         ch2SensorState;
UINT8         ch3SensorState;
UINT8         ch4SensorState;

UINT32        ch1Delta;
UINT32        ch2Delta;
UINT32        ch3Delta;
UINT32        ch4Delta;

UINT32        sum;

UINT8         i;

UINT8         sensorState;

INT32         value;


for(i=0; i<MTOUCH_4CHSLIDERS_NUMBER; i++)
{

    ch1SensorState = pSlider->pCh1Sensor->state;
    ch2SensorState = pSlider->pCh2Sensor->state;
    ch3SensorState = pSlider->pCh3Sensor->state;
    ch4SensorState = pSlider->pCh4Sensor->state;
  
	if( (ch1SensorState == SENSOR_PRESSED) ||
        (ch2SensorState == SENSOR_PRESSED) ||
        (ch3SensorState == SENSOR_PRESSED) ||
        (ch4SensorState == SENSOR_PRESSED) )
	{
	    sensorState = SENSOR_PRESSED;
        pSlider->state = CONTROL_PRESSED;
	}else{
        pSlider->state = CONTROL_RELEASED;
        if( (ch1SensorState == SENSOR_INITIALIZING) ||
            (ch2SensorState == SENSOR_INITIALIZING) ||
            (ch3SensorState == SENSOR_INITIALIZING) ||
            (ch4SensorState == SENSOR_INITIALIZING) )
		{
	        sensorState = SENSOR_INITIALIZING;
		}else{
	        sensorState = SENSOR_RELEASED;
		}
	}

    if(sensorState == SENSOR_PRESSED)
    {
        ch1Delta = pSlider->pCh1Sensor->delta;
        ch2Delta = pSlider->pCh2Sensor->delta;
        ch3Delta = pSlider->pCh3Sensor->delta;
        ch4Delta = pSlider->pCh4Sensor->delta;

        sum = ch1Delta + ch2Delta + ch3Delta + ch4Delta;

        if(sum > 0)
        {
            value = ((333*ch2Delta)+(666*ch3Delta)+(1000*ch4Delta))/sum;


            pSlider->value -= pSlider->value>>AVG_SLIDER_VALUE;
            pSlider->value += value>>AVG_SLIDER_VALUE;

            if(pSlider->value > 1000)
            {
                pSlider->value = 1000;
            }

        }

    } // end of if

    pSlider++; // move to the next slider

} //end of for


}

#endif
