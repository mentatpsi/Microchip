/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  List box
 *****************************************************************************
 * FileName:        ListBox.h
 * Dependencies:    None 
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
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
 * 07/26/11     Rename LbClrtSel() to LbClrSel().
 *****************************************************************************/
#ifndef _LISTBOX_H
    #define _LISTBOX_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define LB_FOCUSED      0x0001  // Bit for focused state
    #define LB_DISABLED     0x0002  // Bit for disabled state
    #define LB_RIGHT_ALIGN  0x0004  // Bit to indicate text is left aligned
    #define LB_CENTER_ALIGN 0x0008  // Bit to indicate text is center aligned
    #define LB_SINGLE_SEL   0x0010  // Bit to indicate the only item can be selected
    #define LB_DRAW_ITEMS   0x1000  // Bit to indicate whole edit box must be redrawn
    #define LB_DRAW_FOCUS   0x2000  // Bit to indicate whole edit box must be redrawn
    #define LB_DRAW         0x4000  // Bit to indicate whole edit box must be redrawn
    #define LB_HIDE         0x8000  // Bit to remove object from screen
    #define LB_INDENT       0x02    // Indentation constant for the text from the frame

/*********************************************************************
* Overview: Defines the parameters required for a list item used in  
*			list box.
*
*********************************************************************/
typedef struct
{
    void    *pPrevItem;             // Pointer to the next item
    void    *pNextItem;             // Pointer to the next item
    WORD    status;                 // Specifies the status of the item.

    // The following values are defined for
    // the status: LB_STS_SELECTED, LB_STS_REDRAW.
    XCHAR   *pText;                 // Pointer to the text for the item
    void    *pBitmap;               // Pointer to the bitmap
    WORD    data;                   // Some data associated with the item
} LISTITEM;

/*********************************************************************
* Bit definitions for the status of an item 
*********************************************************************/
    #define LB_STS_SELECTED 0x0001  // Item is selected.
    #define LB_STS_REDRAW   0x0002  // Item is to be redrawn.

/*********************************************************************
* Overview: Defines the parameters required for a list box Object.
*
*********************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;            // Generic header for all Objects (see OBJ_HEADER).
    LISTITEM    *pItemList;     // Pointer to the list of items.
    LISTITEM    *pFocusItem;    // Pointer to the focused item.
    WORD        itemsNumber;    // Number of items in the list box.
    SHORT       scrollY;        // Scroll displacement for the list.
    SHORT       textHeight;     // Pre-computed text height.
} LISTBOX;

/*********************************************************************
* Macros:  LbSetBitmap(pItem, pBtmap)
*
* Overview: This macro sets the bitmap used in the item. 
*
* PreCondition: none
*
* Input: pItem - Pointer to the item.
*        pBtmap - Pointer to the bitmap to be used.
*
* Output: none
*
* Example:
*   See LbAddItem() example.
*
* Side Effects: none
*
********************************************************************/
    #define LbSetBitmap(pItem, pBtmap)  ((LISTITEM *)pItem)->pBitmap = pBtmap

/*********************************************************************
* Macros:  LbGetBitmap(pItem)
*
* Overview: This macro returns the location of the currently 
*			used bitmap for the item.
*
* PreCondition: none
*
* Input: pItem - Pointer to the list item.
*
* Output: Returns the pointer to the current bitmap used.
*
* Example:
*	<CODE>
*	// Assume pLb is initialized to an existing list box
*	LISTITEM *pItem; 
*	void *pBitmap;
*
*	pItem = LbGetItemList(pLb); 
*	pBitmap = LbGetBitmap(pItem);
*	</CODE>
*
* Side Effects: none
*
********************************************************************/
    #define LbGetBitmap(pItem)  ((LISTITEM *)pItem)->pBitmap

/*********************************************************************
* Function: LISTBOX  *LbCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*							   SHORT bottom, WORD state, XCHAR* pText, 
*							   GOL_SCHEME *pScheme)
*
* Overview: This function creates a LISTBOX object with the parameters given. 
*			It automatically attaches the new object into a global linked list of 
*			objects and returns the address of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the Object.
* 		 top - Top most position of the Object. 
*		 right - Right most position of the Object.
*		 bottom - Bottom most position of the Object.
*        state - Sets the initial state of the object.
*        pText - Pointer to the initialization text for the items.
*        pScheme - Pointer to the style scheme.
*
* Output: Returns the pointer to the object created.
*
* Example:
*   <CODE> 
*	#define LISTBOX_ID   10
*
*	const XCHAR ItemList[] = "Line1\n" "Line2\n";
*
*	GOL_SCHEME *pScheme;
*	LISTBOX *pLb;
*	XCHAR *pTemp;  
*	WORD state, counter;
*	
*		pScheme = GOLCreateScheme();
*		state = LB_DRAW;
*		
*		// create an empty listbox with default style scheme
*		pLb = LbCreate(	LISTBOX_ID,			// ID number
*						10,10,150,200,		// dimension
*						state,				// initial state
*						NULL,				// set items to be empty
*						NULL);				// use default style scheme
*		// check if Listbox was created
*		if (pLb == NULL)		
*		   return 0;
*		
*		// create the list of items to be placed in the listbox
*		// Add items (each line will become one item, 
*	    // lines must be separated by '\n' character)
*       pTemp = ItemList;
*       counter = 0;
*       while(*pTemp){
*			// since each item is appended NULL is assigned to 
*			// LISTITEM pointer.
*           if(NULL == LbAddItem(pLb, NULL, pTemp, NULL, 0, counter))
*               break;
*           while((unsigned XCHAR)*pTemp++ > (unsigned XCHAR)31);
*           if(*(pTemp-1) == 0)
*               break;
*           counter++;
*		}
*	</CODE>		
*
* Side Effects: none
*
********************************************************************/
LISTBOX * LbCreate
    (WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, XCHAR * pText, GOL_SCHEME * pScheme);

/*********************************************************************
* Function: LISTITEM* LbAddItem(LISTBOX *pLb, LISTITEM *pPrevItem, 
*								XCHAR *pText, void* pBitmap, WORD status, WORD data)
*
* Overview: This function allocates memory for the LISTITEM and adds 
*			it to the list box. The newly created LISTITEM will store
*			the location of pText, pBitmap and other parameters describing 
*			the added item.
*
* PreCondition: none
*
* Input: pLb - The pointer to the list box object.
*        pPrevItem - Pointer to the item after which a new item must 
*					 be inserted, if this pointer is NULL, the item 
*					 will be appended at the end of the items list.
*        pText -  Pointer to the text that will be inserted. Text must
*				  persist in memory for as long as it is referenced 
*				  by an item in the list box.
*        pBitmap - Pointer to the bitmap for the item. Bitmap must
*				  persist in memory for as long as it is referenced  
*				  by the an item in the list box.
*        status - This parameter specifies if the item being added
*				  will be selected or redrawn 
*				  (LB_STS_SELECTED or LB_STS_REDRAW). Refer to 
*				  LISTITEM structure for details. 
*        data -  User assigned data associated with the item.
*
* Output: Return a pointer to the item created, 
*		  NULL if the operation was not successful.
*
* Example:
*   <CODE> 
*	const XCHAR ItemList[] = "Line1\n" "Line2\n" "Line3\n";
*
*	extern BITMAP_FLASH myIcon;
*	LISTBOX *pLb;
*	LISTITEM *pItem, *pItemList;
*   XCHAR *pTemp;  
*
*	// Assume that pLb is pointing to an existing list box in memory 
*	// that is empty (no list).
*
*	// Create the list of the list box
*
*	// Initialize this to NULL to indicate that items will be added 
*	// at the end of the list if the list exist on the list box or 
*	// start a new list if the list box is empty. 
*	pItem = NULL;				
*	pTemp = ItemList;
*	pItem = LbAddItem(pLb, pItem, pTemp, NULL, LB_STS_SELECTED, 1)
*   if(pItem == NULL)
*		return 0;
*	LbSetBitmap(pItem, &myIcon);
*
*	// Adjust pTemp to point to the next line
*	while((unsigned XCHAR)*pTemp++ > (unsigned XCHAR)31);
*
*	// add the next item
*	pItem = LbAddItem(pLb, pItem, pTemp, NULL, 0, 2)
*   if(pItem == NULL)
*		return 0;
*	LbSetBitmap(pItem, &myIcon);
*
*	// Adjust pTemp to point to the next line
*	while((unsigned XCHAR)*pTemp++ > (unsigned XCHAR)31);
*
*	// this time insert the next item after the first item on the list
*	pItem = LbGetItemList(pLb); 
*	pItem = LbAddItem(pLb, pItem, pTemp, NULL, 0, 3)
*   if(pItem == NULL)
*		return 0;
*	LbSetBitmap(pItem, &myIcon);
*
*	</CODE> 
*	
* Side Effects: none
*
********************************************************************/
LISTITEM    *LbAddItem(LISTBOX *pLb, LISTITEM *pPrevItem, XCHAR *pText, void *pBitmap, WORD status, WORD data);

/*********************************************************************
* Function: void LbDelItem(LISTBOX *pLb, LISTITEM *pItem)
*
* Overview: This function removes an item from the list box 
*			and frees the memory used.
*
* PreCondition: none
*
* Input: pLb - The pointer to the list box object.
*        pItem -  The pointer to the item that will be removed.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void        LbDelItem(LISTBOX *pLb, LISTITEM *pItem);

/*********************************************************************
* Function: void LbDelItemsList(void *pObj)
*
* Overview: This function removes all items from the list box 
*			and frees the memory used.
*
* PreCondition: none
*
* Input: pLb - The pointer to the list box object.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void        LbDelItemsList(void *pObj);

/*********************************************************************
* Function: LISTITEM* LbGetSel(LISTBOX *pLb, LISTITEM *pFromItem)
*
* Overview: This function searches for selected items from the list box.
*		    A starting position can optionally be given. If starting 
*			position is set to NULL, search will begin from the first
*			item list. It returns the pointer to the first selected item 
*			found or NULL if there are no items selected.
*
* PreCondition: none
*
* Input: pLb 		- The pointer to the list box object.
*        pFromItem  - The pointer to the item the search must start from,
*                     if the pointer is NULL the search begins from the 
*					  start of the items list.
*
* Output: pointer to the selected item, NULL if there are no items selected
*
*
* Side Effects: none
*
********************************************************************/
LISTITEM    *LbGetSel(LISTBOX *pLb, LISTITEM *pFromItem);

/*********************************************************************
* Function: void LbChangeSel(LISTBOX *pLb, LISTITEM *pItem)
*
* Overview: This function changes the selection status of an item
*			in the list box. If the item is currently selected, it 
*		    resets the selection. If the item is currently not 
*			selected it is set to be selected.
*
* PreCondition: none
*
* Input: pLb 	- The pointer to the list box object.
*        pItem  - The pointer to the item the selection status
*                     will be changed.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void        LbChangeSel(LISTBOX *pLb, LISTITEM *pItem);

/*********************************************************************
* Macro: LbSetSel(pLb, pItem)
*
* Overview: This macro sets the selection status of an item to
*			selected.
*
* PreCondition: none
*
* Input: pLb 	- The pointer to the list box object.
*        pItem  - The pointer to the item the selection status
*                     will be set.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define LbSetSel(pLb, pItem)           \
    if(!(pItem->status & LB_STS_SELECTED)) \
        LbChangeSel((LISTBOX *)pLb, pItem);

/*********************************************************************
* Macro: LbClrSel(pLb, pItem)
*
* Overview: This macro clears the selection of an item.
*
* PreCondition: none
*
* Input: pLb - The pointer to the list box.
*        pItem - The pointer to the item the selection status should be cleared.
*
* Output: none
*
* Side Effects: none 
*
********************************************************************/
    #define LbClrSel(pLb, pItem)       \
    if(pItem->status & LB_STS_SELECTED) \
        LbChangeSel((LISTBOX *)pLb, pItem);

/*********************************************************************
* Macro: LbGetCount(pLb)
*
* Overview: This macro returns the number of items in the list box.
*
* PreCondition: none
*
* Input: pLb - The pointer to the list box object.
*
* Output: The number of items the list box contains.
*
* Side Effects: none
*
********************************************************************/
    #define LbGetCount(pLb) ((LISTBOX *)pLb)->itemsNumber

/*********************************************************************
* Macro: LbGetVisibleCount(pLb)
*
* Overview: This macro returns the number of items visible in the 
*			list box window.
*
* PreCondition: none
*
* Input: pLb - The pointer to the list box object.
*
* Output: The number of items visible in the list box window.
*
* Side Effects: none
*
********************************************************************/
    #define LbGetVisibleCount(pLb)                                                                           \
        (                                                                                                    \
            (((LISTBOX *)pLb)->hdr.bottom - ((LISTBOX *)pLb)->hdr.top - 2 * (GOL_EMBOSS_SIZE + LB_INDENT)) / \
                ((LISTBOX *)pLb)->textHeight                                                                 \
        )

/*********************************************************************
* Function: void LbSetFocusedItem(LISTBOX* pLb, SHORT index)
*
* Overview: This function sets the focus for the item with the 
*			given index.
*
* PreCondition: none
*
* Input: pLb   - The pointer to the list box object.
*		 index - The index number of the item to be focused.
*				 First item on the list is always indexed 0.
*
* Output: none.
*
* Side Effects: none
*
********************************************************************/
void    LbSetFocusedItem(LISTBOX *pLb, SHORT index);

/*********************************************************************
* Function: SHORT LbGetFocusedItem(LISTBOX* pLb)
*
* Overview: This function returns the index of the focused item
*			in the list box.
*
* PreCondition: none
*
* Input: pLb   - The pointer to the list box object.
*
* Output: Returns the index of the focused item in the list box.
*
* Side Effects: none
*
********************************************************************/
SHORT   LbGetFocusedItem(LISTBOX *pLb);

/*********************************************************************
* Macro: LISTITEM LbGetItemList(LISTBOX* pLb)
*
* Overview: This function returns the pointer to the current 
*			item list used in the list box.
*
* PreCondition: none
*
* Input: pLb   - The pointer to the list box object.
*
* Output: Returns the pointer to the LISTITEM used in the list box.
*
* Example:
*   See LbAddItem() example.
*
* Side Effects: none
*
********************************************************************/
    #define LbGetItemList(pLb)  ((LISTITEM *)((LISTBOX *)pLb)->pItemList)

/*********************************************************************
* Function: WORD LbTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. The table below enumerates the translated 
*			messages for each event of the touch screen and keyboard inputs.
*
*	<TABLE>
*    	Translated Message   Input Source  Events         				Description
*     	##################   ############  ######         				###########
*     	LB_MSG_TOUCHSCREEN   Touch Screen  Any    						Item is selected using touch screen.
*		LB_MSG_MOVE	 		 Keyboard      EVENT_KEYSCAN  				Focus is moved to the next item depending on the key pressed (UP or DOWN key).
*		LB_MSG_SEL			 Keyboard      EVENT_KEYSCAN  				LB_MSG_SEL – Selection is set to the currently focused item.
*		OBJ_MSG_INVALID		 Any		   Any			  				If the message did not affect the object.							 
*	</TABLE>
*
* PreCondition: none
*
* Input: pLB   - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		  - LB_MSG_TOUCHSCREEN – Item is selected using touch screen.
*         - LB_MSG_MOVE – Focus is moved to the next item depending on the key pressed (UP or DOWN key).
*         - LB_MSG_SEL – Selection is set to the currently focused item.
*
* Side Effects: none
*
********************************************************************/
WORD    LbTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: void LbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
*
* Overview: This function performs the actual state change 
*			based on the translated message given. The following state changes 
*			are supported:
*	<TABLE>
*    	Translated Message   Input Source  Set/Clear State Bit		Description
*     	##################   ############  ######     				###########
*     	LB_MSG_TOUCHSCREEN   Touch Screen  Set LB_FOCUSED,  	   	If focus is enabled, the focus state bit LB_FOCUSED will be set. LB_DRAW_FOCUS draw state bit will force 
*										   Set LB_DRAW_FOCUS		the List Box to be redrawn with focus.
*										   Set LB_DRAW_ITEMS		List Box will be redrawn with selected item(s).
*		LB_MSG_MOVE	 		 KeyBoard	   Set LB_DRAW_ITEMS		List Box will be redrawn with focus on one item.
*		LB_MSG_SEL			 KeyBoard	   Set LB_DRAW_ITEMS		List Box will be redrawn with selection on the current item focused.
*	</TABLE>
*
* PreCondition: none
*
* Input: translatedMsg - The translated message
*        pB            - The pointer to the object whose state will be modified.
*        pMsg          - The pointer to the GOL message.
*        
* Output: none
*
* Side Effects: none
*
********************************************************************/
void    LbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD LbDraw(void *pObj)
*
* Overview: This function renders the object on the screen using 
* 			the current parameter settings. Location of the object is 
*			determined by the left, top, right and bottom parameters. 
*			The colors used are dependent on the state of the object. 
*			The font used is determined by the style scheme set.
*			
*			The text or items drawn in the visible window of the 
*			list box is dependent on the alignment set. 
*			
*			When rendering objects of the same type, each object 
*			must be rendered completely before the rendering of the 
*			next object is started. This is to avoid incomplete 
*			object rendering.
*
* PreCondition: Object must be created before this function is called.
*
* Input: pLb - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing 
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Side Effects: none
*
********************************************************************/
WORD LbDraw(void *pObj);
#endif // _LISTBOX_H
