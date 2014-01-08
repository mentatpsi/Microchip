/********************************************************************
  File Information:
    FileName:     	mtouch.c
    Dependencies:   See INCLUDES section
    Processor:      PIC18F46J50
    Hardware:       PIC18F Starter Kit
    Complier:  	    Microchip C18 (for PIC18)
    Company:        Microchip Technology, Inc.
    
    Software License Agreement:
    
    The software supplied herewith by Microchip Technology Incorporated
    (the “Company”) for its PIC® Microcontroller is intended and
    supplied to you, the Company’s customer, for use solely and
    exclusively on Microchip PIC Microcontroller products. The
    software is owned by the Company and/or its supplier, and is
    protected under applicable copyright laws. All rights are reserved.
    Any use in violation of the foregoing restrictions may subject the
    user to criminal sanctions under applicable laws, as well as to
    civil liability for the breach of the terms and conditions of this
    license.
    
    THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
    TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
    IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

  File Description:
    
    Change History:
     Rev   Date         Description
     1.0                Initial release

********************************************************************/

/******** Include files **********************/

#include "mtouch.h"
/*********************************************/


/******** Configuration Options **************/
#define MTOUCH_CHANNELS	4
/********************************************/


/*************** Variables ******************/
char g_mTouchCalibrationITRIM[MTOUCH_CHANNELS];
/********************************************/

/**************************************************************************/
/*Setup CTMU *****************************************************************/
/**************************************************************************/
void InitCTMU(void)
{
	/**************************************************************************/
	//Setup CTMU;
	/**************************************************************************/
	CTMUICON=0x03;
	CTMUCONHbits.IDISSEN=0;
	CTMUCONHbits.CTTRIG=0;
	CTMUCONHbits.EDGEN=0;
	CTMUCONLbits.EDG1POL=1;
	CTMUCONLbits.EDG2POL=1;
	CTMUCONLbits.EDG1STAT=0;
	CTMUCONLbits.EDG2STAT=0;
	PIR3bits.CTMUIF=0;
	CTMUCONHbits.CTMUEN=1;	
}

void InitADC(void)
{
	/**************************************************************************/
	//Setup AD converter;
	/**************************************************************************/
	TRISA=0x2F; //set channels as an input
	// Configured ANx as an analog channel
	// ANCON0
	ANCON0 = 0XE0;
	// ANCON1
	ANCON1 = 0X1F;
	// ADCON1
	ADCON1bits.ADFM=1; // Resulst format 1= Right justified
	ADCON1bits.ADCAL=0; // Normal A/D conversion operation
	ADCON1bits.ACQT=1; // Acquition time 7 = 20TAD 2 = 4TAD 1=2TAD
	ADCON1bits.ADCS=2; // Clock conversion bits 6= FOSC/64 2=FOSC/32
	ANCON1bits.VBGEN=1; // Turn on the Bandgap needed for Rev A0 parts
	// ADCON0
	ADCON0bits.VCFG0 =0; // Vref+ = AVdd
	ADCON0bits.VCFG1 =0; // Vref- = AVss
	ADCON0bits.CHS=2; // Select ADC channel 2
	ADCON0bits.ADON=1; // Turn on ADC
}

void mTouchInit(void)
{
	InitCTMU();
	InitADC();
}

unsigned int mTouchReadButton(unsigned char buttonID)
{
	/* Storage for reading the voltage */
	unsigned int Vread; 

	/* Select ADC channel */
	ADCON0bits.CHS = buttonID; 

	/* Set the ITRIM value for the callibration of the channel */
	CTMUICONbits.ITRIM = g_mTouchCalibrationITRIM[buttonID];

	/* Enable the CTMU */
	CTMUCONHbits.CTMUEN = 1;

	/* Drain charge on the circuit */
	CTMUCONHbits.IDISSEN = 1; 
	Nop();		Nop();		Nop();		Nop();		Nop();
	Nop();		Nop();		Nop();		Nop();		Nop();
	Nop();		Nop();		Nop();		Nop();		Nop();
	Nop();		Nop();		Nop();		Nop();		Nop();

	/* End drain of circuit */
	CTMUCONHbits.IDISSEN = 0;
 
	/* Begin charging the circuit using CTMU current source */
	CTMUCONLbits.EDG1STAT = 1; 
//	Nop();		Nop();		Nop();		Nop();		Nop();
//	Nop();		Nop();		Nop();		Nop();		Nop();
//	Nop();		Nop();		Nop();		Nop();		Nop();
	Nop();		Nop();		Nop();		Nop();		Nop();
	Nop();		Nop();		Nop();		Nop();		Nop();
	Nop();		Nop();		Nop();		Nop();		Nop();
	Nop();		Nop();		Nop();		Nop();		Nop();

	/* Stop charging circuit */
	CTMUCONLbits.EDG1STAT = 0;

	/* Make sure A/D interrupt is not set */
	PIR1bits.ADIF = 0; 
	
	/* Begin A/D conversion */
	ADCON0bits.GO=1; 
	//Wait for A/D convert complete
	while(!PIR1bits.ADIF);

	/* Get the value from the A/D */
	Vread = ADRES;

	return Vread;
}

void mTouchCalibrate(void)
{
	unsigned int Vread;
	unsigned char i;

	for (i = 0; i < MTOUCH_CHANNELS; i++)
	{
		g_mTouchCalibrationITRIM[i] = 0;

		Vread =  mTouchReadButton(i);
		if (Vread < 800)
		{
			do
			{
				if (g_mTouchCalibrationITRIM[i] == 0x1F)
				{
					/* Callibration failed */
					g_mTouchCalibrationITRIM[i] = 0;
					break;
				}
				g_mTouchCalibrationITRIM[i] ++;
				Vread = mTouchReadButton(i);
			}while (Vread < 800);	
		}
		else if(Vread > 800)
		{
			do
			{
				if (g_mTouchCalibrationITRIM[i] == 0xE1)
				{
					/* Callibration failed */
					g_mTouchCalibrationITRIM[i] = 0;
					break;
				}
				g_mTouchCalibrationITRIM[i] --;
				Vread = mTouchReadButton(i);
			}while (Vread > 800);	
		}
	}
}

