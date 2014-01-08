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
#include "Graphics/GOL.h"

/*****************************************************************************
 * This defines a default GOL scheme that gets populated when an application
 * calls the GOLCreateScheme().  The application is able to define a default 
 * GOL scheme in GraphicsConfig header file.
 *****************************************************************************/
const GOL_SCHEME GOLSchemeDefault =
{
    61,                               // Emboss dark color used for 3d effect.
    62,                               // Emboss light color used for 3d effect.
    63,                               // Character color 0 used for objects that supports text.
    21,                               // Character color 1 used for objects that supports text.
    45,                               // Character color used when object is in a disabled state.
    50,                               // Color 0 usually assigned to an Object state.
    65,                               // Color 1 usually assigned to an Object state.
    66,                               // Color used when an Object is in a disabled state.
    67,                               // Background color used to hide Objects.
    (void *)&FONTDEFAULT,             // Font selected for the scheme.
};

