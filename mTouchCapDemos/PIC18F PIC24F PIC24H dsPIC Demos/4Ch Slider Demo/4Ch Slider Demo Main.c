/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        4Ch Slider Demo Main.c
 *  Dependencies:    system.h, mTouch.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This demo explains how to use mTouch 4 channel slider.
 *                   4 channel slider plugin module must be connected to
 *                   the low part of the sensors connector (channels from 0 to 3).
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
#endif

    // STEP 3
    // Slider initialization. All 4 channel sliders must be initialized (see MTOUCH_4CHSLIDERS_NUMBER in mTouchConfig.h).
    // For example if slider is connected to sensors #6, #3, #5 and #11
    // MTouchSet4ChSlider(0,              // slider number
    //                    6,              // 1st sensor number
    //                    3,              // 2nd sensor number
    //                    5,              // 3rd sensor number
    //                   11);             // 4th sensor number

    // slider #0
    MTouchSet4ChSlider(0, 0, 1, 2, 3);

    // STEP 4
    // Timer interrupt initialization to call mTouch acquisition pereodically.
    SysTimerInit();


    while(1)
    {

        // STEP 5
        // Decode all controls.
        MTouchDecode();


        // STEP 6
        // Get current value of the slider.
        
        if(MTouchGet4ChSliderState(0) == CONTROL_PRESSED)
        {
            Led_ALLOff();

                 if(MTouchGet4ChSliderValue(0) > 15*1000/16) { Led15On(); }

            else if(MTouchGet4ChSliderValue(0) > 14*1000/16) { Led14On(); }

            else if(MTouchGet4ChSliderValue(0) > 13*1000/16) { Led13On(); }

            else if(MTouchGet4ChSliderValue(0) > 12*1000/16) { Led12On(); }

            else if(MTouchGet4ChSliderValue(0) > 11*1000/16) { Led11On(); }

            else if(MTouchGet4ChSliderValue(0) > 10*1000/16) { Led10On(); }

            else if(MTouchGet4ChSliderValue(0) > 9*1000/16)  { Led9On(); }

            else if(MTouchGet4ChSliderValue(0) > 8*1000/16)  { Led8On(); }

            else if(MTouchGet4ChSliderValue(0) > 7*1000/16)  { Led7On(); }

            else if(MTouchGet4ChSliderValue(0) > 6*1000/16)  { Led6On(); }

            else if(MTouchGet4ChSliderValue(0) > 5*1000/16)  { Led5On(); }

            else if(MTouchGet4ChSliderValue(0) > 4*1000/16)  { Led4On(); }

            else if(MTouchGet4ChSliderValue(0) > 3*1000/16)  { Led3On(); }

            else if(MTouchGet4ChSliderValue(0) > 2*1000/16)  { Led2On(); }

            else if(MTouchGet4ChSliderValue(0) > 1*1000/16)  { Led1On(); }

            else if(MTouchGet4ChSliderValue(0) > 0)          { Led0On(); }
        }


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
