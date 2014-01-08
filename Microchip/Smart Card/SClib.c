/********************************************************************
 FileName:		SClib.c
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
  1.0      Initial release
  1.01     Cleaned up unnecessary variables,supported T=1 protocol
           and improvments in T=0 functions following the coding standards
  1.02     Disciplined the code. No functional changes made.
  1.02.2   Added F and D calculation function.
           Modifed PPS functionality.
           Fixed BWT (Block Wait Time) and WT (Wait Time) calculation issues.
           Modified the code to make it well structured and organized.
  1.02.4   1) The wait time was getting reinitialized to default value
              while communicating with smart card using T = 0 protocol.
              So deleted "t0WWTetu = 10752;" in "SC_TransactT0" function.
           2) Modified the function "SC_SendT1Block" in such a way that EDC 
              is transmitted more effeciently for LRC/CRC mode in T = 1 protocol.
           3) Initialized local variable "txLength" to '0' in function 
              "SC_TransactT1" to remove non-critical compiler warnings.
  1.02.6   1) Changed the size of some input/output parameters of following static function:-
                  'SC_UpdateCRC','SC_UpdateEDC' & 'SC_SendT1Block'.
              This fix is done to optimize the code.
           2) Modified the contents of 'SC_UpdateCRC' & 'SC_SendT1Block' function to suit
              the above change.
           3) Modified "SC_TransactT0" function, to transmit first byte as 0x00 
              when LC & LE bytes are 0x00.
           4) Changed the local variable 'edc' from 'WORD' type to 'unsigned short int' type.
              (In static function :- 'SC_ReceiveT1Block')
  1.02.8   1) "SC_TransactT0" function is modified to handle a 256 bytes read from smart card as
              per the "Case 2S" requirement of ISO 7816 specification.
           2) The assignment of "apduResponse->SW1" & "apduResponse->SW2" is modified in 
             "SC_TransactT1" function
  1.03     1) Changed the data type of variable "cgtETU" from "BYTE" to "unsigned short int".
           2) Modified "SC_DoPPS" function, so as to add the guard time between transmission of
              bytes to smart card.
           3) Modified "SC_CalculateWaitTime" function, so as to calculate correct guard & wait
              time values.
********************************************************************/

#include <string.h>
#include "./Smart Card/SClib.h"
#include "sc_config.h"

#if defined(__PIC24F__) || defined(__PIC24H__)
	#include "./Smart Card/SCpic24.h"
#elif defined(__PIC32MX__)
	#include "./Smart Card/SCpic32.h"
#elif defined(__dsPIC33F__)
	#include "./Smart Card/SCdspic33f.h"
#else
	#ifdef __18CXX
	#include "./Smart Card/SCpic18.h"
	#else
	#error "Only PIC18 and PIC24F currently supported by SmartCard Library"
	#endif
#endif

#define MAX_ATR_LEN			(BYTE)33


unsigned int factorF = 372;
BYTE factorDNumerator = 1;
BYTE factorDdenominator = 1;

BYTE scCardATR[MAX_ATR_LEN];
BYTE scATRLength = 0;

BYTE scTA1, scTA2, scTA3;
BYTE scTB1, scTB2, scTB3;
BYTE scTC1, scTC2, scTC3;
BYTE scTD1, scTD2, scTD3;

BYTE* scATR_HistoryBuffer = NULL;
BYTE  scATR_HistoryLength = 0;

BYTE scPPSresponseLength;
BYTE scPPSresponse[7];

typedef enum
{
	UNKNOWN,
	ATR_ON
} SC_STATUS;

SC_STATUS gCardState = UNKNOWN;
SC_ERROR scLastError = SC_ERR_NONE;

// Work Wait time for T=0 Protocol in units of etu's
unsigned long t0WWTetu;

static void SC_FindFDvalues(BYTE tA1Type);
static void SC_CalculateWaitTime(void);

#ifdef SC_PROTO_T1

	#define R_BLOCK_IDENTIFIER		(BYTE)0x80
	#define S_BLOCK_IDENTIFIER		(BYTE)0xC0
	#define M_BIT_SET				(BYTE)0x20
	#define M_BIT_CLR				(BYTE)0xDF
	#define S_BIT_SET				(BYTE)0x40
	#define S_BIT_CLR				(BYTE)0xBF
	#define S_BIT_POSITION			(BYTE)0x40

	unsigned int t1BWTetu;
	unsigned int t1CWTetu;

	BYTE t1BGTetu = 22;

	BYTE edcType = SC_LRC_TYPE_EDC;
	BYTE maxSegmentLength = 0x20;
	BOOL txSbit = TRUE;

	static unsigned short int SC_UpdateCRC(BYTE data,unsigned short int crc);
	static void SC_UpdateEDC(BYTE data,unsigned short int *edc);
	static void SC_SendT1Block(BYTE nad,BYTE pcb,BYTE length,BYTE *buffer);
	static BOOL SC_ReceiveT1Block(BYTE *rxNAD,BYTE *rxPCB,BYTE *rxLength,BYTE *buffer,unsigned long blockWaitTime);

#endif

// Character Guard Time for T=0 & T=1 Protocol
unsigned short int cgtETU;
unsigned int cgtInMicroSeconds;

// CLA set to '00' = no command chaining, 
//                   no secure messaging, 
//					 basic logical channel.

/*******************************************************************************
  Function:
    void SC_Initialize(void)
	
  Description:
    This function initializes the smart card library

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None
	
  Remarks:
    None
  *****************************************************************************/
void SC_Initialize()
{
	//Initialize the low level driver
	SCdrv_InitUART();

	#ifdef SC_PROTO_T1
	// Initialize smart card library variables
	txSbit = TRUE;
	#endif
}

/*******************************************************************************
  Function:
	BOOL SC_CardPresent(void)	
  
  Description:
    This macro checks if card is inserted in the socket

  Precondition:
    SC_Initialize() is called

  Parameters:
    None

  Return Values:
    TRUE if Card is inserted, otherwise return FALSE
	
  Remarks:
    None
  *****************************************************************************/
BOOL SC_CardPresent()
{
	return SCdrv_CardPresent();
}	

/*******************************************************************************
  Function:
	BOOL SC_PowerOnATR(void)	
  
  Description:
    This function performs the power on sequence of the SmartCard and 
	interprets the Answer-to-Reset data received from the card.

  Precondition:
    SC_Initialize() is called, and card is present

  Parameters:
    None

  Return Values:
    TRUE if Answer to Reset (ATR) was successfuly received and processed
	
  Remarks:
    None
  *****************************************************************************/
BOOL SC_PowerOnATR()
{
	unsigned long atrDelayETUs;

	 //check whether card is present before doing power on reset to the card
	if( !SCdrv_CardPresent() )
	{
		gCardState = UNKNOWN;
		return FALSE;
	}
		
	//make sure card is in reset
	SCdrv_SetSwitchCardReset(0);
	memset( scCardATR, 0xFF, sizeof scCardATR );

	WaitMilliSec(2);

	//Turn on power to the card
	#ifdef ENABLE_SC_POWER_THROUGH_PORT_PIN
		SCdrv_SetSwitchCardPower(1);
	#endif

	scATR_HistoryLength = 0;
	scATR_HistoryBuffer = NULL;
	gCardState = UNKNOWN;
	scLastError = SC_ERR_NONE;
	scATRLength = 0;
	
	// Wait for maximum of 40,000 to 45,000 smard card clock cycles 
	// to get an ATR from card.The number of clock cycles is converted
	// into etu's for easier usage in the code.
	atrDelayETUs = (unsigned short int)((45000UL * baudRate)/scReferenceClock);

	WaitMilliSec(2);

	// Enable UART
	SCdrv_EnableUART();

	WaitMilliSec(2);

	//Start the clock
	SCdrv_EnableClock();

	// Wait for atleast 400 Clock Cycles after applying reference clock to card.
	WaitMilliSec(2);

	//Release card reset line. set to high state
	SCdrv_SetSwitchCardReset(1);

	// Read Answer to RESET
	while(1)
	{
		//wait for data byte from CARD
		if( SCdrv_GetRxData( &scCardATR[scATRLength], atrDelayETUs ) )
		{
			scATRLength++;

			if( scATRLength == MAX_ATR_LEN )
				break;
			else
				atrDelayETUs = (unsigned short int)9600;
		}
		else
			break;	//no data
	}
		
	//decode the ATR values
	if( scATRLength >= 3 ) //min TS, T0 and setup byte
	{
		BYTE T0 = scCardATR[1];	
		BYTE atrIdx = 2;
		
		//Extract Interface bytes TAx TBx TCx and TDx from ATR
		
		scTA1 = scTB1 = scTC1 = scTD1 = 0;
		scTA2 = scTB2 = scTC2 = scTD2 = 0;
		scTA3 = scTB3 = scTC3 = scTD3 = 0;
		
		// Read the global interface bytes
		
		if( T0 & 0x10 )
			scTA1 = scCardATR[atrIdx++];
			
		if( T0 & 0x20 )
			scTB1 = scCardATR[atrIdx++];
			
		if( T0 & 0x40 )
			scTC1 = scCardATR[atrIdx++];

		if( T0 & 0x80 )
			scTD1 = scCardATR[atrIdx++];
			
		//read the next set of interface bytes if present
		if( scTD1 & 0xF0 )
		{
			if( scTD1 & 0x10 )
				scTA2 = scCardATR[atrIdx++];
				
			if( scTD1 & 0x20 )
				scTB2 = scCardATR[atrIdx++];
				
			if( scTD1 & 0x40 )
				scTC2 = scCardATR[atrIdx++];
				
			if( scTD1 & 0x80 )
				scTD2 = scCardATR[atrIdx++];
				
			if( scTD2 & 0xF0 )
			{
				if( scTD2 & 0x10 )
				{
					scTA3 = scCardATR[atrIdx++];
					
					if ((scTA3 < 0x10) || (scTA3 == 0xFF))
					{
						SC_Shutdown();
						scLastError = SC_ERR_ATR_DATA;
						return FALSE;
					}

					#ifdef SC_PROTO_T1
						maxSegmentLength = scTA3;
					#endif
				}

				if( scTD2 & 0x20 )
					scTB3 = scCardATR[atrIdx++];
					
				if( scTD2 & 0x40 )
				{
					scTC3 = scCardATR[atrIdx++];

					#ifdef SC_PROTO_T1
						edcType = (scTC3 & 0x01) ? SC_CRC_TYPE_EDC : SC_LRC_TYPE_EDC;
					#endif
				}

				if( scTD2 & 0x80 )
					scTD3 = scCardATR[atrIdx++];				
			}
		}
		
		scATR_HistoryLength = T0 & 0x0F;
		scATR_HistoryBuffer = (scATR_HistoryLength)?(&scCardATR[atrIdx]):NULL;
		// Calculate the protocol wait times for default values
		SC_CalculateWaitTime();
		gCardState = ATR_ON;
		return TRUE;
	}
	else
	{
		// Not a Valid ATR Reponse
		scLastError = SC_ERR_BAR_OR_NO_ATR_RESPONSE;
		gCardState = UNKNOWN;	
		SC_Shutdown();	
		return FALSE;
	}	
}


/*******************************************************************************
  Function:
	BOOL SC_DoPPS(BYTE *ppsPtr)
  
  Description:
    This function does the PPS exchange with the smart card & configures the baud 
    rate of the PIC UART module as per the PPS response from the smart card.

  Precondition:
    SC_PowerOnATR was success

  Parameters:
    Input is pointer to PPS string

  Return Values:
    TRUE if PPS exchange is successful
	
  Remarks:
    This function is called when SC_PowerOnATR() returns TRUE.
  *****************************************************************************/
BOOL SC_DoPPS(BYTE *ppsPtr)
{
	BYTE ppsn[3] = {0,0,0};
	BYTE index1,index2 = 0x10,index3 = 2,ppsStrLength = 3,pckByte = 0x00;
	unsigned int ppsCGTinMicroSeconds;

	// For PPS exchange GT = 12 ETUs
	ppsCGTinMicroSeconds = (12 * 1000000UL)/baudRate;

	// Calculate the length of PPS request and store PPS1, PPS2 and PPS3
	// in local variables for future calculations
	for(index1 = 0;index1 < 3;index1++)
	{
		if(ppsPtr[1] & index2)
		{
			ppsStrLength++;
			ppsn[index1] = ppsPtr[index3];
			index3++;
		}
		index2 = index2 << 1;
	}

	// Check for the conditions whether PPS can be done to the card...
	scPPSresponseLength = 0;
	if(!SCdrv_CardPresent() || (gCardState != ATR_ON) || (scTD1 & 0x10))
		return FALSE;

	// Send PPS request to the card
	index1 = 0;
	while(ppsStrLength--)
	{
		WaitMicroSec(ppsCGTinMicroSeconds);
		SCdrv_SendTxData( ppsPtr[index1++] );
	}

	// Recieve PPS response from the smart card
	index1 = 0;
	while (1)
	{
		//wait for data byte from CARD
		if(SCdrv_GetRxData( &scPPSresponse[index1], 9600 ) )
		{
			if(++index1 == 7)
				return FALSE;
		}	
		else
			break;
	}

	// If PPS response length is greater or equal to 3 bytes and PPSS byte is 0xFF and lower 
	// nibble of PPS0 byte of PPS response matches with the PPS request then proceed for 
	// further calculation or else exit...
	if((index1 > 2) && (scPPSresponse[0] == 0xFF) && ((ppsPtr[1] & 0x0F) == (scPPSresponse[1] & 0x0F)))
	{
		index2 = 2;
		if(scPPSresponse[1] & 0x10)
		{
			// Check PPS1 Response is identical to PPS1 Request.
			if(scPPSresponse[index2] != ppsn[0])
				return FALSE;
			index2++;
		}

		if(scPPSresponse[1] & 0x20)
		{
			// Check PPS1 Response is identical to PPS1 Request.
			if(scPPSresponse[index2] != ppsn[1])
				return FALSE;
			index2++;
		}

		if(scPPSresponse[1] & 0x40)
		{
			// Check PPS1 Response is identical to PPS1 Request.
			if(scPPSresponse[index2] != ppsn[2])
				return FALSE;
		}

		// Calculate PCK for the PPS response and ex-or it with recieved PCK byte
		pckByte = 0x00;
		for(index2 = 0;index2 < index1;index2++)
			pckByte = pckByte ^ scPPSresponse[index2];

		// If the final vaue is non-zero then exit...
		if(pckByte)
			return FALSE;
		
		// If baud rate modification request has been accepted by the smart card,
		// change the UART baud rate and other wait time constants
		if(scPPSresponse[1] & 0x10)
		{
			SC_FindFDvalues(scPPSresponse[2]);
			// Calculate the new baud rate
			baudRate = (unsigned long long)((unsigned long long)((unsigned long long)scReferenceClock * factorDNumerator)/(unsigned long)(factorF * (unsigned long)factorDdenominator));
			// Configure UART for new baud rate
			SCdrv_SetBRG();
			SC_CalculateWaitTime();
		}

		// Store the PPS response length in global variable
		scPPSresponseLength = index1;

		return TRUE;
	}
	else
		return FALSE;
}

/*******************************************************************************
  Function:
    int SC_GetCardState(void)
	
  Description:
    This function returns the current state of SmartCard

  Precondition:
    SC_Initialize is called.

  Parameters:
    None

  Return Values:
    SC_STATE_CARD_NOT_PRESENT:		No Card Detected
    SC_STATE_CARD_ACTIVE:			Card is powered and ATR received
    SC_STATE_CARD_INACTIVE:			Card present but not powered
	
  Remarks:
    None
  *****************************************************************************/
int SC_GetCardState()
{
	if( !SCdrv_CardPresent() )
		return SC_STATE_CARD_NOT_PRESENT;
	else if( gCardState == ATR_ON )
		return SC_STATE_CARD_ACTIVE;
	else
		return SC_STATE_CARD_INACTIVE;
}		

/*******************************************************************************
  Function:
    void SC_Shutdown(void)
	
  Description:
    This function Performs the Power Down sequence of the SmartCard

  Precondition:
    SC_Initialize is called.

  Parameters:
    None

  Return Values:
    None
	
  Remarks:
    None
  *****************************************************************************/
void SC_Shutdown()
{
	//Bring reset line low
	SCdrv_SetSwitchCardReset(0);

	WaitMilliSec(1);

	// Turn off external Clock given to the card
	SCdrv_DisableClock();
	
	// Shut down UART and remove any pullups
	SCdrv_CloseUART();
	
	// Turn Off Card Power
	#ifdef ENABLE_SC_POWER_THROUGH_PORT_PIN
		SCdrv_SetSwitchCardPower(0);
	#endif
	gCardState = UNKNOWN;
}


/*******************************************************************************
  Function:
    void SC_FindFDvalues(BYTE tA1Type)
	
  Description:
    This function finds the clock rate and baud rate adjustment integers(F,D)

  Precondition:
    SC_PowerOnATR is called.

  Parameters:
    Input byte as encoded in TA1 character

  Return Values:
    None
	
  Remarks:
  *****************************************************************************/
static void SC_FindFDvalues(BYTE tA1Type)
{
	BYTE ta1Code;

	ta1Code = tA1Type & 0x0F;

	factorDNumerator = 1;
	factorDdenominator = 1;

	// Calculate Factor 'D' from TA1 value as per ISO 7816-3 specifications
	switch(ta1Code)
	{
		case 0x00:
		case 0x07:
		case 0x01:
					break;

		case 0x02:
					factorDNumerator = 2;
					break;

		case 0x03:
					factorDNumerator = 4;
					break;

		case 0x04:
					factorDNumerator = 8;
					break;

		case 0x05:
					factorDNumerator = 16;
					break;

		case 0x06:
					factorDNumerator = 32;
					break;

		case 0x08:
					factorDNumerator = 12;
					break;

		case 0x09:
					factorDNumerator = 20;
					break;

		case 0x0A:
					factorDdenominator = 2;
					break;

		case 0x0B:
					factorDdenominator = 4;
					break;

		case 0x0C:
					factorDdenominator = 8;
					break;

		case 0x0D:
					factorDdenominator = 16;
					break;

		case 0x0E:
					factorDdenominator = 32;
					break;

		case 0x0F:
					factorDdenominator = 64;
					break;
	}

	ta1Code = (tA1Type & 0xF0) >> 4;
		
	factorF = 372;

	// Calculate Factor 'F' from TA1 value as per ISO 7816-3 specifications
	switch(ta1Code)
	{
		case 0x00:
		case 0x07:
		case 0x08:
		case 0x0E:
		case 0x0F:
		case 0x01:
					break;

		case 0x02:
					factorF = 558;
					break;

		case 0x03:
					factorF = 744;
					break;
	
		case 0x04:
					factorF = 1116;
					break;
	
		case 0x05:
					factorF = 1488;
					break;
	
		case 0x06:
					factorF = 1860;
					break;
	
		case 0x09:
					factorF = 512;
					break;
	
		case 0x0A:
					factorF = 768;
					break;
	
		case 0x0B:
					factorF = 1024;
					break;
	
		case 0x0C:
					factorF = 1536;
					break;
	
		case 0x0D:
					factorF = 2048;
					break;	
	}
}


/*******************************************************************************
  Function:
    void SC_CalculateWaitTime(void)
	
  Description:
    This function calculates the wait time values for T=0 and T=1 Protocol

  Precondition:
    SC_PowerOnATR is called.

  Parameters:
    None

  Return Values:
    None
	
  Remarks:
  *****************************************************************************/
static void SC_CalculateWaitTime()
{
	BYTE tempVariable1;
	#ifdef SC_PROTO_T1
	BYTE tb2Code,index;
	unsigned int tempVariable2 = 1;
	#endif

	// Calculate Character Guard Time ETU
	if(scTC1 != 0xFF)
	{
		cgtETU = 12 + scTC1;
	}

	// Check whether T=0 or T=1 protocol ?
	switch(scTD1 & 0x0F)
	{
		// For T = 1 protocol
		case 1 :
					// Calculate Character Guard Time in ETU's for T=1 Protocol
					if(scTC1 == 0xFF)
					{
						cgtETU = (BYTE)11;
					}
		
					#ifdef SC_PROTO_T1

						if(scTD1 & 0x20)
						{
							tb2Code = scTB2 & 0x0F;
						
							tempVariable1 = (scTB2 & 0xF0) >> 4;
						}
						else
						{
							tb2Code = SC_CWI;

							tempVariable1 = SC_BWI;

						}

						for(index = 0;index < tb2Code;index++)
							tempVariable2 = tempVariable2 * 2;

						// Calculate Character Wait Time in ETU's for T=1 Protocol as set in the card
						t1CWTetu = 11 + tempVariable2;
						
						tempVariable2 = 1;

						for(index = 0;index < tempVariable1;index++)
							tempVariable2 = tempVariable2 * 2;

						// Calculate Block Wait Time in ETU's for T=1 Protocol as set in the card						
						t1BWTetu = 11 + (unsigned int)((unsigned long)tempVariable2 * (((357120UL * factorDNumerator)/factorF)/factorDdenominator));
					
					#endif

					break;
		
		// For T = 0 Protocol
		case 0 :
		default :

					// Calculate Character Guard Time in ETU's for T=0 Protocol		
					if(scTC1 == 0xFF)
					{
						cgtETU = (BYTE)12;
					}

					// If scTC2 is transmitted by the card then calculate work wait time
					// or else use default value
					if(scTD1 & 0x40)
					{
						tempVariable1 = scTC2;
					}
					else
					{
						tempVariable1 = SC_WI;
					}

					// Calculate Wait Time used for T = 0 protocol
					t0WWTetu = (unsigned long)((unsigned long)(tempVariable1 * 960UL * factorDNumerator)/factorDdenominator);

					break;
	}

	// Convert CGT etu value in terms of microseconds
	cgtInMicroSeconds = (cgtETU * 1000000UL)/baudRate;
}	


/*******************************************************************************
  Function:
	BOOL SC_TransactT0(SC_APDU_COMMAND* apduCommand, SC_APDU_RESPONSE* apduResponse, BYTE* apduDataBuffer)	
  
  Description:
    This function Sends/recieves the ISO 7816-4 compaliant APDU commands to the card.

  Precondition:
    SC_DoPPS was success or SC_DoPPS functionality not called

  Parameters:
    SC_APDU_COMMAND* apduCommand	- Pointer to APDU Command Structure 
	SC_APDU_RESPONSE* pResp - Pointer to APDU Response structure
			BYTE* pResp - Pointer to the Command/Response Data buffer

  Return Values:
    TRUE if transaction was success, and followed the ISO 7816-4 protocol. 
	
  Remarks:
    In the APDU command structure, the LC field defines the number of data bytes to 
	be transmitted to the card. This array can hold max of 256 bytes, which 
	can be redefined by the user.  The LE field in APDU command defines the number
	of bytes expected to be received from the card.  This array can hold max 256 bytes,
	which can be redefined by the user.	
  *****************************************************************************/
BOOL SC_TransactT0(SC_APDU_COMMAND* apduCommand, SC_APDU_RESPONSE* apduResponse, BYTE* apduDataBuffer)
{
	unsigned short int leLength = 0,le = apduCommand->LE;
	BYTE* apduCommandBuffer;
	BYTE index,lc = apduCommand->LC,ins = apduCommand->INS;
	BYTE rx_char;
	BYTE lcLength = 0;

	// Return False if there is no Card inserted in the Slot
	if( !SCdrv_CardPresent() || gCardState != ATR_ON )
	{
		scLastError = SC_ERR_CARD_NOT_PRESENT;
		return FALSE;	
	}	

	// Clear APDU Response data if present before getting the new one's
	memset( apduResponse, 0, sizeof(SC_APDU_RESPONSE) );
	
	apduCommandBuffer = (BYTE*)apduCommand;
	

	//Send the Command Bytes: CLA INS P1 P2
	for( index = 0; index < 4; index++ )
	{
		SCdrv_SendTxData( apduCommandBuffer[index] );
		WaitMicroSec(cgtInMicroSeconds);
	}	
	
	//Now transmit LE or LC field if non zero
	if( lc )
		SCdrv_SendTxData( lc );
	else if( le )
		SCdrv_SendTxData( le );
	else
	{		
		SCdrv_SendTxData(0x00);
		// If LE is '0x00' then u expect 256 bytes.
		// if LE is absent, then there is no issue in writing le variable as 256
		le = 256;
	}
			
	while (1)
	{
    	// Get Procedure byte
		if(!SCdrv_GetRxData( &rx_char, t0WWTetu ) ) //wait for data byte from CARD
		{
			scLastError = SC_ERR_CARD_NO_RESPONSE;
			return FALSE;	//no response received
		}	

		// Process Procedure Byte
		if (rx_char == 0x60)
		{
			// Do Nothing
    	}
    	else if (((rx_char & 0xF0) == 0x60) || ((rx_char & 0xF0) == 0x90))
    	{
      		// SW1, get SW2
			apduResponse->SW1 = rx_char; //save SW1
			
			//now receive SW2
			if( SCdrv_GetRxData( &rx_char, t0WWTetu ) ) //wait for data byte from CARD
			{
				apduResponse->SW2 = rx_char;
            }	    
			else
			{
				scLastError = SC_ERR_CARD_NO_RESPONSE;
				return FALSE;	//no response received
			}

			break;
    	}
		else if(rx_char == ins)
		{
			// Send all remaining bytes
			if( lcLength < lc)		//transmit app data if any
			{
				WaitMicroSec(cgtInMicroSeconds);

				for(;lcLength < lc; lcLength++ )
				{	
					SCdrv_SendTxData( apduDataBuffer[lcLength] );
					WaitMicroSec(cgtInMicroSeconds);
				}
			}
			else
			{
				// Recive all remaining bytes
				for(;leLength < le; leLength++ )
				{	
					if( SCdrv_GetRxData( &rx_char, t0WWTetu ) ) //wait for data byte from CARD
						apduDataBuffer[leLength] = rx_char;	
					else
					{
						scLastError = SC_ERR_CARD_NO_RESPONSE;
						return FALSE;	//no response received
					}
				}		
			}
		}
		else if(rx_char == ~ins)
		{
        	// ACK, send one byte if remaining
    		if (lcLength < lc)
      		{
				WaitMicroSec(cgtInMicroSeconds);

				SCdrv_SendTxData( apduDataBuffer[lcLength++] );
      		}
      		else
      		{
				//wait for data byte from CARD or timeout
				if( SCdrv_GetRxData( &rx_char, t0WWTetu ) ) 
					apduDataBuffer[leLength++] = rx_char;	
				else
				{
					scLastError = SC_ERR_CARD_NO_RESPONSE;
					return FALSE;	//no response received
				}
			}
		}
		else
		{
			// Do Nothing
		}
	}

	// Store the number of recieved data bytes including the
	// status codes to make the life of Smart Card Reader easier
	apduResponse->RXDATALEN = leLength;

	return TRUE;
}

#ifdef SC_PROTO_T1

/*******************************************************************************
  Function:
    static unsigned short int SC_UpdateCRC(BYTE data,unsigned short int crc)
	
  Description:
    This function calculates 16 bit CRC for T=1 Protocol

  Precondition:
    Initial value of crc should be 0xFFFF.

  Parameters:
    BYTE data - Data that has to be used to update CRC.
	unsigned short int crc - current crc value

  Return Values:
    WORD - updated CRC
	
  Remarks:
    CRC 16 - X^16 + X^12 + X^5 + 1

  *****************************************************************************/
static unsigned short int SC_UpdateCRC(BYTE data,unsigned short int crc)
{
	BYTE index;
	unsigned short int tempData = (unsigned short int)data << 8;

	// Update the CRC & return it Back
	for (index = 0;index < 8;index++)
	{
		if ((crc ^ tempData) & 0x8000)
		{
			crc <<= 1;
			crc ^= (unsigned short int)0x1021; // X^12 + X^5 + 1
		}
		else
		{
			crc <<= 1;
		}
		
		tempData <<= 1;
	}
	
	return(crc);
}

/*******************************************************************************
  Function:
    static void SC_UpdateEDC(BYTE data,unsigned short int *edc)
	
  Description:
    This function updates Error Data Check value depending on the EDC type
    for T=1 Protocol

  Precondition:
    None.

  Parameters:
    BYTE data - Data that has to be used to update EDC.
	unsigned short int *edc - Pointer to EDC

  Return Values:
    None
	
  Remarks:
    None

*****************************************************************************/
static void SC_UpdateEDC(BYTE data,unsigned short int *edc)
{
	// Store the updated LRC/CRC in the EDC
	if (edcType == SC_CRC_TYPE_EDC)	// type = CRC
	{
		*edc = SC_UpdateCRC(data,*edc);
	}
	else // type = LRC
	{
		*edc = *edc ^ data;
	}
}

/*******************************************************************************
  Function:
    static void SC_SendT1Block(BYTE nad,BYTE pcb,BYTE length,BYTE *buffer)
	
  Description:
    This function transmits a T=1 formatted block

  Precondition:
    Complete ATR...

  Parameters:
    BYTE nad - NAD to be transmitted to the card
    BYTE pcb - PCB to be transmitted to the card
    BYTE length - Length of I-Field transmitted to the card
    BYTE *buffer - Pointer to data that is to be transmitted to the card

  Return Values:
    None
	
  Remarks:
    None

*****************************************************************************/
static void SC_SendT1Block(BYTE nad,BYTE pcb,BYTE length,BYTE *buffer)
{
	BYTE index;
	unsigned short int edc;

	// Choose the initial value of edc depending upon LRC or CRC
	if (edcType == SC_CRC_TYPE_EDC)
	{
		edc = 0xFFFF;
	}
	else
	{
		edc = 0;
	}
	// Update the edc for Node Address Data Byte
	SC_UpdateEDC(nad,&edc);

	// Update the edc for Protocol Control Byte
	SC_UpdateEDC(pcb,&edc);

	// Update the edc for length of tx Bytes
	SC_UpdateEDC(length,&edc);

	// Update the edc for the data to be transmitted
	for (index=0;index<length;index++)
	{
		SC_UpdateEDC(buffer[index],&edc);
	}

	// Transmit Node Address
	SCdrv_SendTxData(nad);

	// Transmit Protocol Control Byte	
	SCdrv_SendTxData(pcb);

	// Transmit length of Data Byte	
	SCdrv_SendTxData(length);

	// Transmit Data Bytes
	for (index=0;index<length;index++)
	{
		SCdrv_SendTxData(buffer[index]);
	}

	// Transmit EDC
	SCdrv_SendTxData((BYTE)edc);
	if (edcType == SC_CRC_TYPE_EDC)
	{
		SCdrv_SendTxData(edc>>8);
	}
}

/*******************************************************************************
  Function:
    void SC_ReceiveT1Block(void)
	
  Description:
    This function receives a T=1 formatted block

  Precondition:
    Transmit a block before expecting the response...

  Parameters:
    BYTE *rxNAD - Pointer to NAD recieved from the card
    BYTE *rxPCB - Pointer to PCB recieved from the card
    BYTE *rxLength - Pointer to Length of I-Field recieved from the card
    BYTE *buffer - Pointer to data recieved from the card
	unsigned long blockWaitTime - value of Block Wait Time

  Return Values:
    TRUE if block recieve is successful, and follows the ISO 7816-4 protocol. 
	
  Remarks:
    None
*****************************************************************************/
static BOOL SC_ReceiveT1Block(BYTE *rxNAD,BYTE *rxPCB,BYTE *rxLength,BYTE *buffer,unsigned long blockWaitTimeEtu)
{
	unsigned short int edc;
	WORD index;
	BYTE expectedLength;

  	// Get NAD
	if(!SCdrv_GetRxData( rxNAD, blockWaitTimeEtu ))
	{
		scLastError = SC_ERR_CARD_NO_RESPONSE;
		return FALSE;
	}

  	// Get PCB
	if(!SCdrv_GetRxData( rxPCB, t1CWTetu ))
	{
		scLastError = SC_ERR_CARD_NO_RESPONSE;
		return FALSE;
	}

  	// Get Length	
	if(!SCdrv_GetRxData( rxLength, t1CWTetu ))
	{
		scLastError = SC_ERR_CARD_NO_RESPONSE;
		return FALSE;
	}

	// Add one to the expected length for LRC
	expectedLength = *rxLength + 1;

	// Add additional byte to the length if using CRC
	if (edcType == SC_CRC_TYPE_EDC)
		expectedLength++;

	// Get all the data bytes plus EDC (1 or 2 bytes at end)
	for (index = 0;index < expectedLength;)
	{
		if(!SCdrv_GetRxData( buffer + index, t1CWTetu ))
		{
			scLastError = SC_ERR_CARD_NO_RESPONSE;
			return FALSE;
		}
		
		++index;
	}

	// Check for the LRC Error
	if (edcType == SC_LRC_TYPE_EDC)
	{
		edc = 0;
		edc = edc ^ *rxNAD;
		edc = edc ^ *rxPCB;
		edc = edc ^ *rxLength;

		for (index = 0;index < expectedLength;)
		{
			edc = edc ^ buffer[index];
			++index;
		}

		if (edc != 0)
		{
			scLastError = SC_ERR_RECEIVE_LRC;
			return FALSE;
		}
	}
	else // // Check for the CRC Error
	{
		edc = 0xFFFF;
		edc = SC_UpdateCRC(*rxNAD,edc);
		edc = SC_UpdateCRC(*rxPCB,edc);
		edc = SC_UpdateCRC(*rxLength,edc);

		for (index = 0;index < (expectedLength-2);)
		{
			edc = SC_UpdateCRC(buffer[index],edc);
			++index;
		}

		if (((edc >> 8) != buffer[expectedLength-2]) || ((edc & 0xFF) != buffer[expectedLength-1]))
	    {
			scLastError = SC_ERR_RECEIVE_CRC;
			return FALSE;
		}
	}
	
	// Return TRUE if there is no LRC or CRC error & data bytes are recieved sucessfully
	return TRUE;
}

/*******************************************************************************
  Function:
	BOOL SC_TransactT1(SC_T1_PROLOGUE_FIELD* pfield,BYTE* iField,SC_APDU_RESPONSE* apduResponse)
  
  Description:
    This function Sends/recieves the ISO 7816-4 compaliant T = 1 commands to the card.  

  Precondition:
    SC_DoPPS was success

  Parameters:
    SC_T1_PROLOGUE_FIELD* pfield - Pointer to Prologue Field 
	BYTE* iField - Pointer to the Information Field of Tx/Rx Data
	SC_APDU_RESPONSE* apduResponse - Pointer to APDU Response structure

  Return Values:
    TRUE if transaction was success, and followed the ISO 7816-4 protocol. 
	
  Remarks:    	
  *****************************************************************************/

BOOL SC_TransactT1(SC_T1_PROLOGUE_FIELD* pfield,BYTE* iField,SC_APDU_RESPONSE* apduResponse)
{
	BOOL	t1TransactCompleted = FALSE,txMbit = FALSE;
	BOOL	rxMbit = FALSE,rxSbit = FALSE,transmitNextSegment = TRUE;
	BYTE	txLength = 0,txPCB = pfield->PCB,rxNAD,rxPCB,rxLEN;
	BYTE	initialLength = pfield->LENGTH,iFieldLength,retryR = 0,retrySync = 0;
	WORD	rxLength = 0;
	BYTE*	rxField = iField;
	BYTE*	txField = iField;
	BYTE*	initialField = iField;
	unsigned long currT1BWTetu = t1BWTetu;
	T1BLOCK_TYPE t1TxBlockType,currentT1RxBlockType;

	iFieldLength = initialLength;

	// Initialize the recieved data length packet to zero
	apduResponse->RXDATALEN = 0;

	// Determine which type of block is to be transmitted to the card
	if((txPCB & 0x80) == 0x00)
	{
		// I-Block
		t1TxBlockType = I_BLOCK;

		if(txSbit)
		{
			txPCB = txPCB & S_BIT_CLR;
			txSbit = FALSE;
		}
		else
		{
			txPCB = txPCB | S_BIT_SET;
			txSbit = TRUE;
		}
	}
	else if((txPCB & 0xC0) == 0xC0)
	{
		// S-Block
		t1TxBlockType = S_BLOCK;
	}
	else if((txPCB & 0xC0) == 0x80)
	{
		// R-Block
		t1TxBlockType = R_BLOCK;
	}
	else
	{
		// INVALID BLOCK
		return FALSE;
	}

	// Go to appropriate case depending upon the type of block
	switch(t1TxBlockType)
	{
		case	I_BLOCK:
							// Continue Untill Transaction is Passed or Failed...
							while (!t1TransactCompleted)
							{
								// If Next segment has to be transmitted to the card
								if(transmitNextSegment)
								{
									txMbit = FALSE;

									// Set/Reset the M bit appropriately
									if(iFieldLength > maxSegmentLength)
									{
										txLength = maxSegmentLength;
										txMbit = TRUE;
										txPCB = txPCB | M_BIT_SET;
									}
									else
									{
										txLength = iFieldLength;
										txPCB = txPCB & M_BIT_CLR;
									}
								
									txField = iField;
								}

								// Send block with chaining mode, current sequence number, and maximum length.
								SC_SendT1Block(pfield->NAD,txPCB,txLength,txField);

								// Recieve the Block
								if(SC_ReceiveT1Block(&rxNAD,&rxPCB,&rxLEN,rxField,currT1BWTetu))
								{
									// Determine the type of Block recieved from the card
									if((rxPCB & 0x80) == 0x00)
									{
										// I-Block
										currentT1RxBlockType = I_BLOCK;
										
										if((rxPCB & 0x20) == 0x20)
											rxMbit = TRUE;
										else
											rxMbit = FALSE;

										if((rxPCB & 0x40) == 0x40)
											rxSbit = TRUE;
										else
											rxSbit = FALSE;

										transmitNextSegment = FALSE;
										
										retryR = 0;retrySync = 0;
									}
									else if((rxPCB & 0xC0) == 0xC0)
									{
										// S-Block
										currentT1RxBlockType = S_BLOCK;
										
										retryR = 0;retrySync = 0;
									}
									else if((rxPCB & 0xC0) == 0x80)
									{
										// R-Block
										currentT1RxBlockType = R_BLOCK;
										
										retryR = 0;retrySync = 0;
									}
									else
									{
										// INVALID BLOCK
										currentT1RxBlockType = INVALID_BLOCK;
									}
								}
								else
								{
									// No Block Recieved or Error Block Recieved
									currentT1RxBlockType = INVALID_BLOCK;
								}
						
								currT1BWTetu = t1BWTetu;
								
								switch(currentT1RxBlockType)
								{
									case	I_BLOCK	:
														rxField = rxField + (BYTE)rxLEN;
														rxLength = rxLength + rxLEN;
														iFieldLength = 0;

														// If More Bit is set by the card,
														// send the apprpriate R Block
														if(rxMbit)
														{
															// Transmit R(N) - Expected Seq
															txLength = 0x00;
															
															if(rxSbit)
																txPCB = 0x80;
															else
																txPCB = 0x90;
														}
														else
														{
															// No More Bit set from the card,
															// Data is recieved with the status
															// codes...we are done
															if(rxLEN)
															{
																// We are Done here
																t1TransactCompleted = TRUE;
																if(rxLength >= 2)
																{
																	apduResponse->RXDATALEN = rxLength - 2;
																	apduResponse->SW1 = *(initialField + rxLength - (BYTE)2);
																	apduResponse->SW2 = *(initialField + rxLength - (BYTE)1);
																}
															}
															else
															{
																// Transmit Forced Acknowledge I Block
																txLength = 0x00;
															
																if(txSbit)
																{
																	txPCB = 0x00;
																	txSbit = FALSE;
																}
																else
																{
																	txPCB = 0x40;
																	txSbit = TRUE;
																}
															}
														}

														break;

									case	S_BLOCK	:
														// Card can only send Resync Response...
														// Card cant do Resync request
														if((rxPCB & 0x3F) == 0x20) // Resync Response from the card
														{
															txSbit = FALSE;
															return FALSE;
														}
														else if((rxPCB & 0x3F) == 0x01) // Request IFS Change
														{
															txPCB = SC_IFS_RESPONSE;
															txLength = 1;
															txField = rxField;
															maxSegmentLength = *rxField;
															transmitNextSegment = FALSE;
															continue;
														}
														else if((rxPCB & 0x3F) == 0x03) // Request Wait time Extension
														{
															currT1BWTetu = t1BWTetu * *rxField;
															txPCB = SC_WAIT_TIME_EXT_RESPONSE;
															txLength = 1;
															txField = rxField;
															transmitNextSegment = FALSE;
															continue;
														}
														else if((rxPCB & 0x3F) == 0x24) // VPP Error Response
														{
															scLastError = SC_CARD_VPP_ERR;
															return FALSE;
														}
														else if((rxPCB & 0x3F) == 0x02) // Abort Request
														{
															txPCB = SC_ABORT_RESPONSE;
															txLength = 0;
															if(txMbit)
															{
																// Do this so that there is last byte transmission to terminate
																// the communication
																iFieldLength = maxSegmentLength + 1;
															}
															transmitNextSegment = FALSE;
															continue;
														}
														break;

									case	R_BLOCK	:
														// If Recieved Seq Number not equal
														// to transmitted Seq Number
														if(rxSbit != txSbit)
														{
															// If More Bit is set by the reader
															if(txMbit)
															{
																// Transmission of previous segment was
																// succesful. Transmit next segment.
																transmitNextSegment = TRUE;

																iFieldLength = iFieldLength - maxSegmentLength;
																iField = iField + maxSegmentLength;

																// Toggle the Sequence Bit
																if(txSbit)
																{
																	txPCB = 0x00;
																	txSbit = FALSE;
																}
																else
																{
																	txPCB = 0x40;
																	txSbit = TRUE;
																}
															}
															else
															{
																// There was some error, trasmit previous
																// block
																transmitNextSegment = FALSE;
															}
														}
														else
														{
															// Retransmit the I-Block
															transmitNextSegment = TRUE;
														}
														
														break;

									case	INVALID_BLOCK	:
																// If 1st Block transaction itself 
																// is failing transmit R(0)
																if(initialLength == iFieldLength)
																{
																	txPCB = 0x82;
																	txLength = 0x00;
																	transmitNextSegment = FALSE;
																	retryR++;
																	retrySync = 0;

																	// Try transmitting R(0) twice
																	// before telling error to the
																	// Smart Card Reader
																	if(retryR > 2)
																	{
																		scLastError = SC_ERR_CARD_NO_RESPONSE;
																		return FALSE;
																	}
																}
																else
																{
																	transmitNextSegment = FALSE;
																	
																	// Try transmitting R(0) twice																															// Try transmitting R(0) twice
																	// before transmitting ReSync
																	// Request to the card
																	if(retryR < 2)
																	{
																		if(rxMbit)
																			txPCB = 0x82;
																		else
																			txPCB = 0x92;
																	
																		txLength = 0x00;
																		retryR++;
																		retrySync = 0;
																	}
																	else
																	{
																		txPCB = 0xC0;
																		txLength = 0x00;
																		retrySync++;
																																		// Try transmitting R(0) twice																															// Try transmitting R(0) twice
																		// Try transmitting Resync Request
																		// thrice before telling error to the
																		// Smart Card Reader
																		if(retrySync > 3)
																		{
																			scLastError = SC_ERR_CARD_NO_RESPONSE;
																			return FALSE;
																		}
																	}
																}
																
																continue;
																
																break;
								}
							}	
							
							break;

		case	S_BLOCK:
							// Continue Untill Transaction is Passed or Failed...
							while (!t1TransactCompleted)
							{
								// Send mode, current sequence number, and maximum length.
								SC_SendT1Block(pfield->NAD,txPCB,0,txField);

								// Recieve the Block	
								if(SC_ReceiveT1Block(&rxNAD,&rxPCB,&rxLEN,rxField,currT1BWTetu))
								{
									// Determine the type of Block recieved from the card
									if((rxPCB & 0x80) == 0x00)
									{
										// I-Block
										currentT1RxBlockType = I_BLOCK;
									}
									else if((rxPCB & 0xC0) == 0xC0)
									{
										// S-Block
										currentT1RxBlockType = S_BLOCK;
									}
									else if((rxPCB & 0xC0) == 0x80)
									{
										// R-Block
										currentT1RxBlockType = R_BLOCK;
									}
									else
									{
										// INVALID BLOCK
										currentT1RxBlockType = INVALID_BLOCK;
									}
								}
								else
								{
									// No Block Recieved or Error Block Recieved
									currentT1RxBlockType = INVALID_BLOCK;
								}
								
								switch(currentT1RxBlockType)
								{
									case	S_BLOCK	:
														// If Acknowledged properly, return 
														// TRUE to the card reader
														if((txPCB | 0x20) == rxPCB)
														{
															t1TransactCompleted = TRUE;
															break;
														}
														else
														{
															// Try transmitting thrice before
															// telling error to the Smart
															// Card Reader
															retrySync++;
															
															if(retrySync > 3)
															{
																scLastError = SC_ERR_CARD_NO_RESPONSE;
																return FALSE;
															}

															continue;
														}
														break;
									case	R_BLOCK	:
									case	I_BLOCK	:
									case	INVALID_BLOCK	:
															// Try transmitting thrice before
															// telling error to the Smart
															// Card Reader
															retrySync++;
															if(retrySync > 3)
															{
																scLastError = SC_ERR_CARD_NO_RESPONSE;
																return FALSE;
															}
															continue;
															break;
								}
							}
							break;
		
		case	R_BLOCK:
		default:
						break;
	}

	// Return TRUE if everything is fine
	return TRUE;
}

#endif

