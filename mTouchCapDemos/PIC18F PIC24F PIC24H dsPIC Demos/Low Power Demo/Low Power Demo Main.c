/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        Low Power Demo Main.c
 *  Dependencies:    system.h, mTouch.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This demo shows how to reduce power consumption.
 *                   8 buttons plugin module must be connected to the
 *                   low part of the sensors connector (channels from 0 to 7).
 *                   The LEDs from 1 to 8 indicate buttons' press/release state.
 *                   If there are no pressed buttons in some timeout the
 *                   system enters into power save mode (LED 16 is on).
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

// This counter is used to detect a few seconds timeout.
volatile UINT32 sleepTimeoutCounter;

// This flag define current mode. If it is 0 the system
// operates in normal mode. If this flag is 1 the system
// enter sleep pereodically to save power.
volatile UINT8  sleepModeFlag;

// This macro resets sleep mode timeout counter.
#define SleepResetTimeoutCounter()       {sleepTimeoutCounter = 0;}

// This macro increments sleep mode timeout counter.
#define SleepIncrementTimeoutCounter()   {sleepTimeoutCounter++;}

// This macro clears sleep mode flag.
#define SleepSetRunMode()                {sleepModeFlag = 0;}

// This macro sets sleep mode flag.
#define SleepSetSleepMode()              {sleepModeFlag = 1;}

// This macro checks the sleep mode flag state.
#define SleepIsSleepMode()               (sleepModeFlag == 1)

// This function returns 1 if sleep mode timeout event is detected.
UINT8 SleepIsTimeoutEvent(void)
{
static UINT8 prevTimeoutState = 0;

    if(sleepTimeoutCounter >= SLEEP_TIMEOUT_DELAY)
    {
        if(prevTimeoutState == 0)
        {
            prevTimeoutState = 1;

            return 1;
        }
    }else{

        prevTimeoutState = 0;
    }
    
    return 0;
}

// This function returns 1 if wakeup from sleep mode event is detected.
UINT8 SleepIsWakeUpEvent(void)
{
static UINT8 prevTimeoutState = 0;

    if(sleepTimeoutCounter < SLEEP_TIMEOUT_DELAY)
    {
        if(prevTimeoutState == 1)
        {
            prevTimeoutState = 0;

            return 1;
        }
    }else{

        prevTimeoutState = 1;
    }
    
    return 0;
}
   

RET_MAIN main(void)
{
#ifdef MTOUCH_DEBUG
DEBUGDELAY* pDelay;
#endif
UINT8  i;

    // Board hardware initialization.
    SysBoardInit();

    // Clear sleep timeout counter.
    SleepResetTimeoutCounter();

    // Clear sleep mode flag.
    SleepSetRunMode();


    // mTouch library initialization.
    MTouchInit();

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

    // Timer interrupt initialization to call mTouch acquisition pereodically.
    SysTimerInit();

    while(1)
    {
        // Get current states of the buttons.
        // If any sensor is pressed then reset sleep mode timeout counter.
        Led_ALLOff();
        // sensor #0
        if(MTouchGetSensorState(0) == SENSOR_PRESSED) { SleepResetTimeoutCounter(); Led0On();} 
        // sensor #1
        if(MTouchGetSensorState(1) == SENSOR_PRESSED) { SleepResetTimeoutCounter(); Led1On();}
        // sensor #2
        if(MTouchGetSensorState(2) == SENSOR_PRESSED) { SleepResetTimeoutCounter(); Led2On();}
        // sensor #3
        if(MTouchGetSensorState(3) == SENSOR_PRESSED) { SleepResetTimeoutCounter(); Led3On();}
        // sensor #4
        if(MTouchGetSensorState(4) == SENSOR_PRESSED) { SleepResetTimeoutCounter(); Led4On();}
        // sensor #5
        if(MTouchGetSensorState(5) == SENSOR_PRESSED) { SleepResetTimeoutCounter(); Led5On();}
        // sensor #6
        if(MTouchGetSensorState(6) == SENSOR_PRESSED) { SleepResetTimeoutCounter(); Led6On();}
        // sensor #7
        if(MTouchGetSensorState(7) == SENSOR_PRESSED) { SleepResetTimeoutCounter(); Led7On();}


        // If the timeout counter transition is from sleep to run value then
        // change mode for normal operation.
        if(SleepIsWakeUpEvent())
        {
             
            // Disable watchdog timer.
            SysDisableWDT();

            // Initialize sensors
            MTouchInitializeSensor(0);
            MTouchInitializeSensor(1);
            MTouchInitializeSensor(2);
            MTouchInitializeSensor(3);
            MTouchInitializeSensor(4);
            MTouchInitializeSensor(5);
            MTouchInitializeSensor(6);
            MTouchInitializeSensor(7);

            // Enable timer interrupt
            SysEnableTimerInterrupt();

            // Clear sleep mode flag.
            SleepSetRunMode();

        }

        // If the timeout counter transition is from run to sleep value then
        // change mode for power save operation.
        if(SleepIsTimeoutEvent())
        {

            // Disable timer interrupt
            SysDisableTimerInterrupt();

            // Enable watchdog timer to wakeup.
            SysEnableWDT();

            // Set sleep mode flag.
            SleepSetSleepMode();
           
        }


        // Check sleep mode flag.
        if(SleepIsSleepMode())
        {

            // Indicate power saving mode.
            Led15On();
            
            // Enter device sleep mode (sleep time is 1024 mS).
            // Device will wake up from watchdog timer.
            Sleep();

            // Reset watchdog timer.
            ClrWdt();

            for(i=0; i<4*MTOUCH_DEFAULT_OVERSAMPLING; i++)
            {
                // Run acquisition.
                MTouchAcquisition();

                // Reset watchdog timer.
                ClrWdt();
            }


        }else{

            SleepIncrementTimeoutCounter();

        }
            
    }

}


// This callback function called every 1ms by timer
// only for run mode. In sleep mode the timer is not used.
void SysTimerInterrupt(void)
{
   // Get samples from sensors.
   MTouchAcquisition(); 
}
