/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Round Dial
 *****************************************************************************
 * FileName:        RoundDial.h
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
 * Date            	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07	    	Version 1.0 release
 * 03/12/09	     	Added Object Header for Double Buffering Support
 * 11/15/10	     	Added new structure member for KEYBOARD support 
 *****************************************************************************************/
#ifndef _ROUNDDIAL_H
    #define _ROUNDDIAL_H

    #include "GenericTypeDefs.h"
    #include "GraphicsConfig.h"
    #include "Graphics/GOL.h"
/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define RDIA_DISABLED   0x0002  // Bit for disabled state.
    #define RDIA_ROT_CW     0x0004  // Bit for rotate clockwise state.
    #define RDIA_ROT_CCW    0x0008  // Bit for rotate counter clockwise state.
    #define RDIA_DRAW       0x4000  // Bit to indicate object must be redrawn.
    #define RDIA_HIDE       0x8000  // Bit to indicate object must be removed from screen.
#ifdef USE_KEYBOARD
    #define RDIA_QUADRANT_POSITIONS 6
    #define RDIA_MAX_POSITIONS      24
	extern SHORT    _cosine[RDIA_QUADRANT_POSITIONS];
	SHORT           RdiaCosine(SHORT v);
	SHORT           RdiaSine(SHORT v);
#endif

/*********************************************************************
* Overview: Defines the parameters required for a dial Object.
* 			The curr_xPos, curr_yPos, new_xPos and new_yPos parameters
*			are internally generated to aid in the redrawing of the 
* 			dial. User must avoid modifying these values. 
*
*********************************************************************/
typedef struct
{
    OBJ_HEADER      hdr;        // Generic header for all Objects (see OBJ_HEADER).
    SHORT           xCenter;        // x coordinate center position.
    SHORT           yCenter;        // y coordinate center position.
    SHORT           radius;         // Radius of the dial.
    SHORT           value;          // Initial value of the dial.
    WORD            max;            // Maximum value of variable value (maximum = 65535).

    // Minimum is always zero.
    WORD            res;            // Resolution of movement.
    SHORT           curr_xPos;      // Current x position.
    SHORT           curr_yPos;      // Current y position.
    SHORT           new_xPos;       // New x position.
    SHORT           new_yPos;       // New y position.
    #ifdef USE_KEYBOARD
    SHORT 			vAngle;
    #endif
} ROUNDDIAL;

/*********************************************************************
* Function: ROUNDDIAL  *RdiaCreate(	WORD ID, SHORT x, SHORT y, SHORT radius, 
*						WORD state, SHORT res, SHORT value, SHORT max, 
*						GOL_SCHEME *pScheme);				 
*
* Overview: This function creates a ROUNDDIAL object with the parameters given. 
*			It automatically attaches the new object into a global linked list of 
*			objects and returns the address of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        x - Location of the center of the dial in the x coordinate.
*        y - Location of the center of the dial in the y coordinate.
* 		 radius - Defines the radius of the dial. 
*        state - Sets the initial state of the object.
*        res - Sets the resolution of the dial when rotating clockwise or
*			   counter clockwise.
*        value - Sets the initial value of the dial.
*        max - Sets the maximum value of the dial.
*        pScheme - Pointer to the style scheme used.
*
* Output: Returns the pointer to the object created.
*
* Example:
*   <CODE> 
*	GOL_SCHEME *pScheme;
*	ROUNDDIAL *pDial;
*	WORD state;
*	
*		pScheme = GOLCreateScheme();
*		state = RDIA_DRAW;
*
*		// creates a dial at (50,50) x,y location, with an initial value
*		// of 50, a resolution of 2 and maximum value of 100.
*		pDial = RdiaCreate(1,50,50,25,118,0, state, 2, 50, 100, pScheme);
*		// check if dial was created
*		if (pDial == NULL)		
*		   return 0;
*
*		return 1;
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
ROUNDDIAL   *RdiaCreate
            (
                WORD        ID,
                SHORT       x,
                SHORT       y,
                SHORT       radius,
                WORD        state,
                SHORT       res,
                SHORT       value,
                SHORT       max,
                GOL_SCHEME  *pScheme
            );

/*********************************************************************
* Function: RdiaTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. The table below enumerates the translated 
*			messages for each event of the touch screen inputs.
*
*	<TABLE>
*    	Translated Message      Input Source  Events        Description
*     	#####################   ############  ######        ###########
*     	RD_MSG_CLOCKWISE        Touch Screen  EVENT_MOVE	If events occurs and the x,y position falls in the face of the Dial and moving in the clockwise rotation.
*		RD_MSG_CTR_CLOCKWISE    Touch Screen  EVENT_MOVE	If events occurs and the x,y position falls in the face of the Dial and moving in the counter clockwise rotation.     					      			   
*		OBJ_MSG_INVALID		    Any		      Any			If the message did not affect the object.							 
*	</TABLE>
*
* PreCondition: none
*
* Input: pDia  - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		  - RD_MSG_CLOCKWISE – Dial is moved in a clockwise direction.
*         - RD_MSG_CTR_CLOCKWISE – Dial is moved in a counter clockwise direction.
*         - OBJ_MSG_INVALID – Dial is not affected
*
* Example:
*   <CODE> 
*	void MyGOLMsg(GOL_MSG *pMsg){
*
*		OBJ_HEADER *pCurrentObj;
*		WORD objMsg;
*			
*		if(pMsg->event == EVENT_INVALID)
*			return;
*		pCurrentObj = GOLGetList();
*		
*		while(pCurrentObj != NULL){
*			// Process only ROUNDDIAL
*			if(!IsObjUpdated(pCurrentObj)){
*				switch(pCurrentObj->type){
*					case OBJ_ROUNDIAL:
*						objMsg = RdiaTranslateMsg((ROUNDDIAL*)pCurrentObj, pMsg);
*						if(objMsg == OBJ_MSG_INVALID)
*							break;
*						if(GOLMsgCallback(objMsg,pCurrentObj,pMsg))
*							RdiaMsgDefault(objMsg,(ROUNDDIAL*)pCurrentObj);
*						break;
*					default: break;	
*				}
*			}
*		}
*		pCurrentObj = pCurrentObj->pNxtObj;
*	}
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
WORD        RdiaTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: RdiaMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
* Overview: This function performs the actual state change 
*			based on the translated message given. The following state changes 
*			are supported:
*	<TABLE>
*    	Translated Message   	Input Source  	Set/Clear State Bit		Description
*     	##################   	############  	######     				###########
*     	RD_MSG_CLOCKWISE	 	Touch Screen	Set RDIA_ROT_CW,		Dial will be redrawn with clockwise update.
*										    	 Set RDIA_DRAW
*		RD_MSG_CTR_CLOCKWISE 	Touch Screen  	Set RDIA_ROT_CCW,		Dial will be redrawn with counter clockwise update.
*										    	 Set RDIA_DRAW
*	</TABLE>
*
* PreCondition: none
*
* Input: translatedMsg - The translated message
*        pDia          - The pointer to the object whose state will be modified
*        pMsg          - The pointer to the GOL message
*        
* Output: none
*
* Example:
*	See RdiaTranslateMsg() example.
*
* Side Effects: none
*
********************************************************************/
void        RdiaMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Macros:  RdiaGetVal(pDia)
*
* Overview: Returns the current dial value. Value is always
*           in the 0-max range inclusive.
*
* PreCondition: none
*
* Input: pDia - Pointer to the object.
*
* Output: Returns the current value of the dial.
*
* Example:
*   <CODE> 
*	WORD currVal;
*	ROUNDDIAL *pDia;
*
*		// assuming pDia is initialized to an existing dial Object
*		currVal = RdiaGetVal(pDia);
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
    #define RdiaGetVal(pDia)    (pDia)->value

/*********************************************************************
* Macros:  RdiaSetVal(pDia, newVal)	
*
* Overview: Sets the value to the given new value. Value set must be in 0-max
*			range inclusive.
*
* PreCondition: none
*
* Input: pDia - Pointer to the object. 
*		 newVal - New dial value.
*
* Output: none
*
* Example:
*   <CODE> 
*	WORD updatedVal;
*	ROUNDDIAL *pDia;
*
*		// assuming pDia is initialized to an existing dial Object
*		// assume GetInput() is a function that retrieves source data
*		updatedVal = GetInput();
*		RdiaSetVal(pDia, updatedVal);
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
    #define RdiaSetVal(pDia, newVal)    (pDia)->value = newVal

/*********************************************************************
* Macros:  RdiaIncVal(pDia)
*
* Overview: Used to directly increment the value. The delta
*           change used is the resolution setting (res).
*
* PreCondition: none
*
* Input: pDia - Pointer to the object.
*
* Output: none
*
* Example:
*   <CODE> 
*	WORD updatedVal, prevVal;
*	ROUNDDIAL *pDia;
*
*		// assuming pDia is initialized to an existing dial Object
*		// assume GetInput() is a function that retrieves source data
*		prevVal = RdiaGetVal(pDia);
*		updatedVal = GetInput();
*		if (updatedVal > prevVal)
*			RdiaIncVal(pDia);
*		if (updatedVal < prevVal)
*			RdiaDecVal(pDia);
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
    #define RdiaIncVal(pDia)    RdiaSetVal(pDia, (pDia->val + pDia->res))

/*********************************************************************
* Macros:  RdiaDecVal(pDia)
*
* Overview: Used to directly decrement the value. The delta
*           change used is the resolution setting (res).
*
* PreCondition: none
*
* Input: pDia - Pointer to the object.
*
* Output: none
*
* Example:
*   Refer to RdiaIncVal() example.
*
* Side Effects: none
*
********************************************************************/
    #define RdiaDecVal(pDia)    RdiaSetVal(pDia, (pDia->pos - pDia->res))

/*********************************************************************
* Function: WORD RdiaDraw(void *pObj)
*
* Overview: This function renders the object on the screen using 
* 			the current parameter settings. Location of the object is 
*			determined by the center (x,y) postion and the radius parameters. 
*			The colors used are dependent on the state of the object. 
*			
*			When rendering objects of the same type, each object 
*			must be rendered completely before the rendering of the 
*			next object is started. This is to avoid incomplete 
*			object rendering.
*
* PreCondition: Object must be created before this function is called.
*
* Input: pDia - Pointer to the object 
*
* Output: Returns the status of the drawing
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*
* Side Effects: none
*
********************************************************************/
WORD RdiaDraw(void *pObj);
#endif // _ROUNDDIAL_H
