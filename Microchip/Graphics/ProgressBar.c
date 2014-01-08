/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Progress Bar
 *****************************************************************************
 * FileName:        ProgressBar.c
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
 * 08/05/11     Fixed rendering to check IsDeviceBusy() if not exiting the
 *              draw routine. 
 * 11/04/11     Added state bit PB_NOPROGRESS to indicate the progress in 
 *              text will not be shown.
 *****************************************************************************/
#include "Graphics/Graphics.h"

#ifdef USE_PROGRESSBAR

/*********************************************************************
* Function: void PbWordToString(WORD value, XCHAR* buffer)
*
* Input: value  - value to be converted (from 0 - 100)
*        buffer - buffer receiving string (must be at least 5 bytes)
*
* Output: none
*
* Overview: converts SHORT into string with % at the end
*
********************************************************************/
void PbWordToString(WORD value, XCHAR *buffer)
{
    WORD    result;
    BYTE    pos;

    if(value > 99)
    {
        buffer[0] = '1';
        buffer[1] = '0';
        buffer[2] = '0';
        buffer[3] = '%';
        buffer[4] = 0;
        return;
    }

    pos = 0;
    result = value / 10;
    if(result)
        buffer[pos++] = result + '0';
    result = value - 10 * result;

    buffer[pos++] = result + '0';
    buffer[pos++] = '%';
    buffer[pos++] = 0;
}

/*********************************************************************
* Function: PROGRESSBAR  *PbCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, WORD state, WORD pos, WORD range,
*                              GOL_SCHEME *pScheme)
*
* Overview: creates the progress bar
*
********************************************************************/
PROGRESSBAR *PbCreate
(
    WORD        ID,
    SHORT       left,
    SHORT       top,
    SHORT       right,
    SHORT       bottom,
    WORD        state,
    WORD        pos,
    WORD        range,
    GOL_SCHEME  *pScheme
)
{
    PROGRESSBAR *pPb = NULL;

    pPb = (PROGRESSBAR *)GFX_malloc(sizeof(PROGRESSBAR));
    if(pPb == NULL)
        return (pPb);

    pPb->hdr.ID         = ID;
    pPb->hdr.pNxtObj    = NULL;
    pPb->hdr.type       = OBJ_PROGRESSBAR;
    pPb->hdr.left       = left;
    pPb->hdr.top        = top;
    pPb->hdr.right      = right;
    pPb->hdr.bottom     = bottom;
    pPb->pos            = pos;
    pPb->range          = (range == 0) ? 100 : range;
    pPb->prevPos        = 0;
    pPb->hdr.state      = state;
    pPb->hdr.DrawObj    = PbDraw;   // draw function
    pPb->hdr.MsgObj     = NULL;     // message function
    pPb->hdr.MsgDefaultObj = NULL;  // default message function
    pPb->hdr.FreeObj    = NULL;		// free function

    // Set the style scheme to be used
    if(pScheme == NULL)
        pPb->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pPb->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    GOLAddObject((OBJ_HEADER *)pPb);

    return (pPb);
}

/*********************************************************************
* Function: void PbSetPos(PROGRESSBAR *pPb, SHORT position)
*
* Overview: sets the current position of the progress bar
*
********************************************************************/
void PbSetPos(PROGRESSBAR *pPb, WORD position)
{
    if(pPb->range < position)
        position = pPb->range;

    pPb->pos = position;
}

/*********************************************************************
* Function: void PbSetRange(PROGRESSBAR *pPb, WORD range)
*
* Overview: sets the range of the progress bar
*
********************************************************************/
void PbSetRange(PROGRESSBAR *pPb, WORD range)
{

    // range cannot be assigned a zero value
    if(range != 0)
        pPb->range = range;
    pPb->pos = range;
    pPb->prevPos = 0;
}

/*********************************************************************
* Function: WORD PbTranslateMsg(PROGRESSBAR *pPb, GOL_MSG *pMsg)
*
* Overview: translates the GOL message for the progress bar
*
********************************************************************/
WORD PbTranslateMsg(PROGRESSBAR *pPb, GOL_MSG *pMsg)
{

    // Evaluate if the message is for the progress bar
    // Check if disabled first
    if(GetState(pPb, PB_DISABLED))
        return (OBJ_MSG_INVALID);

        #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {

        // Check if it falls in the progress bar border
        if
        (
            (pPb->hdr.left < pMsg->param1) &&
            (pPb->hdr.right > pMsg->param1) &&
            (pPb->hdr.top < pMsg->param2) &&
            (pPb->hdr.bottom > pMsg->param2)
        )
        {
            return (PB_MSG_SELECTED);
        }
    }

        #endif
    return (OBJ_MSG_INVALID);
}

/*********************************************************************
* Function: WORD PbDraw(void *pObj)
*
* Output: returns the status of the drawing
*		  0 - not complete
*         1 - done
*
* Overview: draws progress bar
*
********************************************************************/
WORD PbDraw(void *pObj)
{
    typedef enum
    {
        REMOVE,
        BOX_DRAW,
        RUN_DRAW,
        BAR_DRAW_SET,
        BAR_DRAW,
        TEXT_DRAW1,
        TEXT_DRAW2,
        TEXT_DRAW3
    } PB_DRAW_STATES;

    static PB_DRAW_STATES state = REMOVE;
    static DWORD x1;
    static DWORD x2;
    static WORD  intLeft, intTop, intRight, intBottom;
    static XCHAR text[5] = {'0','0','%',0};
    PROGRESSBAR *pPb;

    pPb = (PROGRESSBAR *)pObj;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);
    
        switch(state)
        {
            case RUN_DRAW:
                if(!GOLPanelDrawTsk())
                    return (0);
                state = BAR_DRAW_SET;
                break;

            case REMOVE:
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
            GFX_DRIVER_SetupDrawUpdate( pPb->hdr.left,
                                        pPb->hdr.top,
                                        pPb->hdr.right,
                                        pPb->hdr.bottom);
#endif
                if(GetState(pPb, PB_HIDE))
                {
                    SetColor(pPb->hdr.pGolScheme->CommonBkColor);
                    if(!Bar(pPb->hdr.left, pPb->hdr.top, pPb->hdr.right, pPb->hdr.bottom))
                        return (0);
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pPb->hdr.left,
                                                pPb->hdr.top,
                                                pPb->hdr.right,
                                                pPb->hdr.bottom);
#endif
                    return (1);
                }
                state = BOX_DRAW;
    
            case BOX_DRAW:
                if(GetState(pPb, PB_DRAW))
                {
                    GOLPanelDraw
                    (
                        pPb->hdr.left,
                        pPb->hdr.top,
                        pPb->hdr.right,
                        pPb->hdr.bottom,
                        0,
                        pPb->hdr.pGolScheme->Color0,
                        pPb->hdr.pGolScheme->EmbossDkColor,
                        pPb->hdr.pGolScheme->EmbossLtColor,
                        NULL,
                        GOL_EMBOSS_SIZE
                    );
                    state = RUN_DRAW;
                    break;
                } 
                // else progress bar state = PB_DRAW_BAR, refresh only the level
                state = BAR_DRAW_SET;
    
            case BAR_DRAW_SET:
            	if(GetState(pPb, PB_VERTICAL))
                {
    	            x1 = ((DWORD) (pPb->range-pPb->prevPos)) * (pPb->hdr.bottom - pPb->hdr.top - (2 * GOL_EMBOSS_SIZE)) / pPb->range;
    	            x2 = ((DWORD) (pPb->range-pPb->pos)) * (pPb->hdr.bottom - pPb->hdr.top - (2 * GOL_EMBOSS_SIZE)) / pPb->range;
    	            x1 += (pPb->hdr.top + GOL_EMBOSS_SIZE);
    	            x2 += (pPb->hdr.top + GOL_EMBOSS_SIZE);	            
    	        }
    	        else
    	        { 
    	            x1 = ((DWORD) pPb->pos) * (pPb->hdr.right - pPb->hdr.left - (2 * GOL_EMBOSS_SIZE)) / pPb->range;
    	            x2 = ((DWORD) pPb->prevPos) * (pPb->hdr.right - pPb->hdr.left - (2 * GOL_EMBOSS_SIZE)) / pPb->range;
    	            x1 += (pPb->hdr.left + GOL_EMBOSS_SIZE);
    	            x2 += (pPb->hdr.left + GOL_EMBOSS_SIZE);
             	}   

                if(pPb->prevPos > pPb->pos)
                {
    	            SetColor(pPb->hdr.pGolScheme->Color0);
    	        	if(GetState(pPb, PB_VERTICAL))
                    {
                        intLeft   = pPb->hdr.left + GOL_EMBOSS_SIZE; 
                        intTop    = x1;
                        intRight  = pPb->hdr.right - GOL_EMBOSS_SIZE;
                        intBottom = x2;
                    } 
                    else
                    {
                        intLeft   = x1; 
                        intTop    = pPb->hdr.top + GOL_EMBOSS_SIZE;
                        intRight  = x2;
                        intBottom = pPb->hdr.bottom - GOL_EMBOSS_SIZE;
                    }
                }
                else
                {
                    SetColor(pPb->hdr.pGolScheme->Color1);
    	        	if(GetState(pPb, PB_VERTICAL))
        	        {
                        intLeft   = pPb->hdr.left + GOL_EMBOSS_SIZE; 
                        intTop    = x2;
                        intRight  = pPb->hdr.right - GOL_EMBOSS_SIZE;
                        intBottom = x1;
    	    	    } 
                    else
                    {
                        intLeft   = x2; 
                        intTop    = pPb->hdr.top + GOL_EMBOSS_SIZE;
                        intRight  = x1;
                        intBottom = pPb->hdr.bottom - GOL_EMBOSS_SIZE;
        	        }     
                }
                state = BAR_DRAW;

            case BAR_DRAW:
   
                if(!Bar(intLeft, intTop, intRight, intBottom))
   	                return (0);
    
                if (GetState(pPb, PB_NOPROGRESS) || (pPb->hdr.pGolScheme->pFont == NULL))
                {
                    state = REMOVE;
                    pPb->prevPos = pPb->pos;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
	                GFX_DRIVER_CompleteDrawUpdate(   pPb->hdr.left,
	                                                pPb->hdr.top,
	                                                pPb->hdr.right,
	                                                pPb->hdr.bottom);
#endif
                    return (1);
                }

           	    if(GetState(pPb, PB_VERTICAL))
       	        {
    	            SetColor(pPb->hdr.pGolScheme->Color0);
                    intLeft   = pPb->hdr.left + GOL_EMBOSS_SIZE;
                    intTop    = (pPb->hdr.top + pPb->hdr.bottom - GetTextHeight(pPb->hdr.pGolScheme->pFont)) >> 1;
    	            intRight  = pPb->hdr.right - GOL_EMBOSS_SIZE;
    	            intBottom = x2;
    			} 
    			else
    			{
    	            SetColor(pPb->hdr.pGolScheme->Color1);
                    intLeft   = (pPb->hdr.left + pPb->hdr.right - GetTextWidth(text, pPb->hdr.pGolScheme->pFont)) >> 1;
    	            intTop    = pPb->hdr.top + GOL_EMBOSS_SIZE;
    	            intRight  = x1;
    	            intBottom = pPb->hdr.bottom - GOL_EMBOSS_SIZE;
    			}	

                state = TEXT_DRAW1;
    
            case TEXT_DRAW1:
                if(!Bar(intLeft, intTop, intRight, intBottom))
   	                return (0);

            	if(GetState(pPb, PB_VERTICAL))
       	        {
    	            SetColor(pPb->hdr.pGolScheme->Color1);
                    intLeft   = pPb->hdr.left + GOL_EMBOSS_SIZE;
                    intTop    = x2;
    	            intRight  = pPb->hdr.right - GOL_EMBOSS_SIZE;
    	            intBottom = (pPb->hdr.top + pPb->hdr.bottom + GetTextHeight(pPb->hdr.pGolScheme->pFont)) >> 1;
    			}
    			else 
    			{
    	            SetColor(pPb->hdr.pGolScheme->Color0);
                    intLeft   = x1;
    	            intTop    = pPb->hdr.top + GOL_EMBOSS_SIZE;
    	            intRight  = (pPb->hdr.left + pPb->hdr.right + GetTextWidth(text, pPb->hdr.pGolScheme->pFont)) >> 1;
    	            intBottom = pPb->hdr.bottom - GOL_EMBOSS_SIZE;
    			}	 
                state = TEXT_DRAW2;
    
            case TEXT_DRAW2:
                if(!Bar(intLeft, intTop, intRight, intBottom))
   	                return (0);
    
                PbWordToString((DWORD) pPb->pos * 100 / pPb->range, text);
                SetColor(pPb->hdr.pGolScheme->TextColor0);
    
                MoveTo
                (
                    (pPb->hdr.left + pPb->hdr.right - GetTextWidth(text, pPb->hdr.pGolScheme->pFont)) >> 1,
                    (pPb->hdr.top + pPb->hdr.bottom - GetTextHeight(pPb->hdr.pGolScheme->pFont)) >> 1
                );
    
                SetFont(pPb->hdr.pGolScheme->pFont);
                state = TEXT_DRAW3;
    
            case TEXT_DRAW3:
                if(!OutText(text))
                    return (0);
    
                pPb->prevPos = pPb->pos;
                state = REMOVE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
            GFX_DRIVER_CompleteDrawUpdate(   pPb->hdr.left,
                                            pPb->hdr.top,
                                            pPb->hdr.right,
                                            pPb->hdr.bottom);
#endif
                return (1);
        } // end of switch()
    } // end of while(1)
}

#endif // USE_PROGRESSBAR
