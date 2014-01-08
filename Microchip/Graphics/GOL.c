/*****************************************************************************
 *  Module for Microchip Graphics Library 
 *  GOL Layer 
 *****************************************************************************
 * FileName:        GOL.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
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
 * Date                  Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07              Version 1.0 release
 * 06/29/09				Added multi-line support for Buttons.
 * 03/12/09             Added Double Buffering Support
 * 04/29/10				Fixed GOLGetFocusNext() issue.
 * 06/07/10				To save drawing time, add check on panel to skip 
 * 						drawing the panel face and frame if the bitmap
 * 						used is greater than area of the panel.
 * 09/08/10				Removed redundant code in GOLRedrawRec().
 *                      Added EditBox in GOLCanBeFocused().
 * 03/30/11             In 1 BPP mode: Removed all references to WHITE and 
 *                      BLACK color. Replaced them to use the emboss light 
 *                      and dark color instead.
 * 02/03/12             Added missing cases in GOLRedrawRec().
 * 03/05/12             Enabled Gradient feature when rendering in blocking 
 *                      mode.
 *****************************************************************************/
#include "Graphics/Graphics.h"

#ifdef USE_GOL

#include <string.h>

#ifdef USE_TRANSITION_EFFECTS
    
    #include "Graphics/Transitions.h"
    
    static BYTE TransitionPendingStatus;
#endif

// Pointer to the current linked list of objects displayed and receiving messages
OBJ_HEADER  *_pGolObjects = NULL;

// Pointer to the default GOL scheme (created in GOLInit())
GOL_SCHEME  *_pDefaultGolScheme = NULL;

// Pointer to the object receiving keyboard input
OBJ_HEADER  *_pObjectFocused = NULL;

#ifdef USE_GRADIENT
GFX_GRADIENT_STYLE _gradientScheme;
#endif

    #ifdef USE_FOCUS

/*********************************************************************
* Function: OBJ_HEADER *GOLGetFocusPrev()
*
* Overview: This function returns the pointer to the previous object
*		 	in the active linked list which is able to receive 
*		 	keyboard input.
*
* PreCondition: none
*
* Input: none
*
* Output: This returns the pointer of the previous object in the 
*		  active list capable of receiving keyboard input. If 
*		  there is no object capable of receiving keyboard 
*		  inputs (i.e. none can be focused) NULL is returned.
*
* Side Effects: none
*
********************************************************************/
OBJ_HEADER *GOLGetFocusPrev(void)
{
    OBJ_HEADER  *pPrevObj;
    OBJ_HEADER  *pCurObj;
    OBJ_HEADER  *pFocusedObj;

    if(_pGolObjects == NULL)
        return (NULL);

    if(_pObjectFocused == NULL)
    {
        pFocusedObj = _pGolObjects;
    }
    else
    {
        pFocusedObj = _pObjectFocused;
    }

    pPrevObj = NULL;
    pCurObj = pFocusedObj;

    while(1)
    {
        if(GOLCanBeFocused(pCurObj))
            pPrevObj = pCurObj;

        if(pCurObj->pNxtObj == NULL)
            if(_pGolObjects == pFocusedObj)
                return (pPrevObj);

        if(pCurObj->pNxtObj == pFocusedObj)
            return (pPrevObj);

        pCurObj = (OBJ_HEADER *)pCurObj->pNxtObj;

        if(pCurObj == NULL)
            pCurObj = _pGolObjects;
    }
}

/*********************************************************************
* Function: OBJ_HEADER *GOLGetFocusNext()
*
* Overview: This function returns the pointer to the next object
*		 	in the active linked list which is able to receive 
*		 	keyboard input.
*
* PreCondition: none
*
* Input: none
*
* Output: This returns the pointer of the next object in the 
*		  active list capable of receiving keyboard input. If 
*		  there is no object capable of receiving keyboard 
*		  inputs (i.e. none can be focused) NULL is returned.
*
* Side Effects: none
*
********************************************************************/
OBJ_HEADER *GOLGetFocusNext(void)
{
    OBJ_HEADER  *pNextObj, *pObjStart;

    if(_pGolObjects == NULL)
        return (NULL);

    if(_pObjectFocused == NULL)
    {
        pNextObj = _pGolObjects;
    }
    else
    {
        pNextObj = _pObjectFocused;
    }

	pObjStart = pNextObj;
	
    do
    {
        pNextObj = (OBJ_HEADER *)pNextObj->pNxtObj;

        if(pNextObj == NULL)
            pNextObj = _pGolObjects;

        if(GOLCanBeFocused(pNextObj))
            break;
    } while(pNextObj != pObjStart);

	// if we reached the starting point and the starting point cannot
	// be focused, then all objects cannot be focused. return NULL
    if(!GOLCanBeFocused(pNextObj))
		return NULL;

    return (pNextObj);
}

/*********************************************************************
* Function: void GOLSetFocus(OBJ_HEADER* object)
*
* PreCondition: none
*
* Input: pointer to the object to be focused
*
* Output: 
*
* Side Effects: none
*
* Overview: moves keyboard focus to the object
*
* Note: none
*
********************************************************************/
void GOLSetFocus(OBJ_HEADER *object)
{
    if(!GOLCanBeFocused(object))
        return;

    if(_pObjectFocused != NULL)
    {
        ClrState(_pObjectFocused, FOCUSED);
        SetState(_pObjectFocused, DRAW_FOCUS);
    }

    SetState(object, DRAW_FOCUS | FOCUSED);

    _pObjectFocused = object;
}

/*********************************************************************
* Function: WORD GOLCanBeFocused(OBJ_HEADER* object)
*
* PreCondition: none
*
* Input: pointer to the object 
*
* Output: non-zero if the object supports keyboard focus, zero if not
*
* Side Effects: none
*
* Overview: checks if object support keyboard focus
*
* Note: none
*
********************************************************************/
WORD GOLCanBeFocused(OBJ_HEADER *object)
{
    if
    (
        (object->type == OBJ_BUTTON) ||
        (object->type == OBJ_CHECKBOX) ||
        (object->type == OBJ_RADIOBUTTON) ||
        (object->type == OBJ_LISTBOX) ||
        (object->type == OBJ_SLIDER) ||
        (object->type == OBJ_EDITBOX)
    )
    {
        if(!GetState(object, DISABLED))
            return (1);
    }

    return (0);
}

    #endif //#ifdef USE_FOCUS

/*********************************************************************
* Function: GOL_SCHEME *GOLCreateScheme(void)
*
* PreCondition: none
*
* Input: none
*
* Output: pointer to scheme object
*
* Side Effects: none
*
* Overview: creates a color scheme object and assign default colors
*
* Note: none
*
********************************************************************/
GOL_SCHEME *GOLCreateScheme(void)
{
    GOL_SCHEME  *pTemp;

    pTemp = (GOL_SCHEME *)GFX_malloc(sizeof(GOL_SCHEME));

    if(pTemp != NULL)
    {
        memcpy(pTemp, &GFX_SCHEMEDEFAULT, sizeof(GOL_SCHEME));
    }

    return (pTemp);
}

/*********************************************************************
* Function: void GOLInit()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: initializes GOL
*
* Note: none
*
********************************************************************/
void GOLInit(void)
{

    // Initialize display
    InitGraph();

    // Initialize the default GOL scheme
    _pDefaultGolScheme = GOLCreateScheme();

    #ifdef USE_TRANSITION_EFFECTS

        TransitionPendingStatus = 0;

    #endif
}

/*********************************************************************
* Function: void GOLFree()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: frees memory of all objects in the current linked list
*           and starts a new linked list. This function must be 
*           called only inside the GOLDrawCallback()function when 
*           using GOLDraw() and GOLMsg() to manage rendering and 
*           message processing.
*
* Note: drawing and messaging must be suspended
*
********************************************************************/
void GOLFree(void)
{
    OBJ_HEADER  *pNextObj;
    OBJ_HEADER  *pCurrentObj;

    pCurrentObj = _pGolObjects;
    while(pCurrentObj != NULL)
    {
        pNextObj = (OBJ_HEADER *)pCurrentObj->pNxtObj;
		
		// check if there are additional items to free
        if(pCurrentObj->FreeObj)
            pCurrentObj->FreeObj(pCurrentObj);

        GFX_free(pCurrentObj);
        pCurrentObj = pNextObj;
    }

    GOLNewList();

	#ifdef USE_DOUBLE_BUFFERING
	
	    InvalidateAll();

	#endif
}

/*********************************************************************
* Function: BOOL GOLDeleteObject(OBJ_HEADER * object)
*
* PreCondition: none
*
* Input: pointer to the object
*
* Output: none
*
* Side Effects: none
*
* Overview: Deletes an object to the linked list objects for the current screen.
*
* Note: none
*
********************************************************************/
BOOL GOLDeleteObject(OBJ_HEADER *object)
{
    if(!_pGolObjects)
        return (FALSE);

    if(object == _pGolObjects)
    {
        _pGolObjects = (OBJ_HEADER *)object->pNxtObj;
    }
    else
    {
        OBJ_HEADER  *curr;
        OBJ_HEADER  *prev;

        curr = (OBJ_HEADER *)_pGolObjects->pNxtObj;
        prev = (OBJ_HEADER *)_pGolObjects;

        while(curr)
        {
            if(curr == object)
                break;

            prev = (OBJ_HEADER *)curr;
            curr = (OBJ_HEADER *)curr->pNxtObj;
        }

        if(!curr)
            return (FALSE);

        prev->pNxtObj = curr->pNxtObj;
    }

    if(object->FreeObj)
        object->FreeObj(object);

    GFX_free(object);

    return (TRUE);
}

/*********************************************************************
* Function: BOOL GOLDeleteObject(OBJ_HEADER * object)
*
* PreCondition: none
*
* Input: pointer to the object
*
* Output: none
*
* Side Effects: none
*
* Overview: Deletes an object to the linked list objects for the current screen using
*           the given ID to search for the object.
*
* Note: none
*
********************************************************************/
BOOL GOLDeleteObjectByID(WORD ID)
{
    OBJ_HEADER  *object;

    object = GOLFindObject(ID);

    if(!object)
        return (FALSE);

    return (GOLDeleteObject(object));
}

/*********************************************************************
* Function: OBJ_HEADER* GOLFindObject(WORD ID)
*
* PreCondition: none
*
* Input: object ID
*
* Output: pointer to the object
*
* Side Effects: none
*
* Overview: searches for the object by its ID in the current objects linked list,
*           returns NULL if the object is not found
*
* Note: none
*
********************************************************************/
OBJ_HEADER *GOLFindObject(WORD ID)
{
    OBJ_HEADER  *pNextObj;

    pNextObj = _pGolObjects;
    while(pNextObj != NULL)
    {
        if(pNextObj->ID == ID)
        {
            return (pNextObj);
        }

        pNextObj = (OBJ_HEADER *)pNextObj->pNxtObj;
    }

    return (NULL);
}

/*********************************************************************
* Function: void GOLAddObject(OBJ_HEADER * object)
*
* PreCondition: none
*
* Input: pointer to the object
*
* Output: none
*
* Side Effects: none
*
* Overview: adds an object to the linked list objects for the current screen.
*           Object will be drawn and will receive messages.
*
* Note: none
*
********************************************************************/
void GOLAddObject(OBJ_HEADER *object)
{
    OBJ_HEADER  *pNextObj;

    if(_pGolObjects == NULL)
    {
        _pGolObjects = object;
    }
    else
    {
        pNextObj = _pGolObjects;
        while(pNextObj->pNxtObj != NULL)
        {
            pNextObj = (OBJ_HEADER *)pNextObj->pNxtObj;
        }

        pNextObj->pNxtObj = (void *)object;
    }

    object->pNxtObj = NULL;
}

/*********************************************************************
* Function: WORD GOLDraw()
*
* PreCondition: none
*
* Input: none
*
* Output: non-zero if drawing is complete
*
* Side Effects: none
*
* Overview: redraws objects in the current linked list
*
* Note: none
*
********************************************************************/
WORD GOLDraw(void)
{
    static OBJ_HEADER   *pCurrentObj = NULL;
    SHORT               done;

    #ifdef USE_DOUBLE_BUFFERING
    static BYTE         DisplayUpdated = 0;
    if(IsDisplayUpdatePending())
    {
        return 0;
    }
    #endif // USE_DOUBLE_BUFFERING

    if(pCurrentObj == NULL)
    {
        if(GOLDrawCallback())
        {
            #ifdef USE_DOUBLE_BUFFERING
				#ifdef USE_TRANSITION_EFFECTS
                if(TransitionPendingStatus)
                {
                    TransitionPendingStatus = 0;
	                while(IsDisplayUpdatePending());
                   	GFXExecutePendingTransition(GetDrawBufferAddress(), GetFrameBufferAddress());
                }
				#endif
            #endif //USE_DOUBLE_BUFFERING
            
            // It's last object jump to head
            pCurrentObj = _pGolObjects;

            #ifdef USE_DOUBLE_BUFFERING
            if(DisplayUpdated)
            {
                RequestDisplayUpdate();
                DisplayUpdated = 0;
                return(0);
            }
            #endif //USE_DOUBLE_BUFFERING
        }
        else
        {
            return (0);     // drawing is not done
        }
    }

    done = 0;
    while(pCurrentObj != NULL)
    {
        if(IsObjUpdated(pCurrentObj))
        {
            done = pCurrentObj->DrawObj(pCurrentObj);

            if(done)
            {
                GOLDrawComplete(pCurrentObj);

			#ifdef USE_DOUBLE_BUFFERING

			    InvalidateRectangle(pCurrentObj->left, pCurrentObj->top,
			                        pCurrentObj->right, pCurrentObj->bottom);
                DisplayUpdated = 1;

			#endif //USE_DOUBLE_BUFFERING

            }
            else
            {
                return (0); // drawing is not done
            }
        }

        pCurrentObj = (OBJ_HEADER *)pCurrentObj->pNxtObj;
    }

	#if defined(USE_TRANSITION_EFFECTS) && defined(USE_DOUBLE_BUFFERING)

        TransitionPendingStatus = GFXIsTransitionPending();
	    InvalidateAll(); //If needed, invalidate only the transition rectangle instead

	#endif

    return (1);             // drawing is completed
}

/*********************************************************************
* Function: void GOLRedrawRec(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: none
*
* Input: left,top,right,bottom - rectangle borders
*
* Output: none
*
* Side Effects: none
*
* Overview: marks objects with parts in the rectangle to be redrawn
*
* Note: none
*
********************************************************************/
void GOLRedrawRec(SHORT left, SHORT top, SHORT right, SHORT bottom)
{

   OBJ_HEADER  *pCurrentObj;
    int overlapX, overlapY;

    pCurrentObj = _pGolObjects;

    while(pCurrentObj != NULL)
    {
        overlapX = overlapY = 0;

        // check overlaps in x direction
        if( ((pCurrentObj->left <= left) &&  (pCurrentObj->right >= left))    ||  \
            ((pCurrentObj->left <= right) &&  (pCurrentObj->right >= right))  ||  \
            ((pCurrentObj->left <= left) &&  (pCurrentObj->right >= right))   ||  \
            ((pCurrentObj->left >= left) &&  (pCurrentObj->right <= right))       \
          )
            overlapX = 1;
    
        // check overlaps in y direction
        if( ((pCurrentObj->top <= top) &&  (pCurrentObj->bottom >= top))      ||  \
            ((pCurrentObj->top <= bottom) &&  (pCurrentObj->bottom >= bottom))||  \
            ((pCurrentObj->top <= top) &&  (pCurrentObj->bottom >= bottom))   ||  \
            ((pCurrentObj->top >= top) &&  (pCurrentObj->bottom <= bottom))       \
          )
            overlapY = 1;
        
        // when any portion of the widget is touched by the defined rectangle the
        // x and y overlaps will exist.
        if (overlapX & overlapY)
		{
        	GOLRedraw(pCurrentObj);
        }
    
        pCurrentObj = (OBJ_HEADER *)pCurrentObj->pNxtObj;
    }   //end of while

}

/*********************************************************************
* Function: void GOLMsg(GOL_MSG *pMsg)
*
* PreCondition: none
*
* Input: pointer to the message
*
* Output: none
*
* Side Effects: none
*
* Overview: processes message for all objects in the liked list
*
* Note: none
*
********************************************************************/
void GOLMsg(GOL_MSG *pMsg)
{
    OBJ_HEADER  *pCurrentObj;
    WORD        translatedMsg;

    if(pMsg->uiEvent == EVENT_INVALID)
        return;

    pCurrentObj = _pGolObjects;

    while(pCurrentObj != NULL)
    {
        if(pCurrentObj->MsgObj)
        {
            translatedMsg = pCurrentObj->MsgObj(pCurrentObj, pMsg);

            if(translatedMsg != OBJ_MSG_INVALID)
            {
                if(GOLMsgCallback(translatedMsg, pCurrentObj, pMsg))
                    if(pCurrentObj->MsgDefaultObj)
                        pCurrentObj->MsgDefaultObj(translatedMsg, pCurrentObj, pMsg);
            }
        }

        pCurrentObj = (OBJ_HEADER *)pCurrentObj->pNxtObj;
    }
}

/*********************************************************************
* Variables for rounded panel drawing. Used by GOLRndPanelDraw and GOLRndPanelDrawTsk
********************************************************************/
SHORT   _rpnlX1,        // Center x position of upper left corner
_rpnlY1,                // Center y position of upper left corner
_rpnlX2,                // Center x position of lower right corner
_rpnlY2,                // Center y position of lower right corner
_rpnlR;                 // radius
GFX_COLOR _rpnlFaceColor,   // face color
_rpnlEmbossLtColor,         // emboss light color
_rpnlEmbossDkColor,         // emboss dark color
_rpnlEmbossSize;            // emboss size

#ifdef USE_ALPHABLEND_LITE
WORD        _rpnlAlpha; // alpha value
#endif

void    *_pRpnlBitmap = NULL;

/*********************************************************************
* Function: WORD GOLPanelDrawTsk(void) 
*
* PreCondition: parameters must be set with
*               GOLRndPanelDraw(x,y,radius,width,height,faceClr,embossLtClr,
*								embossDkClr,pBitmap,embossSize)
*
* Input: None
*
* Output: Output: non-zero if drawing is completed
*
* Overview: draws a rounded panel on screen. Must be called repeatedly. Drawing is done
*           when it returns non-zero. 
*
* Note: none
*
********************************************************************/
WORD GOLPanelDrawTsk(void)
{

#ifndef USE_NONBLOCKING_CONFIG

    WORD    counter;

    // check if we need to draw the panels and emboss sides
    if 	(
    		(_pRpnlBitmap == NULL) || 
    		(
    			((_rpnlX2 - _rpnlX1 + (_rpnlR<<1)) > GetImageWidth((void *)_pRpnlBitmap)) 	&& 
            	((_rpnlY2 - _rpnlY1 + (_rpnlR<<1)) > GetImageHeight((void *)_pRpnlBitmap))	&&
    			(_pRpnlBitmap != NULL)	
			)
        )
    {
	    if(_rpnlR)
	    {
	
	        // draw upper left portion of the embossed area
	        SetColor(_rpnlEmbossLtColor);
	        Arc(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize, _rpnlR, 0xE1);
	
	        // draw lower right portion of the embossed area
	        SetColor(_rpnlEmbossDkColor);
	        Arc(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize, _rpnlR, 0x1E);
	    }
	    else
	    {
	
	        // object is rectangular panel draw the embossed areas
	        counter = 1;
	        SetColor(_rpnlEmbossLtColor);
	        while(counter < _rpnlEmbossSize)
	        {
	            Bar(_rpnlX1 + counter, _rpnlY1 + counter, _rpnlX2 - counter, _rpnlY1 + counter);    // draw top
	            Bar(_rpnlX1 + counter, _rpnlY1 + counter, _rpnlX1 + counter, _rpnlY2 - counter);    // draw left
	            counter++;
	        }
	
	        counter = 1;
	        SetColor(_rpnlEmbossDkColor);
	        while(counter < _rpnlEmbossSize)
	        {
	            Bar(_rpnlX1 + counter, _rpnlY2 - counter, _rpnlX2 - counter, _rpnlY2 - counter);    // draw bottom
	            Bar(_rpnlX2 - counter, _rpnlY1 + counter, _rpnlX2 - counter, _rpnlY2 - counter);    // draw right	
	            counter++;
	        }
	    }
	
	    // draw the face color
	    SetColor(_rpnlFaceColor);
#ifdef USE_ALPHABLEND_LITE
        // set alpha value
        SetAlpha(_rpnlAlpha); 
#endif

	    if(_rpnlR)

#ifdef USE_GRADIENT
            if(_gradientScheme.gradientType != GRAD_NONE)
            {
                BevelGradient(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize,
                              _gradientScheme.gradientStartColor,_gradientScheme.gradientEndColor,
                              _gradientScheme.gradientLength,_gradientScheme.gradientType);
            }
            else
#endif
	        FillBevel(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize);
	    else
            { 

             Bar(_rpnlX1 + _rpnlEmbossSize, _rpnlY1 + _rpnlEmbossSize, _rpnlX2 - _rpnlEmbossSize, _rpnlY2 - _rpnlEmbossSize);
	        }
	            #if (COLOR_DEPTH == 1)
	    if(_rpnlFaceColor == _rpnlEmbossDkColor)
	    {
	        SetColor(_rpnlEmbossLtColor);
	        if(_rpnlR)
	            Bevel(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - (_rpnlEmbossSize - 1));
	        else
	            Bevel
	            (
	                _rpnlX1 + (_rpnlEmbossSize - 1),
	                _rpnlY1 + (_rpnlEmbossSize - 1),
	                _rpnlX2 - (_rpnlEmbossSize - 1),
	                _rpnlY2 - (_rpnlEmbossSize - 1),
	                0
	            );
	    }
	
	            #endif
	} // end of check if we need to draw the panels and emboss sides
	
    // draw bitmap
    if(_pRpnlBitmap != NULL)
    {
        PutImage
        (
            (((_rpnlX2 + _rpnlX1) - (GetImageWidth((void *)_pRpnlBitmap))) >> 1) + 1,
            (((_rpnlY2 + _rpnlY1) - (GetImageHeight((void *)_pRpnlBitmap))) >> 1) + 1,
            _pRpnlBitmap,
            IMAGE_NORMAL
        );
   }

#ifdef USE_ALPHABLEND_LITE
    //Disable Alpha Blending
    SetAlpha(100); 
#endif

    // check if we need to draw the frame
    if
    (
        (_pRpnlBitmap == NULL) ||
        (
            ((_rpnlX2 - _rpnlX1 + _rpnlR) >= GetImageWidth((void *)_pRpnlBitmap)) &&
            ((_rpnlY2 - _rpnlY1 + _rpnlR) >= GetImageHeight((void *)_pRpnlBitmap))
        )
    )
    {

        // draw the outline
        SetColor(_rpnlEmbossDkColor);
        Bevel(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR);
    }

    return (1);

#else

    typedef enum
    {
        BEGIN,
        ARC1,
        DRAW_EMBOSS1,
        DRAW_EMBOSS2,
        DRAW_EMBOSS3,
        DRAW_EMBOSS4,
        DRAW_FACECOLOR,
                #if (COLOR_DEPTH == 1)
        DRAW_INNERFRAME,
                #endif
        DRAW_FRAME,
        DRAW_IMAGE,
    } ROUND_PANEL_DRAW_STATES;

    static ROUND_PANEL_DRAW_STATES state = BEGIN;
    static WORD counter;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);
        switch(state)
        {
            case BEGIN:

	           	// check if we need to draw the face and emboss
                if (_pRpnlBitmap != NULL)
                {
                    if 	(
                        ((_rpnlX2 - _rpnlX1 + (_rpnlR<<1)) <= GetImageWidth((void *)_pRpnlBitmap)) &&
                        ((_rpnlY2 - _rpnlY1 + (_rpnlR<<1)) <= GetImageHeight((void *)_pRpnlBitmap))
                    	)
                	{
	                	state = DRAW_IMAGE;
	                	goto rnd_panel_draw_image;
	                }	
	            } 
            
                if(_rpnlR)
                {

                    // draw upper left portion of the embossed area
                    SetColor(_rpnlEmbossLtColor);
                    if(!Arc(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize, _rpnlR, 0xE1))
                        return (0);
                    state = ARC1;
                }
                else
                {
                    state = DRAW_EMBOSS1;
                    counter = 1;
                    goto rnd_panel_draw_emboss;
                }

            case ARC1:

                // draw upper left portion of the embossed area
                SetColor(_rpnlEmbossDkColor);
                if(!Arc(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize, _rpnlR, 0x1E))
                    return (0);
                state = DRAW_FACECOLOR;
                goto rnd_panel_draw_facecolor;

            // now draw the upper portion of the embossed area
            case DRAW_EMBOSS1:
                rnd_panel_draw_emboss : SetColor(_rpnlEmbossLtColor);
                while(counter < _rpnlEmbossSize)
                {

                    // draw top
                    if(!Bar(_rpnlX1 + counter, _rpnlY1 + counter, _rpnlX2 - counter, _rpnlY1 + counter))
                    {
                        return (0);
                    }

                    counter++;
                }

                counter = 1;
                state = DRAW_EMBOSS2;
                break;

            case DRAW_EMBOSS2:
                while(counter < _rpnlEmbossSize)
                {

                    // draw left   	
                    if(!Bar(_rpnlX1 + counter, _rpnlY1 + counter, _rpnlX1 + counter, _rpnlY2 - counter))
                    {
                        return (0);
                    }

                    counter++;
                }

                counter = 1;
                state = DRAW_EMBOSS3;
                break;

            // now draw the lower portion of the embossed area
            case DRAW_EMBOSS3:
                SetColor(_rpnlEmbossDkColor);
                while(counter < _rpnlEmbossSize)
                {

                    // draw bottom
                    if(!Bar(_rpnlX1 + counter, _rpnlY2 - counter, _rpnlX2 - counter, _rpnlY2 - counter))
                    {
                        return (0);
                    }

                    counter++;
                }

                counter = 1;
                state = DRAW_EMBOSS4;
                break;

            case DRAW_EMBOSS4:
                while(counter < _rpnlEmbossSize)
                {

                    // draw right	   	
                    if(!Bar(_rpnlX2 - counter, _rpnlY1 + counter, _rpnlX2 - counter, _rpnlY2 - counter))
                    {
                        return (0);
                    }

                    counter++;
                }

                state = DRAW_FACECOLOR;
                break;

            // draw the face color
            case DRAW_FACECOLOR:
                rnd_panel_draw_facecolor : SetColor(_rpnlFaceColor);
#ifdef USE_ALPHABLEND_LITE
                // set alpha value
                SetAlpha(_rpnlAlpha); 
#endif

                if(_rpnlR)
                {
                    #ifdef USE_GRADIENT
                    if(_gradientScheme.gradientType != GRAD_NONE)
                    {
                       BevelGradient(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize,
                                     _gradientScheme.gradientStartColor,_gradientScheme.gradientEndColor,
                                    _gradientScheme.gradientLength,_gradientScheme.gradientType);
                    }
                   else
                   #endif



                    if(!FillBevel(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize))
                        return (0);
                }
                else
                {
                    if
                    (
                        !Bar
                            (
                                _rpnlX1 + _rpnlEmbossSize,
                                _rpnlY1 + _rpnlEmbossSize,
                                _rpnlX2 - _rpnlEmbossSize,
                                _rpnlY2 - _rpnlEmbossSize
                            )
                    )
                    {
                        return (0);
                    }
                }

                state = DRAW_IMAGE;
                break;

            case DRAW_IMAGE:
            	rnd_panel_draw_image : 
                if(_pRpnlBitmap != NULL)
                {
                    if
                    (
                        !PutImage
                            (
                                ((_rpnlX2 + _rpnlX1 - GetImageWidth((void *)_pRpnlBitmap)) >> 1) + 1,
                                ((_rpnlY2 + _rpnlY1 - GetImageHeight((void *)_pRpnlBitmap)) >> 1) + 1,
                                _pRpnlBitmap,
                                IMAGE_NORMAL
                            )
                    )
                    {
                        return (0);
                    }
                }
#ifdef USE_ALPHABLEND_LITE
                //Disable Alpha Blending
                SetAlpha(100); 
#endif


                        #if (COLOR_DEPTH == 1)
                state = DRAW_INNERFRAME;
                break;
                        #else
                state = DRAW_FRAME;
                        #endif
                break;

                        #if (COLOR_DEPTH == 1)

            case DRAW_INNERFRAME:
                if(_rpnlFaceColor == _rpnlEmbossDkColor)
                {
                    SetColor(_rpnlEmbossLtColor); 
					if(_rpnlR)
					{
                        if(!Bevel(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - (_rpnlEmbossSize - 1)))
                        {
                            return (0);
                        }    
					}
					else
    				{
					    if(!Bevel(  _rpnlX1 + (_rpnlEmbossSize - 1),
                                    _rpnlY1 + (_rpnlEmbossSize - 1),
                                    _rpnlX2 - (_rpnlEmbossSize - 1),
                                    _rpnlY2 - (_rpnlEmbossSize - 1),
                                    0 ))
				        {
							return (0);
						}
					}
                }

                state = DRAW_FRAME;
                break;
                        #endif

            case DRAW_FRAME:

                // check if we need to draw the frame
                if
                (
                    (_pRpnlBitmap == NULL) ||
                    (
                        ((_rpnlX2 - _rpnlX1 + _rpnlR) >= GetImageWidth((void *)_pRpnlBitmap)) &&
                        ((_rpnlY2 - _rpnlY1 + _rpnlR) >= GetImageHeight((void *)_pRpnlBitmap))
                    )
                )
                {

                    // draw the outline frame
                            #if (COLOR_DEPTH == 1)
                    // When in 1BPP mode, the outline should always be the light color 
                    // Ideally WHITE.   
                    if (_rpnlEmbossLtColor > _rpnlEmbossDkColor)
                        SetColor(_rpnlEmbossLtColor);     
                    else
                        SetColor(_rpnlEmbossDkColor);     
                            #else
                    SetColor(_rpnlEmbossDkColor);
                            #endif
                    if(!Bevel(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR))
                    {
                        return (0);
                    }
                }

                state = BEGIN;
                return (1);
        }   // end of switch
    }       // end of while
#endif //#ifndef USE_NONBLOCKING_CONFIG
}


/*********************************************************************
* Function: WORD GOLTwoTonePanelDrawTsk(void) 
*
* PreCondition: parameters must be set with
*               GOLRndPanelDraw(x,y,radius,width,height,faceClr,embossLtClr,
*								embossDkClr,pBitmap,embossSize)
*
* Input: None
*
* Output: Output: non-zero if drawing is completed
*
* Overview: draws a rounded panel on screen. Must be called repeatedly. Drawing is done
*           when it returns non-zero. 
*
* Note: none
*
********************************************************************/
WORD GOLTwoTonePanelDrawTsk(void)
{
// In this panel draw task the emboss light and dark colors are used as
// the panel face colors and the panel face color is used as an outline color

        #ifndef USE_NONBLOCKING_CONFIG

    WORD    counter;

	SetColor(_rpnlFaceColor);
    if(_rpnlR)
    {
        // draw the outline
        Arc(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize, _rpnlR, 0xFF);  	
    }
    else
    {
        // object is rectangular panel draw the outline embossed areas
        counter = 1;
        while(counter < _rpnlEmbossSize)
        {
            Bar(_rpnlX1 + counter, _rpnlY1 + counter, _rpnlX2 - counter, _rpnlY1 + counter);    // draw top
            Bar(_rpnlX1 + counter, _rpnlY1 + counter, _rpnlX1 + counter, _rpnlY2 - counter);    // draw left
            Bar(_rpnlX1 + counter, _rpnlY2 - counter, _rpnlX2 - counter, _rpnlY2 - counter);    // draw bottom
            Bar(_rpnlX2 - counter, _rpnlY1 + counter, _rpnlX2 - counter, _rpnlY2 - counter);    // draw right	
            counter++;
        }        
    }
	
	// draw the top half of the face 
    SetColor(_rpnlEmbossLtColor);
    if(_rpnlR)
    {
    	SetBevelDrawType(DRAWTOPBEVEL);
        FillBevel(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize);
    }
    else
    {
        Bar(_rpnlX1 + _rpnlEmbossSize, _rpnlY1 + _rpnlEmbossSize, 
        	_rpnlX2 - _rpnlEmbossSize, (_rpnlY1 + ((_rpnlY2 - _rpnlY1) >> 1)));
	}

	// draw the bottom half of the face 
    SetColor(_rpnlEmbossDkColor);
    if(_rpnlR)
    {
    	SetBevelDrawType(DRAWBOTTOMBEVEL);
        FillBevel(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize);
    }
    else
    {
        Bar(_rpnlX1 + _rpnlEmbossSize, (_rpnlY1 + ((_rpnlY2 - _rpnlY1) >> 1)) + 1, 
        	_rpnlX2 - _rpnlEmbossSize, _rpnlY2 - _rpnlEmbossSize);
	}
	
	SetBevelDrawType(DRAWFULLBEVEL);


            #if (COLOR_DEPTH == 1)
    if(_rpnlFaceColor == _rpnlEmbossDkColor)
    {
        SetColor(_rpnlEmbossLtColor);
        if(_rpnlR)
            Bevel(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - (_rpnlEmbossSize - 1));
        else
            Bevel
            (
                _rpnlX1 + (_rpnlEmbossSize - 1),
                _rpnlY1 + (_rpnlEmbossSize - 1),
                _rpnlX2 - (_rpnlEmbossSize - 1),
                _rpnlY2 - (_rpnlEmbossSize - 1),
                0
            );
    }

            #endif

    // draw bitmap
    if(_pRpnlBitmap != NULL)
    {
        PutImage
        (
            (((_rpnlX2 + _rpnlX1) - (GetImageWidth((void *)_pRpnlBitmap))) >> 1) + 1,
            (((_rpnlY2 + _rpnlY1) - (GetImageHeight((void *)_pRpnlBitmap))) >> 1) + 1,
            _pRpnlBitmap,
            IMAGE_NORMAL
        );
    }

    return (1);

        #else

    typedef enum
    {
        BEGIN,
        DRAW_EMBOSS1,
        DRAW_EMBOSS2,
        DRAW_EMBOSS3,
        DRAW_EMBOSS4,
        DRAW_FACECOLOR1,
        DRAW_FACECOLOR2,
                #if (COLOR_DEPTH == 1)
        DRAW_INNERFRAME,
                #endif
        DRAW_IMAGE,
    } ROUND_PANEL_DRAW_STATES;

    static ROUND_PANEL_DRAW_STATES state = BEGIN;
    static WORD counter;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);
        switch(state)
        {
            case BEGIN:
                if(_rpnlR)
                {

                    // draw the outline
					SetColor(_rpnlFaceColor);
        			if(!Arc(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize, _rpnlR, 0xFF))  	                    
                        return (0);
                    state = DRAW_FACECOLOR1;
                }
                else
                {
                    state = DRAW_EMBOSS1;
                    counter = 1;
                    goto rnd_panel_draw_emboss;
                }

            // now draw the upper portion of the embossed area
            case DRAW_EMBOSS1:
                rnd_panel_draw_emboss : SetColor(_rpnlFaceColor);
                while(counter < _rpnlEmbossSize)
                {

                    // draw top
                    if(!Bar(_rpnlX1 + counter, _rpnlY1 + counter, _rpnlX2 - counter, _rpnlY1 + counter))
                    {
                        return (0);
                    }

                    counter++;
                }

                counter = 1;
                state = DRAW_EMBOSS2;
                break;

            case DRAW_EMBOSS2:
                while(counter < _rpnlEmbossSize)
                {

                    // draw left   	
                    if(!Bar(_rpnlX1 + counter, _rpnlY1 + counter, _rpnlX1 + counter, _rpnlY2 - counter))
                    {
                        return (0);
                    }

                    counter++;
                }

                counter = 1;
                state = DRAW_EMBOSS3;
                break;

            // now draw the lower portion of the embossed area
            case DRAW_EMBOSS3:
                //SetColor(_rpnlEmbossDkColor);
                while(counter < _rpnlEmbossSize)
                {

                    // draw bottom
                    if(!Bar(_rpnlX1 + counter, _rpnlY2 - counter, _rpnlX2 - counter, _rpnlY2 - counter))
                    {
                        return (0);
                    }

                    counter++;
                }

                counter = 1;
                state = DRAW_EMBOSS4;
                break;

            case DRAW_EMBOSS4:
                while(counter < _rpnlEmbossSize)
                {

                    // draw right	   	
                    if(!Bar(_rpnlX2 - counter, _rpnlY1 + counter, _rpnlX2 - counter, _rpnlY2 - counter))
                    {
                        return (0);
                    }

                    counter++;
                }

                state = DRAW_FACECOLOR1;
                break;

            // draw the top half of the face 
            case DRAW_FACECOLOR1:
                SetColor(_rpnlEmbossLtColor);
                if(_rpnlR)
                {
    				SetBevelDrawType(DRAWTOPBEVEL);
        			if(!FillBevel(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize))	                
                        return (0);
                }
                else
                {
                    if
                    (
                        !Bar
                            (
        						_rpnlX1 + _rpnlEmbossSize, 
        						_rpnlY1 + _rpnlEmbossSize, 
        						_rpnlX2 - _rpnlEmbossSize, 
        						(_rpnlY1 + ((_rpnlY2 - _rpnlY1) >> 1))                            
                            )
                    )
                    {
                        return (0);
                    }
                }

                state = DRAW_FACECOLOR2;
                break;

           	// draw the bottom half of the face 
            case DRAW_FACECOLOR2:
                SetColor(_rpnlEmbossDkColor);
                if(_rpnlR)
                {
	                SetBevelDrawType(DRAWBOTTOMBEVEL);
                    if(!FillBevel(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - _rpnlEmbossSize))
                        return (0);
                }
                else
                {
                    if
                    (
                        !Bar
                            (
      							_rpnlX1 + _rpnlEmbossSize, 
      							(_rpnlY1 + ((_rpnlY2 - _rpnlY1) >> 1)) + 1, 
        						_rpnlX2 - _rpnlEmbossSize, 
        						_rpnlY2 - _rpnlEmbossSize
                            )
                    )
                    {
                        return (0);
                    }
                }
				SetBevelDrawType(DRAWFULLBEVEL);
                state = DRAW_IMAGE;
                break;

            case DRAW_IMAGE:
                if(_pRpnlBitmap != NULL)
                {
                    if
                    (
                        !PutImage
                            (
                                ((_rpnlX2 + _rpnlX1 - GetImageWidth((void *)_pRpnlBitmap)) >> 1) + 1,
                                ((_rpnlY2 + _rpnlY1 - GetImageHeight((void *)_pRpnlBitmap)) >> 1) + 1,
                                _pRpnlBitmap,
                                IMAGE_NORMAL
                            )
                    )
                    {
                        return (0);
                    }
                }

                        #if (COLOR_DEPTH == 1)
                state = DRAW_INNERFRAME;
                break;
                        #else
                state = BEGIN;
                return (1);
                        #endif
                break;

                        #if (COLOR_DEPTH == 1)

            case DRAW_INNERFRAME:
                if(_rpnlFaceColor == _rpnlEmbossDkColor)
                {
                    SetColor(_rpnlEmbossLtColor);
					if(_rpnlR)
					{
                        if(!Bevel(_rpnlX1, _rpnlY1, _rpnlX2, _rpnlY2, _rpnlR - (_rpnlEmbossSize - 1)))
                        {
                            return (0);
                        }    
					}
					else
    				{
					    if(!Bevel(  _rpnlX1 + (_rpnlEmbossSize - 1),
                                    _rpnlY1 + (_rpnlEmbossSize - 1),
                                    _rpnlX2 - (_rpnlEmbossSize - 1),
                                    _rpnlY2 - (_rpnlEmbossSize - 1),
                                    0 ))
				        {
							return (0);
						}
					}
                }

                state = BEGIN;
                return (1);
                        #endif

        }   // end of switch
    }       // end of while
        #endif //#ifndef USE_NONBLOCKING_CONFIG
}


#endif // USE_GOL
