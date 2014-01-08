/****************************************************************************
 * FileName:        AlphaBlendScreen.c
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
 *****************************************************************************/
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "Graphics/Graphics.h"
#include "AlphaBlendScreen.h"
#include "JPEGImage.h"
#include "InternalResources.h"

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT'S IDs
/////////////////////////////////////////////////////////////////////////////
#define BUTTON1_ID  1111                // button unique ID
#define BUTTON2_ID  2222                // button unique ID
#define BUTTON3_ID  3333                // button unique ID
    
#define BUTTON_WIDTH        (150)
#define BTN_X_START         ((GetMaxX() - BUTTON_WIDTH) >> 1)
#define BTN_X_END           (BTN_X_START + BUTTON_WIDTH)
    
const XCHAR  Button1Str[] = {'B','u','t','t','o','n',' ','1',0};
const XCHAR  Button2Str[] = {'B','u','t','t','o','n',' ','2',0};
const XCHAR  Button3Str[] = {'B','u','t','t','o','n',' ','3',0};

extern GOL_SCHEME      *AlphaScheme;                // alternative style scheme

    // filled by the keyboard driver and
    // passed to the message manager
    OBJ_HEADER * pFocusedObj;           // temporary variable for the widget

    // receiving the keyboard focus
    BYTE previousKey1State;             // previous state of the button 1
    BYTE previousKey2State;             // previous state of the button 2

   
/////////////////////////////////////////////////////////////////////////////
// Function: WORD CreateAlphaBlendScreen(void)
// Input: None
// Output: Returns non-zero if the objects are created 
//         returns 0 when one of the objects was not created.
// Overview: Creates the AlphaBlend Demo screen.
/////////////////////////////////////////////////////////////////////////////
WORD CreateAlphaBlendScreen(void)
{

            ImageDecode                            
        (                                      
            JPEGfopen((void *)&Demo),         
            IMG_JPEG,                          
            0,                              
            0,                               
            (GetMaxX()),                   
            (GetMaxY()),                   
            0,                                 
            &_jpegFileApi,                     
            JPEG_PixelOutput                   
        );

    // create button widgets
    BtnCreate (
        BUTTON1_ID,             // button unique ID
        BTN_X_START, 40,        // left, top corner coordinates
        BTN_X_END, 90,          // right, bottom corner
        0,                      // corner radius is zero, it’s a square button
        BTN_DRAW,               // will be dislayed after creation
        NULL,                   // no bitmap
        (XCHAR*)Button1Str,     // text for released state
        AlphaScheme                    // default color scheme is used
              );

    BtnCreate (
        BUTTON2_ID,             // button unique ID
        BTN_X_START, 100,       // left, top corner coordinates
        BTN_X_END, 150,         // right, bottom corner
        0,                      // corner radius is zero, it’s a square button
        BTN_DRAW,               // will be dislayed after creation
        NULL,                   // no bitmap
        (XCHAR*)Button2Str,     // text
        AlphaScheme                     // default color scheme is used
               );

    BtnCreate (
        BUTTON3_ID,             // button unique ID
        BTN_X_START, 160,       // left, top corner coordinates
        BTN_X_END, 210,         // right, bottom corner
        0,                      // corner radius is zero, it’s a square button
        BTN_DRAW |
        BTN_DRAW,           // will be dislayed and disabled after creation
        NULL,                   // no bitmap
        (XCHAR*)Button3Str,    // text
        AlphaScheme                     // default color scheme is used
              );
    
    return (1);
}       

/////////////////////////////////////////////////////////////////////////////
// Function: WORD MsgAlphaBlendScreenCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
// Input: objMsg - translated message for the object,
//        pObj - pointer to the object,
//        pMsg - pointer to the non-translated, raw GOL message
// Output: if the function returns non-zero the message will be processed 
//         by default
// Overview: This is called by GOLMsgCallback() to determine if 
//           a valid message for objects in this demo needs to be processed.
/////////////////////////////////////////////////////////////////////////////
WORD MsgAlphaBlendScreenCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    return (1); // process the message by default
}               // end of GOLMessageCallback   
