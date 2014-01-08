	/**********************************************************************
* ?2007 Microchip Technology Inc.
*
* FileName:        OC_PWM_Dedicatd_TMR.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC24Fxxxx
* Compiler:        MPLAB?C30 v3.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all
* ownership and intellectual property rights in the code accompanying
* this message and in all derivatives hereto.  You may use this code,
* and any derivatives created by any person or entity by or on your
* behalf, exclusively with Microchip's proprietary products.  Your
* acceptance and/or use of this code constitutes agreement to the
* terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS". NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
* NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT,
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS
* CODE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS, COMBINATION WITH
* ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE
* LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
* BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY,
* CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR
* EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER
* CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP
* SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author        Date      	Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Albert Z.		1/4/09		Original Release
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Pin #23, #24, #32, #33, #72, #71, #70, #69, #76 on Explorer 16 with
* PIC24FJ256GA110 PIM or PIC24FJ256GB110 PIM are 9 output compare
* modules (OC1 ~ OC9) respectively.
*
* This code example is tested on PIC24FJ256GA110 and PIC24FJ256GB110.
*
* PIC24FJ128GA010 family and PIC24FJ64GA004 family have a different
* Output Compare module, please read datasheet for more detail.
* The Family Reference Manual has two sections about Output Compare.
* For PIC24FJ256GA110 and PIC24FJ256GB110 family, please refer
* Section 35. Output Compare with Dedicated Timer.
* For PIC24FJ128GA010 and PIC24FJ64GA004 family, please refer
* Section 16. Output Compare.
**********************************************************************/
#include "p24fxxxx.h"
//PPS Outputs for PIC24FJ256GA110 and PIC24FJ256GB110
#define OC1_IO		18
#define OC2_IO		19
#define OC3_IO		20
#define OC4_IO		21
#define OC5_IO		22
#define OC6_IO          23
#define OC7_IO          24
#define OC8_IO          25
#define OC9_IO          35

#define cFosc   =      8; //8 MHz
#define cFpwm   =      15000;   //15 KHz


void InitializeTimer2For_PWM(void)
        {// Disable Watch Dog Timer
	RCONbits.SWDTEN = 0;


	// Configure RA6 as output
	ODCAbits.ODA6 = 0;
	TRISAbits.TRISA6 = 0;

	// PPS
	// Unlock Registers
	__builtin_write_OSCCONL(OSCCON & 0xbf);
	// Configure Output Functions *********************
	RPOR6bits.RP13R 	= OC1_IO;	// RP13	pin#23
	RPOR0bits.RP1R 		= OC2_IO;	// RP1	pin#24
	RPOR4bits.RP8R 		= OC3_IO;	// RP8	pin#32
	RPOR4bits.RP9R 		= OC4_IO;	// RP9	pin#33

	RPOR5bits.RP11R 	= OC5_IO;	// RP11	pin#72
	RPOR6bits.RP12R 	= OC6_IO;	// RP12	pin#71
	RPOR1bits.RP3R 		= OC7_IO;	// RP3	pin#70 <-
	RPOR2bits.RP4R 		= OC8_IO;	// RP4	pin#69

	RPOR12bits.RP24R 	= OC9_IO;	// RP24	pin#76
	// Lock Registers
    
	__builtin_write_OSCCONL(OSCCON | 0x40);
}



/*
	The following configuration for OC7 will set the Output Compare
	modules for PWM mode w/o FAULT pin enabled, a 33% duty cycle
	and PWM frequency of 20 Hz at Fosc = 32 MHz.
	Timer 2 is selected as the clock for the PWM time base
	and no interrupt is enabled. The PWM channel is	synchronised
	by itself.

	Find the Period register value for a desired PWM frequency of 20 Hz,
	where Fosc = 8 MHz with PLL (32 MHz device clock rate).
	Tcy = 2/Fosc = 62.5 ns
	PWM Period   =  1/PWM Frequency = 1/20 Hz = 50 ms
	PWM Period   = (OC9RS + 1) * Tcy * (Timer 2 Prescale Value)
	50 ms = (OC9RS + 1) * 62.5 ns * 64
	OC9RS = 12499 (0x30d3)
	*/
void Initialize_PWM(void) {
	T2CON                       = 0;		// Timer 2 is clock for OC5, OC6, OC7 and OC8.

	T2CONbits.TCKPS             = 2;		// Timer Input Clock Prescale is 1:64

	OC7CON1                     = 0; 		// It is a good practice to clear off the control bits initially
	OC7CON2                     = 0;
	
        OC7R                        = 0x0F;           // Initialize Compare Register1 with desired duty cycle
	OC7RS                       = 0x30d3;           // Initialize Compare Register1 with desired period

        //OC7R                        = 0x1046;           // Initialize Compare Register1 with desired duty cycle
	//OC7RS                       = 0x30d3;           // Initialize Compare Register1 with desired period

        
	OC7CON2bits.SYNCSEL         = 0x1f;		// synchronized by itself
	OC7CON1bits.OCTSEL          = 0x0;		// Timer 2 is the clock source for output Compare
	OC7CON1bits.OCFLT           = 0;		// No PWM Fault condition has occurred (this bit is only used when OCM<2:0> = 111)
	OC7CON2bits.OCINV           = 0;		// OCx output is not inverted
	OC7CON1bits.OCM             = 0x6;		// Edge-aligned PWM mode on OC

	T2CONbits.TON               = 1;		// Output compare modules do not run until sync source is switched on

	
}

void SetDutyCycle_PWM(unsigned int DutyCycle) {
    OC7R                                 = DutyCycle; //0x13f;	// This is the period of OC5, OC6, OC7 and OC8.
}

void SetPWMPer(unsigned int percent) {
    unsigned int DutyCycle = (0xFFFF * percent) / 100;
    OC7R                                 = DutyCycle; //0x13f;	// This is the period of OC5, OC6, OC7 and OC8.
}

/*
Find the Period register value for a desired PWM frequency of 20 Hz,
	where Fosc = 8 MHz with PLL (32 MHz device clock rate).
	Tcy = 2/Fosc = 62.5 ns
	PWM Period   =  1/PWM Frequency = 1/20 Hz = 50 ms
	PWM Period   = (OC9RS + 1) * Tcy * (Timer 2 Prescale Value)
	50 ms = (OC9RS + 1) * 62.5 ns * 64              mili - 10^-3  nano - 10^-9
	OC9RS = 12499 (0x30d3)
	*/
int getPWMPeriod(float oscF, int pwmF, int T_prescale) {
    float T_cy = 2/oscF;
    float pwmP = (1/(pwmF));
    int periodReg = ((pwmP)/(T_cy*T_prescale)) - 1;
    return periodReg;
}

void setRegister() {
    
}


