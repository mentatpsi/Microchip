/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		adcDrv1.c
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


#include "adcDrv1.h"         

int BufferA[NUMSAMP] __attribute__((space(dma)));
int BufferB[NUMSAMP] __attribute__((space(dma)));

//Output Buffer
int outputSignal[NUMSAMP];


/*=============================================================================
initAdc1() is used to configure A/D to convert channel 7 on Timer event. 
It generates event to DMA on every sample/convert sequence. 
=============================================================================*/
void initAdc1(void)
{

	AD1CON1bits.FORM   = 3;					// Data Output Format: Signed Fractional (Q15 format)
	AD1CON1bits.SSRC   = 2;					// Sample Clock Source: GP Timer starts conversion
	AD1CON1bits.ASAM   = 1;					// ADC Sample Control: Sampling begins immediately after conversion
	AD1CON1bits.AD12B  = 1;					// 12-bit ADC operation
	AD1CON1bits.ADDMABM = 1; 				// DMA buffers are built in conversion order mode
	
	AD1CON2bits.CHPS  = 0;					// Converts CH0
	AD1CON2bits.SMPI    = 0;					// SMPI must be 0
	
	AD1CON3bits.ADRC = 0;					// ADC Clock is derived from Systems Clock
	AD1CON3bits.ADCS = 3;					// ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/40M)*4 = 100ns
												// ADC Conversion Time for 12-bit Tc=14*Tad = ~1.4us	
	
	// AD1CHS0: A/D Input Select Register
	AD1CHS0bits.CH0SA=7;					// MUXA +ve input selection (AN5) for CH0
	AD1CHS0bits.CH0NA=0;					// MUXA -ve input selection (Vref-) for CH0
	
	// AD1PCFGL: Port Configuration Register
	AD1PCFGL=0xFFFF;
	AD1PCFGLbits.PCFG7 = 0;					// AN7 as analog input
	TRISCbits.TRISC1 = 1;						// Setting the direction as input
	
	IFS0bits.AD1IF = 0;							// Clear the A/D interrupt flag bit
	IEC0bits.AD1IE = 0;							// Disable A/D interrupt 
	AD1CON1bits.ADON = 1;					// Turn on the A/D converter	


}

/*=============================================================================  
Timer 3 is setup to time-out every Ts = (1/Fs) secs. As a result, the module 
will stop sampling and trigger a conversion on every Timer3 time-out, i.e., Ts. 
At that time, the conversion process starts and completes Tc=14*Tad periods later.
When the conversion completes, the module starts sampling again. However, since Timer3 
is already on and counting, about (Ts-Tc)us later, Timer3 will expire again and trigger 
next conversion. 
=============================================================================*/
void initTmr3() 
{
        TMR3 = 0x0000;
        PR3 = SAMPPRD;							// Timer period for sampling frequency Fs
        IFS0bits.T3IF = 0;							// Clear the TMR3 interrupt flag bit
        IEC0bits.T3IE = 0;         						// Disable the TMR3 interrupt
        T3CONbits.TON = 1;						// Turn on the TMR3
}

/*=============================================================================  
 DMA0 configuration
 Direction: Read from peripheral address 0-x300 (ADC1BUF0) and write to DMA RAM 
 AMODE: Register indirect with post increment
 MODE: Continuous, Ping-Pong Mode
 IRQ: ADC Interrupt
 ADC stores results stored alternatively between BufferA[] and BufferB[]
 ============================================================================= */
void initDma0(void)
{
	DMA0CONbits.AMODE = 0;				// Configure DMA0 for Register indirect with post increment
	DMA0CONbits.MODE  = 2;					// Configure DMA0 for Continuous Ping-Pong mode
	
	DMA0PAD=(int)&ADC1BUF0;				// DMA0 peripheral address register is set to ADC1 buffer (ADC1BUF0)
	DMA0CNT=(NUMSAMP-1);					// DMA0 transfer count is set to NUMSAMPL-1
	
	DMA0REQ=13;								// DMA0 channel IRQ is set to use ADC1
	
	DMA0STA = __builtin_dmaoffset(BufferA);	// DMA0 RAM start address offset A points to BufferA array
	DMA0STB = __builtin_dmaoffset(BufferB);	// DMA0 RAM start address offset B points to BufferB array
	
	IFS0bits.DMA0IF = 0;						// Clear the DMA0 interrupt flag bit
	IEC0bits.DMA0IE = 1;						// Set the DMA0 interrupt enable bit
	
	DMA0CONbits.CHEN=1;					// Turn on DMA0 
}


/*=============================================================================
DMA Interrupt Service Routine
=============================================================================*/
unsigned int DmaBuffer = 0;
int flag = 0;

void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{

	DmaBuffer ^= 1;
	
	flag = 1;

 	IFS0bits.DMA0IF = 0;							//Clear the DMA0 Interrupt Flag

}







