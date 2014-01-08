/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        Gesture Demo Main.c
 *  Dependencies:    system.h, mTouch.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This demo explains how to use mTouch buttons to
 *                   detect a gesture. 8 buttons plugin module must be
 *                   connected to the low part of the
 *                   sensors connector (channels from 0 to 7). The LEDs
 *                   indicate direction of movement on the keyboard.
 *
 *                   Some sensors inputs can be used for programming.
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

#define GESTURE_SHOW_DELAY	20 // ms

// This function reads buttons states and decode a gesture.
void DecodeGesture(void);

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
    MTouchSetSensor(0, CH0_TRIS, CH0_LAT, CH0_IO_BIT, CH0_AN_NUM, -1, -1, -1);
    // sensor #1
    MTouchSetSensor(1, CH1_TRIS, CH1_LAT, CH1_IO_BIT, CH1_AN_NUM, -1, -1, -1);  
    // sensor #2
    MTouchSetSensor(2, CH2_TRIS, CH2_LAT, CH2_IO_BIT, CH2_AN_NUM, -1, -1, -1);  
    // sensor #3
    MTouchSetSensor(3, CH3_TRIS, CH3_LAT, CH3_IO_BIT, CH3_AN_NUM, -1, -1, -1);  
    // sensor #4
    MTouchSetSensor(4, CH4_TRIS, CH4_LAT, CH4_IO_BIT, CH4_AN_NUM, -1, -1, -1);  
    // sensor #5
    MTouchSetSensor(5, CH5_TRIS, CH5_LAT, CH5_IO_BIT, CH5_AN_NUM, -1, -1, -1);  
    // sensor #6
    MTouchSetSensor(6, CH6_TRIS, CH6_LAT, CH6_IO_BIT, CH6_AN_NUM, -1, -1, -1);  
    // sensor #7
    MTouchSetSensor(7, CH7_TRIS, CH7_LAT, CH7_IO_BIT, CH7_AN_NUM, -1, -1, -1);

#ifdef MTOUCH_DEBUG
    // Debug log initialization (allows streaming data to PC GUI). PC GUI helps to
    // choose right values for sensors' oversampling factors and thresholds.
    SysLogInit();

    // MTouchDebugDelay(sensorNumber) function calculates CTMU charge delay value
    // to provide charging sensor about 75% of AVdd.
    pDelay = MTouchDebugDelay(0);
    // Set adjusted CTMU charge delay value for the sensor.
    MTouchSetChargeDelay(0, pDelay->delay);
    pDelay = MTouchDebugDelay(1);
    MTouchSetChargeDelay(1, pDelay->delay);
    pDelay = MTouchDebugDelay(2);
    MTouchSetChargeDelay(2, pDelay->delay);
    pDelay = MTouchDebugDelay(3);
    MTouchSetChargeDelay(3, pDelay->delay);
    pDelay = MTouchDebugDelay(4);
    MTouchSetChargeDelay(4, pDelay->delay);
    pDelay = MTouchDebugDelay(5);
    MTouchSetChargeDelay(5, pDelay->delay);
    pDelay = MTouchDebugDelay(6);
    MTouchSetChargeDelay(6, pDelay->delay);
    pDelay = MTouchDebugDelay(7);
    MTouchSetChargeDelay(7, pDelay->delay);
#endif

    // STEP 3
    // Timer interrupt initialization to call mTouch acquisition pereodically.
    SysTimerInit();


    while(1)
    {

        // STEP 4
        // Get current states of the buttons and decode a gesture.
        DecodeGesture();


        #ifdef MTOUCH_DEBUG
           // Log data from sensors to PC GUI.
           MTouchDebugLogDeltas();

        #endif

     
    }

}

void DecodeGesture(void)
{
static UINT8 curButton = 0;
UINT8        prevButton;

	prevButton = curButton;

    // button #0
    if(MTouchGetSensorState(0) == SENSOR_PRESSED) { curButton = 0; }
    // button #1
    if(MTouchGetSensorState(1) == SENSOR_PRESSED) { curButton = 1; }
    // button #2
    if(MTouchGetSensorState(2) == SENSOR_PRESSED) { curButton = 2; }
    // button #3
    if(MTouchGetSensorState(3) == SENSOR_PRESSED) { curButton = 3; }
    // button #4
    if(MTouchGetSensorState(4) == SENSOR_PRESSED) { curButton = 4; }
    // button #5
    if(MTouchGetSensorState(5) == SENSOR_PRESSED) { curButton = 5; }
    // button #6
    if(MTouchGetSensorState(6) == SENSOR_PRESSED) { curButton = 6; }
    // button #7
    if(MTouchGetSensorState(7) == SENSOR_PRESSED) { curButton = 7; }

    if(prevButton == curButton)
    {
        return; 
    }

	if( (curButton == 0 && prevButton == 1) ||
        (curButton == 1 && prevButton == 2) ||
        (curButton == 2 && prevButton == 3) ||
        (curButton == 4 && prevButton == 5) ||
        (curButton == 5 && prevButton == 6) ||
        (curButton == 6 && prevButton == 7) )
    {
        Led0On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led1On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
        Led2On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led3On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
        Led4On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led5On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
        Led6On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led7On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();

    } else

	if( (curButton == 1 && prevButton == 0) ||
        (curButton == 2 && prevButton == 1) ||
        (curButton == 3 && prevButton == 2) ||
        (curButton == 5 && prevButton == 4) ||
        (curButton == 6 && prevButton == 5) ||
        (curButton == 7 && prevButton == 6) )
    {
        Led7On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led6On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
        Led5On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led4On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
        Led3On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led2On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
        Led1On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led0On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
    } else

	if( (curButton == 0 && prevButton == 4) ||
        (curButton == 1 && prevButton == 5) ||
        (curButton == 2 && prevButton == 6) ||
        (curButton == 3 && prevButton == 7) )
    {
        Led8On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led9On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
        Led10On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led11On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
        Led12On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led13On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
        Led14On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led15On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();

    } else

	if( (curButton == 4 && prevButton == 0) ||
        (curButton == 5 && prevButton == 1) ||
        (curButton == 6 && prevButton == 2) ||
        (curButton == 7 && prevButton == 3) )
    {
        Led15On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led14On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
        Led13On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led12On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
        Led11On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led10On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
        Led9On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();Led8On();SysDelayMs(GESTURE_SHOW_DELAY);Led_ALLOff();
    }
}


// This callback function called every 1ms by timer.
void SysTimerInterrupt(void)
{
   // Get samples from sensors.
   MTouchAcquisition(); 
}
