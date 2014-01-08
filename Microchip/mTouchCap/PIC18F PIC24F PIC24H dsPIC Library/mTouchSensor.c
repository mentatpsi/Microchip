/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                        
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouchSensor.c
 *  Dependencies:    mTouch.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch sensors filtering and decoding module.
 *                   This module provides a group of functions performing
 *                   acquisition and the post acquisition filtering,
 *                   decoding for all sensors.
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
  
///////////////////////////////
// GLOBAL VARIABLES
///////////////////////////////
#if defined(__18CXX) && !defined(__PICC18__)
#pragma udata bigdata
#endif
volatile SENSOR      sensors[MTOUCH_SENSORS_NUMBER];    // The array contains configuration data for each sensor.
#if defined(__18CXX) && !defined(__PICC18__)
#pragma udata
#endif

///////////////////////////////
// LOCAL PROTOTYPES
///////////////////////////////
void MTouchUpdateSensorAverage(SENSOR* pSensor, UINT16 rate);
void MTouchSensorDecode (SENSOR* pSensor);

///////////////////////////////
// FUNCTIONS
///////////////////////////////
void MTouchSetSensor(UINT8 sensorNumber,SFR tris, SFR lat,
                     UINT8 ioBitNumber, UINT8 channelNumber, INT16 threshold, INT16 oversampling, INT8 chargeDelay)
{
SENSOR* pSensor;

    pSensor = (SENSOR*)sensors + sensorNumber;

    if(threshold > 0)
    {
        pSensor->pressThreshold = threshold;
        pSensor->releaseThreshold = threshold>>1;
    }else{
        pSensor->pressThreshold = MTOUCH_DEFAULT_THRESHOLD;
        pSensor->releaseThreshold = MTOUCH_DEFAULT_THRESHOLD>>1;
    }
	pSensor->trisReg = tris;
	pSensor->latReg =  lat;
    pSensor->ioMask = 1<<ioBitNumber;
	pSensor->channelNumber = channelNumber;
    pSensor->state = SENSOR_INITIALIZING;
    pSensor->debounceCount = DEBOUNCE_COUNT + POWER_UP_SAMPLES;       
    

    if(oversampling > 0)
    {
        pSensor->samplesNumber = oversampling;
    }else{
        pSensor->samplesNumber = MTOUCH_DEFAULT_OVERSAMPLING;
    }

     pSensor->sampleCounter = pSensor->samplesNumber;

#ifdef MTOUCH_USE_CTMU
    if(chargeDelay >= 0)
    {    
       pSensor->chargeDelay = chargeDelay;
    }else{
       pSensor->chargeDelay = MTOUCH_DEFAULT_CHARGE_DELAY;
    }
#endif

    pSensor->delta = 0;
    pSensor->oversampleFilterData = 0;
    pSensor->decimateFilterData = 0;
}


void MTouchInit(void)
{

    MTouchADCInit();
#ifdef MTOUCH_USE_CTMU
    MTouchCTMUInit();
#endif
   
}


void MTouchSensorDecode ( SENSOR* pSensor )
{

INT16 delta;

    delta = pSensor->oversampleFilterData - pSensor->average; 

    if(delta < 0)                         
    {
        delta = 0;
    }

    pSensor->delta = delta;
   
    switch(pSensor->state)
    {
    	case SENSOR_INITIALIZING:    
        	{
                if (pSensor->debounceCount-- <= 0)
                {
                    pSensor->state = SENSOR_RELEASED;
                    pSensor->debounceCount = DEBOUNCE_COUNT; 
                }
                pSensor->average = pSensor->oversampleFilterData;
                break; 
        	}

                                                  
        case SENSOR_RELEASED:
            {                  
                if(delta > pSensor->pressThreshold)
                {
                    if (pSensor->debounceCount-- <= 0)
                    {
                        pSensor->state = SENSOR_PRESSED;
                        pSensor->debounceCount = DEBOUNCE_COUNT;
                    }
                }
                else
                {
                    pSensor->debounceCount = DEBOUNCE_COUNT;
                }

                MTouchUpdateSensorAverage(pSensor, AVG_RATE_RELEASED);

                break;
                
            }

        case SENSOR_PRESSED:
            {            
                if(delta < pSensor->releaseThreshold) 
                {
                    if (pSensor->debounceCount-- <= 0)
                    {
                        pSensor->state = SENSOR_RELEASED;
                        pSensor->debounceCount = DEBOUNCE_COUNT; 
                    }
                }
                else
                {
                    pSensor->debounceCount = DEBOUNCE_COUNT;
                }

                MTouchUpdateSensorAverage(pSensor, AVG_RATE_PRESSED);

                break;
            }
            
            default: break; 
            
    }
      
    pSensor->oversampleFilterData = 0;
 
} 


void MTouchUpdateSensorAverage(SENSOR* pSensor, UINT16 rate)
{
    if(pSensor->averageCount++ >= rate)
    {
        pSensor->averageCount = 0;

        #if     (AVG_UPDATE == 2)
        
            // avg = (avg + reading) / 2   
            pSensor->average += pSensor->oversampleFilterData;
            pSensor->average >>= 1;
            
        #elif   (AVG_UPDATE == 4)                                   
        
            // avg = avg - (25% * avg) + (25% * reading)        
            pSensor->average -=  pSensor->average >> 2; 
            pSensor->average += pSensor->oversampleFilterData >> 2;
            
        #elif   (AVG_UPDATE == 8)                                   
        
            // avg = avg - (12.5% * avg) + (12.5% * reading)   
            pSensor->average -=  pSensor->average >> 3; 
            pSensor->average += pSensor->oversampleFilterData >> 3;

        #elif   (AVG_UPDATE == 16)                                  
        
            // avg = avg - (6.25% * avg) + (6.25% * reading)
            pSensor->average -=  pSensor->average >> 4; 
            pSensor->average += pSensor->oversampleFilterData >> 4;
            
        #endif
    }
}


void MTouchAcquisition(void)
{
INT16   data; 
UINT8   i;

static UINT8   startSensorNumber = 0;
SENSOR* pCurrentSensor;
SENSOR* pLastSensor;

#if defined(MTOUCH_USE_CVD)
SENSOR* pPreviousSensor;
#endif

    // Start scan from different sensor each time to create a jitter.
    // The jitter improves a noise immunity.
    pCurrentSensor = (SENSOR*)sensors + startSensorNumber;
    pLastSensor = (SENSOR*)sensors + (MTOUCH_SENSORS_NUMBER-1);

    #if defined(MTOUCH_USE_CVD)
    if(startSensorNumber == (MTOUCH_SENSORS_NUMBER-1))
    {
        pPreviousSensor = (SENSOR*)sensors;
    }else{
        pPreviousSensor = pCurrentSensor + 1;
    }
    #endif

    for(i=0; i<MTOUCH_SENSORS_NUMBER; i++)
    {
        if((pCurrentSensor->state&SENSOR_DISCONNECTED) == 0)
        {

            #if defined(MTOUCH_USE_CTMU)

                data = MTouchAcquisitionCTMU(pCurrentSensor);

            #elif defined(MTOUCH_USE_CVD)

                data = MTouchAcquisitionCVD(pCurrentSensor,pPreviousSensor);

            #endif

            // Add acquisition result to decimate filter
            if(data > pCurrentSensor->decimateFilterData)
            {
                pCurrentSensor->decimateFilterData++;
            } else {
                pCurrentSensor->decimateFilterData--;
            }

            // Add decimate filter to the oversampling filter.
            pCurrentSensor->oversampleFilterData += pCurrentSensor->decimateFilterData;

            if((pCurrentSensor->sampleCounter)-- <= 0)
            {
                MTouchSensorDecode(pCurrentSensor);
                pCurrentSensor->sampleCounter = pCurrentSensor->samplesNumber;
            }

        }

        // Move pointers to the next sensor.
        #if defined(MTOUCH_USE_CVD)
        pPreviousSensor = pCurrentSensor;
        #endif

        if(pCurrentSensor == pLastSensor)
        {
            pCurrentSensor = (SENSOR*)sensors;
            continue;
        }

        pCurrentSensor++;
    }

    startSensorNumber++;

    if(startSensorNumber > (MTOUCH_SENSORS_NUMBER-1))
    {
        startSensorNumber = 0;
    }

}
