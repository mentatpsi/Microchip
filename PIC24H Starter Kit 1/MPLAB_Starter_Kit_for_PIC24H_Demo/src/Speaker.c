/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		Speaker.c
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
************************************************************************/

/************************************************************************
 Header Includes													
 ************************************************************************/
#include "Speaker.h"
#include "Timers.h"

/************************************************************************
 Variables													
 ************************************************************************/
int	 							samplesFromPgmMem	[ PGM_MEM_FRAME_SIZE];
char 							inputSamples			[FRAME_SIZE];
int								outputSamples1		[FRAME_SIZE] = {0};
int 							outputSamples2		[FRAME_SIZE];
int*							currentOutputBuffer;
unsigned int 					segmentIndex=0;
char 							bufferIndicator;
unsigned int 					pgmMemIndex;
unsigned long 					samplePtr;
unsigned int					sampleLength;
int 							flagBufferFull=0;
unsigned int 					speakerBusyFlag = 0;
static volatile unsigned int	currentPWMDutyCycle;		/* Stores the current duty cycle					*/
static volatile int				pwmCycleCount;				/* Keeps track of PWM cycles per speech sample  	*/
static volatile int				bufferIndex 	= 0;		/* Index into the external sample buffer			*/
static volatile int 			totalSamples;				/* Total samples to play							*/
volatile int * 					ptrDataBuffer;
volatile int 					isPlayingCurrentBuffer;

/****************************************************************************
  Function:
    void speakerInit(void)
  Description:
	Initializes speaker.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void speakerInit()
{
	// Configure Output Compare module
	OCPWMConfig();
	
	// Start Output Compare module
	OCPWMStart();
}	

/****************************************************************************
  Function:
    unsigned int speakerBusy(void)
  Description:
	Checks if the speaker is busy.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    1 - If the speaker is busy.
    0 - If the speaker is not busy.
  Remarks:
    None
  ***************************************************************************/
unsigned int speakerBusy(void)
{
	return speakerBusyFlag;
}

/****************************************************************************
  Function:
    void speakerActivate(long SpeechSegment, long SpeechSegmentSize)
  Description:
	Function to play message out of the speaker.
  Precondition:
    None.
  Parameters:
    long SpeechSegment - Address of G.711 compressed speech segment 
    					 stored in program memory. 
    long SpeechSegmentSize - length of the speech segment.
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void speakerActivate(long SpeechSegment, long SpeechSegmentSize)
{
	speakerBusyFlag = 1;
	segmentIndex=0;
	
	samplePtr = (long)SpeechSegment;
	sampleLength = SpeechSegmentSize/FRAME_SIZE;
		
	currentOutputBuffer = outputSamples1;
	pgmMemIndex = 0;
	bufferIndicator = 0;
	flagBufferFull=0;
	isPlayingCurrentBuffer = 0;
	initTmr5();	
}

/****************************************************************************
  Function:
    void OCPWMConfig(void)
  Description:
	Configures Output Compare module used by speaker.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void OCPWMConfig(void)
{
	/* Timer 2 is used for OCPWM	*/
	T2CON					= 0;
	TMR2					= 0;
	PR2						= 0;		/* This will produce an average value of Vdd/2	*/
	IFS0bits.T2IF			= 0;
	IEC0bits.T2IE			= 0;
	IPC1bits.T2IP			= TIMER2_INT_PRIORITY;
	TRISAbits.TRISA4 = 1;		// Setting the direction of I/O pin C0 as input
	RPOR2 					= 0b0000000000010010;
}

/****************************************************************************
  Function:
    void OCPWMStart(void)
  Description:
	Starts Ouput Compare module in PWM mode.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/ 
void OCPWMStart(void)
{
	IEC0bits.T2IE			= 1;						// Enable Timer2 Interrupt	
	T2CONbits.TON			= 1;						// Enable Timer2			
	Delay(100);
	PR2 					= MAX_PWM_PERIOD;			// PWM Period			
	OCRS					= (MAX_PWM_PERIOD/2);		// Initial Duty Cycle at 50% 	
	OCR						= (MAX_PWM_PERIOD/2);
	OCCON					= OCCON_WORD;				// Turn module on			
	pwmCycleCount			= 0;
	bufferIndex				= 0;
	isPlayingCurrentBuffer 	= 0;
}	

/****************************************************************************
  Function:
    void OCPWMStop(void)
  Description:
	Stops Output Compare module.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/ 
void OCPWMStop(void)
{
	OCCONbits.OCM 		= 0;		// Disable output compare
}

/****************************************************************************
  Function:
    int OCPWMIsBusy(void)
  Description:
	Checks if Output Compare module is busy.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    1 - If the Output Compare module is busy.
    0 - If the Output Compare module is not busy.
  Remarks:
    None
  ***************************************************************************/
int OCPWMIsBusy(void)
{
	return isPlayingCurrentBuffer;
}

/****************************************************************************
  Function:
    void OCPWMWrite(int * sourceDataBuffer,	int size)
  Description:
	Write to Ouput Compare module.
  Precondition:
    None.
  Parameters:
    int * sourceDataBuffer - Address of the source buffer played out of the speaker.
    int size - Length of the buffer.
  Returns:
	None
  Remarks:
    None
  ***************************************************************************/
void OCPWMWrite(int * sourceDataBuffer,	int size)
{
	ptrDataBuffer 			= sourceDataBuffer;		
	pwmCycleCount			= 0;					
	bufferIndex				= 0;					// Point to the first sample		
	isPlayingCurrentBuffer 	= 1;					// Playing the current buffer flag		
	totalSamples 			= size;
}
		
/****************************************************************************
  Function:
    void __attribute__((interrupt, shadow, auto_psv)) _T2Interrupt(void)
  Description:
    Timer 2 interrupt service routine which populates the Output Compare
    module with speech data.
  Precondition:
    Timer 2 and the Timer 2 interrupt must be enabled in order for
    this function to execute.  
  Parameters:
    None
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void __attribute__((__interrupt__,no_auto_psv)) _T2Interrupt(void)
{
	IFS0bits.T2IF			= 0;
	
	if(flagBufferFull)
	{
		if(pwmCycleCount == 0)
		{
			/* The first sample	*/
			unsigned int sample = ptrDataBuffer[bufferIndex] - (LOWEST_INPUT_VALUE);
			currentPWMDutyCycle = ((sample * MAX_PWM_PERIOD))/INPUT_RANGE;
			if(currentPWMDutyCycle > MAX_PWM_PERIOD) 
				currentPWMDutyCycle = MAX_PWM_PERIOD - 1;
			if (currentPWMDutyCycle <= 0) 
				currentPWMDutyCycle = 1;
			OCRS	= currentPWMDutyCycle;
		}
			
		/* When the required number of PWM cycles have been completed	
		   its time to move to the next sample and update the duty cycle */
					
		if((++ pwmCycleCount) == FPWM_FS_RATIO)
		{
			bufferIndex ++;
			pwmCycleCount = 0;
		}
		
		if(bufferIndex == totalSamples)
		{
			bufferIndex = 0;
			isPlayingCurrentBuffer = 0;
			flagBufferFull = 0;
		}
	}
	
}

/****************************************************************************
  Function:
    void initTmr5(void)
  Description:
	Initializes Timer 5 module to play out the entire speech segment.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/ 
void initTmr5() 
{
        TMR5 = 0x0000;
        PR5 = SAMPPRD_SPEAK;
        IFS1bits.T5IF = 0;
        IEC1bits.T5IE = 1;
		IPC7bits.T5IP			= TIMER2_INT_PRIORITY;
		
        //Start Timer4
        T5CONbits.TON = 1;
}

/****************************************************************************
  Function:
    void StopTmr5(void)
  Description:
	Stop Timer 5 module.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/ 
void StopTmr5()
{
	T5CONbits.TON = 0;
}	

/****************************************************************************
  Function:
    void StopTmr5(void)
  Description:
	Start Timer 5 module.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/ 
void StartTmr5()
{
	T5CONbits.TON = 1;
}

/****************************************************************************
  Function:
    void __attribute__((interrupt, shadow, auto_psv)) _T5Interrupt(void)
  Description:
    Timer 5 interrupt service routine which plays out all the frames of
    speech segment.
  Precondition:
    Timer 5 and the Timer 5 interrupt must be enabled in order for
    this function to execute.  
  Parameters:
    None
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void __attribute__ ((interrupt, no_auto_psv)) _T5Interrupt()
{

	if(!flagBufferFull)
	{
		if(segmentIndex++ < sampleLength)
		{
			// Read program memory frame
		    ReadProgramMemory((samplePtr+pgmMemIndex),samplesFromPgmMem, PGM_MEM_FRAME_SIZE);
			PackForG711(samplesFromPgmMem,inputSamples, PGM_MEM_FRAME_SIZE);
			
			// G.711 Decoding of the buffered speech frame
			G711Alaw2Lin(inputSamples,currentOutputBuffer,FRAME_SIZE);
			
			// Generate PWM signal using Output Compare module for the decoded speech frame
			OCPWMWrite(currentOutputBuffer,FRAME_SIZE);
			
			bufferIndicator ^= 1;
			currentOutputBuffer = (bufferIndicator) ? outputSamples2:outputSamples1;
			pgmMemIndex += PGM_MEM_FRAME_SIZE;
			flagBufferFull = 1;
		}
		else
		{
			flagBufferFull = 0;
			segmentIndex = 0;
			OCRS			= (( MAX_PWM_PERIOD)/2);	/* Initial Duty Cycle at 50% 	*/
			StopTmr5();
			speakerBusyFlag = 0;
		}
	}
	
	IFS1bits.T5IF = 0;	
}

