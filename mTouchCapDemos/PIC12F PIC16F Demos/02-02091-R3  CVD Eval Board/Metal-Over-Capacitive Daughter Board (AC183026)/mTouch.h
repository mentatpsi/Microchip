/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch.h
 *  Dependencies:    mTouch_config.h
 *                   includes/mTouch_HardwareProfile_*******.h
 *                      (Based on current PIC® configuration)
 *                   mTouchCVD_macroLibrary_PIC****.h
 *                      (Based on current PIC® processor core)
 *                   See documentation for more information about which
 *                   files are required for compiling.
 *
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     Master mTouch Framework Include File
 *                   - No application code should be implemented in this
 *                     or any other mTouch framework file. This will allow
 *                     for better customer support and easier upgrades to
 *                     later firmware versions. Use the main.c and user-
 *                     generated files to implement your application.
 *                   - This is THE ONLY REQUIRED INCLUDE FILE for any
 *                     application implementing mTouch. 
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
 
//================================================================================================
//  __  __       _                 _____                _       _   _                _           
// |  \/  | __ _(_)_ __    _ __ __|_   _|__  _   _  ___| |__   | | | | ___  __ _  __| | ___ _ __ 
// | |\/| |/ _` | | '_ \  | '_ ` _ \| |/ _ \| | | |/ __| '_ \  | |_| |/ _ \/ _` |/ _` |/ _ \ '__|
// | |  | | (_| | | | | | | | | | | | | (_) | |_| | (__| | | | |  _  |  __/ (_| | (_| |  __/ |   
// |_|  |_|\__,_|_|_| |_| |_| |_| |_|_|\___/ \__,_|\___|_| |_| |_| |_|\___|\__,_|\__,_|\___|_| 
//
//================================================================================================
//
//  NOTE:   To CONFIGURE the mTouch framework, start in the 'includes/mTouch_config.h' file.
//
//================================================================================================
/** @file   mTouch.h
*   @brief  Required include! The main framework header file used by all framework files.
*/
#ifndef __MTOUCH_H
#define __MTOUCH_H


//================================================================================================
//  ___            _           _           
// |_ _|_ __   ___| |_   _  __| | ___  ___ 
//  | || '_ \ / __| | | | |/ _` |/ _ \/ __|
//  | || | | | (__| | |_| | (_| |  __/\__ \
// |___|_| |_|\___|_|\__,_|\__,_|\___||___/
//                                                                    
//================================================================================================

#if defined(HI_TECH_C)
    #include <htc.h>                            // Provides PIC microcontroller SFR information
    #include <stdint.h>                         // Provides "uint#_t" typedefs for each compiler
#else
    #include <p18cxxx.h>
#endif

    #include "mTouch_config.h"                  // Stores the mTouch Framework's configuration details
    #include "mTouch_processConfiguration.h"    // Validates and processes the mTouch configuration for compiling
    #include "mTouch_modes.h"

    #if defined(MTOUCH_PROXIMITY_ENABLED)
    #include "mTouch_proximity.h"
    #endif
    

    #include "mComm.h"                          // mTouch communication module header file
    

//================================================================================================
//  ____  _                   _       
// / ___|| |_ _ __ _   _  ___| |_ ___ 
// \___ \| __| '__| | | |/ __| __/ __|
//  ___) | |_| |  | |_| | (__| |_\__ \
// |____/ \__|_|   \__,_|\___|\__|___/
//                                
//================================================================================================


    /** @struct     mTouch_SensorVariables
    *   @brief      Contains all variables used in active mode which can/will be reinitialized
    *               after each mode switch.
    */
    typedef struct 
    {        
        unsigned state              :2;         ///< Tracks the current state of each sensor
        unsigned baselineCorrection :1;         
        
        #if (MTOUCH_BUTTON_TIMEOUT > 0)
        unsigned timeout            :1;
        #endif
        
        MTOUCH_STATE_BASELINE_COUNT ();         ///< 'baselineCount': Counter to track the number of decodes between updating the baseline                           
        MTOUCH_STATE_TIMER          ();         ///< 'timer': Multipurpose timer used for press timeouts, inactivity timers, and initialization       
        #if defined(MTOUCH_STATE_DEBOUNCE)
        MTOUCH_STATE_DEBOUNCE       ();         ///< 'debounce': Counter to implement debouncing on state transitions                             
        #endif
    } mTouch_SensorVariables;
   

    
    
    /** @union      mTouch_StateVariables
    *   @brief      Contains sensor and state variables
    *
    *   By defining this as a union, the variable space is able to be reused regardless of
    *   differences in the variables' sizes and quantity.
    */
    typedef struct 
    {
        #if (CVD_STATE_SPS_BITS > 8)
        uint16_t                sampleCounter;
        #else
        uint8_t                 sampleCounter;
        #endif
        
        #if (MTOUCH_NUM_MODES > 1)   
        #if MTOUCH_SLEEP_INACTIVE_RESET > 255
        uint16_t                inactivity;     ///< Counter to track the number of consecutive decode calls in a row where all sensors are considered released
        #else
        uint8_t                 inactivity;     ///< Same as above, but implemented more efficiently if the counter will not be exceeding 255
        #endif
        #endif

        mTouch_SensorVariables  sensor[MTOUCH_NUMBER_SENSORS];
            
    } mTouch_StateVariables;

    /** @union      mTouch_AcquisitionData
    *   @ingroup    Acquisition
    *   @brief      Structure used to implement both a 12-bit and a 20-bit variable inside a 
    *               single 32-bit array
    *
    *   The 12-bit result value is stored in the MSBs of the 32-bit array.  When processing the 
    *   'result' variable, we will need to always right shift by 4. See the ASCII drawing, below, 
    *   for a clearer picture of what's happening.
    *
    *   @code
    * // ._____._____._____._____.
    * // |xx xx xx|--|
    * //  result  
    *   @endcode
    *
    *   The 20-bit accumulation register is stored in the LSBs of the 32-bit array. When processing 
    *   this variable, we will need to always mask off the 4 MSbs since those are part of the result 
    *   variable.
    *
    *   @code
    * //  ._____._____._____._____.
    * //        |--|xx xx xx xx xx|
    * //               accumulator
    *   @endcode
    *
    *   Together, the variables are able to fit into a single 32-bit register, as shown:
    *
    *   @code
    * // ._____._____._____._____.
    * // |xx xx xx|xx xx xx xx xx|
    * //  result      accumulator
    *   @endcode
    *
    *   
    */
    typedef union                       
    {                                   
        uint32_t  all;              ///< Accesses full 32-bit register

        struct                          
        {                               
            uint16_t    invalid;        
            uint16_t    v;              ///< Result's value (must have bottom 4 bits shifted out)
        } result;                   ///< Decimate-filtered differential ADC result register
        
        struct                          
        {                               
            uint24_t    v;              ///< Accumulator's value (must have top 4 bits masked off)
            uint8_t     invalid;        
        } accumulator;              ///< Accumulator array for sensor oversampling        
       
    } mTouch_AcquisitionData;


    /** @struct     mTouch_State
    *   @brief      Collection of single-bit flags used by the mTouch Framework 
    */
    typedef struct 
    {
        unsigned dataReady          :1; ///< Flag to determine if new data is ready for processing   
        unsigned areInitialized     :1; ///< Temporary flag used in local functions for a variety of uses     
        unsigned buttonStateChange  :1; ///< Flag used to track when there has been a change in the button state
        
        #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
        unsigned isrServiced        :1; ///< Flag to alert the mTouch scanning algorithm that the ISR was serviced mid-scan     
        unsigned isRepeatScan       :1; ///< Flag to track if this scan is being repeated due to an interrupting ISR call       
        #endif
        
        #if (MTOUCH_NUM_MODES > 1)
        unsigned allReleased        :1; ///< Flag used to track when all mTouch sensors are released 
        unsigned skippedDecode      :1; ///< Flag used to track if we just skipped the decode call
        unsigned change             :1; 
        unsigned justChanged        :1;
        #endif
        
        #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
        unsigned scanningEnabled    :1; ///< If running the mTouch scan from the main loop, this flag tracks whether scanning is currently enabled or not
        #endif
        
        #if defined(MTOUCH_ERROR_DETECTION_ENABLED)
        unsigned error              :1;
        #endif
        
    } mTouch_State; 
    
    #if defined(MTOUCH_MOST_PRESSED_ONLY) || defined(MTOUCH_MATRIX_ENABLED)
    typedef struct 
    {   
        uint16_t    maxValue;
        uint8_t     maxIndex;
    } mTouch_MostPressedData;
    
    typedef struct 
    {        
        uint8_t     start;
        uint8_t     end;
    } mTouch_MostPressedIndex;
    #endif
    
//================================================================================================
// __     __         _       _     _                      ____ _       _           _ 
// \ \   / /_ _ _ __(_) __ _| |__ | | ___  ___           / ___| | ___ | |__   __ _| |
//  \ \ / / _` | '__| |/ _` | '_ \| |/ _ \/ __|  _____  | |  _| |/ _ \| '_ \ / _` | |
//   \ V / (_| | |  | | (_| | |_) | |  __/\__ \ |_____| | |_| | | (_) | |_) | (_| | |
//    \_/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/          \____|_|\___/|_.__/ \__,_|_|
// 
//================================================================================================
    // mTouch RAM Variables
    extern mTouch_State             mTouch_state;       ///< Tracks the state of the mTouch algorithm 
    extern mTouch_StateVariables    mTouch_stateVars;   ///< Stores all state-temporary variables to be reinitialized each mode change
    extern uint16_t                 mTouch_sensorData   [MTOUCH_NUMBER_SENSORS];    ///< Latch register storing the latest mTouch acquisition results
    extern mTouch_AcquisitionData   mTouch_acqData      [MTOUCH_NUMBER_SENSORS];    ///< Internal mTouch acquisition variable structure
    extern uint16_t                 mTouch_average      [MTOUCH_NUMBER_SENSORS];    ///< Array to track the unpressed value of each sensor
    
    #if defined(MCOMM_ENABLED)
    #if (MTOUCH_NUMBER_SENSORS <= 8)
    extern uint8_t                  mTouch_stateMask;   ///< Stores the state of each sensor in a bit mask for output by mComm when there are 8 or fewer sensors.
    #elif (MTOUCH_NUMBER_SENSORS <= 16)
    extern uint16_t                 mTouch_stateMask;   ///< Stores the state of each sensor in a bit mask for output by mComm when there are 16 or fewer sensors.
    #elif (MTOUCH_NUMBER_SENSORS <= 24)
    extern uint24_t                 mTouch_stateMask;   ///< Stores the state of each sensor in a bit mask for output by mComm when there are 24 or fewer sensors.
    #elif (MTOUCH_NUMBER_SENSORS <= 32)
    extern uint32_t                 mTouch_stateMask;   ///< Stores the state of each sensor in a bit mask for output by mComm when there are 32 or fewer sensors.
    #endif
    #endif
    
    #if defined(MTOUCH_SLIDERS_ENABLED) 
    #if defined(MTOUCH_SLIDER_SCALING_ENABLED)
    extern uint8_t                  mTouch_slider       [MTOUCH_NUMBER_OF_SLIDERS]; ///< Stores the current slider output value when scaling is enabled
    #else
    extern uint16_t                 mTouch_slider       [MTOUCH_NUMBER_OF_SLIDERS]; ///< Stores the current slider output value when scaling is disabled
    #endif
    #endif
    
    #if defined(MTOUCH_TOGGLE_ENABLED)
    #if (MTOUCH_NUMBER_SENSORS <= 8)
    extern uint8_t                  mTouch_toggle;      ///< Toggle state register when there are 8 or fewer sensors
    #elif (MTOUCH_NUMBER_SENSORS <= 16)
    extern uint16_t                 mTouch_toggle;      ///< Toggle state register when there are 16 or fewer sensors
    #elif (MTOUCH_NUMBER_SENSORS <= 24)
    extern uint24_t                 mTouch_toggle;      ///< Toggle state register when there are 24 or fewer sensors
    #elif (MTOUCH_NUMBER_SENSORS <= 32)
    extern uint32_t                 mTouch_toggle;      ///< Toggle state register when there are 25 or more sensors
    #endif
    #endif
        
    #if defined(MTOUCH_SLIDERS_ENABLED) || defined(MTOUCH_MOST_PRESSED_ONLY) || defined(MTOUCH_MATRIX_ENABLED) 
    extern uint16_t                 mTouch_deltas       [MTOUCH_NUMBER_SENSORS];            ///< Stores the latest delta value for each sensor. Only needed for functions that must compare sensors' relative shift amounts with each other.
    #endif
        
    #if defined(MTOUCH_MATRIX_ENABLED)
    extern uint8_t                  mTouch_matrix;          ///< Stores the current matrix output value for access by main application
    extern uint8_t                  mTouch_matrix_latch;    ///< Stores the last matrix value for comparison with the new one
    #endif
    
    // ISR Helper Variables
    #if (MTOUCH_INTEGRATION_TYPE != MTOUCH_CONTROLS_ISR) & defined(_PIC14)
    extern near     uint8_t         int_w;              ///< ISR: Saves the state of the W register, if required       
    extern near     uint8_t         int_status;         ///< ISR: Saves the state of the STATUS register, if required 
    extern near     uint8_t         int_fsr;            ///< ISR: Saves the state of the FSR register, if required     
    extern near     uint8_t         int_pclath;         ///< ISR: Saves the state of the PCLATH register, if required 
    #elif defined(_PIC18) || defined(__18CXX)
    extern          uint8_t         int_w;              ///< ISR: Saves the state of the W register, if required       
    extern          uint8_t         int_status;         ///< ISR: Saves the state of the STATUS register, if required 
    extern          uint8_t         int_bsr;            ///< ISR: Saves the state of the BSR register, if required     
    #endif
    
    // Constant arrays
    #if !defined(MTOUCH_EEPROM_ENABLED)
    extern const    uint16_t        mTouch_pressThreshold   [MTOUCH_NUMBER_SENSORS];    ///< Constant array containing the press threshold for each sensor
    extern const    uint16_t        mTouch_releaseThreshold [MTOUCH_NUMBER_SENSORS];    ///< Constant array containing the release threshold for each sensor
    #endif
    
    #if defined(MTOUCH_EEPROM_ENABLED) && defined(MTOUCH_EEPROM_STORED_IN_RAM)
    extern          uint8_t         mTouch_configOptions    [MTOUCH_EEPROM_START_ADR + MTOUCH_EEPROM_SIZE];
    #endif
    
    #if (MTOUCH_NUMBER_SENSORS > 1)
        #if (MTOUCH_NUM_MODES > 1)
    extern          uint8_t         mTouch_modeIndex; 
    extern const    uint8_t*        mTouch_currentScan;
        #define                     MTOUCH_CURRENTSCAN_VALUE    (*mTouch_currentScan)
        #else
    extern          uint8_t         mTouch_currentScan;
        #define                     MTOUCH_CURRENTSCAN_VALUE    mTouch_currentScan
        #endif
    #else
    #define                         MTOUCH_CURRENTSCAN_VALUE    0
    #endif
    
    #if MTOUCH_NUM_MODES > 1
    extern const    uint8_t*  const mTouch_mode [MTOUCH_NUM_MODES];
    #endif
    
//================================================================================================
//  _____                           
// | ____|_ __  _   _ _ __ ___  ___ 
// |  _| | '_ \| | | | '_ ` _ \/ __|
// | |___| | | | |_| | | | | | \__ \
// |_____|_| |_|\__,_|_| |_| |_|___/
//                               
//================================================================================================
    /** Enumeration of the valid mTouch active-mode sensor states */
    enum    mTouch_enum_ButtonState 
            {
                MTOUCH_INITIALIZING,        ///< Sensor is still initializing
                MTOUCH_RELEASED,            ///< Sensor is currently released
                MTOUCH_PRESSED              ///< Sensor is currently pressed
            };  

    #if defined(MTOUCH_TOGGLE_ENABLED)
    /** Enumeration of the valid mTouch toggle states */
    enum    mTouch_enum_ToggleState
            {
                MTOUCH_TOGGLE_OFF,          ///< Sensor toggle output is now 'off'
                MTOUCH_TOGGLE_ON            ///< Sensor toggle output is now 'on'
            };
    #endif
  
    #if defined(MTOUCH_PROXIMITY_ENABLED)
    /** Enumeration of the valid mTouch proximity sensor states */
    enum    mTouch_Prox_enum_SensorState
            {
                MTOUCH_PROX_INITIALIZING,   ///< Proximity sensor is still initializing 
                MTOUCH_PROX_NOT_ACTIVATED,  ///< Proximity sensor reading is stable
                MTOUCH_PROX_ACTIVATED       ///< Proximity sensor reading is changing
            };
    #endif


//================================================================================================
//  _____                 _   _                            ____ _       _           _ 
// |  ___|   _ _ __   ___| |_(_) ___  _ __  ___           / ___| | ___ | |__   __ _| |
// | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|  _____  | |  _| |/ _ \| '_ \ / _` | |
// |  _|| |_| | | | | (__| |_| | (_) | | | \__ \ |_____| | |_| | | (_) | |_) | (_| | |
// |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/          \____|_|\___/|_.__/ \__,_|_| 
//
//================================================================================================
    void    mTouch_Init             (void);     // Initializes the mTouch Framework
    void    mTouch_Decode           (void);     // DO NOT USE: Call mTouch_Service() macro instead.

    #if MTOUCH_NUM_MODES > 1
    void mTouch_ChangeMode          (uint8_t);
    #else
    void mTouch_ChangeMode          (void);
    #endif
    
    #if     (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_ISR)
    void    mTouch_Scan             (void);     // Service function if calling from main ISR
    #elif   (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
    uint8_t mTouch_Scan             (void);     // DO NOT USE: Call mTouch_isDataReady() macro instead.
    #endif

    #if defined(MTOUCH_SLIDERS_ENABLED)  
    void    mTouch_SliderDecode     (void);     // DO NOT USE: Called from within mTouch_Decode().
    #endif
    
    #if defined(MTOUCH_ERROR_DETECTION_ENABLED)
    void    mTouch_ErrorDetect      (void);     // Checks all sensor I/Os for shorts to Vss or Vdd
    #endif
    
    #if defined(PIC_ADC_HCVD_AVAILABLE)
    void    mTouch_HCVD_Init        (void);
    #endif
    
//================================================================================================
//          _____                _          _    ____ ___ 
//  _ __ __|_   _|__  _   _  ___| |__      / \  |  _ \_ _|
// | '_ ` _ \| |/ _ \| | | |/ __| '_ \    / _ \ | |_) | | 
// | | | | | | | (_) | |_| | (__| | | |  / ___ \|  __/| | 
// |_| |_| |_|_|\___/ \__,_|\___|_| |_| /_/   \_\_|  |___|
// 
//================================================================================================

    /**
    * @def          mTouch_GetButtonState(i)
    * @ingroup      mTouchAPI
    * @param        i       index of the sensor
    * @brief        Get the current state of the given active-mode sensor
    *
    *   Example usage:
    *   @code
    *   if (mTouch_GetButtonState(0) < MTOUCH_PRESSED) { 
    *       LED0 = LED_OFF; 
    *   } else { 
    *       LED0 = LED_ON; 
    *   }
    *   @endcode
    *
    * @hideinitializer
    */ 
    #define     mTouch_GetButtonState(i)            mTouch_stateVars.sensor[i].state     
    
    /**
    * @def          mTouch_GetAverage(i)
    * @ingroup      mTouchAPI
    * @param        i       index of the sensor
    * @brief        Get the current average value of the given active-mode sensor
    * @hideinitializer
    */ 
    #define     mTouch_GetAverage(i)                mTouch_average[i]   
    
    /**
    * @def          mTouch_GetSensor(i)
    * @ingroup      mTouchAPI
    * @param        i       index of the sensor
    * @brief        Get the current reading value of the given active-mode sensor
    * @hideinitializer
    */     
    #define     mTouch_GetSensor(i)                 mTouch_sensorData[i]                        

    
    // Slider
    #if defined(MTOUCH_SLIDERS_ENABLED)
        /**
        * @def          mTouch_GetSlider(i)
        * @ingroup      mTouchAPI
        * @param        i       index of the slider
        * @brief        Returns the current output value of the given slider
        * @hideinitializer
        */ 
        #define     mTouch_GetSlider(i)             mTouch_slider[i]                            
    #endif
    
    // Toggle
    #if defined(MTOUCH_TOGGLE_ENABLED)
        /**
        * @def          mTouch_GetToggleState(i)
        * @ingroup      mTouchAPI
        * @param        i       index of the active-mode sensor
        * @brief        Returns the active-mode sensor's 'toggle' state
        * @hideinitializer
        */ 
        #define     mTouch_GetToggleState(i)        ((uint8_t)(mTouch_toggle >> i) & 0x01)

        /**
        * @def      mTouch_ToggleToggle()
        * @ingroup  mTouchAPI
        * @brief    Toggles the 'toggle' bit for the active-mode sensor
        * @hideinitializer
        */
        /**
        * @def      mTouch_SetToggle()
        * @ingroup  mTouchAPI
        * @brief    Sets the 'toggle' bit for the active-mode sensor
        * @hideinitializer
        */
        /**
        * @def      mTouch_ClearToggle()
        * @ingroup  mTouchAPI
        * @brief    Clears the 'toggle' bit for the active-mode sensor
        * @hideinitializer
        */
        #if (MTOUCH_NUMBER_SENSORS <= 8)
            #define     mTouch_ToggleToggle(i)      mTouch_toggle ^=  (uint8_t) (0x01 << i)
            #define     mTouch_SetToggle(i)         mTouch_toggle |=  (uint8_t) (0x01 << i)     
            #define     mTouch_ClearToggle(i)       mTouch_toggle &= ~(uint8_t) (0x01 << i)     
        #elif (MTOUCH_NUMBER_SENSORS <= 16)
            #define     mTouch_ToggleToggle(i)      mTouch_toggle ^=  (uint16_t)(0x01 << i)
            #define     mTouch_SetToggle(i)         mTouch_toggle |=  (uint16_t)(0x01 << i)
            #define     mTouch_ClearToggle(i)       mTouch_toggle &= ~(uint16_t)(0x01 << i)
        #elif (MTOUCH_NUMBER_SENSORS <= 24)
            #define     mTouch_ToggleToggle(i)      mTouch_toggle ^=  (uint24_t)(0x01 << i)
            #define     mTouch_SetToggle(i)         mTouch_toggle |=  (uint24_t)(0x01 << i)
            #define     mTouch_ClearToggle(i)       mTouch_toggle &= ~(uint24_t)(0x01 << i)
        #elif (MTOUCH_NUMBER_SENSORS <= 32)
            #define     mTouch_ToggleToggle(i)      mTouch_toggle ^=  (uint32_t)(0x01 << i)
            #define     mTouch_SetToggle(i)         mTouch_toggle |=  (uint32_t)(0x01 << i)
            #define     mTouch_ClearToggle(i)       mTouch_toggle &= ~(uint32_t)(0x01 << i)
        #endif
        
    #endif
    

    /**
    * @def      mTouch_EnableScanning()
    * @ingroup  mTouchAPI
    * @brief    Enables mTouch scanning.
    * @hideinitializer
    */ 
    /**
    * @def      mTouch_DisableScanning()
    * @ingroup  mTouchAPI
    * @brief    Disables mTouch scanning.
    * @hideinitializer
    */ 
    #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
        #define     mTouch_EnableScanning()     mTouch_state.scanningEnabled = 1;
        #define     mTouch_DisableScanning()    mTouch_state.scanningEnabled = 0;       
    #else
        #define     mTouch_EnableScanning()     MTOUCH_ISR_TMRxIF = 0; MTOUCH_ISR_TMRxIE = 1;
        #define     mTouch_DisableScanning()    MTOUCH_ISR_TMRxIE = 0;
    #endif



    /**
    * @def      mTouch_Service()
    * @ingroup  mTouchAPI
    * @brief    Performs mTouch decoding, communications output, and mode switching.
    *
    *   The data ready flag is cleared and the mTouch decode routine is called.
    *
    * @hideinitializer
    */ 
    #define mTouch_Service()                    mTouch_state.dataReady = 0; mTouch_Decode() 


    /**
    * @def      mTouch_isDataReady()
    * @ingroup  mTouchAPI
    * @brief    Sometimes function call, sometimes bit-check. Required mTouch API call.
    *
    *   The mTouch_isDataReady() macro will return '1' when:
    *   @li mTouch is being scanned from the ISR and new data is ready to process.
    *   @li mTouch is being called from the mainloop and new data is ready to process.
    *
    *   This macro call is sometimes implemented as a bit-check on a variable. If 
    *   mTouch is not being called from the ISR, this macro is implemented as a
    *   function call to the scan routine. 
    *
    * @return   1 when new data is ready for processing, 0 otherwise.
    * @hideinitializer
    */ 
    #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
        #define mTouch_isDataReady()                mTouch_Scan()                               
    #else
        #define mTouch_isDataReady()                (mTouch_state.dataReady)                    
    #endif

    /**
    * @def      mTouch_checkInterrupt()
    * @ingroup  mTouchAPI
    * @brief    Implements the ISR flag checks to see if the mTouch timer caused the 
    *           interrupt.
    * @hideinitializer
    */ 
    #define mTouch_checkInterrupt()                 (MTOUCH_ISR_TMRxIF && MTOUCH_ISR_TMRxIE)
    
    #if defined(MTOUCH_MATRIX_ENABLED)
    #define mTouch_Matrix_hasChanged()              (mTouch_matrix != mTouch_matrix_latch)
    #define mTouch_Matrix_latchNewValue()           mTouch_matrix_latch = mTouch_matrix
    #define mTouch_Matrix_isPressed()               (mTouch_matrix != 0xFF)
    #define mTouch_Matrix_getColumn()               mTouch_matrix >> MTOUCH_MATRIX_ROW_OFFSET
    #define mTouch_Matrix_getRow()                  mTouch_matrix & MTOUCH_MATRIX_ROW_MASK
    #endif

#endif      // end: ifndef __MTOUCH_H


