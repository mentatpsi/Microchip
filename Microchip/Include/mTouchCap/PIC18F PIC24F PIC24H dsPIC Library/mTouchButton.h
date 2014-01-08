/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                      
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouchButton.h
 *  Dependencies:    None.
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This header contains all definitions related to the
 *                   button control.
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

#ifndef _MTOUCH_BUTTON_H_
#define _MTOUCH_BUTTON_H_

/****************************************************************************
  Structure:
     MTOUCHBUTTON

  Description:
    Contains all parameters related to button.
  ***************************************************************************/
typedef struct tag_MTOUCHBUTTON {
UINT8   state;          // button state (see MTOUCHCONTROLSTATE union in mTouchControl.h)
UINT8   curState;       // current button state (used in calculations)
UINT8   decode;         // decode method (see MTOUCHCONTROLDECODE union in mTouchControl.h)
UINT8   prevSensorState;// previous state of the sensor
SENSOR* pSensor;        // pointer to sensor
INT16   pressCounter;   // counter used to generate RRESS/RELEASE events (for DECODE_PRESS_REPEAT decode method)
}MTOUCHBUTTON;

/****************************************************************************
  Structure:
     MTOUCHBUTTON

  Description:
    Contains information about all buttons. Numbers of buttons
    MTOUCH_BUTTONS_NUMBER is defined in mTouchConfig.h.
  ***************************************************************************/
#if (MTOUCH_BUTTONS_NUMBER > 0)
extern MTOUCHBUTTON  buttons[MTOUCH_BUTTONS_NUMBER];
#endif

/****************************************************************************
  Function:
    void MTouchSetButton(UINT8 buttonNumber, UINT8 sensorNumber, UINT8 decode)

  Summary:
    Initializes button.

  Description:
    Initializes button.
 
  Precondition:
    None.

  Parameters:
    buttonNumber    - button number.
    sensorNumber    - sensor number.
    decode          - ORed combination of decode method flags
                      (see MTOUCHCONTROLDECODE union in mTouchControl.h).

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void MTouchSetButton(UINT8 buttonNumber, UINT8 sensorNumber, UINT8 decode);

/****************************************************************************
  Function:
    void MTouchDecodeButtons(void)

  Summary:
    Decodes all buttons.

  Description:
    Decodes all buttons.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void MTouchDecodeButtons(void);

/****************************************************************************
  Macro:
    MTouchGetButtonState(buttonNumber)

  Summary:
    Returns the button state.

  Description:
    Returns the button state.
 
  Precondition:
    None.

  Parameters:
    buttonNumber - button number.

  Returns:
    The button state flags (see MTOUCHCONTROLSTATE union in mTouchControl.h).

  Remarks:
    None.
  ***************************************************************************/
#define MTouchGetButtonState(buttonNumber)    (buttons[buttonNumber].state)

#endif

