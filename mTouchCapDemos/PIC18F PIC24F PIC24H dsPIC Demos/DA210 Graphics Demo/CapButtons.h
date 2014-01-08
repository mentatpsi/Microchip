/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        CapButtons.h
 *  Dependencies:    Graphics.h,mTouch.h,GraphicsCallbacks.h
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

#ifndef _CAP_BUTTONS_H_
#define _CAP_BUTTONS_H_

#include "Graphics/Graphics.h"
#include "mTouch.h"
#include "GraphicsCallbacks.h"

/****************************************************************************
These variables control LEDs for LEFT and RIGHT buttons.
  ***************************************************************************/
extern volatile UINT8 leftLedState;
extern volatile UINT8 rightLedState;

/****************************************************************************
Numbers of buttons.
  ***************************************************************************/
#define CAP_BTN_UP     0
#define CAP_BTN_DOWN   1

/****************************************************************************
Numbers of matrix buttons.
  ***************************************************************************/
#define CAP_BTN_LEFT   0
#define CAP_BTN_RIGHT  1

/****************************************************************************
This definition delays reaction of direct buttons. Thus press detection
for the matrix buttons has a high priotity.
  ***************************************************************************/
#define DIRECT_BUTTONS_DELAY   1000

/****************************************************************************
  Function:
    void CapButtonsInit(void);

  Summary:
    Initializes all capacitive touch buttons.

  Description:
    Initializes all capacitive touch buttons.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void CapButtonsInit(void);

/****************************************************************************
  Function:
    void CapButtonsGetMsg(GOL_MSG* pMsg)

  Summary:
    This function translates states of the buttons into Graphics library
    messages.

  Description:
    This function translates states of the buttons into Graphics library
    messages. Each button generates a pressed or released scan code message
    for the library.
 
  Precondition:
    None.

  Parameters:
    pMsg  - pointer to the Graphics library GOL message structure.

  Returns:
    The function fills a message structure for the Graphics library.

  Remarks:
    None.
  ***************************************************************************/
void CapButtonsGetMsg(GOL_MSG* pMsg);

#endif
