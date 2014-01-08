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
#include "ID.h"

#include "gfx_font_resources.h"

/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define DATASAMPLES     15
/*****************************************************************************
 * SECTION: Scheme
 *****************************************************************************/
extern const GOL_SCHEME chartScheme;
/*****************************************************************************
 * SECTION: Strings
 *****************************************************************************/
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
const XCHAR             XName[] = {'C','a','t','e','g','o','r','y',0};
const XCHAR             YName[] = {'V','a','l','u','e','s',0};
/*****************************************************************************
 * SECTION: Samples
 *****************************************************************************/
WORD                    samples1[] = {1,   4,  7, 10, 20, 25, 35, 46, 50, 42, 37, 24, 17, 9,  2};
WORD                    samples2[] = {10, 25, 15,  5, 50,  0,  1,  0,  0, 35, 30,  7,  8,  9, 10};
WORD                    samples3[] = {48, 40, 30, 30, 25, 18, 12,  8,  9,  2, 28, 32, 45, 33, 37};
/*****************************************************************************
 * SECTION: Color Table
 *****************************************************************************/
// define the color set used for the Chart, note that this is application defined in this demo code.
// if application do not set the colors used (in ChSetColorTable()) the default colors will be used
GFX_COLOR MyColorTable[16] = 
{
    0x00,
    0x04,
    0x0A,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09,
    0x0A,
    0x0B,
    0x0C,
    0x0D,
    0x0E,
    0x0F,
};

/*****************************************************************************
 * void CreateFontScreen(void)
 *****************************************************************************/
void CreateChart( void )
{
	CHART * pMyChart = ChCreate(ID_CHART,
								30,
								40,  // dimensions
								290,
								230,   //
								CH_BAR | CH_VALUE | CH_PERCENT | CH_DRAW,   // state of the chart
								NULL,                               // data not initialized yet
								NULL,                               // no paraters and data yet
								(GOL_SCHEME *)&chartScheme
								);   
	
	if( pMyChart )
	{
		ChSetTitle(pMyChart, "Bar Graph Demo");
		ChSetTitleFont(pMyChart, (void *) &FONTDEFAULT);
		
		// set the grid labels and axis labels font	
		ChSetGridLabelFont(pMyChart, (void *) &DroidSans_12);
		ChSetAxisLabelFont(pMyChart, (void *) &DroidSans_12);
		
		// set the sample label and value label strings
		ChSetSampleLabel(pMyChart, (XCHAR *)XName);
		ChSetValueLabel(pMyChart, (XCHAR *)YName);
		
		// add the variables and the data of the variables. This series of function calls will
		// create the link list of variable structures.
		ChAddDataSeries(pMyChart, DATASAMPLES, samples1, (XCHAR *)Var1Name);
		ChAddDataSeries(pMyChart, DATASAMPLES, samples2, (XCHAR *)Var2Name);
		ChAddDataSeries(pMyChart, DATASAMPLES, samples3, (XCHAR *)Var3Name);
		
		// set the color table to be used. Note that if this is not called, the default
		// color table will be used. (see ChartVarClr[] in Chart.c file)
		ChSetColorTable(pMyChart, MyColorTable);
		
		// set the range of the values
		ChSetValueRange(pMyChart, 0, 50);
		ChSetPercentRange(pMyChart, 0, 75); // ChSetPerRange(pMyChart, 40, 60);
		ChSetSampleRange(pMyChart, 1, 3);
	}
}
/*****************************************************************************
 * void DeleteChart( void )
 *****************************************************************************/
void DeleteChart( void )
{
	GOLDeleteObjectByID(ID_CHART); 
}
