/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                      
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouchDebug.c
 *  Dependencies:    mTouch.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This file contains debug functions.
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

#ifdef MTOUCH_DEBUG

///////////////////////////////
// FUNCTIONS
///////////////////////////////  
void LogUINT16(UINT16 value)
{
UINT8 ctr = 0;
UINT8 flg = 0;

 	while (value >= 10000)  { value -= 10000; ctr++; } if(ctr) {flg=1;} if(flg) {LogChar(ctr+0x30);} ctr=0;
 	while (value >=  1000)  { value -=  1000; ctr++; } if(ctr) {flg=1;} if(flg) {LogChar(ctr+0x30);} ctr=0;
	while (value >=   100)  { value -=   100; ctr++; } if(ctr) {flg=1;} if(flg) {LogChar(ctr+0x30);} ctr=0;
	while (value >=    10)  { value -=    10; ctr++; } if(ctr) {flg=1;} if(flg) {LogChar(ctr+0x30);} ctr=0;
	while (value >=     1)  { value -=     1; ctr++; } LogChar(ctr+0x30);	
    LogChar(LOG_DELIMITER);
}

void LogString(char* pString)
{
   while(*pString != 0)
   {
       LogChar(*pString);
       pString++;
   }
}

DEBUGCURRENT* MTouchDebugCurrent(UINT8 sensorNumber)
{
volatile static  DEBUGCURRENT data;

UINT8   currentResult = 0;
INT16   errorResult = 0;

#ifdef MTOUCH_USE_CTMU

DEBUGDELAY* pDelay;

INT16   error;
INT16   errorMin;
INT8    irng;
INT8    itrim;

    errorMin = 4096;

    currentResult = 0;
    errorResult = 0;

	for(irng=DEBUG_CTMU_IRNG_MIN; irng<=DEBUG_CTMU_IRNG_MAX; irng++)
	{
        for(itrim=DEBUG_CTMU_ITRIM_MIN; itrim<=DEBUG_CTMU_ITRIM_MAX; itrim++)
        {
            MTouchSetCTMUCurrent(irng|(itrim<<2));

            pDelay = MTouchDebugDelay(sensorNumber);

            error = pDelay->delay - (INT16)DEBUG_OPTIMAL_CHARGE_DELAY; 

            if(error <= errorMin)
            {
                errorMin = error;
                currentResult = irng|(itrim<<2);
                errorResult = error;
            }

            if(error <= 0)
            {
                goto debug_current_end;
            }
        }
    }

debug_current_end:

#endif

    data.current = currentResult;
    data.error   = errorResult;

    return ((DEBUGCURRENT*)&data);
}

DEBUGDELAY* MTouchDebugDelay(UINT8 sensorNumber)
{
volatile static  DEBUGDELAY data;

INT8    delayResult = 0;
INT16   errorResult = 0;

#ifdef MTOUCH_USE_CTMU
SENSOR* pSensor;
INT16   error;
INT16   errorMin;
UINT8   chargeDelay;
INT16   sample;

    pSensor = (SENSOR*)sensors + sensorNumber;

    delayResult = 0;
    errorResult = 0;

    errorMin = 4096;

    for(chargeDelay=0; chargeDelay<128; chargeDelay++)
    {
        pSensor->chargeDelay = chargeDelay;

        sample = MTouchAcquisitionCTMU(pSensor);

#ifdef MTOUCH_USE_12_BITS_ADC
        error = sample - (INT16)(4096/DEBUG_OPTIMAL_CHARGE_DIVIDER);
#else
        error = sample - (INT16)(1024/DEBUG_OPTIMAL_CHARGE_DIVIDER);
#endif

        if(error < 0)
        {
            error = -error;
        }


        if(error <= errorMin)
        {
            errorMin = error;
            delayResult = (INT8)chargeDelay;
            errorResult = error;
        }

    }

#endif

    data.delay   = delayResult;
    data.error   = errorResult;

    return ((DEBUGDELAY*)&data);
}

INT16 MTouchDebugThreshold(UINT8 sensorNumber)
{
UINT32 timeout;
INT32  maxDelta;
INT32  delta;

    maxDelta = 0xffffffff;
    timeout = DEBUG_THRESHOLD_CALC_TIMEOUT;

    MTouchSetThreshold(sensorNumber, DEBUG_NOISE_PER_SAMPLE*sensors[sensorNumber].samplesNumber);

	while(timeout--){

        MTouchAcquisition();     	
	
		if(MTouchGetSensorState(sensorNumber) == SENSOR_PRESSED)
		{
		   delta = (INT32)sensors[sensorNumber].delta;
		   if(maxDelta <= delta)
		   {
		     maxDelta = delta; 
		   }else{
		     break;
		   }
		}
	}
     
    return ((INT16)(maxDelta/DEBUG_OPTIMAL_THRESHOLD_DIVIDER));
}

void MTouchDebugLogDeltas(void)
{   
UINT8   i;
SENSOR* pSensor;
UINT16  states;
UINT16  mask;

    pSensor = (SENSOR*)sensors;
    states = 0;
    mask = 1;

    for(i=0; i<MTOUCH_SENSORS_NUMBER; i++)
    {
        if(pSensor->state == SENSOR_PRESSED)
        {
            states |= mask;
        }
        mask <<= 1;
        pSensor++;
    }

    LogUINT16(states);
    LogUINT16(0);
    LogUINT16(0);

    pSensor = (SENSOR*)sensors;

    for(i=0; i<MTOUCH_SENSORS_NUMBER; i++)
    {
        LogUINT16(pSensor->delta);
        pSensor++;
    }

    LogChar(0x0D);
    LogChar(0x0A);

}

void MTouchDebugLogAverages(void)
{
UINT8   i;
SENSOR* pSensor;
UINT16  states;
UINT16  mask;

    pSensor = (SENSOR*)sensors;
    states = 0;
    mask = 1;

    for(i=0; i<MTOUCH_SENSORS_NUMBER; i++)
    {
        if(pSensor->state == SENSOR_PRESSED)
        {
            states |= mask;
        }
        mask <<= 1;
        pSensor++;
    }

    LogUINT16(states);
	LogUINT16(0);
	LogUINT16(0);

    pSensor = (SENSOR*)sensors;

    for(i=0; i<MTOUCH_SENSORS_NUMBER; i++)
    {
        LogUINT16((UINT16)pSensor->average);
        pSensor++;
    }

    LogChar(0x0D);
    LogChar(0x0A);
}

void MTouchDebugLogStates(void)
{
UINT8   i;
SENSOR* pSensor;
UINT16  states;
UINT16  mask;

    pSensor = (SENSOR*)sensors;
    states = 0;
    mask = 1;

    for(i=0; i<MTOUCH_SENSORS_NUMBER; i++)
    {
        if(pSensor->state == SENSOR_PRESSED)
        {
            states |= mask;
        }
        mask <<= 1;
        pSensor++;
    }

    LogUINT16(states);
	LogUINT16(0);
	LogUINT16(0);

    LogChar(0x0D);
    LogChar(0x0A);

}


#endif
