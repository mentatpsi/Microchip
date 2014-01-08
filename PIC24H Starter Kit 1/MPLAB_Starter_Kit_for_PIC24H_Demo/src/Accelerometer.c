/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		Accelerometer.c
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
#include "Main.h"
#include "Accelerometer.h"

/************************************************************************
 Variables													
 ************************************************************************/
ACCELEROMETERPADS           buttons;
ACCELEROMETERPADS       	buttonsPrevious;
unsigned char               buttonIndex;                    			// Index of what  ACCELEROMETERPAD is being checked.
int 						bufferX[SAMP_BUFF_SIZE];					// Data buffer for X-axis data from the accelerometer
int 						bufferIndexX;								// Index variable for X-axis buffer
int 						bufferY[SAMP_BUFF_SIZE];					// Data buffer for Y-axis data from the accelerometer
int 						bufferIndexY;								// Index variable for Y-axis buffer
int 						bufferZ[SAMP_BUFF_SIZE];					// Data buffer for Z-axis data from the accelerometer
int 						bufferIndexZ;								// Index variable for Z-axis buffer
int 						dirX = 0, dirY = 0, dirZ = 0;				// Direction storage variables
int 						jetX = 0, jetY = 0;
int 						orientationFlag = 0;
int 						maxXValue = 0;
int 						maxIndexXValue = 0;
int 						maxYValue = 0;
int 						maxIndexYValue = 0;
int 						maxZValue = 0;
int 						maxIndexZValue = 0;
unsigned int 				DmaBuffer = 0;								// DMA flag
int volatile 				flag = 0;									// DMA buffer full flag

extern int 					SwitchS1Flag;
extern int 					SwitchS2Flag;
extern int 					BufferA[MAX_CHNUM][SAMP_BUFF_SIZE];			// Ping pong buffer A
extern int 					BufferB[MAX_CHNUM][SAMP_BUFF_SIZE];			// Ping pong buffer B
extern int 					dirX1, dirY1, dirZ1;	
extern int 					speedFlagDone;
extern int 					speedFlag;
extern SCREEN_STATES        screenState;
extern WORD       			graphXVal;
extern WORD       			graphYVal;
extern WORD       			graphZVal;

/****************************************************************************
  Function:
    void AccelerometerInit( void )
  Description:
    This routine sets up the ADC to sample the tri-axial outputs of the Accelerometer.
  Precondition:
    None
  Parameters:
    None
  Return Values:
    None
  Remarks:
    None
  ***************************************************************************/
void AccelerometerInit( void )
{
  	initAdc1();		// Initialize the A/D sample accelerometer outputs 	
}

/****************************************************************************
  Function:
    void AccelerometerStop( void )
  Description:
    This routine stops the ADC which samples the Accelerometer.
  Precondition:
    None
  Parameters:
    None
  Return Values:
    None
  Remarks:
    None
  ***************************************************************************/
void AccelerometerStop()
{
	stopAdc1();
}

/****************************************************************************
  Function:
    void ReadAccelerometer( void )
  Description:
    This is a service routine for Accelerometer. It determines the direction
    of acceleration from the sampled tri-axial data.  
  Precondition:
    None
  Parameters:
    None
  Return Values:
    None
  Remarks:
	None
  ***************************************************************************/
void ReadAccelerometer( void )
{
	int i, j = 3, k;
				
	if(screenState == SCREEN_ES_GRAPH)
	{
		CheckSelectButton();
		return;
	}
	
	if(screenState == SCREEN_GRAPH)		
	{	
		CheckSelectButton();	
		GraphReadAccelerometer();
	}
			
	if(flag)
	{    
		// Buffering of accelerometer samples for X, Y, Z axes 
		for(j = 4; j <= 6; j++)    
		{
			// Copy the Ping-Pong buffer to the working buffer
			if(DmaBuffer == 0)
				for(i = 0; i < SAMP_BUFF_SIZE; i++)
				{
					if(j == 4)
						bufferX[i] = BufferA[j][i];
					else if(j == 5)
						bufferY[i] = BufferA[j][i];
					else if(j == 6)
				 		bufferZ[i] = BufferA[j][i];
				}
			else
				for(i = 0; i < SAMP_BUFF_SIZE; i++)
				{
					if(j == 4)
						bufferX[i] = BufferB[j][i];
					else if(j == 5)
						bufferY[i] = BufferB[j][i];
					else if(j == 6)
				 		bufferZ[i] = BufferB[j][i];
				}
		}
		
		// Apply moving average filter on X, Y, Z axes data buffers
		for(k = 0; k < SAMP_BUFF_SIZE-8; k++)
		{
			bufferX[k] = VectorAvg(8, &bufferX[k]);
			bufferY[k] = VectorAvg(8, &bufferY[k]);
			bufferZ[k] = VectorAvg(8, &bufferZ[k]);
		}
			
		// Find the maximum value in the X, Y, Z axes data buffers
		maxXValue = VectorMax(SAMP_BUFF_SIZE, bufferX, &maxIndexXValue);
		maxYValue = VectorMax(SAMP_BUFF_SIZE, bufferY, &maxIndexYValue);
		maxZValue = VectorMax(SAMP_BUFF_SIZE, bufferZ, &maxIndexZValue);
		
		if(screenState == SCREEN_ORIENTATION)
		{
			dirX1 = direction1X(maxXValue);
			dirY1 = direction1Y(maxYValue);
			dirZ1 = direction1Z(maxZValue);	
			orientationFlag = 1;
			
			CheckSelectButton();
		}
		else
		{
			// Direction of acceleration detection using maximum value 
			dirX = directionX(maxXValue);
			dirY = directionY(maxYValue);
			//dirZ = directionZ(maxZValue);
			
			jetX = dirX;
			jetY = dirY;
			
			orientationFlag = 1;
			
			if(abs(dirX) == 1 || abs(dirY) == 1)
			{
				if(dirX == 1)
				{
					 buttons.BTN2  = 1;		
					 buttons.BTN4  = 0;
				}
				else if(dirX == -1)
				{
					 buttons.BTN4  = 1;		
					 buttons.BTN2  = 0;
				}
				else if(dirX == 0)
				{
					 buttons.BTN2  = 0;	
					 buttons.BTN4  = 0;	
				}
				if(dirY == 1)
				{
					 buttons.BTN1  = 1;		
					 buttons.BTN3  = 0;
				}
				else if(dirY == -1)
				{
					 buttons.BTN3  = 1;	
					 buttons.BTN1  = 0;
				}
				else if(dirY == 0)
				{
					 buttons.BTN1  = 0;	
					 buttons.BTN3  = 0;	
				}
		
			}
		
			CheckSelectButton();
	    
		    dirX = 0;
			dirY = 0;
			dirZ = 0;
		}

		bufferIndexX = 0;
		bufferIndexY = 0;
		bufferIndexZ = 0;
	
	}				
			
	flag = 0;		
}

/****************************************************************************
  Function:
    void AccelerometerMsg(GOL_MSG* msg)
  Description:
    This routine creates approprite messages for button presses & releases.
  Precondition:
    None
  Parameters:
    GOL_MSG *msg    - The new system message
  Returns:
    None
  Remarks:
	None
  ***************************************************************************/
void AccelerometerMsg(GOL_MSG* msg)
{
    msg->uiEvent    = EVENT_INVALID;
    msg->type       = TYPE_KEYBOARD;


	if (buttons.BTN1 != buttonsPrevious.BTN1)
	{
	    msg->uiEvent = EVENT_KEYSCAN;
	    msg->param1  = ID_ACCELEROMETER_PAD;            //ID_ACCELEROMETER_BUTTON_01
	    if (buttons.BTN1)
	    {
	        msg->param2  = SCAN_UP_PRESSED;     		//SCAN_UP_PRESSED
	    }
	    else
	    {
	        msg->param2  = SCAN_UP_RELEASED;    		//SCAN_UP_RELEASED
	    }
	    buttonsPrevious.BTN1 = buttons.BTN1;
	    return;
	}
	
	if (buttons.BTN2 != buttonsPrevious.BTN2)
	{
	    msg->uiEvent = EVENT_KEYSCAN;
	    msg->param1  = ID_ACCELEROMETER_PAD;            //ID_ACCELEROMETER_BUTTON_02
	    if (buttons.BTN2)
	    {
	        msg->param2  = SCAN_RIGHT_PRESSED;  		//SCAN_RIGHT_PRESSED
	    }
	    else
	    {
	        msg->param2  = SCAN_RIGHT_RELEASED; 		//SCAN_RIGHT_RELEASED
	    }
	    buttonsPrevious.BTN2 = buttons.BTN2;
	    return;
	}
	
	if (buttons.BTN3 != buttonsPrevious.BTN3)
	{
	    msg->uiEvent = EVENT_KEYSCAN;
	    msg->param1  = ID_ACCELEROMETER_PAD;            //ID_ACCELEROMETER_BUTTON_03;
	    if (buttons.BTN3)
	    {
	        msg->param2  = SCAN_DOWN_PRESSED;   		//SCAN_DOWN_PRESSED
	    }
	    else
	    {
	        msg->param2  = SCAN_DOWN_RELEASED;  		//SCAN_DOWN_RELEASED
	    }
	    buttonsPrevious.BTN3 = buttons.BTN3;
	    return;
	}
	
	if (buttons.BTN4 != buttonsPrevious.BTN4)
	{
	    msg->uiEvent = EVENT_KEYSCAN;
	    msg->param1  = ID_ACCELEROMETER_PAD;            //ID_ACCELEROMETER_BUTTON_04
	    if (buttons.BTN4)
	    {
	        msg->param2  = SCAN_LEFT_PRESSED;   		//SCAN_LEFT_PRESSED
	    }
	    else
	    {
	        msg->param2  = SCAN_LEFT_RELEASED;  		//SCAN_LEFT_RELEASED
	    }
	    buttonsPrevious.BTN4 = buttons.BTN4;
	    return;
	}
	
	if (buttons.BTN5 != buttonsPrevious.BTN5)
	{
	    msg->uiEvent = EVENT_KEYSCAN;
	    msg->param1  = ID_ACCELEROMETER_PAD;            //ID_ACCELEROMETER_BUTTON_05;
	    if (buttons.BTN5)
	    {
	        msg->param2  = SCAN_CR_PRESSED;				//SCAN_CR_PRESSED
	    }
	    else
	    {
	        msg->param2  = SCAN_CR_RELEASED;			//SCAN_CR_RELEASED
	    }
	    buttonsPrevious.BTN5 = buttons.BTN5;
	    return;
	}
	
	if (buttons.BTN6 != buttonsPrevious.BTN6)
	{
	    msg->uiEvent = EVENT_KEYSCAN;
	    msg->param1  = ID_ACCELEROMETER_PAD;            //ID_ACCELEROMETER_BUTTON_06;
	    if (buttons.BTN6)
	    {
	        msg->param2  = SCAN_CRA_PRESSED;			//SCAN_CRA_PRESSED
	    }
	    else
	    {
	        msg->param2  = SCAN_CRA_RELEASED;			//SCAN_CRA_RELEASED
	    }
	    buttonsPrevious.BTN6 = buttons.BTN6;
	    return;
	}

}

/****************************************************************************
  Function:
    void CheckSelectButton( void )
  Description:
    This is a service routine for Switches. It determines the switch press.  
  Precondition:
    None
  Parameters:
    None
  Return Values:
    None
  Remarks:
	None
  ***************************************************************************/
void CheckSelectButton()
{
    if(SwitchS2Flag == 1)
	{
		buttons.BTN5  = 1;
		SwitchS1Flag = 0;
		SwitchS2Flag = 0;
	}
	else
		if(buttons.val == buttonsPrevious.val)
			buttons.val  = 0x0000;

		
    if(SwitchS1Flag == 1)
	{
		buttons.BTN6  = 1;
		SwitchS1Flag = 0;
		SwitchS2Flag = 0;
	}
	else
		if(buttons.val == buttonsPrevious.val)
			buttons.val  = 0x0000;	
}

/****************************************************************************
  Function:
    int directionX(int acc)
  Description:
    Detection of acceleration in X axis.
  Precondition:
    None
  Parameters:
    int - Accelerometer sample in X direction.
  Return Values:
    Variable indicating whether there is acceleration in X direction.
    +1 - Acceleration in +ve X direction
    -1 - Acceleration in -ve X direction
     0 - No acceleration in X direction
  Remarks:
	None
  ***************************************************************************/
int directionX(int acc)
{
	if(acc < 540 && acc > 500)
		acc = 0;
	else if(acc >= 540)
		acc = +1;
	else if(acc <= 500 && acc >= 1)
		acc = -1;
		
	return acc;
}

/****************************************************************************
  Function:
    int directionY(int acc)
  Description:
    Detection of acceleration in Y axis.
  Precondition:
    None
  Parameters:
    int - Accelerometer sample in Y direction.
  Return Values:
    Variable indicating whether there is acceleration in Y direction.
    +1 - Acceleration in +ve Y direction
    -1 - Acceleration in -ve Y direction
     0 - No acceleration in Y direction
  Remarks:
	None
  ***************************************************************************/
int directionY(int acc)
{
	if(acc < 530 && acc > 490)
		acc = 0;
	else if(acc >= 530)
		acc = +1;
	else if(acc <= 490 && acc >= 1)
		acc = -1;

	return acc;
}

/****************************************************************************
  Function:
	int VectorAvg(int numElems, int* srcV)
  Description:
    Vector average function.
  Precondition:
    None
  Parameters:
    numElems - Number of elements to be averaged.
    srcV 	 - Pointer to the input source vector.
  Return Values:
	Returns average value.
  Remarks:
	None
  ***************************************************************************/
int VectorAvg(int numElems, int* srcV)
{
	int Avg = 0, i;
	
	if(numElems > 25)
		return 0;
	
	for(i = 0; i < numElems; i++)
		Avg += srcV[i];	

	return(Avg/numElems);
	
}

/****************************************************************************
  Function:
    void initAdc1( void )
  Description:
    This routine is used to configure A/D to convert channels on Timer event. 
	It generates event to DMA on every sample/convert sequence. Accelerometer
	tri-axial outputs X, Y, Z are sampled on AN4, AN5, AN6 respectively.
  Precondition:
    None
  Parameters:
    None
  Return Values:
    None
  Remarks:
    None
  ***************************************************************************/
// Number of locations for ADC buffer = 7 (AN0 to AN6) x 32 = 224 words
// Align the buffer to 224 words or 448 bytes. This is needed for peripheral indirect mode
int  BufferA[MAX_CHNUM][SAMP_BUFF_SIZE] __attribute__((space(dma),aligned(64)));
int  BufferB[MAX_CHNUM][SAMP_BUFF_SIZE] __attribute__((space(dma),aligned(64)));
void initAdc1(void)
{
	AD1CON1bits.FORM = 0;		// Data Output Format: Integer
	AD1CON1bits.SSRC = 2;		// Sample Clock Source: GP Timer starts conversion
	AD1CON1bits.ASAM = 1;		// ADC Sample Control: Sampling begins immediately after conversion
	AD1CON1bits.AD12B = 0;		// 10-bit ADC operation
	AD1CON1bits.SIMSAM = 1; 	// Simultaneous Sampling
	AD1CON1bits.ADDMABM = 0; 	// DMA buffers are built in scatter/gather mode
	
	AD1CON2bits.CSCNA = 0;		// Do not scan inputs
	AD1CON2bits.CHPS = 2;		// Converts CH0, CH1, CH2, and CH3
	AD1CON2bits.SMPI = (NUM_CHS2SCAN-1);	// 4 ADC Channels are scanned
	
	AD1CON3bits.ADRC = 0;		// ADC Clock is derived from Systems Clock
	AD1CON3bits.ADCS = 63;		// ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/40M)*64 = 1.6us (625Khz)
								// ADC Conversion Time for 10-bit Tc=12*Tab = 19.2us	
									
	AD1CON4bits.DMABL = 5;		// 32 DMA buffer locations per analog input bits
	
	AD1CHS123bits.CH123SA = 1;	// CH1 positive input is AN3, CH2 positive input is AN4, CH3 positive input is AN5
	AD1CHS123bits.CH123NA = 0;	// CH1, CH2, CH3 negative input is VREFL
	
	AD1CHS0bits.CH0SA = 6;		// Channel 0 positive input is AN6
	AD1CHS0bits.CH0NA = 0;	
	
	// AD1PCFGL: Port Configuration Register
	AD1PCFGL=0xFFFF;
	AD1PCFGLbits.PCFG4 = 0;		// AN4 as analog input
	AD1PCFGLbits.PCFG5 = 0;		// AN5 as analog input
	AD1PCFGLbits.PCFG6 = 0;		// AN6 as analog input
	
	TRISBbits.TRISB2 = 1;		// Setting the direction of I/O pin B2 as input
	TRISBbits.TRISB3 = 1;		// Setting the direction of I/O pin B3 as input
	TRISCbits.TRISC0 = 1;		// Setting the direction of I/O pin C0 as input
	
	IFS0bits.AD1IF   = 0;		// Clear the A/D interrupt flag bit
	IEC0bits.AD1IE   = 0;		// Do Not Enable A/D interrupt 
	AD1CON1bits.ADON = 1;		// Turn on the A/D converter
	
	initTmr3();					// Initialize TMR3 
	initDma0();					// Initialize the DMA controller to buffer ADC data in conversion order
}

/****************************************************************************
  Function:
    void stopAdc1( void )
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
void stopAdc1()
{
	DMA0CONbits.CHEN = 0;		// Disable DMA
	AD1CON1bits.ADON = 0;		// Turn off ADC1
	T3CONbits.TON = 0;			// Stop Timer 3
}	

/****************************************************************************
  Function:
    void initTmr3( void )
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
void initTmr3() 
{
	T3CON = 0x0000;			
	TMR3 = 0x0000;
	PR3  = SAMPPRD_CT;			// Trigger ADC1 
	IFS0bits.T3IF = 0;			// Clear Timer 3 interrupt
	IEC0bits.T3IE = 0;			// Disable Timer 3 interrupt
	T3CONbits.TON = 1;			// Start Timer 3
}

/****************************************************************************
  Function:
    void initDma0( void )
  Description:
	 DMA0 configuration
	 Direction: Read from peripheral address 0-x300 (ADC1BUF0) and write to DMA RAM 
	 AMODE: Register indirect with post increment
	 MODE: Continuous, Ping-Pong Mode
	 IRQ: ADC Interrupt
	 ADC stores results stored alternatively between BufferA[] and BufferB[]
  Precondition:
    None
  Parameters:
    None
  Return Values:
    None
  Remarks:
    None
  ***************************************************************************/	
void initDma0(void)
{
	DMA0CONbits.AMODE = 2;			// Configure DMA for Peripheral indirect mode
	DMA0CONbits.MODE  = 2;			// Configure DMA for Continuous Ping-Pong mode
	DMA0PAD=(int)&ADC1BUF0;			// DMA0 peripheral address register is set to ADC1 buffer
	DMA0CNT = (SAMP_BUFF_SIZE*NUM_CHS2SCAN)-1;	// DMA0 transfer count is set to  (SAMP_BUFF_SIZE*NUM_CHS2SCAN)-1				
	DMA0REQ = 13;					// Select ADC1 as DMA Request source

	DMA0STA = __builtin_dmaoffset(BufferA);	// DMA0 RAM start address offset A points to BufferA array	
	DMA0STB = __builtin_dmaoffset(BufferB);	// DMA0 RAM start address offset B points to BufferB array

	IFS0bits.DMA0IF = 0;			// Clear the DMA interrupt flag bit
    IEC0bits.DMA0IE = 1;			// Set the DMA interrupt enable bit

	DMA0CONbits.CHEN = 1;			// Enable DMA
}

/****************************************************************************
  Function:
    void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
  Description:
	DMA Interrupt Service Routine.
  Precondition:
	DMA0 must be enabled.
  Parameters:
    None
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
	IFS0bits.DMA0IF = 0;		// Clear the DMA0 Interrupt Flag	
	DmaBuffer ^= 1;				// Ping-Pong buffer full 
	flag = 1;					// DMA buffer full 
		
}





