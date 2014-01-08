/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouchCVD_macroLibrary_PIC18F.h
 *  Dependencies:    mTouch.h
 *                   mTouch_macroLibrary_common.h
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework PIC18F Macro Library Header
 *                   - Editting this file will make customer support for
 *                     your application extremely difficult. Be careful,
 *                     as it can also have a negative impact on your
 *                     system's noise susceptibility.
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
/** @file   mTouchCVD_macroLibrary_PIC18F.h
*   @brief  Internal framework header file that defines the macros used to 
*           generate the CVD acquisition method for the PIC18F devices.
*
* @note These macros should not be edited in most cases. If you do edit 
*       them, it could negatively impact the noise robustness of the final 
*       system.
*/
#ifndef __MTOUCHCVD_MACROLIBRARY_H
#define __MTOUCHCVD_MACROLIBRARY_H

#define ljmp    goto

#include "mTouch_macroLibrary_common.h"

#undef  SAVE_STATE
#if defined(HI_TECH_C)
    #define SAVE_STATE()                        \
        do {                                    \
            asm("movwf  _int_w");               \
            asm("movff  STATUS, _int_status");  \
            asm("movff  BSR, _int_bsr");        \
        } while (0)
#else
    #define SAVE_STATE()                        \
        do {                                    \
            _asm    movwf  int_w                \
                    movff  STATUS, int_status   \
                    movff  BSR, int_bsr         \
            _endasm                             \
        } while (0)
#endif

#undef  RESTORE_STATE
#if defined(HI_TECH_C)
    #define RESTORE_STATE()                     \
        do {                                    \
            asm("movff  _int_bsr, BSR");        \
            asm("movf   _int_w, W");            \
            asm("movff  _int_status, STATUS");  \
        } while (0)
#else
    #define RESTORE_STATE()                     \
        do {                                    \
            _asm    movff  int_bsr, BSR         \
                    movf   int_w, WREG          \
                    movff  int_status, STATUS   \
            _endasm                             \
        } while (0)
#endif
#undef  MTOUCH_SLEEP_STABILIZE_OSC
#define MTOUCH_SLEEP_STABILIZE_OSC()

#undef  JITTER_START_TIME
#undef  JITTER_MAIN_LOOP
#if defined(MTOUCH_JITTER_ENABLE) 
    #define JITTER_START_TIME()                                         \
        do {                                                            \
            if (mTouch_jitter & 0x01) {                                 \
                mTouch_jitter = (uint8_t)(mTouch_jitter >> 1) ^ 0xB4;   \
            } else {                                                    \
                mTouch_jitter = (uint8_t)(mTouch_jitter >> 1);          \
            }                                                           \
            MTOUCH_ISR_TMR_C = mTouch_jitter & MTOUCH_JITTER_MASK;      \
        } while(0)
        
    #define JITTER_MAIN_LOOP()                                                  \
        do {                                                                    \
            if (mTouch_jitter & 0x01) {                                         \
                mTouch_jitter = (uint8_t)(mTouch_jitter >> 1) ^ 0xB4;           \
            } else {                                                            \
                mTouch_jitter = (uint8_t)(mTouch_jitter >> 1);                  \
            }                                                                   \
            for (uint8_t i = (mTouch_jitter & MTOUCH_JITTER_MASK); i > 0; i--); \
        } while(0)

#else           
    #define JITTER_START_TIME()     MTOUCH_ISR_TMR_C = 0
    #define JITTER_MAIN_LOOP()      
#endif

#undef  CHECK_AND_CLEAR_TMRxIF
#define CHECK_AND_CLEAR_TMRxIF()        \
do {                                    \
    if (MTOUCH_ISR_TMRxIF == 0)         \
    {                                   \
        asm("ljmp END_MTOUCH_SCAN");    \
    } else {                            \
        MTOUCH_ISR_TMRxIF = 0;          \
    }                                   \
} while (0)



#define CVD_RS_INIT_A(indexRef)                 MTOUCH_LAT_C_SENSOR(indexRef) |=   (uint8_t)(1 << MTOUCH_PIN_SENSOR(indexRef))
#define CVD_RS_INIT_B(indexRef)                 MTOUCH_LAT_C_SENSOR(indexRef) &= ~ (uint8_t)(1 << MTOUCH_PIN_SENSOR(indexRef))

#define CVD_RS_MUXSELECT(index, indexRef)       ADCON0 = MTOUCH_SELECT_SENSOR(indexRef);       \
                                                ADCON0bits.DONE = 1;                  

#define CVD_RS_PREPARE(index)                   

// NOTE: Previously a NOP delay(11) after ADON
#define CVD_RS_CONNECT_A(index)                 ADCON0bits.ADON = 0;                                                            /*  Stop sampling to avoid completion   */  \
                                                ADCON0bits.ADON = 1;                                                            /*  Start again to finish charging      */  \
                                                MTOUCH_TRIS_C_SENSOR(index)   |=  ((uint8_t)(1 << MTOUCH_PIN_SENSOR(index)));   /*  Set sensor to input                 */  \
                                                ADCON0 = MTOUCH_SELECT_SENSOR(index);                                           /*  ADC Mux --> Sensor                  */
                                                
#define CVD_RS_CONNECT_B(index)                 CVD_RS_CONNECT_A(index)

#define CVD_RS_SET_GODONE()                     ADCON0bits.DONE = 1;                                                            /*  Start conversion.                   */

#define CVD_RS_EXIT_A(index)                    MTOUCH_LAT_C_SENSOR(index)    |=  ((uint8_t)(1 << MTOUCH_PIN_SENSOR(index)));   /*  Set sensor LAT high                 */  \
                                                MTOUCH_TRIS_C_SENSOR(index)   &= ~((uint8_t)(1 << MTOUCH_PIN_SENSOR(index)));   /*  Set sensor TRIS as output           */ 

#define CVD_RS_EXIT_B(index)                    MTOUCH_LAT_C_SENSOR(index)    &= ~((uint8_t)(1 << MTOUCH_PIN_SENSOR(index)));   /*  Set sensor LAT low                  */  \
                                                MTOUCH_TRIS_C_SENSOR(index)   &= ~((uint8_t)(1 << MTOUCH_PIN_SENSOR(index)));   /*  Set sensor TRIS as output           */

           

// MUTUAL SENSOR
#if defined(CVD_MUTUAL_ENABLED)
    #define CVD_RS_MUTUAL_A(index)                                  CVD_MUTUAL_LAT = 0
    #define CVD_RS_MUTUAL_B(index)                                  CVD_MUTUAL_LAT = 1
#elif defined(CVD_MUTUAL_EXCLUSIVE_ENABLED)
    #define CVD_RS_MUTUAL_A(index)                                  CVD_MUTUAL_LAT = 1
    #define CVD_RS_MUTUAL_B(index)                                  CVD_MUTUAL_LAT = 0
#else
    #define CVD_RS_MUTUAL_A(index)                               
    #define CVD_RS_MUTUAL_B(index)                         
#endif


// ACTIVE SCANNING - ENTER, and REFERENCE
// #if defined(CVD_MUTUAL_EXCLUSIVE_ENABLED)
    // #define CVD_RS_INIT_A(ref_port, ref_pin)                        
    // #define CVD_RS_INIT_B(ref_port, ref_pin)                        
    // #define CVD_RS_MUXSELECT(current_channel, ref_channel)          
// #else
    // #if defined(CVD_FORCE_REF_DAC) || (MTOUCH_NUMBER_SENSORS == 1)
        // #define CVD_RS_INIT_A(ref_port, ref_pin)                    
        // #define CVD_RS_INIT_B(ref_port, ref_pin)                    
        // #define CVD_RS_MUXSELECT(current_channel, ref_channel)      
    // #else
        // #define CVD_RS_INIT_A(ref_port, ref_pin)                     
        // #define CVD_RS_INIT_B(ref_port, ref_pin)                    
        // #define CVD_RS_MUXSELECT(current_channel, ref_channel)      
    // #endif
// #endif

// ACTIVE SCANNING - CONNECTION, CONVERSION, and EXIT
// #if defined(CVD_MUTUAL_EXCLUSIVE_ENABLED)
    // #define CVD_RS_PREPARE(port)                                    
    // #define CVD_RS_CONNECT_A(current_channel, pin)                  
    // #define CVD_RS_CONNECT_B(current_channel, pin)                  
    // #define CVD_RS_EXIT_A(port, pin, ref_port, ref_pin)             
    // #define CVD_RS_EXIT_B(port, pin, ref_port, ref_pin)             
// #elif defined(CVD_GUARD_DACOUT_ENABLED)
    // #define CVD_RS_PREPARE(port)                                    
    // #define CVD_RS_CONNECT_A(current_channel, pin)                  
    // #define CVD_RS_CONNECT_B(current_channel, pin)                  
    // #define CVD_RS_EXIT_A(port, pin, ref_port, ref_pin)             
    // #define CVD_RS_EXIT_B(port, pin, ref_port, ref_pin)             
// #elif defined(CVD_GUARD_IO_ENABLED)   
    // #define CVD_RS_PREPARE(port)                                    
    // #define CVD_RS_CONNECT_A(current_channel, pin)                  
    // #define CVD_RS_CONNECT_B(current_channel, pin)                  
    // #define CVD_RS_EXIT_A(port, pin, ref_port, ref_pin)             
    // #define CVD_RS_EXIT_B(port, pin, ref_port, ref_pin)             
// #else
    // #define CVD_RS_PREPARE(port)                                    
    // #define CVD_RS_CONNECT_A(current_channel, pin)                  
    // #define CVD_RS_CONNECT_B(current_channel, pin)                  
    // #define CVD_RS_EXIT_A(port, pin, ref_port, ref_pin)             
    // #define CVD_RS_EXIT_B(port, pin, ref_port, ref_pin)             
// #endif


#undef  MTOUCH_READ_SENSOR
#define MTOUCH_READ_SENSOR(indexSensor, indexRef)           CVD_READ_SENSOR(indexSensor,indexRef)


// #define CVD_READ_SENSOR(index, indexRef)                                                                                            \
    // do {                                                                                                                            \
        // CVD_RS_MUTUAL_A();                                                              /*  Set up mutual sensor, if enabled    */  \
        // CVD_RS_START_CRITICAL();                                                        /*  Start critical code segment         */  \
        // MTOUCH_LAT_C_SENSOR(indexRef) |=   (uint8_t)(1 << MTOUCH_PIN_SENSOR(indexRef));                                             \
        // ADCON0 = MTOUCH_SELECT_SENSOR(indexRef);                                        /*  ADC Mux --> Reference               */  \
        // ADCON0bits.DONE = 1;                                                            /*  Start sampling to connect CHOLD     */  \
        // _NOP_DELAY(18);                                                                 /*  Fixed delay while CHOLD charges     */  \
        // ADCON0bits.ADON = 0;                                                            /*  Stop sampling to avoid completion   */  \
        // ADCON0bits.ADON = 1;                                                            /*  Start again to finish charging      */  \
        // _NOP_DELAY(11);                                                                 /*  Fixed delay while CHOLD charges     */  \
        // MTOUCH_TRIS_C_SENSOR(index)   |=  ((uint8_t)(1 << MTOUCH_PIN_SENSOR(index)));   /*  Set sensor to input                 */  \
        // ADCON0 = MTOUCH_SELECT_SENSOR(index);                                           /*  ADC Mux --> Sensor                  */  \
        // ADCON0bits.DONE = 1;                                                            /*  Start conversion.                   */  \
        // CVD_RS_POSTA_MATH();                                                            /*  Perform math while Scan A completes */  \
        // while(ADCON0bits.DONE);                                                         /*  Make sure conversion is finished    */  \
        // MTOUCH_LAT_C_SENSOR(index)    |=  ((uint8_t)(1 << MTOUCH_PIN_SENSOR(index)));   /*  Set sensor LAT high                 */  \
        // MTOUCH_TRIS_C_SENSOR(index)   &= ~((uint8_t)(1 << MTOUCH_PIN_SENSOR(index)));   /*  Set sensor TRIS as output           */  \
        // MTOUCH_STORE_SCAN_A();                                                          /*  Store the result of the ADC scan    */  \
        // CVD_RS_MUTUAL_B();                                                              /*  Set up mutual sensor, if enabled    */  \
        // ADCON0 = MTOUCH_SELECT_SENSOR(indexRef);                                        /*  ADC Mux --> Reference               */  \
        // MTOUCH_LAT_C_SENSOR(indexRef) &= ~ (uint8_t)(1 << MTOUCH_PIN_SENSOR(indexRef)); /*  Output low on reference             */  \
        // ADCON0bits.DONE = 1;                                                            /*  Start sampling to connect CHOLD     */  \
        // _NOP_DELAY(18);                                                                 /*  Fixed delay while CHOLD charges     */  \
        // ADCON0bits.ADON = 0;                                                            /*  Stop sampling to avoid completion   */  \
        // ADCON0bits.ADON = 1;                                                            /*  Start again to finish charging      */  \
        // _NOP_DELAY(11);                                                                 /*  Fixed delay while CHOLD charges     */  \
        // MTOUCH_TRIS_C_SENSOR(index)   |=  ((uint8_t)(1 << MTOUCH_PIN_SENSOR(index)));   /*  Set sensor to input                 */  \
        // ADCON0 = MTOUCH_SELECT_SENSOR(index);                                           /*  ADC Mux --> Sensor                  */  \
        // ADCON0bits.DONE = 1;                                                            /*  Start conversion.                   */  \
        // while(ADCON0bits.DONE);                                                         /*  Make sure conversion is finished    */  \
        // MTOUCH_LAT_C_SENSOR(index)    &= ~((uint8_t)(1 << MTOUCH_PIN_SENSOR(index)));   /*  Set sensor LAT low                  */  \
        // MTOUCH_TRIS_C_SENSOR(index)   &= ~((uint8_t)(1 << MTOUCH_PIN_SENSOR(index)));   /*  Set sensor TRIS as output           */  \
        // CVD_RS_END_CRITICAL(index);                                                     /*  End critical code segment           */  \
    // } while(0)
    
#endif




