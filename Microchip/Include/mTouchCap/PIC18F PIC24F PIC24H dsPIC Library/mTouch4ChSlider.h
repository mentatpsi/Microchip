/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                      
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouch4ChSlider.h
 *  Dependencies:    None.
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This header contains all definitions related to 
 *                   4 channels slider.
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
#ifndef _MTOUCH_4CH_SLIDER_H_
#define _MTOUCH_4CH_SLIDER_H_

/****************************************************************************
  Structure:
     MTOUCH4CHSLIDER

  Description:
    Contains all parameters related to 4 channel slider.
  ***************************************************************************/
typedef struct tag_MTOUCH4CHSLIDER {
UINT8   state;      // current state (see MTOUCHCONTROLSTATE union in mTouchControl.h)
INT16   value;      // current position from 0 to 1000
SENSOR* pCh1Sensor; // pointer to sensor 1
SENSOR* pCh2Sensor; // pointer to sensor 2
SENSOR* pCh3Sensor; // pointer to sensor 3
SENSOR* pCh4Sensor; // pointer to sensor 4
}MTOUCH4CHSLIDER;

/****************************************************************************
  Array:
     sliders4ch[MTOUCH_4CHSLIDERS_NUMBER]

  Description:
    Contains information about all 4 channel sliders. Numbers of sliders
    MTOUCH_4CHSLIDERS_NUMBER is defined in mTouchConfig.h.
  ***************************************************************************/
#if (MTOUCH_4CHSLIDERS_NUMBER > 0)
extern MTOUCH4CHSLIDER  sliders4ch[MTOUCH_4CHSLIDERS_NUMBER];
#endif

/****************************************************************************
  Function:
    void MTouchSet4ChSlider(UINT8 sliderNumber, UINT8 ch1SensorNumber,  UINT8 ch2SensorNumber,
                                                UINT8 ch3SensorNumber,  UINT8 ch4SensorNumber)

  Summary:
    Initializes 4 channels slider.

  Description:
    Sets parameters for 4 channel slider.
 
  Precondition:
    None.

  Parameters:
    sliderNumber    - slider number.
    ch1SensorNumber - sensor 1 number.
    ch2SensorNumber - sensor 2 number.
    ch1SensorNumber - sensor 3 number.
    ch2SensorNumber - sensor 4 number.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void MTouchSet4ChSlider(UINT8 sliderNumber, UINT8 ch1SensorNumber,  UINT8 ch2SensorNumber,  UINT8 ch3SensorNumber,  UINT8 ch4SensorNumber);

/****************************************************************************
  Function:
    void MTouchDecode4ChSliders(void)

  Summary:
    Decodes all 4 channels sliders.

  Description:
    Decodes all 4 channels sliders.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void MTouchDecode4ChSliders(void);

/****************************************************************************
  Macro:
    MTouchGet4ChSliderState(sliderNumber)

  Summary:
    Returns the slider state.

  Description:
    Returns the slider state.
 
  Precondition:
    None.

  Parameters:
    sliderNumber - number of slider.

  Returns:
    THe slider state (see MTOUCHCONTROLSTATE union in mTouchControl.h).

  Remarks:
    None.
  ***************************************************************************/
#define MTouchGet4ChSliderState(sliderNumber)    (sliders4ch[sliderNumber].state)

/****************************************************************************
  Macro:
    MTouchGet4ChSliderValue(sliderNumber)

  Summary:
    Returns the slider current position.

  Description:
    Returns the slider current position.
 
  Precondition:
    None.

  Parameters:
    sliderNumber - number of slider.

  Returns:
    The slider value (current position) from 0 to 1000.

  Remarks:
    None.
  ***************************************************************************/
#define MTouchGet4ChSliderValue(sliderNumber)    (sliders4ch[sliderNumber].value)

#endif

