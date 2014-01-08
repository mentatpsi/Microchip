;-------------------------------------------------------------------------------
; FileName:		PIC18InterruptVectors.asm
; Dependencies:	See INCLUDES section
; Processor:	PIC18 USB Microcontrollers
; Complier:  	Microchip C18 (for PIC18)
; Company:		Microchip Technology, Inc.
;
; Software License Agreement:
;
; The software supplied herewith by Microchip Technology Incorporated
; (the "Company") for its PIC® Microcontroller is intended and
; supplied to you, the Company's customer, for use solely and
; exclusively on Microchip PIC Microcontroller products. The
; software is owned by the Company and/or its supplier, and is
; protected under applicable copyright laws. All rights are reserved.
; Any use in violation of the foregoing restrictions may subject the
; user to criminal sanctions under applicable laws, as well as to
; civil liability for the breach of the terms and conditions of this
; license.
;
; THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
; WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
; TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
; PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
; IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
; CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;-------------------------------------------------------------------------------

#include <p18cxxx.inc>

	extern YourHighPriorityISRCode	;Function is in main.c.  This is the actual interrupt handler which checks flags and decides what to do.
	extern YourLowPriorityISRCode	;Function is in main.c.  This is the actual interrupt handler which checks flags and decides what to do.


;-------------------------------------------------------------------------------
;Important Notes regarding clock switching to 31kHz INTRC during USB Suspend
;-------------------------------------------------------------------------------
;During USB suspend, a USB device is required to reduce its total Vbus power
;consumption to no more than 2.5mA, in order to successfully pass USB compliance
;tests.  In an application that is bus powered, this 2.5mA or less current 
;consumption is normally achieved by placing the microcontroller into either sleep
;mode, or by clock switching to a lower frequency clock source (ex: the 31kHz INTRC).

;If the microcontroller firmware wishes to clock switch to a very low frequency
;clock source, such as 31kHz, then special considerations can become necessary.

;During an interrupt event in a C18 based project, the interrupt handler routine
;will execute some code to perform context saving operations.  The C compiler
;manages certain registers and resources (ex: such as pointers, intermediate variables
;used during mathematical operations, etc.), which may need to get saved during the
;interrupt context save (in addition to the standard registers, such as BSR, 
;WREG, STATUS).  This C compiler context save can take a relatively long time
;to execute.  At a PIC18 microcontroller frequency of 48MHz, a context save
;operation may take approximately 10us.  At 31kHz, the operation will take much
;longer (ex: ~15ms).

;When a USB host wakes up a USB bus segment from the USB  suspend state, it will 
;do so by sending non-idle bus signalling to the device (normally a special 
;"resume" event, which is a driven 'K' state).  This will trigger the 
;UIR, ACTVIF interrupt flag, allowing the microcontroller firmware to know that 
;it should wake up from sleep and/or clock switch back to the primary clock 
;source (back to settings compatible with normal USB operation).

;Once the host has sent the special resume signalling, the host is required
;to wait a minimum of 10ms to allow for a "recovery interval."  This interval 
;is intended to allow the USB device to finish waking up and prepare itself to 
;begin receiving new USB commands/traffic.  The USB device is required to be 
;ready to communicate and respond to traffic at the end of the 10ms interval.

;If the microcontroller clock switched to 31kHz at the start of the USB suspend
;condition, a problem can occur if the interrupt context save operation (caused
;by the UIR, ACTVIF interrupt event) takes the full ~15ms (at 31kHz).  In this 
;scenario, the USB device firmware may not be ready in time to receive the 
;first USB packets sent from the host following the resume recovery interval.

;To avoid this potential problem, the microcontroller firmware should quickly 
;switch back to the primary clock source, after the UIR,ACTVIF interrupt has
;occurred, but prior to performing the interrupt context save operation.  
;In this way, the context save operation can take place at a much higher 
;frequency (ex: the full 48MHz), and therefore can be completed much quicker 
;than the 10ms recovery interval.  The USB device will therefore be fully
;ready to begin receiving USB traffic well before the 10ms recovery interval
;has elapsed.

#ifdef __18F87J94
    #define PIC18F97J94_FAMILY
#endif
#ifdef __18F97J94
    #define PIC18F97J94_FAMILY
#endif



#ifdef PIC18F97J94_FAMILY
;-----------------------------------------------
;High Priority Interrupt Vector
;-----------------------------------------------
HiPriVector		code	0x08
HighPriorityIntVector:
		goto	YourHighPriorityISRCode	;Interrupt context save will be done at the start of this C function.

;-----------------------------------------------
;Low Priority Interrupt Vector
;-----------------------------------------------
LowPriVector	code	0x18
LowPriorityIntVector:
		goto	YourLowPriorityISRCode	;Interrupt context save will be done at the start of this C function.

;------------------------------------------------------------------------------------------------------------
;BELOW SECTION IS ONLY NECESSARY IF THIS PROJECT WILL BE PROGRAMMED WITH THE HID BOOTLOADER.  IF THIS IS
;NOT REQUIRED, THE BELOW SECTIONS MAY BE COMMENTED OUT.
;------------------------------------------------------------------------------------------------------------

;----------------------------------------------------------------------
;Remapped High Priority Interrupt Vector (for use with HID Bootloader)
;----------------------------------------------------------------------
Remapped_HiPriVector		code	0x1008
Remapped_HighPriorityIntVector:
		goto	YourHighPriorityISRCode	;Interrupt context save will be done at the start of this C function.

;----------------------------------------------------------------------
;Remapped Low Priority Interrupt Vector (for use with HID Bootloader)
;----------------------------------------------------------------------
Remapped_LowPriVector	code	0x1018
Remapped_LowPriorityIntVector:
		goto	YourLowPriorityISRCode	;Interrupt context save will be done at the start of this C function.
		
#else  ;#ifdef PIC18F97J94_FAMILY
;All other PIC18 devices



;-----------------------------------------------
;High Priority Interrupt Vector
;-----------------------------------------------
HiPriVector		code	0x08
HighPriorityIntVector:
		;Clock switch to the Primary clock source (ex: OSCCON<SCS1:SCS0> = 00).
		;Make sure that the below instructions do not change any status bits,
		;WREG contents, etc.  We have not yet done the interrupt context save
		;operation, so we need to be careful not to touch any important resources yet.
		bcf		OSCCON, SCS0		
		bcf		OSCCON, SCS1			;OSCCON<SCS1:SCS0> now = 00 <-- Primary Clock Source Selected
		bsf		OSCCON, IRCF2			;Switch to some moderate frequency well above 31kHz (if the "primary clock" happens to be the (HF)INTOSC)
GotoHighPriorityInterruptHandler:
		goto	YourHighPriorityISRCode	;Interrupt context save will be done at the start of this C function.



;-----------------------------------------------
;Low Priority Interrupt Vector
;-----------------------------------------------
LowPriVector	code	0x18
LowPriorityIntVector:
		;Clock switch to the Primary clock source (ex: OSCCON<SCS1:SCS0> = 00).
		;Make sure that the below instructions do not change any status bits,
		;WREG contents, etc.  We have not yet done the interrupt context save
		;operation, so we need to be careful not to touch any important resources yet.
		bcf		OSCCON, SCS0		
		bcf		OSCCON, SCS1			;OSCCON<SCS1:SCS0> now = 00 <-- Primary Clock Source Selected
		bsf		OSCCON, IRCF2			;Switch to some moderate frequency well above 31kHz (if the "primary clock" happens to be the (HF)INTOSC)
GotoLowPriorityInterruptHandler:
		goto	YourLowPriorityISRCode	;Interrupt context save will be done at the start of this C function.







;------------------------------------------------------------------------------------------------------------
;BELOW SECTION IS ONLY NECESSARY IF THIS PROJECT WILL BE PROGRAMMED WITH THE HID BOOTLOADER.  IF THIS IS
;NOT REQUIRED, THE BELOW SECTIONS MAY BE COMMENTED OUT.
;------------------------------------------------------------------------------------------------------------

;----------------------------------------------------------------------
;Remapped High Priority Interrupt Vector (for use with HID Bootloader)
;----------------------------------------------------------------------
Remapped_HiPriVector		code	0x1008
Remapped_HighPriorityIntVector:
		;Clock switch to the Primary clock source (ex: OSCCON<SCS1:SCS0> = 00).
		;Make sure that the below instructions do not change any status bits,
		;WREG contents, etc.  We have not yet done the interrupt context save
		;operation, so we need to be careful not to touch any important resources yet.
		bcf		OSCCON, SCS0		
		bcf		OSCCON, SCS1			;OSCCON<SCS1:SCS0> now = 00 <-- Primary Clock Source Selected
		bsf		OSCCON, IRCF2			;Switch to some moderate frequency well above 31kHz (if the "primary clock" happens to be the (HF)INTOSC)
Remapped_GotoHighPriIntHandler:
		goto	YourHighPriorityISRCode	;Interrupt context save will be done at the start of this C function.



;----------------------------------------------------------------------
;Remapped Low Priority Interrupt Vector (for use with HID Bootloader)
;----------------------------------------------------------------------
Remapped_LowPriVector	code	0x1018
Remapped_LowPriorityIntVector:
		;Clock switch to the Primary clock source (ex: OSCCON<SCS1:SCS0> = 00).
		;Make sure that the below instructions do not change any status bits,
		;WREG contents, etc.  We have not yet done the interrupt context save
		;operation, so we need to be careful not to touch any important resources yet.
		bcf		OSCCON, SCS0		
		bcf		OSCCON, SCS1			;OSCCON<SCS1:SCS0> now = 00 <-- Primary Clock Source Selected
		bsf		OSCCON, IRCF2			;Switch to some moderate frequency well above 31kHz (if the "primary clock" happens to be the (HF)INTOSC)
Remapped_GotoLowPriIntHandler:
		goto	YourLowPriorityISRCode	;Interrupt context save will be done at the start of this C function.
		



#endif		
		
		end