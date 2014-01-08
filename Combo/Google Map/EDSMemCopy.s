;/*****************************************************************************
; *
; * Memory Copy routines from/to EDS data memory and ordinary PIC RAM (addresses
; *   < 32768)(ex: on PIC24FJ256DA210)
; *
; *****************************************************************************
; * FileName:		EDSMemCopy.s
; * Dependencies:	None
; * Processor:		PIC24F, PIC24H, or dsPIC33F
; * Compiler:		MPLAB ASM30 2.02 or later
; * Linker:			MPLAB LINK30 2.02 or later
; * Company:		Microchip Technology Incorporated
; *
; * Software License Agreement
; *
; * The software supplied herewith by Microchip Technology Incorporated
; * (the “Company”) for its PICmicro® Microcontroller is intended and
; * supplied to you, the Company’s customer, for use solely and
; * exclusively on Microchip PICmicro Microcontroller products. The
; * software is owned by the Company and/or its supplier, and is
; * protected under applicable copyright laws. All rights are reserved.
; * Any use in violation of the foregoing restrictions may subject the
; * user to criminal sanctions under applicable laws, as well as to
; * civil liability for the breach of the terms and conditions of this
; * license.
; *
; * Microchip Technology Inc. (“Microchip”) licenses this software to 
; * you solely for use with Microchip products.  The software is owned 
; * by Microchip and is protected under applicable copyright laws.  
; * All rights reserved.
; *
; * You may not export or re-export Software, technical data, direct 
; * products thereof or any other items which would violate any applicable
; * export control laws and regulations including, but not limited to, 
; * those of the United States or United Kingdom.  You agree that it is
; * your responsibility to obtain copies of and to familiarize yourself
; * fully with these laws and regulations to avoid violation.
; *
; * SOFTWARE IS PROVIDED “AS IS.”  MICROCHIP EXPRESSLY DISCLAIM ANY 
; * WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT 
; * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
; * PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
; * BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES,
; * LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF PROCUREMENT
; * OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS BY THIRD PARTIES
; * (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), ANY CLAIMS FOR 
; * INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS. 
; *
; * Author				Date        Comment
; *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; * Howard Schlunder	04/21/10	Original release
; *****************************************************************************/
.equ VALID_ID,0
    .ifdecl __dsPIC33F
	    .include "p33fxxxx.inc"
    .endif

    .ifdecl __dsPIC30F
	    .include "p30fxxxx.inc"
    .endif

    .ifdecl __PIC24H
	    .include "p24hxxxx.inc"
    .endif

    .ifdecl __PIC24F
	    .include "p24fxxxx.inc"
    .endif

    .ifdecl __PIC24FK
	    .include "p24fxxxx.inc"
    .endif

.if VALID_ID <> 1
	.error "Processor ID not specified in generic include files.  New ASM30 assembler needs to be downloaded?"
.endif

.text
;***************************************************************************
; Function: void memcpyeds2ram(void *destPointer, unsigned long sourceAddress, unsigned short length)
;
; Input: *destPointer: Pointer to PIC RAM location to write data to.  This pointer need not be aligned to any particular memory boundaries.
;        sourceAddress: EDS address to read data from.  Note that this is a linear 32-bit address, not a __eds__ * pointer.  This pointer need not be aligned to any particular memory boundaries.
;		 length: Number of bytes to copy from the source address (sourceAddress) to the destination pointer (*destPointer).
;
; Output: None
;
; Side Effects: None
;
; Overview: Copies bytes. W0 = destPointer, W1 = length, W2:W3 = sourceAddress
;***************************************************************************
	.global _memcpyeds2ram
_memcpyeds2ram:						
	push	DSRPAG
	sl		W3, #1, W3
	mov		W3, DSRPAG				; Load DSWPAG with correct destination address page
	btsc	W2, #15
	bset	DSRPAG, #0
	cp0		W1						; Test length parameter for zeroness
	bra		TestLengthEDS2RAM

CopyEDSByteToRAM:
	bset	W2, #15					; Set MSbit of sourceAddress DSR offset pointer so it can be used to access EDS space.  This is done every time because the increment opperation can cause us to enter a different page, making the pointer roll over to 0x0000
	mov.b	[W2++], [W0++]			; Copy 1 byte and increment source and dest pointers
	cp0		W2						; Check to see if we crossed an EDS page boundary, if so increment the DSWPAG register
	btsc	SR, #Z
	inc		DSRPAG
	dec		W1, W1					; Decrement length parameter

TestLengthEDS2RAM:
	bra		NZ, CopyEDSByteToRAM	; Return when we reach zero length
	pop		DSRPAG
	return



;***************************************************************************
; Function: void memcpyram2eds(unsigned long destAddress, void *sourcePointer, unsigned short length)
;
; Input: destAddress: EDS address to write data to.  Note that this is a linear 32-bit address, not a __eds__ * pointer.  This pointer need not be aligned to any particular memory boundaries.
;        sourcePointer: RAM pointer to read data from. This pointer need not be aligned to any particular memory boundaries.
;		 length: Number of bytes to copy from the source pointer (*sourcePointer) to the destination address (destAddress).
;
; Output: None
;
; Side Effects: None
;
; Overview: Copies bytes.  W0:W1 = destAddress, W2 = sourcePointer, W3 = length
;***************************************************************************
	.global _memcpyram2eds
_memcpyram2eds:						
	push	DSWPAG
	sl		W1, #1, W1
	mov		W1, DSWPAG				; Load DSWPAG with correct destination address page
	btsc	W0, #15
	bset	DSWPAG, #0
	cp0		W3						; Test length parameter for zeroness
	bra		TestLengthRAM2EDS

CopyRAMByteToEDS:
	bset	W0, #15					; Set MSbit of sourceAddress DSW offset pointer so it can be used to access EDS space.  This is done every time because the increment opperation can cause us to enter a different page, making the pointer roll over to 0x0000
	mov.b	[W2++], [W0++]			; Copy 1 byte and increment source and dest pointers
	cp0		W0						; Check to see if we crossed an EDS page boundary, if so increment the DSWPAG register
	btsc	SR, #Z
	inc		DSWPAG
	dec		W3, W3					; Decrement length parameter

TestLengthRAM2EDS:
	bra		NZ, CopyRAMByteToEDS	; Return when we reach zero length
	pop		DSWPAG
	return
