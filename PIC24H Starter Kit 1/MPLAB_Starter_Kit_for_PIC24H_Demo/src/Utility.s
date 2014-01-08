/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		Utility.s
* Dependencies:    		Header (.inc) files if applicable, see below
* Processor:       		PIC24FJ128GP504
* Compiler:        		MPLAB® C30 v3.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
************************************************************************/

;**************************************************************************
; ReadProgramMemory (long programMemoryAddress,int * targetMemory,int size)												
;**************************************************************************
; Function to read from program memory
.global	_ReadProgramMemory
	;W1 - Contains bits 23-16 of program address 
	;W0 - Contains bits 15-0  of program address 
	;W2 - Contains the destination data address
	;W3 - Contains the number of bytes to transfer
.text
_ReadProgramMemory:
	push 		TBLPAG
once_again:
	mov			W1,TBLPAG			; The upper 8 bits of program address
	tblrdl		[W0],W4				; The lower 16 bits are already in W0
	mov		    W4,[W2++]			; Store the lower byte last.
	tblrdh		[W0++],W4			; Read the higher byte
	mov			W4,[W2++]			; move the higher word
	cp0			W0					; Check if a page boundary has been crossed
	bra			nz, same_page		; No then proceed as normal	
	inc			W1,W1				; else increment the psv pag
same_page:
	dec2		W3,W3				; decrement count by1 
	bra			nz, once_again		; continue till count is zero
	pop			TBLPAG
	nop
	nop
	nop
	return

;**************************************************************************
; void PackForG711(int * source, char* destination,int sizeOfSource)												
;**************************************************************************
; Function to pack data G.711 format
.global _PackForG711
.text
;	W0 - Source Address
;	W1 - Destination Address
;	W2 - Source Count
;	W3 - Working register
_PackForG711:	
read_more:
	mov.b	[W0],W3			;The first byte
	mov.b	W3,[W1++]		; 
	mov		[W0++],W3		; The second byte
	lsr		W3,#8,W3		; then make the upper byte lower
	mov.b	W3,[W1++]		; Store the second byte
	mov		[W0++],W3		;The third byte
	mov.b	W3,[W1++]		; 	
	dec2	W2,W2			; decrement count by1 
	bra		nz, read_more	; continue till count is zero
	return	
	
;**************************************************************************
; int VectorMax (int numElems, int* srcV, int* maxIndex)												
;**************************************************************************
; Function to get vector element with maximum value.
;
; Operation:
;	maxVal = max {srcV[n], n in {0, 1,...numElems-1} }
;   residually,
;	if srcV[i] = srcV[j] = maxVal, and i < j, then *(maxIndex) = j
;
; Input:
;	w0 = number elements in vector(s) (numElems)
;	w1 = ptr to source vector (srcV)
;	w2 = ptr to index of maximum value (&maxIndex)
; Return:
;	w0 = maximum value (maxVal)
;
; System resources usage:
;	{w0..w5}	used, not restored
;
; DO and REPEAT instruction usage.
;	no DO intructions
;	no REPEAT intructions
;
; Program words (24-bit instructions):
;	13
;
; Cycles (including C-function call and return overheads):
;	14			(if numElems == 1)
;	20 + 8*(numElems-2)	(if srcV[n] <= srcV[n+1], 0 <= n < numElems-1)
;	19 + 7*(numElems-2)	(if srcV[n] >  srcV[n+1], 0 <= n < numElems-1)
;............................................................................
.global	_VectorMax	; export
_VectorMax:
	mov	[w1++],w5			; w5 = srcV[0] (current maxVal)
	clr	[w2]				; *w2 = current max value index
	dec	w0,w4				; w4 = numElems-1
	bra	le,_endMax			; no more elements...
_compare:
	mov	[w1++],w3			; w3 = srcV[n+1]
	cp	w5,w3				; srcV[n] < srcV[n+1] ?
	bra	gt,_noUpdate			; yes => no update
_doUpdate:					; no  => do update
	mov	w3,w5				; w5 = (current maxVal)
	sub	w0,w4,[w2]			; *w2 = current max value index
_noUpdate:
	dec	w4,w4				; w4 = numElems-n
	bra	gt,_compare			; no more elements...
_endMax:
	mov	w5,w0				; restore return value
	return	

;**************************************************************************
; int VectorMax (int numElems, int* srcV, int* maxIndex)												
;**************************************************************************
; Delay function
.set Fcy,        40000000
.set US_K,       Fcy/1000000
.set MS_K,       Fcy/10000
.global _Delay
.global _Delay_Us
;===============================================
; ms Delay Function
;===============================================
_Delay:
ms_oloop:
    mov #MS_K,w1
ms_iloop:
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    dec     w1, w1
    bra     nz, ms_iloop    
    dec     w0,w0
    bra     nz,ms_oloop
    return
;===============================================
; us Delay Function
;===============================================
_Delay_Us:
us_oloop:   
    .rept (US_K-3)
    nop
    .endr   
    dec     w0,w0
    bra     nz,us_oloop
    return

.end

	
	

	
	