/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Densitron HIT1270 LCD controller driver 
 *  Landscape orientation only
 *****************************************************************************
 * FileName:        HIT1270.h
 * Dependencies:    p24Fxxxx.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
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
 * Date			Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07		Version 1.0 release
 * 04/12/11     Support for Graphics Library Version 3.00
 *****************************************************************************/
#ifndef _HIT1270L_H
    #define _HIT1270L_H

    #include "HardwareProfile.h"
    #include "GraphicsConfig.h"
    #include "GenericTypeDefs.h"
   
    #ifdef USE_16BIT_PMP
        #error This driver doesn't support 16-bit PMP (USE_8BIT_PMP instead in HardwareProfile.h)
    #endif
    #ifndef DISP_HOR_RESOLUTION
        #error DISP_HOR_RESOLUTION must be defined in HardwareProfile.h
    #endif
    #ifndef DISP_VER_RESOLUTION
        #error DISP_VER_RESOLUTION must be defined in HardwareProfile.h
    #endif
    #ifndef COLOR_DEPTH
        #error COLOR_DEPTH must be defined in GraphicsConfig.h
    #endif
    #ifndef DISP_ORIENTATION
        #error DISP_ORIENTATION must be defined in HardwareProfile.h
    #endif

// Horizontal and vertical screen size
    #if (DISP_HOR_RESOLUTION != 320)
        #error This driver doesn't supports this resolution. Horisontal resolution must be 320 pixels.
    #endif
    #if (DISP_VER_RESOLUTION > 234)
        #error This driver doesn't supports this resolution. Vertical resolution must be 234 pixels or less.
    #endif

// Display orientation
    #if (DISP_ORIENTATION != 0)
        #error This driver doesn't support this orientation.
    #endif

// Color depth
    #if (COLOR_DEPTH != 16)
        #error This driver doesn't support this color depth. It should be 16.
    #endif

// Memory pitch for line
    #define LINE_MEM_PITCH  320

// Video buffer offset
    #define BUF_MEM_OFFSET  (DWORD) 0x18000

// HIT1270 commands
    #define SET_DATA_POINTER    0x50
    #define SRAM_WRITE          0x51
    #define STAMP_ICON          0x52
    #define FILL_BLOCK          0x53

    #define FLASH_WRITE         0x60
    #define FLASH_READ          0x61


/*********************************************************************
* Macros: SetPalette(colorNum, color)
*
* PreCondition: none
*
* Input: colorNum - register number, color - color
*
* Output: none
*
* Side Effects: none
*
* Overview:  sets palette register
*
* Note: S6D0129 has no palette
*
********************************************************************/
    #define SetPalette(colorNum, color)

#endif // HIT1270L.h
