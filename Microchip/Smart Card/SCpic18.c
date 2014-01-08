/********************************************************************
 FileName:		SCpic18.c
 Dependencies:	See INCLUDES section
 Processor:		PIC18 Microcontrollers
 Hardware:		This demo is natively intended to be used on LPC & HPC Exp board.
 				This demo can be modified for use on other hardware platforms.
 Complier:  	Microchip C18 (for PIC18)
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
  1.0     Initial release
  1.01    Cleaned up unnecessary variables
  1.02    Optimized code by removing redundant code in "SCdrv_SetBRG"
          function.
  1.02.2  Removed recursive function calls and added delay functions.
          Modified the code in more structured way.
  1.02.6  The variable 'delayLapsedFlag' is declared as 'volatile' type,
          as it is modified in the Interrupt Service Routine.
  1.02.8  "SCdrv_InitUART" function is modifed to switch on the power
          supply to the smart card during initialization phase.
********************************************************************/

#include 	<Compiler.h>

#if defined(__18CXX)

#include	"stdio.h"
#include 	"Smart Card/pps18.h"
#include 	"string.h"
#include	"GenericTypeDefs.h"
#include	"Smart Card/SCpic18.h"
#include    "sc_config.h"


unsigned long baudRate;
unsigned long scReferenceClock;
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
	SCdrv_SetDelayTimerCnt(0xFFFF - microSec/TIMER0_SINGLE_COUNT_MICRO_SECONDS);

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
		//calculate parity
		_asm
	     swapf    data,0,0  //reducing byte to nibble
	     xorwf    data,0,0
	     addlw    0x41    // bit 1 becomes B0^B1 and bit 7 becomes B6^B7
	     iorlw    0x7C    // for carry propagation from bit 1 to bit 7
	     addlw    2       // Done! the parity bit is bit 7 of W
		_endasm
		
		TXSTAbits.TX9D = WREG >> 7; //copy calc parity bit to the transmitter
		
		TXREG = data;
		TXSTAbits.TXEN = 1;

		while( !TXSTAbits.TRMT ) //wait for Tx Complete
			Nop();
		
		TXSTAbits.TXEN = 0;

		//turn off uart
		RCSTAbits.SPEN = 0;

		// Receiver pulls Tx line low, to indicate PE or FR errors. If Such a
		// scenario occurs then try to retransmit the byte(maximum of 5 trys allowed).
		if(SCdrv_GetRxPinData())
			break;
		else
			RCSTAbits.SPEN = 1;
	}

	RCSTAbits.SPEN = 1;
	
	if( RCSTA & 0x6 )	//FERR or OERR occured
	{
		RCSTAbits.CREN = 0; //clear error flags
		RCSTAbits.CREN = 1;
	}	

	while(1)	// remove rx data recvd from our Tx line
	{
		BYTE temp;		
		if( PIR1bits.RCIF )
		{
			temp = RCREG;
		}	
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
BOOL SCdrv_GetRxData( BYTE* pDat, unsigned short int timeOutETUs)
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
		while( !(PIR1 & 0x20) )
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
		
		SCdrv_DisableDelayTimer();
		
		delayLapsedFlag = FALSE;

		//calculate parity
		_asm
	     swapf    RCREG,0,0  //reducing byte to nibble
	     xorwf    RCREG,0,0
	     addlw    0x41    // bit 1 becomes B0^B1 and bit 7 becomes B6^B7
	     iorlw    0x7C    // for carry propagation from bit 1 to bit 7
	     addlw    2       // Done! the parity bit is bit 7 of W
	     RLNCF    WREG,0,0		//rotate left W reg
	     xorwf    RCSTA,0,0
		_endasm

		if( WREG & 1 )	//Parity Error detected
		{
			RCSTAbits.SPEN = 0;
			SCdrv_TxPin_Direction(0);  //pull it low to tell the card that there was error receiving data
			
			WaitMicroSec(oneETUtimeinMicroSeconds);
					
			SCdrv_TxPin_Direction(1); //release line. Card should retransmit now.
					
			RCSTAbits.SPEN = 1;
		}
		else
		{
			*pDat = RCREG;
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
	unsigned int baudReg;
	
	baudReg = (unsigned int)((FCY/baudRate) - 1);

	SPBRG = (unsigned char)baudReg;
	SPBRGH = (unsigned char)(baudReg >> 8);

	// Calculate the recieve ETU divisor which will be used to calculate the timeouts
	// while recieving the data bytes. This calculation is done here because baudrate
	// of ISO 7816 communication is recalculated here.
	oneETUtimeinMicroSeconds = (unsigned short int)(1000000UL/baudRate);
	rxETUtimeDivisorFactor = oneETUtimeinMicroSeconds / TIMER0_SINGLE_COUNT_MICRO_SECONDS;
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
	RCSTAbits.SPEN = 0;		//turn off UART
	RCSTAbits.CREN = 0;
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
	unsigned int tempCalculationVariable = 1;
	BYTE power2temp;

	SCdrv_PowerPin_Direction(0);	//set RB0 as output to power the Smart Card
	SCdrv_ResetPin_Direction(0);	//set RB4 as output for Smart Card Reset Pin
	SCdrv_CardPresent_Direction(1);  //RB3 Input Card Present
	SCdrv_SimPresent_Direction(1);   //RB1 Input Card Present
	
	SCdrv_SetTxPinData(0);
	SCdrv_TxPin_Direction(1);	// TX1 also act as gpio to pull the line low
	SCdrv_RxPin_Direction(1);	
	
	SCdrv_EnableCardSimPresentPinPullUp();	//enable pull up, active low

	SCdrv_SetSwitchCardPower(1);	//Turn on power to smart card
	SCdrv_SetSwitchCardReset(0); //keep card in reset state		

	// Set the clock value that you want to trigger the smart card.
	// Dont enable the clock,only set it.
	Scdrv_ClockSet();

	// Initial Baud Rate of Smart Card for external Clock
	power2temp = REF_CLOCK_POWER2_VALUE;

	while(power2temp--)
	{
		tempCalculationVariable = tempCalculationVariable * (BYTE)2;
	}

	scReferenceClock = REF_CLOCK_CIRCUIT_INPUT_CLK/(tempCalculationVariable + REF_CLOCK_DIVISOR_VALUE);

	baudRate = scReferenceClock/(unsigned int)372;

	// Calculate the recieve ETU divisor which will be used to calculate the timeouts
	// while recieving the data bytes. This calculation is done here because baudrate
	// of ISO 7816 communication is calculated here.
	oneETUtimeinMicroSeconds = (unsigned short int)(1000000UL/baudRate);
	rxETUtimeDivisorFactor = oneETUtimeinMicroSeconds / TIMER0_SINGLE_COUNT_MICRO_SECONDS;

	tempCalculationVariable = ((FCY/baudRate) - 1);
	SPBRG = (unsigned char)tempCalculationVariable;
	SPBRGH = (unsigned char)(tempCalculationVariable >> 8);

	TXSTAbits.TX9  = 1; //select 9 bit transmission
	TXSTAbits.SYNC = 0; //Async mode	
	TXSTAbits.BRGH = 1;
	BAUDCONbits.BRG16 = 1;

	RCSTAbits.CREN = 1;
	RCSTAbits.RX9  = 1;

	// Enable Delay Timer Interrupts for future use
	SCdrv_EnableDelayTimerIntr();
}

#endif //__18CXX
