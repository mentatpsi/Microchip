/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        main_screen.c
 * Dependencies:    See Includes Section
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
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
 *****************************************************************************/

/*****************************************************************************
 * SECTION: Includes
 *****************************************************************************/
#include "Graphics/Graphics.h"
#include "gfx_schemes.h"
#include "gfx_strings.h"
#include "gfx_screens.h"
#include "gol_ids.h"
#include "MainDemo.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define SECURITY_EB_ORIGIN_X    ((GetMaxX() - 212 + 1) / 2)-30
#define SECURITY_EB_ORIGIN_Y    (100)+50
#define SECURITY_KEYSTARTX      SECURITY_EB_ORIGIN_X
#define SECURITY_KEYSTARTY      SECURITY_EB_ORIGIN_Y
#define SECURITY_KEYSIZEX       73
#define SECURITY_KEYSIZEY       59
#define SECURITY_BUTTON_ORIGIN  (SECURITY_KEYSTARTX + (3 * SECURITY_KEYSIZEX)+35) 
/*****************************************************************************
 * SECTION: Variables
 *****************************************************************************/
static XCHAR    padNumbers[][2] =  
{   
    {'0',0},
    {'1',0},
    {'2',0},
    {'3',0},
    {'4',0},
    {'5',0},
    {'6',0},
    {'7',0},
    {'8',0},
    {'9',0}
};

/*****************************************************************************
 * void CreateSecurityScreen(void)
 *****************************************************************************/
void CreateSecurityScreen(void)
{
    GOL_SCHEME *currentScheme;

    currentScheme = GFX_SchemeGetCurrentScheme();

    CreatePanelScreen();                    // This adds the widgets seen on the left of the screen

    SetState(GOLFindObject(PANEL_SCREEN_ID_SECURITY_BUT), BTN_DISABLED);

    if(GetState(GOLFindObject(PANEL_SCREEN_ID_COMFORT_BUT),BTN_DISABLED))
    {
        ClrState(GOLFindObject(PANEL_SCREEN_ID_COMFORT_BUT), BTN_DISABLED);
        SetState(GOLFindObject(PANEL_SCREEN_ID_COMFORT_BUT),BTN_DRAW);
    }

    if(GetState(GOLFindObject(PANEL_SCREEN_ID_LIGHTING_BUT),BTN_DISABLED))
    {
        ClrState(GOLFindObject(PANEL_SCREEN_ID_LIGHTING_BUT), BTN_DISABLED);
        SetState(GOLFindObject(PANEL_SCREEN_ID_LIGHTING_BUT),BTN_DRAW);
    }

    if(GetState(GOLFindObject(PANEL_SCREEN_ID_ENERGY_USAGE_BUT),BTN_DISABLED))
    { 
        ClrState(GOLFindObject(PANEL_SCREEN_ID_ENERGY_USAGE_BUT), BTN_DISABLED);
        SetState(GOLFindObject(PANEL_SCREEN_ID_ENERGY_USAGE_BUT),BTN_DRAW);
    }


     SetFont((void *) &FONTDEFAULT);
     SetColor(WHITE);
     OutTextXY(SECURITY_EB_ORIGIN_X + 70,SECURITY_EB_ORIGIN_Y -25, (XCHAR *)EnterPINStr);
     OutTextXY(SECURITY_BUTTON_ORIGIN,SECURITY_EB_ORIGIN_Y -25, (XCHAR *)SecurityOptionsStr);

    BtnCreate    //Disarmed
    (
        SERCURITY_SCREEN_ID_DISARMED_BUT,                         // button ID
        SECURITY_BUTTON_ORIGIN,
        SECURITY_EB_ORIGIN_Y,
        SECURITY_BUTTON_ORIGIN + (SECURITY_KEYSIZEX << 1),
        SECURITY_EB_ORIGIN_Y + 1 * SECURITY_KEYSIZEY,                                // dimension
        5,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)DisarmedStr,                 // "Button",     	// text
        currentScheme
    ); 

    BtnCreate    //Armed
    (
        SERCURITY_SCREEN_ID_ARMED_BUT,                         // button ID
        SECURITY_BUTTON_ORIGIN,
        SECURITY_EB_ORIGIN_Y + 1 * SECURITY_KEYSIZEY + 3,
        SECURITY_BUTTON_ORIGIN + (SECURITY_KEYSIZEX << 1),
        SECURITY_EB_ORIGIN_Y + 2 * SECURITY_KEYSIZEY,                                // dimension
        5,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)ArmedStr,                 // "Button",     	// text
        currentScheme
    ); 
 
    BtnCreate         //Sleeping
    (
        SERCURITY_SCREEN_ID_SLEEPING_BUT,                         // button ID
        SECURITY_BUTTON_ORIGIN,
        SECURITY_EB_ORIGIN_Y + 2 * SECURITY_KEYSIZEY +3,
        SECURITY_BUTTON_ORIGIN + (SECURITY_KEYSIZEX << 1),
        SECURITY_EB_ORIGIN_Y + 3 * SECURITY_KEYSIZEY,                                // dimension
        5,                                 // set radius
        BTN_DRAW,               // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)SleepingStr,                 // "Button",     	// text
        currentScheme
    );  

    BtnCreate         //Vacation
    (
        SERCURITY_SCREEN_ID_VACATION_BUT,                         // button ID
        SECURITY_BUTTON_ORIGIN,
        SECURITY_EB_ORIGIN_Y + 3 * SECURITY_KEYSIZEY +3,
        SECURITY_BUTTON_ORIGIN + (SECURITY_KEYSIZEX << 1),
        SECURITY_EB_ORIGIN_Y + 4 * SECURITY_KEYSIZEY,                                // dimension
        5,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)VacationStr,                 // "Button",     	// text
        currentScheme
    ); 
 
    BtnCreate       //Entertaining
    (
        SERCURITY_SCREEN_ID_ENTERTAINING_BUT,                         // button ID
        SECURITY_BUTTON_ORIGIN,
        SECURITY_EB_ORIGIN_Y + 4 * SECURITY_KEYSIZEY +3,
        SECURITY_BUTTON_ORIGIN + (SECURITY_KEYSIZEX << 1),
        SECURITY_EB_ORIGIN_Y + 5 * SECURITY_KEYSIZEY,                                // dimension
        5,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)EntertainingStr,                 // "Button",     	// text
        currentScheme
    ); 

    EbCreate
    (
        SERCURITY_SCREEN_ID_CODE_EDIT_BOX,    // ID
        SECURITY_KEYSTARTX+5,
        SECURITY_KEYSTARTY + 1,
        SECURITY_KEYSTARTX + (3 * SECURITY_KEYSIZEX)-5,
        SECURITY_KEYSTARTY + 1 * SECURITY_KEYSIZEY - GOL_EMBOSS_SIZE, // dimension
        EB_DRAW | EB_CARET | EB_FOCUSED,            // will be dislayed after creation
        NULL,
        17,
        currentScheme
    );                              // default GOL scheme


    BtnCreate
    (
        SERCURITY_SCREEN_ID_KEYPAD_0_BUT,              // ID
        SECURITY_KEYSTARTX + 1 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 4 * SECURITY_KEYSIZEY,
        SECURITY_KEYSTARTX + 2 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 5 * SECURITY_KEYSIZEY,   // dimension
        5,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        padNumbers[0],              // text
        currentScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        SERCURITY_SCREEN_ID_KEYPAD_1_BUT,              // ID
        SECURITY_KEYSTARTX + 0 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 1 * SECURITY_KEYSIZEY,
        SECURITY_KEYSTARTX + 1 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 2 * SECURITY_KEYSIZEY,   // dimension
        5,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        padNumbers[1],              // text
       currentScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        SERCURITY_SCREEN_ID_KEYPAD_2_BUT,              // ID
        SECURITY_KEYSTARTX + 1 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 1 * SECURITY_KEYSIZEY,
        SECURITY_KEYSTARTX + 2 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 2 * SECURITY_KEYSIZEY,   // dimension
        5,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        padNumbers[2],              // text
        currentScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        SERCURITY_SCREEN_ID_KEYPAD_3_BUT,              // ID
        SECURITY_KEYSTARTX + 2 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 1 * SECURITY_KEYSIZEY,
        SECURITY_KEYSTARTX + 3 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 2 * SECURITY_KEYSIZEY,   // dimension
        5,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        padNumbers[3],              // text
        currentScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        SERCURITY_SCREEN_ID_KEYPAD_4_BUT,              // ID
        SECURITY_KEYSTARTX + 0 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 2 * SECURITY_KEYSIZEY,
        SECURITY_KEYSTARTX + 1 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 3 * SECURITY_KEYSIZEY,   // dimension
        5,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        padNumbers[4],              // text
        currentScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        SERCURITY_SCREEN_ID_KEYPAD_5_BUT,              // ID
        SECURITY_KEYSTARTX + 1 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 2 * SECURITY_KEYSIZEY,
        SECURITY_KEYSTARTX + 2 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 3 * SECURITY_KEYSIZEY,   // dimension
        5,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        padNumbers[5],              // text
        currentScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        SERCURITY_SCREEN_ID_KEYPAD_6_BUT,              // ID
        SECURITY_KEYSTARTX + 2 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 2 * SECURITY_KEYSIZEY,
        SECURITY_KEYSTARTX + 3 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 3 * SECURITY_KEYSIZEY,   // dimension
        5,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        padNumbers[6],              // text
        currentScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        SERCURITY_SCREEN_ID_KEYPAD_7_BUT,              // ID
        SECURITY_KEYSTARTX + 0 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 3 * SECURITY_KEYSIZEY,
        SECURITY_KEYSTARTX + 1 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 4 * SECURITY_KEYSIZEY,   // dimension
        5,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        padNumbers[7],              // text
        currentScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        SERCURITY_SCREEN_ID_KEYPAD_8_BUT,              // ID
        SECURITY_KEYSTARTX + 1 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 3 * SECURITY_KEYSIZEY,
        SECURITY_KEYSTARTX + 2 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 4 * SECURITY_KEYSIZEY,   // dimension
        5,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        padNumbers[8],              // text
        currentScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        SERCURITY_SCREEN_ID_KEYPAD_9_BUT,              // ID
        SECURITY_KEYSTARTX + 2 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 3 * SECURITY_KEYSIZEY,
        SECURITY_KEYSTARTX + 3 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 4 * SECURITY_KEYSIZEY,   // dimension
        5,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        padNumbers[9],              // text
        currentScheme
    );                              // alternative GOL scheme

    BtnCreate
    (
        SERCURITY_SCREEN_ID_CLEAR_BUT,                // ID
        SECURITY_KEYSTARTX + 0 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 4 * SECURITY_KEYSIZEY,
        SECURITY_KEYSTARTX + 1 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 5 * SECURITY_KEYSIZEY,   // dimension
        5,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        (XCHAR *)ClearStr,             // text
        currentScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        SERCURITY_SCREEN_ID_ENTER_BUT,                   // ID
        SECURITY_KEYSTARTX + 2 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 4 * SECURITY_KEYSIZEY,
        SECURITY_KEYSTARTX + 3 * SECURITY_KEYSIZEX,
        SECURITY_KEYSTARTY + 5 * SECURITY_KEYSIZEY,   // dimension
        5,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        (XCHAR *)EnterStr,             // text
        currentScheme
    );                              // alternative GOL scheme

  
}
/*****************************************************************************
 * WORD MsgSecurityScreen(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
 *****************************************************************************/
WORD MsgSecurityScreen(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    EDITBOX     *pEb;
    SHORT       id;
    XCHAR       temp;
    static char status = 0; // status to check if calling, holding or not
    id = GetObjID(pObj);

    MsgPanelScreen(objMsg, pObj);  //get messages from the panel also

    // If number key is pressed
    if(objMsg == BTN_MSG_PRESSED)
    {
        if(id >= SERCURITY_SCREEN_ID_KEYPAD_0_BUT)
        {
            if(id <= SERCURITY_SCREEN_ID_KEYPAD_9_BUT)
            {
                if(!status)
                {
                    pEb = (EDITBOX *)GOLFindObject(SERCURITY_SCREEN_ID_CODE_EDIT_BOX);
                    EbAddChar(pEb, '*');
                    SetState(pEb, EB_DRAW);
                }

                return (1);
            }
        }
    }

    switch(id)
    {

        case SERCURITY_SCREEN_ID_CLEAR_BUT:
        case SERCURITY_SCREEN_ID_ENTER_BUT:
            if(!status)
            {
                if(objMsg == BTN_MSG_PRESSED)
                {
                    pEb = (EDITBOX *)GOLFindObject(SERCURITY_SCREEN_ID_CODE_EDIT_BOX);
                    temp = 0x0000;
                    EbSetText(pEb, &temp);
                    SetState(pEb, EB_DRAW);
                    status = 0;
                }
            }

            return (1);

        default:
            return (0);                         // process by default
    }
}
