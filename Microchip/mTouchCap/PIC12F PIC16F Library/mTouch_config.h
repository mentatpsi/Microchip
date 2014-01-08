/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_config.h
 *  Dependencies:    mTouch_optionsForConfig.h  
 *                              :: Defines labels for easier configuration
 *                   mTouch_config_slider.h     
 *                              :: Configuration for sliders
 *                   mTouch_config_modes.h
 *                              :: Configuration for scanning modes
 *                   mTouch_config_cvdAdvanced.h
 *                              :: Advanced CVD acquisition options
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework Configuration File
 *                   - The values in this and the other mTouch_config_...
 *                     header files determine how the framework will
 *                     generate the application.
 *                   - See documentation for better explanations of all
 *                     configuration options.
 *************************************************************************/
/***********************************************************************
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
/** @file   mTouch_Config.h
*   @brief  Framework Configuration! Main configuration file for the mTouch 
*           Framework.
*/
#ifndef __MTOUCH_CONFIG_H
#define __MTOUCH_CONFIG_H

#include "mTouch_optionsForConfig.h"
#include "mTouch_config_slider.h"
#include "mTouch_config_modes.h"
#include "mTouch_config_cvdAdvanced.h"

//****************************************************************************************************
//  ____            _                   ____       _               
// / ___| _   _ ___| |_ ___ _ __ ___   / ___|  ___| |_ _   _ _ __  
// \___ \| | | / __| __/ _ \ '_ ` _ \  \___ \ / _ \ __| | | | '_ \ 
//  ___) | |_| \__ \ ||  __/ | | | | |  ___) |  __/ |_| |_| | |_) |
// |____/ \__, |___/\__\___|_| |_| |_| |____/ \___|\__|\__,_| .__/ 
//        |___/                                             |_|        
//
//****************************************************************************************************
    #define _XTAL_FREQ                      32000000 // <-- Oscillator frequency of your MCU in Hz   

    // Framework-Application Integration Type
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //  Option Label                    :: Option Behavior
    //.................................................................................................
    //  MTOUCH_CONTROLS_ISR             :: The framework defines and controls 100% of the ISR.
    //  MTOUCH_CALLED_FROM_ISR          :: The application's ISR function is responsible for calling mTouch_Scan().
    //  MTOUCH_CALLED_FROM_MAINLOOP     :: The mainloop mTouch API will automatically call the scan function.    
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    
    #define MTOUCH_INTEGRATION_TYPE         MTOUCH_CALLED_FROM_ISR

    
        // If MTOUCH_INTEGRATION_TYPE is defined as either MTOUCH_CONTROLS_ISR or MTOUCH_CALLED_FROM_ISR,
        // this option will determine which timer is used to clock the mTouch scan routine.
        #define MTOUCH_ISR_TIMER            0       // NOTE: You are responsible for initializing
                                                    //       the timer with any desired pre- or
                                                    //       post-scaling options including any
                                                    //       period registers (PR2/4/6). 
                                                    // 8-bit Timers Only. Not compatible with TMR1/3/5.
    
    
    // mTouch Scan Method Functionality
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //  Option Label                    :: Option Behavior
    //.................................................................................................
    //  MTOUCH_SCANS_ONE_SENSOR         :: One mTouch function call will result in one sensor being scanned once.
    //  MTOUCH_SCANS_ALL_SENSORS        :: One mTouch function call will result in all sensors being scanned once.
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    #define MTOUCH_SCAN_FUNCTIONALITY       MTOUCH_SCANS_ONE_SENSOR

    
    
    // mTouch Scan Method Functionality
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //  Option Label                    :: Option Behavior
    //.................................................................................................
    //  MTOUCH_ALWAYS_CONTROLS_ADC      :: ADC is considered under the control of the framework at all times.
    //  MTOUCH_RELEASES_ADC_AFTER_SCAN  :: ADC is released and available for use between scans.
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    
    #define MTOUCH_ADC_CONTROL              MTOUCH_ALWAYS_CONTROLS_ADC
                                                    

                                                    //:::::::::::::::::::::::::::::::::::::::::::::::::
                                                    // System Version Control
                                                    //
                                                    //  These values are used only when communicating
                                                    //  with the mTouch PC software to relay
                                                    //  information about the current version of
                                                    //  firmware being used.
                                                    //
    #define MTOUCH_BOARD_ID                 0x00    //  Which board is this? 
                                                    //      Used in 'V2' mTouch GUI to identify the board.
                                                    //      No effect on framework behavior.
    #define MTOUCH_HARDWARE_VERSION         0x01    //  Which hardware revision?
                                                    //      Displayed in 'V2' mTouch GUI. 
                                                    //      No effect on framework behavior.
    #define MTOUCH_FRAMEWORK_VERSION        0x20    //  Which version of the mTouch framework is this?
                                                    //      Displayed in 'V2' mTouch GUI. 
                                                    //      No effect on framework behavior.
    #define MTOUCH_SOFTWARE_VERSION         0x01    //  Which version of user-application code is this?
                                                    //      Displayed in 'V2' mTouch GUI. 
                                                    //      No effect on framework behavior.
                                                    //:::::::::::::::::::::::::::::::::::::::::::::::::
                                                    
    // mTouch EEPROM Configuration Value Storage
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //#define MTOUCH_EEPROM_ENABLED                 // <-- If defined, mTouch configuration values
                                                    //     will be stored in and read from EEPROM.
                                                    //     This allows for real-time updates to values
                                                    //     such as debounce max values, and thresholds.
                                                    //     The mComm module can be used to read/write
                                                    //     these values. See mComm_config.h.
                                                    
        #define MTOUCH_EEPROM_START_ADR     0x02    // <-- If EEPROM is enabled, this value determines
                                                    //     where the starting address of the mTouch
                                                    //     storage block will be located.
                                                    //      Valid Values: Must be 0x02 or higher.
                                                    //  
                                                    //     DO NOT CHOOSE 0x00:
                                                    //      Brown-out resets may result in unwanted 
                                                    //      EEPROM writes to address 0x00 - so this 
                                                    //      address should always be skipped.
                                                    //
                                                    //     DO NOT CHOOSE 0x01:
                                                    //      An initialization byte is placed at location
                                                    //      0x01 to determine if the EEPROM has been
                                                    //      initialized. By checking this byte, we can
                                                    //      initialize the configuration values on the
                                                    //      first power up, and leave them alone for all
                                                    //      future power ups.
                                                    //
                                                    //     NOTE: MTOUCH_EEPROM_END_ADR will be defined
                                                    //           automatically to let you know the
                                                    //           address of the final mTouch value.
                                                    
        #define MTOUCH_EEPROM_INIT_VALUE    0xBD    // <-- If EEPROM is enabled, the first time the 
                                                    //     EEPROM is initialized, it will load this
                                                    //     value into the starting address location.
                                                    //     On power-up after that, the device will
                                                    //     see that this value is already loaded into
                                                    //     EEPROM and will not re-initialize. This
                                                    //     allows the device to maintain its
                                                    //     configuration through power cycles.
                                                    //
                                                    //     NOTE: When programming the device, the
                                                    //           first 8 bytes of EEPROM will be
                                                    //           cleared (0xFF) to reset the mTouch
                                                    //           EEPROM initialization byte and force
                                                    //           a rewrite of the EEPROM values.
                                                    //      On normal device power up, the init value
                                                    //           is not erased.
                                                    
        #define MTOUCH_EEPROM_STORED_IN_RAM         // <-- If EEPROM is enabled and this value is
                                                    //     defined, the mTouch configuration options
                                                    //     will be stored in a RAM array - not in EEPROM.
                                                    //     This can be used to support two-way
                                                    //     communications on PICs with no EEPROM or
                                                    //     as a way to reset to factory settings when
                                                    //     the processor is powered off.
        
        
//****************************************************************************************************
//  ____                              ____       _               
// / ___|  ___ _ __  ___  ___  _ __  / ___|  ___| |_ _   _ _ __  
// \___ \ / _ \ '_ \/ __|/ _ \| '__| \___ \ / _ \ __| | | | '_ \ 
//  ___) |  __/ | | \__ \ (_) | |     ___) |  __/ |_| |_| | |_) |
// |____/ \___|_| |_|___/\___/|_|    |____/ \___|\__|\__,_| .__/ 
//                                                        |_|    
//
//****************************************************************************************************
                                                    
    #define MTOUCH_NUMBER_SENSORS       8           // <-- Total number of mTouch sensors that will
                                                    //     be scanned. This includes all matrix,
                                                    //     proximity, button, or slider traces.

    // Analog Pin Configuration
    #define MTOUCH_SENSOR0              AN0          
    #define MTOUCH_SENSOR1              AN1          /// @cond    
    #define MTOUCH_SENSOR2              AN2    
    #define MTOUCH_SENSOR3              AN3   
    #define MTOUCH_SENSOR4              AN4 
    #define MTOUCH_SENSOR5              AN5 
    #define MTOUCH_SENSOR6              AN6
    #define MTOUCH_SENSOR7              AN7
    #define MTOUCH_SENSOR8              AN8 
    #define MTOUCH_SENSOR9              AN9
    #define MTOUCH_SENSOR10             AN10    
    #define MTOUCH_SENSOR11             AN11    
    #define MTOUCH_SENSOR12             AN12    
    #define MTOUCH_SENSOR13             AN13
    #define MTOUCH_SENSOR14             AN14   
    #define MTOUCH_SENSOR15             AN15  
    #define MTOUCH_SENSOR16             AN16  
    #define MTOUCH_SENSOR17             AN17  
    #define MTOUCH_SENSOR18             AN18  
    #define MTOUCH_SENSOR19             AN19  
    #define MTOUCH_SENSOR20             AN20   
    #define MTOUCH_SENSOR21             AN21   
    #define MTOUCH_SENSOR22             AN22   
    #define MTOUCH_SENSOR23             AN23   
    #define MTOUCH_SENSOR24             AN24       // TIP:
    #define MTOUCH_SENSOR25             AN25       // Extra sensor definitions can be ignored - they will
    #define MTOUCH_SENSOR26             AN26       // only be implemented if MTOUCH_NUMBER_SENSORS defines that
    #define MTOUCH_SENSOR27             AN27       // they are enabled.
    #define MTOUCH_SENSOR28             AN28       
    #define MTOUCH_SENSOR29             AN29      
    /// @endcond


   
   
   
    // Press Threshold Configuration
    #define THRESHOLD_PRESS_SENSOR0     500         
    #define THRESHOLD_PRESS_SENSOR1     500         /// @cond
    #define THRESHOLD_PRESS_SENSOR2     500         // <-- Required difference between the sensor's baseline
    #define THRESHOLD_PRESS_SENSOR3     500         //     value and the current reading to trigger a press    
    #define THRESHOLD_PRESS_SENSOR4     500         //     Allowable Range :: [ 1 to 65535 ]
    #define THRESHOLD_PRESS_SENSOR5     500        
    #define THRESHOLD_PRESS_SENSOR6     500  
    #define THRESHOLD_PRESS_SENSOR7     500  
    #define THRESHOLD_PRESS_SENSOR8     500  
    #define THRESHOLD_PRESS_SENSOR9     500  
    #define THRESHOLD_PRESS_SENSOR10    500
    #define THRESHOLD_PRESS_SENSOR11    500
    #define THRESHOLD_PRESS_SENSOR12    500
    #define THRESHOLD_PRESS_SENSOR13    500
    
    #define THRESHOLD_PRESS_SENSOR14    200
    #define THRESHOLD_PRESS_SENSOR15    200
    #define THRESHOLD_PRESS_SENSOR16    200
    #define THRESHOLD_PRESS_SENSOR17    200
    #define THRESHOLD_PRESS_SENSOR18    200
    #define THRESHOLD_PRESS_SENSOR19    200
    #define THRESHOLD_PRESS_SENSOR20    200
    #define THRESHOLD_PRESS_SENSOR21    200      
    #define THRESHOLD_PRESS_SENSOR22    200         // TIP:
    #define THRESHOLD_PRESS_SENSOR23    200         // Extra threshold definitions for sensors that are not
    #define THRESHOLD_PRESS_SENSOR24    200         // implemented can be ignored.
    #define THRESHOLD_PRESS_SENSOR25    200  
    #define THRESHOLD_PRESS_SENSOR26    200  
    #define THRESHOLD_PRESS_SENSOR27    200  
    #define THRESHOLD_PRESS_SENSOR28    200  
    #define THRESHOLD_PRESS_SENSOR29    200         
    /// @endcond



    #define MTOUCH_RELEASE_THRESH_FACTOR    0.5     // <-- Defines the hysteresis between press and release
                                                    //     In other words, '0.4' would result in a release
                                                    //     threshold equal to 40% of the press threshold.
                                                    //     
                                                    //     Example: With a finger on the sensor, the reading
                                                    //      settles to a value (1000). When a finger is added, 
                                                    //      we see the reading increase (1200). The press 
                                                    //      threshold is then set to determine how much shift
                                                    //      we want to see before we get a press. In this case
                                                    //      we see a shift of 200. So we can set the threshold
                                                    //      to 160. The release threshold is a percentage of
                                                    //      the press threshold. If set to 0.4, it will be
                                                    //      40% of the 160 press threshold, or '64' counts.
                                                    //      So the sensor will remain pressed until the reading
                                                    //      returns to a value of 1064 or less.
                                                    //
                                                    //     Allowable Range :: [ 0.30 to 1.00 ]
 


//****************************************************************************************************
//  _____ _ _ _            _             
// |  ___(_) | |_ ___ _ __(_)_ __   __ _ 
// | |_  | | | __/ _ \ '__| | '_ \ / _` |
// |  _| | | | ||  __/ |  | | | | | (_| |
// |_|   |_|_|\__\___|_|  |_|_| |_|\__, |
//                                 |___/ 
//
//****************************************************************************************************

    #define MTOUCH_SAMPLES_PER_SCAN     30          // <-- Number of times each sensor will be scanned 
                                                    //     before asserting the mTouch_state.dataReady 
                                                    //     bit. See the mTouch Framework's documentation 
                                                    //     for help achieving a specific response time.
                                                    //     Allowed Range :: [ 1 to 65535 ]
                                                
    #define MTOUCH_BASELINE_WEIGHT      3           // <-- Determines the weight given to the reading
    // The configuration option formerly known as   //     in the calculation of a new baseline average.
    //  MTOUCH_AVG_UPDATE                           //     The reading will have a weight of 
                                                    //     1/(2^MTOUCH_BASELINE_WEIGHT) in the baseline 
                                                    //     calculation. 
                                                    //     Example: If this value is '3', the baseline 
                                                    //              calculation would be:
                                                    //        baseline = (1/8)*reading + (7/8)*baseline
                                                    //     Example: If this value is '2', the baseline 
                                                    //              calculation would be:
                                                    //        baseline = (1/4)*reading + (3/4)*baseline
                                                    //     Allowed Values :: [ 1 to 4 ]

    #define MTOUCH_BASELINE_RATE        10          // <-- Determines how often the baseline average will
    // The configuration option formerly known as   //     be updated. A new calculation will occur every
    //  MTOUCH_AVG_RATE                             //     N'th mTouch decode service call, where 'N' is 
                                                    //     this value. (Fastest = 1)
                                                    //     Allowed Range :: [ 1 to 65535 ]

    #define MTOUCH_DECIMATION_MAX_STEP  10          // <-- Maximum change allowed during one CVD sample
    
    #define MTOUCH_JITTER_BITS          6           // <-- Determines the number of bits to use to implement
                                                    //     a randomized delay between mTouch scans.
                                                    //     Allowed Range :: [ 0 to 8 ], 0 = off.

    #define MTOUCH_SCALING              1           // <-- Defines the degree of post-scaling that is 
                                                    //     implemented on the acquisition's accumulation
                                                    //     register before being stored as the result.
                                                    //     The scaling factor should be set so that it is
                                                    //     not possible for the result register to overflow 
                                                    //     (Max result output value is 65535.)
                                                    //     Set this equal to the number of times to 
                                                    //     right-shift the value.
                                                    //     Allowed Range :: [ 0 to 8 ]
                                                    //       0 - No scaling is performed.
                                                    //           1:1 ratio between accumulator and result
                                                    //
                                                    //       4 - Accumulator is divided by 16.
                                                    //           16:1 ratio between accumulator and result
                                                    //
                                                    //       8 - Accumulator is divided by 256.
                                                    //           256:1 ratio between accumulator and result
                                                    //
                                                
                                            
                                                
                                                
//****************************************************************************************************
//  ____                     _ _             
// |  _ \  ___  ___ ___   __| (_)_ __   __ _ 
// | | | |/ _ \/ __/ _ \ / _` | | '_ \ / _` |
// | |_| |  __/ (_| (_) | (_| | | | | | (_| |
// |____/ \___|\___\___/ \__,_|_|_| |_|\__, |
//                                     |___/ 
//****************************************************************************************************
    

    #define MTOUCH_POWER_UP_SAMPLES     30          // <-- Determines the number of initialization samples
                                                    //     taken at power up. Each sensor will be decoded
                                                    //     this number of times.
                                                    //     Allowable Range :: [ 1 to 65535 ]

    #define	MTOUCH_BUTTON_TIMEOUT       300         // <-- Number of consecutive 'pressed' decode decisions
                                                    //     before the sensor's state is reset. 
                                                    //     See the note, below, for more information about
                                                    //     how the baseline's behavior changes when a timeout
                                                    //     condition occurs.
                                                    //     
                                                    //     Allowable Range :: [ 0 to 65535 ], '0' is off.
                                                
    #define	MTOUCH_DEBOUNCE_RELEASE     0           // <-- Number of consecutive scans a button must be seen
                                                    //     as released before a 'released state' is declared
                                                    //     Allowable Range :: [ 0 to 65535 ], '0' is off.
 
    #define	MTOUCH_DEBOUNCE_PRESS    	0           // <-- Number of consecutive scans a button must be seen 
                                                    //     as pressed before a 'pressed state' is declared
                                                    //     Allowable Range :: [ 0 to 65535 ], '0' is off.
                                                
    //#define MTOUCH_TOGGLE_ENABLED                 // <-- If enabled, a toggle latch bit will be implemented for
                                                    //     each active sensor. You can access the current toggle
                                                    //     state using mTouch_GetToggleState(i).
                                                    
    //#define MTOUCH_ERROR_DETECTION_ENABLED        // <-- If enabled, implements the mTouch_errorDetect()
                                                    //     function which can be called by the application to
                                                    //     verify that all enabled sensor lines are floating and
                                                    //     are not being held high or low. If one is found to be
                                                    //     in this condition, the mTouch_state.error flag is set.

                                                    
    // NOTE: The MTOUCH_NEGATIVE_CAPACITANCE option has been removed. The baseline now automatically changes its
    //       behavior after a timeout has occured to solve the issue this option was addressing. On-press, the
    //       baseline remains at the unpressed value of the sensor. After a button timeout has occurred, the 
    //       baseline is reset to the current value of the sensor (the pressed value). This triggers a special
    //       flag (mTouch_stateVars.active.sensor[i].timeout) which causes the baseline to look for the next
    //       large (> 1/2 * Press Threshold) negative shift. When this shift occurs, the baseline will quickly
    //       update itself until the difference between the reading and the baseline is less than the release
    //       threshold. This allows the average to maintain steady filtering while still providing fast recovery
    //       in case of a timeout condition.
                                                    
                                                    
    // mTouch Matrix and Most-Pressed Algorithm Options
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    
    #define MTOUCH_MOST_PRESSED_THRESHOLD   10      // <-- MUST BE DEFINED FOR BOTH MATRIX AND MOST-PRESSED-ONLY
                                                    //     This option determines how much more pressed the
                                                    //     most-pressed sensor must be above all other sensors
                                                    //     in order to allow activation. 
                                                    //     This must also be defined if there is a matrix being
                                                    //     implemented. The matrix will automatically run a most-
                                                    //     pressed algorithm on the rows and columns separately.
                                                    //     It needs this value to determine if the row/column is
                                                    //     sufficiently more-pressed than another.
    
    //#define MTOUCH_MATRIX_ENABLED                 // <-- If enabled, the framework will implement a matrix.
                                                    //     The sensors implementing the rows must all be grouped
                                                    //     together by index. The same is true for the columns.
                                                    //     To have both matrix and non-matrix sensors, the matrix
                                                    //     sensors must be grouped together and either at the start
                                                    //     or end of all the sensors.
                                                    //
        #define MTOUCH_MATRIX_ROW_START         0   // <-- The sensor index of the first row
        #define MTOUCH_MATRIX_ROW_END           1   // <-- The sensor index of the last row
                                                    //
        #define MTOUCH_MATRIX_COLUMN_START      2   // <-- The sensor index of the first column
        #define MTOUCH_MATRIX_COLUMN_END        3   // <-- The sensor index of the last column
                                                    //
                                                    //      
                                                    //     Examples:    VALID       VALID       INVALID     INVALID   
                                                    //
                                                    //     Sensor0    / ROW0        Normal      Normal    / ROW0
                                                    //     Sensor1    | ROW1        Normal    / ROW0      \ ROW1
                                                    //     Sensor2    | COL0      / COL0      | ROW1        Normal
                                                    //     Sensor3    | COL1      | COL1      | COL0        Normal
                                                    //     Sensor4    \ COL2      | COL2      | COL1      / COL0
                                                    //     Sensor5      Normal    | ROW0      \ COL2      | COL1
                                                    //     Sensor6      Normal    \ ROW1        Normal    \ COL2
                                                    //     --------------------------------------------------------
                                                    //     ROW_START      0           5         Matrix      Matrix
                                                    //     ROW_END        1           6         sensors     sensors
                                                    //     COL_START      2           2         not at      not
                                                    //     COL_END        4           4         beginning   grouped
                                                    //                                          or end
    
    
    //#define MTOUCH_MOST_PRESSED_ONLY                // <-- If enabled, only one non-matrix, active sensor will be 
                                                    //     able to be in the pressed state at any given time. 
                                                    //     In order for a sensor to enter the pressed state with
                                                    //     this option enabled, it must:
                                                    //      1. Be pressed enough to cross the sensor's press
                                                    //         threshold value. (ex: THRESHOLD_PRESS_SENSOR0)
                                                    //      2. Be more pressed than the second-most-pressed sensor
                                                    //         by at least MTOUCH_MOST_PRESSED_THRESHOLD.
                                                    //
                                                    //     NOTE: Matrix sensors are separated from 'normal' sensors
                                                    //           and are not compared with each other. Matrix
                                                    //           sensors are automatically split into their 'row'
                                                    //           and 'column' groups to find the most pressed.
                                                    
    

//****************************************************************************************************
//  ____                _           _ _         
// |  _ \ _ __ _____  _(_)_ __ ___ (_) |_ _   _ 
// | |_) | '__/ _ \ \/ / | '_ ` _ \| | __| | | |
// |  __/| | | (_) >  <| | | | | | | | |_| |_| |
// |_|   |_|  \___/_/\_\_|_| |_| |_|_|\__|\__, |
//                                        |___/ 
//****************************************************************************************************
    
    //
    // NOTE: To enable a proximity sensor, you must perform two steps:
    //      
    //      1. Define MTOUCH_NUMBER_PROXIMITY with the number of proximity sensors in total.
    //         This is used to determine how large to make the arrays.
    //
    //      2. There must be one MTOUCH_SENSOR#_IS_PROX definition per enabled proximity sensor.
    //         The value of the definition must be it's "proximity-id" value.
    //         "Proximity-ID" is used as an index to the proximity variables. In total, the IDs
    //         should start at 0 and end with MTOUCH_NUMBER_PROXIMITY-1. The order is arbitrary.
    //
    //          Example:
    //
    //              #define MTOUCH_NUMBER_PROXIMITY     2
    //
    //              #define MTOUCH_SENSOR5_IS_PROX      0
    //              #define MTOUCH_SENSOR2_IS_PROX      1
    //
    
    
    #define MTOUCH_NUMBER_PROXIMITY     0               // Defines how many proximity sensors are activated

    #define MTOUCH_SENSOR3_IS_PROX      0               // MTOUCH_SENSOR3 is a proximity sensor with index 0
    #define MTOUCH_SENSOR1_IS_PROX      1               // MTOUCH_SENSOR1 is a proximity sensor with index 1
    
    // mTouch Proximity Filtering Options
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        #define MTOUCH_PROX_BUFFER_SIZE         5   // <-- This determines the size of the buffer used
                                                    //     in the proximity filter.
                                                    //     
                                                    //     Options -
                                                    //      5   :: Least filtering, fastest response time
                                                    //      9   ::
                                                    //      15  :: Most filtering, longest response time
                                                    
        #define MTOUCH_PROX_REMOVE_EXTREME      1   // <-- The N largest and N smallest values in the
                                                    //     buffer will be removed. The remaining values
                                                    //     will then be averaged to achieve a mean result.
                                                    //
                                                    //     This value is 'N'. It must be less than half
                                                    //     the buffer size.

    
        #define MTOUCH_PROX_USE_32BIT_ACCUM         // <-- If defined, the accumulator used during the
                                                    //     decoding process of the proximity sensor will
                                                    //     be implemented as a 32-bit value. This solves
                                                    //     overflow problems - at the cost of additional
                                                    //     temporary RAM requirements.
    

//****************************************************************************************************
//  ____  _ _     _                                 _  __        ___               _     
// / ___|| (_) __| | ___ _ __ ___    __ _ _ __   __| | \ \      / / |__   ___  ___| |___ 
// \___ \| | |/ _` |/ _ \ '__/ __|  / _` | '_ \ / _` |  \ \ /\ / /| '_ \ / _ \/ _ \ / __|
//  ___) | | | (_| |  __/ |  \__ \ | (_| | | | | (_| |   \ V  V / | | | |  __/  __/ \__ \
// |____/|_|_|\__,_|\___|_|  |___/  \__,_|_| |_|\__,_|    \_/\_/  |_| |_|\___|\___|_|___/
//                                  
//****************************************************************************************************

// See 'mTouch_config_slider.h'


    
//****************************************************************************************************
//   ____                                      _           _   _                 
//  / ___|___  _ __ ___  _ __ ___  _   _ _ __ (_) ___ __ _| |_(_) ___  _ __  ___ 
// | |   / _ \| '_ ` _ \| '_ ` _ \| | | | '_ \| |/ __/ _` | __| |/ _ \| '_ \/ __|
// | |__| (_) | | | | | | | | | | | |_| | | | | | (_| (_| | |_| | (_) | | | \__ \
//  \____\___/|_| |_| |_|_| |_| |_|\__,_|_| |_|_|\___\__,_|\__|_|\___/|_| |_|___/
//
//****************************************************************************************************

//--------------------------------------------------------------------------------------
// #define APFCON_INITIALIZED       // For processors with an APFCON register(s), this 
//                                  // #define can be uncommented to stop the mTouch 
//                                  // Framework from producing a "remember to set 
//                                  // APFCON" warning.
//                                  //
//                                  // RULE OF PROGRAMMING #4: Register bits initialize, 
//                                  //      by law, to the value you don't want. Always 
//                                  //      explicitly initialize.
//--------------------------------------------------------------------------------------

// See 'mComm_config.h'



//****************************************************************************************************
//     _                   _     _ _   _             
//    / \   ___ __ _ _   _(_)___(_) |_(_) ___  _ __    _
//   / _ \ / __/ _` | | | | / __| | __| |/ _ \| '_ \  (_)
//  / ___ \ (_| (_| | |_| | \__ \ | |_| | (_) | | | |  _
// /_/   \_\___\__, |\__,_|_|___/_|\__|_|\___/|_| |_| (_)
//                |_|  
//   ______     ______    ____                     ____            _             _     
//  / ___\ \   / /  _ \  / ___|  ___ __ _ _ __    / ___|___  _ __ | |_ _ __ ___ | |___ 
// | |    \ \ / /| | | | \___ \ / __/ _` | '_ \  | |   / _ \| '_ \| __| '__/ _ \| / __|
// | |___  \ V / | |_| |  ___) | (_| (_| | | | | | |__| (_) | | | | |_| | | (_) | \__ \
//  \____|  \_/  |____/  |____/ \___\__,_|_| |_|  \____\___/|_| |_|\__|_|  \___/|_|___/
//
//****************************************************************************************************

// See 'mTouch_config_cvdAdvanced.h'
    
       
#endif
