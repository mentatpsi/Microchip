/*****************************************************************************
 * 
 * Module for Microchip Graphics Library
 * GOL Layer 
 * This template can be used to create additional controls
 * 
 *****************************************************************************
 * FileName:        Template.h
 * Dependencies:    None 
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00
 * Linker:          MPLAB LINK30
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
 * 06/15/07    Beta release 
 *****************************************************************************/
#ifndef _CUSTOM_H
    #define _CUSTOM_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

// Control states:
    #define CC_FOCUSED  0x0001  // bit  to indicate the control is focused
    #define CC_DISABLED 0x0002  // bit  to indicate the control is disabled
    #define CC_HIDE     0x8000  // bit  to indicate the control must be removed from screen
    #define CC_DRAW     0x4000  // bits to indicate the whole control must be redrawn

// The structure contains data for the control
typedef struct
{
    OBJ_HEADER  hdr;
} CUSTOM;

/*********************************************************************
* Function: CUSTOM  *CcCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, WORD state, GOL_SCHEME *pScheme)
*
* PreCondition: none
*
* Input: ID - user defined ID for the object
*        left, top, right, bottom - location of the left,top and 
*             right, bottom corners of the object
*        state - state of object
*        pScheme - pointer to the style scheme
*
* Output: returns the pointer to the object created
*
* Side Effects: none
*
* Overview: creates the object and initialize with the passed parameters and 
*           default settings
*
* Note: none
*
********************************************************************/
CUSTOM  *CcCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, GOL_SCHEME *pScheme);

/*********************************************************************
* Function: WORD CcTranslateMsg(CUSTOM *pCc, GOL_MSG *pMsg)
*
* PreCondition: none
*
* Input: pMsg - pointer to the message struct containing the message from 
*               the user interface
*        pCc - the pointer to the object where the message will be tested
*               to check if the message will affect the object
*
* Output: returns the action that the object will be performing
*
* Overview: evaluates the message if the object will be affected by the 
*           message or not
*
* Note: THIS FUNCTION CALL SHOULD BE ADDED INTO GOLMsg() FUNCTION IN
*       GOL.C FILE
*
********************************************************************/
WORD    CcTranslateMsg(CUSTOM *pCc, GOL_MSG *pMsg);

/*********************************************************************
* Function: void CcMsgDefault(CUSTOM* pCc, GOL_MSG* pMsg)
*
* PreCondition: None
*
* Input: pMsg -  pointer to the message struct containing the message the user
*        pCc - the pointer to the object whose state will be modified 
*
* Output: none
*
* Side Effects: 
*
* Overview: changes the state of the object
*
* Note:  THIS FUNCTION CALL SHOULD BE ADDED INTO GOLMsg() FUNCTION IN
*        GOL.C FILE 
*
********************************************************************/
void    CcMsgDefault(CUSTOM *pCc, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD CcDraw(CUSTOM *pCc)
*
* PreCondition: object must be created before this is called
*
* Input: pCc - pointer to struct CUSTOM with data
*
* Output: returns the status of the drawing
*		  0 - not complete
*         1 - done
*
* Overview: draws control
*
* Note: THIS FUNCTION CALL SHOULD BE ADDED INTO GOLDraw() FUNCTION IN
*       GOL.C FILE
*
********************************************************************/
WORD CcDraw(void *pObj);
#endif // _CUSTOM_H
