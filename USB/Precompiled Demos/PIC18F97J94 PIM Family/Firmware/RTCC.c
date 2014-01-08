/********************************************************************
 FileName:      RTCC.c
 Dependencies:  See INCLUDES section
 Processor:     PIC18F97J94 and PIC18F87J94
 Hardware:      This demo is natively intended to be used on Microchip USB demo
                boards supported by the MCHPFSUSB stack.  See release notes for
                support matrix.  This demo can be modified for use on other 
                hardware platforms.
 Complier:      Microchip C18 (for PIC18)
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC® Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
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
  1.1   Adding PIC18F87J94 part support
********************************************************************/

#include "RTCC.h"

BOOL RTCC_Init ( DTVALUE* pInitialDTV, BYTE RTCC_Source )
{	
	EECON2 = 0x55;
	EECON2 = 0xAA;
	RTCCON1bits.RTCWREN = 1;
	
	// Ensure initial state of RTCC is off
	RTCCON1 = 0x20;
	RTCCON2 = 0x00;
	ALRMCFG = 0x00;
	
	RTCCON1bits.RTCPTR = 0x3;
	
	RTCVALL = pInitialDTV->Year;
	RTCVALH = 0x00;
	RTCVALL = pInitialDTV->Day;
	RTCVALH = pInitialDTV->Month;
	RTCVALL = pInitialDTV->Hour;
	RTCVALH = pInitialDTV->DOW;
	RTCVALL = pInitialDTV->Second;
	RTCVALH = pInitialDTV->Minute;
	
	//RTCCAL = 0x7F;	// Attempt to compensate for PIM hardware segments in close
					// proximity to SOSCO and SOSCI pins.
	
	RTCCAL = 0x00; // With better SOSC layout nominal 0 trim may be used.
	 
	RTCCON2bits.RTCCLKSEL = RTCC_Source;
	RTCCON1bits.RTCEN = 1;
	RTCCON1bits.RTCWREN = 0;
	
	//Could send back error state
	return TRUE;
}	
BOOL Set_RTCC_Value ( DTVALUE* pDesiredDTV )
{
	EECON2 = 0x55;
	EECON2 = 0xAA;
	RTCCON1bits.RTCWREN = 1;
	
	// Ensure initial state of RTCC is off
	RTCCON1 = 0x20;
	RTCCON2 = 0x00;
	ALRMCFG = 0x00;
	
	RTCCON1bits.RTCPTR = 0x3;
	
	while(RTCCON1bits.RTCSYNC);
	RTCVALL = pDesiredDTV->Year;
	RTCVALH = 0x00;
	RTCVALL = pDesiredDTV->Day;
	RTCVALH = pDesiredDTV->Month;
	RTCVALL = pDesiredDTV->Hour;
	RTCVALH = pDesiredDTV->DOW;
	RTCVALL = pDesiredDTV->Second;
	RTCVALH = pDesiredDTV->Minute;
	
	RTCCON1bits.RTCEN = 1;
	RTCCON1bits.RTCWREN = 0;
	
	//Could send back error state
	return TRUE;
}
BOOL Get_RTCC_Value ( DTVALUE* pCurrentDTV )
{
	BYTE Dummy;
	
	RTCCON1bits.RTCPTR = 0x3;
	
	while(RTCCON1bits.RTCSYNC);
	
	pCurrentDTV->Year = RTCVALL;
	Dummy = RTCVALH;
	pCurrentDTV->Day = RTCVALL;
	pCurrentDTV->Month = RTCVALH;
	pCurrentDTV->Hour = RTCVALL;
	pCurrentDTV->DOW = RTCVALH;
	pCurrentDTV->Second = RTCVALL;
	pCurrentDTV->Minute = RTCVALH;
	
	//Could send back error state
	return TRUE;
}	
