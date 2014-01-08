/*****************************************************************************
 * FileName:        InternalResourceXC16.h
 * Processor:       PIC24F, PIC24H, dsPIC
 * Compiler:        MPLAB C30/XC16 (see release notes for tested revision)
 * Linker:          MPLAB LINK30/XC16
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright(c) 2012 Microchip Technology Inc.  All rights reserved.
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
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * AUTO-GENERATED CODE:  Graphics Resource Converter version: 3.28.15
 *****************************************************************************/

#ifndef INTERNALRESOURCEXC16_H_FILE
#define INTERNALRESOURCEXC16_H_FILE
/*****************************************************************************
 * SECTION:  Includes
 *****************************************************************************/
#include <Graphics/Graphics.h>
#include "HardwareProfile.h"

/*****************************************************************************
 * SECTION:  Graphics Library Firmware Check
 *****************************************************************************/
#if(GRAPHICS_LIBRARY_VERSION != 0x0306)
#warning "It is suggested to use Graphics Library version 3.06 with this version of GRC."
#endif

/*****************************************************************************
 * SECTION:  Graphics Bitmap Padding Check
 *****************************************************************************/
#ifdef USE_BITMAP_NO_PADDING_LINE
#error These bitmap resources have been converted with padding of the horizontal lines, but USE_BITMAP_NO_PADDING_LINE is defined in GraphicsConfig.h.
#endif

/*****************************************************************************
 * SECTION: Hardware Profile
 * Place these byte count defines in the application level hardware profile header file
 *
 * #define GFX_COMPRESSED_BUFFER_SIZE       (1542)
 * #define GFX_DECOMPRESSED_BUFFER_SIZE     (2074)
 *****************************************************************************/

/*****************************************************************************
 * This is an error check for the color depth
 *****************************************************************************/
#if (COLOR_DEPTH > 16)
#error "Color Depth needs to be 16 to correctly use these resources"
#endif



/*****************************************************************************
 * SECTION:  BITMAPS
 *****************************************************************************/

/*********************************
 * Bitmap Structure
 * Label: MenuButton
 * Description:  76x35 pixels, 16-bits per pixel
 ***********************************/
extern const IMAGE_FLASH MenuButton;
#define MenuButton_WIDTH     (76)
#define MenuButton_HEIGHT    (35)
#define MenuButton_SIZE      (5326)
/*********************************
 * Bitmap Structure
 * Label: MenuButtonPressed
 * Description:  76x35 pixels, 16-bits per pixel
 ***********************************/
extern const IMAGE_FLASH MenuButtonPressed;
#define MenuButtonPressed_WIDTH     (76)
#define MenuButtonPressed_HEIGHT    (35)
#define MenuButtonPressed_SIZE      (5326)
/*********************************
 * Bitmap Structure
 * Label: Engine1
 * Description:  73x55 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER Engine1;
#define Engine1_WIDTH     (73)
#define Engine1_HEIGHT    (55)
#define Engine1_SIZE      (2073)
/*********************************
 * Bitmap Structure
 * Label: Engine2
 * Description:  73x55 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER Engine2;
#define Engine2_WIDTH     (73)
#define Engine2_HEIGHT    (55)
#define Engine2_SIZE      (2073)
/*********************************
 * Bitmap Structure
 * Label: Engine3
 * Description:  73x55 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER Engine3;
#define Engine3_WIDTH     (73)
#define Engine3_HEIGHT    (55)
#define Engine3_SIZE      (2073)
/*********************************
 * Bitmap Structure
 * Label: Engine4
 * Description:  73x55 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER Engine4;
#define Engine4_WIDTH     (73)
#define Engine4_HEIGHT    (55)
#define Engine4_SIZE      (2073)
/*****************************************************************************
 * SECTION:  Fonts
 *****************************************************************************/

/*********************************
 * TTF Font File Structure
 * Label: GOLSmallFont
 * Description:  Height: 15 pixels, 1 bit per pixel, Range: ' ' to '~'
 ***********************************/
extern const FONT_FLASH GOLSmallFont;
#define GOLSmallFont_SIZE    (1903)
/*********************************
 * TTF Font File Structure
 * Label: GOLMediumFont
 * Description:  Height: 19 pixels, 1 bit per pixel, Range: ' ' to '~'
 ***********************************/
extern const FONT_FLASH GOLMediumFont;
#define GOLMediumFont_SIZE    (2668)
/*********************************
 * TTF Font File Structure
 * Label: VeraMono_28
 * Description:  Height: 33 pixels, 1 bit per pixel, Range: '.' to '9'
 ***********************************/
extern const FONT_FLASH VeraMono_28;
#define VeraMono_28_SIZE    (1244)
/*********************************
 * TTF Font File Structure
 * Label: VeraMono_17
 * Description:  Height: 20 pixels, 1 bit per pixel, Range: '.' to '9'
 ***********************************/
extern const FONT_FLASH VeraMono_17;
#define VeraMono_17_SIZE    (536)
#endif

