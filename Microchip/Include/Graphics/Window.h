/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Window
 *****************************************************************************
 * FileName:        Window.h
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
#ifndef _WINDOW_H
    #define _WINDOW_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

// Indent for the title bar text from the left side of bitmap or title bar emboss
    #define WND_INDENT  2

// Height of the title bar
    #define WND_TITLE_HEIGHT    35

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define WND_FOCUSED     0x0001  // Bit for focus state
    #define WND_DISABLED    0x0002  // Bit for disabled state
    #define WND_TITLECENTER 0x0004  // Bit to center the text on the Title Area
    #define WND_HIDE        0x8000  // Bit to indicate window must be removed from screen
    #define WND_DRAW_CLIENT 0x4000  // Bit to indicate client area must be redrawn
    #define WND_DRAW_TITLE  0x2000  // Bit to indicate title area must be redrawn
    #define WND_DRAW        0x6000  // Bits to indicate whole window must be redrawn

/*****************************************************************************
 * Overview: The structure contains data for the window
 *****************************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;        // Generic header for all Objects (see OBJ_HEADER).
    SHORT       textHeight; // Pre-computed text height
    XCHAR       *pText;     // Pointer to the title text
    void        *pBitmap;   // Pointer to the bitmap for the title bar
} WINDOW;

/*********************************************************************
* Function: WINDOW  *WndCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, WORD state,  void* pBitmap, XCHAR* pText,
*                              GOL_SCHEME *pScheme)
*
* Overview: This function creates a WINDOW object with the parameters 
*			given. It automatically attaches the new object into a 
*			global linked list of objects and returns the address 
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
*        pBitmap - Pointer to the bitmap used in the title bar.
*        pText - Pointer to the text used as a title of the window.
*        pScheme - Pointer to the style scheme used.
*
* Output: Returns the pointer to the object created
*
* Example:
*   <CODE> 
*	WINDOW *pWindow;
*		pWindow  = WndCreate(ID_WINDOW1,				// ID
*					         0,0,GetMaxX(),GetMaxY(),	// whole screen dimension
*				             WND_DRAW,					// set state to draw all
*					         (char*)myIcon,				// icon
*				             "Place Title Here.",		// text
*				             NULL);						// use default GOL scheme
*
*		if (pWindow == NULL)
*			return 0;
*		WndDraw(pWindow);
*		return 1;
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
WINDOW  *WndCreate
        (
            WORD        ID,
            SHORT       left,
            SHORT       top,
            SHORT       right,
            SHORT       bottom,
            WORD        state,
            void        *pBitmap,
            XCHAR       *pText,
            GOL_SCHEME  *pScheme
        );

/*********************************************************************
* Macros:  WndGetText(pW)
*
* Overview: This macro returns the address of the current 
*			text string used for the title bar.
*
* PreCondition: none
*
* Input: pW - Pointer to the object 
*
* Output: Returns pointer to the text string being used.
*
* Example:
*   <CODE> 
*	WINDOW *pWindow;
*	XCHAR textUsed = “USE THIS!”;
*
*		if (WndGetText(pWindow) == NULL)
*			WndSetText(&textUsed);
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
    #define WndGetText(pW)  pW->pText

/*********************************************************************
* Function: WndSetText(WINDOW *pW, XCHAR *pText)
*
* Overview: This function sets the string used for the title bar.
*
* PreCondition: none
*
* Input: pW - The pointer to the object whose text will be modified 
*        pText - Pointer to the text that will be used
*
* Output: none
*
* Example:
*  See WndGetText() example.
*
* Side Effects: none
*
********************************************************************/
void    WndSetText(WINDOW *pW, XCHAR *pText);

/*********************************************************************
* Function: WORD WndTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if 
*			the message will affect the object or not. The table 
*			below enumerates the translated messages for each 
*			event of the touch screen inputs.
*
*	<TABLE>
*    	Translated Message   Input Source  Events         							Description
*     	##################   ############  ######         							###########
*		WND_MSG_TITLE		 Touch Screen  EVENT_PRESS, EVENT_RELEASE, EVENT_MOVE	If events occurs and the x,y position falls in the TITLE area of the window
*		WND_MSG_CLIENT		 Touch Screen  EVENT_PRESS, EVENT_RELEASE, EVENT_MOVE	If events occurs and the x,y position falls in the CLIENT area of the window
*		OBJ_MSG_INVALID		 Any		   Any			  							If the message did not affect the object.							 
*	</TABLE>
*
* PreCondition: none
*
* Input: pW    - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		- WND_MSG_TITLE – Title area is selected
* 		- WND_MSG_CLIENT – Client area is selected
* 		- OBJ_MSG_INVALID – Window is not affected
*
* Example:
*   Usage is similar to BtnTranslateMsg() example.
*
* Side Effects: none
*
********************************************************************/
WORD    WndTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD WndDraw(void *pObj)
*
* Overview: This function renders the object on the screen 
*			using the current parameter settings. Location of 
*			the object is determined by the left, top, right 
*			and bottom parameters. The colors used are dependent 
*			on the state of the object. The font used is 
*			determined by the style scheme set.
*			
*			When rendering objects of the same type, each object 
*			must be rendered completely before the rendering of the 
*			next object is started. This is to avoid incomplete 
*			object rendering.
*
* PreCondition: Object must be created before this function is called.
*
* Input: pW - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing 
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Example:
*   See WndCreate() example.
*
* Side Effects: none
*
********************************************************************/
WORD WndDraw(void *pObj);
#endif // _WINDOW_H
