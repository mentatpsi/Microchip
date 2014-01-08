/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Radio Button
 *****************************************************************************
 * FileName:        RadioButton.c
 * Dependencies:    Graphics.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07	    Version 1.0 release
 * 04/20/11     Fixed KEYBOARD bug on object ID and GOL_MSG param1 comparison.
 * 07/29/11     Fixed state transition when rendering the text.
 *****************************************************************************/
#include "Graphics/Graphics.h"

#ifdef USE_RADIOBUTTON

// This pointer is used to create linked list of radio buttons for the group
RADIOBUTTON *_pListButtons = NULL;

/*********************************************************************
* Function: RADIOBUTTON  *RbCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, WORD state, XCHAR *pText, GOL_SCHEME *pScheme)
*
* Overview: creates the radio button
*
********************************************************************/
RADIOBUTTON *RbCreate
(
    WORD        ID,
    SHORT       left,
    SHORT       top,
    SHORT       right,
    SHORT       bottom,
    WORD        state,
    XCHAR       *pText,
    GOL_SCHEME  *pScheme
)
{
    RADIOBUTTON *pRb = NULL;
    RADIOBUTTON *pointer;

    pRb = (RADIOBUTTON *)GFX_malloc(sizeof(RADIOBUTTON));
    if(pRb == NULL)
        return (pRb);

    pRb->hdr.ID = ID;
    pRb->hdr.pNxtObj = NULL;
    pRb->hdr.type = OBJ_RADIOBUTTON;
    pRb->hdr.left = left;
    pRb->hdr.top = top;
    pRb->hdr.right = right;
    pRb->hdr.bottom = bottom;
    pRb->pText = pText;
    pRb->pNext = NULL;						// last radio button in the list
    pRb->hdr.state = state;
    pRb->hdr.DrawObj = RbDraw;				// draw function
    pRb->hdr.MsgObj = RbTranslateMsg;       // message function
    pRb->hdr.MsgDefaultObj = RbMsgDefault;  // default message function
    pRb->hdr.FreeObj = NULL;				// free function

    if(GetState(pRb, RB_GROUP))
    {

        // If it's first button in the group start new button's list
        _pListButtons = pRb;

        // Attach the button to the list
        pRb->pHead = (OBJ_HEADER *)_pListButtons;
    }
    else
    {

        // Attach the button to the list
        pRb->pHead = (OBJ_HEADER *)_pListButtons;
        pointer = _pListButtons;
        while(pointer->pNext != NULL)
        {
            pointer = (RADIOBUTTON *)pointer->pNext;
        }

        pointer->pNext = (OBJ_HEADER *)pRb;
    }

    // Set the style scheme to be used
    if(pScheme == NULL)
        pRb->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pRb->hdr.pGolScheme = pScheme;

    // Set the text height
    pRb->textHeight = 0;
    if(pText != NULL)
    {
        pRb->textHeight = GetTextHeight(pRb->hdr.pGolScheme->pFont);
    }

    GOLAddObject((OBJ_HEADER *)pRb);

    return (pRb);
}

/*********************************************************************
* Function: void RbSetCheck(RADIOBUTTON *pRb, WORD ID)
*
* Input: pRb - the pointer to any radio button in the group
*        ID - ID of button to be checked
*
* Output: none
*
* Overview: sets radio button to checked state, and marks states of group
*           radio buttons to be redrawn
*
********************************************************************/
void RbSetCheck(RADIOBUTTON *pRb, WORD ID)
{
    RADIOBUTTON *pointer;

    pointer = (RADIOBUTTON *)pRb->pHead;

    while(pointer != NULL)
    {
        if(pointer->hdr.ID == ID)
        {
            SetState(pointer, RB_CHECKED | RB_DRAW_CHECK);  // set check and redraw
        }
        else
        {
            ClrState(pointer, RB_CHECKED);                  // reset checked
            SetState(pointer, RB_DRAW_CHECK);               // redraw
        }

        pointer = (RADIOBUTTON *)pointer->pNext;
    }
}

/*********************************************************************
* Function: WORD RbGetCheck(RADIOBUTTON *pRb)
*
* Input: pRb - the pointer to any radio button in the group
*
* Output: ID of checked button, -1 if there are no checked buttons
*
* Overview: gets ID of checked radio button
*
********************************************************************/
WORD RbGetCheck(RADIOBUTTON *pRb)
{
    RADIOBUTTON *pointer;

    pointer = (RADIOBUTTON *)pRb->pHead;

    while(pointer != NULL)
    {
        if(GetState(pointer, RB_CHECKED))
        {
            return (pointer->hdr.ID);
        }

        pointer = (RADIOBUTTON *)pointer->pNext;
    }

    return (-1);
}

/*********************************************************************
* Function: RbSetText(RADIOBUTTON *pRb, XCHAR *pText)
*
* Input: pRb - the pointer to the radio button
*        pText - pointer to the text
*
* Output: none
*
* Overview: sets text
*
********************************************************************/
void RbSetText(RADIOBUTTON *pRb, XCHAR *pText)
{
    pRb->pText = pText;
    pRb->textHeight = GetTextHeight((BYTE *)pRb->hdr.pGolScheme->pFont);
}

/*********************************************************************
* Function: RbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
* Overview: changes the state of the radio button by default
*
********************************************************************/
void RbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
{
    RADIOBUTTON *pointer;
    RADIOBUTTON *pRb;

    pRb = (RADIOBUTTON *)pObj;

        #ifdef USE_FOCUS
            #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {
        if(!GetState(pRb, RB_FOCUSED))
        {
            GOLSetFocus((OBJ_HEADER *)pRb);
        }
    }

            #endif
        #endif
    if(translatedMsg == RB_MSG_CHECKED)
    {

        // Uncheck radio buttons in the group
        pointer = (RADIOBUTTON *)pRb->pHead;

        while(pointer != NULL)
        {
            if(GetState(pointer, RB_CHECKED))
            {
                ClrState(pointer, RB_CHECKED);      // reset check
                SetState(pointer, RB_DRAW_CHECK);   // redraw
            }

            pointer = (RADIOBUTTON *)pointer->pNext;
        }

        SetState(pRb, RB_CHECKED | RB_DRAW_CHECK);  // set check and redraw
    }
}

/*********************************************************************
* Function: WORD RbTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: translates the GOL message for the radio button
*
********************************************************************/
WORD RbTranslateMsg(void *pObj, GOL_MSG *pMsg)
{

    RADIOBUTTON *pRb;

    pRb = (RADIOBUTTON *)pObj;

    // Evaluate if the message is for the radio button
    // Check if disabled first
    if(GetState(pRb, RB_DISABLED))
        return (OBJ_MSG_INVALID);

        #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {
        if(pMsg->uiEvent == EVENT_PRESS)
        {

            // Check if it falls in the radio button borders
            if
            (
                (pRb->hdr.left < pMsg->param1) &&
                (pRb->hdr.right > pMsg->param1) &&
                (pRb->hdr.top < pMsg->param2) &&
                (pRb->hdr.bottom > pMsg->param2)
            )
            {
                if(!GetState(pRb, RB_CHECKED))
                    return (RB_MSG_CHECKED);
            }
        }

        return (OBJ_MSG_INVALID);
    }

        #endif
        #ifdef USE_KEYBOARD
    if(pMsg->type == TYPE_KEYBOARD)
    {
        if((WORD)pMsg->param1 == pRb->hdr.ID)
        {
            if(pMsg->uiEvent == EVENT_KEYSCAN)
            {
                if((pMsg->param2 == SCAN_SPACE_PRESSED) || (pMsg->param2 == SCAN_CR_PRESSED))
                {
                    if(!GetState(pRb, RB_CHECKED))
                        return (RB_MSG_CHECKED);
                }
            }
        }

        return (OBJ_MSG_INVALID);
    }

        #endif
    return (OBJ_MSG_INVALID);
}

/*********************************************************************
* Function: WORD RbDraw(void *pObj)
*
* Output: returns the status of the drawing
*		  0 - not completed
*         1 - done
*
* Overview: draws radio button
*
********************************************************************/
WORD RbDraw(void *pObj)
{
    typedef enum
    {
        REMOVE,
        DRAW_BUTTON0,
        DRAW_BUTTON1,
        DRAW_TEXT,
        DRAW_TEXT_RUN,
        DRAW_CHECK,
        DRAW_CHECK_RUN,
        DRAW_FOC,
        DRAW_FOC_ACTUAL
    } RB_DRAW_STATES;

        #define SIN45   46341

    static RB_DRAW_STATES   state = REMOVE;
    SHORT                   checkIndent;
    static SHORT            radius;
    static SHORT            x, y;
    static DWORD_VAL        temp;

    GFX_COLOR               faceClr;
    RADIOBUTTON *pRb;

    pRb = (RADIOBUTTON *)pObj;

    while(1)
    {

        if(IsDeviceBusy())
            return (0);
    
        switch(state)
        {
            case REMOVE:
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
            GFX_DRIVER_SetupDrawUpdate( pRb->hdr.left,
                                        pRb->hdr.top,
                                        pRb->hdr.right,
                                        pRb->hdr.bottom);
#endif
                if(GetState(pRb, (RB_HIDE | RB_DRAW)))
                {
                    SetColor(pRb->hdr.pGolScheme->CommonBkColor);
                    if(!Bar(pRb->hdr.left, pRb->hdr.top, pRb->hdr.right, pRb->hdr.bottom))
                        return (0);
                }
    
                if(GetState(pRb, RB_HIDE))
            	{
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
	                GFX_DRIVER_CompleteDrawUpdate(   pRb->hdr.left,
	                                                pRb->hdr.top,
	                                                pRb->hdr.right,
	                                                pRb->hdr.bottom);
#endif
	                return (1);
	            }
                radius = ((pRb->hdr.bottom - pRb->hdr.top) >> 1) - RB_INDENT;
                x = pRb->hdr.left + ((pRb->hdr.bottom - pRb->hdr.top) >> 1) + RB_INDENT;
                y = (pRb->hdr.bottom + pRb->hdr.top) >> 1;
                temp.Val = SIN45;
    
                if(GetState(pRb, RB_DRAW))
                {
                    state = DRAW_BUTTON0;
                }
                else if (GetState(pRb, RB_DRAW_CHECK))
                {
                    state = DRAW_CHECK;
                    break;
                }
                else if (GetState(pRb, RB_DRAW_FOCUS))
                {
                    state = DRAW_FOC;
                    break;
                }
                else
            	{
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
	                GFX_DRIVER_CompleteDrawUpdate(   pRb->hdr.left,
	                                                pRb->hdr.top,
	                                                pRb->hdr.right,
	                                                pRb->hdr.bottom);
#endif
	                return (1);
	            }
    
            case DRAW_BUTTON0:
                if(!GetState(pRb, RB_DISABLED))
                {
                    faceClr = pRb->hdr.pGolScheme->Color0;
                }
                else
                {
                    faceClr = pRb->hdr.pGolScheme->ColorDisabled;
                }
    
                GOLPanelDraw
                (
                    x,
                    y,
                    x,
                    y,
                    radius,
                    faceClr,
                    pRb->hdr.pGolScheme->EmbossDkColor,
                    pRb->hdr.pGolScheme->EmbossLtColor,
                    NULL,
                    GOL_EMBOSS_SIZE
                );
                state = DRAW_BUTTON1;
    
            case DRAW_BUTTON1:
                if(!GOLPanelDrawTsk())
                {
                    return (0);
                }
    
                state = DRAW_TEXT;
    
            case DRAW_TEXT:
                if(pRb->pText != NULL)
                {
                    SetFont(pRb->hdr.pGolScheme->pFont);
    
                    if(GetState(pRb, RB_DISABLED))
                    {
                        SetColor(pRb->hdr.pGolScheme->TextColorDisabled);
                    }
                    else
                    {
                        SetColor(pRb->hdr.pGolScheme->TextColor0);
                    }
    
                    MoveTo
                    (
                        pRb->hdr.left + pRb->hdr.bottom - pRb->hdr.top + RB_INDENT,
                        (pRb->hdr.bottom + pRb->hdr.top - pRb->textHeight) >> 1
                    );
    
                    state = DRAW_TEXT_RUN;
                }
    			else
    			{
    			    state = DRAW_CHECK;
                    break;
    			}
            
    		case DRAW_TEXT_RUN:
                if(!OutText(pRb->pText))
                    return (0);
            
                state = DRAW_CHECK;
                break;
    
            case DRAW_CHECK:
                if(GetState(pRb, RB_CHECKED))
                {
                    if(GetState(pRb, RB_DISABLED))
                    {
                        SetColor(pRb->hdr.pGolScheme->TextColorDisabled);
                    }
                    else
                    {
                            #if (COLOR_DEPTH == 1)
                        SetColor(BLACK);
                            #else
                        SetColor(pRb->hdr.pGolScheme->TextColor0);
                            #endif
                    }
                }
                else
                {
                    if(GetState(pRb, RB_DISABLED))
                    {
                        SetColor(pRb->hdr.pGolScheme->ColorDisabled);
                    }
                    else
                    {
                            #if (COLOR_DEPTH == 1)
                        SetColor(WHITE);
                            #else
                        SetColor(pRb->hdr.pGolScheme->Color0);
                            #endif
                    }
                }
    
                state = DRAW_CHECK_RUN;
    
            case DRAW_CHECK_RUN:
                checkIndent = (pRb->hdr.bottom - pRb->hdr.top) >> 2;
                if(!FillCircle(x, y, radius - checkIndent))
                    return (0);
    
                state = DRAW_FOC;
                break;
    
            case DRAW_FOC:
                if(GetState(pRb, RB_DRAW | RB_DRAW_FOCUS))
                {
                    if(GetState(pRb, RB_FOCUSED))
                    {
                        SetColor(pRb->hdr.pGolScheme->TextColor0);
                    }
                    else
                    {
                        SetColor(pRb->hdr.pGolScheme->CommonBkColor);
                    }
    
                    SetLineType(FOCUS_LINE);
                    state = DRAW_FOC_ACTUAL;
                    break;
                }
                state = REMOVE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pRb->hdr.left,
                                                pRb->hdr.top,
                                                pRb->hdr.right,
                                                pRb->hdr.bottom);
#endif
                return (1);
    
            case DRAW_FOC_ACTUAL:
                if(!Rectangle(pRb->hdr.left, pRb->hdr.top, pRb->hdr.right, pRb->hdr.bottom))
                    return (0);
                SetLineType(SOLID_LINE);
                state = REMOVE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pRb->hdr.left,
                                                pRb->hdr.top,
                                                pRb->hdr.right,
                                                pRb->hdr.bottom);
#endif
                return (1);
        } // end of switch()
    } // end of while(1)

}

#endif // USE_RADIOBUTTON
