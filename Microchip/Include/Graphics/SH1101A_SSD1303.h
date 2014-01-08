/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Sino Wealth Microelectronic SH1101A OLED controller driver 
 *  Solomon Systech SSD1303 LCD controller driver 
  *****************************************************************************
 * FileName:        SH1101A_SSD1303.h
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
 * 03/10/07		Original
 * 12/20/07		Ported to PIC24 Kit
 * 03/22/11     Modified for Graphics Library Version 3.00
 *****************************************************************************/
#ifndef _SH1101A_SSD1303_OLED_H
    #define _SH1101A_SSD1303_OLED_H

    #include "HardwareProfile.h"
    #include "GenericTypeDefs.h"
    #include "GraphicsConfig.h"
    
#if defined (USE_GFX_PMP)
    #include "Graphics/gfxpmp.h"
#elif defined (USE_GFX_PMP)
    #include "Graphics/gfxepmp.h"
#endif

    #ifdef USE_16BIT_PMP
        #error "This driver do not support 16-bit PMP (remove USE_16BIT_PMP option from HardwareProfile.h)"
    #endif
    #ifndef DISP_HOR_RESOLUTION
        #error "DISP_HOR_RESOLUTION must be defined in HardwareProfile.h"
    #endif
    #ifndef DISP_VER_RESOLUTION
        #error "DISP_VER_RESOLUTION must be defined in HardwareProfile.h"
    #endif
    #ifndef COLOR_DEPTH
        #error "COLOR_DEPTH must be defined in GraphicsConfig.h"
    #endif
    #ifndef DISP_ORIENTATION
        #error "DISP_ORIENTATION must be defined in HardwareProfile.h"
    #endif

/*********************************************************************
* Overview: Horizontal and vertical screen size.
*********************************************************************/
    #if (DISP_HOR_RESOLUTION != 128)
        #error "This driver do not support this resolution. Set horisontal resolution to 128 pixels."
    #endif
    #if (DISP_VER_RESOLUTION != 64)
        #error "This driver do not support this resolution. Set vertical resolution to 64 pixels."
    #endif

/*********************************************************************
* Overview: Display orientation.
*********************************************************************/
    #if (DISP_ORIENTATION != 0)
        #error "This driver do not support this orientation. Set the DISP_ORIENTATION to 0."
    #endif

/*********************************************************************
* Overview: Defines the display offset in x direction. Dependent on the display 
* used and how it is connected.
*********************************************************************/
    #define OFFSET  2


/*********************************************************************
* Overview: Screen Saver parameters. 
*			- SSON - Means that screen saver will be enabled when 
*					 ScreenSaver(SSON) function is called with SSON as 
*					 parameter.
*			- SSOFF - Means that screen saver will be disbled when 
*					 ScreenSaver(SSOFF) function is called with SSOFF as 
*					 parameter.
*           
*********************************************************************/
    #define SSON    1           // screen saver is turned on
    #define SSOFF   0           // screen saver is turned off

    // Memory pitch for line
    #define LINE_MEM_PITCH  0x100

/*********************************************************************
* Macros: SetPalette(colorNum, color)
*
* Overview:  Sets palette register.
*
* PreCondition: none
*
* Input: colorNum - Register number.
*        color - Color.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define SetPalette(colorNum, color)

#endif // _SH1101A_SSD1303_OLED_H
