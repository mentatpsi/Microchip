/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Solomon Systech SSD1305 LCD controller driver 
  *****************************************************************************
 * FileName:        SSD1305.h
 * Dependencies:    p24Fxxxx.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30
 * Linker:          MPLAB LINK30
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
 * 12/28/11		Ported from SSD1303/SH1101A driver
 *****************************************************************************/
#ifndef _SSD1305_OLED_H
    #define _SSD1305_OLED_H

    #include "HardwareProfile.h"
    #include "GenericTypeDefs.h"
    #include "GraphicsConfig.h"
    
#if defined (USE_GFX_PMP)
    #include "Graphics/gfxpmp.h"
#elif defined (USE_GFX_EPMP)
    #include "Graphics/gfxepmp.h"
#endif

    #ifdef USE_16BIT_PMP
        #error This driver doesn't support 16-bit PMP (remove USE_16BIT_PMP option from HardwareProfile.h)
    #endif
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
    #if (DISP_HOR_RESOLUTION > 132)
        #error This driver doesn't supports this resolution. Horisontal resolution must be 128 pixels or less.
    #endif
    #if (DISP_VER_RESOLUTION > 64)
        #error This driver doesn't supports this resolution. Vertical resolution must be 64 pixels or less.
    #endif

/*********************************************************************
* Overview: Display orientation.
*********************************************************************/
    #if (DISP_ORIENTATION != 0) && (DISP_ORIENTATION != 180)
        #error This driver doesn't support this orientation.
    #endif

/*********************************************************************
* Overview: Defines the display offset in x direction. Dependent on the display 
* used and how it is connected.
*********************************************************************/
#if (DISP_ORIENTATION == 0) 
    #define GRAM_COLUMN_OFFSET  4 
#elif (DISP_ORIENTATION == 180)
    #define GRAM_COLUMN_OFFSET  0 
#endif

/*********************************************************************
* Overview: Screen Saver parameters. 
*			- SSON - Means that screen saver will be enabled when 
*					 ScreenSaver(SSON) function is called with SSON as 
*					 parameter.
*			- SSOFF - Means that screen saver will be disbled when 
*					 ScreenSaver(SSOFF) function is called with SSOFF as 
*					 parameter.
*           
*********************************************************************/
    #define SSON    1           // screen saver is turned on
    #define SSOFF   0           // screen saver is turned off

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

typedef enum
{
    SCROLL_RIGHT = 0,
    SCROLL_LEFT = 1
} SSD1305_SCROLL_DIR;    

typedef enum
{
    SCROLL_OFFSET_0 = 0,        // no scrolling will occur
    SCROLL_OFFSET_1 = 1,        // 1 column offset
    SCROLL_OFFSET_2 = 2,        // 2 column offset
    SCROLL_OFFSET_3 = 3,        // 3 column offset
    SCROLL_OFFSET_4 = 4         // 4 column offset
} SSD1305_SCROLL_OFFSET;    

typedef enum
{
    SCROLL_PAGE_0 = 0,
    SCROLL_PAGE_1 = 1,
    SCROLL_PAGE_2 = 2,
    SCROLL_PAGE_3 = 3,
    SCROLL_PAGE_4 = 4,
    SCROLL_PAGE_5 = 5,
    SCROLL_PAGE_6 = 6,
    SCROLL_PAGE_7 = 7,
} SSD1305_SCROLL_PAGE; 

typedef enum
{
    SCROLL_INTERVAL_6 = 0,      // 6 frames interval
    SCROLL_INTERVAL_32 = 1,     // 32 frames interval
    SCROLL_INTERVAL_64 = 2,     // 64 frames interval
    SCROLL_INTERVAL_128 = 3,    // 128 frames interval
} SSD1305_SCROLL_INTERVAL;

/*********************************************************************
* Function: void GFX_Horizontal_Scroll()
*
* PreCondition: none
*
* Input: dir - defines the direction of horizontal scrolling (see type SSD1305_SCROLL_DIR).
*		          -  SCROLL_RIGHT : scroll direction is right
*		          -  SCROLL_LEFT  : scroll direction is left
*		 offset - sets the scroll offset. Allowed values are: 
*		          -  SCROLL_OFFSET_0 : no scrolling will occur
*		          -  SCROLL_OFFSET_1 : 1 column offset
*		          -  SCROLL_OFFSET_2 : 2 column offset
*		          -  SCROLL_OFFSET_3 : 3 column offset
*		          -  SCROLL_OFFSET_4 : 4 column offset
*		 pageStart - sets the start page of scrolling. There are 8 pages in SSD1305.
*                    Verify where the display buffer is located to see the scrolling on the screen.  
*		          -  SCROLL_PAGE_0 : page 0
*		          -  SCROLL_PAGE_1 : page 1
*		          -  SCROLL_PAGE_2 : page 2
*		          -  SCROLL_PAGE_3 : page 3
*		          -  SCROLL_PAGE_4 : page 4
*		          -  SCROLL_PAGE_5 : page 5
*		          -  SCROLL_PAGE_6 : page 6
*		          -  SCROLL_PAGE_7 : page 7
*		 pageEnd - sets the end page of scrolling. pageEnd must be equal or greater than pageStart.
*                  See pageStart for valid values.
*		 frameInterval - sets the frame interval per scroll.
*		          -  SCROLL_INTERVAL_6   : 6 frames per scroll
*		          -  SCROLL_INTERVAL_32  : 32 frames per scroll
*		          -  SCROLL_INTERVAL_64  : 64 frames per scroll
*		          -  SCROLL_INTERVAL_128 : 128 frames per scroll
*
* Output: none
*
* Side Effects: none
*
* Overview: Performs horizontal scrolling feature of SSD1305. Call 
*           SSD1305_Scroll_Stop() To halt the scrolling.
*
* Note: none
*
********************************************************************/
void SSD1305_Horizontal_Scroll( SSD1305_SCROLL_DIR dir, 
                                SSD1305_SCROLL_OFFSET offset, 
                                SSD1305_SCROLL_PAGE pageStart, 
                                SSD1305_SCROLL_PAGE pageEnd, 
                                SSD1305_SCROLL_INTERVAL frameInterval);

/*********************************************************************
* Function: void SSD1305_Vertical_Scroll(   WORD fixedRowsStart, WORD noScrollRows, 
*                                           SSD1305_SCROLL_OFFSET offset, 
*                                           SSD1305_SCROLL_INTERVAL frameInterval)
*
* PreCondition: none
*
* Input: fixedRowsStart - defines the number of lines that are fixed (top fixed area). 
*                         For example, if start of fixed area is the first line, set
*                         this value to 0. If start if fixed area is line 20, set 
*                         this value to 20. 
*		 noScrollRows - defines the number of scrolling lines. Scrolling line starts immediately 
*                       after the top fixed area.
*		          -  SCROLL_OFFSET_0 : no scrolling will occur
*		          -  SCROLL_OFFSET_1 : 1 column offset
*		          -  SCROLL_OFFSET_2 : 2 column offset
*		          -  SCROLL_OFFSET_3 : 3 column offset
*		          -  SCROLL_OFFSET_4 : 4 column offset
*		 offset - sets the scroll offset. Allowed values are: 
*		          -  SCROLL_OFFSET_0 : no scrolling will occur
*		          -  SCROLL_OFFSET_1 : 1 column offset
*		          -  SCROLL_OFFSET_2 : 2 column offset
*		          -  SCROLL_OFFSET_3 : 3 column offset
*		          -  SCROLL_OFFSET_4 : 4 column offset
*		 frameInterval - sets the frame interval per scroll.
*		          -  SCROLL_INTERVAL_6   : 6 frames per scroll
*		          -  SCROLL_INTERVAL_32  : 32 frames per scroll
*		          -  SCROLL_INTERVAL_64  : 64 frames per scroll
*		          -  SCROLL_INTERVAL_128 : 128 frames per scroll
*
* Output: none
*
* Side Effects: none
*
* Overview: Performs vertical scrolling feature of SSD1305. Call 
*           SSD1305_Scroll_Stop() To halt the scrolling.
*
* Note: none
*
********************************************************************/
void SSD1305_Vertical_Scroll(   WORD fixedRowsStart, WORD noScrollRows, 
                                SSD1305_SCROLL_OFFSET offset, 
                                SSD1305_SCROLL_INTERVAL frameInterval);

/*********************************************************************
* Function: void SSD1305_Scroll_Stop()
*   
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Stops the scrolling feature of SSD1305. 
*
* Note: none
*
********************************************************************/
void SSD1305_Scroll_Stop();


/*********************************************************************
* Macro: void SSD1305_Inverse_Pixel_Color()
*   
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Reverses the pixel colors on the screen. Pixels in GRAM 
*           that are BLACK will show as WHITE on the screen and pixels 
*           in GRAM that are WHITE will show as BLACK on the screen. 
*
* Note: none
*
********************************************************************/
#define SSD1305_Inverse_Pixel_Color()   {	                        \
                                            DisplayEnable();        \
                                            DisplaySetCommand();    \
                                            DeviceWrite(0xA7);      \
                                            DisplaySetData();       \
                                            DisplayDisable();       \
                                        }                                        

/*********************************************************************
* Macro: void SSD1305_Normal_Pixel_Color()
*   
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Normal pixel colors on the screen. Pixels in GRAM 
*           that are BLACK will show as BLACK on the screen and pixels 
*           in GRAM that are WHITE will show as WHITE on the screen. 
*
* Note: none
*
********************************************************************/       
#define SSD1305_Normal_Pixel_Color()    {	                        \
                                            DisplayEnable();        \
                                            DisplaySetCommand();    \
                                            DeviceWrite(0xA6);      \
                                            DisplaySetData();       \
                                            DisplayDisable();       \
                                        }                                        


#endif //   #define _SSD1305_OLED_H

