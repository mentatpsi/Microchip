/*****************************************************************************
 * Simple 4 wire resistive touch screen driver
 *****************************************************************************
 * FileName:        TouchScreenResistive.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
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
 * 01/19/11		Ported from TouchScreen.h. 
 *****************************************************************************/

/*****************************************************************************
 Description:  This is a resistive touch screen driver that is using the 
			   Microchip Graphics Library. The calibration values are 
			   automatically checked (by reading a specific memory location
			   on the non-volatile memory) when initializing the module if the 
			   function pointers to the read and write callback functions 
			   are initialized. If the read value is invalid calibration 
			   will automatically be executed. Otherwise, the calibration
			   values will be loaded and used.
			   The driver assumes that the application side provides the 
			   read and write routines to a non-volatile memory. 
			   If the callback functions are not initialized, the calibration
			   routine will always be called at startup to initialize the
			   global calibration values.
			   This driver assumes that the Graphics Library is initialized 
			   and will be using the default font of the library.
 *****************************************************************************/

#ifndef _TOUCHSCREEN_RESISTIVE_H
#define _TOUCHSCREEN_RESISTIVE_H

#include "GenericTypeDefs.h"

// Default calibration points
#define TOUCHCAL_ULX 0x2A
#define TOUCHCAL_ULY 0xE67E
#define TOUCHCAL_URX 0x2A
#define TOUCHCAL_URY 0xE67E
#define TOUCHCAL_LLX 0x2A
#define TOUCHCAL_LLY 0xE67E
#define TOUCHCAL_LRX 0x2A
#define TOUCHCAL_LRY 0xE67E


// use this macro to debug the touch screen panel 
// this will enable the use of debugging functions in the C file.
// It assumes that the graphics portion is working.
//#define ENABLE_DEBUG_TOUCHSCREEN


#endif //_TOUCHSCREEN_RESISTIVE_H
