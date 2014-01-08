/******************************************************************************

    LCD interface header file

Description:
	This file contains prototypes for basic LCD driver routines for explorer 
	16 board.

Summary:
	None

Remarks:
	None

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

* File Name:       lcd_demo.h
* Dependencies:    None
* Processor:       PIC24FJ256GB110
* Compiler:        C30 v2.01
* Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the “Company”) for its PICmicro® Microcontroller is intended and
supplied to you, the Company’s customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
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

Author          Date    Comments
--------------------------------------------------------------------------------
ADG         14-Apr-2008 First release

*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Function Prototypes
// *****************************************************************************
// *****************************************************************************

void LCDInit(void);
void LCDHome(void);
void LCDL1Home(void);
void LCDL2Home(void);
void LCDClear(void);
void LCDPut(char A);
void LCDShiftCursorLeft(void);
void LCDShiftCursorRight(void);
void LCDShiftCursorUp(void);
void LCDShiftCursorDown(void);
void Wait(unsigned int);
