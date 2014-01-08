/*********************************************************************
 * Microchip Graphic Library Demo Application
 * This is the header file used in the Animation demo.
 *********************************************************************
 * FileName:        GraphShow.h
 * Dependencies:    MainDemo.h
 * Processor:       PIC24/PIC32
 * Compiler:        C30/C32
 * Company:         Microchip Technology, Inc.
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
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Paolo Tamayo         01/29/09    ...
 ********************************************************************/
#ifndef GRAPHSHOW_H
    #define GRAPHSHOW_H

    #include "Graphics/Graphics.h"

////////////////////////////////////////////////////////////////////////////
//                             FONTS USED
/////////////////////////////////////////////////////////////////////////////
extern const FONT_FLASH GOLFontDefault;         // default GOL font
extern const FONT_FLASH GOLMediumFont;          // medium font
extern const FONT_FLASH GOLSmallFont;           // small font

    #define UPDATECHARTINTERVAL (2500)
    #define CHARTTITLESTRLENGTH (30)


/////////////////////////////////////////////////////////////////////////////
//                            DEMO TYPES
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    BAR_LEGEND_PERCENT_VERTICAL         = (CH_BAR | CH_LEGEND | CH_VALUE | CH_PERCENT),
    BAR_LEGEND_NPERCENT_VERTICAL        = (CH_BAR | CH_LEGEND | CH_VALUE),
    BAR_NLEGEND_PERCENT_VERTICAL        = (CH_BAR | CH_VALUE | CH_PERCENT),
    BAR_LEGEND_PERCENT_HORIZONTAL       = (CH_BAR | CH_LEGEND | CH_VALUE | CH_PERCENT | CH_BAR_HOR),
    BAR_LEGEND_NPERCENT_HORIZONTAL      = (CH_BAR | CH_LEGEND | CH_VALUE | CH_BAR_HOR),
    BAR_NLEGEND_PERCENT_HORIZONTAL      = (CH_BAR | CH_VALUE | CH_PERCENT | CH_BAR_HOR),
    BAR_3D_LEGEND_PERCENT_VERTICAL      = (CH_BAR | CH_LEGEND | CH_VALUE | CH_PERCENT | CH_3D_ENABLE),
    BAR_3D_LEGEND_NPERCENT_VERTICAL     = (CH_BAR | CH_LEGEND | CH_VALUE | CH_3D_ENABLE),
    BAR_3D_NLEGEND_PERCENT_VERTICAL     = (CH_BAR | CH_VALUE | CH_PERCENT | CH_3D_ENABLE),
    BAR_3D_LEGEND_PERCENT_HORIZONTAL    = (CH_BAR | CH_LEGEND | CH_VALUE | CH_PERCENT | CH_BAR_HOR | CH_3D_ENABLE),
    BAR_3D_LEGEND_NPERCENT_HORIZONTAL   = (CH_BAR | CH_LEGEND | CH_VALUE | CH_BAR_HOR | CH_3D_ENABLE),
    BAR_3D_NLEGEND_PERCENT_HORIZONTAL   = (CH_BAR | CH_VALUE | CH_PERCENT | CH_BAR_HOR | CH_3D_ENABLE),
    PIE_NLEGEND_PERCENT                 = (CH_PIE | CH_VALUE | CH_PERCENT),
    PIE_LEGEND_PERCENT                  = (CH_PIE | CH_LEGEND | CH_VALUE | CH_PERCENT | CH_DONUT),
    PIE_LEGEND_NPERCENT                 = (CH_PIE | CH_LEGEND | CH_VALUE),
    PIE_NLEGEND_NPERCENT                = (CH_PIE | CH_VALUE | CH_DONUT),
} CHART_DEMO_TYPES;

/*///////////////////////////////////////////////////////////////////////////
//                            CHART TYPES      
///////////////////////////////////////////////////////////////////////////*/
typedef enum
{
    SHOW_SINGLE_BAR                     = 0x50, // Bar Chart with single variable
    SHOW_MULTIPLE_BAR,                          // Bar Chart with multiple variables
    SHOW_SINGLE_PIE,                            // Pie Chart with single variable
    SHOW_MULTIPLE_PIE,                          // Pie Chart with multiple variables
} CHART_TYPES;

/************************************************************************
 Function: void InitGraphDemoStyleScheme(GOL_SCHEME *pScheme)
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void    InitGraphDemoStyleScheme(GOL_SCHEME *pScheme);

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
WORD    GraphMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: void UpdateGraphDemo(DWORD tick)
                                                                       
 Overview: This function is called by GOLDraw() to modify the data 
 		   shown in the Chart and redraw the Chart.
 		                                        
 Input: tick - this is used as the counter (time the change) value to 
 			   evaluate if the Chart data will be modified or not.
                                                                       
 Output: none
************************************************************************/
void    UpdateGraphDemo(DWORD tick);

/************************************************************************
 Function: WORD CreateGraphDemo(void)
                                                                       
 Overview: This function creates the Chart demo.
 		                                                                          
 Input: none
                                                                       
 Output: Return value indicates if the creation of the objects in this
         demo was successful. If not successful, the calling function
         can recover and not continue the demo. 
************************************************************************/
WORD    CreateGraphDemo(void);
#endif // GRAPHSHOW_H
