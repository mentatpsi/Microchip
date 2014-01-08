/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Progress Bar
 *****************************************************************************
 * FileName:        ProgressBar.h
 * Dependencies:    None 
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30 MPLAB LINK32
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
 * 11/04/11     Added state bit PB_NOPROGRESS to indicate the progress in 
 *              text will not be shown.
 *****************************************************************************/
#ifndef _PROGRESSBAR_H
    #define _PROGRESSBAR_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define PB_DISABLED     0x0002  // Bit to indicate Progress Bar is in a disabled state.
	#define PB_VERTICAL     0x0004  // Bit for orientation  (0 - horizontal, 1 - vertical)
    #define PB_NOPROGRESS   0x0008  // Bit to suppress rendering of progress in text
    #define PB_HIDE         0x8000  // Bit to indicate Progress Bar must be hidden.
    #define PB_DRAW_BAR     0x2000  // Bit to indicate Progress Bar must be redrawn.
    #define PB_DRAW         0x4000  // Bit to indicate Progress Bar must be redrawn.

/*****************************************************************************
 * Overview: The structure contains data for the progress bar
 *****************************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;        // Generic header for all Objects (see OBJ_HEADER).
    WORD        pos;        // Current progress position.
    WORD        prevPos;    // Previous progress position.
    WORD        range;      // Sets the range of the object.
} PROGRESSBAR;

/*********************************************************************
* Macros:  PbGetPos(pPb)
*
* Overview: This macro returns the current progress bar position. 
*
* PreCondition: none
*
* Input: pPb - Pointer to the object 
*
* Output: Returns the progress bar position.
*
* Example:
*   See PbSetPos() exmaple. 
*
* Side Effects: none
*
********************************************************************/
    #define PbGetPos(pPb)   pPb->pos

/*********************************************************************
* Function: void PbSetPos(PROGRESSBAR *pPb, WORD position)
*
* Overview: This function sets the position of the progress bar. 
*			Position should be in the given range inclusive.
*
* PreCondition: none
*
* Input: pPb - Pointer to the object 
*	     position - New position.
*
* Output: none
*
* Example:
*   <CODE> 
*	PROGRESSBAR *pPb;
*	BYTE  direction = 1;
*		
*		// this code increments and decrements the progress bar by 1
*		// assume progress bar was created and initialized before
*		while (1) {
*			if(direction) {
*				if(pPb ->pos == pPb ->range)
*					direction = 0;
*				else
*					PbSetPos(pPb,PbGetPos(pPb)+1);
*			} else {
*				if(pPb ->pos == 0)
*					direction = 1;
*				else
*					PbSetPos(pPb,PbGetPos(pPb)-1);
*			}
*		}
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
void        PbSetPos(PROGRESSBAR *pPb, WORD position);

/*********************************************************************
* Function: PROGRESSBAR  *PbCreate( WORD ID, SHORT left, SHORT top, 
*									SHORT right, SHORT bottom, WORD state, 
*									WORD pos, WORD range, GOL_SCHEME *pScheme)
*
* Overview: This function creates a PROGRESSBAR object with the parameters 
*			given. It automatically attaches the new object into a global 
*			linked list of objects and returns the address of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the Object.
* 		 top - Top most position of the Object. 
*		 right - Right most position of the Object.
*		 bottom - Bottom most position of the Object.
*        state - Sets the initial state of the Object.
*        pos - Defines the initial position of the progress.
*		 range - This specifies the maximum value of the progress 
*				 bar when the progress bar is at 100% position.
*        pScheme - Pointer to the style scheme used for the object. 
*				   Set to NULL if default style scheme is used.
*
* Output: Returns the pointer to the object created
*
* Example:
*   <CODE> 
*	PROGRESSBAR *pPBar;
*	void CreateProgressBar(){
*		pPBar = PbCreate(ID_PROGRESSBAR1,    // ID
*						 50,90,270,140,      // dimension
*						 PB_DRAW,            // Draw the object
*						 25,                 // position
*						 50,                 // set the range
*						 NULL);              // use default GOL scheme
*		while(!PbDraw(pPBar));
*	}
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
PROGRESSBAR *PbCreate
            (
                WORD        ID,
                SHORT       left,
                SHORT       top,
                SHORT       right,
                SHORT       bottom,
                WORD        state,
                WORD        pos,
                WORD        range,
                GOL_SCHEME  *pScheme
            );

/*********************************************************************
* Function: WORD PbTranslateMsg(PROGRESSBAR *pPb, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. The table below 
*			enumerates the translated messages for each event of 
*			the touch screen inputs.
*
*	<TABLE>
*    	Translated Message   Input Source  Events         							Description
*     	##################   ############  ######         							###########
*		PB_MSG_SELECTED      Touch Screen  EVENT_PRESS, EVENT_RELEASE, EVENT_MOVE	If events occurs and the x,y position falls in the area of the progress bar.
*		OBJ_MSG_INVALID		 Any		   Any										If the message did not affect the object.
*	</TABLE>
*
* PreCondition: none
*
* Input: pPb   - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		 - PB_MSG_SELECTED – Progress Bar is selected.
* 		 - OBJ_MSG_INVALID – Progress Bar is not affected
*
* Example:
*   Usage is similar to BtnTranslateMsg() example.
*
* Side Effects: none
*
********************************************************************/
WORD        PbTranslateMsg(PROGRESSBAR *pPb, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD PbDraw(void *pObj)
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
* Input: pPb - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing 
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Example:
*   See PbCreate() example.
*
* Side Effects: none
*
********************************************************************/
WORD PbDraw(void *pObj);

/*********************************************************************
* Function:  PbSetRange(PROGRESSBAR *pPb, WORD range)
*
* Overview: This function sets the range of the progress bar. Calling
*		    this function also resets the position equal to the new
*			range value. 
*
* PreCondition: none
*
* Input: pPb - Pointer to the object 
*
* Output: none.
*
* Side Effects: Sets the position equal to the new range.
*
********************************************************************/
void        PbSetRange(PROGRESSBAR *pPb, WORD range);

/*********************************************************************
* Macros:  PbGetRange(pPb)
*
* Overview: This macro returns the current range of the progress bar.
*
* PreCondition: none
*
* Input: pPb - Pointer to the object 
*
* Output: Returns the range value.
*
* Side Effects: none
*
********************************************************************/
    #define PbGetRange(pPb) (pPb->range)
#endif // _PROGRESSBAR_H
