/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        GraphShow.c
 * Dependencies:    GraphShow.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2007 Microchip Technology Inc.  All rights reserved.
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
 * Paolo Tamayo         01/29/09	...
 *****************************************************************************/
#include <stdlib.h>
#include "MainDemo.h"

#define DEMOTYPECOUNT   16                          // # of types of chart configuration
#define DATASAMPLES     15
#define SAMPLECOUNT     3
#define MAXSERIES       5

////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void                    ErrorTrap(XCHAR *message);  // outputs text message on screen and stop execution
void                    SetUpNextDemo(CHART *pChart);
void                    ChangeChartData(void);
void                    AssembleTitleString(CHART *pChart, XCHAR *pTitle);
WORD                    xcharStrCopyAppend(XCHAR *pDest, XCHAR *pSrc, WORD copy);

void                    DebugPauseCode(void);
void                    CreateChart(WORD state);

/////////////////////////////////////////////////////////////////////////////
//                            IMAGES USED
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//                            STRINGS USED
/////////////////////////////////////////////////////////////////////////////
extern XCHAR            ExitStr[];
extern XCHAR            ScaleStr[];
extern XCHAR            LeftArrowStr[];
extern XCHAR            RightArrowStr[];

/////////////////////////////////////////////////////////////////////////////
//                             FONTS USED
/////////////////////////////////////////////////////////////////////////////
extern const FONT_FLASH GOLSmallFont;
extern const FONT_FLASH GOLMediumFont;

/////////////////////////////////////////////////////////////////////////////
//                       GLOBAL VARIABLES FOR DEMO
/////////////////////////////////////////////////////////////////////////////
CHART_DEMO_TYPES        demoType = BAR_LEGEND_PERCENT_VERTICAL; // current type of the chart shown
WORD                    dTypeCnt;                               // global counter for the chart demo types
CHART_TYPES             chartType;                              // gloabl that specifies if the chart is pie or bar

/////////////////////////////////////////////////////////////////////////////

//                            DEMO TITLES
/////////////////////////////////////////////////////////////////////////////
XCHAR                   DemoTitle[CHARTTITLESTRLENGTH];

/////////////////////////////////////////////////////////////////////////////
//                            DEMO TYPES
/////////////////////////////////////////////////////////////////////////////
const CHART_DEMO_TYPES  demoTypeList[DEMOTYPECOUNT] = {
    BAR_3D_LEGEND_PERCENT_VERTICAL,
    BAR_3D_LEGEND_NPERCENT_VERTICAL,
    BAR_3D_NLEGEND_PERCENT_VERTICAL,

    BAR_LEGEND_PERCENT_VERTICAL,
    BAR_LEGEND_NPERCENT_VERTICAL,
    BAR_NLEGEND_PERCENT_VERTICAL,

    BAR_LEGEND_PERCENT_HORIZONTAL,
    BAR_LEGEND_NPERCENT_HORIZONTAL,
    BAR_NLEGEND_PERCENT_HORIZONTAL,

    BAR_3D_LEGEND_PERCENT_HORIZONTAL,
    BAR_3D_LEGEND_NPERCENT_HORIZONTAL,
    BAR_3D_NLEGEND_PERCENT_HORIZONTAL,

    PIE_NLEGEND_PERCENT,
    PIE_LEGEND_PERCENT,
    PIE_LEGEND_NPERCENT,
    PIE_NLEGEND_NPERCENT,
};

const XCHAR             BarStr[] = {'B','a','r',0};
const XCHAR             PieStr[] = {'P','i','e',0};
const XCHAR             ThreeDStr[] = {'3','D',0};
const XCHAR             LegendStr[] = {'L','e','g','e','n','d',0};
const XCHAR             ValueStr[] = {'V','a','l','u','e',0};
const XCHAR             DonutStr[] = {'D','o','n','u','t',0};
const XCHAR             PercentStr[] = {'%',0};
const XCHAR             ParenOpenStr[] = {'(',0};
const XCHAR             ParenCloseStr[] = {')',0};
const XCHAR             CommaStr[] = {',',0};
const XCHAR             SpaceStr[] = {' ',0};

const XCHAR             SingleStr[] = {'S','i','n','g','l','e',0};
const XCHAR             MultiStr[] = {'M','u','l','t','i',0};

const XCHAR             Var1Name[] = {'V','a','r','1',0};
const XCHAR             Var2Name[] = {'V','a','r','2',0};
const XCHAR             Var3Name[] = {'V','a','r','3',0};
const XCHAR             Var4Name[] = {'V','a','r','4',0};
const XCHAR             Var5Name[] = {'V','a','r','5',0};
const XCHAR             XName[] = {'C','a','t','e','g','o','r','y',0};
const XCHAR             YName[] = {'V','a','l','u','e','s',0};

WORD                    samples1[] = {1,   4,  7, 10, 20, 25, 35, 46, 50, 42, 37, 24, 17, 9,  2};
WORD                    samples2[] = {10, 25, 15,  5, 50,  0,  1,  0,  0, 35, 30,  7,  8,  9, 10};
WORD                    samples3[] = {48, 40, 30, 30, 25, 18, 12,  8,  9,  2, 28, 32, 45, 33, 37};
WORD                    samples4[] = {32, 11, 24,  1,  1, 47, 26, 14, 44, 12, 33, 14, 15, 43, 10};
WORD                    samples5[] = {41, 28, 43, 30, 26, 0,  12, 13, 14, 15, 19,  6, 39,  3, 28};

// define the color set used for the Chart, note that this is application defined in this demo code.
// if application do not set the colors used (in ChSetColorTable()) the default colors will be used
WORD                    MyColorTable[16] = {LIGHTGREEN, RGB565CONVERT(255,200,0), RGB565CONVERT(255,255,150),LIGHTRED, LIGHTBLUE,
    GREEN, RGB565CONVERT(255,140,0), YELLOW, RED, BLUE,
    BRIGHTGREEN, RGB565CONVERT(0xFF,0xBB,0x4C), BRIGHTYELLOW, BRIGHTRED, BRIGHTBLUE,
    RGB565CONVERT(255,215,0)};

/************************************************************************
 Function: WORD CreateGraphDemo(void)
                                                                       
 Overview: This function creates the Chart demo.
 		                                                                          
 Input: none
                                                                       
 Output: Return value indicates if the creation of the objects in this
         demo was successful. If not successful, the calling function
         can recover and not continue the demo. 
************************************************************************/
WORD CreateGraphDemo(void)
{
    CHART   *pMyChart;

    // free memory for the objects in the previous linked list and start new list
    GOLFree();

    SetColor(SCREEN_BACKGROUND_COLOR);
    ClearDevice();

    // type count dTypeCnt is used here to select the type of chart in the array demoTypeList
    // demoTypeList is an array of chart property state settings defined in this application
    dTypeCnt = 0;

    // chartType will indicate if the chart is pie or bar with single or multiple variables
    chartType = SHOW_MULTIPLE_BAR;

    // create the chart object
    pMyChart = ChCreate
        (
            ID_CHART,
            0,
            0,  // dimensions
            GetMaxX(),
            GetMaxY() - (CTRLBTN_HEIGHT + 1),   //
            demoTypeList[dTypeCnt] | CH_DRAW,   // state of the chart
            NULL,                               // data not initialized yet
            NULL,                               // no paraters and data yet
            graphScheme
        );                              // style scheme used
    if(pMyChart == NULL)
    {
        screenState = CREATE_DEMOSELECTION;
        return (0);
    }

    // create the demo navigation/control buttons
    CreateCtrlButtons(ExitStr, (XCHAR *)BarStr, (XCHAR *)MultiStr, RightArrowStr);

    // Add the chart title, x-axis label and y-axis label
    //ChSetTitle(pMyChart, (XCHAR*)&DemoTitles[dTypeCnt]);
    AssembleTitleString(pMyChart, DemoTitle);
    ChSetTitle(pMyChart, DemoTitle);

    ChSetTitleFont(pMyChart, (void *) &GOLFontDefault);

    // set the grid labels and axis labels font	
    ChSetGridLabelFont(pMyChart, (void *) &GOLSmallFont);
    ChSetAxisLabelFont(pMyChart, (void *) &GOLSmallFont);

    // set the sample label and value label strings
    ChSetSampleLabel(pMyChart, (XCHAR *)XName);
    ChSetValueLabel(pMyChart, (XCHAR *)YName);

    // add the variables and the data of the variables. This series of function calls will
    // create the link list of variable structures.
    ChAddDataSeries(pMyChart, DATASAMPLES, samples1, (XCHAR *)Var1Name);
    ChAddDataSeries(pMyChart, DATASAMPLES, samples2, (XCHAR *)Var2Name);
    ChAddDataSeries(pMyChart, DATASAMPLES, samples3, (XCHAR *)Var3Name);
    ChAddDataSeries(pMyChart, DATASAMPLES, samples4, (XCHAR *)Var4Name);
    ChAddDataSeries(pMyChart, DATASAMPLES, samples5, (XCHAR *)Var5Name);

    // set the color table to be used. Note that if this is not called, the default
    // color table will be used. (see ChartVarClr[] in Chart.c file)
    ChSetColorTable(pMyChart, MyColorTable);

    // set the range of the values
    ChSetValueRange(pMyChart, 0, 50);
    ChSetPercentRange(pMyChart, 0, 75); // ChSetPerRange(pMyChart, 40, 60);
    SetUpNextDemo(pMyChart);
    return (1);
}

/************************************************************************
 Function: void InitGraphDemoStyleScheme(GOL_SCHEME *pScheme)
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void InitGraphDemoStyleScheme(GOL_SCHEME *pScheme)
{

    // colors used for the x-axis, y-axis and legend labels
    pScheme->TextColor1 = BRIGHTBLUE;

    // sets the colors of the grid labels
    pScheme->TextColor0 = RGB565CONVERT(0xFF, 0xBB, 0x4C);
    pScheme->pFont = (void *) &GOLSmallFont;

    // sets the color of the chart data values when drawn
    pScheme->EmbossLtColor = DARKGRAY;

    // sets the color of the chart title when drawn	
    pScheme->EmbossDkColor = BLUE;
}

/************************************************************************
 Function: WORD GraphMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: This function is called by GOLMsg() function when in this 
 		   particular demo each time the valid message is received for the 
  		   objects in the demo.
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the original GOL message.
                                                                       
 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD GraphMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    CHART   *pChart = (CHART *)GOLFindObject(ID_CHART);

    if(objMsg == BTN_MSG_RELEASED)
    {
        switch(GetObjID(pObj))
        {
            case ID_BUTTON_A:
                if(objMsg == BTN_MSG_RELEASED)
                {

                    // do not process if user moved the touch away from the button
                    // returning 1 wil update the button
                    if(pMsg->uiEvent == EVENT_MOVE)
                        return (1);

                    // implements some form of debounce
                    // add code here.
                    screenState = CREATE_DEMOSELECTION;
                    return (0);
                }

            case ID_BUTTON_B:
                if(objMsg == BTN_MSG_RELEASED)
                {

                    // do not process if user moved the touch away from the button
                    // returning 1 wil update the button
                    if(pMsg->uiEvent == EVENT_MOVE)
                        return (1);

                    // toggle through the 4 types (see CHART_TYPES definition)	
                    if(chartType == SHOW_SINGLE_PIE)
                    {
                        chartType = SHOW_SINGLE_BAR;
                        BtnSetText((BUTTON *)pObj, (XCHAR *)BarStr);
                        dTypeCnt = 0;
                    }
                    else if(chartType == SHOW_MULTIPLE_PIE)
                    {
                        chartType = SHOW_MULTIPLE_BAR;
                        BtnSetText((BUTTON *)pObj, (XCHAR *)BarStr);
                        dTypeCnt = 0;
                    }
                    else if(chartType == SHOW_MULTIPLE_BAR)
                    {
                        chartType = SHOW_MULTIPLE_PIE;
                        BtnSetText((BUTTON *)pObj, (XCHAR *)PieStr);
                        dTypeCnt = 12;
                    }
                    else if(chartType == SHOW_SINGLE_BAR)
                    {
                        chartType = SHOW_SINGLE_PIE;
                        BtnSetText((BUTTON *)pObj, (XCHAR *)PieStr);
                        dTypeCnt = 12;
                    }

                    SetUpNextDemo(pChart);
                }

                return (1);

            case ID_BUTTON_C:
                if(objMsg == BTN_MSG_RELEASED)
                {

                    // do not process if user moved the touch away from the button
                    // returning 1 wil update the button
                    if(pMsg->uiEvent == EVENT_MOVE)
                        return (1);

                    // toggle through the 4 types (see CHART_TYPES definition)	
                    if(chartType == SHOW_SINGLE_PIE)
                    {
                        chartType = SHOW_MULTIPLE_PIE;
                        BtnSetText((BUTTON *)pObj, (XCHAR *)MultiStr);
                    }
                    else if(chartType == SHOW_MULTIPLE_PIE)
                    {
                        chartType = SHOW_SINGLE_PIE;
                        BtnSetText((BUTTON *)pObj, (XCHAR *)SingleStr);
                    }
                    else if(chartType == SHOW_MULTIPLE_BAR)
                    {
                        chartType = SHOW_SINGLE_BAR;
                        BtnSetText((BUTTON *)pObj, (XCHAR *)SingleStr);
                    }
                    else if(chartType == SHOW_SINGLE_BAR)
                    {
                        chartType = SHOW_MULTIPLE_BAR;
                        BtnSetText((BUTTON *)pObj, (XCHAR *)MultiStr);
                    }

                    SetUpNextDemo(pChart);
                }

                return (1);

            case ID_BUTTON_D:
                if(objMsg == BTN_MSG_RELEASED)
                {

                    // do not process if user moved the touch away from the button
                    // returning 1 wil update the button
                    if(pMsg->uiEvent == EVENT_MOVE)
                        return (1);

                    // toggle through the different demo types (see CHART_DEMO_TYPES definition)
                    if((chartType == SHOW_SINGLE_BAR) || (chartType == SHOW_MULTIPLE_BAR))
                    {
                        if(dTypeCnt < 11)
                        {
                            dTypeCnt++;
                        }
                        else
                        {
                            dTypeCnt = 0;
                        }
                    }
                    else if((chartType == SHOW_SINGLE_PIE) || (chartType == SHOW_MULTIPLE_PIE))
                    {
                        if(dTypeCnt < 15)
                        {
                            dTypeCnt++;
                        }
                        else
                        {
                            dTypeCnt = 12;
                        }
                    }

                    SetUpNextDemo(pChart);
                }

                return (1);

            default:
                return (1);
        }
    }

    return (1);
}

/************************************************************************
 Function: void SetUpNextDemo(CHART* pChart)
                                                                       
 Overview: This function is called to modify the Chart parameters 
 		   to change it to Bar to Pie Chart and/or change it from 
 		   Single variable to Multiple variables.
 		                                        
 Input: pChart - pointer to the Chart object
                                                                       
 Output: none
************************************************************************/
void SetUpNextDemo(CHART *pChart)
{
    WORD    state = 0;
    WORD    temp, i;

    // reset all
    ChHideSeries(pChart, 0);

    // reset all chart state bits
    ClrState(pChart, 0xE36F);

    // set up the percent range				
    temp = rand() % 100;

    // to make sure the displayed data is not showing a very low percentage range.
    if(temp < 50)
        temp += 50;
    ChSetPercentRange(pChart, 0, temp);

    switch(chartType)
    {
        case SHOW_SINGLE_BAR:
        case SHOW_SINGLE_PIE:

            // show bar chart with single data and changing 			
            // start and end sampling points.
            // randomize the data series to display
            temp = rand() % (MAXSERIES - 1);

            // set the data series to show, +1 to cover the possibility of temp = 0.
            // data series reference starts from 1 not zero.
            ChShowSeries(pChart, temp + 1);

            // randomize the # of samples to display, +1 is added to make sure that the
            // range is at least 1 sample.
            temp = rand() % (DATASAMPLES >> 1) + 1;
            ChSetSampleRange(pChart, 1, temp);      // set the first and last sample to be displayed	

            // set the Chart Object state & title
            state = demoTypeList[dTypeCnt] | CH_DRAW;

            break;

        case SHOW_MULTIPLE_BAR:

            // show bar chart with multiple data and changing 			
            // start and end sampling points.
            // randomize which data series to show
            // algorithm is simply selecting each data series and randomize if we want to show this
            // series or not (rand()%2)	
            temp = 0;
            while(temp < 2)
            {
                for(i = 1; i <= MAXSERIES; i++)
                {
                    if((rand() % 2) != 0)
                    {
                        ChShowSeries(pChart, i);    // set the data series to be shown
                        temp++;
                    }
                }
            }

            // randomize the # of samples to display, +1 is added to make sure that there will be no
            // range is at least 2 samples.
            temp = rand() % (DATASAMPLES >> 1) + 1;
            ChSetSampleRange(pChart, 1, temp + 1);  // set the first and last sample to be displayed

            // set the state & title
            state = demoTypeList[dTypeCnt] | CH_DRAW;

            break;

        case SHOW_MULTIPLE_PIE:

            // show pie chart with multiple data.
            // when drawing pie chart with multiple variables, the Chart is drawn
            // using the start sample point of each variable. End sample point is ignored.
            // In other words there is no start to end sample point range.
            // randomize which variable to show
            temp = 0;
            while(temp < 2)
            {
                for(i = 1; i <= MAXSERIES; i++)
                {
                    if((rand() % 2) != 0)
                    {
                        ChShowSeries(pChart, i);    // set the variable to be shown
                        temp++;
                    }
                }
            }

            // randomize the sample point to be shown
            temp = rand() % DATASAMPLES + 1;

            // set the sample point to be displayed, 1 here is just a filler since this is ignored
            // when set to multiple samples for pie chart
            ChSetSampleRange(pChart, temp, 1);

            // set the Chart Object state & title
            state = demoTypeList[dTypeCnt] | CH_DRAW;

            break;

        default:
            break;
    }

    // do not show percentage when multiple samples are shown on a bar chart.
    // the percentage is computed on the total of a variable samples
    if(chartType == SHOW_MULTIPLE_BAR)
    {
        state = ~CH_PERCENT & state;
    }

    // if the sample range is one single sample and percentage is on
    // turn it off. This is done since the percentage is computed on the
    // samples that are to be shown, so if there is ony one sample to be
    // shown it will always be 100%
    if(ChGetSampleRange(pChart) == 0)
        state = ~CH_PERCENT & state;

    // if the chart type is showing single data series turn off the legend
    // since there is no point of showing the legend of one data source.
    if(chartType == SHOW_SINGLE_BAR)
        state = ~CH_LEGEND & state;

    SetState(pChart, state);
    AssembleTitleString(pChart, DemoTitle);
    ChSetTitle(pChart, DemoTitle);
}

/************************************************************************
 Function: void UpdateGraphDemo(DWORD tick)
                                                                       
 Overview: This function is called by GOLDraw() to modify the data 
 		   shown in the Chart and redraw the Chart.
 		                                        
 Input: tick - this is used as the counter (time the change) value to 
 			   evaluate if the Chart data will be modified or not.
                                                                       
 Output: none
************************************************************************/
void UpdateGraphDemo(DWORD tick)
{
    static DWORD    prevTick = 0;
    CHART           *pChart = (CHART *)GOLFindObject(ID_CHART);

    if((tick - prevTick) > UPDATECHARTINTERVAL)
    {

        // update the samples being shown, reset the displayed samples once it reaches the
        // maximum sample count.
        if(ChGetSampleEnd(pChart) < DATASAMPLES)
        {
            ChSetSampleRange(pChart, ChGetSampleStart(pChart) + 1, ChGetSampleEnd(pChart) + 1);
            SetState(pChart, CH_DRAW_DATA);
        }
        else
        {
            SetUpNextDemo(pChart);
            SetState(pChart, CH_DRAW);
        }

        prevTick = tick;
    }
}

// copy or append string to destination from source
WORD xcharStrCopyAppend(XCHAR *pDest, XCHAR *pSrc, WORD copy)
{
    XCHAR   ch;
    WORD    length;
    XCHAR   *pointerFrom;
    XCHAR   *pointerTo;

    // Copy/Append and count length
    pointerFrom = pSrc;
    pointerTo = pDest;
    length = 0;

    if(copy == 0)
    {

        // find the end of the string to append the source to the destination
        while((ch = *pointerTo) != 0)
        {
            pointerTo++;
            length++;
        }
    }

    ch = *pointerFrom;

    // now copy/append the source to destination
    do
    {
        ch = *pointerFrom++;
        *pointerTo++ = ch;
        length++;
    } while(ch);

    return (length);
}

// Assemble all the strings to create the Title, the Title will show the features that is being

// shown in the demo.
void AssembleTitleString(CHART *pChart, XCHAR *pTitle)
{

    // this function assumes that pTitle is pointing to a string of length == CHARTTITLESTRLENGTH
    XCHAR   *pT = pTitle;

    if(GetState(pChart, CH_BAR) == CH_BAR)
    {

        // copy the string
        xcharStrCopyAppend(pT, (XCHAR *)BarStr, 1);
        if(GetState(pChart, CH_3D_ENABLE))
        {

            // append the string
            xcharStrCopyAppend(pT, (XCHAR *)ThreeDStr, 0);
        }
    }
    else if(GetState(pChart, CH_PIE) == CH_PIE)
    {

        // copy the string
        xcharStrCopyAppend(pT, (XCHAR *)PieStr, 1);
        if(GetState(pChart, CH_DONUT) == CH_DONUT)
        {
            xcharStrCopyAppend(pT, (XCHAR *)SpaceStr, 0);
            xcharStrCopyAppend(pT, (XCHAR *)DonutStr, 0);
        }
    }
    else
        return; // just in case Bar or Pie is not defined
    if(GetState(pChart, CH_LEGEND | CH_VALUE | CH_PERCENT))
    {

        // insert the space
        xcharStrCopyAppend(pT, (XCHAR *)SpaceStr, 0);

        // insert the open parenthesis
        xcharStrCopyAppend(pT, (XCHAR *)ParenOpenStr, 0);
    }

    if(GetState(pChart, CH_VALUE))
    {
        xcharStrCopyAppend(pT, (XCHAR *)ValueStr, 0);
    }

    if(GetState(pChart, CH_PERCENT))
    {
        if(GetState(pChart, CH_VALUE))
            xcharStrCopyAppend(pT, (XCHAR *)CommaStr, 0);
        xcharStrCopyAppend(pT, (XCHAR *)PercentStr, 0);
    }

    if(GetState(pChart, CH_LEGEND))
    {
        if(GetState(pChart, CH_BAR) || (GetState(pChart, CH_PIE) && (ChGetShowSeriesCount(pChart) > 1)))
        {
            if(GetState(pChart, CH_PERCENT | CH_VALUE))
                xcharStrCopyAppend(pT, (XCHAR *)CommaStr, 0);
            xcharStrCopyAppend(pT, (XCHAR *)LegendStr, 0);
        }
    }

    xcharStrCopyAppend(pT, (XCHAR *)ParenCloseStr, 0);
}
