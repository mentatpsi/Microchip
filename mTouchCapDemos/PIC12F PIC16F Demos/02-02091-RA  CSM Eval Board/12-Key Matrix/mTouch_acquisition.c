/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_acquisition.c
 *  Dependencies:    mTouch.h
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework Acquisition Module
 *                   - Implements the mTouch acquisition process. 
 *                     Additional ISR functions should be carefully 
 *                     designed to not interfere with the mTouch interrupt 
 *                     service.
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
 
/** 
* @file     mTouch_acquisition.c
* @brief    Implements scanning procedure for the framework which performs the 
*           mTouch acquisition and provides an oversampled, scaled integer
*           value representing the relative capacitance of the sensor.
*/
#include "mTouch.h"

#if !defined(PIC_ADC_HCVD_AVAILABLE)

#asm
    #ifndef MTOUCH_ACQ_CAS_INCLUDE
    #define MTOUCH_ACQ_CAS_INCLUDE
        #if defined(_12F617) && (_HTC_VER_MAJOR_ == 9 && _HTC_VER_MINOR_ == 80)
            #include <cas12F617.h>      // HiTech's 9.80 compiler's caspic.h includes the 617 header file twice, causing compiling 
                                        // errors. This will by-pass the mistake by directly including the correct file only once.
                                        // This issue has been fixed as of the 9.81 release.
        #elif (_HTC_VER_MAJOR_ == 9 && _HTC_VER_MINOR_ >= 83)
            #if defined(_PIC14) || defined(_PIC14E)
            #include <caspic.h>
            #endif
        #endif
    #endif
#endasm

/** @name mTouch Acquisition ISR Variables
* These variables are used by the ISR of the mTouch Framework and should not be directly
* accessed by the main loop application.
*/
//@{
  mTouch_AcquisitionData        mTouch_acqData      [MTOUCH_NUMBER_SENSORS];  
  mTouch_AcquisitionData*       mTouch_currentAcqData;  
                uint16_t        mTouch_sensorData   [MTOUCH_NUMBER_SENSORS];    
                uint16_t        mTouch_lastResult;                                      ///< Stores the last active-mode ADC result for differential calculation        @ingroup Acquisition
                uint8_t         mTouch_delayCount;                                      ///< Delay counter variable used in CVD scan sequence                           @ingroup Acquisition

#if (MTOUCH_NUMBER_SENSORS > 1)
    #if (MTOUCH_NUM_MODES > 1)
                uint8_t         mTouch_modeIndex; 
          const uint8_t*        mTouch_currentScan;
    #else
                uint8_t         mTouch_currentScan;
    #endif
#endif

    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE) && (MTOUCH_NUMBER_SENSORS > 1)
                uint8_t         mTouch_oversample   [MTOUCH_NUMBER_SENSORS];
    #endif
    
#if defined(MTOUCH_JITTER_ENABLE)
                uint8_t         mTouch_jitter = 0x55;                                   ///< Stores the current random seed value for jittering                         @ingroup Acquisition
#endif
//@}

    
    //
    // TODO:
    //
    //  1. Rework storage macros to not be offset
    //  2. Rewrite mTouch scan loop - allow for independant oversampling values
    //  3. Adjust for a variable number of modes
    //      - If only one mode, simplify code
    //  4. If only one sensor, simplify code further
    
// Local Function Prototypes
void mTouch_DecimationFilter    (void);
void mTouch_WaitForGoDone       (void);
void mTouch_StoreScanA          (void);
void mTouch_StoreScanB          (void);
#if MTOUCH_NUM_MODES > 1
void mTouch_ChangeMode          (uint8_t);
#else
void mTouch_ChangeMode          (void);
#endif
MTOUCH_SCAN_PROTOTYPES();
    
    
// Constant Memory
#if (MTOUCH_NUMBER_SENSORS > 1)
    #if MTOUCH_NUM_MODES > 1
    const uint8_t           mTouch_mode0[MTOUCH_MODE0_NUM_SENSORS + 1]  = MTOUCH_MODE0_VAR_INIT;
    const uint8_t           mTouch_mode1[MTOUCH_MODE1_NUM_SENSORS + 1]  = MTOUCH_MODE1_VAR_INIT;
    #endif
    #if MTOUCH_NUM_MODES > 2
    const uint8_t           mTouch_mode2[MTOUCH_MODE2_NUM_SENSORS + 1]  = MTOUCH_MODE2_VAR_INIT;
    #endif
    #if MTOUCH_NUM_MODES > 3
    const uint8_t           mTouch_mode3[MTOUCH_MODE3_NUM_SENSORS + 1]  = MTOUCH_MODE3_VAR_INIT;
    #endif
    #if MTOUCH_NUM_MODES > 4
    const uint8_t           mTouch_mode4[MTOUCH_MODE4_NUM_SENSORS + 1]  = MTOUCH_MODE4_VAR_INIT;
    #endif
    #if MTOUCH_NUM_MODES > 5
    const uint8_t           mTouch_mode5[MTOUCH_MODE5_NUM_SENSORS + 1]  = MTOUCH_MODE5_VAR_INIT;
    #endif
    #if MTOUCH_NUM_MODES > 6
    const uint8_t           mTouch_mode6[MTOUCH_MODE6_NUM_SENSORS + 1]  = MTOUCH_MODE6_VAR_INIT;
    #endif
    #if MTOUCH_NUM_MODES > 7
    const uint8_t           mTouch_mode7[MTOUCH_MODE7_NUM_SENSORS + 1]  = MTOUCH_MODE7_VAR_INIT;
    #endif
    #if MTOUCH_NUM_MODES > 1
    const uint8_t*    const mTouch_mode             [MTOUCH_NUM_MODES]  = MTOUCH_MODE_VAR_INIT;
    #endif
    
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE) && (MTOUCH_NUMBER_SENSORS > 1)
    const uint8_t   mTouch_Oversample_Default[MTOUCH_NUMBER_SENSORS]  = MTOUCH_UNIQUE_OVERSAMPLE_INIT;
    #endif
    
    void (*const mTouch_ScanA[MTOUCH_NUMBER_SENSORS])(void)             = MTOUCH_SCANA_VAR_INIT;    // Constant array of function pointers
    void (*const mTouch_ScanB[MTOUCH_NUMBER_SENSORS])(void)             = MTOUCH_SCANB_VAR_INIT;    // Constant array of function pointers
#endif


/**
*   @ingroup    Acquisition
*   @brief      Implements the framework's active-mode acquisition routine. 
*               Sometimes implemented as an ISR, sometimes as a normal function.
*
*   Implements the acquisition process for the mTouch Framework. 
*   Please do not modify.
*
*/
#if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CONTROLS_ISR)
void interrupt  mTouch_Scan(void)
#elif (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_ISR)
void            mTouch_Scan(void)
#elif (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
uint8_t         mTouch_Scan(void)
#endif
{
#asm
mTouch_Scan:
#endasm 
        
    //
    // Intro Logic :: Check to see if we should scan.
    //
    #if     (MTOUCH_INTEGRATION_TYPE == MTOUCH_CONTROLS_ISR)
        if (MTOUCH_ISR_TMRxIF == 0) 
        {
            return;
        }  
        
        MTOUCH_ISR_TMRxIF = 0; 
    #elif   (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
        if (mTouch_state.scanningEnabled == 0)
        {
            return 0;
        }    
    #endif
    
    #if (MTOUCH_SCAN_FUNCTIONALITY == MTOUCH_SCANS_ALL_SENSORS)
    do
    {
    #endif
    
        #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
        if (mTouch_oversample[MTOUCH_CURRENTSCAN_VALUE] != 0)
        {
        #endif
    
            mTouch_currentAcqData   = &mTouch_acqData[MTOUCH_CURRENTSCAN_VALUE];
            
            #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
            do
            {
                mTouch_state.isrServiced = 0;
            #endif
            
                //
                //  Scan A
                //   
                #if MTOUCH_NUMBER_SENSORS > 1
                mTouch_ScanA[MTOUCH_CURRENTSCAN_VALUE]();
                #else
                mTouch_ScanA_0();
                #endif
                
                
                mTouch_WaitForGoDone();
                mTouch_StoreScanA();
                
                //
                // Scan B
                //
                #if MTOUCH_NUMBER_SENSORS > 1
                mTouch_ScanB[MTOUCH_CURRENTSCAN_VALUE]();
                #else
                mTouch_ScanB_0();
                #endif
                
                
                mTouch_WaitForGoDone();
                mTouch_StoreScanB();
                
                //
                //  Math
                //
                #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
                if (!mTouch_state.isRepeatScan)
                {
                #endif
                    mTouch_DecimationFilter();
                #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
                }
                #endif
                
            #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
                mTouch_state.isRepeatScan = mTouch_state.isrServiced;
            } while (mTouch_state.isRepeatScan);
            #endif
        
        #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
            mTouch_oversample[MTOUCH_CURRENTSCAN_VALUE]--;
        }
        #endif
        
    
    #if MTOUCH_NUMBER_SENSORS > 1
        mTouch_currentScan++;
    #if (MTOUCH_SCAN_FUNCTIONALITY == MTOUCH_SCANS_ALL_SENSORS)
    } while (MTOUCH_CURRENTSCAN_VALUE != MTOUCH_NUMBER_SENSORS);
    #else
    if  (MTOUCH_CURRENTSCAN_VALUE == MTOUCH_NUMBER_SENSORS)
    {
    #endif
    
        #if MTOUCH_NUM_MODES > 1
        mTouch_currentScan      = mTouch_mode[mTouch_modeIndex];
        #else
        mTouch_currentScan      = 0;
        #endif
    #endif  //  end: if MTOUCH_NUMBER_SENSORS > 1
    
        #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE) && (MTOUCH_NUMBER_SENSORS > 1)
        mTouch_delayCount = 0;      // Used as a temporary flag variable
        do
        {
            if (mTouch_oversample[MTOUCH_CURRENTSCAN_VALUE])               // Are there any sensors left to be scanned?
                mTouch_delayCount++;
            mTouch_currentScan++;   
        } while (MTOUCH_CURRENTSCAN_VALUE != MTOUCH_NUMBER_SENSORS);
        
            #if MTOUCH_NUM_MODES > 1
            mTouch_currentScan      = mTouch_mode[mTouch_modeIndex];
            #else
            mTouch_currentScan      = 0;
            #endif
        
        if (mTouch_delayCount == 0)
        {
        #else
        if (--mTouch_stateVars.sampleCounter == 0)
        {
            mTouch_stateVars.sampleCounter   = MTOUCH_SpS_VALUE; 
        #endif

            #if (MTOUCH_NUMBER_SENSORS > 1)
            do 
            {
                mTouch_currentAcqData = &mTouch_acqData[MTOUCH_CURRENTSCAN_VALUE];
                mTouch_sensorData[MTOUCH_CURRENTSCAN_VALUE] = (uint16_t)(((uint24_t)((*mTouch_currentAcqData).accumulator.v) & 0xFFFFF) >> MTOUCH_SCALING_VALUE);
                (*mTouch_currentAcqData).accumulator.v &= 0xF00000;
                
                mTouch_currentScan++;    
            } while (MTOUCH_CURRENTSCAN_VALUE != MTOUCH_NUMBER_SENSORS);
            #else
                mTouch_sensorData[MTOUCH_CURRENTSCAN_VALUE] = (uint16_t)(((uint24_t)(mTouch_acqData[0].accumulator.v) & 0xFFFFF) >> MTOUCH_SCALING_VALUE);
                mTouch_acqData[0].accumulator.v &= 0xF00000;
            #endif
            
            #if MTOUCH_NUM_MODES > 1
            mTouch_ChangeMode(mTouch_modeIndex);
            #else
            mTouch_ChangeMode();
            #endif
            mTouch_state.dataReady  = 1;
        }
    
    #if MTOUCH_NUMBER_SENSORS > 1
    #if (MTOUCH_SCAN_FUNCTIONALITY != MTOUCH_SCANS_ALL_SENSORS)
    }
    #endif
    #endif
        
    #if defined(MTOUCH_EEPROM_ENABLE) 
    if (MTOUCH_JITTER_VALUE != 0)
    {
    #endif
    
        #if defined(MTOUCH_EEPROM_ENABLE) || defined(MTOUCH_JITTER_ENABLE)
            #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
                JITTER_MAIN_LOOP(); 
            #else
                JITTER_START_TIME(); 
            #endif
        #endif
        
    #if defined(MTOUCH_EEPROM_ENABLE)
    }
    #endif
    
    #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CONTROLS_ISR) || (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_ISR)
    MTOUCH_ISR_TMRxIF = 0;          // Clear TMRxIF to avoid immediate ISR re-entry upon leaving.        
    #endif        
    
    #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CONTROLS_ISR) || (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_ISR)
        return;
    #elif (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
        return mTouch_state.dataReady;
    #endif
}

void mTouch_DecimationFilter(void)
{
    // Get the current decimated filter value for the sensor.
    #if MTOUCH_NUMBER_SENSORS > 1
    uint16_t    result  = (uint16_t)((*mTouch_currentAcqData).result.v >> 4);
    #else
    uint16_t    result  = (uint16_t)(mTouch_acqData[0].result.v >> 4);
    #endif
    
    #if defined(MTOUCH_EEPROM_ENABLED)
        #if MTOUCH_DECIMATION_MAX_STEP > 255
        uint16_t    decMaxStep  = MTOUCH_DECIMATION_MAX_VALUE;
        #else
        uint8_t     decMaxStep  = MTOUCH_DECIMATION_MAX_VALUE;
        #endif
        
        #undef MTOUCH_DECIMATION_MAX_VALUE
        #define MTOUCH_DECIMATION_MAX_VALUE decMaxStep      // Replace the EEPROM access code with our temporary variable
    #endif

    #if defined(MTOUCH_DECIMATION_MAX_STEP) || defined(MTOUCH_EEPROM_ENABLED)
        #if (MTOUCH_DECIMATION_MAX_STEP != 0) || defined(MTOUCH_EEPROM_ENABLED)
            if ((uint16_t)(mTouch_lastResult) > (uint16_t)(result + MTOUCH_DECIMATION_MAX_VALUE))
            {                                                                                           
                result += MTOUCH_DECIMATION_MAX_VALUE;      // Latest conversion is very large. Increment by max step size only.
            }                                                                         
            else if (result > MTOUCH_DECIMATION_MAX_VALUE)                                     
            {                                                                               
                if ((uint16_t)(mTouch_lastResult) < result - MTOUCH_DECIMATION_MAX_VALUE)  
                {                                                                           
                    result -= MTOUCH_DECIMATION_MAX_VALUE;  // Latest conversion is very small. Decrement by max step size only.
                } else {          
                    result = mTouch_lastResult;             // Latest conversion is similar in value to decimated value.
                }                                                                               
            }                                                                                   
            else                                            // Latest conversion is less than (decimated value + STEP_SIZE)
            {                                               // AND the decimated value is in danger of underflowing.                               
                result = mTouch_lastResult; 
            } 
        #else                                           
            result = mTouch_lastResult;                     // Decimation disabled.
        #endif
    #else                                               
        result = mTouch_lastResult;                         // Decimation disabled.
    #endif
 
    #if MTOUCH_NUMBER_SENSORS > 1
    (*mTouch_currentAcqData).result.v &= 0x000F;            // Store the new decimated value into memory for later use.
    (*mTouch_currentAcqData).result.v |= (uint16_t)(result << 4);     
    #else
    mTouch_acqData[0].result.v &= 0x000F;            // Store the new decimated value into memory for later use.
    mTouch_acqData[0].result.v |= (uint16_t)(result << 4);     
    #endif
    
    #if MTOUCH_NUMBER_SENSORS > 1
    (*mTouch_currentAcqData).accumulator.v += (uint16_t)((*mTouch_currentAcqData).result.v >> 4);
    #else
    mTouch_acqData[0].accumulator.v += (uint16_t)(mTouch_acqData[0].result.v >> 4);
    #endif
}

void mTouch_WaitForGoDone(void)
{
    mTouch_delayCount = 0xFF;  
    while (GO_nDONE && --mTouch_delayCount);  
}

void mTouch_StoreScanA(void)
{
    #if (PIC_ADC_BITS == 8)
    mTouch_lastResult = ADRES;
    #elif (PIC_ADC_BITS == 10)
    mTouch_lastResult = (uint16_t) (ADRESH << 8) + ADRESL;
    #elif (PIC_ADC_BITS == 12)
    mTouch_lastResult = (uint16_t) (ADRESH << 8) + ADRESL;
    #else
    #error The PIC_ADC_BITS value in your device's hardware profile is invalid. It must be 8, 10, or 12.
    #endif
}

void mTouch_StoreScanB(void)
{
    #if (PIC_ADC_BITS == 8)
    mTouch_lastResult = (uint16_t) (ADRES  | 0x0100)                - mTouch_lastResult;
    #elif (PIC_ADC_BITS == 10)
    mTouch_lastResult = (uint16_t)((ADRESH |   0x04) << 8) + ADRESL - mTouch_lastResult;
    #elif (PIC_ADC_BITS == 12)
    mTouch_lastResult = (uint16_t)((ADRESH |   0x10) << 8) + ADRESL - mTouch_lastResult;
    #else
    #error The PIC_ADC_BITS value in your device's hardware profile is invalid. It must be 8, 10, or 12.
    #endif
}

#if MTOUCH_NUM_MODES > 1
void mTouch_ChangeMode(uint8_t newModeIndex)
{
#else
void mTouch_ChangeMode(void)
{
#endif
    mTouch_currentAcqData   = 0;
    
    #if MTOUCH_NUMBER_SENSORS > 1
    #if MTOUCH_NUM_MODES > 1
    if (mTouch_modeIndex != newModeIndex)
    {
        mTouch_modeIndex            = newModeIndex; 
        mTouch_state.justChanged    = 1;

        newModeIndex = 0;
        do
        {
            mTouch_acqData[newModeIndex].accumulator.v &= 0xF00000;
            newModeIndex++;
        } while (newModeIndex != MTOUCH_NUMBER_SENSORS);
    }

    mTouch_currentScan      = mTouch_mode[mTouch_modeIndex];
    #else   
    mTouch_currentScan      = 0;
    #endif
    #endif
    
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE) && MTOUCH_NUMBER_SENSORS > 1
    do
    {
        mTouch_oversample[MTOUCH_CURRENTSCAN_VALUE] = mTouch_Oversample_Default[MTOUCH_CURRENTSCAN_VALUE];    
        mTouch_currentScan++;   
    } while (MTOUCH_CURRENTSCAN_VALUE != MTOUCH_NUMBER_SENSORS);
    
        #if MTOUCH_NUM_MODES > 1
        mTouch_currentScan      = mTouch_mode[mTouch_modeIndex];
        #else
        mTouch_currentScan      = 0;
        #endif
        
    #else
    mTouch_stateVars.sampleCounter   = MTOUCH_SpS_VALUE;
    #endif
}

#if MTOUCH_NUMBER_SENSORS > 0    
MTOUCH_SCAN_FUNCTION(0);   
#endif
#if MTOUCH_NUMBER_SENSORS > 1    
MTOUCH_SCAN_FUNCTION(1);   
#endif
#if MTOUCH_NUMBER_SENSORS > 2    
MTOUCH_SCAN_FUNCTION(2);   
#endif
#if MTOUCH_NUMBER_SENSORS > 3    
MTOUCH_SCAN_FUNCTION(3);   
#endif
#if MTOUCH_NUMBER_SENSORS > 4    
MTOUCH_SCAN_FUNCTION(4);   
#endif
#if MTOUCH_NUMBER_SENSORS > 5    
MTOUCH_SCAN_FUNCTION(5);   
#endif
#if MTOUCH_NUMBER_SENSORS > 6    
MTOUCH_SCAN_FUNCTION(6);   
#endif
#if MTOUCH_NUMBER_SENSORS > 7    
MTOUCH_SCAN_FUNCTION(7);   
#endif
#if MTOUCH_NUMBER_SENSORS > 8    
MTOUCH_SCAN_FUNCTION(8);   
#endif
#if MTOUCH_NUMBER_SENSORS > 9    
MTOUCH_SCAN_FUNCTION(9);   
#endif
#if MTOUCH_NUMBER_SENSORS > 10   
MTOUCH_SCAN_FUNCTION(10);  
#endif
#if MTOUCH_NUMBER_SENSORS > 11   
MTOUCH_SCAN_FUNCTION(11);  
#endif
#if MTOUCH_NUMBER_SENSORS > 12   
MTOUCH_SCAN_FUNCTION(12);  
#endif
#if MTOUCH_NUMBER_SENSORS > 13   
MTOUCH_SCAN_FUNCTION(13);  
#endif
#if MTOUCH_NUMBER_SENSORS > 14   
MTOUCH_SCAN_FUNCTION(14);  
#endif
#if MTOUCH_NUMBER_SENSORS > 15   
MTOUCH_SCAN_FUNCTION(15);  
#endif
#if MTOUCH_NUMBER_SENSORS > 16   
MTOUCH_SCAN_FUNCTION(16);  
#endif
#if MTOUCH_NUMBER_SENSORS > 17   
MTOUCH_SCAN_FUNCTION(17);  
#endif
#if MTOUCH_NUMBER_SENSORS > 18   
MTOUCH_SCAN_FUNCTION(18);  
#endif
#if MTOUCH_NUMBER_SENSORS > 19   
MTOUCH_SCAN_FUNCTION(19);  
#endif
#if MTOUCH_NUMBER_SENSORS > 20   
MTOUCH_SCAN_FUNCTION(20);  
#endif
#if MTOUCH_NUMBER_SENSORS > 21   
MTOUCH_SCAN_FUNCTION(21);  
#endif
#if MTOUCH_NUMBER_SENSORS > 22   
MTOUCH_SCAN_FUNCTION(22);  
#endif
#if MTOUCH_NUMBER_SENSORS > 23   
MTOUCH_SCAN_FUNCTION(23);  
#endif
#if MTOUCH_NUMBER_SENSORS > 24   
MTOUCH_SCAN_FUNCTION(24);  
#endif
#if MTOUCH_NUMBER_SENSORS > 25   
MTOUCH_SCAN_FUNCTION(25);  
#endif
#if MTOUCH_NUMBER_SENSORS > 26   
MTOUCH_SCAN_FUNCTION(26);  
#endif
#if MTOUCH_NUMBER_SENSORS > 27   
MTOUCH_SCAN_FUNCTION(27);  
#endif
#if MTOUCH_NUMBER_SENSORS > 28   
MTOUCH_SCAN_FUNCTION(28);  
#endif
#if MTOUCH_NUMBER_SENSORS > 29   
MTOUCH_SCAN_FUNCTION(29);  
#endif

#endif