/*****************************************************************************
 *  Module for Microchip Graphics Library 
 *  GOL Layer 
 *  Static Text
 *****************************************************************************
 * FileName:        StaticText.c
 * Dependencies:    None 
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07	   Version 1.0 release
 * 11/12/07	   Fixed clipping enabling location
 * 08/04/11    Fixed rendering to check IsDeviceBusy() if not exiting the
 *             draw routine.
*****************************************************************************/
#include "Graphics/Graphics.h"

#ifdef USE_STATICTEXT

/*********************************************************************
* Function: STATICTEXT  *StCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, 
*								  WORD state , XCHAR *pText, GOL_SCHEME *pScheme)
*
* Notes: Creates a STATICTEXT object and adds it to the current active list.
*        If the creation is successful, the pointer to the created Object 
*        is returned. If not successful, NULL is returned.
*
********************************************************************/
STATICTEXT *StCreate
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
    STATICTEXT  *pSt = NULL;

    pSt = (STATICTEXT *)GFX_malloc(sizeof(STATICTEXT));
    if(pSt == NULL)
        return (pSt);

    pSt->hdr.ID = ID;               // unique id assigned for referencing
    pSt->hdr.pNxtObj = NULL;        // initialize pointer to NULL
    pSt->hdr.type = OBJ_STATICTEXT; // set object type
    pSt->hdr.left = left;           // left,top corner
    pSt->hdr.top = top;
    pSt->hdr.right = right;         // right buttom corner
    pSt->hdr.bottom = bottom;
    pSt->pText = pText;             // location of the text
    pSt->hdr.state = state;
    pSt->hdr.DrawObj = StDraw;			// draw function
    pSt->hdr.MsgObj = StTranslateMsg;   // message function
    pSt->hdr.MsgDefaultObj = NULL;		// default message function
    pSt->hdr.FreeObj = NULL;			// free function

    // Set the color scheme to be used
    if(pScheme == NULL)
        pSt->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pSt->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    pSt->textHeight = 0;
    if(pSt->pText != NULL)
    {

        // Set the text height
        pSt->textHeight = GetTextHeight(pSt->hdr.pGolScheme->pFont);
    }

    GOLAddObject((OBJ_HEADER *)pSt);
    return (pSt);
}

/*********************************************************************
* Function: StSetText(STATICTEXT *pSt, XCHAR *pText)
*
* Notes: Sets the string that will be used.
*
********************************************************************/
void StSetText(STATICTEXT *pSt, XCHAR *pText)
{
    pSt->pText = pText;
    pSt->textHeight = GetTextHeight(pSt->hdr.pGolScheme->pFont);
}

/*********************************************************************
* Function: WORD StTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Notes: Evaluates the message if the object will be affected by the 
*		 message or not.
*
********************************************************************/
WORD StTranslateMsg(void *pObj, GOL_MSG *pMsg)
{

    STATICTEXT *pSt;

    pSt = (STATICTEXT *)pObj;


    // Evaluate if the message is for the static text
    // Check if disabled first
    if(GetState(pSt, ST_DISABLED))
        return (OBJ_MSG_INVALID);

        #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {

        // Check if it falls in static text control borders
        if
        (
            (pSt->hdr.left < pMsg->param1) &&
            (pSt->hdr.right > pMsg->param1) &&
            (pSt->hdr.top < pMsg->param2) &&
            (pSt->hdr.bottom > pMsg->param2)
        )
        {
            return (ST_MSG_SELECTED);
        }
    }

        #endif
    return (OBJ_MSG_INVALID);
}

/*********************************************************************
* Function: WORD StDraw(void *pObj)
*
* Notes: This is the state machine to draw the static text.
*
********************************************************************/
WORD StDraw(void *pObj)
{
    typedef enum
    {
        ST_STATE_IDLE,
        ST_STATE_CLEANAREA,
        ST_STATE_INIT,
        ST_STATE_SETALIGN,
        ST_STATE_DRAWTEXT
    } ST_DRAW_STATES;

    static ST_DRAW_STATES state = ST_STATE_IDLE;
    static SHORT charCtr = 0, lineCtr = 0;
    static XCHAR *pCurLine = NULL;
    SHORT textWidth;
    XCHAR ch = 0;
    STATICTEXT *pSt;

    pSt = (STATICTEXT *)pObj;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);
    
        switch(state)
        {
            case ST_STATE_IDLE:
                SetClip(CLIP_DISABLE);
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_SetupDrawUpdate( pSt->hdr.left,
                                            pSt->hdr.top,
                                            pSt->hdr.right,
                                            pSt->hdr.bottom);
#endif
    
                if(GetState(pSt, ST_HIDE))
                {
                    SetColor(pSt->hdr.pGolScheme->CommonBkColor);
                    if(!Bar(pSt->hdr.left, pSt->hdr.top, pSt->hdr.right, pSt->hdr.bottom))
                        return (0);
    
                    // State is still IDLE STATE so no need to set state
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                    GFX_DRIVER_CompleteDrawUpdate(   pSt->hdr.left,
                                                    pSt->hdr.top,
                                                    pSt->hdr.right,
                                                    pSt->hdr.bottom);
#endif
                    return (1);
                }
    
                if(GetState(pSt, ST_DRAW))
                {
                    // show frame if specified to be shown
                    SetLineType(SOLID_LINE);
                    SetLineThickness(NORMAL_LINE);
    
                    if(GetState(pSt, ST_FRAME))
                    {
    
                        if(!GetState(pSt, ST_DISABLED))
                        {
    
                            // show enabled color
                            SetColor(pSt->hdr.pGolScheme->Color1);
                            if(!Rectangle(pSt->hdr.left, pSt->hdr.top, pSt->hdr.right, pSt->hdr.bottom))
                                return (0);
                        }
                        else
                        {
    
                            // show disabled color
                            SetColor(pSt->hdr.pGolScheme->ColorDisabled);
                            if(!Rectangle(pSt->hdr.left, pSt->hdr.top, pSt->hdr.right, pSt->hdr.bottom))
                                return (0);
                        }
                    }
                    else
                    {
                        // show enabled color
                        SetColor(pSt->hdr.pGolScheme->CommonBkColor);
                        if(!Rectangle(pSt->hdr.left, pSt->hdr.top, pSt->hdr.right, pSt->hdr.bottom))
                        	return (0);
    	                
    	            } 
                }
    
                state = ST_STATE_CLEANAREA;
    
            case ST_STATE_CLEANAREA:
    
                // clean area where text will be placed.
                SetColor(pSt->hdr.pGolScheme->CommonBkColor);
                if(!Bar(pSt->hdr.left + 1, pSt->hdr.top + 1, pSt->hdr.right - 1, pSt->hdr.bottom - 1))
                    return (0);
    
                // set clipping area, text will only appear inside the static text area.
                SetClip(CLIP_ENABLE);
                SetClipRgn(pSt->hdr.left + ST_INDENT, pSt->hdr.top, pSt->hdr.right - ST_INDENT, pSt->hdr.bottom);
                state = ST_STATE_INIT;
    
            case ST_STATE_INIT:
                if(IsDeviceBusy())
                    return (0);
    
                // set the text color
                if(!GetState(pSt, ST_DISABLED))
                {
                    SetColor(pSt->hdr.pGolScheme->TextColor0);
                }
                else
                {
                    SetColor(pSt->hdr.pGolScheme->TextColorDisabled);
                }
    
                // use the font specified in the object
                SetFont(pSt->hdr.pGolScheme->pFont);
                pCurLine = pSt->pText;                  // get first line of text
                state = ST_STATE_SETALIGN;              // go to drawing of text
    
            case ST_STATE_SETALIGN:
                if(charCtr == 0)
                {
    
                    // set position of the next character (based on alignment and next character)
                    textWidth = GetTextWidth(pCurLine, pSt->hdr.pGolScheme->pFont);
    
                    // Display text with center alignment
                    if(GetState(pSt, (ST_CENTER_ALIGN)))
                    {
                        MoveTo((pSt->hdr.left + pSt->hdr.right - textWidth) >> 1, pSt->hdr.top + (lineCtr * pSt->textHeight));
                    }
    
                    // Display text with right alignment
                    else if(GetState(pSt, (ST_RIGHT_ALIGN)))
                    {
                        MoveTo((pSt->hdr.right - textWidth - ST_INDENT), pSt->hdr.top + (lineCtr * pSt->textHeight));
                    }
    
                    // Display text with left alignment
                    else
                    {
                        MoveTo(pSt->hdr.left + ST_INDENT, pSt->hdr.top + (lineCtr * pSt->textHeight));
                    }
                }
    
                state = ST_STATE_DRAWTEXT;
    
            case ST_STATE_DRAWTEXT:
                ch = *(pCurLine + charCtr);
    
                // output one character at time until a newline character or a NULL character is sampled
                while((0x0000 != ch) && (0x000A != ch))
                {
                    if(!OutChar(ch))
                        return (0);                     // render the character
                    charCtr++;                          // update to next character
                    ch = *(pCurLine + charCtr);
                }
    
                // pCurText is updated for the next line
                if(ch == 0x000A)
                {                                       // new line character
                    pCurLine = pCurLine + charCtr + 1;  // go to first char of next line
                    lineCtr++;                          // update line counter
                    charCtr = 0;                        // reset char counter
                    state = ST_STATE_SETALIGN;          // continue to next line
                    break;
                }
    
                // end of text string is reached no more lines to display
                else
                {
                    pCurLine = NULL;                    // reset static variables
                    lineCtr = 0;
                    charCtr = 0;
                    SetClip(CLIP_DISABLE);              // remove clipping
                    state = ST_STATE_IDLE;              // go back to IDLE state
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                    GFX_DRIVER_CompleteDrawUpdate(   pSt->hdr.left,
                                                    pSt->hdr.top,
                                                    pSt->hdr.right,
                                                    pSt->hdr.bottom);
#endif
                    return (1);
                }
        } // end of switch()
    } // end of while(1)    
}

#endif // USE_STATICTEXT
