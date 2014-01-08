/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		Timers.c
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
#include "Timers.h"

/************************************************************************
 Variables													
 ************************************************************************/
int 					SwitchS1Flag = 0;
int 					SwitchS2Flag = 0;

extern int 				activemissileJet;
extern int 				EMWaveFlag;

/****************************************************************************
  Function:
    void initTmr4(void)
  Description:
	Intialize Timer 4 for polling Switches S1 and S2.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/ 
void initTmr4() 
{
        TMR4 = 0x0000;
        PR4 = SAMPPRD4;
        IFS1bits.T4IF = 0;
        IEC1bits.T4IE = 1;

        //Start Timer4
        T4CONbits.TON = 1;
}

/****************************************************************************
  Function:
    void StopTmr4(void)
  Description:
	Stops timer 4.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/ 
void StopTmr4(void)
{
	T4CONbits.TON = 0;
}	

/****************************************************************************
  Function:
    void StopTmr4(void)
  Description:
	Starts timer 4.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/ 
void StartTmr4()
{
	T4CONbits.TON = 1;
}

/****************************************************************************
  Function:
    void __attribute__((interrupt, shadow, auto_psv)) _T4Interrupt(void)
  Description:
    Timer 4 interrupt service routine polls for Switch press event.
  Precondition:
    Timer 4 and the Timer 4 interrupt must be enabled in order for
    this function to execute.  
  Parameters:
    None
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void __attribute__ ((interrupt, no_auto_psv)) _T4Interrupt()
{

	IFS1bits.T4IF = 0;	

	if(activemissileJet || EMWaveFlag)
		return;
			
	// Check if switch S1 event occured
	if(CheckSwitchS1())
	{
		SwitchS1Flag = 0;
		SwitchS2Flag = 0;
		SwitchS1Flag = 1;
	}

	// Check if switch S2 event occured
	if(CheckSwitchS2())
	{
		SwitchS1Flag = 0;
		SwitchS2Flag = 0;
		SwitchS2Flag = 1;
	}

}	


	


