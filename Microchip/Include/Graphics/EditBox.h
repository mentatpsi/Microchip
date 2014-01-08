/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Edit box
 *****************************************************************************
 * FileName:        EditBox.h
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
 * Date			Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07		Version 1.0 release
 * 09/16/10     Modified use of EB_DRAW_CARET, & EB_CARET. 
 *				EB_DRAW_CARET when set will draw the caret. EB_DRAW_CARET can
 *				also be enabled by the EB_FOCUSED bit.
 *				EB_CARET when set will always draw the caret.
 * 08/05/11     EB_CARET will indicate that the cursor caret will always be drawn.
 *              Cursor caret drawing will also serve as focus indicator. EB_DRAW_CARET 
 *              with EB_FOCUSED set will draw the cursor caret regardless of EB_CARET 
 *              state.
 *****************************************************************************/
#ifndef _EDITBOX_H
    #define _EDITBOX_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define EB_FOCUSED      0x0001  // Bit for focused state. Cursor caret will be drawn when EB_DRAW_CARET is also set.
    #define EB_DISABLED     0x0002  // Bit for disabled state.
    #define EB_RIGHT_ALIGN  0x0004  // Bit to indicate text is left aligned.
    #define EB_CENTER_ALIGN 0x0008  // Bit to indicate text is center aligned.
    #define EB_CARET        0x0010  // Bit to indicate the cursor caret will always be shown. 
    #define EB_DRAW_CARET   0x2000  // Bit to indicate the cursor caret will be drawn if EB_FOCUSED state bit is set and erase when EB_FOCUSED state bit is not set. 
    #define EB_DRAW         0x4000  // Bit to indicate whole edit box must be redrawn.
    #define EB_HIDE         0x8000  // Bit to remove object from screen.
    #define EB_INDENT       0x02    // Indent for the text from the frame.
    #define EB_CARET_WIDTH  0x02    // Caret line width.

/*********************************************************************
* Overview: Defines the parameters required for a Edit Box Object.
*********************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;        // Generic header for all Objects (see OBJ_HEADER).
    SHORT       textHeight; // Pre-computed text height.
    XCHAR       *pBuffer;   // Pointer to text buffer.
    WORD        charMax;    // Maximum number of characters in the edit box.
    WORD        length;     // Current text length.
} EDITBOX;

/*********************************************************************
* Function: EDITBOX  *EbCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, 
*							   WORD state , XCHAR *pText, WORD charMax, GOL_SCHEME *pScheme)
*
* Overview: This function creates a EDITBOX object with the parameters given and 
*			initializes the default settings. It automatically attaches the new 
*			object into a global linked list of objects and returns the address 
*			of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the Object.
* 		 top - Top most position of the Object. 
*		 right - Right most position of the Object.
*		 bottom - Bottom most position of the object.
*        state - Sets the initial state of the object.
*        pText - Pointer to the text to be used.
*        charMax - Defines the maximum number of characters in the edit box.
*        pScheme - Pointer to the style scheme.
*
* Output: Returns the pointer to the object created.
*
* Example:
*   <CODE> 
*	#define ID_MYEDITBOX	101
*	EDITBOX *pEb;
*
*	pEb = EbCreate(ID_MYEDITBOX,	// ID
*                10,				// left
*				 10,				// top
*                100,   			// right
*                30,  			    // bottom
*                EB_DRAW,			// redraw after creation  					
*             	 NULL,				// no text yet
*              	 4,					// display only four characters
*              	 pScheme);			// pointer to the style scheme		   
*
*	if( pEb == NULL )
*   {
*		// MEMORY ERROR. Object was not created.
*   }
*		
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
EDITBOX *EbCreate
        (
            WORD        ID,
            SHORT       left,
            SHORT       top,
            SHORT       right,
            SHORT       bottom,
            WORD        state,
            XCHAR       *pText,
            WORD        charMax,
            GOL_SCHEME  *pScheme
        );

/*********************************************************************
* Function: EbSetText(EDITBOX *pEb, XCHAR *pText)
*
* Overview: This function sets the text to be used for the object.
*
* PreCondition: none
*
* Input: pEb - The pointer to the object whose text will be modified.
*        pText - Pointer to the text that will be used.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void    EbSetText(EDITBOX *pEb, XCHAR *pText);

/*********************************************************************
* Macros:  EbGetText(pEb)
*
* Overview: This macro returns the address of the current 
*			text string used for the object.
*
* PreCondition: none
*
* Input: pEb - Pointer to the object 
*
* Output: Returns pointer to the text string being used.
*
* Side Effects: none
*
********************************************************************/
    #define EbGetText(pEb)  (pEb->pBuffer)

/*********************************************************************
* Function: void  EbAddChar(EDITBOX* pEb, XCHAR ch)
*
* Overview: This function inserts a character at the end of the 
*			text used by the object.
*
* PreCondition: none
*
* Input: pEb - The pointer to the object whose text will be modified.
*        ch - Character to be inserted. 
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void    EbAddChar(EDITBOX *pEb, XCHAR ch);

/*********************************************************************
* Function: void  EbDeleteChar(EDITBOX* pEb)
*
* Overview: This function removes a character at the end of the 
*			text used by the object.
*
* PreCondition: none
*
* Input: pEb - The pointer to the object to be modified.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void    EbDeleteChar(EDITBOX *pEb);

/*********************************************************************
* Function: WORD EbTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. The table below enumerates the translated 
*			messages for each event of the touch screen and keyboard inputs.
*
*	<TABLE>
*    	Translated Message   Input Source  Events         					Description
*     	##################   ############  ######         					###########
*     	EB_MSG_CHAR      	 Keyboard 	   EVENT_CHARCODE                   New character should be added.
*     	EB_MSG_DEL		     Keyboard	   EVENT_KEYPRESS  				    Last character should be removed.
*		OBJ_MSG_INVALID		 Any		   Any			  					If the message did not affect the object.							 
*	</TABLE>
*
* PreCondition: none
*
* Input: pEb   - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		  - EB_MSG_CHAR – New character should be added.
*         - EB_MSG_DEL – Last character should be removed.
*         - OBJ_MSG_INVALID – Object is not affected.
*
* Example:
*   Usage is similar to BtnTranslateMsg() example.
*
* Side Effects: none
*
********************************************************************/
WORD    EbTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: void EbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
*
* Overview: This function performs the actual state change 
*			based on the translated message given. The following state changes 
*			are supported:
*	<TABLE>
*    	Translated Message   Input Source	  Set/Clear State Bit	Description
*     	##################   ############	  ######     			###########
*     	EB_MSG_CHAR          Keyboard  	  	  Set EB_DRAW  	    	New character is added and Edit Box will be redrawn.
*		EB_MSG_DEL	 		 Keyboard     	  Set EB_DRAW			Last character is removed and Edit Box will be redrawn.
*	</TABLE>
*
* PreCondition: none
*
* Input: translatedMsg - The translated message.
*        pEb           - The pointer to the object whose state will be modified.
*        pMsg          - The pointer to the GOL message.
*        
* Output: none
*
* Example:
*	Usage is similar to BtnMsgDefault() example.
*
* Side Effects: none
*
********************************************************************/
void    EbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD EbDraw(void *pObj)
*
* Overview: This function renders the object on the screen using 
* 			the current parameter settings. Location of the object is 
*			determined by the left, top, right and bottom parameters. 
*			The colors used are dependent on the state of the object. 
*			The font used is determined by the style scheme set.
*			
*			When rendering objects of the same type, each object 
*			must be rendered completely before the rendering of the 
*			next object is started. This is to avoid incomplete 
*			object rendering.
*
* PreCondition: Object must be created before this function is called.
*
* Input: pEb - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing 
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Side Effects: none
*
********************************************************************/
WORD EbDraw(void *pObj);
#endif // _EDITBOX_H
