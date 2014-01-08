/*****************************************************************************
 * FileName:        PicturesFont.h
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

#ifndef PICTURESFONT_H_FILE
#define PICTURESFONT_H_FILE
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
 * Label: snakeBody
 * Description:  8x8 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH snakeBody;
#define snakeBody_WIDTH     (8)
#define snakeBody_HEIGHT    (8)
#define snakeBody_SIZE      (18)
/*********************************
 * Bitmap Structure
 * Label: snakeFood
 * Description:  8x8 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH snakeFood;
#define snakeFood_WIDTH     (8)
#define snakeFood_HEIGHT    (8)
#define snakeFood_SIZE      (18)
/*********************************
 * Bitmap Structure
 * Label: snakeHead
 * Description:  8x8 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH snakeHead;
#define snakeHead_WIDTH     (8)
#define snakeHead_HEIGHT    (8)
#define snakeHead_SIZE      (18)
/*********************************
 * Bitmap Structure
 * Label: ballCircle
 * Description:  8x8 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH ballCircle;
#define ballCircle_WIDTH     (8)
#define ballCircle_HEIGHT    (8)
#define ballCircle_SIZE      (18)
/*********************************
 * Bitmap Structure
 * Label: ballCross
 * Description:  8x8 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH ballCross;
#define ballCross_WIDTH     (8)
#define ballCross_HEIGHT    (8)
#define ballCross_SIZE      (18)
/*********************************
 * Bitmap Structure
 * Label: ballMarking
 * Description:  8x8 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH ballMarking;
#define ballMarking_WIDTH     (8)
#define ballMarking_HEIGHT    (8)
#define ballMarking_SIZE      (18)
/*********************************
 * Bitmap Structure
 * Label: ballSquare
 * Description:  8x8 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH ballSquare;
#define ballSquare_WIDTH     (8)
#define ballSquare_HEIGHT    (8)
#define ballSquare_SIZE      (18)
/*********************************
 * Bitmap Structure
 * Label: ballTriangle
 * Description:  8x8 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH ballTriangle;
#define ballTriangle_WIDTH     (8)
#define ballTriangle_HEIGHT    (8)
#define ballTriangle_SIZE      (18)
/*********************************
 * Bitmap Structure
 * Label: iconFolderSmall
 * Description:  12x10 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH iconFolderSmall;
#define iconFolderSmall_WIDTH     (12)
#define iconFolderSmall_HEIGHT    (10)
#define iconFolderSmall_SIZE      (30)
/*********************************
 * Bitmap Structure
 * Label: iconMicrochip
 * Description:  16x16 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH iconMicrochip;
#define iconMicrochip_WIDTH     (16)
#define iconMicrochip_HEIGHT    (16)
#define iconMicrochip_SIZE      (42)
/*********************************
 * Bitmap Structure
 * Label: intro
 * Description:  128x64 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH intro;
#define intro_WIDTH     (128)
#define intro_HEIGHT    (64)
#define intro_SIZE      (1034)
/*********************************
 * Bitmap Structure
 * Label: introDevice
 * Description:  128x64 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH introDevice;
#define introDevice_WIDTH     (128)
#define introDevice_HEIGHT    (64)
#define introDevice_SIZE      (1034)
/*****************************************************************************
 * SECTION:  Fonts
 *****************************************************************************/

/*********************************
 * FNT Font File Structure
 * Label: GOLFontDefault
 * Description:  Height: 14 pixels, range: '' to ''
 ***********************************/
extern const FONT_FLASH GOLFontDefault;
#define GOLFontDefault_SIZE    (1892)
#endif

