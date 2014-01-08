/*****************************************************************************
 * FileName:        ExternalResource.h
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

#ifndef EXTERNALRESOURCE_H_FILE
#define EXTERNALRESOURCE_H_FILE
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
#define GRC_CRC32_EXTERNAL_MARKER 0xA20F6E95ul
#define GRC_CRC32_EXTERNAL_ADDR 0x00015B40ul

/*****************************************************************************
 * SECTION:  BITMAPS
 *****************************************************************************/

/*********************************
 * Bitmap Structure
 * Label: icon5
 * Description:  50x50 pixels, 16-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER icon5;
#define icon5_WIDTH     (50)
#define icon5_HEIGHT    (50)
#define icon5_SIZE      (5006)
/*********************************
 * Bitmap Structure
 * Label: icon5_large
 * Description:  70x70 pixels, 16-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER icon5_large;
#define icon5_large_WIDTH     (70)
#define icon5_large_HEIGHT    (70)
#define icon5_large_SIZE      (9806)
/*********************************
 * Bitmap Structure
 * Label: icon6
 * Description:  50x50 pixels, 16-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER icon6;
#define icon6_WIDTH     (50)
#define icon6_HEIGHT    (50)
#define icon6_SIZE      (5006)
/*********************************
 * Bitmap Structure
 * Label: icon6_large
 * Description:  70x70 pixels, 16-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER icon6_large;
#define icon6_large_WIDTH     (70)
#define icon6_large_HEIGHT    (70)
#define icon6_large_SIZE      (9806)
/*********************************
 * Bitmap Structure
 * Label: icon1
 * Description:  50x50 pixels, 16-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER icon1;
#define icon1_WIDTH     (50)
#define icon1_HEIGHT    (50)
#define icon1_SIZE      (5006)
/*********************************
 * Bitmap Structure
 * Label: icon1_large
 * Description:  70x70 pixels, 16-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER icon1_large;
#define icon1_large_WIDTH     (70)
#define icon1_large_HEIGHT    (70)
#define icon1_large_SIZE      (9806)
/*********************************
 * Bitmap Structure
 * Label: icon2
 * Description:  50x50 pixels, 16-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER icon2;
#define icon2_WIDTH     (50)
#define icon2_HEIGHT    (50)
#define icon2_SIZE      (5006)
/*********************************
 * Bitmap Structure
 * Label: icon2_large
 * Description:  70x70 pixels, 16-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER icon2_large;
#define icon2_large_WIDTH     (70)
#define icon2_large_HEIGHT    (70)
#define icon2_large_SIZE      (9806)
/*********************************
 * Bitmap Structure
 * Label: icon3
 * Description:  50x50 pixels, 16-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER icon3;
#define icon3_WIDTH     (50)
#define icon3_HEIGHT    (50)
#define icon3_SIZE      (5006)
/*********************************
 * Bitmap Structure
 * Label: icon3_large
 * Description:  70x70 pixels, 16-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER icon3_large;
#define icon3_large_WIDTH     (70)
#define icon3_large_HEIGHT    (70)
#define icon3_large_SIZE      (9806)
/*********************************
 * Bitmap Structure
 * Label: icon4
 * Description:  50x50 pixels, 16-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER icon4;
#define icon4_WIDTH     (50)
#define icon4_HEIGHT    (50)
#define icon4_SIZE      (5006)
/*********************************
 * Bitmap Structure
 * Label: icon4_large
 * Description:  70x70 pixels, 16-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER icon4_large;
#define icon4_large_WIDTH     (70)
#define icon4_large_HEIGHT    (70)
#define icon4_large_SIZE      (9806)
#endif

