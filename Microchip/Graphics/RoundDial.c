/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Round Dial
 *****************************************************************************
 * FileName:        RoundDial.c
 * Dependencies:    math.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 Version 3.00, MPLAB C32
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07	    Version 1.0 release
 * 12/03/09     Added Object Header for Double Buffering Support
 * 11/15/10		Fixed build error when USE_KEYBOARD is not defined
 * 04/20/11     Fixed KEYBOARD bug on object ID and GOL_MSG param1 comparison.
 * 08/04/11     Fixed drawing states for accelerated Circle and FillCircle functions.
 *****************************************************************************/
#include "Graphics/Graphics.h"
#include <math.h>

#ifdef USE_ROUNDDIAL

/*********************************************************************
* Function: ROUNDDIAL  *RdiaCreate(	WORD ID, SHORT x, SHORT y, SHORT radius, 
*						WORD state, SHORT res, SHORT value, SHORT max, 
*						GOL_SCHEME *pScheme)
*
*
* Notes: Creates a ROUNDDIAL object and adds it to the current active list.
*        If the creation is successful, the pointer to the created Object 
*        is returned. If not successful, NULL is returned.
*
********************************************************************/
ROUNDDIAL *RdiaCreate
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
)
{
    ROUNDDIAL   *pDia = NULL;

    pDia = (ROUNDDIAL *)GFX_malloc(sizeof(ROUNDDIAL));
    if(pDia == NULL)
        return (NULL);

    pDia->hdr.ID = ID;              // unique id assigned for referencing
    pDia->hdr.pNxtObj = NULL;       // initialize pointer to NULL
    pDia->hdr.type = OBJ_ROUNDDIAL; // set object type
    pDia->xCenter = x;				// x coordinate of center
    pDia->yCenter = y;				// y coordinate of center
    pDia->radius = radius;			// radius of dial
    pDia->res = res;
    pDia->value = value;
    pDia->max = max;
    pDia->hdr.state = state;        // state
    #ifdef USE_KEYBOARD
    pDia->vAngle = 0;				// initial position
    #endif
    pDia->curr_xPos = x + radius * 2 / 3;
    pDia->curr_yPos = y;
    pDia->hdr.DrawObj = RdiaDraw;				// draw function
    pDia->hdr.MsgObj = RdiaTranslateMsg;		// message function
    pDia->hdr.MsgDefaultObj = RdiaMsgDefault;	// default message function
    pDia->hdr.FreeObj = NULL;					// free function

    pDia->hdr.left = x - radius;    // left position
    pDia->hdr.top = y - radius;     // top position
    pDia->hdr.right = x + radius;   // right position
    pDia->hdr.bottom = y + radius;  // bottom position

    // Set the color scheme to be used
    if(pScheme == NULL)
        pDia->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pDia->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    GOLAddObject((OBJ_HEADER *)pDia);

    return (pDia);
}

/*********************************************************************
* Function: RdiaMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
*
* Notes: This the default operation to change the state of the dial.
*		 Called inside GOLMsg() when GOLMsgCallback() returns a 1.
*
********************************************************************/
void RdiaMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
{
    ROUNDDIAL *pDia;

    pDia = (ROUNDDIAL *)pObj;

    switch(translatedMsg)
    {
        case RD_MSG_CLOCKWISE:      SetState(pDia, RDIA_ROT_CW | RDIA_DRAW);    // set rotate left and redraw
            break;
        case RD_MSG_CTR_CLOCKWISE:  SetState(pDia, RDIA_ROT_CCW | RDIA_DRAW);   // set rotate right and redraw
            break;
    }
}

/*********************************************************************
* Function: SHORT RdiaCosine( SHORT v )
*
*
* Notes: Returns the cosine of the dial position.
*
********************************************************************/
    #ifdef USE_KEYBOARD

// Dimple position table for 15 degree increments
SHORT   _cosine[RDIA_QUADRANT_POSITIONS] = { 100, 97, 87, 71, 50, 26};

/* */
SHORT RdiaCosine(SHORT v)
{
    if(v >= RDIA_QUADRANT_POSITIONS * 3)
    {
        v -= RDIA_QUADRANT_POSITIONS * 3;
        return (_cosine[RDIA_QUADRANT_POSITIONS - 1 - v]);
    }
    else if(v >= RDIA_QUADRANT_POSITIONS * 2)
    {
        v -= RDIA_QUADRANT_POSITIONS * 2;
        return (-(_cosine[v]));
    }
    else if(v >= RDIA_QUADRANT_POSITIONS)
    {
        v -= RDIA_QUADRANT_POSITIONS;
        return (-(_cosine[RDIA_QUADRANT_POSITIONS - 1 - v]));
    }
    else
    {
        return (_cosine[v]);
    }
}

/*********************************************************************
* Function: SHORT RdiaSine( SHORT v )
*
*
* Notes: Returns the sine of the dial position.
*
********************************************************************/
SHORT RdiaSine(SHORT v)
{
    if(v >= RDIA_QUADRANT_POSITIONS * 3)
    {
        v -= RDIA_QUADRANT_POSITIONS * 3;
        return (-(_cosine[v]));
    }
    else if(v >= RDIA_QUADRANT_POSITIONS * 2)
    {
        v -= RDIA_QUADRANT_POSITIONS * 2;
        return (-(_cosine[RDIA_QUADRANT_POSITIONS - 1 - v]));
    }
    else if(v >= RDIA_QUADRANT_POSITIONS)
    {
        v -= RDIA_QUADRANT_POSITIONS;
        return (_cosine[v]);
    }
    else
    {
        return (_cosine[RDIA_QUADRANT_POSITIONS - 1 - v]);
    }
}
    #endif

/*********************************************************************
* Function: WORD RdiaTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
*
* Notes: Evaluates the message if the object will be affected by the 
*		 message or not.
*
********************************************************************/
WORD RdiaTranslateMsg(void *pObj, GOL_MSG *pMsg)
{
    ROUNDDIAL *pDia;
    WORD      messageID = OBJ_MSG_INVALID;

    pDia = (ROUNDDIAL *)pObj;

        #ifdef USE_TOUCHSCREEN

    SHORT           touchRadius, touchX, touchY;
    static SHORT    prevX = -1, prevY = -1;

    // Evaluate if the message is for the button
    // Check if disabled first
    if(GetState(pDia, RDIA_DISABLED))
        return (OBJ_MSG_INVALID);

    if((pMsg->type == TYPE_TOUCHSCREEN) && (pMsg->uiEvent == EVENT_MOVE))
    {

        // Check if it falls in the dial's face
        // to check this the x,y position must be within the circle
        // (x - xCenter)^2 + (y - yCenter)^2 = r^2  where x and y are the points
        // to test, the distance between x,y position of touch and center must be
        // greater than or equal to the radius of the dial
        if
        (
            ((pDia->xCenter - pDia->radius) < pMsg->param1) &&
            ((pDia->xCenter + pDia->radius) > pMsg->param1) &&
            ((pDia->yCenter - pDia->radius) < pMsg->param2) &&
            ((pDia->yCenter + pDia->radius) > pMsg->param2)
        )
        {

            // first get the radius of the touch point
            touchX = pMsg->param1 - pDia->xCenter;
            touchY = pMsg->param2 - pDia->yCenter;
            touchRadius = sqrt(touchX * touchX + touchY * touchY);

            if(touchRadius <= pDia->radius)
            {

                // difference of 3 is used to remove jitter caused by noise or sensitivity of the touchscreen
                if((abs(prevX - pMsg->param1) > 3) || (abs(prevY - pMsg->param2) > 3))
                {

                    // The first MOVE event is used to record the current position only. The second MOVE event
                    // will be used together with the previous MOVE and determine if the movement is in the
                    // clockwise or counter clockwise direction.
                    if((prevX == -1) || (prevY == -1))
                    {
                        prevX = pMsg->param1;
                        prevY = pMsg->param2;
                        messageID = OBJ_MSG_INVALID;
                    }
                    else
                    {

                        // this makes the sampling area a ring where the max radius is the dial radius
                        // and min radius is 5
                        if(touchRadius > 5)
                        {
                            pDia->new_xPos = (pDia->radius * 2 / 3) * (pMsg->param1 - pDia->xCenter) / touchRadius;
                            pDia->new_yPos = (pDia->radius * 2 / 3) * (pMsg->param2 - pDia->yCenter) / touchRadius;

                            // check if moving in clockwise direction or counter clockwise direction
                            if((pDia->xCenter >= pMsg->param1) && (pDia->yCenter > pMsg->param2))
                            {
                                if((prevX < pMsg->param1) && (prevY >= pMsg->param2))
                                    messageID = RD_MSG_CLOCKWISE;
                                else if((prevX >= pMsg->param1) && (prevY < pMsg->param2))
                                    messageID = RD_MSG_CTR_CLOCKWISE;
                            }

                            if((pDia->xCenter < pMsg->param1) && (pDia->yCenter > pMsg->param2))
                            {
                                if((prevX < pMsg->param1) && (prevY <= pMsg->param2))
                                    messageID = RD_MSG_CLOCKWISE;
                                else if((prevX >= pMsg->param1) && (prevY > pMsg->param2))
                                    messageID = RD_MSG_CTR_CLOCKWISE;
                            }

                            if((pDia->xCenter < pMsg->param1) && (pDia->yCenter <= pMsg->param2))
                            {
                                if((prevX > pMsg->param1) && (prevY <= pMsg->param2))
                                    messageID = RD_MSG_CLOCKWISE;
                                else if((prevX <= pMsg->param1) && (prevY > pMsg->param2))
                                    messageID = RD_MSG_CTR_CLOCKWISE;
                            }

                            if((pDia->xCenter >= pMsg->param1) && (pDia->yCenter <= pMsg->param2))
                            {
                                if((prevX > pMsg->param1) && (prevY >= pMsg->param2))
                                    messageID = RD_MSG_CLOCKWISE;
                                else if((prevX <= pMsg->param1) && (prevY < pMsg->param2))
                                    messageID = RD_MSG_CTR_CLOCKWISE;
                            }
                        }
                        else
                            messageID = OBJ_MSG_INVALID;

                        prevX = pMsg->param1;
                        prevY = pMsg->param2;
                    }
                }
                else
                    messageID = OBJ_MSG_INVALID;

                // determine the movement clockwise or counter clockwise
                // this is important to update the value variable
                return (messageID);
            }
            else
            {
                prevX = -1;
                prevY = -1;
            }
        }
    }

        #endif
       #ifdef USE_KEYBOARD

    SHORT   newValue;

    // Evaluate if the message is for the Dial
    // Check if disabled first
    if(GetState(pDia, RDIA_DISABLED))
        return (OBJ_MSG_INVALID);

    if((pMsg->type == TYPE_KEYBOARD) && ((WORD)pMsg->param1 == pDia->hdr.ID) && (pMsg->uiEvent == EVENT_KEYSCAN))
    {
        if(pMsg->param2 == SCAN_RIGHT_PRESSED)
        {
            newValue = pDia->value + pDia->res;
            if(newValue > pDia->max)
            {
                newValue -= (pDia->max + 1);
            }

            pDia->vAngle += 1;
            messageID = RD_MSG_CLOCKWISE;
        }

        if(pMsg->param2 == SCAN_LEFT_PRESSED)
        {
            newValue = pDia->value - pDia->res;
            if(newValue < 0)
            {
                newValue += (pDia->max + 1);
            }

            pDia->vAngle -= 1;
            messageID = RD_MSG_CTR_CLOCKWISE;
        }
        
        if (pDia->vAngle > (RDIA_MAX_POSITIONS - 1))
           	pDia->vAngle = 0;
        else if (pDia->vAngle < 0)
           	pDia->vAngle = (RDIA_MAX_POSITIONS - 1);
           	
        pDia->new_xPos = pDia->radius * 2 * RdiaCosine(pDia->vAngle) / 100 / 3;
        pDia->new_yPos = pDia->radius * 2 * RdiaSine(pDia->vAngle) / 100 / 3;
    }

        #endif
    return (messageID);
}

/*********************************************************************
* Function: WORD RdiaDraw(void *pObj)
*
*
* Notes: This is the state machine to draw the dial.
*
********************************************************************/
WORD RdiaDraw(void *pObj)
{
    typedef enum
    {
        REMOVE,
        RND_PANEL_DRAW,
        RND_PANEL_TASK,
        ERASE_POSITION,
        DRAW_POSITION,
        DRAW_POSITION_1,
        DRAW_POSITION_2
    } RDIA_DRAW_STATES;

    static RDIA_DRAW_STATES state = REMOVE;
    static SHORT dimpleRadius;
    GFX_COLOR faceClr;
    ROUNDDIAL *pDia;

    pDia = (ROUNDDIAL *)pObj;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);
    
        switch(state)
        {
            case REMOVE:

#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_SetupDrawUpdate( pDia->hdr.left,
                                            pDia->hdr.top,
                                            pDia->hdr.right,
                                            pDia->hdr.bottom);
#endif
                if(GetState(pDia, RDIA_HIDE))
                {   // Hide the dial (remove from screen)
                    SetColor(pDia->hdr.pGolScheme->CommonBkColor);
                    if
                    (
                        !Bar
                            (
                                pDia->xCenter - pDia->radius,
                                pDia->yCenter - pDia->radius,
                                pDia->xCenter + pDia->radius,
                                pDia->yCenter + pDia->radius
                            )
                    ) 
                    return (0);
                    
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                    GFX_DRIVER_CompleteDrawUpdate(   pDia->hdr.left,
                                                    pDia->hdr.top,
                                                    pDia->hdr.right,
                                                    pDia->hdr.bottom);
#endif
                    return (1);
                }
    
                dimpleRadius = (pDia->radius >> 3) + 1;
    
                if(GetState(pDia, RDIA_ROT_CCW | RDIA_ROT_CW))
                {
                    SetColor(pDia->hdr.pGolScheme->Color0);
                    state = ERASE_POSITION;
                    break;
                }
    
                state = RND_PANEL_DRAW;
    
            case RND_PANEL_DRAW:
                if(!GetState(pDia, RDIA_DISABLED))
                {
                    faceClr = pDia->hdr.pGolScheme->Color0;
                }
                else
                {
                    faceClr = pDia->hdr.pGolScheme->ColorDisabled;
                }
    
                SetLineThickness(NORMAL_LINE);
                SetLineType(SOLID_LINE);
                GOLPanelDraw
                (
                    pDia->xCenter,
                    pDia->yCenter,
                    pDia->xCenter,
                    pDia->yCenter,
                    pDia->radius,
                    faceClr,
                    pDia->hdr.pGolScheme->EmbossLtColor,
                    pDia->hdr.pGolScheme->EmbossDkColor,
                    NULL,
                    GOL_EMBOSS_SIZE
                );
                state = RND_PANEL_TASK;
    
            case RND_PANEL_TASK:
                if(!GOLPanelDrawTsk())
                {
                    return (0);
                }
    
                state = DRAW_POSITION;
                break;
    
            case ERASE_POSITION:
                if 
                (
                    !Bar
                        (
                            pDia->curr_xPos - dimpleRadius,
                            pDia->curr_yPos - dimpleRadius,
                            pDia->curr_xPos + dimpleRadius,
                            pDia->curr_yPos + dimpleRadius
                        )
                ) return (0);
    
                // determine if the value will increment or decrement
                if(GetState(pDia, RDIA_ROT_CW))
                {
                    pDia->value = pDia->value + pDia->res;
                    if(pDia->value > pDia->max)
                    {
                        pDia->value -= (pDia->max + 1);
                    }
                }
                else if(GetState(pDia, RDIA_ROT_CCW))
                {
                    pDia->value = pDia->value - pDia->res;
                    if(pDia->value < 0)
                    {
                        pDia->value += (pDia->max + 1);
                    }
                }
    
                // else do not update counter yet
                // locate the new position of the dimple	
                pDia->curr_xPos = pDia->xCenter + pDia->new_xPos;
                pDia->curr_yPos = pDia->yCenter + pDia->new_yPos;
    
                ClrState(pDia, RDIA_ROT_CW | RDIA_ROT_CCW); // make sure this is cleared to avoid
    
                // unwanted redraw
                state = DRAW_POSITION;
    
            case DRAW_POSITION:

                SetColor(pDia->hdr.pGolScheme->EmbossLtColor);
                SetLineThickness(NORMAL_LINE);
                SetLineType(SOLID_LINE);
                state = DRAW_POSITION_1;

            case DRAW_POSITION_1:
                if(!Circle(pDia->curr_xPos, pDia->curr_yPos, dimpleRadius))
                    return (0);
                SetColor(pDia->hdr.pGolScheme->EmbossDkColor);
                state = DRAW_POSITION_2;

            case DRAW_POSITION_2:
                if(!FillCircle(pDia->curr_xPos, pDia->curr_yPos, dimpleRadius - 1))
                    return (0);
    
                state = REMOVE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pDia->hdr.left,
                                                pDia->hdr.top,
                                                pDia->hdr.right,
                                                pDia->hdr.bottom);
#endif
                return (1);

        } // end of switch()
    } // end of while(1)
}

#endif // USE_ROUNDDIAL
