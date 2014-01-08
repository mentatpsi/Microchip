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
 * void LoadEpsonQRCode(void)
 *****************************************************************************/
void LoadQRCode(void)
{	
    static WORD qrCode = 0;
    WORD width;
    WORD height;

	qrCode++;

    if(qrCode & 0x01)
    {
		width = GetMaxX() - epson_qr_WIDTH;
        width >>= 1;

        height = (GetMaxY() - 50) - epson_qr_HEIGHT;
        height >>= 1;
        height += 50;

        PutImage(width, height, (void *)&epson_qr, IMAGE_NORMAL);
    }else
	{
		width = GetMaxX() - mchp_graphics_qr_WIDTH;
        width >>= 1;

        height = (GetMaxY() - 50) - mchp_graphics_qr_HEIGHT;
        height >>= 1;
        height += 50;

        PutImage(width, height, (void *)&mchp_graphics_qr, IMAGE_NORMAL);
    }
}
