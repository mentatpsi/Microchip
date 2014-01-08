/*********************************************************************
 * Module for Microchip Graphics Library
 * The header file joins all header files used in the graphics library
 * and contains compile options and defaults.
 *********************************************************************
 * FileName:        Graphics.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:        MPLAB C30, MPLAB C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
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
 * Date        	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07		Version 1.0 release
 * 02/24/11     - Removed GRAPHICS CONTROLLER and DISPLAY PANEL Codes
 *              - Changed USE_CUSTOM to USE_CUSTOM_WIDGET. When custom widgets 
 *                are created and users do not want to modify files in the
 *                graphics library.
 *              - Removed the dependency of this file from HardwareProfile.h
 *              - Removed macros that defines supported controllers and
 *                display panels. Refer to Graphics Library Help file 
 *                for complete list.
 *              - added #include "gfxcolors.h"
 ********************************************************************/
#ifndef _GRAPHICS_H
    #define _GRAPHICS_H

/*************************************************
 * Some versions of the C30 compiler are known
 * to be incompatible with the Graphics Library.
 * A compile time check has been added.
 *************************************************/
#ifdef __C30_VERSION__
#if (__C30_VERSION__ == 325)
#error "C30 compiler version 3.25 is not compatible with the Graphics Library, please use a different compiler version."
#endif
#endif

/////////////////////// GRAPHICS_LIBRARY_VERSION /////////////////////
// MSB is version, LSB is subversion
    #define GRAPHICS_LIBRARY_VERSION    0x0306

////////////////////////////// INCLUDES //////////////////////////////
    #include <stdlib.h>             // needed because of malloc()
    #include "GenericTypeDefs.h"
	#include "GraphicsConfig.h"
    #include "DisplayDriver.h"      // Display Driver layer
    #include "Primitive.h"          // Graphic Primitives layer
    #include "GOL.h"                // Graphics Object layer

	#include "ScanCodes.h"          // Scan codes for AT keyboard
	#include "gfxcolors.h"          // default color definitions, can be overidden by application side
	                                // see gfxcolor.h for details 
    
    #include "Graphics/Palette.h"

    #if defined(USE_BUTTON) || defined(USE_BUTTON_MULTI_LINE)
        #include "Button.h"
    #endif
    #ifdef USE_WINDOW
        #include "Window.h"
    #endif
    #ifdef USE_GROUPBOX
        #include "GroupBox.h"
    #endif
    #ifdef USE_STATICTEXT
        #include "StaticText.h"
    #endif
    #ifdef USE_SLIDER
        #include "Slider.h"
    #endif
    #ifdef USE_CHECKBOX
        #include "CheckBox.h"
    #endif
    #ifdef USE_RADIOBUTTON
        #include "RadioButton.h"
    #endif
    #ifdef USE_PICTURE
        #include "Picture.h"
    #endif
    #ifdef USE_PROGRESSBAR
        #include "ProgressBar.h"
    #endif
    #ifdef USE_EDITBOX
        #include "EditBox.h"
    #endif
    #ifdef USE_LISTBOX
        #include "ListBox.h"
    #endif
    #ifdef USE_ROUNDDIAL
        #include "RoundDial.h"
    #endif
    #ifdef USE_METER
        #include "Meter.h"
    #endif
    #ifdef USE_GRID
        #include "Grid.h"
    #endif
    #ifdef USE_CHART
        #include "Chart.h"
    #endif
    #ifdef USE_TEXTENTRY
        #include "TextEntry.h"
    #endif
    #ifdef USE_DIGITALMETER
        #include "DigitalMeter.h"
    #endif
    #ifdef USE_ANALOGCLOCK
        #include "AnalogClock.h"
    #endif
    #ifdef USE_CUSTOM_WIDGET
        // Use this in case users wants to create a custom widget and do not want to
        // modify the Graphics Library files.
        #include "CustomControlWidget.h"
    #endif
#endif
