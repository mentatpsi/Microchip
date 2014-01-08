/*****************************************************************************
 *
 * Module for Microchip Graphics Library
 * The header file contains scan codes for the standard AT keyboard
 *
 *****************************************************************************
 * FileName:        ScanCodes.h
 * Dependencies:    None
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is SHORTegrated SHORTo your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Date        	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 07/20/07		...
 *****************************************************************************/
#ifndef _SCANCODES_H
    #define _SCANCODES_H

/*********************************************************************
* Overview: The following are the defined scan codes for AT keyboard.
*
*********************************************************************/

// Carriage return pressed.
    #define SCAN_CR_PRESSED 0x1C

// Carriage return released.
    #define SCAN_CR_RELEASED    0x9C

// Carriage return alternate pressed.
    #define SCAN_CRA_PRESSED    0x2C

// Carriage return alternate released.
    #define SCAN_CRA_RELEASED   0xAC

// Delete key pressed.
    #define SCAN_DEL_PRESSED    0x53

// Delete key released.
    #define SCAN_DEL_RELEASED   0xD3

// Back space key pressed.
    #define SCAN_BS_PRESSED 0x0E

// Back space key released.
    #define SCAN_BS_RELEASED    0x8E

// Tab key pressed.
    #define SCAN_TAB_PRESSED    0x0F

// Tab key released.
    #define SCAN_TAB_RELEASED   0x8F

// Home key pressed.
    #define SCAN_HOME_PRESSED   0x47

// Home key released.
    #define SCAN_HOME_RELEASED  0xC7

// End key pressed.
    #define SCAN_END_PRESSED    0x4F

// End key released.
    #define SCAN_END_RELEASED   0xCF

// Page up key pressed.
    #define SCAN_PGUP_PRESSED   0x49

// Page up key released.
    #define SCAN_PGUP_RELEASED  0xC9

// Page down key pressed.
    #define SCAN_PGDOWN_PRESSED 0x51

// Page down key released.
    #define SCAN_PGDOWN_RELEASED    0xD1

// Up key pressed.
    #define SCAN_UP_PRESSED 0x48

// Up key released.
    #define SCAN_UP_RELEASED    0xC8

// Down key pressed.
    #define SCAN_DOWN_PRESSED   0x50

// Down key released.
    #define SCAN_DOWN_RELEASED  0xD0

// Left key pressed.
    #define SCAN_LEFT_PRESSED   0x4B

// Left key released.
    #define SCAN_LEFT_RELEASED  0xCB

// Right key pressed.
    #define SCAN_RIGHT_PRESSED  0x4D

// Right key released.
    #define SCAN_RIGHT_RELEASED 0xCD

// Space key pressed.
    #define SCAN_SPACE_PRESSED  0x39

// Space key released.
    #define SCAN_SPACE_RELEASED 0xB9
#endif // _SCANCODES_H
