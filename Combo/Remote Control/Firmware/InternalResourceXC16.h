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
 * SECTION:  Fonts
 *****************************************************************************/

/*********************************
 * TTF Font File Structure
 * Label: DroidSansMono_11
 * Description:  Height: 15 pixels, 1 bit per pixel, Range: ' ' to '~'
 ***********************************/
extern const FONT_FLASH DroidSansMono_11;
#define DroidSansMono_11_SIZE    (1813)
/*********************************
 * TTF Font File Structure
 * Label: DroidSansMono_22
 * Description:  Height: 30 pixels, 1 bit per pixel, Range: ' ' to '~'
 ***********************************/
extern const FONT_FLASH DroidSansMono_22;
#define DroidSansMono_22_SIZE    (6088)
/*********************************
 * TTF Font File Structure
 * Label: DroidSans_18
 * Description:  Height: 22 pixels, 1 bit per pixel, Range: ' ' to '~'
 ***********************************/
extern const FONT_FLASH DroidSans_18;
#define DroidSans_18_SIZE    (3930)
/*********************************
 * TTF Font File Structure
 * Label: DroidSans_Bold_14
 * Description:  Height: 17 pixels, 1 bit per pixel, Range: ' ' to '~'
 ***********************************/
extern const FONT_FLASH DroidSans_Bold_14;
#define DroidSans_Bold_14_SIZE    (2530)
/*********************************
 * TTF Font File Structure
 * Label: DroidSansMono_36
 * Description:  Height: 48 pixels, 1 bit per pixel, Range: '+' to '9'
 ***********************************/
extern const FONT_FLASH DroidSansMono_36;
#define DroidSansMono_36_SIZE    (2228)
/*****************************************************************************
 * SECTION:  Palettes
 *****************************************************************************/

/*********************************
 * PALETTE Structure
 * Label: RemoteControlPalette
 * Description:  256 Colors
 ***********************************/
extern const PALETTE_FLASH RemoteControlPalette;
#define RemoteControlPalette_SIZE    (512)
#endif

