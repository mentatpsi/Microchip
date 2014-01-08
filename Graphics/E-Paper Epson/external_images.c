/*****************************************************************************
 * FileName:        external_images.c
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
 * SECTION:  BITMAPS
 *****************************************************************************/

/*********************************
 * Bitmap Structure
 * Label: mchp_epson_logo_4bpp
 * Description:  319x58 pixels, 4-bits per pixel
 ***********************************/
const IMAGE_EXTERNAL mchp_epson_logo_4bpp = 
{
    (EXTERNAL | IMAGE_MBITMAP | COMP_NONE),
    0x0000,
    0x00000000
};
/*********************************
 * Bitmap Structure
 * Label: boy_on_hill
 * Description:  268x198 pixels, 4-bits per pixel
 ***********************************/
const IMAGE_EXTERNAL boy_on_hill = 
{
    (EXTERNAL | IMAGE_MBITMAP | COMP_NONE),
    0x0000,
    0x00002470
};
/*********************************
 * Bitmap Structure
 * Label: MachuPichu
 * Description:  268x198 pixels, 4-bits per pixel
 ***********************************/
const IMAGE_EXTERNAL MachuPichu = 
{
    (EXTERNAL | IMAGE_MBITMAP | COMP_NONE),
    0x0000,
    0x00008C40
};
/*********************************
 * Bitmap Structure
 * Label: parots
 * Description:  268x198 pixels, 4-bits per pixel
 ***********************************/
const IMAGE_EXTERNAL parots = 
{
    (EXTERNAL | IMAGE_MBITMAP | COMP_NONE),
    0x0000,
    0x0000F410
};
/*********************************
 * Bitmap Structure
 * Label: epson_qr
 * Description:  135x135 pixels, 1-bits per pixel
 ***********************************/
const IMAGE_EXTERNAL epson_qr = 
{
    (EXTERNAL | IMAGE_MBITMAP | COMP_NONE),
    0x0000,
    0x00015BE0
};
/*********************************
 * Bitmap Structure
 * Label: mchp_graphics_qr
 * Description:  175x175 pixels, 1-bits per pixel
 ***********************************/
const IMAGE_EXTERNAL mchp_graphics_qr = 
{
    (EXTERNAL | IMAGE_MBITMAP | COMP_NONE),
    0x0000,
    0x000164F0
};
/*********************************
 * Bitmap Structure
 * Label: mouse_cursor_icon_270
 * Description:  32x32 pixels, 4-bits per pixel
 ***********************************/
const IMAGE_EXTERNAL mouse_cursor_icon_270 = 
{
    (EXTERNAL | IMAGE_MBITMAP | COMP_NONE),
    0x0000,
    0x00017410
};
