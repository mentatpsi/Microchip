/********************************************************************
 FileName:		SClib.h
 Dependencies:	See INCLUDES section
 Processor:		PIC18, PIC24 & PIC32 Microcontrollers
 Hardware:		This demo is natively intended to be used on Exp 16, LPC
 				& HPC Exp board. This demo can be modified for use on other hardware
 				platforms.
 Complier:  	Microchip C18 (for PIC18), C30 (for PIC24) & C30 (for PIC32)
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
  1.02.2   Modifed PPS functionality API.
           Modified the code in more structured way.
********************************************************************/

// Smart Card Library
#ifndef __SC_MCP_LIB__
#define __SC_MCP_LIB__

#include "GenericTypeDefs.h"

// Smart Card error types
typedef enum
{
	SC_ERR_NONE,	// No Error
	SC_ERR_CARD_NOT_SUPPORTED,	// Card Not Supported
	SC_ERR_BAR_OR_NO_ATR_RESPONSE,	// No ATR Response from the card
	SC_ERR_CARD_NOT_PRESENT,	// Card Not present in the slot
	SC_ERR_CARD_NO_RESPONSE,	// No response from the card
	SC_ERR_RECEIVE_LRC,	// LRC Error in the block recieved from the card
	SC_ERR_RECEIVE_CRC,	// CRC Error in the block recieved from the card
	SC_CARD_VPP_ERR,	// VPP Error recieved from the card
	SC_ERR_ATR_DATA,		// ERROR in ATR data recieved from the card
	SC_ERR_RSV1	// Smart Card Error 1 (Reserved) - can be used based upon the Application
} SC_ERROR;

// Block types in T=1 protocol
typedef enum
{
	I_BLOCK,	// I Block
	R_BLOCK,	// R Block
	S_BLOCK,	// S Block
	INVALID_BLOCK	// INVALID BLOCK
}T1BLOCK_TYPE;

// Returns '1' if T=1 protocol is supported & Returns 0 otherwise
#define		SC_T1ProtocolType()		(((scTD1 & 0x0F) == 0x01)?TRUE:FALSE)

// Returns '1' if T=0 protocol is supported & Returns 0 otherwise
#define		SC_T0ProtocolType()		(((scTD1 & 0x0F) == 0x00)?TRUE:FALSE)

// Returns '1' if TA1 present & Returns 0 otherwise
#define		SC_TA1Present()			((scCardATR[1] & 0x10)?TRUE:FALSE)

// Returns '1' if TB1 present & Returns 0 otherwise
#define		SC_TB1Present()			((scCardATR[1] & 0x20)?TRUE:FALSE)

// Returns '1' if TC1 present & Returns 0 otherwise
#define		SC_TC1Present()			((scCardATR[1] & 0x40)?TRUE:FALSE)

// Returns '1' if TD1 present & Returns 0 otherwise
#define		SC_TD1Present()			((scCardATR[1] & 0x80)?TRUE:FALSE)

// Returns '1' if TA2 present & Returns 0 otherwise
#define		SC_TA2Present()			((scTD1 & 0x10)?TRUE:FALSE)

// Returns '1' if TB2 present & Returns 0 otherwise
#define		SC_TB2Present()			((scTD1 & 0x20)?TRUE:FALSE)

// Returns '1' if TC2 present & Returns 0 otherwise
#define		SC_TC2Present()			((scTD1 & 0x40)?TRUE:FALSE)

// Returns '1' if TD2 present & Returns 0 otherwise
#define		SC_TD2Present()			((scTD1 & 0x80)?TRUE:FALSE)

// Smart Card Commands

// Start Session Command code to the Smart Card
#define SC_START_SESSION	0x84

// Authenticate Command code to the Smart Card
#define SC_AUTHENTICATE		0x82

// Get Response Command code to the Smart Card
#define SC_GET_RESPONSE		0xC0

// Submit Code Command to the Smart Card
#define SC_SUBMIT_CODE		0x20

// Clear Card Command code to the Smart Card
#define SC_CLEAR_CARD		0x30

// Select File Command code to the Smart Card
#define SC_SELECT_FILE		0xA4

// Read Record Command code to the Smart Card
#define SC_READ_RECORD		0xB2

// Write Record Command code to the Smart Card
#define SC_WRITE_RECORD		0xD2

// Credit Command code to the Smart Card
#define SC_CREDIT			0xE2

// Debit Command code to the Smart Card
#define SC_DEBIT			0xE6

// Revoke Command code to the Smart Card
#define SC_REVOKE			0xE8

// Inquire Account Command code to the Smart Card
#define SC_INQUIRE_ACCT		0xE4

// Change Pin Command code to the Smart Card
#define SC_CHANGE_PIN		0x24

// ATR data sent by smartcard.
extern BYTE scCardATR[];

// length of ATR data sent by smart card
extern BYTE scATRLength;

// Smart Card Error type is stored in this variable
extern SC_ERROR scLastError;

// TA1 determines the clock-rate conversion factor F & bit-rate-adjustment factor D
extern BYTE scTA1;

// TA2 determines whether the smart card will operate in specific mode or negotiable mode
// following the ATR
extern BYTE scTA2;

// TA3 conveys the Information Field Size Integer (IFSI) for the smart card.
extern BYTE scTA3;

// TB1 conveys information on the smart card's programming voltage requirements.
extern BYTE scTB1;

// TB2 conveys PI2, which determines the value of programming voltage required by
// the smart card. The value of PI1 in TB1 is superceded when TB2 is present
extern BYTE scTB2;

// TB3 indicates the value of the Character Waiting Time Integer (CWI) and 
// Block Waiting Time Integer (BWI) used to compute the Character Waiting Time (CWT)
// and Block Waiting Time (BWT).
extern BYTE scTB3;

// TC1 determines the extra guard time to be added between consecutive characters
// sent to the smart card from the terminal.
extern BYTE scTC1;

// TC2 is specific to protocol type T=0. TC2 conveys work waiting-time integer (WI)
// that determines the maximum interval between the leading edge of the start bit
// of any character sent by the smart card and the leading edge of the start bit
// of the previous character sent either by the card or the reader
extern BYTE scTC2;

// When TC3 is present, it indicates the type of block-error detection to be used.
// When TC3 is not present, the default longitudinal redundancy check (LRC) is used.
extern BYTE scTC3;

// TD1 indicates if any further interface bytes are to be transmitted, and if so,
// which protocol will be used.
extern BYTE scTD1;

// The TD2 character has the same function as the TD1 character.
extern BYTE scTD2;

// TD3 indicates interface bytes similar to that of TD1 & TD2
extern BYTE scTD3;

// Historical bytes sent by Smart Card
extern BYTE* scATR_HistoryBuffer;

// Number of Historical bytes present
extern BYTE  scATR_HistoryLength;

// Length of PPS Response
extern BYTE scPPSresponseLength;

// PPS Response Bytes
extern BYTE scPPSresponse[7];

// SmartCard States
#define	SC_STATE_CARD_NOT_PRESENT	10	// No Card Detected
#define	SC_STATE_CARD_ACTIVE		20	// Card is powered and ATR received
#define	SC_STATE_CARD_INACTIVE		30	// Card present but not powered

// Longitudnal Redundancy Check(LRC) type is used for EDC in Epilogue Field
#define	SC_LRC_TYPE_EDC				(BYTE)0

// Cyclic Redundancy Check(CRC) type is used for EDC in Epilogue Field
#define	SC_CRC_TYPE_EDC				(BYTE)1

// PCB byte for Resync Request of T1 Protocol
#define	SC_RESYNC_REQ				(BYTE)0xC0

// PCB byte for Abort Response of T1 Protocol
#define	SC_ABORT_RESPONSE			(BYTE)0xE2

// PCB byte for IFS Response of T1 Protocol
#define	SC_IFS_RESPONSE				(BYTE)0xE1

// PCB byte for Wait Time Extension Response of T1 Protocol
#define	SC_WAIT_TIME_EXT_RESPONSE	(BYTE)0xE3

// DEFAULT Value of WI Indicator used in calculation of WWT for T=0 protocol
#define	SC_WI						(BYTE)0x0A

// DEFAULT Value of CWI Indicator used in calculation of CWT for T=1 protocol
#define	SC_CWI						(BYTE)13

// DEFAULT Value of BWI Indicator used in calculation of BWT for T=1 protocol
#define	SC_BWI						(BYTE)0x04

////////// SmartCard APDU Command 7816-4
typedef struct
{
  BYTE CLA;  			// Command class
  BYTE INS;  			// Operation code 
  BYTE P1;   			// Selection Mode 
  BYTE P2;   			// Selection Option 
  BYTE LC;				// Data length
  BYTE LE;         		// Expected length of data to be returned
} SC_APDU_COMMAND;

/////////// SmartCard APDU Response structure 7816-4
typedef struct
{
  WORD RXDATALEN;		// Recieved Data length from smart card(excluding SW1 and SW2 bytes)
  BYTE SW1;          	// Status byte 1
  BYTE SW2;          	// Status byte 2
} SC_APDU_RESPONSE;

//// Prologue Field for T=1 Protocol
typedef struct
{
  BYTE NAD;				// Node Address
  BYTE PCB;				// Protocol Control Byte
  BYTE LENGTH;			// LENGTH of I-Field
}SC_T1_PROLOGUE_FIELD;	


// Smart Card Lib API //////////////////

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
extern void SC_Initialize(void);


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
extern BOOL SC_CardPresent(void);


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
extern int SC_GetCardState(void);

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
extern BOOL SC_PowerOnATR(void);

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
extern BOOL SC_DoPPS(BYTE *ppsPtr);

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
extern BOOL SC_TransactT0(SC_APDU_COMMAND* apduCommand, SC_APDU_RESPONSE* apduResponse, BYTE* apduDataBuffer);

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
extern void SC_Shutdown(void);

#ifdef SC_PROTO_T1

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

extern BOOL SC_TransactT1(SC_T1_PROLOGUE_FIELD* pfield,BYTE* iField,SC_APDU_RESPONSE* apduResponse);

#endif

#endif



