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
#include "GraphicsConfig.h"
#ifndef GFX_SCHEMEDEFAULT

#include "Graphics/GOL.h"

#ifdef USE_GOL
/*****************************************************************************
 * Overview: This defines a default GOL scheme that gets populated when an 
 *           application calls the GOLCreateScheme().  The application can 
 *           override this definition by defining the macro GFX_SCHEMEDEFAULT 
 *           in the GraphicsConfig.h header file and defining GOLSchemeDefault 
 *           structure in the application code. It is important to use the same
 *           structure name since the library assumes that this object exists 
 *           and assigns the default style scheme pointer to this object.
 *****************************************************************************/
const GOL_SCHEME GOLSchemeDefault =
{
#if (COLOR_DEPTH == 1)
    BLACK,                              // Emboss dark color used for 3d effect.
    WHITE,                              // Emboss light color used for 3d effect.
    WHITE,                              // Character color 0 used for objects that supports text.
    BLACK,                              // Character color 1 used for objects that supports text.
    WHITE,                              // Character color used when object is in a disabled state.
    BLACK,                              // Color 0 usually assigned to an Object state.
    WHITE,                              // Color 1 usually assigned to an Object state.
    BLACK,                              // Color used when an Object is in a disabled state.
    BLACK,                              // Background color used to hide Objects.
#elif (COLOR_DEPTH == 4)
    GRAY006,                            // Emboss dark color used for 3d effect.
    GRAY010,                            // Emboss light color used for 3d effect.
    WHITE,                              // Character color 0 used for objects that supports text.
    BLACK,                              // Character color 1 used for objects that supports text.
    GRAY012,                            // Character color used when object is in a disabled state.
    GRAY008,                            // Color 0 usually assigned to an Object state.
    GRAY004,                            // Color 1 usually assigned to an Object state.
    GRAY014,                            // Color used when an Object is in a disabled state.
    BLACK,                              // Background color used to hide Objects.
#else    
    RGBConvert(0x2B, 0x55, 0x87),       // Emboss dark color used for 3d effect.
    RGBConvert(0xD4, 0xE4, 0xF7),       // Emboss light color used for 3d effect.
    RGBConvert(0x07, 0x1E, 0x48),       // Character color 0 used for objects that supports text.
    RGBConvert(0xFF, 0xFF, 0xFF),       // Character color 1 used for objects that supports text.
    RGBConvert(245, 245, 220),          // Character color used when object is in a disabled state.
    RGBConvert(0xA9, 0xDB, 0xEF),       // Color 0 usually assigned to an Object state.
    RGBConvert(0x26, 0xC7, 0xF2),       // Color 1 usually assigned to an Object state.
    RGBConvert(0xB6, 0xD2, 0xFB),       // Color used when an Object is in a disabled state.
    RGBConvert(0xD4, 0xED, 0xF7),       // Background color used to hide Objects.
#endif
    (void *)&FONTDEFAULT,               // Font selected for the scheme.

#if defined USE_ALPHABLEND_LITE || defined USE_ALPHABLEND
    100,                                // Alpha value used (percent based)
#endif

#ifdef USE_GRADIENT

#ifdef USE_PALETTE
    #error "The USE_GRADIENT feature is not currently supported when USE_PALETTE is enabled."
#endif

    {
        GRAD_NONE,                      // Gradient type
        RGBConvert(0xA9, 0xDB, 0xEF),   // Starting color
        RGBConvert(0x26, 0xC7, 0xF2),   // Ending color
        50                              // percentage of the object length
    }
#endif
};

#endif
#endif
