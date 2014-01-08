/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Graphic Primitives Layer
 *****************************************************************************
 * FileName:        Primitive.c
 * Processor:       PIC24, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright (c) 2011 Microchip Technology Inc.  All rights reserved.
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07     Version 1.0 release
 * 05/11/10     Added dynamic Arc() where start and end
 *									angle can be specified.
 * 03/04/11     - Removed SetColor(WHITE) in InitGraph(). Default color
 *                after InitGraph() is not 0 (BLACK in most displays).
 *              - removed USE_DRV_XX checks, replaced them with 
 *                weak attributes.
 * 05/13/11     Add Transparent Color support in PutImage() and PutImageRLE()
 *              functions defined in this layer. 
 * 05/20/11     Added GetCirclePoint() commonly used in Widgets.
 * 07/05/11     Fixed GetTextHeight() limitation of 127 pixels. Limitation
 *              is now 256 pixels. 
 * 01/05/12     - Removed the _font global pointer and replaced with currentFont
 *                structure to make faster OutChar() rendering. 
 *              - Removed _fontFirstChar, _fontLastChar and _fontHeight globals.
 *                These are now included in currentFont.
 *              - modified OutChar(), GetTextWidth() and GetTextHeight() to 
 *                use of new currentFont structure.
 *              - break up SetFont(), OutChar() and GetTextWidth() to allow
 *                versatility of implementing text rendering functions in drivers.
 *              - added extended glyph support and font anti-aliasing
 * 05/01/12     - Added clipping support commonly used in drivers
 *              - Added putimagepartial support                          
 * 06/11/12     - Added alpha blending support for Bar(), FillBevel(), Arc().
 * 10/16/12     Fixed issue on PutImageRLExBpp() &  PutImageRLExBppExt() when
 *              system level palette is used. 
 *****************************************************************************/
#include "HardwareProfile.h"              // needed to provide values for GetMaxX() and GetMaxY() macros
#include "Graphics/DisplayDriver.h"
#include "Graphics/Primitive.h"
#include "Compiler.h" 

/////////////////////// LOCAL FUNCTIONS PROTOTYPES ////////////////////////////
#ifdef USE_BITMAP_FLASH
    void    PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
    void    PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
    void    PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
    void    PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);         

#ifdef USE_COMP_RLE
    void    PutImageRLE4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch);
    void    PutImageRLE8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch);
#endif
#endif

#ifdef USE_BITMAP_EXTERNAL
    void    PutImage1BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData); 
    void    PutImage4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData); 
    void    PutImage8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData); 
    void    PutImage16BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);

#ifdef USE_COMP_RLE
    void    PutImageRLE4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch);
    void    PutImageRLE8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch);
#endif
#endif

// Current line type
SHORT   _lineType;

// Current line thickness
BYTE    _lineThickness;

// Font orientation
BYTE    _fontOrientation;

// Current cursor x-coordinates
SHORT   _cursorX;

// Current cursor y-coordinates
SHORT   _cursorY;

// information on currently set font
GFX_FONT_CURRENT currentFont;


#ifdef USE_ANTIALIASED_FONTS

    BYTE    _antialiastype;

	#ifdef USE_PALETTE

		#error "Antialiasing can not be used when Palette is enabled"

	#endif

#endif

#ifdef USE_PALETTE

    void    *_palette;

#endif

// bevel drawing type (0 = full bevel, 0xF0 - top bevel only, 0x0F - bottom bevel only
BYTE _bevelDrawType; 

#define COSINETABLEENTRIES	90
// Cosine table used to calculate angles when rendering circular objects and  arcs  
// Make cosine values * 256 instead of 100 for easier math later
const SHORT   _CosineTable[COSINETABLEENTRIES+1] __attribute__((aligned(2))) = 
						{	
							256, 256, 256, 256, 255, 255, 255, 254, 254, 253,
							252, 251, 250, 249, 248, 247, 246, 245, 243, 242,
							241, 239, 237, 236, 234, 232, 230, 228, 226, 224,
							222, 219, 217, 215, 212, 210, 207, 204, 202, 199,
							196, 193, 190, 187, 184, 181, 178, 175, 171, 168,
							165, 161, 158, 154, 150, 147, 143, 139, 136, 132,
							128, 124, 120, 116, 112, 108, 104, 100, 96,  92,
							88,  83,  79,  75,  71,  66,  62,  58,  53,  49,
							44,  40,  36,  31,  27,  22,  18,  13,  9,   4, 
							0 
						};

#ifdef USE_ALPHABLEND_LITE
GFX_COLOR _prevAlphaColor = BLACK;
BYTE      _alpha = 100;
#endif

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
void __attribute__((weak)) SetClipRgn(SHORT left, SHORT top, SHORT right, SHORT bottom)
{
    _clipLeft=left;
    _clipTop=top;
    _clipRight=right;
    _clipBottom=bottom;

}

/*********************************************************************
* Function: SetClip(control)
*
* Overview: Enables/disables clipping.
*
* PreCondition: none
*
* Input: control - Enables or disables the clipping.
*			- 0: Disable clipping
*			- 1: Enable clipping
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void __attribute__((weak)) SetClip(BYTE control)
{
    _clipRgn=control;
}

#ifdef USE_ALPHABLEND_LITE
/*********************************************************************
* Function: void BarAlpha(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: alphaPercentage > 0
*
* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: Draws alpha blended bar. The bar is color is alpha blended to the
*           current pixel colors in the display buffer area set by left, top, right
*           and bottom. The alpha blending percentage used is the value set by 
*           SetAlpha(). 
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) BarAlpha(SHORT left, SHORT top,WORD  right, WORD bottom)
{

    SHORT x,y;
    GFX_COLOR  fcolor, bcolor, newColor;
    
    // save current color
    newColor = fcolor = GetColor();

    for(y=top;y<bottom+1;y++)
    {
        if(GetPrevAlphaColor() != BLACK)
        {
            for(x=left;x<right+1;x++)
            {
                bcolor = GetPixel(x,y);
                switch(GetAlpha())
                {
                    case 50: newColor = (bcolor - ConvertColor50(GetPrevAlphaColor())) + ConvertColor50(fcolor);break;
                    case 75: newColor = (bcolor - ConvertColor75(GetPrevAlphaColor())) + ConvertColor75(fcolor);break;
                    case 25: newColor = (bcolor - ConvertColor25(GetPrevAlphaColor())) + ConvertColor25(fcolor);break;
                    default: break;
                }
                SetColor(newColor);
                PutPixel(x,y);
            }     
        }
        else
        {                          
            for(x=left;x<right+1;x++)
            {
                bcolor = GetPixel(x,y);
                switch(GetAlpha())
                {
                    case 50: newColor = ConvertColor50(fcolor) + ConvertColor50(bcolor); break;
                    case 25: newColor = ConvertColor25(fcolor) + ConvertColor75(bcolor); break;
                    case 75: newColor = ConvertColor75(fcolor) + ConvertColor25(bcolor); break;
                    default: break;
                } 
                SetColor(newColor);
                PutPixel(x,y);
            } 
        }        
    }

    // reset to original _color
    SetColor(fcolor); 
    return (1);
}
#endif

/*********************************************************************
* Function: WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: none
*

* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws rectangle filled with current color
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
{
    SHORT   x, y;

#ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    /* Ready */
#else
    if(IsDeviceBusy() != 0)
        return (0);
#endif

#ifdef USE_ALPHABLEND_LITE
    // NOTE: Alpha is never set to 0 so no need to check
    if(GetAlpha() != 100) 
    {
        return (BarAlpha(left,top,right,bottom));
    }
    else 
        // if alpha = 100, then just perform the normal Bar() rendering
        // since pixel colors will be replaced by the SetColor()
#endif
    {
        for(y = top; y < bottom + 1; y++)
            for(x = left; x < right + 1; x++)
                PutPixel(x, y);
    }
    return (1);
}

/*********************************************************************
* Function: WORD Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
*
* PreCondition: none
*
* Input: x1,y1 - starting coordinates, x2,y2 - ending coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws line
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
    SHORT   deltaX, deltaY;
    SHORT   error, stepErrorLT, stepErrorGE;
    SHORT   stepX, stepY;
    SHORT   steep;
    SHORT   temp;
    SHORT   style, type;

        #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    /* Ready */
        #else
    if(IsDeviceBusy() != 0)
        return (0);
        #endif

    // Move cursor
    MoveTo(x2, y2);

    if(x1 == x2)
    {
        if(y1 > y2)
        {
            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        style = 0;
        type = 1;
        for(temp = y1; temp < y2 + 1; temp++)
        {
            if((++style) == _lineType)
            {
                type ^= 1;
                style = 0;
            }

            if(type)
            {
                PutPixel(x1, temp);
                if(_lineThickness)
                {
                    PutPixel(x1 + 1, temp);
                    PutPixel(x1 - 1, temp);
                }
            }
        }

        return (1);
    }

    if(y1 == y2)
    {
        if(x1 > x2)
        {
            temp = x1;
            x1 = x2;
            x2 = temp;
        }

        style = 0;
        type = 1;
        for(temp = x1; temp < x2 + 1; temp++)
        {
            if((++style) == _lineType)
            {
                type ^= 1;
                style = 0;
            }

            if(type)
            {
                PutPixel(temp, y1);
                if(_lineThickness)
                {
                    PutPixel(temp, y1 + 1);
                    PutPixel(temp, y1 - 1);
                }
            }
        }

        return (1);
    }

    stepX = 0;
    deltaX = x2 - x1;
    if(deltaX < 0)
    {
        deltaX = -deltaX;
        --stepX;
    }
    else
    {
        ++stepX;
    }

    stepY = 0;
    deltaY = y2 - y1;
    if(deltaY < 0)
    {
        deltaY = -deltaY;
        --stepY;
    }
    else
    {
        ++stepY;
    }

    steep = 0;
    if(deltaX < deltaY)
    {
        ++steep;
        temp = deltaX;
        deltaX = deltaY;
        deltaY = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
        temp = stepX;
        stepX = stepY;
        stepY = temp;
        PutPixel(y1, x1);
    }
    else
    {
        PutPixel(x1, y1);
    }

    // If the current error greater or equal zero
    stepErrorGE = deltaX << 1;

    // If the current error less than zero
    stepErrorLT = deltaY << 1;

    // Error for the first pixel
    error = stepErrorLT - deltaX;

    style = 0;
    type = 1;

    while(--deltaX >= 0)
    {
        if(error >= 0)
        {
            y1 += stepY;
            error -= stepErrorGE;
        }

        x1 += stepX;
        error += stepErrorLT;

        if((++style) == _lineType)
        {
            type ^= 1;
            style = 0;
        }

        if(type)
        {
            if(steep)
            {
                PutPixel(y1, x1);
                if(_lineThickness)
                {
                    PutPixel(y1 + 1, x1);
                    PutPixel(y1 - 1, x1);
                }
            }
            else
            {
                PutPixel(x1, y1);
                if(_lineThickness)
                {
                    PutPixel(x1, y1 + 1);
                    PutPixel(x1, y1 - 1);
                }
            }
        }
    }   // end of while

    return (1);
}

/*********************************************************************
* Function: void ClearDevice(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: clears screen with current color and sets cursor to 0,0
*
* Note: none
*
********************************************************************/
void __attribute__((weak)) ClearDevice(void)
{
    while(Bar(0, 0, GetMaxX(), GetMaxY()) == 0);
    MoveTo(0, 0);
}

/*********************************************************************
* Function:  void InitGraph(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: initializes LCD controller,
*           sets cursor position to upper left corner,
*           sets active and visual pages to page 0,
*           clears active page with BLACK,
*           sets color to WHITE,
*           disables clipping
*
* Note: none
*
********************************************************************/
void InitGraph(void)
{

    // Current line type
    SetLineType(SOLID_LINE);

    // Current line thickness
    SetLineThickness(NORMAL_LINE);

    // Current cursor coordinates to 0,0
    MoveTo(0, 0);

    // Reset device
    ResetDevice();

    // Set color to BLACK
    SetColor(0);

    // set the transparent color check to be disabled
#ifdef USE_TRANSPARENT_COLOR
    TransparentColorDisable();
#endif    

#ifdef USE_ALPHABLEND_LITE
    SetAlpha(100);
#endif

    // Clear screen
    ClearDevice();

    // Disable clipping
    SetClip(CLIP_DISABLE);

    // Set font orientation
    SetFontOrientation(ORIENT_HOR);
    
    // set Bevel drawing 
    SetBevelDrawType(DRAWFULLBEVEL);
}

/*********************************************************************
* Function: WORD Arc(SHORT xL, SHORT yT, SHORT xR, SHORT yB, SHORT r1, SHORT r2, BYTE octant);
*
* PreCondition: none
*
* Input: xL, yT - location of the upper left center in the x,y coordinate
*		 xR, yB - location of the lower right left center in the x,y coordinate
*		 r1, r2 - the two concentric circle radii, r1 as the radius 
*				  of the smaller circle and and r2 as the radius of the 
*				  larger circle.
*		 octant - bitmask of the octant that will be drawn.
*				  Moving in a clockwise direction from x = 0, y = +radius
*                 bit0 : first octant 	bit4 : fifth octant
*                 bit1 : second octant  bit5 : sixth octant
*                 bit2 : third octant   bit6 : seventh octant
*                 bit3 : fourth octant  bit7 : eigth octant
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: Draws the octant arc of a beveled figure with given centers, radii
*			and octant mask. When r1 is zero and r2 has some value, a filled 
*			circle is drawn; when the radii have values, an arc of
*			thickness (r2-r1) is drawn; when octant = 0xFF, a full ring 
*			is drawn. When r1 and r2 are zero, a rectangular object is drawn, where
*			xL, yT specifies the left top corner; xR, yB specifies the right bottom
*			corner.
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) Arc(SHORT xL, SHORT yT, SHORT xR, SHORT yB, SHORT r1, SHORT r2, BYTE octant)
{

    // this is using a variant of the Midpoint (Bresenham's) Algorithm
    #ifndef USE_NONBLOCKING_CONFIG

    SHORT       y1Limit, y2Limit;
    SHORT       x1, x2, y1, y2, ovrlap;
    SHORT       err1, err2;
    SHORT       x1Cur, y1Cur, y1New;
    SHORT       x2Cur, y2Cur, y2New;
    DWORD_VAL   temp;

    temp.Val = SIN45 * r1;
    y1Limit = temp.w[1];
    temp.Val = SIN45 * r2;
    y2Limit = temp.w[1];

    temp.Val = (DWORD) (ONEP25 - ((LONG) r1 << 16));
    err1 = (SHORT) (temp.w[1]);

    temp.Val = (DWORD) (ONEP25 - ((LONG) r2 << 16));
    err2 = (SHORT) (temp.w[1]);

    x1 = r1;
    x2 = r2;
    y1 = 0;
    y2 = 0;

    x1Cur = x1;
    y1Cur = y1 + 1;
    y1New = y1;
    x2Cur = x2;
    y2Cur = y2;
    y2New = y2;

    while(y2 <= y2Limit)
    {   // just watch for y2 limit since outer circle
        // will have greater value.
        // Drawing of the rounded panel is done only when there is a change in the
        // x direction. Bars are drawn to be efficient.
        // detect changes in the x position. Every change will mean a bar will be drawn
        // to cover the previous area. y1New records the last position of y before the
        // change in x position.
        // y1New & y2New records the last y positions, must remember this to
        // draw the correct bars (non-overlapping).
        y1New = y1;
        y2New = y2;

        if(y1 <= y1Limit)
        {
            if(err1 > 0)
            {
                x1--;
                err1 += 5;
                err1 += (y1 - x1) << 1;
            }
            else
            {
                err1 += 3;
                err1 += y1 << 1;
            }

            y1++;
        }
        else
        {
            y1++;
            if(x1 < y1)
                x1 = y1;
        }

        if(err2 > 0)
        {
            x2--;
            err2 += 5;
            err2 += (y2 - x2) << 1;
        }
        else
        {
            err2 += 3;
            err2 += y2 << 1;
        }

        y2++;

        if((x1Cur != x1) || (x2Cur != x2))
        {
            if(octant & 0x01)
            {
                // check overlaps
                if (yT - x1Cur < yT - y1New)
                    ovrlap = yT - x1Cur;
                else    
                    ovrlap = yT - y1New - 1;
                Bar(xR + y2Cur, yT - x2Cur, xR + y1New, ovrlap);    // 1st octant
            }

            if(octant & 0x02)
            {
                // check overlaps
                if (xR + x1Cur > xR + y1New)
                    ovrlap = xR + x1Cur;
                else    
                    ovrlap = xR + y1New;
                Bar(ovrlap, yT - y1New, xR + x2Cur, yT - y2Cur);    // 2nd octant
            }

            if(octant & 0x04)
            {
                // check overlaps
                if (xR + x1Cur > xR + y2New)
                    ovrlap = xR + x1Cur;
                else    
                    ovrlap = xR + y2New;
                Bar(ovrlap, yB + y1Cur, xR + x2Cur, yB + y2New);    // 3rd octant
            }

            if(octant & 0x08)
            {
                // check overlaps
                if (yB + x1Cur > yB + y2New)
                    ovrlap = yB + x1Cur;
                else    
                    ovrlap = yB + y2New + 1;
                Bar(xR + y1Cur, ovrlap, xR + y2New, yB + x2Cur);    // 4th octant
            }

            if(octant & 0x10)
            {
                // check overlaps
                if (yB + x1Cur > yB + y1New)
                    ovrlap = yB + x1Cur;
                else    
                    ovrlap = yB + y1New + 1;
                Bar(xL - y1New, ovrlap, xL - y2Cur, yB + x2Cur);    // 5th octant
            }

            if(octant & 0x20)
            {
                // check overlaps
                if (xL - x1Cur < xL - y1New)
                    ovrlap = xL - x1Cur;
                else    
                    ovrlap = xL - y1New;
                Bar(xL - x2Cur, yB + y2Cur, ovrlap, yB + y1New);    // 6th octant
            }

            if(octant & 0x40)
            {
                // check overlaps
                if (xL - x1Cur < xL - y2New)
                    ovrlap = xL - x1Cur;
                else    
                    ovrlap = xL - y2New;
                Bar(xL - x2Cur, yT - y2New, ovrlap, yT - y1Cur);    // 7th octant
            }

            if(octant & 0x80)
            {
                // check overlaps
                if (yT - x1Cur < yT - y2New)
                    ovrlap = yT - x1Cur;
                else    
                    ovrlap = yT - y2New - 1;
                Bar(xL - y2New, yT - x2Cur, xL - y1Cur, ovrlap);    // 8th octant
            }

            // update current values
            x1Cur = x1;
            y1Cur = y1;
            x2Cur = x2;
            y2Cur = y2;

        }
    }                           // end of while loop

    // draw the width and height
    if((xR - xL) || (yB - yT))
    {
        // draw right
        if(octant & 0x02)
        {
            Bar(xR + r1, yT + 1, xR + r2, (yB + yT) >> 1);
        }

        if(octant & 0x04)
        {
            Bar(xR + r1, ((yB + yT) >> 1) + 1, xR + r2, yB);
        }

        // draw bottom
        if(octant & 0x10)
        {
            Bar(xL + 1, yB + r1, ((xR + xL) >> 1), yB + r2);
        }

        if(octant & 0x08)
        {
            Bar(((xR + xL) >> 1) + 1, yB + r1, xR, yB + r2);
        }

        if(xR - xL)
        {

            // draw top
            if(octant & 0x80)
            {
                Bar(xL, yT - r2, ((xR + xL) >> 1), yT - r1);
            }

            if(octant & 0x01)
            {
                Bar(((xR + xL) >> 1) + 1, yT - r2, xR - 1, yT - r1);
            }
        }

        if(yT - yB)
        {

            // draw left
            if(octant & 0x40)
            {
                Bar(xL - r2, yT, xL - r1, ((yB + yT) >> 1));
            }

            if(octant & 0x20)
            {
                Bar(xL - r2, ((yB + yT) >> 1) + 1, xL - r1, yB - 1);
            }
        }
    }

    return (1);
    #else

    typedef enum
    {
        BEGIN,
        QUAD11,
        BARRIGHT1,
        QUAD12,
        BARRIGHT2,
        QUAD21,
        BARLEFT1,
        QUAD22,
        BARLEFT2,
        QUAD31,
        BARTOP1,
        QUAD32,
        BARTOP2,
        QUAD41,
        BARBOTTOM1,
        QUAD42,
        BARBOTTOM2,
        CHECK,
    } OCTANTARC_STATES;

    DWORD_VAL temp;

    //	LONG temp1;
    static SHORT y1Limit, y2Limit;
    static SHORT x1, x2, y1, y2, ovrlap;
    static SHORT err1, err2;
    static SHORT x1Cur, y1Cur, y1New;
    static SHORT x2Cur, y2Cur, y2New;
    static OCTANTARC_STATES state = BEGIN;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);
        switch(state)
        {
            case BEGIN:
                temp.Val = SIN45 * r1;
                y1Limit = temp.w[1];
                temp.Val = SIN45 * r2;
                y2Limit = temp.w[1];

                temp.Val = (DWORD) (ONEP25 - ((LONG) r1 << 16));
                err1 = (SHORT) (temp.w[1]);

                temp.Val = (DWORD) (ONEP25 - ((LONG) r2 << 16));
                err2 = (SHORT) (temp.w[1]);

                x1 = r1;
                x2 = r2;
                y1 = 0;
                y2 = 0;

                x1Cur = x1;
                y1Cur = y1 + 1;
                y1New = y1;
                x2Cur = x2;
                y2Cur = y2;
                y2New = y2;
                state = CHECK;

            case CHECK:
                arc_check_state : if(y2 > y2Limit)
                {
                    state = BARRIGHT1;
                    goto arc_draw_width_height_state;
                }

                // y1New & y2New records the last y positions
                y1New = y1;
                y2New = y2;

                if(y1 <= y1Limit)
                {
                    if(err1 > 0)
                    {
                        x1--;
                        err1 += 5;
                        err1 += (y1 - x1) << 1;
                    }
                    else
                    {
                        err1 += 3;
                        err1 += y1 << 1;
                    }

                    y1++;
                }
                else
                {
                    y1++;
                    if(x1 < y1)
                        x1 = y1;
                }

                if(err2 > 0)
                {
                    x2--;
                    err2 += 5;
                    err2 += (y2 - x2) << 1;
                }
                else
                {
                    err2 += 3;
                    err2 += y2 << 1;
                }

                y2++;

                state = QUAD11;

                //break;

            case QUAD11:
                if((x1Cur != x1) || (x2Cur != x2))
                {

                    // 1st octant
                    if(octant & 0x01)
                    {
                        // check overlaps
                        if (yT - x1Cur < yT - y1New)
                            ovrlap = yT - x1Cur;
                        else    
                            ovrlap = yT - y1New - 1;
                        if(Bar(xR + y2Cur, yT - x2Cur, xR + y1New, ovrlap) == 0)
                            return 0;    
                    }
                }
                else
                {
                    state = CHECK;
                    goto arc_check_state;
                }

                state = QUAD12;
                break;

            case QUAD12:

                // 2nd octant
                if(octant & 0x02)
                {
                    // check overlaps
                    if (xR + x1Cur > xR + y1New)
                        ovrlap = xR + x1Cur;
                    else    
                        ovrlap = xR + y1New;
                    if(Bar(ovrlap, yT - y1New, xR + x2Cur, yT - y2Cur) == 0)
                        return 0;    
                }

                state = QUAD21;
                break;

            case QUAD21:

                // 3rd octant
                if(octant & 0x04)
                {
                    // check overlaps
                    if (xR + x1Cur > xR + y2New)
                        ovrlap = xR + x1Cur;
                    else    
                        ovrlap = xR + y2New;
                    if(Bar(ovrlap, yB + y1Cur, xR + x2Cur, yB + y2New) == 0)
                        return 0;    
                }

                state = QUAD22;
                break;

            case QUAD22:

                // 4th octant
                if(octant & 0x08)
                {

                    // check overlaps
                    if (yB + x1Cur > yB + y2New)
                        ovrlap = yB + x1Cur;
                    else    
                        ovrlap = yB + y2New + 1;
                    if(Bar(xR + y1Cur, ovrlap, xR + y2New, yB + x2Cur) == 0)
                        return 0;    
                }

                state = QUAD31;
                break;

            case QUAD31:

                // 5th octant
                if(octant & 0x10)
                {
                    // check overlaps
                    if (yB + x1Cur > yB + y1New)
                        ovrlap = yB + x1Cur;
                    else    
                        ovrlap = yB + y1New + 1;
                    if(Bar(xL - y1New, ovrlap, xL - y2Cur, yB + x2Cur) == 0)
                        return 0;    
                }

                state = QUAD32;
                break;

            case QUAD32:

                // 6th octant
                if(octant & 0x20)
                {
                    // check overlaps
                    if (xL - x1Cur < xL - y1New)
                        ovrlap = xL - x1Cur;
                    else    
                        ovrlap = xL - y1New;
                    if(Bar(xL - x2Cur, yB + y2Cur, ovrlap, yB + y1New) == 0)
                        return 0;
                }

                state = QUAD41;
                break;

            case QUAD41:

                // 7th octant
                if(octant & 0x40)
                {
                    // check overlaps
                    if (xL - x1Cur < xL - y2New)
                        ovrlap = xL - x1Cur;
                    else    
                        ovrlap = xL - y2New;
                    if(Bar(xL - x2Cur, yT - y2New, ovrlap, yT - y1Cur) == 0)
                        return 0;
                }

                state = QUAD42;
                break;

            case QUAD42:

                // 8th octant
                if(octant & 0x80)
                {
                    // check overlaps
                    if (yT - x1Cur < yT - y2New)
                        ovrlap = yT - x1Cur;
                    else    
                        ovrlap = yT - y2New - 1;
                    if(Bar(xL - y2New, yT - x2Cur, xL - y1Cur, ovrlap) == 0)
                        return 0;    
                }

                // update current values
                x1Cur = x1;
                y1Cur = y1;
                x2Cur = x2;
                y2Cur = y2;
                state = CHECK;
                break;

            case BARRIGHT1:     // draw upper right
                arc_draw_width_height_state : if((xR - xL) || (yB - yT))
                {

                    // draw right
                    if(octant & 0x02)
                    {
                        if(Bar(xR + r1, yT + 1, xR + r2, (yB + yT) >> 1) == 0)
                            return 0;
                    }
                }
                else
                {
                    state = BEGIN;
                    return (1);
                }

                state = BARRIGHT2;
                break;

            case BARRIGHT2:     // draw lower right
                if(octant & 0x04)
                {
                    if(Bar(xR + r1, ((yB + yT) >> 1) + 1, xR + r2, yB) == 0)
                        return (0);
                }

                state = BARBOTTOM1;
                break;

            case BARBOTTOM1:    // draw left bottom
                // draw bottom
                if(octant & 0x10)
                {
                    if(Bar(xL + 1, yB + r1, ((xR + xL) >> 1), yB + r2) == 0)
                        return (0);
                }

                state = BARBOTTOM2;
                break;

            case BARBOTTOM2:    // draw right bottom
                if(octant & 0x08)
                {
                    if(Bar(((xR + xL) >> 1) + 1, yB + r1, xR, yB + r2) == 0)
                        return (0);
                }

                state = BARTOP1;
                break;

            case BARTOP1:       // draw left top
                if(xR - xL)
                {

                    // draw top
                    if(octant & 0x80)
                    {
                        if(Bar(xL, yT - r2, ((xR + xL) >> 1), yT - r1) == 0)
                            return (0);
                    }

                    state = BARTOP2;
                }
                else
                    state = BARLEFT1;   // no width go directly to height bar
                break;

            case BARTOP2:               // draw right top
                if(octant & 0x01)
                {
                    if(Bar(((xR + xL) >> 1) + 1, yT - r2, xR - 1, yT - r1) == 0)
                        return (0);
                }

                state = BARLEFT1;
                break;

            case BARLEFT1:              // draw upper left
                if(yT - yB)
                {

                    // draw left
                    if(octant & 0x40)
                    {
                        if(Bar(xL - r2, yT, xL - r1, ((yB + yT) >> 1)) == 0)
                            return (0);
                    }

                    state = BARLEFT2;
                }
                else
                {
                    state = BEGIN;      // no height go back to BEGIN
                    return (1);
                }

                break;

            case BARLEFT2:              // draw lower left
                if(octant & 0x20)
                {
                    if(Bar(xL - r2, ((yB + yT) >> 1) + 1, xL - r1, yB - 1) == 0)
                        return (0);
                }

                state = BEGIN;
                return (1);
        }                               // end of switch
    }   // end of while
    #endif // USE_NONBLOCKING_CONFIG
}

/*********************************************************************
* Function: WORD Bevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
*
* PreCondition: None
*
* Input: x1, y1 - coordinate position of the upper left center of the 
* 				  circle that draws the rounded corners,
*		 x2, y2 - coordinate position of the lower right center of the 
* 				  circle that draws the rounded corners,
*        rad - defines the redius of the circle,
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Overview: Draws a beveled figure on the screen. 
*           For a pure circular object x1 = x2 and y1 = y2. 
*           For a rectangular object radius = 0.
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) Bevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
{
    SHORT       style, type, xLimit, xPos, yPos, error;
    DWORD_VAL   temp;

    #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    /* Ready */
    #else
    if(IsDeviceBusy() != 0)
        return (0);
    #endif
    temp.Val = SIN45 * rad;
    xLimit = temp.w[1] + 1;
    temp.Val = (DWORD) (ONEP25 - ((LONG) rad << 16));
    error = (SHORT) (temp.w[1]);
    yPos = rad;

    style = 0;
    type = 1;

    if(rad)
    {
        for(xPos = 0; xPos <= xLimit; xPos++)
        {
            if((++style) == _lineType)
            {
                type ^= 1;
                style = 0;
            }

            if(type)
            {
                PutPixel(x2 + xPos, y1 - yPos);         // 1st quadrant
                PutPixel(x2 + yPos, y1 - xPos);
                PutPixel(x2 + xPos, y2 + yPos);         // 2nd quadrant
                PutPixel(x2 + yPos, y2 + xPos);
                PutPixel(x1 - xPos, y2 + yPos);         // 3rd quadrant
                PutPixel(x1 - yPos, y2 + xPos);
                PutPixel(x1 - yPos, y1 - xPos);         // 4th quadrant
                PutPixel(x1 - xPos, y1 - yPos);

                if(_lineThickness)
                {
                    PutPixel(x2 + xPos, y1 - yPos - 1); // 1st quadrant
                    PutPixel(x2 + xPos, y1 - yPos + 1);
                    PutPixel(x2 + yPos + 1, y1 - xPos);
                    PutPixel(x2 + yPos - 1, y1 - xPos);
                    PutPixel(x2 + xPos, y2 + yPos - 1); // 2nd quadrant
                    PutPixel(x2 + xPos, y2 + yPos + 1);
                    PutPixel(x2 + yPos + 1, y2 + xPos);
                    PutPixel(x2 + yPos - 1, y2 + xPos);
                    PutPixel(x1 - xPos, y2 + yPos - 1); // 3rd quadrant
                    PutPixel(x1 - xPos, y2 + yPos + 1);
                    PutPixel(x1 - yPos + 1, y2 + xPos);
                    PutPixel(x1 - yPos - 1, y2 + xPos);
                    PutPixel(x1 - yPos + 1, y1 - xPos); // 4th quadrant
                    PutPixel(x1 - yPos - 1, y1 - xPos);
                    PutPixel(x1 - xPos, y1 - yPos + 1);
                    PutPixel(x1 - xPos, y1 - yPos - 1);
                }
            }

            if(error > 0)
            {
                yPos--;
                error += 5 + ((xPos - yPos) << 1);
            }
            else
                error += 3 + (xPos << 1);
        }
    }
    // Must use lines here since this can also be used to draw focus of round buttons
    if(x2 - x1)
    {
        while(!Line(x1, y1 - rad, x2, y1 - rad));

        // draw top
    }

    if(y2 - y1)
    {
        while(!Line(x1 - rad, y1, x1 - rad, y2));

        // draw left
    }

    if((x2 - x1) || (y2 - y1))
    {
        while(!Line(x2 + rad, y1, x2 + rad, y2));

        // draw right
        while(!Line(x1, y2 + rad, x2, y2 + rad));

        // draw bottom
    }

    return (1);
}

/*********************************************************************
* Function: WORD FillBevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
*
* PreCondition: None
*
* Input: x1, y1 - coordinate position of the upper left center of the 
* 				  circle that draws the rounded corners,
*		 x2, y2 - coordinate position of the lower right center of the 
* 				  circle that draws the rounded corners,
*        rad - defines the redius of the circle,
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Overview: Draws a filled beveled figure on the screen. 
*           For a filled circular object x1 = x2 and y1 = y2. 
*           For a filled rectangular object radius = 0.
*
* Note: none
*
********************************************************************/
WORD FillBevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
{

    #ifndef USE_NONBLOCKING_CONFIG

    SHORT       yLimit, xPos, yPos, err;
    SHORT       xCur, yCur, yNew;
    SHORT       last54Y, last18Y;
    DWORD_VAL   temp;


    // this covers filled rectangle.
    if (_bevelDrawType == DRAWFULLBEVEL) 
	    Bar(x1 - rad, y1, x2 + rad, y2);
    else if (_bevelDrawType == DRAWTOPBEVEL)    
	    Bar(x1 - rad, y1, x2 + rad, y1+((y2-y1)>>1));
    else    
	    Bar(x1 - rad, y1+((y2-y1)>>1), x2 + rad, y2);
	 
    // note that octants here is defined as:
    // from yPos=-radius, xPos=0 in the clockwise direction octant 1 to 8 are labeled
    // assumes an origin at 0,0. Quadrants are defined in the same manner
    if(rad)
    {
        temp.Val = SIN45 * rad;
        yLimit = temp.w[1];
        temp.Val = (DWORD) (ONEP25 - ((LONG) rad << 16));
        err = (SHORT) (temp.w[1]);
        xPos = rad;
        yPos = 0;

        // yCur starts at 1 so the center line is not drawn and will be drawn separately
        // this is to avoid rendering twice on the same line
        yCur = 1;           
        xCur = xPos;
        yNew = yPos;

        // note initial values are important
        last54Y = GetMaxY();
        last18Y = 0;

        while(yPos <= yLimit)
        {
            // Drawing of the rounded panel is done only when there is a change in the
            // x direction. Bars are drawn to be efficient.
            // detect changes in the x position. Every change will mean a bar will be drawn
            // to cover the previous area. yNew records the last position of y before the
            // change in x position.
            yNew = yPos;

            if(err > 0)
            {
                xPos--;
                err += 5 + ((yPos - xPos) << 1);
            }
            else
                err += 3 + (yPos << 1);
            yPos++;

            if(xCur != xPos)
            {
	            if (_bevelDrawType & DRAWBOTTOMBEVEL) 
	            { 
	                // 6th octant to 3rd octant
                    // check first if there will be an overlap
                    if (y2 + yNew > last54Y)
    	                Bar(x1 - xCur, y2 + yCur, x2 + xCur, last54Y - 1);
                    else
    	                Bar(x1 - xCur, y2 + yCur, x2 + xCur, y2 + yNew);
            
   	                // 5th octant to 4th octant
                    // check: if overlapping then no need to proceed
                    if ((y2 + xCur) > (y2 + yNew))
                    {
   	                    Bar(x1 - yNew, y2 + xCur, x2 + yNew, y2 + xCur);
                        last54Y = (y2 + xCur);
  
                    }
                    
				}

	            if (_bevelDrawType & DRAWTOPBEVEL) 
				{
                    // 7th octant to 2nd octant
                    // check: if overlapping then no need to proceed
                    if (y1 - yNew < last18Y)
	                    Bar(x1 - xCur, last18Y + 1, x2 + xCur, y1 - yCur);
                    else
	                    Bar(x1 - xCur, y1 - yNew, x2 + xCur, y1 - yCur);

	                // 8th octant to 1st octant
                    // check first if there will be an overlap
                    if ((y1 - xCur) < (y1 - yNew))
                    {
    	                Bar(x1 - yNew, y1 - xCur, x2 + yNew, y1 - xCur);
                        last18Y = y1 - xCur;    
                    }

				}
                // update current values
                xCur = xPos;
                yCur = yPos;
            }
        }

        if ((y1 - yNew) > (y1 - xPos)) 
        {
            // there is a missing line, draw that line
            if (_bevelDrawType & DRAWTOPBEVEL)
                Bar(x1 - yNew, y1 - xPos, x2 + yNew, y1 - xPos);
        }
        if ((y2 + yNew) < (y2 + xPos)) 
        {
            // there is a missing line, draw that line
            if (_bevelDrawType & DRAWBOTTOMBEVEL)
                Bar(x1 - yNew, y2 + xPos, x2 + yNew, y2 + xPos);
        }
    }
   
    return (1);

    #else

    typedef enum
    {
        FB_BEGIN,
        FB_INIT_FILL,
        FB_CHECK,
        FB_Q8TOQ1,
        FB_Q7TOQ2,
        FB_Q6TOQ3,
        FB_Q5TOQ4,
        FB_DRAW_MISSING_1,    
        FB_DRAW_MISSING_2,    
        FB_WAITFORDONE,        
    } FB_FILLCIRCLE_STATES;

    static SHORT err;
    static SHORT yLimit, xPos, yPos;
    static SHORT xCur, yCur, yNew;
    static SHORT last54Y, last18Y;
    DWORD_VAL temp;

    static FB_FILLCIRCLE_STATES state = FB_BEGIN;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);

        switch(state)
        {
            case FB_BEGIN:

                // draw the face
                if (_bevelDrawType == DRAWFULLBEVEL)
				{ 
				    if(Bar(x1 - rad, y1, x2 + rad, y2) == 0)
	                    return (0);
				}
                else if (_bevelDrawType == DRAWTOPBEVEL)
				{    
				    if(Bar(x1 - rad, y1, x2 + rad, y1+((y2-y1)>>1)) == 0)
	                    return (0);
                }
				else    
				{
				    if(Bar(x1 - rad, y1+((y2-y1)>>1), x2 + rad, y2) == 0)
	                    return (0);
    			}            
                state = FB_INIT_FILL;
                break;
            
            case FB_INIT_FILL:

                if(!rad)
                {   // no radius object is a filled rectangle
                    state = FB_WAITFORDONE;
                    break;
                }

                // compute variables
                temp.Val = SIN45 * rad;
                yLimit = temp.w[1];
                temp.Val = (DWORD) (ONEP25 - ((LONG) rad << 16));
                err = (SHORT) (temp.w[1]);
                xPos = rad;
                yPos = 0;

                // yCur starts at 1 so the center line is not drawn and will be drawn separately
                // this is to avoid rendering twice on the same line
                yCur = 1;           
                xCur = xPos;
                yNew = yPos;
    
                // note initial values are important
                last54Y = GetMaxY();
                last18Y = 0;

                state = FB_CHECK;

            case FB_CHECK:

                // check first for limits
                if(yPos > yLimit)
                {
                    if (rad)
                        state = FB_DRAW_MISSING_1;
                    else
                        state = FB_WAITFORDONE;
                    break;
                }

                // yNew records the last y position
                yNew = yPos;

                // calculate the next value of x and y
                if(err > 0)
                {
                    xPos--;
                    err += 5 + ((yPos - xPos) << 1);
                }
                else
                    err += 3 + (yPos << 1);
                yPos++;
                state = FB_Q6TOQ3;

            case FB_Q6TOQ3:

                if(xCur != xPos)
                {

                    // 6th octant to 3rd octant
	            	if (_bevelDrawType & DRAWBOTTOMBEVEL) 
	            	{ 

                        // check first if there will be an overlap
                        if (y2 + yNew > last54Y)
                        {
        	                if(Bar(x1 - xCur, y2 + yCur, x2 + xCur, last54Y - 1) == 0)
                                return 0;
                        }        
                        else
                        {
        	               if (Bar(x1 - xCur, y2 + yCur, x2 + xCur, y2 + yNew) == 0)
                                return 0;
                        }
              		}          	
                    state = FB_Q5TOQ4;
                    break;
                }

                state = FB_CHECK;
                break;

            case FB_Q5TOQ4:

            	if (_bevelDrawType & DRAWBOTTOMBEVEL) 
            	{ 

   	                // 5th octant to 4th octant
                    // check: if overlapping then no need to proceed
                    if ((y2 + xCur) > (y2 + yNew))
                    {
   	                    if (Bar(x1 - yNew, y2 + xCur, x2 + yNew, y2 + xCur) == 0)
                            return 0;
                        last54Y = (y2 + xCur);
  
                    }

				}
                state = FB_Q7TOQ2;
                break;

            case FB_Q7TOQ2:

                // 7th octant to 2nd octant
	            if (_bevelDrawType & DRAWTOPBEVEL) 
				{
                    // check: if overlapping then no need to proceed
                    if (y1 - yNew < last18Y)
                    {
	                    if (Bar(x1 - xCur, last18Y + 1, x2 + xCur, y1 - yCur) == 0)
                            return 0;
                    }
                    else
                    {
	                    if (Bar(x1 - xCur, y1 - yNew, x2 + xCur, y1 - yCur) == 0)   
                            return 0;
                    }

				}
                state = FB_Q8TOQ1;
                break;

            case FB_Q8TOQ1:

                // 8th octant to 1st octant
	            if (_bevelDrawType & DRAWTOPBEVEL) 
				{

                    // check first if there will be an overlap
                    if ((y1 - xCur) < (y1 - yNew))
                    {
    	                if (Bar(x1 - yNew, y1 - xCur, x2 + yNew, y1 - xCur) == 0)
                            return 0;
                        last18Y = y1 - xCur;    
                    }

    			}                	
                // update current values
                xCur = xPos;
                yCur = yPos;

                state = FB_CHECK;
                break;

            case FB_DRAW_MISSING_1:
                // check only one side since they are the same    
                if ((y1 - yNew) > (y1 - xPos)) 
                {
                    // there is a missing line, draw that line
                    if (_bevelDrawType & DRAWTOPBEVEL)
                        if (Bar(x1 - yNew, y1 - xPos, x2 + yNew, y1 - xPos) == 0)
                            return 0;
                    
                    state = FB_DRAW_MISSING_2;
                }
                else
                    state = FB_WAITFORDONE;
                break;

            case FB_DRAW_MISSING_2:
                if ((y2 + yNew) < (y2 + xPos)) 
                {
                    if (_bevelDrawType & DRAWBOTTOMBEVEL)
                        if (Bar(x1 - yNew, y2 + xPos, x2 + yNew, y2 + xPos) == 0)

                            return 0;
                }
                state = FB_WAITFORDONE;

            case FB_WAITFORDONE:
                if(IsDeviceBusy())
                    return (0);
                
                state = FB_BEGIN;
                return (1);

        }           // end of switch
    }               // end of while
    #endif // end of USE_NONBLOCKING_CONFIG
}

/*********************************************************************
* Function: WORD DrawPoly(SHORT numPoints, SHORT* polyPoints)
*
* PreCondition: none
*
* Input: numPoints - points number, polyPoints - pointer to points array
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws line polygon
*
* Note: This function has a weak attribute, the driver layer
*       may implement this same function to support driver layer
*       features.
*
********************************************************************/
WORD __attribute__((weak)) DrawPoly(SHORT numPoints, SHORT *polyPoints)
{
#ifndef USE_NONBLOCKING_CONFIG

    SHORT   counter;
    SHORT   sx, sy, ex, ey;

    while(IsDeviceBusy() != 0) Nop();

    sx = *polyPoints++;
    sy = *polyPoints++;
    for(counter = 0; counter < numPoints - 1; counter++)
    {
        ex = *polyPoints++;
        ey = *polyPoints++;
        while(Line(sx, sy, ex, ey) == 0);
        sx = ex;
        sy = ey;
    }

#else

    typedef enum
    {
        POLY_BEGIN,
        POLY_DRAWING,
    } DRAWPOLY_STATES;

    static DRAWPOLY_STATES state = POLY_BEGIN;
    static SHORT counter;
    SHORT sx, sy, ex, ey;
    while(1)
    {
        if(IsDeviceBusy())
            return (0);
        switch(state)
        {
            case POLY_BEGIN:
                counter = 1;
                state = POLY_DRAWING;

            case POLY_DRAWING:
                if(counter == 0 || counter >= numPoints)
                {
                    state = POLY_BEGIN;
                    break;
                }

                while(counter < numPoints)
                {
                    sx = polyPoints[(counter - 1) * 2];
                    sy = polyPoints[(counter * 2) - 1];
                    ex = polyPoints[counter * 2];
                    ey = polyPoints[counter * 2 + 1];
                    if(Line(sx, sy, ex, ey) == 0)
                    {
                        return (0);
                    }

                    counter++;
                }

                state = POLY_BEGIN;
                return (1);
        }
    }
#endif
    return (1);
}

/*********************************************************************
* Function: void SetFontFlash(void* pFont)
*
* PreCondition: none
*
* Input: pFont - pointer to the font image in FLASH.
*
* Output: none
*
* Side Effects: none
*
* Overview: Sets the current font located in FLASH.
*
* Note: This function has a weak attribute, the driver layer
*       may implement this same function to support driver layer
*       features.
*
********************************************************************/
void __attribute__((weak)) SetFontFlash(void *pFont)
{
#if defined (USE_FONT_FLASH)
    GFX_FONT_SPACE BYTE *src;
    BYTE *dest;
    int i;

    src = (GFX_FONT_SPACE BYTE *)((FONT_FLASH *)pFont)->address;
    dest = (BYTE *)&(currentFont.fontHeader);
    for(i = 0; i < sizeof(FONT_HEADER); i++)
    {
        *dest = *src;
        src++;
        dest++;
    }
    currentFont.pFont = pFont;
#endif
#if defined (USE_FONT_RAM)
    memcpy( &(currentFont.fontHeader), 
            (FONT_HEADER *) ((FONT_RAM *)pFont)->address, 
            sizeof(FONT_HEADER));
    currentFont.pFont = pFont;
#endif
}

/*********************************************************************
* Function: void SetFontExternal(void* pFont)
*
* PreCondition: none
*
* Input: pFont - pointer to the font image located in External Memory.
*
* Output: none
*
* Side Effects: none
*
* Overview: Sets the current font located in External Memory.
*           When using fonts in external memory, the font glyph
*           buffer size defined by EXTERNAL_FONT_BUFFER_SIZE must
*           be big enough for the character glyphs.
*
* Note: This function has a weak attribute, the driver layer
*       may implement this same function to support driver layer
*       features.
*
********************************************************************/
void __attribute__((weak)) SetFontExternal(void *pFont)
{
#if defined (USE_FONT_EXTERNAL)
    FONT_HEADER *pHeader;

    pHeader = &(currentFont.fontHeader);
    ExternalMemoryCallback(pFont, 0, sizeof(FONT_HEADER), pHeader);
    currentFont.pFont = pFont;
#endif
}

/*********************************************************************
* Function: void SetFont(void* pFont)
*
* PreCondition: none
*
* Input: pointer to the font image
*
* Output: none
*
* Side Effects: none
*
* Overview: defines current font
*
* Note: This function has a weak attribute, the driver layer
*       may implement this same function to support driver layer
*       features.
*
********************************************************************/
void __attribute__((weak)) SetFont(void *pFont)
{
    switch(*((SHORT *)pFont))
    {
        case FLASH:
        case RAM:
            SetFontFlash(pFont);
            break;
				
        case EXTERNAL:
            SetFontExternal(pFont);
            break;

        default:
            break;
    }
}

#ifdef USE_ANTIALIASED_FONTS

static GFX_COLOR   _fgcolor100;
static GFX_COLOR   _fgcolor25;
static GFX_COLOR   _fgcolor75;
static GFX_COLOR   _bgcolor100;
static GFX_COLOR   _bgcolor25;
static GFX_COLOR   _bgcolor75;

/*********************************************************************
* Function: static void __attribute__((always_inline)) calculateColors(void)
*
* PreCondition: _bgcolor100 and _fgcolor100 must be set
*
* Input: none
*
* Output: none
*
* Side Effects: _bgcolor25, _bgcolor75, _fgcolor25 and _fgcolor75 will be calculated
*
* Overview: calculates mid values of colors
*
* Note: Internal to this file
*
********************************************************************/
static void __attribute__((always_inline)) calculateColors(void)
{
    GFX_COLOR   _fgcolor50;
    GFX_COLOR   _bgcolor50;
    
#if (COLOR_DEPTH == 16) || (COLOR_DEPTH == 24)

    _fgcolor50  = ConvertColor50(_fgcolor100);
    _fgcolor25  = ConvertColor25(_fgcolor100);
    _fgcolor75  = _fgcolor50   +  _fgcolor25;

    _bgcolor50  = ConvertColor50(_bgcolor100);
    _bgcolor25  = ConvertColor25(_bgcolor100);
    _bgcolor75  = _bgcolor50   +  _bgcolor25;

#elif ((COLOR_DEPTH == 8) || (COLOR_DEPTH == 4))
        
    _fgcolor50  = _fgcolor100 >> 1;
    _fgcolor25  = _fgcolor50  >> 1;
    _fgcolor75  = _fgcolor50  +  _fgcolor25;

    _bgcolor50  = _bgcolor100 >> 1;
    _bgcolor25  = _bgcolor50  >> 1;
    _bgcolor75  = _bgcolor50  +  _bgcolor25;

    #warning "Antialiasing at 8BPP and 4BPP is supported only for Grayscale mode"
        
#else
    
    #error "Anit-aliasing is currently supported only in 8BPP Grayscale and 16BPP, 24BPP color modes"
    
#endif
    _fgcolor25 += _bgcolor75;
    _fgcolor75 += _bgcolor25;
}

#endif //#ifdef USE_ANTIALIASED_FONTS

/*********************************************************************
* Function:  void OutCharGetInfoFlash (XCHAR ch, OUTCHAR_PARAM *pParam)
*
* PreCondition: none
*
* Input: ch - character code
*        pParam - pointer to character information structure.
*
* Output: none
*
* Side Effects: none
*
* Overview: Gathers the parameters for the specified character of the 
*           currently set font from flash memory. This is a step performed
*           before the character is rendered. 
*
* Note: Application should not call this function. This function is for 
*       versatility of implementing hardware accelerated text rendering
*       only.
*
********************************************************************/
void __attribute__((weak)) OutCharGetInfoFlash (XCHAR ch, OUTCHAR_PARAM *pParam)
{
#ifdef USE_FONT_FLASH

    GFX_FONT_SPACE GLYPH_ENTRY_EXTENDED    *pChTableExtended;
    GFX_FONT_SPACE GLYPH_ENTRY             *pChTable;

    // set color depth of font,
    // based on 2^bpp where bpp is the color depth setting in the FONT_HEADER
    pParam->bpp = 1 << currentFont.fontHeader.bpp;

    if(currentFont.fontHeader.extendedGlyphEntry)
    {
        pChTableExtended = (GFX_FONT_SPACE GLYPH_ENTRY_EXTENDED *) (((FONT_FLASH *)currentFont.pFont)->address + sizeof(FONT_HEADER)) + ((UXCHAR)ch - (UXCHAR)currentFont.fontHeader.firstChar);
        pParam->pChImage = (GFX_FONT_SPACE BYTE *) (((FONT_FLASH *)currentFont.pFont)->address + pChTableExtended->offset);
        pParam->chGlyphWidth = pChTableExtended->glyphWidth;
        pParam->xWidthAdjust = pChTableExtended->glyphWidth - pChTableExtended->cursorAdvance;
        pParam->xAdjust = pChTableExtended->xAdjust;
        pParam->yAdjust = pChTableExtended->yAdjust;
                    
        if(pParam->yAdjust > 0)
        {
            pParam->heightOvershoot = pParam->yAdjust;
        }
    }
    else
    {
        pChTable = (GFX_FONT_SPACE GLYPH_ENTRY *) (((FONT_FLASH *)currentFont.pFont)->address + sizeof(FONT_HEADER)) + ((UXCHAR)ch - (UXCHAR)currentFont.fontHeader.firstChar);
        pParam->pChImage = (GFX_FONT_SPACE BYTE *) (((FONT_FLASH *)currentFont.pFont)->address + ((DWORD)(pChTable->offsetMSB) << 8) + pChTable->offsetLSB);
        pParam->chGlyphWidth = pChTable->width;
    }

#endif // #ifdef USE_FONT_FLASH
    
}

/*********************************************************************
* Function:  void OutCharGetInfoExternal (XCHAR ch, OUTCHAR_PARAM *pParam)
*
* PreCondition: none
*
* Input: ch - character code
*        pParam - pointer to character information structure. 
*
* Output: none
*
* Side Effects: none
*
* Overview: Gathers the parameters for the specified character of the 
*           currently set font from external memory. This is a step performed
*           before the character is rendered. 
*
* Note: Application should not call this function. This function is for 
*       versatility of implementing hardware accelerated text rendering
*       only.
*
********************************************************************/
void __attribute__((weak)) OutCharGetInfoExternal(XCHAR ch, OUTCHAR_PARAM *pParam)
{
#ifdef USE_FONT_EXTERNAL

    GLYPH_ENTRY             chTable;
    GLYPH_ENTRY_EXTENDED    chTableExtended;
    WORD                    imageSize;
    DWORD_VAL               glyphOffset;

    // set color depth of font,
    // based on 2^bpp where bpp is the color depth setting in the FONT_HEADER
    pParam->bpp = 1 << currentFont.fontHeader.bpp;
       
    if(currentFont.fontHeader.extendedGlyphEntry)
    {
        ExternalMemoryCallback
        (
            currentFont.pFont,
            sizeof(FONT_HEADER) + ((UXCHAR)ch - (UXCHAR)currentFont.fontHeader.firstChar) * sizeof(GLYPH_ENTRY_EXTENDED),
            sizeof(GLYPH_ENTRY_EXTENDED),
            &chTableExtended
        );
    
        pParam->chGlyphWidth = chTableExtended.glyphWidth;
        pParam->xWidthAdjust = chTableExtended.glyphWidth - chTableExtended.cursorAdvance;
        pParam->xAdjust = chTableExtended.xAdjust;
        pParam->yAdjust = chTableExtended.yAdjust;
    
        if(pParam->yAdjust > 0)
        {
            pParam->heightOvershoot = pParam->yAdjust;
        }
                    
    }
    else
    {
        // get glyph entry
        ExternalMemoryCallback
        (
            currentFont.pFont,
            sizeof(FONT_HEADER) + ((UXCHAR)ch - (UXCHAR)currentFont.fontHeader.firstChar) * sizeof(GLYPH_ENTRY),
            sizeof(GLYPH_ENTRY),
            &chTable
        );
        pParam->chGlyphWidth = chTable.width;
    }

    // width of glyph in bytes
    imageSize = 0;
    pParam->chEffectiveGlyphWidth = pParam->chGlyphWidth * pParam->bpp;
    if(pParam->chEffectiveGlyphWidth & 0x0007)
        imageSize = 1;
    imageSize += (pParam->chEffectiveGlyphWidth >> 3);

    // glyph image size
    imageSize *= currentFont.fontHeader.height;

    if(currentFont.fontHeader.extendedGlyphEntry)
    {
        glyphOffset.Val = chTableExtended.offset;
    }
    else
    {
        // get glyph image
        glyphOffset.w[1] = (chTable.offsetMSB >> 8);
        glyphOffset.w[0] = (chTable.offsetMSB << 8) + (chTable.offsetLSB);
    }
            
    ExternalMemoryCallback(currentFont.pFont, glyphOffset.Val, imageSize, &(pParam->chImage));
    pParam->pChImage = (BYTE *) &(pParam->chImage);

#endif //#ifdef USE_FONT_EXTERNAL
}

/*********************************************************************
* Function:  void OutCharRender(XCHAR ch, OUTCHAR_PARAM *pParam)
*
* PreCondition: none
*
* Input: ch - character code
*        pParam - pointer to character information structure. 
*
* Output: none
*
* Side Effects: none
*
* Overview: Performs the actual rendering of the character using PutPixel().
*
* Note: Application should not call this function. This function is for 
*       versatility of implementing hardware accelerated text rendering
*       only.
*
********************************************************************/
WORD __attribute__((weak)) OutCharRender(XCHAR ch, OUTCHAR_PARAM *pParam)
{
    BYTE        temp = 0;
    BYTE        mask;
    BYTE        restoremask;
    SHORT       xCnt, yCnt, x = 0, y;
#ifdef USE_ANTIALIASED_FONTS
    BYTE        val, shift;
    GFX_COLOR   bgcolor;
#endif
    
#ifdef USE_ANTIALIASED_FONTS
    if(pParam->bpp == 1)
    {
        restoremask = 0x01;
    }
    else
    {
        if(pParam->bpp == 2)
        {
            restoremask = 0x03;
        }
        else
        {
            return -1; // BPP > 2 are not yet supported
        }
        
        bgcolor = GetPixel(GetX(), GetY() + (currentFont.fontHeader.height >> 1));
        
        if((_fgcolor100 != GetColor()) || (_bgcolor100 != bgcolor))
        {
            _fgcolor100 = GetColor();
            _bgcolor100 = bgcolor;
            calculateColors();
        }
    }
#else
    restoremask = 0x01;
#endif
    
    if(_fontOrientation == ORIENT_HOR)
    {
        y = GetY() + pParam->yAdjust;
        for(yCnt = 0; yCnt < currentFont.fontHeader.height + pParam->heightOvershoot; yCnt++)
        {
            x = GetX() + pParam->xAdjust;
            mask = 0;

#ifdef USE_ANTIALIASED_FONTS
            shift = 0;
#endif

            for(xCnt = 0; xCnt < pParam->chGlyphWidth; xCnt++)
            {
                if(mask == 0)
                {
                    temp = *(pParam->pChImage)++;
                    mask = restoremask;

#ifdef USE_ANTIALIASED_FONTS
                    shift = 0;
#endif
                }

#ifdef USE_ANTIALIASED_FONTS
                if(pParam->bpp == 1)                
                {
                    if(temp & mask)
                    {
                        PutPixel(x, y);
                    }
                }
                else
                {
                    val = (temp & mask) >> shift;
                    if(val)
                    {
                        if(GFX_Font_GetAntiAliasType() == ANTIALIAS_TRANSLUCENT)
                        {
                            bgcolor = GetPixel(x, y);
                            if(_bgcolor100 != bgcolor)
                            {
                                _bgcolor100 = bgcolor;
                                calculateColors();
                            }
                        }

                        switch(val)
                        {
                            case 1:     SetColor(_fgcolor25);
                                        break;
                                        
                            case 2:     SetColor(_fgcolor75);
                                        break;
                                        
                            case 3:     SetColor(_fgcolor100);
                        }
                        
                        PutPixel(x, y);
                    }
                }

                mask  <<=  pParam->bpp;
                shift  +=  pParam->bpp;
#else
                if(temp & mask)
                {
                    PutPixel(x, y);
                }
                
                mask <<= 1;
#endif
                x++;
            }
            y++;
        }

        // move cursor
        _cursorX = x - pParam->xAdjust - pParam->xWidthAdjust;
    }
    else    // If extended glyph is used, then vertical alignment may not be rendered properly and hence users must position the texts properly
    {
        y = GetX() + pParam->xAdjust;
        for(yCnt = 0; yCnt < currentFont.fontHeader.height + pParam->heightOvershoot; yCnt++)
        {
            x = GetY() + pParam->yAdjust;
            mask = 0; 

#ifdef USE_ANTIALIASED_FONTS
            shift = 0;
#endif

            for(xCnt = 0; xCnt < pParam->chGlyphWidth; xCnt++)
            {
                if(mask == 0)
                {
                    temp = *(pParam->pChImage)++;
                    mask = restoremask; 

#ifdef USE_ANTIALIASED_FONTS
                    shift = 0;
#endif
                }

#ifdef USE_ANTIALIASED_FONTS
                if(pParam->bpp == 1)                
                {
                    if(temp & mask)
                    {
                        PutPixel(y, x);
                    }
                }
                else
                {
                    val = (temp & mask) >> shift;
                    if(val)
                    {
                        if(GFX_Font_GetAntiAliasType() == ANTIALIAS_TRANSLUCENT)
                        {
                            bgcolor = GetPixel(x, y);
                            if(_bgcolor100 != bgcolor)
                            {
                                _bgcolor100 = bgcolor;
                                calculateColors();
                            }
                        }
                        switch(val)
                        {
                            case 1: SetColor(_fgcolor25);
                                    break;
                                        
                            case 2: SetColor(_fgcolor75);
                                    break;
                                        
                            case 3: SetColor(_fgcolor100);
                        }
                        PutPixel(y, x);
                    }
                }

                mask  <<=  pParam->bpp;
                shift  +=  pParam->bpp;
#else
                if(temp & mask)
                {
                    PutPixel(y, x);
                }
                mask  <<=  1;
#endif
                x--;
            }
            y++;
        }

        // move cursor
        _cursorY = x - pParam->xAdjust;
    }

    // restore color
#ifdef USE_ANTIALIASED_FONTS
    if(pParam->bpp > 1)
    {
        SetColor(_fgcolor100);
    }
#endif
    return (1);

}

/*********************************************************************
* Function: WORD OutChar(XCHAR ch)
*
* PreCondition: none
*
* Input: character code
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the character is not yet completely drawn.
*         - Returns 1 when the character is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: outputs a character
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) OutChar(XCHAR ch)
{
    static OUTCHAR_PARAM OutCharParam;

    // initialize variables
#ifdef USE_FONT_EXTERNAL	
    OutCharParam.pChImage = NULL;
#endif
    OutCharParam.xAdjust = 0;
    OutCharParam.yAdjust = 0;
    OutCharParam.xWidthAdjust = 0;
    OutCharParam.heightOvershoot = 0;

    // check for error conditions (a return value of 0xFFFF means error)
    if((UXCHAR)ch < (UXCHAR)currentFont.fontHeader.firstChar)
        return (-1);
    if((UXCHAR)ch > (UXCHAR)currentFont.fontHeader.lastChar)
        return (-1);
#ifndef USE_ANTIALIASED_FONTS
    if(currentFont.fontHeader.bpp > 1)
        return (-1);
#endif

#ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();
#else
    if(IsDeviceBusy() != 0)
        return (0);
#endif

    switch(*((SHORT *)currentFont.pFont))
    {

        case FLASH:
            OutCharGetInfoFlash(ch, &OutCharParam); 
            break;

        case EXTERNAL:
            OutCharGetInfoExternal(ch, &OutCharParam);
            break;
        
        default:
            return 1;
    }    
    
    return (OutCharRender(ch, &OutCharParam));
}

/*********************************************************************
* Function: WORD OutText(XCHAR* textString)
*
* PreCondition: none
*
* Input: textString - pointer to text string
*
* Output: non-zero if drawing done (used for NON-BLOCKING configuration)
*
* Side Effects: none
*
* Overview: outputs text from current position
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) OutText(XCHAR *textString)
{
    #ifndef USE_NONBLOCKING_CONFIG

    XCHAR   ch;
    while((UXCHAR)15 < (UXCHAR)(ch = *textString++))
        while(OutChar(ch) == 0);
    return (1);

    #else

    XCHAR       ch;
    static WORD counter = 0;

    while((UXCHAR)(ch = *(textString + counter)) > (UXCHAR)15)
    {
        if(OutChar(ch) == 0)
            return (0);
        counter++;
    }

    counter = 0;
    return (1);
    #endif
}

/*********************************************************************
* Function: WORD OutTextXY(SHORT x, SHORT y, XCHAR* textString)
*
* PreCondition: none
*
* Input: x,y - starting coordinates, textString - pointer to text string
*
* Output: non-zero if drawing done (used for NON-BLOCKING configuration)
*
* Side Effects: none
*
* Overview: outputs text from x,y position
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) OutTextXY(SHORT x, SHORT y, XCHAR *textString)
{
    #ifndef USE_NONBLOCKING_CONFIG
    MoveTo(x, y);
    OutText(textString);
    return (1);

    #else

    static BYTE start = 1;

    if(start)
    {
        MoveTo(x, y);
        start = 0;
    }

    if(OutText(textString) == 0)
    {
        return (0);
    }
    else
    {
        start = 1;
        return (1);
    }

    #endif
}

/*********************************************************************
* Function: SHORT GetTextWidthRam(XCHAR* textString, void* pFont)
*
* PreCondition: none
*
* Input: textString - pointer to the text string,
*        pFont - pointer to the font in RAM
*
* Output: text width in pixels
*
* Side Effects: none
*
* Overview: returns text width for the font
*
* Note: Application should not call this function. This function is for 
*       versatility of implementing hardware accelerated text rendering
*       only.
*
********************************************************************/
SHORT __attribute__((weak)) GetTextWidthRam(XCHAR* textString, void* pFont)
{
#if defined (USE_FONT_RAM)
    GLYPH_ENTRY *pChTable = NULL;
    GLYPH_ENTRY_EXTENDED *pChTableExtended = NULL;
    FONT_HEADER *pHeader;

    SHORT       textWidth;
    XCHAR       ch;
    XCHAR       fontFirstChar;
    XCHAR       fontLastChar;

    pHeader = (FONT_HEADER *) ((FONT_RAM *)pFont)->address;
    fontFirstChar = pHeader->firstChar;
    fontLastChar = pHeader->lastChar;
    if(pHeader->extendedGlyphEntry)
    {
        pChTableExtended = (GLYPH_ENTRY_EXTENDED *) (pHeader + 1);
    }
    else
    {
        pChTable = (GLYPH_ENTRY *) (pHeader + 1);
    }    
    textWidth = 0;
    while((UXCHAR)15 < (UXCHAR)(ch = *textString++))
    {
        if((UXCHAR)ch < (UXCHAR)fontFirstChar)
            continue;
        if((UXCHAR)ch > (UXCHAR)fontLastChar)
            continue;
        if(pHeader->extendedGlyphEntry)
        {
            textWidth += (pChTableExtended + ((UXCHAR)ch - (UXCHAR)fontFirstChar))->cursorAdvance;
        }
        else
        {
            textWidth += (pChTable + ((UXCHAR)ch - (UXCHAR)fontFirstChar))->width;
        }
    }
    return textWidth;

#else
    return 0;
#endif //#if defined (USE_FONT_RAM) 
}

/*********************************************************************
* Function: SHORT GetTextWidthFlash(XCHAR* textString, void* pFont)
*
* PreCondition: none
*
* Input: textString - pointer to the text string,
*        pFont - pointer to the font in flash memory
*
* Output: text width in pixels
*
* Side Effects: none
*
* Overview: returns text width for the font
*
* Note: Application should not call this function. This function is for 
*       versatility of implementing hardware accelerated text rendering
*       only.
*
********************************************************************/
SHORT __attribute__((weak)) GetTextWidthFlash(XCHAR* textString, void* pFont)
{
#if defined (USE_FONT_FLASH) 
    GFX_FONT_SPACE GLYPH_ENTRY *pChTable = NULL;
    GFX_FONT_SPACE GLYPH_ENTRY_EXTENDED *pChTableExtended = NULL;
    GFX_FONT_SPACE FONT_HEADER *pHeader;

    SHORT       textWidth;
    XCHAR       ch;
    XCHAR       fontFirstChar;
    XCHAR       fontLastChar;

    pHeader = (GFX_FONT_SPACE FONT_HEADER *) ((FONT_FLASH *)pFont)->address;
    fontFirstChar = pHeader->firstChar;
    fontLastChar = pHeader->lastChar;
    if(pHeader->extendedGlyphEntry)
    {
        pChTableExtended = (GFX_FONT_SPACE GLYPH_ENTRY_EXTENDED *) (pHeader + 1);
    }
    else
    {
        pChTable = (GFX_FONT_SPACE GLYPH_ENTRY *) (pHeader + 1);
    }    
    textWidth = 0;
    while((UXCHAR)15 < (UXCHAR)(ch = *textString++))
    {
        if((UXCHAR)ch < (UXCHAR)fontFirstChar)
            continue;
        if((UXCHAR)ch > (UXCHAR)fontLastChar)
            continue;
        if(pHeader->extendedGlyphEntry)
        {
            textWidth += (pChTableExtended + ((UXCHAR)ch - (UXCHAR)fontFirstChar))->cursorAdvance;
        }
        else
        {
            textWidth += (pChTable + ((UXCHAR)ch - (UXCHAR)fontFirstChar))->width;
        }
    }

    return (textWidth);

#else
    return 0;
#endif //#if defined (USE_FONT_FLASH) 

}

/*********************************************************************
* Function: SHORT GetTextWidthExternal(XCHAR* textString, void* pFont)
*
* PreCondition: none
*
* Input: textString - pointer to the text string,
*        pFont - pointer to the font in external memory
*
* Output: text width in pixels
*
* Side Effects: none
*
* Overview: returns text width for the font
*
* Note: Application should not call this function. This function is for 
*       versatility of implementing hardware accelerated text rendering
*       only.
*
********************************************************************/
SHORT __attribute__((weak)) GetTextWidthExternal(XCHAR* textString, void* pFont)
{

#ifdef USE_FONT_EXTERNAL
    GLYPH_ENTRY chTable;
    GLYPH_ENTRY_EXTENDED chTableExtended;
    FONT_HEADER header;

    SHORT       textWidth;
    XCHAR       ch;
    XCHAR       fontFirstChar;
    XCHAR       fontLastChar;


    ExternalMemoryCallback(pFont, 0, sizeof(FONT_HEADER), &header);
    fontFirstChar = header.firstChar;
    fontLastChar = header.lastChar;
    textWidth = 0;
    while((UXCHAR)15 < (UXCHAR)(ch = *textString++))
    {
        if((UXCHAR)ch < (UXCHAR)fontFirstChar)
            continue;
        if((UXCHAR)ch > (UXCHAR)fontLastChar)
            continue;
        if(header.extendedGlyphEntry)
        {
            ExternalMemoryCallback
            (
                pFont,
                sizeof(FONT_HEADER) + sizeof(GLYPH_ENTRY_EXTENDED) * ((UXCHAR)ch - (UXCHAR)fontFirstChar),
                sizeof(GLYPH_ENTRY_EXTENDED),
                &chTableExtended
            );
            textWidth += chTableExtended.cursorAdvance;
        }
        else
        {
            ExternalMemoryCallback
            (
                pFont,
                sizeof(FONT_HEADER) + sizeof(GLYPH_ENTRY) * ((UXCHAR)ch - (UXCHAR)fontFirstChar),
                sizeof(GLYPH_ENTRY),
                &chTable
            );
            textWidth += chTable.width;
        }
    }

    return (textWidth);
#else
    return 0;
#endif //#ifdef USE_FONT_EXTERNAL
}

/*********************************************************************
* Function: SHORT GetTextWidth(XCHAR* textString, void* pFont)
*
* PreCondition: none
*
* Input: textString - pointer to the text string,
*        pFont - pointer to the font
*
* Output: text width in pixels
*
* Side Effects: none
*
* Overview: returns text width for the font
*
* Note: none
*
********************************************************************/
SHORT __attribute__((weak)) GetTextWidth(XCHAR *textString, void *pFont)
{
    switch(*((SHORT *)pFont))
    {
        case RAM:
            return GetTextWidthRam(textString, pFont);
                	    
        case FLASH:
            return GetTextWidthFlash(textString, pFont);

        case EXTERNAL:
            return GetTextWidthExternal(textString, pFont);
            
        default:
            return (0);
    }
}


/*********************************************************************
* Function: SHORT GetTextHeight(void* pFont)
*
* PreCondition: none
*
* Input: pointer to the font
*
* Output: character height in pixels
*
* Side Effects: none
*
* Overview: returns characters height for the font
*
* Note: none
*
********************************************************************/
SHORT __attribute__((weak)) GetTextHeight(void *pFont)
{
#ifdef USE_FONT_EXTERNAL
    SHORT height;
#endif

    // if the current set font is the same just return with 
    // the already set value in currentFont
    if (pFont == currentFont.pFont)
        return currentFont.fontHeader.height;
    else
    {
        switch(*((SHORT *)pFont))
        {
#ifdef USE_FONT_RAM
            case RAM:
                return ((FONT_HEADER *) ((FONT_RAM *)pFont)->address)->height;
#endif
                    
#ifdef USE_FONT_FLASH
            case FLASH:
                return ((GFX_FONT_SPACE FONT_HEADER *) ((FONT_FLASH *)pFont)->address)->height;
#endif
                    
#ifdef USE_FONT_EXTERNAL
            case EXTERNAL:
                ExternalMemoryCallback(pFont, sizeof(FONT_HEADER) - 2, 2, &height);
                return (height);
#endif
    
            default:
                return (0);
        }
    }
}

/*********************************************************************
* Function: SHORT GetImageWidth(void* image)
*
* PreCondition: none
*
* Input: image - image pointer
*
* Output: none
*
* Side Effects: none
*
* Overview: returns image width
*
* Note: none
*
********************************************************************/
SHORT __attribute__((weak)) GetImageWidth(void *image)
{
#ifdef USE_BITMAP_EXTERNAL
    SHORT   width;
#endif

    switch(*((SHORT *)image))
    {

#ifdef USE_BITMAP_FLASH
        case FLASH:
            return (*((FLASH_WORD *) ((IMAGE_FLASH *)image)->address + 2));

    #if defined(USE_COMP_RLE)
        case FLASH | COMP_RLE | IMAGE_MBITMAP:
            return (((GFX_IMAGE_HEADER *)image)->width);
    #endif
#endif

#ifdef USE_BITMAP_EXTERNAL

        case EXTERNAL:
            ExternalMemoryCallback(image, 4, 2, &width);
            return (width);

    #if defined(USE_COMP_RLE)
        case EXTERNAL | COMP_RLE | IMAGE_MBITMAP:
            return (((GFX_IMAGE_HEADER *)image)->width);
    #endif


#endif

        default:
            return (0);
    }
}

/*********************************************************************
* Function: SHORT GetImageHeight(void* image)
*
* PreCondition: none
*
* Input: image - image pointer
*
* Output: none
*
* Side Effects: none
*
* Overview: returns image height
*
* Note: none
*
********************************************************************/
SHORT __attribute__((weak)) GetImageHeight(void *image)
{
#ifdef USE_BITMAP_EXTERNAL
    SHORT   height;
#endif

    switch(*((SHORT *)image))
    {
#ifdef USE_BITMAP_FLASH

        case FLASH:
            return (*((FLASH_WORD *) ((IMAGE_FLASH *)image)->address + 1));

    #if defined(USE_COMP_RLE)
        case FLASH | COMP_RLE | IMAGE_MBITMAP:
            return (((GFX_IMAGE_HEADER *)image)->height);
    #endif


#endif

#ifdef USE_BITMAP_EXTERNAL
            
        case EXTERNAL:
            ExternalMemoryCallback(image, 2, 2, &height);
            return (height);

    #if defined(USE_COMP_RLE)
        case EXTERNAL | COMP_RLE | IMAGE_MBITMAP:
            return (((GFX_IMAGE_HEADER *)image)->height);
    #endif


#endif

        default:
            return (0);
    }
}

    #ifdef USE_BITMAP_FLASH

/*********************************************************************
* Function: void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        partialImage - partial image pointer
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void __attribute__((weak)) PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_BYTE *flashAddress, *tempFlashAddress;
    BYTE                temp = 0;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                xc, yc;
    WORD                pallete[2];
    WORD                mask;
    WORD                addressOffset = 0, adjOffset;
    WORD                OffsetFlag = 0x01;     //Offset from BYTE color bit0 for the partial image 
                  
    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    pallete[0] = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    pallete[1] = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    if (sizeX & 0x07)
        adjOffset = 1; 
    else
        adjOffset = 0;

    // compute for addressOffset this is the offset needed to jump to the 
    // next line 
    addressOffset = ((sizeX >> 3) + adjOffset); 

    if(pPartialImageData->width != 0)
    {
        WORD mod3 = ((pPartialImageData->xoffset) & 0x07);

        // adjust the flashAddress to the starting pixel location
        // adjust one address if the data is not byte aligned
        flashAddress += (pPartialImageData->yoffset*((sizeX >> 3) + adjOffset));

        // adjust flashAddress for x offset (if xoffset is zero address stays the same)
        flashAddress += ((pPartialImageData->xoffset) >> 3);

        OffsetFlag <<= mod3;

        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width;
    }

    yc = top;

    // store current line data address 
    tempFlashAddress = flashAddress;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {

        for(y = 0; y < sizeY; y++)
        {
            // get flash address location of current line being processed
            flashAddress = tempFlashAddress;

            xc = left;
            if (OffsetFlag != 0x01)
    		{
                // grab the first set of data then set up the mask to the starting pixel
    			temp = *flashAddress++;
            }
            mask = OffsetFlag;
 
            for(x = 0; x < sizeX; x++)
            {

            	// Read 8 pixels from flash
                if(mask == 0x01)
			    {
				    temp = *flashAddress++;
                    mask = 0x01;
                }

                // Set color
                if(mask & temp)
                {
                // Set color
                    #ifdef USE_PALETTE
                        SetColor(1);
                    #else
                        SetColor(pallete[1]);
                    #endif                
                }
                else
                {
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(0);
                    #else
                        SetColor(pallete[0]);
                    #endif
                }

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    // adjust to next location
                    xc++;

                // Read 8 pixels from flash
                if(mask == 0x80)
                    mask = 0x01;
                else
                    // Shift to the next pixel
                    mask <<= 1;
            }  
            tempFlashAddress += (addressOffset);          
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            // get flash address location of current line being processed
            flashAddress = tempFlashAddress;

            xc = left;
            if (OffsetFlag != 0x01)
    		{
                // grab the first set of data then set up the mask to the starting pixel
    			temp = *flashAddress++;
            }
            mask = OffsetFlag;

            for(x = 0; x < sizeX; x++)
            {

            	// Read 8 pixels from flash
                if(mask == 0x01)
			    {
				    temp = *flashAddress++;
                    mask = 0x01;
                }

                    // Set color
                if(mask & temp)
                {
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(1);
                    #else
                        SetColor(pallete[1]);
                    #endif                
                }
                else
                {
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(0);
                    #else
                        SetColor(pallete[0]);
                    #endif
                }
                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 2x2 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
                                // Read 8 pixels from flash

                // Read 8 pixels from flash
                if(mask == 0x80)
                    mask = 0x01;
                else
                    // Shift to the next pixel
                    mask <<= 1;
            }
            tempFlashAddress += (addressOffset);
            yc+=2;
        }
    }    

}

/*********************************************************************
* Function: void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        partialImage - partial image pointer
* Output: none
*
* Side Effects: none
*
* Overview: outputs 16 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
        #if (COLOR_DEPTH >= 4)

/* */
void __attribute__((weak)) PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_BYTE *flashAddress;
    WORD                sizeX, sizeY;
    register WORD       x, y;
    WORD                xc, yc;
    BYTE                temp = 0;
    WORD                pallete[16];
    WORD                counter;
    WORD                addressOffset = 0;
    BYTE                OffsetFlag = 0;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    // Read pallete
    for(counter = 0; counter < 16; counter++)
    {
        pallete[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
    }

    if(pPartialImageData->width != 0)
    {
         WORD mod1 = (sizeX & 1);
         WORD mod2 = (pPartialImageData->width) & 1; 
         WORD mod3 = (pPartialImageData->xoffset) & 1; 

         flashAddress += (pPartialImageData->yoffset)*((sizeX>>1)+mod1);
         flashAddress += ((pPartialImageData->xoffset)>>1);                

         addressOffset = (sizeX>>1)+mod1;
         addressOffset -= ((pPartialImageData->width)>>1);
         addressOffset -= mod3;
         OffsetFlag = (mod3);

          if(OffsetFlag == 0)
          {
            addressOffset -= mod2;
          }
         
         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
         
    }

    yc = top;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            temp = *flashAddress;
            flashAddress += OffsetFlag;

            for(x = OffsetFlag; x < (sizeX+OffsetFlag); x++)
            {
                // Read 2 pixels from flash
                if(x & 0x01)
                {
                // second pixel in byte
                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp >> 4);
                #else
                    SetColor(pallete[temp >> 4]);
                #endif
                }
                else
                {
                    temp = *flashAddress++;
                // first pixel in byte
                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp & 0x0f);
                #else
                    SetColor(pallete[temp & 0x0f]);
                #endif
                }

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                        xc++;
            }  

            flashAddress += addressOffset;
  
            yc++;
        }

    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            temp = *flashAddress;
            flashAddress += OffsetFlag;

            for(x = OffsetFlag; x < (sizeX+OffsetFlag); x++)
            {

               if(x & 0x01)
               {

                // second pixel in byte
                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp >> 4);
                #else
                    SetColor(pallete[temp >> 4]);
                #endif
                }
                else
                {
                    temp = *flashAddress++;

                // first pixel in byte
                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp & 0x0f);
                #else
                    SetColor(pallete[temp & 0x0f]);
                #endif
                }

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 2x2 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
            }
            flashAddress += addressOffset;
            yc+=2;
        }
    }    
}
        #endif // #if (COLOR_DEPTH >= 4)

/*********************************************************************
* Function: void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        partialImage - partial image pointer
* Output: none
*
* Side Effects: none
*
* Overview: outputs 256 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
        #if (COLOR_DEPTH >= 8)

/* */
void __attribute__((weak)) PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_BYTE *flashAddress;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                xc, yc;
    BYTE                temp;
    WORD                pallete[256];
    WORD                counter;
    WORD                addressOffset = 0;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    // Read pallete
    for(counter = 0; counter < 256; counter++)
    {
        pallete[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
    }

    if(pPartialImageData->width != 0)
    {
         flashAddress += pPartialImageData->xoffset + pPartialImageData->yoffset*(sizeX);
         addressOffset = sizeX - pPartialImageData->width;
         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }

    yc = top;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *flashAddress++;

                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp);
                #else
                    SetColor(pallete[temp]);
                #endif

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;
            }   
            flashAddress += addressOffset;         
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *flashAddress++;

                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp);
                #else
                    SetColor(pallete[temp]);
                #endif
    
                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 2x2 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
            }    
            flashAddress += addressOffset;        
            yc+=2;
        }
    }    

}
        #endif // #if (COLOR_DEPTH >= 8)

/*********************************************************************
* Function: void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        partialImage - partial image pointer
* Output: none
*
* Side Effects: none
*
* Overview: outputs hicolor image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
        #if (COLOR_DEPTH == 16)

/* */
void __attribute__((weak)) PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_WORD *flashAddress;
    WORD                sizeX, sizeY;
    register WORD       x, y;
    WORD                xc, yc;
    WORD                temp;
    WORD                addressOffset = 0;

    // Move pointer to size information
    flashAddress = (FLASH_WORD *)image + 1;

    // Read image size
    sizeY = *flashAddress;
    flashAddress++;
    sizeX = *flashAddress;
    flashAddress++;
    
    if(pPartialImageData->width != 0)
    {
         flashAddress += pPartialImageData->xoffset + pPartialImageData->yoffset*(sizeX);
         addressOffset = sizeX - pPartialImageData->width;
         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }

    yc = top;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {    
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *flashAddress++;
                SetColor(temp);

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;
            }          
            flashAddress += addressOffset;  
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                
                temp = *flashAddress++;
                SetColor(temp);
    
                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 2x2 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    } 
                    xc += 2;   
            }      
            flashAddress += addressOffset;      
            yc+=2;
        }
    }    

}

        #endif //#if (COLOR_DEPTH == 16)
    #endif // #ifdef USE_BITMAP_FLASH

    #ifdef USE_BITMAP_EXTERNAL

/*********************************************************************
* Function: void PutImage1BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        partialImage - partial image pointer
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void __attribute__((weak)) PutImage1BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[2];
    BYTE            lineBuffer[((GetMaxX() + 1) / 8) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp = 0;
    BYTE            mask;
    WORD            sizeX, sizeY, lineLength;
    WORD            x, y;
    WORD            xc, yc;
    BYTE                OffsetFlag = 0x01;     //Offset from BYTE color bit0 for the partial image 

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (2 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 2 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 2 * sizeof(WORD);

    // Line width in bytes
    byteWidth = (bmp.width >> 3);
    if(bmp.width & 0x07)
        byteWidth++;

    if(pPartialImageData->width != 0)
    {

         memOffset += pPartialImageData->yoffset*byteWidth;
         memOffset += (pPartialImageData->xoffset)>>3;

         OffsetFlag <<= ((pPartialImageData->xoffset) & 0x07);

         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }
    else
    {
        // Get size
        sizeX = bmp.width;
        sizeY = bmp.height;
    }

    // calculate the length of bytes needed per line
    lineLength = (sizeX >> 3) + 1;
    if (sizeX & 0x07)
        lineLength++;
    
    yc = top;
    
    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, lineLength, lineBuffer);
            memOffset += byteWidth;
            
            pData = lineBuffer;
            mask = OffsetFlag;
            temp = *pData++;
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                    // Set color
                    if(mask & temp)
                    {
                        // Set color
                        #ifdef USE_PALETTE
                            SetColor(1);
                        #else
                            SetColor(pallete[1]);
                        #endif
                    }
                    else
                    {
                        // Set color
                        #ifdef USE_PALETTE
                            SetColor(0);
                        #else
                            SetColor(pallete[0]);
                        #endif
                    }


                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;

                // Read 8 pixels from flash
                if(mask == 0x80)
                {
                    temp = *pData++;
                    mask = 0x01;
                }
                else
                {
                    // Shift to the next pixel
                    mask <<= 1;
                }    
            }            
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, lineLength, lineBuffer);
            memOffset += byteWidth;
            
            pData = lineBuffer;
            mask = OffsetFlag;
            temp = *pData++;
            xc = left;

            for(x = 0; x < sizeX; x++)
            {
                    // Set color
                    if(mask & temp)
                    {
                        // Set color
                        #ifdef USE_PALETTE
                            SetColor(1);
                        #else
                            SetColor(pallete[1]);
                        #endif
                    }
                    else
                    {
                        // Set color
                        #ifdef USE_PALETTE
                            SetColor(0);
                        #else
                            SetColor(pallete[0]);
                        #endif
                    }

    
                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 2x2 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;

                // Read 8 pixels from flash
                if(mask == 0x80)
                {
                    temp = *pData++;
                    mask = 0x01;
                }
                else
                {
                    // Shift to the next pixel
                    mask <<= 1;
                }  

            }            
            yc+=2;
        }
    }    

}

/*********************************************************************
* Function: void PutImage4BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        partialImage - partial image pointer
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
        #if (COLOR_DEPTH >= 4)

/* */
void __attribute__((weak)) PutImage4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[16];
    BYTE            lineBuffer[((GetMaxX() + 1) / 2) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp = 0;
    WORD            sizeX, sizeY, lineLength;
    WORD            x, y;
    WORD            xc, yc;
    WORD            nibbleOffset = 0; 


    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (16 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 16 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 16 * sizeof(WORD);

     // Line width in bytes
    byteWidth = (bmp.width) >> 1;

    // if the width is odd, add another byte count since the shift 1 caused
    // us to loose a nibble
    if(bmp.width & 0x01)
        byteWidth++;

    if(pPartialImageData->width != 0)
    {
        // check the bit position of the starting pixel
        nibbleOffset = pPartialImageData->xoffset & 0x01;

        memOffset += (pPartialImageData->yoffset)*byteWidth;
        memOffset += (pPartialImageData->xoffset) >> 1;

        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width;
    }
    else
    {
        // Get size
        sizeX = bmp.width;
        sizeY = bmp.height;
    }

    // calculate the length of bytes needed per line
    lineLength = (sizeX >> 1) + 1;
    if (sizeX & 0x01)
        lineLength++;

    yc = top;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, lineLength, lineBuffer);
            memOffset += byteWidth;
            
            pData = lineBuffer;
            if (nibbleOffset)
            {
                temp = *pData++;
            }
            
            xc = left;
            for(x = nibbleOffset; x < (sizeX + nibbleOffset); x++)
            {
                
                // Read 2 pixels from flash
                if(x & 0x0001)
                {

                    // second pixel in byte
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(temp >> 4);
                    #else
                        SetColor(pallete[temp >> 4]);
                    #endif
                }
                else
                {
                    temp = *pData++;

                    // first pixel in byte
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(temp & 0x0f);
                    #else
                        SetColor(pallete[temp & 0x0f]);
                    #endif
                }

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;
            }            
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, lineLength, lineBuffer);
            memOffset += byteWidth;
            
            pData = lineBuffer;
            if (nibbleOffset)
            {
                temp = *pData++;
            }
            
            xc = left;
            for(x = nibbleOffset; x < (sizeX + nibbleOffset); x++)
            {

                // Read 2 pixels from flash
                if(x & 0x0001)
                {

                    // second pixel in byte
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(temp >> 4);
                    #else
                        SetColor(pallete[temp >> 4]);
                    #endif
                }
                else
                {
                    temp = *pData++;

                    // first pixel in byte
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(temp & 0x0f);
                    #else
                        SetColor(pallete[temp & 0x0f]);
                    #endif
                }

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {    
                        // Write pixel(s) to screen, basically writes a tile of 2x2 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
            }            
            yc+=2;
        }
    }    
}

        #endif

/*********************************************************************
* Function: void PutImage8BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *partialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        partialImage - partial image pointer
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
        #if (COLOR_DEPTH >= 8)

/* */
void __attribute__((weak)) PutImage8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[256];
    BYTE            lineBuffer[(GetMaxX() + 1)];
    BYTE            *pData;

    BYTE            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            xc, yc;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (256 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 256 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 256 * sizeof(WORD);

    if(pPartialImageData->width != 0)
    {
         memOffset += pPartialImageData->xoffset + pPartialImageData->yoffset*(bmp.width);
         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }
    else
    {
        // Get size
        sizeX = bmp.width;
        sizeY = bmp.height;
    }

    yc = top;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, sizeX, lineBuffer);
            memOffset += bmp.width;
            
            pData = lineBuffer;
            
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *pData++;
                
                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp);
                #else
                    SetColor(pallete[temp]);
                #endif

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;
            }            
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, sizeX, lineBuffer);
            memOffset += bmp.width;
            
            pData = lineBuffer;
            
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *pData++;

                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp);
                #else
                    SetColor(pallete[temp]);
                #endif
    
                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 2x2 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
            }            
            yc+=2;
        }
    }    

}

        #endif

/*********************************************************************
* Function: void PutImage16BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        partialImage - partial image pointer
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
        #if (COLOR_DEPTH == 16)

/* */
void __attribute__((weak)) PutImage16BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            lineBuffer[(GetMaxX() + 1)];
    WORD            *pData;
    WORD            byteWidth;

    WORD            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            xc, yc;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER);

    if(pPartialImageData->width != 0)
    {
         memOffset += (pPartialImageData->xoffset + pPartialImageData->yoffset*(bmp.width))<<1;
         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }
    else
    {
        // Get size
        sizeX = bmp.width;
        sizeY = bmp.height;
    }

    byteWidth = bmp.width << 1;
    yc = top;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
            memOffset += byteWidth;
            
            pData = lineBuffer;
            
            xc = left;
            for(x = 0; x < sizeX; x++)
                {
                temp = *pData++;
                SetColor(temp);

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;
            }            
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
            memOffset += byteWidth;
            pData = lineBuffer;
            
            xc = left;
            for(x = 0; x < sizeX; x++)
                {
                temp = *pData++;
                SetColor(temp);
    
                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 2x2 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
            }            
            yc+=2;
        }
    }    
}

        #endif
    #endif


/*********************************************************************
* Function: WORD PutImagePartial(SHORT left, SHORT top, void* image, BYTE stretch, SHORT xoffset, SHORT yoffset, WORD width, WORD height)
*
* Overview: This function outputs a full or a partial image starting 
*           from left,top coordinates. The partial image starts at 
*           xoffset and yoffset and is given a width and height.
*
* Input: left - horizontal starting position of full or partial image on the screen
*        top - vertical starting position of full or partial image on the screen,
*        image - pointer to the image location.
*        stretch - The image stretch factor. 
*                  - IMAGE_NORMAL : no stretch 
*                  - IMAGE_X2 : image is stretched to twice 
*                    its width and height
*        xoffset – Specifies the horizontal offset in pixels of the selected partial 
*                  image from the left most pixel of the full image.
*        yoffset – Specifies the vertical offset in pixels of the selected partial 
*                  image from the top most pixel of the full image.
*        width - width of the partial image to be rendered. 
*                xoffset + width must not exceed the full image width. 
*        height - height of the partial image to be rendered. 
*                 yoffset + height must not exceed the full image height.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the image is not 
*           yet completely drawn.
*         - Returns 1 when the image is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
* 
* Side Effects: none
********************************************************************/
WORD PutImagePartial(SHORT left, SHORT top, void *image, BYTE stretch, SHORT xoffset, SHORT yoffset, WORD width, WORD height)
{

   PUTIMAGE_PARAM partialImage;

#if defined (USE_BITMAP_FLASH)
    FLASH_BYTE  *flashAddress;
#endif    
#if defined (USE_BITMAP_FLASH) || defined (USE_BITMAP_EXTERNAL)
    BYTE        colorDepth;
#endif
    WORD        colorTemp;
    WORD        resType;

#if defined(USE_COMP_RLE) 
    GFX_IMAGE_HEADER *pimghdr = (GFX_IMAGE_HEADER *)image;
#endif
    
#ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    /* Ready */
#else
    if(IsDeviceBusy() != 0)
        return (0);
#endif

    // Save current color
    colorTemp = GetColor();
    resType = *((WORD *)image);

    // Store the partial image offset
    partialImage.xoffset = xoffset;
    partialImage.yoffset = yoffset;
    partialImage.width   = width;
    partialImage.height = height;

    switch(resType & (GFX_MEM_MASK | GFX_COMP_MASK))
    {

                #ifdef USE_COMP_RLE
                #ifdef USE_BITMAP_FLASH
        case (FLASH | COMP_RLE):
            
            // Image address
            flashAddress = pimghdr->LOCATION.progByteAddress;
            // Read color depth
            colorDepth = pimghdr->colorDepth;
        
            // Draw picture
            switch(colorDepth)
            {
                    #if (COLOR_DEPTH >= 4)
                case 4: 
                    PutImageRLE4BPP(left, top, flashAddress, stretch); break;
                    #endif
  
                    #if (COLOR_DEPTH >= 8)
                case 8: 
                    PutImageRLE8BPP(left, top, flashAddress, stretch); break;
                    #endif
                        
                    default:    break;
            }
            break;
                #endif //#ifdef USE_BITMAP_FLASH

                #ifdef USE_BITMAP_EXTERNAL
        case (EXTERNAL | COMP_RLE):
        
            // Get color depth
            ExternalMemoryCallback(image, 1, 1, &colorDepth);
    
            // Draw picture
            switch(colorDepth)
            {
                    #if (COLOR_DEPTH >= 4)
                case 4:     
                    PutImageRLE4BPPExt(left, top, image, stretch); break;
                    #endif
                            
                    #if (COLOR_DEPTH >= 8)
                case 8:     
                    PutImageRLE8BPPExt(left, top, image, stretch); break;
                    #endif
                            
                default:    break;
            }
            break;
                #endif //#ifdef USE_BITMAP_EXTERNAL
                #endif // #ifdef USE_COMP_RLE
        
                #ifdef USE_BITMAP_FLASH

        case (FLASH | COMP_NONE):

            // Image address
            flashAddress = ((IMAGE_FLASH *)image)->address;

            // Read color depth
            colorDepth = *(flashAddress + 1);

            // Draw picture
            switch(colorDepth)
            {
                case 1:     
                    PutImage1BPP(left, top, flashAddress, stretch, &partialImage); break;
                
                    #if (COLOR_DEPTH >= 4)
                case 4:     
                    PutImage4BPP(left, top, flashAddress, stretch, &partialImage); break;
                    #endif
                    
                    #if (COLOR_DEPTH >= 8)
                case 8:     
                    PutImage8BPP(left, top, flashAddress, stretch, &partialImage); break;
                    #endif
                    
                    #if (COLOR_DEPTH == 16)
                case 16:    
                    PutImage16BPP(left, top, flashAddress, stretch, &partialImage); break;
                    #endif
                    
                default:    break;

            }

            break;
                #endif // #ifdef USE_BITMAP_FLASH
                
                #ifdef USE_BITMAP_EXTERNAL

        case (EXTERNAL | COMP_NONE):

            // Get color depth
            ExternalMemoryCallback(image, 1, 1, &colorDepth);

            // Draw picture
            switch(colorDepth)
            {
                case 1:     
                    PutImage1BPPExt(left, top, image, stretch, &partialImage); break;
                
                    #if (COLOR_DEPTH >= 4)
                case 4:     
                    PutImage4BPPExt(left, top, image, stretch, &partialImage); break;
                    #endif
                    
                    #if (COLOR_DEPTH >= 8)
                case 8:     
                    PutImage8BPPExt(left, top, image, stretch, &partialImage); break;
                    #endif
                    
                    #if (COLOR_DEPTH == 16)
                case 16:    
                    PutImage16BPPExt(left, top, image, stretch, &partialImage); break;
                    #endif
                    
                default:    break;
            }

            break;
                #endif //#ifdef USE_BITMAP_EXTERNAL

                #if defined (GFX_USE_DISPLAY_CONTROLLER_MCHP_DA210)
                #ifdef USE_COMP_IPU
        case (FLASH | COMP_IPU):
        case (EXTERNAL | COMP_IPU):
        case (EDS_EPMP | COMP_IPU):
                #endif // #ifdef USE_COMP_IPU
        case (EDS_EPMP | COMP_NONE):
        
            // this requires special processing of images in Extended Data Space
            // call the driver specific function to perform the processing
            PutImageDrv(left, top, image, stretch, &partialImage); 
            break;

                #endif //#if defined (__PIC24FJ256DA210__)
            
        default:
            break;
    }

    // Restore current color
    SetColor(colorTemp);
    return (1);
}


/*********************************************************************
* Function:  SHORT GetSineCosine(SHORT v, WORD type)
*
* PreCondition: none
*
* Input: v - the angle used to calculate the sine or cosine value. 
*			 The angle must be in the range of -360 to 360 degrees.
*		 type - sets if the angle calculation is for a sine or cosine
*				- GETSINE (0) - get the value of sine(v).
*				- GETCOSINE (1) - return the value of cosine(v).
*
* Output: Returns the sine or cosine of the angle given.
*
* Side Effects: none
*
* Overview: This calculates the sine or cosine value of the given angle.
*
* Note: none
*
********************************************************************/
SHORT GetSineCosine(SHORT v, WORD type)
{
	// if angle is neg, convert to pos equivalent
    if (v < 0) 
    	v += 360;   					
    //v /= DEGREECOUNT;                 // convert into ticks from degrees, tick = 3 deg
           
    if(v >= COSINETABLEENTRIES * 3)
    {
        v -= COSINETABLEENTRIES * 3;
        return ((type == GETSINE) ? -(_CosineTable[v]) : (_CosineTable[COSINETABLEENTRIES - v]));
    }
    else if(v >= COSINETABLEENTRIES * 2)
    {
        v -= COSINETABLEENTRIES * 2;
        return ((type == GETSINE) ? -(_CosineTable[(COSINETABLEENTRIES - v)]) : -(_CosineTable[v]));
    }
    else if(v >= COSINETABLEENTRIES)
    {
        v -= COSINETABLEENTRIES;
        return ((type == GETSINE) ? (_CosineTable[v]) : -(_CosineTable[COSINETABLEENTRIES - v]));
    }
    else
    {
        return ((type == GETSINE) ? (_CosineTable[COSINETABLEENTRIES - v]) : (_CosineTable[v]));
    }


}

/*********************************************************************
* Function:  WORD DrawArc(SHORT cx, SHORT cy, SHORT r1, SHORT r2, SHORT startAngle, SHORT endAngle)
*
* PreCondition: none
*
* Input: cx - the location of the center of the arc in the x direction. 
*	     cy - the location of the center of the arc in the y direction. 		 
*	     r1 - the smaller radius of the arc. 		 
*	     r2 - the larger radius of the arc. 		 
*	     startAngle - start angle of the arc. 		 
*	     endAngle - end angle of the arc. 		 
*
* Output: Returns 1 if the rendering is done, 0 if not yet done.
*
* Side Effects: none
*
* Overview: This renders an arc with from startAngle to endAngle with the thickness 
*		    of r2-r1. The function returns 1 when the arc is rendered successfuly
* 			and returns a 0 when it is not yet finished. The next call to the 
*			function will continue the rendering.
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) DrawArc(SHORT cx, SHORT cy, SHORT r1, SHORT r2, SHORT startAngle, SHORT endAngle)
{

	SHORT i;
	SHORT x1,y1,x2,y2;

	for (i=startAngle; i <= endAngle; i++)
	{
		// get inner arc x,y position
		y1 = (r1*Sine(i))/256;
		x1 = (r1*Cosine(i))/256;

		// get outer arc x,y position
		if (r1 != r2)
		{
			y2 = (r2*Sine(i))/256;
			x2 = (r2*Cosine(i))/256;

			// check if we need to double the line to cover all pixels
			if ((x1 == x2) || (y1 == y2))
			{
				SetLineThickness(NORMAL_LINE);
			}	
			else
			{
				SetLineThickness(THICK_LINE);
			}	
			// draw the lines to fill the arc
			while(!Line(cx+x1, cy+y1, cx+x2, cy+y2));
		}
		else 
		{	
			PutPixel(cx+x1, cy+y1);
		}	
		
	}
	return 1;	
}	


#ifdef USE_GRADIENT

#if ((COLOR_DEPTH != 16) && (COLOR_DEPTH != 24))
    #error "The USE_GRADIENT feature can currently support the COLOR_DEPTH of 16 and 24 only."
#endif

#ifdef USE_PALETTE
    #error "The USE_GRADIENT feature is not currently supported when USE_PALETTE is enabled."
#endif

#if (COLOR_DEPTH == 24)
#define GetRed(color)       (((color) & 0xFF0000) >> 16)
#define GetGreen(color)     (((color) & 0x00FF00) >> 8)
#define GetBlue(color)      ((color) & 0x0000FF)
#else
#define GetRed(color)       (((color) & 0xF800) >> 8)
#define GetGreen(color)     (((color) & 0x07E0) >> 3)
#define GetBlue(color)      (((color) & 0x001F) << 3)
#endif

WORD BarGradient(SHORT left, SHORT top, SHORT right, SHORT bottom, GFX_COLOR color1, GFX_COLOR color2, DWORD length, BYTE direction)
{

    WORD startRed, startBlue, startGreen;
    WORD endRed, endBlue, endGreen;

    long rdiff=0,gdiff=0,bdiff=0;
    short i,steps;

    SetColor(color2);

    // if length is 100, why waste the bar call?
    switch(direction)
    {
    case GRAD_UP:
        length = length * (bottom - top);
        length /= 100;
        steps = length;
        while(!Bar(left,top,right,bottom-steps));
        break;

    case GRAD_DOWN: 
        length = length * (bottom - top); 
        length /= 100; 
        steps = length;    
        while(!Bar(left,top+steps,right,bottom));    
        break;

    case GRAD_RIGHT:
        length = length * (right - left);
        length /= 100;
        steps = length;
        while(!Bar(left+steps,top,right,bottom)); 
        break;

    case GRAD_LEFT:
        length = length * (right - left);
        length /= 100;
        steps = length;
        while(!Bar(left,top,right-steps,bottom)); 
        break;

    case GRAD_DOUBLE_VER:
        steps = (right - left) >> 1;
        break;

    case GRAD_DOUBLE_HOR:
        steps = (bottom - top) >> 1;
        break; 

    default: 
        return 1;
    }

    startRed    = GetRed(color1);
    startGreen  = GetGreen(color1);
    startBlue   = GetBlue(color1);

    endRed      = GetRed(color2);
    endGreen    = GetGreen(color2);
    endBlue     = GetBlue(color2);

    ///////////////////////////////////

    //Find the step size for the red portion//
    rdiff = ((long)endRed - (long)startRed) << 8;
    rdiff /= steps;

    //Find the step size for the green portion//
    gdiff = ((long)endGreen - (long)startGreen) << 8;
    gdiff /= steps;

    //Find the step size for the blue portion//
    bdiff = ((long)endBlue - (long)startBlue) << 8;
    bdiff /= steps;

    short barSize = 1;
    color1 = RGBConvert(startRed, startGreen, startBlue);

    // PERCENTAGE BASED CODE
    for(i=0; i < steps; i++)
    {
        //Calculate the starting RGB values
        endRed      = startRed + ((rdiff*i) >> 8);
        endGreen    = startGreen + ((gdiff*i) >> 8);
        endBlue     = startBlue + ((bdiff*i) >> 8);

        color2 = RGBConvert(endRed, endGreen, endBlue);

        if(color2 == color1)
        {
            barSize++;
            continue;
        }

        SetColor(color2);
        color1 = color2;

        switch(direction)          //This switch statement draws the gradient depending on direction chosen
        {
        case GRAD_DOWN:
            while(!Bar(left, top, right, top + barSize));
            top += barSize;
        break;
        
        case GRAD_UP:
            while(!Bar(left,bottom - barSize,right,bottom));
            bottom -= barSize;
        break;

        case GRAD_RIGHT:
            while(!Bar(left, top, left + barSize, bottom));
            left += barSize;
        break;
        
        case GRAD_LEFT:
            while(!Bar(right - barSize, top, right, bottom));
            right -= barSize;
        break;

        case GRAD_DOUBLE_VER:
            while(!Bar(right - barSize, top, right, bottom));
            right -= barSize;
            while(!Bar(left, top, left + barSize, bottom));
            left += barSize;
        break;

        case GRAD_DOUBLE_HOR:
            while(!Bar(left, bottom - barSize, right, bottom));
            bottom -= barSize;
            while(!Bar(left, top, right, top + barSize));
            top += barSize;
        break; 

        default: 
            break;
        }

        barSize = 1;
    }

    if(barSize > 1)
    {

        SetColor(RGBConvert(endRed, endGreen, endBlue));

        switch(direction)          //This switch statement draws the gradient depending on direction chosen
        {
        case GRAD_DOWN:
            while(!Bar(left, top, right, top + barSize));
        break;
        
        case GRAD_UP:
            while(!Bar(left,bottom - barSize,right,bottom));
        break;

        case GRAD_RIGHT:
            while(!Bar(left, top, left + barSize, bottom));
        break;
        
        case GRAD_LEFT:
            while(!Bar(right - barSize, top, right, bottom));
        break;

        case GRAD_DOUBLE_VER:
            while(!Bar(right - barSize, top, right, bottom));
            while(!Bar(left, top, left + barSize, bottom));
        break;

        case GRAD_DOUBLE_HOR:
            while(!Bar(left, bottom - barSize, right, bottom));
            while(!Bar(left, top, right, top + barSize));
        break; 

        default: 
            break;
        }

    }
    return 1;
}

WORD BevelGradient(SHORT left, SHORT top, SHORT right, SHORT bottom, SHORT rad, GFX_COLOR color1, GFX_COLOR color2, DWORD length, BYTE direction)
{
    WORD i;
    WORD sred,sblue,sgreen;
    WORD ered,eblue,egreen;
    GFX_COLOR EndColor;
    long rdiff=0,gdiff=0,bdiff=0;
    short steps;
    EndColor = color2;

    switch(direction)     //This switch statement calculates the amount of transitions needed
    {
    case GRAD_UP:
    case GRAD_DOWN:
        length = length * (bottom - top +(rad << 1));
        length /= 100;
        steps = length;
        break;

    case GRAD_RIGHT:
    case GRAD_LEFT:
        length = length * (right - left +(rad << 1));
        length /= 100;
        steps = length;
        break;

    case GRAD_DOUBLE_VER:
        steps = (right - left +(rad << 1)) >> 1;
        break;

    case GRAD_DOUBLE_HOR:
        steps = (bottom - top +(rad << 1)) >> 1;
        break; 

    default: 
        return (1);
    }

    //Calculate the starting RGB values
    sred    = GetRed(color1);
    sgreen  = GetGreen(color1);
    sblue   = GetBlue(color1);
            
    ered    = GetRed(color2);
    egreen  = GetGreen(color2);
    eblue   = GetBlue(color2);

    ///////////////////////////////////

    //Find the step size for the red portion//
    rdiff = ((long)ered - (long)sred) << 8;
    rdiff /= steps;

    //Find the step size for the green portion//
    gdiff = ((long)egreen - (long)sgreen) << 8;
    gdiff /= steps;

    //Find the step size for the blue portion//
    bdiff = ((long)eblue - (long)sblue) << 8;
    bdiff /= steps;

    typedef enum
    {
        BEGIN,
        CHECK,
        Q8TOQ1,
        Q7TOQ2,
        Q6TOQ3,
        Q5TOQ4,
        WAITFORDONE,
        FACE
    } FILLCIRCLE_STATES;

    DWORD_VAL temp;
    static LONG err;
    static SHORT yLimit, xPos, yPos;
    static SHORT xCur, yCur, yNew;

    static FILLCIRCLE_STATES state = BEGIN;

    while(1)
    {
        while(IsDeviceBusy()); 
            
        switch(state)
        {
            case BEGIN:
                if(!rad)
                {   // no radius object is a filled rectangle
                    state = FACE;
                    break;
                }

                // compute variables
                temp.Val = SIN45 * rad;
                yLimit = temp.w[1];
                temp.Val = (DWORD) (ONEP25 - ((LONG) rad << 16));
                err = (SHORT) (temp.w[1]);
                xPos = rad;
                yPos = 0;
                xCur = xPos;
                yCur = yPos;
                yNew = yPos;
                state = CHECK;

            case CHECK:
                bevel_fill_check : if(yPos > yLimit)
                {
                    state = FACE;
                    break;
                }

                // New records the last y position
                yNew = yPos;

                // calculate the next value of x and y
                if(err > 0)
                {
                    xPos--;
                    err += 5 + ((yPos - xPos) << 1);
                }
                else
                    err += 3 + (yPos << 1);
                yPos++;
                state = Q6TOQ3;

            case Q6TOQ3:
                if(xCur != xPos)
                {

                    // 6th octant to 3rd octant
	            	//if (_bevelDrawType & DRAWBOTTOMBEVEL) 
	            	{ 
                        if(direction == GRAD_DOUBLE_VER || direction == GRAD_DOUBLE_HOR)
                            i = (top - yCur) - top + rad;
                        else
                            i = (bottom + yCur) - top + rad;
                
                        ered = sred + ((rdiff*i) >> 8);
                        egreen = sgreen + ((gdiff*i) >> 8);
                        eblue = sblue + ((bdiff*i) >> 8);
 
                        color2 = RGBConvert(ered,egreen,eblue);
                                         
                        SetColor(color2);
            
                        switch(direction)    //Direction matter because different portions of the circle are drawn
                        {
                           case GRAD_LEFT:
                               if(i>length) SetColor(EndColor);
                               Bar(left - yNew, top - xCur, left - yCur, bottom + xCur);
                               break;
                           
                           case GRAD_RIGHT:                     
                           case GRAD_DOUBLE_VER:
                                if(i>length) 
                                    SetColor(EndColor);
                               
                                Bar(right + yCur, top - xCur, right + yNew, bottom + xCur);          
                                break;
                           
                           case GRAD_UP:
                                if(i>length) 
                                    SetColor(EndColor);
                                
                                Bar(left - xCur, top - yNew, right + xCur, top - yCur);
                                break;

                           case GRAD_DOWN:
                           case GRAD_DOUBLE_HOR:
                                if(i>length) 
                                    SetColor(EndColor);
                                Bar(left - xCur, bottom + yCur, right + xCur, bottom + yNew);
                           
                           default: 
                               break;
                        }

                  	}          	
                    state = Q5TOQ4;
                    break;
                }

                state = CHECK;
                goto bevel_fill_check;

            case Q5TOQ4:

            	//if (_bevelDrawType & DRAWBOTTOMBEVEL) 
            	{ 
                    if(direction == GRAD_DOUBLE_VER || direction == GRAD_DOUBLE_HOR)
                    i = top + xPos - top + rad;
                    else

	                // 5th octant to 4th octant
                    i = (bottom + xPos) - top + rad ;

                    //Calculate the starting RGB values
                    ered = sred + ((rdiff*i) >> 8);
                    egreen = sgreen + ((gdiff*i) >> 8);
                    eblue = sblue + ((bdiff*i) >> 8);
                    
                    color2 = RGBConvert(ered,egreen,eblue);
                    SetColor(color2);

                    switch(direction)    //Direction matter because different portions of the circle are drawn
                    {
                       case GRAD_LEFT:
                           if(i>length) 
                               SetColor(EndColor);
                           Bar(left - xCur, top - yNew, left - xPos, bottom + yNew); 
                            break;
                       
                       case GRAD_RIGHT:                     
                       case GRAD_DOUBLE_VER:
                           if(i>length) 
                               SetColor(EndColor);
                           Bar(right + xPos, top - yNew, right + xCur, bottom + yNew);           
                            break;
                       
                       case GRAD_UP:
                            if(i>length) 
                                SetColor(EndColor);
                            Bar(left - yNew, top - xCur, right + yNew, top - xPos); 
                            break;

                       case GRAD_DOWN:
                       case GRAD_DOUBLE_HOR:
                            if(i>length) 
                                SetColor(EndColor);
                            Bar(left - yNew, bottom + xPos, right + yNew, bottom + xCur);
                       default: 
                           break;
                    }

				}
                state = Q8TOQ1;
                break;

            case Q8TOQ1:

                // 8th octant to 1st octant
	            //if (_bevelDrawType & DRAWTOPBEVEL) 
				{
                    i = (top - xCur) - top + rad;

                    //Calculate the starting RGB values
                    ered = sred + ((rdiff*i) >> 8);
                    egreen = sgreen + ((gdiff*i) >> 8);
                    eblue = sblue + ((bdiff*i) >> 8);

                    color2 = RGBConvert(ered,egreen,eblue);
                    SetColor(color2);

                    switch(direction)    //Direction matter because different portions of the circle are drawn
                    {
                       case GRAD_LEFT:
                           if(i>length) 
                               SetColor(EndColor);
                           Bar(right + xPos, top - yNew, right + xCur, bottom + yNew);
                            break;
                       
                       case GRAD_RIGHT:                     
                       case GRAD_DOUBLE_VER:
                           if(i>length) 
                               SetColor(EndColor);
                           Bar(left - xCur, top - yNew, left - xPos, bottom + yNew);           
                            break;
                       
                       case GRAD_UP:
                            if(i>length) 
                                SetColor(EndColor);
                            Bar(left - yNew, bottom + xPos, right + yNew, bottom + xCur);
                            break;

                       case GRAD_DOWN:
                       case GRAD_DOUBLE_HOR:
                            if(i>length) 
                                SetColor(EndColor);
                            Bar(left - yNew, top - xCur, right + yNew, top - xPos);
                            break;
                       
                       default: 
                           break;
                    }

    			}                	
                state = Q7TOQ2;
                break;

            case Q7TOQ2:

                // 7th octant to 2nd octant
	            //if (_bevelDrawType & DRAWTOPBEVEL) 
				{             
                    i = (top - yNew) - top + rad;

                    //Calculate the starting RGB values
                    ered = sred + ((rdiff*i) >> 8);
                    egreen = sgreen + ((gdiff*i) >> 8);
                    eblue = sblue + ((bdiff*i) >> 8);

                    color2 = RGBConvert(ered,egreen,eblue);
                    
                    SetColor(color2);
         
                    switch(direction)    //Direction matter because different portions of the circle are drawn
                    {
                       case GRAD_LEFT:
                           if(i>length) 
                               SetColor(EndColor);
                           Bar(right + yCur, top - xCur, right + yNew, bottom + xCur);
                            break;
                       
                       case GRAD_RIGHT:                     
                       case GRAD_DOUBLE_VER:
                           if(i>length) 
                               SetColor(EndColor);
                           Bar(left - yNew, top - xCur, left - yCur, bottom + xCur);           
                            break;
                       
                       case GRAD_UP:
                            if(i>length) 
                                SetColor(EndColor);
                            Bar(left - xCur, bottom + yCur, right + xCur, bottom + yNew);
                            break;

                       case GRAD_DOWN:
                       case GRAD_DOUBLE_HOR:
                            if(i>length) 
                                SetColor(EndColor);
                            Bar(left - xCur, top - yNew, right + xCur, top - yCur);
                            break;
                       
                       default: 
                           break;
                    }

				}
                // update current values
                xCur = xPos;
                yCur = yPos;
                state = CHECK;
                break;

 

            case FACE:
                if((right - left) || (bottom - top))
                {
                i = (top) - top + rad;
                //Calculate the starting RGB values
                ered = sred + ((rdiff*i) >> 8);
                egreen = sgreen + ((gdiff*i) >> 8);
                eblue = sblue + ((bdiff*i) >> 8);

                color1 = RGBConvert(ered,egreen,eblue);
 
                if(i>length) 
                    color1 = EndColor;  

                i = (bottom) - top + rad;
                //Calculate the ending RGB values
                ered = sred + ((rdiff*i) >> 8);
                egreen = sgreen + ((gdiff*i) >> 8);
                eblue = sblue + ((bdiff*i) >> 8);

                color2 = RGBConvert(ered,egreen,eblue);
 
                if(i>length) 
                    color2 = EndColor;
      
                length -= rad;   //Subtract the radius from the length needed for gradient   
              
                if(direction == GRAD_UP || direction == GRAD_DOWN || direction == GRAD_DOUBLE_HOR)
                {
                   if(length>= bottom-top)
                   {
                       length = 100;
                   }
                   else
                   {
                       length *= 100;
                       length /= (bottom -top);
                   }               
                   BarGradient(left-rad, top, right+rad, bottom,color1,color2,length,direction);
                }
                else
                {
                  if(length>=right-left)
                  {
                      length = 100;
                  }
                  else
                  {
                       length *= 100;
                       length /= (right - left);
                  }
                   BarGradient(left, top-rad, right, bottom+rad,color1,color2,length,direction);
                }
            
                    state = WAITFORDONE;
                }
                else
                {
                    state = BEGIN;
                    return (1);
                }

            case WAITFORDONE:
                while(IsDeviceBusy());
                state = BEGIN;
                return (1);
        }           // end of switch
    }               // end of while
  
}
#endif


#ifdef USE_COMP_RLE

    #ifdef USE_BITMAP_FLASH

        #if (COLOR_DEPTH >= 8)
/*********************************************************************
* Function: WORD DecodeRLE8(FLASH_BYTE *flashAddress, BYTE *pixelrow, WORD size)
*
* PreCondition: tempFlashAddress should point to the beginning of a RLE compressed block
*
* Input: flashAddress - Address of the beginning of a RLE compressed block
*        pixelrow - Pointer to an array where the decoded row must be stored
*        size - Size of the decoded data in bytes
*
* Output: Number of source bytes traversed
*
* Side Effects: none
*
* Overview: Decodes the data
*
********************************************************************/
WORD DecodeRLE8(FLASH_BYTE *flashAddress, BYTE *pixel_row, WORD size)
{
    WORD sourceOffset = 0;
    WORD decodeSize = 0;

    while(decodeSize < size)
    {
        BYTE code = *flashAddress++;
        BYTE value = *flashAddress++;
        sourceOffset += 2;
        
        if(code > 0)
        {
            decodeSize += code;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                code -= (decodeSize - size);
            }
            
            while(code)
            {
                *pixel_row++ = value;
                code--;
            }
        }
        else
        {
            decodeSize += value;
            sourceOffset += value;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                value -= (decodeSize - size);
            }
            
            while(value)
            {
                *pixel_row++ = *flashAddress++;
                value--;
            }
        }
    }    
    return (sourceOffset);
}

/*********************************************************************
* Function: void PutImageRLE8BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - Should be NORMAL when RLE is used
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs the image starting from left,top coordinates
*
* Note: image must be located in internal memory
*
********************************************************************/
void PutImageRLE8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch)
{
    register FLASH_BYTE *flashAddress;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                xc, yc;
    BYTE                temp;
    BYTE                stretchX, stretchY;
#ifndef USE_PALETTE
    GFX_COLOR           imagePalette[256];
    WORD                counter;
#endif
    BYTE                pixelrow[GetMaxX() + 1];
    WORD                offset;
    BYTE                *pixelAddress;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    #if COLOR_DEPTH == 24
    flashAddress += 2;      // pad word for alignment
    #endif

    // Read pallete
    #ifdef USE_PALETTE
        // Adjust the address to skip the palette section.
        // Pixel data are indices not actual color data.
        // Palette to be used is assumed to be system level
        // defined. This supports only 16 bits per palette entry width.
        // 24 bits palette entry width is not supported.
        flashAddress = flashAddress + 512;
        
    #else
        // grab the palette of the image
        // the actual color used are in this palette
        for(counter = 0; counter < 256; counter++)
        {
            #if COLOR_DEPTH == 16
                imagePalette[counter] = *((FLASH_WORD *)flashAddress);
                flashAddress += 2;
            #endif
    
            #if COLOR_DEPTH == 24
                imagePalette[counter] = *((FLASH_DWORD *)flashAddress);
                flashAddress += 4;
            #endif
        }

    #endif
    
    yc = top;

    for(y = 0; y < sizeY; y++)
    {
        offset = DecodeRLE8(flashAddress, pixelrow, sizeX);
        flashAddress += offset;
        
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pixelAddress = pixelrow;
            xc = left;
            for(x = 0; x < sizeX; x++)
            {

                // Read pixels from flash
                temp = *pixelAddress;
                pixelAddress++;

                // Set color
            #ifdef USE_PALETTE
                SetColor(temp);
            #else
                SetColor(imagePalette[temp]);
            #endif

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    #ifdef USE_TRANSPARENT_COLOR
                        if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                    #endif
                            PutPixel(xc, yc);
                        xc++;    
                }
            }

            yc++;
        }
    }
}
        #endif //#if (COLOR_DEPTH >= 8)

        #if (COLOR_DEPTH >= 4)
        
/*********************************************************************
* Function: WORD DecodeRLE4(FLASH_BYTE *flashAddress, BYTE *pixelrow, WORD size)
*
* PreCondition: tempFlashAddress should point to the beginning of a RLE compressed block
*
* Input: flashAddress - Address of the beginning of a RLE compressed block
*        pixelrow - Pointer to an array where the decoded row must be stored
*        size - Size of the decoded data in bytes
*
* Output: Number of source bytes traversed
*
* Side Effects: none
*
* Overview: Decodes the data
*
********************************************************************/
WORD DecodeRLE4(FLASH_BYTE *flashAddress, BYTE *pixel_row, WORD size)
{
    WORD sourceOffset = 0;
    WORD decodeSize = 0;

    while(decodeSize < size)
    {
        BYTE code = *flashAddress++;
        BYTE value = *flashAddress++;
        BYTE counter;
        
        sourceOffset += 2;
        
        if(code > 0)
        {
            decodeSize += code;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                code -= (decodeSize - size);
            }
            
            for(counter = 0; counter < code; counter++)
            {
                if(counter & 0x01)
                {
                    *pixel_row++ = (value >> 4) & 0x0F;
                }
                else
                {
                    *pixel_row++ = (value) & 0x0F;
                }
            }
        }
        else
        {
            decodeSize += value;
            sourceOffset += (value + 1) >> 1;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                value -= (decodeSize - size);
            }
            
            for(counter = 0; counter < value; counter++)
            {
                if(counter & 0x01)
                {
                    *pixel_row++ = (*flashAddress >> 4) & 0x0F;
                    flashAddress++;
                }
                else
                {
                    *pixel_row++ = (*flashAddress) & 0x0F;
                }
            }
        }
    }

    return (sourceOffset);
}

/*********************************************************************
* Function: void PutImageRLE4BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - Should be NORMAL when RLE is used
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs the image starting from left,top coordinates
*
* Note: image must be located in internal memory
*
********************************************************************/
void PutImageRLE4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch)
{
    register FLASH_BYTE *flashAddress;
    WORD                sizeX, sizeY;
    register WORD       x, y;
    WORD                xc, yc;
    BYTE                temp = 0;
    register BYTE       stretchX, stretchY;
#ifndef USE_PALETTE
    GFX_COLOR           imagePalette[16];
    WORD                counter;
#endif
    BYTE                pixelrow[GetMaxX() + 1];
    WORD                offset;
    BYTE                *pixelAddress;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    #if COLOR_DEPTH == 24
    flashAddress += 2;          // pad for alignment
    #endif

    // Read pallete
    #ifdef USE_PALETTE
        // Adjust the address to skip the palette section.
        // Pixel data are indices not actual color data.
        // Palette to be used is assumed to be system level
        // defined. This supports only 16 bits per palette entry width.
        // 24 bits palette entry width is not supported.
        flashAddress = flashAddress + 32;

    #else
        for(counter = 0; counter < 16; counter++)
        {
            #if COLOR_DEPTH == 16    
                imagePalette[counter] = *((FLASH_WORD *)flashAddress);
                flashAddress += 2;
            #endif
    
            #if COLOR_DEPTH == 24
                imagePalette[counter] = *((FLASH_DWORD *)flashAddress);
                flashAddress += 4;
            #endif
        }
    #endif

    yc = top;
    
    for(y = 0; y < sizeY; y++)
    {
        offset = DecodeRLE4(flashAddress, pixelrow, sizeX);
        flashAddress += offset;

        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pixelAddress = pixelrow;
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *pixelAddress;
                pixelAddress++;

                #ifdef USE_PALETTE
                    SetColor(temp);
                #else
                    SetColor(imagePalette[temp]);
                #endif

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    #ifdef USE_TRANSPARENT_COLOR
                        if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                    #endif
                            PutPixel(xc, yc);
                        xc++;    
                }
            }

            yc++;
        }
    }
}
        #endif //#if (COLOR_DEPTH >= 4)

    #endif //    #ifdef USE_BITMAP_FLASH

    #ifdef USE_BITMAP_EXTERNAL
    
        #if (COLOR_DEPTH >= 8)

/*********************************************************************
* Function: WORD DecodeRLE8Ext(void *image, DWORD  memAddress, BYTE *pixelrow, WORD size)
*
* PreCondition: tempFlashAddress should point to the beginning of a RLE compressed block
*
* Input: image - External memory image pointer
*        memAddress - Address of the beginning of a RLE compressed block
*        pixelrow - Pointer to an array where the decoded row must be stored
*        size - Size of the decoded data in bytes
*
* Output: Number of source bytes traversed
*
* Side Effects: none
*
* Overview: Decodes the data from the external flash
*
********************************************************************/
WORD DecodeRLE8Ext(void *image, DWORD memAddress, BYTE *pixel_row, WORD size)
{
    WORD sourceOffset = 0;
    WORD decodeSize = 0;

    while(decodeSize < size)
    {
        BYTE code, value;
        
        ExternalMemoryCallback(image, memAddress, sizeof(BYTE), &code);
        memAddress++;
        ExternalMemoryCallback(image, memAddress, sizeof(BYTE), &value);
        memAddress++;

        sourceOffset += 2;
        
        if(code > 0)
        {
            decodeSize += code;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                code -= (decodeSize - size);
            }
            
            while(code)
            {
                *pixel_row++ = value;
                code--;
            }
        }
        else
        {
            decodeSize += value;
            sourceOffset += value;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                value -= (decodeSize - size);
            }
            
            ExternalMemoryCallback(image, memAddress, value * sizeof(BYTE), pixel_row);
            pixel_row += value;
            memAddress += value;
        }
    }    
    return (sourceOffset);
}

/*********************************************************************
* Function: void PutImageRLE8BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImageRLE8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
#ifndef USE_PALETTE
    GFX_COLOR       imagePalette[256];
#endif
    BYTE            pixelrow[(GetMaxX() + 1)];
    BYTE            *pixelAddress;

    BYTE            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            xc, yc;
    BYTE            stretchX, stretchY;
    WORD            offset;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Set offset to the image data, 256 is used since this function is for 8bpp images
    #ifdef USE_PALETTE
        // use system palette, skip the image palette and adjust the offset
        // assumes palette entry width is 16 bits, 
        // 24 bits palette width is not supported
        memOffset = sizeof(BITMAP_HEADER) + 256 * sizeof(WORD);
    #else
        memOffset = sizeof(BITMAP_HEADER) + 256 * sizeof(GFX_COLOR);

        // Get image palette, since this will be used to render the pixels
        // when system palette is used, the image palette is assumed to be
        // the same as the system palette so we do not fetch it
        ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 256 * sizeof(GFX_COLOR), imagePalette);

    #endif

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    yc = top;
    for(y = 0; y < sizeY; y++)
    {
        offset = DecodeRLE8Ext(image, memOffset, pixelrow, sizeX);
        memOffset += offset;
        
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pixelAddress = pixelrow;
            xc = left;
            for(x = 0; x < sizeX; x++)
            {

                // Read pixels from flash
                temp = *pixelAddress;
                pixelAddress++;

                // Set color
            #ifdef USE_PALETTE
                SetColor(temp);
            #else
                SetColor(imagePalette[temp]);
            #endif

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    #ifdef USE_TRANSPARENT_COLOR
                        if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                    #endif
                            PutPixel(xc, yc);
                        xc++;    
                }
            }

            yc++;
        }
    }
}
        #endif //#if (COLOR_DEPTH >= 8)
        
        #if (COLOR_DEPTH >= 4)

/*********************************************************************
* Function: WORD DecodeRLE4Ext(void *image, DWORD memAddress, BYTE *pixelrow, WORD size)
*
* PreCondition: tempFlashAddress should point to the beginning of a RLE compressed block
*
* Input: image - External memory image pointer
*        memAddress - Address of the beginning of a RLE compressed block
*        pixelrow - Pointer to an array where the decoded row must be stored
*        size - Size of the decoded data in bytes
*
* Output: Number of source bytes traversed
*
* Side Effects: none
*
* Overview: Decodes the data
*
********************************************************************/
WORD DecodeRLE4Ext(void *image, DWORD memAddress, BYTE *pixel_row, WORD size)
{
    WORD sourceOffset = 0;
    WORD decodeSize = 0;

    while(decodeSize < size)
    {
        BYTE code, value;
        BYTE counter, temp;
        
        ExternalMemoryCallback(image, memAddress, sizeof(BYTE), &code);
        memAddress++;
        ExternalMemoryCallback(image, memAddress, sizeof(BYTE), &value);
        memAddress++;
        
        sourceOffset += 2;
        
        if(code > 0)
        {
            decodeSize += code;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                code -= (decodeSize - size);
            }
            
            for(counter = 0; counter < code; counter++)
            {
                if(counter & 0x01)
                {
                    *pixel_row++ = (value >> 4) & 0x0F;
                }
                else
                {
                    *pixel_row++ = (value) & 0x0F;
                }
            }
        }
        else
        {
            decodeSize += value;
            sourceOffset += (value + 1) >> 1;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                value -= (decodeSize - size);
            }
            
            for(counter = 0; counter < value; counter++)
            {
                if(counter & 0x01)
                {
                    *pixel_row++ = (temp >> 4) & 0x0F;
                    memAddress++;
                }
                else
                {
                    ExternalMemoryCallback(image, memAddress, sizeof(BYTE), &temp);
                    *pixel_row++ = temp & 0x0F;
                }
            }
        }
    }

    return (sourceOffset);
}

/*********************************************************************
* Function: void PutImageRLE4BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImageRLE4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
#ifndef USE_PALETTE
    GFX_COLOR       imagePalette[16];
#endif
    BYTE            pixelrow[(GetMaxX() + 1)];
    BYTE            *pixelAddress;

    BYTE            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            xc, yc;
    BYTE            stretchX, stretchY;
    WORD            offset;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Set offset to the image data, 16 is used since this function is for 4bpp images
    #ifdef USE_PALETTE
        // assumes palette entry width is 16 bits, 
        // 24 bits palette width is not supported
        memOffset = sizeof(BITMAP_HEADER) + 16 * sizeof(WORD);
    #else
        memOffset = sizeof(BITMAP_HEADER) + 16 * sizeof(GFX_COLOR);

        // Get image palette, since this will be used to render the pixels
        // when system palette is used, the image palette is assumed to be
        // the same as the system palette so we do not fetch it
        ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 16 * sizeof(GFX_COLOR), imagePalette);

    #endif

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    yc = top;
    for(y = 0; y < sizeY; y++)
    {
        offset = DecodeRLE4Ext(image, memOffset, pixelrow, sizeX);
        memOffset += offset;
        
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pixelAddress = pixelrow;
            xc = left;
            for(x = 0; x < sizeX; x++)
            {

                // Read pixels from flash
                temp = *pixelAddress;
                pixelAddress++;

                // Set color
            #ifdef USE_PALETTE
                SetColor(temp);
            #else
                SetColor(imagePalette[temp]);
            #endif

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    #ifdef USE_TRANSPARENT_COLOR
                        if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                    #endif
                            PutPixel(xc, yc);
                        xc++;    
                }
            }

            yc++;
        }
    }
}
        #endif //#if (COLOR_DEPTH >= 4)

    #endif //#ifdef USE_BITMAP_EXTERNAL
    
#endif //#ifdef USE_COMP_RLE

void GetCirclePoint(SHORT radius, SHORT angle, SHORT *x, SHORT *y)
{
    DWORD   rad;
    SHORT   ang;
    SHORT   temp;

    ang = angle % 45;
    if((angle / 45) & 0x01)
        ang = 45 - ang;

    rad = radius;
    // there is a shift by 8 bits here since this function assumes a shift on the calculations for accuracy
    // and to avoid long and complex multiplications.
    rad *= ((DWORD) GetSineCosine(ang, GETCOSINE) << 8);

    *x = ((DWORD_VAL) rad).w[1];
    rad = radius;
    rad *= ((DWORD) GetSineCosine(ang, GETSINE) << 8);

    *y = ((DWORD_VAL) rad).w[1];

    if(((angle > 45) && (angle < 135)) || ((angle > 225) && (angle < 315)))
    {
        temp = *x;
        *x = *y;
        *y = temp;
    }

    if((angle > 90) && (angle < 270))
    {
        *x = -*x;
    }

    if((angle > 180) && (angle < 360))
    {
        *y = -*y;
    }
}


