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
 * Label: gradientButton
 * Description:  121x45 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH gradientButton;
#define gradientButton_WIDTH     (121)
#define gradientButton_HEIGHT    (45)
#define gradientButton_SIZE      (5963)
/*********************************
 * Bitmap Structure
 * Label: Engine1
 * Description:  73x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH Engine1;
#define Engine1_WIDTH     (73)
#define Engine1_HEIGHT    (55)
#define Engine1_SIZE      (4533)
/*********************************
 * Bitmap Structure
 * Label: Engine2
 * Description:  73x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH Engine2;
#define Engine2_WIDTH     (73)
#define Engine2_HEIGHT    (55)
#define Engine2_SIZE      (4533)
/*********************************
 * Bitmap Structure
 * Label: Engine3
 * Description:  73x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH Engine3;
#define Engine3_WIDTH     (73)
#define Engine3_HEIGHT    (55)
#define Engine3_SIZE      (4533)
/*********************************
 * Bitmap Structure
 * Label: Engine4
 * Description:  73x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH Engine4;
#define Engine4_WIDTH     (73)
#define Engine4_HEIGHT    (55)
#define Engine4_SIZE      (4533)
/*********************************
 * Bitmap Structure
 * Label: intro
 * Description:  300x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH intro;
#define intro_WIDTH     (300)
#define intro_HEIGHT    (55)
#define intro_SIZE      (17018)
/*********************************
 * Bitmap Structure
 * Label: bulbon
 * Description:  120x44 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH bulbon;
#define bulbon_WIDTH     (120)
#define bulbon_HEIGHT    (44)
#define bulbon_SIZE      (5798)
/*********************************
 * Bitmap Structure
 * Label: arrowDown
 * Description:  18x26 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH arrowDown;
#define arrowDown_WIDTH     (18)
#define arrowDown_HEIGHT    (26)
#define arrowDown_SIZE      (986)
/*********************************
 * Bitmap Structure
 * Label: arrowUp
 * Description:  18x26 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH arrowUp;
#define arrowUp_WIDTH     (18)
#define arrowUp_HEIGHT    (26)
#define arrowUp_SIZE      (986)
/*********************************
 * Bitmap Structure
 * Label: bulboff
 * Description:  120x44 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH bulboff;
#define bulboff_WIDTH     (120)
#define bulboff_HEIGHT    (44)
#define bulboff_SIZE      (5798)
/*********************************
 * Bitmap Structure
 * Label: mchpIcon0
 * Description:  32x32 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH mchpIcon0;
#define mchpIcon0_WIDTH     (32)
#define mchpIcon0_HEIGHT    (32)
#define mchpIcon0_SIZE      (1542)
/*********************************
 * Bitmap Structure
 * Label: mchpIcon
 * Description:  32x32 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH mchpIcon;
#define mchpIcon_WIDTH     (32)
#define mchpIcon_HEIGHT    (32)
#define mchpIcon_SIZE      (1542)
/*********************************
 * Bitmap Structure
 * Label: redphone
 * Description:  48x35 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH redphone;
#define redphone_WIDTH     (48)
#define redphone_HEIGHT    (35)
#define redphone_SIZE      (2198)
/*********************************
 * Bitmap Structure
 * Label: greenphone
 * Description:  48x35 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_FLASH greenphone;
#define greenphone_WIDTH     (48)
#define greenphone_HEIGHT    (35)
#define greenphone_SIZE      (2198)
/*****************************************************************************
 * SECTION:  Fonts
 *****************************************************************************/

/*********************************
 * TTF Font File Structure
 * Label: GOLMediumFont
 * Description:  Height: 19 pixels, 1 bit per pixel, Range: ' ' to '~'
 * Comment Block:
 * Created from Gentium font. 
 * Gentium font is Copyright (c) 2003-2005, SIL International (http://scripts.sil.org/). 
 * All Rights Reserved. 
 * Gentium font is licensed under the SIL Open Font License, Version 1.0. 
 * http://scripts.sil.org/OFL 
 ***********************************/
extern const FONT_FLASH GOLMediumFont;
#define GOLMediumFont_SIZE    (2668)
/*********************************
 * TTF Font File Structure
 * Label: GOLSmallFont
 * Description:  Height: 16 pixels, 1 bit per pixel, Range: ' ' to '~'
 * Comment Block:
 * Created from Gentium font. 
 * Gentium font is Copyright (c) 2003-2005, SIL International (http://scripts.sil.org/). 
 * All Rights Reserved. 
 * Gentium font is licensed under the SIL Open Font License, Version 1.0. 
 * http://scripts.sil.org/OFL 
 ***********************************/
extern const FONT_FLASH GOLSmallFont;
#define GOLSmallFont_SIZE    (2052)
/*********************************
 * TTF Font File Structure
 * Label: monofont
 * Description:  Height: 20 pixels, 1 bit per pixel, Range: '.' to '9'
 * Comment Block:
 * Created from Bitstream Vera Fonts 
 *  
 * Copyright (c) 2003, Bitstream Inc. 
 * All Rights Reserved. Bitstream 
 * Vera is a trademark of Bitstream, Inc. 
 *  
 * Permission is hereby granted, free of charge, to any person obtaining 
 * a copy of the fonts accompanying this license ("Fonts") and associated 
 * documentation files (the "Font Software"), to reproduce and distribute 
 * the Font Software, including without limitation the rights to use, 
 * copy, merge, publish, distribute, and/or sell copies of the Font 
 * Software, and to permit persons to whom the Font Software is furnished 
 * to do so, subject to the following conditions: 
 *  
 * The above copyright and trademark notices and this permission notice 
 * shall be included in all copies of one or more of the Font Software 
 * typefaces. 
 *  
 * The Font Software may be modified, altered, or added to, and in 
 * particular the designs of glyphs or characters in the Fonts may be 
 * modified and additional glyphs or characters may be added to the 
 * Fonts, only if the fonts are renamed to names not containing either 
 * the words "Bitstream" or the word "Vera". 
 *  
 * This License becomes null and void to the extent applicable to Fonts 
 * or Font Software that has been modified and is distributed under the 
 * "Bitstream Vera" names. 
 *  
 * The Font Software may be sold as part of a larger software package but 
 * no copy of one or more of the Font Software typefaces may be sold by 
 * itself. 
 * 
 * THE FONT SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT 
 * OF COPYRIGHT, PATENT, TRADEMARK, OR OTHER RIGHT. IN NO EVENT SHALL 
 * BITSTREAM OR THE GNOME FOUNDATION BE LIABLE FOR ANY CLAIM, DAMAGES OR 
 * OTHER LIABILITY, INCLUDING ANY GENERAL, SPECIAL, INDIRECT, INCIDENTAL, 
 * OR CONSEQUENTIAL DAMAGES, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 * OTHERWISE, ARISING FROM, OUT OF THE USE OR INABILITY TO USE THE FONT 
 * SOFTWARE OR FROM OTHER DEALINGS IN THE FONT SOFTWARE. 
 *  
 * Except as contained in this notice, the names of Gnome, the Gnome 
 * Foundation, and Bitstream Inc., shall not be used in advertising or 
 * otherwise to promote the sale, use or other dealings in this Font 
 * Software without prior written authorization from the Gnome Foundation 
 * or Bitstream Inc., respectively. For further information, contact: 
 * fonts at gnome dot org. 
 *  
 ***********************************/
extern const FONT_FLASH monofont;
#define monofont_SIZE    (536)
/*****************************************************************************
 * SECTION:  Palettes
 *****************************************************************************/

/*********************************
 * PALETTE Structure
 * Label: GOL_Palette_Demo
 * Description:  218 Colors
 ***********************************/
extern const PALETTE_FLASH GOL_Palette_Demo;
#define GOL_Palette_Demo_SIZE    (436)
#endif

