/*********************************************************************
 * Module for Microchip Graphics Library
 * This file contains compile time options for the Graphics Library.
 *********************************************************************
 * FileName:        none
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC24/PIC30/PIC32
 * Compiler:        C30 V3.00/C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright � 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok     10/28/2007
 ********************************************************************/

#ifndef _GRAPHICSCONFIG_H
#define _GRAPHICSCONFIG_H

//////////////////// COMPILE OPTIONS AND DEFAULTS ////////////////////

/*********************************************************************
* Overview: Hardware profile.
*
*********************************************************************/
#include "HardwareProfile.h"


//#define USE_GOL

/*********************************************************************
* Overview:
*   Graphics PICtail Plus Board Version setting. The value assigned
*   to this macro determines the version of the PICtail Plus Board.
*   - 1 - Uses Graphics PICtail Plus Board Version 1
*   - 2 - Uses Graphics PICtail Plus Board Version 2
*
********************************************************************/
//#define GRAPHICS_PICTAIL_VERSION   1

/*********************************************************************
* Overview: Portrait orientation for LCD and touch screen. When this
*           is not defined, setting defaults to landscape orientation.
*
********************************************************************/
//#define USE_PORTRAIT

/*********************************************************************
* Overview: Defines to use monochrome graphics mode. When this is
*           defined, only two colors are defined to be used.
*           BLACK and WHITE.
*
********************************************************************/
//#define USE_MONOCHROME

/*********************************************************************
* Overview: Blocking and Non-Blocking configuration selection. To
*           enable non-blocking configuration USE_NONBLOCKING_CONFIG
*           must be defined. If this is not defined, blocking
*           configuration is assumed.
*
********************************************************************/
//#define USE_NONBLOCKING_CONFIG // Comment this line to use blocking configuration


/*********************************************************************
* Overview: Keyboard control on some objects can be used by enabling
*           the GOL Focus (USE_FOCUS)support.
*
*********************************************************************/
//#define USE_FOCUS

/*********************************************************************
* Overview: Input devices used defines the messages that Objects will
*           process. The following definitions indicate the usage of
*           the different input device:
*           - USE_TOUCHSCREEN - enables the touch screen support.
*           - USE_KEYBOARD - enables the key board support.
*
*********************************************************************/
//#define USE_TOUCHSCREEN         // Enable touch screen support.
//#define USE_KEYBOARD          // Enable key board support.


//#define USE_MOUSE

/*********************************************************************
* Overview: To save program memory, unused Widgets or Objects can be
*           removed at compile time.
*
*********************************************************************/
//#define USE_BUTTON              // Enable Button Object.
//#define USE_WINDOW                // Enable Window Object.
//#define USE_CHECKBOX          // Enable Checkbox Object.
//#define USE_RADIOBUTTON           // Enable Radio Button Object.
//#define USE_EDITBOX               // Enable Edit Box Object.
//#define USE_LISTBOX             // Enable List Box Object.
//#define USE_SLIDER                // Enable Slider or Scroll Bar Object.
//#define USE_PROGRESSBAR           // Enable Progress Bar Object.
//#define USE_STATICTEXT            // Enable Static Text Object.
//#define USE_PICTURE             // Enable Picture Object.
//#define USE_GROUPBOX          // Enable Group Box Object.
//#define USE_ROUNDDIAL         // Enable Dial Object.
//#define USE_METER             // Enable Meter Object.
//#define USE_CUSTOM                // Enable Custom Control Object (an example to create customized Object).

/*********************************************************************
* Overview: To enable support for unicode fonts, USE_MULTIBYTECHAR
*           must be defined. This changes XCHAR definition. See XCHAR
*           for details.
*
*********************************************************************/
//#define USE_MULTIBYTECHAR

/*********************************************************************
* Overview: Font data can be placed in two locations. One is in
*           FLASH memory and the other is from external memory.
*           Definining one or both enables the support for fonts located
*           in internal flash and external memory.
*   - USE_FONT_FLASH - Font in internal flash memory support.
*   - USE_FONT_EXTERNAL - Font in external memory support.
*
*********************************************************************/
//#define USE_FONT_FLASH          // Support for fonts located in internal flash
//#define USE_FONT_EXTERNAL     // Support for fonts located in external memory

/*********************************************************************
* Overview: Similar to Font data bitmaps can also be placed in
*           two locations. One is in FLASH memory and the other is
*           from external memory.
*           Definining one or both enables the support for bitmaps located
*           in internal flash and external memory.
*   - USE_BITMAP_FLASH - Font in internal flash memory support.
*   - USE_BITMAP_EXTERNAL - Font in external memory support.
*
*********************************************************************/
#define USE_BITMAP_FLASH        // Support for bitmaps located in internal flash
//#define USE_BITMAP_EXTERNAL       // Support for bitmaps located in external memory

/*********************************************************************
* Overview: LCD driver.
*
********************************************************************/

//#include "Graphics/LGDP4531_R61505_S6D0129_S6D0139_SPFD5408.h"
//#include "Graphics/SSD1906.h"   // Driver for Solomon Systech. SSD1906 controller
//#include "Graphics/SSD1303.h"   // Driver for SSD1303 (monochrome OLED display) controller
//#include "Graphics/SH1101A.h"   // Driver for the HS1101A (monochrome OLED display) controller
//#include "Graphics/HIT1270L.h"  // Landscape driver for Densitron HIT1270 controller
//#include "Graphics/SSD1339.h"   // Driver for Solomon Systech. SSD1339 controller
#define DISPLAY_CONTROLLER NO_CONTROLLER_DEFINED

#define COLOR_DEPTH 1

#endif // _GRAPHICSCONFIG_H
