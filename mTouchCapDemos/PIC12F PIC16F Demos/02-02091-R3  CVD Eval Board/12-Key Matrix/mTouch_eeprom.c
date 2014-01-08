/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_eeprom.c
 *  Dependencies:    mTouch.h
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch EEPROM Functions
 *                   - Implements the mTouch EEPROM initialization and
 *                     editing process. 
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
 
#include "mTouch.h"

#if defined(MTOUCH_EEPROM_ENABLED)


    #if defined(MTOUCH_EEPROM_ENABLED)
    
    // At compile-time, initialize the first 8 bytes of EEPROM to 0xFF.
    // This erases the mTouch initialization byte, allowing a new configuration to be loaded on power-up.
    #if !defined(MTOUCH_EEPROM_STORED_IN_RAM)
    __EEPROM_DATA(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF);             // Must write 8 bytes at a time.
    #endif
    
    //
    // NOTE: Do not change the order of these values without also editing the address calculations in mTouch_eeprom.h
    //       New values can be safely added to the end without causing calculation errors.
    //   
    
    const uint8_t   mTouch_EEPROM_default       [MTOUCH_EEPROM_SIZE]    =   {
                                                                                (uint8_t)(MTOUCH_DEBOUNCE_PRESS & 0xFF),                
                                                                                (uint8_t)(MTOUCH_DEBOUNCE_PRESS >> 8),                  
                                                                                (uint8_t)(MTOUCH_DEBOUNCE_RELEASE & 0xFF),              
                                                                                (uint8_t)(MTOUCH_DEBOUNCE_RELEASE >> 8),                
                                                                                (uint8_t)(MTOUCH_JITTER_MASK),                                         
                                                                                (uint8_t)(MTOUCH_BASELINE_WEIGHT),                       
                                                                                (uint8_t)(MTOUCH_BASELINE_RATE & 0xFF),                 
                                                                                (uint8_t)(MTOUCH_BASELINE_RATE >> 8),                   
                                                                                (uint8_t)(MTOUCH_BUTTON_TIMEOUT & 0xFF),                
                                                                                (uint8_t)(MTOUCH_BUTTON_TIMEOUT >> 8),                  
                                                                                (uint8_t)(MTOUCH_SAMPLES_PER_SCAN & 0xFF),              
                                                                                (uint8_t)(MTOUCH_SAMPLES_PER_SCAN >> 8),                
                                                                                (uint8_t)(MTOUCH_SCALING),                               
                                                                                (uint8_t)(MTOUCH_DECIMATION_MAX_STEP & 0xFF),           
                                                                                (uint8_t)(MTOUCH_DECIMATION_MAX_STEP >> 8)           // Lack of comma is intentional: provided in threshold macro.
                                                                                MTOUCH_EEPROM_THRESHOLDS                                
                                                                            };
    #endif
    
          #if defined(MTOUCH_EEPROM_STORED_IN_RAM)
          uint8_t   mTouch_configOptions        [MTOUCH_EEPROM_START_ADR + MTOUCH_EEPROM_SIZE];
          #endif
    
    // Function Prototypes
    void mTouch_EEPROM_Init(void);
    void mTouch_EEPROM_Reset(void);

    // Initializes the EEPROM
    void mTouch_EEPROM_Init()
    {
        // Only writes the default values to the EEPROM if the mTouch initialization
        // value is not present at the starting address. This ensures configuration
        // settings and edits are (both) non-volatile and reset when programmed.
        //  So...
        //      Programming the Microcontroller:        Resets EEPROM values
        //      Powering on/off the Microcontroller:    No change to EEPROM values

        __delay_ms(2);
        
        if (MTOUCH_EEPROM_read(0x01) != MTOUCH_EEPROM_INIT_VALUE)                                
        {   
            mTouch_EEPROM_Reset();                                  // mTouch EEPROM Block
            MTOUCH_EEPROM_write(0x01, MTOUCH_EEPROM_INIT_VALUE);    // Initialization Value
        }
    }
    
    // Resets the EEPROM
    void mTouch_EEPROM_Reset()
    {    
        // The default EEPROM values are stored in constant memory. Loop to initialize.
        for (uint8_t i = 0; i < MTOUCH_EEPROM_SIZE; i++)
        {
            MTOUCH_EEPROM_write(MTOUCH_EEPROM_START_ADR + i, mTouch_EEPROM_default[i]);
        }
    }
    
#endif