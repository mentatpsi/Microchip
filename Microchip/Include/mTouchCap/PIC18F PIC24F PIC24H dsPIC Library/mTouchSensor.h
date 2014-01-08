/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouchSensor.h
 *  Dependencies:    None.
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This header files contains all definitions for mTouch
 *                   sensors.
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

#ifndef _MTOUCH_SENSOR_H_
#define _MTOUCH_SENSOR_H_

/****************************************************************************
  Types:
    SFR and IOMASK

  Description:
    SFR - pointer to SFR register, IOMASK - bit mask for SFR IO registers.
  ***************************************************************************/
#if   defined(__PICC18__)
typedef volatile unsigned char*      SFR;
typedef unsigned char                IOMASK;
#elif defined(__18CXX)
typedef volatile near unsigned char* SFR;
typedef unsigned char                IOMASK;
#elif defined(__C30__)
typedef volatile unsigned int*       SFR;
typedef unsigned int                 IOMASK;
#else
typedef volatile unsigned long*      SFR;
typedef unsigned long                IOMASK;
#endif


/****************************************************************************
  Structure:
    SENSOR

  Description:
    Contains all parameters related to mTouch sensor.
  ***************************************************************************/
typedef struct tag_SENSOR {
INT32          average;              // tracks environmental changes in the system
INT32          oversampleFilterData; // oversampling filter data
SFR            trisReg;              // TRIS register address
SFR            latReg;               // LAT register address
INT16          decimateFilterData;   // decimate-filtered ADC result
UINT16         sampleCounter;        // counter variable for oversampling
UINT16         samplesNumber;        // defines how many samples used for oversampling
UINT16         pressThreshold;       // press detection threshold
UINT16         releaseThreshold;     // release detection threshold
UINT16         averageCount;         // counter variable for updating averages
UINT16         delta;                // difference between current and average values
UINT16         debounceCount;        // counter variable for state transition debouncing
UINT8          state;                // tracks the current state of the sensor
IOMASK         ioMask;               // sensor bit mask for LAT and TRIS registers
UINT8          channelNumber;        // analog channel number
INT8           chargeDelay;          // settings for CTMU charge delay
} SENSOR;

/****************************************************************************
  Array:
    sensors[MTOUCH_SENSORS_NUMBER]

  Description:
    Contains all parameters related to all sensors.The array size (MTOUCH_SENSORS_NUMBER)
    must be defined in mTouchConfig.h.
  ***************************************************************************/
extern volatile SENSOR sensors [MTOUCH_SENSORS_NUMBER];

/****************************************************************************
  Enumeration:
    MTOUCHSENSORSTATE

  Description:
    Defines all possible states for sensor.
  ***************************************************************************/
extern enum  _MTOUCHSENSORSTATE_ 
             {
                SENSOR_INITIALIZING,        // sensor is still initializing (see POWER_UP_SAMPLES in mTouchConfig.h)
                SENSOR_RELEASED,            // sensor is currently released
                SENSOR_PRESSED,             // sensor is currently pressed
                SENSOR_DISCONNECTED = 0x80  // sensor is removed from scan
             } MTOUCHSENSORSTATE;    


/****************************************************************************
  Function:
    void  MTouchAcquisition(void)

  Summary:
    Scans and decode all sensors.

  Description:
    Performs an acquisition for all sensors (using CVD or CTMU).
    Contains decimate and oversampling filters. When oversampling is finished 
    it decodes the sensor state.
 
  Precondition:
    The initialization should be done with MTouchInit() and MTouchSetSensor(...)
    functions.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    This function can be called periodically (for example by timer interrupt).
  ***************************************************************************/
void  MTouchAcquisition(void);


/****************************************************************************
  Function:
    INT16  MTouchAcquisitionCVD(SENSOR* pCurrentSensor, SENSOR* pPreviousSensor)

  Summary:
    Gets one sample from sensor.

  Description:
    Gets one sample from sensor using CVD (when MTOUCH_USE_CVD is defined
    in mTouchConfig.h).
 
  Precondition:
    None.

  Parameters:
    pCurrentSensor  - pointer to the current/sampled sensor.
    pPreviousSensor - pointer to the previous sensor in the scan.

  Returns:
    Differential CVD sample value for the current sensor.

  Remarks:
    None.
  ***************************************************************************/
INT16  MTouchAcquisitionCVD(SENSOR* pCurrentSensor, SENSOR* pPreviousSensor);

/****************************************************************************
  Function:
    INT16  MTouchAcquisitionCTMU(SENSOR* pSensor)

  Summary:
    Gets one sample from sensor.

  Description:
    Gets one sample from sensor using CTMU (when MTOUCH_USE_CTMU is defined
    in mTouchConfig.h).
 
  Precondition:
    None.

  Parameters:
    pCurrentSensor  - pointer to the current/sampled sensor.

  Returns:
    CTMU sample value for the current sensor.

  Remarks:
    None.
  ***************************************************************************/
INT16  MTouchAcquisitionCTMU(SENSOR* pSensor);


/****************************************************************************
  Function:
    void MTouchInit(void)

  Summary:
    Initializes mTouch library.

  Description:
    Initializes mTouch library. 

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void    MTouchInit (void);

/****************************************************************************
  Function:
     void MTouchSetSensor(UINT8 sensorNumber, SFR tris, SFR lat,
                          UINT8 ioBitNumber, UINT8 channelNumber,
                          INT16 threshold, INT16 oversampling,
                          INT8  chargeDelay);
  Summary:
    Initializes a sensor.

  Description:
    Initializes a sensor. 

  Precondition:
    None.

  Parameters:
    sensorNumber  - sensor number.
    tris          - address of TRIS register for the sensor.
    lat           - address of LAT register for the sensor.
    ioBitNumber   - sensor IO bit number for LAT and TRIS registers.
    channelNumber - analog input number for the sensor.
    threshold     - press detection threshold.
                    Set this parameter to -1 to use default value MTOUCH_DEFAULT_THRESHOLD
                    (mTouchConfig.h).   
    oversampling  - defines how many samples used for oversampling.
                    Set this parameter to -1 to use default value MTOUCH_DEFAULT_OVERSAMPLING
                    (mTouchConfig.h).   
    chargeDelay     - CTMU charge delay. Set this parameter to -1 to use default value
                    MTOUCH_DEFAULT_CHARGE_DELAY (mTouchConfig.h).   

  Returns:
    None.

  Remarks:
    All sensors must be added before aquisition.
  ***************************************************************************/
void MTouchSetSensor(UINT8 sensorNumber, SFR tris, SFR lat,
                     UINT8 ioBitNumber, UINT8 channelNumber, INT16 threshold, INT16 oversampling, INT8 chargeDelay);

/****************************************************************************
  Macro:
    MTouchSuspendSensor(sensorNumber)

  Summary:
    Excludes the sensor from scan.

  Description:
    Excludes the sensor from scan.
 
  Precondition:
    None.

  Parameters:
    sensorNumber - sensor number.

  Returns:
    None.

  Remarks:
    Use MTouchResumeSensor(...) to start the sensor scanning again.
  ***************************************************************************/
#define MTouchSuspendSensor(sensorNumber) {sensors[sensorNumber].state |= SENSOR_DISCONNECTED;}


/****************************************************************************
  Macro:
    MTouchResumeSensor(sensorNumber)

  Summary:
    Resumes the sensor scanning stopped by MTouchSuspendSensor(...).

  Description:
    Resumes the sensor scanning stopped by MTouchSuspendSensor(...).
 
  Precondition:
    None.

  Parameters:
    sensorNumber - sensor number.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#define MTouchResumeSensor(sensorNumber) {sensors[sensorNumber].state &= ~SENSOR_DISCONNECTED;}


/****************************************************************************
  Macro:
    MTouchSetChargeDelay(sensorNumber, delay)

  Summary:
    Sets charge delay value for sensor.

  Description:
    Sets charge delay value for sensor.
 
  Precondition:
    None.

  Parameters:
    sensorNumber - sensor number.
    delay        - charge delay.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#define MTouchSetChargeDelay(sensorNumber, delay) {sensors[sensorNumber].chargeDelay = delay;}

/****************************************************************************
  Macro:
    MTouchSetThreshold(sensorNumber, _threshold)

  Summary:
    Sets threshold for sensor.

  Description:
    Sets press detection threshold for sensor.
 
  Precondition:
    None.

  Parameters:
    sensorNumber - sensor number.
    _threshold   - press detection threshold.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#define MTouchSetThreshold(sensorNumber, _threshold) {sensors[sensorNumber].pressThreshold = _threshold;\
                                                     sensors[sensorNumber].releaseThreshold = _threshold>>1;}

/****************************************************************************
  Macro:
    MTouchSetOversampling(sensorNumber, _oversampling)

  Summary:
    Sets oversampling factor for sensor.

  Description:
    Sets oversampling factor for sensor.
 
  Precondition:
    None.

  Parameters:
    sensorNumber - sensor number.
    _oversampling - oversampling factor.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#define MTouchSetOversampling(sensorNumber, _oversampling) {sensors[sensorNumber].oversampling = _oversampling;}

/****************************************************************************
  Macro:
    MTouchGetSensorState(sensorNumber)

  Summary:
    Returns current state of sensor.

  Description:
    Returns current state of sensor.
 
  Precondition:
    None.

  Parameters:
    sensorNumber - sensor number.

  Returns:
    State of sensor (see MTOUCHSENSORSTATE enumeration).

  Remarks:
    None.
  ***************************************************************************/
#define MTouchGetSensorState(sensorNumber)        sensors[sensorNumber].state


/****************************************************************************
  Macro:
    MTouchInitializeSensor(sensorNumber)

  Summary:
    Starts the sensor initialization.

  Description:
    Starts the sensor initialization.
 
  Precondition:
    None.

  Parameters:
    sensorNumber - sensor number.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/

#define MTouchInitializeSensor(sensorNumber)     {sensors[sensorNumber].debounceCount = (DEBOUNCE_COUNT + POWER_UP_SAMPLES); sensors[sensorNumber].state = SENSOR_INITIALIZING;}

#endif
