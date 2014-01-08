/*****************************************************************************
 *
 *  Custom control.
 *  GOLDraw() and  GOLMsg() are modified to provide GOL drawing and messaging 
 *  for this control. See GOL.c file for details.
 *  To remove the code related to this control comment 
 *  #define USE_CUSTOM line in GraphicsConfig.h header file.
 *
 *****************************************************************************
 * FileName:        CustomControl.c
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 06/18/07    Beta release
 *****************************************************************************/
#include "MainDemo.h"

#ifdef USE_CUSTOM

/*********************************************************************
* Function: CUSTOM* CcCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, WORD state, GOL_SCHEME *pScheme)
*
* Overview: creates the object
*
********************************************************************/
CUSTOM *CcCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, GOL_SCHEME *pScheme)
{
    CUSTOM  *pCc = NULL;

    pCc = (CUSTOM *)malloc(sizeof(CUSTOM));
    if(pCc == NULL)
        return (pCc);

    pCc->hdr.ID = ID;
    pCc->hdr.pNxtObj = NULL;
    pCc->hdr.type = OBJ_CUSTOM;
    pCc->hdr.left = left;
    pCc->hdr.top = top;
    pCc->hdr.right = right;
    pCc->hdr.bottom = bottom;
    pCc->pos = (pCc->hdr.top + pCc->hdr.bottom) >> 1;   // position
    pCc->prevPos = pCc->hdr.bottom - GOL_EMBOSS_SIZE;   // previos position
    pCc->hdr.state = state;                             // set state
    pCc->hdr.DrawObj = CcDraw;              			// draw function
    pCc->hdr.MsgObj = CcTranslateMsg;       			// message function
    pCc->hdr.MsgDefaultObj = CcMsgDefault;  			// default message function
    pCc->hdr.FreeObj = NULL;  							// default message function

    // Set the style scheme to be used
    if(pScheme == NULL)
        pCc->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pCc->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    GOLAddObject((OBJ_HEADER *)pCc);

    return (pCc);
}

/*********************************************************************
* Function: WORD CcTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: translates the GOL message to this control
*
********************************************************************/
WORD CcTranslateMsg(void *pObj, GOL_MSG *pMsg)
{
    CUSTOM *pCc;

    pCc = (CUSTOM *)pObj;

    // Check if disabled first
    if(GetState(pCc, CC_DISABLED))
        return (OBJ_MSG_INVALID);

        #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {

        // Check if it falls in the control area
        if
        (
            (pCc->hdr.left + GOL_EMBOSS_SIZE < pMsg->param1) &&
            (pCc->hdr.right - GOL_EMBOSS_SIZE > pMsg->param1) &&
            (pCc->hdr.top + GOL_EMBOSS_SIZE < pMsg->param2) &&
            (pCc->hdr.bottom - GOL_EMBOSS_SIZE > pMsg->param2)
        )
        {
            return (CC_MSG_SELECTED);
        }
    }

        #endif
    return (OBJ_MSG_INVALID);
}

/*********************************************************************
* Function: void CcMsgDefault(void* pObj, GOL_MSG* pMsg)
*
* Overview: changes the state of the object by default
*
********************************************************************/
void CcMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
{
    CUSTOM *pCc;

    pCc = (CUSTOM *)pObj;
	
    pCc->pos = pMsg->param2;
    SetState(pCc, CC_DRAW_BAR);
}

/*********************************************************************
* Function: WORD CcDraw(CUSTOM *pCc)
*
* Output: returns the status of the drawing
*		  0 - not complete
*         1 - done
*
* Overview: draws control
*
********************************************************************/
WORD CcDraw(void *pObj)
{
    typedef enum
    {
        REMOVE,
        BOX_DRAW,
        RUN_DRAW,
        BAR_DRAW,
        GRID_DRAW
    } CC_DRAW_STATES;

    static CC_DRAW_STATES state = REMOVE;
    static SHORT counter;
    static SHORT delta;
    CUSTOM *pCc;

	pCc = (CUSTOM*) pObj;

    switch(state)
    {
        case REMOVE:
            if(GetState(pCc, CC_HIDE))
            {
                SetColor(pCc->hdr.pGolScheme->CommonBkColor);
                if(!Bar(pCc->hdr.left, pCc->hdr.top, pCc->hdr.right, pCc->hdr.bottom))
                    return (0);
                return (1);
            }

            state = BOX_DRAW;

        case BOX_DRAW:
            if(GetState(pCc, CC_DRAW))
            {
                GOLPanelDraw
                (
                    pCc->hdr.left,
                    pCc->hdr.top,
                    pCc->hdr.right,
                    pCc->hdr.bottom,
                    0,
                    pCc->hdr.pGolScheme->Color0,
                    pCc->hdr.pGolScheme->EmbossDkColor,
                    pCc->hdr.pGolScheme->EmbossLtColor,
                    NULL,
                    GOL_EMBOSS_SIZE
                );

                state = RUN_DRAW;

            case RUN_DRAW:
                if(!GOLPanelDrawTsk())
                    return (0);
            }

            state = BAR_DRAW;

        case BAR_DRAW:
            if(pCc->prevPos > pCc->pos)
            {
                SetColor(pCc->hdr.pGolScheme->Color1);
                if(!Bar(pCc->hdr.left + GOL_EMBOSS_SIZE, pCc->pos, pCc->hdr.right - GOL_EMBOSS_SIZE, pCc->prevPos))
                    return (0);
            }
            else
            {
                SetColor(pCc->hdr.pGolScheme->Color0);
                if(!Bar(pCc->hdr.left + GOL_EMBOSS_SIZE, pCc->prevPos, pCc->hdr.right - GOL_EMBOSS_SIZE, pCc->pos))
                    return (0);
            }

            SetColor(pCc->hdr.pGolScheme->TextColor0);
            counter = 1;
            delta = (pCc->hdr.bottom - pCc->hdr.top - (2 * GOL_EMBOSS_SIZE)) >> 3;
            state = GRID_DRAW;

        case GRID_DRAW:
            while(counter < 8)
            {
                if
                (
                    !Bar
                        (
                            pCc->hdr.left + GOL_EMBOSS_SIZE,
                            pCc->hdr.top + GOL_EMBOSS_SIZE + counter * delta,
                            pCc->hdr.right - GOL_EMBOSS_SIZE,
                            pCc->hdr.top + GOL_EMBOSS_SIZE + counter * delta
                        )
                ) return (0);
                counter++;
            }

            pCc->prevPos = pCc->pos;
            state = REMOVE;
            return (1);
    }

    return (1);
}

#endif // USE_CUSTOM
