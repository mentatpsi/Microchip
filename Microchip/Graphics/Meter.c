/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Meter
 *****************************************************************************
 * FileName:        Meter.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 Version 3.00, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS" WITHOUT WARRANTY OF ANY
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
 * 07/31/08	    Added arc colors options
 * 08/20/08	    Added accuracy option for displaying values
 * 04/01/11     Removed sineTable[], use the one in the Primitive.c instead.
 * 08/05/11     - Modified compile time setting macro names. 
 *              - Relocated internal macros to the Meter.c.
 *              - Fixed rendering to check IsDeviceBusy() in between primitive
 *                calls in the draw routine.
 *****************************************************************************/
#include "Graphics/Graphics.h"
#include <math.h>
#include <stdio.h>

#ifdef USE_METER


/* Internal Used Constants */
    #define RADIAN      1144            // Radian definition. Equivalent to sine(1) * 2^16.
    #define PIIOVER2    102944          // The constant Pii divided by two (pii/2).

    #define ARC1_DEGREE 180             // defines one arc1 limit (used for determining colors)
    #define ARC2_DEGREE 135             // defines one arc2 limit (used for determining colors)
    #define ARC3_DEGREE 90              // defines one arc3 limit (used for determining colors)
    #define ARC4_DEGREE 45              // defines one arc4 limit (used for determining colors)
    #define ARC5_DEGREE 0               // defines one arc5 limit (used for determining colors)

/* These selects the other parameters of the meter that are dependent on the shape. */
    #if (METER_TYPE == MTR_WHOLE_TYPE)
        #define DEGREE_START    - 45    // Defines the start angle to draw the meter.
        #define DEGREE_END      225     // Defines the end angle to draw the meter.
    #elif (METER_TYPE == MTR_HALF_TYPE)
        #define DEGREE_START    0       // Defines the start angle to draw the meter.
        #define DEGREE_END      180     // Defines the end angle to draw the meter.
    #elif (METER_TYPE == MTR_QUARTER_TYPE)
        #define DEGREE_START    0       // Defines the start angle to draw the meter.
        #define DEGREE_END      90      // Defines the end angle to draw the meter.
    #endif


/* Internal Functions */
void    MtrCalcDimensions(METER *pMtr); // used to calculate the meter dimensions

// which is dependent on meter type

/*********************************************************************
* Function: METER  *MtrCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*							  SHORT bottom, WORD state, SHORT value, 
*							  SHORT minValue, SHORT maxValue, XCHAR *pText, 
*							  GOL_SCHEME *pScheme)				 
*
*
* Notes: Creates a METER object and adds it to the current active list.
*        If the creation is successful, the pointer to the created Object 
*        is returned. If not successful, NULL is returned.
*
********************************************************************/
METER *MtrCreate
(
    WORD        ID,
    SHORT       left,
    SHORT       top,
    SHORT       right,
    SHORT       bottom,
    WORD        state,
    SHORT       value,
    SHORT       minValue,
    SHORT       maxValue,
    void        *pTitleFont,
    void        *pValueFont,
    XCHAR       *pText,
    GOL_SCHEME  *pScheme
)
{
    METER   *pMtr = NULL;

    pMtr = (METER *)GFX_malloc(sizeof(METER));
    if(pMtr == NULL)
        return (NULL);

    pMtr->hdr.ID = ID;          // unique id assigned for referencing
    pMtr->hdr.pNxtObj = NULL;   // initialize pointer to NULL
    pMtr->hdr.type = OBJ_METER; // set object type
    pMtr->hdr.left = left;      // left,top coordinate
    pMtr->hdr.top = top;        //
    pMtr->hdr.right = right;    // right,bottom coordinate
    pMtr->hdr.bottom = bottom;  //
    pMtr->minValue = minValue;
    pMtr->maxValue = maxValue;
    pMtr->value = value;
    pMtr->hdr.state = state;    // state
    pMtr->pText = pText;
    pMtr->hdr.DrawObj = MtrDraw;				// draw function	
    pMtr->hdr.MsgObj = MtrTranslateMsg;         // message function
    pMtr->hdr.MsgDefaultObj = MtrMsgDefault;    // default message function
    pMtr->hdr.FreeObj = NULL;					// free function

    // set the default scale colors
#if (COLOR_DEPTH == 8) || (COLOR_DEPTH == 16) || (COLOR_DEPTH == 24)
    MtrSetScaleColors(pMtr, LIGHTGREEN, YELLOW, BRIGHTGREEN, BRIGHTBLUE, RED, BRIGHTRED);
#elif (COLOR_DEPTH == 4)
    MtrSetScaleColors(pMtr, GRAY000, GRAY003, GRAY006, GRAY009, GRAY012, GRAY015);
#elif (COLOR_DEPTH == 1)
    MtrSetScaleColors(pMtr, WHITE, BLACK, WHITE, BLACK, WHITE, BLACK);
#endif

    // Set the color scheme to be used
    if(pScheme == NULL)
        pMtr->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pMtr->hdr.pGolScheme = pScheme;

    // Set the Title Font to be used
    if(pTitleFont == NULL)
        pMtr->pTitleFont = (void *) &FONTDEFAULT;
    else
        pMtr->pTitleFont = pTitleFont;

    // Set the Value Font to be used
    if(pValueFont == NULL)
        pMtr->pValueFont = (void *) &FONTDEFAULT;
    else
        pMtr->pValueFont = pValueFont;

    // calculate dimensions of the meter	
    MtrCalcDimensions(pMtr);

    GOLAddObject((OBJ_HEADER *)pMtr);

    return (pMtr);
}

/*********************************************************************
* Function: MtrCalcDimensions(void)
*
* Notes: Calculates the dimension of the meter. Dependent on the 
*		 meter type set.
*
********************************************************************/
void MtrCalcDimensions(METER *pMtr)
{
    SHORT   tempHeight, tempWidth;
    SHORT   left, top, right, bottom;
    XCHAR   tempChar[2] = {'8',0};

    left = pMtr->hdr.left;
    right = pMtr->hdr.right;
    top = pMtr->hdr.top;
    bottom = pMtr->hdr.bottom;

    // get the text width reference. This is used to scale the meter
    if(pMtr->pText != NULL)
    {
        tempHeight = (GOL_EMBOSS_SIZE << 1) + GetTextHeight(pMtr->hdr.pGolScheme->pFont);
    }
    else
    {
        tempHeight = (GOL_EMBOSS_SIZE << 1);
    }

    tempWidth = (GOL_EMBOSS_SIZE << 1) + (GetTextWidth(tempChar, pMtr->hdr.pGolScheme->pFont) * MTR_BUILD_OPTION_SCALECHARCOUNT);

    // Meter size is dependent on the width or height.
    // The radius is also adjusted to add space for the scales
#if (METER_TYPE == MTR_WHOLE_TYPE)

    // choose the radius
    if((right - left - tempWidth) > (bottom - top - tempHeight - GetTextHeight(pMtr->pTitleFont)))
    {
        pMtr->radius = ((bottom - top - tempHeight - GetTextHeight(pMtr->pTitleFont)) >> 1) - ((tempHeight + bottom - top) >> 3);
    }
    else
        pMtr->radius = ((right - left) >> 1) - (tempWidth + ((right - left) >> 3));

    // center the meter on the given dimensions
    pMtr->xCenter = (left + right) >> 1;
    pMtr->yCenter = ((bottom + top) >> 1) - (tempHeight >> 1);

#elif (METER_TYPE == MTR_HALF_TYPE)

    // choose the radius
    if((right - left) >> 1 > (bottom - top))
    {
        pMtr->radius = (bottom - top) - ((tempHeight << 1) + ((bottom - top) >> 3));
        pMtr->yCenter = ((bottom + top) >> 1) + ((pMtr->radius + ((bottom - top) >> 3)) >> 1);
    }
    else
    {
        pMtr->radius = ((right - left) >> 1) - (tempWidth + ((right - left) >> 3));
        pMtr->yCenter = ((bottom + top) >> 1) + ((pMtr->radius + ((right - left) >> 3)) >> 1);
    }

    // center the meter on the given dimensions
    pMtr->xCenter = (left + right) >> 1;

#elif (METER_TYPE == MTR_QUARTER_TYPE)

    // choose the radius
    if
    (
        (right - left - tempWidth) >
            (bottom - top - (GetTextHeight(pMtr->pTitleFont) + GetTextHeight(pMtr->hdr.pGolScheme->pFont))) -
                (GOL_EMBOSS_SIZE << 1)
    )
    {
        pMtr->radius = bottom - top - (GetTextHeight(pMtr->pTitleFont) + GetTextHeight(pMtr->hdr.pGolScheme->pFont) + (GOL_EMBOSS_SIZE << 1));
    }
    else
    {
        pMtr->radius = right -
            left -
            (GetTextWidth(tempChar, pMtr->hdr.pGolScheme->pFont) * (MTR_BUILD_OPTION_SCALECHARCOUNT + 1)) -
            GOL_EMBOSS_SIZE;
    }

    pMtr->radius -= (((pMtr->radius) >> 2) + GOL_EMBOSS_SIZE);

    // center the meter on the given dimensions
    pMtr->xCenter = ((left + right) >> 1) - ((pMtr->radius + tempWidth + (pMtr->radius >> 2)) >> 1);
    pMtr->yCenter = ((top + bottom) >> 1) + ((pMtr->radius + (pMtr->radius >> 2)) >> 1);
#endif

}

/*********************************************************************
* Function: MtrSetVal(METER *pMtr, SHORT newVal)
*
* Notes: Sets the value of the meter to newVal. If newVal is less
*		 than 0, 0 is assigned. If newVal is greater than range,
*		 range is assigned.
*
********************************************************************/
void MtrSetVal(METER *pMtr, SHORT newVal)
{
    if((newVal < 0) || (newVal < pMtr->minValue))
    {
        pMtr->value = pMtr->minValue;
        return;
    }

    if(newVal > pMtr->maxValue)
    {
        pMtr->value = pMtr->maxValue;
        return;
    }

    pMtr->value = newVal;
}

/*********************************************************************
* Function: MtrMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
* Notes: This the default operation to change the state of the meter.
*		 Called inside GOLMsg() when GOLMsgCallback() returns a 1.
*
********************************************************************/
void MtrMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
{
    METER *pMtr;

    pMtr = (METER *)pObj;

    if(translatedMsg == MTR_MSG_SET)
    {
        MtrSetVal(pMtr, pMsg->param2);      // set the value	
        SetState(pMtr, MTR_DRAW_UPDATE);    // update the meter
    }
}

/*********************************************************************
* Function: WORD MtrTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Notes: Evaluates the message if the object will be affected by the 
*		 message or not.
*
********************************************************************/
WORD MtrTranslateMsg(void *pObj, GOL_MSG *pMsg)
{
    METER *pMtr;

    pMtr = (METER *)pObj;

    // Evaluate if the message is for the meter
    // Check if disabled first
    if(GetState(pMtr, MTR_DISABLED))
        return (OBJ_MSG_INVALID);

    if(pMsg->type == TYPE_SYSTEM)
    {
        if(pMsg->param1 == pMtr->hdr.ID)
        {
            if(pMsg->uiEvent == EVENT_SET)
            {
                return (MTR_MSG_SET);
            }
        }
    }

    return (OBJ_MSG_INVALID);
}

/*********************************************************************
* Function: WORD MtrDraw(void *pObj)
*
* Notes: This is the state machine to draw the meter.
*
********************************************************************/
WORD MtrDraw(void *pObj)
{
    typedef enum
    {
        IDLE,
        FRAME_DRAW,
        NEEDLE_DRAW,
        NEEDLE_ERASE,
        TEXT_DRAW,
        TEXT_DRAW_RUN,
        ARC_DRAW_SETUP,
        ARC_DRAW,
        SCALE_COMPUTE,
        SCALE_LABEL_DRAW,
        SCALE_DRAW,
#ifdef MTR_BUILD_OPTION_DISPLAY_VALUES_ENABLE
        VALUE_ERASE,
        VALUE_DRAW,
        VALUE_DRAW_RUN,
#endif
    } MTR_DRAW_STATES;

    static MTR_DRAW_STATES state = IDLE;
    static SHORT x1, y1, x2, y2;
    static SHORT temp, j, i, angle;
    static XCHAR strVal[MTR_BUILD_OPTION_SCALECHARCOUNT + 1];    // add one more space here for the NULL character
#ifdef MTR_BUILD_OPTION_DISPLAY_VALUES_ENABLE
    static XCHAR tempXchar[2] = {'8',0};        // NULL is pre-defined here	
#endif
    static float radian;
    static DWORD_VAL dTemp, dRes;
    METER *pMtr;

    pMtr = (METER *)pObj;


    while(1)
    {
        if(IsDeviceBusy())
            return (0);

        switch(state)
        {
            // location of this case is strategic so the partial redraw of the Meter will be faster        
            case FRAME_DRAW:
                if(!GOLPanelDrawTsk())
                {
                    return (0);
                }
                state = TEXT_DRAW;
                break;

            case IDLE:
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
            GFX_DRIVER_SetupDrawUpdate( pMtr->hdr.left,
                                        pMtr->hdr.top,
                                        pMtr->hdr.right,
                                        pMtr->hdr.bottom);
#endif
                if(GetState(pMtr, MTR_HIDE))
                {   // Hide the meter (remove from screen)
                    SetColor(pMtr->hdr.pGolScheme->CommonBkColor);
                    if(!Bar(pMtr->hdr.left, pMtr->hdr.top, pMtr->hdr.right, pMtr->hdr.bottom))
                        return (0);
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                    GFX_DRIVER_CompleteDrawUpdate(   pMtr->hdr.left,
                                                    pMtr->hdr.top,
                                                    pMtr->hdr.right,
                                                    pMtr->hdr.bottom);
#endif
                    return (1);
                }
    
                // Check if we need to draw the whole object
                SetLineThickness(NORMAL_LINE);
                SetLineType(SOLID_LINE);
                if(GetState(pMtr, MTR_DRAW))
                {
                    // set parameters to draw the frame
                    GOLPanelDraw
                        (
                            pMtr->hdr.left, pMtr->hdr.top, pMtr->hdr.right, pMtr->hdr.bottom, 0, pMtr->hdr.pGolScheme->Color0,
                            pMtr->hdr.pGolScheme->EmbossLtColor, pMtr->hdr.pGolScheme->EmbossDkColor, NULL,
                            GOL_EMBOSS_SIZE - 1
                        );
                    state = FRAME_DRAW;
                    break;
                }
                else
                {
                    state = NEEDLE_ERASE;
                    break;
                }
    
            case TEXT_DRAW:
    
                // draw the meter title
                SetColor(pMtr->hdr.pGolScheme->TextColor1);
                SetFont(pMtr->pTitleFont);
                temp = GetTextWidth(pMtr->pText, pMtr->pTitleFont);
    
                // set the start location of the meter title
#if (METER_TYPE == MTR_WHOLE_TYPE)
    #ifdef MTR_BUILD_OPTION_DISPLAY_VALUES_ENABLE
                MoveTo(pMtr->xCenter - (temp >> 1), pMtr->yCenter + pMtr->radius + GetTextHeight(pMtr->pValueFont));
    #else
                MoveTo(pMtr->xCenter - (temp >> 1), pMtr->yCenter + pMtr->radius + GetTextHeight(pMtr->hdr.pGolScheme->pFont));
    #endif
#elif (METER_TYPE == MTR_HALF_TYPE)
                MoveTo(pMtr->xCenter - (temp >> 1), pMtr->yCenter + 3);
#elif (METER_TYPE == MTR_QUARTER_TYPE)
                MoveTo
                    (
                        ((pMtr->hdr.right + pMtr->hdr.left) >> 1) - (temp >> 1), pMtr->hdr.bottom - GOL_EMBOSS_SIZE - GetTextHeight
                        (pMtr->pTitleFont)
                    );
#endif
                state = TEXT_DRAW_RUN;
    
            case TEXT_DRAW_RUN:
    
                // render the title of the meter
                if(!OutText(pMtr->pText))
                    return (0);
                state = ARC_DRAW_SETUP;
    
            //case ARC0_DRAW:
            case ARC_DRAW_SETUP:
    
                // check if we need to draw the arcs
                if(!GetState(pMtr, MTR_RING))
                {
    
                    // if meter is not RING type, for scale label colors use
                    // the three colors (normal, critical and danger)
                    i = DEGREE_END;
                    state = SCALE_COMPUTE;
                    break;
                }
                else
                {
    
                    // set the arc radii: x1 smaller radius and x2 as the larger radius
                    x1 = pMtr->radius + 2;
                    x2 = pMtr->radius + (pMtr->radius >> 2) + 2;
#if (METER_TYPE == MTR_WHOLE_TYPE)
                    temp = 6;
#elif (METER_TYPE == MTR_HALF_TYPE)
                    temp = 5;
#elif (METER_TYPE == MTR_QUARTER_TYPE)
                    temp = 3;
#endif
                    state = ARC_DRAW;
                    break;
                }
    
            case ARC_DRAW:
    
                // draw the arcs
#if (METER_TYPE == MTR_WHOLE_TYPE)
                while(temp)
#elif (METER_TYPE == MTR_HALF_TYPE)
                while(temp > 1)
#elif (METER_TYPE == MTR_QUARTER_TYPE)
                while(temp > 1)
#endif
                {
                    // decide which arc will be drawn
                    switch(temp)
                    {
                        case 6:
                            SetColor(pMtr->arcColor1);
                            if(!Arc(pMtr->xCenter, pMtr->yCenter, pMtr->xCenter, pMtr->yCenter, x1, x2, 0x20))
                                return (0);
                            break;
    
                        case 5:
                            SetColor(pMtr->arcColor2);
                            if(!Arc(pMtr->xCenter, pMtr->yCenter, pMtr->xCenter, pMtr->yCenter, x1, x2, 0x40))
                                return (0);
                            break;
    
                        case 4:
                            SetColor(pMtr->arcColor3);
                            if(!Arc(pMtr->xCenter, pMtr->yCenter, pMtr->xCenter, pMtr->yCenter, x1, x2, 0x80))
                                return (0);
                            break;
    
                        case 3:
                            SetColor(pMtr->arcColor4);
                            if(!Arc(pMtr->xCenter, pMtr->yCenter, pMtr->xCenter, pMtr->yCenter, x1, x2, 0x01))
                                return (0);
                            break;
    
                        case 2:
                            SetColor(pMtr->arcColor5);
                            if(!Arc(pMtr->xCenter, pMtr->yCenter, pMtr->xCenter, pMtr->yCenter, x1, x2, 0x02))
                                return (0);
                            break;
    
                        case 1:
                            SetColor(pMtr->arcColor6);
                            if(!Arc(pMtr->xCenter, pMtr->yCenter, pMtr->xCenter, pMtr->yCenter, x1, x2, 0x04))
                                return (0);
                            break;
    
                        default:
                            break;
                    }
                    temp--;
                }
    
                // set the color for the scale labels
                SetColor(pMtr->hdr.pGolScheme->Color1);
                i = DEGREE_END;
                state = SCALE_COMPUTE;
            
            case SCALE_COMPUTE:

                if(i >= DEGREE_START)
                {
                    radian = i * .0175;
    
                    if(!GetState(pMtr, MTR_RING))
                    {
                        if(i >= ARC1_DEGREE)
                        {
                            SetColor(pMtr->arcColor1);
                        }
                        else if(i >= ARC2_DEGREE)
                        {
                            SetColor(pMtr->arcColor2);
                        }
                        else if(i >= ARC3_DEGREE)
                        {
                            SetColor(pMtr->arcColor3);
                        }
                        else if(i >= ARC4_DEGREE)
                        {
                            SetColor(pMtr->arcColor4);
                        }
                        else if(i >= ARC5_DEGREE)
                        {
                            SetColor(pMtr->arcColor5);
                        }
                        else
                        {
                            SetColor(pMtr->arcColor6);
                        }
                    }
    
                    // compute for the effective radius of the scales
                    if((i % 45) == 0)
                        x2 = pMtr->radius + (pMtr->radius >> 2) + 2;
                    else
                        x2 = pMtr->radius + (pMtr->radius >> 3) + 3;
    
                    // compute the starting x1 and y1 position of the scales
                    // x2 here is the distance from the center to the x1, y1
                    // position. Sin and cos is used here since computation speed in initial
                    // drawing is not yet critical.
                    x1 = x2 * cos(radian);
                    y1 = (-1) * (x2 * sin(radian));
    
                    // using ratio and proportion we get the x2,y2 position
                    dTemp.Val = 0;
                    dTemp.w[1] = (pMtr->radius + 3);
                    dTemp.Val /= x2;
    
                    dRes.Val = dTemp.Val * x1;
                    x2 = dRes.w[1] + pMtr->xCenter; // adjusted to center
                    dRes.Val = dTemp.Val * y1;
                    y2 = dRes.w[1] + pMtr->yCenter; // adjusted to center
                    x1 += pMtr->xCenter;
                    y1 += pMtr->yCenter;            // adjust x1, y1 to the center
                    state = SCALE_DRAW;
                    break;
                }
                else
                {
                    state = NEEDLE_ERASE;
                    break;
                }
    
            case SCALE_DRAW:
                if(!Line(x1, y1, x2, y2))               // now draw the scales
                    return (0);
    
                if((i % 45) == 0)
                {
    
                    // draw the scale labels
                    // reusing radian, x2 and y2
                    // compute for the actual angle of needle to be shown in screen				
                    radian = (DEGREE_END - DEGREE_START) - (i - (DEGREE_START));
    
                    // compute the values of the label to be shown per 45 degree
                    temp = (pMtr->maxValue - pMtr->minValue) * (radian / (DEGREE_END - DEGREE_START));
    
                    // adjust for the minimum or offset value
                    temp += pMtr->minValue;
    
                    // this implements sprintf(strVal, "%d", temp); faster
                    // note that this is just for values >= 0, while sprintf covers negative values.
                    j = 1;
    
                    // get the ones value first and account for the required decimal point if enabled
                    if(GetState(pMtr, MTR_ACCURACY))
                    {
    
                        // round off to nearest tens
                        dTemp.w[0] = (temp % MTR_BUILD_OPTION_RESOLUTION) / (MTR_BUILD_OPTION_RESOLUTION / 10);
                        if((dTemp.w[0]) > (MTR_BUILD_OPTION_RESOLUTION >> 1))
                            temp += (MTR_BUILD_OPTION_RESOLUTION - dTemp.w[0]);
                        temp /= MTR_BUILD_OPTION_RESOLUTION;
                    }
    
                    do
                    {
                        strVal[MTR_BUILD_OPTION_SCALECHARCOUNT - j] = (temp % 10) + '0';
                        if(((temp /= 10) == 0) || (j >= MTR_BUILD_OPTION_SCALECHARCOUNT))
                            break;
                        j++;
                    } while(j <= MTR_BUILD_OPTION_SCALECHARCOUNT);
    
                    // the (&strVal[MTR_BUILD_OPTION_SCALECHARCOUNT-j]) removes the leading zeros.
                    // if leading zeroes will be printed change (&strVal[MTR_BUILD_OPTION_SCALECHARCOUNT-j])
                    // to simply strVal and remove the break statement above in the do-while loop
                    x2 = GetTextWidth((&strVal[MTR_BUILD_OPTION_SCALECHARCOUNT - j]), pMtr->hdr.pGolScheme->pFont);
                    y2 = GetTextHeight(pMtr->hdr.pGolScheme->pFont);
    
                    if(i == -45)
                    {
                        MoveTo(x1 + 3, y1);
                    }
                    else if(i == 0)
                    {
                        MoveTo(x1 + 3, y1 - (y2 >> 1));
                    }
                    else if(i == 45)
                    {
                        MoveTo(x1 + 3, y1 - (y2 >> 1) - 3);
                    }
                    else if(i == 90)
                    {
                        MoveTo(x1 - (x2 >> 1), y1 - (y2) - 3);
                    }
                    else if(i == 135)
                    {
                        MoveTo(x1 - (x2), y1 - (y2));
                    }
                    else if(i == 180)
                    {
                        MoveTo(x1 - (x2) - 3, y1 - (y2 >> 1));
                    }
                    else if(i == 225)
                    {
                        MoveTo(x1 - (x2 + 3), y1);
                    }
    
                    state = SCALE_LABEL_DRAW;
                    SetFont(pMtr->hdr.pGolScheme->pFont);
                }
                else
                {
                    i -= MTR_BUILD_OPTION_DEGREECOUNT;
                    state = SCALE_COMPUTE;
                    break;
                }
    
            case SCALE_LABEL_DRAW:
                if(!OutText(&strVal[MTR_BUILD_OPTION_SCALECHARCOUNT - j]))
                    return (0);
                i -= MTR_BUILD_OPTION_DEGREECOUNT;
                state = SCALE_COMPUTE;
                break;
    
            case NEEDLE_ERASE:

                if(GetState(pMtr, MTR_DRAW_UPDATE))
                {
    
                    // to update the needle, redraw the old position with background color
                    SetColor(pMtr->hdr.pGolScheme->Color0);
                    SetLineThickness(THICK_LINE);
                    if(!Line(pMtr->xCenter, pMtr->yCenter, pMtr->xPos, pMtr->yPos))
                        return (0);
                }
    
                state = NEEDLE_DRAW;
    
            case NEEDLE_DRAW:
                if(IsDeviceBusy())
                    return (0);
    
                // At this point, pMtr->value is assumed to contain the new value of the meter.
                // calculate the new angle:
                // equation is still ratio and proportion of angle and values.
                dTemp.Val = 0;
                dTemp.w[1] = pMtr->value - pMtr->minValue;
                dTemp.Val /= (pMtr->maxValue - pMtr->minValue);
                dTemp.Val *= (DEGREE_END - DEGREE_START);
    
                angle = DEGREE_END - (dTemp.w[1]);
                temp = angle;
    
                /* The method uses a lookup table of pre-calculated sine values. The table
    			   is derived from calculating sine values from 0 degrees to 90 degrees.
    			   To save space, the entries are just a byte size. So 360/256 = 1.40625 degrees
    			   increments is used for each entry. (i.e entry 0 is zero, entry i is 1.40625,
    			   entry 2 is 2*1.40625,... entry n is n*1.40625. 
    			   Get the sine of the entries yields a float value. Since we only use a 
    			   byte for storage we can shift the values by 128 without overflowing the 
    			   storage. Thus we need to shift the computed values by 8 bits. Shifting now 
    			   permits us to work with integers which greatly reduces the execution time.
    			   With this in mind, the input angles must be translated to 0-90 range with the
    			   quadrant of the original angle stored to translate back the value from the 
    			   table to the correct quadrant. Also the quadrant number will determine if 
    			   the calculated x and y positions are to be swapped. 
    			   In summary:
    			   	Swap x and y when calculating points in quadrant 2 and 4
    			   	Negate x when in quadrant 2 and 3
    			   	Negate y when in quadrant 1 and 2
    			*/
    
                // translate the angle to 0-90 range
                while(temp < 0)                     // this is needed for negative
                    temp += 90;                     // for negative values
                while(temp > 90)
                    temp -= 90;
    
                // determine which quadrant the angle is located
                // i determines if x and y are swapped (0 no swapping, 1 swapping needed)
                // y2 and x2 are the multiplier to negate or not the x and y values
                if((180 < angle) && (angle <= 270))
                {               // quadrant 3
                    i = 0;
                    y2 = 1;
                    x2 = -1;
                }
                else if((90 < angle) && (angle <= 180))
                {               // quadrant 2
                    i = 1;
                    y2 = -1;
                    x2 = -1;
                }
                else if((0 <= angle) && (angle <= 90))
                {               // quadrant 1
                    i = 0;
                    y2 = -1;
                    x2 = 1;
                }
                else if((-90 < angle) && (angle < 0))
                {               // quadrant 4
                    i = 1;
                    y2 = 1;
                    x2 = 1;
                }
    
                // using the calculated angle we get the sine and cosine 
                // values from the look up table, then to avoid
                // division, the inverse is used
                x1 = Cosine(temp) * pMtr->radius;
                y1 = Sine(temp) * pMtr->radius;
    
                // calculate new positions, check if we need to reverse x and y values
                pMtr->xPos = ((x2) * (((i == 0) ? x1 : y1) >> 8)) + pMtr->xCenter;
                pMtr->yPos = ((y2) * (((i == 0) ? y1 : x1) >> 8)) + pMtr->yCenter;
    
                // now draw the needle with the new position	
#if (COLOR_DEPTH == 8) || (COLOR_DEPTH == 16) || (COLOR_DEPTH == 24)
                SetColor(BRIGHTRED);
#elif (COLOR_DEPTH == 4)
                SetColor(GRAY000);
#elif (COLOR_DEPTH == 1)
                SetColor(BLACK);
#endif
                SetLineThickness(THICK_LINE);
                if(!Line(pMtr->xCenter, pMtr->yCenter, pMtr->xPos, pMtr->yPos))
                    return (0);
                SetLineThickness(NORMAL_LINE);
#ifdef MTR_BUILD_OPTION_DISPLAY_VALUES_ENABLE
                state = VALUE_ERASE;
#else
                // reset the line to normal
                SetLineThickness(NORMAL_LINE);
                state = IDLE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pMtr->hdr.left,
                                                pMtr->hdr.top,
                                                pMtr->hdr.right,
                                                pMtr->hdr.bottom);
#endif
                return (1);
#endif
#ifdef MTR_BUILD_OPTION_DISPLAY_VALUES_ENABLE
    
            case VALUE_ERASE:
                if(IsDeviceBusy())
                    return (0);
    
                // reset the line to normal
                SetLineThickness(NORMAL_LINE);
    
                // display the value
                // erase previous value first. The temp>>1 accomodates fonts with characters that has unequal widths
                temp = GetTextWidth(tempXchar, pMtr->pValueFont);
                temp = temp * MTR_BUILD_OPTION_SCALECHARCOUNT + (temp >> 1);
    
                SetColor(pMtr->hdr.pGolScheme->Color0);
    
#if (METER_TYPE == MTR_WHOLE_TYPE)
                if
                (
                    !Bar
                        (
                            pMtr->xCenter -
                            (temp >> 1), pMtr->yCenter +
                            pMtr->radius, pMtr->xCenter +
                            (temp >> 1), pMtr->yCenter +
                            pMtr->radius +
                            GetTextHeight(pMtr->pValueFont)
                        )
                ) return (0);
    
                        #elif (METER_TYPE == MTR_HALF_TYPE)
                if
                (
                    !Bar
                        (
                            pMtr->xCenter -
                            (temp >> 1), pMtr->yCenter -
                            GetTextHeight(pMtr->pValueFont), pMtr->xCenter +
                            (temp >> 1), pMtr->yCenter
                        )
                ) return (0);
    
                        #elif (METER_TYPE == MTR_QUARTER_TYPE)
                if
                (
                    !Bar
                        (
                            pMtr->xCenter -
                            1, pMtr->yCenter -
                            GetTextHeight(pMtr->pValueFont), pMtr->xCenter +
                            temp, pMtr->yCenter +
                            1
                        )
                ) return (0);
#endif
                state = VALUE_DRAW;
    
            case VALUE_DRAW:
                if(IsDeviceBusy())
                    return (0);
    
                if(angle >= ARC1_DEGREE)
                {
                    SetColor(pMtr->arcColor1);
                }
                else if(angle >= ARC2_DEGREE)
                {
                    SetColor(pMtr->arcColor2);
                }
                else if(angle >= ARC3_DEGREE)
                {
                    SetColor(pMtr->arcColor3);
                }
                else if(angle >= ARC4_DEGREE)
                {
                    SetColor(pMtr->arcColor4);
                }
                else if(angle >= ARC5_DEGREE)
                {
                    SetColor(pMtr->arcColor5);
                }
                else
                {
                    SetColor(pMtr->arcColor6);
                }
    
                // display the current value
                SetFont(pMtr->pValueFont);
    
                // this implements sprintf(strVal, "%03d", pMtr->value); faster
                // note that this is just for values >= 0, while sprintf covers negative values.
                i = pMtr->value;
                j = 1;
    
                // get the ones value first and account for the required decimal point if enabled
                if(GetState(pMtr, MTR_ACCURACY))
                {
                    strVal[MTR_BUILD_OPTION_SCALECHARCOUNT - j] = (((i % MTR_BUILD_OPTION_RESOLUTION)) / (MTR_BUILD_OPTION_RESOLUTION / 10)) + '0';
                    i /= MTR_BUILD_OPTION_RESOLUTION;
                    j++;
                    strVal[MTR_BUILD_OPTION_SCALECHARCOUNT - j] = '.';
                    j++;
                }
    
                do
                {
                    strVal[MTR_BUILD_OPTION_SCALECHARCOUNT - j] = (i % 10) + '0';
                    if(((i /= 10) == 0) || (j >= MTR_BUILD_OPTION_SCALECHARCOUNT))
                        break;
                    j++;
                } while(j <= MTR_BUILD_OPTION_SCALECHARCOUNT);
    
                temp = GetTextWidth(&strVal[MTR_BUILD_OPTION_SCALECHARCOUNT - j], pMtr->pValueFont);
    
#if (METER_TYPE == MTR_WHOLE_TYPE)
                MoveTo(pMtr->xCenter - (temp >> 1), pMtr->yCenter + pMtr->radius);
#elif (METER_TYPE == MTR_HALF_TYPE)
                MoveTo(pMtr->xCenter - (temp >> 1), pMtr->yCenter - GetTextHeight(pMtr->pValueFont));
#elif (METER_TYPE == MTR_QUARTER_TYPE)
                MoveTo(pMtr->xCenter, pMtr->yCenter - GetTextHeight(pMtr->pValueFont));
#endif
                state = VALUE_DRAW_RUN;
    
            case VALUE_DRAW_RUN:
                if(!OutText(&strVal[MTR_BUILD_OPTION_SCALECHARCOUNT - j]))
                    return (0);
                state = IDLE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pMtr->hdr.left,
                                                pMtr->hdr.top,
                                                pMtr->hdr.right,
                                                pMtr->hdr.bottom);
#endif
                return (1);
#endif // end of #ifdef MTR_BUILD_OPTION_DISPLAY_VALUES_ENABLE
        }   // endo of switch()
    }  // end of while(1)
}

            #endif // USE_METER
