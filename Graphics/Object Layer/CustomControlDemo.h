/*****************************************************************************
 * 
 *  Custom control.
 *  This file is added to Graphics.h header file.
 *  GOLDraw() and  GOLMsg() are modified to provide GOL drawing and messaging 
 *  for this control. See GOL.c file for details.
 *  To remove the code related to this control comment 
 *  #define USE_CUSTOM line in GraphicsConfig.h header file.
 * 
 *****************************************************************************
 * FileName:        CustomControl.h
 * Dependencies:    None 
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok     06/15/07    ... 
 *****************************************************************************/
#ifndef _CUSTOM_H
    #define _CUSTOM_H

// Control states
    #define CC_DISABLED 0x0002  // disabled
    #define CC_HIDE     0x8000  // must be removed from screen
    #define CC_DRAW_BAR 0x2000  // control bar should be redrawn
    #define CC_DRAW     0x4000  // whole control must be redrawn

// The structure contains data for the control
typedef struct
{
    WORD            ID;             // instance unique ID
    void            *pNxtObj;       // a pointer to the next object in the linked list
    GOL_OBJ_TYPE    type;           // must be set to OBJ_CUSTOM
    WORD            state;          // state
    SHORT           left;           // left border
    SHORT           top;            // top border
    SHORT           right;          // right border
    SHORT           bottom;         // bottom border
    GOL_SCHEME      *pGolScheme;    // the style scheme used
    DRAW_FUNC       draw;
    FREE_FUNC           FreeObj;                        // function pointer to the object free function
    MSG_FUNC            MsgObj;                         // function pointer to the object message function
    MSG_DEFAULT_FUNC    MsgDefaultObj;                  // function pointer to the object default message function
    WORD            pos;            // current position
    WORD            prevPos;        // previous position
} CUSTOM;

/*********************************************************************
* Macros:  CcGetPos(pCc)
*
* PreCondition: none
*
* Input: pCc - pointer to the object 
*
* Output: current position
*
* Side Effects: none
*
* Overview: returns position
*
* Note: none
*
********************************************************************/
    #define CcGetPos(pCc)   pCc->pos

/*********************************************************************
* Macros: CcSetPos(pCc,position)
*
* PreCondition: none
*
* Input: pCc - the pointer to the object
*        position - new position 
*
* Output: none
*
* Overview: sets the current position of the control
*
* Note: none
*
********************************************************************/
    #define CcSetPos(pCc, position) pCc->pos = position

/*********************************************************************
* Function: CUSTOM  *CcCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, WORD state, GOL_SCHEME *pScheme)
*
* PreCondition: none
*
* Input: ID - instance unique ID
*        left, top, right, bottom - dimensions of the object
*        state - state of the object
*        pScheme - pointer to the style scheme
*
* Output: returns the pointer to the object created
*
* Side Effects: none
*
* Overview: creates the custom control
*
* Note: none
*
********************************************************************/
CUSTOM * CcCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, GOL_SCHEME * pScheme);

/*********************************************************************
* Function: WORD CcTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* PreCondition: none
*
* Input: pMsg - pointer to the GOL message
*        pCc - the pointer to the custom control
*
* Output: translated message for the custom control
*
* Side Effects: none
*
* Overview: translates the GOL message for the custom control
*
* Note: none
*
********************************************************************/
WORD    CcTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: void CcMsgDefault(void *pObj, GOL_MSG* pMsg)
*
* PreCondition: none
*
* Input: pMsg -  pointer to the GOL message
*        pCc - the pointer to the custom control
*
* Output: none
*
* Side Effects: none
*
* Overview: changes the state of the object
*
* Note: none
*
********************************************************************/
void    CcMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD CcDraw(CUSTOM *pCc)
*
* PreCondition: none
*
* Input: pCc - pointer to the custom control
*
* Output: returns the status of the drawing
*		  0 - not completed
*         1 - done
*
* Side Effects: none
*
* Overview: draws custom control
*
* Note: none
*
********************************************************************/
WORD CcDraw(void *pObj);
#endif // _CUSTOM_H
