/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        Proximity Demo Main.c
 *  Dependencies:    system.h, mTouch.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This demo explains how to use mTouch basic button as
 *                   a proximity sensor. 8 buttons plugin module must be
 *                   connected to the low part of the sensors connector
 *                   (channels from 0 to 7). The button '8' 
 *                   indicates press/release state on hand approach.
 *                   LED 15 shows that the proximity sensor is still 
 *                   initializing.
 *
 *                   Noise from debugger/programmer can influence on operation.
 *                   Disconnect programmer/debugger from the board for
 *                   the correct operation.
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

RET_MAIN main(void)
{
#ifdef MTOUCH_DEBUG
DEBUGDELAY* pDelay;
#endif
    // Board hardware initialization.
    SysBoardInit();

    // STEP 1
    // mTouch library initialization.
    MTouchInit();

    // STEP 2
    // Sensors initialization. All sensors must be initialized (see MTOUCH_SENSORS_NUMBER in mTouchConfig.h).
	// PLEASE READ "SENSOR OPTIMIZATION (DEBUG MODULE)" CHAPTER IN THE HELP FILE TO SELECT OPTIMAL PARAMETERS.
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
    // SET VERY BIG OVERSAMPLING FACTOR AND SMALL THRESHOLD TO INCREASE SENSITIVITY
    MTouchSetSensor(0, CH0_TRIS, CH0_LAT, CH0_IO_BIT, CH0_AN_NUM, 20, 500, -1);

#ifdef MTOUCH_DEBUG
    // Debug log initialization (allows streaming data to PC GUI). PC GUI helps to
    // choose right values for sensors' oversampling factors and thresholds.
    SysLogInit();

    // MTouchDebugDelay(sensorNumber) function calculates CTMU charge delay value
    // to provide charging sensor about 75% of AVdd.
    pDelay = MTouchDebugDelay(0);
    // Set adjusted CTMU charge delay value for the sensor.
    MTouchSetChargeDelay(0, pDelay->delay);
#endif

    // STEP 3
    // Timer interrupt initialization to call mTouch acquisition pereodically.
    SysTimerInit();


    while(1)
    {

        // STEP 4
        // Get current states of the buttons.

        Led_ALLOff();
        // sensor #0
        if(MTouchGetSensorState(0) == SENSOR_INITIALIZING) { Led15On(); }

        // button #0
        if(MTouchGetSensorState(0) == SENSOR_PRESSED) { Led0On(); }


        #ifdef MTOUCH_DEBUG
           // Log data from sensors to PC GUI.
           MTouchDebugLogDeltas();

        #endif

     
    }

}


// This callback function called every 1ms by timer.
void SysTimerInterrupt(void)
{
   // Get samples from sensors.
   MTouchAcquisition(); 
}
