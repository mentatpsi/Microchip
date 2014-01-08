/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_macroLibrary_common.h
 *  Dependencies:    mTouch.h
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework Common Macro Library Header File
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
/** @file   mTouch_macroLibrary_common.h
*   @brief  Implements non-core-specific acquisition macros
*/

/**
* @def      MASKBANK(var,bank)
* @param    var     the variable address to mask
* @param    bank    the bank location of the variable
* @brief    Masks the bank out of the variable address for ASM code usage. 
*           Requires that the actual bank bits being masked equal the bank 
*           value provided.
* @hideinitializer
*/
#define MASKBANK(var,bank)      ((var)^((bank)*80h))
/**
* @def      NOBANK(var)
* @param    var     the variable address to mask
* @brief    Masks the bank out of the variable address for ASM code usage.
* @hideinitializer
*/
#define NOBANK(var)             (var & 0x7F)

/** @name Housekeeping Macros
*
*   These macros perform basic ISR and timer module functions such as checking
*   that the interrupt flag is set and reloading the TMR0 counter.
*/
//@{

#if defined(MTOUCH_ERROR_DETECTION_ENABLED)         

    #define MTOUCH_onDETECTACTION()                  mTouch_state.error = 1     

    #define MTOUCH_CHECKforSHORTonSENSOR(index)                                                                             \
        __paste2(MTOUCH_LAT_C_SENSOR(index),MTOUCH_PIN_SENSOR(index))   = 1;        /* Translates to: LATA0  = 1;       */  \
        __paste2(MTOUCH_TRIS_C_SENSOR(index),MTOUCH_PIN_SENSOR(index))  = 0;        /* Translates to: TRISA0 = 0;       */  \
        asm("NOP"); asm("NOP");                                                     /*                                  */  \
        __paste2(MTOUCH_TRIS_C_SENSOR(index),MTOUCH_PIN_SENSOR(index))  = 1;        /* Translates to: TRISA0 = 1;       */  \
        if (__paste2(MTOUCH_PORT_C_SENSOR(index),MTOUCH_PIN_SENSOR(index)) == 0)    /* Translates to: if(PORTA0 == 0)   */  \
            MTOUCH_onDETECTACTION();                                                /*                                  */  \
                                                                                    /*                                  */  \
        __paste2(MTOUCH_LAT_C_SENSOR(index),MTOUCH_PIN_SENSOR(index))   = 0;        /* Translates to: LATA0  = 0;       */  \
        __paste2(MTOUCH_TRIS_C_SENSOR(index),MTOUCH_PIN_SENSOR(index))  = 0;        /* Translates to: TRISA0 = 0;       */  \
        asm("NOP"); asm("NOP");                                                     /*                                  */  \
        __paste2(MTOUCH_TRIS_C_SENSOR(index),MTOUCH_PIN_SENSOR(index))  = 1;        /* Translates to: TRISA0 = 1;       */  \
        if (__paste2(MTOUCH_PORT_C_SENSOR(index),MTOUCH_PIN_SENSOR(index)) == 1)    /* Translates to: if(PORTA0 == 1)   */  \
            MTOUCH_onDETECTACTION();

        
#endif

/**
* @def          SAVE_STATE()
* @brief        Executes any required actions to save the current main-loop 
*               process
* @hideinitializer
*/
#define SAVE_STATE()                    \
    do {                                \
        asm("movwf  _int_w");           \
        asm("swapf  _int_w, F");        \
        asm("movf   STATUS, W");        \
        asm("clrf   STATUS");           \
        asm("movwf  _int_status");      \
        asm("movf   PCLATH, W");        \
        asm("clrf   PCLATH");           \
        asm("movwf  _int_pclath");      \
        asm("movf   FSR, W");           \
        asm("movwf  _int_fsr");         \
    } while (0)
// The Enhanced Midrange Core does not require saving/restoring state on entering
// the ISR, so these macros will be undefined later for those parts. 
// They remain implemented in the non-enhanced core version.

/**
* @def          RESTORE_STATE()
* @brief        Executes any required actions to save the current main-loop process
* @hideinitializer
*/
#define RESTORE_STATE()                 \
    do {                                \
        asm("movf   _int_fsr,W");       \
        asm("movwf  FSR");              \
        asm("movf   _int_pclath,W");    \
        asm("movwf  PCLATH");           \
        asm("movf   _int_status,W");    \
        asm("movwf  STATUS");           \
        asm("swapf  _int_w, W");        \
    } while (0)
// The Enhanced Midrange Core does not require saving/restoring state on entering
// the ISR, so these macros will be undefined later for those parts. 
// They remain implemented in the non-enhanced core version.

/**
* @def          MTOUCH_SLEEP_STABILIZE_OSC()
* @brief        Causes the system to wait until the oscillator's frequency has stabilized
* @hideinitializer
*/
#define MTOUCH_SLEEP_STABILIZE_OSC()            \
    do {                                        \
        while (!HFIOFL);                        \
    } while(0)


#if defined(MTOUCH_JITTER_ENABLE)

    #if !defined(MTOUCH_EEPROM_ENABLED)
        /**
        * @def      JITTER_START_TIME()
        * @brief    Calculates a random value and uses it to seed the mTouch interrupt timer
        *
        *   Implements a linear feedback shift register algorithm to increase the randomness 
        *   of the jitter function. This implementation costs one byte of RAM.
        *
        * @hideinitializer
        */
        #define JITTER_START_TIME()                                                                                             \
            do {                                                                                                                \
                asm("BANKSEL    _mTouch_jitter"                     );  /* Make sure we're starting in the correct bank     */  \
                asm("bcf    "   ___mkstr(STATUS)            ",0"    );  /* Clear the carry bit                              */  \
                asm("rrf    "   ___mkstr(_mTouch_jitter)    ",W"    );  /* Right shift the current jitter seed value        */  \
                asm("btfsc  "   ___mkstr(STATUS)            ",0"    );  /* Check the carry bit - if set, perform XOR        */  \
                asm("xorlw      0xB4"                               );  /* (cond) XOR the jitter seed with 0xB4             */  \
                asm("movwf  "   ___mkstr(_mTouch_jitter)            );  /* Store the result as the new jitter seed value    */  \
                asm("andlw  "   ___mkstr(MTOUCH_JITTER_MASK)        );  /* Mask the seed value to limit the number of bits  */  \
                asm("BANKSEL "  ___mkstr(MTOUCH_ISR_TMR_ASM)        );  /* Move to Bank 0 to access timer SFR               */  \
                asm("clrf   "   ___mkstr(NOBANK(MTOUCH_ISR_TMR_ASM)));  /* Clear timer                                      */  \
                asm("addwf  "   ___mkstr(NOBANK(MTOUCH_ISR_TMR_ASM)));  /* Add the masked LFSR value as an offset to timer  */  \
            } while (0)

            
        /**
        * @def      JITTER_MAIN_LOOP() 
        * @brief    Calculates a random value and uses it to jitter the mTouch scanning 
        *           routine when operating out of the main loop.
        *
        *   Implements a linear feedback shift register algorithm to increase the randomness 
        *   of the jitter function. This implementation costs one byte of RAM.
        *
        * @hideinitializer
        */
        #define JITTER_MAIN_LOOP()                                                                                          \
            do {                                                                                                            \
                asm("BANKSEL    _mTouch_jitter"                 );  /* Make sure we're starting in the correct bank     */  \
                asm("bcf    "   ___mkstr(STATUS)            ",0");  /* Clear the carry bit                              */  \
                asm("rrf    "   ___mkstr(_mTouch_jitter)    ",W");  /* Right shift the current jitter seed value        */  \
                asm("btfsc  "   ___mkstr(STATUS)            ",0");  /* Check the carry bit - if set, perform XOR        */  \
                asm("xorlw      0xB4"                           );  /* (cond) XOR the jitter seed with 0xB4             */  \
                asm("movwf  "   ___mkstr(_mTouch_jitter)        );  /* Store the result as the new jitter seed value    */  \
                for (uint8_t i = (mTouch_jitter & MTOUCH_JITTER_MASK); i > 0; i--); /* Delay loop                       */  \
            } while (0)
    
    #else
    
        #define JITTER_START_TIME()                                                                 \
            do {                                                                                    \
                if (mTouch_jitter & 0x01) {                                                         \
                    mTouch_jitter = (uint8_t)(mTouch_jitter >> 1) ^ 0xB4;                           \
                } else {                                                                            \
                    mTouch_jitter = (uint8_t)(mTouch_jitter >> 1);                                  \
                }                                                                                   \
                MTOUCH_ISR_TMR_C = mTouch_jitter & MTOUCH_EEPROM_read(MTOUCH_EEPROM_JITTER_ADR);    \
            } while(0)
            
        #define JITTER_MAIN_LOOP()                                          \
            do {                                                            \
                if (mTouch_jitter & 0x01) {                                 \
                    mTouch_jitter = (uint8_t)(mTouch_jitter >> 1) ^ 0xB4;   \
                } else {                                                    \
                    mTouch_jitter = (uint8_t)(mTouch_jitter >> 1);          \
                }                                                           \
                for (uint8_t i = mTouch_jitter & MTOUCH_EEPROM_read(MTOUCH_EEPROM_JITTER_ADR); i > 0; i--); \
            } while(0)
            
    #endif
        
#else                                                       // Do not implement the jittering function
    #define JITTER_START_TIME()     \
        do {                        \
            MTOUCH_ISR_TMR_C = 0;   \
        } while(0) 
    #define JITTER_MAIN_LOOP()
#endif


#define MTOUCH_SCANFUNCTIONA(index)             __paste(mTouch_ScanA_, index)
#define MTOUCH_SCANFUNCTIONB(index)             __paste(mTouch_ScanB_, index)

/**
* @def          CVD_SCANA_GEN(index, indexRef)
* @param[in]    index       the index of the sensor to be scanned
* @param[in]    indexRef    the index of the sensor to be used as the reference
* @brief        Function-generating macro called by MTOUCH_SCAN_FUNCTION(i). Do not use directly.
*
*   This macro creates a new CVD 'Scan A' function based on the index value provided.
*   The function's name will be mTouch_ScanA_#().
*
* @hideinitializer
*/
#define CVD_SCANA_GEN(index, indexRef)          void MTOUCH_SCANFUNCTIONA(index)(void)                                                  \
                                                {                                                                                       \
                                                    CVD_RS_MUTUAL_A(index);                 /*  Set up mutual sensor, if enabled    */  \
                                                    CVD_RS_INIT_A(indexRef);                /*  Set up charge reference             */  \
                                                    CVD_RS_MUXSELECT(index, indexRef);      /*  ADC Mux --> Reference               */  \
                                                    CVD_RS_PREPARE(index);                  /*  Initialize FSRs                     */  \
                                                    CVD_DELAY_CHOLD();                      /*  Delay while CHOLD charges           */  \
                                                    CVD_RS_CONNECT_A(index);                /*  Connect CHOLD and external sensor   */  \
                                                    CVD_DELAY_SETTLE();                     /*  Delay while voltage averages        */  \
                                                    CVD_RS_SET_GODONE();                    /*  Begin ADC conversion                */  \
                                                    CVD_GO_DONE_DELAY();                    /*  Delay while CHOLD disconnects       */  \
                                                    CVD_RS_EXIT_A(index);                   /*  Prepare sensor for Scan B           */  \
                                                }
/**
* @def          CVD_SCANB_GEN(index, indexRef)
* @param[in]    index       the index of the sensor to be scanned
* @param[in]    indexRef    the index of the sensor to be used as the reference
* @brief        Function-generating macro called by MTOUCH_SCAN_FUNCTION(i). Do not use directly.
*
*   This macro creates a new CVD 'Scan B' function based on the index value provided.
*   The function's name will be mTouch_ScanB_#().
*
* @hideinitializer
*/
#define CVD_SCANB_GEN(index, indexRef)          void MTOUCH_SCANFUNCTIONB(index)(void)                                                  \
                                                {                                                                                       \
                                                    CVD_RS_MUTUAL_B(index);                 /*  Set up mutual sensor, if enabled    */  \
                                                    CVD_RS_INIT_B(indexRef);                /*  Set up charge reference             */  \
                                                    CVD_RS_MUXSELECT(index, indexRef);      /*  ADC Mux --> Reference               */  \
                                                    CVD_RS_PREPARE(index);                  /*  Initialize FSRs                     */  \
                                                    CVD_DELAY_CHOLD();                      /*  Delay while CHOLD charges           */  \
                                                    CVD_RS_CONNECT_B(index);                /*  Connect CHOLD and external sensor   */  \
                                                    CVD_DELAY_SETTLE();                     /*  Delay while voltage averages        */  \
                                                    CVD_RS_SET_GODONE();                    /*  Begin ADC conversion                */  \
                                                    CVD_GO_DONE_DELAY();                    /*  Delay while CHOLD disconnects       */  \
                                                    CVD_RS_EXIT_B(index);                   /*  Prepare sensor for exitting ISR     */  \
                                                }

#define MTOUCH_SCAN_FUNCTION(index)             CVD_SCANA_GEN(index, __paste(CVD_REFSENSOR_, index));  \
                                                CVD_SCANB_GEN(index, __paste(CVD_REFSENSOR_, index))

#define MTOUCH_SCAN_PROTOTYPE(index)            void MTOUCH_SCANFUNCTIONA(index)(void);   \
                                                void MTOUCH_SCANFUNCTIONB(index)(void);

#if     MTOUCH_NUMBER_SENSORS == 1
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0) }
#define MTOUCH_SCANMODE_VAR_INIT        { 1, {&mTouch_sensorScans[0]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0)
#elif   MTOUCH_NUMBER_SENSORS == 2
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1) }
#define MTOUCH_SCANMODE_VAR_INIT        { 2, {&mTouch_sensorScans[0], &mTouch_sensorScans[1]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1)
#elif   MTOUCH_NUMBER_SENSORS == 3
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2) }
#define MTOUCH_SCANMODE_VAR_INIT        { 3, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2)
#elif   MTOUCH_NUMBER_SENSORS == 4
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3) }
#define MTOUCH_SCANMODE_VAR_INIT        { 4, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3)
#elif   MTOUCH_NUMBER_SENSORS == 5
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4) }
#define MTOUCH_SCANMODE_VAR_INIT        { 5, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4)
#elif   MTOUCH_NUMBER_SENSORS == 6
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5) }
#define MTOUCH_SCANMODE_VAR_INIT        { 6, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5)
#elif   MTOUCH_NUMBER_SENSORS == 7
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6) }
#define MTOUCH_SCANMODE_VAR_INIT        { 7, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6)
#elif   MTOUCH_NUMBER_SENSORS == 8
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7) }
#define MTOUCH_SCANMODE_VAR_INIT        { 8, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7)
#elif   MTOUCH_NUMBER_SENSORS == 9
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8) }
#define MTOUCH_SCANMODE_VAR_INIT        { 9, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8)
#elif   MTOUCH_NUMBER_SENSORS == 10
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9) }
#define MTOUCH_SCANMODE_VAR_INIT        { 10, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9)
#elif   MTOUCH_NUMBER_SENSORS == 11
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10) }
#define MTOUCH_SCANMODE_VAR_INIT        { 11, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10)
#elif   MTOUCH_NUMBER_SENSORS == 12
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11) }
#define MTOUCH_SCANMODE_VAR_INIT        { 12, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11)
#elif   MTOUCH_NUMBER_SENSORS == 13
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12) }
#define MTOUCH_SCANMODE_VAR_INIT        { 13, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12)
#elif   MTOUCH_NUMBER_SENSORS == 14
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13) }
#define MTOUCH_SCANMODE_VAR_INIT        { 14, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13)
#elif   MTOUCH_NUMBER_SENSORS == 15
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14) }
#define MTOUCH_SCANMODE_VAR_INIT        { 15, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14)
#elif   MTOUCH_NUMBER_SENSORS == 16
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15) }
#define MTOUCH_SCANMODE_VAR_INIT        { 16, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15)
#elif   MTOUCH_NUMBER_SENSORS == 17
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16) }
#define MTOUCH_SCANMODE_VAR_INIT        { 17, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16)
#elif   MTOUCH_NUMBER_SENSORS == 18
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17) }
#define MTOUCH_SCANMODE_VAR_INIT        { 18, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17)
#elif   MTOUCH_NUMBER_SENSORS == 19
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)}, {&MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONB(18)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONA(18) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17), &MTOUCH_SCANFUNCTIONB(18) }
#define MTOUCH_SCANMODE_VAR_INIT        { 19, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17], &mTouch_sensorScans[18]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17); MTOUCH_SCAN_PROTOTYPE(18)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17); MTOUCH_SCAN_FUNCTION(18)
#elif   MTOUCH_NUMBER_SENSORS == 20
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(v2)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)}, {&MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONB(18)}, {&MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONB(19)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONA(19) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17), &MTOUCH_SCANFUNCTIONB(18), &MTOUCH_SCANFUNCTIONB(19) }
#define MTOUCH_SCANMODE_VAR_INIT        { 20, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17], &mTouch_sensorScans[18], &mTouch_sensorScans[19]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17); MTOUCH_SCAN_PROTOTYPE(18); MTOUCH_SCAN_PROTOTYPE(19)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17); MTOUCH_SCAN_FUNCTION(18); MTOUCH_SCAN_FUNCTION(19)
#elif   MTOUCH_NUMBER_SENSORS == 21
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(v4), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)}, {&MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONB(18)}, {&MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONB(19)}, {&MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONB(20)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONA(20) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17), &MTOUCH_SCANFUNCTIONB(18), &MTOUCH_SCANFUNCTIONB(19), &MTOUCH_SCANFUNCTIONB(20) }
#define MTOUCH_SCANMODE_VAR_INIT        { 21, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17], &mTouch_sensorScans[18], &mTouch_sensorScans[19], &mTouch_sensorScans[20]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17); MTOUCH_SCAN_PROTOTYPE(18); MTOUCH_SCAN_PROTOTYPE(19); MTOUCH_SCAN_PROTOTYPE(20)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17); MTOUCH_SCAN_FUNCTION(18); MTOUCH_SCAN_FUNCTION(19); MTOUCH_SCAN_FUNCTION(20)
#elif   MTOUCH_NUMBER_SENSORS == 22
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)}, {&MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONB(18)}, {&MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONB(19)}, {&MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONB(20)}, {&MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONB(21)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONA(21) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17), &MTOUCH_SCANFUNCTIONB(18), &MTOUCH_SCANFUNCTIONB(19), &MTOUCH_SCANFUNCTIONB(20), &MTOUCH_SCANFUNCTIONB(21) }
#define MTOUCH_SCANMODE_VAR_INIT        { 22, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17], &mTouch_sensorScans[18], &mTouch_sensorScans[19], &mTouch_sensorScans[20], &mTouch_sensorScans[21]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17); MTOUCH_SCAN_PROTOTYPE(18); MTOUCH_SCAN_PROTOTYPE(19); MTOUCH_SCAN_PROTOTYPE(20); MTOUCH_SCAN_PROTOTYPE(21)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17); MTOUCH_SCAN_FUNCTION(18); MTOUCH_SCAN_FUNCTION(19); MTOUCH_SCAN_FUNCTION(20); MTOUCH_SCAN_FUNCTION(21)
#elif   MTOUCH_NUMBER_SENSORS == 23
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)}, {&MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONB(18)}, {&MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONB(19)}, {&MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONB(20)}, {&MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONB(21)}, {&MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONB(22)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONA(22) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17), &MTOUCH_SCANFUNCTIONB(18), &MTOUCH_SCANFUNCTIONB(19), &MTOUCH_SCANFUNCTIONB(20), &MTOUCH_SCANFUNCTIONB(21), &MTOUCH_SCANFUNCTIONB(22) }
#define MTOUCH_SCANMODE_VAR_INIT        { 23, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17], &mTouch_sensorScans[18], &mTouch_sensorScans[19], &mTouch_sensorScans[20], &mTouch_sensorScans[21], &mTouch_sensorScans[22]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17); MTOUCH_SCAN_PROTOTYPE(18); MTOUCH_SCAN_PROTOTYPE(19); MTOUCH_SCAN_PROTOTYPE(20); MTOUCH_SCAN_PROTOTYPE(21); MTOUCH_SCAN_PROTOTYPE(22)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17); MTOUCH_SCAN_FUNCTION(18); MTOUCH_SCAN_FUNCTION(19); MTOUCH_SCAN_FUNCTION(20); MTOUCH_SCAN_FUNCTION(21); MTOUCH_SCAN_FUNCTION(22)
#elif   MTOUCH_NUMBER_SENSORS == 24
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)}, {&MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONB(18)}, {&MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONB(19)}, {&MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONB(20)}, {&MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONB(21)}, {&MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONB(22)}, {&MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONB(23)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONA(23) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17), &MTOUCH_SCANFUNCTIONB(18), &MTOUCH_SCANFUNCTIONB(19), &MTOUCH_SCANFUNCTIONB(20), &MTOUCH_SCANFUNCTIONB(21), &MTOUCH_SCANFUNCTIONB(22), &MTOUCH_SCANFUNCTIONB(23) }
#define MTOUCH_SCANMODE_VAR_INIT        { 24, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17], &mTouch_sensorScans[18], &mTouch_sensorScans[19], &mTouch_sensorScans[20], &mTouch_sensorScans[21], &mTouch_sensorScans[22], &mTouch_sensorScans[23]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17); MTOUCH_SCAN_PROTOTYPE(18); MTOUCH_SCAN_PROTOTYPE(19); MTOUCH_SCAN_PROTOTYPE(20); MTOUCH_SCAN_PROTOTYPE(21); MTOUCH_SCAN_PROTOTYPE(22); MTOUCH_SCAN_PROTOTYPE(23)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17); MTOUCH_SCAN_FUNCTION(18); MTOUCH_SCAN_FUNCTION(19); MTOUCH_SCAN_FUNCTION(20); MTOUCH_SCAN_FUNCTION(21); MTOUCH_SCAN_FUNCTION(22); MTOUCH_SCAN_FUNCTION(23)
#elif   MTOUCH_NUMBER_SENSORS == 25
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)}, {&MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONB(18)}, {&MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONB(19)}, {&MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONB(20)}, {&MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONB(21)}, {&MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONB(22)}, {&MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONB(23)}, {&MTOUCH_SCANFUNCTIONA(24), &MTOUCH_SCANFUNCTIONB(24)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONA(24) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17), &MTOUCH_SCANFUNCTIONB(18), &MTOUCH_SCANFUNCTIONB(19), &MTOUCH_SCANFUNCTIONB(20), &MTOUCH_SCANFUNCTIONB(21), &MTOUCH_SCANFUNCTIONB(22), &MTOUCH_SCANFUNCTIONB(23), &MTOUCH_SCANFUNCTIONB(24) }
#define MTOUCH_SCANMODE_VAR_INIT        { 25, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17], &mTouch_sensorScans[18], &mTouch_sensorScans[19], &mTouch_sensorScans[20], &mTouch_sensorScans[21], &mTouch_sensorScans[22], &mTouch_sensorScans[23], &mTouch_sensorScans[24]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17); MTOUCH_SCAN_PROTOTYPE(18); MTOUCH_SCAN_PROTOTYPE(19); MTOUCH_SCAN_PROTOTYPE(20); MTOUCH_SCAN_PROTOTYPE(21); MTOUCH_SCAN_PROTOTYPE(22); MTOUCH_SCAN_PROTOTYPE(23); MTOUCH_SCAN_PROTOTYPE(24)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17); MTOUCH_SCAN_FUNCTION(18); MTOUCH_SCAN_FUNCTION(19); MTOUCH_SCAN_FUNCTION(20); MTOUCH_SCAN_FUNCTION(21); MTOUCH_SCAN_FUNCTION(22); MTOUCH_SCAN_FUNCTION(23); MTOUCH_SCAN_FUNCTION(24)
#elif   MTOUCH_NUMBER_SENSORS == 26
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(v2)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)}, {&MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONB(18)}, {&MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONB(19)}, {&MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONB(20)}, {&MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONB(21)}, {&MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONB(22)}, {&MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONB(23)}, {&MTOUCH_SCANFUNCTIONA(24), &MTOUCH_SCANFUNCTIONB(24)}, {&MTOUCH_SCANFUNCTIONA(25), &MTOUCH_SCANFUNCTIONB(25)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONA(24), &MTOUCH_SCANFUNCTIONA(25) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17), &MTOUCH_SCANFUNCTIONB(18), &MTOUCH_SCANFUNCTIONB(19), &MTOUCH_SCANFUNCTIONB(20), &MTOUCH_SCANFUNCTIONB(21), &MTOUCH_SCANFUNCTIONB(22), &MTOUCH_SCANFUNCTIONB(23), &MTOUCH_SCANFUNCTIONB(24), &MTOUCH_SCANFUNCTIONB(25) }
#define MTOUCH_SCANMODE_VAR_INIT        { 26, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17], &mTouch_sensorScans[18], &mTouch_sensorScans[19], &mTouch_sensorScans[20], &mTouch_sensorScans[21], &mTouch_sensorScans[22], &mTouch_sensorScans[23], &mTouch_sensorScans[24], &mTouch_sensorScans[25]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17); MTOUCH_SCAN_PROTOTYPE(18); MTOUCH_SCAN_PROTOTYPE(19); MTOUCH_SCAN_PROTOTYPE(20); MTOUCH_SCAN_PROTOTYPE(21); MTOUCH_SCAN_PROTOTYPE(22); MTOUCH_SCAN_PROTOTYPE(23); MTOUCH_SCAN_PROTOTYPE(24); MTOUCH_SCAN_PROTOTYPE(25)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17); MTOUCH_SCAN_FUNCTION(18); MTOUCH_SCAN_FUNCTION(19); MTOUCH_SCAN_FUNCTION(20); MTOUCH_SCAN_FUNCTION(21); MTOUCH_SCAN_FUNCTION(22); MTOUCH_SCAN_FUNCTION(23); MTOUCH_SCAN_FUNCTION(24); MTOUCH_SCAN_FUNCTION(25)
#elif   MTOUCH_NUMBER_SENSORS == 27
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)}, {&MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONB(18)}, {&MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONB(19)}, {&MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONB(20)}, {&MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONB(21)}, {&MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONB(22)}, {&MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONB(23)}, {&MTOUCH_SCANFUNCTIONA(24), &MTOUCH_SCANFUNCTIONB(24)}, {&MTOUCH_SCANFUNCTIONA(25), &MTOUCH_SCANFUNCTIONB(25)}, {&MTOUCH_SCANFUNCTIONA(26), &MTOUCH_SCANFUNCTIONB(26)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONA(24), &MTOUCH_SCANFUNCTIONA(25), &MTOUCH_SCANFUNCTIONA(26) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17), &MTOUCH_SCANFUNCTIONB(18), &MTOUCH_SCANFUNCTIONB(19), &MTOUCH_SCANFUNCTIONB(20), &MTOUCH_SCANFUNCTIONB(21), &MTOUCH_SCANFUNCTIONB(22), &MTOUCH_SCANFUNCTIONB(23), &MTOUCH_SCANFUNCTIONB(24), &MTOUCH_SCANFUNCTIONB(25), &MTOUCH_SCANFUNCTIONB(26) }
#define MTOUCH_SCANMODE_VAR_INIT        { 27, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17], &mTouch_sensorScans[18], &mTouch_sensorScans[19], &mTouch_sensorScans[20], &mTouch_sensorScans[21], &mTouch_sensorScans[22], &mTouch_sensorScans[23], &mTouch_sensorScans[24], &mTouch_sensorScans[25], &mTouch_sensorScans[26]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17); MTOUCH_SCAN_PROTOTYPE(18); MTOUCH_SCAN_PROTOTYPE(19); MTOUCH_SCAN_PROTOTYPE(20); MTOUCH_SCAN_PROTOTYPE(21); MTOUCH_SCAN_PROTOTYPE(22); MTOUCH_SCAN_PROTOTYPE(23); MTOUCH_SCAN_PROTOTYPE(24); MTOUCH_SCAN_PROTOTYPE(25); MTOUCH_SCAN_PROTOTYPE(26)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17); MTOUCH_SCAN_FUNCTION(18); MTOUCH_SCAN_FUNCTION(19); MTOUCH_SCAN_FUNCTION(20); MTOUCH_SCAN_FUNCTION(21); MTOUCH_SCAN_FUNCTION(22); MTOUCH_SCAN_FUNCTION(23); MTOUCH_SCAN_FUNCTION(24); MTOUCH_SCAN_FUNCTION(25); MTOUCH_SCAN_FUNCTION(26)
#elif   MTOUCH_NUMBER_SENSORS == 28
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)}, {&MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONB(18)}, {&MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONB(19)}, {&MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONB(20)}, {&MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONB(21)}, {&MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONB(22)}, {&MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONB(23)}, {&MTOUCH_SCANFUNCTIONA(24), &MTOUCH_SCANFUNCTIONB(24)}, {&MTOUCH_SCANFUNCTIONA(25), &MTOUCH_SCANFUNCTIONB(25)}, {&MTOUCH_SCANFUNCTIONA(26), &MTOUCH_SCANFUNCTIONB(26)}, {&MTOUCH_SCANFUNCTIONA(27), &MTOUCH_SCANFUNCTIONB(27)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONA(24), &MTOUCH_SCANFUNCTIONA(25), &MTOUCH_SCANFUNCTIONA(26), &MTOUCH_SCANFUNCTIONA(27) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17), &MTOUCH_SCANFUNCTIONB(18), &MTOUCH_SCANFUNCTIONB(19), &MTOUCH_SCANFUNCTIONB(20), &MTOUCH_SCANFUNCTIONB(21), &MTOUCH_SCANFUNCTIONB(22), &MTOUCH_SCANFUNCTIONB(23), &MTOUCH_SCANFUNCTIONB(24), &MTOUCH_SCANFUNCTIONB(25), &MTOUCH_SCANFUNCTIONB(26), &MTOUCH_SCANFUNCTIONB(27) }
#define MTOUCH_SCANMODE_VAR_INIT        { 28, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17], &mTouch_sensorScans[18], &mTouch_sensorScans[19], &mTouch_sensorScans[20], &mTouch_sensorScans[21], &mTouch_sensorScans[22], &mTouch_sensorScans[23], &mTouch_sensorScans[24], &mTouch_sensorScans[25], &mTouch_sensorScans[26], &mTouch_sensorScans[27]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17); MTOUCH_SCAN_PROTOTYPE(18); MTOUCH_SCAN_PROTOTYPE(19); MTOUCH_SCAN_PROTOTYPE(20); MTOUCH_SCAN_PROTOTYPE(21); MTOUCH_SCAN_PROTOTYPE(22); MTOUCH_SCAN_PROTOTYPE(23); MTOUCH_SCAN_PROTOTYPE(24); MTOUCH_SCAN_PROTOTYPE(25); MTOUCH_SCAN_PROTOTYPE(26); MTOUCH_SCAN_PROTOTYPE(27)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17); MTOUCH_SCAN_FUNCTION(18); MTOUCH_SCAN_FUNCTION(19); MTOUCH_SCAN_FUNCTION(20); MTOUCH_SCAN_FUNCTION(21); MTOUCH_SCAN_FUNCTION(22); MTOUCH_SCAN_FUNCTION(23); MTOUCH_SCAN_FUNCTION(24); MTOUCH_SCAN_FUNCTION(25); MTOUCH_SCAN_FUNCTION(26); MTOUCH_SCAN_FUNCTION(27)
#elif   MTOUCH_NUMBER_SENSORS == 29
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)}, {&MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONB(18)}, {&MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONB(19)}, {&MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONB(20)}, {&MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONB(21)}, {&MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONB(22)}, {&MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONB(23)}, {&MTOUCH_SCANFUNCTIONA(24), &MTOUCH_SCANFUNCTIONB(24)}, {&MTOUCH_SCANFUNCTIONA(25), &MTOUCH_SCANFUNCTIONB(25)}, {&MTOUCH_SCANFUNCTIONA(26), &MTOUCH_SCANFUNCTIONB(26)}, {&MTOUCH_SCANFUNCTIONA(27), &MTOUCH_SCANFUNCTIONB(27)}, {&MTOUCH_SCANFUNCTIONA(28), &MTOUCH_SCANFUNCTIONB(28)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONA(24), &MTOUCH_SCANFUNCTIONA(25), &MTOUCH_SCANFUNCTIONA(26), &MTOUCH_SCANFUNCTIONA(27), &MTOUCH_SCANFUNCTIONA(28) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17), &MTOUCH_SCANFUNCTIONB(18), &MTOUCH_SCANFUNCTIONB(19), &MTOUCH_SCANFUNCTIONB(20), &MTOUCH_SCANFUNCTIONB(21), &MTOUCH_SCANFUNCTIONB(22), &MTOUCH_SCANFUNCTIONB(23), &MTOUCH_SCANFUNCTIONB(24), &MTOUCH_SCANFUNCTIONB(25), &MTOUCH_SCANFUNCTIONB(26), &MTOUCH_SCANFUNCTIONB(27), &MTOUCH_SCANFUNCTIONB(28) }
#define MTOUCH_SCANMODE_VAR_INIT        { 29, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17], &mTouch_sensorScans[18], &mTouch_sensorScans[19], &mTouch_sensorScans[20], &mTouch_sensorScans[21], &mTouch_sensorScans[22], &mTouch_sensorScans[23], &mTouch_sensorScans[24], &mTouch_sensorScans[25], &mTouch_sensorScans[26], &mTouch_sensorScans[27], &mTouch_sensorScans[28]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17); MTOUCH_SCAN_PROTOTYPE(18); MTOUCH_SCAN_PROTOTYPE(19); MTOUCH_SCAN_PROTOTYPE(20); MTOUCH_SCAN_PROTOTYPE(21); MTOUCH_SCAN_PROTOTYPE(22); MTOUCH_SCAN_PROTOTYPE(23); MTOUCH_SCAN_PROTOTYPE(24); MTOUCH_SCAN_PROTOTYPE(25); MTOUCH_SCAN_PROTOTYPE(26); MTOUCH_SCAN_PROTOTYPE(27); MTOUCH_SCAN_PROTOTYPE(28)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17); MTOUCH_SCAN_FUNCTION(18); MTOUCH_SCAN_FUNCTION(19); MTOUCH_SCAN_FUNCTION(20); MTOUCH_SCAN_FUNCTION(21); MTOUCH_SCAN_FUNCTION(22); MTOUCH_SCAN_FUNCTION(23); MTOUCH_SCAN_FUNCTION(24); MTOUCH_SCAN_FUNCTION(25); MTOUCH_SCAN_FUNCTION(26); MTOUCH_SCAN_FUNCTION(27); MTOUCH_SCAN_FUNCTION(28)
#elif   MTOUCH_NUMBER_SENSORS == 30
#define MTOUCH_SCANSEQUENCE_VAR_INIT    { {&MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONB(0)}, {&MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONB(1)}, {&MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONB(2)}, {&MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONB(3)}, {&MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONB(4)}, {&MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONB(5)}, {&MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONB(6)}, {&MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONB(7)}, {&MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONB(8)}, {&MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONB(9)}, {&MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONB(10)}, {&MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONB(11)}, {&MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONB(12)}, {&MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONB(13)}, {&MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONB(14)}, {&MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONB(15)}, {&MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONB(16)}, {&MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONB(17)}, {&MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONB(18)}, {&MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONB(19)}, {&MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONB(20)}, {&MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONB(21)}, {&MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONB(22)}, {&MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONB(23)}, {&MTOUCH_SCANFUNCTIONA(24), &MTOUCH_SCANFUNCTIONB(24)}, {&MTOUCH_SCANFUNCTIONA(25), &MTOUCH_SCANFUNCTIONB(25)}, {&MTOUCH_SCANFUNCTIONA(26), &MTOUCH_SCANFUNCTIONB(26)}, {&MTOUCH_SCANFUNCTIONA(27), &MTOUCH_SCANFUNCTIONB(27)}, {&MTOUCH_SCANFUNCTIONA(28), &MTOUCH_SCANFUNCTIONB(28)}, {&MTOUCH_SCANFUNCTIONA(29), &MTOUCH_SCANFUNCTIONB(29)} }
#define MTOUCH_SCANA_VAR_INIT           { &MTOUCH_SCANFUNCTIONA(0), &MTOUCH_SCANFUNCTIONA(1), &MTOUCH_SCANFUNCTIONA(2), &MTOUCH_SCANFUNCTIONA(3), &MTOUCH_SCANFUNCTIONA(4), &MTOUCH_SCANFUNCTIONA(5), &MTOUCH_SCANFUNCTIONA(6), &MTOUCH_SCANFUNCTIONA(7), &MTOUCH_SCANFUNCTIONA(8), &MTOUCH_SCANFUNCTIONA(9), &MTOUCH_SCANFUNCTIONA(10), &MTOUCH_SCANFUNCTIONA(11), &MTOUCH_SCANFUNCTIONA(12), &MTOUCH_SCANFUNCTIONA(13), &MTOUCH_SCANFUNCTIONA(14), &MTOUCH_SCANFUNCTIONA(15), &MTOUCH_SCANFUNCTIONA(16), &MTOUCH_SCANFUNCTIONA(17), &MTOUCH_SCANFUNCTIONA(18), &MTOUCH_SCANFUNCTIONA(19), &MTOUCH_SCANFUNCTIONA(20), &MTOUCH_SCANFUNCTIONA(21), &MTOUCH_SCANFUNCTIONA(22), &MTOUCH_SCANFUNCTIONA(23), &MTOUCH_SCANFUNCTIONA(24), &MTOUCH_SCANFUNCTIONA(25), &MTOUCH_SCANFUNCTIONA(26), &MTOUCH_SCANFUNCTIONA(27), &MTOUCH_SCANFUNCTIONA(28), &MTOUCH_SCANFUNCTIONA(29) }
#define MTOUCH_SCANB_VAR_INIT           { &MTOUCH_SCANFUNCTIONB(0), &MTOUCH_SCANFUNCTIONB(1), &MTOUCH_SCANFUNCTIONB(2), &MTOUCH_SCANFUNCTIONB(3), &MTOUCH_SCANFUNCTIONB(4), &MTOUCH_SCANFUNCTIONB(5), &MTOUCH_SCANFUNCTIONB(6), &MTOUCH_SCANFUNCTIONB(7), &MTOUCH_SCANFUNCTIONB(8), &MTOUCH_SCANFUNCTIONB(9), &MTOUCH_SCANFUNCTIONB(10), &MTOUCH_SCANFUNCTIONB(11), &MTOUCH_SCANFUNCTIONB(12), &MTOUCH_SCANFUNCTIONB(13), &MTOUCH_SCANFUNCTIONB(14), &MTOUCH_SCANFUNCTIONB(15), &MTOUCH_SCANFUNCTIONB(16), &MTOUCH_SCANFUNCTIONB(17), &MTOUCH_SCANFUNCTIONB(18), &MTOUCH_SCANFUNCTIONB(19), &MTOUCH_SCANFUNCTIONB(20), &MTOUCH_SCANFUNCTIONB(21), &MTOUCH_SCANFUNCTIONB(22), &MTOUCH_SCANFUNCTIONB(23), &MTOUCH_SCANFUNCTIONB(24), &MTOUCH_SCANFUNCTIONB(25), &MTOUCH_SCANFUNCTIONB(26), &MTOUCH_SCANFUNCTIONB(27), &MTOUCH_SCANFUNCTIONB(28), &MTOUCH_SCANFUNCTIONB(29) }
#define MTOUCH_SCANMODE_VAR_INIT        { 30, {&mTouch_sensorScans[0], &mTouch_sensorScans[1], &mTouch_sensorScans[2], &mTouch_sensorScans[3], &mTouch_sensorScans[4], &mTouch_sensorScans[5], &mTouch_sensorScans[6], &mTouch_sensorScans[7], &mTouch_sensorScans[8], &mTouch_sensorScans[9], &mTouch_sensorScans[10], &mTouch_sensorScans[11], &mTouch_sensorScans[12], &mTouch_sensorScans[13], &mTouch_sensorScans[14], &mTouch_sensorScans[15], &mTouch_sensorScans[16], &mTouch_sensorScans[17], &mTouch_sensorScans[18], &mTouch_sensorScans[19], &mTouch_sensorScans[20], &mTouch_sensorScans[21], &mTouch_sensorScans[22], &mTouch_sensorScans[23], &mTouch_sensorScans[24], &mTouch_sensorScans[25], &mTouch_sensorScans[26], &mTouch_sensorScans[27], &mTouch_sensorScans[28], &mTouch_sensorScans[29]} }
#define MTOUCH_SCAN_PROTOTYPES()        MTOUCH_SCAN_PROTOTYPE(0); MTOUCH_SCAN_PROTOTYPE(1); MTOUCH_SCAN_PROTOTYPE(2); MTOUCH_SCAN_PROTOTYPE(3); MTOUCH_SCAN_PROTOTYPE(4); MTOUCH_SCAN_PROTOTYPE(5); MTOUCH_SCAN_PROTOTYPE(6); MTOUCH_SCAN_PROTOTYPE(7); MTOUCH_SCAN_PROTOTYPE(8); MTOUCH_SCAN_PROTOTYPE(9); MTOUCH_SCAN_PROTOTYPE(10); MTOUCH_SCAN_PROTOTYPE(11); MTOUCH_SCAN_PROTOTYPE(12); MTOUCH_SCAN_PROTOTYPE(13); MTOUCH_SCAN_PROTOTYPE(14); MTOUCH_SCAN_PROTOTYPE(15); MTOUCH_SCAN_PROTOTYPE(16); MTOUCH_SCAN_PROTOTYPE(17); MTOUCH_SCAN_PROTOTYPE(18); MTOUCH_SCAN_PROTOTYPE(19); MTOUCH_SCAN_PROTOTYPE(20); MTOUCH_SCAN_PROTOTYPE(21); MTOUCH_SCAN_PROTOTYPE(22); MTOUCH_SCAN_PROTOTYPE(23); MTOUCH_SCAN_PROTOTYPE(24); MTOUCH_SCAN_PROTOTYPE(25); MTOUCH_SCAN_PROTOTYPE(26); MTOUCH_SCAN_PROTOTYPE(27); MTOUCH_SCAN_PROTOTYPE(28); MTOUCH_SCAN_PROTOTYPE(29)
#define MTOUCH_SCAN_FUNCTIONS()         MTOUCH_SCAN_FUNCTION(0); MTOUCH_SCAN_FUNCTION(1); MTOUCH_SCAN_FUNCTION(2); MTOUCH_SCAN_FUNCTION(3); MTOUCH_SCAN_FUNCTION(4); MTOUCH_SCAN_FUNCTION(5); MTOUCH_SCAN_FUNCTION(6); MTOUCH_SCAN_FUNCTION(7); MTOUCH_SCAN_FUNCTION(8); MTOUCH_SCAN_FUNCTION(9); MTOUCH_SCAN_FUNCTION(10); MTOUCH_SCAN_FUNCTION(11); MTOUCH_SCAN_FUNCTION(12); MTOUCH_SCAN_FUNCTION(13); MTOUCH_SCAN_FUNCTION(14); MTOUCH_SCAN_FUNCTION(15); MTOUCH_SCAN_FUNCTION(16); MTOUCH_SCAN_FUNCTION(17); MTOUCH_SCAN_FUNCTION(18); MTOUCH_SCAN_FUNCTION(19); MTOUCH_SCAN_FUNCTION(20); MTOUCH_SCAN_FUNCTION(21); MTOUCH_SCAN_FUNCTION(22); MTOUCH_SCAN_FUNCTION(23); MTOUCH_SCAN_FUNCTION(24); MTOUCH_SCAN_FUNCTION(25); MTOUCH_SCAN_FUNCTION(26); MTOUCH_SCAN_FUNCTION(27); MTOUCH_SCAN_FUNCTION(28); MTOUCH_SCAN_FUNCTION(29)
#endif



#if !defined(PIC_ADC_BITS)
    #warning The hardware profile for this device has not defined the ADC resolution in bits. Defaulting to 10.
    #define PIC_ADC_BITS 10
#endif
//@}

/** @name   Timing Critical CVD Scanning Routine Macros
*
*   These macros implement the CVD scanning routine and basic acquisition-level filtering 
*   techniques. Editting these macros could reduce the noise immunity of the system.
*/
//@{


    
//@}


/** @name       Polishing/Finishing Macros
*
*   These macros perform scaling and final operations before setting the 
*   #mTouchCVD_dataReady flag and allowing the main loop application to process
*   the new data.
*/
//@{

/**
* @def          STORE_SENSOR_SCALE(index, shift)
* @brief        Do not use directly. Expanded utility macro called by STORE_SENSOR(index). 
* @param[in]    index   the index of the sensor to be scaled and stored
* @param[in]    shift   the number of times to shift the accumulator value to the right before storing its value in the mTouch_sensorData array
* @hideinitializer
*/
#define STORE_SENSOR_SCALE(index, shift)                                                                \
    do {                                                                                                \
        mTouch_prevSensor           = &mTouch_acqData[index];                                           \
        mTouch_sensorData[index-1]  = (uint16_t)(mTouch_GetAccumulator(mTouch_prevSensor) >> shift);    \
        mTouch_ClearAccumulator(mTouch_prevSensor);                                                     \
    } while(0)
    
/**
* @def          STORE_LAST_SENSOR_SCALE(index, shift)
* @brief        Do not use directly. Expanded utility macro called by STORE_SENSOR(index). 
* @param[in]    index   the index of the sensor to be scaled and stored
* @param[in]    shift   the number of times to shift the accumulator value to the right before storing its value in the mTouch_sensorData array
* @hideinitializer
*/
#define STORE_LAST_SENSOR_SCALE(index, shift)                                                                       \
    do {                                                                                                            \
        mTouch_prevSensor    = &mTouch_acqData[index];                                                              \
        mTouch_sensorData[MTOUCH_NUMBER_SENSORS-1] = (uint16_t)(mTouch_GetAccumulator(mTouch_prevSensor) >> shift); \
        mTouch_ClearAccumulator(mTouch_prevSensor);                                                                 \
    } while(0)
    
/**
* @def          STORE_SINGLE_SENSOR_SCALE(index, shift)
* @brief        Do not use directly. Expanded utility macro called by STORE_SENSOR(index) if there is only one sensor in the mTouch application. 
* @param[in]    index   the index of the sensor to be scaled and stored
* @param[in]    shift   the number of times to shift the accumulator value to the right before storing its value in the mTouch_sensorData array
* @hideinitializer
*/
#define STORE_SINGLE_SENSOR_SCALE(index, shift)                                                 \
    do {                                                                                        \
        mTouch_sensorData[0] = (uint16_t)(mTouch_GetAccumulator(mTouch_prevSensor) >> shift);   \
        mTouch_ClearAccumulator(mTouch_prevSensor);                                             \
    } while(0)

/**
* @def          STORE_SENSOR(index)
* @ingroup      Acquisition
* @param[in]    index   the index of the sensor to be scaled and stored
* @brief        Scales the accumulated result of the CVD acquisition and stores it for main-application access
*
*   This macro uses the MTOUCH_SCALING configuration option to determine which scaling option to choose.
* 
* @hideinitializer
*/
#if (MTOUCH_NUMBER_SENSORS > 1)
    #if (MTOUCH_SCALING >= 0) && (MTOUCH_SCALING <= 8)
        #define STORE_SENSOR(index)             STORE_SENSOR_SCALE(index, MTOUCH_SCALING_VALUE) 
        #define STORE_LAST_SENSOR(index)        STORE_LAST_SENSOR_SCALE(index, MTOUCH_SCALING_VALUE) 
    #else
        #error MTOUCH_SCALING must be set to a value between 0 and 8.
    #endif
#else
    #if (MTOUCH_SCALING >= 0) && (MTOUCH_SCALING <= 8)
        #define STORE_SENSOR(index)             STORE_SINGLE_SENSOR_SCALE(index, MTOUCH_SCALING_VALUE) 
    #else
        #error MTOUCH_SCALING must be set to a value between 0 and 8.
    #endif
#endif

/**
* @def          EXIT_SENSOR(index)
* @ingroup      Acquisition
* @param[in]    index   the index variable to increment
* @brief        Increments the given index variable and exits the ISR
* @hideinitializer
*/
#if (MTOUCH_ADC_CONTROL == MTOUCH_ALWAYS_CONTROLS_ADC)
    #define EXIT_SENSOR(index)                                  \
        do {                                                    \
            index++;                                            \
            asm("ljmp END_MTOUCH_SCAN");    /* Exit the ISR */  \
        } while(0)
#else
    #define EXIT_SENSOR(index)                                  \
        do {                                                    \
            index++;                                            \
            WAIT_FOR_GODONE_BIT();                              \
            MTOUCH_STORE_SCAN_B();                              \
            asm("ljmp END_MTOUCH_SCAN");    /* Exit the ISR */  \
        } while(0)
#endif

/**
* @def          EXIT_LAST_SENSOR(index)
* @ingroup      Acquisition
* @param[in]    index   the index variable to reset to 0
* @brief        Performs several important functions after all sensors have been scanned
*
*   Resets the index variable to 0 for the next scan, decrements the oversampling
*   counter to determine if it is time to complete the sample and store the result,
*   and then exits the ISR.
*
* @hideinitializer
*/
/**
* @def          EXIT_LAST_SENSOR_DECINDEX(index) 
* @param[in]    index   the index variable to reset to 0
* @brief        Do not use directly. Utility macro used by EXIT_LAST_SENSOR() to reset the index variable.
* @hideinitializer
*/
/**
* @def          EXIT_LAST_SENSOR_MAIN()
* @brief        Do not use directly. Utility macro used by EXIT_LAST_SENSOR() to decrement the oversampling counter. 
* @hideinitializer
*/
#if (MTOUCH_ADC_CONTROL == MTOUCH_ALWAYS_CONTROLS_ADC)
    #if (MTOUCH_NUMBER_SENSORS > 1)
        #define EXIT_LAST_SENSOR(index)     EXIT_LAST_SENSOR_DECINDEX(index);   \
                                            EXIT_LAST_SENSOR_MAIN()
    #else
        #define EXIT_LAST_SENSOR(index)     EXIT_LAST_SENSOR_MAIN()             // Only one sensor, so only updates the sample counter
    #endif
#else
    #if (MTOUCH_NUMBER_SENSORS > 1)
        #define EXIT_LAST_SENSOR(index)     EXIT_LAST_SENSOR_DECINDEX(index);   \
                                            WAIT_FOR_GODONE_BIT();              \
                                            MTOUCH_STORE_SCAN_B();              \
                                            EXIT_LAST_SENSOR_MAIN()
    #else
        #define EXIT_LAST_SENSOR(index)     WAIT_FOR_GODONE_BIT();      \
                                            MTOUCH_STORE_SCAN_B();      \
                                            EXIT_LAST_SENSOR_MAIN()                                     
    #endif
#endif

#define EXIT_LAST_SENSOR_MAIN()                                                                                                                 \
do {                                                                                                                                            \
    if (--mTouch_stateVars.active.sampleCounter != 0) {                 /* Decrement the sample counter and check if it's equal to 0        */  \
        asm("ljmp END_MTOUCH_SCAN");                                    /* If not equal to 0, exit the ISR                                  */  \
    }                                                                                                                                           \
    mTouch_stateVars.active.sampleCounter = MTOUCH_SpS_VALUE;           /* If equal to 0, reset the sample counter continue with storage    */  \
} while(0)
    
#define EXIT_LAST_SENSOR_DECINDEX(index)        index = 0;

#define MTOUCH_EXIT_SCAN_FUNCTION(index)        __3paste(MTOUCH_EXIT_SENSOR,index,())

#define MTOUCH_INC_INDEX_AND_STORE_RESULT()                                     \
                mTouch_currentSensor++;                                         \
                mTouch_prevSensor    = &mTouch_acqData[mTouch_currentSensor];   \
                WAIT_FOR_GODONE_BIT();                                          \
                MTOUCH_STORE_SCAN_B();


/**
* @def          SET_DATA_READY_FLAG()
* @ingroup      Acquisition
* @brief        Sets the dataReady flag to signal the main application of a new reading
* @hideinitializer
*/
#define SET_DATA_READY_FLAG()               mTouch_state.dataReady = 1
//@}

#if (MTOUCH_NUMBER_SENSORS > 1)

    
    #define MTOUCH_CHECK_SHORT_0        MTOUCH_CHECKforSHORTonSENSOR(0)
    #define MTOUCH_CHECK_SHORT_1        MTOUCH_CHECKforSHORTonSENSOR(1)
    #define MTOUCH_CHECK_SHORT_2        MTOUCH_CHECKforSHORTonSENSOR(2)
    #define MTOUCH_CHECK_SHORT_3        MTOUCH_CHECKforSHORTonSENSOR(3)
    #define MTOUCH_CHECK_SHORT_4        MTOUCH_CHECKforSHORTonSENSOR(4)
    #define MTOUCH_CHECK_SHORT_5        MTOUCH_CHECKforSHORTonSENSOR(5)
    #define MTOUCH_CHECK_SHORT_6        MTOUCH_CHECKforSHORTonSENSOR(6)
    #define MTOUCH_CHECK_SHORT_7        MTOUCH_CHECKforSHORTonSENSOR(7)
    #define MTOUCH_CHECK_SHORT_8        MTOUCH_CHECKforSHORTonSENSOR(8)
    #define MTOUCH_CHECK_SHORT_9        MTOUCH_CHECKforSHORTonSENSOR(9)
    #define MTOUCH_CHECK_SHORT_10       MTOUCH_CHECKforSHORTonSENSOR(10)
    #define MTOUCH_CHECK_SHORT_11       MTOUCH_CHECKforSHORTonSENSOR(11)
    #define MTOUCH_CHECK_SHORT_12       MTOUCH_CHECKforSHORTonSENSOR(12)
    #define MTOUCH_CHECK_SHORT_13       MTOUCH_CHECKforSHORTonSENSOR(13)
    #define MTOUCH_CHECK_SHORT_14       MTOUCH_CHECKforSHORTonSENSOR(14)
    #define MTOUCH_CHECK_SHORT_15       MTOUCH_CHECKforSHORTonSENSOR(15)
    #define MTOUCH_CHECK_SHORT_16       MTOUCH_CHECKforSHORTonSENSOR(16)
    #define MTOUCH_CHECK_SHORT_17       MTOUCH_CHECKforSHORTonSENSOR(17)
    #define MTOUCH_CHECK_SHORT_18       MTOUCH_CHECKforSHORTonSENSOR(18)
    #define MTOUCH_CHECK_SHORT_19       MTOUCH_CHECKforSHORTonSENSOR(19)
    #define MTOUCH_CHECK_SHORT_20       MTOUCH_CHECKforSHORTonSENSOR(20)
    #define MTOUCH_CHECK_SHORT_21       MTOUCH_CHECKforSHORTonSENSOR(21)
    #define MTOUCH_CHECK_SHORT_22       MTOUCH_CHECKforSHORTonSENSOR(22)
    #define MTOUCH_CHECK_SHORT_23       MTOUCH_CHECKforSHORTonSENSOR(23)
    #define MTOUCH_CHECK_SHORT_24       MTOUCH_CHECKforSHORTonSENSOR(24)
    #define MTOUCH_CHECK_SHORT_25       MTOUCH_CHECKforSHORTonSENSOR(25)
    #define MTOUCH_CHECK_SHORT_26       MTOUCH_CHECKforSHORTonSENSOR(26)
    #define MTOUCH_CHECK_SHORT_27       MTOUCH_CHECKforSHORTonSENSOR(27)
    #define MTOUCH_CHECK_SHORT_28       MTOUCH_CHECKforSHORTonSENSOR(28)
    #define MTOUCH_CHECK_SHORT_29       MTOUCH_CHECKforSHORTonSENSOR(29)

    #if (MTOUCH_NUMBER_SENSORS > 30)
        #error This version of the framework macro library unable to support more than 30 sensors at this time.
    #endif
    
    #if MTOUCH_NUMBER_SENSORS < 30
        #undef  MTOUCH_CHECK_SHORT_29
        #define MTOUCH_CHECK_SHORT_29
    #endif
    #if MTOUCH_NUMBER_SENSORS < 29
        #undef  MTOUCH_CHECK_SHORT_28
        #define MTOUCH_CHECK_SHORT_28
    #endif
    #if MTOUCH_NUMBER_SENSORS < 28  
        #undef  MTOUCH_CHECK_SHORT_27
        #define MTOUCH_CHECK_SHORT_27
    #endif
    #if MTOUCH_NUMBER_SENSORS < 27
        #undef  MTOUCH_CHECK_SHORT_26
        #define MTOUCH_CHECK_SHORT_26
    #endif
    #if MTOUCH_NUMBER_SENSORS < 26
        #undef  MTOUCH_CHECK_SHORT_25
        #define MTOUCH_CHECK_SHORT_25
    #endif
    #if MTOUCH_NUMBER_SENSORS < 25 
        #undef  MTOUCH_CHECK_SHORT_24
        #define MTOUCH_CHECK_SHORT_24
    #endif
    #if MTOUCH_NUMBER_SENSORS < 24 
        #undef  MTOUCH_CHECK_SHORT_23
        #define MTOUCH_CHECK_SHORT_23
    #endif
    #if MTOUCH_NUMBER_SENSORS < 23  
        #undef  MTOUCH_CHECK_SHORT_22
        #define MTOUCH_CHECK_SHORT_22
    #endif
    #if MTOUCH_NUMBER_SENSORS < 22
        #undef  MTOUCH_CHECK_SHORT_21
        #define MTOUCH_CHECK_SHORT_21
    #endif
    #if MTOUCH_NUMBER_SENSORS < 21
        #undef  MTOUCH_CHECK_SHORT_20
        #define MTOUCH_CHECK_SHORT_20
    #endif
    #if MTOUCH_NUMBER_SENSORS < 20  
        #undef  MTOUCH_CHECK_SHORT_19
        #define MTOUCH_CHECK_SHORT_19 
    #endif
    #if MTOUCH_NUMBER_SENSORS < 19
        #undef  MTOUCH_CHECK_SHORT_18
        #define MTOUCH_CHECK_SHORT_18
    #endif
    #if MTOUCH_NUMBER_SENSORS < 18  
        #undef  MTOUCH_CHECK_SHORT_17
        #define MTOUCH_CHECK_SHORT_17
    #endif
    #if MTOUCH_NUMBER_SENSORS < 17
        #undef  MTOUCH_CHECK_SHORT_16
        #define MTOUCH_CHECK_SHORT_16
    #endif
    #if MTOUCH_NUMBER_SENSORS < 16
        #undef  MTOUCH_CHECK_SHORT_15
        #define MTOUCH_CHECK_SHORT_15
    #endif
    #if MTOUCH_NUMBER_SENSORS < 15  
        #undef  MTOUCH_CHECK_SHORT_14
        #define MTOUCH_CHECK_SHORT_14
    #endif
    #if MTOUCH_NUMBER_SENSORS < 14
        #undef  MTOUCH_CHECK_SHORT_13
        #define MTOUCH_CHECK_SHORT_13
    #endif
    #if MTOUCH_NUMBER_SENSORS < 13
        #undef  MTOUCH_CHECK_SHORT_12
        #define MTOUCH_CHECK_SHORT_12
    #endif
    #if MTOUCH_NUMBER_SENSORS < 12
        #undef  MTOUCH_CHECK_SHORT_11
        #define MTOUCH_CHECK_SHORT_11
    #endif
    #if MTOUCH_NUMBER_SENSORS < 11
        #undef  MTOUCH_CHECK_SHORT_10
        #define MTOUCH_CHECK_SHORT_10
    #endif
    #if MTOUCH_NUMBER_SENSORS < 10 
        #undef  MTOUCH_CHECK_SHORT_9
        #define MTOUCH_CHECK_SHORT_9
    #endif
    #if MTOUCH_NUMBER_SENSORS < 9
        #undef  MTOUCH_CHECK_SHORT_8
        #define MTOUCH_CHECK_SHORT_8
    #endif
    #if MTOUCH_NUMBER_SENSORS < 8
        #undef  MTOUCH_CHECK_SHORT_7
        #define MTOUCH_CHECK_SHORT_7
    #endif
    #if MTOUCH_NUMBER_SENSORS < 7
        #undef  MTOUCH_CHECK_SHORT_6
        #define MTOUCH_CHECK_SHORT_6
    #endif
    #if MTOUCH_NUMBER_SENSORS < 6
        #undef  MTOUCH_CHECK_SHORT_5
        #define MTOUCH_CHECK_SHORT_5
    #endif
    #if MTOUCH_NUMBER_SENSORS < 5
        #undef  MTOUCH_CHECK_SHORT_4
        #define MTOUCH_CHECK_SHORT_4
    #endif
    #if MTOUCH_NUMBER_SENSORS < 4  
        #undef  MTOUCH_CHECK_SHORT_3
        #define MTOUCH_CHECK_SHORT_3
    #endif
    #if MTOUCH_NUMBER_SENSORS < 3
        #undef  MTOUCH_CHECK_SHORT_2
        #define MTOUCH_CHECK_SHORT_2
    #endif
    #if MTOUCH_NUMBER_SENSORS < 2
        #undef  MTOUCH_CHECK_SHORT_1
        #define MTOUCH_CHECK_SHORT_1
    #endif
    
        
    #define MTOUCH_GENERATE_SHORTCHECKS()       \
        do {                                    \
            MTOUCH_CHECK_SHORT_0;               \
            MTOUCH_CHECK_SHORT_1;               \
            MTOUCH_CHECK_SHORT_2;               \
            MTOUCH_CHECK_SHORT_3;               \
            MTOUCH_CHECK_SHORT_4;               \
            MTOUCH_CHECK_SHORT_5;               \
            MTOUCH_CHECK_SHORT_6;               \
            MTOUCH_CHECK_SHORT_7;               \
            MTOUCH_CHECK_SHORT_8;               \
            MTOUCH_CHECK_SHORT_9;               \
            MTOUCH_CHECK_SHORT_10;              \
            MTOUCH_CHECK_SHORT_11;              \
            MTOUCH_CHECK_SHORT_12;              \
            MTOUCH_CHECK_SHORT_13;              \
            MTOUCH_CHECK_SHORT_14;              \
            MTOUCH_CHECK_SHORT_15;              \
            MTOUCH_CHECK_SHORT_16;              \
            MTOUCH_CHECK_SHORT_17;              \
            MTOUCH_CHECK_SHORT_18;              \
            MTOUCH_CHECK_SHORT_19;              \
            MTOUCH_CHECK_SHORT_20;              \
            MTOUCH_CHECK_SHORT_21;              \
            MTOUCH_CHECK_SHORT_22;              \
            MTOUCH_CHECK_SHORT_23;              \
            MTOUCH_CHECK_SHORT_24;              \
            MTOUCH_CHECK_SHORT_25;              \
            MTOUCH_CHECK_SHORT_26;              \
            MTOUCH_CHECK_SHORT_27;              \
            MTOUCH_CHECK_SHORT_28;              \
            MTOUCH_CHECK_SHORT_29;              \
        } while (0)
        
#endif

