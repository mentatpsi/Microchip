/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                      
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouchControl.h
 *  Dependencies:    mTouchButton.h, mTouchMatrixButton.h, mTouch2ChSlider.h,
 *                   mTouch4ChSlider.h
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


#ifndef _MTOUCH_CONTROL_H_
#define _MTOUCH_CONTROL_H_

#include "mTouchButton.h"
#include "mTouchMatrixButton.h"
#include "mTouch2ChSlider.h"
#include "mTouch4ChSlider.h"

/****************************************************************************
  Enumeration:
    MTOUCHCONTROLSTATE

  Description:
    Defines possible state flags for controls:
        CONTROL_IDLE     - control is in idle state.
        CONTROL_PRESSED  - control pressed.
        CONTROL_RELEASED - control released.
  ***************************************************************************/
extern enum  _MTOUCHCONTROLSTATE_ 
{
    CONTROL_IDLE = 0x80,
    CONTROL_PRESSED = 0x01,
    CONTROL_RELEASED = 0x02
} MTOUCHCONTROLSTATE;

/****************************************************************************
  Enumeration:
    MTOUCHCONTROLDECODE

  Description:
    Defines possible decode method flags for controls:

       DECODE_TOGGLE         - Toggled button.
       DECODE_PRESS_RELEASE  - Simple button (reports pressed or released states).
       DECODE_MOST_PRESSED   - Looks through all pressed buttons having decode method
                               DECODE_MOST_PRESSED and reports "pressed" state only
                               for one which has a bigger signal.
       DECODE_PRESS_REPEAT   - If button is held for some time it starts to
                               generate "pressed"/"released" events pereodically.
                               See MCONTROL_REPEAT_INITIAL_DELAY and
                               MCONTROL_REPEAT_DELAY settings in mTouchConfig.h
       DECODE_ONE_EVENT      - If control's state is not changed it will set
                               CONTROL_IDLE state flag.
     
  ***************************************************************************/
extern enum  _MTOUCHCONTROLDECODE_ 
{
    DECODE_TOGGLE = 0x01,
    DECODE_PRESS_RELEASE = 0x02,
    DECODE_MOST_PRESSED = 0x04,
    DECODE_PRESS_REPEAT = 0x08,
    DECODE_ONE_EVENT = 0x10
} MTOUCHCONTROLDECODE;

/****************************************************************************
  Function:
    void mTouchDecode(void)

  Summary:
    Decodes states for all controls.

  Description:
    Decodes states for all controls.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    Should be called pereodically before reading of the controls states.
  ***************************************************************************/
void MTouchDecode(void);


#endif
