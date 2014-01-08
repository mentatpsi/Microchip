/********************************************************************
 FileName:		sc_config.h
 Dependencies:	See INCLUDES section
 Processor:		PIC18,PIC24,PIC32 & dsPIC33F Microcontrollers
 Hardware:		This demo is natively intended to be used on Exp 16, LPC
 				& HPC Exp board. This demo can be modified for use on other hardware
 				platforms.
 Complier:  	Microchip C18 (for PIC18), C30 (for PIC24 & dsPIC) & C32 (for PIC32) 
 Company:		Microchip Technology, Inc.

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

********************************************************************
 File Description:

 Change History:
  Rev   Description
  ----  -----------------------------------------
  1.0   Initial release
  1.01  Modified to Support T=1 protocol
  1.02  Modified to Support PIC32,dsPIC33F & PIC24H controllers
********************************************************************/

#ifndef SCCFG_H
#define SCCFG_H

#include "HardwareProfile.h"

	#define		SC_PROTO_T1

	#define		SC_APDU_BUFF_SIZE		256

	// Modify the T1 block buffer size as per the RAM size of the chosen micro & project requirement
	#define		SC_T1_PROTOCOL_MAX_BUFF_SIZE		256

	/*******************************************************************/
	/****************If using PIC24FJ256B110 PIM ***********************/
	/*******************************************************************/
	#ifdef __PIC24FJ256GB110__
	
		#define EXPLORER_16

		// Note : SYS_FREQ denotes the System clock frequency
		#define SYS_FREQ 32000000UL

		// Note : FCY denotes the instruction cycle clock frequency (SYS_FREQ/2)"
		#define FCY (SYS_FREQ/2)

		#include "p24Fxxxx.h"
		#include "libpic30.h"
		#include <math.h>

//		#define  WaitMicroSec(MicroSec)    __delay_us(MicroSec)
//		#define  WaitMilliSec(Waitms)      __delay_ms(Waitms)

		// Enable Port Pin of Micro as Vcc for Smart Card
		#define  ENABLE_SC_POWER_THROUGH_PORT_PIN

		// Set Clock Freq to drive Smart Card
		#define Scdrv_ClockSet()            (REFOCON = 0x0300)

		//Enable UART
		#define SCdrv_EnableUART()	    	(U1MODEbits.UARTEN = 1)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableClock()	    	(REFOCONbits.ROON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableClock()	    (REFOCONbits.ROON = 0)

		// Set Clock Freq to drive Smart Card
		#define SCdrv_EnableDelayTimerIntr()   (IPC0bits.T1IP = 4,IFS0bits.T1IF = 0,T1CON = 0x0030,PR1 = 0xFFFF,IEC0bits.T1IE = 1)

		// One count of timer 1 corresponds to how much micro seconds...
		#define TIMER1_SINGLE_COUNT_MICRO_SECONDS	(BYTE)(256/(FCY/1000000UL))

		// Enable Clock to drive Smart Card
		#define SCdrv_SetDelayTimerCnt(count)	    (TMR1 = count)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableDelayTimer()	    	(T1CONbits.TON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableDelayTimer()	    	(T1CONbits.TON = 0)
		
			//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_CIRCUIT_INPUT_CLK    		SYS_FREQ

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_POWER2_VALUE      		(BYTE)REFOCONbits.RODIV

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_DIVISOR_VALUE      		(BYTE)0

        #define SCdrv_SetClockPinLow() LATBbits.LATB15 = 0;
        #define SCdrv_SetClockPinHigh() LATBbits.LATB15 = 1;
        

		#define REF_CLOCK_TO_SMART_CARD     (unsigned long)((unsigned long long)((unsigned long long)FCY * 2)/(pow(2,3)))
		#define SMART_CARD_CLOCK_IN_KHZ 4000 //this macro used by USB descriptors, SMART_CARD_CLOCK_IN_KHZ =REF_CLOCK_TO_SMART_CARD/1000
		#define SMART_CARD_DEFAULT_DATA_RATE 10752 //this macro used by USB descriptors, SMART_CARD_DEFAULT_DATA_RATE = REF_CLOCK_TO_SMART_CARD/372

		//Turn on 1/off 0 card power
		#define SCdrv_SetSwitchCardPower(x) 			(LATBbits.LATB9=(x))

		//set reset state to the value x
		#define SCdrv_SetSwitchCardReset(x) 			(LATEbits.LATE8=(x))

		//set tx pin to the value x
		#define SCdrv_SetTxPinData(x) 					(LATFbits.LATF2=(x))

		//Get Smart Card Present status
		#define SCdrv_CardPresent()	      				(PORTBbits.RB10 || !PORTBbits.RB1)

		//Get Rx Pin Data
		#define SCdrv_GetRxPinData()               		(PORTCbits.RC4)

		//Set Tx Pin direction
		#define SCdrv_TxPin_Direction(flag)    			(TRISFbits.TRISF2 = flag)

		//Set Power Pin direction connected to the smart card
		#define SCdrv_PowerPin_Direction(flag) 			(TRISBbits.TRISB9 = flag)

		//Set Reset Pin direction connected to the smart card
		#define SCdrv_ResetPin_Direction(flag) 			(TRISEbits.TRISE8 = flag)

		//Set Card Present Pin direction connected to the smart card
		#define SCdrv_CardPresent_Direction(flag) 		(TRISBbits.TRISB10 = flag)
		
		//Set Sim Present Pin direction connected to the smart card
		#define SCdrv_SimPresent_Direction(flag) 		(TRISBbits.TRISB1 = flag)

		// Enable Pull up at Tx Pin
		#define SCdrv_EnableTxPinPullUp()				(_CN48PUE = 1)

		// Enable Pull up at Rx Pin
		#define SCdrv_EnableRxPinPullUp()				(_CN70PUE = 1)

		// Enable Pull up for SMART_CARD_DETECTION
		#define SCdrv_EnableCardPresentPinPullUp()		(_CN28PUE = 1)
		
		// Enable Pull up for SIM_CARD_DETECTION
		#define SCdrv_EnableSimPresentPinPullUp()		(_CN3PUE = 1)

		// Disable Pull up at Tx Pin
		#define SCdrv_DisableTxPinPullUp()				(_CN48PUE = 0)

		// Disable Pull up at Rx Pin
		#define SCdrv_DisableRxPinPullUp()				(_CN70PUE = 0)

		// MAP UART Rx Pin
		#define MapUART1RxPin()							PPSIn( RPI41, U1RX_FUNC )

		// MAP UART Tx Pin
		#define MapUART1TxPin()							PPSOut( RP30, U1TX_FUNC )
		
    /*******************************************************************/
    /****************If using PIC24FJ64GB004 PIM ***********************/
    /*******************************************************************/ 		
   #elif defined __PIC24FJ64GB004__

		#define EXPLORER_16

		// Note : SYS_FREQ denotes the System clock frequency
		#define SYS_FREQ 32000000UL

		// Note : FCY denotes the instruction cycle clock frequency (SYS_FREQ/2)"
		#define FCY (SYS_FREQ/2)

		#include "p24Fxxxx.h"
		#include "libpic30.h"
		#include <math.h>

//		#define  WaitMicroSec(MicroSec)    __delay_us(MicroSec)
//		#define  WaitMilliSec(Waitms)      __delay_ms(Waitms)

		// Enable Port Pin of Micro as Vcc for Smart Card
		#define  ENABLE_SC_POWER_THROUGH_PORT_PIN

		// Set Clock Freq to drive Smart Card
		#define Scdrv_ClockSet()            (REFOCON = 0x0300)

		//Enable UART
		#define SCdrv_EnableUART()	    	(U1MODEbits.UARTEN = 1)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableClock()	    	{ __builtin_write_OSCCONL(OSCCON & 0xbf);\
									           RPOR11bits.RP23R = 18;\
									            __builtin_write_OSCCONL(OSCCON | 0x40);\
									            TRISCbits.TRISC7 = 0;\
									        OC1CON1          = 0x0000;\
									        OC1CON2 		 = 0x0000;\
									        OC1R             = 2;\
									        OC1RS            = 4;\
									        OC1CON1bits.OCTSEL = 7;\
									        OC1CON1bits.OCM = 6;\
									        OC1CON2bits.SYNCSEL = 0x1F;\
								            }

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableClock()	    (OC1R = 0)
		
		// Set Clock Freq to drive Smart Card
		#define SCdrv_EnableDelayTimerIntr()   (IPC0bits.T1IP = 4,IFS0bits.T1IF = 0,T1CON = 0x0030,PR1 = 0xFFFF,IEC0bits.T1IE = 1)

		// One count of timer 1 corresponds to how much micro seconds...
		#define TIMER1_SINGLE_COUNT_MICRO_SECONDS	(BYTE)(256/(FCY/1000000UL))

		// Enable Clock to drive Smart Card
		#define SCdrv_SetDelayTimerCnt(count)	    (TMR1 = count)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableDelayTimer()	    	(T1CONbits.TON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableDelayTimer()	    	(T1CONbits.TON = 0)
		
			//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_CIRCUIT_INPUT_CLK    		SYS_FREQ

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_POWER2_VALUE      		(BYTE)0

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_DIVISOR_VALUE      		(BYTE)OC1RS


        #define SCdrv_SetClockPinLow() (OC1R = 0,LATCbits.LATC7 = 0)
        #define SCdrv_SetClockPinHigh()(OC1R = 0,LATCbits.LATC7 = 1)
        

		#define REF_CLOCK_TO_SMART_CARD  3200000   //(unsigned long)((unsigned long long)((unsigned long long)FCY * 2)/(pow(2,3)))
		#define SMART_CARD_CLOCK_IN_KHZ 3200 //this macro used by USB descriptors, SMART_CARD_CLOCK_IN_KHZ =REF_CLOCK_TO_SMART_CARD/1000
		#define SMART_CARD_DEFAULT_DATA_RATE 8602 //this macro used by USB descriptors, SMART_CARD_DEFAULT_DATA_RATE = REF_CLOCK_TO_SMART_CARD/372

		//Turn on 1/off 0 card power
		#define SCdrv_SetSwitchCardPower(x) 			(LATAbits.LATA0=(x))

		//set reset state to the value x
		#define SCdrv_SetSwitchCardReset(x) 			(LATCbits.LATC2=(x))

		//set tx pin to the value x
		#define SCdrv_SetTxPinData(x) 					(LATBbits.LATB13=(x))

		//Get Smart Card Present status
		#define SCdrv_CardPresent()	      				(PORTAbits.RA0 || !PORTAbits.RA1)

		//Get Rx Pin Data
		#define SCdrv_GetRxPinData()               		(PORTBbits.RB7)

		//Set Tx Pin direction
		#define SCdrv_TxPin_Direction(flag)    			(TRISBbits.TRISB13 = flag)

		//Set Power Pin direction connected to the smart card
		#define SCdrv_PowerPin_Direction(flag) 			(TRISAbits.TRISA0 = flag)

		//Set Reset Pin direction connected to the smart card
		#define SCdrv_ResetPin_Direction(flag) 			(TRISCbits.TRISC2 = flag)

		//Set Card Present Pin direction connected to the smart card
		#define SCdrv_CardPresent_Direction(flag) 		(TRISAbits.TRISA0 = flag)

		//Set Sim Present Pin direction connected to the smart card
		#define SCdrv_SimPresent_Direction(flag) 		(TRISAbits.TRISA1 = flag)

		// Enable Pull up at Tx Pin
		#define SCdrv_EnableTxPinPullUp()				(_CN13PUE = 1)

		// Enable Pull up at Rx Pin
		#define SCdrv_EnableRxPinPullUp()				(_CN23PUE = 1)

		// Enable Pull up for SMART_CARD_DETECTION
		#define SCdrv_EnableCardPresentPinPullUp()		(_CN22PUE = 1)

		// Enable Pull up for SIM_CARD_DETECTION
		#define SCdrv_EnableSimPresentPinPullUp()		(_CN3PUE = 1)

		// Disable Pull up at Tx Pin
		#define SCdrv_DisableTxPinPullUp()				(_CN13PUE = 0)

		// Disable Pull up at Rx Pin
		#define SCdrv_DisableRxPinPullUp()				(_CN23PUE = 0)

		// MAP UART Rx Pin
		#define MapUART1RxPin()							RPINR18bits.U1RXR  = 7; //PPSIn( RPI7, U1RX_FUNC ) //RPOR5bits.RP11R = 18;

		// MAP UART Tx Pin
		#define MapUART1TxPin()						    RPOR6bits.RP13R = 3;	  //PPSOut( RP13, U1TX_FUNC )

    /*******************************************************************/
    /****************If using PIC18F46J50 PIM **************************/
    /*******************************************************************/ 
	#elif defined(__18F46J50)

		#define HPC_EXPLORER

		// Note : SYS_FREQ denotes the System clock frequency
		#define SYS_FREQ 48000000UL

		// Note : FCY denotes the instruction cycle clock frequency (SYS_FREQ/4)"
		#define FCY (SYS_FREQ/4)

		#include <p18cxxx.h>
		#include <delays.h>
		#include <math.h>

		// Enable Port Pin of Micro as Vcc for Smart Card
		#define  ENABLE_SC_POWER_THROUGH_PORT_PIN

		// Set Clock Freq to drive Smart Card
		#define Scdrv_ClockSet()            (REFOCON = 0x04)

		// Enable UART
		#define SCdrv_EnableUART()	    	(RCSTA1bits.SPEN = 1)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableClock()	    	(REFOCONbits.ROON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableClock()	    (REFOCONbits.ROON = 0)

		// Set Clock Freq to drive Smart Card
		#define SCdrv_EnableDelayTimerIntr()   (T0CON = 0x86,INTCON2bits.TMR0IP = 0,RCONbits.IPEN = 1,INTCONbits.PEIE = 1,INTCONbits.TMR0IE = 1,INTCONbits.GIEH = 1)

		// One count of timer 0 corresponds to how much micro seconds...
		#define TIMER0_SINGLE_COUNT_MICRO_SECONDS	(BYTE)(128/(FCY/1000000UL))

		// Enable Clock to drive Smart Card
		#define SCdrv_SetDelayTimerCnt(count)	    (TMR0H = (count) >> 8,TMR0L = (count) & 0x00FF)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableDelayTimer()	    	(T0CONbits.TMR0ON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableDelayTimer()	    	(T0CONbits.TMR0ON = 0)

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_CIRCUIT_INPUT_CLK    		SYS_FREQ

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_POWER2_VALUE      		(BYTE)REFOCONbits.RODIV

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_DIVISOR_VALUE      		(BYTE)0

		#define SCdrv_SetClockPinLow() LATBbits.LATB2 = 0;
        #define SCdrv_SetClockPinHigh() LATBbits.LATB2 = 1;

		#define REF_CLOCK_TO_SMART_CARD     (unsigned long)((FCY * 4)/(pow(2,4)))
        #define SMART_CARD_CLOCK_IN_KHZ 3000 //this macro used by USB descriptors, SMART_CARD_CLOCK_IN_KHZ =REF_CLOCK_TO_SMART_CARD/1000
		#define SMART_CARD_DEFAULT_DATA_RATE 8064 //this macro used by USB descriptors, SMART_CARD_DEFAULT_DATA_RATE = REF_CLOCK_TO_SMART_CARD/372


		//Turn on 1/off 0 card power
		#define SCdrv_SetSwitchCardPower(x) 			(LATBbits.LATB0=(x))

		//set reset state to the value x
		#define SCdrv_SetSwitchCardReset(x) 			(LATBbits.LATB4=(x))

		//set tx pin to value x
		#define SCdrv_SetTxPinData(x) 					(LATCbits.LATC6=(x))

		//Get Smart Card Present status
		#define SCdrv_CardPresent()	      				(PORTBbits.RB3 || !PORTBbits.RB1)

		//Get Rx Pin Data
		#define SCdrv_GetRxPinData()               		(PORTCbits.RC7)

		//Set Tx Pin direction
		#define SCdrv_TxPin_Direction(flag)    			(TRISCbits.TRISC6 = flag)

		//Set Rx Pin direction
		#define SCdrv_RxPin_Direction(flag)    			(TRISCbits.TRISC7 = flag)

		//Set Power Pin direction connected to the smart card
		#define SCdrv_PowerPin_Direction(flag) 			(TRISBbits.TRISB0 = flag)

		//Set Reset Pin direction connected to the smart card
		#define SCdrv_ResetPin_Direction(flag) 			(TRISBbits.TRISB4 = flag)

		//Set Card Present Pin direction connected to the smart card
		#define SCdrv_CardPresent_Direction(flag) 		(TRISBbits.TRISB3 = flag)

		//Set Sim Present Pin direction connected to the smart card
		#define SCdrv_SimPresent_Direction(flag) 		(TRISBbits.TRISB1 = flag)

		// Enable Pull up for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_EnableCardSimPresentPinPullUp()	(ANCON1 = (ANCON1 | 0x1F),INTCON2bits.RBPU = 0)

		// Disable Pull up for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_DisbleCardSimPresentPinPullUp()	(INTCON2bits.RBPU = 1)
		
    /*******************************************************************/
    /****************If using PIC18F87J50 PIM **************************/
    /*******************************************************************/ 
    /*******************************************************************/
    /****************If using PIC18F47J53 PIM **************************/
    /*******************************************************************/ 
	#elif defined(__18F47J53)

		#define HPC_EXPLORER

		// Note : SYS_FREQ denotes the System clock frequency
		#define SYS_FREQ 48000000UL

		// Note : FCY denotes the instruction cycle clock frequency (SYS_FREQ/4)"
		#define FCY (SYS_FREQ/4)

		#include <p18cxxx.h>
		#include <delays.h>
		#include <math.h>

		// Enable Port Pin of Micro as Vcc for Smart Card
		#define  ENABLE_SC_POWER_THROUGH_PORT_PIN

		// Provide clock from the Micro to the Smart Card
		#define  ENABLE_SC_EXTERNAL_CLOCK

		// Set Clock Freq to drive Smart Card
		#define Scdrv_ClockSet()            (REFOCON = 0x04)

		// Enable UART
		#define SCdrv_EnableUART()	    	(RCSTA1bits.SPEN = 1)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableClock()	    	(REFOCONbits.ROON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableClock()	    (REFOCONbits.ROON = 0)

		// Set Clock Freq to drive Smart Card
		#define SCdrv_EnableDelayTimerIntr()   (T0CON = 0x86,INTCON2bits.TMR0IP = 0,RCONbits.IPEN = 1,INTCONbits.PEIE = 1,INTCONbits.TMR0IE = 1,INTCONbits.GIEH = 1)

		// One count of timer 0 corresponds to how much micro seconds...
		#define TIMER0_SINGLE_COUNT_MICRO_SECONDS	(BYTE)(128/(FCY/1000000UL))

		// Enable Clock to drive Smart Card
		#define SCdrv_SetDelayTimerCnt(count)	    (TMR0H = (count) >> 8,TMR0L = (count) & 0x00FF)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableDelayTimer()	    	(T0CONbits.TMR0ON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableDelayTimer()	    	(T0CONbits.TMR0ON = 0)

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_CIRCUIT_INPUT_CLK    		SYS_FREQ

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_POWER2_VALUE      		(BYTE)REFOCONbits.RODIV

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_DIVISOR_VALUE      		(BYTE)0

		#define SCdrv_SetClockPinLow() LATBbits.LATB2 = 0;
        #define SCdrv_SetClockPinHigh() LATBbits.LATB2 = 1;

		#define REF_CLOCK_TO_SMART_CARD     (unsigned long)((FCY * 4)/(pow(2,4)))
        #define SMART_CARD_CLOCK_IN_KHZ 3000 //this macro used by USB descriptors, SMART_CARD_CLOCK_IN_KHZ =REF_CLOCK_TO_SMART_CARD/1000
		#define SMART_CARD_DEFAULT_DATA_RATE 8064 //this macro used by USB descriptors, SMART_CARD_DEFAULT_DATA_RATE = REF_CLOCK_TO_SMART_CARD/372


		//Turn on 1/off 0 card power
		#define SCdrv_SetSwitchCardPower(x) 			(LATBbits.LATB0=(x))

		//set reset state to the value x
		#define SCdrv_SetSwitchCardReset(x) 			(LATBbits.LATB4=(x))

		//set tx pin to value x
		#define SCdrv_SetTxPinData(x) 					(LATCbits.LATC6=(x))

		//Get Smart Card Present status
		#define SCdrv_CardPresent()	      				(PORTBbits.RB3 || !PORTBbits.RB1)

		//Get Rx Pin Data
		#define SCdrv_GetRxPinData()               		(PORTCbits.RC7)

		//Set Tx Pin direction
		#define SCdrv_TxPin_Direction(flag)    			(TRISCbits.TRISC6 = flag)

		//Set Rx Pin direction
		#define SCdrv_RxPin_Direction(flag)    			(TRISCbits.TRISC7 = flag)

		//Set Power Pin direction connected to the smart card
		#define SCdrv_PowerPin_Direction(flag) 			(TRISBbits.TRISB0 = flag)

		//Set Reset Pin direction connected to the smart card
		#define SCdrv_ResetPin_Direction(flag) 			(TRISBbits.TRISB4 = flag)

		//Set Card Present Pin direction connected to the smart card
		#define SCdrv_CardPresent_Direction(flag) 		(TRISBbits.TRISB3 = flag)

		//Set Sim Present Pin direction connected to the smart card
		#define SCdrv_SimPresent_Direction(flag) 		(TRISBbits.TRISB1 = flag)

		// Enable Pull up for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_EnableCardSimPresentPinPullUp()	(ANCON1 = (ANCON1 | 0x1F),INTCON2bits.RBPU = 0)

		// Disable Pull up for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_DisbleCardSimPresentPinPullUp()	(INTCON2bits.RBPU = 1)
		
    /*******************************************************************/
    /****************If using PIC18F87J50 PIM **************************/
    /*******************************************************************/ 

	#elif defined(__18F87J50)

		#define HPC_EXPLORER

		// Note : SYS_FREQ denotes the System clock frequency
		#define SYS_FREQ 48000000UL

		// Note : FCY denotes the instruction cycle clock frequency (SYS_FREQ/4)"
		#define FCY (SYS_FREQ/4)

		#include <p18cxxx.h>
		#include <delays.h>
		#include <math.h>

		// Enable Port Pin of Micro as Vcc for Smart Card
		#define  ENABLE_SC_POWER_THROUGH_PORT_PIN

		// Provide clock from the Micro to the Smart Card
		#define  ENABLE_SC_EXTERNAL_CLOCK

		// Set Clock Freq to drive Smart Card
		#define Scdrv_ClockSet()            (TMR2 = 0,T2CON = 0x00,PR2 = 3,CCPR1L = 2,TRISCbits.TRISC2 = 0,CCP1CON = 0x00)

		//Enable UART
		#define SCdrv_EnableUART()	    	(RCSTAbits.SPEN = 1)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableClock()	    	(CCP1CON = 0x0C,T2CONbits.TMR2ON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableClock()	    (CCP1CON = 0x00,T2CONbits.TMR2ON = 0,TMR2 = 0)

		// Set Clock Freq to drive Smart Card
		#define SCdrv_EnableDelayTimerIntr()   (T0CON = 0x86,INTCON2bits.TMR0IP = 0,RCONbits.IPEN = 1,INTCONbits.PEIE = 1,INTCONbits.TMR0IE = 1,INTCONbits.GIEH = 1)

		// One count of timer 0 corresponds to how much micro seconds...
		#define TIMER0_SINGLE_COUNT_MICRO_SECONDS	(BYTE)(128/(FCY/1000000UL))

		// Enable Clock to drive Smart Card
		#define SCdrv_SetDelayTimerCnt(count)	    (TMR0H = (count) >> 8,TMR0L = (count) & 0x00FF)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableDelayTimer()	    	(T0CONbits.TMR0ON = 1)
		#define SCdrv_SetClockPinLow() LATEbits.LATE3 = 0;
        #define SCdrv_SetClockPinHigh() LATEbits.LATE3 = 1;

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableDelayTimer()	    	(T0CONbits.TMR0ON = 0)

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_CIRCUIT_INPUT_CLK    		FCY

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_POWER2_VALUE      		(BYTE)0

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_DIVISOR_VALUE      		(BYTE)PR2
		
		#define REF_CLOCK_TO_SMART_CARD     (unsigned long)((FCY * 4)/(pow(2,4)))
        #define SMART_CARD_CLOCK_IN_KHZ 3000 //this macro used by USB descriptors, SMART_CARD_CLOCK_IN_KHZ =REF_CLOCK_TO_SMART_CARD/1000
		#define SMART_CARD_DEFAULT_DATA_RATE 8064 //this macro used by USB descriptors, SMART_CARD_DEFAULT_DATA_RATE = REF_CLOCK_TO_SMART_CARD/372


		//Turn on 1/off 0 card power
		#define SCdrv_SetSwitchCardPower(x) 			(LATBbits.LATB0=(x))

		//set reset state to the value x
		#define SCdrv_SetSwitchCardReset(x) 			(LATBbits.LATB4=(x))

		//set tx pin to value x
		#define SCdrv_SetTxPinData(x) 					(LATCbits.LATC6=(x))

		//Get Smart Card Present status
		#define SCdrv_CardPresent()	      				(PORTBbits.RB3 || !PORTBbits.RB1)

		//Get Rx Pin Data
		#define SCdrv_GetRxPinData()               		(PORTCbits.RC7)

		//Set Tx Pin direction
		#define SCdrv_TxPin_Direction(flag)    			(TRISCbits.TRISC6 = flag)

		//Set Rx Pin direction
		#define SCdrv_RxPin_Direction(flag)    			(TRISCbits.TRISC7 = flag)

		//Set Power Pin direction connected to the smart card
		#define SCdrv_PowerPin_Direction(flag) 			(TRISBbits.TRISB0 = flag)

		//Set Reset Pin direction connected to the smart card
		#define SCdrv_ResetPin_Direction(flag) 			(TRISBbits.TRISB4 = flag)

		//Set Card Present Pin direction connected to the smart card
		#define SCdrv_CardPresent_Direction(flag) 		(TRISBbits.TRISB3 = flag)

		//Set Sim Present Pin direction connected to the smart card
		#define SCdrv_SimPresent_Direction(flag) 		(TRISBbits.TRISB1 = flag)

		// Enable Pull up's for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_EnableCardSimPresentPinPullUp()	(ANCON1 = (ANCON1 | 0x1F),INTCON2bits.RBPU = 0)

		// Disable Pull up's for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_DisbleCardSimPresentPinPullUp()	(INTCON2bits.RBPU = 1)

	#elif defined(__18F87J94)

        #define RCIF   RC1IF

		#define HPC_EXPLORER

		// Note : SYS_FREQ denotes the System clock frequency
		#define SYS_FREQ 64000000UL

		// Note : FCY denotes the instruction cycle clock frequency (SYS_FREQ/4)"
		#define FCY (SYS_FREQ/4)

		#include <p18cxxx.h>
		#include <delays.h>
		#include <math.h>

		// Enable Port Pin of Micro as Vcc for Smart Card
		#define  ENABLE_SC_POWER_THROUGH_PORT_PIN

		// Provide clock from the Micro to the Smart Card
		#define  ENABLE_SC_EXTERNAL_CLOCK

		// Set Clock Freq to drive Smart Card
		#define Scdrv_ClockSet()            (TMR2 = 0,T2CON = 0x00,PR2 = 3,CCPR1L = 2,TRISCbits.TRISC2 = 0,CCP1CON = 0x00)

		//Enable UART
		#define SCdrv_EnableUART()	    	(RCSTAbits.SPEN = 1)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableClock()	    	(CCP1CON = 0x0C,T2CONbits.TMR2ON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableClock()	    (CCP1CON = 0x00,T2CONbits.TMR2ON = 0,TMR2 = 0)

		// Set Clock Freq to drive Smart Card
		#define SCdrv_EnableDelayTimerIntr()   (T0CON = 0x86,INTCON2bits.TMR0IP = 0,RCONbits.IPEN = 1,INTCONbits.PEIE = 1,INTCONbits.TMR0IE = 1,INTCONbits.GIEH = 1)

		// One count of timer 0 corresponds to how much micro seconds...
		#define TIMER0_SINGLE_COUNT_MICRO_SECONDS	(BYTE)(128/(FCY/1000000UL))

		// Enable Clock to drive Smart Card
		#define SCdrv_SetDelayTimerCnt(count)	    (TMR0H = (count) >> 8,TMR0L = (count) & 0x00FF)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableDelayTimer()	    	(T0CONbits.TMR0ON = 1)
		#define SCdrv_SetClockPinLow() LATEbits.LATE3 = 0;
        #define SCdrv_SetClockPinHigh() LATEbits.LATE3 = 1;

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableDelayTimer()	    	(T0CONbits.TMR0ON = 0)

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_CIRCUIT_INPUT_CLK    		FCY

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_POWER2_VALUE      		(BYTE)0

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_DIVISOR_VALUE      		(BYTE)PR2
		
		#define REF_CLOCK_TO_SMART_CARD     (unsigned long)((FCY * 4)/(pow(2,4)))
        #define SMART_CARD_CLOCK_IN_KHZ 3000 //this macro used by USB descriptors, SMART_CARD_CLOCK_IN_KHZ =REF_CLOCK_TO_SMART_CARD/1000
		#define SMART_CARD_DEFAULT_DATA_RATE 8064 //this macro used by USB descriptors, SMART_CARD_DEFAULT_DATA_RATE = REF_CLOCK_TO_SMART_CARD/372


		//Turn on 1/off 0 card power
		#define SCdrv_SetSwitchCardPower(x) 			(LATBbits.LATB0=(x))

		//set reset state to the value x
		#define SCdrv_SetSwitchCardReset(x) 			(LATBbits.LATB4=(x))

		//set tx pin to value x
		#define SCdrv_SetTxPinData(x) 					(LATCbits.LATC6=(x))

		//Get Smart Card Present status
		#define SCdrv_CardPresent()	      				(PORTBbits.RB3 || !PORTBbits.RB1)

		//Get Rx Pin Data
		#define SCdrv_GetRxPinData()               		(PORTCbits.RC7)

		//Set Tx Pin direction
		#define SCdrv_TxPin_Direction(flag)    			(TRISCbits.TRISC6 = flag)

		//Set Rx Pin direction
		#define SCdrv_RxPin_Direction(flag)    			(TRISCbits.TRISC7 = flag)

		//Set Power Pin direction connected to the smart card
		#define SCdrv_PowerPin_Direction(flag) 			(TRISBbits.TRISB0 = flag)

		//Set Reset Pin direction connected to the smart card
		#define SCdrv_ResetPin_Direction(flag) 			(TRISBbits.TRISB4 = flag)

		//Set Card Present Pin direction connected to the smart card
		#define SCdrv_CardPresent_Direction(flag) 		(TRISBbits.TRISB3 = flag)

		//Set Sim Present Pin direction connected to the smart card
		#define SCdrv_SimPresent_Direction(flag) 		(TRISBbits.TRISB1 = flag)

		// Enable Pull up's for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_EnableCardSimPresentPinPullUp()	(ANCON1 = (ANCON1 | 0x1F),INTCON2bits.RBPU = 0)

		// Disable Pull up's for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_DisbleCardSimPresentPinPullUp()	(INTCON2bits.RBPU = 1)

		
    /*******************************************************************/
    /****************If using PIC18F14K50 Lowpincount board*************/
    /*******************************************************************/ 
	#elif defined(__18F14K50)

		#define LOW_PIN_COUNT_USB_DEVELOPMENT_KIT

		// Note : SYS_FREQ denotes the System clock frequency
		#define SYS_FREQ 48000000UL

		// Note : FCY denotes the instruction cycle clock frequency (SYS_FREQ/4)"
		#define FCY (SYS_FREQ/4)

		#include <p18cxxx.h>
		#include <delays.h>
		#include <math.h>

		// Enable Port Pin of Micro as Vcc for Smart Card
		#define  ENABLE_SC_POWER_THROUGH_PORT_PIN

		// Provide clock from the Micro to the Smart Card
		#define  ENABLE_SC_EXTERNAL_CLOCK

		// Set Clock Freq to drive Smart Card
		#define Scdrv_ClockSet()            (TMR2 = 0,T2CON = 0x00,PR2 = 3,CCPR1L = 2,PSTRCON = 0x08,TRISCbits.TRISC2 = 0,CCP1CON = 0x00) 

		//Enable UART
		#define SCdrv_EnableUART()	    	(RCSTAbits.SPEN = 1)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableClock()	    	(CCP1CON = 0x0C,T2CONbits.TMR2ON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableClock()	    (CCP1CON = 0x00,T2CONbits.TMR2ON = 0,TMR2 = 0)

		// Set Clock Freq to drive Smart Card
		#define SCdrv_EnableDelayTimerIntr()   (T0CON = 0x86,INTCON2bits.TMR0IP = 0,RCONbits.IPEN = 1,INTCONbits.PEIE = 1,INTCONbits.TMR0IE = 1,INTCONbits.GIEH = 1)

		// One count of timer 0 corresponds to how much micro seconds...
		#define TIMER0_SINGLE_COUNT_MICRO_SECONDS	(BYTE)(128/(FCY/1000000UL))

		// Enable Clock to drive Smart Card
		#define SCdrv_SetDelayTimerCnt(count)	    (TMR0H = (count) >> 8,TMR0L = (count) & 0x00FF)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableDelayTimer()	    	(T0CONbits.TMR0ON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableDelayTimer()	    	(T0CONbits.TMR0ON = 0)

        #define SCdrv_SetClockPinLow() LATCbits.LATC2 = 0;
        #define SCdrv_SetClockPinHigh() LATCbits.LATC2 = 1;

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_CIRCUIT_INPUT_CLK    		FCY

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_POWER2_VALUE      		(BYTE)0

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_DIVISOR_VALUE      		(BYTE)PR2
		#define REF_CLOCK_TO_SMART_CARD     (unsigned long)(FCY/4)
        #define SMART_CARD_CLOCK_IN_KHZ 3000 //this macro used by USB descriptors, SMART_CARD_CLOCK_IN_KHZ =REF_CLOCK_TO_SMART_CARD/1000
		#define SMART_CARD_DEFAULT_DATA_RATE 8064 //this macro used by USB descriptors, SMART_CARD_DEFAULT_DATA_RATE = REF_CLOCK_TO_SMART_CARD/372


		//Turn on 1/off 0 card power
		#define SCdrv_SetSwitchCardPower(x) 			(LATCbits.LATC0=(x))

		//set reset state to the value x
		#define SCdrv_SetSwitchCardReset(x) 			(LATCbits.LATC1=(x))

		//set tx pin to the value x
		#define SCdrv_SetTxPinData(x) 					(LATBbits.LATB7=(x))

		//Get Smart Card Present status
		#define SCdrv_CardPresent()	      				(PORTBbits.RB6 || !PORTBbits.RB4)

		//Get Rx Pin Data
		#define SCdrv_GetRxPinData()               		(PORTBbits.RB5)

		//Set Tx Pin direction
		#define SCdrv_TxPin_Direction(flag)    			(TRISBbits.TRISB7 = flag)

		//Set Rx Pin direction
		#define SCdrv_RxPin_Direction(flag)    			(ANSELH = 0,TRISBbits.TRISB5 = flag)

		//Set Power Pin direction connected to the smart card
		#define SCdrv_PowerPin_Direction(flag) 			(TRISCbits.TRISC0 = flag)

		//Set Reset Pin direction connected to the smart card
		#define SCdrv_ResetPin_Direction(flag) 			(TRISCbits.TRISC1 = flag)

		//Set Card Present Pin direction connected to the smart card
		#define SCdrv_CardPresent_Direction(flag) 		(TRISBbits.TRISB6 = flag)

		//Set Sim Present Pin direction connected to the smart card
		#define SCdrv_SimPresent_Direction(flag) 		(TRISBbits.TRISB4 = flag)

		// Enable Pull up's for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_EnableCardSimPresentPinPullUp()	(WPUB = 0xE0,INTCON2bits.RABPU = 0)

		// Disable Pull up's for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_DisbleCardSimPresentPinPullUp()	(INTCON2bits.RABPU = 1)

    /*******************************************************************/
    /****************If using PIC18F4550 PICDEMFSUSB Board**************/
    /*******************************************************************/
	#elif defined(__18F4550)

		#define PICDEM_FS_USB

		// Note : SYS_FREQ denotes the System clock frequency
		#define SYS_FREQ 48000000UL

		// Note : FCY denotes the instruction cycle clock frequency (SYS_FREQ/4)"
		#define FCY (SYS_FREQ/4)

		#include <p18cxxx.h>
		#include <delays.h>
		#include <math.h>

		// Enable Port Pin of Micro as Vcc for Smart Card
		#define  ENABLE_SC_POWER_THROUGH_PORT_PIN

		// Provide clock from the Micro to the Smart Card
		#define  ENABLE_SC_EXTERNAL_CLOCK

		// Set Clock Freq to drive Smart Card
		#define Scdrv_ClockSet()            (TMR2 = 0,T2CON = 0x00,PR2 = 3,CCPR1L = 2,TRISCbits.TRISC2 = 0,CCP1CON = 0x00)

		//Enable UART
		#define SCdrv_EnableUART()	    	(RCSTAbits.SPEN = 1)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableClock()	    	(CCP1CON = 0x0C,T2CONbits.TMR2ON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableClock()	    (CCP1CON = 0x00,T2CONbits.TMR2ON = 0,TMR2 = 0)

		// Set Clock Freq to drive Smart Card
		#define SCdrv_EnableDelayTimerIntr()   (T0CON = 0x86,INTCON2bits.TMR0IP = 0,RCONbits.IPEN = 1,INTCONbits.PEIE = 1,INTCONbits.TMR0IE = 1,INTCONbits.GIEH = 1)

		// One count of timer 0 corresponds to how much micro seconds...
		#define TIMER0_SINGLE_COUNT_MICRO_SECONDS	(BYTE)(128/(FCY/1000000UL))

		// Enable Clock to drive Smart Card
		#define SCdrv_SetDelayTimerCnt(count)	    (TMR0H = (count) >> 8,TMR0L = (count) & 0x00FF)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableDelayTimer()	    	(T0CONbits.TMR0ON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableDelayTimer()	    	(T0CONbits.TMR0ON = 0)

		#define SCdrv_SetClockPinLow() LATCbits.LATC2 = 0;
        #define SCdrv_SetClockPinHigh() LATCbits.LATC2 = 1;

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_CIRCUIT_INPUT_CLK    		FCY

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_POWER2_VALUE      		(BYTE)0

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_DIVISOR_VALUE      		(BYTE)PR2
		#define REF_CLOCK_TO_SMART_CARD     (unsigned long)(FCY/4)
		#define SMART_CARD_CLOCK_IN_KHZ 3000 //this macro used by USB descriptors, SMART_CARD_CLOCK_IN_KHZ =REF_CLOCK_TO_SMART_CARD/1000
		#define SMART_CARD_DEFAULT_DATA_RATE 8064 //this macro used by USB descriptors, SMART_CARD_DEFAULT_DATA_RATE = REF_CLOCK_TO_SMART_CARD/372

		//Turn on 1/off 0 card power
		#define SCdrv_SetSwitchCardPower(x) 			(LATBbits.LATB0=(x))

		//set reset state to the value x
		#define SCdrv_SetSwitchCardReset(x) 			(LATBbits.LATB4=(x))

		//set tx pin to value x
		#define SCdrv_SetTxPinData(x) 					(LATCbits.LATC6=(x))

		//Get Smart Card Present status
		#define SCdrv_CardPresent()	      				(PORTBbits.RB3 || !PORTBbits.RB1)

		//Get Rx Pin Data
		#define SCdrv_GetRxPinData()               		(PORTCbits.RC7)

		//Set Tx Pin direction
		#define SCdrv_TxPin_Direction(flag)    			(TRISCbits.TRISC6 = flag)

		//Set Rx Pin direction
		#define SCdrv_RxPin_Direction(flag)    			(TRISCbits.TRISC7 = flag)

		//Set Power Pin direction connected to the smart card
		#define SCdrv_PowerPin_Direction(flag) 			(TRISBbits.TRISB0 = flag)

		//Set Reset Pin direction connected to the smart card
		#define SCdrv_ResetPin_Direction(flag) 			(TRISBbits.TRISB4 = flag)

		//Set Card Present Pin direction connected to the smart card
		#define SCdrv_CardPresent_Direction(flag) 		(TRISBbits.TRISB3 = flag)

		//Set Sim Present Pin direction connected to the smart card
		#define SCdrv_SimPresent_Direction(flag) 		(TRISBbits.TRISB1 = flag)

		// Enable Pull up for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_EnableCardSimPresentPinPullUp()	(INTCON2bits.RBPU = 0)

		// Disable Pull up for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_DisbleCardSimPresentPinPullUp()	(INTCON2bits.RBPU = 1)

    #elif defined(__18F45K50)

		#define PICDEM_FS_USB_K50

		// Note : SYS_FREQ denotes the System clock frequency
		#define SYS_FREQ 48000000UL

		// Note : FCY denotes the instruction cycle clock frequency (SYS_FREQ/4)"
		#define FCY (SYS_FREQ/4)

		#include <p18cxxx.h>
		#include <delays.h>
		#include <math.h>

		// Enable Port Pin of Micro as Vcc for Smart Card
		#define  ENABLE_SC_POWER_THROUGH_PORT_PIN

		// Provide clock from the Micro to the Smart Card
		#define  ENABLE_SC_EXTERNAL_CLOCK

		// Set Clock Freq to drive Smart Card
		#define Scdrv_ClockSet()            (TMR2 = 0,T2CON = 0x00,PR2 = 3,CCPR1L = 2,TRISCbits.TRISC2 = 0,CCP1CON = 0x00)

		//Enable UART
		#define SCdrv_EnableUART()	    	(RCSTAbits.SPEN = 1)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableClock()	    	(CCP1CON = 0x0C,T2CONbits.TMR2ON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableClock()	    (CCP1CON = 0x00,T2CONbits.TMR2ON = 0,TMR2 = 0)

		// Set Clock Freq to drive Smart Card
		#define SCdrv_EnableDelayTimerIntr()   (T0CON = 0x86,INTCON2bits.TMR0IP = 0,RCONbits.IPEN = 1,INTCONbits.PEIE = 1,INTCONbits.TMR0IE = 1,INTCONbits.GIEH = 1)

		// One count of timer 0 corresponds to how much micro seconds...
		#define TIMER0_SINGLE_COUNT_MICRO_SECONDS	(BYTE)(128/(FCY/1000000UL))

		// Enable Clock to drive Smart Card
		#define SCdrv_SetDelayTimerCnt(count)	    (TMR0H = (count) >> 8,TMR0L = (count) & 0x00FF)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableDelayTimer()	    	(T0CONbits.TMR0ON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableDelayTimer()	    	(T0CONbits.TMR0ON = 0)

		#define SCdrv_SetClockPinLow() LATCbits.LATC2 = 0;
        #define SCdrv_SetClockPinHigh() LATCbits.LATC2 = 1;

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_CIRCUIT_INPUT_CLK    		FCY

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_POWER2_VALUE      		(BYTE)0

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_DIVISOR_VALUE      		(BYTE)PR2
		#define REF_CLOCK_TO_SMART_CARD     (unsigned long)(FCY/4)
		#define SMART_CARD_CLOCK_IN_KHZ 3000 //this macro used by USB descriptors, SMART_CARD_CLOCK_IN_KHZ =REF_CLOCK_TO_SMART_CARD/1000
		#define SMART_CARD_DEFAULT_DATA_RATE 8064 //this macro used by USB descriptors, SMART_CARD_DEFAULT_DATA_RATE = REF_CLOCK_TO_SMART_CARD/372

		//Turn on 1/off 0 card power
		#define SCdrv_SetSwitchCardPower(x) 			(LATBbits.LATB0=(x))

		//set reset state to the value x
		#define SCdrv_SetSwitchCardReset(x) 			(LATBbits.LATB4=(x))

		//set tx pin to value x
		#define SCdrv_SetTxPinData(x) 					(LATCbits.LATC6=(x))

		//Get Smart Card Present status
		#define SCdrv_CardPresent()	      				(PORTBbits.RB3 || !PORTBbits.RB1)

		//Get Rx Pin Data
		#define SCdrv_GetRxPinData()               		(PORTCbits.RC7)

		//Set Tx Pin direction
		#define SCdrv_TxPin_Direction(flag)    			(TRISCbits.TRISC6 = flag)

		//Set Rx Pin direction
		#define SCdrv_RxPin_Direction(flag)    			(TRISCbits.TRISC7 = flag)

		//Set Power Pin direction connected to the smart card
		#define SCdrv_PowerPin_Direction(flag) 			(TRISBbits.TRISB0 = flag)

		//Set Reset Pin direction connected to the smart card
		#define SCdrv_ResetPin_Direction(flag) 			(TRISBbits.TRISB4 = flag)

		//Set Card Present Pin direction connected to the smart card
		#define SCdrv_CardPresent_Direction(flag) 		(TRISBbits.TRISB3 = flag)

		//Set Sim Present Pin direction connected to the smart card
		#define SCdrv_SimPresent_Direction(flag) 		(TRISBbits.TRISB1 = flag)

		// Enable Pull up for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_EnableCardSimPresentPinPullUp()	(INTCON2bits.RBPU = 0)

		// Disable Pull up for SMART_CARD_DETECTION & SIM_CARD_DETECTION
		#define SCdrv_DisbleCardSimPresentPinPullUp()	(INTCON2bits.RBPU = 1)    

	#elif defined (PIC32MX795F512L_PIM) || defined (PIC32MX460F512L_PIM)

		#define EXPLORER_16

		// Note : SYS_FREQ denotes the System clock frequency
		#define SYS_FREQ 16000000UL

		// Note : FCY denotes the instruction cycle clock frequency (SYS_FREQ/2)"
		#define FCY (SYS_FREQ)

		#include "p32xxxx.h"
		#include "plib.h"
		#include <math.h>

		// Enable Port Pin of Micro as Vcc for Smart Card
		#define  ENABLE_SC_POWER_THROUGH_PORT_PIN

		// Provide clock from the Micro to the Smart Card
		#define  ENABLE_SC_EXTERNAL_CLOCK

		// Set Clock Freq to drive Smart Card
		#define Scdrv_ClockSet()            (TMR2 = 0,T2CON = 0x00,PR2 = 3,OC2R = 2,OC2RS = 2,TRISDbits.TRISD1 = 0,OC2CON = 0x0006)

		//Enable UART
		#define SCdrv_EnableUART()	    	(U1MODEbits.UARTEN = 1)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableClock()	    	(OC2CONbits.ON = 1,T2CONbits.ON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableClock()	    (OC2CONbits.ON = 0,T2CONbits.ON = 0,TMR2 = 0)

		// Set Clock Freq to drive Smart Card
		#define SCdrv_EnableDelayTimerIntr()   (IPC1bits.T1IP = 3,IPC1bits.T1IS = 1,IFS0bits.T1IF = 0,T1CON = 0x0030,PR1 = 0xFFFF,IEC0bits.T1IE = 1,INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR),INTEnableInterrupts())

		// One count of timer 1 corresponds to how much micro seconds...
		#define TIMER1_SINGLE_COUNT_MICRO_SECONDS	(BYTE)(256/(FCY/1000000UL))

		// Enable Clock to drive Smart Card
		#define SCdrv_SetDelayTimerCnt(count)	    (TMR1 = count)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableDelayTimer()	    	(T1CONbits.TON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableDelayTimer()	    	(T1CONbits.TON = 0)

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_CIRCUIT_INPUT_CLK    		FCY

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_POWER2_VALUE      		(BYTE)0

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_DIVISOR_VALUE      		(BYTE)PR2

		#define REF_CLOCK_TO_SMART_CARD				(unsigned long)(FCY/(PR2 + 1))

        //#define REF_CLOCK_TO_SMART_CARD     (unsigned long)((unsigned long long)((unsigned long long)FCY * 2)/(pow(2,3)))
		#define SMART_CARD_CLOCK_IN_KHZ 4000 //this macro used by USB descriptors, SMART_CARD_CLOCK_IN_KHZ =REF_CLOCK_TO_SMART_CARD/1000
		#define SMART_CARD_DEFAULT_DATA_RATE 10752 //this macro used by USB descriptors, SMART_CARD_DEFAULT_DATA_RATE = REF_CLOCK_TO_SMART_CARD/372

        #define SCdrv_SetClockPinLow() LATDbits.LATD0 = 0;
        #define SCdrv_SetClockPinHigh() LATDbits.LATD0 = 1;
        
        
		//Turn on 1/off 0 card power
		#define SCdrv_SetSwitchCardPower(x) 			(LATBbits.LATB9=(x))
		
		//set reset state to the value x
		#define SCdrv_SetSwitchCardReset(x) 			(LATEbits.LATE8=(x))
		
		//set tx pin to the value x
		#define SCdrv_SetTxPinData(x) 					(LATFbits.LATF8=(x))
		
		//Get Smart Card Present status
		#define SCdrv_CardPresent()	      				(PORTBbits.RB0 || !PORTBbits.RB1)

		//Get Rx Pin Data
		#define SCdrv_GetRxPinData()               		(PORTFbits.RF2)
		
		//Set Tx Pin direction
		#define SCdrv_TxPin_Direction(flag)    			(TRISFbits.TRISF8 = flag)
		
		//Set Power Pin direction connected to the smart card
		#define SCdrv_PowerPin_Direction(flag) 			(TRISBbits.TRISB9 = flag)
		
		//Set Reset Pin direction connected to the smart card
		#define SCdrv_ResetPin_Direction(flag) 			(TRISEbits.TRISE8 = flag)
		
		//Set Card Present Pin direction connected to the smart card
		#define SCdrv_CardPresent_Direction(flag) 		(TRISBbits.TRISB0 = flag)
		
		//Set Sim Present Pin direction connected to the smart card
		#define SCdrv_SimPresent_Direction(flag) 		(TRISBbits.TRISB1 = flag)
		
		// Enable Pull up at Tx Pin
		#define SCdrv_EnableTxPinPullUp()				
		
		// Enable Pull up at Rx Pin
		#define SCdrv_EnableRxPinPullUp()				
		
		// Enable Pull up for SMART_CARD_DETECTION
		#define SCdrv_EnableCardPresentPinPullUp()		(CNPUEbits.CNPUE2 = 1)
		
		// Enable Pull up for SIM_CARD_DETECTION
		#define SCdrv_EnableSimPresentPinPullUp()		(CNPUEbits.CNPUE3 = 1)
		
		// Disable Pull up at Tx Pin
		#define SCdrv_DisableTxPinPullUp()				
		
		// Disable Pull up at Rx Pin
		#define SCdrv_DisableRxPinPullUp()				
		
		// MAP UART Rx Pin
		#define MapUART1RxPin()							
		
		// MAP UART Tx Pin
		#define MapUART1TxPin()							

	#elif defined(__dsPIC33FJ128MC710__)

		#define EXPLORER_16

		// Note : SYS_FREQ denotes the System clock frequency
		#define SYS_FREQ 32000000UL

		// Note : FCY denotes the instruction cycle clock frequency (SYS_FREQ/2)"
		#define FCY (SYS_FREQ/2)

		#include "p33Fxxxx.h"
		#include "libpic30.h"
		#include <math.h>

//		#define  WaitMicroSec(MicroSec)    __delay_us(MicroSec)
//		#define  WaitMilliSec(Waitms)      __delay_ms(Waitms)

		// Enable Port Pin of Micro as Vcc for Smart Card
		#define  ENABLE_SC_POWER_THROUGH_PORT_PIN

		// Provide clock from the Micro to the Smart Card
		#define  ENABLE_SC_EXTERNAL_CLOCK

		// Set Clock Freq to drive Smart Card
		#define Scdrv_ClockSet()            (TMR2 = 0,T2CON = 0x00,PR2 = 3,OC2R = 2,OC2RS = 2,TRISDbits.TRISD1 = 0,OC2CON = 0x0006)

		//Enable UART
		#define SCdrv_EnableUART()	    	(U1MODEbits.UARTEN = 1)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableClock()	    	(T2CONbits.TON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableClock()	    (T2CONbits.TON = 0,TMR2 = 0)

		// Set Clock Freq to drive Smart Card
		#define SCdrv_EnableDelayTimerIntr()   (IPC0bits.T1IP = 4,IFS0bits.T1IF = 0,T1CON = 0x0030,PR1 = 0xFFFF,IEC0bits.T1IE = 1)

		// One count of timer 1 corresponds to how much micro seconds...
		#define TIMER1_SINGLE_COUNT_MICRO_SECONDS	(BYTE)(256/(FCY/1000000UL))

		// Enable Clock to drive Smart Card
		#define SCdrv_SetDelayTimerCnt(count)	    (TMR1 = count)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableDelayTimer()	    	(T1CONbits.TON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableDelayTimer()	    	(T1CONbits.TON = 0)

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_CIRCUIT_INPUT_CLK    		FCY

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_POWER2_VALUE      		(BYTE)0

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_DIVISOR_VALUE      		(BYTE)PR2

		#define REF_CLOCK_TO_SMART_CARD     (unsigned long)(FCY/(PR2 + 1))

		//Turn on 1/off 0 card power
		#define SCdrv_SetSwitchCardPower(x) 			(LATBbits.LATB9=(x))
		
		//set reset state to the value x
		#define SCdrv_SetSwitchCardReset(x) 			(LATEbits.LATE8=(x))
		
		//set tx pin to the value x
		#define SCdrv_SetTxPinData(x) 					(LATFbits.LATF3=(x))
		
		//Get Smart Card Present status
		#define SCdrv_CardPresent()	      				(PORTBbits.RB0 || !PORTBbits.RB1)

		//Get Rx Pin Data
		#define SCdrv_GetRxPinData()               		(PORTFbits.RF2)
		
		//Set Tx Pin direction
		#define SCdrv_TxPin_Direction(flag)    			(TRISFbits.TRISF3 = flag)
		
		//Set Power Pin direction connected to the smart card
		#define SCdrv_PowerPin_Direction(flag) 			(TRISBbits.TRISB9 = flag)
		
		//Set Reset Pin direction connected to the smart card
		#define SCdrv_ResetPin_Direction(flag) 			(TRISEbits.TRISE8 = flag)
		
		//Set Card Present Pin direction connected to the smart card
		#define SCdrv_CardPresent_Direction(flag) 		(TRISBbits.TRISB0 = flag)
		
		//Set Sim Present Pin direction connected to the smart card
		#define SCdrv_SimPresent_Direction(flag) 		(TRISBbits.TRISB1 = flag)
		
		// Enable Pull up at Tx Pin
		#define SCdrv_EnableTxPinPullUp()				
		
		// Enable Pull up at Rx Pin
		#define SCdrv_EnableRxPinPullUp()				
		
		// Enable Pull up for SMART_CARD_DETECTION
		#define SCdrv_EnableCardPresentPinPullUp()		(_CN2PUE = 1)
		
		// Enable Pull up for SIM_CARD_DETECTION
		#define SCdrv_EnableSimPresentPinPullUp()		(_CN3PUE = 1)
		
		// Disable Pull up at Tx Pin
		#define SCdrv_DisableTxPinPullUp()				
		
		// Disable Pull up at Rx Pin
		#define SCdrv_DisableRxPinPullUp()				
		
		// MAP UART Rx Pin
		#define MapUART1RxPin()							
		
		// MAP UART Tx Pin
		#define MapUART1TxPin()							

	#elif defined(__PIC24HJ256GP610__)

		#define EXPLORER_16

		// Note : SYS_FREQ denotes the System clock frequency
		#define SYS_FREQ 32000000UL

		// Note : FCY denotes the instruction cycle clock frequency (SYS_FREQ/2)"
		#define FCY (SYS_FREQ/2)

		#include "p24Hxxxx.h"
		#include "libpic30.h"
		#include <math.h>

//		#define  WaitMicroSec(MicroSec)    __delay_us(MicroSec)
//		#define  WaitMilliSec(Waitms)      __delay_ms(Waitms)

		// Enable Port Pin of Micro as Vcc for Smart Card
		#define  ENABLE_SC_POWER_THROUGH_PORT_PIN

		// Provide clock from the Micro to the Smart Card
		#define  ENABLE_SC_EXTERNAL_CLOCK

		// Set Clock Freq to drive Smart Card
		#define Scdrv_ClockSet()            (TMR2 = 0,T2CON = 0x00,PR2 = 3,OC2R = 2,OC2RS = 2,TRISDbits.TRISD1 = 0,OC2CON = 0x0006)

		//Enable UART
		#define SCdrv_EnableUART()	    	(U1MODEbits.UARTEN = 1)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableClock()	    	(T2CONbits.TON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableClock()	    (T2CONbits.TON = 0,TMR2 = 0)

		// Set Clock Freq to drive Smart Card
		#define SCdrv_EnableDelayTimerIntr()   (IPC0bits.T1IP = 4,IFS0bits.T1IF = 0,T1CON = 0x0030,PR1 = 0xFFFF,IEC0bits.T1IE = 1)

		// One count of timer 1 corresponds to how much micro seconds...
		#define TIMER1_SINGLE_COUNT_MICRO_SECONDS	(BYTE)(256/(FCY/1000000UL))

		// Enable Clock to drive Smart Card
		#define SCdrv_SetDelayTimerCnt(count)	    (TMR1 = count)

		// Enable Clock to drive Smart Card
		#define SCdrv_EnableDelayTimer()	    	(T1CONbits.TON = 1)

		// Disable Clock used to drive Smart Card
		#define SCdrv_DisableDelayTimer()	    	(T1CONbits.TON = 0)

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_CIRCUIT_INPUT_CLK    		FCY

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_POWER2_VALUE      		(BYTE)0

		//Reference Clock Circuit - Input Clock
		#define REF_CLOCK_DIVISOR_VALUE      		(BYTE)PR2

		#define REF_CLOCK_TO_SMART_CARD     (unsigned long)(FCY/(PR2 + 1))

		//Turn on 1/off 0 card power
		#define SCdrv_SetSwitchCardPower(x) 			(LATBbits.LATB9=(x))
		
		//set reset state to the value x
		#define SCdrv_SetSwitchCardReset(x) 			(LATAbits.LATA12=(x))
		
		//set tx pin to the value x
		#define SCdrv_SetTxPinData(x) 					(LATFbits.LATF3=(x))
		
		//Get Smart Card Present status
		#define SCdrv_CardPresent()	      				(PORTBbits.RB0 || !PORTBbits.RB1)

		//Get Rx Pin Data
		#define SCdrv_GetRxPinData()               		(PORTFbits.RF2)
		
		//Set Tx Pin direction
		#define SCdrv_TxPin_Direction(flag)    			(TRISFbits.TRISF3 = flag)
		
		//Set Power Pin direction connected to the smart card
		#define SCdrv_PowerPin_Direction(flag) 			(TRISBbits.TRISB9 = flag)
		
		//Set Reset Pin direction connected to the smart card
		#define SCdrv_ResetPin_Direction(flag) 			(TRISAbits.TRISA12 = flag)
		
		//Set Card Present Pin direction connected to the smart card
		#define SCdrv_CardPresent_Direction(flag) 		(TRISBbits.TRISB0 = flag)
		
		//Set Sim Present Pin direction connected to the smart card
		#define SCdrv_SimPresent_Direction(flag) 		(TRISBbits.TRISB1 = flag)
		
		// Enable Pull up at Tx Pin
		#define SCdrv_EnableTxPinPullUp()				
		
		// Enable Pull up at Rx Pin
		#define SCdrv_EnableRxPinPullUp()				
		
		// Enable Pull up for SMART_CARD_DETECTION
		#define SCdrv_EnableCardPresentPinPullUp()		(_CN2PUE = 1)
		
		// Enable Pull up for SIM_CARD_DETECTION
		#define SCdrv_EnableSimPresentPinPullUp()		(_CN3PUE = 1)
		
		// Disable Pull up at Tx Pin
		#define SCdrv_DisableTxPinPullUp()				
		
		// Disable Pull up at Rx Pin
		#define SCdrv_DisableRxPinPullUp()				
		
		// MAP UART Rx Pin
		#define MapUART1RxPin()							
		
		// MAP UART Tx Pin
		#define MapUART1TxPin()							
		
	#else

	#endif

#endif //SCCFG_H
