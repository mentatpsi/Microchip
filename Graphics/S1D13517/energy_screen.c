/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        main_screen.c
 * Dependencies:    See Includes Section
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
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
 *****************************************************************************/

/*****************************************************************************
 * SECTION: Includes
 *****************************************************************************/
#include "Graphics/Graphics.h"
#include "MainDemo.h"
#include "gfx_schemes.h"
#include "gfx_strings.h"
#include "gfx_screens.h"
#include "gol_ids.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define ENERGY_EB_ORIGIN_X      (((GetMaxX() - 212 + 1) / 2) - 30)
#define ENERGY_EB_ORIGIN_Y      ((100) + 50)
#define ENERGY_ESCREEN_START    (ENERGY_EB_ORIGIN_X + 30) 
/*****************************************************************************
 * SECTION: Externs
 *****************************************************************************/
extern const FONT_FLASH     Large;          // equal width font
/*****************************************************************************
 * SECTION: Variables
 *****************************************************************************/
GFX_COLOR    MyColorTable[16] = 
{   
    BRIGHTBLUE, BRIGHTBLUE, BRIGHTBLUE, BRIGHTBLUE, 
    BRIGHTBLUE, BRIGHTBLUE, BRIGHTBLUE, YELLOW, 
    RED, BLUE, BRIGHTGREEN, BRIGHTGREEN, 
    BRIGHTYELLOW, BRIGHTRED, BRIGHTBLUE, BLACK
};
WORD samples1[] = { 15, 4, 27, 10, 20, 25, 35 };

/*****************************************************************************
 * void CreateEnergyScreen(void)
 *****************************************************************************/
void CreateEnergyScreen(void)
{
    CHART   *pMyChart;
    GOL_SCHEME *currentScheme;
    WORD state;

    currentScheme = GFX_SchemeGetCurrentScheme();

    SetState(GOLFindObject(PANEL_SCREEN_ID_ENERGY_USAGE_BUT), BTN_DISABLED|BTN_DRAW);

    SetColor(currentScheme->Color0);

    while(!FillBevel((GetMaxX() >> 2) + 20,90 ,GetMaxX() - 10, GetMaxY()-10,5));				     	

	while(!AlphaBlendWindow(GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
					 GFX_PAGE1, (GetMaxX() >> 2) + 15, 85,
					 GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
				     (GetMaxX())-((GetMaxX() >> 2) + 15), 
				     GetMaxY() - 90,  	
				     GFX_SchemeGetDefaultScheme()->AlphaValue));

    if(GetState(GOLFindObject(PANEL_SCREEN_ID_COMFORT_BUT),BTN_DISABLED))
    {
        ClrState(GOLFindObject(PANEL_SCREEN_ID_COMFORT_BUT), BTN_DISABLED);
        SetState(GOLFindObject(PANEL_SCREEN_ID_COMFORT_BUT),BTN_DRAW);
    }

    if(GetState(GOLFindObject(PANEL_SCREEN_ID_SECURITY_BUT),BTN_DISABLED))
    {
        ClrState(GOLFindObject(PANEL_SCREEN_ID_SECURITY_BUT), BTN_DISABLED);
        SetState(GOLFindObject(PANEL_SCREEN_ID_SECURITY_BUT),BTN_DRAW);
    }

    if(GetState(GOLFindObject(PANEL_SCREEN_ID_LIGHTING_BUT),BTN_DISABLED))
    { 
        ClrState(GOLFindObject(PANEL_SCREEN_ID_LIGHTING_BUT), BTN_DISABLED);
        SetState(GOLFindObject(PANEL_SCREEN_ID_LIGHTING_BUT),BTN_DRAW);
    }

    SetColor(currentScheme->Color0);
    while(!FillBevel(ENERGY_ESCREEN_START+10 , ENERGY_EB_ORIGIN_Y , ENERGY_ESCREEN_START + 125, ENERGY_EB_ORIGIN_Y+70,20));
    while(!FillBevel(ENERGY_ESCREEN_START +210+10,  ENERGY_EB_ORIGIN_Y , ENERGY_ESCREEN_START + 335, ENERGY_EB_ORIGIN_Y+70,20));

    SetFont((void *) &FONTDEFAULT);
    SetColor(RGB565CONVERT(255, 102, 0));//SetColor(WHITE);
    OutTextXY(ENERGY_ESCREEN_START + 5,ENERGY_EB_ORIGIN_Y-20,(XCHAR *)TodaysUsageStr);
    OutTextXY(ENERGY_ESCREEN_START + 210,ENERGY_EB_ORIGIN_Y-20,(XCHAR *)MonthlyUsageStr);
    OutTextXY(ENERGY_ESCREEN_START +115,ENERGY_EB_ORIGIN_Y+93,(XCHAR *)UsageHistoryStr);
    OutTextXY(ENERGY_ESCREEN_START + 90,ENERGY_EB_ORIGIN_Y+40,(XCHAR *)kwhStr);
    OutTextXY(ENERGY_ESCREEN_START + 310,ENERGY_EB_ORIGIN_Y+40,(XCHAR *)kwhStr);
    SetFont((void *) &Large);
    OutTextXY(ENERGY_ESCREEN_START + 5,ENERGY_EB_ORIGIN_Y+10,(XCHAR *)D20Str);
    OutTextXY(ENERGY_ESCREEN_START + 200,ENERGY_EB_ORIGIN_Y+10,(XCHAR *)M230Str);

    #if (COLOR_DEPTH >= 16)
        state = CH_BAR | CH_DRAW | CH_3D_ENABLE;
    #else
        state = CH_BAR | CH_DRAW;
    #endif

    // create the chart object
    pMyChart =   ChCreate(
                            ENERGY_SCREEN_ID_CHART,
                            ENERGY_ESCREEN_START - 5,
                            ENERGY_EB_ORIGIN_Y+120,  // dimensions
                            ENERGY_ESCREEN_START + 335,
                            GetMaxY() - 20,   //
                            CH_BAR | CH_DRAW | CH_3D_ENABLE,   // state of the chart
                            NULL,                               // data not initialized yet
                            NULL,                               // no paraters and data yet
                            currentScheme//ComfortScheme
                            );                              // style scheme used

    ChSetTitle(pMyChart, (XCHAR*)spaceStr);

    ChSetTitleFont(pMyChart, (void *) &GOLSmallFont);

    // set the grid labels and axis labels font	
    ChSetGridLabelFont(pMyChart, (void *) &GOLSmallFont);
    ChSetAxisLabelFont(pMyChart, (void *) &GOLSmallFont);

    // set the sample label and value label strings
    ChSetSampleLabel(pMyChart, (XCHAR *)DaysStr);
    ChSetValueLabel(pMyChart, (XCHAR *)kwhStr);

    // add the variables and the data of the variables. This series of function calls will
    // create the link list of variable structures.
    ChAddDataSeries(pMyChart, 15, samples1, (XCHAR *)Var1Name);

    // set the color table to be used. Note that if this is not called, the default
    // color table will be used. (see ChartVarClr[] in Chart.c file)
    ChSetColorTable(pMyChart, MyColorTable);

    // set the range of the values
    ChSetValueRange(pMyChart, 0, 40);

    // set the data series to show, +1 to cover the possibility of temp = 0.
    // data series reference starts from 1 not zero.
    ChShowSeries(pMyChart, 1);
    ChSetSampleRange(pMyChart, 1, 7);      // set the first and last sample to be displayed

}
