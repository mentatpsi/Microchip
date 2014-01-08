/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Slider
 *****************************************************************************
 * FileName:        Slider.h
 * Dependencies:    none 
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
 * 11/12/07    Version 1.0 release
 * 08/12/10    Added casting to pointers in macros.
 *****************************************************************************/
#ifndef _SLIDER_H
    #define _SLIDER_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define SLD_FOCUSED     0x0001  // Bit for focus state
    #define SLD_DISABLED    0x0002  // Bit for disabled state
    #define SLD_VERTICAL    0x0004  // Bit for orientation  (0 - horizontal, 1 - vertical)
    #define SLD_SCROLLBAR   0x0010  // Bit for type usage (0 - Slider, 1 - ScrollBar)
    #define SLD_DRAW_THUMB  0x1000  // Bit to indicate that only thumb area must be redrawn
    #define SLD_DRAW_FOCUS  0x2000  // Bit to indicate that only the focus will be redrawn
    #define SLD_DRAW        0x4000  // Bit to indicate whole slider must be redrawn
    #define SLD_HIDE        0x8000  // Bit to remove object from screen

/*********************************************************************
* Overview: Defines the parameters required for a slider/scrollbar Object.
* 			Depending on the SLD_SCROLLBAR state bit slider or scrollbar mode 
*			is set. If SLD_SCROLLBAR is set, mode is scrollbar; if not set
*			mode is slider. For scrollbar mode, focus rectangle is not drawn.
*
*********************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;        // Generic header for all Objects (see OBJ_HEADER).
    WORD        currPos;    // Position of the slider relative to minimum.
    WORD        prevPos;    // Previous position of the slider relative to minimum.
    WORD        range;      // User defined range of the slider. Minimum value at 0 and maximum at 0x7FFF.
    WORD        pos;        // Position of the slider in range domain.
    WORD        page;       // User specified resolution to incrementally change the position

    // in range domain.
    WORD        thWidth;    // Thumb width. This is computed internally.

    // User must not change this value.
    WORD        thHeight;   // Thumb width. This is computed internally.
    // User must not change this value.
} SLIDER;

/*********************************************************************
* Function:  SldSetRange(SLIDER *pSld, SHORT newRange)
*
* Overview: This sets the range of the thumb. If this field is changed
*			Object must be completely redrawn to reflect the change.
*
* PreCondition: none
*
* Input: pSld - Pointer to the object.
*        newRange - Value of the new range used.
*
* Output: None.
*
* Example:
*   <CODE> 
*	SLIDER *pSld;
*
*		SldSetRange(pSld, 100);
*		// to completely redraw the object when GOLDraw() is executed.
*		SetState(pSld, SLD_DRAW);		
*												
*	</CODE> 			
*
* Side Effects: Position of the thumb may change when redrawn.
*
********************************************************************/
void    SldSetRange(SLIDER *pSld, SHORT newRange);

/*********************************************************************
* Macros:  SldGetRange(pSld)
*
* Overview: Returns the current range of the thumb. 
*
* PreCondition: none
*
* Input: pSld - Pointer to the object.
*
* Output: Returns the current range of the slider thumb.
*
* Example:
*   <CODE> 
*	WORD range;
*	SLIDER *pSld;
*
*		range = SldGetRange(pSld);
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
    #define SldGetRange(pSld)   (((SLIDER*)pSld)->range)

/*********************************************************************
* Function: void SldSetPage(SLIDER *pSld, WORD newPage) 
*
* Overview: This sets the page size of the object. Page size defines 
*			the delta change of the thumb position when incremented 
*      		via SldIncPos() or decremented via SldDecPos(). Page size 
*			minimum value is 1. Maximum value is range/2.
*
* PreCondition: none
*
* Input: pSld - Pointer to the object.
*        newPage - Value of the new page used.
*
* Output: None.
*
* Example:
*	See SldIncPos() example.
*
* Side Effects: Position of the thumb may change when redrawn.
*
********************************************************************/
void    SldSetPage(SLIDER *pSld, WORD newPage);

/*********************************************************************
* Macros:  SldGetPage(pSld)
*
* Overview: Returns the current page size of the object. Page size defines 
*			the delta change of the thumb position when incremented 
*      		via SldIncPos() or decremented via SldDecPos(). Page size 
*			minimum value is 1. Maximum value is range/2. 
*
* PreCondition: none
*
* Input: pSld - Pointer to the object.
*
* Output: Returns the current value of the slider page.
*
* Example:
*   <CODE> 
*	WORD page;
*	SLIDER *pSld;
*
*		page = SldGetPage(pSld);
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
    #define SldGetPage(pSld)    (((SLIDER*)pSld)->page)

/*********************************************************************
* Macros:  SldGetPos(pSld)
*
* Overview: Returns returns the current position of the slider thumb. 
*
* PreCondition: none
*
* Input: pSld - Pointer to the object.
*
* Output: Returns the current position of the slider's thumb.
*
* Example:
*   <CODE> 
*	#define MAXVALUE 100;
*	
*	SLIDER *pSlider;
*	DWORD ctr = 0;
*	
*		// create slider here and initialize parameters
*		SetState(pSlider, SLD_DRAW);
*		SldDraw(pSlider);
*	
*		while("some condition") {
*			SldSetPos(pSlider, ctr);
*			SetState(pSlider, SLD_DRAW_THUMB);
*			SldDraw(pSlider);
*			// update ctr here
*			ctr = "some source of value";
*		}
*	
*		if (SldGetPos(pSlider) > (MAXVALUE – 1))
*		   return 0;
*		else  
*			"do something else"
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
    #define SldGetPos(pSld) (((SLIDER*)pSld)->pos)

/*********************************************************************
* Function:  SldSetPos(SLIDER *pSld, SHORT newPos)
*
* Overview: This function sets the position of the slider thumb. 
*			Value should be in the set range inclusive. Object must 
*			be redrawn to reflect the change.
*
* PreCondition: none
*
* Input: pSld - Pointer to the object.
*		 newPos - The new position of the slider's thumb.
*
* Output: none
*
* Example:
*   <CODE> 
*	SLIDER *pSlider;
*	DWORD ctr = 0;
*	
*		// create slider here and initialize parameters
*		SetState(pSlider, SLD_DRAW);
*		SldDraw(pSlider);
*	
*		while("some condition") {
*			SldSetPos(pSlider, ctr);
*			SetState(pSlider, SLD_DRAW_THUMB);
*			SldDraw(pSlider);
*			// update ctr here
*			ctr = "some source of value";
*		}
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
void    SldSetPos(SLIDER *pSld, SHORT newPos);

/*********************************************************************
* Macros:  SldIncPos(pSld)
*
* Overview: This macro increment the slider position by the delta 
*			change (page) value set. Object must be redrawn after 
*			this function is called to reflect the changes to the object.
*
* PreCondition: none
*
* Input: pSld - Pointer to the object.
*
* Output: none
*
* Example:
*   <CODE> 
*	void ControlSpeed(SLIDER* pSld, int setSpeed, int curSpeed)
*	{
*		SldSetPage(pSld, 1);					// set page size to 1
*		if (setSpeed < curSpeed) {
*       	while(SldGetPos(pSld) < SetSpeed)
*				SldIncPos(pSld);              	// increment by 1
*	   	}
*	   	else if (setSpeed > curSpeed) {
*	   		while(SldGetPos(pSld) > SetSpeed)   
*				SldDecPos(pSld);              	// decrement by 1
*  		}
*	}
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
    #define SldIncPos(pSld)                                                                        \
    SldSetPos                                                                                      \
    (                                                                                              \
        pSld,                                                                                      \
        (((DWORD) pSld->pos + (DWORD) ((SLIDER*)pSld)->page) <= (DWORD) ((SLIDER*)pSld)->range) ?  \
        				(((SLIDER*)pSld)->pos + ((SLIDER*)pSld)->page) : ((SLIDER*)pSld)->range    \
    )

/*********************************************************************
* Macros:  SldDecPos(pSld)
*
* Overview: This macro decrement the slider position by the delta 
*			change (page) value set. Object must be redrawn after 
*			this function is called to reflect the changes to the object.
*
* PreCondition: none
*
* Input: pSld - Pointer to the object.
*
* Output: none
*
* Example:
*   See SldIncPos() example.
*
* Side Effects: none
*
********************************************************************/
    #define SldDecPos(pSld)                                                                        \
    SldSetPos                                                                                      \
    (                                                                                              \
    	pSld,                                                                                      \
    	(((LONG) ((SLIDER*)pSld)->pos - (LONG) ((SLIDER*)pSld)->page) >= 0) ?                      \
    					(((SLIDER*)pSld)->pos - ((SLIDER*)pSld)->page) : 0                         \
	)

/*********************************************************************
* Function: SLIDER *SldCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*							  SHORT bottom, WORD state, SHORT range, 
*							  SHORT page, SHORT pos, GOL_SCHEME *pScheme);
*
* Overview: This function creates a SLIDER object with the parameters given. 
*			Depending on the SLD_SCROLLBAR state bit slider or scrollbar mode 
*			is set. If SLD_SCROLLBAR is set, mode is scrollbar; if not set
*			mode is slider. It automatically attaches the new object into a 
*			global linked list of objects and returns the address of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the Object.
* 		 top - Top most position of the Object. 
*		 right - Right most position of the Object.
*		 bottom - Bottom most position of the Object.
*		 state - This defines the initial state of the Object.
*		 range - This specifies the maximum value of the slider when the
*				 thumb is on the rightmost position for a horizontal orientation
*				 and bottom most position for the vertical orientation. Minimum
*				 value is always at zero.
*		 page - This is the incremental change of the slider when user action
*			   requests to move the slider thumb. This value is added or 
*			   subtracted to the current position of the thumb.
*		 pos - This defines the initial position of the thumb.
*		 pScheme - The pointer to the style scheme used for the Object.
*				   Set to NULL if default style scheme is used.
*
* Output: Returns the pointer to the object created.
*
* Example:
*   <CODE> 
*	GOL_SCHEME *pScheme;
*	SLIDER *slider[3];
*	WORD state;
*	
*		pScheme = GOLCreateScheme();
*		
*		// create a slider with
*		//       range = [0 – 50000]
*		//       delta = 500
*		//       initial position = 25000
*		state = SLD_DRAW;
*		slider[0] = SldCreate(  5,
*								150, 145, 285, 181,
*								state,
*								50000, 500, 25000,
*								pScheme);
*		if (slider[0] == NULL)
*			return 0;
*	
*	 	// create a scrollbar with
*		//       range = [0 – 100]
*		//       delta = 20
*		//       initial position = 0
*	
*		state = SLD_DRAW|SLD_SCROLLBAR;
*	 	slider[1] = SldCreate(  6,
*								150, 190, 285, 220,
*								state,
*								100, 20, 0,
*								pScheme);
*		if (slider[1] == NULL)
*			return 0;
*	 
*		// create a vertical slider with
*		//       range = [0 – 30]
*		//       delta = 2
*		//       initial position = 20
*	
*	 	state = SLD_DRAW|SLD_VERTICAL;
*		slider[2] = SldCreate(  7,
*								120, 145, 140, 220,
*								state,
*								30, 2, 20,
*								pScheme);
*		if (slider[2] == NULL)
*			return 0;
*	
*		// draw the sliders
*		while(!sldDraw(slider[0]);		// draw the horizontal slider
*		while(!sldDraw(slider[1]);		// draw the horizontal scroll bar
*		while(!sldDraw(slider[2]);		// draw the vertical slider
*		return 1;
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
SLIDER  *SldCreate
        (
            WORD        ID,
            SHORT       left,
            SHORT       top,
            SHORT       right,
            SHORT       bottom,
            WORD        state,
            WORD        range,
            WORD        page,
            WORD        pos,
            GOL_SCHEME  *pScheme
        );

/*********************************************************************
* Function: void SldMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
* Overview: This function performs the actual state change 
*			based on the translated message given. The following state changes 
*			are supported:
*	<TABLE>
*    	Translated Message   Input Source	  Set/Clear State Bit	Description
*     	##################   ############	  ######     			###########
*     	SLD_MSG_INC       	 Touch Screen,	  Set SLD_DRAW_THUMB  	Slider will be redrawn with thumb in the incremented position.
*     					      Keyboard	     				
*		SLD_MSG_DEC	 		 Touch Screen, 	  Set SLD_DRAW_THUMB	Slider will be redrawn with thumb in the decremented position.
*							  Keyboard        				
*	</TABLE>
*
* PreCondition: none
*
* Input: translatedMsg - The translated message.
*        pSld          - The pointer to the object whose state will be modified.
*        pMsg          - The pointer to the GOL message.
*        
* Output: none
*
* Example:
*	Usage is similar to BtnTranslateMsg() example.
*
* Side Effects: none
*
********************************************************************/
void    SldMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD SldTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. The table below 
*			enumerates the translated messages for each event of the 
*			touch screen and keyboard inputs.
*
*	<TABLE>
*    	Translated Message   Input Source  Events         				Description
*     	##################   ############  ######         				###########
*		SLD_MSG_INC			 Touch Screen  EVENT_PRESS, EVENT_MOVE		If events occurs and the x,y position falls in the area of the slider and the slider position is to the LEFT of the x,y position for a horizontal slider or BELOW the x,y position for a vertical slider.
*							 Keyboard 	   EVENT_KEYSCAN				If event occurs and parameter1 passed matches the object’s ID and parameter 2 passed matches SCAN_UP_PRESSED or SCAN_LEFT_PRESSED.
*		SLD_MSG_DEC			 Touch Screen  EVENT_PRESS, EVENT_MOVE		If events occurs and the x,y position falls in the area of the slider and the slider position is to the RIGHT of the x,y position for a horizontal slider or ABOVE the x,y position for a vertical slider.
*							 Keyboard	   EVENT_KEYSCAN				If event occurs and parameter1 passed matches the object’s ID and parameter 2 passed matches SCAN_DOWN_PRESSED or SCAN_RIGHT_PRESSED.
*		OBJ_MSG_PASSIVE		 Touch Screen  EVENT_RELEASE				If events occurs and the x,y position falls in the area of the slider.
*		OBJ_MSG_INVALID		 Any		   Any							If the message did not affect the object.
*	</TABLE>
*
* PreCondition: none
*
* Input: pSld  - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		  - SLD_MSG_INC – Increment slider position
*         - SLD_MSG_DEC – Decrement slider position
*     	  - OBJ_MSG_PASSIVE – Slider is not affected
*     	  - OBJ_MSG_INVALID – Slider is not affected
*
* Example:
*	Usage is similar to BtnTranslateMsg() example.
*
* Side Effects: none
*
********************************************************************/
WORD    SldTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD SldDraw(void *pObj)
*
* Overview: This function renders the object on the screen using 
* 			the current parameter settings. Location of the object is 
*			determined by the left, top, right and bottom parameters. 
*			The colors used are dependent on the state of the object. 
*			
*			When rendering objects of the same type, each object 
*			must be rendered completely before the rendering of the 
*			next object is started. This is to avoid incomplete 
*			object rendering.
*
* PreCondition: Object must be created before this function is called.
*
* Input: pSld - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing 
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Example:
*   See SldCreate() example.
*
* Side Effects: none
*
********************************************************************/
WORD SldDraw(void *pObj);
#endif //_SLIDER_H
