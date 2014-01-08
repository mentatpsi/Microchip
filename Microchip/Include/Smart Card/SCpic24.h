/********************************************************************
 FileName:		SCpic24.h
 Dependencies:	See INCLUDES section
 Processor:		PIC24 Microcontrollers
 Hardware:		This demo is natively intended to be used on Exp 16 board.
 				This demo can be modified for use on other hardware platforms.
 Complier:  	Microchip C30 (for PIC24)
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
  1.01     Cleaned up unnecessary variables
  1.02     Modified to Support PIC24H microcontrollers
  1.02.2   Modified to support WaitMicroSec & WaitMilliSec functions.
           The way the UART baud rate is set, is changed( function : SCdrv_SetBRG()).
           Removed extern declarations of "factorD" & "factorF" variables.
********************************************************************/

#if defined(__PIC24F__) || defined(__PIC24H__)

#ifndef __SC_DRV24_LIB__
#define __SC_DRV24_LIB__

#include "GenericTypeDefs.h"

#if defined(__PIC24F__)
	#include "p24Fxxxx.h"
#elif	defined(__PIC24H__)
	#include "p24Hxxxx.h"
#endif

extern unsigned long baudRate;	// Baud Rate of Smart Card Transmission/Reception Data
extern unsigned long scReferenceClock; // Smart Card Reference Clock


extern void SCdrv_InitUART(void);
extern void SCdrv_CloseUART(void);
extern void SCdrv_SetBRG(void);
extern BOOL SCdrv_GetRxData( BYTE* pDat, unsigned short int timeOutETUs);
extern void SCdrv_SendTxData( BYTE data );

extern void WaitMicroSec(unsigned long int microSec);
extern void WaitMilliSec(unsigned long int ms);

#endif

#endif

