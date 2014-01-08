/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Analog Clock
 *****************************************************************************
 * FileName:        AnalogClock.h
 * Processor:       PIC24, dsPIC, PIC32
 * Compiler:       	MPLAB C30, C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright (c) 2012 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
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
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *****************************************************************************/
#ifndef _ANALOGCLOCK_H
    #define _ANALOGCLOCK_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"
/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define AC_DISABLED    0x0002   // Bit for disabled state.
    #define AC_PRESSED     0x0004   // Bit for press state.
    #define AC_TICK	       0x1000   // Bit to tick second hand
    #define UPDATE_HOUR    0x2000   // Bit to indicate hour hand must be redrawn
    #define UPDATE_MINUTE  0x0100   // Bit to indicate minute hand must be redrawn
    #define UPDATE_SECOND  0x0200   // Bit to indicate minute hand must be redrawn
    #define AC_DRAW        0x4000   // Bit to indicate button must be redrawn.
    #define AC_HIDE        0x8000   // Bit to indicate button must be removed from screen.
    #define AC_REMOVE      0x8000

/*********************************************************************
* Overview: Defines the parameters required for a clock Object.
* 			The following relationships of the parameters determines
*			the general shape of the clock:
* 			1. centerx and centery determine the middle of the clock.
*			2. radius defines the radius of the clock.
*			4. *pBitmap points to the background image for the analog clock.
*
*********************************************************************/
typedef struct
{
    OBJ_HEADER    hdr;              // Generic header for all Objects (see OBJ_HEADER).
    SHORT         radius;           // Radius of the clock.
    SHORT		  centerx;          // center location in X-axis.
    SHORT		  centery;          // center location in Y-axis.
    SHORT		  valueS;           // time in Second
    SHORT		  prev_valueS;      // previous time in Second
    SHORT		  valueM;           // time in Minute
    SHORT		  prev_valueM;      // previous time in Minute
    SHORT		  valueH;           // time in Hour
    SHORT		  prev_valueH;      // previous time in Hour
    void          *pBitmap;         // Pointer to bitmap used.
} ANALOGCLOCK;



/*********************************************************************
* Function: ANALOGCLOCK *AcCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                             SHORT bottom, SHORT radius, void *pBitmap, XCHAR *pText, 
*                             GOL_SCHEME *pScheme)
*
* Overview: This function creates an Analog Clock object with the parameters given. 
*			It automatically attaches the new object into a global linked list of 
*			objects and returns the address of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the object.
*        top - Top most position of the object. 
*        right - Right most position of the object.
*        bottom - Bottom most position of the object.
*        hour - Initial hour value.
*        minute - Initial minute value. 
*        radius - Sets the radius of the clock.
*        sechand - Flag to indicate if the second hand will be drawn or not.
*        state - Sets the initial state of the object.
*        pBitmap - Pointer to the bitmap used on the face of the analog clock
*                  dimension of the image must match the dimension of the 
*				   widget.
*        pScheme - Pointer to the style scheme used.
*
* Output: Returns the pointer to the object created.
*
* Example:
*   <CODE> 
*	GOL_SCHEME *pScheme;
*	WORD state;
*	
*		pScheme = GOLCreateScheme();
*		state = AC_DRAW;
*
*		AnalogClock = AcCreate(ANALOGCLOCK_ID, 20, 64, 50, 118, 1, 20, 30, FALSE, state, NULL, pScheme);
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
ANALOGCLOCK *AcCreate
(
    WORD        ID,
    SHORT       left,
    SHORT       top,
    SHORT       right,
    SHORT       bottom,
    SHORT       hour,
    SHORT       minute,
    SHORT       radius,
    BOOL	    sechand,
    WORD        state,
    void        *pBitmap,
    GOL_SCHEME  *pScheme
);

/*********************************************************************
* Function: WORD AcDraw(ANALOGCLOCK *pAc)
*
* Overview: This function renders the object on the screen using 
* 			the current parameter settings. Location of the object is 
*			determined by the left, top, right and bottom parameters. 
*			The colors used are dependent on the state of the object. 
*			The font used is determined by the style scheme set.
*			
*
* PreCondition: Object must be created before this function is called.
*
* Input: pAc - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Example:
*   <CODE> 
*	void MyGOLDraw(){
*		static OBJ_HEADER *pCurrentObj = NULL;
*		int done;
*		
*		// There are no objects
*		if(GOLGetList() == NULL)
*			return;
*
* 		// If it's last object jump to head
*		if(pCurrentObj == NULL)
*			pCurrentObj = GOLGetList();
*
*		done = 0;
*
*		// this only process Button and Window
*		while(pCurrentObj != NULL){
*			// check if object state indicates redrawing
*			if(pCurrentObj->state&0xFC00) {
*				switch(pCurrentObj->type){
*					case OBJ_ANALOGCLOCK:
*						done = AcDraw((ANALOGCLOCK*)pCurrentObj);
*						break;
*					case OBJ_WINDOW:
*						done = WndDraw((WINDOW*)pCurrentObj);
*						break;
*					default: 
*						done = 1;
*						break;
*				}
*				if(done){
*					// reset only the state if drawing was finished
*					pCurrentObj->state = 0;
*				}else{
*					// done processing the list
*					return;
*				}
*			}
*			// go to next object
*			pCurrentObj = pCurrentObj->pNxtObj;
*		}
*	}
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
WORD    AcDraw(void *pAc);

/*********************************************************************
* Function: AcHandsDraw(ANALOGCLOCK *pAc, SHORT hand, SHORT thickness, WORD color, void *pBitmap);
*
* Overview: Draws the current position of the clock hands with the given thickness and color.
*
* PreCondition: none
*
* Input: pAc - The pointer to the object whose hands will be modified.
*        hand - which hand to be drawn (second, minute, hour)
*        thickness - thickness to draw the hand
*        color - color to draw the hand
*        *pBitmap - bitmap background to be redrawn
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
WORD AcHandsDraw(ANALOGCLOCK *pAc, SHORT hand, SHORT thickness, WORD color, void *pBitmap);


/*********************************************************************
* Function: AcSetHour(ANALOGCLOCK *pAc, SHORT hour)
*
* Overview: Sets the hour value of the analog clock.
*
* PreCondition: none
*
* Input: pAc - The pointer to the object whose hands will be modified.
*        hour - New hour time. 
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void AcSetHour(ANALOGCLOCK *pAc, SHORT hour);

/*********************************************************************
* Function: AcSetMinute(ANALOGCLOCK *pAc, SHORT minute)
*
* Overview: Sets the minute value of the analog clock.
*
* PreCondition: none
*
* Input: pAc - The pointer to the object whose hands will be modified.
*        minute - New minute time. 
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void AcSetMinute(ANALOGCLOCK *pAc, SHORT minute);

/*********************************************************************
* Function: AcSetSecond(ANALOGCLCOK *pAc, SHORT second)
*
* Overview: Sets the second value of the analog clock.
*
* PreCondition: none
*
* Input: pAc -    The pointer to the object whose hands will be modified.
*        second - New second time. 
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void AcSetSecond(ANALOGCLOCK *pAc, SHORT second);

#endif // _ANALOGCLOCK_H
