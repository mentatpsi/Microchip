/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		main.c
* Dependencies:    		Header (.h) files if applicable, see below
* Processor:       		PIC24HJ128GP504
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

#include "p24HJ128GP504.h"
#include "adcDrv1.h"

//Macros for Configuration Fuse Registers:
//Invoke macros to set up  device configuration fuse registers.
//The fuses will select the oscillator source, power-up timers, watch-dog
//timers etc. The macros are defined within the device
//header files. The configuration fuse registers reside in Flash memory.
// Internal FRC Oscillator
_FOSCSEL(FNOSC_FRC);			// FRC Oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON  & POSCMD_NONE); 
								// Clock Switching is enabled and Fail Safe Clock Monitor is disabled
								// OSC2 Pin Function: OSC2 is Clock Output
								// Primary Oscillator Mode: Disabled
_FWDT(FWDTEN_OFF);             	// Watchdog Timer enabled/disabled by user software
								// (LPRC can be disabled by clearing SWDTEN bit in RCON register
_FICD(ICS_PGD1 & JTAGEN_OFF);	// Communication Channel: PGC1/EMUC1 and PGD1/EMUD1
								// JTAG disabled
								

// Function Prototype of the Sensor Data Processing Function
void ProcessBuffer(int* inputBuffer, int* outputBuffer);



int main (void)
{
	// Configure FRC to operate the device at 40MIPS
	// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
	// Fosc= 7.37M*43/(2*2)=79.2275Mhz for ~40MIPS input clock
	PLLFBD=41;									// M=43
	CLKDIVbits.PLLPOST=0;						// N1=2
	CLKDIVbits.PLLPRE=0;							// N2=2
	OSCTUN=0;									// Tune FRC oscillator, if FRC is used

	// Disable Watch Dog Timer
	RCONbits.SWDTEN=0;

	// Clock switch to incorporate PLL
	__builtin_write_OSCCONH(0x01);				// Initiate Clock Switch to
													// FRC with PLL (NOSC=0b001)
	__builtin_write_OSCCONL(0x01);				// Start clock switching

	while (OSCCONbits.COSC != 0b001);				// Wait for Clock switch to occur

	while(OSCCONbits.LOCK!=1) {};				// Wait for PLL to lock


	initAdc1();
	initTmr3();
	initDma0();

	extern int BufferA[NUMSAMP];					// Ping pong buffer A
	extern int BufferB[NUMSAMP];					// Ping pong buffer B
	extern int outputSignal[NUMSAMP];				// Output buffer for the processed signal
	extern unsigned int DmaBuffer;					// DMA flag
	extern int flag;									// Flag

	while (1)               								//Loop Endlessly - Execution is interrupt driven
	{  
		if(flag) 
		{            
			if(DmaBuffer == 0)
			{
				ProcessBuffer(BufferA, outputSignal);	// Processing on BufferA 
			}
			else
			{
				ProcessBuffer(BufferB, outputSignal);	// Processing on BufferB 
			}
			flag = 0;
		}
	
	}
	
	return 0;

}

void ProcessBuffer(int* inputBuffer, int* outputBuffer)
{
	// TODO: Add sensor samples processing code here
	// Process the input buffer (inputBuffer) and populate the output buffer (outputBuffer) with the result
}	
