/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Static text
 *****************************************************************************
 * FileName:        StaticText.h
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
#ifndef _STATICTEXT_H
    #define _STATICTEXT_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define ST_DISABLED     0x0002  // Bit for disabled state.
    #define ST_RIGHT_ALIGN  0x0004  // Bit to indicate text is left aligned.
    #define ST_CENTER_ALIGN 0x0008  // Bit to indicate text is center aligned.
    #define ST_FRAME        0x0010  // Bit to indicate frame is displayed.
    #define ST_UPDATE       0x2000  // Bit to indicate that text area only is redrawn.
    #define ST_DRAW         0x4000  // Bit to indicate static text must be redrawn.
    #define ST_HIDE         0x8000  // Bit to remove object from screen.

/* Indent constant for the text used in the frame. */
    #define ST_INDENT   0x02        // Text indent constant.

/*********************************************************************
* Overview: Defines the parameters required for a Static Text Object.
*
*********************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;        // Generic header for all Objects (see OBJ_HEADER).
    SHORT       textHeight; // Pre-computed text height.
    XCHAR       *pText;     // The pointer to text used.
} STATICTEXT;

/*********************************************************************
* Macros:  StGetText(pSt)
*
* Overview: This macro returns the address of the current 
*			text string used for the object.
*
* PreCondition: none
*
* Input: pSt - Pointer to the object.
*
* Output: Returns the pointer to the text string used.
*
* Side Effects: none
*
********************************************************************/
    #define StGetText(pSt)  pSt->pText

/*********************************************************************
* Function: StSetText(STATICTEXT *pSt, XCHAR *pText)
*
* Overview: This function sets the string that will be used for the object.
*
* PreCondition: none
*
* Input: pSt - The pointer to the object whose text string will be modified. 
*        pText - The pointer to the string that will be used.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void        StSetText(STATICTEXT *pSt, XCHAR *pText);

/*********************************************************************
* Function: STATICTEXT  *StCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, 
*								  WORD state , XCHAR *pText, GOL_SCHEME *pScheme)
*
* Overview: This function creates a STATICTEXT object with the 
*			parameters given. It automatically attaches the new 
*			object into a global linked list of objects and returns 
*			the address of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the object.
* 		 top - Top most position of the object. 
*		 right - Right most position of the object.
*		 bottom - Bottom most position of the object.
*        state - Sets the initial state of the object.
*        pText - Pointer to the text used in the static text.
*        pScheme - Pointer to the style scheme. Set to NULL if 
*				   default style scheme is used.
*
* Output: Returns the pointer to the object created.
*
* Example:
*   <CODE> 
*	GOL_SCHEME *pScheme;
*	STATICTEXT *pSt;
*		
*		pScheme = GOLCreateScheme();
*		state = ST_DRAW | ST_FRAME | ST_CENTER_ALIGN;
*		StCreate(ID_STATICTEXT1,          // ID
*		         30,80,235,160,           // dimension
*		         state,                   // has frame and center aligned
*		         "Static Text\n Example", // text
*		         pScheme);                // use given scheme
*		
*		while(!StDraw(pSt));			  // draw the object
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
STATICTEXT  *StCreate
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
* Function: WORD StTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. The table below 
*			enumerates the translated messages for each event of the 
*			touch screen and keyboard inputs.
*
*	<TABLE>
*    	Translated Message   Input Source  Events         				Description
*     	##################   ############  ######         				###########
*		ST_MSG_SELECTED      Touch Screen  EVENT_PRESS, EVENT_RELEASE   If events occurs and the x,y position falls in the area of the static text.
*		OBJ_MSG_INVALID		 Any		   Any							If the message did not affect the object.
*	</TABLE>
*
* PreCondition: none
*
* Input: pSt   - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		  - ST_MSG_SELECTED – Static Text is selected
*    	  - OBJ_MSG_INVALID – Static Text is not affected
*
* Example:
*   Usage is similar to BtnTranslateMsg() example.
*
* Side Effects: none
*
********************************************************************/
WORD        StTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD StDraw(void *pObj)
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
*			object is started. This is to avoid incomplete object rendering.
*
* PreCondition: Object must be created before this function is called.
*
* Input: pSt - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Example:
*   See StCreate() Example.
*
* Side Effects: none
*
********************************************************************/
WORD StDraw(void *pObj);
#endif // _STATICTEXT_H
