/*****************************************************************************
 *  Module for Microchip Graphics Library 
 *  GOL Layer 
 *  Picture
 *****************************************************************************
 * FileName:        Picture.c
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
 * Date        	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07		Version 1.0 release
 * 01/19/11		Fixed bug when drawing picture from (0,0) position.
 *****************************************************************************/
#include "Graphics/Graphics.h"

#ifdef USE_PICTURE

/*********************************************************************
* Function: PICTURE  *PictCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, WORD state, char scale, void *pBitmap, 
*                              GOL_SCHEME *pScheme)
*
* Overview: creates the picture control
*
********************************************************************/
PICTURE *PictCreate
(
    WORD        ID,
    SHORT       left,
    SHORT       top,
    SHORT       right,
    SHORT       bottom,
    WORD        state,
    char        scale,
    void        *pBitmap,
    GOL_SCHEME  *pScheme
)
{
    PICTURE *pPict = NULL;

    pPict = (PICTURE *)GFX_malloc(sizeof(PICTURE));
    if(pPict == NULL)
        return (pPict);

    pPict->hdr.ID = ID;
    pPict->hdr.pNxtObj = NULL;
    pPict->hdr.type = OBJ_PICTURE;
    pPict->hdr.left = left;
    pPict->hdr.top = top;
    pPict->hdr.right = right;
    pPict->hdr.bottom = bottom;
    pPict->pBitmap = pBitmap;
    pPict->hdr.state = state;
    pPict->scale = scale;
    pPict->hdr.DrawObj = PictDraw;			// draw function
    pPict->hdr.MsgObj = PictTranslateMsg;   // message function
    pPict->hdr.MsgDefaultObj = NULL;		// default message function
    pPict->hdr.FreeObj = NULL;				// free function

    pPict->partial.width=0;                //This voids the partial image

    // Set the style scheme to be used
    if(pScheme == NULL)
        pPict->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pPict->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    GOLAddObject((OBJ_HEADER *)pPict);

    return (pPict);
}

/*********************************************************************
* Function: WORD PictTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: translates the GOL message for the picture control
*
********************************************************************/
WORD PictTranslateMsg(void *pObj, GOL_MSG *pMsg)
{
    PICTURE *pPict;

    pPict = (PICTURE *)pObj;

    // Evaluate if the message is for the picture
    // Check if disabled first
    if(GetState(pPict, PICT_DISABLED))
        return (OBJ_MSG_INVALID);

        #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {

        // Check if it falls in the picture area
        if
        (
            (pPict->hdr.left < pMsg->param1) &&
            (pPict->hdr.right > pMsg->param1) &&
            (pPict->hdr.top < pMsg->param2) &&
            (pPict->hdr.bottom > pMsg->param2)
        )
        {
            return (PICT_MSG_SELECTED);
        }
    }

        #endif
    return (OBJ_MSG_INVALID);
}

/*********************************************************************
* Function: WORD PictDraw(void *pObj)
*
* Output: returns the status of the drawing
*		  0 - not completed
*         1 - done
*
* Overview: draws picture
*
********************************************************************/
WORD PictDraw(void *pObj)
{
    typedef enum
    {
        REMOVE,
        DRAW_IMAGE,
        DRAW_BACKGROUND1,
        DRAW_BACKGROUND2,
        DRAW_BACKGROUND3,
        DRAW_BACKGROUND4,
        DRAW_FRAME
    } PICT_DRAW_STATES;

    static PICT_DRAW_STATES state = REMOVE;
    static SHORT posleft;
    static SHORT postop;
    static SHORT posright;
    static SHORT posbottom;
    PICTURE *pPict;

    pPict = (PICTURE *)pObj;

    if(IsDeviceBusy())
        return (0);

    switch(state)
    {
        case REMOVE:
            if(GetState(pPict, PICT_HIDE))
            {
                SetColor(pPict->hdr.pGolScheme->CommonBkColor);
                if(!Bar(pPict->hdr.left, pPict->hdr.top, pPict->hdr.right, pPict->hdr.bottom))
                    return (0);
                return (1);
            }

            posleft = (pPict->hdr.left + 1 + pPict->hdr.right - pPict->scale * GetImageWidth(pPict->pBitmap)) >> 1;
            postop = (pPict->hdr.top + 1 + pPict->hdr.bottom - pPict->scale * GetImageHeight(pPict->pBitmap)) >> 1;
            posright = (pPict->hdr.right + pPict->hdr.left + pPict->scale * GetImageWidth(pPict->pBitmap)) >> 1;
            posbottom = (pPict->hdr.bottom + pPict->hdr.top + pPict->scale * GetImageHeight(pPict->pBitmap)) >> 1;
            state = DRAW_IMAGE;

        case DRAW_IMAGE:
            if(pPict->pBitmap != NULL)
            {
                if(IsDeviceBusy())
                    return (0);
                if(!PutImagePartial(posleft, postop, pPict->pBitmap, pPict->scale, pPict->partial.xoffset,pPict->partial.yoffset,pPict->partial.width,pPict->partial.height))
                    return (0);
            }

            SetColor(pPict->hdr.pGolScheme->CommonBkColor);
            state = DRAW_BACKGROUND1;

        case DRAW_BACKGROUND1:
            if(!Bar(pPict->hdr.left + 1, pPict->hdr.top + 1, pPict->hdr.right - 1, postop - 1))
                return (0);
            state = DRAW_BACKGROUND2;

        case DRAW_BACKGROUND2:
            if(!Bar(pPict->hdr.left + 1, posbottom, pPict->hdr.right - 1, pPict->hdr.bottom - 1))
                return (0);
            state = DRAW_BACKGROUND3;

        case DRAW_BACKGROUND3:
            if(!Bar(pPict->hdr.left + 1, postop, posleft - 1, posbottom))
                return (0);
            state = DRAW_BACKGROUND4;

        case DRAW_BACKGROUND4:
            if(!Bar(posright, postop, pPict->hdr.right - 1, posbottom))
                return (0);
            state = DRAW_FRAME;

        case DRAW_FRAME:
            if(GetState(pPict, PICT_FRAME))
            {
                SetLineType(SOLID_LINE);
                SetColor(pPict->hdr.pGolScheme->TextColor0);
                if(!Rectangle(pPict->hdr.left, pPict->hdr.top, pPict->hdr.right, pPict->hdr.bottom))
                    return (0);
            }

            state = REMOVE;
            return (1);
    }

    return (1);
}

#endif // USE_PICTURE
