/*****************************************************************************
 *  Module for Microchip Graphics Library 
 *  GOL Layer 
 *  List Box
 *****************************************************************************
 * FileName:        ListBox.c
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
 * Date        	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07		Version 1.0 release
 * 11/18/10		Fixed bug on icons drawn after the bottom edge of the list box.
 * 12/21/10     Fixed bug on the list count to rest back to zero when 
 *              LbDelItemsList() is used to clear the list. 
 * 04/20/11     Fixed KEYBOARD bug on object ID and GOL_MSG param1 comparison.
 * 08/05/11     Modified drawing state to isolate primitive calls in case
 *              the primitive function is hardware accelerated.
 *****************************************************************************/
#include "Graphics/Graphics.h"

#ifdef USE_LISTBOX

/*********************************************************************
* Function: LISTBOX  *LbCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, 
*				               WORD state, XCHAR* pText, GOL_SCHEME *pScheme)
*
* Overview: creates the list box
*
********************************************************************/
LISTBOX *LbCreate
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
    LISTBOX *pLb = NULL;
    XCHAR   *pointer;
    WORD    counter;

    pLb = (LISTBOX *)GFX_malloc(sizeof(LISTBOX));

    if(pLb == NULL)
        return (pLb);

    pLb->hdr.ID         = ID;
    pLb->hdr.pNxtObj    = NULL;
    pLb->hdr.type       = OBJ_LISTBOX;
    pLb->hdr.left       = left;
    pLb->hdr.top        = top;
    pLb->hdr.right      = right;
    pLb->hdr.bottom     = bottom;
    pLb->hdr.state      = state;
    pLb->pItemList      = NULL;
    pLb->scrollY        = 0;
    pLb->itemsNumber    = 0;
    pLb->hdr.DrawObj    = LbDraw;			// draw function
    pLb->hdr.MsgObj     = LbTranslateMsg;   // message function
    pLb->hdr.MsgDefaultObj = LbMsgDefault;  // default message function
    pLb->hdr.FreeObj    = LbDelItemsList;	// free function

    // Set the style scheme to be used
    if(pScheme == NULL)
        pLb->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pLb->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    pLb->textHeight = GetTextHeight(pLb->hdr.pGolScheme->pFont);

    GOLAddObject((OBJ_HEADER *)pLb);

    // Add items if there's an initialization text (each line will become one item,
    // lines must be separated by '\n' character)
    if(pText != NULL)
    {
        pointer = pText;
        counter = 0;
        while(*pointer)
        {
            if(NULL == LbAddItem(pLb, NULL, pointer, NULL, 0, counter))
                break;
            while((XCHAR) *pointer++ > (XCHAR)31);
            if(*(pointer - 1) == 0)
                break;
            counter++;
        }
    }

    pLb->pFocusItem = pLb->pItemList;

    // Set focus for the object if FOCUSED state is set
        #ifdef USE_FOCUS
    if(GetState(pLb, LB_FOCUSED))
        GOLSetFocus((OBJ_HEADER *)pLb);
        #endif
    return (pLb);
}

/*********************************************************************
* Function: LISTITEM* LbAddItem(LISTBOX *pLb, LISTITEM *pPrevItem, XCHAR *pText, void* pBitmap, WORD status, WORD data)
*
* Input: pLb - the pointer to the list box,
*        pPrevItem - pointer to the item after which a new item must be inserted,
*                    if this pointer is NULL, the item will be appended at the end of the items list,
*        pText -  pointer to the text,
*        pBitmap - pointer to the bitmap,
*        status - status after creation,
*        data -  some data associated with the item
*
* Output: pointer to the item created, NULL if the operation was not successful
*
* Overview: allocates memory for a new item and adds it to the list box
*
********************************************************************/
LISTITEM *LbAddItem(LISTBOX *pLb, LISTITEM *pPrevItem, XCHAR *pText, void *pBitmap, WORD status, WORD data)
{
    LISTITEM    *pItem;
    LISTITEM    *pCurItem;

    pItem = (LISTITEM *)GFX_malloc(sizeof(LISTITEM));

    if(pItem == NULL)
    {
        return (NULL);
    }

    pLb->itemsNumber++;

    if(pLb->pItemList == NULL)
    {
        pLb->pItemList = pItem;
        pItem->pNextItem = NULL;
        pItem->pPrevItem = NULL;
    }
    else
    {
        if(pPrevItem == NULL)
        {

            // Find last item
            pCurItem = (LISTITEM *)pLb->pItemList;
            while(pCurItem->pNextItem != NULL)
                pCurItem = (LISTITEM *)pCurItem->pNextItem;
        }
        else
        {
            pCurItem = (LISTITEM *)pPrevItem;
        }

        // Insert a new item
        pItem->pNextItem = pCurItem->pNextItem;
        pItem->pPrevItem = pCurItem;
        pCurItem->pNextItem = pItem;
    }

    pItem->pText = pText;
    pItem->pBitmap = pBitmap;
    pItem->status = status;
    pItem->data = data;

    return (pItem);
}

/*********************************************************************
* Function: void LbDelItem(LISTBOX *pLb, LISTITEM *pItem)
*
* Input: pLb - the pointer to the object,
*        pItem - the pointer to the item must be deleted
*
* Output: none
*
* Overview: removes an item from the list box and frees memory
*
********************************************************************/
void LbDelItem(LISTBOX *pLb, LISTITEM *pItem)
{
    if(pItem->pNextItem != NULL)
    {
        ((LISTITEM *)pItem->pNextItem)->pPrevItem = pItem->pPrevItem;
        if(pItem->pPrevItem == NULL)
            pLb->pItemList = (LISTITEM *)pItem->pNextItem;
    }

    if(pItem->pPrevItem != NULL)
        ((LISTITEM *)pItem->pPrevItem)->pNextItem = pItem->pNextItem;

    GFX_free(pItem);

    pLb->itemsNumber--;

    if(pLb->itemsNumber == 0)
        pLb->pItemList = NULL;
}

/*********************************************************************
* Function: void LbDelItemsList(void *pObj)
*
* Input: pLb - the pointer to the object
*
* Output: none
*
* Overview: removes all items from list box and frees memory
*
********************************************************************/
void LbDelItemsList(void *pObj)
{
    LISTITEM    *pCurItem;
    LISTITEM    *pItem;
    LISTBOX     *pLb;

    pLb = (LISTBOX *)pObj;

    pCurItem = pLb->pItemList;

    while(pCurItem != NULL)
    {
        pItem = pCurItem;
        pCurItem = (LISTITEM *)pCurItem->pNextItem;
        GFX_free(pItem);
    }

    pLb->pItemList = NULL;
	pLb->itemsNumber = 0;  
}

/*********************************************************************
* Function: LISTITEM* LbGetSel(LISTBOX *pLb, LISTITEM *pFromItem)
*
* Input: pLb - the pointer to the object,
*        pFromItem - pointer to the item the seach must start from,
*                    if the pointer is NULL the search begins from the start of the items list
*
* Output: pointer to the selected item, NULL if there are no items selected
*
* Overview: seaches for selected items
*
********************************************************************/
LISTITEM *LbGetSel(LISTBOX *pLb, LISTITEM *pFromItem)
{
    if(pFromItem == NULL)
    {
        pFromItem = pLb->pItemList;
    }

    while(pFromItem != NULL)
    {
        if(pFromItem->status & LB_STS_SELECTED)
            break;
        pFromItem = (LISTITEM *)pFromItem->pNextItem;
    }

    return (pFromItem);
}

/*********************************************************************
* Function: void LbChangeSel(LISTBOX *pLb, LISTITEM *pItem)
*
* Input: pLb - the pointer to the object,
*        pItem - pointer to the item the selection status will be changed
*
* Output: none
*
* Overview: changes selection status of an item
*
********************************************************************/
void LbChangeSel(LISTBOX *pLb, LISTITEM *pItem)
{
    LISTITEM    *pCurItem;

    if(GetState(pLb, LB_SINGLE_SEL))
    {

        // Remove selection from all items
        pCurItem = pLb->pItemList;
        while(pCurItem != NULL)
        {
            if(pCurItem->status & LB_STS_SELECTED)
            {
                pCurItem->status &= ~LB_STS_SELECTED;
                pCurItem->status |= LB_STS_REDRAW;
            }

            pCurItem = (LISTITEM *)pCurItem->pNextItem;
        }
    }

    pItem->status ^= LB_STS_SELECTED;
    pItem->status |= LB_STS_REDRAW;
}

/*********************************************************************
* Function: void LbSetFocusedItem(LISTBOX* pLb, SHORT index)
*
* Input: pLb - the pointer to the object
*        index - item number from the list beginning
*
* Output: none
*
* Overview: sets focus for the item with index defined
*
********************************************************************/
void LbSetFocusedItem(LISTBOX *pLb, SHORT index)
{
    LISTITEM    *pCurItem;

    // Look for item to be focused
    pCurItem = pLb->pItemList;

	if(pCurItem == NULL)
		return;

    while(pCurItem->pNextItem != NULL)
    {
        if(index <= 0)
            break;
        index--;
        pCurItem = (LISTITEM *)pCurItem->pNextItem;
    }

    if(pCurItem != NULL)
    {
        if(pLb->pFocusItem != NULL)
        {
            pLb->pFocusItem->status |= LB_STS_REDRAW;
        }

        pLb->pFocusItem = pCurItem;
        pCurItem->status |= LB_STS_REDRAW;
    }
}

/*********************************************************************
* Function: SHORT LbGetFocusedItem(LISTBOX* pLb)
*
* Input: pLb - the pointer to the list box
*
* Output: index of the focused item, -1 if there's no item in focus
*
* Overview: returns focused item number from the list beginning
*
********************************************************************/
SHORT LbGetFocusedItem(LISTBOX *pLb)
{
    LISTITEM    *pCurItem;
    SHORT       index;

    if(pLb->pFocusItem == NULL)
        return (-1);

    // Look for the focused item index
    index = 0;
    pCurItem = pLb->pItemList;
    while(pCurItem->pNextItem != NULL)
    {
        if(pCurItem == pLb->pFocusItem)
            break;
        index++;
        pCurItem = (LISTITEM *)pCurItem->pNextItem;
    }

    return (index);
}

/*********************************************************************
* Function: WORD LbTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: translates the GOL message for the list box
*
********************************************************************/
WORD LbTranslateMsg(void *pObj, GOL_MSG *pMsg)
{
    LISTBOX *pLb;

    pLb = (LISTBOX *)pObj;

    // Evaluate if the message is for the list box
    // Check if disabled first
    if(GetState(pLb, LB_DISABLED))
        return (OBJ_MSG_INVALID);

        #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {

        // Check if it falls in list box borders
		// We do not check any touch events here so application can have 
		// the versatility to define the action on any of the touch events that 
		// will be peformed on the list box. Once application sees the LB_MSG_TOUCHSCREEN
		// reply (returned by this translate message function when touch was performed 
		// on the list box), application can define any behavior for the list box.
        if
        (
            (pLb->hdr.left < pMsg->param1) &&
            (pLb->hdr.right > pMsg->param1) &&
            (pLb->hdr.top < pMsg->param2) &&
            (pLb->hdr.bottom > pMsg->param2)
        ) return (LB_MSG_TOUCHSCREEN);

        return (OBJ_MSG_INVALID);
    }

        #endif
        #ifdef USE_KEYBOARD
    if(pMsg->type == TYPE_KEYBOARD)
        if((WORD)pMsg->param1 == pLb->hdr.ID)
        {
			// Check of the event is done here to properly detect that a keyscan event occured.
            // LB_MSG_MOVE can move the highlight (selection) of the items on the list box when 
			// scan up or down was performed (example: pressing up or down keys) or LB_MSG_SEL  
            // can indicate to the application that the highlighted item is selected by the user.
            // Application can then call the application function that corresponds to the selected item.
            if(pMsg->uiEvent == EVENT_KEYSCAN)
            {
                if((pMsg->param2 == SCAN_UP_PRESSED) || (pMsg->param2 == SCAN_DOWN_PRESSED))
                    return (LB_MSG_MOVE);

                if((pMsg->param2 == SCAN_SPACE_PRESSED) || (pMsg->param2 == SCAN_CR_PRESSED))
                    return (LB_MSG_SEL);
            }

            return (OBJ_MSG_INVALID);
        }

        #endif
    return (OBJ_MSG_INVALID);
}

/*********************************************************************
* Function: void LbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
*
* Overview: changes the state of the list box by default
*
********************************************************************/
void LbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
{

    LISTBOX *pLb;

    pLb = (LISTBOX *)pObj;

        #ifdef USE_TOUCHSCREEN

    SHORT       pos;
    LISTITEM    *pItem;

    if(pMsg->type == TYPE_TOUCHSCREEN)
    {
                #ifdef USE_FOCUS
        if(pMsg->type == TYPE_TOUCHSCREEN)
        {
            if(!GetState(pLb, LB_FOCUSED))
            {
                GOLSetFocus((OBJ_HEADER *)pLb);
            }
        }

                #endif
        if(pMsg->uiEvent == EVENT_PRESS)
        {
            pos = (pMsg->param2 - pLb->scrollY - pLb->hdr.top - LB_INDENT - GOL_EMBOSS_SIZE) / pLb->textHeight;
            pItem = (LISTITEM *)pLb->pItemList;
            while(pos)
            {
                if(pItem == NULL)
                    break;
                if(pItem->pNextItem == NULL)
                    break;
                pItem = (LISTITEM *)pItem->pNextItem;
                pos--;
            }

            if(pLb->pFocusItem != pItem)
            {
                pItem->status |= LB_STS_REDRAW;
                pLb->pFocusItem->status |= LB_STS_REDRAW;
                pLb->pFocusItem = pItem;
                SetState(pLb, LB_DRAW_ITEMS);
            }

            LbChangeSel(pLb, pItem);
            SetState(pLb, LB_DRAW_ITEMS);
        }
    }

        #endif
        #ifdef USE_KEYBOARD
    if(pMsg->type == TYPE_KEYBOARD)
    {
        switch(translatedMsg)
        {
            case LB_MSG_SEL:
                if(pLb->pFocusItem != NULL)
                    LbChangeSel(pLb, pLb->pFocusItem);
                SetState(pLb, LB_DRAW_ITEMS);
                break;

            case LB_MSG_MOVE:
                switch(pMsg->param2)
                {
                    case SCAN_UP_PRESSED:
                        LbSetFocusedItem(pLb, LbGetFocusedItem(pLb) - 1);
                        SetState(pLb, LB_DRAW_ITEMS);
                        break;

                    case SCAN_DOWN_PRESSED:
                        LbSetFocusedItem(pLb, LbGetFocusedItem(pLb) + 1);
                        SetState(pLb, LB_DRAW_ITEMS);
                        break;
                }

                break;
        }
    }   // end of if
        #endif
}

/*********************************************************************
* Function: WORD LbDraw(void *pObj)
*
* Output: returns the status of the drawing
*		  0 - not completed
*         1 - done
*
* Overview: draws edit box
*
********************************************************************/
WORD LbDraw(void *pObj)
{
    typedef enum
    {
        LB_STATE_START,
        LB_STATE_PANEL,
        LB_STATE_DRAW_ITEMS,
        LB_STATE_DRAW_CURRENT_ITEMS,
        LB_STATE_SET_ERASEITEM,
        LB_STATE_ERASEITEM,
        LB_STATE_SET_ITEMFOCUS,
        LB_STATE_ITEMFOCUS,
        LB_STATE_GET_NEXTITEM,
        LB_STATE_BITMAP,
        LB_STATE_TEXT
    } LB_DRAW_STATES;

    static LB_DRAW_STATES state = LB_STATE_START;
    static LISTITEM *pCurItem;
    static GFX_COLOR tempColor;
    static SHORT temp;
    LISTBOX *pLb;

    pLb = (LISTBOX *)pObj;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);

        switch(state)
        {
            // location of this case is strategic so the partial redraw 
            // of the ListBox will be faster        
            case LB_STATE_PANEL:
                if(!GOLPanelDrawTsk())
                    return (0);

                state = LB_STATE_DRAW_ITEMS;
                break;

            case LB_STATE_START:
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_SetupDrawUpdate( pLb->hdr.left,
                                            pLb->hdr.top,
                                            pLb->hdr.right,
                                            pLb->hdr.bottom);
#endif
                /////////////////////////////////////////////////////////////////////
                // REMOVE FROM SCREEN
                /////////////////////////////////////////////////////////////////////
                if(GetState(pLb, LB_HIDE))
                {
                    SetColor(pLb->hdr.pGolScheme->CommonBkColor);
                    if(!Bar(pLb->hdr.left, pLb->hdr.top, pLb->hdr.right, pLb->hdr.bottom))
                        return (0);
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                    GFX_DRIVER_CompleteDrawUpdate(   pLb->hdr.left,
                                                    pLb->hdr.top,
                                                    pLb->hdr.right,
                                                    pLb->hdr.bottom);
#endif

                    return (1);
                }
    
                if(GetState(pLb, LB_DRAW_FOCUS))
                {
                    if(pLb->pFocusItem != NULL)
                        ((LISTITEM *)pLb->pFocusItem)->status |= LB_STS_REDRAW;
                    SetState(pLb, LB_DRAW_ITEMS);
                }
    
                /////////////////////////////////////////////////////////////////////
                // DRAW PANEL
                /////////////////////////////////////////////////////////////////////
                if(GetState(pLb, LB_DRAW))
                {
                    if(GetState(pLb, LB_DISABLED))
                    {
                        tempColor = pLb->hdr.pGolScheme->ColorDisabled;
                    }
                    else
                    {
                        tempColor = pLb->hdr.pGolScheme->Color0;
                    }
    
                    GOLPanelDraw
                    (
                        pLb->hdr.left,
                        pLb->hdr.top,
                        pLb->hdr.right,
                        pLb->hdr.bottom,
                        0,
                        tempColor,
                        pLb->hdr.pGolScheme->EmbossDkColor,
                        pLb->hdr.pGolScheme->EmbossLtColor,
                        NULL,
                        GOL_EMBOSS_SIZE
                    );
    
                    state = LB_STATE_PANEL;
                    break;
                }
                else
                {
                    state = LB_STATE_DRAW_ITEMS;
                }
            
            case LB_STATE_DRAW_ITEMS:
    
                /////////////////////////////////////////////////////////////////////
                // DRAW ITEMS
                /////////////////////////////////////////////////////////////////////
                SetClip(CLIP_ENABLE);
    
                SetClipRgn
                (
                    pLb->hdr.left + GOL_EMBOSS_SIZE + LB_INDENT,
                    pLb->hdr.top + GOL_EMBOSS_SIZE + LB_INDENT,
                    pLb->hdr.right - GOL_EMBOSS_SIZE - LB_INDENT,
                    pLb->hdr.bottom - GOL_EMBOSS_SIZE - LB_INDENT
                );
    
                SetFont(pLb->hdr.pGolScheme->pFont);
    
                // Set graphics cursor
                MoveTo(pLb->hdr.left + GOL_EMBOSS_SIZE + LB_INDENT, pLb->hdr.top + GOL_EMBOSS_SIZE + LB_INDENT + pLb->scrollY);
    
                pCurItem = pLb->pItemList;
                state = LB_STATE_DRAW_CURRENT_ITEMS;
    
            case LB_STATE_DRAW_CURRENT_ITEMS:

                /////////////////////////////////////////////////////////////////////
                // DRAW CURRENT ITEM
                /////////////////////////////////////////////////////////////////////

                // check if at the end of the list of items
                // this is the exit from the drawing state machine
                if(pCurItem == NULL)
                {
                    state = LB_STATE_START;
                    SetClip(CLIP_DISABLE);
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                    GFX_DRIVER_CompleteDrawUpdate(   pLb->hdr.left,
                                                    pLb->hdr.top,
                                                    pLb->hdr.right,
                                                    pLb->hdr.bottom);
#endif
                    return (1);
                }
    
                /////////////////////////////////////////////////////////////////////
                // pCurItem is a valid item, check first if within the bounds
                /////////////////////////////////////////////////////////////////////
                if(GetY() >= pLb->hdr.bottom - GOL_EMBOSS_SIZE - LB_INDENT)
                {
                    state = LB_STATE_SET_ITEMFOCUS;
                    break;
                }
                // yes, still within bounds process the item
                if((GetY() + pLb->textHeight) >= (pLb->hdr.top + GOL_EMBOSS_SIZE + LB_INDENT))
                {
                    if(!GetState(pLb, LB_DRAW))
                        if(!(pCurItem->status & LB_STS_REDRAW))
                        {
                            state = LB_STATE_SET_ITEMFOCUS;
                            break;
                        }   
    
                    pCurItem->status &= ~LB_STS_REDRAW;
                    state = LB_STATE_SET_ERASEITEM;
                }
                else
                {
                    state = LB_STATE_SET_ITEMFOCUS;
                    break;
                }
    
            case LB_STATE_SET_ERASEITEM:

                if(GetState(pLb, LB_DISABLED))
                {
                    SetColor(pLb->hdr.pGolScheme->ColorDisabled);
                }
                else
                {
                    SetColor(pLb->hdr.pGolScheme->Color0);
                    if(pCurItem != NULL)
                        if(pCurItem->status & LB_STS_SELECTED)
                        {
                            SetColor(pLb->hdr.pGolScheme->Color1);
                        }
                }
                state = LB_STATE_ERASEITEM;

            case LB_STATE_ERASEITEM:
                if
                (
                    !Bar
                        (
                            pLb->hdr.left + GOL_EMBOSS_SIZE + LB_INDENT,
                            GetY(),
                            pLb->hdr.right - GOL_EMBOSS_SIZE - LB_INDENT,
                            GetY() + pLb->textHeight
                        )
                ) return (0);

                if(!GetState(pLb, LB_CENTER_ALIGN | LB_RIGHT_ALIGN))
                {
                    MoveTo(pLb->hdr.left + GOL_EMBOSS_SIZE + LB_INDENT, GetY());
                }
                else
                {
                    temp = GetTextWidth(pCurItem->pText, pLb->hdr.pGolScheme->pFont);
                    if(pCurItem->pBitmap != NULL)
                    {
                        temp += GetImageWidth(pCurItem->pBitmap) + LB_INDENT;
                    }

                    if(GetState(pLb, LB_RIGHT_ALIGN))
                    {
                        MoveTo(pLb->hdr.right - temp - LB_INDENT - GOL_EMBOSS_SIZE, GetY());
                    }
                    else
                    {
                        MoveTo((pLb->hdr.left + pLb->hdr.right - temp) >> 1, GetY());
                    }
                }

                if(GetState(pLb, LB_DISABLED))
                {
                    SetColor(pLb->hdr.pGolScheme->TextColorDisabled);
                }
                else
                {
                    if(pCurItem->status & LB_STS_SELECTED)
                    {
                        SetColor(pLb->hdr.pGolScheme->TextColor1);
                    }
                    else
                    {
                        SetColor(pLb->hdr.pGolScheme->TextColor0);
                    }
                }

                state = LB_STATE_BITMAP;
                // break here to force check of IsDeviceBusy() in case the last Bar() function call
                // is still being rendered.
                break;
    
            case LB_STATE_BITMAP:
                if(pCurItem->pBitmap != NULL)
                {
	                    // check if the image will go beyond the list box (bottom check only)
	                    if ((GetY() + GetImageHeight(pCurItem->pBitmap)) < pLb->hdr.bottom)
	                    {
	                        if
	                        (
	                            !PutImage
	                                (
	                                    GetX(),
	                                    GetY() + ((pLb->textHeight - GetImageHeight(pCurItem->pBitmap)) >> 1),
	                                    pCurItem->pBitmap,
	                                    1
	                                )
	                        ) return (0);
                 	}   

                    MoveRel(GetImageWidth(pCurItem->pBitmap) + LB_INDENT, 0);
                }

                state = LB_STATE_TEXT;
                // break here to force IsDeviceBusy() check in case the last PutImage() is still being rendered
                break;
    
            case LB_STATE_TEXT:
                if(!OutText(pCurItem->pText))
                    return (0);
                state = LB_STATE_SET_ITEMFOCUS;
    
                /////////////////////////////////////////////////////////////////////
    
            case LB_STATE_SET_ITEMFOCUS:
                if(pLb->pFocusItem == pCurItem)
                {
                    if(IsDeviceBusy())
                        return (0);
    
                    if(GetState(pLb, LB_FOCUSED))
                    {
                        SetColor(pLb->hdr.pGolScheme->TextColor1);
                    }
                    else
                    {
                        SetColor(pLb->hdr.pGolScheme->TextColor0);
                    }
    
                    SetLineType(FOCUS_LINE);
                    temp = GetY();
                    state = LB_STATE_ITEMFOCUS;
                }
                else
                {
                    state = LB_STATE_GET_NEXTITEM;
                    break;
                }

            case LB_STATE_ITEMFOCUS:
                if
                (
                    !Rectangle
                        (
                            pLb->hdr.left + GOL_EMBOSS_SIZE + LB_INDENT,
                            GetY(),
                            pLb->hdr.right - GOL_EMBOSS_SIZE - LB_INDENT,
                            GetY() + pLb->textHeight - 1
                        )
                ) return (0);

                MoveTo(0, temp);
                SetLineType(SOLID_LINE);

                // Scroll if needed
                if(GetY() < (pLb->hdr.top + GOL_EMBOSS_SIZE + LB_INDENT))
                {
                    pLb->scrollY += (pLb->hdr.top + GOL_EMBOSS_SIZE + LB_INDENT) - GetY();
                    SetState(pLb, LB_DRAW);
                    state = LB_STATE_DRAW_ITEMS;
                    break;
                }

                if((GetY() + pLb->textHeight) > (pLb->hdr.bottom - GOL_EMBOSS_SIZE - LB_INDENT))
                {
                    pLb->scrollY += pLb->hdr.bottom - GetY() - pLb->textHeight - GOL_EMBOSS_SIZE - LB_INDENT;
                    SetState(pLb, LB_DRAW);
                    state = LB_STATE_DRAW_ITEMS;
                    break;
                }
                state = LB_STATE_GET_NEXTITEM;
    
            case LB_STATE_GET_NEXTITEM:

                pCurItem = (LISTITEM *)pCurItem->pNextItem;
                MoveRel(0, pLb->textHeight);

                state = LB_STATE_DRAW_CURRENT_ITEMS;
                break;    

        }   // end of switch
    } // end of while(1)
}

#endif // USE_LISTBOX
