/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Main header file for the display driver
 *****************************************************************************
 * FileName:        DisplayDriver.h
 * Dependencies:    p24Fxxxx.h or plib.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 09/04/08    ...
 * 12/09/10	   Modified CustomDisplayDriver.h to be implemented in application
 *             directory.
 * 03/09/11    - Added prototypes for driver layer. This now serves as a 
 *               guide for Graphics Library required items for display driver.
 * 03/11/11    - For Graphics Library Version 3.00
 *             - Reverted to use GetMaxX() and GetMaxY() macros.
 * 03/22/11    Changed GetPixel() to return the type GFX_COLOR.
 * 03/27/11    Added double buffering feature prototypes.
 * 05/04/11    Added CopyWindow(), CopyPageWindow() and GetPageAddress().
  *****************************************************************************/
#ifndef _DISPLAYDRIVER_H
#define _DISPLAYDRIVER_H

#include "GenericTypeDefs.h"
#include "GraphicsConfig.h"
#include "gfxcolors.h"
#include "HardwareProfile.h"
    
/*********************************************************************
* Overview: Clipping region control codes to be used with SetClip(...)
*           function. 
*********************************************************************/
#define CLIP_DISABLE                0   // Disables clipping.
#define CLIP_ENABLE                 1   // Enables clipping.

#ifdef USE_TRANSPARENT_COLOR
#define TRANSPARENT_COLOR_ENABLE    1   // Check pixel if color is equal to transparent color, if equal do not render pixel
#define TRANSPARENT_COLOR_DISABLE   0   // Check of transparent color is not performed
#endif

extern GFX_COLOR            _color;
extern GFX_COLOR            _chrcolor;

#ifdef USE_TRANSPARENT_COLOR
extern GFX_COLOR            _colorTransparent;
extern SHORT                _colorTransparentEnable;
#endif

/*********************************************************************
* Overview: Clipping region control and border settings.
*
*********************************************************************/
// Clipping region enable control
extern SHORT _clipRgn;
// Left clipping region border
extern SHORT _clipLeft;
// Top clipping region border
extern SHORT _clipTop;
// Right clipping region border
extern SHORT _clipRight;
// Bottom clipping region border
extern SHORT _clipBottom;    


#ifdef GFX_DRV_PAGE_COUNT
/*********************************************************************
* Overview: Page table that summarizes the addresses of each available
*           page. The number of pages is dictated by the GFX_DRV_PAGE_COUNT
*           value defined in the hardware profile.
*
*********************************************************************/
extern volatile DWORD _PageTable[GFX_DRV_PAGE_COUNT];
#endif

/*********************************************************************
* Function:  void ResetDevice()
*
* Overview: Initializes LCD module.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void ResetDevice(void);

/*********************************************************************
* Macros:  GetMaxX()
*
* Overview: Returns maximum horizontal coordinate.
*
* PreCondition: none
*
* Input: none
*
* Output: Maximum horizontal coordinate.
*
*
* Example:
*   <CODE> 
*
*	// Create a window that will occupy the whole screen.
*	WndCreate(0xFF,				    	// ID
*             0,0,
*			  GetMaxX(),GetMaxY(),		// dimension
*             WND_DRAW,					// will be dislayed after creation
*             (void*)&mchpIcon,         // use icon used
*             pText,	   				// set to text pointed to by pText
*             NULL);					// use default scheme 
*
*  </CODE>
*
* Side Effects: none
*
********************************************************************/
    #if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)
        #define GetMaxX()   (DISP_VER_RESOLUTION - 1)
    #elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)
        #define GetMaxX()   (DISP_HOR_RESOLUTION - 1)
    #endif
        
/*********************************************************************
* Macros:  GetMaxY()
*
* Overview: Returns maximum vertical coordinate.
*
* PreCondition: none
*
* Input: none
*
* Output: Maximum vertical coordinate.
*
* Example: (see GetMaxX()) example.
*
* Side Effects: none
*
********************************************************************/
    #if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)
        #define GetMaxY()   (DISP_HOR_RESOLUTION - 1)
    #elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)
        #define GetMaxY()   (DISP_VER_RESOLUTION - 1)
    #endif
    
/*********************************************************************
* Macros:  SetColor(color)
*
* Overview: Sets current drawing color.
*
* PreCondition: none
*
* Input: color - Color coding is based on GFX_COLOR definition. GFX_COLOR  
*                definition is based on the color depth (COLOR_DEPTH) used. 
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetColor(color) _color = (color)

/*********************************************************************
* Macros:  GetColor()
*
* Overview: Returns current drawing color.
*
* PreCondition: none
*
* Input: none
*
* Output: Color where coding is based on GFX_COLOR definition. GFX_COLOR 
*         definition is based on the color depth (COLOR_DEPTH) used.
*
* Side Effects: none
*
********************************************************************/
#define GetColor() _color

#ifdef USE_TRANSPARENT_COLOR
/*********************************************************************
* Function:  void TransparentColorEnable(GFX_COLOR color)
*
* Overview: Sets current transparent color. PutImage() will not render
*           pixels that matches the set transparent color. To enable 
*           Transparent Color feature, define the macro USE_TRANSPARENT_COLOR 
*           in the GraphicsConfig.h file. 
*
* Description: 
*        <img name="TransparencyColorExample.jpg" />
*
* PreCondition: none
*
* Input: color - Chosen transparent color.
*
* Output: none
*
* Example:
*   <CODE> 
*   TransparentColorEnable(BLACK);
*   PutImage(0,0, (void*)&ScreenBackground);
*   PutImage(0,0, (void*)&RibbonIcon);
*	</CODE> 
*
* Side Effects: None
********************************************************************/
void TransparentColorEnable(GFX_COLOR color);

/*********************************************************************
* Macros:  TransparentColorDisable()
*
* Overview: Disables the transparent color function.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define TransparentColorDisable() _colorTransparentEnable = TRANSPARENT_COLOR_DISABLE;

/*********************************************************************
* Macros:  GetTransparentColorStatus()
*
* Overview: Returns the current transparent color function enable status.
*
* PreCondition: none
*
* Input: none
*
* Output: Returns the current transparent color function enable status
*	<CODE> 
*          0 – Transparent color function is disabled.
*          1 – Transparent color function is enabled.
*	</CODE>	
*
* Side Effects: None
*
********************************************************************/
#define GetTransparentColorStatus() _colorTransparentEnable

/*********************************************************************
* Macros:  GetTransparentColor()
*
* Overview: Returns the current transparent color value.
*
* PreCondition: none
*
* Input: none
*
* Output: Returns the current transparent color used.
*
* Side Effects: none
*
********************************************************************/
#define GetTransparentColor() _colorTransparent

#endif

/*********************************************************************
* Functions:  SetActivePage(page)
*
* Overview: Sets active graphic page.
*
* PreCondition: none
*
* Input: page - Graphic page number.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void SetActivePage(WORD page);

/*********************************************************************
* Functions: SetVisualPage(page)
*
* Overview: Sets graphic page to display.
*
* PreCondition: none
*
* Input: page - Graphic page number
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void SetVisualPage(WORD page);    

/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* Overview: Puts pixel with the given x,y coordinate position.
*
* PreCondition: none
*
* Input: x - x position of the pixel.
*		 y - y position of the pixel.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void PutPixel(SHORT x, SHORT y);

/*********************************************************************
* Function: GFX_COLOR GetPixel(SHORT x, SHORT y)
*
* Overview: Returns pixel color at the given x,y coordinate position.
*
* PreCondition: none
*
* Input: x - x position of the pixel.
*		 y - y position of the pixel.
*
* Output: pixel color
*
* Side Effects: none
*
********************************************************************/
GFX_COLOR GetPixel(SHORT x, SHORT y);

/*********************************************************************
* Function: SetClipRgn(left, top, right, bottom)
*
* Overview: Sets clipping region.
*
* PreCondition: none
*
* Input: left - Defines the left clipping region border.
*		 top - Defines the top clipping region border.
*		 right - Defines the right clipping region border.
*	     bottom - Defines the bottom clipping region border.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void SetClipRgn(SHORT left, SHORT top, SHORT right, SHORT bottom);

/*********************************************************************
* Macros: GetClipLeft()
*
* Overview: Returns left clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Left clipping border.
*
* Side Effects: none
*
********************************************************************/
#define GetClipLeft() _clipLeft

/*********************************************************************
* Macros: GetClipRight()
*
* Overview: Returns right clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Right clipping border.
*
* Side Effects: none
*
********************************************************************/
#define GetClipRight() _clipRight

/*********************************************************************
* Macros: GetClipTop()
*
* Overview: Returns top clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Top clipping border.
*
* Side Effects: none
*
********************************************************************/
#define GetClipTop() _clipTop

/*********************************************************************
* Macros: GetClipBottom()
*
* Overview: Returns bottom clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Bottom clipping border.
*
* Side Effects: none
*
********************************************************************/
#define GetClipBottom() _clipBottom

/*********************************************************************
* Function: SetClip(control)
*
* Overview: Enables/disables clipping.
*
* PreCondition: none
*
* Input: control - Enables or disables the clipping.
*			- CLIP_DISABLE: Disable clipping
*			- CLIP_ENABLE: Enable clipping
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void SetClip(BYTE control);
    
/*********************************************************************
* Function: IsDeviceBusy()
*
* Overview: Returns non-zero if LCD controller is busy 
*           (previous drawing operation is not completed).
*
* PreCondition: none
*
* Input: none
*
* Output: Busy status.
*
* Side Effects: none
*
********************************************************************/
WORD IsDeviceBusy();

/*********************************************************************
* Function: WORD PutImageDrv(SHORT left, SHORT top, void* image, BYTE stretch, void *pPartialImageData)
*
* Overview: Optional function that can be implemented in Display 
*           Driver Layer to implement PutImage() functions.
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - pointer to the partial image parameters 
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the image is not yet completely drawn.
*         - Returns 1 when the image is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
********************************************************************/
WORD PutImageDrv(SHORT left, SHORT top, void *image, BYTE stretch, void *pPartialImageData);


/*********************************************************************
* OPTIONAL APIs - the following functions and macros are used
*                 if the corresponding driver can support the 
*                 features.
*
*********************************************************************/

/*********************************************************************
* Function: WORD CopyWindow( DWORD srcAddr, DWORD dstAddr, 
*                            WORD srcX, WORD srcY, 
*                            WORD dstX, WORD dstY, 
*                            WORD width, WORD height)  
*
* Overview: A windows is a rectangular area with the given width and height  
*           located in the given base source address. The source and destination 
*           window can be located in the same base address. If this is the 
*           case, then srcAddr = dstAddr. The operation is similar to CopyPageWindow()
*           but instead of using page numbers, addresses are used for versatility.
*           This is a blocking function.    
*
* PreCondition: none
*
* Input: srcAddr - Base Address of the source window,
*        dstPage - Base Address of the destination window,
*        srcX - x location of the left top corner of the source window 
*               respect to the srcPage.
*        srcY - y location of the left top corner of the source window 
*               respect to the srcPage.
*        dstX - x location of the left top corner of the destination window 
*               respect to the dstPage.
*        dstY - y location of the left top corner of the destination window 
*               respect to the dstPage.
*        width - the width in pixels of the window to copy
*        height - the height in pixels of the window to copy
*
* Output: None
*
* Side Effects: none
*
********************************************************************/
WORD CopyWindow( DWORD srcAddr, DWORD dstAddr,          \
                 WORD srcX, WORD srcY,                  \
                 WORD dstX, WORD dstY,                  \
                 WORD width, WORD height);                   

/*********************************************************************
* Function: WORD CopyBlock(DWORD srcAddr, DWORD dstAddr, 
*						   DWORD srcOffset, DWORD dstOffset, 
*                          WORD width, WORD height)
*
* Overview: Copies a block of data from source specified by srcAddr 
*           and srcOffset to the destination specified by dstAddr 
*           and dstOffset. This is similar to the CopyWindow() and
*           but instead of using left, top position of the source
*           and destination, it uses offsets instead. 
*           This is a blocking function.
*
* PreCondition: none
*
* Input: srcAddr - the base address of the data to be moved
*        dstAddr - the base address of the new location of the moved data 
*        srcOffset - offset of the data to be moved with respect to the 
*					 source base address.
*        dstOffset - offset of the new location of the moved data respect 
*					 to the source base address.
*        width - width of the block of data to be moved
*        height - height of the block of data to be moved
*
* Output: none
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
WORD CopyBlock(DWORD srcAddr, DWORD dstAddr, DWORD srcOffset, DWORD dstOffset, WORD width, WORD height); 

/*********************************************************************
* Macros: GetPageAddress(page)
*
* Overview: Returns the address of the given page.
*
* PreCondition: none
*
* Input: none
*
* Output: Address in memory of the given page number. The number of pages
*         is dictated by GFX_DRV_PAGE_COUNT value defined in the hardware
*         profile. GFX_DRV_PAGE_COUNT is not mandatory. Drivers that 
*         do not have enough memory for paging may not define this constant.
*         If GFX_DRV_PAGE_COUNT is not defined, all API's related to 
*         paging operation is will not be available.
*
* Side Effects: none
*
********************************************************************/
#define GetPageAddress(page)    (_PageTable[page])   

/*********************************************************************
* Function:  CopyPageWindow(srcPage, dstPage, srcX, srcY, dstX, dstY, 
*                       width, height)  
*
* Overview: This is a blocking call.
*           A windows is a rectangular area with the given width and height  
*           of a page. The source and destination window can be located in 
*           different pages and each page is assumed to have the same dimensions
*           (equal width and height).
*
* PreCondition: none
*
* Input: srcPage - page number of the source window,
*        dstPage - page number of the destination window,
*        srcX - x location of the left top corner of the source window 
*               respect to the srcPage.
*        srcY - y location of the left top corner of the source window 
*               respect to the srcPage.
*        dstX - x location of the left top corner of the destination window 
*               respect to the dstPage.
*        dstY - y location of the left top corner of the destination window 
*               respect to the dstPage.
*        width - the width in pixels of the window to copy
*        height - the height in pixels of the window to copy
*
* Output: None
*
* Side Effects: none
*
********************************************************************/
void CopyPageWindow( BYTE srcPage, BYTE dstPage,     \
                     WORD srcX, WORD srcY,                  \
                     WORD dstX, WORD dstY,                  \
                     WORD width, WORD height);          
                                                         

#ifdef USE_DOUBLE_BUFFERING

/*********************************************************************
* Function:  SwitchOnDoubleBuffering()
*
* Overview: Switches on the double buffering.
*			Double buffering utilizes two buffers. The frame buffer and the
*           draw buffer. The frame buffer is the buffer that is being displayed
*			while the draw buffer is used for all rendering. 
*           When this function is called, it copies the contents of the frame buffer 
*           to the draw buffer once and all succeeding rendering will be performed on 
*           the draw buffer. To update the frame buffer with newly drawn 
*           items on the draw buffer call UpdateDisplayNow() or RequestDisplayUpdate().
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void SwitchOnDoubleBuffering(void);

/*********************************************************************
* Function:  SwitchOffDoubleBuffering()
*
* Overview: Switches off the double buffering.
*           All rendering will be performed on the frame buffer. Calls
*           to UpdateDisplayNow() or RequestDisplayUpdate() will 
*           have no effect.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void SwitchOffDoubleBuffering(void);

/*********************************************************************
* Function:  InvalidateAll()
*
* Overview: Invalidates the whole screen and resets the number of
*           invalidated areas to zero. See also InvalidateRectangle().
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
extern inline void __attribute__((always_inline)) InvalidateAll(void)
{
    extern BYTE blInvalidateAll;
      
    blInvalidateAll = 1;
}

/*********************************************************************
* Function:  IsDisplayUpdatePending()
*
* Overview: Returns the status of the display update. A display update is
*           initiated by RequestDisplayUpdate() or UpdateDisplayNow().
*			RequestDisplayUpdate() will wait for the Vertical Blanking 
*           interrupt before the update is performed. While waiting for 
*           the interrupt, display update will be pending and the status
*           returned by this macro is 1.
*
* PreCondition: none
*
* Input: none
*
* Output: Returns the status of the display update 
*		  - 0 – There is no pending update
*         - 1 - Update is still pending 
*
* Side Effects: none
*
********************************************************************/
extern inline BYTE __attribute__((always_inline)) IsDisplayUpdatePending(void)
{
    extern volatile BYTE blDisplayUpdatePending;
      
    return blDisplayUpdatePending;
}

/*********************************************************************
* Function:  void InvalidateRectangle(WORD left, WORD top, WORD right, WORD bottom)
*
* Overview: Invalidates the specified rectangular area. This increments the 
*           number of invalidated areas and if the number of 
*           invalidated areas exceed the GFX_MAX_INVALIDATE_AREAS,
*           the whole frame buffer is invalidated.
*
* PreCondition: None
*
* Input: left - left position
*		 top - top position
*		 right - right position
*        bottom - bottom position
*
* Output: None
*
* Side Effects: Copies back the invalidated areas only to the draw 
*               buffer after the exchange of draw and frame buffers.
*
********************************************************************/
void InvalidateRectangle(WORD left, WORD top, WORD right, WORD bottom);

/*********************************************************************
* Function:  void RequestDisplayUpdate(void)
*
* Overview: Synchronizes the draw and frame buffers at next VBlank
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void RequestDisplayUpdate(void);

/*********************************************************************
* Function:  void UpdateDisplayNow(void)
*
* Overview: Synchronizes the draw and frame buffers immediately. 
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void UpdateDisplayNow(void);

/*********************************************************************
* Function: WORD GetDrawBufferAddress(void)
*
* Overview: Returns the address of Draw Buffer
*
* PreCondition: none
*
* Input: none
*
* Output: Address of the Draw Buffer
*
* Side Effects: none
*
********************************************************************/
DWORD GetDrawBufferAddress(void);

/*********************************************************************
* Function: DWORD GetFrameBufferAddress(void)
*
* Overview: Returns the address of Frame Buffer
*
* PreCondition: _drawbuffer must be set to either GFX_BUFFER1 or GFX_BUFFER2
*
* Input: none
*
* Output: Address of the Frame Buffer
*
* Side Effects: none
*
********************************************************************/
DWORD GetFrameBufferAddress(void);

#endif  //#ifdef USE_DOUBLE_BUFFERING

#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH

/*********************************************************************
* Function: void GFX_DRIVER_SetupDrawUpdate(UInt16 startx, UInt16 starty, UInt16 endx, UInt16 endy)
*
* Overview: Sets up the partital update for bi-stable displays.  
*
* PreCondition: none.
*
* Input:    startx - the starting position of the partital area that is being updated.
*           starty - the starting position of the partital area that is being updated.
*           endx - the ending position of the partital area that is being updated.
*           endy - the ending position of the partital area that is being updated.
*
* Output: none
*
* Side Effects: none
********************************************************************/
void GFX_DRIVER_SetupDrawUpdate(WORD startx, WORD starty, WORD endx, WORD endy);

/*********************************************************************
* Function: void GFX_DRIVER_CompleteDrawUpdate(UInt16 startx, UInt16 starty, UInt16 endx, UInt16 endy)
*
* Overview: Completes the partital update for bi-stable displays.  
*
* PreCondition: none.
*
* Input:    startx - the starting position of the partital area that is being updated.
*           starty - the starting position of the partital area that is being updated.
*           endx - the ending position of the partital area that is being updated.
*           endy - the ending position of the partital area that is being updated.
*
* Output: none
*
* Side Effects: none
********************************************************************/
void GFX_DRIVER_CompleteDrawUpdate(WORD startx, WORD starty, WORD endx, WORD endy);
#endif

#endif
