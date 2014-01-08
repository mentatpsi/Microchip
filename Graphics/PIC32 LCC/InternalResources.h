/*****************************************************************************
 * FileName:        InternalResources.h
 * Processor:       PIC32MX
 * Compiler:        MPLAB C32/XC32 (see release notes for tested revision)
 * Linker:          MPLAB LINK32/XC32
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

#ifndef INTERNALRESOURCES_H_FILE
#define INTERNALRESOURCES_H_FILE
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
 * Label: Back_4bpp_16x16
 * Description:  16x16 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_FLASH Back_4bpp_16x16;
#define Back_4bpp_16x16_WIDTH     (16)
#define Back_4bpp_16x16_HEIGHT    (16)
#define Back_4bpp_16x16_SIZE      (166)
/*********************************
 * Bitmap Structure
 * Label: MCHPFolderEmpty_8bpp_72x72
 * Description:  72x72 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH MCHPFolderEmpty_8bpp_72x72;
#define MCHPFolderEmpty_8bpp_72x72_WIDTH     (72)
#define MCHPFolderEmpty_8bpp_72x72_HEIGHT    (72)
#define MCHPFolderEmpty_8bpp_72x72_SIZE      (5702)
/*********************************
 * Bitmap Structure
 * Label: MCHPFolderFile_8bpp_72x72
 * Description:  72x72 pixels, 16-bits per pixel
 ***********************************/
extern const IMAGE_FLASH MCHPFolderFile_8bpp_72x72;
#define MCHPFolderFile_8bpp_72x72_WIDTH     (72)
#define MCHPFolderFile_8bpp_72x72_HEIGHT    (72)
#define MCHPFolderFile_8bpp_72x72_SIZE      (10374)
/*********************************
 * Bitmap Structure
 * Label: MchpIcon
 * Description:  100x58 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_FLASH MchpIcon;
#define MchpIcon_WIDTH     (100)
#define MchpIcon_HEIGHT    (58)
#define MchpIcon_SIZE      (2938)
/*********************************
 * Bitmap Structure
 * Label: mouse
 * Description:  13x15 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_FLASH mouse;
#define mouse_WIDTH     (13)
#define mouse_HEIGHT    (15)
#define mouse_SIZE      (143)
/*********************************
 * Bitmap Structure
 * Label: Pacman
 * Description:  90x50 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_FLASH Pacman;
#define Pacman_WIDTH     (90)
#define Pacman_HEIGHT    (50)
#define Pacman_SIZE      (2288)
/*****************************************************************************
 * SECTION:  JPEGS
 *****************************************************************************/

/*********************************
 * JPEG Structure
 * Label: clouds
 * Description:  141x107 pixels
 ***********************************/
extern const IMAGE_FLASH clouds;
#define clouds_WIDTH     (141)
#define clouds_HEIGHT    (107)
#define clouds_SIZE      (2526)
/*********************************
 * JPEG Structure
 * Label: Demo
 * Description:  480x272 pixels
 ***********************************/
extern const IMAGE_FLASH Demo;
#define Demo_WIDTH     (480)
#define Demo_HEIGHT    (272)
#define Demo_SIZE      (38024)
/*****************************************************************************
 * SECTION:  Fonts
 *****************************************************************************/

/*********************************
 * Installed Font Structure
 * Label: Monospaced_bold_Bold_72
 * Description:  Height: 95 pixels, 1 bit per pixel, Range: '0' to '9'
 ***********************************/
extern const FONT_FLASH Monospaced_bold_Bold_72;
#define Monospaced_bold_Bold_72_SIZE    (5748)
/*********************************
 * Installed Font Structure
 * Label: Monospaced_plain_36
 * Description:  Height: 48 pixels, 1 bit per pixel, Range: ' ' to 'z'
 ***********************************/
extern const FONT_FLASH Monospaced_plain_36;
#define Monospaced_plain_36_SIZE    (13476)
/*********************************
 * TTF Font File Structure
 * Label: Gentium_16
 * Description:  Height: 19 pixels, 1 bit per pixel, Range: ' ' to '~'
 ***********************************/
extern const FONT_FLASH Gentium_16;
#define Gentium_16_SIZE    (2668)
#endif

