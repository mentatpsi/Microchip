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
 *  Description:     mTouch Framework PIC16F Core Macro Library Header
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
/** @file mTouchCVD_macroLibrary_PIC16F.h
*   @brief  Internal framework header file that defines the macros used to 
*           generate the CVD acquisition method for the PIC12/16F mid-range 
*           core devices.
*
* @note These macros should not be edited in most cases. If you do edit 
*       them, it could negatively impact the noise robustness of the final 
*       system.
*/
#ifndef __MTOUCHCVD_MACROLIBRARY_H
#define __MTOUCHCVD_MACROLIBRARY_H

#include "mTouch_macroLibrary_common.h"


#undef MTOUCH_SLEEP_STABILIZE_OSC()
#define MTOUCH_SLEEP_STABILIZE_OSC()

// Store current state for:
//  * Working register
//  * STATUS register
//  * PCLATH register
//  * FSR register

// #define SAVE_STATE()                \
// do {                                \
    // asm("movwf  _int_w");           \
    // asm("swapf  _int_w, F");        \
    // asm("movf   STATUS, W");        \
    // asm("clrf   STATUS");           \
    // asm("movwf  _int_status");      \
    // asm("movf   PCLATH, W");        \
    // asm("clrf   PCLATH");           \
    // asm("movwf  _int_pclath");      \
    // asm("movf   FSR, W");           \
    // asm("movwf  _int_fsr");         \
// } while (0)

// Restore current state for:
//  * Working register
//  * STATUS register
//  * PCLATH register
//  * FSR register

// #define RESTORE_STATE()             \
// do {                                \
    // asm("movf   _int_fsr,W");       \
    // asm("movwf  FSR");              \
    // asm("movf   _int_pclath,W");    \
    // asm("movwf  PCLATH");           \
    // asm("movf   _int_status,W");    \
    // asm("movwf  STATUS");           \
    // asm("swapf  _int_w, W");        \
// } while (0)

#define CVD_RS_SET_GODONE()                                                         \
    asm("BANKSEL    "   ___mkstr(ADCON0)               );  /*                  */  \
    asm("bsf        "   ___mkstr(NOBANK(ADCON0))   ",1");  /* GO_nDONE = 1;    */ 


    
// Sensor tris set using FSR
// ADCON0 loaded directly
#define CVD_RSC_FSR(current_channel, pin)                                                               \
    asm("BANKSEL    "   ___mkstr(ADCON0)                       );  /*                              */  \
    asm("movlw      "   ___mkstr(current_channel)               );  /*                              */  \
    asm("bsf        "   ___mkstr(INDF)     "," ___mkstr(pin)   );  /* Set the sensor to an input   */  \
    _NOP_DELAY(CVD_SWITCH_DELAY);                                                                       \
    asm("movwf      "   ___mkstr(NOBANK(ADCON0))               );  /* ADC Mux --> Sensor           */  
    
// GUARD SENSOR - IO
#if defined(CVD_GUARD_IO_ENABLED)

    #define CVD_RSCa_FSR_gSET(current_channel, pin)                                                                     \
        asm("BANKSEL    "   ___mkstr(ADCON0)                                   );  /*                              */  \
        asm("movlw      "   ___mkstr(current_channel)                           );  /*                              */  \
        asm("bsf        "   ___mkstr(CVD_GUARD_PORT) "," ___mkstr(CVD_GUARD_PIN));  /*                              */  \
        asm("bsf        "   ___mkstr(INDF)          "," ___mkstr(pin)          );  /* Set the sensor to an input   */  \
        _NOP_DELAY(CVD_SWITCH_DELAY);                                                                                   \
        asm("movwf      "   ___mkstr(NOBANK(ADCON0))                           );  /* ADC Mux --> Sensor           */     
        
    #define CVD_RSCb_FSR_gCLR(current_channel, pin)                                                                     \
        asm("BANKSEL    "   ___mkstr(ADCON0)                                   );  /*                              */  \
        asm("movlw      "   ___mkstr(current_channel)                           );  /*                              */  \
        asm("bcf        "   ___mkstr(CVD_GUARD_PORT) "," ___mkstr(CVD_GUARD_PIN));  /*                              */  \
        asm("bsf        "   ___mkstr(INDF)          "," ___mkstr(pin)          );  /* Set the sensor to an input   */  \
        _NOP_DELAY(CVD_SWITCH_DELAY);                                                                                   \
        asm("movwf      "   ___mkstr(NOBANK(ADCON0))                           );  /* ADC Mux --> Sensor           */   
           
#endif     

    
    
// EXCLUSIVELY MUTUAL SENSING
#define CVD_RSC_FSR_Input_Only(current_channel, pin)    asm("bsf" ___mkstr(INDF) "," ___mkstr(pin));  /* Set the sensor to an input   */


// REFERENCE SOURCE - IO
#define CVD_RSIb_rLAT_CLR(ref_port, ref_pin)                                                                \
    asm("BANKSEL    "   ___mkstr(ref_port)                          );  /*                              */  \
    asm("bcf        "   ___mkstr(ref_port) "," ___mkstr(ref_pin)    );  /* Clear the reference port bit */
    
#define CVD_RSIa_rLAT_SET(ref_port, ref_pin)                                                                \
    asm("BANKSEL    "   ___mkstr(ref_port)                          );  /*                              */  \
    asm("bsf        "   ___mkstr(ref_port) "," ___mkstr(ref_pin)    );  /* Set the reference port bit   */
    
#define CVD_RS_MUX_REF(ref_channel)                                                 \
    asm("BANKSEL    "   ___mkstr(ADCON0)               );  /*                  */  \
    asm("movlw      "   ___mkstr(ref_channel)           );  /*                  */  \
    asm("movwf      "   ___mkstr(NOBANK(ADCON0))       );  /* ADC Mux --> Ref  */ 

#define CVD_RS_MUX_SENSOR(current_channel)                                              \
    asm("BANKSEL    "   ___mkstr(ADCON0)               );  /*                      */  \
    asm("movlw      "   ___mkstr(current_channel)       );  /*                      */  \
    asm("movwf      "   ___mkstr(NOBANK(ADCON0))       );  /* ADC Mux --> Sensor   */    
   
// GENERIC EXITING OPTIONS

// Assumes: FSR is Sensor TRIS
//          No Mutual Sensor
//          No DACOUT Guard
#define CVD_RSEa_FSR_CLR_mPORTSET(sensorLat, pin)                                                                \
    asm("BANKSEL    "   ___mkstr(sensorLat)                              );  /*                              */  \
    asm("bsf        "   ___mkstr(NOBANK(sensorLat))  "," ___mkstr(pin)   );  /* Set sensor LAT high          */  \
    asm("bcf        "   ___mkstr(INDF)        "," ___mkstr(pin)    );  /* Set the sensor to an output  */
    
#define CVD_RSEb_FSR_CLR_mPORTCLR(sensorLat, pin)                                                                \
    asm("BANKSEL    "   ___mkstr(sensorLat)                              );  /*                              */  \
    asm("bcf        "   ___mkstr(NOBANK(sensorLat))  "," ___mkstr(pin)   );  /* Set sensor LAT low           */  \
    asm("bcf        "   ___mkstr(INDF)        "," ___mkstr(pin)    );  /* Set the sensor to an output  */
    
    

 
// Currently Unused
//  FSR = Sensor TRIS
#define CVD_RSP_FSR_sTRIS(port)                                         \
    asm("movlw      "   ___mkstr(port)  );  /*                      */  \
    asm("movwf      "   ___mkstr(FSR)  );  /* FSR = Sensor TRIS    */ 
    
    
#define CVD_RSIa_VRCON_MAX()                                                                        \
    asm("BANKSEL    "   ___mkstr(VRCON)            );  /*                                      */  \
    asm("movlw      "   ___mkstr(PIC_VRCON_MAX)     );  /*                                      */  \
    asm("movwf      "   ___mkstr(NOBANK(VRCON))    );  /* VRCON = PIC_VRCON_MAX (75% of VDD)   */  
    
#define CVD_RSIb_VRCON_MIN()                                                                \
    asm("BANKSEL    "   ___mkstr(VRCON)            );  /*                              */  \
    asm("movlw      "   ___mkstr(PIC_VRCON_MIN)     );  /*                              */  \
    asm("movwf      "   ___mkstr(NOBANK(VRCON))    );  /* VRCON = PIC_VRCON_MAX (VSS)  */   
    
#define CVD_RS_MUX_VRCON()                                                                  \
    asm("BANKSEL    "   ___mkstr(ADCON0)                   );  /*                      */  \
    asm("movlw      "   ___mkstr(MTOUCH_AD_DAC_NOGO)        );  /*                      */  \
    asm("movwf      "   ___mkstr(NOBANK(ADCON0))           );  /* ADC Mux --> CVref    */
    
    
#define CVD_RS_EXIT_FSR_CLR_mPORTSET(port, pin)                                                                                             \
    asm("bcf        "   ___mkstr(INDF)  ","      ___mkstr(pin)     );  /* Set the sensor to an output                                  */  \
    asm("BANKSEL    "   ___mkstr(port)                              );  /*                                                              */  \
    asm("bsf        "   ___mkstr(port)   ","      ___mkstr(pin)     );  /* Set the sensor to output high in preparation for next scan   */
#define CVD_RS_EXIT_FSR_CLR_mPORTCLR(port, pin)                                                                                             \
    asm("bcf        "   ___mkstr(INDF)  ","      ___mkstr(pin)     );  /* Set the sensor to an output                                  */  \
    asm("BANKSEL    "   ___mkstr(port)                              );  /*                                                              */  \
    asm("bcf        "   ___mkstr(port)   ","      ___mkstr(pin)     );  /* Set the sensor to output low now that the scan is complete   */




#define CVD_RS_POSTA_MATH()                                         CVD_RS_POSTA()
#define CVD_RS_POSTB_MATH()                                         CVD_RS_POSTB()

// MUTUAL SENSOR
#if defined(CVD_MUTUAL_ENABLED)
    #define CVD_RS_MUTUAL_A(index)                                       CVD_MUTUAL_LAT = 0
    #define CVD_RS_MUTUAL_B(index)                                       CVD_MUTUAL_LAT = 1
#elif defined(CVD_MUTUAL_EXCLUSIVE_ENABLED)
    #define CVD_RS_MUTUAL_A(index)                                       CVD_MUTUAL_LAT = 1
    #define CVD_RS_MUTUAL_B(index)                                       CVD_MUTUAL_LAT = 0
#else
    #define CVD_RS_MUTUAL_A(index)                               
    #define CVD_RS_MUTUAL_B(index)                         
#endif


// ACTIVE SCANNING - ENTER, and REFERENCE
#if defined(CVD_MUTUAL_EXCLUSIVE_ENABLED)
    #define CVD_RS_INIT_A(indexRef)                         // Left blank
    #define CVD_RS_INIT_B(indexRef)                         // Left blank
    #define CVD_RS_MUXSELECT(index, indexRef)               CVD_RS_MUX_SENSOR(MTOUCH_SELECT_SENSOR(index))
#else
    #if defined(CVD_FORCE_REF_DAC) || (MTOUCH_NUMBER_SENSORS == 1)
        #if defined(MTOUCH_AD_DAC_NOGO) 
            #define CVD_RS_INIT_A(indexRef)                 CVD_RSIa_VRCON_MAX()
            #define CVD_RS_INIT_B(indexRef)                 CVD_RSIb_VRCON_MIN()
            #define CVD_RS_MUXSELECT(index, indexRef)       CVD_RS_MUX_VRCON()
        #else
            #if defined(CVD_FORCE_REF_DAC)
                #warning This part does not have a DAC. CVD_FORCE_REF_DAC is being ignored.
                #define CVD_RS_INIT_A(indexRef)             CVD_RSIa_rLAT_SET(MTOUCH_LAT_ASM_SENSOR(indexRef), MTOUCH_PIN_SENSOR(indexRef)) 
                #define CVD_RS_INIT_B(indexRef)             CVD_RSIb_rLAT_CLR(MTOUCH_LAT_ASM_SENSOR(indexRef), MTOUCH_PIN_SENSOR(indexRef)) 
                #define CVD_RS_MUXSELECT(index, indexRef)   CVD_RS_MUX_REF(MTOUCH_SELECT_SENSOR(indexRef))
            #elif (MTOUCH_NUMBER_SENSORS == 1)
                #error The mTouch Framework requires a CVref mux option on the ADC in order to scan only one sensor but this part does not have a VRCON.
            #endif
        #endif
    #else
        #define CVD_RS_INIT_A(indexRef)                     CVD_RSIa_rLAT_SET(MTOUCH_LAT_ASM_SENSOR(indexRef), MTOUCH_PIN_SENSOR(indexRef)) 
        #define CVD_RS_INIT_B(indexRef)                     CVD_RSIb_rLAT_CLR(MTOUCH_LAT_ASM_SENSOR(indexRef), MTOUCH_PIN_SENSOR(indexRef)) 
        #define CVD_RS_MUXSELECT(index, indexRef)           CVD_RS_MUX_REF(MTOUCH_SELECT_SENSOR(indexRef))
    #endif
#endif

// ACTIVE SCANNING - CONNECTION, CONVERSION, and EXIT
#if defined(CVD_MUTUAL_EXCLUSIVE_ENABLED)
    #define CVD_RS_PREPARE(index)                       CVD_RSP_FSR_sTRIS(MTOUCH_TRIS_ASM_SENSOR(index))
    #define CVD_RS_CONNECT_A(index)                     CVD_RSC_FSR_Input_Only(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_CONNECT_B(index)                     CVD_RSC_FSR_Input_Only(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_EXIT_A(index)                        CVD_RSEa_FSR_CLR_mPORTSET(MTOUCH_LAT_ASM_SENSOR(index), MTOUCH_PIN_SENSOR(index)) 
    #define CVD_RS_EXIT_B(index)                        CVD_RSEb_FSR_CLR_mPORTCLR(MTOUCH_LAT_ASM_SENSOR(index), MTOUCH_PIN_SENSOR(index)) 
#elif defined(CVD_GUARD_IO_ENABLED)   
    #define CVD_RS_PREPARE(index)                       CVD_RSP_FSR_sTRIS(MTOUCH_TRIS_ASM_SENSOR(index))
    #define CVD_RS_CONNECT_A(index)                     CVD_RSCa_FSR_gSET(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))   
    #define CVD_RS_CONNECT_B(index)                     CVD_RSCb_FSR_gCLR(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))   
    #define CVD_RS_EXIT_A(index)                        CVD_RSEa_FSR_CLR_mPORTSET(MTOUCH_LAT_ASM_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_EXIT_B(index)                        CVD_RSEb_FSR_CLR_mPORTCLR(MTOUCH_LAT_ASM_SENSOR(index), MTOUCH_PIN_SENSOR(index))
#else
    #define CVD_RS_PREPARE(index)                       CVD_RSP_FSR_sTRIS(MTOUCH_TRIS_ASM_SENSOR(index))
    #define CVD_RS_CONNECT_A(index)                     CVD_RSC_FSR(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_CONNECT_B(index)                     CVD_RSC_FSR(MTOUCH_SELECT_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_EXIT_A(index)                        CVD_RSEa_FSR_CLR_mPORTSET(MTOUCH_LAT_ASM_SENSOR(index), MTOUCH_PIN_SENSOR(index))
    #define CVD_RS_EXIT_B(index)                        CVD_RSEb_FSR_CLR_mPORTCLR(MTOUCH_LAT_ASM_SENSOR(index), MTOUCH_PIN_SENSOR(index)) 
#endif

//@}



#endif




