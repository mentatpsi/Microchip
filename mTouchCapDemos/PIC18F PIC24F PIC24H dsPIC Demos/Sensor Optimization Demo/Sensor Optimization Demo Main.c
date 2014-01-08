/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        Sensor Optimization Demo Main.c
 *  Dependencies:    system.h, mTouch.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This demo explains how to select optimal parameters
 *                   for a sensor. PLEASE READ "SENSOR OPTIMIZATION
 *                   (DEBUG MODULE)" CHAPTER IN THE HELP FILE FOR MORE
 *                   DETAILS. 8 buttons plugin module must be connected
 *                   to the low part of the sensors connector (channels
 *                   from 0 to 7).
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

#include "system.h" 
#include "mTouch.h"

//////////////////////////////////////////////////////////
//                  GLOBAL VARIABLES
//////////////////////////////////////////////////////////
// This structure will contain the optimal CTMU current.
volatile DEBUGCURRENT* pOptimalCurrent;
// This structure will contain the optimal CTMU charge delay.
volatile DEBUGDELAY* pOptimalDelay;
// This variable will contain the optimal threshold.
volatile UINT16     optimalThreshold;

RET_MAIN main(void)
{
    // Board hardware initialization.
    SysBoardInit();

    // STEP 1
    // mTouch library initialization.
    MTouchInit();

    // STEP 2
    // Sensors initialization. Set default parameters.
    // For example if sensor is connected to RB1/AN2 pin
    // MTouchSetSensor(0,      // sensor number
    //                 &TRISB, // port B
    //                 &LATB,
    //                 1,      // IO bit number
    //                 2,      // analog channel number
    //                 -1,     // press detection threshold by default (see MTOUCH_DEFAULT_THRESHOLD in mTouchConfig.h)
    //                 -1,     // oversampling by default (see MTOUCH_DEFAULT_OVERSAMPLING in mTouchConfig.h)
    //                 -1 );   // CTMU charge delay by default (see MTOUCH_DEFAULT_CHARGE_DELAY in mTouchConfig.h, not used for CVD acquisition)

                  
    // sensor #0
    MTouchSetSensor(0, CH0_TRIS, CH0_LAT, CH0_IO_BIT, CH0_AN_NUM, -1, -1, -1);
    
	// STEP 3
    // MTouchDebugCurrent(sensorNumber) function calculates the optimal CTMU 
	// current value (required CTMU charge delay will be about 8).
	// This will be a final value for MTOUCH_CTMU_CURRENT parameter in mTouchConfig.h.
	// Before measurement set MTOUCH_CTMU_CURRENT to 0x01.
	// Sensor #0 is tested.
	pOptimalCurrent = (volatile DEBUGCURRENT*)MTouchDebugCurrent(0);
	// Set adjusted CTMU current value.
    MTouchSetCTMUCurrent(pOptimalCurrent->current);

    // STEP 4 
    // MTouchDebugDelay(sensorNumber) function calculates the optimal 
	// CTMU charge delay value to provide charging of sensor to about 75% of AVdd.
	// Optimal delay for sensor #0.
    pOptimalDelay = (volatile DEBUGDELAY*)MTouchDebugDelay(0);
	// Set adjusted CTMU charge delay value for the sensor # 0
    MTouchSetChargeDelay(0, pOptimalDelay->delay);

    // STEP 5
    // MTouchDebugThreshold(sensorNumber) function calculates the optimal
	// press detection threshold value. It waits for the sensor press event 
	// from user to measure maximum signal amplitude (delta).
	// Optimal threshold for sensor #0.
    optimalThreshold = MTouchDebugThreshold(0);
	// Set adjusted threshold value for the sensor # 0
    MTouchSetThreshold(0, optimalThreshold);

	// STEP 6
    // Put break point here. Use Watch Window to see pOptimalCurrent->current,
	// pOptimalDelay->delay and optimalThreshold values.
    while(1)
    {
        Nop();
    }
}

// Timer interrupt is not used.
void SysTimerInterrupt()
{
}

