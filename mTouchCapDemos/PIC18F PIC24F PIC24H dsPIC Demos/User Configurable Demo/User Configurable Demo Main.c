/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        User Configurable Demo Main.c
 *  Dependencies:    system.h, mTouch.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This demo can be used as a start point for the 
 *                   custom application. All PIC18F, PIC24 and dsPIC
 *                   devices are supported.
 *                   Hardware depended information such as PIC configuration
 *                   bits and channels mapping is located in
 *                   ...\Configurations\User_Board folder.
 *                   Also device specific ADC and CTMU types selection
 *                   is done in mTouchConfig.h file.
 *
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
// This variable is used to store the sliders values.
// Uncomment it if sliders are used in the project.
// UINT16 sliderValue;

#ifdef MTOUCH_DEBUG
DEBUGDELAY*   pDelay;
#endif

    // Board hardware initialization.
    SysBoardInit();

    // STEP 1
    // mTouch library initialization.
    MTouchInit();

    // STEP 2
    // Define number of sensors in MTOUCH_SENSORS_NUMBER parameter in mTouchConfig.h.
    // Sensors initialization. All sensors must be initialized.
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

    // All ANALOG CHANNELS information is PREDEFINED in system.h file.
	// Refer to the PIC datasheet for the available analog channels for the specific PIC device.
                  
    // sensor #0
    MTouchSetSensor(0, CH0_TRIS, CH0_LAT, CH0_IO_BIT, CH0_AN_NUM, -1, -1, -1); // AN0
    // sensor #1
    MTouchSetSensor(1, CH1_TRIS, CH1_LAT, CH1_IO_BIT, CH1_AN_NUM, -1, -1, -1); // AN1 

    // Add more sensors here.


#ifdef MTOUCH_DEBUG
	// MTouchDebugDelay(sensorNumber) function calculates an optimal CTMU charge delay value
    // to provide charging sensor about 75% of AVdd.
    pDelay = MTouchDebugDelay(0);
    // Set the adjusted optimal CTMU charge delay value for the sensor.
    MTouchSetChargeDelay(0, pDelay->delay);

    pDelay = MTouchDebugDelay(1);
    MTouchSetChargeDelay(1, pDelay->delay);

    // Debug charge delay for more sensors here.

#endif

    // STEP 3
    // Controls initialization.


    // Define number of buttons in MTOUCH_BUTTONS_NUMBER parameter in mTouchConfig.h.
    // Buttons initialization. All buttons must be initialized.
    // For example if button 0 is connected to sensor 5
    // MTouchSetButton(0,              // button number
    //                 5,              // sensor number
    //                 DECODE_TOGGLE); // decode method (see MTOUCHCONTROLDECODE enumeration in mTouchControl.h)

    // Button #0 and #1. Only button with a bigger signal is indicated as pressed.
    MTouchSetButton(0, 0, DECODE_MOST_PRESSED|DECODE_PRESS_REPEAT|DECODE_ONE_EVENT);


    // Define number of matrix buttons in MTOUCH_MATRIXBUTTONS_NUMBER parameter in mTouchConfig.h.
    // Matrix buttons initialization. All buttons must be initialized.
    // For example if matrix button is connected to sensor 5 and sensor 11
    // MTouchSetMatrixButton(0,              // button number
    //                 5,              // first sensor number
    //                11,              // second sensor number      
    //                 DECODE_PRESS_REPEAT|DECODE_ONE_EVENT); // decode method (see MTOUCHCONTROLDECODE enumeration in mTouchControl.h)


    // Define number of 2 channel sliders in MTOUCH_2CHSLIDERS_NUMBER parameter in mTouchConfig.h.
    // 2 channel slider initialization. All sliders must be initialized.
    // For example if slider is connected to sensor 5 and sensor 11
    // MTouchSet2ChSlider(0,           // slider number
    //                    5,           // first sensor number
    //                    11);         // second sensor number

    // Define number of 4 channel sliders in MTOUCH_4CHSLIDERS_NUMBER parameter in mTouchConfig.h.
    // 4 channel slider initialization. All 4 channel sliders must be initialized.
    // For example if slider is connected to sensors #6, #3, #5 and #11
    // MTouchSet4ChSlider(0,              // slider number
    //                    6,              // 1st sensor number
    //                    3,              // 2nd sensor number
    //                    5,              // 3rd sensor number
    //                   11);             // 4th sensor number


    // STEP 4
    // Timer interrupt initialization to call mTouch acquisition pereodically.
    SysTimerInit();


    while(1)
    {

        // STEP 5
        // Decode all controls. Not required for Basic Buttons.
        MTouchDecode();


        // STEP 6
        // Get current states of the controls.

        // Basic Buttons.
        if(MTouchGetSensorState(0) == SENSOR_PRESSED)
        {
            // Put your code here.
        }

        // Buttons with decoding.
        if(MTouchGetButtonState(0) == CONTROL_PRESSED)
        {
            // Put your code here.
        }

        // Matrix Buttons.
        //if(MTouchGetMatrixButtonState(0) == CONTROL_PRESSED)
        //{
        //   // Put your code here.
        //}

        // 2 Channel Sliders.
        //if(MTouchGet2ChSliderState(0) == CONTROL_PRESSED)
        //{
        //    sliderValue = MTouchGet2ChSliderValue(0);
        //    // Put your code here.
        //}

        // 4 Channel Sliders.
        //if(MTouchGet4ChSliderState(0) == CONTROL_PRESSED)
        //{
        //    sliderValue = MTouchGet4ChSliderValue(0);
        //    // Put your code here.
        //}
     
    }

}


// This callback function called every 1ms by timer.
void SysTimerInterrupt(void)
{
   // Get samples from sensors.
   MTouchAcquisition(); 
}
