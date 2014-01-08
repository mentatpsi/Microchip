/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch.c
 *  Dependencies:    mTouch.h
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework filtering and decoding modules
 *                   - No application code should be implemented in this
 *                     or any other mTouch framework file. This will allow
 *                     for better customer support and easier upgrades to
 *                     later firmware versions. Use the main.c and user-
 *                     generated files to implement your application.
 *                   - See the documentation located in the docs/ folder
 *                     for a more information about how the framework is
 *                     implemented.
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
/** @file   mTouch.c
*   @brief  Implements the post-acquisition filtering and decoding of the
*           mTouch sensors.
*/

//========================================================================
// INCLUDES 
//========================================================================
#include "mTouch.h"
  
    
//========================================================================
// GLOBAL VARIABLES
//========================================================================
/** @name   Global Sensor Variables
* 
*   @brief  These variables store information about the current state of 
*           the sensors. They should only ever be updated by the mTouch 
*           framework, but may be read globally so that special actions 
*           can be taken based on the sensors' status.
*           Refer to the mTouch.h file for more information about these 
*           variables.
*/ 
//@{

    mTouch_State                mTouch_state;           // Collection of single-bit flags
    mTouch_StateVariables       mTouch_stateVars;       // Sensor variables and other counters
    uint16_t                    mTouch_average          [MTOUCH_NUMBER_SENSORS];    

    #if defined(MCOMM_ENABLED)
    #if (MTOUCH_NUMBER_SENSORS <= 8)
        uint8_t                 mTouch_stateMask;          
    #elif (MTOUCH_NUMBER_SENSORS <= 16)
        uint16_t                mTouch_stateMask;
    #elif (MTOUCH_NUMBER_SENSORS <= 24)
        uint24_t                mTouch_stateMask;
    #elif (MTOUCH_NUMBER_SENSORS <= 32)
        uint32_t                mTouch_stateMask;  
    #endif
    #endif

    #if defined(MTOUCH_SLIDERS_ENABLED) || defined(MTOUCH_MOST_PRESSED_ONLY) || defined(MTOUCH_MATRIX_ENABLED)
        uint16_t                mTouch_deltas           [MTOUCH_NUMBER_SENSORS];
    #endif
    
    #if defined(MTOUCH_TOGGLE_ENABLED)
    #if (MTOUCH_NUMBER_SENSORS <= 8)
        uint8_t                 mTouch_toggle;          
    #elif (MTOUCH_NUMBER_SENSORS <= 16)
        uint16_t                mTouch_toggle;
    #elif (MTOUCH_NUMBER_SENSORS <= 24)
        uint24_t                mTouch_toggle;
    #elif (MTOUCH_NUMBER_SENSORS <= 32)
        uint32_t                mTouch_toggle;  
    #endif
    #endif

    #if defined(MTOUCH_MATRIX_ENABLED) && defined(MTOUCH_MOST_PRESSED_ONLY)
    
        #define MTOUCH_MOST_PRESSED_INDEX       0
        #define MTOUCH_MATRIX_ROW_INDEX         1
        #define MTOUCH_MATRIX_COL_INDEX         2
        
        #define MTOUCH_MOSTPRESSED_ARRAYSIZE    3
        
        uint8_t                         mTouch_matrix;                  // Output value
        uint8_t                         mTouch_matrix_latch;            // Latch variable used to compare the new matrix value with the old
        mTouch_MostPressedData*         mTouch_mostPressed;             // Pointer
        mTouch_MostPressedData          mTouch_mostPressedData [3];     // Stores max-pressed sensor data
        const mTouch_MostPressedIndex   mTouch_mostPressedIndex[3] =    {   
                                                                            { MTOUCH_SENSOR_START,          MTOUCH_SENSOR_END        },
                                                                            { MTOUCH_MATRIX_ROW_START,      MTOUCH_MATRIX_ROW_END    },
                                                                            { MTOUCH_MATRIX_COLUMN_START,   MTOUCH_MATRIX_COLUMN_END }
                                                                        };
    #elif defined(MTOUCH_MATRIX_ENABLED)
    
        #define MTOUCH_MATRIX_ROW_INDEX         0
        #define MTOUCH_MATRIX_COL_INDEX         1
        #define MTOUCH_DUMMY_MOST_PRESSED_INDEX 2
        
        #define MTOUCH_MOSTPRESSED_ARRAYSIZE    2
        uint8_t                         mTouch_matrix;                  // Output value
        uint8_t                         mTouch_matrix_latch;            // Latch variable used to compare the new matrix value with the old
        mTouch_MostPressedData*         mTouch_mostPressed;             // Pointer
        mTouch_MostPressedData          mTouch_mostPressedData [3];     // Stores max-pressed sensor data
                                                                        // NOTE: The 3rd (index 2) array location is a dummy location used to
                                                                        // allow all non-matrix sensors to be implemented normally, without
                                                                        // a most-pressed algorithm limiting the number that can be
                                                                        // active at one time.
        
        const mTouch_MostPressedIndex   mTouch_mostPressedIndex[2] =    {   
                                                                            { MTOUCH_MATRIX_ROW_START,      MTOUCH_MATRIX_ROW_END    },
                                                                            { MTOUCH_MATRIX_COLUMN_START,   MTOUCH_MATRIX_COLUMN_END }
                                                                        };
    
    #elif defined(MTOUCH_MOST_PRESSED_ONLY)
    
        #define MTOUCH_MOST_PRESSED_INDEX       0
        #define MTOUCH_MOSTPRESSED_ARRAYSIZE    1
        
        mTouch_MostPressedData*         mTouch_mostPressed;             // Pointer
        mTouch_MostPressedData          mTouch_mostPressedData [1];     // Stores max-pressed sensor data
        const mTouch_MostPressedIndex   mTouch_mostPressedIndex[1] =    {   { 0,                            MTOUCH_NUMBER_SENSORS-1 } };
        
    #endif
    

//@}

//========================================================================
// CONSTANT ARRAYS
//========================================================================
/** @name   Constant Threshold Arrays
*
*   @brief  These arrays are stored in program memory at compile time, so 
*           they do not require RAM. The downside to this implementation 
*           is that their values cannot be updated at runtime. If this is 
*           something your application requires, further editing will be 
*           needed.
*/
//@{

    #if !defined(MTOUCH_EEPROM_ENABLED)
    // If EEPROM is enabled, the thresholds will be stored in EEPROM. 
    //  Otherwise we need to define a constant array for them.
    const uint16_t  mTouch_pressThreshold       [MTOUCH_NUMBER_SENSORS] = PRESS_THRESHOLD_INIT;     
    const uint16_t  mTouch_releaseThreshold     [MTOUCH_NUMBER_SENSORS] = RELEASE_THRESHOLD_INIT;   
    #endif

//@}

//========================================================================
// FUNCTION PROTOTYPES                
//========================================================================
void mTouch_Init                    (void);
void mTouch_Decode                  (void);
void mTouch_UpdateBaseline          (void);
void mTouch_ChangeState             (void);

uint8_t mTouch_ButtonStateMachine   (void);

#if defined(MTOUCH_MOST_PRESSED_ONLY) || defined(MTOUCH_MATRIX_ENABLED)
void mTouch_FindMostPressed         (uint8_t);
#endif
#if defined(MTOUCH_MATRIX_ENABLED)
void mTouch_UpdateMatrixOutput      (void);
#endif
#if defined(MTOUCH_ERROR_DETECTION_ENABLED)
void mTouch_ErrorDetect             (void);
#endif


#if (MTOUCH_NUMBER_SENSORS > 1) && ((defined(MTOUCH_PROXIMITY_ENABLED) && (MTOUCH_NUMBER_SENSORS != MTOUCH_NUMBER_PROXIMITY)))
uint8_t (*const mTouch_StateMachine [MTOUCH_NUMBER_SENSORS])(void) = {                              // Constant array of function pointers
                                                                        MTOUCH_STATEMACHINE_0 
                                                                        MTOUCH_STATEMACHINE_1 
                                                                        MTOUCH_STATEMACHINE_2 
                                                                        MTOUCH_STATEMACHINE_3 
                                                                        MTOUCH_STATEMACHINE_4 
                                                                        MTOUCH_STATEMACHINE_5 
                                                                        MTOUCH_STATEMACHINE_6 
                                                                        MTOUCH_STATEMACHINE_7 
                                                                        MTOUCH_STATEMACHINE_8 
                                                                        MTOUCH_STATEMACHINE_9 
                                                                        MTOUCH_STATEMACHINE_10 
                                                                        MTOUCH_STATEMACHINE_11 
                                                                        MTOUCH_STATEMACHINE_12 
                                                                        MTOUCH_STATEMACHINE_13 
                                                                        MTOUCH_STATEMACHINE_14 
                                                                        MTOUCH_STATEMACHINE_15 
                                                                        MTOUCH_STATEMACHINE_16 
                                                                        MTOUCH_STATEMACHINE_17 
                                                                        MTOUCH_STATEMACHINE_18 
                                                                        MTOUCH_STATEMACHINE_19 
                                                                        MTOUCH_STATEMACHINE_20 
                                                                        MTOUCH_STATEMACHINE_21 
                                                                        MTOUCH_STATEMACHINE_22 
                                                                        MTOUCH_STATEMACHINE_23 
                                                                        MTOUCH_STATEMACHINE_24 
                                                                        MTOUCH_STATEMACHINE_25 
                                                                        MTOUCH_STATEMACHINE_26 
                                                                        MTOUCH_STATEMACHINE_27 
                                                                        MTOUCH_STATEMACHINE_28 
                                                                        MTOUCH_STATEMACHINE_29 
                                                                     };
#endif

//================================================================================================
//          _____                _         ___       _ _   
//  _ __ __|_   _|__  _   _  ___| |__     |_ _|_ __ (_) |_ 
// | '_ ` _ \| |/ _ \| | | |/ __| '_ \     | || '_ \| | __|
// | | | | | | | (_) | |_| | (__| | | |    | || | | | | |_ 
// |_| |_| |_|_|\___/ \__,_|\___|_| |_|___|___|_| |_|_|\__|
//                                   |_____|   
//================================================================================================
/**
*   @brief  Main initialization routine for all mTouch framework modules including the built-in 
*           communication module, if enabled.
*/
void mTouch_Init(void)
{
    #if defined(MTOUCH_EEPROM_ENABLED)
    mTouch_EEPROM_Init();
    #endif
    
    /** @section InitImp Framework Initialization Notes
    * 1. Loop through all enabled, active sensors and initialize their variables.
    */  
    #if (MTOUCH_NUMBER_SENSORS == 1)
        #define sensorIndex 0       // If only one sensor is implemented, don't implement
                                    // the 'for' loop, just replace all 'sensorIndex'
                                    // text with the value '0'.
    #else
    // TIP: By declaring the index variable within the 'for' instead of before it, the
    //      compiler is able to recycle that register in memory immediately after the
    //      loop's execution and use it for something else. Limiting the life-span of 
    //      temporary variables such as this helps limit the total amount of temporary 
    //      variable space that is required.
    for (int8_t sensorIndex = MTOUCH_NUMBER_SENSORS-1; sensorIndex >= 0; sensorIndex--)
    {
    #endif
    
        mTouch_AcquisitionData* acqData     = &mTouch_acqData[sensorIndex];
        (*acqData).result.v &= 0x000F;                                            
        (*acqData).result.v |= (uint16_t)(0x47F << 4);  
        
        mTouch_stateVars.sensor[sensorIndex].state           = MTOUCH_INITIALIZING;
        #if defined(MTOUCH_BUTTON_TIMEOUT) && (MTOUCH_BUTTON_TIMEOUT > 0)   // (Only if the press timer has been enabled)
        mTouch_stateVars.sensor[sensorIndex].timeout         = 0;
        #endif
        mTouch_stateVars.sensor[sensorIndex].timer           = MTOUCH_POWER_UP_SAMPLES;
        mTouch_stateVars.sensor[sensorIndex].baselineCount   = MTOUCH_BASELINE_RATE_VALUE;
        #if defined(MTOUCH_STATE_DEBOUNCE)
        mTouch_stateVars.sensor[sensorIndex].debounce        = 0;
        #endif
        
        mTouch_average      [sensorIndex] = 0;
        #if defined(MTOUCH_SLIDERS_ENABLED) || defined(MTOUCH_MOST_PRESSED_ONLY) || defined(MTOUCH_MATRIX_ENABLED)
        mTouch_deltas       [sensorIndex] = 0;
        #endif
        
    #if (MTOUCH_NUMBER_SENSORS == 1)
        #undef sensorIndex          // Turn off the text replacement on 'sensorIndex'
    #else
    }
    #endif
        
    #if defined(MTOUCH_TOGGLE_ENABLED)
    mTouch_toggle = 0;
    #endif
    
    /// 2. Loop through all enabled sliders and initialize the slider's output value.
    #if defined(MTOUCH_SLIDERS_ENABLED) 
        #if (MTOUCH_NUMBER_OF_SLIDERS == 1)
        mTouch_slider[0] = 0;
        #elif (MTOUCH_NUMBER_OF_SLIDERS == 2)
        mTouch_slider[0] = 0;
        mTouch_slider[1] = 0;
        #else
        for (int8_t i = MTOUCH_NUMBER_OF_SLIDERS-1; i >= 0; i--)
        {
            mTouch_slider       [i] = 0;
        }
        #endif
    #endif
    
    /// 3. Initialize mode variables and flags
    #if MTOUCH_NUM_MODES > 1
    mTouch_state.skippedDecode      = 0;
    mTouch_state.allReleased        = 0;
    mTouch_state.justChanged        = 0;
    #endif
    
    mTouch_state.dataReady          = 0;
    mTouch_state.areInitialized     = 0; 
    
    /// 4. Initialize non-array system variables
    #if defined(MCOMM_ENABLED)
    mTouch_stateMask                = 0;
    #endif
    
    #if defined(MTOUCH_ACTIVE_RUNS_AS_MAINLOOP)
    mTouch_state.isrServiced        = 0;
    mTouch_state.isRepeatScan       = 0;
    #endif
    
    /// 5. Initialize the DACOUT pin when it is used as a guard ring.
    #if defined(CVD_GUARD_DACOUT_ENABLED)
    PIC_DACOUT_LAT_C                = 0;
    PIC_DACOUT_TRIS_C               = 0;    
    #endif
    
    #if defined(MTOUCH_PROXIMITY_ENABLED)
    mTouch_ProxInit();
    #endif
    
    #if MTOUCH_NUM_MODES > 1
    mTouch_ChangeMode(0);
    #else
    mTouch_ChangeMode();
    #endif
    
    /// 6. Initialize the ADC module for mTouch operation.
    MTOUCH_ADC_INIT();
    
    /// 7. Initialization complete - enable active-mode mTouch scanning.
    #if MTOUCH_ISR_TIMER != 0
    PEIE = 1;
    #endif
    mTouch_EnableScanning();
    /// <br><br>

}


//================================================================================================
//          _____                _         ____                     _      
//  _ __ __|_   _|__  _   _  ___| |__     |  _ \  ___  ___ ___   __| | ___ 
// | '_ ` _ \| |/ _ \| | | |/ __| '_ \    | | | |/ _ \/ __/ _ \ / _` |/ _ \
// | | | | | | | (_) | |_| | (__| | | |   | |_| |  __/ (_| (_) | (_| |  __/
// |_| |_| |_|_|\___/ \__,_|\___|_| |_|___|____/ \___|\___\___/ \__,_|\___|
//                                   |_____|  
//================================================================================================
/**
*   @ingroup Decoding
* 
*   @brief Updates the state of each active-mode sensor using the latest acquisition value
*   
*   This is the mTouch service routine that performs sensor state transitions based on the value
*   of the latest acquisition value as compared to its previous behavior.
*
*/
void mTouch_Decode(void)
{
    
    // Disable current mTouch scanning while this is in process. This helps guarantee stack
    // overflows will not occur on 12F cores while also ensuring the raw data array will not
    // change mid-decode.
    mTouch_DisableScanning();
    
    // Initialize mTouch decode state flags used to check the state of all sensors at once.
    mTouch_state.areInitialized     = 1;
    mTouch_state.buttonStateChange  = 0;
    #if MTOUCH_NUM_MODES > 1
    mTouch_state.allReleased        = 1;
    #endif
    
    #if MTOUCH_NUM_MODES > 1
    if (mTouch_state.justChanged == 1)     // Trash the first sample on wake-up.
    {
        mTouch_state.justChanged = 0;
        mTouch_state.skippedDecode = 1;
        mTouch_EnableScanning();
        return;                         
    }
    mTouch_state.skippedDecode = 0;
    #endif
    
    // Initialize the index value
    #if MTOUCH_NUM_MODES > 1
    mTouch_currentScan      = mTouch_mode[mTouch_modeIndex];
    #else
    #if MTOUCH_NUMBER_SENSORS > 1
    mTouch_currentScan      = 0;
    #endif
    #endif
    
    // Populate delta array
    #if defined(MTOUCH_REQUIRES_DELTA_ARRAY)
    do
    {
        /** 
        * @li   Get the current shift amount by comparing the new reading with the average
        *       and store it in the variable 'delta'. If a negative shift is detected, clear
        *       the delta variable. Then, if required by the features that have been enabled,
        *       store the delta value in the mTouch_deltas array for later use.
        */
        int16_t delta = mTouch_GetSensor(MTOUCH_CURRENTSCAN_VALUE) - mTouch_GetAverage(MTOUCH_CURRENTSCAN_VALUE);
        
        if (delta < 0)  // Shifts should increase. Setting to 0 helps to prevent triggering
        {               // on negative spikes.
            delta = 0;  
        }
        mTouch_deltas[MTOUCH_CURRENTSCAN_VALUE] = (uint16_t) delta;
        
        mTouch_currentScan++;
    } while (MTOUCH_CURRENTSCAN_VALUE != MTOUCH_NUMBER_SENSORS);
    #endif
    
    // MATRIX Variable Service
    #if defined(MTOUCH_MATRIX_ENABLED)

    #if MTOUCH_NUM_MODES > 1    
        // TODO: If modes are enabled, check flag to see if this mode is decoding the matrix
        // RIGHT NOW: Matrix is decoded during all states.
    #endif
    
    mTouch_FindMostPressed(MTOUCH_MATRIX_ROW_INDEX);
    mTouch_FindMostPressed(MTOUCH_MATRIX_COL_INDEX);
    #endif
    
    // MOST-PRESSED Variable Service
    #if defined(MTOUCH_MOST_PRESSED_ONLY) && !defined(MTOUCH_MATRIX_SENSORS_ONLY)
    
    #if MTOUCH_NUM_MODES > 1    
        // TODO: If modes are enabled, check flag to see if this mode is decoding most-pressed
        // RIGHT NOW: Matrix is decoded during all states.
    #endif
    
    mTouch_FindMostPressed(MTOUCH_MOST_PRESSED_INDEX);
    #endif
    
    //
    // Decode
    //
    #if MTOUCH_NUM_MODES > 1
    mTouch_currentScan      = mTouch_mode[mTouch_modeIndex];
    #else
    #if MTOUCH_NUMBER_SENSORS > 1
    mTouch_currentScan      = 0;
    #endif
    #endif
    
    #if MTOUCH_NUMBER_SENSORS > 1
    do
    {    
        #if defined(MTOUCH_PROXIMITY_ENABLED) && (MTOUCH_NUMBER_SENSORS != MTOUCH_NUMBER_PROXIMITY)
        if (mTouch_StateMachine[MTOUCH_CURRENTSCAN_VALUE]())
        #elif defined(MTOUCH_PROXIMITY_ENABLED)
        if (mTouch_ProxStateMachine())
        #else
        if (mTouch_ButtonStateMachine())
        #endif
    #else
        if ((*MTOUCH_STATEMACHINE_0)())
    #endif
        {
            mTouch_UpdateBaseline();
        }
        
    #if MTOUCH_NUMBER_SENSORS > 1
        mTouch_currentScan++;
    } while (MTOUCH_CURRENTSCAN_VALUE != MTOUCH_NUMBER_SENSORS);
    #endif
    
    //
    // Update structured outputs:
    //  Sliders, Wheels, Matrix, State mask
    //
    /**
    * 3. Now that all sensors have had their initial decoding step completed, if sliders 
    *    are implemented in this application, perform decoding on them now.
    */
    #if defined(MTOUCH_SLIDERS_ENABLED)  
    mTouch_SliderDecode();
    #endif
    
    #if defined(MTOUCH_MATRIX_ENABLED)  
    mTouch_UpdateMatrixOutput();
    #endif
    
    #if defined(MCOMM_ENABLED)
    if (mTouch_state.buttonStateChange)             // Update the mTouch state mask variable with its new value
    {
        mTouch_stateMask = 0;
        
        #if MTOUCH_NUMBER_SENSORS > 1
        for (int8_t i = MTOUCH_NUMBER_SENSORS-1; i >= 0; i--)
        {
            if (mTouch_GetButtonState(i) == MTOUCH_PRESSED)
            {
                mTouch_stateMask |= 1 << i;
            }
        }
        #else
        if (mTouch_GetButtonState(0) == MTOUCH_PRESSED)
            mTouch_stateMask = 1;
        #endif
    }
    #endif

    /**
    * 6. Finally, if all active sensors are initialized and the system is not
    *    about to change modes, send communications data.
    */
    #if defined(MCOMM_ENABLED) && defined(MCOMM_UART_IMPLEMENTED)
    if (mTouch_state.areInitialized)                // Only trasmit data if all sensors are initialized
    {
    #if MTOUCH_NUM_MODES > 1   
        if (mTouch_state.change == 0)               // If the system is not going to be changing modes right now,
        {
    #endif
    
        #if defined(MCOMM_TWO_WAY_ENABLED) && defined(MCOMM_ENABLE_STREAM)
            mComm_streamConfig.go = 1;
        #endif
    
            mComm_Service();                        // Transmit data packet using mTouch Communication module
            
    #if MTOUCH_NUM_MODES > 1   
        }
    #endif
    }
    #endif
    
    #if MTOUCH_NUM_MODES > 1
    mTouch_ChangeMode(mTouch_modeIndex);
    #else
    mTouch_ChangeMode();
    #endif
    
    /**
    * 7. Enable active-mode mTouch scanning now that we're finished.
    */
    mTouch_EnableScanning();                // Enable the timer's interrupt
    
}



uint8_t mTouch_ButtonStateMachine(void)
{
    #if MTOUCH_NUMBER_SENSORS == 1
    
        #define     MTOUCH_SENSOR_state           mTouch_stateVars.sensor[0].state
        #define     MTOUCH_SENSOR_timer           mTouch_stateVars.sensor[0].timer
        #define     MTOUCH_SENSOR_debounce        mTouch_stateVars.sensor[0].debounce
        #define     MTOUCH_SENSOR_timeout         mTouch_stateVars.sensor[0].timeout
        
    #else

        #define     MTOUCH_SENSOR_state           (*sensor).state
        #define     MTOUCH_SENSOR_timer           (*sensor).timer
        #define     MTOUCH_SENSOR_debounce        (*sensor).debounce
        #define     MTOUCH_SENSOR_timeout         (*sensor).timeout

    mTouch_SensorVariables* sensor          = &mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE];
    
    #endif

        
        #if defined(MTOUCH_REQUIRES_DELTA_ARRAY)
        uint16_t delta = mTouch_deltas[MTOUCH_CURRENTSCAN_VALUE];
        #else
        uint16_t delta = mTouch_GetSensor(MTOUCH_CURRENTSCAN_VALUE) - mTouch_GetAverage(MTOUCH_CURRENTSCAN_VALUE);
        
        if ((int16_t) delta < 0)    // Shifts should increase. Setting to 0 helps to prevent triggering
        {                           // on negative spikes.
            delta = 0;  
        }
        #endif

        
        #if defined(MTOUCH_MOST_PRESSED_ONLY) || defined(MTOUCH_MATRIX_ENABLED)  
        #if defined(MTOUCH_MATRIX_ENABLED)                  // If matrix is enabled,
        #if !defined(MTOUCH_MATRIX_SENSORS_ONLY)            // and there are more than just row and column sensors,
        #if defined(MTOUCH_MATRIX_DEFS_AT_START)            // and the matrix row/columns come BEFORE normal sensors...
        if (MTOUCH_CURRENTSCAN_VALUE < MTOUCH_SENSOR_START) //      Is this sensor part of the matrix?
        #else                                               // and the matrix row/columns come AFTER normal sensors...
        if (MTOUCH_CURRENTSCAN_VALUE > MTOUCH_SENSOR_END)   //      Is this sensor part of the matrix?
        #endif
        {
        #endif                                                          // If this sensor is part of the matrix,
            #if defined(MTOUCH_MATRIX_ROWS_FIRST)                       // and rows are defined first...
            if (MTOUCH_CURRENTSCAN_VALUE < MTOUCH_MATRIX_COLUMN_START)  //      Is this sensor a row?
            #else                                                       // and columns are defined first...
            if (MTOUCH_CURRENTSCAN_VALUE > MTOUCH_MATRIX_COLUMN_END)    //      Is this sensor a row?
            #endif
            {
                mTouch_mostPressed = &mTouch_mostPressedData[MTOUCH_MATRIX_ROW_INDEX];  // Sensor is a matrix row.
            }
            else                                        
            {
                mTouch_mostPressed = &mTouch_mostPressedData[MTOUCH_MATRIX_COL_INDEX];  // Sensor is a matrix column.
            }
        #if !defined(MTOUCH_MATRIX_SENSORS_ONLY)        // If there are other sensors besides those implementing
        }                                               // the matrix...
        else
        {   
            #if defined(MTOUCH_MOST_PRESSED_ONLY)
            mTouch_mostPressed = &mTouch_mostPressedData[MTOUCH_MOST_PRESSED_INDEX];
            #else
            
            // If matrix is enabled, but the non-matrix sensors are NOT supposed to be decoded
            // in the 'most pressed' algorithm (ie. if all non-matrix sensors can be pressed at
            // the same time), then use the dummy location of the mostPressed structure array 
            // to store this sensor's data as if it were the most-pressed of its group. 
            mTouch_mostPressed = &mTouch_mostPressedData[MTOUCH_DUMMY_MOST_PRESSED_INDEX];
            (*mTouch_mostPressed).maxIndex = MTOUCH_CURRENTSCAN_VALUE;
            (*mTouch_mostPressed).maxValue = delta;
            #endif
        }
        #endif
        #else   // If there isn't a matrix being implemented, most-pressed must be.
            mTouch_mostPressed = &mTouch_mostPressedData[MTOUCH_MOST_PRESSED_INDEX];
        #endif
        #endif
        
    switch(MTOUCH_SENSOR_state)                                 // Begin state machine
    {
        /** @section InitState Initialization State
        *
        *   The default state of all sensors on power up. Once sensors leave this state, 
        *   they do not return. Initialization begins by decrementing the timer variable
        *   for the sensor. 
        *   If #timer has not yet reached 0, the areInitialized and allReleased flags are
        *   cleared to prevent any advanced decoding logic, such as sliders and wheels, 
        *   or communications from occuring. 
        *   Once #timer reaches 0, the sensor transitions to the #MTOUCH_RELEASED state, 
        *   the debounce counter is reset to the value needed to enter the pressed state,
        *   and the baseline is updated to equal to the current sensor value.
        */
        case MTOUCH_INITIALIZING:    
        {
            if (--(MTOUCH_SENSOR_timer) <= 0)                       // If initialization delay has elapsed
            {                                                   // NOTE: This delay determined by
                                                                //       MTOUCH_POWER_UP_SAMPLES
                //-----------------------------
                // State Transition -> Released
                //-----------------------------
                MTOUCH_SENSOR_state             = MTOUCH_RELEASED;  // Sensor is now released and stable  
                
                mTouch_average[MTOUCH_CURRENTSCAN_VALUE] = mTouch_GetSensor(MTOUCH_CURRENTSCAN_VALUE);  
                
                #if defined(MTOUCH_STATE_DEBOUNCE)              // Initialize sensor's debounce counter
                MTOUCH_SENSOR_debounce          = MTOUCH_pDEBOUNCE_VALUE;   
                #endif
                
            }
            else                                                // If initialization delay is not complete
            {
            
                mTouch_state.areInitialized  = 0;               // Disable data transfer until initialized
                
                #if MTOUCH_NUM_MODES > 1   
                mTouch_state.allReleased     = 0;               // Clear the all-sensors-are-released flag.
                #endif
                
            }
            
            return 0;         
        }                                                   
        
        /** @section ReleaseState Released State
        * 
        *   This state can be entered from either the #MTOUCH_INITIALIZING or 
        *   #MTOUCH_PRESSED state. The only way to leave this state is by creating enough 
        *   consecutive positive shifts of the reading above the baseline to overflow the 
        *   #debounce variable for the sensor. Once this is done, the sensor will enter 
        *   the pressed state.<br>
        *   The code for this section begins by checking to see if the latest reading
        *   is greater than the baseline by at least the press threshold. If it is, the
        *   debounce variable is decremented. Once the debounce value has been reached 0, 
        *   the sensor will change to the #MTOUCH_PRESSED state, reset the debounce 
        *   counter, initialize the press timer (if enabled), toggle the toggle bit,
        *   and clear the 'allReleased' flag.<br>
        *   If the threshold has not been crossed, the debounce counter is reset to the
        *   initial press debounce value and the baseline is updated.
        *
        *   @note This is the only state and condition where the baseline is able to be 
        *   updated, and we only use the latest reading to update the baseline once we 
        *   know it's not crossing any thresholds.
        */
        case MTOUCH_RELEASED:                                           // Sensor is released and stable
        {                  

            if(delta > (int16_t)mTouch_GetPressThreshold(MTOUCH_CURRENTSCAN_VALUE))  // If reading has crossed the press threshold
            {
                
                #if defined(MTOUCH_MOST_PRESSED_ONLY) || defined(MTOUCH_MATRIX_ENABLED)                
                                                                        // If this sensor is the most pressed, and is
                                                                        // at least MTOUCH_MOST_PRESSED_THRESHOLD
                                                                        // more pressed than the second-most-pressed.
                if ((MTOUCH_CURRENTSCAN_VALUE == (*mTouch_mostPressed).maxIndex) && 
                   ((*mTouch_mostPressed).maxValue > MTOUCH_MOST_PRESSED_THRESHOLD))
                {
                #endif
                
                    #if defined(MTOUCH_STATE_DEBOUNCE)
                    if (MTOUCH_SENSOR_debounce == 0)                            // Check the debounce counter
                    {                                                       
                        MTOUCH_SENSOR_debounce  = MTOUCH_rDEBOUNCE_VALUE;       // Initialize the pressed state's debounce variable
                    #endif
                    
                        //-----------------------------
                        // State Transition -> Pressed
                        //-----------------------------
                        MTOUCH_SENSOR_state     = MTOUCH_PRESSED;               // Sensor is now pressed
                        
                        #if defined(MTOUCH_BUTTON_TIMEOUT)                  // If the press timer is enabled
                        #if MTOUCH_BUTTON_TIMEOUT > 0
                        MTOUCH_SENSOR_timer    = MTOUCH_pTIMEOUT_VALUE;         // Initialize the press timer counter
                        #endif                                              // NOTE: This delay determined by
                        #endif                                              //       MTOUCH_BUTTON_TIMEOUT
                        
                        #if MTOUCH_NUM_MODES > 1   
                        mTouch_state.allReleased = 0;                       // Clear the all-sensors-are-released flag.
                        #endif
                        
                        #if defined(MTOUCH_TOGGLE_ENABLED)
                        mTouch_ToggleToggle(MTOUCH_CURRENTSCAN_VALUE);      // Alternate toggle between 'on' and 'off'.
                        #endif
                        
                        mTouch_state.buttonStateChange = 1;                 // Set flag notifying that a change has occured
                                                                            // to a sensor's 'button' state in this decode
                        
                    #if defined(MTOUCH_STATE_DEBOUNCE)
                    }
                    else
                    {
                        MTOUCH_SENSOR_debounce--;
                    }
                    #endif
                    
                #if defined(MTOUCH_MOST_PRESSED_ONLY) || defined(MTOUCH_MATRIX_ENABLED)    
                }
                else                                                    // If this sensor is not the most pressed or if it
                {                                                       // is not significantly more pressed than the others,
                    #if defined(MTOUCH_STATE_DEBOUNCE)
                    MTOUCH_SENSOR_debounce  = MTOUCH_pDEBOUNCE_VALUE;       // reset the debounce variable 
                    #endif
                }
                #endif
                
            }
            else                                                        // If the reading has not crossed the press threshold
            {
                #if defined(MTOUCH_STATE_DEBOUNCE)
                MTOUCH_SENSOR_debounce      = MTOUCH_pDEBOUNCE_VALUE;       // Reset the debounce variable   
                #endif
                
                // NOTE: This is the only state (MTOUCH_RELEASED) and condition 
                //       (delta < PressThreshold) where the average is able to
                //       be updated. Also note how we only use the reading to
                //       update the baseline after we have verified it is not
                //       crossing any thresholds.
                return 1; 
            }
            return 0;            
        }

        /** @section PressState Pressed State
        * 
        *   A sensor can only enter this state from the #MTOUCH_RELEASED state, but there
        *   are two ways to leave: 
        *       First, by being in the state for too long and allowing the press timer to 
        *       run out. 
        *       Second, by having a the delta value smaller than the release threshold on 
        *       enough consecutive scans to cause the debounce counter to reach 0.
        *
        *   The code for this section begins by decrementing and checking the #timer 
        *   variable to see if it has reached 0. If so, the sensor transfers to the 
        *   #MTOUCH_RELEASED state, the debounce counter is reset and the baseline is 
        *   re-initialized to the sensor's current reading. The baseline is reset to allow 
        *   for a quick recovery if some environmental condition caused the sensor to 
        *   become stuck. See the #NEGATIVE_CAPACITANCE configuration option for the
        *   different behaviors that can be implemented to quickly allow the baseline to
        *   re-settle to the unpressed value.
        *
        *   If the timer has not yet reached 0, the reading is checked against the 
        *   release threshold. If the difference between the sensor's reading and the
        *   baseline (delta) is less than the release threshold, the debounce variable is
        *   decremented in the same manner as the press timer. When it reaches 0, the
        *   sensor enters the #MTOUCH_RELEASED state and the debounce counter is set to
        *   debounce value required to enter the pressed state. If it has not crossed the 
        *   threshold (meaning: the delta value is still large enough to register as a
        *   press), the debounce counter is reset to the release debounce value.
        *
        *   @note The baseline is never updated while the sensor is pressed so the system
        *   can remember where the unpressed value should end up. Because of this, the
        *   baseline can be better thought of as a released-state average.
        */
        case MTOUCH_PRESSED:
        {

            #if defined(MTOUCH_BUTTON_TIMEOUT)                  // (Only if the press timer has been enabled)
            #if MTOUCH_BUTTON_TIMEOUT > 0   

            if (--(MTOUCH_SENSOR_timer) <= 0)                       // Decrement the press timer counter
            {     
                //-----------------------------
                // State Transition -> Released
                //-----------------------------
                MTOUCH_SENSOR_state     = MTOUCH_RELEASED;          // Sensor is now released
                MTOUCH_SENSOR_timeout   = 1;                        // Signal that a timeout has occurred
                
                mTouch_average[MTOUCH_CURRENTSCAN_VALUE]   = mTouch_GetSensor(MTOUCH_CURRENTSCAN_VALUE);
                
                #if defined(MTOUCH_STATE_DEBOUNCE)
                MTOUCH_SENSOR_debounce = MTOUCH_pDEBOUNCE_VALUE;    // Initialize the debounce counter for the MTOUCH_PRESSED state
                #endif                                          // and check if it has reached 0.
                
                mTouch_state.buttonStateChange = 1;             // Set flag notifying that a change has occured
                                                                // to a sensor's 'button' state in this decode
                
                break;
            }
            
            #endif
            #endif
            
            #if !defined(MTOUCH_MOST_PRESSED_ONLY) && !defined(MTOUCH_MATRIX_ENABLED)
                                                                                                    // If most pressed is not enabled,
            if  (delta < (int16_t)mTouch_GetReleaseThreshold(MTOUCH_CURRENTSCAN_VALUE))             // Look only at the threshold to determine release.
            #else                                                                                   // Otherwise,
            if ((delta < (int16_t)mTouch_GetReleaseThreshold(MTOUCH_CURRENTSCAN_VALUE))      ||     // Check the threshold,
                (MTOUCH_CURRENTSCAN_VALUE != (*mTouch_mostPressed).maxIndex)                 ||     // Make sure this sensor is still the most-pressed,
                ((*mTouch_mostPressed).maxValue < MTOUCH_MOST_PRESSED_THRESHOLD) )                  // and is most-pressed by a significant amount.
            #endif
            {                                                   // If the reading has crossed the release threshold
            
                #if defined(MTOUCH_STATE_DEBOUNCE)
                if (MTOUCH_SENSOR_debounce == 0)                    // Check if the debounce counter has reached 0.
                {                                               
                    MTOUCH_SENSOR_debounce = MTOUCH_pDEBOUNCE_VALUE;
                #endif
                
                    //-----------------------------
                    // State Transition -> Released
                    //-----------------------------
                    MTOUCH_SENSOR_state = MTOUCH_RELEASED;          // Sensor is now released
                    
                    mTouch_state.buttonStateChange = 1;         // Set flag notifying that a change has occured
                                                                // to a sensor's 'button' state in this decode
                    
                #if defined(MTOUCH_STATE_DEBOUNCE)
                } else {
                
                    MTOUCH_SENSOR_debounce--;
                
                    #if MTOUCH_NUM_MODES > 1   
                    mTouch_state.allReleased = 0;               // Clear the all-sensors-are-released flag.
                    #endif
                    
                }
                #endif    
                
            }
            else                                                // If the reading has not crossed the release threshold
            {
            
                #if defined(MTOUCH_STATE_DEBOUNCE)
                MTOUCH_SENSOR_debounce  = MTOUCH_rDEBOUNCE_VALUE;   // Reset the debounce counter
                #endif                    
                #if MTOUCH_NUM_MODES > 1   
                mTouch_state.allReleased = 0;                   // Clear the all-sensors-are-released flag.
                #endif
                
            }
            return 0;
        }
        
        default: break; 
        
    }   // End state machine switch command
    return 0;
}

#if defined(MTOUCH_MOST_PRESSED_ONLY) || defined(MTOUCH_MATRIX_ENABLED)
//================================================================================================
//          _____                _         _____ _           _ __  __           _   ____                             _ 
//  _ __ __|_   _|__  _   _  ___| |__     |  ___(_)_ __   __| |  \/  | ___  ___| |_|  _ \ _ __ ___  ___ ___  ___  __| |
// | '_ ` _ \| |/ _ \| | | |/ __| '_ \    | |_  | | '_ \ / _` | |\/| |/ _ \/ __| __| |_) | '__/ _ \/ __/ __|/ _ \/ _` |
// | | | | | | | (_) | |_| | (__| | | |   |  _| | | | | | (_| | |  | | (_) \__ \ |_|  __/| | |  __/\__ \__ \  __/ (_| |
// |_| |_| |_|_|\___/ \__,_|\___|_| |_|___|_|   |_|_| |_|\__,_|_|  |_|\___/|___/\__|_|   |_|  \___||___/___/\___|\__,_|
//                                   |_____|                                    
//================================================================================================
void mTouch_FindMostPressed(uint8_t index)
{
    //
    // Initialize variables
    //
    mTouch_mostPressed                  = &mTouch_mostPressedData[index];
    (*mTouch_mostPressed).maxIndex      = 0;
    (*mTouch_mostPressed).maxValue      = 0;
    uint16_t              secValue      = 0; 

    // Find the most pressed sensor and store it's index value
    for(int8_t i = mTouch_mostPressedIndex[index].end; i >= mTouch_mostPressedIndex[index].start; i--)
    {
        if (mTouch_deltas[i] > (*mTouch_mostPressed).maxValue)
        {
            (*mTouch_mostPressed).maxValue = mTouch_deltas[i];      
            (*mTouch_mostPressed).maxIndex = i;                     
        }
    }
    
    // Find the second-most-pressed row for comparison with the most-pressed.
    for(int8_t i = mTouch_mostPressedIndex[index].end; i >= mTouch_mostPressedIndex[index].start; i--)
    {
        if (i != (*mTouch_mostPressed).maxIndex)                                      
        {
            if (mTouch_deltas[i] > secValue)
            {
                secValue = mTouch_deltas[i]; 
            }
        }
    }
    
    // Store the difference between the most-pressed sensor's 
    //  delta and the second-most-pressed sensor's delta.
    (*mTouch_mostPressed).maxValue = (*mTouch_mostPressed).maxValue - secValue;
}
#endif

#if defined(MTOUCH_MATRIX_ENABLED)

//================================================================================================
//         _____                _        _   _           _       _       __  __       _        _       ___        _               _   
// _ __ __|_   _|__  _   _  ___| |__    | | | |_ __   __| | __ _| |_ ___|  \/  | __ _| |_ _ __(_)_  __/ _ \ _   _| |_ _ __  _   _| |_ 
//| '_ ` _ \| |/ _ \| | | |/ __| '_ \   | | | | '_ \ / _` |/ _` | __/ _ \ |\/| |/ _` | __| '__| \ \/ / | | | | | | __| '_ \| | | | __|
//| | | | | | | (_) | |_| | (__| | | |  | |_| | |_) | (_| | (_| | ||  __/ |  | | (_| | |_| |  | |>  <| |_| | |_| | |_| |_) | |_| | |_ 
//|_| |_| |_|_|\___/ \__,_|\___|_| |_|___\___/| .__/ \__,_|\__,_|\__\___|_|  |_|\__,_|\__|_|  |_/_/\_\\___/ \__,_|\__| .__/ \__,_|\__|
//                                  |_____|   |_|                                                                    |_|    
//================================================================================================
void mTouch_UpdateMatrixOutput(void)
{
    mTouch_matrix = 0xFF;                                                       // Reset the matrix output value
    
    for(int8_t rowIndex = MTOUCH_MATRIX_ROW_END;                                
        rowIndex >= MTOUCH_MATRIX_ROW_START; 
        rowIndex--)                                                             // For each ROW in the matrix, check to see if
    {                                                                           // any are considered pressed. Only one is able
        if (mTouch_GetButtonState(rowIndex) == MTOUCH_PRESSED)                  // to be in the pressed state at a time.
        {
            for(int8_t colIndex = MTOUCH_MATRIX_COLUMN_END; 
                colIndex >= MTOUCH_MATRIX_COLUMN_START; 
                colIndex--)                                                     // For each COLUMN in the matrix, check to see if
            {                                                                   // any are considered pressed. Only one is able
                if (mTouch_GetButtonState(colIndex) == MTOUCH_PRESSED)          // to be in the pressed state at a time.
                {
                    mTouch_matrix   = colIndex - MTOUCH_MATRIX_COLUMN_START;    // Store the matrix output value
                    mTouch_matrix <<= MTOUCH_MATRIX_ROW_OFFSET;                 
                    mTouch_matrix  += rowIndex - MTOUCH_MATRIX_ROW_START;       // Use the APIs to access the row and column info
                    break;  // Break out of the 'for' loop. Column found.
                }
            }    
            break;  // Break out of the 'for' loop. Row found.
        }
    }  
}
#endif



//================================================================================================
//          _____                _        _   _           _       _          _                                 
//  _ __ __|_   _|__  _   _  ___| |__    | | | |_ __   __| | __ _| |_ ___   / \__   _____ _ __ __ _  __ _  ___ 
// | '_ ` _ \| |/ _ \| | | |/ __| '_ \   | | | | '_ \ / _` |/ _` | __/ _ \ / _ \ \ / / _ \ '__/ _` |/ _` |/ _ \
// | | | | | | | (_) | |_| | (__| | | |  | |_| | |_) | (_| | (_| | ||  __// ___ \ V /  __/ | | (_| | (_| |  __/
// |_| |_| |_|_|\___/ \__,_|\___|_| |_|___\___/| .__/ \__,_|\__,_|\__\___/_/   \_\_/ \___|_|  \__,_|\__, |\___|
//                                   |_____|   |_|                                                  |___/      
//================================================================================================

/**
*   @ingroup    Decoding
*
*   @brief      Updates the baseline for the given sensor with the given value.
*   
*   Updates a sensor's baseline (or baseline) in order to track the environmental changes of the 
*   system. 
*/
void mTouch_UpdateBaseline(void)
{
    uint16_t reading = mTouch_GetSensor(MTOUCH_CURRENTSCAN_VALUE);
    /** @section UAvgImp Algorithm Implementation Notes
    *
    *   This function has several possible implementations based on the current configuration 
    *   options of the framework. The important configuration options are:
    *   @li #MTOUCH_BASELINE_RATE - acts as a debounce counter for updating the baseline only every 
    *       N scans.
    *   @li #MTOUCH_BASELINE_WEIGHT - determines what weight to give the reading when updating the 
    *       baseline.
    *
    *   The average updates the counter variable and compares it with #MTOUCH_BASELINE_RATE
    *   to see if it should continue or return. If the counter allows it to continue, the 
    *   average is then updated using one of four possible implementations which is 
    *   determined by the #MTOUCH_BASELINE_WEIGHT option.
    */    
    #if (MTOUCH_BUTTON_TIMEOUT > 0)
    if (mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].timeout)
    {
        if ((int16_t)(mTouch_average[MTOUCH_CURRENTSCAN_VALUE] - reading) > (int16_t)(mTouch_GetPressThreshold(MTOUCH_CURRENTSCAN_VALUE) >> 1))    // Is the delta at least half the press threshold in the negative direction?
        {
            mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].baselineCorrection = 1;
            mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].timeout = 0;
        }
    }
    #endif
    
    if (mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].baselineCorrection)
    {
        mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].baselineCount = 1; 
        
        if ((int16_t)(mTouch_average[MTOUCH_CURRENTSCAN_VALUE] - reading) < (int16_t)(mTouch_GetReleaseThreshold(MTOUCH_CURRENTSCAN_VALUE)))       // Is the delta at least half the press threshold in the negative direction?
        { 
            mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].baselineCorrection    = 0;
        }
    }
    
    #if defined(MTOUCH_BLCOUNT_16BITS_REQUIRED)                // If the baseline counter has been exceeded
    if ((uint16_t)(--(mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].baselineCount)) == 0)    
    #else
    if ((uint8_t)(--(mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].baselineCount)) == 0) 
    #endif
    {                                                           // Reset the baseline counter
        mTouch_stateVars.sensor[MTOUCH_CURRENTSCAN_VALUE].baselineCount = MTOUCH_BASELINE_RATE_VALUE;  

        #if defined(MTOUCH_EEPROM_ENABLED)
        if (MTOUCH_BASELINE_WEIGHT_VALUE == 1)  // If 'V2' communications are enabled, this configuration
        {                                       // option is changable. So we need to check it each time.
        #endif
        
            #if defined(MTOUCH_EEPROM_ENABLED) || (MTOUCH_BASELINE_WEIGHT == 1)
                mTouch_average[MTOUCH_CURRENTSCAN_VALUE]  += reading;              // Update the baseline with an equal
                mTouch_average[MTOUCH_CURRENTSCAN_VALUE] >>= 1;                    // weight given to both.
            #endif                                              
        
        #if defined(MTOUCH_EEPROM_ENABLED)
        }
        else
        {
        #endif

            #if defined(MTOUCH_EEPROM_ENABLED) || (MTOUCH_BASELINE_WEIGHT != 1) 
                mTouch_average[MTOUCH_CURRENTSCAN_VALUE]  -= (uint16_t)(mTouch_average[MTOUCH_CURRENTSCAN_VALUE] >> MTOUCH_BASELINE_WEIGHT_VALUE); 
                mTouch_average[MTOUCH_CURRENTSCAN_VALUE]  += (uint16_t)(reading >> MTOUCH_BASELINE_WEIGHT_VALUE);
            #endif

        #if defined(MTOUCH_EEPROM_ENABLED)
        }
        #endif
        
    }
        /// <br><br><br>    
}


//================================================================================================
//          _____                _         _____                     ____       _            _   
//  _ __ __|_   _|__  _   _  ___| |__     | ____|_ __ _ __ ___  _ __|  _ \  ___| |_ ___  ___| |_ 
// | '_ ` _ \| |/ _ \| | | |/ __| '_ \    |  _| | '__| '__/ _ \| '__| | | |/ _ \ __/ _ \/ __| __|
// | | | | | | | (_) | |_| | (__| | | |   | |___| |  | | | (_) | |  | |_| |  __/ ||  __/ (__| |_ 
// |_| |_| |_|_|\___/ \__,_|\___|_| |_|___|_____|_|  |_|  \___/|_|  |____/ \___|\__\___|\___|\__|
//                                   |_____|    
//================================================================================================ 
#if defined(MTOUCH_ERROR_DETECTION_ENABLED)     
void mTouch_ErrorDetect(void)
{
    mTouch_DisableScanning();
    MTOUCH_GENERATE_SHORTCHECKS();
    mTouch_EnableScanning();
}
#endif