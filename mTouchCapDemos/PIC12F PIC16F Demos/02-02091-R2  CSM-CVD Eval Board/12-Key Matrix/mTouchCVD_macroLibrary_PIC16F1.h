/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouchCVD_macroLibrary_PIC16F1.h
 *  Dependencies:    mTouch.h
 *                   mTouch_macroLibrary_common.h
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework Enhanced Core Macro Library Header
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
/** @file   mTouchCVD_macroLibrary_PIC16F1.h
*   @brief  Internal framework header file that defines the macros used to 
*           generate the CVD acquisition method for the PIC16F1 enhanced 
*           core devices.
*
* @note These macros should not be edited in most cases. If you do edit 
*       them, it could negatively impact the noise robustness of the final 
*       system.
*/
#ifndef __MTOUCHCVD_MACROLIBRARY_H
#define __MTOUCHCVD_MACROLIBRARY_H

#include "mTouch_macroLibrary_common.h"


#undef  SAVE_STATE()
#define SAVE_STATE()     
#undef  RESTORE_STATE()
#define RESTORE_STATE()  
// The Enhanced Midrange Core does not require saving/restoring state on entering
// the ISR, so these macros are undefined.


#define CVD_RS_SET_GODONE()                                                         \
    asm("BANKSEL    "   ___mkstr(ADCON0)               );  /*                  */  \
    asm("bsf        "   ___mkstr(NOBANK(ADCON0))   ",1");  /* GO_nDONE = 1;    */ 
    


// Sensor tris set using FSR0
// ADCON0 loaded directly
#define CVD_RSC_FSR0(current_channel, pin)                                                              \
    asm("BANKSEL    "   ___mkstr(ADCON0)                        );  /*                              */  \
    asm("movlw      "   ___mkstr(current_channel)               );  /*                              */  \
    asm("bsf        "   ___mkstr(INDF0)    "," ___mkstr(pin)    );  /* Set the sensor to an input   */  \
    _NOP_DELAY(CVD_SWITCH_DELAY);                                                                       \
    asm("movwf      "   ___mkstr(NOBANK(ADCON0))                );  /* ADC Mux --> Sensor           */  

    
    
    
    
    
// GUARD SENSOR - IO
#if defined(CVD_GUARD_IO_ENABLED) || defined(MTOUCH_UNIQUE_GUARD_OPTIONS)

    // Assumes: FSR0 is Sensor TRIS
    //          FSR1 is Guard LAT
    #define CVD_RSCa_FSR0_gFSR1SET(current_channel, pin)                                                            \
        asm("BANKSEL    "   ___mkstr(ADCON0)                                );  /*                              */  \
        asm("movlw      "   ___mkstr(current_channel)                       );  /*                              */  \
        asm("bsf        "   ___mkstr(INDF1)    "," ___mkstr(CVD_GUARD_PIN)  );  /*                              */  \
        asm("bsf        "   ___mkstr(INDF0)    "," ___mkstr(pin)            );  /* Set the sensor to an input   */  \
        _NOP_DELAY(CVD_SWITCH_DELAY);                                                                               \
        asm("movwf      "   ___mkstr(NOBANK(ADCON0))                        );  /* ADC Mux --> Sensor           */  
        
    #define CVD_RSCb_FSR0_gFSR1CLR(current_channel, pin)                                                            \
        asm("BANKSEL    "   ___mkstr(ADCON0)                                );  /*                              */  \
        asm("movlw      "   ___mkstr(current_channel)                       );  /*                              */  \
        asm("bcf        "   ___mkstr(INDF1)    "," ___mkstr(CVD_GUARD_PIN)  );  /*                              */  \
        asm("bsf        "   ___mkstr(INDF0)    "," ___mkstr(pin)            );  /* Set the sensor to an input   */  \
        _NOP_DELAY(CVD_SWITCH_DELAY);                                                                               \
        asm("movwf      "   ___mkstr(NOBANK(ADCON0))                        );  /* ADC Mux --> Sensor           */  
        
    // FSR0 = Sensor TRIS
    // FSR1 = Guard IO PORT
    #define CVD_RSP_FSR0sTRIS_FSR1gLAT(sensorTris)                                                      \
        asm("movlw LOW  "   ___mkstr(CVD_GUARD_PORT)    );  /*                              */  \
        asm("movwf      "   ___mkstr(FSR1L)             );          /*                              */  \
        asm("movlw HIGH "   ___mkstr(CVD_GUARD_PORT)    );  /*                              */  \
        asm("movwf      "   ___mkstr(FSR1H)             );          /*                              */  \
        asm("movlw LOW  "   ___mkstr(sensorTris)        );          /*                              */  \
        asm("movwf      "   ___mkstr(FSR0L)             );          /*                              */  \
        asm("movlw HIGH "   ___mkstr(sensorTris)        );          /*                              */  \
        asm("movwf      "   ___mkstr(FSR0H)             );          /*                              */  
         
#endif 
 

// GUARD SENSOR - DACOUT PIN
#if defined(CVD_GUARD_DACOUT_ENABLED) || defined(MTOUCH_UNIQUE_GUARD_OPTIONS)
    #if !defined(CVD_FORCE_REF_DAC) && (MTOUCH_NUMBER_SENSORS != 1)
    
        // Assumes: FSR0 is Sensor TRIS
        //          FSR1 is ADCON0
        //          DAC is not reference source
        #define CVD_RSCa_FSR0_FSR1ADC_gDAC(current_channel, pin)                                                        \
            asm("BANKSEL    "   ___mkstr(DACCON1)                               );  /*                              */  \
            asm("movlw      "   ___mkstr(CVD_GUARD_DACCON0_A & 0xDF)            );  /* Clear DACOUT enable bit      */  \
            asm("movwf      "   ___mkstr(NOBANK(DACCON0))                       );  /* Configure DAC                */  \
            asm("movlw      "   ___mkstr(CVD_GUARD_DACCON1_A)                   );  /*                              */  \
            asm("movwf      "   ___mkstr(NOBANK(DACCON1))                       );  /* DAC output level = 1/3 VDD   */  \
            asm("movlw      "   ___mkstr(current_channel)                       );  /*                              */  \
            asm("bsf        "   ___mkstr(INDF0)    "," ___mkstr(pin)            );  /* Set the sensor to an input   */  \
            _NOP_DELAY(CVD_SWITCH_DELAY);                                                                               \
            asm("movwf      "   ___mkstr(INDF1)                                 );  /* ADC Mux --> Sensor           */  \
            asm("bsf        "   ___mkstr(NOBANK(DACCON0))  ",5"                 );  /* DAC Output Enable            */
            
        #define CVD_RSCb_FSR0_FSR1ADC_gDAC(current_channel, pin)                                                        \
            asm("BANKSEL    "   ___mkstr(DACCON1)                               );  /*                              */  \
            asm("movlw      "   ___mkstr(CVD_GUARD_DACCON0_B & 0xDF)            );  /* Clear DACOUT enable bit      */  \
            asm("movwf      "   ___mkstr(NOBANK(DACCON0))                       );  /* Configure DAC                */  \
            asm("movlw      "   ___mkstr(CVD_GUARD_DACCON1_B)                   );  /*                              */  \
            asm("movwf      "   ___mkstr(NOBANK(DACCON1))                       );  /* DAC output level = 2/3 VDD   */  \
            asm("movlw      "   ___mkstr(current_channel)                       );  /*                              */  \
            asm("bsf        "   ___mkstr(INDF0)    "," ___mkstr(pin)            );  /* Set the sensor to an input   */  \
            _NOP_DELAY(CVD_SWITCH_DELAY);                                                                               \
            asm("movwf      "   ___mkstr(INDF1)                                 );  /* ADC Mux --> Sensor           */  \
            asm("bsf        "   ___mkstr(NOBANK(DACCON0))  ",5"                 );  /* DAC Output Enable            */
    
    #else
    
        // Assumes: FSR0 is Sensor TRIS
        //          FSR1 is ADCON0
        //          DAC is also used as reference source
        #define CVD_RSCa_FSR0_FSR1ADC_gDAC(current_channel, pin)                                                        \
            asm("BANKSEL    "   ___mkstr(DACCON1)                               );  /*                              */  \
            asm("movlw      "   ___mkstr(current_channel)                       );  /*                              */  \
            asm("bsf        "   ___mkstr(INDF0)    "," ___mkstr(pin)            );  /* Set the sensor to an input   */  \
            _NOP_DELAY(CVD_SWITCH_DELAY);                                                                               \
            asm("movwf      "   ___mkstr(INDF1)                                 );  /* ADC Mux --> Sensor           */  \
            asm("movlw      "   ___mkstr(CVD_GUARD_DACCON1_A)                   );  /*                              */  \
            asm("movwf      "   ___mkstr(NOBANK(DACCON1))                       );  /* Adjust DAC Output Level      */  \
            asm("bsf        "   ___mkstr(NOBANK(DACCON0))  ",5"                 );  /* DAC Output Enable            */
            
        #define CVD_RSCb_FSR0_FSR1ADC_gDAC(current_channel, pin)                                                        \
            asm("BANKSEL    "   ___mkstr(DACCON1)                               );  /*                              */  \
            asm("movlw      "   ___mkstr(current_channel)                       );  /*                              */  \
            asm("bsf        "   ___mkstr(INDF0)    "," ___mkstr(pin)            );  /* Set the sensor to an input   */  \
            _NOP_DELAY(CVD_SWITCH_DELAY);                                                                               \
            asm("movwf      "   ___mkstr(INDF1)                                 );  /* ADC Mux --> Sensor           */  \
            asm("movlw      "   ___mkstr(CVD_GUARD_DACCON1_B)                   );  /*                              */  \
            asm("movwf      "   ___mkstr(NOBANK(DACCON1))                       );  /* Adjust DAC Output Level      */  \
            asm("bsf        "   ___mkstr(NOBANK(DACCON0))  ",5"                 );  /* DAC Output Enable            */

    #endif
#endif


// FSR0 = Sensor TRIS
// FSR1 = ADCON0
#define CVD_RSP_FSR0sTRIS_FSR1ADC0(sensorTris)                                              \
    asm("movlw LOW  "   ___mkstr(ADCON0)            );  /*                              */  \
    asm("movwf      "   ___mkstr(FSR1L)             );  /*                              */  \
    asm("movlw HIGH "   ___mkstr(ADCON0)            );  /*                              */  \
    asm("movwf      "   ___mkstr(FSR1H)             );  /*                              */  \
    asm("movlw LOW  "   ___mkstr(sensorTris)        );  /*                              */  \
    asm("movwf      "   ___mkstr(FSR0L)             );  /*                              */  \
    asm("movlw HIGH "   ___mkstr(sensorTris)        );  /*                              */  \
    asm("movwf      "   ___mkstr(FSR0H)             );  /*                              */  
    
#define CVD_RSEa_FSR0CLR_mPORTSET_DAC(sensorLat, pin)                                                                                \
    asm("BANKSEL    "   ___mkstr(sensorLat)                                                 );  /*                              */  \
    asm("bsf        "   ___mkstr(NOBANK(sensorLat))  "," ___mkstr(pin)                      );  /* Set sensor LAT high          */  \
    asm("bcf        "   ___mkstr(INDF0)        "," ___mkstr(pin)                            );  /* Set the sensor to an output  */  \
    asm("BANKSEL    "   ___mkstr(PIC_DACOUT_LAT_ASM)                                        );  /*                              */  \
    asm("bsf        "   ___mkstr(NOBANK(PIC_DACOUT_LAT_ASM)) "," ___mkstr(PIC_DACOUT_PIN)   );  /* Set DACOUT LAT high          */  \
    asm("BANKSEL    "   ___mkstr(DACCON0)                                                   );  /*                              */  \
    asm("bcf        "   ___mkstr(NOBANK(DACCON0))       ",5"                                );  /* Turn off DAC output pin      */  

#define CVD_RSEb_FSR0CLR_mPORTCLR_DAC(sensorLat, pin)                                                                                \
    asm("BANKSEL    "   ___mkstr(sensorLat)                                                 );  /*                              */  \
    asm("bcf        "   ___mkstr(NOBANK(sensorLat))      "," ___mkstr(pin)                  );  /* Set sensor LAT low           */  \
    asm("bcf        "   ___mkstr(INDF0)                 "," ___mkstr(pin)                   );  /* Set the sensor to an output  */  \
    asm("BANKSEL    "   ___mkstr(PIC_DACOUT_LAT_ASM)                                        );  /*                              */  \
    asm("bcf        "   ___mkstr(NOBANK(PIC_DACOUT_LAT_ASM)) "," ___mkstr(PIC_DACOUT_PIN)   );  /* Set DACOUT LAT low           */  \
    asm("BANKSEL    "   ___mkstr(DACCON0)                                                   );  /*                              */  \
    asm("bcf        "   ___mkstr(NOBANK(_DACCON0))       ",5"                               );  /* Turn off DAC output pin      */  
    
// EXCLUSIVELY MUTUAL SENSING
#define CVD_RSC_FSR0_Input_Only(current_channel, pin)                                                   \
    asm("bsf        "   ___mkstr(_INDF0)    "," ___mkstr(pin)   );  /* Set the sensor to an input   */  \

// REFERENCE SOURCE - DAC
#define CVD_RSIa_DAC_VDD()                                                                  \
    asm("BANKSEL    "   ___mkstr(DACCON0)           );  /*                              */  \
    asm("movlw      "   ___mkstr(PIC_DACCON0_VDD)   );  /*                              */  \
    asm("movwf      "   ___mkstr(NOBANK(DACCON0))   );  /* DACCON0 = PIC_DACCON0_VDD    */  \
    asm("movlw      "   ___mkstr(PIC_DACCON1_VDD)   );  /*                              */  \
    asm("movwf      "   ___mkstr(NOBANK(DACCON1))   );  /* DACCON1 = PIC_DACCON1_VDD    */  
#define CVD_RSIb_DAC_VSS()                                                                  \
    asm("BANKSEL    "   ___mkstr(DACCON0)           );  /*                              */  \
    asm("movlw      "   ___mkstr(PIC_DACCON0_VSS)   );  /*                              */  \
    asm("movwf      "   ___mkstr(NOBANK(DACCON0))   );  /* DACCON0 = PIC_DACCON0_VSS    */  \
    asm("movlw      "   ___mkstr(PIC_DACCON1_VSS)   );  /*                              */  \
    asm("movwf      "   ___mkstr(NOBANK(DACCON1))   );  /* DACCON1 = PIC_DACCON1_VSS    */  
#define CVD_RS_MUX_DAC()                                                            \
    asm("BANKSEL    "   ___mkstr(ADCON0)                    );  /*                  */  \
    asm("movlw      "   ___mkstr(MTOUCH_SELECT_DAC_NOGO)    );  /*                  */  \
    asm("movwf      "   ___mkstr(NOBANK(ADCON0))            );  /* ADC Mux --> DAC  */    


// REFERENCE SOURCE - IO
#define CVD_RSIb_rLAT_CLR(sensorLat, ref_pin)                                                                                      \
    asm("BANKSEL    "   ___mkstr(sensorLat)                          );  /*                              */   \
    asm("bcf        "   ___mkstr(NOBANK(sensorLat)) "," ___mkstr(ref_pin)    );  /* Clear the reference port bit */
#define CVD_RSIa_rLAT_SET(sensorLat, ref_pin)                                                                                      \
    asm("BANKSEL    "   ___mkstr(sensorLat)                          );  /*                              */   \
    asm("bsf        "   ___mkstr(NOBANK(sensorLat)) "," ___mkstr(ref_pin)    );  /* Set the reference port bit   */
#define CVD_RS_MUX_REF(ref_channel)                                                 \
    asm("BANKSEL    "   ___mkstr(ADCON0)                );  /*                  */  \
    asm("movlw      "   ___mkstr(ref_channel)           );  /*                  */  \
    asm("movwf      "   ___mkstr(NOBANK(ADCON0))        );  /* ADC Mux --> Ref  */ 
   
#define CVD_RS_MUX_SENSOR(current_channel)                                              \
    asm("BANKSEL    "   ___mkstr(ADCON0)                );  /*                      */  \
    asm("movlw      "   ___mkstr(current_channel)       );  /*                      */  \
    asm("movwf      "   ___mkstr(NOBANK(ADCON0))        );  /* ADC Mux --> Sensor   */    
   
// GENERIC EXITING OPTIONS
// Assumes: FSR0 is Sensor TRIS
//          No Mutual Sensor
//          No DACOUT Guard
#define CVD_RSEa_FSR0_CLR_mPORTSET(sensorLat, pin)                                                               \
    asm("BANKSEL    "   ___mkstr(sensorLat)                             );  /*                              */  \
    asm("bsf        "   ___mkstr(NOBANK(sensorLat)) "," ___mkstr(pin)   );  /* Set sensor LAT high          */  \
    asm("bcf        "   ___mkstr(INDF0)             "," ___mkstr(pin)   );  /* Set the sensor to an output  */
#define CVD_RSEb_FSR0_CLR_mPORTCLR(sensorLat, pin)                                                               \
    asm("BANKSEL    "   ___mkstr(sensorLat)                             );  /*                              */  \
    asm("bcf        "   ___mkstr(NOBANK(sensorLat)) "," ___mkstr(pin)   );  /* Set sensor LAT low           */  \
    asm("bcf        "   ___mkstr(INDF0)             "," ___mkstr(pin)   );  /* Set the sensor to an output  */
    
// Assumes: FSR0 is Sensor TRIS
//          FSR1 is Sensor LAT
//          No Mutual Sensor
//          No DACOUT Guard
#define CVD_RSEa_FSR0_CLR_FSR1_SET(pin)                                                                 \
    asm("bsf        "   ___mkstr(INDF1)    "," ___mkstr(pin)   );  /* Set sensor LAT high          */  \
    asm("bcf        "   ___mkstr(INDF0)    "," ___mkstr(pin)   );  /* Set the sensor to an output  */
#define CVD_RSEb_FSR0_CLR_FSR1_CLR(pin)                                                                 \
    asm("bcf        "  ___mkstr(INDF1)     "," ___mkstr(pin)   );  /* Set sensor LAT low           */  \
    asm("bcf        "  ___mkstr(INDF0)     "," ___mkstr(pin)   );  /* Set the sensor to an output  */
    


// PREPARE FOR CONNECTION
    
// FSR0 = Sensor TRIS
// FSR1 = Sensor LAT
#define CVD_RSP_FSR0_sTRIS_FSR1_sLAT(sensorLat, sensorTris)                                 \
    asm("movlw LOW  "   ___mkstr(sensorLat)         );  /*                              */  \
    asm("movwf      "   ___mkstr(FSR1L)             );  /*                              */  \
    asm("movlw HIGH "   ___mkstr(sensorLat)         );  /*                              */  \
    asm("movwf      "   ___mkstr(FSR1H)             );  /*                              */  \
    asm("movlw LOW  "   ___mkstr(sensorTris)        );  /*                              */  \
    asm("movwf      "   ___mkstr(FSR0L)             );  /*                              */  \
    asm("movlw HIGH "   ___mkstr(sensorTris)        );  /*                              */  \
    asm("movwf      "   ___mkstr(FSR0H)             );  /*                              */  
 
 
 
 
 
 
 
 
 
 
 
 
 
 

// MUTUAL SENSOR
#if defined(CVD_MUTUAL_ENABLED)
    #define CVD_RS_MUTUAL_A(index)                              CVD_MUTUAL_LAT = 0
    #define CVD_RS_MUTUAL_B(index)                              CVD_MUTUAL_LAT = 1
#elif defined(CVD_MUTUAL_EXCLUSIVE_ENABLED)
    #define CVD_RS_MUTUAL_A(index)                              CVD_MUTUAL_LAT = 1
    #define CVD_RS_MUTUAL_B(index)                              CVD_MUTUAL_LAT = 0
#else
    #define CVD_RS_MUTUAL_A(index)                               
    #define CVD_RS_MUTUAL_B(index)                         
#endif

// ACTIVE SCANNING - ENTER, and REFERENCE
#if defined(CVD_MUTUAL_EXCLUSIVE_ENABLED)
    #define CVD_RS_INIT_A(indexRef)                        
    #define CVD_RS_INIT_B(indexRef)                        
    #define CVD_RS_MUXSELECT(index, indexRef)                   CVD_RS_MUX_SENSOR(MTOUCH_SELECT_SENSOR(index))
#else
    #if defined(CVD_FORCE_REF_DAC) || (MTOUCH_NUMBER_SENSORS == 1)
        #if defined(MTOUCH_AD_DAC_NOGO) 
            #define CVD_RS_INIT_A(indexRef)                     CVD_RSIa_DAC_VDD()
            #define CVD_RS_INIT_B(indexRef)                     CVD_RSIb_DAC_VSS()
            #define CVD_RS_MUXSELECT(index, indexRef)           CVD_RS_MUX_DAC()
        #else
            #if defined(CVD_FORCE_REF_DAC)
                #warning This part does not have a DAC.         CVD_FORCE_REF_DAC is being ignored.
                #define CVD_RS_INIT_A(indexRef)                 CVD_RSIa_rLAT_SET(MTOUCH_LAT_ASM_SENSOR(indexRef), MTOUCH_PIN_SENSOR(indexRef)) 
                #define CVD_RS_INIT_B(indexRef)                 CVD_RSIb_rLAT_CLR(MTOUCH_LAT_ASM_SENSOR(indexRef), MTOUCH_PIN_SENSOR(indexRef)) 
                #define CVD_RS_MUXSELECT(index, indexRef)       CVD_RS_MUX_REF(MTOUCH_SELECT_SENSOR(indexRef))
            #elif (MTOUCH_NUMBER_SENSORS == 1) && !defined(PIC_ADC_HCVD_AVAILABLE)
                #error The mTouch Framework requires a DAC mux option on the ADC in order to scan only one sensor but this part does not have a DAC.
            #endif
        #endif
    #else
        #define CVD_RS_INIT_A(indexRef)                         CVD_RSIa_rLAT_SET(MTOUCH_LAT_ASM_SENSOR(indexRef), MTOUCH_PIN_SENSOR(indexRef)) 
        #define CVD_RS_INIT_B(indexRef)                         CVD_RSIb_rLAT_CLR(MTOUCH_LAT_ASM_SENSOR(indexRef), MTOUCH_PIN_SENSOR(indexRef)) 
        #define CVD_RS_MUXSELECT(index, indexRef)               CVD_RS_MUX_REF(MTOUCH_SELECT_SENSOR(indexRef))
    #endif
#endif

// ACTIVE SCANNING - CONNECTION, CONVERSION, and EXIT
#if defined(CVD_MUTUAL_EXCLUSIVE_ENABLED)
    #define CVD_RS_PREPARE(index)                               CVD_RSP_FSR0_sTRIS_FSR1_sLAT(MTOUCH_LAT_ASM_SENSOR(index), MTOUCH_TRIS_ASM_SENSOR(index))
    #define CVD_RS_CONNECT_A(index)                             CVD_RSC_FSR0_Input_Only(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_CONNECT_B(index)                             CVD_RSC_FSR0_Input_Only(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_EXIT_A(index)                                CVD_RSEa_FSR0_CLR_FSR1_SET(MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_EXIT_B(index)                                CVD_RSEb_FSR0_CLR_FSR1_CLR(MTOUCH_PIN_SENSOR(index))
#elif defined(CVD_GUARD_DACOUT_ENABLED)
    #define CVD_RS_PREPARE(index)                               CVD_RSP_FSR0sTRIS_FSR1ADC0(MTOUCH_TRIS_ASM_SENSOR(index))
    #define CVD_RS_CONNECT_A(index)                             CVD_RSCa_FSR0_FSR1ADC_gDAC(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_CONNECT_B(index)                             CVD_RSCb_FSR0_FSR1ADC_gDAC(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_EXIT_A(index)                                CVD_RSEa_FSR0CLR_mPORTSET_DAC(MTOUCH_LAT_ASM_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_EXIT_B(index)                                CVD_RSEb_FSR0CLR_mPORTCLR_DAC(MTOUCH_LAT_ASM_SENSOR(index), MTOUCH_PIN_SENSOR(index))
#elif defined(CVD_GUARD_IO_ENABLED)   
    #define CVD_RS_PREPARE(index)                               CVD_RSP_FSR0sTRIS_FSR1gLAT(MTOUCH_TRIS_ASM_SENSOR(index))
    #define CVD_RS_CONNECT_A(index)                             CVD_RSCa_FSR0_gFSR1SET(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_CONNECT_B(index)                             CVD_RSCb_FSR0_gFSR1CLR(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_EXIT_A(index)                                CVD_RSEa_FSR0_CLR_mPORTSET(MTOUCH_LAT_ASM_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_EXIT_B(index)                                CVD_RSEb_FSR0_CLR_mPORTCLR(MTOUCH_LAT_ASM_SENSOR(index), MTOUCH_PIN_SENSOR(index))
#else
    #define CVD_RS_PREPARE(index)                               CVD_RSP_FSR0_sTRIS_FSR1_sLAT(MTOUCH_LAT_ASM_SENSOR(index), MTOUCH_TRIS_ASM_SENSOR(index))
    #define CVD_RS_CONNECT_A(index)                             CVD_RSC_FSR0(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_CONNECT_B(index)                             CVD_RSC_FSR0(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_EXIT_A(index)                                CVD_RSEa_FSR0_CLR_FSR1_SET(MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_EXIT_B(index)                                CVD_RSEb_FSR0_CLR_FSR1_CLR(MTOUCH_PIN_SENSOR(index))
#endif


    
#endif




