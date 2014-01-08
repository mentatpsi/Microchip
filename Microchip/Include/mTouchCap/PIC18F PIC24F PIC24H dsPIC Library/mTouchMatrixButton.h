/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                      
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouchMatrixButton.h
 *  Dependencies:    None.
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This header contains all definitions related to the
 *                   matrix button control.
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

#ifndef _MTOUCH_MATRIX_BUTTON_H_
#define _MTOUCH_MATRIX_BUTTON_H_

/****************************************************************************
  Structure:
     MTOUCHMATRIXBUTTON

  Description:
    Contains all parameters related to matrix button.
  ***************************************************************************/
typedef struct tag_MTOUCHMATRIXBUTTON {
UINT8   state;          // button state (see MTOUCHCONTROLSTATE union in mTouchControl.h)
UINT8   curState;       // current button state (used in calculations)
UINT8   decode;         // decode method (see MTOUCHCONTROLDECODE union in mTouchControl.h)
UINT8   prevSensorState;// previous state of the sensor
SENSOR* pCh1Sensor;     // pointer to first sensor
SENSOR* pCh2Sensor;     // pointer to second sensor
INT16   pressCounter;   // counter used to generate RRESS/RELEASE events (for DECODE_PRESS_REPEAT decode method)

}MTOUCHMATRIXBUTTON;

/****************************************************************************
  Structure:
     MTOUCHMATRIXBUTTON  matrixButtons[MTOUCH_MATRIXBUTTONS_NUMBER]

  Description:
    Contains information about all matrix buttons. Numbers of matrix buttons
    MTOUCH_MATRIXBUTTONS_NUMBER is defined in mTouchConfig.h.
  ***************************************************************************/
#if (MTOUCH_MATRIXBUTTONS_NUMBER > 0)
extern MTOUCHMATRIXBUTTON  matrixButtons[MTOUCH_MATRIXBUTTONS_NUMBER];
#endif

/****************************************************************************
  Function:
    void MTouchSetMatrixButton(UINT8 buttonNumber, UINT8 ch1SensorNumber, UINT8 ch2SensorNumber, UINT8 decode)

  Summary:
    Initializes matrix button.

  Description:
    Initializes matrix button.
 
  Precondition:
    None.

  Parameters:
    buttonNumber    - button number.
    ch1SensorNumber - first sensor number (row or column).
    ch2SensorNumber - second sensor number (row or column).
    decode          - ORed combination of decode method flags
                      (see MTOUCHCONTROLDECODE union in mTouchControl.h).

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void MTouchSetMatrixButton(UINT8 buttonNumber, UINT8 ch1SensorNumber, UINT8 ch2SensorNumber, UINT8 decode);

/****************************************************************************
  Function:
    void MTouchDecodeMatrixButtons(void)

  Summary:
    Decodes all matrix buttons.

  Description:
    Decodes all matrix buttons.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void MTouchDecodeMatrixButtons(void);

/****************************************************************************
  Macro:
    MTouchGetMatrixButtonState(buttonNumber)

  Summary:
    Returns the matrix button state.

  Description:
    Returns the matrix button state.
 
  Precondition:
    None.

  Parameters:
    buttonNumber - matrix button number.

  Returns:
    The matrix button state flags (see MTOUCHCONTROLSTATE union in mTouchControl.h).

  Remarks:
    None.
  ***************************************************************************/
#define MTouchGetMatrixButtonState(buttonNumber)    (matrixButtons[buttonNumber].state)

#endif

