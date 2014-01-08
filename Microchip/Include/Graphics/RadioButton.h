/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Radio Button
 *****************************************************************************
 * FileName:        RadioButton.h
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
 *****************************************************************************/
#ifndef _RADIOBUTTON_H
    #define _RADIOBUTTON_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

    #define RB_INDENT   2           // Indent for the text from title bar border

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define RB_FOCUSED      0x0001  // Bit for focused state.
    #define RB_DISABLED     0x0002  // Bit for disabled state.
    #define RB_CHECKED      0x0004  // Bit to indicate Radio Button is checked.
    #define RB_GROUP        0x0008  // Bit to indicate the first Radio Button in the group. 
                                    // Each group MUST have this bit set for its first member
                                    // even for a single member group. This means that any 
                                    // independent or stand alone Radio Button, the RB_GROUP bit
                                    // must be always set.
                                    // or alone in the group.
    #define RB_HIDE         0x8000  // Bit to indicate that button must be removed from screen.
    #define RB_DRAW_FOCUS   0x2000  // Bit to indicate focus must be redrawn.
    #define RB_DRAW_CHECK   0x1000  // Bit to indicate check mark should be redrawn.
    #define RB_DRAW         0x4000  // Bit to indicate whole Radio Button must be redrawn.

/*****************************************************************************
* Overview: the structure contains data for the Radio Button
*****************************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;        // Generic header for all Objects (see OBJ_HEADER).
    OBJ_HEADER  *pHead;     // Pointer to the first Radio Button in the group
    OBJ_HEADER  *pNext;     // Pointer to the next Radio Button in the group
    SHORT       textHeight; // Pre-computed text height
    XCHAR       *pText;     // Pointer to the text
} RADIOBUTTON;

/*********************************************************************
* Macros:  RbGetText(pRb)
*
* Overview: This macro returns the address of the current 
*			text string used for the object.
*
* PreCondition: none
*
* Input: pRb - Pointer to the object 
*
* Output: Returns pointer to the text string being used.
*
* Side Effects: none
*
********************************************************************/
    #define RbGetText(pRb)  pRb->pText

/*********************************************************************
* Function: RbSetText(RADIOBUTTON *pRb, XCHAR *pText)
*
* Overview: This function sets the string used for the object.
*
* PreCondition: none
*
* Input: pRb - The pointer to the object whose text will be modified 
*        pText - Pointer to the text that will be used
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void        RbSetText(RADIOBUTTON *pRb, XCHAR *pText);

/*********************************************************************
* Function: void RbSetCheck(RADIOBUTTON *pRb, WORD ID)
*
* Overview: This function sets the Radio Button with the given ID 
*			to its checked state.
*
* PreCondition: none
*
* Input: pRb - Pointer to the Radio Button in the group.
*		 ID - ID of the object to be checked.
*
* Output: none
*
* Example:
* 	See RbGetCheck() example.
*
* Side Effects: none
*
********************************************************************/
void        RbSetCheck(RADIOBUTTON *pRb, WORD ID);

/*********************************************************************
* Function: WORD RbGetCheck(RADIOBUTTON *pRb)
*
* Overview: This function returns the ID of the currently 
*			checked Radio Button in the group.
*
* PreCondition: none
*
* Input: pRb - Pointer to the Radio Button in the group.
*
* Output: Returns the ID of the selected button in the group. 
*		  It returns -1 if there is no object checked.
*
* Example:
*   <CODE> 
*	GOL_SCHEME *pScheme;
*	RADIOBUTTON *pRb[3];
*	SHORT ID;
*
*		pScheme = GOLCreateScheme();
*		pRb[0] = RbCreate(ID_RADIOBUTTON1,				// ID
*						  255,40,310,80,				// dimension
*				          RB_DRAW|RB_GROUP|RB_CHECKED, 	// will be dislayed and
*				                                        // focused after creation
*					                                    // first button in the group
*				          "RB1",                       	// text
*				          pScheme);                    	// scheme used
*
*		pRb[1] = RbCreate(ID_RADIOBUTTON2,   		 	// ID
*				          255,85,310,125,              	// dimension
*				          RB_DRAW,                     	// will be dislayed and
*						                              	// checked after creation
*				          "RB2",                       	// text
*				          pScheme);                    	// scheme used
*
*		pRb[2] = RbCreate(ID_RADIOBUTTON3,    			// ID
*				          255,130,310,170,             	// dimension
*				          RB_DRAW,                     	// will be dislayed and
*				                                      	// disabled after creation
*					      "RB3",                        // text
*				          pScheme);                     // scheme used
*
*		// draw the Radio Buttons here
*		
*		ID = RbGetCheck(pRb[2]);              			// can also use pRb[1] or
*		                                      			// pRb[0] to search the
*		                                      			// checked radio button of the
*		                                      			// group. ID here should
*		                                      			// be ID_RADIOBUTTON1
*		if (ID == ID_RADIOBUTTON1) {
*			// do something here then clear the check
*			ClrState(pRb[0], RB_CHECKED);     			
*			// Change the checked object. Pointer used is any of the three.
*			// the ID used will find the correct object to be checked	
*			RbSetCheck(pRb[3], ID_RADIOBUTTON2);  		
*		}
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
WORD        RbGetCheck(RADIOBUTTON *pRb);

/*********************************************************************
* Function: RADIOBUTTON  *RbCreate( WORD ID, SHORT left, SHORT top, 
*									SHORT right, SHORT bottom, WORD state, 
*									XCHAR *pText, GOL_SCHEME *pScheme)
*
* Overview: This function creates a RADIOBUTTON object with the parameters given. 
*			It automatically attaches the new object into a global linked list of 
*			objects and returns the address of the object. A group of Radio Buttons
*           MUST have the first member of the group set it's RB_GROUP bit set.
*           Any standalone Radio Button must also have it's RB_GROUP bit set.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the Object.
* 		 top - Top most position of the Object. 
*		 right - Right most position of the Object
*		 bottom - Bottom most position of the object
*        state - Sets the initial state of the object
*        pText – The pointer to the text used for the Radio Button.
*        pScheme - Pointer to the style scheme used.
*
* Output: Returns the pointer to the object created
*
* Example:
*   <CODE> 
*	GOL_SCHEME *pScheme;
*	RADIOBUTTON *pRb[3];
*
*		pScheme = GOLCreateScheme();
*		pRb[0] = RbCreate(ID_RADIOBUTTON1,				// ID
*						  255,40,310,80,				// dimension
*				          RB_DRAW|RB_GROUP|RB_CHECKED, 	// will be dislayed and
*				                                        // focused after creation
*					                                    // first button in the group
*				          "RB1",                       	// text
*				          pScheme);                    	// scheme used
*
*		pRb[1] = RbCreate(ID_RADIOBUTTON2,   		 	// ID
*				          255,85,310,125,              	// dimension
*				          RB_DRAW,                     	// will be dislayed and
*						                              	// checked after creation
*				          "RB2",                       	// text
*				          pScheme);                    	// scheme used
*
*		pRb[2] = RbCreate(ID_RADIOBUTTON3,    			// ID
*				          255,130,310,170,             	// dimension
*				          RB_DRAW,                     	// will be dislayed and
*				                                      	// disabled after creation
*					      "RB3",                        // text
*				          pScheme);                     // scheme used
*
*		while(!RbDraw(pRb[0]));               			// draw the objects
*		while(!RbDraw(pRb[1]));
*		while(!RbDraw(pRb[2]));
*	</CODE> 
*
* Side Effects: none
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
            );

/*********************************************************************
* Function: WORD RbTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if 
*			the message will affect the object or not. The table 
*			below enumerates the translated messages for each event 
*			of the touch screen and keyboard inputs.
*
*	<TABLE>
*    	Translated Message   Input Source  Events         	Description
*     	##################   ############  ######         	###########
*		RB_MSG_CHECKED 		 Touch Screen  EVENT_PRESS		If event occurs and the x,y position falls in the area of the Radio Button while the Radio Button is not checked.
*							 Keyboard	   EVENT_KEYSCAN	If event occurs and parameter1 passed matches the object’s ID and parameter 2 passed matches SCAN_CR_PRESSED or SCAN_SPACE_PRESSED while the Radio Button is not checked.
*		OBJ_MSG_INVALID		 Any		   Any				If the message did not affect the object.
*	</TABLE>
*
* PreCondition: none
*
* Input: pRb   - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		 - RB_MSG_CHECKED – Radio Button is checked
* 		 - OBJ_MSG_INVALID – Radio Button is not affected
*
* Example:
*	Usage is similar to BtnTranslateMsg() example. 
*
* Side Effects: none
*
********************************************************************/
WORD        RbTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: RbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
* Overview: This function performs the actual state change 
*			based on the translated message given. The following state changes 
*			are supported:
*	<TABLE>
*    	Translated Message   Input Source 	Set/Clear State Bit		Description
*     	##################   ############ 	######     				###########
*     	RB_MSG_CHECKED       Touch Screen,	Set RB_DRAW,         	Depending on the current value of RB_CHECKED Check Box will be redrawn.
*							  Keyboard		Set/Clear RB_CHECKED 
*	</TABLE>
*
* PreCondition: none
*
* Input: translatedMsg - The translated message
*        pRb           - The pointer to the object whose state will be modified
*        pMsg          - The pointer to the GOL message
*        
* Output: none
*
* Example:
*	See BtnTranslateMsg() example.
*
* Side Effects: none
*
********************************************************************/
void        RbMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD RbDraw(void *pObj)
*
* Overview: This function renders the object on the screen using 
*			the current parameter settings. Location of the object 
*			is determined by the left, top, right and bottom 
*			parameters. The colors used are dependent on the state 
*			of the object. The font used is determined by the style 
*			scheme set.
*			
*			When rendering objects of the same type, each object must 
*			be rendered completely before the rendering of the next 
*			object is started. This is to avoid incomplete object 
*			rendering.
*
* PreCondition: Object must be created before this function is called.
*
* Input: pB - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing 
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Example:
*   See RbCreate() example.
*
* Side Effects: none
*
********************************************************************/
WORD RbDraw(void *pObj);
#endif // _RADIOBUTTON_H
