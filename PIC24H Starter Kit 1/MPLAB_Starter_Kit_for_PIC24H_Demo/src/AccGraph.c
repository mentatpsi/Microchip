/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		AccGraph.c
* Description:			This file contains the main entry function which
* Dependencies:    		Header (.h) files if applicable, see below
* Processor:       		PIC24FJ128GP504
* Compiler:        		MPLAB® C30 v3.00 or higher
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
*
************************************************************************/

/************************************************************************
 Header Includes													
 ************************************************************************/
#include "AccGraph.h"

/************************************************************************
 Variables													
 ************************************************************************/
int 						bufferXVal[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int 						bufferYVal[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int 						bufferZVal[8] = {0, 0, 0, 0, 0, 0, 0, 0};
BYTE                		graphData[NUM_AXES][GRAPH_DATA_BUFFER_SIZE];
WORD                		graphDataHead;
WORD                		graphDataTail;
BYTE                		graphOffset;
WORD                		graphUpdateRate;
volatile WORD       		graphXVal;
volatile WORD       		graphYVal;
volatile WORD       		graphZVal;

extern int 					enableSoundFlag;
extern DWORD            	previousTick;
extern DWORD               	tick;
extern SCREEN_STATES       	screenState;
extern PAGE_INFORMATION    	pageInformation;

/****************************************************************************
  Function:
    void InitializeGraphScreen( void )
  Description:
    This function initializes the GOL parameters.
  Precondition:
    None
  Parameters:
    None
  Returns:
    None
  Remarks:
    The global list of graphics objects is destroyed and replaced.  Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/
void InitializeGraphScreen(void)
{
    GOLFree();
    SetColor(BLACK);        // set color to BLACK
    ClearDevice();          // set screen to all BLACK


    StCreate(       ID_TITLE,
                    0, (GRAPH_GRID_HEIGHT/6)-5, GetMaxX()/10, (GRAPH_GRID_HEIGHT/6)-5+GetTextHeight( (void *)&FONTDEFAULT )-1,
                    ST_DRAW,
                    "X",
                    NULL );
               
    StCreate(       ID_TITLE,
                    0, 3*(GRAPH_GRID_HEIGHT/6)-5, GetMaxX()/10, 3*(GRAPH_GRID_HEIGHT/6)-5+GetTextHeight( (void *)&FONTDEFAULT )-1,
                    ST_DRAW,
                    "Y",
                    NULL );    
               
    StCreate(       ID_TITLE,
                    0, 5*(GRAPH_GRID_HEIGHT/6)-5, GetMaxX()/10, 5*(GRAPH_GRID_HEIGHT/6)-5+GetTextHeight( (void *)&FONTDEFAULT )-1,
                    ST_DRAW,
                    "Z",
                    NULL );                
                        
    previousTick = tick; 
}

/****************************************************************************
  Function:
    void ShowScreenGraph( void )
  Description:
    This function erases the currently displayed screen and replaces it with
    the Accelerometer output signals graph.  It draws the initial grid, and 
    initializes the variables needed for the demo.
  Precondition:
    None
  Parameters:
    None
  Returns:
    None
  Remarks:
    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/
void ShowScreenGraph(void)
{
	int i;
	
    InitializeGraphScreen();
  
    // Create the hidden controls to receive witch messages.
    BtnCreate(          ID_GRAPH_HOME,
                        GetMaxX()+1, 0, GetMaxX()+1, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    GOLPanelDraw( GRAPH_PANEL_LEFT+15, GRAPH_PANEL_LEFT, GRAPH_PANEL_RIGHT, GRAPH_PANEL_BOTTOM, 0,
        BLACK, NULL, NULL, NULL, GOL_EMBOSS_SIZE);

    GOLPanelDrawTsk();          // draw panel for graph

    // Draw the initial grid.
    SetColor( WHITE );
    SetLineType( DOTTED_LINE );

    previousTick    = tick;
    graphOffset     = 0;
    graphDataHead   = 0;
    graphDataTail   = 0;
    graphUpdateRate = GRAPH_UPDATE_INTERVAL_INIT;

    DrawGridVerticals();
    DrawGridHorizontals();
    
    for(i = 7; i >= 0; i--)
		bufferXVal[i] = bufferYVal[i] = bufferZVal[i] = 0;
}

/****************************************************************************
  Function:
    void TranslateMessageGraph( GOL_MSG *pMsg )
  Description:
    If the message is valid, this function translates the raw message
    pMsg->param1 to the proper message for the appropriate control.
  Precondition:
    ShowScreenGraph() must be the last screen display routine called before
    this function executes.
  Parameters:
    GOL_MSG* pMsg - The original system message information
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void TranslateMessageGraph(GOL_MSG *pMsg)
{
    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_ACCELEROMETER_PAD))
    {
		if (pMsg->param2 == SCAN_CR_PRESSED)
        {
            pMsg->param1 = ID_GRAPH_HOME;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
      	else if (pMsg->param2 == SCAN_CRA_PRESSED)
        {
           pMsg->param1 = ID_GRAPH_HOME;
           pMsg->param2 = SCAN_CRA_PRESSED;
      	}
    }
}

/****************************************************************************
  Function:
    WORD ProcessMessageGraph( WORD translatedMsg, OBJ_HEADER* pObj,
                    		  GOL_MSG* pMsg )
  Description:
    This function processes the switch press message.
  Precondition:
    Call ShowScreenGraph() prior to using this function to display the
    correct screen.
  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information
  Return Values:
    0 - Do not call the default message handler.
    1 - Call the default message handler.
  Remarks:
    None
  ***************************************************************************/
WORD ProcessMessageGraph(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
    WORD    controlID;

    controlID = GetObjID(pObj);

    switch (controlID)
    {
        case ID_GRAPH_HOME:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
		        if(enableSoundFlag)
		        {
					if(!speakerBusy())
						speakerActivate(SPEECH_ADDR_SELECT, SPEECH_SIZE_SELECT);  
				}
	            AccelerometerInit();
                screenState = SCREEN_DISPLAY_MAIN;
            }
            break;

        default:
            break;
    }

    return 0;
}

/****************************************************************************
  Function:
    void UpdateGraph( void )
  Description:
    This routine updates the display of the scrolling graph.  The update rate
    is controlled by the variable graphUpdateRate.
  Precondition:
    The graph information (graphData, graphDataHead, and graphDataTail) must
    be valid.
  Parameters:
    None
  Returns:
    None
  Remarks:
    Since normally a graph's 0 point is on the bottom, but our (0,0) is the
    upper left corner, we need to flip the graph.  
  ***************************************************************************/
void UpdateGraph(void)
{
    GOLPanelDraw( GRAPH_PANEL_LEFT+15, GRAPH_PANEL_LEFT, GRAPH_PANEL_RIGHT, GRAPH_PANEL_BOTTOM, 0,
    			  BLACK, NULL, NULL, NULL, GOL_EMBOSS_SIZE);
    
    if ((tick - previousTick ) > graphUpdateRate)
    {
        previousTick = tick;

        // Remove the old graph and vertical grid lines
        SetColor( BLACK );
        DrawGraphLine();
        DrawGridVerticals();

        // Get the latest accelerometer reading.
        graphData[0][graphDataTail] = (((graphXVal-420)*GRAPH_GRID_HEIGHT)/(3*180));
        graphData[1][graphDataTail] = (((graphYVal-420)*GRAPH_GRID_HEIGHT)/(3*180));
        graphData[2][graphDataTail++] = (((graphZVal-420)*GRAPH_GRID_HEIGHT)/(3*180));
        if (graphDataTail == GRAPH_DATA_BUFFER_SIZE)
        {
            graphDataTail = 0;
        }

        // When the graph is full, update the head pointer to wrap and the display offset
        if (graphDataHead == graphDataTail)
        {
            graphDataHead ++;
            if (graphDataHead == GRAPH_DATA_BUFFER_SIZE)
            {
                graphDataHead = 0;
            }

            graphOffset ++;
            if (graphOffset >= GRID_SPACING_HORIZONTAL)
            {
                graphOffset = 0;
            }
        }

        // Draw the new graph
        SetColor( WHITE );
        DrawGraphLine();

        // Draw the updated grid
        SetColor( WHITE );
        DrawGridVerticals();
        DrawGridHorizontals();
    }    
}


/****************************************************************************
  Function:
    void DrawGridHorizontals( void )
  Description:
    This routine draws the horizontal lines on the scrolling graph.  The
    routine uses the current line color, so it can be used to erase the
    current horizontal lines by first calling SetColor( BLACK ).
  Precondition:
    The line color must be set prior to calling this routine.
  Parameters:
    None
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void DrawGridHorizontals(void)
{
    SHORT   y;

    SetLineType( DOTTED_LINE );
    for (y = GRAPH_GRID_TOP + GRID_SPACING_VERTICAL; y < GRAPH_GRID_BOTTOM; y += GRID_SPACING_VERTICAL)
    {
        Line( GRAPH_GRID_LEFT, y, GRAPH_GRID_RIGHT, y );
    }
}

/****************************************************************************
  Function:
    void DrawGridVerticals( void )
  Description:
    This routine draws the vertical lines on the scrolling graph.  Since the
    graph is scrolling, the verticals scroll to match.  The current scroll
    offset is given by the variable graphOffset.  The routine uses the
    current line color, so it can be used to erase the current vertical lines
    by first calling SetColor( BLACK ).
  Precondition:
    The line color and scroll offset (graphOffset) must be set prior to
    calling this routine.
  Parameters:
    None
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void DrawGridVerticals( void )
{
    SHORT   x;

    SetLineType( DOTTED_LINE );
    for (x = GRAPH_GRID_LEFT + GRID_SPACING_HORIZONTAL - graphOffset; x < GRAPH_GRID_RIGHT; x += GRID_SPACING_HORIZONTAL)
    {
        Line( x, GRAPH_GRID_TOP, x, GRAPH_GRID_BOTTOM );
    }
}

/****************************************************************************
  Function:
    void DrawGraphLine( void )
  Description:
    This data draws the line graph of the current graph data.  The routine
    uses the current line type and color, so it can be used to erase a
    line by first calling SetColor( BLACK ).
  Precondition:
    The line color must be set prior to calling this routine.  The graph
    variables graphData, graphDataHead, and graphDataTail, must also be
    valid.
  Parameters:
    None
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void DrawGraphLine(void)
{
    static SHORT    previousY_X;
    static SHORT    previousY_Y;
    static SHORT    previousY_Z;
    WORD            tempHead;
    SHORT           x;
    SHORT           y;

    tempHead = graphDataHead;

    for (x = 0; (x < GRAPH_GRID_WIDTH) && (tempHead != graphDataTail); x++)
    {
        if (x == 0)
        {
            PutPixel( GRAPH_GRID_LEFT + x, (GRAPH_GRID_BOTTOM/3)-(graphData[0][tempHead])+2 );
            PutPixel( GRAPH_GRID_LEFT + x, 2*(GRAPH_GRID_BOTTOM/3)-(graphData[1][tempHead])+2 );
            PutPixel( GRAPH_GRID_LEFT + x, 3*(GRAPH_GRID_BOTTOM/3)-(graphData[2][tempHead])+2 );
        }
        else
        {
            if (graphData[0][tempHead] > previousY_X)
                for (y = previousY_X + GRAPH_GRID_TOP; y <= graphData[0][tempHead] + GRAPH_GRID_TOP; y++)
                    PutPixel( GRAPH_GRID_LEFT + x, (GRAPH_GRID_BOTTOM/3)-y+2 );
            else
                for (y = previousY_X + GRAPH_GRID_TOP; y >= graphData[0][tempHead] + GRAPH_GRID_TOP; y--)
                    PutPixel( GRAPH_GRID_LEFT + x, (GRAPH_GRID_BOTTOM/3)-y+2 );
                    
            if (graphData[1][tempHead] > previousY_Y)
                for (y = previousY_Y + GRAPH_GRID_TOP; y <= graphData[1][tempHead] + GRAPH_GRID_TOP; y++)
                    PutPixel( GRAPH_GRID_LEFT + x, 2*(GRAPH_GRID_BOTTOM/3)-y+2 );            
            else        
                for (y = previousY_Y + GRAPH_GRID_TOP; y >= graphData[1][tempHead] + GRAPH_GRID_TOP; y--)
                    PutPixel( GRAPH_GRID_LEFT + x, 2*(GRAPH_GRID_BOTTOM/3)-y+2 );
                    
            if (graphData[2][tempHead] > previousY_Z)   
                for (y = previousY_Z + GRAPH_GRID_TOP; y <= graphData[2][tempHead] + GRAPH_GRID_TOP; y++)
                    PutPixel( GRAPH_GRID_LEFT + x, 3*(GRAPH_GRID_BOTTOM/3)-y+2 );     
            else
                for (y = previousY_Z + GRAPH_GRID_TOP; y >= graphData[2][tempHead] + GRAPH_GRID_TOP; y--)
                    PutPixel( GRAPH_GRID_LEFT + x, 3*(GRAPH_GRID_BOTTOM/3)-y+2 );
        }

        previousY_X = graphData[0][tempHead];
        previousY_Y = graphData[1][tempHead];
        previousY_Z = graphData[2][tempHead];
        
        tempHead ++;
        if (tempHead == GRAPH_GRID_WIDTH+1)
        {
            tempHead = 0;
        }
    }  
}



/****************************************************************************
  Function:
    void GraphReadAccelerometer( void )
  Description:
    This function reads the Accelerometer and stores the current tri-axial values into
    a global variable for later display on the scrolling graph.  
  Precondition:
    None
  Parameters:
    None
  Returns:
    None
  Remarks:
    This routine is called from the tick interrupt.
  ***************************************************************************/
void GraphReadAccelerometer(void)
{
	int i;
	
	// Sample Accelerometer X-axis output
    AD1CON1             = 0x00E4;   // Off, Auto sample start, auto-convert
    AD1CON2             = 0;        // AVdd, AVss, int every conversion, MUXA only
    AD1CON3             = 0x1F05;   // 31 Tad auto-sample, Tad = 5*Tcy
    AD1CHS0             = 0x0004;   // MUXA uses AN4
    AD1CSSL             = 0;        // No scanned inputs
    AD1CON1bits.ADON    = 1;        // Turn on module

    while(!AD1CON1bits.DONE);       // Wait for conversion to complete

	for(i = 7; i > 0; i--)
		bufferXVal[i] = bufferXVal[i-1];
	bufferXVal[i] = ADC1BUF0;
	graphXVal = VectorAvg(8, bufferXVal);
	
    AD1CON1bits.ADON    = 0;        // Turn off module


	// Sample Accelerometer Y-axis output        
    AD1CON1             = 0x00E4;   // Off, Auto sample start, auto-convert
    AD1CON2             = 0;        // AVdd, AVss, int every conversion, MUXA only
    AD1CON3             = 0x1F05;   // 31 Tad auto-sample, Tad = 5*Tcy
    AD1CHS0             = 0x0005;   // MUXA uses AN5
    AD1CSSL             = 0;        // No scanned inputs
    AD1CON1bits.ADON    = 1;        // Turn on module

    while(!AD1CON1bits.DONE);       // Wait for conversion to complete

	for(i = 7; i > 0; i--)
		bufferYVal[i] = bufferYVal[i-1];
	bufferYVal[i] = ADC1BUF0;
	graphYVal = VectorAvg(8, bufferYVal);
	
    AD1CON1bits.ADON    = 0;        // Turn off module


   	// Sample Accelerometer Z-axis output
    AD1CON1             = 0x00E4;   // Off, Auto sample start, auto-convert
    AD1CON2             = 0;        // AVdd, AVss, int every conversion, MUXA only
    AD1CON3             = 0x1F05;   // 31 Tad auto-sample, Tad = 5*Tcy
    AD1CHS0             = 0x0006;   // MUXA uses AN6
    AD1CSSL             = 0;        // No scanned inputs
    AD1CON1bits.ADON    = 1;        // Turn on module

    while(!AD1CON1bits.DONE);       // Wait for conversion to complete

	for(i = 7; i > 0; i--)
		bufferZVal[i] = bufferZVal[i-1];
	bufferZVal[i] = ADC1BUF0;
	graphZVal = VectorAvg(8, bufferZVal);
	
    AD1CON1bits.ADON    = 0;        // Turn off module
    
    
    if((screenState == SCREEN_GRAPH))
	{
	   if(graphXVal > 645 || graphYVal > 645 || graphZVal > 645)
	   {
			if(enableSoundFlag)
				if(!speakerBusy())
					speakerActivate(SPEECH_ADDR_SNAKE_MOVE, SPEECH_SIZE_SNAKE_MOVE);
	   }
	   else if(graphXVal < 375 || graphYVal < 375 || graphZVal < 375)
	   {
			if(enableSoundFlag)
				if(!speakerBusy())
					speakerActivate(SPEECH_ADDR_SNAKE_EAT, SPEECH_SIZE_SNAKE_EAT);
	   }
	   
	    if(graphXVal > 600)
	    	graphXVal = 600;
	    else if(graphXVal < 420)
	    	graphXVal = 420;
	   	
	   	if(graphYVal > 600)
	    	graphYVal = 600;
	    else if(graphYVal < 420)
	    	graphYVal = 420;
	    	
	   	if(graphZVal > 600)
	    	graphZVal = 600;
	    else if(graphZVal < 420)
	    	graphZVal = 420;
	    	

	}
}




