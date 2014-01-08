/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                     
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouch2ChSlider.h
 *  Dependencies:    None.
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This header contains all definitions related to 
 *                   2 channels slider.
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

#ifndef _MTOUCH_2CH_SLIDER_H_
#define _MTOUCH_2CH_SLIDER_H_

/****************************************************************************
  Structure:
     MTOUCH2CHSLIDER

  Description:
    Contains all parameters related to 2 channel slider.
  ***************************************************************************/
typedef struct tag_MTOUCH2CHSLIDER {
UINT8   state;      // current state (see MTOUCHCONTROLSTATE union in mTouchControl.h)
INT16   value;      // current position from 0 to 1000
SENSOR* pCh1Sensor; // pointer to the first sensor
SENSOR* pCh2Sensor; // pointer to the second sensor
}MTOUCH2CHSLIDER;

/****************************************************************************
  Array:
     sliders2ch[MTOUCH_2CHSLIDERS_NUMBER]

  Description:
    Contains information about all 2 channel sliders. Numbers of sliders
    MTOUCH_2CHSLIDERS_NUMBER is defined in mTouchConfig.h.
  ***************************************************************************/
#if (MTOUCH_2CHSLIDERS_NUMBER > 0)
extern MTOUCH2CHSLIDER  sliders2ch[MTOUCH_2CHSLIDERS_NUMBER];
#endif

/****************************************************************************
  Function:
    void MTouchSet2ChSlider(UINT8 sliderNumber, UINT8 ch1SensorNumber,  UINT8 ch2SensorNumber)

  Summary:
    Initializes 2 channels slider.

  Description:
    Initializes 2 channels slider.
 
  Precondition:
    None.

  Parameters:
    sliderNumber    - slider number.
    ch1SensorNumber - first sensor number.
    ch2SensorNumber - second sensor number.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void MTouchSet2ChSlider(UINT8 sliderNumber, UINT8 ch1SensorNumber,  UINT8 ch2SensorNumber);

/****************************************************************************
  Function:
    void MTouchDecode2ChSliders(void)

  Summary:
    Decodes all 2 channels sliders.

  Description:
    Decodes all 2 channels sliders.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void MTouchDecode2ChSliders(void);

/****************************************************************************
  Macro:
    MTouchGet2ChSliderState(sliderNumber)

  Summary:
    Returns the slider state.

  Description:
    Returns the slider state.
 
  Precondition:
    None.

  Parameters:
    sliderNumber - slider number.

  Returns:
    THe slider state (see MTOUCHCONTROLSTATE union in mTouchControl.h).

  Remarks:
    None.
  ***************************************************************************/
#define MTouchGet2ChSliderState(sliderNumber)    (sliders2ch[sliderNumber].state)

/****************************************************************************
  Macro:
    MTouchGet2ChSliderValue(sliderNumber)

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
#define MTouchGet2ChSliderValue(sliderNumber)    (sliders2ch[sliderNumber].value)

#endif

