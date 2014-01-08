/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer
 *  Edit Box
 *****************************************************************************
 * FileName:        EditBox.c
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
 * Date        	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07	   	Version 1.0 release
 * 09/16/10    	Fixed issue on focus. Modified EB_CARET behavior. Caret 
 *				is now controlled by the EB_CARET state bit. If this bit  
 * 				is set, focus or manual user set of EB_DRAW_CARET will draw 
 *				the caret. If this bit is not set, the caret will never 
 *				be shown.
 * 02/24/11     Modified draw state machine to remove incorrect loops.
 * 08/05/11     EB_CARET will indicate that the cursor caret will always be drawn.
 *              Cursor caret drawing will also serve as focus indicator. EB_DRAW_CARET 
 *              with EB_FOCUSED set will draw the cursor caret regardless of EB_CARET 
 *              state.
 * 09/27/11     EbTranslateMsg() should process touch without USE_FOCUS
 *              enabled.
 * 03/20/12     Modified EbSetText() to terminate if end of string is detected
 *              before max length is reached.
*****************************************************************************/
#include "Graphics/Graphics.h"

#ifdef USE_EDITBOX

/*********************************************************************
* Function: EDITBOX  *EbCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom,
*							   WORD state , XCHAR *pText, WORD charMax, GOL_SCHEME *pScheme)
*
* Notes: Create the EDITBOX Object.
*
********************************************************************/
EDITBOX  *EbCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom,
				   WORD state , XCHAR *pText, WORD charMax, GOL_SCHEME *pScheme){

	EDITBOX *pEb = NULL;

  	pEb = (EDITBOX*)GFX_malloc(sizeof(EDITBOX)+ (charMax + 1)*sizeof(XCHAR)); // ending zero is not included into charMax
	if (pEb == NULL)
		return pEb;

    pEb->pBuffer  = (XCHAR*)((BYTE*)pEb+sizeof(EDITBOX));
    *pEb->pBuffer = 0;
    pEb->length   = 0;
    pEb->charMax  = charMax;

    if(pText != NULL)
        EbSetText(pEb, pText);

	pEb->hdr.ID      	    = ID;
	pEb->hdr.pNxtObj 	    = NULL;
	pEb->hdr.type    	    = OBJ_EDITBOX;
	pEb->hdr.left    	    = left;
	pEb->hdr.top     	    = top;
	pEb->hdr.right   	    = right;
	pEb->hdr.bottom  	    = bottom;
	pEb->hdr.state   	    = state;
    pEb->hdr.DrawObj        = EbDraw;		 // draw function
    pEb->hdr.MsgObj         = EbTranslateMsg;// message function
    pEb->hdr.MsgDefaultObj  = EbMsgDefault;  // default message function
    pEb->hdr.FreeObj        = NULL;			 // free function

	// Set the style scheme to be used
	if (pScheme == NULL)
		pEb->hdr.pGolScheme = _pDefaultGolScheme;
	else
		pEb->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    pEb->textHeight = GetTextHeight(pEb->hdr.pGolScheme->pFont);

    GOLAddObject((OBJ_HEADER*) pEb);
	return pEb;
}

/*********************************************************************
* Function: EbSetText(EDITBOX *pEb, XCHAR *pText)
*
* Notes: Sets a new text.
*
********************************************************************/
void EbSetText(EDITBOX *pEb, XCHAR *pText){
WORD  ch;
WORD  length;
XCHAR* pointerFrom;
XCHAR* pointerTo;

    // Copy and count length
    pointerFrom = pText;
    pointerTo = pEb->pBuffer;
    length = 0;

    do{
        ch = *pointerFrom++;
        *pointerTo++ = ch;
        length++;
        if(length >= pEb->charMax){
            *pointerTo = 0;
            break;
        }
        if(ch == 0){
            break;
        }
    }while(ch);


    pEb->length = length-1;
}

/*********************************************************************
* Function: void  EbAddChar(EDITBOX* pEb, XCHAR ch)
*
* Notes: Adds character at the end.
*
********************************************************************/
void EbAddChar(EDITBOX* pEb, XCHAR ch){

    if(pEb->length >= pEb->charMax)
        return;

    // Add character
    pEb->pBuffer[pEb->length] = ch;
    pEb->length++;
    pEb->pBuffer[pEb->length] = 0;
}

/*********************************************************************
* Function: void  EbDeleteChar(EDITBOX* pEb)
*
* Notes: Deletes character at the end.
*
********************************************************************/
void EbDeleteChar(EDITBOX* pEb){

    if(pEb->length == 0)
        return;

    // Delete charachter
    pEb->length--;
    pEb->pBuffer[pEb->length] = 0;

}

/*********************************************************************
* Function: WORD EbTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Notes: Translates GOL message for the edit box
*
********************************************************************/
WORD EbTranslateMsg(void *pObj, GOL_MSG *pMsg)
{
    EDITBOX *pEb;

    pEb = (EDITBOX *)pObj;

    // Evaluate if the message is for the edit box
    // Check if disabled first
	if (GetState(pEb, EB_DISABLED))
      return OBJ_MSG_INVALID;

#ifdef  USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN) {
		// Check if it falls in edit box borders
		if( (pEb->hdr.left     < pMsg->param1) &&
	  	    (pEb->hdr.right    > pMsg->param1) &&
	   	    (pEb->hdr.top      < pMsg->param2) &&
	   	    (pEb->hdr.bottom   > pMsg->param2) )
            return EB_MSG_TOUCHSCREEN;

        return OBJ_MSG_INVALID;
	}
#endif

#ifdef  USE_KEYBOARD
    if(pMsg->type == TYPE_KEYBOARD) {

        if(pMsg->uiEvent == EVENT_CHARCODE)
            return EB_MSG_CHAR;

        if(pMsg->uiEvent == EVENT_KEYSCAN)
            if(pMsg->param2 == SCAN_BS_PRESSED)
                return EB_MSG_DEL;

	    return OBJ_MSG_INVALID;
	}
#endif


	return OBJ_MSG_INVALID;
}

/*********************************************************************
* Function: void EbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
*
* Notes: Changes the state of the edit box by default.
*
********************************************************************/
void EbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg){

    EDITBOX *pEb;

    pEb = (EDITBOX *)pObj;

#ifdef  USE_FOCUS
#ifdef  USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN){
        if(!GetState(pEb,EB_FOCUSED)){
                GOLSetFocus((OBJ_HEADER*)pEb);
        }
    }
#endif
#endif

    switch(translatedMsg){

        case EB_MSG_CHAR:
            EbAddChar(pEb,(XCHAR)pMsg->param2);
            SetState(pEb, EB_DRAW);
            break;

        case EB_MSG_DEL:
            EbDeleteChar(pEb);
            SetState(pEb, EB_DRAW);
            break;

    }
}

/*********************************************************************
* Function: WORD EbDraw(void *pObj)
*
* Notes: This is the state machine to draw the button.
*
********************************************************************/
WORD EbDraw(void *pObj)
{
typedef enum {
	EB_STATE_START,
	EB_STATE_DRAW_PANEL,
	EB_STATE_PREPARE_FOR_TEXT,
	EB_STATE_POSITION_TEXT,
	EB_STATE_TEXT,
    EB_STATE_CARET,
} EB_DRAW_STATES;

static EB_DRAW_STATES state = EB_STATE_START;
static XCHAR* pText;
static GFX_COLOR tempColor;
static SHORT temp;
static SHORT width = 0;
EDITBOX *pEb;

    pEb = (EDITBOX *)pObj;

    while(!IsDeviceBusy())
    {
        switch(state){
    
            case EB_STATE_START:
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_SetupDrawUpdate( pEb->hdr.left,
                                            pEb->hdr.top,
                                            pEb->hdr.right,
                                            pEb->hdr.bottom);
#endif
    
              	if(GetState(pEb, EB_HIDE)){
       	   	        SetColor(pEb->hdr.pGolScheme->CommonBkColor);
        	        if(!Bar(pEb->hdr.left,pEb->hdr.top,pEb->hdr.right,pEb->hdr.bottom)) 
                        return 0;

#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                    GFX_DRIVER_CompleteDrawUpdate(   pEb->hdr.left,
                                                    pEb->hdr.top,
                                                    pEb->hdr.right,
                                                    pEb->hdr.bottom);
#endif
                    return 1;
        	    }
    
                if(GetState(pEb,EB_DISABLED)){
                    tempColor = pEb->hdr.pGolScheme->ColorDisabled;
    	        }else{
                    tempColor = pEb->hdr.pGolScheme->Color0;
                }
    
                if(GetState(pEb,EB_DRAW))
                {
    
                    GOLPanelDraw(pEb->hdr.left,pEb->hdr.top,pEb->hdr.right,pEb->hdr.bottom,0,
                        tempColor,
                        pEb->hdr.pGolScheme->EmbossDkColor,
                        pEb->hdr.pGolScheme->EmbossLtColor,
                        NULL,
                        GOL_EMBOSS_SIZE);
    
                    state = EB_STATE_DRAW_PANEL;
                    // no break here since it will always go to the EB_STATE_DRAW_PANEL state
                }
                else
                {
                    state = EB_STATE_PREPARE_FOR_TEXT;
                    break;
                }    
                
            case EB_STATE_DRAW_PANEL:
                if(!GOLPanelDrawTsk())
                    return 0;
                state = EB_STATE_PREPARE_FOR_TEXT;
                
                // no break here since it will always go to the EB_STATE_PREPARE_FOR_TEXT state
    
            case EB_STATE_PREPARE_FOR_TEXT:
    
                SetClip(CLIP_ENABLE);
    
                SetClipRgn(pEb->hdr.left+GOL_EMBOSS_SIZE+EB_INDENT,
                           pEb->hdr.top+GOL_EMBOSS_SIZE+EB_INDENT,
                           pEb->hdr.right-GOL_EMBOSS_SIZE-EB_INDENT,
                           pEb->hdr.bottom-GOL_EMBOSS_SIZE-EB_INDENT);
    
    	        SetFont(pEb->hdr.pGolScheme->pFont);
    
                if(GetState(pEb,EB_DISABLED)){
                    SetColor(pEb->hdr.pGolScheme->TextColorDisabled);
                }else{
                    SetColor(pEb->hdr.pGolScheme->TextColor0);
                }
    
                // get the string buffer
                pText = pEb->pBuffer;
                temp = 1;
    
                // calculate how many lines are expected so we know where to
                // place the starting point
                while((XCHAR)*pText != (XCHAR)0){
                    if((XCHAR)*pText == (XCHAR)'\n')
                        temp++;
                    pText++;
                }
    
                // go back to the start of the buffer
                pText = pEb->pBuffer;
                // position the cursor 
                MoveTo(GetX(),(pEb->hdr.top+pEb->hdr.bottom-temp*pEb->textHeight)>>1);
                
                // set state to actual string rendering loop
                state = EB_STATE_POSITION_TEXT;
                
                // no break here since it will always go to the EB_STATE_POSITION_TEXT state

            case EB_STATE_POSITION_TEXT:
    
                // get width of the string to render
                width = GetTextWidth(pText,pEb->hdr.pGolScheme->pFont);
    
                // place the starting point based on text alignment
                if (!GetState(pEb, EB_CENTER_ALIGN|EB_RIGHT_ALIGN)) {
                    MoveTo(pEb->hdr.left+GOL_EMBOSS_SIZE+EB_INDENT, GetY());
                }else{
                    if (GetState(pEb, EB_RIGHT_ALIGN)) {
    			        MoveTo(pEb->hdr.right-width-EB_INDENT-GOL_EMBOSS_SIZE, GetY());
                    }else{
                        MoveTo((pEb->hdr.left+pEb->hdr.right-width)>>1,GetY());
                    }
                }
    
    		    state = EB_STATE_TEXT;
    		    
    		    // no break here since it will always go to the EB_STATE_TEXT state
    
            case EB_STATE_TEXT:
            
                // this is the actual string rendering 
                
                if(GetState(pEb,EB_DRAW)){
                    if(!OutText(pText))
                        return 0;
                }else{
                    MoveRel(width, 0);
                }
                while((XCHAR)*pText>(XCHAR)15)
                    pText++;
    
                if((XCHAR)*pText == (XCHAR)'\n')
                {
                    MoveRel(0, pEb->textHeight);
                }
                
                // check if end of string
                if (*pText != 0)
                {
                    state = EB_STATE_POSITION_TEXT;
                    pText++;
                    break;
                }    
       			
                // draw the caret if required
                if(!GetState(pEb,EB_DISABLED)){
    	            
                    if((GetState(pEb,EB_FOCUSED) && GetState(pEb,EB_DRAW_CARET)) || (GetState(pEb,EB_CARET)))
                    {
                        SetColor(pEb->hdr.pGolScheme->TextColor0);
                    }
                    else
                    {
                        SetColor(pEb->hdr.pGolScheme->Color0);
                    }
           			state = EB_STATE_CARET;
        		    // no break here since it will always go to the EB_STATE_CARET state
                }
                else
                {
                    SetClip(CLIP_DISABLE);
        			state = EB_STATE_START;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                    GFX_DRIVER_CompleteDrawUpdate(   pEb->hdr.left,
                                                    pEb->hdr.top,
                                                    pEb->hdr.right,
                                                    pEb->hdr.bottom);
#endif
    	    		return 1;
                }
   
            case EB_STATE_CARET:
            
                // draw the caret if required
                if(!Bar(GetX(),GetY(),GetX()+EB_CARET_WIDTH,GetY()+pEb->textHeight)) 
                    return 0;
    
                SetClip(CLIP_DISABLE);
    			state = EB_STATE_START;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pEb->hdr.left,
                                                pEb->hdr.top,
                                                pEb->hdr.right,
                                                pEb->hdr.bottom);
#endif
    			return 1;
    			
        } // switch()
        
    }//  while(!IsDeviceBusy())
        
    return 0;
}

#endif // USE_EDITBOX
