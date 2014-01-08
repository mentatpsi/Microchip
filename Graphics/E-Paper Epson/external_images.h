/*****************************************************************************
 * FileName:        external_images.h
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

#ifndef EXTERNAL_IMAGES_H_FILE
#define EXTERNAL_IMAGES_H_FILE
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
#ifndef USE_BITMAP_NO_PADDING_LINE
#error These bitmap resources have been converted with no padding of the horizontal lines, but USE_BITMAP_NO_PADDING_LINE is not defined in GraphicsConfig.h.
#endif


/*****************************************************************************
 * This is an error check for the color depth
 *****************************************************************************/
#if (COLOR_DEPTH > 16)
#error "Color Depth needs to be 16 to correctly use these resources"
#endif


/*****************************************************************************
 * This is a unique marker to verify the external memory data
 * The marker is a total of 8 bytes in length starting at the location GRC_CRC32_EXTERNAL_ADDR.
 * The first four bytes, starting at GRC_CRC32_EXTERNAL_ADDR, will always equal 'M', 'C', 'H', 'P'.
 * The next four byte is the generated 32-bit CRC.  The application can compare the value read from the
 * external memory to the value of GRC_CRC32_EXTERNAL_MARKER to verify that the data is valid.
 *****************************************************************************/
#define GRC_CRC32_EXTERNAL_MARKER 0xCF056303ul
#define GRC_CRC32_EXTERNAL_ADDR 0x00017640ul

/*****************************************************************************
 * SECTION:  BITMAPS
 *****************************************************************************/

/*********************************
 * Bitmap Structure
 * Label: mchp_epson_logo_4bpp
 * Description:  319x58 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL mchp_epson_logo_4bpp;
#define mchp_epson_logo_4bpp_WIDTH     (319)
#define mchp_epson_logo_4bpp_HEIGHT    (58)
#define mchp_epson_logo_4bpp_SIZE      (9318)
/*********************************
 * Bitmap Structure
 * Label: boy_on_hill
 * Description:  268x198 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL boy_on_hill;
#define boy_on_hill_WIDTH     (268)
#define boy_on_hill_HEIGHT    (198)
#define boy_on_hill_SIZE      (26570)
/*********************************
 * Bitmap Structure
 * Label: MachuPichu
 * Description:  268x198 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL MachuPichu;
#define MachuPichu_WIDTH     (268)
#define MachuPichu_HEIGHT    (198)
#define MachuPichu_SIZE      (26570)
/*********************************
 * Bitmap Structure
 * Label: parots
 * Description:  268x198 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL parots;
#define parots_WIDTH     (268)
#define parots_HEIGHT    (198)
#define parots_SIZE      (26570)
/*********************************
 * Bitmap Structure
 * Label: epson_qr
 * Description:  135x135 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL epson_qr;
#define epson_qr_WIDTH     (135)
#define epson_qr_HEIGHT    (135)
#define epson_qr_SIZE      (2305)
/*********************************
 * Bitmap Structure
 * Label: mchp_graphics_qr
 * Description:  175x175 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL mchp_graphics_qr;
#define mchp_graphics_qr_WIDTH     (175)
#define mchp_graphics_qr_HEIGHT    (175)
#define mchp_graphics_qr_SIZE      (3860)
/*********************************
 * Bitmap Structure
 * Label: mouse_cursor_icon_270
 * Description:  32x32 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL mouse_cursor_icon_270;
#define mouse_cursor_icon_270_WIDTH     (32)
#define mouse_cursor_icon_270_HEIGHT    (32)
#define mouse_cursor_icon_270_SIZE      (550)
#endif

