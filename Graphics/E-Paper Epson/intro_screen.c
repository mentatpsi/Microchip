/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        main_screen.c
 * Dependencies:    See Includes Section
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 *
 * Copyright © 2011 Microchip Technology Inc.  All rights reserved.
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
 *****************************************************************************/

/*****************************************************************************
 * SECTION: Includes
 *****************************************************************************/
#include "Graphics/Graphics.h"
#include "gfx_font_resources.h"
#include "MainDemo.h"

#ifdef USE_BITMAP_EXTERNAL
#include "external_images.h"
#endif

#ifdef USE_BITMAP_FLASH
#ifdef __PIC32MX__
#include "internal_images_C32.h"
#else
#include "internal_images_C30.h"
#endif
#endif
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define size_x 320
#define size_y 240 // place holders

#if(COLOR_DEPTH == 8)
#define TEXT_COLOR_1    0x20
#define TEXT_COLOR_2    0x10
#elif (COLOR_DEPTH == 4)
#define TEXT_COLOR_1    0x02
#define TEXT_COLOR_2    0x00
#else
#define TEXT_COLOR_1    0x01
#define TEXT_COLOR_2    0x00
#endif
/*****************************************************************************
 * void LoadIntroScreen(void)
 *****************************************************************************/
void LoadIntroScreen(void)
{
    SHORT width, height;

    PutImage(0, 0, (void *)&mchp_epson_logo_4bpp, IMAGE_NORMAL);
    height = GetImageHeight((void *)&mchp_epson_logo_4bpp) + 25;

    SetFont((void *) &Arimo_Regular_Bold_36);
    SetColor(TEXT_COLOR_1);
    width = GetTextWidth((XCHAR *)driverStr, (void *) &Arimo_Regular_Bold_36);

    OutTextXY((size_x - width) >> 1, height, (XCHAR *)driverStr);
    height += GetTextHeight((void *) &Arimo_Regular_Bold_36) + 10;

    width = GetTextWidth((XCHAR *)descriptionStr, (void *) &Arimo_Regular_Bold_36);
    OutTextXY((size_x - width) >> 1, height, (XCHAR *)descriptionStr);

    height += GetTextHeight((void *) &Arimo_Regular_Bold_36) + 10;

    SetFont((void *) &DroidSans_24);
    SetColor(TEXT_COLOR_2);
    width = GetTextWidth("Press Any Button...", (void *) &DroidSans_24);
   	OutTextXY((size_x - width) >> 1, height, "Press Any Button...");

}
