/*****************************************************************************
 *  
 * Module for Microchip Graphics Library
 * GOL Layer 
 * This template can be used to create additional controls
 *
 *****************************************************************************
 * FileName:        Template.c
 * Dependencies:    none 
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok     11/12/07	Version 1.0 release
 *****************************************************************************/
#ifdef USE_CUSTOM

#include "Graphics/Graphics.h"

/*********************************************************************
* Function: CUSTOM* CcCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, WORD state, GOL_SCHEME *pScheme)
*
* PreCondition: none
*
* Input: ID - user defined ID for the object
*        left, top, right, bottom - location of the left,top and 
*             right, bottom corners of the object
*        state - state of the object
*        pScheme - pointer to the color scheme and font used for the object
*
* Output: returns the pointer to the object created
*
* Overview: creates the object and initialize with the passed parameters and 
*           default settings
*
* Note: none
*
********************************************************************/
CUSTOM *CcCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, GOL_SCHEME *pScheme)
{
    CUSTOM  *pCc = NULL;

    pCc = GFX_malloc(sizeof(CUSTOM));
    if(pCc == NULL)
        return (pCc);

    pCc->ID = ID;           // unique id assigned for referencing
    pCc->pNxtObj = NULL;    // initialize pointer to NULL
    pCc->type = OBJ_CUSTOM; // set object type
    pCc->left = left;       // left,top corner
    pCc->top = top;
    pCc->right = right;     // right buttom corner
    pCc->bottom = bottom;
    pCc->state = state;     // set state
    pCc->draw = CcDraw;

    // Set the color scheme to be used
    if(pScheme == NULL)
        pCc->pGolScheme = _pDefaultGolScheme;
    else
        pCc->pGolScheme = (GOL_SCHEME *)pScheme;

    GOLAddObject((OBJ_HEADER *)pCc);

    return (pCc);
}

/*********************************************************************
* Function: WORD CcTranslateMsg(CUSTOM *pCc, GOL_MSG *pMsg)
*
* PreCondition: none
*
* Input: pMsg - pointer to the message struct containing the message from 
*               the user interface
*        pCc - the pointer to the object where the message will be tested
*               to check if the message will affect the object
*
* Output: returns the action that the object will be performing
*
* Overview: evaluates the message if the object will be affected by the 
*           message or not
*
* Note: THIS FUNCTION CALL SHOULD BE ADDED INTO GOLMsg() FUNCTION IN
*       GOL.C FILE
*
********************************************************************/
WORD CcTranslateMsg(CUSTOM *pCc, GOL_MSG *pMsg)
{

    // Check if disabled first
    if(GetState(pCc, CC_DISABLED))
        return (OBJ_MSG_INVALID);

    // TRANSLATE MESSAGE FOR THE INPUT DEVICES
    // TRANSLATED MESSAGES SHOULD BE ADDED INTO TRANS_MSG ENUMERATION IN GOL.H FILE
        #ifdef USE_TOUCHSCREEN
        #endif
        #ifdef USE_KEYBOARD
        #endif
        #ifdef USE_MOUSE
        #endif
    return (OBJ_MSG_INVALID);
}

/*********************************************************************
* Function: void CcMsgDefault(CUSTOM* pCc, GOL_MSG* pMsg)
*
* PreCondition: None
*
* Input: pMsg -  pointer to the message struct containing the message the user
*        pCc - the pointer to the object whose state will be modified 
*
* Output: none
*
* Side Effects: 
*
* Overview: changes the state of the object
*
* Note:  THIS FUNCTION CALL SHOULD BE ADDED INTO GOLMsg() FUNCTION IN
*        GOL.C FILE 
*
********************************************************************/
void CcMsgDefault(CUSTOM *pCc, GOL_MSG *pMsg)
{

    // IMPLEMENT DEFAULT ACTIONS FOR TRANSLATED MESSAGES HERE
}

/*********************************************************************
* Function: WORD CcDraw(CUSTOM *pCc)
*
* PreCondition: object must be created before this is called
*
* Input: pCc - pointer to struct CUSTOM with data
*
* Output: returns the status of the drawing
*		  0 - not complete
*         1 - done
*
* Overview: draws control
*
* Note: THIS FUNCTION CALL SHOULD BE ADDED INTO GOLDraw() FUNCTION IN
*       GOL.C FILE
*
********************************************************************/
WORD CcDraw(void *pObj)
{
    typedef enum
    {
        REMOVE,
        BOX_DRAW,
        RUN_DRAW
    } CC_DRAW_STATES;

    static CC_DRAW_STATES state = REMOVE;
    CUSTOM *pCc;

    pCc = (CUSTOM *)pObj;

    switch(state)
    {
        case REMOVE:
            if(GetState(pCc, CC_HIDE))
            {
                SetColor(pCc->pGolScheme->CommonBkColor);
                if(!Bar(pCc->left, pCc->top, pCc->right, pCc->bottom))
                    return (0);
                return (1);
            }

            state = BOX_DRAW;

        case BOX_DRAW:
            if(GetState(pCc, CC_DRAW))
            {
                GOLPanelDraw
                (
                    pCc->left,
                    pCc->top,
                    pCc->right,
                    pCc->bottom,
                    pCc->pGolScheme->Color0,
                    pCc->pGolScheme->EmbossDkColor,
                    pCc->pGolScheme->EmbossLtColor,
                    NULL,
                    GOL_EMBOSS_SIZE
                );
            }
            else
            {
                state = BAR_DRAW;
                goto bar_draw;
            }

            state = RUN_DRAW;

        case RUN_DRAW:
            if(!GOLPanelDrawTsk())
            {
                return (0);
            }

            // DRAWING IS DONE
            state = REMOVE;
            return (1);
    }
}

#endif // USE_CUSTOM
