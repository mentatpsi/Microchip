/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Group Box
 *****************************************************************************
 * FileName:        GroupBox.h
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
#ifndef _GROUPBOX_H
    #define _GROUPBOX_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define GB_DISABLED     0x0002  // Bit for disabled state
    #define GB_RIGHT_ALIGN  0x0004  // Bit to indicate text is right aligned
    #define GB_CENTER_ALIGN 0x0008  // Bit to indicate text is center aligned

// When center and right bits are zero alignment is left
    #define GB_DRAW 0x4000          // Bit to indicate group box must be redrawn
    #define GB_HIDE 0x8000          // Bit to remove object from screen

/*********************************************************************
* Overview: Defines the parameters required for a group box Object.
* 			The textwidth and textHeight is not checked with the actual 
*			dimension of the object. Clipping is not supported in 
*			group box object. It is possible for the text to exceed 
*			the dimension of the Object.
* 
*********************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;        // Generic header for all Objects (see OBJ_HEADER).
    SHORT       textWidth;  // Pre-computed text width.
    SHORT       textHeight; // Pre-computed text height.
    XCHAR       *pText;     // Text string used.
} GROUPBOX;

/*********************************************************************
* Macros:  GbGetText(pGb)
*
* Overview: This macro returns the location of the text used.
*
* PreCondition: none
*
* Input: pGb - Pointer to the object.
*
* Output: Returns the address of the text string used.
*
* Side Effects: none
*
********************************************************************/
    #define GbGetText(pB)   pGb->pText

/*********************************************************************
* Function: GbSetText(GROUPBOX *pGb, XCHAR *pText)
*
* Overview: This function sets the text used by passing the pointer
*			to the static string.
*
* PreCondition: The style scheme used for the object MUST be initialized
*               with a valid font. If font is not valid, textWidth and
*               textHeight parameter of GROUPBOX will be undefined.
*
* Input: pGb - the pointer to the object whose state will be modified.
*        pText - pointer to the text that will be used.
*
* Output: none
*
* Side Effects: Modifies the object width and height depending on the 
*               selected string width and font height.
*
********************************************************************/
void        GbSetText(GROUPBOX *pGb, XCHAR *pText);

/*********************************************************************
* Function: WORD GbTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. The table below 
*			enumerates the translated messages for each event of 
*			the touch screen inputs.
*
*	<TABLE>
*    	Translated Message   Input Source  Events         				Description
*     	##################   ############  ######         				###########
*		GB_MSG_SELECTED		 Touch Screen  EVENT_PRESS, EVENT_RELEASE   If events occurs and the x,y position falls in the area of the group box.
*		OBJ_MSG_INVALID      Any		   Any 							If the message did not affect the object.
*	</TABLE>
*
* PreCondition: none
*
* Input: pGb   - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		  - GB_MSG_SELECTED – Group Box is selected
*         - OBJ_MSG_INVALID – Group Box is not affected
*
* Example:
*   Usage is similar to BtnTranslateMsg() example.
*
* Side Effects: none
*
********************************************************************/
WORD        GbTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: GROUPBOX *GbCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, 
*			                   WORD state, XCHAR *pText, GOL_SCHEME *pScheme)
*
* Overview: This function creates a GROUPBOX object with the parameters 
*			given. It automatically attaches the new object into a global 
*			linked list of objects and returns the address of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the Object.
* 		 top - Top most position of the Object. 
*		 right - Right most position of the Object.
*		 bottom - Bottom most position of the object.
*		 state - Sets the initial state of the object.
*		 pText - The pointer to the text used for the group box.
*				 Length of string must be checked not to exceed the 
*				 object’s width. Clipping is not supported for the 
*				 text of this object.
*		 pScheme - Pointer to the style scheme used for the object.
*				   Set to NULL if default style scheme is used.
*
* Output: Returns the pointer to the object created.
*
* Example:
*   <CODE> 
*	GOL_SCHEME *pScheme;
*	GROUPBOX *groupbox[2];
*	WORD state;
*
*	pScheme = GOLCreateScheme();
*	state = GB_DRAW | GB_RIGHT_ALIGN;
*	groupbox[0] = GbCreate( 10, 14,48,152,122,
*							state, "Power", scheme);
*	if (groupbox[0] == NULL)
*		return 0;
*	state = GB_DRAW;
*	groupbox[1] = GbCreate( 11, 160,48,298,122,
*							state, "Pressure", scheme);
*	if (groupbox[1] == NULL)
*		return 0;
*
*	while(!GbDraw(groupbox[0]));
*	while(!GbDraw(groupbox[1]));
*	return 1;
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
GROUPBOX    *GbCreate
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
* Function: WORD GbDraw(void *pObj)
*
* Overview: This function renders the object on the screen using 
*			the current parameter settings. Location of the object 
*			is determined by the left, top, right and bottom parameters. 
*			The colors used are dependent on the state of the object. 
*			The font used is determined by the style scheme set.
*			
*			When rendering objects of the same type, each object must 
*			be rendered completely before the rendering of the next object 
*			is started. This is to avoid incomplete object rendering.
*
* PreCondition: Object must be created before this function is called.
*
* Input: pGb - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing 
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Example:
*   See GbCreate() example.
*
* Side Effects: none
*
********************************************************************/
WORD GbDraw(void *pObj);
#endif //_GROUPBOX_H
