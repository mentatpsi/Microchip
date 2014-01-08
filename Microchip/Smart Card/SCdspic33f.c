/********************************************************************
 FileName:		SCdspic33f.c
 Dependencies:	See INCLUDES section
 Processor:		dsPIC33F Microcontrollers
 Hardware:		This demo is natively intended to be used on Exp 16 board.
 				This demo can be modified for use on other hardware platforms.
 Complier:  	Microchip C30 (for dsPIC33F)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the �Company�) for its PIC� Microcontroller is intended and
 supplied to you, the Company�s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
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
  1.02     Initial release
  1.02.2   Removed recursive function calls and added delay functions.
           Modified the code in more structured way.
  1.02.6  The variable 'delayLapsedFlag' is declared as 'volatile' type,
          as it is modified in the Interrupt Service Routine.
  1.02.8  "SCdrv_InitUART" function is modifed to switch on the power
          supply to the smart card during initialization phase.
********************************************************************/

#include    <Compiler.h>

#if defined(__dsPIC33F__)

#include 	"string.h"
#include	"GenericTypeDefs.h"
#include 	"sc_config.h"
#include    "./Smart Card/SCdspic33f.h"

unsigned long baudRate;
unsigned long scReferenceClock; // Smart Card Reference Clock
unsigned short int rxETUtimeDivisorFactor;
unsigned short int oneETUtimeinMicroSeconds;
volatile BOOL delayLapsedFlag = FALSE;

/*******************************************************************************
  Function:
    void WaitMicroSec(void)
	
  Description:
    This function provides the approximate delay in microseconds.The delay
    sometimes may not be exact delay as per 'microsec' input value, but
    is approximately the same. This approximatation is sufficient for
    smart card stack requirement.

  Precondition:
    None

  Parameters:
    microSec - Number of microseconds delay required

  Return Values:
    None
	
  Remarks:
    This function supports upto 10000's and more of micro seconds delay. 
    For higher time values use 'WaitMilliSec' function.
  *****************************************************************************/

void WaitMicroSec( unsigned long int microSec )
{
	// Set the Timer Count as per the delay needed
	SCdrv_SetDelayTimerCnt(0xFFFF - microSec/TIMER1_SINGLE_COUNT_MICRO_SECONDS);

	// Enable the delay timer
	SCdrv_EnableDelayTimer();

	// Wait until the delay is elapsed
	while(!delayLapsedFlag);

	// Clear the delay flag
	delayLapsedFlag = FALSE;
}

/*******************************************************************************
  Function:
    void WaitMilliSec(void)
	
  Description:
    This function provides the approximate delay in milliseconds.The delay
    sometimes may not be exact delay as per 'ms' input value, but is
    approximately the same. This approximatation is sufficient for
    smart card stack requirement.

  Precondition:
    None

  Parameters:
    ms - Number of milliseconds delay required

  Return Values:
    None
	
  Remarks:
    None
  *****************************************************************************/

void WaitMilliSec( unsigned long int ms )
{
	while(ms--)
		WaitMicroSec(1000);
}

/*******************************************************************************
  Function:
    void SCdrv_SendTxData(BYTE data)
	
  Description:
    This function transmits one byte of data as per ISO-7816 standard

  Precondition:
    None.

  Parameters:
    data - Data byte to be transmitted

  Return Values:
    None
	
  Remarks:
    None.
  *****************************************************************************/

void SCdrv_SendTxData( BYTE data )
{
	BYTE txRetryCounter;

	// Try to trasmit the byte for 5 times before giving up...
	for(txRetryCounter = 0;txRetryCounter < 5;txRetryCounter++)
	{
		U1STAbits.UTXEN = 1;
		U1TXREG = data;		

		while( !U1STAbits.TRMT )
			Nop();
		
		U1STAbits.UTXEN = 0;

		U1MODEbits.UARTEN = 0;	// Disable UART Module

//		WaitMicroSec( oneETUtimeinMicroSeconds/2 );	// 0.5 Bit time delay
		
		// Receiver pulls Tx line low, to indicate PE or FR errors. If Such a
		// scenario occurs then try to retransmit the byte(maximum of 5 trys allowed).
		if(SCdrv_GetRxPinData())
			break;
		else
		{
			WaitMicroSec(oneETUtimeinMicroSeconds);  //wait one etu before repeating
			
			U1MODEbits.UARTEN = 1;
		}
	}

	U1MODEbits.UARTEN = 1;	// Enable UART Module
	
	U1STAbits.OERR = 0;	//clear any overflow error that we caused
	
	while(1)	// remove rx data recvd from our Tx line
	{
		WORD temp;		
		if( U1STAbits.URXDA )
			temp = U1RXREG;
		else
			break;
	}	
}

/*******************************************************************************
  Function:
    BOOL SCdrv_GetRxData( BYTE* pDat, unsigned long int timeOutInMicroSeconds)
	
  Description:
    This function recieves one byte of data as per ISO-7816 standard

  Precondition:
    None.

  Parameters:
    pDat - pointer to Data byte to be recieved
    timeOutETUs - maximum etu time within which the data byte is
                  is expected to be recieved

  Return Values:
    None
	
  Remarks:
    None.
  *****************************************************************************/

BOOL SCdrv_GetRxData( BYTE* pDat,unsigned short int timeOutETUs )
{
	unsigned long int tmrCntDifference = timeOutETUs * rxETUtimeDivisorFactor;
	unsigned short int timerCountValue;
	BYTE rxRetryCounter;

	delayLapsedFlag = TRUE;
	
	// If any error occured while recieving the byte, give chance to smart card
	// to send the corrected byte within 5 attempts
	for(rxRetryCounter = 0;rxRetryCounter < 5;rxRetryCounter++)
	{
		//wait for data byte
		while(!U1STAbits.URXDA)
		{
			if( delayLapsedFlag )
			{
				delayLapsedFlag = FALSE;
				
				if(tmrCntDifference)
				{
					if(tmrCntDifference > 0xFFFF)
					{
						timerCountValue = 0;
						tmrCntDifference = tmrCntDifference - 0xFFFF;
					}
					else
					{
						timerCountValue = (unsigned short int)tmrCntDifference;
						tmrCntDifference = 0;
					}

					// Set the Timer Count as per the delay needed
					SCdrv_SetDelayTimerCnt(0xFFFF - timerCountValue);

					// Enable the delay timer
					SCdrv_EnableDelayTimer();
				}
				else
					return FALSE;
			}
		}
		
		// Disable Timer if it is still running
		SCdrv_DisableDelayTimer();
		
		delayLapsedFlag = FALSE;

		if( U1STAbits.PERR )	//Parity Error detected
		{
			SCdrv_TxPin_Direction(0);  //pull it low to tell the card that there was error receiving data

			U1MODEbits.URXINV = 1;  //do not recognize this low state as a valid start bit
					
			//Read the data from UART to clear the error flag
			*pDat = U1RXREG;		
		
			WaitMicroSec( oneETUtimeinMicroSeconds/2 );	// 0.5 Bit time delay

			SCdrv_TxPin_Direction(1); //release RD10. Card should retransmit now.

			U1MODEbits.URXINV = 0;
		}
		else
		{
			//Read the data from UART
			*pDat = U1RXREG;
			return TRUE;
		}
	}

	return FALSE;
}

/*******************************************************************************
  Function:
    void SCdrv_SetBRG()
	
  Description:
    This function sets the UART baud rate as per the speed code used in ISO 7816
    standard communication.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None
	
  Remarks:
    None
  *****************************************************************************/
void SCdrv_SetBRG(void)
{
	U1BRG = (unsigned int)((unsigned long)((unsigned long)(FCY/4)/baudRate) - 1);

	// Calculate the recieve ETU divisor which will be used to calculate the timeouts
	// while recieving the data bytes. This calculation is done here because baudrate
	// of ISO 7816 communication is recalculated here.
	oneETUtimeinMicroSeconds = (unsigned short int)(1000000UL/baudRate);
	rxETUtimeDivisorFactor = oneETUtimeinMicroSeconds / TIMER1_SINGLE_COUNT_MICRO_SECONDS;
}

/*******************************************************************************
  Function:
    void SCdrv_CloseUART(void)
	
  Description:
    This function closes the UART used for ISO 7816 standard communication

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None
	
  Remarks:
    None
  *****************************************************************************/
void SCdrv_CloseUART(void)
{
	U1MODEbits.UARTEN = 0;  // Turn off UART
	U1STAbits.OERR = 0;;

	// Disable Pull-ups at Tx & Rx pins
	SCdrv_DisableTxPinPullUp();
	SCdrv_DisableRxPinPullUp();	
}

/*******************************************************************************
  Function:
    void SCdrv_InitUART(void)
	
  Description:
    This function initializes the UART used for ISO 7816 standard communication

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None
	
  Remarks:
    None
  *****************************************************************************/
void SCdrv_InitUART(void)
{
	unsigned int power2Value = 1;
	BYTE power2temp;

	#ifdef ENABLE_SC_POWER_THROUGH_PORT_PIN
		SCdrv_PowerPin_Direction(0);	//set RG8 as output to power the Smart Card
	#endif
	
	SCdrv_ResetPin_Direction(0);	//set RD0 as output for Smart Card Reset Pin
	SCdrv_CardPresent_Direction(1);   //RD3 Input Card Present - SmartCard Conn Active Hi
	SCdrv_SimPresent_Direction(1);   //RG12 Input Card Present - SimCard Conn Active Low
	
	SCdrv_SetTxPinData(0);
	SCdrv_TxPin_Direction(1);	// use as gpio to pull the line low

	//Turn on the pull-up on both RX and TX line for faster transitions.
	SCdrv_EnableTxPinPullUp();
	SCdrv_EnableRxPinPullUp();

	// Enable Pull-ups for card present/sim present pins
	SCdrv_EnableCardPresentPinPullUp();
	SCdrv_EnableSimPresentPinPullUp();

	#ifdef ENABLE_SC_POWER_THROUGH_PORT_PIN
		SCdrv_SetSwitchCardPower(1);	//Turn on power to smart card
	#endif

	SCdrv_SetSwitchCardReset(0); //keep card in reset state		

	MapUART1RxPin();		// Map UART1 Rx pin
	MapUART1TxPin();		// Map UART1 Tx pin

	Scdrv_ClockSet();

	// Initial Baud Rate of Smart Card for external Clock
	power2temp = REF_CLOCK_POWER2_VALUE;

	while(power2temp--)
	{
		power2Value = power2Value * (BYTE)2;
	}

	scReferenceClock = REF_CLOCK_CIRCUIT_INPUT_CLK/(power2Value + REF_CLOCK_DIVISOR_VALUE);
	
	baudRate = scReferenceClock/372;

	// Calculate the recieve ETU divisor which will be used to calculate the timeouts
	// while recieving the data bytes. This calculation is done here because baudrate
	// of ISO 7816 communication is calculated here.
	oneETUtimeinMicroSeconds = (unsigned short int)(1000000UL/baudRate);
	rxETUtimeDivisorFactor = oneETUtimeinMicroSeconds / TIMER1_SINGLE_COUNT_MICRO_SECONDS;

	U1BRG = (unsigned int)((unsigned long)((unsigned long)(FCY/4)/baudRate) - 1);

	U1MODEbits.PDSEL	= 1;	//8bits + even parity
	U1MODEbits.STSEL	= 0;	//1 stop bit
	U1MODEbits.BRGH		= 1;

	// Enable Delay Timer Interrupts for future use
	SCdrv_EnableDelayTimerIntr();
}

/*********************************************************************
 * Function:        void _T1Interrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Timer 1 overflow interrupt.
 *
 * Note:            None
 ********************************************************************/
void _ISRFAST __attribute__((interrupt, auto_psv)) _T1Interrupt(void)
{
	// Clear Timer Interrupt Flag
	IFS0bits.T1IF = 0;
	SCdrv_DisableDelayTimer();
	delayLapsedFlag = 1;
}

#endif //__dsPIC33F__


