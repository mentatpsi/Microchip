/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		SK_PIC24H.c
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
#include "SK_PIC24H.h"
#include "Main.h"

/************************************************************************
 Variables													
 ************************************************************************/
int 					debounceS1 = 0;						// Debounce counter for switch S1	
int 					debounceS2 = 0;						// Debounce counter for switch S2	
int 					debounceS1Flag = 1;
int 					debounceS2Flag = 1;
extern int 				SwitchS1Flag;
extern int 				SwitchS2Flag;
extern int 				enableSoundFlag;
extern SCREEN_STATES 	screenState;

/****************************************************************************
  Function:
    void PIC24HSKInit(void)
  Description:
	Intialize the starter kit board: switches, speaker driver.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void PIC24HSKInit(void)
{
	// Configuring I/O ports used by LED matrix
	TRISA = 0x0000;
	TRISB = 0x0000;
	TRISC = 0x0000;
	LATA = 0x0000;
	LATB = 0x0000;
	LATC = 0x0000;
	
	AD1PCFGL = 0xFFFF;
	
	// Initialize switches
	SWITCH_S1_TRIS = 1;
	SWITCH_S2_TRIS = 1;
	
	// Initialize speaker
   	if(enableSoundFlag)
		speakerInit();									
					
	// Initialize timer 4 to activate switch press listener
   	initTmr4(); 													
}

/****************************************************************************
  Function:
    int CheckSwitchS1(void)
  Description:
	Checks switch S1 press event.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    1 - If Switch S1 is pressed.
    0 - If Switch S1 is not pressed.
  Remarks:
    None
  ***************************************************************************/
int CheckSwitchS1(void)
{
	// This function returns a 1 if a valid key press was detected on SW1 
	int isActive = 0;
	
	if(SWITCH_S1 == 0 && debounceS1Flag == 1){
		// If SW2 press was valid then toggle the record function	
		debounceS1++;
		if (debounceS1 == SWITCH_DEBOUNCE){
			
			/* This means that the Switch S3 was pressed long enough
			 * for a valid key press	*/
			debounceS1Flag = 0; 
			debounceS1  = 0;
			isActive = 1;
		}
		else
			isActive = 0;
	}
	else if(SWITCH_S1 == 1)	{
		if(debounceS1Flag == 0)
		{
			debounceS1--;
			if(debounceS1 <= 0)
			{
				debounceS1 = 0;
				debounceS1Flag = 1;
			}
		}
		isActive = 0;
	}
	
	if(screenState == SCREEN_JET || screenState == SCREEN_ES_GRAPH)
		debounceS1Flag = 1;
		
	return(isActive);
}	

/****************************************************************************
  Function:
    int CheckSwitchS2(void)
  Description:
	Checks switch S2 press event.
  Precondition:
    None.
  Parameters:
    None.
  Returns:
    1 - If Switch S2 is pressed.
    0 - If Switch S2 is not pressed.
  Remarks:
    None
  ***************************************************************************/
int CheckSwitchS2(void)
{
	
	// This function returns a 1 if a valid key press was detected on SW1 
	int isActive = 0;

	
	if(SWITCH_S2 == 0 && debounceS2Flag == 1){
		// If SW2 press was valid then toggle the record function	
		debounceS2++;
		if (debounceS2 == SWITCH_DEBOUNCE){
			
			/* This means that the Switch S3 was pressed long enough
			 * for a valid key press	*/
			debounceS2Flag = 0; 
			debounceS2  = 0;
			isActive = 1;
		}
		else
			isActive = 0;
	}
	else if(SWITCH_S2 == 1)	{
		if(debounceS2Flag == 0)
		{
			debounceS2--;
			if(debounceS2 <= 0)
			{
				debounceS2 = 0;
				debounceS2Flag = 1;
			}
		}
		isActive = 0;
	}

	if(screenState == SCREEN_JET)
		debounceS2Flag = 1;
			
	return(isActive);
}

