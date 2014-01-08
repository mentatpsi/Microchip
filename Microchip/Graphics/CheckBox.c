/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Check Box
 *****************************************************************************
 * FileName:        CheckBox.c
 * Dependencies:    None 
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, C32
 * Linker:          MPLAB LINK30, LINK32
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
 * 08/08/11     Fixed rendering to check IsDeviceBusy() if not exiting the 
 *              draw routine.
 *****************************************************************************/
#include "Graphics/Graphics.h"

#ifdef USE_CHECKBOX

/*********************************************************************
* Function: CHECKBOX  *CbCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, WORD state, XCHAR *pText, 
*                              GOL_SCHEME *pScheme)
*
* Overview: Creates the check box.
*
********************************************************************/
CHECKBOX *CbCreate
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
    CHECKBOX    *pCb = NULL;

    pCb = (CHECKBOX *)GFX_malloc(sizeof(CHECKBOX));
    if(pCb == NULL)
        return (pCb);

    pCb->hdr.ID = ID;
    pCb->hdr.pNxtObj = NULL;
    pCb->hdr.type = OBJ_CHECKBOX;
    pCb->hdr.left = left;
    pCb->hdr.top = top;
    pCb->hdr.right = right;
    pCb->hdr.bottom = bottom;
    pCb->pText = pText;
    pCb->hdr.state = state;
    pCb->hdr.DrawObj = CbDraw;				// draw function
    pCb->hdr.MsgObj = CbTranslateMsg;       // message function
    pCb->hdr.MsgDefaultObj = CbMsgDefault;  // default message function
    pCb->hdr.FreeObj = NULL;				// free function
    
    // Set the style scheme
    if(pScheme == NULL)
        pCb->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pCb->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    // Set the text height
    pCb->textHeight = 0;
    if(pText != NULL)
    {
        pCb->textHeight = GetTextHeight(pCb->hdr.pGolScheme->pFont);
    }

    GOLAddObject((OBJ_HEADER *)pCb);

    return (pCb);
}

/*********************************************************************
* Function: CbSetText(CHECKBOX *pCb, char *pText)
*
* Overview: Sets the text.
*
********************************************************************/
void CbSetText(CHECKBOX *pCb, XCHAR *pText)
{
    pCb->pText = pText;
    pCb->textHeight = GetTextHeight((BYTE *)pCb->hdr.pGolScheme->pFont);
}

/*********************************************************************
* Function: CbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
* Overview: Changes the state of the check box by default.
*
********************************************************************/
void CbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
{
    CHECKBOX *pCb;

    pCb = (CHECKBOX *)pObj;

        #ifdef USE_FOCUS
            #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {
        if(!GetState(pCb, CB_FOCUSED))
        {
            GOLSetFocus((OBJ_HEADER *)pCb);
        }
    }

            #endif
        #endif
    switch(translatedMsg)
    {
        case CB_MSG_CHECKED:
            SetState(pCb, CB_CHECKED | CB_DRAW_CHECK);  // Set checked and redraw
            break;

        case CB_MSG_UNCHECKED:
            ClrState(pCb, CB_CHECKED);                  // Reset check
            SetState(pCb, CB_DRAW_CHECK);               // Redraw
            break;
    }
}

/*********************************************************************
* Function: WORD CbTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: Checks if the check box will be affected by the message
*           and returns translated message.
*
********************************************************************/
WORD CbTranslateMsg(void *pObj, GOL_MSG *pMsg)
{

    CHECKBOX *pCb;

    pCb = (CHECKBOX *)pObj;

    // Evaluate if the message is for the check box
    // Check if disabled first
    if(GetState(pCb, CB_DISABLED))
        return (OBJ_MSG_INVALID);

        #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {

        // Check if it falls in the check box borders
        if
        (
            (pCb->hdr.left < pMsg->param1) &&
            (pCb->hdr.right > pMsg->param1) &&
            (pCb->hdr.top < pMsg->param2) &&
            (pCb->hdr.bottom > pMsg->param2)
        )
        {
            if(pMsg->uiEvent == EVENT_PRESS)
            {
                if(GetState(pCb, CB_CHECKED))
                    return (CB_MSG_UNCHECKED);
                else
                    return (CB_MSG_CHECKED);
            }
        }

        return (OBJ_MSG_INVALID);
    }

        #endif
        #ifdef USE_KEYBOARD
    if(pMsg->type == TYPE_KEYBOARD)
    {
        if((WORD)pMsg->param1 == pCb->hdr.ID)
        {
            if(pMsg->uiEvent == EVENT_KEYSCAN)
            {
                if((pMsg->param2 == SCAN_SPACE_PRESSED) || (pMsg->param2 == SCAN_CR_PRESSED))
                {
                    if(GetState(pCb, CB_CHECKED))
                        return (CB_MSG_UNCHECKED);
                    else
                        return (CB_MSG_CHECKED);
                }
            }
        }

        return (OBJ_MSG_INVALID);
    }

        #endif
    return (OBJ_MSG_INVALID);
}

/*********************************************************************
* Function: WORD CbDraw(void *pObj)
*
* Output: returns the status of the drawing
*		  0 - not complete
*         1 - done
*
* Overview: Draws check box.
*
********************************************************************/
WORD CbDraw(void *pObj)
{
    typedef enum
    {
        CB_STATE_REMOVE,
        CB_STATE_BOX_DRAW,
        CB_STATE_RUN_DRAW,
        CB_STATE_TEXT_DRAW,
        CB_STATE_TEXT_DRAW_RUN,
        CB_STATE_SET_CHECK_DRAW,
        CB_STATE_CHECK_DRAW,
        CB_STATE_FOCUS_DRAW
    } CB_DRAW_STATES;

    static CB_DRAW_STATES state = CB_STATE_REMOVE;

    static SHORT checkIndent;
    CHECKBOX *pCb;

    pCb = (CHECKBOX *)pObj;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);
    
        switch(state)
        {
            case CB_STATE_RUN_DRAW:
                if(!GOLPanelDrawTsk())
                    return (0);
                state = CB_STATE_TEXT_DRAW;

            case CB_STATE_TEXT_DRAW:
                if(pCb->pText != NULL)
                {
                    SetFont(pCb->hdr.pGolScheme->pFont);
    
                    if(!GetState(pCb, CB_DISABLED))
                    {
                        SetColor(pCb->hdr.pGolScheme->TextColor0);
                    }
                    else
                    {
                        SetColor(pCb->hdr.pGolScheme->TextColorDisabled);
                    }
    
                    MoveTo
                    (
                        pCb->hdr.left + pCb->hdr.bottom - pCb->hdr.top + CB_INDENT,
                        (pCb->hdr.bottom + pCb->hdr.top - pCb->textHeight) >> 1
                    );
    
                    state = CB_STATE_TEXT_DRAW_RUN;
                }
                else
                {
                    state = CB_STATE_SET_CHECK_DRAW;
                    break;
                }
    
            case CB_STATE_TEXT_DRAW_RUN:
                if(!OutText(pCb->pText))
                    return (0);

                state = CB_STATE_SET_CHECK_DRAW;
                break;

            case CB_STATE_REMOVE:
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
	            GFX_DRIVER_SetupDrawUpdate( pCb->hdr.left,
	                                        pCb->hdr.top,
	                                        pCb->hdr.right,
	                                        pCb->hdr.bottom);
#endif
                if(GetState(pCb, CB_HIDE | CB_DRAW))
                {
                    SetColor(pCb->hdr.pGolScheme->CommonBkColor);
                    if(!Bar(pCb->hdr.left, pCb->hdr.top, pCb->hdr.right, pCb->hdr.bottom))
                    {
                        return (0);
                    }
    
                    if(GetState(pCb, CB_HIDE))
                	{
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                    	GFX_DRIVER_CompleteDrawUpdate(   pCb->hdr.left,
	                                                    pCb->hdr.top,
	                                                    pCb->hdr.right,
	                                                    pCb->hdr.bottom);
#endif
	                    return (1);
                	}
				}
    
                state = CB_STATE_BOX_DRAW;
    
            case CB_STATE_BOX_DRAW:
                if(GetState(pCb, CB_DRAW))
                {
                    if(!GetState(pCb, CB_DISABLED))
                    {
                        GOLPanelDraw
                        (
                            pCb->hdr.left + CB_INDENT,
                            pCb->hdr.top + CB_INDENT,
                            pCb->hdr.left + (pCb->hdr.bottom - pCb->hdr.top) - CB_INDENT,
                            pCb->hdr.bottom - CB_INDENT,
                            0,
                            pCb->hdr.pGolScheme->Color0,
                            pCb->hdr.pGolScheme->EmbossDkColor,
                            pCb->hdr.pGolScheme->EmbossLtColor,
                            NULL,
                            GOL_EMBOSS_SIZE
                        );
                    }
                    else
                    {
                        GOLPanelDraw
                        (
                            pCb->hdr.left + CB_INDENT,
                            pCb->hdr.top + CB_INDENT,
                            pCb->hdr.left + (pCb->hdr.bottom - pCb->hdr.top) - CB_INDENT,
                            pCb->hdr.bottom - CB_INDENT,
                            0,
                            pCb->hdr.pGolScheme->ColorDisabled,
                            pCb->hdr.pGolScheme->EmbossDkColor,
                            pCb->hdr.pGolScheme->EmbossLtColor,
                            NULL,
                            GOL_EMBOSS_SIZE
                        );
                    }
    
                    state = CB_STATE_RUN_DRAW;
                    break;
                }
                else
                {
                    state = CB_STATE_SET_CHECK_DRAW;
                }
   
            case CB_STATE_SET_CHECK_DRAW:
                if(GetState(pCb, CB_DRAW | CB_DRAW_CHECK))
                {
                    if(!GetState(pCb, CB_DISABLED))
                    {
                        if(GetState(pCb, CB_CHECKED))
                        {
                            SetColor(pCb->hdr.pGolScheme->TextColor0);
                        }
                        else
                        {
                            SetColor(pCb->hdr.pGolScheme->Color0);
                        }
                    }
                    else
                    {
                        if(GetState(pCb, CB_CHECKED))
                        {
                            SetColor(pCb->hdr.pGolScheme->TextColorDisabled);
                        }
                        else
                        {
                            SetColor(pCb->hdr.pGolScheme->ColorDisabled);
                        }
                    }
    
                    checkIndent = (pCb->hdr.bottom - pCb->hdr.top) >> 2;
                    state = CB_STATE_CHECK_DRAW;
                }   
                else
                {
                    state = CB_STATE_FOCUS_DRAW;
                    break;
                }
    
            case CB_STATE_CHECK_DRAW:
                if 
                (
                    !Bar
                        (
                            pCb->hdr.left + checkIndent + GOL_EMBOSS_SIZE,
                            pCb->hdr.top + checkIndent + GOL_EMBOSS_SIZE,
                            pCb->hdr.left + (pCb->hdr.bottom - pCb->hdr.top) - checkIndent - GOL_EMBOSS_SIZE,
                            pCb->hdr.bottom - checkIndent - GOL_EMBOSS_SIZE
                        )
                )
                {
                    return (0);
                }
    
                state = CB_STATE_FOCUS_DRAW;
    
            case CB_STATE_FOCUS_DRAW:
                if(GetState(pCb, CB_DRAW | CB_DRAW_FOCUS))
                {
                    if(GetState(pCb, CB_FOCUSED))
                    {
                        SetColor(pCb->hdr.pGolScheme->TextColor0);
                    }
                    else
                    {
                        SetColor(pCb->hdr.pGolScheme->CommonBkColor);
                    }
    
                    SetLineType(FOCUS_LINE);
                    if(!Rectangle(pCb->hdr.left, pCb->hdr.top, pCb->hdr.right, pCb->hdr.bottom))
                    {
                        return (0);
                    }
    
                    SetLineType(SOLID_LINE);
                }
    
                state = CB_STATE_REMOVE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
            	GFX_DRIVER_CompleteDrawUpdate(   pCb->hdr.left,
                                                pCb->hdr.top,
                                                pCb->hdr.right,
                                                pCb->hdr.bottom);
#endif
                return (1);
        } // end of switch()   
    } // end of while(1)
}

#endif // USE_CHECKBOX
