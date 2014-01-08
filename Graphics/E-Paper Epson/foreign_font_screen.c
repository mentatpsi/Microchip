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

/*****************************************************************************
 * void LoadFontScreen(void)
 *****************************************************************************/
void LoadForeignFontScreen(void)
{
    SHORT width, height;    

    height = 50;
    SetColor(GRAY002);
    SetFont((void *) &fireflysung_22);
    width = GetTextWidth((XCHAR *)ChineseStr, (void *) &fireflysung_22);
    OutTextXY((GetMaxX() - width) >> 1, height, (XCHAR *)ChineseStr);
    height += GetTextHeight((void *) &fireflysung_22) + 5;

    width = GetTextWidth((XCHAR *)JapaneseStr, (void *) &fireflysung_22);
    OutTextXY((GetMaxX() - width) >> 1, height, (XCHAR *)JapaneseStr);
    height += GetTextHeight((void *) &fireflysung_22) + 5;

    SetFont((void *) &UnBatang_0613_22);
    width = GetTextWidth((XCHAR *)KoreanStr, (void *) &UnBatang_0613_22);
    OutTextXY((GetMaxX() - width) >> 1, height, (XCHAR *)KoreanStr);
    height += GetTextHeight((void *) &UnBatang_0613_22) + 5;

    width = GetTextWidth((XCHAR *)RussianStr, (void *) &UnBatang_0613_22);
    OutTextXY((GetMaxX() - width) >> 1, height, (XCHAR *)RussianStr);
    height += GetTextHeight((void *) &UnBatang_0613_22) + 5;

    SetFont((void *) &DroidSans_24);
    width = GetTextWidth("Hello World!", (void *) &DroidSans_24);
    OutTextXY((GetMaxX() - width) >> 1, height, "Hello World!");
}
