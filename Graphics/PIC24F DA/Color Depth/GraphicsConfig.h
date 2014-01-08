/*********************************************************************
 * Module for Microchip Graphics Library
 * This file contains compile time options for the Graphics Library. 
 *********************************************************************
 * FileName:        GraphicsConfig.h
 * Dependencies:    none
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
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
 * Date            Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 12/05/2011      ...
 ********************************************************************/

#ifndef _GRAPHICSCONFIG_H
    #define _GRAPHICSCONFIG_H

//////////////////// COMPILE OPTIONS AND DEFAULTS ////////////////////

/*********************************************************************
* Overview: Blocking and Non-Blocking configuration selection. To
*			enable non-blocking configuration USE_NONBLOCKING_CONFIG
*			must be defined. If this is not defined, blocking 
*			configuration is assumed.
*
********************************************************************/
    #define USE_NONBLOCKING_CONFIG  // Comment this line to use blocking configuration

/*********************************************************************
* Overview: Using Palettes, different colors can be used with the same
*			bit depth.
*
********************************************************************/
    #define USE_PALETTE // Comment this line if Palette is not required.
    
    #ifdef USE_PALETTE
        //#define USE_PALETTE_EXTERNAL // Uncomment this line if Palette must be stored in External Memory.
    #endif

/*********************************************************************
* Overview: Input devices used defines the messages that Objects will
*			process. The following definitions indicate the usage of 
*			the different input device:
*			- USE_TOUCHSCREEN - enables the touch screen support.
*			- USE_KEYBOARD - enables the key board support.
*
*********************************************************************/
#define USE_TOUCHSCREEN			// Enable touch screen support.

/*********************************************************************
* Overview: To save program memory, unused Widgets or Objects can be 
*			removed at compile time. 
*
*********************************************************************/
#define USE_GOL             // Enable Graphics Object Layer.
#define USE_BUTTON          // Enable Button Object.
//#define USE_WINDOW			// Enable Window Object.
//#define USE_CHECKBOX			// Enable Checkbox Object.
//#define USE_RADIOBUTTON		// Enable Radio Button Object.
//#define USE_EDITBOX			// Enable Edit Box Object.
//#define USE_LISTBOX			// Enable List Box Object.
//#define USE_SLIDER            // Enable Slider or Scroll Bar Object.
//#define USE_PROGRESSBAR		// Enable Progress Bar Object.
//#define USE_STATICTEXT		// Enable Static Text Object.
#define USE_PICTURE			// Enable Picture Object.
//#define USE_GROUPBOX			// Enable Group Box Object.
//#define USE_ROUNDDIAL			// Enable Dial Object.
//#define USE_METER				// Enable Meter Object.
//#define USE_TEXTENTRY			// Enable TextEntry Object.
//#define USE_CUSTOM				// Enable Custom Control Object (an example to create customized Object).

/*********************************************************************
* Overview: To enable support for unicode fonts, USE_MULTIBYTECHAR  
*			must be defined. This changes XCHAR definition. See XCHAR 
*			for details.
*
*********************************************************************/
//#define USE_MULTIBYTECHAR

/*********************************************************************
* Overview: Font data can be placed in two locations. One is in 
*			FLASH memory and the other is from external memory.
*			Definining one or both enables the support for fonts located
*			in internal flash and external memory. 
*	- USE_FONT_FLASH - Font in internal flash memory support.	
*	- USE_FONT_EXTERNAL - Font in external memory support.	
*
*********************************************************************/
#define USE_FONT_FLASH          // Support for fonts located in internal flash
//#define USE_FONT_EXTERNAL       // Support for fonts located in external memory

/*********************************************************************
* Overview: Similar to Font data bitmaps can also be placed in 
*			two locations. One is in FLASH memory and the other is 
*			from external memory.
*			Definining one or both enables the support for bitmaps located
*			in internal flash and external memory. 
*	- USE_BITMAP_FLASH - Font in internal flash memory support.	
*	- USE_BITMAP_EXTERNAL - Font in external memory support.	
*
*********************************************************************/
#define USE_BITMAP_FLASH        // Support for bitmaps located in internal flash
//#define USE_BITMAP_EXTERNAL     // Support for bitmaps located in external memory

/*********************************************************************
* Overview: Define the malloc() and free() for versatility on OS 
*           based systems. 
*
*********************************************************************/
#define GFX_malloc(size)    	malloc(size)
#define GFX_free(pObj)    	    free(pObj)

/*********************************************************************
* Overview: Specifies the color depth used in the demo. Normally, the color
*           depth is a simple macro #define COLOR_DEPTH value.
*           For this project a special case is implemented for a single 
*           project to cover three color depths.
*           The color depth used is dependent on the contents of the 
*           included file.
*
*********************************************************************/
#ifndef CFG_INCLUDE_MPLAB_X
#define COLOR_DEPTH 8 // change to 8, 4 or 1 
#else

#if defined (CFG_INCLUDE_REMOTECONTROL_8BPP_INTERNAL_QVGA) || defined (CFG_INCLUDE_DA210_BRD_16PMP_8BPP_INTERNAL_QVGAv1) ||  \
    defined (CFG_INCLUDE_DA210_BRD_16PMP_8BPP_WQVGAv1) || defined (CFG_INCLUDE_DA210_BRD_16PMP_8BPP_INTERNAL_IPU_QVGAv1) ||  \
    defined (CFG_INCLUDE_DA210_BRD_16PMP_8BPP_IPU_WQVGAv1)
        #define COLOR_DEPTH 8
#elif defined (CFG_INCLUDE_REMOTECONTROL_4BPP_INTERNAL_QVGA) || defined (CFG_INCLUDE_DA210_BRD_16PMP_4BPP_INTERNAL_QVGAv1) || \
      defined (CFG_INCLUDE_DA210_BRD_16PMP_4BPP_INTERNAL_WQVGAv1) || defined (CFG_INCLUDE_DA210_BRD_16PMP_4BPP_INTERNAL_IPU_QVGAv1) || \
      defined (CFG_INCLUDE_DA210_BRD_16PMP_4BPP_INTERNAL_IPU_WQVGAv1)
        #define COLOR_DEPTH 4
#elif defined (CFG_INCLUDE_REMOTECONTROL_1BPP_INTERNAL_QVGA) || defined (CFG_INCLUDE_DA210_BRD_16PMP_1BPP_INTERNAL_QVGAv1) || \
      defined (CFG_INCLUDE_DA210_BRD_16PMP_1BPP_INTERNAL_WQVGAv1) || defined (CFG_INCLUDE_DA210_BRD_16PMP_1BPP_INTERNAL_IPU_QVGAv1) || \
      defined (CFG_INCLUDE_DA210_BRD_16PMP_1BPP_INTERNAL_IPU_WQVGAv1)
        #define COLOR_DEPTH 1
#endif

#endif

/*********************************************************************
* Overview: Specifies the default font to be used is the user defined
*           font. This replaces the default font supplied by the 
*           Graphics Library.
*
*********************************************************************/
#define FONTDEFAULT             (DroidSans_Bold_14)

/*********************************************************************
* Overview: To enable support for DEFLATE compressed images for PutImage(). 
*           When this macro is enabled, the PutImage() function will 
*           be able to process images generated by the Graphics Resource 
*           Converter (GRC) that are compressed using the DEFLATE algorithm.
*           PutImage() will need the IPU module of the Microchip Graphics 
*           Module to decompress. Enable this feature only when the driver
*           features the IPU module (example: PIC24FJ2456DA210).
*           Define this in GraphicsConfig.h
*
*********************************************************************/
#ifndef CFG_INCLUDE_MPLAB_X
//#define USE_COMP_IPU
#else

#if defined (CFG_INCLUDE_DA210_BRD_16PMP_8BPP_INTERNAL_IPU_QVGAv1) || defined (CFG_INCLUDE_DA210_BRD_16PMP_4BPP_INTERNAL_IPU_QVGAv1) || \
    defined (CFG_INCLUDE_DA210_BRD_16PMP_1BPP_INTERNAL_IPU_QVGAv1) || defined (CFG_INCLUDE_DA210_BRD_16PMP_8BPP_IPU_WQVGAv1) || \
    defined (CFG_INCLUDE_DA210_BRD_16PMP_4BPP_INTERNAL_IPU_WQVGAv1) || defined (CFG_INCLUDE_DA210_BRD_16PMP_1BPP_INTERNAL_IPU_WQVGAv1)
#define USE_COMP_IPU
#endif

#endif
/*********************************************************************
* Overview: Specifies the colors used in different color depths.
*           BRIGHTRED is requires by the Resistive Touchscreen driver.
*           For 4 and 1 BPP BRIGHTRED is replaced by BLACK.
*
*********************************************************************/
#if (COLOR_DEPTH == 1)

    #define BLACK	    0   
    #define WHITE	    1    
    #define BRIGHTRED   BLACK
			
#elif (COLOR_DEPTH == 4)

	#define BLACK       0
    #define GRAY000     0
    #define GRAY001     1
	#define GRAY002     2
	#define GRAY003     3 
	#define GRAY004     4 
	#define GRAY005     5 
    #define GRAY006     6
	#define GRAY007     7
	#define GRAY008     8    
	#define GRAY009     9 
	#define GRAY010     10 
	#define GRAY011     11 
	#define GRAY012     12 
	#define GRAY013     13 
	#define GRAY014     14 
	#define GRAY015     15 
	#define WHITE       15
    #define BRIGHTRED   0

#elif (COLOR_DEPTH == 8)    

    #define WHITE       0
    #define BLACK       255
    #define BRIGHTRED   254

#endif

#endif // _GRAPHICSCONFIG_H
