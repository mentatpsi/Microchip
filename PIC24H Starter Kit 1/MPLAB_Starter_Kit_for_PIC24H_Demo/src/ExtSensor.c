/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		ExtSensor.c
* Description:			This file contains the main entry function which
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
*
************************************************************************/

/************************************************************************
 Header Includes													
 ************************************************************************/
#include "ExtSensor.h"

/************************************************************************
 Variables													
 ************************************************************************/
unsigned int 				BufferA1[GRAPH_DATA_BUFFER_SIZE] __attribute__((space(dma)));	// Ping-pong buffer A1
unsigned int 				BufferB1[GRAPH_DATA_BUFFER_SIZE] __attribute__((space(dma)));	// Ping-pong buffer B1
volatile WORD       		graphVal;

extern DWORD               	previousTick;
extern DWORD               	tick;
extern SCREEN_STATES       	screenState;
extern BYTE                 graphData[NUM_AXES][GRAPH_DATA_BUFFER_SIZE];
extern WORD                 graphDataHead;
extern WORD                 graphDataTail;
extern BYTE                 graphOffset;
extern WORD                 graphUpdateRate;
extern int 				    enableSoundFlag;
extern int 					SwitchS1Flag;
extern int 					SwitchS2Flag;
extern unsigned int 		DmaBuffer;
extern int 					flag;

/****************************************************************************
  Function:
    void ShowScreenESInfo( void )
  Description:
    This function erases the currently displayed screen and replaces it with
    the external sensor connect information screen.  
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
void ShowScreenESInfo(void)
{
	extern IMAGE_FLASH iconMicrochip;
	
	GOLFree();
    SetColor(BLACK);        // set color to BLACK
    ClearDevice();          // set screen to all BLACK

    PictCreate(     ID_ICON,                    // ID
                    0,0,15,15,                  // dimension
                    PICT_DRAW,                  // will be displayed, has frame
                    1,                          // scale factor is x1
                    &iconMicrochip,             // bitmap
                    NULL );                     // default GOL scheme

    StCreate(       ID_TITLE,
                    16, 0, GetMaxX()-11, GetTextHeight( (void *)&FONTDEFAULT )-1,
                    ST_DRAW,
                    "External Sensor Demo",
                    NULL );
                    
    StCreate(		ID_TITLE,                       // ID
	                0,                              // left
	                20,                             // top
	                GetMaxX(),                      // right
	                20+GetTextHeight( (void *)&FONTDEFAULT ),  // bottom
	                ST_DRAW,
	                "Connect External Sensor...",
	                NULL );
}

/****************************************************************************
  Function:
    void InitializeESGraphScreen( void )
  Description:
    This function initializes the GOL parameters for External Sensor screen.
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
void InitializeESGraphScreen(void)
{
    GOLFree();
    SetColor(BLACK);        // set color to BLACK
    ClearDevice();          // set screen to all BLACK


    StCreate(       ID_TITLE,
                    0, (GRAPH_GRID_HEIGHT/6)-5, GetMaxX()/10, (GRAPH_GRID_HEIGHT/6)-5+GetTextHeight( (void *)&FONTDEFAULT )-1,
                    ST_DRAW,
                    "V",
                    NULL );
                              
    previousTick = tick; 
}

/****************************************************************************
  Function:
    void ShowScreenESInfo( void )
  Description:
    This function erases the currently displayed screen and replaces it with
    the external sensor output signal graph.  It draws the initial grid, and 
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
void ShowScreenESGraph(void)
{
    InitializeESGraphScreen();
  
    // Create the hidden controls to receive switch messages.
    BtnCreate(          ID_GRAPH_HOME,
                        GetMaxX()+1, 0, GetMaxX()+1, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    // Create the hidden controls to receive switch messages.
    BtnCreate(          ID_GRAPH_REFRESH_RATE_CHANGE,
                        GetMaxX()+4, 0, GetMaxX()+4, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );
                        
    GOLPanelDraw( GRAPH_PANEL_LEFT+15, GRAPH_PANEL_LEFT/*GRAPH_PANEL_TOP*/, GRAPH_PANEL_RIGHT, GRAPH_PANEL_BOTTOM, 0,
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
}

/****************************************************************************
  Function:
    WORD ProcessMessageESGraph( WORD translatedMsg, OBJ_HEADER* pObj,
                    		    GOL_MSG* pMsg )
  Description:
    This function processes the switch press message.
  Precondition:
    Call ShowScreenESGraph() prior to using this function to display the
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
WORD ProcessMessageESGraph(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
    WORD    controlID;

    controlID = GetObjID(pObj);

    switch (controlID)
    {
        case ID_GRAPH_REFRESH_RATE_CHANGE:
            if (translatedMsg == BTN_MSG_PRESSED)
            {	
					if(graphUpdateRate <= GRAPH_UPDATE_INTERVAL_MAX && graphUpdateRate >= GRAPH_UPDATE_INTERVAL_MIN)
						graphUpdateRate += GRAPH_UPDATE_INTERVAL_STEP;
					else
						graphUpdateRate = GRAPH_UPDATE_INTERVAL_MIN;
            }
            break;

        case ID_GRAPH_HOME:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
		        if(enableSoundFlag)
					if(!speakerBusy())
						speakerActivate(SPEECH_ADDR_SELECT, SPEECH_SIZE_SELECT);  
	            AccelerometerStop();
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
    void TranslateMessageESGraph( GOL_MSG *pMsg )
  Description:
    If the message is valid, this function translates the raw message
    pMsg->param1 to the proper message for the appropriate control.
  Precondition:
    ShowScreenESGraph() must be the last screen display routine called before
    this function executes.
  Parameters:
    GOL_MSG* pMsg - The original system message information
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void TranslateMessageESGraph(GOL_MSG *pMsg)
{
    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_ACCELEROMETER_PAD))
    {
        if (pMsg->param2 == SCAN_CRA_PRESSED)
        {
            pMsg->param1 = ID_GRAPH_REFRESH_RATE_CHANGE;
            pMsg->param2 = SCAN_CRA_PRESSED;
        }
        else if (pMsg->param2 == SCAN_CR_PRESSED)
        {
            pMsg->param1 = ID_GRAPH_HOME;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
    }
}

/****************************************************************************
  Function:
    void UpdateESGraph( void )
  Description:
    This routine updates the display of the scrolling graph.  The update rate
    is controlled by the variable graphUpdateRate, which the user can
    increase or decrease by pressing switch S1. 
  Precondition:
    The graph information (graphData, graphDataHead, and graphDataTail) must
    be valid.
  Parameters:
    None
  Returns:
    None
  Remarks:
	None
  ***************************************************************************/
void UpdateESGraph(void)
{
	int i;
	        
	if(flag)
	{
	    if ((tick - previousTick ) > graphUpdateRate)
	    {
	        previousTick = tick;
	
	        // Remove the old graph and vertical grid lines
	        SetColor( BLACK );
	        DrawESGraphLine();
	        DrawGridVerticals();
	
	        // Get the latest accelerometer reading.
			for(i = 0; i < GRAPH_DATA_BUFFER_SIZE; i++)
	        if(DmaBuffer == 0)
	        	graphData[0][graphDataTail++] = (BufferA1[i]>>5) * GRAPH_GRID_HEIGHT/128 ;
	        else
	        	graphData[0][graphDataTail++] = (BufferB1[i]>>5) * GRAPH_GRID_HEIGHT/128;
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
	        DrawESGraphLine();
	
	        // Draw the updated grid
	        SetColor( WHITE );
	        DrawGridVerticals();
	        DrawGridHorizontals();	        
	    }

	    flag = 0;
    }
}

/****************************************************************************
  Function:
    void DrawESGraphLine( void )
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
void DrawESGraphLine(void)
{
    static SHORT    previousY;

    WORD            tempHead;
    SHORT           x;
    SHORT           y;

    tempHead = graphDataHead;

    for (x = 0; (x < GRAPH_GRID_WIDTH) && (tempHead != graphDataTail); x++)
    {
        if (x == 0)
        {
            PutPixel( GRAPH_GRID_LEFT + x, GRAPH_GRID_BOTTOM-graphData[0][tempHead] );
        }
        else
        {
            if (graphData[0][tempHead] > previousY)
                for (y = previousY + GRAPH_GRID_TOP; y <= graphData[0][tempHead] + GRAPH_GRID_TOP; y++)
                    PutPixel( GRAPH_GRID_LEFT + x, GRAPH_GRID_BOTTOM-y );
            else
                for (y = previousY + GRAPH_GRID_TOP; y >= graphData[0][tempHead] + GRAPH_GRID_TOP; y--)
                    PutPixel( GRAPH_GRID_LEFT + x, GRAPH_GRID_BOTTOM-y );
                    
        }

        previousY = graphData[0][tempHead];
        
        tempHead ++;
        if (tempHead == GRAPH_GRID_WIDTH+1)
        {
            tempHead = 0;
        }
    }
   
}





/****************************************************************************
  Function:
    void initESADC( void )
  Description:
    This routine is used to configure A/D to convert AN4 on Timer event. 
	It generates event to DMA on every sample/convert sequence. 
  Precondition:
    None
  Parameters:
    None
  Return Values:
    None
  Remarks:
    None
  ***************************************************************************/
void initESADC(void)
{	
	AD1CON1bits.FORM = 0;			// Data Output Format: Signed Fraction (Q15 format)
	AD1CON1bits.SSRC = 2;			// Sample Clock Source: GP Timer starts conversion
	AD1CON1bits.ASAM = 1;			// ADC Sample Control: Sampling begins immediately after conversion
	AD1CON1bits.AD12B = 1;			// 12-bit ADC operation

	AD1CON2bits.CHPS = 0;			// Converts CH0
    
	AD1CON3bits.ADRC = 0;			// ADC Clock is derived from Systems Clock
	AD1CON3bits.ADCS = 3;			// ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/40M)*4 = 100ns 
									// ADC Conversion Time for 12-bit Tc=14*Tad = 1.4us 
				
	AD1CON1bits.ADDMABM = 1; 		// DMA buffers are built in conversion order mode
	AD1CON2bits.SMPI = 0;			// SMPI must be 0


    //AD1CHS0: A/D Input Select Register
    AD1CHS0bits.CH0SA = 7;			// MUXA +ve input selection (AN7) for CH0
	AD1CHS0bits.CH0NA = 0;			// MUXA -ve input selection (Vref-) for CH0

    //AD1PCFGH/AD1PCFGL: Port Configuration Register
	AD1PCFGL=0xFFFF;
 	AD1PCFGLbits.PCFG7 = 0;			// AN7 as analog input
	TRISCbits.TRISC1 = 1;			// Setting the direction of I/O pin C1 as input
	   
    IFS0bits.AD1IF = 0;				// Clear the A/D interrupt flag bit
    IEC0bits.AD1IE = 0;				// Do Not Enable A/D interrupt 
    AD1CON1bits.ADON = 1;			// Turn on the A/D converter
    	
    flag = 0;
    
    initESTMR();					// Initialize TMR3 
	initESDMA();					// Initialize the DMA controller to buffer ADC data in conversion order
}

/****************************************************************************
  Function:
    void stopESADC( void )
  Description:
    This routine stops the ADC and the associated DMA and timer.
  Precondition:
    None
  Parameters:
    None
  Return Values:
    None
  Remarks:
    None
  ***************************************************************************/	
void stopESADC()
{
	DMA0CONbits.CHEN = 0;		// Disable DMA
	AD1CON1bits.ADON = 0;		// Turn off ADC1
	T3CONbits.TON = 0;			// Stop Timer 3
}

/****************************************************************************
  Function:
    void initESTMR( void )
  Description:
    Timer 3 is setup to time-out every Ts = (1/Fs) secs. As a result, the module 
	will stop sampling and trigger a conversion on every Timer3 time-out, i.e., Ts. 
	At that time, the conversion process starts and completes Tc=12*Tad periods later.	
	When the conversion completes, the module starts sampling again. However, since Timer3 
	is already on and counting, about (Ts-Tc)us later, Timer3 will expire again and trigger 
	next conversion. 
  Precondition:
    None
  Parameters:
    None
  Return Values:
    None
  Remarks:
    None
  ***************************************************************************/
void initESTMR(void) 
{
	TMR3 = 0x0000;					// Clear TMR3
	PR3 = SAMPPRD5;					// Load period value in PR3
	IFS0bits.T3IF = 0;				// Clear Timer 3 Interrupt Flag
	IEC0bits.T3IE = 0;				// Clear Timer 3 interrupt enable bit
	
	T3CONbits.TON = 1;				// Enable Timer 3
}

/****************************************************************************
  Function:
    void initESDMA( void )
  Description:
	 DMA0 configuration
	 Direction: Read from peripheral address 0-x300 (ADC1BUF0) and write to DMA RAM 
	 AMODE: Register indirect with post increment
	 MODE: Continuous, Ping-Pong Mode
	 IRQ: ADC Interrupt
	 ADC stores results stored alternatively between BufferA1[] and BufferB1[]
  Precondition:
    None
  Parameters:
    None
  Return Values:
    None
  Remarks:
    None
  ***************************************************************************/	
void initESDMA(void)
{
	DMA0CONbits.AMODE = 0;			// Configure DMA for Register indirect with post increment
	DMA0CONbits.MODE = 2;			// Configure DMA for Continuous Ping-Pong mode

	DMA0PAD = (int)&ADC1BUF0;		// Peripheral Address Register: ADC buffer
	DMA0CNT = (GRAPH_DATA_BUFFER_SIZE-1);			// DMA Transfer Count is (NUMSAMP-1)	
	
	DMA0REQ = 13;					// ADC interrupt selected for DMA channel IRQ
	
	DMA0STA = __builtin_dmaoffset(BufferA1);	// DMA RAM Start Address A	
	DMA0STB = __builtin_dmaoffset(BufferB1); // DMA RAM Start Address B

	IFS0bits.DMA0IF = 0;			// Clear the DMA interrupt flag bit
    IEC0bits.DMA0IE = 1;			// Set the DMA interrupt enable bit
	
	DMA0CONbits.CHEN = 1;			// Enable DMA channel
}









