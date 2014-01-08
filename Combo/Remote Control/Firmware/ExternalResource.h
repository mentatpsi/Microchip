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
#define GRC_CRC32_EXTERNAL_MARKER 0xE0322CAEul
#define GRC_CRC32_EXTERNAL_ADDR 0x00024130ul

/*****************************************************************************
 * SECTION:  BITMAPS
 *****************************************************************************/

/*********************************
 * Bitmap Structure
 * Label: Favorites
 * Description:  80x79 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL Favorites;
#define Favorites_WIDTH     (80)
#define Favorites_HEIGHT    (79)
#define Favorites_SIZE      (6838)
/*********************************
 * Bitmap Structure
 * Label: Home
 * Description:  80x79 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL Home;
#define Home_WIDTH     (80)
#define Home_HEIGHT    (79)
#define Home_SIZE      (6838)
/*********************************
 * Bitmap Structure
 * Label: Play_DiscPressed
 * Description:  80x79 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL Play_DiscPressed;
#define Play_DiscPressed_WIDTH     (80)
#define Play_DiscPressed_HEIGHT    (79)
#define Play_DiscPressed_SIZE      (6838)
/*********************************
 * Bitmap Structure
 * Label: SettingsPressed
 * Description:  80x79 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL SettingsPressed;
#define SettingsPressed_WIDTH     (80)
#define SettingsPressed_HEIGHT    (79)
#define SettingsPressed_SIZE      (6838)
/*********************************
 * Bitmap Structure
 * Label: HomeIconSmallPressed
 * Description:  60x59 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL HomeIconSmallPressed;
#define HomeIconSmallPressed_WIDTH     (60)
#define HomeIconSmallPressed_HEIGHT    (59)
#define HomeIconSmallPressed_SIZE      (4058)
/*********************************
 * Bitmap Structure
 * Label: button7
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button7;
#define button7_WIDTH     (55)
#define button7_HEIGHT    (55)
#define button7_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button7press
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button7press;
#define button7press_WIDTH     (55)
#define button7press_HEIGHT    (55)
#define button7press_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button2
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button2;
#define button2_WIDTH     (55)
#define button2_HEIGHT    (55)
#define button2_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button4
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button4;
#define button4_WIDTH     (55)
#define button4_HEIGHT    (55)
#define button4_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button3
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button3;
#define button3_WIDTH     (55)
#define button3_HEIGHT    (55)
#define button3_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button2press
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button2press;
#define button2press_WIDTH     (55)
#define button2press_HEIGHT    (55)
#define button2press_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button6
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button6;
#define button6_WIDTH     (55)
#define button6_HEIGHT    (55)
#define button6_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button1
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button1;
#define button1_WIDTH     (55)
#define button1_HEIGHT    (55)
#define button1_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button4press
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button4press;
#define button4press_WIDTH     (55)
#define button4press_HEIGHT    (55)
#define button4press_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button6press
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button6press;
#define button6press_WIDTH     (55)
#define button6press_HEIGHT    (55)
#define button6press_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button5press
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button5press;
#define button5press_WIDTH     (55)
#define button5press_HEIGHT    (55)
#define button5press_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button3press
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button3press;
#define button3press_WIDTH     (55)
#define button3press_HEIGHT    (55)
#define button3press_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button1press
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button1press;
#define button1press_WIDTH     (55)
#define button1press_HEIGHT    (55)
#define button1press_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: button5
 * Description:  55x55 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL button5;
#define button5_WIDTH     (55)
#define button5_HEIGHT    (55)
#define button5_SIZE      (3543)
/*********************************
 * Bitmap Structure
 * Label: HomePressed
 * Description:  80x79 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL HomePressed;
#define HomePressed_WIDTH     (80)
#define HomePressed_HEIGHT    (79)
#define HomePressed_SIZE      (6838)
/*********************************
 * Bitmap Structure
 * Label: TV
 * Description:  80x79 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL TV;
#define TV_WIDTH     (80)
#define TV_HEIGHT    (79)
#define TV_SIZE      (6838)
/*********************************
 * Bitmap Structure
 * Label: TVPressed
 * Description:  80x79 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL TVPressed;
#define TVPressed_WIDTH     (80)
#define TVPressed_HEIGHT    (79)
#define TVPressed_SIZE      (6838)
/*********************************
 * Bitmap Structure
 * Label: InternetPressed
 * Description:  80x79 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL InternetPressed;
#define InternetPressed_WIDTH     (80)
#define InternetPressed_HEIGHT    (79)
#define InternetPressed_SIZE      (6838)
/*********************************
 * Bitmap Structure
 * Label: FavoritesPressed
 * Description:  80x79 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL FavoritesPressed;
#define FavoritesPressed_WIDTH     (80)
#define FavoritesPressed_HEIGHT    (79)
#define FavoritesPressed_SIZE      (6838)
/*********************************
 * Bitmap Structure
 * Label: Smaller_FavoritesPressed
 * Description:  50x50 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL Smaller_FavoritesPressed;
#define Smaller_FavoritesPressed_WIDTH     (50)
#define Smaller_FavoritesPressed_HEIGHT    (50)
#define Smaller_FavoritesPressed_SIZE      (3018)
/*********************************
 * Bitmap Structure
 * Label: Play_Disc
 * Description:  80x79 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL Play_Disc;
#define Play_Disc_WIDTH     (80)
#define Play_Disc_HEIGHT    (79)
#define Play_Disc_SIZE      (6838)
/*********************************
 * Bitmap Structure
 * Label: Smaller_Favorites
 * Description:  50x50 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL Smaller_Favorites;
#define Smaller_Favorites_WIDTH     (50)
#define Smaller_Favorites_HEIGHT    (50)
#define Smaller_Favorites_SIZE      (3018)
/*********************************
 * Bitmap Structure
 * Label: Internet
 * Description:  80x79 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL Internet;
#define Internet_WIDTH     (80)
#define Internet_HEIGHT    (79)
#define Internet_SIZE      (6838)
/*********************************
 * Bitmap Structure
 * Label: HomeIconSmall
 * Description:  60x59 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL HomeIconSmall;
#define HomeIconSmall_WIDTH     (60)
#define HomeIconSmall_HEIGHT    (59)
#define HomeIconSmall_SIZE      (4058)
/*********************************
 * Bitmap Structure
 * Label: Settings
 * Description:  80x79 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL Settings;
#define Settings_WIDTH     (80)
#define Settings_HEIGHT    (79)
#define Settings_SIZE      (6838)
/*********************************
 * Bitmap Structure
 * Label: MICH4CBsmall
 * Description:  35x33 pixels, 8-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL MICH4CBsmall;
#define MICH4CBsmall_WIDTH     (35)
#define MICH4CBsmall_HEIGHT    (33)
#define MICH4CBsmall_SIZE      (1673)
#endif

