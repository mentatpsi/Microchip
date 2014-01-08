/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_eeprom.h
 *
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework Include File
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
/** @file   mTouch_eeprom.h
*   @brief  Internal framework header file implementing EEPROM functionality
*/
#ifndef __MTOUCH_EEPROM_H
#define __MTOUCH_EEPROM_H

    #if (_EEPROMSIZE == 0) && !defined(MTOUCH_EEPROM_STORED_IN_RAM)
        #error The mTouch Framework's EEPROM implementation has been enabled, but the selected part does not contain an EEPROM! Please choose another processor or comment-out MTOUCH_EEPROM_ENABLED in mTouch_config.h
    #endif

    #if defined(MTOUCH_EEPROM_STORED_IN_RAM)
        #define MTOUCH_EEPROM_read(addr)            mTouch_configOptions[addr]
        #define MTOUCH_EEPROM_write(addr, value)    mTouch_configOptions[addr] = value
    #else
    #if defined(_PIC18) | defined(_PIC14)
        #define MTOUCH_EEPROM_read(addr)            EEPROM_READ(addr)
        #define MTOUCH_EEPROM_write(addr, value)    EEPROM_WRITE(addr, value)
    #else
        #define MTOUCH_EEPROM_read(addr)            eeprom_read(addr)
        #define MTOUCH_EEPROM_write(addr, value)    eeprom_write(addr, value)
    #endif
    #endif

    #if defined(MTOUCH_EEPROM_ENABLED)
    void mTouch_EEPROM_Init(void);
    void mTouch_EEPROM_Reset(void);
    #endif

    //
    // MTOUCH EEPROM MAPPING
    //
    
    // Block sizes
    #define   MTOUCH_EEPROM_SYS_SIZE    15
    #define   MTOUCH_EEPROM_SENS_SIZE   4
    #define   MTOUCH_EEPROM_SIZE        MTOUCH_EEPROM_SYS_SIZE + (MTOUCH_EEPROM_SENS_SIZE * MTOUCH_NUMBER_SENSORS)
    
    // Addresses Calculations
    #define   MTOUCH_EEPROM_SYS_ADR     MTOUCH_EEPROM_START_ADR
    #define   MTOUCH_EEPROM_SENS_ADR    MTOUCH_EEPROM_SYS_ADR   + MTOUCH_EEPROM_SYS_SIZE    
    
    // Ending Address Calculations
    #define   MTOUCH_EEPROM_END_ADR     MTOUCH_EEPROM_SENS_ADR  + (MTOUCH_EEPROM_SENS_SIZE * MTOUCH_NUMBER_SENSORS)

    
    //
    // Individual Value Address Calculations
    //
    
    // Noise immunity related parameters
    #define   MTOUCH_EEPROM_pDEB_ADRL       MTOUCH_EEPROM_START_ADR + 0
    #define   MTOUCH_EEPROM_pDEB_ADRH       MTOUCH_EEPROM_START_ADR + 1
    #define   MTOUCH_EEPROM_rDEB_ADRL       MTOUCH_EEPROM_START_ADR + 2
    #define   MTOUCH_EEPROM_rDEB_ADRH       MTOUCH_EEPROM_START_ADR + 3
    #define   MTOUCH_EEPROM_JITTER_ADR      MTOUCH_EEPROM_START_ADR + 4

    // Baseline related parameters
    #define   MTOUCH_EEPROM_BLWEIGHT_ADR    MTOUCH_EEPROM_START_ADR + 5
    #define   MTOUCH_EEPROM_BLRATE_ADRL     MTOUCH_EEPROM_START_ADR + 6
    #define   MTOUCH_EEPROM_BLRATE_ADRH     MTOUCH_EEPROM_START_ADR + 7
    
    // Touch behavior related parameters
    #define   MTOUCH_EEPROM_pTIMEOUT_ADRL   MTOUCH_EEPROM_START_ADR + 8 
    #define   MTOUCH_EEPROM_pTIMEOUT_ADRH   MTOUCH_EEPROM_START_ADR + 9 
    #define   MTOUCH_EEPROM_SpS_ADRL        MTOUCH_EEPROM_START_ADR + 10
    #define   MTOUCH_EEPROM_SpS_ADRH        MTOUCH_EEPROM_START_ADR + 11
    #define   MTOUCH_EEPROM_SCALE_ADR       MTOUCH_EEPROM_START_ADR + 12
    #define   MTOUCH_EEPROM_DECSTEP_ADRL    MTOUCH_EEPROM_START_ADR + 13
    #define   MTOUCH_EEPROM_DECSTEP_ADRH    MTOUCH_EEPROM_START_ADR + 14
    
    // Sensor Block
    #define   MTOUCH_EEPROM_pTHRESH_ADRL    MTOUCH_EEPROM_SENS_ADR
    #define   MTOUCH_EEPROM_pTHRESH_ADRH    MTOUCH_EEPROM_SENS_ADR + 1
    #define   MTOUCH_EEPROM_rTHRESH_ADRL    MTOUCH_EEPROM_SENS_ADR + 2
    #define   MTOUCH_EEPROM_rTHRESH_ADRH    MTOUCH_EEPROM_SENS_ADR + 3
    
    //
    // Generate Default EEPROM Threshold Values
    //
    
    #define MTOUCH_pTHRESH(index)           __paste(THRESHOLD_PRESS_SENSOR, index)
    #define MTOUCH_rTHRESH(index)           __paste(THRESHOLD_RELEASE_SENSOR, index)
    
    #define MTOUCH_EEPROM_THRESH(index)     ,(uint8_t)(MTOUCH_pTHRESH(index) & 0xFF),                   \
                                            (uint8_t)(MTOUCH_pTHRESH(index) >> 8),                      \
                                            (uint8_t)(MTOUCH_rTHRESH(index) & 0xFF),                    \
                                            (uint8_t)(MTOUCH_rTHRESH(index) >> 8)
                                            
    #define MTOUCH_EEPROM_THRESH0           MTOUCH_EEPROM_THRESH(0)
    #define MTOUCH_EEPROM_THRESH1           MTOUCH_EEPROM_THRESH(1)
    #define MTOUCH_EEPROM_THRESH2           MTOUCH_EEPROM_THRESH(2)
    #define MTOUCH_EEPROM_THRESH3           MTOUCH_EEPROM_THRESH(3)
    #define MTOUCH_EEPROM_THRESH4           MTOUCH_EEPROM_THRESH(4)
    #define MTOUCH_EEPROM_THRESH5           MTOUCH_EEPROM_THRESH(5)
    #define MTOUCH_EEPROM_THRESH6           MTOUCH_EEPROM_THRESH(6)
    #define MTOUCH_EEPROM_THRESH7           MTOUCH_EEPROM_THRESH(7)
    #define MTOUCH_EEPROM_THRESH8           MTOUCH_EEPROM_THRESH(8)
    #define MTOUCH_EEPROM_THRESH9           MTOUCH_EEPROM_THRESH(9)
    #define MTOUCH_EEPROM_THRESH10          MTOUCH_EEPROM_THRESH(10)
    #define MTOUCH_EEPROM_THRESH11          MTOUCH_EEPROM_THRESH(11)
    #define MTOUCH_EEPROM_THRESH12          MTOUCH_EEPROM_THRESH(12)
    #define MTOUCH_EEPROM_THRESH13          MTOUCH_EEPROM_THRESH(13)
    #define MTOUCH_EEPROM_THRESH14          MTOUCH_EEPROM_THRESH(14)
    #define MTOUCH_EEPROM_THRESH15          MTOUCH_EEPROM_THRESH(15)
    #define MTOUCH_EEPROM_THRESH16          MTOUCH_EEPROM_THRESH(16)
    #define MTOUCH_EEPROM_THRESH17          MTOUCH_EEPROM_THRESH(17)
    #define MTOUCH_EEPROM_THRESH18          MTOUCH_EEPROM_THRESH(18)
    #define MTOUCH_EEPROM_THRESH19          MTOUCH_EEPROM_THRESH(19)
    #define MTOUCH_EEPROM_THRESH20          MTOUCH_EEPROM_THRESH(20)
    #define MTOUCH_EEPROM_THRESH21          MTOUCH_EEPROM_THRESH(21)
    #define MTOUCH_EEPROM_THRESH22          MTOUCH_EEPROM_THRESH(22)
    #define MTOUCH_EEPROM_THRESH23          MTOUCH_EEPROM_THRESH(23)
    #define MTOUCH_EEPROM_THRESH24          MTOUCH_EEPROM_THRESH(24)
    #define MTOUCH_EEPROM_THRESH25          MTOUCH_EEPROM_THRESH(25)
    #define MTOUCH_EEPROM_THRESH26          MTOUCH_EEPROM_THRESH(26)
    #define MTOUCH_EEPROM_THRESH27          MTOUCH_EEPROM_THRESH(27)
    #define MTOUCH_EEPROM_THRESH28          MTOUCH_EEPROM_THRESH(28)
    #define MTOUCH_EEPROM_THRESH29          MTOUCH_EEPROM_THRESH(29)
    
    #define MTOUCH_EEPROM_THRESHOLDS        MTOUCH_EEPROM_THRESH0   \
                                            MTOUCH_EEPROM_THRESH1   \
                                            MTOUCH_EEPROM_THRESH2   \
                                            MTOUCH_EEPROM_THRESH3   \
                                            MTOUCH_EEPROM_THRESH4   \
                                            MTOUCH_EEPROM_THRESH5   \
                                            MTOUCH_EEPROM_THRESH6   \
                                            MTOUCH_EEPROM_THRESH7   \
                                            MTOUCH_EEPROM_THRESH8   \
                                            MTOUCH_EEPROM_THRESH9   \
                                            MTOUCH_EEPROM_THRESH10  \
                                            MTOUCH_EEPROM_THRESH11  \
                                            MTOUCH_EEPROM_THRESH12  \
                                            MTOUCH_EEPROM_THRESH13  \
                                            MTOUCH_EEPROM_THRESH14  \
                                            MTOUCH_EEPROM_THRESH15  \
                                            MTOUCH_EEPROM_THRESH16  \
                                            MTOUCH_EEPROM_THRESH17  \
                                            MTOUCH_EEPROM_THRESH18  \
                                            MTOUCH_EEPROM_THRESH19  \
                                            MTOUCH_EEPROM_THRESH20  \
                                            MTOUCH_EEPROM_THRESH21  \
                                            MTOUCH_EEPROM_THRESH22  \
                                            MTOUCH_EEPROM_THRESH23  \
                                            MTOUCH_EEPROM_THRESH24  \
                                            MTOUCH_EEPROM_THRESH25  \
                                            MTOUCH_EEPROM_THRESH26  \
                                            MTOUCH_EEPROM_THRESH27  \
                                            MTOUCH_EEPROM_THRESH28  \
                                            MTOUCH_EEPROM_THRESH29   
    
    #if MTOUCH_NUMBER_SENSORS < 30
        #undef  MTOUCH_EEPROM_THRESH29
        #define MTOUCH_EEPROM_THRESH29
    #endif
    #if MTOUCH_NUMBER_SENSORS < 29
        #undef  MTOUCH_EEPROM_THRESH28
        #define MTOUCH_EEPROM_THRESH28
    #endif
    #if MTOUCH_NUMBER_SENSORS < 28
        #undef  MTOUCH_EEPROM_THRESH27
        #define MTOUCH_EEPROM_THRESH27
    #endif
    #if MTOUCH_NUMBER_SENSORS < 27
        #undef  MTOUCH_EEPROM_THRESH26
        #define MTOUCH_EEPROM_THRESH26
    #endif
    #if MTOUCH_NUMBER_SENSORS < 26
        #undef  MTOUCH_EEPROM_THRESH25
        #define MTOUCH_EEPROM_THRESH25
    #endif
    #if MTOUCH_NUMBER_SENSORS < 25 
        #undef  MTOUCH_EEPROM_THRESH24
        #define MTOUCH_EEPROM_THRESH24
    #endif
    #if MTOUCH_NUMBER_SENSORS < 24
        #undef  MTOUCH_EEPROM_THRESH23
        #define MTOUCH_EEPROM_THRESH23
    #endif
    #if MTOUCH_NUMBER_SENSORS < 23
        #undef  MTOUCH_EEPROM_THRESH22
        #define MTOUCH_EEPROM_THRESH22
    #endif
    #if MTOUCH_NUMBER_SENSORS < 22
        #undef  MTOUCH_EEPROM_THRESH21
        #define MTOUCH_EEPROM_THRESH21
    #endif
    #if MTOUCH_NUMBER_SENSORS < 21
        #undef  MTOUCH_EEPROM_THRESH20
        #define MTOUCH_EEPROM_THRESH20
    #endif
    #if MTOUCH_NUMBER_SENSORS < 20
        #undef  MTOUCH_EEPROM_THRESH19
        #define MTOUCH_EEPROM_THRESH19 
    #endif
    #if MTOUCH_NUMBER_SENSORS < 19
        #undef  MTOUCH_EEPROM_THRESH18
        #define MTOUCH_EEPROM_THRESH18
    #endif
    #if MTOUCH_NUMBER_SENSORS < 18
        #undef  MTOUCH_EEPROM_THRESH17
        #define MTOUCH_EEPROM_THRESH17
    #endif
    #if MTOUCH_NUMBER_SENSORS < 17
        #undef  MTOUCH_EEPROM_THRESH16
        #define MTOUCH_EEPROM_THRESH16
    #endif
    #if MTOUCH_NUMBER_SENSORS < 16
        #undef  MTOUCH_EEPROM_THRESH15
        #define MTOUCH_EEPROM_THRESH15
    #endif
    #if MTOUCH_NUMBER_SENSORS < 15
        #undef  MTOUCH_EEPROM_THRESH14
        #define MTOUCH_EEPROM_THRESH14
    #endif
    #if MTOUCH_NUMBER_SENSORS < 14
        #undef  MTOUCH_EEPROM_THRESH13
        #define MTOUCH_EEPROM_THRESH13
    #endif
    #if MTOUCH_NUMBER_SENSORS < 13
        #undef  MTOUCH_EEPROM_THRESH12
        #define MTOUCH_EEPROM_THRESH12
    #endif
    #if MTOUCH_NUMBER_SENSORS < 12
        #undef  MTOUCH_EEPROM_THRESH11
        #define MTOUCH_EEPROM_THRESH11
    #endif
    #if MTOUCH_NUMBER_SENSORS < 11
        #undef  MTOUCH_EEPROM_THRESH10
        #define MTOUCH_EEPROM_THRESH10
    #endif
    #if MTOUCH_NUMBER_SENSORS < 10
        #undef  MTOUCH_EEPROM_THRESH9
        #define MTOUCH_EEPROM_THRESH9
    #endif
    #if MTOUCH_NUMBER_SENSORS < 9
        #undef  MTOUCH_EEPROM_THRESH8
        #define MTOUCH_EEPROM_THRESH8
    #endif
    #if MTOUCH_NUMBER_SENSORS < 8
        #undef  MTOUCH_EEPROM_THRESH7
        #define MTOUCH_EEPROM_THRESH7
    #endif
    #if MTOUCH_NUMBER_SENSORS < 7
        #undef  MTOUCH_EEPROM_THRESH6
        #define MTOUCH_EEPROM_THRESH6
    #endif
    #if MTOUCH_NUMBER_SENSORS < 6
        #undef  MTOUCH_EEPROM_THRESH5
        #define MTOUCH_EEPROM_THRESH5
    #endif
    #if MTOUCH_NUMBER_SENSORS < 5
        #undef  MTOUCH_EEPROM_THRESH4
        #define MTOUCH_EEPROM_THRESH4
    #endif
    #if MTOUCH_NUMBER_SENSORS < 4
        #undef  MTOUCH_EEPROM_THRESH3
        #define MTOUCH_EEPROM_THRESH3
    #endif
    #if MTOUCH_NUMBER_SENSORS < 3
        #undef  MTOUCH_EEPROM_THRESH2
        #define MTOUCH_EEPROM_THRESH2
    #endif
    #if MTOUCH_NUMBER_SENSORS < 2
        #undef  MTOUCH_EEPROM_THRESH1
        #define MTOUCH_EEPROM_THRESH1
    #endif
    
    

    #define MTOUCH_EEPROM_pTHRESHOLD(index) (uint16_t)((uint16_t)(MTOUCH_EEPROM_read(MTOUCH_EEPROM_pTHRESH_ADRH + (MTOUCH_EEPROM_SENS_SIZE*index))) << 8) + MTOUCH_EEPROM_read(MTOUCH_EEPROM_pTHRESH_ADRL + (MTOUCH_EEPROM_SENS_SIZE*index))
    #define MTOUCH_EEPROM_rTHRESHOLD(index) (uint16_t)((uint16_t)(MTOUCH_EEPROM_read(MTOUCH_EEPROM_rTHRESH_ADRH + (MTOUCH_EEPROM_SENS_SIZE*index))) << 8) + MTOUCH_EEPROM_read(MTOUCH_EEPROM_rTHRESH_ADRL + (MTOUCH_EEPROM_SENS_SIZE*index))

 

#endif
