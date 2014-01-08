/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        CapButtons.c
 *  Dependencies:    CapButtons.h
 *  Processor:       PIC24F
 *  Compiler:        MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This demo explains how to use mTouch library with
 *                   Graphics library.
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

#include "CapButtons.h"

volatile UINT8 leftLedState = 0;
volatile UINT8 rightLedState = 0;

void CapButtonsInit(void)
{
    // mTouch library initialization.
    MTouchInit();

    // Sensors initialization. All sensors must be initialized (see MTOUCH_SENSORS_NUMBER in mTouchConfig.h).
	// PLEASE READ "SENSOR OPTIMIZATION (DEBUG MODULE)" CHAPTER IN THE HELP FILE TO SELECT OPTIMAL PARAMETERS.
                  
    // sensor #0 connected to AN19/RG8
    MTouchSetSensor(0, &TRISG, &LATG, 8, 19, -1, -1, -1);
    // sensor #1 connected to AN21/RE9
    MTouchSetSensor(1, &TRISE, &LATE, 9, 21, -1, -1, -1);  
    // sensor #2 connected to AN5/RB5
    MTouchSetSensor(2, &TRISB, &LATB, 5, 5, -1, -1, -1);  

    // Buttons initialization. All buttons must be initialized (see MTOUCH_BUTTONS_NUMBER and
    // MTOUCH_MATRIXBUTTONS_NUMBER in mTouchConfig.h).

    // button #0 connected to sensor 1
    MTouchSetButton(CAP_BTN_UP,   1, DECODE_PRESS_RELEASE|DECODE_ONE_EVENT);
    // button #1 connected to sensor 2
    MTouchSetButton(CAP_BTN_DOWN, 2, DECODE_PRESS_RELEASE|DECODE_ONE_EVENT);

    // matrix button #0 connected to sensors 0 and 1
    MTouchSetMatrixButton(CAP_BTN_LEFT,   0, 1, DECODE_PRESS_REPEAT|DECODE_ONE_EVENT);
    // matrix button #1 connected to sensors 0 and 2
    MTouchSetMatrixButton(CAP_BTN_RIGHT,  0, 2, DECODE_PRESS_REPEAT|DECODE_ONE_EVENT);

}

void CapButtonsGetMsg(GOL_MSG* pMsg)
{
UINT8  btnState;

static UINT16 delayCounter = 0;

    MTouchDecodeMatrixButtons();
    
    pMsg->type = TYPE_KEYBOARD;
    pMsg->uiEvent = EVENT_INVALID; // invalid event
    pMsg->param1 = -1; // invalid ID


    btnState = MTouchGetMatrixButtonState(CAP_BTN_LEFT);

    if(btnState == CONTROL_PRESSED)
    {
        pMsg->uiEvent = EVENT_KEYSCAN;
        pMsg->param1 = ID_BTN1;
        pMsg->param2 = SCAN_CR_PRESSED;

        leftLedState = 1;

        return;
   
    }else{

        if(btnState == CONTROL_RELEASED)
        {
            pMsg->uiEvent = EVENT_KEYSCAN;
            pMsg->param1 = ID_BTN1;
            pMsg->param2 = SCAN_CR_RELEASED;

            leftLedState = 0;
            
            return;
        }
    }

    btnState = MTouchGetMatrixButtonState(CAP_BTN_RIGHT);

    if(btnState == CONTROL_PRESSED)
    {
        pMsg->uiEvent = EVENT_KEYSCAN;
        pMsg->param1 = ID_BTN2;
        pMsg->param2 = SCAN_CR_PRESSED;

        rightLedState = 1;

        return;
    }else{

        if(btnState == CONTROL_RELEASED)
        {
            pMsg->uiEvent = EVENT_KEYSCAN;
            pMsg->param1 = ID_BTN2;
            pMsg->param2 = SCAN_CR_RELEASED;

            rightLedState = 0;
            return;
        }
    }


    if(leftLedState || rightLedState)
    {
        delayCounter = 0;
        return;
    }

    delayCounter++;

    if(delayCounter < DIRECT_BUTTONS_DELAY)
    {
        return;
    }

    delayCounter = 0;

    MTouchDecodeButtons();

    btnState = MTouchGetButtonState(CAP_BTN_UP);

    if(btnState == CONTROL_PRESSED)
    {
        pMsg->uiEvent = EVENT_KEYSCAN;
        pMsg->param1 = ID_SLD1;
        pMsg->param2 = SCAN_RIGHT_PRESSED;
    }

    btnState = MTouchGetButtonState(CAP_BTN_DOWN);

    if(btnState == CONTROL_PRESSED)
    {
        pMsg->uiEvent = EVENT_KEYSCAN;
        pMsg->param1 = ID_SLD1;
        pMsg->param2 = SCAN_LEFT_PRESSED;
    }

}
