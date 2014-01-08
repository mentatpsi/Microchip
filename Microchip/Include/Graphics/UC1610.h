/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  UltraChip UC1610 LCD controllers driver
 *****************************************************************************
 * FileName:        UC1610.h
 * Dependencies:    p24Fxxxx.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2009 Microchip Technology Inc.  All rights reserved.
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
 * 02/25/09		...
 * 04/12/11     Graphics Library Version 3.00 Support
 *****************************************************************************/
#ifndef _UC1610_H
    #define _UC1610_H

    #include "HardwareProfile.h"
    #include "GraphicsConfig.h"
    #include "GenericTypeDefs.h"
    
    #ifdef USE_16BIT_PMP
        #error This driver doesn't support 16-bit PMP (remove USE_16BIT_PMP option from HardwareProfile.h)
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

/*********************************************************************
* Overview: Horizontal and vertical screen size.
*********************************************************************/
    #if (DISP_HOR_RESOLUTION != 160)
        #error This driver doesn't supports this resolution. Horizontal resolution must be 160 pixels.
    #endif
    #if (DISP_VER_RESOLUTION != 100)
        #error This driver doesn't supports this resolution. Vertical resolution must be 100 pixels.
    #endif

/*********************************************************************
* Overview: Display orientation.
*********************************************************************/
    #if (DISP_ORIENTATION != 0)
        #error This driver doesn't support this orientation. It must be 0.
    #endif

/*********************************************************************
* Overview: Color depth.
*********************************************************************/
    #if (COLOR_DEPTH != 8)
        #error This driver doesn't support the selected color depth. It should be 8 but the device uses only 2 bits. The 6 MSbits will be ignored.
    #endif
    #define CMD_DISPLAY_ON          0b10101111
    #define CMD_DISPLAY_OFF         0b10101110

    #define CMD_INVERSE_ON          0b10100111
    #define CMD_INVERSE_OFF         0b10100110

    #define CMD_PANEL_LOADING_16NF  0b00101000
    #define CMD_PANEL_LOADING_21NF  0b00101001
    #define CMD_PANEL_LOADING_28NF  0b00101010
    #define CMD_PANEL_LOADING_38NF  0b00101011

    #define CMD_LINE_RATE_12KLPS    0b10100000
    #define CMD_LINE_RATE_13KLPS    0b10100001
    #define CMD_LINE_RATE_14KLPS    0b10100010
    #define CMD_LINE_RATE_16KLPS    0b10100011

    #define CMD_DISPLAY_START       0b11110010
    #define CMD_DISPLAY_END         0b11110011
    #define CMD_COM_END             0b11110001

    #define CMD_BIAS_RATIO_5        0b11101000
    #define CMD_BIAS_RATIO_10       0b11101001
    #define CMD_BIAS_RATIO_11       0b11101010
    #define CMD_BIAS_RATIO_12       0b11101011

    #define CMD_CONTRAST            0b10000001
    
    #define CMD_RAM_ADDR_INCR_OFF   0b10001000
    #define CMD_RAM_ADDR_INCR_ON    0b10001101

    #define CMD_MAPPING_CTRL        0b11000000
    #define CMD_MAPPING_MY          0b11000100
    #define CMD_MAPPING_MX          0b11000010

    #define CMD_WND_PRG_DISABLE     0b11111000
    #define CMD_WND_PRG_ENABLE      0b11111001
    #define CMD_START_COLUMN        0b11110100
    #define CMD_START_PAGE          0b11110101
    #define CMD_END_COLUMN          0b11110110
    #define CMD_END_PAGE            0b11110111

    #define CMD_COLUMN_ADDR_LSB     0b00000000
    #define CMD_COLUMN_ADDR_MSB     0b00010000
    #define CMD_PAGE_ADDR           0b01100000

    #define DISP_START_COLUMN       0
    #define DISP_START_PAGE         7
    #define DISP_END_COLUMN         159
    #define DISP_END_PAGE           31

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

/*********************************************************************
* Function: void ContrastSet(BYTE contrast)
*
* PreCondition: none
*
* Input: contrast value
*
* Output: none
*
* Side Effects: none
*
* Overview: sets contrast
*
* Note: none
*
********************************************************************/
void    ContrastSet(BYTE contrast);
#endif // _UC1610_H
