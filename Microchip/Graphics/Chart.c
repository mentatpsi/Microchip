/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Chart
 *****************************************************************************
 * FileName:        Chart.c
 * Dependencies:    Chart.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 Version 3.00, C32
 * Linker:          MPLAB LINK30, LINK32
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
 * 4/8/08       ...
 * 8/8/08       Centered values displayed on bar charts
 *              Removed line drawn on pie chart when no slices 
 *              are present.
 * 9/30/08      3-D bar depth is now equal to chart depth.
 *              Flushed 2-D Bars to equal max height of chart
 *              when equal or greater than.
 * 6/29/09      Modified Draw Sector function to be state based.
 *****************************************************************************/
#include "Graphics/Graphics.h"
#include <math.h>

#ifdef USE_CHART

// internal functions and macros
WORD        word2xchar(WORD pSmple, XCHAR *xcharArray, WORD cnt);
void        GetCirclePoint(SHORT radius, SHORT angle, SHORT *x, SHORT *y);
WORD        DrawSector(SHORT cx, SHORT cy, SHORT outRadius, SHORT angleFrom, SHORT angleTo, GFX_COLOR outLineColor);
GFX_COLOR   GetColorShade(GFX_COLOR color, BYTE shade);
WORD        ChParseShowData(DATASERIES *pData);
DATASERIES  *ChGetNextShowData(DATASERIES *pData);
SHORT       ChSetDataSeries(CHART *pCh, WORD seriesNum, BYTE status);

// array used to define the default colors used to draw the bars or sectors of the chart
const GFX_COLOR  ChartVarClr[16] =  
            {  
                CH_CLR0, CH_CLR1, CH_CLR2, CH_CLR3,
                CH_CLR4, CH_CLR5, CH_CLR6, CH_CLR7,
                CH_CLR8, CH_CLR9, CH_CLR10,CH_CLR11,
                CH_CLR12,CH_CLR13,CH_CLR14,CH_CLR15
            };

/*********************************************************************
* Function: CHART  *ChCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, WORD state ,CHARTDATA *pData, 
*                              GOL_SCHEME *pScheme)
*
*
* Notes: Creates a CHART object and adds it to the current active list.
*        If the creation is successful, the pointer to the created Object 
*        is returned. If not successful, NULL is returned. Chart is not
*        supporting the use of Palette when rendering 3-D bar charts. 
*
********************************************************************/
CHART *ChCreate
(
    WORD        ID,
    SHORT       left,
    SHORT       top,
    SHORT       right,
    SHORT       bottom,
    WORD        state,
    DATASERIES  *pData,
    CHARTPARAM  *pParam,
    GOL_SCHEME  *pScheme
)
{
    CHART   *pCh = NULL;

    pCh = (CHART *)GFX_malloc(sizeof(CHART));

    if(pCh == NULL)
        return (NULL);

    pCh->hdr.ID = ID;           // unique id assigned for referencing
    pCh->hdr.pNxtObj = NULL;    // initialize pointer to NULL
    pCh->hdr.type = OBJ_CHART;  // set object type
    pCh->hdr.left = left;       // left position
    pCh->hdr.top = top;         // top position
    pCh->hdr.right = right;     // right position
    pCh->hdr.bottom = bottom;   // bottom position
    pCh->hdr.state = state;     // state
    pCh->hdr.DrawObj = ChDraw;				// draw function
    pCh->hdr.MsgObj = ChTranslateMsg;   	// message function
    pCh->hdr.MsgDefaultObj = NULL;  		// default message function
    pCh->hdr.FreeObj = ChFreeDataSeries;  	// free function
    
    if(pParam != NULL)
    {
        pCh->prm.pTitle = pParam->pTitle;
        pCh->prm.pSmplLabel = pParam->pSmplLabel;
        pCh->prm.pValLabel = pParam->pValLabel;
        pCh->prm.smplStart = pParam->smplStart;
        pCh->prm.smplEnd = pParam->smplEnd;
        pCh->prm.valMax = pParam->valMax;
        pCh->prm.valMin = pParam->valMin;
        pCh->prm.pColor = pParam->pColor;
        pCh->prm.pTitleFont = pParam->pTitleFont;
        pCh->prm.pAxisLabelsFont = pParam->pAxisLabelsFont;
        pCh->prm.pGridLabelsFont = pParam->pGridLabelsFont;
    }
    else
    {
        pCh->prm.pTitle = NULL;
        pCh->prm.pSmplLabel = NULL;
        pCh->prm.pValLabel = NULL;
        pCh->prm.smplStart = 0;
        pCh->prm.smplEnd = 0;
        pCh->prm.valMax = 0;
        pCh->prm.valMin = 0;

        // use the default color table
        pCh->prm.pColor = (GFX_COLOR *)ChartVarClr;
        pCh->prm.pTitleFont = _pDefaultGolScheme->pFont;
        pCh->prm.pAxisLabelsFont = _pDefaultGolScheme->pFont;
        pCh->prm.pGridLabelsFont = _pDefaultGolScheme->pFont;
    }

    pCh->pChData = pData;       // assign the chart data

    // check if how variables have SHOW_DATA flag set
    pCh->prm.seriesCount = ChParseShowData(pData);

    // Set the color scheme to be used
    if(pScheme == NULL)
    {
        pCh->hdr.pGolScheme = _pDefaultGolScheme;
    }
    else
    {
        pCh->hdr.pGolScheme = (GOL_SCHEME *)pScheme;
        pCh->prm.pTitleFont = pCh->hdr.pGolScheme->pFont;
        pCh->prm.pAxisLabelsFont = pCh->hdr.pGolScheme->pFont;
        pCh->prm.pGridLabelsFont = pCh->hdr.pGolScheme->pFont;
    }

    GOLAddObject((OBJ_HEADER *)pCh);

    return (pCh);
}

/*********************************************************************
* Function: WORD ChTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Notes: Evaluates the message if the object will be affected by the 
*		 message or not.
*
********************************************************************/
WORD ChTranslateMsg(void *pObj, GOL_MSG *pMsg)
{

    CHART *pCh;

    pCh = (CHART *)pObj;

    // Evaluate if the message is for the static text
    // Check if disabled first
    if(GetState(pCh, CH_DISABLED))
        return (OBJ_MSG_INVALID);

        #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {

        // Check if it falls in static text control borders
        if
        (
            (pCh->hdr.left < pMsg->param1) &&
            (pCh->hdr.right > pMsg->param1) &&
            (pCh->hdr.top < pMsg->param2) &&
            (pCh->hdr.bottom > pMsg->param2)
        )
        {
            return (CH_MSG_SELECTED);
        }
    }

        #endif
    return (OBJ_MSG_INVALID);
}

////////////////////////////////////////////////

// internal functions
////////////////////////////////////////////////
DATASERIES *ChGetNextShowData(DATASERIES *pData)
{
    DATASERIES  *pVar = pData;

    // find the next data series that will be shown
    while(pVar->show != SHOW_DATA)
    {
        if((pVar = (DATASERIES *)pVar->pNextData) == NULL)
            return (NULL);
    }

    return (pVar);
}

/* */
WORD ChParseShowData(DATASERIES *pData)
{
    DATASERIES  *pParse;
    WORD        sCnt = 0;

    if(pData != NULL)
    {
        pParse = pData;
        while(pParse != NULL)
        {
            if(pParse->show == SHOW_DATA)
                sCnt++;
            pParse = (DATASERIES *)pParse->pNextData;
        }
    }

    return (sCnt);
}

/* */
WORD GetLongestNameLength(CHART *pCh)
{
    WORD        temp = 0;
    DATASERIES  *pVar;

    if(!GetState(pCh, CH_LEGEND))
        return (0);

    // find the data series with the longest name
    pVar = pCh->pChData;

    while(pVar)
    {

        // check if the data series is to be shown
        if(pVar->show == SHOW_DATA)
        {
            if(temp < GetTextWidth((XCHAR *)pVar->pSData, pCh->hdr.pGolScheme->pFont))
                temp = GetTextWidth((XCHAR *)pVar->pSData, pCh->hdr.pGolScheme->pFont);
        }

        pVar = pVar->pNextData;
    }

    return (temp);
}

/* */
WORD word2xchar(WORD pSmple, XCHAR *xcharArray, WORD cnt)
{
    WORD            j, z;
    static XCHAR    *pXchar;

    pXchar = xcharArray;

    // this implements sprintf(strVal, "%d", temp); faster
    // note that this is just for values >= 0, while sprintf covers negative values.
    j = 1;
    z = pSmple;

    pXchar = &(*xcharArray) + (cnt - j);
    do
    {
        *pXchar = (z % 10) + '0';
        pXchar--;
        if((z /= 10) == 0)
            break;
        j++;
    } while(j <= cnt);
    return (j);
}

/*********************************************************************
* Function: GFX_COLOR GetColorShade(GFX_COLOR color, BYTE shade)
*
* Notes: This function generates the shades required to draw the 
*        3-D bar chart. The color given color will be in the format  
*        for the given COLOR_DEPTH setting. The r, g and b colors
*        are extracted from the composite GFX_COLOR value and 
* 		 computes the shade of the same color by shifting 
*		 the rgb values depending on the shade value.
*		 The idea here is to get the given r,g and b colors and 
*		 make them approach the gray color by shifting the each value
*		 closer to 128. If rgb values are > 128 we subtract and add 
*		 if < 128 we add.
*        For color depth of 1bpp the returned color is the same 
*        as the given color.
*
********************************************************************/
GFX_COLOR GetColorShade(GFX_COLOR color, BYTE shade)
{
    GFX_COLOR   newColor;
    BYTE        rgb[3];
    BYTE        i, limit;
    BYTE        midValue;

#if (COLOR_DEPTH == 1)
    
    return color;

#elif (COLOR_DEPTH == 4)

    // from: #define RGBConvert(red, green, blue) (GFX_COLOR) (((GFX_COLOR)(red) & 0xE0) | (((GFX_COLOR)(green) & 0xE0) >> 3) | (((GFX_COLOR)(blue)) >> 6))
    rgb[0] = (BYTE)(color & 0x0F);          // red

#elif (COLOR_DEPTH == 8)
    
    // from: #define RGBConvert(red, green, blue) (GFX_COLOR) (((GFX_COLOR)(red) & 0xE0) | (((GFX_COLOR)(green) & 0xE0) >> 3) | (((GFX_COLOR)(blue)) >> 6))
    rgb[0] = (BYTE)(color & 0xE0);          // red
    rgb[1] = (BYTE)(color << 3);            // green
    rgb[2] = (BYTE)(color << 6);            // blue
    
#elif (COLOR_DEPTH == 16)

    // from: #define RGBConvert(red, green, blue)    (GFX_COLOR) (((((GFX_COLOR)(red) & 0xF8) >> 3) << 11) | ((((GFX_COLOR)(green) & 0xFC) >> 2) << 5) | (((GFX_COLOR)(blue) & 0xF8) >> 3))                                               
    rgb[0] = (BYTE)((color >> 11) << 3);    // red
    rgb[1] = (BYTE)((color >> 5) << 2);     // green
    rgb[2] = (BYTE)((color) << 3);          // blue

#elif (COLOR_DEPTH == 24)

    // from: #define RGBConvert(red, green, blue)    (GFX_COLOR) (((GFX_COLOR)(red) << 16) | ((GFX_COLOR)(green) << 8) | (GFX_COLOR)(blue))
    rgb[0] = (BYTE)((GFX_COLOR)(color & ((GFX_COLOR)0x00FF0000ul)) >> 16);  // red
    rgb[1] = (BYTE)((GFX_COLOR)(color & ((GFX_COLOR)0x0000FF00ul)) >> 8);   // green
    rgb[2] = (BYTE)((GFX_COLOR)(color & ((GFX_COLOR)0x000000FFul)));        // blue

    
#endif

#if (COLOR_DEPTH == 4)
    midValue = 7;
    limit = 1;
#else
    midValue = 128;
    limit = 3;
#endif

#if (COLOR_DEPTH != 1)

    for(i = 0; i < limit; i++)
    {
        if(rgb[i] > midValue)
            rgb[i] = rgb[i] - ((rgb[i] - midValue) >> (shade));
        else
            rgb[i] = rgb[i] + ((midValue - rgb[i]) >> (shade));
    }

#if (COLOR_DEPTH == 4)
    newColor = rgb[0];
#else
    newColor = RGBConvert(rgb[0], rgb[1], rgb[2]);
#endif
    return (newColor);

#endif

}

/*********************************************************************
* Function: WORD ChDraw(void *pObj)
*
*
* Notes: This is the state machine to draw the button.
*
********************************************************************/
    #define STR_CHAR_CNT        11
    #define DCLR_STR_CHAR_CNT   (STR_CHAR_CNT + 1)

/* */

WORD ChDraw(void *pObj)
{
    typedef enum
    {
        REMOVE,
        FRAME_DRAW_PREP,
        FRAME_DRAW,
        CHECK_CHART_TYPE,

        // BAR type states
        GRID_PREP,
        SAMPLE_GRID_DRAW1,
        VALUE_GRID_DRAW1,
        SAMPLE_GRID_DRAW2,
        VALUE_GRID_DRAW2,
        VALUE_GRID_3D_DRAW,
        TITLE_LABEL_DRAW_SET,
        TITLE_LABEL_DRAW_RUN,
        SAMPLE_LABEL_DRAW_SET,
        SAMPLE_LABEL_DRAW_RUN,
        VALUE_LABEL_DRAW_INIT,
        VALUE_LABEL_DRAW_SET,
        VALUE_LABEL_DRAW_RUN,
        XAXIS_LABEL_DRAW_RUN,
        XAXIS_LABEL_DRAW_SET,
        YAXIS_LABEL_DRAW_RUN,
        YAXIS_LABEL_DRAW_SET,
        LEGEND_DRAW_BOX,
        LEGEND_DRAW_RUN,
        LEGEND_DRAW_UPDATE_VAR,
        DATA_DRAW_INIT,
        DATA_DRAW_SET,
        BAR_DATA_DRAW,
        BAR_DATA_DRAW_CHECK,
        BAR_DATA_DRAW_3D_PREP,
        BAR_DATA_DRAW_3D_LOOP_1,
        BAR_DATA_DRAW_3D_LOOP_2,
        BAR_DATA_DRAW_3D_OUTLINE1,
        BAR_DATA_DRAW_3D_OUTLINE2,
        BAR_DATA_DRAW_3D_OUTLINE3,
        BAR_DATA_DRAW_3D_OUTLINE4,
        BAR_DATA_DRAW_3D_OUTLINE5,
        PIE_DONUT_HOLE_DRAW,
        BAR_DATA_DRAW_VALUE,
        BAR_DATA_DRAW_VALUE_RUN,

        // PIE type states
        PIE_PREP,
        PIE_DRAW_OUTLINE1,
        PIE_DRAW_SECTOR,
        PIE_DRAW_SECTOR_LOOP,
        PIE_DRAW_SECTOR_ACTUAL,
        PIE_DRAW_SECTOR_LOOP_CONTINUE,
        PIE_DRAW_SECTOR_LOOP_CREATE_STRINGS,
        PIE_DRAW_SECTOR_LOOP_STRINGS_RUN,
    } CH_DRAW_STATES;

    static XCHAR tempXchar[2] = {'B',0};
    static XCHAR temp2Xchar[2] = {'M',0};
    static XCHAR tempStr[DCLR_STR_CHAR_CNT] = {'0','0','0','0','0','0','0','0','0','0',0};

    static CH_DRAW_STATES state = REMOVE;
    static WORD x, y, z, xStart, yStart, ctr, ctry, samplesMax, temp;
    static SHORT uLocator;
    static WORD splDelta, valDelta;
    static WORD barWidth, barDepth, chart3DDepth;

    static DATASERIES *pVar;
    static WORD *pSmple;
    static XCHAR *pXcharTemp;
    static DWORD dTemp;
    static SHORT varCtr, pieX, pieY;
    static DWORD dPercent;
    static WORD j = 0, k = 0, h = 0, i, m;
    static void *pVarFont;

    static WORD pieLabelXPos;
    static WORD pieLabelYPos, pieLabelYPos2, pieLabelYPos3;
    static WORD pieSectorXPos;
    static WORD pieSectorYPos;

    CHART *pCh;

    pCh = (CHART *)pObj;

    if(IsDeviceBusy())
        return (0);

    switch(state)
    {
        case REMOVE:
            if(IsDeviceBusy())
                return (0);

#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
            GFX_DRIVER_SetupDrawUpdate( pCh->hdr.left,
                                        pCh->hdr.top,
                                        pCh->hdr.right,
                                        pCh->hdr.bottom);
#endif
            if(GetState(pCh, CH_HIDE))
            {   // Hide the Chart (remove from screen)
                SetColor(pCh->hdr.pGolScheme->CommonBkColor);
                if(!Bar(pCh->hdr.left, pCh->hdr.top, pCh->hdr.right, pCh->hdr.bottom))
                    return (0);

#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pCh->hdr.left,
                                                pCh->hdr.top,
                                                pCh->hdr.right,
                                                pCh->hdr.bottom);
#endif
                return (1);
            }

            SetLineThickness(NORMAL_LINE);
            SetLineType(SOLID_LINE);

            // check if we only need to refresh the data on the chart
            if(GetState(pCh, CH_DRAW_DATA))
            {

                // this is only performed when refreshing data in the chart
                // erase the current contents
                SetColor(pCh->hdr.pGolScheme->CommonBkColor);

                // get the ending x position where redraw will take place (if legend is drawn,
                // we do not need to redraw this area)
                i = GetLongestNameLength(pCh) + GetTextHeight(pCh->hdr.pGolScheme->pFont) + GOL_EMBOSS_SIZE + (CH_MARGIN << 1);

                if(GetState(pCh, CH_BAR))
                {

                    // get the starting x position where redraw will take place
                    h = xStart - GetTextWidth(tempXchar, pCh->prm.pGridLabelsFont) - (GetTextWidth(temp2Xchar, pCh->prm.pGridLabelsFont) >> 1);

                    // get the starting y position
                    j = yStart - ((ChGetSampleRange(pCh) + 1) * splDelta);
                    if(GetState(pCh, CH_3D_ENABLE))
                    {
                        if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                        {

                            // adjust for 3D effects
                            j -= chart3DDepth;
                            if((GetState(pCh, CH_LEGEND)) && (ChGetShowSeriesCount(pCh) != 1))
                            {
                                if(!Bar(h, j, pCh->hdr.right - i, yStart))
                                    return (0);
                            }
                            else
                            {
                                if(!Bar(h, j, pCh->hdr.right - CH_MARGIN, yStart))
                                    return (0);
                            }
                        }
                        else
                        {
                            if
                            (
                                !Bar
                                    (
                                        xStart,
                                        yStart - ((CH_YGRIDCOUNT - 1) * valDelta) - chart3DDepth - (GetTextHeight(pCh->hdr.pGolScheme->pFont)),
                                        xStart + splDelta * (ChGetSampleRange(pCh) + 1) + chart3DDepth,
                                        yStart + GetTextHeight(pCh->prm.pGridLabelsFont)
                                    )
                            ) return (0);
                        }
                    }
                    else
                    {
                        if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                        {
                            if((GetState(pCh, CH_LEGEND)) && (ChGetShowSeriesCount(pCh) != 1))
                            {
                                if(!Bar(h, j, pCh->hdr.right - i, yStart))
                                    return (0);
                            }
                            else
                            {
                                if(!Bar(h, j, pCh->hdr.right - CH_MARGIN, yStart))
                                    return (0);
                            }
                        }
                        else
                        {
                            if
                            (
                                !Bar
                                    (
                                        xStart,
                                        yStart - ((CH_YGRIDCOUNT - 1) * valDelta) - (GetTextHeight(pCh->hdr.pGolScheme->pFont)),
                                        xStart + splDelta * (ChGetSampleRange(pCh) + 1),
                                        yStart + GetTextHeight(pCh->prm.pGridLabelsFont)
                                    )
                            ) return (0);
                        }
                    }
                }
                else
                {
                    if((GetState(pCh, CH_LEGEND)) && (ChGetShowSeriesCount(pCh) != 1))
                        i = pCh->hdr.right - i;
                    else
                        i = pCh->hdr.right - GOL_EMBOSS_SIZE - CH_MARGIN;
                    h = pCh->hdr.left + GOL_EMBOSS_SIZE;
                    j = pCh->hdr.top + GOL_EMBOSS_SIZE + CH_MARGIN + GetTextHeight(pCh->prm.pTitleFont);

                    // erase the current pie chart drawn
                    if(!Bar(h, j, i, pCh->hdr.bottom - CH_MARGIN))
                        return (0);
                }

                // check the type of chart
                if(GetState(pCh, CH_BAR))
                {
                    state = GRID_PREP;
                    goto chrt_grid_prep;
                }
                else
                {
                    state = PIE_PREP;
                    goto chrt_pie_prep;
                }
            }

            state = FRAME_DRAW_PREP;

        /*========================================================================*/
        //					  Draw the frame
        /*========================================================================*/
        case FRAME_DRAW_PREP:

            // check how many data series do we need to display
            pCh->prm.seriesCount = ChParseShowData(pCh->pChData);

            // set up the frame drawing
            GOLPanelDraw
            (
                pCh->hdr.left,
                pCh->hdr.top,
                pCh->hdr.right,
                pCh->hdr.bottom,
                0,
                pCh->hdr.pGolScheme->CommonBkColor,
                pCh->hdr.pGolScheme->EmbossLtColor,
                pCh->hdr.pGolScheme->EmbossDkColor,
                NULL,
                1
            );

            state = FRAME_DRAW;

        case FRAME_DRAW:
            if(!GOLPanelDrawTsk())
            {
                return (0);
            }

            state = TITLE_LABEL_DRAW_SET;

        /*========================================================================*/
        //					  Draw the Chart Title
        /*========================================================================*/
        case TITLE_LABEL_DRAW_SET:

            // find the location of the title
            MoveTo
            (
                pCh->hdr.left + ((pCh->hdr.right + pCh->hdr.left - GetTextWidth((XCHAR *)pCh->prm.pTitle, pCh->prm.pTitleFont)) >> 1),
                pCh->hdr.top + CH_MARGIN
            );
            state = TITLE_LABEL_DRAW_RUN;

        case TITLE_LABEL_DRAW_RUN:

            // Set the font
            SetFont(pCh->prm.pTitleFont);

            // NOTE: we use the emboss dark color here to draw the chart title.
            SetColor(pCh->hdr.pGolScheme->EmbossDkColor);

            if(!OutText(pCh->prm.pTitle))
                return (0);

            // check if legend will be drawn
            if(GetState(pCh, CH_LEGEND) && (ChGetShowSeriesCount(pCh) != 1))
            {

                // position the x and y points to the start of the first variable
                temp = GetLongestNameLength(pCh);

                x = pCh->hdr.right - (CH_MARGIN << 1) - temp - GetTextHeight(pCh->hdr.pGolScheme->pFont);
                y = ((pCh->hdr.bottom + pCh->hdr.top) >> 1) - ((pCh->prm.seriesCount * GetTextHeight(pCh->hdr.pGolScheme->pFont)) >> 1);

                // initialize the variable counter for the legend drawing	
                temp = 0;
                pVar = (DATASERIES *)pCh->pChData;
                state = LEGEND_DRAW_BOX;
            }
            else
            {

                // legend will not be drawn, go to data drawing next
                state = CHECK_CHART_TYPE;
                goto chrt_check_chart_type;
            }

            /*========================================================================*/
            //					  Draw the Legend
            /*========================================================================*/
    chrt_draw_legend:

        case LEGEND_DRAW_BOX:

            // check if we will be showing this data series
            if(ChGetShowSeriesStatus(pVar) == SHOW_DATA)
            {
                SetColor(*(&(*pCh->prm.pColor) + temp));
                if((ChGetShowSeriesCount(pCh) == 1) && (GetState(pCh, CH_PIE)))
                { }
                else
                {
                    if
                    (
                        !Bar
                            (
                                x,
                                y + (GetTextHeight(pCh->hdr.pGolScheme->pFont) >> 2),
                                x + (GetTextHeight(pCh->hdr.pGolScheme->pFont) >> 1),
                                y +
                                    (
                                        GetTextHeight(pCh->hdr.pGolScheme->pFont) -
                                            (GetTextHeight(pCh->hdr.pGolScheme->pFont) >> 2)
                                    )
                            )
                    ) return (0);
                }

                MoveTo(x + 2 + (GetTextHeight(pCh->hdr.pGolScheme->pFont) >> 1), y);
                state = LEGEND_DRAW_RUN;
            }
            else
            {
                state = LEGEND_DRAW_UPDATE_VAR;
                goto chrt_draw_legend_update;
            }

        case LEGEND_DRAW_RUN:
            SetColor(pCh->hdr.pGolScheme->TextColor1);
            SetFont(pCh->hdr.pGolScheme->pFont);

            if(!OutText(pVar->pSData))
                return (0);

            // increment the variable counter
            temp++;
            if(temp == ChGetShowSeriesCount(pCh))
            {
                state = CHECK_CHART_TYPE;
                goto chrt_check_chart_type;
            }
            else
                state = LEGEND_DRAW_UPDATE_VAR;

    chrt_draw_legend_update:

        case LEGEND_DRAW_UPDATE_VAR:

            // update the data series pointer and y position
            if(ChGetShowSeriesStatus(pVar) == SHOW_DATA)
                y += GetTextHeight(pCh->hdr.pGolScheme->pFont);
            pVar = (DATASERIES *)pVar->pNextData;
            state = LEGEND_DRAW_BOX;
            goto chrt_draw_legend;

    chrt_check_chart_type:

        case CHECK_CHART_TYPE:
            if(GetState(pCh, CH_BAR))
            {
                state = GRID_PREP;
            }
            else if(GetState(pCh, CH_PIE))
            {
                state = PIE_PREP;
                goto chrt_pie_prep;
            }
            else
            {
                state = REMOVE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pCh->hdr.left,
                                                pCh->hdr.top,
                                                pCh->hdr.right,
                                                pCh->hdr.bottom);
#endif
                return (1);
            }

            /**************************************************************************/
            // 					BAR CHART states
            /**************************************************************************/

            /*========================================================================*/
            //					  Draw the grids
            /*========================================================================*/
    chrt_grid_prep:

        case GRID_PREP:

            /* NOTE: X or Y Grid Labels - label for each division in the x or y axis
        			 X or Y Axis Labels - label to name the x or y axis. Text is 
        			 					  user given in the CHART structure, 
        			 					  CHARTPARAM member.
        	*/

            // count the number of characters needed for the axis label that
            // represents the value of the samples (or bars)
            // get the width of one character
            temp = GetTextWidth((XCHAR *)tempXchar, pCh->prm.pGridLabelsFont);

            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {

                // if in the horizontal orientation width will only be
                // max of 2 characters (1, 2, 3...10, 11... or A, B, C...)
                if((ChGetSampleEnd(pCh) - ChGetSampleStart(pCh)) > 9)
                    y = 2;
                else
                    y = 1;
            }
            else
            {
                if(GetState(pCh, CH_PERCENT))
                {

                    // include in the computation the space that will be occupied by '%' sign
                    y = 4;
                }
                else
                {
                    x = ChGetValueRange(pCh);
                    y = 1;

                    // count the number of characters needed
                    while(x /= 10)
                        ++y;
                }
            }

            // estimate the space that will be occupied by the y grid labels
            temp = temp * y;

            // get x starting position
            xStart = CH_MARGIN + temp + pCh->hdr.left;

            // adjust x start to accomodate Y axis label
            xStart += (GetTextHeight(pCh->prm.pAxisLabelsFont) + (GetTextWidth(temp2Xchar, pCh->prm.pGridLabelsFont) >> 1));

            // now get y starting position
            yStart = pCh->hdr.bottom - (GetTextHeight(pCh->prm.pGridLabelsFont) + GetTextHeight(pCh->prm.pAxisLabelsFont) + CH_MARGIN);

            // =======================================================================
            // Sample delta (splDelta) and Value delta (valDelta) will depend if the
            // chart is drawn horizontally or vertically.
            // =======================================================================
            // find the variable with the longest name
            // to add space for the names of variables in the legend
            // Text Height here refers to the legend for colors (the drawn filled rectangle)
            if((ChGetShowSeriesCount(pCh) == 1) || (GetState(pCh, CH_LEGEND) != CH_LEGEND))
                temp = 0;
            else
                temp = GetLongestNameLength(pCh) + GetTextHeight(pCh->hdr.pGolScheme->pFont);

            // get sample delta (space between data) and value delta (defines the grid for the value)
            // check first if we compute in the x-axis or y-axis
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {

                // if horizontally drawn sample delta is not affected by the legend
                splDelta = (yStart - (pCh->hdr.top + CH_MARGIN + GetTextHeight(pCh->prm.pTitleFont)));

                // adjust space for displayed values
                if(GetState(pCh, CH_VALUE))
                {
                    temp += (GetTextWidth(tempXchar, pCh->hdr.pGolScheme->pFont) * 4);
                }

                // get the value delta,
                valDelta = (pCh->hdr.right - xStart - CH_MARGIN - temp);
            }
            else
            {
                if(GetState(pCh, CH_LEGEND) && (ChGetShowSeriesCount(pCh) != 1))
                {
                    splDelta =
                        (
                            pCh->hdr.right -
                            xStart -
                            ((CH_MARGIN << 2) + temp + GetTextHeight(pCh->hdr.pGolScheme->pFont))
                        );
                }
                else
                {
                    splDelta = (pCh->hdr.right - xStart - (CH_MARGIN << 2));
                }

                // get the value delta
                valDelta =
                    (
                        yStart -
                        (pCh->hdr.top + CH_MARGIN + GetTextHeight(pCh->prm.pTitleFont) + GetTextHeight(pCh->hdr.pGolScheme->pFont))
                    );
            }

            // adjust the splDelta for 3D effects, 12 here is the maximum depth of a bar for the 3D effect
            if(GetState(pCh, CH_3D_ENABLE))
            {
                splDelta -= 12;
                valDelta -= 12;
            }

            // get the final splDelta value by checking the number of samples to display		
            splDelta /= (ChGetSampleRange(pCh) + 1);

            // get the final valDelta value by checking the number of samples to display		
            valDelta /= (CH_YGRIDCOUNT - 1);

            // initilize the counter for the sample axis drawing
            temp = ChGetSampleRange(pCh) + 2;
            x = xStart;
            y = yStart;
            state = SAMPLE_GRID_DRAW1;

        case SAMPLE_GRID_DRAW1:

            // draw the small grids on the x-axis
            while(temp)
            {
                SetColor(pCh->hdr.pGolScheme->Color0);
                if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                {
                    if(!Bar(x, y, x + 3, y + 1))
                        return (0);
                    y -= splDelta;
                }
                else
                {
                    if(!Bar(x, y, x + 1, y + 3))
                        return (0);
                    x += splDelta;
                }

                --temp;
            }

            // get the bar width (bar here refers to the sample data represented as bars, where the height
            // of the bar represents the value of the sample)
            barWidth = splDelta / (2 + ChGetShowSeriesCount(pCh));

            temp = CH_YGRIDCOUNT;
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                y = yStart;
            else
                x = xStart;

            if(GetState(pCh, CH_3D_ENABLE))
            {

                // limit the 3-D depth to only 12 pixels.
                chart3DDepth = (barWidth > 12) ? 12 : barWidth;
                chart3DDepth = chart3DDepth >> 1;

                // set the bar 3-D depth.
                barDepth = chart3DDepth;
                state = VALUE_GRID_3D_DRAW;
            }
            else
            {
                state = VALUE_GRID_DRAW1;
                goto chrt_value_grid_draw1;
            }

        case VALUE_GRID_3D_DRAW:

            // draw the 3D grids on the value-axis
            while(temp)
            {
                SetColor(pCh->hdr.pGolScheme->Color0);
                if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                {
                    if
                    (
                        !Bar
                            (
                                x + (chart3DDepth),
                                y - (chart3DDepth) - (splDelta * (ChGetSampleRange(pCh) + 1)),
                                x + (chart3DDepth),
                                y - (chart3DDepth)
                            )
                    ) return (0);
                    x += valDelta;
                }
                else
                {
                    if
                    (
                        !Bar
                            (
                                x + (chart3DDepth),
                                y - (chart3DDepth),
                                x + (chart3DDepth) + (splDelta * (ChGetSampleRange(pCh) + 1)),
                                y - (chart3DDepth)
                            )
                    ) return (0);
                    y -= valDelta;
                }

                --temp;
            }

            temp = CH_YGRIDCOUNT;
            x = xStart;
            y = yStart;
            state = VALUE_GRID_DRAW1;

    chrt_value_grid_draw1:

        case VALUE_GRID_DRAW1:

            // draw the grids on the y-axis
            if(GetState(pCh, CH_3D_ENABLE))
            {

                // just draw the first one to define the x-axis
                SetColor(pCh->hdr.pGolScheme->Color0);
                if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                {
                    if(!Bar(x, y - (splDelta * (ChGetSampleRange(pCh) + 1)), x, y))
                        return (0);
                }
                else
                {
                    if(!Bar(x, y, x + (splDelta * (ChGetSampleRange(pCh) + 1)), y))
                        return (0);
                }
            }
            else
            {
                while(temp)
                {
                    SetColor(pCh->hdr.pGolScheme->Color0);
                    if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                    {
                        if(!Bar(x, y - (splDelta * (ChGetSampleRange(pCh) + 1)), x, y))
                            return (0);
                        x += valDelta;
                    }
                    else
                    {
                        if(!Bar(x, y, x + (splDelta * (ChGetSampleRange(pCh) + 1)), y))
                            return (0);
                        y -= valDelta;
                    }

                    --temp;
                }
            }

            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                x = xStart;
            }
            else
            {
                y = yStart;
            }

            if(GetState(pCh, CH_3D_ENABLE))
            {
                temp = CH_YGRIDCOUNT + 1;
                state = VALUE_GRID_DRAW2;
            }
            else
            {
                temp = 2;
                state = SAMPLE_GRID_DRAW2;
                goto chrt_xgrid_draw2;
            }

        case VALUE_GRID_DRAW2:

            // draw the 3-D effect on the y axis of the chart
            SetColor(pCh->hdr.pGolScheme->Color0);
            while(temp)
            {
                if(temp == (CH_YGRIDCOUNT + 1))
                {
                    if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                    {
                        if
                        (
                            !Line
                                (
                                    x,
                                    y - (splDelta * (ChGetSampleRange(pCh) + 1)),
                                    x + chart3DDepth,
                                    y - (splDelta * (ChGetSampleRange(pCh) + 1)) - chart3DDepth
                                )
                        ) return (0);
                    }
                    else
                    {
                        if
                        (
                            !Line
                                (
                                    x + (splDelta * (ChGetSampleRange(pCh) + 1)),
                                    y,
                                    x + (chart3DDepth) + (splDelta * (ChGetSampleRange(pCh) + 1)),
                                    y - chart3DDepth
                                )
                        ) return (0);
                    }

                    --temp;
                    continue;
                }
                else if(!Line(x, y, x + chart3DDepth, y - chart3DDepth))
                    return (0);

                if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                {
                    x += valDelta;
                }
                else
                {
                    y -= valDelta;
                }

                --temp;
            }

            temp = 3;
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                x = xStart;
            }
            else
            {
                y = yStart;
            }

            state = SAMPLE_GRID_DRAW2;

    chrt_xgrid_draw2:

        case SAMPLE_GRID_DRAW2:

            // draw the left and right edges of the chart
            while(temp)
            {
                if(GetState(pCh, CH_3D_ENABLE))
                {
                    if(temp == 3)
                    {
                        if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                        {
                            if(!Bar(x, y, x + ((CH_YGRIDCOUNT - 1) * valDelta), y))
                                return (0);
                        }
                        else
                        {
                            if(!Bar(x, y - ((CH_YGRIDCOUNT - 1) * valDelta), x, y))
                                return (0);
                        }

                        --temp;
                        continue;
                    }
                    else if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                    {
                        if
                        (
                            !Bar
                                (
                                    x + chart3DDepth,
                                    y - chart3DDepth,
                                    x + chart3DDepth + ((CH_YGRIDCOUNT - 1) * valDelta),
                                    y - chart3DDepth
                                )
                        ) return (0);
                    }
                    else
                    {
                        if
                        (
                            !Bar
                                (
                                    x + chart3DDepth,
                                    y - ((CH_YGRIDCOUNT - 1) * valDelta) - chart3DDepth,
                                    x + chart3DDepth,
                                    y - chart3DDepth
                                )
                        ) return (0);
                    }
                }
                else
                {
                    if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                    {
                        if(!Bar(x, y, x + ((CH_YGRIDCOUNT - 1) * valDelta), y))
                            return (0);
                    }
                    else
                    {
                        if(!Bar(x, y - ((CH_YGRIDCOUNT - 1) * valDelta), x, y))
                            return (0);
                    }
                }

                if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                {
                    y -= (splDelta * (ChGetSampleRange(pCh) + 1));
                }
                else
                {
                    x += (splDelta * (ChGetSampleRange(pCh) + 1));
                }

                --temp;
            }

            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                    #ifdef USE_HORZ_ASCENDING_ORDER
                y = yStart - (ChGetSampleRange(pCh) * splDelta);
                #else
            y = yStart;
                #endif
            else
                x = xStart;
            ctr = ChGetSampleStart(pCh);

            state = SAMPLE_LABEL_DRAW_SET;

            /*========================================================================*/
            //					  Draw the Sample Grid labels
            /*========================================================================*/
    chrt_sample_label_draw_set:

        case SAMPLE_LABEL_DRAW_SET:

            // for data only redraw, we need to refresh the x-axis labels to indicate
            // the correct sample points being shown
            SetFont(pCh->prm.pGridLabelsFont);

            if(GetState(pCh, CH_NUMERIC))
            {
                j = word2xchar(ctr, tempStr, STR_CHAR_CNT);
            }
            else
            {

                // note that we will only have A-Z labels.
                tempStr[STR_CHAR_CNT - 1] = 'A' + (ctr - 1);
                j = 1;
            }

            temp = GetTextWidth((&tempStr[STR_CHAR_CNT - j]), pCh->prm.pGridLabelsFont);
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                MoveTo(x - temp, y - ((splDelta + GetTextHeight(pCh->prm.pGridLabelsFont)) >> 1));
            }
            else
            {
                MoveTo(x + ((splDelta - temp) >> 1), y);
            }

            state = SAMPLE_LABEL_DRAW_RUN;

        case SAMPLE_LABEL_DRAW_RUN:

            // draw the x axis grid numbers
            SetColor(pCh->hdr.pGolScheme->TextColor0);
            if(!OutText(&tempStr[STR_CHAR_CNT - j]))
                return (0);
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                    #ifdef USE_HORZ_ASCENDING_ORDER
                y += splDelta;
                    #else
                y -= splDelta;
                    #endif
            }
            else
            {
                x += splDelta;
            }

            ctr++;
            if(ctr > ChGetSampleEnd(pCh))
            {

                // check if we only need to redraw the data
                if(GetState(pCh, CH_DRAW_DATA))
                {
                    state = DATA_DRAW_INIT;
                    goto chrt_data_draw_init;
                }
                else
                {
                    if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                    {
                        state = YAXIS_LABEL_DRAW_SET;
                        goto chrt_yaxis_label_draw_set;
                    }
                    else
                    {
                        state = XAXIS_LABEL_DRAW_SET;
                    }
                }
            }
            else
            {
                temp = x;
                goto chrt_sample_label_draw_set;
            }

            /*========================================================================*/
            //					  Draw the X - Axis labels
            /*========================================================================*/
    chrt_xaxis_label_draw_set:

        case XAXIS_LABEL_DRAW_SET:

            // find the location of the label
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                pXcharTemp = pCh->prm.pValLabel;
                uLocator = valDelta * (CH_YGRIDCOUNT - 1);
            }
            else
            {
                pXcharTemp = pCh->prm.pSmplLabel;
                uLocator = splDelta * (ChGetSampleRange(pCh) + 1);
            }

            temp = GetTextWidth(pXcharTemp, pCh->prm.pAxisLabelsFont);

            if(temp > uLocator)
                temp = xStart;
            else
                temp = xStart + ((uLocator - temp) >> 1);

            MoveTo(temp, yStart + GetTextHeight(pCh->prm.pGridLabelsFont));
            state = XAXIS_LABEL_DRAW_RUN;

        case XAXIS_LABEL_DRAW_RUN:
            SetFont(pCh->prm.pAxisLabelsFont);

            // enable clipping and set region
            SetClip(CLIP_ENABLE);
            SetClipRgn(pCh->hdr.left, pCh->hdr.top, pCh->hdr.right, pCh->hdr.bottom);

            SetColor(pCh->hdr.pGolScheme->TextColor1);
            if(!OutText(pXcharTemp))
                return (0);

            SetClip(CLIP_DISABLE);
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                state = DATA_DRAW_INIT;
                goto chrt_data_draw_init;
            }
            else
            {
                state = VALUE_LABEL_DRAW_INIT;
            }

    chrt_value_label_draw_init:

        case VALUE_LABEL_DRAW_INIT:
            ctr = 0;

            // x is used here to represent change in value grid labels
            // Note that we add a multiplier of 10 to compute for round off errors.
            // It will not be perfect but approximations is better unless you work with
            // figures divisible by 5.
            if(GetState(pCh, CH_PERCENT))
            {

                // Scaling of the labels is included here
                x = ChGetPercentRange(pCh) * 100 / (CH_YGRIDCOUNT - 1);
            }
            else
            {
                x = (ChGetValueRange(pCh) * 100) / (CH_YGRIDCOUNT - 1);
            }

            // compute for round off error, the adjustment for the factor 100 is done in
            // conversion of the integer to string below.
            if((x % 10) < 5)
                x += 10;

            state = VALUE_LABEL_DRAW_SET;

            /*========================================================================*/
            //					  Draw the Value Grid labels
            /*========================================================================*/
    chrt_value_label_draw_set:

        case VALUE_LABEL_DRAW_SET:

            // note that the adjustment of the 100 factor is done here.
            if(GetState(pCh, CH_PERCENT))
            {

                // add the percent sign on the label
                tempStr[STR_CHAR_CNT - 1] = '%';

                // we have a plus 1 here since we add '%' sign already
                j = 1 + word2xchar((ctr * x / 100) + ChGetPercentMin(pCh), tempStr, STR_CHAR_CNT - 1);
            }
            else
            {
                j = word2xchar((ctr * x / 100) + ChGetValueMin(pCh), tempStr, STR_CHAR_CNT);
            }

            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                temp = GetTextWidth((&tempStr[STR_CHAR_CNT - j]), pCh->prm.pGridLabelsFont);

                MoveTo(xStart + ((valDelta * ctr) - (temp >> 1)), yStart);
            }
            else
            {
                temp = GetTextHeight(pCh->prm.pGridLabelsFont);
                MoveTo
                (
                    xStart - GetTextWidth((&tempStr[STR_CHAR_CNT - j]), pCh->prm.pGridLabelsFont),
                    yStart - ((valDelta * ctr) + (temp >> 1))
                );
            }

            SetFont(pCh->prm.pGridLabelsFont);
            state = VALUE_LABEL_DRAW_RUN;

        case VALUE_LABEL_DRAW_RUN:

            // draw the y axis grid numbers
            SetColor(pCh->hdr.pGolScheme->TextColor0);
            if(!OutText(&tempStr[STR_CHAR_CNT - j]))
                return (0);
            ctr++;
            if(ctr >= CH_YGRIDCOUNT)
            {
                state = XAXIS_LABEL_DRAW_SET;
            }
            else
            {
                goto chrt_value_label_draw_set;
            }

            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                state = XAXIS_LABEL_DRAW_SET;
                goto chrt_xaxis_label_draw_set;
            }
            else
            {
                state = YAXIS_LABEL_DRAW_SET;
            }

            /*========================================================================*/
            //					  Draw the Y - Axis labels
            /*========================================================================*/
    chrt_yaxis_label_draw_set:

        case YAXIS_LABEL_DRAW_SET:

            // find the location of the label
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                pXcharTemp = pCh->prm.pSmplLabel;
                uLocator = splDelta * (ChGetSampleRange(pCh) + 1);
            }
            else
            {
                pXcharTemp = pCh->prm.pValLabel;
                uLocator = valDelta * CH_YGRIDCOUNT;
            }

            temp = GetTextWidth(pXcharTemp, pCh->prm.pAxisLabelsFont);

            if(temp > uLocator)
                temp = (pCh->hdr.bottom + pCh->hdr.top + temp) >> 1;
            else
            {
                temp = yStart - ((uLocator - temp) >> 1);
            }

            MoveTo
            (
                xStart - GetTextWidth((&tempStr[STR_CHAR_CNT - j]), pCh->prm.pGridLabelsFont) - (GetTextWidth(temp2Xchar, pCh->prm.pGridLabelsFont) >> 1) - GetTextHeight(pCh->prm.pAxisLabelsFont),
                temp
            );

            state = YAXIS_LABEL_DRAW_RUN;

        case YAXIS_LABEL_DRAW_RUN:
            SetFont(pCh->prm.pAxisLabelsFont);

            // enable clipping and set region
            SetClip(CLIP_ENABLE);
            SetClipRgn(pCh->hdr.left, pCh->hdr.top, pCh->hdr.right, pCh->hdr.bottom);

            // set the orientation of text to vertical
            SetFontOrientation(ORIENT_VER);
            SetColor(pCh->hdr.pGolScheme->TextColor1);
            if(!OutText(pXcharTemp))
                return (0);

            SetClip(CLIP_DISABLE);

            // place back the orientation of text to horizontal
            SetFontOrientation(ORIENT_HOR);

            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                state = VALUE_LABEL_DRAW_INIT;
                goto chrt_value_label_draw_init;
            }
            else
            {
                state = DATA_DRAW_INIT;
            }

            /*========================================================================*/
            //					  Draw the bars representing the data/samples
            /*========================================================================*/
    chrt_data_draw_init:

        case DATA_DRAW_INIT:

            /* pSmple - points to the sample data of the variables
			   ctr - the sample counter
			   varCtr - variable counter
			   temp - the width of the bars
			   dTemp - the height of the bars
			   x or y - the location of the first bar per sample. For single variables
			       there will be only one bar per sample. x for vertical bars and y for horizontal bars.
			*/
            ctr = 0;
            temp = splDelta / (2 + ChGetShowSeriesCount(pCh));  // <---- note this! this can be used to calculate the minimum size limit of the chart
            state = DATA_DRAW_SET;

    chrt_data_draw_set:

        case DATA_DRAW_SET:
            varCtr = 0;

            pVar = ChGetNextShowData(pCh->pChData);

            // set the position to start bar drawing
            // x and y here are used in horizontal drawing and vertical drawing as the variable
            // that refers to the position of the bar being drawn.
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                    #ifdef USE_HORZ_ASCENDING_ORDER
                y = yStart - temp - ((ChGetSampleRange(pCh) - ctr) * splDelta);
                    #else
                y = yStart - (ctr * splDelta) - temp;
                    #endif
            }
            else
            {
                x = xStart + (ctr * splDelta) + temp;
            }

            state = BAR_DATA_DRAW;

    chrt_data_draw:

        case BAR_DATA_DRAW:

            // get sample data from current variable
            pSmple = (&(*pVar->pData) + (ctr + ChGetSampleStart(pCh) - 1));

            // calculate the total value of the samples to compute the percentages
            if(GetState(pCh, CH_PERCENT))
            {
                j = ChGetSampleStart(pCh);
                samplesMax = 0;

                while(j <= (ChGetSampleRange(pCh) + ChGetSampleStart(pCh)))
                {
                    samplesMax += (*(&(*pVar->pData) + (j - 1)));
                    j++;
                }

                // Get the percentage of the sample
                dTemp = ((DWORD) (*pSmple) * 100) / samplesMax;

                // check if scaling is needed				
                if(ChGetPercentMax(pCh) <= dTemp)
                    dTemp = ChGetPercentRange(pCh);
                else
                {
                    if(dTemp < ChGetPercentMin(pCh))
                        dTemp = 0;
                    else
                        dTemp = dTemp - ChGetPercentMin(pCh);
                }

                dTemp = ((DWORD) (dTemp) * (valDelta * (CH_YGRIDCOUNT - 1))) / (ChGetPercentRange(pCh));
            }
            else
            {

                // get the height of the current bar to draw
                // this should be adjusted to the min and max set values
                if(ChGetValueMax(pCh) <= (*pSmple))
                {
                    dTemp = ChGetValueRange(pCh);
                }
                else
                {
                    if((*pSmple) < ChGetValueMin(pCh))
                        dTemp = 0;
                    else
                        dTemp = (*pSmple) - ChGetValueMin(pCh);
                }

                dTemp = ((DWORD) (dTemp) * (valDelta * (CH_YGRIDCOUNT - 1)) / ChGetValueRange(pCh));
            }

            // draw the front side of the bar
            if(ChGetShowSeriesCount(pCh) > 1)
            {
                SetColor(*(&(*pCh->prm.pColor) + varCtr));
            }
            else
            {
                SetColor(*(&(*pCh->prm.pColor) + ctr));
            }

            if(GetState(pCh, CH_3D_ENABLE))
            {
                if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                {
                    if(!Bar(xStart, y - (temp * (varCtr + 1)), xStart + dTemp, y - (temp * varCtr)))
                        return (0);
                }
                else
                {
                    if(!Bar(x + 1 + 1 + (temp * varCtr), yStart - dTemp, x + (temp * (varCtr + 1)), yStart))
                        return (0);
                }

                state = BAR_DATA_DRAW_3D_PREP;
            }
            else
            {
                if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                {
                    if(!Bar(xStart, y - (temp * (varCtr + 1)), xStart + dTemp, y - 1 - (temp * varCtr)))
                        return (0);
                }
                else
                {
                    if(!Bar(x + 1 + (temp * varCtr), yStart - dTemp, x + (temp * (varCtr + 1)), yStart))
                        return (0);
                }

                if((GetState(pCh, CH_VALUE)) || (GetState(pCh, CH_PERCENT)))
                {
                    state = BAR_DATA_DRAW_VALUE;
                    goto chrt_bar_data_draw_value;
                }
                else
                {
                    state = BAR_DATA_DRAW_CHECK;
                    goto chrt_bar_data_draw_check;
                }
            }

        case BAR_DATA_DRAW_3D_PREP:

            // draw the 3-D component
            // draw the 45 degree lines
            // we will use y here as the variable to move the line drawn
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                z = y;
                x = xStart + dTemp;
            }
            else
            {
                z = x + 1;
                y = yStart - dTemp;
            }

            state = BAR_DATA_DRAW_3D_LOOP_1;

    chrt_bar_data_draw_3d_loop_1:

        case BAR_DATA_DRAW_3D_LOOP_1:
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                if(x <= xStart + dTemp + barDepth)
                {
                    if((x == (xStart + dTemp)) || (x == (xStart + dTemp + barDepth)))
                    {
                        SetColor(BLACK);
                    }
                    else
                    {
                        if(ChGetShowSeriesCount(pCh) > 1)
                        {
                            SetColor(GetColorShade(*(&(*pCh->prm.pColor) + varCtr), 2));
                        }
                        else
                        {
                            SetColor(GetColorShade(*(&(*pCh->prm.pColor) + ctr), 2));
                        }
                    }

                    if(!Bar(x, z - (temp * (varCtr + 1)), x, z - (temp * varCtr)))
                        return (0);

                    state = BAR_DATA_DRAW_3D_LOOP_2;
                }
                else
                {
                    state = BAR_DATA_DRAW_3D_OUTLINE1;
                    goto chrt_bar_data_draw_3d_outline_1;
                }
            }
            else
            {
                if(y >= yStart - dTemp - barDepth)
                {
                    if((y == (yStart - dTemp)) || (y == (yStart - dTemp - barDepth)))
                    {
                        SetColor(BLACK);
                    }
                    else
                    {
                        if(ChGetShowSeriesCount(pCh) > 1)
                        {
                            SetColor(GetColorShade(*(&(*pCh->prm.pColor) + varCtr), 2));
                        }
                        else
                        {
                            SetColor(GetColorShade(*(&(*pCh->prm.pColor) + ctr), 2));
                        }
                    }

                    if(!Bar(z + 1 + (temp * varCtr), y, z - 1 + (temp * (varCtr + 1)), y))
                        return (0);

                    state = BAR_DATA_DRAW_3D_LOOP_2;
                }
                else
                {
                    state = BAR_DATA_DRAW_3D_OUTLINE1;
                    goto chrt_bar_data_draw_3d_outline_1;
                }
            }

        case BAR_DATA_DRAW_3D_LOOP_2:

            // check if we are going to draw the outline or the shade
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                if((x == (xStart + dTemp)) || (x == (xStart + dTemp + barDepth)))
                {
                    SetColor(BLACK);
                }
                else
                {
                    if(ChGetShowSeriesCount(pCh) > 1)
                    {
                        SetColor(GetColorShade(*(&(*pCh->prm.pColor) + varCtr), 1));
                    }
                    else
                    {
                        SetColor(GetColorShade(*(&(*pCh->prm.pColor) + ctr), 1));
                    }
                }
            }
            else
            {
                if((y == (yStart - dTemp)) || (y == (yStart - dTemp - barDepth)))
                {
                    SetColor(BLACK);
                }
                else
                {
                    if(ChGetShowSeriesCount(pCh) > 1)
                    {
                        SetColor(GetColorShade(*(&(*pCh->prm.pColor) + varCtr), 1));
                    }
                    else
                    {
                        SetColor(GetColorShade(*(&(*pCh->prm.pColor) + ctr), 1));
                    }
                }
            }

            // draw the outline or shade
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                if(!Bar(x - dTemp, z - (temp * (varCtr + 1)), x, z - (temp * (varCtr + 1))))
                    return (0);
            }
            else
            {
                if(!Bar(z + (temp * (varCtr + 1)), y, z + (temp * (varCtr + 1)), y + dTemp))
                    return (0);
            }

            // update the loop variables
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                x++;
                z--;
            }
            else
            {
                y--;
                z++;
            }

            state = BAR_DATA_DRAW_3D_LOOP_1;
            goto chrt_bar_data_draw_3d_loop_1;

    chrt_bar_data_draw_3d_outline_1:

        case BAR_DATA_DRAW_3D_OUTLINE1:
            SetColor(BLACK);

            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                if(!Line(x - 1, y - (temp * varCtr) - barDepth, x - 1 - barDepth, y - (temp * varCtr)))
                    return (0);
            }
            else
            {
                if
                (
                    !Line
                        (
                            x + 1 + (temp * varCtr),
                            yStart - dTemp,
                            x + 1 + (temp * varCtr) + barDepth,
                            yStart - dTemp - barDepth
                        )
                ) return (0);
            }

            state = BAR_DATA_DRAW_3D_OUTLINE2;

        case BAR_DATA_DRAW_3D_OUTLINE2:
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                if(!Line(x - 1, y - (temp * (varCtr + 1)) - barDepth, x - 1 - barDepth, y - (temp * (varCtr + 1))))
                    return (0);
            }
            else
            {
                if
                (
                    !Line
                        (
                            x + 1 + (temp * (varCtr + 1)),
                            (yStart - dTemp),
                            x + 1 + (temp * (varCtr + 1)) + barDepth,
                            (yStart - dTemp) - barDepth
                        )
                ) return (0);
            }

            state = BAR_DATA_DRAW_3D_OUTLINE3;

        case BAR_DATA_DRAW_3D_OUTLINE3:
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                if
                (
                    !Line
                        (
                            x - dTemp - 1,
                            y - (temp * (varCtr + 1)) - barDepth,
                            x - dTemp - barDepth - 1,
                            y - (temp * (varCtr + 1))
                        )
                ) return (0);
            }
            else
            {
                if
                (
                    !Line
                        (
                            x + 1 + (temp * (varCtr + 1)),
                            yStart,
                            x + 1 + (temp * (varCtr + 1)) + barDepth,
                            yStart - barDepth
                        )
                ) return (0);
            }

            state = BAR_DATA_DRAW_3D_OUTLINE4;

        case BAR_DATA_DRAW_3D_OUTLINE4:
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                if
                (
                    !Bar
                        (
                            x - dTemp - barDepth - 1,
                            y - (temp * (varCtr + 1)),
                            x - dTemp - barDepth - 1,
                            y - (temp * (varCtr + 1)) + temp
                        )
                ) return (0);
            }
            else
            {
                if(!Bar(x + 1 + (temp * varCtr), yStart - dTemp, x + 1 + (temp * varCtr), yStart))
                    return (0);
            }

            state = BAR_DATA_DRAW_3D_OUTLINE5;

        case BAR_DATA_DRAW_3D_OUTLINE5:

            // draw the horizontal lines
            if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
            {
                if
                (
                    !Bar
                        (
                            x - dTemp - barDepth - 1,
                            y - (temp * (varCtr + 1)) + temp,
                            x - barDepth - 1,
                            y - (temp * (varCtr + 1)) + temp
                        )
                ) return (0);
            }
            else
            {
                if(!Bar(x + 1 + (temp * varCtr), yStart, x + 1 + (temp * (varCtr + 1)), yStart))
                    return (0);
            }

            if((GetState(pCh, CH_VALUE)) || (GetState(pCh, CH_PERCENT)))
                state = BAR_DATA_DRAW_VALUE;
            else
            {
                state = BAR_DATA_DRAW_CHECK;
                goto chrt_bar_data_draw_check;
            }

    chrt_bar_data_draw_value:

        case BAR_DATA_DRAW_VALUE:
            if(GetState(pCh, CH_VALUE))
            {
                j = word2xchar(*pSmple, tempStr, STR_CHAR_CNT);
            }
            else
            {

                // add the percent sign on the label
                tempStr[STR_CHAR_CNT - 1] = '%';

                // compute for the percentage
                if(ChGetValueMax(pCh) <= (*pSmple))
                    dPercent = ChGetValueRange(pCh);
                else
                {
                    if((*pSmple) < ChGetValueMin(pCh))
                        dPercent = 0;
                    else
                        dPercent = (*pSmple) - ChGetValueMin(pCh);
                }

                dPercent = ((DWORD) (dPercent * 1000)) / samplesMax;

                // check if we need to round up or not
                if((dPercent % 10) < 5)
                    dPercent = (dPercent / 10);                 // do not round up to next number
                else
                    dPercent = (dPercent / 10) + 1;             // round up the value

                // we have a plus 1 here since we add '%' sign already
                j = 1 + word2xchar(dPercent, tempStr, STR_CHAR_CNT - 1);
            }

            if(GetState(pCh, CH_3D_ENABLE))
            {
                if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                {
                    MoveTo
                    (
                        xStart + dTemp + barDepth + 3,
                        y - (temp * varCtr) - ((temp + GetTextHeight(pCh->hdr.pGolScheme->pFont)) >> 1) - barDepth
                    );
                }
                else
                {
                    MoveTo
                    (
                        x + barDepth + (temp * varCtr) + (temp >> 1) - (GetTextWidth(&tempStr[STR_CHAR_CNT - j], pCh->hdr.pGolScheme->pFont) >> 1),
                        (yStart - 1) - dTemp - GetTextHeight(pCh->hdr.pGolScheme->pFont) - (barDepth)
                    );
                }
            }
            else
            {
                if(GetState(pCh, CH_BAR_HOR) == CH_BAR_HOR)
                {
                    MoveTo
                    (
                        xStart + dTemp + 3,
                        y - (temp * varCtr) - (temp >> 1) - (GetTextHeight(pCh->hdr.pGolScheme->pFont) >> 1)
                    );
                }
                else
                {
                    MoveTo
                    (
                        x +
                            ((temp >> 1) - (GetTextWidth(&tempStr[STR_CHAR_CNT - j], pCh->hdr.pGolScheme->pFont) >> 1)) +
                            (temp * varCtr),
                        (yStart - 1) - dTemp - (GetTextHeight(pCh->hdr.pGolScheme->pFont))
                    );
                }
            }

            state = BAR_DATA_DRAW_VALUE_RUN;

        case BAR_DATA_DRAW_VALUE_RUN:

            // draw the values on top of the bars
            // NOTE: we use the emboss light color here to draw the values.
            SetColor(pCh->hdr.pGolScheme->EmbossLtColor);
            SetFont(pCh->hdr.pGolScheme->pFont);
            if(!OutText(&tempStr[STR_CHAR_CNT - j]))
                return (0);
            state = BAR_DATA_DRAW_CHECK;

    chrt_bar_data_draw_check:

        case BAR_DATA_DRAW_CHECK:

            // find the next data series that will be shown
            pVar = ChGetNextShowData(pVar);
            if(pVar == NULL)
            {
                state = REMOVE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pCh->hdr.left,
                                                pCh->hdr.top,
                                                pCh->hdr.right,
                                                pCh->hdr.bottom);
#endif
                return (1);
            }

            // increment the variable counter
            varCtr++;
            if(varCtr < ChGetShowSeriesCount(pCh))
            {
                pVar = (DATASERIES *)pVar->pNextData;
                state = BAR_DATA_DRAW;
                goto chrt_data_draw;
            }

            // increment the sample counter
            ctr++;
            if(ctr < ChGetSampleRange(pCh) + 1)
            {
                x += (splDelta + 1);
                state = DATA_DRAW_SET;
                goto chrt_data_draw_set;
            }

            state = REMOVE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
            GFX_DRIVER_CompleteDrawUpdate(   pCh->hdr.left,
                                            pCh->hdr.top,
                                            pCh->hdr.right,
                                            pCh->hdr.bottom);
#endif
            return (1);

            /**************************************************************************/
            // 					PIE CHART states
            /**************************************************************************/

            /*========================================================================*/
            //					  Draw the pie
            /*========================================================================*/
    chrt_pie_prep:

        case PIE_PREP:

            /* If more than two data series have their SHOW_DATA flag set
        	   the pie chart is drawn to represent the values of each variable 
        	   at Start sample point . End sample point is ignored in this case.
        	   If only one data series is set to be shown the pie chart is drawn
        	   from the sample start point and sample end point (inclusive). 

        	   Pie chart is drawn as a percentage of the data samples.
        	   Therefore: percentage is computed depending on the sample
        	   points used.
        	   Only 1 data series is set to be shown
        	   	 total = 	summation of the variable sample points from
        	   				sample start point to sample end point inclusive.
        	   More than 1 data series is set to be shown
        	   	 total = 	summation of the sample points for each variable
        	   				using the sample start point.
			*/

            /* For PIE chart variables used are the following
				ctr = x position of the center of the pie chart
				ctry = y position of the center of the pie chart
				z   = radius of the pie chart
				j	= start angle 
				k   = end angle
			*/

            // calculate the needed variables
            // radius z is affected by the following: CH_LEGEND, CH_VALUE and CH_PERCENT
            temp = CH_MARGIN << 1;

            // check the largest/longest sample
            // use x here as a counter
            varCtr = ChGetShowSeriesCount(pCh);
            if(varCtr == 1)
            {
                varCtr = ChGetSampleRange(pCh) + 1;
            }

            pVar = ChGetNextShowData(pCh->pChData);

            // y and z is used here as a temporary variable
            y = 0;

            // find the sample value that is largest (place in y)
            // also while doing this get the total of the selected data (put in dTemp)
            dTemp = 0;
            while(varCtr >= 0)
            {
                if(ChGetShowSeriesCount(pCh) > 1)
                {
                    z = *(&(*pVar->pData) + ChGetSampleStart(pCh) - 1);
                }
                else
                {
                    z = *(&(*pVar->pData) + ChGetSampleEnd(pCh) - varCtr);
                }

                dTemp += z;
                varCtr--;

                // check if we get a larger value
                if(z > y)
                    y = z;
                if(varCtr == 0)
                    break;

                if(ChGetShowSeriesCount(pCh) > 1)
                {
                    pVar = ChGetNextShowData((DATASERIES *)pVar->pNextData);
                }

                // this is insurance (in case the link list is corrupted)
                if(pVar == NULL)
                {
                    break;
                }
            }

            // initialize pVarFont to use pGridLabelsFont			
            pVarFont = pCh->prm.pGridLabelsFont;

            // get the space occupied by the value
            if(GetState(pCh, CH_VALUE))
            {
                z = word2xchar(y, tempStr, STR_CHAR_CNT);
                x = (GetTextWidth(&tempStr[STR_CHAR_CNT - z], pVarFont));
            }
            else
                x = 0;

            // get the space occupied by percent value
            if(GetState(pCh, CH_PERCENT))
            {

                // 7 is derived from "X:100%," - seven possible characters, 1 is added for buffer
                y = (8 * GetTextWidth((XCHAR *)tempXchar, pVarFont));
            }
            else
                y = 0;

            // get the space occupied by the legend
            if((GetState(pCh, CH_LEGEND)) && (ChGetShowSeriesCount(pCh) > 1))
                temp += ((GetLongestNameLength(pCh) + (GetTextHeight(pCh->hdr.pGolScheme->pFont) >> 1)));

            // calculate the center of the pie chart
            ctr = (pCh->hdr.left + pCh->hdr.right - temp) >> 1;
            ctry = ((pCh->hdr.bottom + (pCh->hdr.top + GetTextHeight(pVarFont))) >> 1) + CH_MARGIN;

            // radius size is checked against the x and y area
            if
            (
                ((pCh->hdr.right - pCh->hdr.left) - temp - ((x + y) << 1)) <
                    (
                        (pCh->hdr.bottom - pCh->hdr.top - GetTextHeight(pCh->prm.pTitleFont)) - temp -
                            (GetTextHeight(pVarFont) << 1)
                    )
            )
            {

                // use dimension in the x axis
                if(x + y)
                {
                    z = ctr - (pCh->hdr.left + (x + y) + CH_MARGIN);
                }
                else
                {
                    z = ctr - pCh->hdr.left + CH_MARGIN;
                }
            }
            else
            {

                // use dimension in the y axis
                if(x + y)
                    z = ctry -
                        (
                            pCh->hdr.top + (CH_MARGIN << 1) + GetTextHeight(pCh->prm.pTitleFont) +
                                (GetTextHeight(pVarFont) << 1)
                        );
                else
                    z = ctry - (pCh->hdr.top + (CH_MARGIN << 1) + (GetTextHeight(pCh->prm.pTitleFont) << 1));
            }

            state = PIE_DRAW_OUTLINE1;

        case PIE_DRAW_OUTLINE1:

            // Required items before the pie chart can be drawn
            SetColor(LIGHTGRAY);

            // Draw pie-chart outline
            if(!Circle(ctr, ctry, z))
                return (0);
            state = PIE_DRAW_SECTOR;

        /*========================================================================*/
        //					  Draw the sectors of the pie
        /*========================================================================*/
        case PIE_DRAW_SECTOR:

            // now we are ready to draw the sectors
            // calculate the sector that a value will need
            k = 0;

            // check if more than one data series set to be shown , draw the pie chart of
            // the data series that are set to be shown.
            pVar = ChGetNextShowData(pCh->pChData);
            y = dTemp;
            varCtr = ChGetShowSeriesCount(pCh);
            if(varCtr == 1)
            {
                varCtr = ChGetSampleRange(pCh) + 1;
            }
            else if(varCtr == 0)
            {
                pVar = NULL;
                y = 0;
            }

            // we start at 0 degree.
            j = 0;

            // initialize the variables that position the pie sector labels.
            // this is used to minimize the occurrence of overlapped labels.
            pieLabelYPos = ctry;
            pieLabelYPos2 = pCh->hdr.bottom - (GOL_EMBOSS_SIZE + 1) - GetTextHeight(pVarFont);
            pieLabelYPos3 = pCh->hdr.top + GOL_EMBOSS_SIZE + CH_MARGIN + GetTextHeight(pCh->prm.pTitleFont) + GetTextHeight(pVarFont);

            state = PIE_DRAW_SECTOR_LOOP;

    chrt_pie_draw_sector_loop:

        case PIE_DRAW_SECTOR_LOOP:
            if(varCtr >= 0)
            {

                // get the value to be computed
                if(ChGetShowSeriesCount(pCh) > 1)
                {
                    temp = *(&(*pVar->pData) + ChGetSampleStart(pCh) - 1);
                }
                else
                {
                    temp = *(&(*pVar->pData) + ChGetSampleEnd(pCh) - varCtr);
                }

                // calculate the sector that the value will occupy
                dTemp = ((DWORD) (temp) * (3600)) / y;

                // check if we need to round up or not
                if((dTemp % 10) < 5)
                    dTemp = (dTemp / 10);                       // do not round up to next number
                else
                    dTemp = (dTemp / 10) + 1;                   // round up the value

                // set the color to the color of the variable
                SetColor(*(&(*pCh->prm.pColor) + k));

                // check if the sector has zero angle if it is zero just draw the
                // line.
                if(dTemp == 0)
                {
                    state = PIE_DRAW_SECTOR_LOOP_CONTINUE;
                    goto chrt_pie_draw_sector_loop_continue;
                }

                // go to the state that draws only. Doing this separates the setup of static variables
                // and rendering. So in cases when the rendering cannot continue, the variables
                // are still set to correct values.
                state = PIE_DRAW_SECTOR_ACTUAL;
                goto pie_draw_sector_actual;
            }
            else
            {
                if(GetState(pCh, CH_DONUT) == CH_DONUT)
                {
                    state = PIE_DONUT_HOLE_DRAW;
                    goto chrt_pie_donut_hole_draw;
                }
                else
                {
                    state = REMOVE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                    GFX_DRIVER_CompleteDrawUpdate(   pCh->hdr.left,
                                                    pCh->hdr.top,
                                                    pCh->hdr.right,
                                                    pCh->hdr.bottom);
#endif
                    return (1);
                }
            }

        case PIE_DRAW_SECTOR_ACTUAL:
            pie_draw_sector_actual :

            // check if it is the last sector to be drawn
            if((varCtr == 1) || ((j + dTemp) >= 358))
            {
                if(!DrawSector(ctr, ctry, z, j, 360, LIGHTGRAY))
                    return (0);
            }
            else
            {
                if(!DrawSector(ctr, ctry, z, j, (j + dTemp), LIGHTGRAY))
                    return (0);
            }

            state = PIE_DRAW_SECTOR_LOOP_CREATE_STRINGS;

        case PIE_DRAW_SECTOR_LOOP_CREATE_STRINGS:

            // create the strings of the values if needed
            if(GetState(pCh, CH_VALUE) || GetState(pCh, CH_PERCENT))
            {
                h = 0;
                GetCirclePoint(z, (j + (dTemp >> 1)), &pieX, &pieY);

                pieX += ctr;
                pieY += ctry;

                // do we need to show the values? create the strings here
                if(GetState(pCh, CH_VALUE))
                {
                    h = word2xchar(temp, tempStr, STR_CHAR_CNT);
                }

                // do we need to show the percentage? create the strings here
                if(GetState(pCh, CH_PERCENT))
                {

                    // add the % sign
                    // check if we need to add comma
                    if(GetState(pCh, CH_VALUE))
                    {
                        h += 1;                         // adjust h
                        tempStr[STR_CHAR_CNT - h] = ',';
                    }

                    h += 1;                             // adjust h
                    tempStr[STR_CHAR_CNT - h] = '%';

                    // now add the percentage
                    dPercent = (DWORD) (dTemp * 1000) / 360;

                    // check if we need to round up or not
                    if((dPercent % 10) < 5)
                        dPercent = (dPercent / 10);     // do not round up to next number
                    else
                        dPercent = (dPercent / 10) + 1; // round up the value
                    i = word2xchar((WORD) dPercent, tempStr, STR_CHAR_CNT - h);

                    // adjust the h position
                    h += i;
                }

                    #ifdef USE_PIE_ENABLE_LABEL

                // add the labels
                h += 1; // adjust h
                tempStr[STR_CHAR_CNT - h] = ':';
                h += 1; // adjust h
                tempStr[STR_CHAR_CNT - h] = 'A' + (ChGetSampleStart(pCh) - 1 + k);
                    #endif
                SetColor(pCh->hdr.pGolScheme->EmbossLtColor);

                m = j;

                // we have to relocate the text depending on the position
                if((m + (dTemp >> 1) >= 0) && (m + (dTemp >> 1) <= 90))
                {

                    // check if we need to draw a line
                    if((dTemp < GetTextHeight(pVarFont)) || (pieLabelYPos > pieY))
                    {
                        pieLabelXPos = ctr + z + GetTextHeight(pVarFont);
                        pieSectorXPos = pieX + 3;
                        if((m + (dTemp >> 1)) < 45)
                            pieSectorYPos = pieY + 1;
                        else
                            pieSectorYPos = pieY + 3;

                        // The label will now exceed the chart dimension, so force the value to be printed near the sector
                        pieLabelXPos = ctr + z + GetTextHeight(pVarFont);
                        if((pieLabelYPos + GetTextHeight(pVarFont)) > (pCh->hdr.bottom - (GOL_EMBOSS_SIZE + 1)))
                        {
                            MoveTo(pieX, pieY + 1);
                        }
                        else
                        {

                            // draw the line
                            if
                            (
                                !Line
                                    (
                                        pieSectorXPos,
                                        pieSectorYPos,
                                        pieLabelXPos,
                                        pieLabelYPos + (GetTextHeight(pVarFont) >> 1)
                                    )
                            ) return (0);

                            MoveTo(pieLabelXPos, pieLabelYPos);
                            pieLabelYPos += GetTextHeight(pVarFont);
                        }
                    }
                    else
                    {
                        MoveTo(pieX + GetTextWidth(tempXchar, pVarFont), pieY);
                        pieLabelYPos = pieY + GetTextHeight(pVarFont);
                    }
                }
                else if((m + (dTemp >> 1) > 90) && (m + (dTemp >> 1) <= 180))
                {

                    // check if we need to draw a line
                    if((dTemp < GetTextHeight(pVarFont)) || (pieLabelYPos2 < pieY))
                    {
                        pieLabelXPos = ctr - z - 3;

                        pieSectorXPos = pieX - 3;

                        if((m + (dTemp >> 1)) < 135)
                            pieSectorYPos = pieY + 3;
                        else
                            pieSectorYPos = pieY;

                        // check if slope of line is greater than -1.
                        // if it is we must adjust position of text to avoid the line
                        // intersecting the circumference of the pie chart
                        if((m + (dTemp >> 1)) < 180)
                        {

                            // make the slope equal to 1, this will make sure it does not intersect the circumference
                            if((abs(pieY - pieLabelYPos2) / abs(pieX - pieLabelXPos)) >= 1)
                            {
                                pieLabelXPos = pieX - abs(pieY - pieLabelYPos2);
                                if
                                (
                                    (SHORT) (pieLabelXPos - GetTextWidth(&tempStr[STR_CHAR_CNT - h], pVarFont)) <=
                                        (pCh->hdr.left + CH_MARGIN)
                                )
                                {
                                    pieLabelXPos =
                                        (
                                            pCh->hdr.left +
                                            CH_MARGIN +
                                            GetTextWidth(&tempStr[STR_CHAR_CNT - h], pVarFont)
                                        );
                                }
                            }
                        }

                        // draw the line
                        if
                        (
                            !Line
                                (
                                    pieSectorXPos,
                                    pieSectorYPos,
                                    pieLabelXPos,
                                    pieLabelYPos2 + (GetTextHeight(pVarFont) >> 1)
                                )
                        ) return (0);

                        MoveTo(pieLabelXPos - GetTextWidth(&tempStr[STR_CHAR_CNT - h], pVarFont), pieLabelYPos2);
                        pieLabelYPos2 -= GetTextHeight(pVarFont);
                    }
                    else
                    {
                        MoveTo
                        (
                            pieX - GetTextWidth(&tempStr[STR_CHAR_CNT - h], pVarFont) - GetTextWidth
                                (
                                    tempXchar,
                                    pVarFont
                                ),
                            pieY
                        );
                        pieLabelYPos2 = pieY - GetTextHeight(pVarFont);
                    }
                }
                else if((m + (dTemp >> 1) > 180) && (m + (dTemp >> 1) <= 270))
                {

                    // check if we need to draw a line
                    if((dTemp < GetTextHeight(pVarFont)) || (pieLabelYPos2 < pieY))
                    {
                        pieLabelXPos = ctr - z - 5;

                        if((m + (dTemp >> 1)) < 225)
                        {
                            pieSectorXPos = pieX - 3;
                            pieSectorYPos = pieY;
                        }
                        else
                        {
                            pieSectorXPos = pieX;
                            pieSectorYPos = pieY - 3;
                        }

                        // check if slope of line is greater than -1.
                        // if it is we must adjust position of text to avoid the line
                        // intersecting the circumference of the pie chart
                        if((m + (dTemp >> 1)) < 270)
                        {

                            // make the slope equal to 1, this will make sure it does not intersect the circumference
                            if((abs(pieY - pieLabelYPos2) / abs(pieX - pieLabelXPos)) >= 1)
                            {
                                pieLabelXPos = ctr - (z + (z >> 1)) - 5;
                                if
                                (
                                    (SHORT) (pieLabelXPos - GetTextWidth(&tempStr[STR_CHAR_CNT - h], pVarFont)) <=
                                        (pCh->hdr.left + CH_MARGIN)
                                )
                                {
                                    pieLabelXPos =
                                        (
                                            pCh->hdr.left +
                                            CH_MARGIN +
                                            GetTextWidth(&tempStr[STR_CHAR_CNT - h], pVarFont)
                                        );
                                }
                            }
                        }

                        // The label will now exceed the chart dimension, so force the value to be printed aligned in
                        // y position with the previous value printed
                        if
                        (
                            (pieLabelYPos2) <
                                (pCh->hdr.top + GOL_EMBOSS_SIZE + CH_MARGIN + GetTextHeight(pCh->prm.pTitleFont))
                        )
                        {
                            if
                            (
                                !Line
                                    (
                                        pieSectorXPos,
                                        pieSectorYPos,
                                        pieLabelXPos + (GetTextWidth(&tempStr[STR_CHAR_CNT - h], pVarFont) >> 1),
                                        pieLabelYPos2 + ((GetTextHeight(pVarFont) >> 1) * 3)
                                    )
                            ) return (0);

                            MoveTo
                            (
                                pieLabelXPos + (GetTextWidth(&tempStr[STR_CHAR_CNT - h], pVarFont) >> 1),
                                pieLabelYPos2 + GetTextHeight(pVarFont)
                            );

                            // adjust the next marker
                            pieLabelYPos3 += GetTextHeight(pVarFont);
                        }
                        else
                        {

                            // draw the line
                            if
                            (
                                !Line
                                    (
                                        pieSectorXPos,
                                        pieSectorYPos,
                                        pieLabelXPos,
                                        pieLabelYPos2 + (GetTextHeight(pVarFont) >> 1)
                                    )
                            ) return (0);

                            MoveTo(pieLabelXPos - GetTextWidth(&tempStr[STR_CHAR_CNT - h], pVarFont), pieLabelYPos2);
                            pieLabelYPos2 -= GetTextHeight(pVarFont);
                        }
                    }
                    else
                    {
                        MoveTo
                        (
                            pieX - GetTextWidth(&tempStr[STR_CHAR_CNT - h], pVarFont) - GetTextWidth
                                (
                                    tempXchar,
                                    pVarFont
                                ),
                            pieY - GetTextHeight(pVarFont)
                        );
                        pieLabelYPos2 = pieY - (GetTextHeight(pVarFont) << 1);
                    }
                }
                else if((m + (dTemp >> 1) > 270) && (m + (dTemp >> 1) <= 360))
                {

                    // check if we need to draw a line
                    if((dTemp < GetTextHeight(pVarFont)) || (pieLabelYPos3 > pieY))
                    {
                        pieLabelXPos = ctr + z + GetTextHeight(pVarFont);

                        pieSectorXPos = pieX + 3;
                        pieSectorYPos = pieY - 2;

                        // draw the line
                        if
                        (
                            !Line
                                (
                                    pieSectorXPos,
                                    pieSectorYPos,
                                    pieLabelXPos,
                                    pieLabelYPos3 - (GetTextHeight(pVarFont) >> 1)
                                )
                        ) return (0);

                        MoveTo(pieLabelXPos, pieLabelYPos3 - GetTextHeight(pVarFont));
                        pieLabelYPos3 += GetTextHeight(pVarFont);
                    }
                    else
                    {
                        MoveTo(pieX + 5, pieY - GetTextHeight(pVarFont));
                        pieLabelYPos3 = pieY + GetTextHeight(pVarFont);
                    }
                }

                state = PIE_DRAW_SECTOR_LOOP_STRINGS_RUN;
            }
            else
            {
                state = PIE_DRAW_SECTOR_LOOP_CONTINUE;
                goto chrt_pie_draw_sector_loop_continue;
            }

        case PIE_DRAW_SECTOR_LOOP_STRINGS_RUN:

            // now draw the strings of the values and/or percentages
			SetColor(pCh->hdr.pGolScheme->EmbossLtColor);
            SetFont(pVarFont);

            if(!OutText(&tempStr[STR_CHAR_CNT - h]))
                return (0);

            state = PIE_DRAW_SECTOR_LOOP_CONTINUE;

    chrt_pie_draw_sector_loop_continue:

        case PIE_DRAW_SECTOR_LOOP_CONTINUE:
            j += dTemp;
            varCtr--;
            if(varCtr == 0)
            {
                if(GetState(pCh, CH_DONUT) == CH_DONUT)
                {
                    state = PIE_DONUT_HOLE_DRAW;
                    goto chrt_pie_donut_hole_draw;
                }
                else
                {
                    state = REMOVE;
                    return (1);
                }
            }

            // check if more than one data series to be shown
            if(ChGetShowSeriesCount(pCh) > 1)
            {
                pVar = ChGetNextShowData((DATASERIES *)pVar->pNextData);
                if(pVar == NULL)
                {
                    break;
                }
            }

            k++;
            state = PIE_DRAW_SECTOR_LOOP;
            goto chrt_pie_draw_sector_loop;

    chrt_pie_donut_hole_draw:

        case PIE_DONUT_HOLE_DRAW:
            SetColor(LIGHTGRAY);
            if(!Circle(ctr, ctry, (z >> 1) - (z >> 3)))
                return (0);
            SetColor(pCh->hdr.pGolScheme->CommonBkColor);
            if(!FillCircle(ctr, ctry, ((z >> 1) - (z >> 3)) - 1))
                return (0);

            state = REMOVE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
            GFX_DRIVER_CompleteDrawUpdate(   pCh->hdr.left,
                                            pCh->hdr.top,
                                            pCh->hdr.right,
                                            pCh->hdr.bottom);
#endif
            return (1);
    }

#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
    GFX_DRIVER_CompleteDrawUpdate(   pCh->hdr.left,
                                    pCh->hdr.top,
                                    pCh->hdr.right,
                                    pCh->hdr.bottom);
#endif
    return (1);
}

/*********************************************************************
* Function: ChAddDataSeries(CHART *pCh, WORD nSamples, WORD *pData, XCHAR *pName)
*
*
* Notes: Adds a new variable data structure in the linked list 
*		 of variable datas. Number of samples is given with the 
*		 array of the samples. If there is only one data 
* 		 nSamples is set to 1 with the address of the variable data.
*		 
*
********************************************************************/
DATASERIES *ChAddDataSeries(CHART *pCh, WORD nSamples, WORD *pData, XCHAR *pName)
{
    DATASERIES  *pVar = NULL, *pListVar;

    pVar = (DATASERIES *)GFX_malloc(sizeof(DATASERIES));

    if(pVar == NULL)
        return (NULL);

    // add the other parameters of the variable data
    pVar->pSData = (XCHAR *)pName;
    pVar->samples = nSamples;
    pVar->pData = (WORD *)pData;
    pVar->show = SHOW_DATA;
    pVar->pNextData = NULL;

    pListVar = pCh->pChData;
    if(pCh->pChData == NULL)
        pCh->pChData = pVar;
    else
    {

        // search the end of the list and append the new data
        while(pListVar->pNextData != NULL)
            pListVar = pListVar->pNextData;
        pListVar->pNextData = pVar;
    }

    // update the variable count before exiting
    pCh->prm.seriesCount++;
    return (DATASERIES *)pVar;
}
/*********************************************************************
* Function: void ChFreeDataSeries(void *pObj)
*
*
* Notes: Removes a data series structure in the linked list 
*		 of data series. 
*
********************************************************************/
void ChFreeDataSeries(void *pObj)
{
    DATASERIES  *pVar = NULL, *pPrevVar = NULL;
    CHART *pCh;

    pCh = (CHART *)pObj;

    pVar = pCh->pChData;

    // check if the list is empty
    if(pVar == NULL)
        return;

    // check if there is only one entry
    if(pVar->pNextData == NULL)
    {
        GFX_free(pVar);
        pCh->pChData = NULL;
        return;
    }

    // remove all
    while(pVar != NULL)
    {
        pPrevVar = pVar;
        pVar = pVar->pNextData;

        // free the memory used by the item
        GFX_free(pPrevVar);
    }
}

/*********************************************************************
* Function: ChRemoveDataSeries(CHART *pCh, WORD number)
*
*
* Notes: Removes a data series structure in the linked list 
*		 of data series. 
*
********************************************************************/
void ChRemoveDataSeries(CHART *pCh, WORD number)
{
    DATASERIES  *pVar = NULL, *pPrevVar = NULL;
    WORD        ctr = 1;

    pVar = pCh->pChData;

    // check if the list is empty
    if(pVar == NULL)
        return;

    // check if there is only one entry
    if(pVar->pNextData == NULL)
    {
        GFX_free(pVar);
        pCh->pChData = NULL;
        return;
    }

    if(number == 0)
    {

        // remove all
        while(pVar != NULL)
        {
            pPrevVar = pVar;
            pVar = pVar->pNextData;

            // free the memory used by the item
            GFX_free(pPrevVar);
        }

        return;
    }

    // there are more than one entry, remove the entry specified
    while(ctr < number)
    {
        pPrevVar = pVar;
        pVar = pVar->pNextData;
        ctr++;
    }

    // remove the item from the list
    pPrevVar->pNextData = pVar->pNextData;

    // free the memory used by the item
    GFX_free(pVar);

    return;
}

/*********************************************************************
* Function: ChSetDataSeries(CHART *pCh, WORD seriesNum, BYTE status)
*
*
* Notes: Sets the specified data series number show flag to be set to 
*		 SHOW_DATA or HIDE_DATA depending on the status. 
*    	 If the seriesNum is 0, it sets all the data series 
*		 entries in the data series linked list. Returns the same passed
*		 number if successful otherwise -1 if unsuccesful.
*
********************************************************************/
SHORT ChSetDataSeries(CHART *pCh, WORD seriesNum, BYTE status)
{
    DATASERIES  *pListSer;
    WORD        ctr = 1;

    pListSer = pCh->pChData;

    // check if the list is empty
    if(pListSer == NULL)
        return (-1);

    while(pListSer != NULL)
    {

        // check if we need to show all
        if(seriesNum == 0)
            pListSer->show = status;
        else if(seriesNum == ctr)
        {
            pListSer->show = status;
            break;
        }

        ctr++;
        pListSer = pListSer->pNextData;
    }

    if(seriesNum == ctr)
        return (seriesNum);
    else
        return (-1);
}

/*********************************************************************
* Function: ChSetSampleRange(CHART *pCh, WORD start, WORD end) 
*
*
* Notes: Sets the sampling start and end points when drawing the chart.
*		 Depending on the number of data series with SHOW_DATA flag
*		 set and the values of end and start samples a single
*		 data series is drawn or multiple data series are drawn.
*
********************************************************************/
void ChSetSampleRange(CHART *pCh, WORD start, WORD end)
{
    pCh->prm.smplStart = start;
    if(end < start)
        pCh->prm.smplEnd = start;
    else
        pCh->prm.smplEnd = end;
}

/*********************************************************************
* Function: ChSetValueRange(CHART *pCh, WORD min, WORD max) 
*
*
* Notes: Sets the sampling start and end points when drawing the chart.
*		 Depending on the number of data series with SHOW_DATA flag
*		 set and the values of end and start samples a single
*		 data series is drawn or multiple data series are drawn.
*
********************************************************************/
void ChSetValueRange(CHART *pCh, WORD min, WORD max)
{
    pCh->prm.valMin = min;
    if(max < min)
        pCh->prm.valMax = min;
    else
        pCh->prm.valMax = max;
}

/*********************************************************************
* Function: ChSetPercentRange(CHART *pCh, WORD min, WORD max) 
*
*
* Notes: Sets the percentage range when drawing the chart. This affects
*		 bar charts only and CH_PERCENTAGE bit state is set.
*
********************************************************************/
void ChSetPercentRange(CHART *pCh, WORD min, WORD max)
{
    pCh->prm.perMin = min;
    if(max < min)
        pCh->prm.perMax = min;
    else
        pCh->prm.perMax = max;
}



/* */

void FillSector(SHORT x, SHORT y, GFX_COLOR outLineColor)
{
    GFX_COLOR    pixel;
    SHORT   left, right;
    SHORT   top, bottom;
    SHORT   xc, yc;

    // scan down
    top = bottom = yc = y;
    left = right = xc = x;
    while(1)
    {

        pixel = GetPixel(xc, yc);
        if(pixel == outLineColor)
        {
            for(xc = left + 1; xc < right; xc++)
            {
                pixel = GetPixel(xc, yc);
                if(pixel != outLineColor)
                {
                    break;
                }
            }

            if(xc == right)
                break;
        }

        // left scan
        left = xc;
        do
        {
            left--;
            pixel = GetPixel(left, yc);
        } while(pixel != outLineColor);
        while(!Line(xc, yc, left + 1, yc));

        // right scan
        right = xc;
        pixel = GetPixel(right, yc);
        do
        {
            right++;
            pixel = GetPixel(right, yc);
        } while(pixel != outLineColor);
        while(!Line(xc, yc, right - 1, yc));

        xc = (left + right) >> 1;
        yc++;
    }

    // scan up
    yc = y;
    xc = x;
    while(1)
    {
        pixel = GetPixel(xc, yc);
        if(pixel == outLineColor)
        {
            for(xc = left + 1; xc < right; xc++)
            {
                pixel = GetPixel(xc, yc);
                if(pixel != outLineColor)
                {
                    break;
                }
            }

            if(xc == right)
                break;
        }

        // left scan
        left = xc;
        do
        {
            left--;
            pixel = GetPixel(left, yc);
        } while(pixel != outLineColor);
        while(!Line(xc, yc, left + 1, yc));

        // right scan
        right = xc;
        pixel = GetPixel(right, yc);
        do
        {
            right++;
            pixel = GetPixel(right, yc);
        } while(pixel != outLineColor);
        while(!Line(xc, yc, right - 1, yc));

        xc = (left + right) >> 1;
        yc--;
    }
}

/* */


/* */
WORD DrawSector(SHORT cx, SHORT cy, SHORT outRadius, SHORT angleFrom, SHORT angleTo, GFX_COLOR outLineColor)
{
    typedef enum
    {
        SEC_DRAW_IDLE,
        SEC_DRAW_EDGE1,
        SEC_DRAW_EDGE2,
        SEC_DRAW_EDGE3,
        SEC_DRAW_EDGE4,
        SEC_DRAW_EDGE5,
        SEC_DRAW_FILLINIT,
        SEC_DRAW_FILL,
    } DRAW_SECTOR_STATES;

    static SHORT x1, y1, x2, y2, x3, y3;
    static SHORT angleMid;
    static GFX_COLOR tempColor;
    LONG temp;

    static DRAW_SECTOR_STATES sectorState = SEC_DRAW_IDLE;

    switch(sectorState)
    {
        case SEC_DRAW_IDLE:

            // calculate points
            angleMid = (angleTo + angleFrom) >> 1;
            GetCirclePoint(outRadius, angleFrom, &x1, &y1);
            GetCirclePoint(outRadius, angleTo, &x2, &y2);
            x1 += cx;
            y1 += cy;
            x2 += cx;
            y2 += cy;

            // grab the current color value for later use
            tempColor = GetColor();

            // check if we need to draw the edges
            // special case for single data shown on pie chart
            // we remove the line drawn at angle 0
            if(!((angleFrom == 0) && (angleTo == 360)))
            {
                sectorState = SEC_DRAW_EDGE1;

                // special case for small angles
                GetCirclePoint(outRadius - 1, angleFrom + 1, &x3, &y3);
                x3 += cx;
                y3 += cy;
                goto sec_draw_edge1;
            }
            else
            {
                sectorState = SEC_DRAW_FILLINIT;
                goto sec_draw_fillinit;
            }

        case SEC_DRAW_EDGE1:
            sec_draw_edge1 : if(!Line(x3, y3, cx, cy)) return (0);
            GetCirclePoint(outRadius - 1, angleTo - 1, &x3, &y3);
            x3 += cx;
            y3 += cy;
            sectorState = SEC_DRAW_EDGE2;

        case SEC_DRAW_EDGE2:
            if(!Line(x3, y3, cx, cy))
                return (0);
            GetCirclePoint(outRadius - 1, angleMid, &x3, &y3);
            x3 += cx;
            y3 += cy;
            sectorState = SEC_DRAW_EDGE3;

        case SEC_DRAW_EDGE3:
            if(!Line(x3, y3, cx, cy))
                return (0);
            SetColor(outLineColor);
            sectorState = SEC_DRAW_EDGE4;

        case SEC_DRAW_EDGE4:
            if(!Line(x1, y1, cx, cy))
                return (0);
            sectorState = SEC_DRAW_EDGE5;

        case SEC_DRAW_EDGE5:
            if(!Line(x2, y2, cx, cy))
                return (0);
            sectorState = SEC_DRAW_FILLINIT;

        case SEC_DRAW_FILLINIT:
            sec_draw_fillinit : SetColor(tempColor);

            temp = ((x1 - x2) * (x1 - x2));
            temp += ((y1 - y2) * (y1 - y2));

            if(((DWORD) temp <= (DWORD) 16) && ((angleTo - angleFrom) < 90))
            {
                sectorState = SEC_DRAW_IDLE;
                return (1);
            }

            GetCirclePoint(outRadius - 2, angleMid, &x3, &y3);
            x3 += cx;
            y3 += cy;
            sectorState = SEC_DRAW_FILL;

        case SEC_DRAW_FILL:

            // FillSector() is a blocking call
            // making it non-blocking will further add delay to the rendering
            FillSector(x3, y3, outLineColor);
            sectorState = SEC_DRAW_IDLE;
            break;
    }   // end of switch()

    return (1);
}

#endif // USE_CHART
