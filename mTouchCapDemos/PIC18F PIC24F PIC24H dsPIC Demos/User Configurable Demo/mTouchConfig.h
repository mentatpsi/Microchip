/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouchConfig.h
 *  Dependencies:    PICFamilies.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     Configuration file for mTouch library.
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

#include "PICFamilies.h"

/****************************************************************************
  Acquisition method. Select only one: CTMU or CVD. 
  ***************************************************************************/
#if defined(__dsPIC30F__)||defined(__dsPIC33F__)||defined(__PIC24H__)||\
    defined(FAMILY_dsPIC33EPXXX_GP_MC_MU_806_810_814_PIC24EPXXX_GP_GU_810_814)

#define MTOUCH_USE_CVD

#else

#define MTOUCH_USE_CTMU

#endif

/****************************************************************************
  Debugging. Uncomment to enable debug functions.
  mTouchDebug.c must be added in project for this option.
  ***************************************************************************/
#define MTOUCH_DEBUG

/****************************************************************************
  Number of sensors (analog inputs connected to sensors).
  ***************************************************************************/
#define MTOUCH_SENSORS_NUMBER         2

/****************************************************************************
  Number of button controls.
  mTouchControl.c and mTouchButton.c must be added in project for this option.
  ***************************************************************************/
#define MTOUCH_BUTTONS_NUMBER         1

/****************************************************************************
  Number of matrix button controls.
  mTouchControl.c and mTouchMatrixButton.c must be added in project for this option.
  ***************************************************************************/
#define MTOUCH_MATRIXBUTTONS_NUMBER   0

/****************************************************************************
  Number of 2 channels slider controls.
  mTouchControl.c and mTouch2ChSlider.c must be added in project for this option.
  ***************************************************************************/
#define MTOUCH_2CHSLIDERS_NUMBER      0

/****************************************************************************
  Number of 4 channels slider controls.
  mTouchControl.c and mTouch4ChSlider.c must be added in project for this option.
  ***************************************************************************/
#define MTOUCH_4CHSLIDERS_NUMBER      0

/****************************************************************************
   When the slider value is updated, this factor determines what weight is
   given in the calculation. Can be set to 0(100% new value/no averaging),
   1(50% of new value), 2(25% of new value), 3(12.5% of new value) and so on.  
  ***************************************************************************/
#define  AVG_SLIDER_VALUE             4

/****************************************************************************
  CTMU charge delay settings.
  This value is used when "chargeDelay" is set to -1 in MTouchSetSensor(...) call.
  ***************************************************************************/
#define MTOUCH_DEFAULT_CHARGE_DELAY   25                                          

/****************************************************************************
  Default threshold for press event detection.
  This value is used when "threshold" is set to -1 in MTouchSetSensor(...) call.
  ***************************************************************************/
#define MTOUCH_DEFAULT_THRESHOLD      1000

/****************************************************************************
  Number of acquisitions for one sample of the sensor.
  This value is used when "oversampling" is set to -1 in MTouchSetSensor(...) call.
  ***************************************************************************/
#define MTOUCH_DEFAULT_OVERSAMPLING   32

/****************************************************************************
  This is the number of total scans that should be taken for the sensor
  before it will be considered initialized. Allowable range is from 1 to 65535.
  ***************************************************************************/
#define POWER_UP_SAMPLES              150

/****************************************************************************
  Number of consecutive scans a button must be seen as pressed or released
  before an updated state is declared. Allowable range is from 1 to 255.
  ***************************************************************************/
#define	DEBOUNCE_COUNT                 2

/****************************************************************************
  Initial delay for control DECODE_PRESS_REPEAT decode method.
  Defines how many times the control decoding must be done before the control
  starts to repeat CONTROL_PRESS/CONTROL_RELEASE events.
  Allowable range is from 1 to 65535.
  ***************************************************************************/
#define MCONTROL_REPEAT_INITIAL_DELAY  100

/****************************************************************************
  Delay between CONTROL_PRESS/CONTROL_RELEASE events for
  the controls DECODE_PRESS_REPEAT decode method.
  Allowable range is from 1 to 65535.
  ***************************************************************************/
#define MCONTROL_REPEAT_DELAY          20

/****************************************************************************
   When the average updates itself using a new reading, this value determines
   what weight is given to the reading variable in the calculation of the new
   average. The reading variable will have a weight of 1/AVG_UPDATE in
   the average calculation. Can be set to 2,4,8 or 16.
  ***************************************************************************/
#define AVG_UPDATE                     8
                                            
/****************************************************************************
   The update rate of the sensors' average values when sensor is released.
   Allowable Range from 1 to 65535.
  ***************************************************************************/
#define AVG_RATE_RELEASED              4

/****************************************************************************
   The update rate of the sensors' average values when sensor is pressed.
   Allowable Range from 1 to 65535.
  ***************************************************************************/
#define AVG_RATE_PRESSED               400

/****************************************************************************
  CTMU current settings.
  Bits 1-0 select the current source range (IRNG) and bits 7-2 select current
  trim value (ITRIM, signed).
  ***************************************************************************/
#define MTOUCH_CTMU_CURRENT            0xbf   

/****************************************************************************
  ADC and CTMU types (see PIC datasheet).
  ***************************************************************************/
#if defined( FAMILY_PIC18XXJ50)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC18XXJ53)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC18XXJ11)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC18XXJ13)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC18_2x_4xK22)

#define MTOUCH_USE_12_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC18XXK22)

#define MTOUCH_USE_12_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC18XXJ90)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC18XXJ93)

#define MTOUCH_USE_12_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC18XXJ72)

#define MTOUCH_USE_12_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC18XXK90)

#define MTOUCH_USE_12_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC18XXJ94)

#define MTOUCH_USE_12_BITS_ADC
#define MTOUCH_CTMU_HAS_CTMUCON2_REG

#elif defined( FAMILY_PIC18XXK50)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC24F04KA20X)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC24FXXKA10X)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC24FXXKA30X)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_CTMUCON2_REG

#elif defined( FAMILY_PIC24FJXXGA10X)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC24FJXXGB10X)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC24FJXXXGA1XX)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC24FJXXXGB1XX)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC24FJXXXGB2XX)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC24FJXXXDAXXX)

#define MTOUCH_USE_10_BITS_ADC
#define MTOUCH_CTMU_HAS_NO_CTMUCON2_REG

#elif defined( FAMILY_PIC24FJXXXGA3XX)

#define MTOUCH_USE_12_BITS_ADC
#define MTOUCH_CTMU_HAS_CTMUCON2_REG

#elif defined(__dsPIC30F__)||defined(__dsPIC33F__)||defined(__PIC24H__)||\
      defined(__dsPIC33E__)||defined(__PIC24E__)

#define MTOUCH_USE_12_BITS_ADC
#define MTOUCH_CTMU_HAS_CTMUCON2_REG

#else

#error The PIC device family is not defined.

#endif
