/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Check box
 *****************************************************************************
 * FileName:        CheckBox.h
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
 *****************************************************************************/
#ifndef _CHECKBOX_H
    #define _CHECKBOX_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"
// This is indent from outside borders
    #define CB_INDENT   2

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define CB_FOCUSED      0x0001  // Focus state
    #define CB_DISABLED     0x0002  // Disabled state
    #define CB_CHECKED      0x0004  // Checked state
    #define CB_HIDE         0x8000  // Check box must be removed from screen
    #define CB_DRAW_FOCUS   0x2000  // Focus must be redrawn
    #define CB_DRAW         0x4000  // Whole check box must be redrawn
    #define CB_DRAW_CHECK   0x1000  // Check box mark should be redrawn

/*********************************************************************
* Overview: The structure contains check box data
*********************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;        // Generic header for all Objects (see OBJ_HEADER).
    SHORT       textHeight; // Pre-computed text height
    XCHAR       *pText;     // Pointer to text
} CHECKBOX;

/*********************************************************************
* Macros: CbGetText(pCb)
*
* Overview: This macro returns the location of the text 
*			used for the check box.
*
* PreCondition: none
*
* Input: pCb - Pointer to the object 
*
* Output: Returns the location of the text used.
*
* Side Effects: none
*
********************************************************************/
    #define CbGetText(pCb)  pCb->pText

/*********************************************************************
* Function: CbSetText(CHECKBOX *pCb, XCHAR *pText)
*
* Overview: This function sets the text that will be used. 
*
* PreCondition: none
*
* Input: pCb - The pointer to the check box whose text will be modified. 
*        pText - The pointer to the text that will be used.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void        CbSetText(CHECKBOX *pCb, XCHAR *pText);

/*********************************************************************
* Function: CHECKBOX  *CbCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                               SHORT bottom, WORD state, XCHAR *pText, 
*							    GOL_SCHEME *pScheme)
*
* Overview: This function creates a CHECKBOX object with the parameters 
*			given. It automatically attaches the new object into a 
*			global linked list of objects and returns the address 
*			of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the Object.
* 		 top - Top most position of the Object. 
*		 right - Right most position of the Object
*		 bottom - Bottom most position of the object
*        state - Sets the initial state of the object
*        pText - Pointer to the text of the check box.
*        pScheme - Pointer to the style scheme
*
* Output: Returns the pointer to the object created
*
* Example:
*   <CODE> 
*	GOL_SCHEME *pScheme;
*	CHECKBOX *pCb[2];
*	
*		pScheme = GOLCreateScheme();
*		pCb = CbCreate(ID_CHECKBOX1,          // ID
*		               20,135,150,175,        // dimension
*		               CB_DRAW,               // Draw the object
*		               "Scale",               // text
*		               pScheme);              // use this scheme
*	
*	 	pCb = CbCreate(ID_CHECKBOX2,          // ID
*		               170,135,300,175,       // dimension
*	    	           CB_DRAW,               // Draw the object
*		               "Animate",             // text
*		               pScheme);              // use this scheme
*	
*		while(!CbDraw(pCb[0]));               // draw the objects
*		while(!CbDraw(pCb[1]));
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
CHECKBOX    *CbCreate
            (
                WORD        ID,
                SHORT       left,
                SHORT       top,
                SHORT       right,
                SHORT       bottom,
                WORD        state,
                XCHAR       *pText,
                GOL_SCHEME  *pScheme
            );

/*********************************************************************
* Function: WORD CbTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if 
*			the message will affect the object or not. The table 
*			below enumerates the translated messages for each event 
*			of the touch screen and keyboard inputs.
*
*	<TABLE>
*    	Translated Message   Input Source  Events         	Description
*     	##################   ############  ######         	###########
*       CB_MSG_CHECKED		 Touch Screen  EVENT_PRESS		If events occurs and the x,y position falls in the area of the check box while the check box is unchecked.
*					 		 Keyboard	   EVENT_KEYSCAN	If event occurs and parameter1 passed matches the object’s ID and parameter 2 passed matches SCAN_CR_PRESSED or SCAN_SPACE_PRESSED while the check box is unchecked.
*		CB_MSG_UNCHECKED     Touch Screen  EVENT_PRESS		If events occurs and the x,y position falls in the area of the check box while the check box is checked.
*							 Keyboard	   EVENT_KEYSCAN	If event occurs and parameter1 passed matches the object’s ID and parameter 2 passed matches SCAN_CR_PRESSED or SCAN_SPACE_PRESSED while the check box is checked.
*		OBJ_MSG_INVALID		 Any		   Any				If the message did not affect the object.
*	</TABLE>
*
* PreCondition: none
*
* Input: pMsg - pointer to the message struct containing the message the user
*        pCb - the pointer to the object where the message will be
*              evaluated to check if the message will affect the object
*
* Output: Returns the translated message depending on the received GOL message:
*		  - CB_MSG_CHECKED – Check Box is checked.
* 		  - CB_MSG_UNCHECKED – Check Box is unchecked.
* 		  - OBJ_MSG_INVALID – Check Box is not affected.
*
* Example:
*   Usage is similar to BtnTranslateMsg() example.
*
* Side Effects: none
*
********************************************************************/
WORD        CbTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: CbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
* Overview: This function performs the actual state change 
*			based on the translated message given. The following state changes 
*			are supported:
*	<TABLE>
*    	Translated Message   Input Source	  Set/Clear State Bit	Description
*     	##################   ############	  ######     			###########
*     	CB_MSG_CHECKED       Touch Screen,	 Set CB_CHECKED  	    Check Box will be redrawn in checked state.
*     					      Keyboard	     				
*		CB_MSG_UNCHECKED	 Touch Screen,	 Clear CB_CHECKED		Check Box will be redrawn in un-checked state.
*							  Keyboard        				
*	</TABLE>
*
* PreCondition: none
*
* Input: translatedMsg - The translated message
*        pCb           - The pointer to the object whose state will be modified
*        pMsg          - The pointer to the GOL message
*        
* Output: none
*
* Side Effects: none
*
********************************************************************/
void        CbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD CbDraw(void *pObj)
*
* Overview: This function renders the object on the screen using 
*			the current parameter settings. Location of the object 
*			is determined by the left, top, right and bottom parameters. 
*			The colors used are dependent on the state of the object. 
*			The font used is determined by the style scheme set.
*			
*			When rendering objects of the same type, each object must 
*			be rendered completely before the rendering of the next 
*			object is started. This is to avoid incomplete object rendering.
*
* PreCondition: Object must be created before this function is called.
*
* Input: pCb - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing 
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Example:
*   See CbCreate() Example.
*
* Side Effects: none
*
********************************************************************/
WORD CbDraw(void *pObj);
#endif // _CHECKBOX_H
