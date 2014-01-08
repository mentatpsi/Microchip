/********************************************************************
 FileName:      RTCC.h
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

#ifndef __RTCC_H
#define __RTCC_H

#include "GenericTypeDefs.h"
#include "Compiler.h"

#define	RTCC60HZ	3
#define RTCC50HZ	2
#define RTCCINTOSC	1
#define RTCCSOSC	0

typedef struct{
	BYTE Year;
	BYTE Month;
	BYTE Day;
	BYTE DOW;
	BYTE Hour;
	BYTE Minute;
	BYTE Second;
} DTVALUE;	

BOOL RTCC_Init ( DTVALUE* pInitialDTV, BYTE RTCC_Source );
BOOL Set_RTCC_Value ( DTVALUE* pDesiredDTV );
BOOL Get_RTCC_Value ( DTVALUE* pCurrentDTV );

#endif
