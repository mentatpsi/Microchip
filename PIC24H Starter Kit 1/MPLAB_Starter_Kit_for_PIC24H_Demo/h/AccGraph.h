/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		AccGraph.h
* Dependencies:    		Header (.h) files if applicable, see below
* Processor:       		PIC24HJ128GP504
* Compiler:        		MPLAB® C30 v3.11b or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
************************************************************************/

/************************************************************************
 Header Includes													
 ************************************************************************/
#include "Main.h"
#include "Speaker.h"

/************************************************************************
 Constants													
 ************************************************************************/
#define GRAPH_DATA_BUFFER_SIZE      (GRAPH_GRID_WIDTH + 1)
#define NUM_AXES					3

#define GRAPH_GRID_LEFT             (GRAPH_PANEL_LEFT + 15 + GOL_EMBOSS_SIZE)
#define GRAPH_GRID_RIGHT            (GRAPH_PANEL_RIGHT - GOL_EMBOSS_SIZE)
#define GRAPH_GRID_TOP              (GRAPH_PANEL_LEFT + GOL_EMBOSS_SIZE)//(GRAPH_PANEL_TOP + GOL_EMBOSS_SIZE)
#define GRAPH_GRID_BOTTOM           (GRAPH_PANEL_BOTTOM - GOL_EMBOSS_SIZE)

#define GRAPH_GRID_WIDTH            (GRAPH_GRID_RIGHT - GRAPH_GRID_LEFT)
#define GRAPH_GRID_HEIGHT           (GRAPH_GRID_BOTTOM - GRAPH_GRID_TOP)

#define GRAPH_PANEL_LEFT            0
#define GRAPH_PANEL_RIGHT           GetMaxX()
#define GRAPH_PANEL_TOP             LOCATION_MIN_Y
#define GRAPH_PANEL_BOTTOM          GetMaxY()

#define GRAPH_UPDATE_INTERVAL_INIT  (100 / MILLISECONDS_PER_TICK)
#define GRAPH_UPDATE_INTERVAL_MAX   (2000 / MILLISECONDS_PER_TICK)
#define GRAPH_UPDATE_INTERVAL_MIN   (MILLISECONDS_PER_TICK / MILLISECONDS_PER_TICK)
#define GRAPH_UPDATE_INTERVAL_STEP  (MILLISECONDS_PER_TICK / MILLISECONDS_PER_TICK)

#define GRID_SPACING_VERTICAL       (GRAPH_GRID_HEIGHT / 6) 
#define GRID_SPACING_HORIZONTAL     (GRAPH_GRID_WIDTH >> 3) 

#define ID_GRAPH_HOME               21

/************************************************************************
 Function Prototypes													
 ************************************************************************/
void InitializeGraphScreen(void);
WORD ProcessMessageGraph(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
void ShowScreenGraph(void);
void TranslateMessageGraph(GOL_MSG *pMsg);
void UpdateGraph(void);
void DrawGridHorizontals(void);
void DrawGridVerticals(void);
void DrawGraphLine(void);
void GraphReadAccelerometer(void);



