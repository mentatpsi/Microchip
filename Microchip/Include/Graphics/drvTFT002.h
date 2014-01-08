/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  LCD controllers driver
 *  Solomon Systech. SSD1289 
 *  Solomon Systech. SSD2119 
 *****************************************************************************
 * FileName:        drvTFT002.h
 * Processor:       PIC24, PIC32
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
 * Date			Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 05/29/09		...
 * 03/22/11     Modified for for Graphics Library Version 3.00
 *              - Removed USE_DRV_xxxx switches. This is not needed anymore
 *                since Primitive Lyer implements weak attributes on 
 *                Primitive Routines that can be implemented in hardware.
 *              - Moved common APIs to DisplayDriver.h. DisplayDriver.h
 *                is now a common file that should be included in the
 *                the driver source code.
 *****************************************************************************/
#ifndef _DRVTFT002_H
    #define _DRVTFT002_H

    #include "HardwareProfile.h"
    #include "GraphicsConfig.h"
    #include "GenericTypeDefs.h"

/*********************************************************************
* Error Checking
*********************************************************************/
    #ifndef DISP_HOR_RESOLUTION
        #error DISP_HOR_RESOLUTION must be defined in HardwareProfile.h
    #endif
    #ifndef DISP_VER_RESOLUTION
        #error DISP_VER_RESOLUTION must be defined in HardwareProfile.h
    #endif
    #ifndef COLOR_DEPTH
        #error COLOR_DEPTH must be defined in GraphicsConfig.h
    #endif
    #ifndef DISP_ORIENTATION
        #error DISP_ORIENTATION must be defined in HardwareProfile.h
    #endif
    
/*********************************************************************
* Overview: Horizontal and vertical screen size.
*********************************************************************/
    #if defined (GFX_USE_DISPLAY_CONTROLLER_SSD1289)
        #if (DISP_HOR_RESOLUTION != 240)
            #error This driver doesn't supports this resolution. Horisontal resolution must be 240 pixels.
        #endif
        #if (DISP_VER_RESOLUTION != 320)
            #error This driver doesn't supports this resolution. Vertical resolution must be 320 pixels.
        #endif
    #elif defined (GFX_USE_DISPLAY_CONTROLLER_SSD2119)
        #if (DISP_HOR_RESOLUTION != 320)
            #error This driver doesn't supports this resolution. Horisontal resolution must be 320 pixels.
        #endif
        #if (DISP_VER_RESOLUTION != 240)
            #error This driver doesn't supports this resolution. Vertical resolution must be 240 pixels.
        #endif
    #endif

/*********************************************************************
* Overview: Display orientation.
*********************************************************************/
    #if (DISP_ORIENTATION != 0) && (DISP_ORIENTATION != 90)
        #error This driver doesn't support this orientation.
    #endif

/*********************************************************************
* Overview: Color depth.
*********************************************************************/
    #if (COLOR_DEPTH != 16)
        #error This driver support 16 BPP color depth only.
    #endif

/*********************************************************************
* Macros: SetPalette(colorNum, color)
*
* Overview:  Sets palette register.
*
* PreCondition: none
*
* Input: colorNum - Register number.
*        color - Color.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define SetPalette(colorNum, color)

#endif // _DRVTFT002_H
