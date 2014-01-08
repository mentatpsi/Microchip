/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use a keyboard.
 *****************************************************************************
 * FileName:        AN1227Demo.c
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 07/30/08	    ...
 * 03/18/11     Ported to common Graphics Application Note Demo
 *****************************************************************************/
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "Graphics/Graphics.h"
#include "AN1227Demo.h"

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT'S IDs
/////////////////////////////////////////////////////////////////////////////
#define BUTTON1_ID  1111                // button unique ID
#define BUTTON2_ID  2222                // button unique ID
#define BUTTON3_ID  3333                // button unique ID
    
#define BUTTON_WIDTH        (100)
#define BTN_X_START         ((GetMaxX() - BUTTON_WIDTH) >> 1)
#define BTN_X_END           (BTN_X_START + BUTTON_WIDTH)
    
const XCHAR  ReleaseStr[] = {'R','e','l','e','a','s','e','d',0};
const XCHAR  PressStr[] = {'P','r','e','s','s','e','d',0};
const XCHAR  Button2Str[] = {'B','u','t','t','o','n',' ','2',0};
const XCHAR  DisabledStr[] = {'D','i','s','a','b','l','e','d',0};

    // message for the button presses
    GOL_MSG AN1227msg;                        
    
    // filled by the keyboard driver and
    // passed to the message manager
    OBJ_HEADER * pFocusedObj;           // temporary variable for the widget

    // receiving the keyboard focus
    BYTE previousKey1State;             // previous state of the button 1
    BYTE previousKey2State;             // previous state of the button 2

   
/////////////////////////////////////////////////////////////////////////////
// Function: WORD CreateAN1227(void)
// Input: None
// Output: Returns non-zero if the objects are created 
//         returns 0 when one of the objects was not created.
// Overview: Creates the AN1227 Demo screen.
/////////////////////////////////////////////////////////////////////////////
WORD CreateAN1227(void)
{

    SetColor(BLACK);
    ClearDevice();

    // HardwareButtonInit() to initialize the hardware buttons
    // should be called prior to the call to this function.
    
    /**
     * Carriage Return button
     * Explorer 16 + GFX PICTail    - S3 (8 bit PMP)
     * Explorer 16 + GFX PICTail    - S5 (16 bit PMP)
     * Starter Kit + GFX PICTail    - S0 (8 bit PMP)
     * Multimedia Expansion Board   - Fire Button
     * DA210 Developement Board     - S1
     * NOTE:    Starter Kit + GFX PICTail will switches are shared
     *          with the 16 bit PMP data bus.
     **/
    previousKey1State = GetHWButtonCR();  // previous state equals the current state
    /**
     * Change Focus button
     * Explorer 16 + GFX PICTail    - S6 (8 bit PMP)
     * Starter Kit + GFX PICTail    - S1 or S2 (8 bit PMP)
     * Multimedia Expansion Board   - left or right
     * DA210 Developement Board     - S2 or S3
     **/
    previousKey2State = GetHWButtonFocus(); // previous state equals the current state

    // create button widgets
    if (!BtnCreate (
        BUTTON1_ID,             // button unique ID
        BTN_X_START, 40,        // left, top corner coordinates
        BTN_X_END, 90,          // right, bottom corner
        0,                      // corner radius is zero, it’s a square button
        BTN_DRAW,               // will be dislayed after creation
        NULL,                   // no bitmap
        (XCHAR*)ReleaseStr,     // text for released state
        NULL                    // default color scheme is used
    ))
        return 0;

    if (!BtnCreate (
        BUTTON2_ID,             // button unique ID
        BTN_X_START, 100,       // left, top corner coordinates
        BTN_X_END, 150,         // right, bottom corner
        0,                      // corner radius is zero, it’s a square button
        BTN_DRAW,               // will be dislayed after creation
        NULL,                   // no bitmap
        (XCHAR*)Button2Str,     // text
        NULL                    // default color scheme is used
    ))
        return 0;

    if (!BtnCreate (
        BUTTON3_ID,             // button unique ID
        BTN_X_START, 160,       // left, top corner coordinates
        BTN_X_END, 210,         // right, bottom corner
        0,                      // corner radius is zero, it’s a square button
        BTN_DRAW |
        BTN_DISABLED,           // will be dislayed and disabled after creation
        NULL,                   // no bitmap
        (XCHAR*)DisabledStr,    // text
        NULL                    // default color scheme is used
    ))
        return 0;

    pFocusedObj = NULL;                 // there are no widgets in focus
    
    return (1);
}       

/////////////////////////////////////////////////////////////////////////////
// Function: WORD MsgAN1227Callback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
// Input: objMsg - translated message for the object,
//        pObj - pointer to the object,
//        pMsg - pointer to the non-translated, raw GOL message
// Output: if the function returns non-zero the message will be processed 
//         by default
// Overview: This is called by GOLMsgCallback() to determine if 
//           a valid message for objects in this demo needs to be processed.
/////////////////////////////////////////////////////////////////////////////
WORD MsgAN1227Callback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    // Application should process messages here
    if(GetObjID(pObj) == BUTTON1_ID)                // if the button with BUTTON_ID is receiving the message
    {
        if(objMsg == BTN_MSG_PRESSED)
        {
            BtnSetText((BUTTON *)pObj, (XCHAR*)PressStr);  // set text for pressed state
        }

        if(objMsg == BTN_MSG_RELEASED)
        {
            BtnSetText((BUTTON *)pObj, (XCHAR*)ReleaseStr); // set text for released state
        }
    }

    return (1); // process the message by default
}               // end of GOLMessageCallback

/////////////////////////////////////////////////////////////////////////////
// Function: WORD DrawAN1227Callback()
// Output: if the function returns non-zero the draw control will be passed to GOL
// Overview: GOLDrawCallback() calls this function for demo specific drawing 
//           requirements.
/////////////////////////////////////////////////////////////////////////////
WORD DrawAN1227Callback(void)
{
    return (1); // pass drawing control to the library				
}               // end of GOLDrawCallback

void AN1227ProcessKeyBoard(void)
{
            
    // Keyboard driver
    if(GOLGetFocus() != NULL)   // if there’s a widget in focus send a message
    {

        // check if the button has changed its state
        if(GetHWButtonCR() != previousKey1State)
        {
            if(previousKey1State)
            {

                // if GetHWButtonCR equals zero it means the button is pressed
                AN1227msg.type = TYPE_KEYBOARD;
                AN1227msg.uiEvent = EVENT_KEYSCAN;

                // the focused button will receive the message
                AN1227msg.param1 = GetObjID(GOLGetFocus());
                AN1227msg.param2 = SCAN_CR_PRESSED;

            }
            else
            {

                // if GetHWButtonCR equals one it means the button is released
                AN1227msg.type = TYPE_KEYBOARD;
                AN1227msg.uiEvent = EVENT_KEYSCAN;

                // the focused button will receive the message
                AN1227msg.param1 = GetObjID(GOLGetFocus());
                AN1227msg.param2 = SCAN_CR_RELEASED;
            }                   // end of else

            // state of the button was changed
            previousKey1State = !previousKey1State;

            // pass the message to the graphics library
            GOLMsg(&AN1227msg);
        }                       // end of if
    }                           // end of if

    // check if the button has changed its state
    if(GetHWButtonFocus() != previousKey2State)
    {
        if(previousKey2State)
        {

            // if GetHWButtonFocus() equals zero it means the button is pressed
            // get the object can be focused next
            pFocusedObj = GOLGetFocusNext();

            // move focus
            GOLSetFocus(pFocusedObj);
        }
        else
        {

            // if GetHWButtonFocus()equals one it means the button is released
        }   // end of else

        // state of the button was changed
        previousKey2State = !previousKey2State;

        // pass the message to the graphics library
        GOLMsg(&AN1227msg);
    } // end of if
}         

