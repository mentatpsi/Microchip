/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                      
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouchDebug.h
 *  Dependencies:    None.
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This header contains headers and definitions related
 *                   to mTouch controls.
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


#ifndef _MTOUCH_DEBUG_H_
#define _MTOUCH_DEBUG_H_

/****************************************************************************
   This parameter defines optimal CTMU charge sensor voltage. It equals
   100*(1 - 1/DEBUG_OPTIMAL_CHARGE_DIVIDER)% of AVdd. For example:
   2 - 50% of AVdd, 3 - 66% of AVdd, 4 - 75% of AVdd and so on. Used in the
   CTMU optimal charge delay calculation.
  ***************************************************************************/
#define DEBUG_OPTIMAL_CHARGE_DIVIDER      3 // 66%

/****************************************************************************
   This parameter defines optimal CTMU charge delay used in the optimal 
   CTMU current calculations.
  ***************************************************************************/
#define DEBUG_OPTIMAL_CHARGE_DELAY        8 // charge delay mistake less than
                                            // (100/DEBUG_OPTIMAL_CHARGE_DELAY)%

/****************************************************************************
   This parameter defines optimal threshold for the sensors. It equals
   (100/DEBUG_OPTIMAL_THRESHOLD_DIVIDER)% of sensor's signal(delta) amplitude.
   For example: 2 - 50% of max. delta, 3 - 33% of max. delta, 4 - 25% of max.
   delta and so on. Used in the optimal sensor's threshold calculations.
  ***************************************************************************/
#define DEBUG_OPTIMAL_THRESHOLD_DIVIDER   8 // 12.5%

/****************************************************************************
   This parameter defines a timeout for MTouchDebugThreshold(...) function.
  ***************************************************************************/
#define DEBUG_THRESHOLD_CALC_TIMEOUT      1500000

/****************************************************************************
   This parameter defines a noise level for one sample. It is used 
   in minimum threshold calculations in MTouchDebugThreshold(...) function.
   minimum threshold = DEBUG_NOISE_PER_SAMPLE*oversampling
  ***************************************************************************/
#define DEBUG_NOISE_PER_SAMPLE            3

/****************************************************************************
   These parameters define ranges for CTMU current selection settings
   (IRNG bits and ITRIM bits).
  ***************************************************************************/
#ifdef MTOUCH_USE_12_BITS_ADC
#define DEBUG_CTMU_IRNG_MIN               0
#else
#define DEBUG_CTMU_IRNG_MIN               1
#endif
#define DEBUG_CTMU_IRNG_MAX               3
#define DEBUG_CTMU_ITRIM_MIN             -31
#define DEBUG_CTMU_ITRIM_MAX              31

/****************************************************************************
   Delimiter character between data
  ***************************************************************************/
#define LOG_DELIMITER	','

/****************************************************************************
  Structure:
    DEBUGDELAY

  Description:
    Contains results for CTMU charge delay adjustment.
    Used by MTouchDebugDelay(...) function.
  ***************************************************************************/
typedef struct tag_DEBUGDELAY {
INT16         error;          // sample error value for the adjusted delay.
                              // It should be as low as possible. If the mistake is
                              // bigger than 128 for 10-bit ADC or bigger than
                              // 512 for 12-bit ADC then the CTMU current must be
                              // adjusted first.
INT8          delay;          // settings for CTMU charge delay.
} DEBUGDELAY;

/****************************************************************************
  Structure:
    DEBUGCURRENT

  Description:
    Contains results for CTMU current adjustment.
    Used for MTOUCH_CTMU_CURRENT setting in mTouchConfig.h file.
  ***************************************************************************/
typedef struct tag_DEBUGCURRENT {
INT16         error;         // charge delay error value for the adjusted current.
UINT8         current;       // settings for CTMU current. Bits 1-0 define
                             // the current source range (IRNG) and bits 7-2
                             // define current trim value (ITRIM, signed).
} DEBUGCURRENT;

/****************************************************************************
  Function:
    void LogChar(char ch)

  Summary:
    Outputs character to debug log.

  Description:
    Outputs character to debug log.
 
  Precondition:
    None.

  Parameters:
    ch - character to be transmitted.

  Returns:
    None.

  Remarks:
    This function must be defined in application.
  ***************************************************************************/
void LogChar(char ch);

/****************************************************************************
  Function:
    DEBUGCURRENT* MTouchDebugCurrent(UINT8 sensorNumber)

  Summary:
    Adjusts CTMU current to charge the sensor.

  Description:
    Adjusts CTMU current to charge the sensor to 75% of AVdd for unpressed
    state when charge delay is 8.
 
  Precondition:
    Sensor must be initialized with MTouchSetSensor(...).

  Parameters:
    sensorNumber - sensor number.

  Returns:
    The function returns a pointer to the structure with the CTMU current
	settings value and corresponding error in the sensor charge delay.

  Remarks:
    Use the CTMU current result to set MTOUCH_CTMU_CURRENT parameter in 
    mTouchConfig.h
  ***************************************************************************/
DEBUGCURRENT* MTouchDebugCurrent(UINT8 sensorNumber);

/****************************************************************************
  Function:
    DEBUGDELAY* MTouchDebugDelay(UINT8 sensorNumber)

  Summary:
    Adjusts CTMU charge delay to charge the sensor to 75% of AVdd for unpressed
    state.

  Description:
    Adjusts CTMU charge delay to charge the sensor to 75% of AVdd for unpressed
    state.
 
  Precondition:
    Sensor must be initialized with MTouchSetSensor(...).

  Parameters:
    sensorNumber - sensor number.

  Returns:
    The function returns a pointer to the structure with the charge delay adjustment.

  Remarks:
    Use the charge delay result to intialize sensor (see parameter "chargeDelay" in 
    MTouchSetSensor(...) function).
  ***************************************************************************/
DEBUGDELAY* MTouchDebugDelay(UINT8 sensorNumber);

/****************************************************************************
  Function:
    INT16 MTouchDebugThreshold(UINT8 sensorNumber)

  Summary:
    This function returns an optimal threshold value for the sensor.

  Description:
    This function waits for the sensor press event and returns an optimal threshold 
	value. The threshold should be about 12.5% percents of the signal(delta) amplitude
    from sensor.
 
  Precondition:
    Sensor must be initialized with MTouchSetSensor(...).

  Parameters:
    sensorNumber - sensor number.

  Returns:
    The function returns an optimal sensor threshold value.

  Remarks:
    Use the threshold result to intialize sensor (see parameter "threshold" in 
    MTouchSetSensor(...) function).
  ***************************************************************************/
INT16 MTouchDebugThreshold(UINT8 sensorNumber);

/****************************************************************************
  Function:
    void MTouchDebugLogDeltas(void)

  Summary:
    Sends deltas for all sensors to debug log.

  Description:
    Sends deltas for all sensors to debug log. 

  Precondition:
    Sensors must be initialized with MTouchSetSensor(...).

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void MTouchDebugLogDeltas(void);

/****************************************************************************
  Function:
    void MTouchDebugLogAverages(void)

  Summary:
    Sends averages values for all sensors to debug log.

  Description:
    Sends averages values for all sensors to debug log.

  Precondition:
    Sensors must be initialized with MTouchSetSensor(...).

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void MTouchDebugLogAverages(void);


#endif
