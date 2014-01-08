/*****************************************************************************
 *  Default scheme for the GOL
 *  GOL Layer 
 *****************************************************************************
 * FileName:        GOLSchemeDefault.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
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
 * Section: Includes
 *****************************************************************************/
#include "Graphics/GOL.h"
#include "gfx_font_resources.h"

/*****************************************************************************
 * Overview: This defines a default GOL scheme that gets populated when an 
 *           application calls the GOLCreateScheme().  The application can 
 *           override this definition by defining the macro GFX_SCHEMEDEFAULT 
 *           in the GraphicsConfig.h header file and defining GOLSchemeDefault 
 *           structure in the application code. It is important to use the same
 *           structure name since the library assumes that this object exists 
 *           and assigns the default style scheme pointer to this object.
 *****************************************************************************/
const GOL_SCHEME GOLScheme =
{
    0x00,                               // Emboss dark color used for 3d effect.
    0x0C,                               // Emboss light color used for 3d effect.
    0x00,                               // Character color 0 used for objects that supports text.
    0x0F,                               // Character color 1 used for objects that supports text.
    0x0F,                               // Character color used when object is in a disabled state.
    0x0B,                               // Color 0 usually assigned to an Object state.
    0x02,                               // Color 1 usually assigned to an Object state.
    0x00,                               // Color used when an Object is in a disabled state.
    0x0F,                               // Background color used to hide Objects.
    (void *)&FONTDEFAULT,               // Font selected for the scheme.
};

const GOL_SCHEME chartScheme =
{
    0x00,                               // Emboss dark color used for 3d effect.
    0x04,                               // Emboss light color used for 3d effect.
    0x01,                               // Character color 0 used for objects that supports text.
    0x02,                               // Character color 1 used for objects that supports text.
    0x0F,                               // Character color used when object is in a disabled state.
    0x00,                               // Color 0 usually assigned to an Object state.
    0x02,                               // Color 1 usually assigned to an Object state.
    0x00,                               // Color used when an Object is in a disabled state.
    0x0F,                               // Background color used to hide Objects.
    (void *)&DroidSans_12,               // Font selected for the scheme.
};
