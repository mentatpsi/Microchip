
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
#include <math.h>

#include <HardwareProfile - PIC24F ADK for Android.h>
//PPS Outputs for PIC24FJ256GA110 and PIC24FJ256GB110
#define OC1_IO		18
#define OC2_IO		19
#define OC3_IO		20
#define OC4_IO		21
#define OC5_IO		22
#define OC6_IO          23
#define OC7_IO          24
#define OC8_IO          25
#define OC9_IO          26

static int pwmPer             = 0x3E7F;
unsigned short PWM_PERIOD     = 0x4E20;

//static int cFosc  =        8*(10^6); //8 MHz
//static int cFpwm  =       15000;   //15 KHz
//unsigned short dCycle = 0x4E20;
unsigned short curCycle = 0x4E20;//0x3e7f;

int getPWMPeriod(int pwmF, int oscF, short prescaleT);
//void setRPM(unsigned int targetRate);

//void SetDutyCycle_PWM(unsigned short DutyCycle);
void SetPWMPer(int percent, int update);
//void SetPWMPer(BYTE percent);


void InitializePWM_T2(void) {
    // Disable Watch Dog Timer
	RCONbits.SWDTEN = 0;

    T2CON				= 0;		// Timer 2 is clock for OC9.
    T2CONbits.TCKPS		= 0;		// Timer Input Clock Prescale is 1:64

    Nop();

    OC1CON1				= 0; 		// It is a good practice to clear off the control bits initially
    OC1CON2				= 0;
    OC1R				= 0x30d2;	// Initialize Compare Register1 with desired duty cycle
    OC1RS				= 0x30d3;	// Initialize Compare Register1 with desired period
    OC1CON2bits.SYNCSEL	= 0x1f;		// synchronized by itself
    OC1CON1bits.OCTSEL	= 0x0;		// Timer 2 is the clock source for output Compare
    OC1CON1bits.OCFLT	= 0;		// No PWM Fault condition has occurred (this bit is only used when OCM<2:0> = 111)
    OC1CON2bits.OCINV	= 0;		// OCx output is not inverted
    OC1CON1bits.OCM		= 0x6;		// Edge-aligned PWM mode on OC
    T2CONbits.TON		= 1;		// Output compare modules do not run until sync source is switched on

    RPOR1bits.RP3R = 18;
    /*

    OC7CON1				= 0; 		// It is a good practice to clear off the control bits initially
    OC7CON2				= 0;
    OC7R				= 0x1046;	// Initialize Compare Register1 with desired duty cycle
    OC7RS				= 0x30d3;	// Initialize Compare Register1 with desired period
    OC7CON2bits.SYNCSEL	= 0x1f;		// synchronized by itself
    OC7CON1bits.OCTSEL	= 0x0;		// Timer 2 is the clock source for output Compare
    OC7CON1bits.OCFLT	= 0;		// No PWM Fault condition has occurred (this bit is only used when OCM<2:0> = 111)
    OC7CON2bits.OCINV	= 0;		// OCx output is not inverted
    OC7CON1bits.OCM		= 0x6;		// Edge-aligned PWM mode on OC
    T2CONbits.TON		= 1;		// Output compare modules do not run until sync source is switched on
     * */
}


void InitializeTimer2For_PWM(void)
        {// Disable Watch Dog Timer
	RCONbits.SWDTEN = 0;

        // Configure RA6 as output
	//ODCAbits.ODA6 = 0;
	//TRISAbits.TRISA6 = 0;

	// Configure RD10 as output
	ODCDbits.ODD10 = 0;
        //ODCEbits.ODE1 = 0;
        //ODCAbits.ODA7 = 0;

        //mInitPWM_RD10();
        RPOR1bits.RP3R = 0;
        //LATDbits.LATD10 = 0;
        TRISDbits.TRISD10 = 0;

	//TRISAbits.TRISA6 = 0;

	// PPS
	// Unlock Registers

	
        __builtin_write_OSCCONL(OSCCON & 0xbf);
	// Configure Output Functions *********************

        RPOR1bits.RP3R 		= OC1_IO;       // RP3	pin#70
        RPOR6bits.RP13R 	= OC7_IO;	// RP13	pin#23
	RPOR0bits.RP1R 		= OC2_IO;	// RP1	pin#24
	RPOR4bits.RP8R 		= OC3_IO;	// RP8	pin#32
	RPOR4bits.RP9R 		= OC4_IO;	// RP9	pin#33

	RPOR5bits.RP11R 	= OC5_IO;	// RP11	pin#72
	RPOR6bits.RP12R 	= OC6_IO;	// RP12	pin#71
	
	//RPOR1bits.RP3R 		= OC7_IO;	// RP3	pin#70 <-
	RPOR2bits.RP4R 		= OC8_IO;	// RP4	pin#69

	RPOR12bits.RP24R 	= OC9_IO;	// RP24	pin#76
	// Lock Registers

	__builtin_write_OSCCONL(OSCCON | 0x40);
         

        //T2CON                       = 0;		// Timer 2 is clock for OC5, OC6, OC7 and OC8.

	//T2CONbits.TCKPS             = 0;		// Timer Input Clock Prescale is 1:1





}


void Initialize_PWM(void) {
      /* Unmap RP31(RF13) */
      //RPOR15bits.RP31R = 0; //Change for output

      /* Let's unmap RD10 instead*/
      RPOR1bits.RP3R = 0;


      LATDbits.LATD10 = 0;
      TRISDbits.TRISD10 = 0;

      //__builtin_write_OSCCONL(OSCCON & 0xbf);
      //RPOR1bits.RP3R 	= OC1_IO;
      //__builtin_write_OSCCONL(OSCCON | 0x40);
      
      //PORTDbits.RD10 = 0;

      /* Drive RF13 low and make it an output */
      //LATFbits.LATF13 = 0;  //Change for output
      //TRISFbits.TRISF13 = 0;    //Change for output

      //LATDbits.LATD10 = 0;
      //TRISDbits.TRISD10 = 0;

      Nop();




      /* Reset PWM */
      OC1CON1 = 0x0000;
      OC1CON2 = 0x0000;

      /* set PWM duty cycle to 50% */


      //OC1R    = curPWM_PERIOD;

      /*
      BYTE gear;

      gear = getGear();
      DWORD temp = PWM_PERIOD > gear;

      DWORD curPWM = PWM_PERIOD - temp;
      */


      //rpm2LEDs(curRate);

      OC1R    = 6250; /* set the duty cycle to 50% */
      OC1RS   = 12499;  /* set the period */

      /* configure PWM */
      OC1CON2 = 0x001F;   /* Sync with This OC module                               */
      OC1CON1 = 0x1C08;   /* Clock sourc Fcyc, trigger mode 1, Mode 0 (disable OC1) */

      /* enable the PWM */
      OC1CON1 = OC1CON1 | 0x0006;   /* Mode 6, Edge-aligned PWM Mode */

      /* Make pin RP31(RF13) OC1 (PWM output) */
      //RPOR15bits.RP31R = 18;    //Change for output

      RPOR1bits.RP3R = 18;
      }


void lowerDuty() {
    if (curCycle >= 100 ) {
        curCycle = curCycle;
        OC1R = curCycle - 100;
    }
}

void higherDuty() {
    if (curCycle < (pwmPer - 100) ) {
        curCycle = curCycle + 100;
        OC1R = curCycle;
    }
}



void setRPM(unsigned int targetRate) {
    //unsigned int curRate = PWM_PERIOD ;

      if (targetRate <= 1250 && targetRate >= 0) {
          //SetDutyCycle_PWM(targetRate);
      }
}

void SetClock() {
    RPOR1bits.RP3R = 18;
    T1CON				= 0;		// Timer 2 is clock for OC9.
    PR1					= 0x13f;	// This is the period of OC5, OC6, OC7 and OC8.
    T1CONbits.TCKPS                     = 0;		// Timer Input Clock Prescale is 1:32
}

void InitializePWM() {
    int period;
    //int dutyC;
    //short curRate;
    //InitializeTimer2For_PWM();

    RPOR1bits.RP3R = 18;
    //RPOR1bits.RP3R = 0;
    period = 0x13f; // 1 KHz

    curCycle = 0;

    T1CON				= 0;		// Timer 1 is clock for OC9.
    PR1					= 0x3E7F;	// This is the period of OC5, OC6, OC7 and OC8.
    T1CONbits.TCKPS                     = 0;		// Timer Input Clock Prescale is 1 (0- 1:1, 1- 1:32, 2- 1:64)

    OC1CON1				= 0; 		// It is a good practice to clear off the control bits initially
    OC1CON2				= 0;
    OC1R				= 0x1E40;		// Initialize Compare Register1 with desired rising edge of the pulse
    OC1RS				= 0x3E7E;		// Initialize Compare Register1 with desired falling edge of the pulse

    OC1CON2bits.SYNCSEL	= 0x0b;		// synchronized by the Timer 1
    OC1CON1bits.OCTSEL	= 0x4;		// Timer 1 is the clock source for output Compare
    OC1CON1bits.OCFLT	= 0;		// No PWM Fault condition has occurred (this bit is only used when OCM<2:0> = 111)
    OC1CON2bits.OCINV	= 0;		// OCx output is not inverted
    OC1CON1bits.OCM	= 0x6;		// 0x6: edge alligned 
                                        // 0x7: Center-aligned PWM mode on OC

    OC3CON2bits.SYNCSEL	= 0x0b;		// synchronized by the Timer 1
    OC3CON1bits.OCTSEL	= 0x4;		// Timer 1 is the clock source for output Compare
    OC3CON1bits.OCFLT	= 0;		// No PWM Fault condition has occurred (this bit is only used when OCM<2:0> = 111)
    OC3CON2bits.OCINV	= 1;		// OCx output is not inverted
    OC3CON1bits.OCM	= 0x6;		// 0x6: edge alligned
                                        // 0x7: Center-aligned PWM mode on OC

    T1CONbits.TON		= 1;		// Output compare modules do not run until sync source is switched on

}

void SetPWMPer(int percent, int update) {
    int curRate, dCycle, newPercent;
    Nop();


    if (update == 1) {

        if ((percent < 98) && (percent >= 50)) {

            curRate = floor(0x3E7C / 100);
            curRate = curRate * percent;

            dCycle = curRate;
            OC1CON2= 0;
            OC1CON2bits.SYNCSEL	= 0x0b;	
            OC1CON2bits.OCINV = 0;
            OC1R = dCycle;
            RPOR1bits.RP3R = 18;
            RPOR4bits.RP8R = 20;
            //OC1RS = 0x3E7C;
        }
        /*
        else if ((percent >=49) && (percent <= 51)) {
            dCycle = 0x1F40;
            //OC1CON2bits.OCINV = 0;
            OC1R = dCycle;
        }
        */

        else if ((percent >2) && (percent < 50)) {
            newPercent = 100 - percent;
            curRate = floor(0x3E7C / 100);
            curRate = curRate * newPercent;
            dCycle = curRate;
            //OC1CON2= 0;
            //OC1CON2bits.OCINV = 1;
            OC3R = curRate - 1;
            //OC1RS = 0x1F40;
            RPOR1bits.RP3R = 20;
            RPOR4bits.RP8R = 18;
        }

        else if ( percent >= 98 ) {

            dCycle = 0x3E7C;
            //OC1CON2bits.OCINV = 0;
            OC1R = dCycle;
            RPOR1bits.RP3R = 18;
            RPOR4bits.RP8R = 20;
        }

        else if ( percent <= 2) {
            dCycle = 0x3E7C;

            //OC1CON2bits.OCINV = 1;
            OC3R = dCycle;
            RPOR1bits.RP3R = 20;
            RPOR4bits.RP8R = 18;

        }
    }
    //OC1CON2bits.OCINV	= 1;
    curCycle = dCycle;

    if (update == 0) {
        
        OC1R				= 0x1E40;		// Initialize Compare Register1 with desired rise edge of the pulse
        curCycle = 0x1E40;
    }
    
    //OC1RS				= 0x3E7F;		// Initialize Compare Register1 with desired falling edge of the pulse
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
/*int getPWMPeriod(float oscF, int pwmF, int T_prescale) {
    float T_cy = (float) 2/oscF;
    float pwmP = (1/(pwmF));
    int periodReg = ((pwmP)/(T_cy*T_prescale)) - 1;
    return periodReg;
}*/

int getPWMPeriod(int pwmF, int oscF, short prescaleT) {
    float curoscF = (float) oscF;
    float T_cy = 2/(curoscF);
    float fpwmF = (float) pwmF;
    float pwmP = (1/(fpwmF));
    int periodReg = (int) ((pwmP)/(T_cy * prescaleT)) - 1;
    return periodReg;
}

/*
void setPWMPeriod(int pwmF) {
    //float curoscF = cFosc;
    float T_cy = 2/(curoscF);
    float fpwmF = (float) pwmF;
    float pwmP = (1/(fpwmF));
    int periodReg = (int) ((pwmP)/(T_cy*64)) - 1;
    OC7RS = periodReg;
}
*/
void setRegister() {

}


