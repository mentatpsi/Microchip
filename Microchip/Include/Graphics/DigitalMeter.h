/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Static text
 *****************************************************************************
 * FileName:        DigitalMeter.h
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 06/11/09	   Version 1.0 release
 * 01/18/10	   Added draw state to redraw only text.
 *****************************************************************************/
#ifndef _DIGITALMETER_H
    #define _DIGITALMETER_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define DM_DISABLED     0x0002  // Bit for disabled state.
    #define DM_RIGHT_ALIGN  0x0004  // Bit to indicate value is left aligned.
    #define DM_CENTER_ALIGN 0x0008  // Bit to indicate value is center aligned.
    #define DM_FRAME        0x0010  // Bit to indicate frame is displayed.
    #define DM_DRAW         0x4000  // Bit to indicate object must be redrawn.
    #define DM_UPDATE       0x2000  // Bit to indicate that only text must be redrawn.
    #define DM_HIDE         0x8000  // Bit to remove object from screen.

/* Indent constant for the text used in the frame. */
    #define DM_INDENT   0x02        // Text indent constant.

/* User should change this value depending on the number of digits he wants to display */
    #define DM_WIDTH    0x0A        // This value should be more than the no of digits displayed

/*********************************************************************
* Structure:   DIGITALMETER
* Overview: Defines the parameters required for a <link Digital Meter> Object.
*********************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;        // Generic header for all Objects (see OBJ_HEADER).
    SHORT       textHeight; // Pre-computed text height
    DWORD       Cvalue;     // Current value
    DWORD       Pvalue;     // Previous value
    BYTE        NoOfDigits; // Number of digits to be displayed
    BYTE        DotPos;     // Position of decimal point
} DIGITALMETER;

/*********************************************************************
* Macros:  DmGetValue(pDm)
*
* Overview: This macro returns the current 
*			value used for the object.
*
* PreCondition: none
*
* Input: pDm - Pointer to the object.
*
* Output: Returns the value used.
*
* Side Effects: none
*
********************************************************************/
    #define DmGetValue(pDm) pDm->Cvalue

/*********************************************************************
* Function: DmSetValue(DIGITALMETER *pDm,  DWORD Value)
*
* Overview: This function sets the value that will be used for the object.
*
* PreCondition: none
*
* Input: pDm - The pointer to the object whose value will be modified. 
* 		 Value -  New value to be set for the <link Digital Meter>.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void    DmSetValue(DIGITALMETER *pDm, DWORD Value);

/*********************************************************************
* Macros:  DmIncVal(pDm, deltaValue)
*
* Overview: This macro is used to directly increment the value. 
*
* PreCondition: none
*
* Input: pDm - Pointer to the object.
*		 deltaValue - Number to be added to the current <link Digital Meter> value.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define DmIncVal(pDm, deltaValue)    DmSetValue(pDm, (pDm->Cvalue + deltaValue))

/*********************************************************************
* Macros:  DmDecVal(pDm, deltaValue)
*
* Overview: This macro is used to directly decrement the value. 
*
* PreCondition: none
*
* Input: pDm - Pointer to the object.
*        deltaValue - Number to be subtracted to the current <link Digital Meter> value.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define DmDecVal(pDm, deltaValue)    DmSetValue(pDm, (pDm->Cvalue - deltaValue))

/*********************************************************************
* Function: DIGITALMETER  *DmCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, 
*								  WORD state , DWORD Value, BYTE NoOfDigits, BYTE DotPos, GOL_SCHEME *pScheme)
*
* Overview: This function creates a DIGITALMETER object with the 
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
*        Value - Sets the initial value to be displayed
*        NoOfDigits - Sets the number of digits to be displayed
*        DotPos - Sets the position of decimal point in the display
*        pScheme - Pointer to the style scheme. Set to NULL if 
*				   default style scheme is used.
*
* Output: Returns the pointer to the object created.
*
* Example:
*   <PRE> 
*	GOL_SCHEME *pScheme;
*	DIGITALMETER *pDm;
*		
*		pScheme = GOLCreateScheme();
*		state = DM_DRAW | DM_FRAME | DM_CENTER_ALIGN;
*		DmCreate(ID_DIGITALMETER1,          // ID
*		         30,80,235,160,           // dimension
*		         state,                   // has frame and center aligned
*		         789,4,1,                 // to display 078.9
*		         pScheme);                // use given scheme
*		
*		while(!DmDraw(pDm));			  // draw the object
*	</PRE> 
*
* Side Effects: none
*
********************************************************************/
DIGITALMETER    *DmCreate
                (
                    WORD        ID,
                    SHORT       left,
                    SHORT       top,
                    SHORT       right,
                    SHORT       bottom,
                    WORD        state,
                    DWORD       Value,
                    BYTE        NoOfDigits,
                    BYTE        DotPos,
                    GOL_SCHEME  *pScheme
                );

/*********************************************************************
* Function: WORD DmTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. The table below 
*			enumerates the translated messages for each event of the 
*			touch screen and keyboard inputs.
*
*	<TABLE>
*    	Translated Message   Input Source  Events         				Description
*     	##################   ############  ######         				###########
*		DM_MSG_SELECTED      Touch Screen  EVENT_PRESS, EVENT_RELEASE   If events occurs and the x,y position falls in the area of the <link Digital Meter>.
*		OBJ_MSG_INVALID		 Any		   Any							If the message did not affect the object.
*	</TABLE>
*
* PreCondition: none
*
* Input: pDm   - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		  - DM_MSG_SELECTED – <link Digital Meter> is selected
*    	  - OBJ_MSG_INVALID – <link Digital Meter> is not affected
*
* Example:
*   Usage is similar to BtnTranslateMsg() example.
*
* Side Effects: none
*
********************************************************************/
WORD            DmTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD DmDraw(void *pObj)
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
* Input: pDm - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Example:
*   See DmCreate() Example.
*
* Side Effects: none
*
********************************************************************/
WORD DmDraw(void *pObj); 
#endif // _DIGITALMETER_H
