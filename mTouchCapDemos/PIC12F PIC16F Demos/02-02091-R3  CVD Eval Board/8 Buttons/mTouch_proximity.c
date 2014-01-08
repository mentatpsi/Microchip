/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_proximity.c
 *  Dependencies:    mTouch.h
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Proximity Functions
 *                   - Implements the mTouch proximity sensor 
 *                     initialization, filtering, and decoding.
 *                   - See the documentation for more information about
 *                     implementing the framework with your application.
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
/** @file   mTouch_proximity.c
*   @brief  Implements the mTouch proximity decoding, if enabled
*/
#include "mTouch.h"

#if defined(MTOUCH_PROXIMITY_ENABLED)
    typedef struct
    {
        uint8_t         currentTime;
        uint16_t        deviation;              ///< An accumulated delta value of the proximity sensor's median-filtered readings and its baseline.
        
        uint16_t        sortBuffer  [MTOUCH_PROX_BUFFER_SIZE];
        uint8_t         timeIndex   [MTOUCH_PROX_BUFFER_SIZE];
    } mTouch_Prox_Variables;
    
    
    
    // Local Variables
    mTouch_Prox_Variables   mTouch_proxVariables[MTOUCH_NUMBER_PROXIMITY];
    
    
    void            mTouch_ProxInit             (void);
    void            mTouch_ProxUpdateBaseline   (void);
    uint8_t         mTouch_ProxStateMachine     (void);

    
    #if MTOUCH_NUMBER_PROXIMITY > 1
    const uint8_t   mTouch_proxIndex    [MTOUCH_NUMBER_SENSORS] = MTOUCH_PROXINDEX_ARRAY_INIT;
    #endif

void mTouch_ProxInit(void)
{
    #if MTOUCH_NUMBER_PROXIMITY > 1
    for (int8_t j = MTOUCH_NUMBER_PROXIMITY-1; j >= 0; j--)
    {
        #define MTOUCH_PROX_tempIndex   j
    #else
        #define MTOUCH_PROX_tempIndex   0
    #endif
    
        mTouch_proxVariables[MTOUCH_PROX_tempIndex].currentTime = 0;
        mTouch_proxVariables[MTOUCH_PROX_tempIndex].deviation   = 0;
        
        for (int8_t i = MTOUCH_PROX_BUFFER_SIZE-1; i >= 0; i--)
        {
            mTouch_proxVariables[MTOUCH_PROX_tempIndex].sortBuffer[i] = 0;
            mTouch_proxVariables[MTOUCH_PROX_tempIndex].timeIndex[i]  = i;
        }  
        
    #if MTOUCH_NUMBER_PROXIMITY > 1
    }    
    #endif
}    


//================================================================================================
//          _____                _         ____                _           _ _         ____                     _      
//  _ __ __|_   _|__  _   _  ___| |__     |  _ \ _ __ _____  _(_)_ __ ___ (_) |_ _   _|  _ \  ___  ___ ___   __| | ___ 
// | '_ ` _ \| |/ _ \| | | |/ __| '_ \    | |_) | '__/ _ \ \/ / | '_ ` _ \| | __| | | | | | |/ _ \/ __/ _ \ / _` |/ _ \
// | | | | | | | (_) | |_| | (__| | | |   |  __/| | | (_) >  <| | | | | | | | |_| |_| | |_| |  __/ (_| (_) | (_| |  __/
// |_| |_| |_|_|\___/ \__,_|\___|_| |_|___|_|   |_|  \___/_/\_\_|_| |_| |_|_|\__|\__, |____/ \___|\___\___/ \__,_|\___|
//                                   |_____|                                     |___/  
//================================================================================================


uint8_t mTouch_ProxStateMachine(void)
{    

    #if MTOUCH_NUMBER_PROXIMITY == 1
    
        #define     MTOUCH_PROX_time            mTouch_proxVariables[0].timeIndex
        #define     MTOUCH_PROX_value           mTouch_proxVariables[0].sortBuffer
        #define     MTOUCH_PROX_currentTime     mTouch_proxVariables[0].currentTime
        #define     MTOUCH_PROX_deviation       mTouch_proxVariables[0].deviation
        #define     MTOUCH_PROX_state           mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].state
        #define     MTOUCH_PROX_timer           mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].timer
        #define     MTOUCH_PROX_debounce        mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].debounce
        #define     MTOUCH_PROX_timeout         mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].timeout
        
    #else

        #define     MTOUCH_PROX_time            (*prox).timeIndex
        #define     MTOUCH_PROX_value           (*prox).sortBuffer
        #define     MTOUCH_PROX_currentTime     (*prox).currentTime
        #define     MTOUCH_PROX_deviation       (*prox).deviation
        #define     MTOUCH_PROX_state           (*sensor).state
        #define     MTOUCH_PROX_timer           (*sensor).timer
        #define     MTOUCH_PROX_debounce        (*sensor).debounce
        #define     MTOUCH_PROX_timeout         (*sensor).timeout

    mTouch_Prox_Variables*  prox            = &mTouch_proxVariables[mTouch_proxIndex[MTOUCH_CURRENTSCAN_VALUE]];
    mTouch_SensorVariables* sensor          = &mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE];
    
    #endif
    
    #if defined(MTOUCH_PROX_USE_32BIT_ACCUM)    
    uint32_t    mean        = 0;                // Stores the mean reading of the prox sensor
    #else
    uint16_t    mean        = 0;                // Stores the mean reading of the prox sensor
    #endif
    
    // Perform median sort and filter logic
    do
    {
        uint16_t    newValue    = mTouch_GetSensor(MTOUCH_CURRENTSCAN_VALUE);
        uint8_t     newTime     = MTOUCH_PROX_currentTime;
        uint8_t     i           = 0;  
        
        while (MTOUCH_PROX_time[i] != MTOUCH_PROX_currentTime)  // As long as we have not hit the time index that
        {                                                       //  will be replaced in the array, continue.
            if (MTOUCH_PROX_value[i] > newValue)                // If the value in the sorted array is greater
            {                                                   //  than the new value, drop the new value here
                uint16_t tempValue  = MTOUCH_PROX_value[i];     //  and pick up the value in the array.
                uint8_t tempTime    = MTOUCH_PROX_time[i];
                
                MTOUCH_PROX_value[i] = newValue;                //  Drop the new value
                MTOUCH_PROX_time [i] = newTime;
                
                newValue            = tempValue;                //  Now we start shifting these values up through
                newTime             = tempTime;                 //  the sorted list until we hit the time index
            }                                                   //  that is to be replaced.
            i++;
        }
        
        MTOUCH_PROX_value[i]    = newValue;         // Remove the value at the current time index
        MTOUCH_PROX_time[i]     = newTime;          //  and place the held value there.
        
        for (i++; i < MTOUCH_PROX_BUFFER_SIZE; i++)             // Now, loop through the values larger than
        {                                                       //  the one we just removed. 
            if (newValue > MTOUCH_PROX_value[i])                // If the value in the sorted array is less
            {                                                   //  than the new value, push the value in the
                MTOUCH_PROX_value[i-1]  = MTOUCH_PROX_value[i]; //  array down one location, and move the new
                MTOUCH_PROX_time [i-1]  = MTOUCH_PROX_time[i];  //  value up.
                
                MTOUCH_PROX_value[i]    = newValue;
                MTOUCH_PROX_time[i]     = newTime;
            }
            else break;                                         // If the value in the sorted array is now 
        }                                                       //  greater than the new value, stop.
        
        if (++MTOUCH_PROX_currentTime >= MTOUCH_PROX_BUFFER_SIZE)   
        {                                                       // Increment the current-time index value and    
            MTOUCH_PROX_currentTime = 0;                        //  rollover, if necessary.
        }
        
    } while(0);
    
    // Sum the median values
    do                          
    {
    
    //
    //                      MTOUCH_PROX_REMOVE_EXTREME value vs. which array locations
    //                      are used to calculate the mean of the proximity sensor.
    //                      ==========================================================
    //  Buffer Index Value |0   1   2   3   4   5   6   7   8   9   10  11  12  13  14|
    //                     |==========================================================|
    //  Size             5 |1   2   -   2   1|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|
    //     of            9 |1   2   3   4   -   4   3   2   1|XXXXXXXXXXXXXXXXXXXXXXXX|
    //      Buffer      15 |1   2   3   4   5   6   7   -   7   6   5   4   3   2   1 |
    //                      ==========================================================
    //
        #if (MTOUCH_PROX_BUFFER_SIZE == 15) 
            #if     (MTOUCH_PROX_REMOVE_EXTREME == 1)
                #define MTOUCH_MEAN_SCALE   13
            #elif   (MTOUCH_PROX_REMOVE_EXTREME == 2)
                #define MTOUCH_MEAN_SCALE   11
            #elif   (MTOUCH_PROX_REMOVE_EXTREME == 3)
                #define MTOUCH_MEAN_SCALE   9
            #elif   (MTOUCH_PROX_REMOVE_EXTREME == 4)
                #define MTOUCH_MEAN_SCALE   7
            #elif   (MTOUCH_PROX_REMOVE_EXTREME == 5)
                #define MTOUCH_MEAN_SCALE   5
            #elif   (MTOUCH_PROX_REMOVE_EXTREME == 6)
                #define MTOUCH_MEAN_SCALE   3
            #elif   (MTOUCH_PROX_REMOVE_EXTREME == 7)
                #define MTOUCH_MEAN_SCALE   1
            #endif
        #elif (MTOUCH_PROX_BUFFER_SIZE == 9) 
            #if     (MTOUCH_PROX_REMOVE_EXTREME == 1)
                #define MTOUCH_MEAN_SCALE   7
            #elif   (MTOUCH_PROX_REMOVE_EXTREME == 2)
                #define MTOUCH_MEAN_SCALE   5
            #elif   (MTOUCH_PROX_REMOVE_EXTREME == 3)
                #define MTOUCH_MEAN_SCALE   3
            #elif   (MTOUCH_PROX_REMOVE_EXTREME == 4)
                #define MTOUCH_MEAN_SCALE   1
            #endif
        #elif (MTOUCH_PROX_BUFFER_SIZE == 5)
            #if     (MTOUCH_PROX_REMOVE_EXTREME == 1)
                #define MTOUCH_MEAN_SCALE   3
            #elif   (MTOUCH_PROX_REMOVE_EXTREME == 2)
                #define MTOUCH_MEAN_SCALE   1
            #endif
        #endif
        
        #if ((MTOUCH_PROX_BUFFER_SIZE == 15) && (MTOUCH_PROX_REMOVE_EXTREME < 2)) || \
            ((MTOUCH_PROX_BUFFER_SIZE ==  9) && (MTOUCH_PROX_REMOVE_EXTREME < 2)) || \
            ((MTOUCH_PROX_BUFFER_SIZE ==  5) && (MTOUCH_PROX_REMOVE_EXTREME < 2))
        mean += MTOUCH_PROX_value[1];
        #endif
        #if ((MTOUCH_PROX_BUFFER_SIZE == 15) && (MTOUCH_PROX_REMOVE_EXTREME < 3)) || \
            ((MTOUCH_PROX_BUFFER_SIZE ==  9) && (MTOUCH_PROX_REMOVE_EXTREME < 3)) || \
             (MTOUCH_PROX_BUFFER_SIZE ==  5)
        mean += MTOUCH_PROX_value[2];
        #endif
        #if ((MTOUCH_PROX_BUFFER_SIZE == 15) && (MTOUCH_PROX_REMOVE_EXTREME < 4)) || \
            ((MTOUCH_PROX_BUFFER_SIZE ==  9) && (MTOUCH_PROX_REMOVE_EXTREME < 4)) || \
            ((MTOUCH_PROX_BUFFER_SIZE ==  5) && (MTOUCH_PROX_REMOVE_EXTREME < 2))
        mean += MTOUCH_PROX_value[3];
        #endif
        #if ((MTOUCH_PROX_BUFFER_SIZE == 15) && (MTOUCH_PROX_REMOVE_EXTREME < 5)) || \
             (MTOUCH_PROX_BUFFER_SIZE ==  9)
        mean += MTOUCH_PROX_value[4];
        #endif
        #if ((MTOUCH_PROX_BUFFER_SIZE == 15) && (MTOUCH_PROX_REMOVE_EXTREME < 6)) || \
            ((MTOUCH_PROX_BUFFER_SIZE ==  9) && (MTOUCH_PROX_REMOVE_EXTREME < 4))
        mean += MTOUCH_PROX_value[5];
        #endif
        #if ((MTOUCH_PROX_BUFFER_SIZE == 15) && (MTOUCH_PROX_REMOVE_EXTREME < 7)) || \
            ((MTOUCH_PROX_BUFFER_SIZE ==  9) && (MTOUCH_PROX_REMOVE_EXTREME < 3))
        mean += MTOUCH_PROX_value[6];
        #endif
        #if  (MTOUCH_PROX_BUFFER_SIZE == 15)                                      || \
            ((MTOUCH_PROX_BUFFER_SIZE ==  9) && (MTOUCH_PROX_REMOVE_EXTREME < 2))
        mean += MTOUCH_PROX_value[7];
        #endif
        #if ((MTOUCH_PROX_BUFFER_SIZE == 15) && (MTOUCH_PROX_REMOVE_EXTREME < 7))
        mean += MTOUCH_PROX_value[8];
        #endif
        #if ((MTOUCH_PROX_BUFFER_SIZE == 15) && (MTOUCH_PROX_REMOVE_EXTREME < 6))
        mean += MTOUCH_PROX_value[9];
        #endif
        #if ((MTOUCH_PROX_BUFFER_SIZE == 15) && (MTOUCH_PROX_REMOVE_EXTREME < 5))
        mean += MTOUCH_PROX_value[10];
        #endif
        #if ((MTOUCH_PROX_BUFFER_SIZE == 15) && (MTOUCH_PROX_REMOVE_EXTREME < 4))
        mean += MTOUCH_PROX_value[11];
        #endif
        #if ((MTOUCH_PROX_BUFFER_SIZE == 15) && (MTOUCH_PROX_REMOVE_EXTREME < 3))
        mean += MTOUCH_PROX_value[12];
        #endif
        #if ((MTOUCH_PROX_BUFFER_SIZE == 15) && (MTOUCH_PROX_REMOVE_EXTREME < 2))
        mean += MTOUCH_PROX_value[13];
        #endif
        
        // Now scale the mean value by the number of summed values.
        // These code snippets implement division by a fixed integer using
        // bit-shifts only. This keeps the execution time and program
        // memory requirements low. Since we'll be performing this to
        // every value the same way, the fact that the divisor is not
        // exactly the desired number is not a problem.
        do 
        {
            #if defined(MTOUCH_PROX_USE_32BIT_ACCUM)
            uint32_t tempCopy = 0;                        // Stores the mean reading of the prox sensor
            #else            
            uint16_t tempCopy = 0;                // Stores the mean reading of the prox sensor
            #endif
        #if     (MTOUCH_MEAN_SCALE == 13)       // Actual Divisor for this snippet: 12.96
            mean     >>= 4;     // 4
            tempCopy += mean;
            mean     >>= 3;     // 7
            tempCopy += mean;
            mean     >>= 1;     // 8
            tempCopy += mean;
            mean     >>= 1;     // 9
            tempCopy += mean;
            mean     >>= 1;     // 10
            mean     += tempCopy;
        #elif   (MTOUCH_MEAN_SCALE == 11)       // Actual Divisor for this snippet: 11.01
            mean     >>= 4;     // 4
            tempCopy += mean;
            mean     >>= 2;     // 6
            tempCopy += mean;
            mean     >>= 1;     // 7
            tempCopy += mean;
            mean     >>= 1;     // 8
            tempCopy += mean;
            mean     >>= 2;     // 10
            mean     += tempCopy;
        #elif   (MTOUCH_MEAN_SCALE == 9)        // Actual Divisor for this snippet: 8.98
            mean     >>= 4;     // 4
            tempCopy += mean;
            mean     >>= 1;     // 5
            tempCopy += mean;
            mean     >>= 1;     // 6
            tempCopy += mean;
            mean     >>= 3;     // 9
            mean     += tempCopy;
        #elif   (MTOUCH_MEAN_SCALE == 7)        // Actual Divisor for this snippet: 7.01
            mean     >>= 3;     // 3
            tempCopy += mean;
            mean     >>= 3;     // 6
            tempCopy += mean;
            mean     >>= 3;     // 9
            mean     += tempCopy;
        #elif   (MTOUCH_MEAN_SCALE == 5)        // Actual Divisor for this snippet: 4.995
            mean     >>= 3;     // 3
            tempCopy += mean;
            mean     >>= 1;     // 4
            tempCopy += mean;
            mean     >>= 3;     // 7
            tempCopy += mean;
            mean     >>= 1;     // 8
            tempCopy += mean;
            mean     >>= 2;     // 10
            mean     += tempCopy;
        #elif   (MTOUCH_MEAN_SCALE == 3)        // Actual Divisor for this snippet: 3.003
            mean     >>= 2;     // 2
            tempCopy += mean;
            mean     >>= 2;     // 4
            tempCopy += mean;
            mean     >>= 2;     // 6
            tempCopy += mean;
            mean     >>= 2;     // 8
            tempCopy += mean;
            mean     >>= 2;     // 10
            mean     += tempCopy;
        #endif
    } while (0);
    } while (0);
 
 
    // Now, with the new mean value, update the deviation amount.     
    //
    // NOTE: This code attempts to integrate the delta values over time. It can add distance
    //       to the proximity sensor, but has not been made easily configurable yet. Variable
    //       overflow is possible unless an experienced programmer can ensure it won't happen
    //       by adjusting the size of variables.
    //
    //    #define MTOUCH_PROX_DEV_GAIN    2
    //    MTOUCH_PROX_deviation  -= (uint16_t)(MTOUCH_PROX_deviation >> MTOUCH_PROX_DEV_GAIN); 
    //    MTOUCH_PROX_deviation  += mean - mTouch_GetAverage(MTOUCH_CURRENTSCAN_VALUE);
    //    
    //    if ((mean - mTouch_GetAverage(MTOUCH_CURRENTSCAN_VALUE)) > 10)
    //    {
    //        MTOUCH_PROX_deviation  += mean - mTouch_GetAverage(MTOUCH_CURRENTSCAN_VALUE);
    //    }

    // For now, treat the deviation variable as a simple 'delta' value for the state machine.
    MTOUCH_PROX_deviation = mean - mTouch_GetAverage(MTOUCH_CURRENTSCAN_VALUE);
    if ((int16_t)MTOUCH_PROX_deviation < 0)
    {
        MTOUCH_PROX_deviation = 0;          // Delta value is not allowed to go negative.
    }
    
    // Update the raw data array for this sensor by storing the median-filtered value.
    mTouch_sensorData[MTOUCH_CURRENTSCAN_VALUE] = mean;
    
    // Proximity State Machine
    //
    // NOTE: You may notice some strong similarities between this state machine and the
    //       regular button state machine. Long term goal is to combine these states and have
    //       unique code only for the pre-state-machine logic.
    switch(MTOUCH_PROX_state)
    {
        case MTOUCH_PROX_INITIALIZING:
            if (--(MTOUCH_PROX_timer) <= 0)     // If initialization delay has elapsed
            {                                   // NOTE: This delay determined by
                                                //       MTOUCH_POWER_UP_SAMPLES
                //-----------------------------
                // State Transition -> Released
                //-----------------------------
                MTOUCH_PROX_state               = MTOUCH_PROX_NOT_ACTIVATED;    // Sensor is stable. Buffer has been filled.
                #if defined(MTOUCH_STATE_DEBOUNCE)                              // Initialize sensor's debounce counter
                MTOUCH_PROX_debounce            = MTOUCH_pDEBOUNCE_VALUE;   
                #endif
                
                MTOUCH_PROX_deviation           = 0;                    // Reset the accumulated deviation
                
                mTouch_average[MTOUCH_CURRENTSCAN_VALUE] = mean;
            }
            else                                                        // If initialization delay is not complete
            {
                mTouch_state.areInitialized     = 0;                    // Disable data transfer until initialized
                
                #if MTOUCH_NUM_MODES > 1
                mTouch_state.allReleased        = 0;                    // Clear the all-sensors-are-released flag.
                #endif
            }
            return 0;
            
        case MTOUCH_PROX_NOT_ACTIVATED:
        
            // If reading has crossed the press threshold
            if(MTOUCH_PROX_deviation > (int16_t)mTouch_GetPressThreshold(MTOUCH_CURRENTSCAN_VALUE))  
            {
                #if defined(MTOUCH_STATE_DEBOUNCE)
                if (MTOUCH_PROX_debounce == 0)                          // Check the debounce counter
                {                                                       
                    MTOUCH_PROX_debounce  = MTOUCH_rDEBOUNCE_VALUE;     // Initialize the pressed state's debounce variable
                #endif
                
                    //-----------------------------
                    // State Transition -> Pressed
                    //-----------------------------
                    MTOUCH_PROX_state     = MTOUCH_PROX_ACTIVATED;      // Sensor is now pressed
                    
                    #if defined(MTOUCH_BUTTON_TIMEOUT)                  // If the press timer is enabled
                    #if MTOUCH_BUTTON_TIMEOUT > 0
                    MTOUCH_PROX_timer    = MTOUCH_pTIMEOUT_VALUE;       // Initialize the press timer counter
                    #endif                                              // NOTE: This delay determined by
                    #endif                                              //       MTOUCH_BUTTON_TIMEOUT
                    
                    #if MTOUCH_NUM_MODES > 1
                    mTouch_state.allReleased = 0;                       // Clear the all-sensors-are-released flag.
                    #endif
                    
                    mTouch_state.buttonStateChange = 1;                 // Set flag notifying that a change has occured
                                                                        // to a sensor's 'button' state in this decode
                    
                #if defined(MTOUCH_STATE_DEBOUNCE)
                }
                else
                {
                    MTOUCH_PROX_debounce--;
                }
                #endif
            }
            else                                                        // If the reading has not crossed the press threshold
            {
                #if defined(MTOUCH_STATE_DEBOUNCE)
                MTOUCH_PROX_debounce      = MTOUCH_pDEBOUNCE_VALUE;     // Reset the debounce variable   
                #endif
                
                // NOTE: This is the only state (MTOUCH_RELEASED) and condition 
                //       (delta < PressThreshold) where the average is able to
                //       be updated. Also note how we only use the reading to
                //       update the baseline after we have verified it is not
                //       crossing any thresholds.
                return 1; 
            }
            return 0; 
            
        case MTOUCH_PROX_ACTIVATED:
            #if defined(MTOUCH_BUTTON_TIMEOUT)                      // (Only if the press timer has been enabled)
            #if MTOUCH_BUTTON_TIMEOUT > 0   

            if (--MTOUCH_PROX_timer <= 0)                           // Decrement the press timer counter
            {     
                //-----------------------------
                // State Transition -> Released
                //-----------------------------
                MTOUCH_PROX_state     = MTOUCH_PROX_NOT_ACTIVATED;  // Sensor is now released
                MTOUCH_PROX_timeout   = 1;                          // Signal that a timeout has occurred
                
                mTouch_average[MTOUCH_CURRENTSCAN_VALUE]   = mTouch_GetSensor(MTOUCH_CURRENTSCAN_VALUE);
                
                #if defined(MTOUCH_STATE_DEBOUNCE)
                MTOUCH_PROX_debounce = MTOUCH_pDEBOUNCE_VALUE;      // Initialize the debounce counter for the MTOUCH_PRESSED state
                #endif                                              // and check if it has reached 0.
                
                mTouch_state.buttonStateChange = 1;                 // Set flag notifying that a change has occured
                                                                    // to a sensor's 'button' state in this decode
                break;
            }
            
            #endif
            #endif
            
            // Look only at the threshold to determine release.
            if(MTOUCH_PROX_deviation < (int16_t)mTouch_GetReleaseThreshold(MTOUCH_CURRENTSCAN_VALUE))   
            {                                                       // If the reading has crossed the release threshold
            
                #if defined(MTOUCH_STATE_DEBOUNCE)
                if (MTOUCH_PROX_debounce == 0)                      // Check if the debounce counter has reached 0.
                {                                               
                    MTOUCH_PROX_debounce = MTOUCH_pDEBOUNCE_VALUE;
                #endif
                
                    //-----------------------------
                    // State Transition -> Released
                    //-----------------------------
                    MTOUCH_PROX_state = MTOUCH_PROX_NOT_ACTIVATED;  // Sensor is now released
                    
                    mTouch_state.buttonStateChange = 1;             // Set flag notifying that a change has occured
                                                                    // to a sensor's 'button' state in this decode
                    
                #if defined(MTOUCH_STATE_DEBOUNCE)
                } else {
                
                    MTOUCH_PROX_debounce--;
                
                    #if MTOUCH_NUM_MODES > 1
                    mTouch_state.allReleased = 0;                   // Clear the all-sensors-are-released flag.
                    #endif
                    
                }
                #endif    
                
            }
            else                                                    // If the reading has not crossed the release threshold
            {
            
                #if defined(MTOUCH_STATE_DEBOUNCE)
                MTOUCH_PROX_debounce  = MTOUCH_rDEBOUNCE_VALUE;     // Reset the debounce counter
                #endif                    
                #if MTOUCH_NUM_MODES > 1
                mTouch_state.allReleased = 0;                       // Clear the all-sensors-are-released flag.
                #endif
                
            }
            return 0;
    }
    return 0;
}


#endif