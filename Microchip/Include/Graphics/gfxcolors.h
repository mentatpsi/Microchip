/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  default color definitions.
 *****************************************************************************
 * FileName:        gfxcolors.h
 * Processor:       PIC24, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright (C) 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
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
 * 02/03/11		...
 * 10/10/11     Added additional compile time checks and re-arrange logics.
 *              Removed RGBConvert() for 1bpp color depth, use BLACK/WHITE definitions.
 *****************************************************************************/

#ifndef _GFX_COLORS_H
#define _GFX_COLORS_H

/*********************************************************************
* Section: Includes
*********************************************************************/
#include "GenericTypeDefs.h"
#include "GraphicsConfig.h"

/*********************************************************************
* Section: Macro Error Checking
*********************************************************************/
#ifndef COLOR_DEPTH
    #error "Please define COLOR_DEPTH in GraphicsConfig.h; valid values (dependent on the driver used) are: 1, 4, 8, 16, 24)"
#endif

#if ((COLOR_DEPTH != 1) && (COLOR_DEPTH != 4) && (COLOR_DEPTH != 8) && (COLOR_DEPTH != 16) && (COLOR_DEPTH != 24))
    #error "Defined COLOR_DEPTH value is not suppported. Valid values (dependent on the driver used) are: 1, 4, 8, 16, 24)"
#endif

/*********************************************************************
* Overview: Data type that defines the color data. This type is dependent 
*           on the COLOR_DEPTH setting. See COLOR_DEPTH.
*           - GFX_COLOR is type BYTE  if COLOR_DEPTH <= 8   
*           - GFX_COLOR is type WORD if COLOR_DEPTH = 16
*           - GFX_COLOR is type DWORD if COLOR_DEPTH = 24  
*
*********************************************************************/
#if (COLOR_DEPTH <= 8)
    typedef BYTE GFX_COLOR;   
#elif (COLOR_DEPTH == 16)
    typedef WORD GFX_COLOR;   
#elif (COLOR_DEPTH == 24)
    typedef DWORD GFX_COLOR;  
#endif

// color depths 1, 2 or 4 are usually for monochrome/grayscale format so we are not defining
// any RGB conversion here.
#if (COLOR_DEPTH == 8)

    #ifndef RGBConvert
		/*********************************************************************
		* Macros:  RGBConvert(red, green, blue)
		*
		* Overview: This macro converts the color data to the selected 
		*           COLOR_DEPTH. This macro is only valid when COLOR_DEPTH is 8, 16, or 24.
		*
		*	<TABLE>
		*    	COLOR_DEPTH            Conversion
		*     	##################     ############
		*     	8                      8-8-8 to 3-3-2 conversion
		*     	16                     8-8-8 to to 5-6-5 conversion
		*     	24                     8-8-8 to 8-8-8 conversion or no conversion						 
		*	</TABLE>
		*
		* PreCondition: none
		*
		* Input: red - red component of the color.
		*        green - green component of the color.
		*        blue - blue component of the color.
		*
		* Output: none
		*
		* Side Effects: none
		*
		********************************************************************/	
        #define RGBConvert(red, green, blue)    (GFX_COLOR) (((GFX_COLOR)(red) & 0xE0) | (((GFX_COLOR)(green) & 0xE0) >> 3) | (((GFX_COLOR)(blue)) >> 6))
    #endif

#elif (COLOR_DEPTH == 16)

    #ifndef RGBConvert
		/*********************************************************************
		* Macros:  RGBConvert(red, green, blue)
		*
		* Overview: This macro converts the color data to the selected 
		*           COLOR_DEPTH. This macro is only valid when COLOR_DEPTH is 8, 16, or 24.
		*
		*	<TABLE>
		*    	COLOR_DEPTH            Conversion
		*     	##################     ############
		*     	8                      8-8-8 to 3-3-2 conversion
		*     	16                     8-8-8 to to 5-6-5 conversion
		*     	24                     8-8-8 to 8-8-8 conversion or no conversion						 
		*	</TABLE>
		*
		* PreCondition: none
		*
		* Input: red - red component of the color.
		*        green - green component of the color.
		*        blue - blue component of the color.
		*
		* Output: none
		*
		* Side Effects: none
		*
		********************************************************************/	
        #define RGBConvert(red, green, blue)    (GFX_COLOR) ((((GFX_COLOR)(red) & 0xF8) << 8) | (((GFX_COLOR)(green) & 0xFC) << 3) | ((GFX_COLOR)(blue) >> 3))                                               
    #endif

   #define ConvertColor50(color)  (GFX_COLOR)((color & (0b1111011111011110))>>1)
   #define ConvertColor25(color)  (GFX_COLOR)((color & (0b1110011110011100))>>2)
   #define ConvertColor75(color)  (GFX_COLOR)(ConvertColor50(color) + ConvertColor25(color))

#elif (COLOR_DEPTH == 24)

    #ifndef RGBConvert
 		/*********************************************************************
		* Macros:  RGBConvert(red, green, blue)
		*
		* Overview: This macro converts the color data to the selected 
		*           COLOR_DEPTH. This macro is only valid when COLOR_DEPTH is 8, 16, or 24.
		*
		*	<TABLE>
		*    	COLOR_DEPTH            Conversion
		*     	##################     ############
		*     	8                      8-8-8 to 3-3-2 conversion
		*     	16                     8-8-8 to to 5-6-5 conversion
		*     	24                     8-8-8 to 8-8-8 conversion or no conversion						 
		*	</TABLE>
		*
		* PreCondition: none
		*
		* Input: red - red component of the color.
		*        green - green component of the color.
		*        blue - blue component of the color.
		*
		* Output: none
		*
		* Side Effects: none
		*
		********************************************************************/	
         #define RGBConvert(red, green, blue)    (GFX_COLOR) (((GFX_COLOR)(red) << 16) | ((GFX_COLOR)(green) << 8) | (GFX_COLOR)(blue))
    #endif

   #define ConvertColor50(color)  (GFX_COLOR)((color & (0x00FEFEFEul))>>1)
   #define ConvertColor25(color)  (GFX_COLOR)((color & (0x00FCFCFCul))>>2)
   #define ConvertColor75(color)  (GFX_COLOR)(ConvertColor50(color) + ConvertColor25(color))

#endif

// this macro is deprecated, use RGBConvert() instead
#define RGB565CONVERT(red, green, blue)         (WORD) (((((GFX_COLOR)(red) & 0xF8) >> 3) << 11) | ((((GFX_COLOR)(green) & 0xFC) >> 2) << 5) | (((GFX_COLOR)(blue) & 0xF8) >> 3))

/*********************************************************************
* Overview: Basic colors definitions.
*           The basic colors defined in this section are defined
*           for basic demo requirements. End application can define additional
*           colors or override existing default colors. If overriding an
*           existing default color, define the new color value before
*           the #include of Graphics.h to avoid the compile time warning.
*           When using palette, a different file 'PaletteColorDefines.h' has
*           to be used instead.
*********************************************************************/
#ifdef USE_PALETTE

	#include "PaletteColorDefines.h"

#else // not USE_PALETTE

	#if (COLOR_DEPTH == 1)

		#ifndef BLACK
			#define BLACK	0   
		#endif
		#ifndef WHITE
			#define WHITE	1    
		#endif       	    
			
	#elif (COLOR_DEPTH == 4)

		#ifndef BLACK
			#define BLACK   0
		#endif
		#ifndef GRAY000
			#define GRAY000 0
		#endif
		#ifndef GRAY001
			#define GRAY001 1
		#endif
		#ifndef GRAY002
			#define GRAY002 2
		#endif
		#ifndef GRAY003
			#define GRAY003 3 
		#endif
		#ifndef GRAY004
			#define GRAY004 4 
		#endif
		#ifndef GRAY005
			#define GRAY005 5 
		#endif
		#ifndef GRAY006
			#define GRAY006 6
		#endif
		#ifndef GRAY007
			#define GRAY007 7
		#endif
		#ifndef GRAY008
			#define GRAY008 8 
		#endif
		#ifndef GRAY009
			#define GRAY009 9 
		#endif
		#ifndef GRAY010
			#define GRAY010 10 
		#endif
		#ifndef GRAY011
			#define GRAY011 11 
		#endif
		#ifndef GRAY012
			#define GRAY012 12 
		#endif
		#ifndef GRAY013
			#define GRAY013 13 
		#endif
		#ifndef GRAY014
			#define GRAY014 14 
		#endif
		#ifndef GRAY015
			#define GRAY015 15 
		#endif
		#ifndef WHITE
			#define WHITE   15
		#endif

		#ifndef LIGHTGRAY
			#define LIGHTGRAY      GRAY012
		#endif	
		#ifndef DARKGRAY
			#define DARKGRAY        GRAY004
		#endif	
		
	#elif (COLOR_DEPTH == 8) || (COLOR_DEPTH == 16) || (COLOR_DEPTH == 24)

		#ifndef BLACK
			#define BLACK           RGBConvert(0, 0, 0)
		#endif	
		#ifndef BRIGHTBLUE
			#define BRIGHTBLUE      RGBConvert(0, 0, 255)
		#endif	
		#ifndef BRIGHTGREEN
			#define BRIGHTGREEN     RGBConvert(0, 255, 0)
		#endif	
		#ifndef BRIGHTCYAN
			#define BRIGHTCYAN      RGBConvert(0, 255, 255)
		#endif	
		#ifndef BRIGHTRED
			#define BRIGHTRED       RGBConvert(255, 0, 0)
		#endif	
		#ifndef BRIGHTMAGENTA
			#define BRIGHTMAGENTA   RGBConvert(255, 0, 255)
		#endif	
		#ifndef YELLOW
			#define YELLOW          RGBConvert(255, 255, 128)
		#endif	
		#ifndef BRIGHTYELLOW
			#define BRIGHTYELLOW    RGBConvert(255, 255, 0)
		#endif	
		#ifndef LIGHTYELLOW
			#define LIGHTYELLOW     RGBConvert(255, 255, 150)
		#endif	
		#ifndef GOLD
			#define GOLD            RGBConvert(255, 215, 0)
		#endif
		#ifndef BLUE
			#define BLUE            RGBConvert(0, 0, 128)
		#endif	
		#ifndef GREEN
			#define GREEN           RGBConvert(0, 128, 0)
		#endif	
		#ifndef CYAN
			#define CYAN            RGBConvert(0, 128, 128)
		#endif	
		#ifndef RED
			#define RED             RGBConvert(128, 0, 0)
		#endif	
		#ifndef MAGENTA
			#define MAGENTA         RGBConvert(128, 0, 128)
		#endif	
		#ifndef BROWN
			#define BROWN           RGBConvert(255, 128, 0)
		#endif	
		#ifndef LIGHTGRAY
			#define LIGHTGRAY       RGBConvert(128, 128, 128)
		#endif	
		#ifndef DARKGRAY
			#define DARKGRAY        RGBConvert(64, 64, 64)
		#endif	
		#ifndef LIGHTBLUE
			#define LIGHTBLUE       RGBConvert(128, 128, 255)
		#endif	
		#ifndef LIGHTGREEN
			#define LIGHTGREEN      RGBConvert(128, 255, 128)
		#endif	
		#ifndef LIGHTCYAN
			#define LIGHTCYAN       RGBConvert(128, 255, 255)
		#endif	
		#ifndef LIGHTRED
			#define LIGHTRED        RGBConvert(255, 128, 128)
		#endif	
		#ifndef LIGHTMAGENTA
			#define LIGHTMAGENTA    RGBConvert(255, 128, 255)
		#endif	
		#ifndef WHITE
			#define WHITE           RGBConvert(255, 255, 255)
		#endif	
		#ifndef SADDLEBROWN
			#define SADDLEBROWN 	RGBConvert(139, 69, 19)
		#endif	
		#ifndef SIENNA
			#define SIENNA      	RGBConvert(160, 82, 45)
		#endif	
		#ifndef PERU
			#define PERU        	RGBConvert(205, 133, 63)
		#endif	
		#ifndef BURLYWOOD
			#define BURLYWOOD  	 	RGBConvert(222, 184, 135)
		#endif	
		#ifndef WHEAT
			#define WHEAT       	RGBConvert(245, 245, 220)
		#endif	
		#ifndef TAN
			#define TAN         	RGBConvert(210, 180, 140)
		#endif	
		#ifndef ORANGE
			#define ORANGE         	RGBConvert(255, 187, 76)
		#endif	
		#ifndef DARKORANGE
			#define DARKORANGE      RGBConvert(255, 140, 0)
		#endif	
		#ifndef LIGHTORANGE
			#define LIGHTORANGE     RGBConvert(255, 200, 0)
		#endif		
		#ifndef GRAY242
			#define GRAY242      	RGBConvert(242, 242, 242)    
		#endif	
		#ifndef GRAY229
			#define GRAY229      	RGBConvert(229, 229, 229)    
		#endif	
		#ifndef GRAY224
			#define GRAY224         RGBConvert(224, 224, 224)
		#endif	
		#ifndef GRAY204
			#define GRAY204      	RGBConvert(204, 204, 204)    
		#endif	
		#ifndef GRAY192
			#define GRAY192         RGBConvert(192, 192, 192)
		#endif	
		#ifndef GRAY160
			#define GRAY160         RGBConvert(160, 160, 160)
		#endif	
		#ifndef GRAY128
			#define GRAY128         RGBConvert(128, 128, 128)
		#endif	
		#ifndef GRAY096
			#define GRAY096          RGBConvert(96, 96, 96)
		#endif	
		#ifndef GRAY032
			#define GRAY032          RGBConvert(32, 32, 32)
		#endif 
		#ifndef GRAY010
			#define GRAY010          RGBConvert(10, 10, 10)
		#endif
		
        //DOM-IGNORE-BEGIN
        //Following color definitions are deprecated and should not be used in new projects.
		#ifndef GRAY95
			#define GRAY95      	RGBConvert(242, 242, 242)
		#endif	
		#ifndef GRAY90
			#define GRAY90      	RGBConvert(229, 229, 229)
		#endif		
		#ifndef GRAY0
			#define GRAY0           RGBConvert(224, 224, 224)
		#endif	
		#ifndef GRAY80
			#define GRAY80      	RGBConvert(204, 204, 204)
		#endif	
		#ifndef GRAY1
			#define GRAY1           RGBConvert(192, 192, 192)
		#endif	
		#ifndef GRAY2
			#define GRAY2           RGBConvert(160, 160, 160)
		#endif	
		#ifndef GRAY3
			#define GRAY3           RGBConvert(128, 128, 128)
		#endif	
		#ifndef GRAY4
			#define GRAY4           RGBConvert(96, 96, 96)
		#endif	
		#ifndef GRAY5
			#define GRAY5           RGBConvert(64, 64, 64)
		#endif	
		#ifndef GRAY6
			#define GRAY6           RGBConvert(32, 32, 32)
		#endif
        //DOM-IGNORE-END		
	#endif

#endif // USE_PALETTE

#endif  // #ifndef _GFX_COLORS_H

