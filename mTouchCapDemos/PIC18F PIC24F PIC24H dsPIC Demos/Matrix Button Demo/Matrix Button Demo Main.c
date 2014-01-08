/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        Matrix Button Demo Main.c
 *  Dependencies:    system.h, mTouch.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This demo explains how to use mTouch matrix buttons.
 *                   mTouch Matrix plugin module must be connected to
 *                   the low part of the sensors connector (channels from 0 to 6).
 *                   The buttons are decoded differently:
 *                   "0","1","2" and "3" - DECODE_MOST_PRESSED (only button
 *                                 with a bigger signal is indicated as pressed),
 *                   "4","5","6" and "7" - DECODE_TOGGLE (toggled buttons),
 *                   "8","9","10" and "11" - DECODE_PRESS_REPEAT (if they are held
 *                                 in pressed state the buttons will toggle
 *                                 pressed/released state continuously).
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
#endif

    // STEP 3
    // Matrix buttons initialization. All buttons must be initialized (see MTOUCH_MATRIXBUTTONS_NUMBER in mTouchConfig.h).
    // For example if matrix button is connected to sensor 5 and sensor 11
    // MTouchSetMatrixButton(0,              // button number
    //                 5,              // first sensor number
    //                11,              // second sensor number      
    //                 DECODE_PRESS_REPEAT|DECODE_ONE_EVENT); // decode method (see MTOUCHCONTROLDECODE enumeration in mTouchControl.h)

    // button #0
    MTouchSetMatrixButton(0, 0, 6,  DECODE_MOST_PRESSED);
    // button #1
    MTouchSetMatrixButton(1, 1, 6,  DECODE_MOST_PRESSED);
    // button #2
    MTouchSetMatrixButton(2, 2, 6,  DECODE_MOST_PRESSED);
    // button #3
    MTouchSetMatrixButton(3, 3, 6,  DECODE_MOST_PRESSED);
    // button #4
    MTouchSetMatrixButton(4, 0, 5,  DECODE_TOGGLE);
    // button #5
    MTouchSetMatrixButton(5, 1, 5,  DECODE_TOGGLE);
    // button #6
    MTouchSetMatrixButton(6, 2, 5,  DECODE_TOGGLE);
    // button #7
    MTouchSetMatrixButton(7, 3, 5,  DECODE_TOGGLE);
    // button #8
    MTouchSetMatrixButton(8, 0, 4,  DECODE_PRESS_REPEAT);
    // button #9
    MTouchSetMatrixButton(9, 1, 4,  DECODE_PRESS_REPEAT);
    // button #10
    MTouchSetMatrixButton(10, 2, 4, DECODE_PRESS_REPEAT);
    // button #11
    MTouchSetMatrixButton(11, 3, 4, DECODE_PRESS_REPEAT);

    // STEP 4
    // Timer interrupt initialization to call mTouch acquisition pereodically.
    SysTimerInit();


    while(1)
    {

        // STEP 5
        // Decode all controls.
        MTouchDecode();


        // STEP 6
        // Get current states of the buttons.

        Led_ALLOff();
        // button #0
        if(MTouchGetMatrixButtonState(0) == CONTROL_PRESSED) { Led0On(); }
        // button #1
        if(MTouchGetMatrixButtonState(1) == CONTROL_PRESSED) { Led1On(); }
        // button #2
        if(MTouchGetMatrixButtonState(2) == CONTROL_PRESSED) { Led2On(); }
        // button #3
        if(MTouchGetMatrixButtonState(3) == CONTROL_PRESSED) { Led3On(); }
        // button #4
        if(MTouchGetMatrixButtonState(4) == CONTROL_PRESSED) { Led4On(); }
        // button #5
        if(MTouchGetMatrixButtonState(5) == CONTROL_PRESSED) { Led5On(); }
        // button #6
        if(MTouchGetMatrixButtonState(6) == CONTROL_PRESSED) { Led6On(); }
        // button #7
        if(MTouchGetMatrixButtonState(7) == CONTROL_PRESSED) { Led7On(); }
        // button #8
        if(MTouchGetMatrixButtonState(8) == CONTROL_PRESSED) { Led8On(); }
        // button #9
        if(MTouchGetMatrixButtonState(9) == CONTROL_PRESSED) { Led9On(); }
        // button #10
        if(MTouchGetMatrixButtonState(10) == CONTROL_PRESSED) { Led10On(); }
        // button #11
        if(MTouchGetMatrixButtonState(11) == CONTROL_PRESSED) { Led11On(); }


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
