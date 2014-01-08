/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Picture control
 *****************************************************************************
 * FileName:        Picture.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
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
 * 03/09/11     Removed compile warnings
 *****************************************************************************/
#ifndef _PICTURE_H
    #define _PICTURE_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define PICT_DISABLED   0x0002  // Bit to indicate Picture is in a disabled state.
    #define PICT_FRAME      0x0004  // Bit to indicate Picture has a frame.
    #define PICT_HIDE       0x8000  // Bit to indicate Picture must be hidden.
    #define PICT_DRAW       0x4000  // Bit to indicate Picture will be redrawn.

/*****************************************************************************
 * Overview: The structure contains data for picture control
 *****************************************************************************/
typedef struct
{
    OBJ_HEADER     hdr;        // Generic header for all Objects (see OBJ_HEADER).
    char           scale;      // Scale factor for the bitmap
    void           *pBitmap;   // Pointer to the bitmap
    PUTIMAGE_PARAM partial;     //structure containing parital image data 
} PICTURE;

/*********************************************************************
* Macros:  PictGetBitmap(pPict)
*
* Overview: This macro returns the pointer to the bitmap used in the object. 
*
* PreCondition: none
*
* Input: pPict - Pointer to the object 
*
* Output: Returns the pointer to the bitmap used.
*
* Side Effects: none
*
********************************************************************/
    #define PictGetBitmap(pPict)    		((PICTURE*)pPict)->pBitmap

/*********************************************************************
* Macros:  PictSetBitmap(pPict,pBtMap)
*
* Overview: This macro sets the bitmap used in the object. 
*
* PreCondition: none
*
* Input: pPict - Pointer to the object 
*        pBtMap - Pointer to the bitmap to be used
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define PictSetBitmap(pPict, pBtmap)    ((PICTURE*)pPict)->pBitmap = pBtmap


/*********************************************************************
* Macros:  PictSetScale(pPict,scl)
*
* Overview: This macro sets the scale factor used to render the 
*			bitmap used in the object. 
*
* PreCondition: none
*
* Input: pPict - Pointer to the object 
*        scl   - The scale factor that will be used to display the
*				 bitmap.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define PictSetScale(pPict, scl)    pPict->scale = scl

/*********************************************************************
* Macros:  PictSetPartial(xoffset,yoffset,width,height)
*
* Overview: This macro sets the partial image used to render the 
*			bitmap used in the object. 
*
* PreCondition: none
*
* Input: xoffset,yoffset - x and y offset of the bitmap 
*        width,height    - width and height of the parital image
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define PictSetPartial(xoffset, yoffset, width, height)    \
                                         pPict->partial.xoffset = xoffset;\
                                         pPict->partial.yoffset = yoffset;\
                                         pPict->partial.width = width;\
                                         pPict->partial.height = height

/*********************************************************************
* Macros:  PictGetScale(pPict,scl)
*
* Overview: This macro returns the current scale factor used to
*			render the bitmap. 
*
* PreCondition: none
*
* Input: pPict - Pointer to the object 
*
* Output: Returns the current scale factor used to display the
*		  bitmap.
*
* Side Effects: none
*
********************************************************************/
    #define PictGetScale(pPict) pPict->scale

/*********************************************************************
* Function: PICTURE  *PictCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                                SHORT bottom, WORD state, char scale, 
*								 void *pBitmap, GOL_SCHEME *pScheme)
*
* Overview: This function creates a PICTURE object with the parameters given. 
*			It automatically attaches the new object into a global linked list of 
*			objects and returns the address of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the Object.
* 		 top - Top most position of the Object. 
*		 right - Right most position of the Object.
*		 bottom - Bottom most position of the object.
*		 radius - Radius of the rounded edge.
*        state - Sets the initial state of the object.
*        scale - Sets the scale factor used to render the bitmap.
*        pBitmap - Pointer to the bitmap that will be used.
*        pScheme - Pointer to the style scheme
*
* Output: Returns the pointer to the object created
*
* Side Effects: none
*
********************************************************************/
PICTURE * PictCreate
    (
        WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, char scale, void *pBitmap, GOL_SCHEME *
        pScheme
    );

/*********************************************************************
* Function: WORD PictTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. The table below 
*			enumerates the translated messages for each event 
*			accepted by the PICTURE Object.
*
*	<TABLE>
*    	Translated Message   Input Source  Events         							Description
*     	##################   ############  ######         							###########
*		PICT_MSG_SELECTED    Touch Screen  EVENT_PRESS, EVENT_RELEASE, EVENT_MOVE 	If events occurs and the x,y position falls in the area of the picture.
*		OBJ_MSG_INVALID		 Any		   Any										If the message did not affect the object.
*	</TABLE>
*
* PreCondition: none
*
* Input: pPict - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		  - PICT_MSG_SELECTED – Picture is touched.
*         - OBJ_MSG_INVALID – Picture is not affected
*
* Side Effects: none
*
********************************************************************/
WORD    PictTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD PictDraw(void *pObj)
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
* Input: pPict - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing 
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Side Effects: none
*
********************************************************************/
WORD PictDraw(void *pObj);
#endif // _PICTURE_H
