/******************************************************************************

    LCD interface file

Description:
    This file contains basic LCD driver routines for explorer 16 board.

Summary:
    This file contains basic LCD driver routines for explorer 16 board.

Remarks:
    None

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

* File Name:       lcd_demo.c
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

//#include  <p24FJ128GA010.h>

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "lcd_demo.h"

// Define a fast instruction execution time in terms of loop time
// typically > 43us
#if defined(__C30__) || defined __XC16__
#define LCD_F_INSTR     100
#elif defined(__PIC32MX__)
#define LCD_F_INSTR     1000
#endif

// Define a slow instruction execution time in terms of loop time
// typically > 1.35ms
#define LCD_S_INSTR     3000

// Define the startup time for the LCD in terms of loop time
// typically > 30ms
#define LCD_STARTUP     20000

#if defined( __PIC32MX__ )
     #define PMDIN1 PMDIN
#endif


unsigned int    _uLCDloops;

void LCDInit(void)
{
    _uLCDloops = LCD_STARTUP;
    Wait(_uLCDloops);

    _uLCDloops = LCD_F_INSTR;
    PMDIN1 = 0x38;      //0b00111000;            // Set the default function
    Wait(_uLCDloops);

    _uLCDloops = LCD_STARTUP;
    Wait(_uLCDloops);

    _uLCDloops = LCD_F_INSTR;
    PMDIN1 = 0x0C;      //0b00001100;
    Wait(_uLCDloops);

    _uLCDloops = LCD_STARTUP;
    Wait(_uLCDloops);

    _uLCDloops = LCD_S_INSTR;
    PMDIN1 = 0x01;      //0b00000001;            // Clear the display
    Wait(_uLCDloops);

    _uLCDloops = LCD_STARTUP;
    Wait(_uLCDloops);

    _uLCDloops = LCD_S_INSTR;
//    PMDIN1 = 0b00001111;            // Clear the display
    PMDIN1 = 0x0C;      //0b00001100;            // Clear the display
    Wait(_uLCDloops);

    _uLCDloops = LCD_STARTUP;
    Wait(_uLCDloops);

    
    _uLCDloops = LCD_S_INSTR;
    PMDIN1 = 0x04;      //0b00000100;            // Set the entry mode
    Wait(_uLCDloops);

    LCDClear();
    LCDHome();
}



void LCDHome(void)
{
    _uLCDloops = LCD_S_INSTR;
    PMADDR = 0x0000;
    PMDIN1 = 0x02;      //0b00000010;
    while(_uLCDloops)
    _uLCDloops--;
}


void LCDL1Home(void)
{
    _uLCDloops = LCD_S_INSTR;
    PMADDR = 0x0000;
    PMDIN1 = 0x80;      //0b10000000;
    while(_uLCDloops)
    _uLCDloops--;
}

void LCDL2Home(void)
{
    _uLCDloops = LCD_S_INSTR;
    PMADDR = 0x0000;
    PMDIN1 = 0xC0;      //0b11000000;
    while(_uLCDloops)
    _uLCDloops--;
}


void LCDClear(void)
{
    _uLCDloops = LCD_S_INSTR;
    PMADDR = 0x0000;
    PMDIN1 = 0x01;      //0b00000001;
    while(_uLCDloops)
    _uLCDloops--;
}


void LCDPut(char A)
{
    _uLCDloops = LCD_F_INSTR;
    PMADDR = 0x0001;
    PMDIN1 = A;
    while(_uLCDloops)
    _uLCDloops--;
    Nop();
    Nop();
    Nop();
    Nop();
}

void LCDShiftCursorLeft(void)
{
    _uLCDloops = LCD_F_INSTR;
    PMADDR = 0x0000;
    PMDIN1 = 0x10;      //0b00010000;
    Wait(_uLCDloops);
}


void LCDShiftCursorRight(void)
{
    _uLCDloops = LCD_F_INSTR;
    PMADDR = 0x0000;
    PMDIN1 = 0x14;      //0b00010100;
    Wait(_uLCDloops);
}

void LCDShiftCursorUp(void)
{
    BYTE i;
    // Set Cursor off
    _uLCDloops = LCD_F_INSTR;
    PMADDR = 0x0000;
    PMDIN1 = 0x0C;      //0b00001100;
    Wait(_uLCDloops);

    for(i=0;i<16;i++)
    {
        LCDShiftCursorLeft();
    }

// Set Cursor Blink
    _uLCDloops = LCD_F_INSTR;
    PMADDR = 0x0000;
    PMDIN1 = 0x0F;      //0b00001111;
    Wait(_uLCDloops);

}

void LCDShiftCursorDown(void)
{
    BYTE i;
    // Set Cursor off
    _uLCDloops = LCD_F_INSTR;
    PMADDR = 0x0000;
    PMDIN1 = 0x0C;      //0b00001100;
    Wait(_uLCDloops);

    for(i=0;i<16;i++)
    {
        LCDShiftCursorRight();
    }

// Set Cursor Blink
    _uLCDloops = LCD_F_INSTR;
    PMADDR = 0x0000;
    PMDIN1 = 0x0F;      //0b00001111;
    Wait(_uLCDloops);

}

void Wait(unsigned int B)
{
    while(B)
    B--;
}


