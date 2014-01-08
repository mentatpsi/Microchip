/*****************************************************************************
 *  Module for Microchip Graphics Library 
 *  Graphic Primitives Layer
 *****************************************************************************
 * FileName:        Primitive.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
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
 * 11/12/07	   Version 1.0 release
 * 18/05/09    Version 1.1 - All Primitive functions have
 *                           return value(To support 2d-Acceleration)
 * 05/12/10    Added sine and cosine functions.
 * 08/20/10    Modified TYPE_MEMORY enum for more types
 *			   and changed the enumeration to GFX_RESOURCE. Also removed 
 *             unused types. 
 *             Added new structure GFX_IMAGE_HEADER. 
 *             Deprecated the following (renamed):
 *                TYPE_MEMORY     
 *                EXTDATA         
 *                BITMAP_FLASH    
 *                BITMAP_RAM      
 *                BITMAP_EXTERNAL  
 * 03/09/11    Removed USE_DRV_xxx.
 * 04/01/11    Added GetSineCosine().
 * 01/05/12     - Removed the _font global pointer and replaced with currentFont
 *                structure to make faster OutChar() rendering. 
 *              - Removed _fontFirstChar, _fontLastChar and _fontHeight globals.
 *                These are now included in currentFont.
 *              - break up SetFont(), OutChar() and GetTextWidth() to allow
 *                versatility of implementing text rendering functions in drivers.
 *              - added extended glyph support and font anti-aliasing
 * 06/15/12     - added Alpha Blending Lite support.    
 *
 *****************************************************************************/
#ifndef _PRIMITIVE_H
    #define _PRIMITIVE_H

#include "GenericTypeDefs.h"
#include "GraphicsConfig.h"
#include "gfxcolors.h"
/*********************************************************************
* Overview: Primitive lines are drawn using line type and line thickness.
*			There are 3 line styles and 2 types of line thickness.
*
*********************************************************************/

// Solid Line Style
    #define SOLID_LINE  0

// Dotted Line Style
    #define DOTTED_LINE 1

// Dashed Line Style
    #define DASHED_LINE 4

// Normal Line (thickness is 1 pixel)
    #define NORMAL_LINE 0

// Thick Line (thickness is 3 pixels)
    #define THICK_LINE  1

/*********************************************************************
* Overview: Drawing bitmaps will have two modes, normal rendering
*			and stretched rendering. Stretched rendering effectively
*			doubles the image size in the horizontal and vertical 
*			direction.
*
*********************************************************************/

// Normal image stretch code
    #define IMAGE_NORMAL    1

// Stretched image stretch code
    #define IMAGE_X2    2

// Current line style
extern SHORT    _lineType;

// Current line thickness
extern BYTE     _lineThickness;

// constants used for circle/arc computation
    #define SIN45   	46341   // sin(45) * 2^16)
    #define ONEP25  	81920   // 1.25 * 2^16
// constants used to get sine(v) and cosine(v) 
	#define GETSINE		0x00
	#define GETCOSINE	0x01

// Current cursor coordinates
extern SHORT    _cursorX;
extern SHORT    _cursorY;

// Font orientation
extern BYTE     _fontOrientation;

    #define ORIENT_HOR  0
    #define ORIENT_VER  1

extern BYTE     _antialiastype;
#ifdef USE_ALPHABLEND_LITE
extern GFX_COLOR    _prevAlphaColor;
#endif

/*********************************************************************
* Overview: Fonts that enables anti-aliasing can be set to use
*           opaque or translucent type of anti-aliasing.
*
*********************************************************************/

// Mid colors are calculated only once while rendering each character. This is ideal for rendering text over a constant background.
    #define ANTIALIAS_OPAQUE        0

// Mid values are calculated for every necessary pixel. This feature is useful when rendering text over an image 
// or when the background is not one flat color.
    #define ANTIALIAS_TRANSLUCENT   1

/*********************************************************************
* Overview: This macro sets the data type for the strings and characters.
*			There are three types used for XCHAR and the type is selected by  
*           adding one of the macros in GraphicsConfig.h. 
*	<TABLE>
*    	In GraphicsConfig.h         XCHAR	                        Description
*     	##########################  #############################	##########################
*     	#define USE_MULTIBYTECHAR   #define XCHAR   unsigned short	Use multibyte characters (0-2^16 range).
*     	#define USE_UNSIGNED_XCHAR  #define XCHAR   unsigned char	Use unsigned char (0-255 range).
*     	none of the two defined     #define XCHAR   char	        Use signed char (0-127 range).
*	</TABLE>
*
*           Note: Only one of the two or none at all are defined in GraphicsConfig.h.
*                 - #define USE_MULTIBYTECHAR 
*                 - #define USE_UNSIGNED_XCHAR 
*                 - when none are defined, XCHAR defaults to type char.
*********************************************************************/
    
    #if defined (USE_MULTIBYTECHAR) 
    
        #define XCHAR   unsigned short
        #define UXCHAR  unsigned short
        
    #elif defined (USE_UNSIGNED_XCHAR)
    
        #define XCHAR   unsigned char
        #define UXCHAR  unsigned char
        
    #else
    
        #define XCHAR   char
        #define UXCHAR  unsigned char
        
    #endif

// bevel drawing type (0 = full bevel, 0xF0 - top bevel only, 0x0F - bottom bevel only
extern BYTE _bevelDrawType;

	#define DRAWFULLBEVEL  	0xFF 
	#define DRAWTOPBEVEL  	0xF0 
	#define DRAWBOTTOMBEVEL 0x0F 

    #ifdef __PIC32MX__
        // Flash data with 32bit pointers
        #define FLASH_BYTE  const BYTE
        #define FLASH_WORD  const WORD
        #define FLASH_DWORD const DWORD
    #else
        // Flash data with 24bit pointers
        #define FLASH_BYTE  __prog__ char
        #define FLASH_WORD  __prog__ short int
        #define FLASH_DWORD __prog__ unsigned long
    #endif

/*********************************************************************
* Overview: This defines the size of the buffer used by font functions
*			to retrieve font data from the external memory. The buffer 
*			size can be increased to accommodate large font sizes. 
*			The user must be aware of the expected glyph sizes of the 
*			characters stored in the font table. 
*           To modify the size used, declare this macro in the 
*           GraphicsConfig.h file with the desired size.
*
********************************************************************/
#ifndef EXTERNAL_FONT_BUFFER_SIZE
    #define EXTERNAL_FONT_BUFFER_SIZE   600
#endif

/*********************************************************************
* Overview: Memory type enumeration to determine the source of data.
*           Used in interpreting bitmap and font from different 
*           memory sources.
*
*********************************************************************/
typedef enum
{
	FLASH         = 0x0000,                 // internal flash
	EXTERNAL      = 0x0001,                 // external memory
	FLASH_JPEG    = 0x0002,                 // internal flash
	EXTERNAL_JPEG = 0x0003,                 // external memory
	RAM           = 0x0004,                 // RAM 
	EDS_EPMP      = 0x0005,                 // memory in EPMP, base addresses are 
                                            // are set in the hardware profile

	IMAGE_MBITMAP = 0x0000,                 // data resource is type Microchip bitmap
	IMAGE_JPEG    = 0x0100,                 // data resource is type JPEG

    COMP_NONE     = 0x0000,                 // no compression  
	COMP_RLE      = 0x1000,                 // compressed with RLE
	COMP_IPU      = 0x2000,                 // compressed with DEFLATE (for IPU)
} GFX_RESOURCE;

#define GFX_COMP_MASK   0xF000
#define GFX_MEM_MASK    0x000F

/*********************************************************************
* Overview: This structure is used to describe external memory.
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE type;                      // Resource type. Valid types are: 
                                            // - EXTERNAL 
                                            // - EDS_EPMP
    WORD         ID;                        // Memory ID, user defined value to differentiate
                                            // between graphics resources of the same type.	
                                            // When using EDS_EPMP the following ID values are
                                            // reserved and used by the Microchip display driver
                                            //   - 0 - reserved (do not use)
                                            //   - 1 - reserved for base address of EPMP CS1
                                            //   - 2 - reserved for base address of EPMP CS2
    DWORD        address;                   // Data image address (user data, bitmap or font) 
} GFX_EXTDATA;

/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
//                         STRUCTURES FOR IMAGES
/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */

/*********************************************************************
* Overview: Structure describing the bitmap header.
*
*********************************************************************/
typedef struct
{
    BYTE        compression;                // Compression setting 
    BYTE        colorDepth;                 // Color depth used
    SHORT       height;                     // Image height
    SHORT       width;                      // Image width
} BITMAP_HEADER;

/*********************************************************************
* Overview: Structure describing the partial image area to render.
*
*********************************************************************/
typedef struct
{
    WORD         width;                       // Parital Image width
    WORD         height;                      // Partial Image height
    SHORT        xoffset;                     // xoffset of image 
    SHORT        yoffset;                     // yoffset of image 
} PUTIMAGE_PARAM;

/*********************************************************************
* Overview: Structure for images stored in FLASH memory.
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE type;                      // must be FLASH
    FLASH_BYTE   *address;                  // image address in FLASH
} IMAGE_FLASH;

/*********************************************************************
* Overview: Structure for images stored in RAM memory.
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE type;                      // must be RAM
    DWORD        *address;                  // image address in RAM
} IMAGE_RAM;

/*********************************************************************
* Overview: Structure for images stored in EXTERNAL memory space.
*           (example: External SPI or parallel Flash, EDS_EPMP)
*
*********************************************************************/
typedef GFX_EXTDATA IMAGE_EXTERNAL;

/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
//                         STRUCTURES FOR FONTS
/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */

/*********************************************************************
* Overview: Structure describing the font header.
*
*********************************************************************/
typedef struct
{
    BYTE        fontID;                     // User assigned value
    BYTE        extendedGlyphEntry : 1;     // Extended Glyph entry flag. When set font has extended glyph feature enabled.
    BYTE        res1               : 1;     // Reserved for future use  (must be set to 0)
    BYTE        bpp                : 2;     // Actual BPP = 2<sup>bpp</sup>  
                                            //   - 0 - 1 BPP
                                            //   - 1 - 2 BPP
                                            //   - 2 - 4 BPP
                                            //   - 3 - 8 BPP
    BYTE        orientation        : 2;     // Orientation of the character glyphs (0,90,180,270 degrees)
                                            //   - 00 - Normal
                                            //   - 01 - Characters rotated 270 degrees clockwise
                                            //   - 10 - Characters rotated 180 degrees
                                            //   - 11 - Characters rotated 90 degrees clockwise
                                            // Note: Rendering DO NOT rotate the characters. The table contains rotated characters
                                            //       and will be rendered as is. 
    BYTE        res2               : 2;     // Reserved for future use (must be set to 0).
    WORD        firstChar;                  // Character code of first character (e.g. 32).
    WORD        lastChar;                   // Character code of last character in font (e.g. 3006).
    WORD        height;                     // Font characters height in pixels. 
} FONT_HEADER;

/*********************************************************************
* Overview: Structures describing the glyph entry.
*
*********************************************************************/
typedef struct
{
    BYTE        width;                      // width of the glyph
    BYTE        offsetLSB;                  // Least Significant Byte of the glyph location offset 
    WORD        offsetMSB;                  // Most Significant (2) Bytes of the glyph location offset
} GLYPH_ENTRY;

typedef struct
{
    DWORD       offset;                     // Offset Order: LSW_LSB LSW_MSB MSW_MSB MSW_MSB
    WORD        cursorAdvance;              // x-value by which cursor has to advance after rendering the glyph
    WORD        glyphWidth;                 // width of the glyph
    INT16       xAdjust;                    // x-position is adjusted as per this signed number
    INT16       yAdjust;                    // y-position is adjusted as per this signed number
} GLYPH_ENTRY_EXTENDED;

/**************************************************************************
 * Overview: Font space section.  The fonts can be located in psv (constant)
 * or program space in PIC24/dsPIC MCUs.  This define allows for switching
 * of the pointer type used to access the font structure in memory
 * See: GraphicsConfig.h for the application define.
 ***************************************************************************/
#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
#ifndef __PIC32MX__
#define GFX_FONT_SPACE __prog__
#else
#define GFX_FONT_SPACE const
#endif
#else
#define GFX_FONT_SPACE const
#endif
/*********************************************************************
* Overview: Internal structure for currently set font. 
*
*********************************************************************/
typedef struct
{
   void         *pFont;                     // pointer to the currently set font
   FONT_HEADER  fontHeader;                 // copy of the currently set font header
#ifdef USE_ANTIALIASED_FONTS
   BYTE  antiAliasType;                     // anti-alias type set
#endif
} GFX_FONT_CURRENT;

/*********************************************************************
* Overview: Structure for character information when rendering the character.
*
*********************************************************************/
typedef struct 
{ 
#ifdef USE_FONT_FLASH	
    GFX_FONT_SPACE GLYPH_ENTRY             *pChTable;
    GFX_FONT_SPACE GLYPH_ENTRY_EXTENDED    *pChTableExtended;
#endif
#ifdef USE_FONT_EXTERNAL
    BYTE                    chImage[EXTERNAL_FONT_BUFFER_SIZE];
    SHORT                   chEffectiveGlyphWidth;
#endif
    BYTE                    bpp;
	SHORT                   chGlyphWidth;
    GFX_FONT_SPACE BYTE     *pChImage;
    SHORT                   xAdjust;
    SHORT                   yAdjust;
    SHORT                   xWidthAdjust;
    SHORT                   heightOvershoot;
} OUTCHAR_PARAM;

/*********************************************************************
* Overview: Structure for font stored in FLASH memory.
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE  type;                     // must be FLASH
    GFX_FONT_SPACE char    *address;                 // font image address in FLASH
} FONT_FLASH;

/*********************************************************************
* Overview: Structure for font stored in RAM memory.
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE  type;                     // must be RAM
    char          *address;                 // font image address in RAM
} FONT_RAM;

/*********************************************************************
* Overview: Structure for font stored in EXTERNAL memory space.
*           (example: External SPI or parallel Flash, EDS_EPMP)
*
*********************************************************************/
typedef GFX_EXTDATA FONT_EXTERNAL;

/*********************************************************************
* Overview: Structure for images stored in various system memory 
*           (Flash, External Memory (SPI, Parallel Flash, 
*           or memory in EPMP).
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE     type;                  // Graphics resource type, determines the type and location of data
    WORD             ID;                    // memory ID, user defined value to differentiate
                                            // between graphics resources of the same type	
							                //   When using EDS_EPMP the following ID values are
							                //   reserved and used by the Microchip display driver
									        //   0 - reserved (do not use)
							                //   1 - reserved for base address of EPMP CS1
							                //   2 - reserved for base address of EPMP CS2 
	union
	{
    	DWORD        extAddress;	        // generic address	
        FLASH_BYTE   *progByteAddress;      // for addresses in program section
        FLASH_WORD   *progWordAddress;      // for addresses in program section
	    const char   *constAddress;         // for addresses in FLASH
	    char         *ramAddress;           // for addresses in RAM
#if defined(__PIC24F__) 	    
	    __eds__ char *edsAddress;           // for addresses in EDS
#endif	    
	} LOCATION;
    
	WORD             width;                 // width of the image 
	WORD             height;                // height of the image
    DWORD            param1;                // Parameters used for the GFX_RESOURCE. Depending on the GFX_RESOURCE type 
                                            // definition of param1 can change. For IPU and RLE compressed images, param1 
                                            // indicates the compressed size of the image.
    DWORD            param2;                // Parameters used for the GFX_RESOURCE. Depending on the GFX_RESOURCE type 
                                            // definition of param2 can change. For IPU and RLE compressed images, param2 
                                            // indicates the uncompressed size of the image.
	WORD             colorDepth;            // color depth of the image
} GFX_IMAGE_HEADER;

/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
//                         DEPRECATED TYPES and VARIABLES
/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
typedef GFX_RESOURCE TYPE_MEMORY     __attribute__ ((deprecated));
typedef GFX_EXTDATA  EXTDATA         __attribute__ ((deprecated));
typedef IMAGE_FLASH  BITMAP_FLASH    __attribute__ ((deprecated));
typedef IMAGE_RAM    BITMAP_RAM      __attribute__ ((deprecated));
typedef GFX_EXTDATA  BITMAP_EXTERNAL __attribute__ ((deprecated));

// information on currently set font
extern GFX_FONT_CURRENT currentFont;

/*********************************************************************
* Function: WORD Arc(SHORT xL, SHORT yT, SHORT xR, SHORT yB, 
*					 SHORT r1, SHORT r2, BYTE octant)
*
* Overview: Draws the octant arc of the beveled figure with the given centers, 
*           radii and octant mask.
*           When octant = 0xFF and the following are true:
*           1. xL = xR, yT = yB , r1 = 0 and r2 = z, a filled circle is drawn with a radius of z.
*           2. radii have values (where r1 < r2), a full ring with thickness of (r2-r1) is drawn.
*           3. xL != xR, yT != yB , r1 = 0 and r2 = 0 (where xR > xL and yB > yT) a rectangle is 
*              drawn. xL, yT specifies the left top corner and xR, yB specifies the right bottom corner.
*           When octant != 0xFF the figure drawn is the subsection of the 8 section figure where 
*           each non-zero bit of the octant value specifies the octants that will be drawn.
*
* Description: 
*          <img name="Arc.jpg" />
*
* PreCondition: none
*
* Input: xL - x location of the upper left center in the x,y coordinate.
*		 yT - y location of the upper left center in the x,y coordinate.
*		 xR - x location of the lower right center in the x,y coordinate.
*		 yB - y location of the lower right center in the x,y coordinate.
*		 r1 - The smaller radius of the two concentric cicles that defines the thickness
*			  of the object.
*	     r2 - The larger of radius the two concentric circles that defines the thickness 
*	     	  of the object.
*		 octant - Bitmask of the octant that will be drawn.
*				  Moving in a clockwise direction from x = 0, y = +radius
*                 - bit0 : first octant 
*                 - bit1 : second octant
*                 - bit2 : third octant 
*                 - bit3 : fourth octant
*                 - bit4 : fifth octant
*                 - bit5 : sixth octant
*                 - bit6 : seventh octant
*                 - bit7 : eight octant
*
* Output: Returns the rendering status.
*		  1 - If the rendering was completed and 
*		  0 - If the rendering is not yet finished. 
*
* Side Effects: none
*
********************************************************************/
WORD            Arc(SHORT xL, SHORT yT, SHORT xR, SHORT yB, SHORT r1, SHORT r2, BYTE octant);

/*********************************************************************
* Function:  void InitGraph(void)
*
* Overview: This function initializes the display controller, sets 
*			the line type to SOLID_LINE, sets the screen to all BLACK, 
*			sets the current drawing color to WHITE, sets the graphic 
*			cursor position to upper left corner of the screen, sets 
*			active and visual pages to page #0, clears the active page 
*			and disables clipping. This function should be called before 
*			using the Graphics Primitive Layer.
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
void            InitGraph(void);

/*********************************************************************
* Macros:  GetX()
*
* Overview: This macro returns the current graphic cursor x-coordinate.
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
    #define GetX()  _cursorX

/*********************************************************************
* Macros:  GetX()
*
* Overview: This macro returns the current graphic cursor y-coordinate.
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
    #define GetY()  _cursorY

/*********************************************************************
* Macros:  MoveTo(x,y)
*
* Overview: This macro moves the graphic cursor to new x,y position.
*
* PreCondition: none
*
* Input: x - Specifies the new x position of the graphic cursor.
*        y - Specifies the new y position of the graphic cursor.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define MoveTo(x, y) \
    _cursorX = x;        \
    _cursorY = y;

/*********************************************************************
* Macros:  MoveRel(dX,dY)
*
* Overview: This macro moves the graphic cursor relative to the 
*			current location. The given dX and dY displacement can 
*			be positive or negative numbers.
*
* PreCondition: none
*
* Input: dX - Specifies the displacement of the graphic cursor for
*			  the horizontal direction.
*		 dY - Specifies the displacement of the graphic cursor for
*			  the vertical direction.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define MoveRel(dX, dY) \
    _cursorX += dX;         \
    _cursorY += dY;

/*********************************************************************
* Macro: SetFontOrientation(orient)
*
* Overview: Sets font orientation vertical or horizontal.
*
* PreCondition: none
*
* Input: orient - sets font orientation when rendering characters and strings.
*        - 1 when font orientation is vertical
*        - 0 when font orientation is horizontal
*
* Output: none
*
* Example:
*   See GetFontOrientation() example.

********************************************************************/
    #define SetFontOrientation(orient)  _fontOrientation = orient;

/*********************************************************************
* Macro: GetFontOrientation()
*
* Overview: Returns font orientation.
*
* PreCondition: none
*
* Input: none
*
* Output: Return the current font orientation. 
*         - 1 when font orientation is vertical
*         - 0 when font orientation is horizontal
*
* Example:
*   <CODE> 
*	void PlaceText(SHORT x, SHORT y, WORD space, XCHAR *pString)
*	{
*		SHORT width;
*		
*		SetColor(BRIGHTRED);                // set color
*		SetFont(pMyFont);                   // set to use global font 
*		
*		// get string width
*		width = GetTextWidth(pString, pMyFont);
*
*       // check if it fits
*       if (space < width)
*       {
*           if (GetFontOrientation() == 0)
*               // reset the orientation to vertical 
*               SetFontOrientation(1);
*       }
*       else
*       {
*           if (GetFontOrientation() == 1)
*               // reset the orientation to horizontal
*               SetFontOrientation(0);
*       }
*		// place string in the middle of the screen
*		OutTextXY(x, y, pString);
*	}
*	</CODE> 
*
********************************************************************/
    #define GetFontOrientation()    _fontOrientation

/*********************************************************************
* Macro: GFX_Font_SetAntiAliasType(transparency)
*
* Overview: Sets font anti-alias type to either Translucent or opaque.
*
* PreCondition: Compiler switch USE_ANTIALIASED_FONTS must be enabled
*
* Input: transparency - sets font font anti-alias type
*        - ANTIALIAS_TRANSLUCENT - (or 1) when font anti-alias type is translucent
*        - ANTIALIAS_OPAQUE - (or 0) when font anti-alias type is opaque
*
* Output: none
*
********************************************************************/
    #ifdef USE_ANTIALIASED_FONTS
        #define GFX_Font_SetAntiAliasType(transparency)  _antialiastype = transparency;
    #else
        #define GFX_Font_SetAntiAliasType(transparency)
    #endif

/*********************************************************************
* Macro: GFX_Font_GetAntiAliasType()
*
* Overview: Returns the font anti-alias type.
*
* PreCondition: Compiler switch USE_ANTIALIASED_FONTS must be enabled
*
* Input: none
*
* Output: Return the current font anti-alias type.
*        - ANTIALIAS_TRANSLUCENT - (or 1) when font anti-alias is type translucent
*        - ANTIALIAS_OPAQUE - (or 0) when font anti-alias is type opaque
*
********************************************************************/
    #ifdef USE_ANTIALIASED_FONTS
        #define GFX_Font_GetAntiAliasType()  _antialiastype
    #else
        #define GFX_Font_GetAntiAliasType()
    #endif

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
*           currently set font from Flash memory. This is a step performed
*           before the character is rendered. 
*
* Note: Application should not call this function. This function is for 
*       versatility of implementing hardware accelerated text rendering
*       only.
*
********************************************************************/
void    OutCharGetInfoFlash (XCHAR ch, OUTCHAR_PARAM *pParam);

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
void    OutCharGetInfoExternal(XCHAR ch, OUTCHAR_PARAM *pParam);

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
WORD    OutCharRender(XCHAR ch, OUTCHAR_PARAM *pParam);

/*********************************************************************
* Function: WORD OutChar(XCHAR ch)
*
* Overview: This function outputs a character from the current graphic 
*		    cursor position. OutChar() uses the current active font 
*		    set with SetFont(). 
*
* PreCondition: none
*
* Input: ch - The character code to be displayed.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the character is not yet completely drawn.
*         - Returns 1 when the character is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Example:
*   <CODE> 
*   static WORD counter = 0;
*   XCHAR   ch;
*
*   // render characters until null character
*   while((XCHAR)(ch = *(textString + counter)) != 0)
*   {
*       if(OutChar(ch) == 0)
*           return (0);
*       counter++;
*   }
*
* </CODE>
*
* Side Effects: After the function is completed, the graphic cursor 
*			    position is moved in the horizontal direction by the 
*			    character width. Vertical position of the graphic cursor 
*			    is not changed.
*
********************************************************************/
WORD    OutChar(XCHAR ch);

/*********************************************************************
* Function: WORD OutText(XCHAR* textString)
*
* Overview: This function outputs a string of characters starting 
*			at the current graphic cursor position. The string must 
*			be terminated by a line feed or zero. For Non-Blocking 
*			configuration, OutText() may return control to the program 
*			due to display device busy status. When this happens zero 
*			is returned and OutText() must be called again to continue 
*			the outputting of the string. For Blocking configuration, 
*			this function always returns a 1. OutText() uses the current 
*			active font set with SetFont().
*
*
* Input: textString - Pointer to the string to be displayed.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when string is not yet outputted completely.
*         - Returns 1 when string is outputted completely.
*         For Blocking configuration:
*         - Always return 1.
*
* Example:
*   <CODE> 
*       SetFont(pMyFont);
*       SetColor(WHITE);
*       // place the string at the upper left corner of the screen        
*       MoveTo(0, 0);
*       OutText("Test String!");
*  </CODE>
*
* Side Effects: Current horizontal graphic cursor position will be moved 
*				to the end of the text. The vertical graphic cursor 
*				position will not be changed.
*
********************************************************************/
WORD    OutText(XCHAR *textString);

/*********************************************************************
* Function: WORD OutTextXY(SHORT x, SHORT y, XCHAR* textString)
*
* Overview: This function outputs a string of characters starting 
*			at the given x, y position. The string must be terminated 
*			by a line feed or zero. For Non-Blocking configuration, 
*			OutTextXY() may return control to the program due to 
*			display device busy status. When this happens zero is 
*			returned and OutTextXY() must be called again to continue 
*			the outputting of the string. For Blocking configuration, 
*			this function always returns a 1. OutTextXY() uses the 
*			current active font set with SetFont().
*
* Input: x - Defines the x starting position of the string.
*		 y - Defines the y starting position of the string.
*		 textString - Pointer to the string to be displayed.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when string is not yet outputted completely.
*         - Returns 1 when string is outputted completely.
*         For Blocking configuration:
*         - Always return 1.
*
* Example:
*   <CODE> 
*	void PlaceText(void)
*	{
*		SHORT width, height;
*		static const XCHAR text[] = "Touch screen to continue";
*		
*		SetColor(BRIGHTRED);                // set color
*		SetFont(pMyFont);                   // set font to my font
*		
*		// get string width & height
*		width = GetTextWidth(text, pMyFont);
*		height = GetTextHeight(pMyFont);
*		
*		// place string in the middle of the screen
*		OutTextXY(  (GetMaxX() - width) >> 1, \
*		(GetMaxY() – height) >> 1, \
*		(char*)text);
*	}
*	</CODE> 
*
* Side Effects: Current horizontal graphic cursor position will be 
*				moved to the end of the text. The vertical graphic 
*				cursor position will not be changed.
*
********************************************************************/
WORD    OutTextXY(SHORT x, SHORT y, XCHAR *textString);

/*********************************************************************
* Function: SHORT GetTextHeight(void* pFont)
*
* Overview: This macro returns the height of the specified font. 
*			All characters in a given font table have a constant 
*			height.
*
* Input: pFont - Pointer to the font image.
*
* Output: Returns the font height.
*
* Example:
*   See OutTextXY() example.
*
* Side Effects: none
*
********************************************************************/
SHORT   GetTextHeight(void *pFont);

/*********************************************************************
* Function: SHORT GetTextWidth(XCHAR* textString, void* pFont)
*
* Overview: This function returns the width of the specified string 
*			for the specified font. The string must be terminated 
*			by a line feed or zero.
*
* Input: textString - Pointer to the string.
*		 pFont - Pointer to the font image.
*
* Output: Returns the string width in the specified font.
*
* Example:
*   See OutTextXY() example.
*
* Side Effects: none
*
********************************************************************/
SHORT   GetTextWidth(XCHAR *textString, void *pFont);

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
SHORT    GetTextWidthRam(XCHAR* textString, void* pFont);

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
SHORT GetTextWidthFlash(XCHAR* textString, void *pFont);

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
SHORT GetTextWidthExternal(XCHAR* textString, void *pFont);

/*********************************************************************
* Function: void SetFont(void* pFont)
*
* Overview: This function sets the current font used in OutTextXY(), 
*			OutText() and OutChar() functions.
*
* Input: pFont - Pointer to the new font image to be used.
*
* Output: none
*
* Example:
*   See OutTextXY() example.
*
* Side Effects: none
*
********************************************************************/
void    SetFont(void *pFont);

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
void    SetFontFlash(void *pFont);

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
void    SetFontExternal(void *pFont);

/*********************************************************************
* Macros: SetLineType(lnType)
*
* Overview: This macro sets the line type to draw.
*
* Input: lnType - The type of line to be used. 
*		 	Supported line types:
*				- SOLID_LINE
* 				- DOTTED_LINE
*          		- DASHED_LINE
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define SetLineType(lnType) _lineType = lnType;

/*********************************************************************
* Macros: SetLineThickness(lnThickness)
*
* Overview: This macro sets sets line thickness to 1 pixel or 3 pixels.
*
* Input: lnThickness - Line thickness code 
*           - NORMAL_LINE : 1 pixel
*           - THICK_LINE : 3 pixels
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define SetLineThickness(lnThickness)   _lineThickness = lnThickness;

/*********************************************************************
* Function: WORD Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
*
* Overview: This function draws a line with the current line type 
*	        from the start point to the end point.
*
* Input: x1 - x coordinate of the start point.
*		 y1 - y coordinate of the start point.
*		 x2 - x coordinate of the end point.
*		 y2 - y coordinate of the end point.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
*
* Side Effects: The graphic cursor position is moved to the end 
*				point of the line.
*
********************************************************************/
WORD Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2);

/*********************************************************************
* Macros: LineRel(dX, dY)
*
* Overview: This macro draws a line with the current line type from 
*			the current graphic cursor position to the position defined 
*			by displacement.
*
* Input: dX - Displacement from the current x position.
*		 dY - Displacement from the current y position.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: The graphic cursor position is moved to the end 
*				point of the line.
*
********************************************************************/
    #define LineRel(dX, dY) Line(GetX(), GetY(), GetX() + dX, GetY() + dY)

/*********************************************************************
* Macros: LineTo(x,y)
*
* Overview: This macro draws a line with the current line type from 
*			the current graphic cursor position to the given x, y position.
*
* Input: x - End point x position.
*		 y - End point y poisiton.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: The graphic cursor position is moved to the end 
*				point of the line.
*
********************************************************************/
    #define LineTo(x, y)    Line(_cursorX, _cursorY, x, y)

/*********************************************************************
* Macro: Circle(x, y, radius)
*
* Overview: This macro draws a circle with the given center and radius.
*
* Input: x - Center x position. 
*		 y - Center y position.
*		 radius - the radius of the circle.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
********************************************************************/
    #define Circle(x, y, radius)    Bevel(x, y, x, y, radius)

/*********************************************************************
* Macro: SetBevelDrawType(type)
*
* Overview: This macro sets the fill bevel type to be drawn.
*
* Input: type - is set using the following. 
*		 - DRAWFULLBEVEL to draw the full shape
*		 - DRAWTOPBEVEL to draw the upper half portion
*		 - DRAWBOTTOMBEVEL to draw the lower half portion
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
	#define SetBevelDrawType(type)		(_bevelDrawType = type)

/*********************************************************************
* Function: WORD Bevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
*
* Overview: Draws a beveled figure on the screen. 
*           When x1 = x2 and y1 = y2, a circular object is drawn. 
*           When x1 < x2 and y1 < y2 and rad (radius) = 0, a rectangular 
*           object is drawn.
*
* Description: 
*        <img name="Bevel.jpg" />
*
* Input: x1 - x coordinate position of the upper left center of the circle that 
*			  draws the rounded corners.
*		 y1 - y coordinate position of the upper left center of the circle that 
*			  draws the rounded corners.
*		 x2 - x coordinate position of the lower right center of the circle that 
*			  draws the rounded corners.
*		 y2 - y coordinate position of the lower right center of the circle that 
*			  draws the rounded corners.
*        rad - defines the redius of the circle, that draws the rounded corners.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
*
* Side Effects: none
*
********************************************************************/
WORD    Bevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad);

/*********************************************************************
* Function: WORD FillBevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
*
* Overview: Draws a filled beveled figure on the screen. 
*           For a filled circular object x1 = x2 and y1 = y2. 
*           For a filled rectangular object radius = 0.
*
* Description: 
*        <img name="FillBevel.jpg" />
*
* Input: x1 - x coordinate position of the upper left center of the circle that 
*			  draws the rounded corners.
*		 y1 - y coordinate position of the upper left center of the circle that 
*			  draws the rounded corners.
*		 x2 - x coordinate position of the lower right center of the circle that 
*			  draws the rounded corners.
*		 y2 - y coordinate position of the lower right center of the circle that 
*			  draws the rounded corners.
*        rad - defines the redius of the circle, that draws the rounded corners.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
********************************************************************/
WORD    FillBevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad);

/*********************************************************************
* Macro: FillCircle(SHORT x1, SHORT y1, SHORT rad)
*
* Overview: This macro draws a filled circle. Uses the FillBevel() function.
*
* Input: x1 - x coordinate position of the center of the circle.
*		 y1 - y coordinate position of the center of the circle.
*        rad - defines the redius of the circle.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
********************************************************************/
    #define FillCircle(x1, y1, rad) FillBevel(x1, y1, x1, y1, rad)

/*********************************************************************
* Macro: Rectangle(left, top, right, bottom)
*
* Overview: This macro draws a rectangle with the given left, 
*			top and right, bottom corners. Current line type is used.
*
* Input: left - x position of the left top corner.
*		 top - y position of the left top corner.
*		 right - x position of the right bottom corner.
*		 bottom - y position of the right bottom corner.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
********************************************************************/
    #define Rectangle(left, top, right, bottom) Bevel(left, top, right, bottom, 0)

/*********************************************************************
* Function: WORD DrawPoly(SHORT numPoints, SHORT* polyPoints)
*
* Overview: This function draws a polygon with the current line 
*			type using the given number of points. The polygon points 
*			(polyPoints) are stored in an array arranged in the following order:
*   <PRE> 
*            SHORT polyPoints[size] = {x0, y0, x1, y1, x2, y2 … xn, yn};
*            Where n = # of polygon sides
*                  size = numPoints * 2
*	</PRE> 
*            DrawPoly() draws any shape defined by the polyPoints. The function 
*            will just draw the lines connecting all the x,y points
*            enumerated by polyPoints[].
*
* Input: numPoints - Defines the number of x,y points in the polygon. 
*		 polyPoints - Pointer to the array of polygon points. The array defines 
*                     the x,y points of the polygon. 
*                     The sequence should be x0, y0, x1, y1, x2, y2, ... xn, yn where
*                     n is the # of polygon sides.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Example:
*	<CODE> 
*       SHORT OpenShapeXYPoints[6] = {10, 10, 20, 10, 20, 20};
*       SHORT ClosedShapeXYPoints[8] = {10, 10, 20, 10, 20, 20, 10, 10};
* 	
*       SetColor(WHITE);                         // set color to WHITE
*       SetLineType(SOLID_LINE);                 // set line to solid line
*       SetLineThickness(THICK_LINE);            // set line to thick line
*		DrawPoly(6, OpenShapeXYPoints);	         // draw the open shape  
*		DrawPoly(8, ClosedShapeXYPoints);	     // draw the closed shape  
*   </CODE>
*
* Side Effects: none
*
********************************************************************/
WORD    DrawPoly(SHORT numPoints, SHORT *polyPoints);

/*********************************************************************
* Function: WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* Overview: This function draws a bar given the left, top and right, 
*			bottom corners with the current set color (SetColor()). 
*           When alpha blending is enabled the bar is alpha blended 
*           with the existing pixels specified by the parameters.
*           The alpha percentage used is the last value set by SetAlpha().
*
* Input: left - x position of the left top corner.
*		 top - y position of the left top corner.
*		 right - x position of the right bottom corner.
*		 bottom - y position of the right bottom corner.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
********************************************************************/
WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom);

#ifdef USE_ALPHABLEND_LITE
/*********************************************************************
* Function: void BarAlpha(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: SetAlpha(alpha) must be called prior where 
*               0 <= alpha <= 100.
*               - alpha = 0       : rendering will not be performed
*               - 0 < alpha < 100 : rendering with alpha blending will be performed
*               - alpha = 100     : rendering with last call to SetColor(color) will replace current color
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
* Overview: This is an internal function used to render alpha blended Bars().
*           Draws alpha blended Bar over the area specified by left, top, right, bottom.
*           The pixels covered by the specified area will be alpha blended to the color
*           set by SetColor().
*
* Note: Application should not call this function. 
*
********************************************************************/
WORD    BarAlpha(SHORT left, SHORT top,WORD  right, WORD bottom);

/*********************************************************************
* Macro: SetAlpha(alpha)
*
* Overview: This macro sets the alpha value. Enabling this feature 
*           requires the macros USE_ALPHABLEND_LITE defined in the 
*           GraphicsConfig.h. See USE_ALPHABLEND_LITE for information
*           on supported primitive rendering functions.
*
* Input: alpha - Defines the alpha blending percentage of the new  
*                color set by SetColor() to the existing pixel color.
*                Valid values for alpha for pure primitive layer 
*                implementation are:  
*			    - 100 : no alpha blending, color set by last SetColor() call
*                       will replace the pixels.
*   			- 75  : alpha blending with new color set by last SetColor()
*                       call will be alpha blended with 75% to the existing
*                       pixel colors.
*			    - 50  : alpha blending with new color set by last SetColor()
*                       call will be alpha blended with 50% to the existing
*                       pixel colors.
*   			- 25  : alpha blending with new color set by last SetColor()
*                       call will be alpha blended with 25% to the existing
*                       pixel colors.
*
* Output: None
*
* Example:
*	        <CODE> 
*		    SetAlpha(50);       // set alpha level
*		    SetColor(BLUE);     // set color to use
*		    Bar(5,10,30,50);    // render an alpha blended Bar 
*           </CODE>
*
* Side Effects: none
*
********************************************************************/
#define SetAlpha(alpha) (_alpha = alpha)

/*********************************************************************
* Macro: GetAlpha(alpha)
*
* Overview: This macro returns the current alpha value. Enabling this  
*           feature requires the macros USE_ALPHABLEND_LITE defined in 
*           the GraphicsConfig.h.
*
* Input: none
*
* Output: Returns the current alpha value set by the last call to
*         SetAlpha(). 
*
* Side Effects: none
*
********************************************************************/
#define GetAlpha() (_alpha)

/*********************************************************************
* Macro: GetPrevAlphaColor()
*
* Overview: This is an internal macro. Returns the previously set color 
*           by the SetPrevAlphaColor() function. Useful when reverting 
*           back the overlaid Bar on a rectangular area.  Enabling this  
*           feature requires the macros USE_ALPHABLEND_LITE defined in 
*           the GraphicsConfig.h.    
*
* Input: none
*
* Output: Returns the previously used color set by SetPrevAlphaColor().
*
* Side Effects: none
*
********************************************************************/
#define GetPrevAlphaColor()      (_prevAlphaColor)

/*********************************************************************
* Macro: SetPrevAlphaColor(color)
*
* Overview: This is an internal macro. Sets the previous alpha 
*           color used. Enabling this  feature requires the macros 
*           USE_ALPHABLEND_LITE defined in the GraphicsConfig.h.
*
* Input: color - Defines the color used when alpha blending was performed.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetPrevAlphaColor(color) (_prevAlphaColor = (color))

#endif  // #ifdef USE_ALPHABLEND_LITE


/*********************************************************************
* Function: void ClearDevice(void)
*
* Overview: This function clears the screen with the current color 
*			and sets the graphic cursor position to (0, 0). 
*			Clipping is NOT supported by ClearDevice().
*
* Input: none
*
* Output: none
*
* Example:
*	<CODE> 
*	void ClearScreen(void)
*	{
*		SetColor(WHITE);		// set color to WHITE
*		ClearDevice();			// set screen to all WHITE
*	}
*   </CODE>
* 
* Side Effects: none
*
********************************************************************/
void    ClearDevice(void);

/*********************************************************************
* Function: WORD PutImagePartial(SHORT left, SHORT top, void* image, BYTE stretch, SHORT xoffset, SHORT yoffset, WORD width, WORD height)
*
* Overview: This function outputs a full or a partial image starting 
*           from left,top coordinates. The partial image starts at 
*           xoffset and yoffset. Size is specified by the given 
*           width and height parameters.
*
* Description: 
*        <img name="PutImagePartialExample.jpg" />
*
* Input: left - horizontal starting position of full or partial image on the screen
*        top - vertical starting position of full or partial image on the screen,
*        image - pointer to the image location.
*        stretch - The image stretch factor. 
*                  - IMAGE_NORMAL : no stretch 
*                  - IMAGE_X2 : image is stretched to twice its width and height
*        xoffset - Specifies the horizontal offset in pixels of the selected partial 
*                  image from the left most pixel of the full image.
*        yoffset - Specifies the vertical offset in pixels of the selected partial 
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
WORD     PutImagePartial(SHORT left, SHORT top, void *image, BYTE stretch, SHORT xoffset, SHORT yoffset, WORD width, WORD height);

/*********************************************************************
* Macro: WORD PutImage(SHORT left, SHORT top, void* image, BYTE stretch)
*
* Overview: This renders the image pointed to by "image" starting from 
*           left, top coordinates.
*
* Input: left - horizontal starting position of the full 
*               image on the screen
*        top - vertical starting position of the full image 
*              on the screen
*        image - pointer to the image location.
*        stretch - The image stretch factor. 
*                  - IMAGE_NORMAL : no stretch 
*                  - IMAGE_X2 : image is stretched to twice its 
*                               width and height
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the image is not 
*           yet completely drawn.
*         - Returns 1 when the image is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
* 
* Side Effects: none
*
********************************************************************/
#define PutImage(left, top, image, stretch) PutImagePartial(left, top, image, stretch, 0, 0, 0, 0)

/*********************************************************************
* Function: SHORT GetImageWidth(void* bitmap)
*
* Overview: This function returns the image width.
*
* Input: bitmap - Pointer to the bitmap.
*
* Output: Returns the image width in pixels.
* 
* Side Effects: none
*
********************************************************************/
SHORT   GetImageWidth(void *bitmap);

/*********************************************************************
* Function: SHORT GetImageHeight(void* bitmap)
*
* Overview: This function returns the image height.
*
* Input: bitmap - Pointer to the bitmap.
*
* Output: Returns the image height in pixels.
* 
* Side Effects: none
*
********************************************************************/
SHORT   GetImageHeight(void *bitmap);

/*********************************************************************
* Function: WORD ExternalMemoryCallback(GFX_EXTDATA* memory, LONG offset, WORD nCount, void* buffer)
*
* Overview: This function must be implemented in the application. 
*           The library will call this function each time when
*           the external memory data will be required. The application
*           must copy requested bytes quantity into the buffer provided.
*           Data start address in external memory is a sum of the address
*           in GFX_EXTDATA structure and offset.
*
* Input:  memory - Pointer to the external memory bitmap or font structures
*                  (FONT_EXTERNAL or BITMAP_EXTERNAL).
*         offset - Data offset.
*         nCount - Number of bytes to be transferred into the buffer.
*         buffer - Pointer to the buffer.
*
* Output: Returns the number of bytes were transferred.
* 
* Example:
*   <CODE>
*   // If there are several memories in the system they can be selected by IDs.
*   // In this example, ID for memory device used is assumed to be 0.
*   #define X_MEMORY 0
*
*   WORD ExternalMemoryCallback(GFX_EXTDATA* memory, LONG offset, WORD nCount, void* buffer) {
*   	int i;
*       long address;
*
*		// Address of the requested data is a start address of the object referred by GFX_EXTDATA structure plus offset
*		address = memory->address+offset;
*
*       if(memory->ID == X_MEMORY){
*       	// MemoryXReadByte() is some function implemented to access external memory. 
*           // Implementation will be specific to the memory used. In this example 
*			// it reads byte each time it is called. 
*           i = 0;
*           while (i < nCount) {
*           	(BYTE*)buffer = MemoryXReadByte(address++);
*               i++;
*           }
*      }
*      // return the actual number of bytes retrieved
*      return (i);
*  }
*  </CODE>
*
* Side Effects: none
*
********************************************************************/
WORD    ExternalMemoryCallback(GFX_EXTDATA *memory, LONG offset, WORD nCount, void *buffer);

/*********************************************************************
* Function:  SHORT GetSineCosine(SHORT v, WORD type)
*
* PreCondition: none
*
* Input: v - the angle used to retrieve the sine or cosine value. 
*			 The angle must be in the range of -360 to 360 degrees.
*		 type - sets if the angle calculation is for a sine or cosine
*				- GETSINE (0) - get the value of sine(v).
*				- GETCOSINE (1) - return the value of cosine(v).
*
* Output: Returns the sine or cosine of the angle given.
*
* Side Effects: none
*
* Overview: Using a lookup table, the sine or cosine values of the given angle
*           is returned.
*
* Note: none
*
********************************************************************/
SHORT GetSineCosine(SHORT v, WORD type);

/*********************************************************************
* Function:  SHORT Sine(SHORT v)
*
* PreCondition: none
*
* Input: v - the angle used to calculate the sine value. 
*			 The angle must be in the range of -360 to 360 degrees.
*
* Output: Returns the sine of the given angle.
*
* Side Effects: none
*
* Overview: This calculates the sine value of the given angle.
*
* Note: none
*
********************************************************************/
#define Sine(v)		GetSineCosine(v, GETSINE)

/*********************************************************************
* Function:  SHORT Cosine(SHORT v)
*
* PreCondition: none
*
* Input: v - the angle used to calculate the cosine value. 
*			 The angle must be in the range of -360 to 360 degrees.
*
* Output: Returns the cosine of the given angle.
*
* Side Effects: none
*
* Overview: This calculates the cosine value of the given angle.
*
* Note: none
*
********************************************************************/
#define Cosine(v)		GetSineCosine(v, GETCOSINE)


/*********************************************************************
* Function:  WORD DrawArc(SHORT cx, SHORT cy, SHORT r1, SHORT r2, SHORT startAngle, SHORT endAngle)
*
* Overview: This renders an arc with from startAngle to endAngle with the thickness 
*		    of r2-r1. The function returns 1 when the arc is rendered successfuly
* 			and returns a 0 when it is not yet finished. The next call to the 
*			function will continue the rendering.
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
* Note: none
*
********************************************************************/
WORD DrawArc(SHORT cx, SHORT cy, SHORT r1, SHORT r2, SHORT startAngle, SHORT endAngle);

void GetCirclePoint(SHORT radius, SHORT angle, SHORT *x, SHORT *y);

#ifdef USE_GRADIENT

/*********************************************************************
* Function:  WORD BarGradient(SHORT left, SHORT top, SHORT right, SHORT bottom, 
*                        GFX_COLOR color1, GFX_COLOR color2, DWORD length, 
*                        BYTE direction);
*
* Overview: This renders a bar onto the screen, but instead of one color, a gradient is drawn
* depending on the direction (GFX_GRADIENT_TYPE), length, and colors chosen. This function is
* a blocking call.
*
* Description: 
*        <img name="BarGradient.jpg" />
*
* PreCondition: USE_GRADIENT macro must be defined (in GraphicsConfig.h)
*
* Input: left - x position of the left top corner.
*		 top - y position of the left top corner.
*		 right - x position of the right bottom corner.
*		 bottom - y position of the right bottom corner.		 
*	     color1 - start color for the gradient
*        color2 - end color for the gradient
*        length - From 0-100%. How much of a gradient is wanted
*        direction - Gradient Direction	
*
* Output: Always returns a 1 since it is a blocking function.
*
* Example:
*   <CODE> 
*   // draw a full screen gradient background
*   // with color transitioning from BRIGHTRED to 
*   // BLACK in the upward direction.  
*
*   GFX_GRADIENT_STYLE  gradScheme;
*
*   gradScheme.gradientType         = GRAD_UP; 
*   gradScheme.gradientStartColor   = BRIGHTRED;
*   gradScheme.gradientEndColor     = BLACK;
* 
*    BarGradient(0,                                         //left position 
*                0,                                         //top position
*                GetMaxX(),                                 //right position
*                GetMaxY(),                                 //bottom position
*                gradScheme.gradientStartColor,
*                gradScheme.gradientEndColor,
*                50,                                        // at the halfway point (50%) of the rectangular area 
*                                                           // defined by the first 4 parameters (full screen), 
*                                                           // the color becomes BLACK and BLACK color is used until 
*                                                           // the rectangle defined is filled up 
*                gradScheme.gradientType);                  // see GFX_GRADIENT_TYPE
*	</CODE> 
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
WORD        BarGradient(SHORT left, SHORT top, SHORT right, SHORT bottom, GFX_COLOR color1, GFX_COLOR color2, DWORD length, BYTE direction);

/*********************************************************************
* Function:  BevelGradient(SHORT left, SHORT top, SHORT right, SHORT bottom, 
*                          SHORT rad, GFX_COLOR color1, GFX_COLOR color2, 
*                          DWORD length, BYTE direction);
*
* Overview: This renders a filled bevel with gradient color on the fill. It works 
* the same as the fillbevel function, except a gradient out of color1 and color2 
* is drawn depending on the direction (GFX_GRADIENT_TYPE). This function is a blocking call.
*
* Description: 
*        <img name="BevelGradient.jpg" />
*
* PreCondition: USE_GRADIENT macro must be defined (in GraphicsConfig.h)
*
* Input: left - x coordinate position of the upper left center of the circle that 
*			  draws the rounded corners.
*		 top - y coordinate position of the upper left center of the circle that 
*			  draws the rounded corners.
*		 right - x coordinate position of the lower right center of the circle that 
*			  draws the rounded corners.
*		 bottom - y coordinate position of the lower right center of the circle that 
*			  draws the rounded corners.
*        rad - defines the redius of the circle, that draws the rounded corners. When 
*              rad = 0, the object drawn is a rectangular gradient.
*	     color1 - start color for the gradient
*        color2 - end color for the gradient
*        length - From 0-100%. How much of a gradient is wanted
*        direction - see GFX_GRADIENT_TYPE	 
*
* Output: Always returns a 1 since it is a blocking function.
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
WORD        BevelGradient(SHORT left, SHORT top, SHORT right, SHORT bottom, SHORT rad, GFX_COLOR color1, GFX_COLOR color2, DWORD length, BYTE direction);

/*********************************************************************
* Overview: Enumeration for gradient type
*********************************************************************/
typedef enum
{
    GRAD_NONE=0,                            // No Gradients to be drawn
    GRAD_DOWN,                              // gradient changes in the vertical direction
    GRAD_RIGHT,                             // gradient change in the horizontal direction
    GRAD_UP,                                // gradient changes in the vertical direction
    GRAD_LEFT,                              // gradient change in the horizontal direction
    GRAD_DOUBLE_VER,                        // two gradient transitions in the vertical direction
    GRAD_DOUBLE_HOR,                        // two gradient transitions in the horizontal direction
} GFX_GRADIENT_TYPE;

/*********************************************************************
* Overview: This structure is used to describe the gradient style.
*
*********************************************************************/
typedef struct
{
    GFX_GRADIENT_TYPE  gradientType;        // selected the gradient type 
    DWORD              gradientStartColor;  // sets the starting color of gradient transition
    DWORD              gradientEndColor;    // sets the ending color of gradient transition
    DWORD              gradientLength;      // defines the length of the gradient transition in pixels
} GFX_GRADIENT_STYLE;

#endif

#ifdef USE_ALPHABLEND_LITE
extern GFX_COLOR prevAlphaColor;
#endif

#if defined USE_ALPHABLEND_LITE || defined USE_ALPHABLEND
extern BYTE _alpha;

extern SHORT _GFXForegroundPage;            // foreground page (or buffer) used in alpha blending
extern SHORT _GFXBackgroundPage;            // background page (or buffer)  used in alpha blending
extern SHORT _GFXDestinationPage;           // destination page (or buffer)  used in alpha blending
extern SHORT _GFXActivePage;                // Active page of application

//Macros to set/get alpha blending page information
#define SetForegroundPage(pageNumber) _GFXForegroundPage = pageNumber;
#define SetBackgroundPage(pageNumber) _GFXBackgroundPage = pageNumber;
#define SetDestinationPage(pageNumber) _GFXDestinationPage = pageNumber;
#define GetForegroundPage() 					_GFXForegroundPage
#define GetBackgroundPage() 					_GFXBackgroundPage
#define GetDestinationPage() 					_GFXDestinationPage

/*********************************************************************
* Function: void AlphaBlendWindow(DWORD foregroundArea, SHORT foregroundLeft, SHORT foregroundTop,
*                                 DWORD backgroundArea, SHORT backgroundLeft, SHORT backgroundTop,
*				                  DWORD destinationArea, SHORT destinationLeft, SHORT destinationTop,		            
*				                  WORD  width, WORD height,  	
*				                  BYTE  alphaPercentage)    
*
* Overview: This Alpha-Blends a foreground and a background stored in frames to a 
*           destination window. A frame is a memory area that contain array of pixels
*           information. An example would be a display buffer. This operation can be 
*           performed on a single frame (where foregroundArea, backgroundArea and destinationArea
*           all points to the same frame), 2 frames (where two of the three areas are pointing 
*           to the same frame and one is another frame), or 3 frames (where each area is a
*           separate frame). The Alpha-Blending is performed on the windows inside the specified 
*           frames. These windows are defined by the offsets for each frame and the given 
*           width and height. The Alpha-Blended windows are always equal in sizes. 
*           This function is only available when it is supported by the display 
*           driver used. Enabling this feature requires the macros USE_ALPHABLEND_LITE 
*           or USE_ALPHABLEND defined in the GraphicsConfig.h.
*
* PreCondition: none
*
* Input:    foregroundArea - Defines the starting address/page of the foreground window.
*	        foregroundLeft - Defines the foreground horizontal offset in pixels starting from the 
*                            starting address/page defined by foregroundArea. 
*           foregroundTop -  Defines the foreground vertical offset in pixels  starting from the 
*                            starting address/page defined by foregroundArea. 
*           backgroundArea - Defines the starting address/page of the background window.
*	        backgroundLeft - Defines the background horizontal offset in pixels starting from the 
*                            starting address/page defined by backgroundArea. 
*           backgroundTop -  Defines the background vertical offset in pixels  starting from the 
*                            starting address/page defined by backgroundArea.
*           destinationArea - Defines the starting address/page of the destination window.
*	        destinationLeft - Defines the destination horizontal offset in pixels starting from the 
*                            starting address/page defined by destinationArea. 
*           destinationTop - Defines the destination vertical offset in pixels  starting from the 
*                            starting address/page defined by destinationArea.
*	        width - Defines the width of the window to be alpha blended.
*           height - Defines the height of the window to be alpha blended.
*           alphaPercentage - This defines the amount of transparency to give the foreground Window. 
*                             Valid range is 0-100. Actual allowed values may be limited by 
*                             the driver used. Refer to the specific driver for allowed values.
*
* Output: none
*
* Side Effects: none
*
* Note: none
********************************************************************/
extern WORD AlphaBlendWindow(DWORD foregroundArea, SHORT foregroundLeft, SHORT foregroundTop,
                             DWORD backgroundArea, SHORT backgroundLeft, SHORT backgroundTop,
                             DWORD destinationArea, SHORT destinationLeft, SHORT destinationTop,		            
                             WORD  width, WORD height,  	
                             BYTE  alphaPercentage);

/*********************************************************************
* Function: DWORD GFXGetPageXYAddress(SHORT pageNumber, WORD x, WORD y)
* PreCondition: none
*
* Input:  pageNumber - the page number 
*         x - the x (horizontal) offset from 0,0 of the pagenumber
*         y - the y (vertical) offset from the 0,0 of the pagenumber
*
* Output: The address of an XY position of a certain page in memory
*
* Side Effects: none
*
* Overview: This function calculates the address of a certain x,y location in 
* memory
*
* Note: none
********************************************************************/
extern DWORD GFXGetPageXYAddress(SHORT pageNumber, WORD x, WORD y);

/*********************************************************************
* Function: DWORD GFXGetPageOriginAddress(SHORT pageNumber)
* PreCondition: none
*
* Input:  pageNumber - the page number 
*
* Output: The address of the start of a certain page in memory
*
* Side Effects: none
*
* Overview: This function calculates the address of a certain 0,0 location of a 
* page in memory
*
* Note: none
********************************************************************/
extern DWORD GFXGetPageOriginAddress(SHORT pageNumber);

#endif


#endif // _PRIMITIVE_H
