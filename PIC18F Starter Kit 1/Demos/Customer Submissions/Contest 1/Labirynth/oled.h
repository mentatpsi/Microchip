/********************************************************************
  File Information:
    FileName:     	oled.h
    Dependencies:   See INCLUDES section
    Processor:      PIC18F46J50
    Hardware:       PIC18F Starter Kit
    Complier:  	    Microchip C18 (for PIC18)
    Company:        Microchip Technology, Inc.
    
    Software License Agreement:
    
    The software supplied herewith by Microchip Technology Incorporated
    (the “Company”) for its PIC® Microcontroller is intended and
    supplied to you, the Company’s customer, for use solely and
    exclusively on Microchip PIC Microcontroller products. The
    software is owned by the Company and/or its supplier, and is
    protected under applicable copyright laws. All rights are reserved.
    Any use in violation of the foregoing restrictions may subject the
    user to criminal sanctions under applicable laws, as well as to
    civil liability for the breach of the terms and conditions of this
    license.
    
    THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
    TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
    IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

  File Description:
    
    Change History:
     Rev   Date         Description
     1.0                Initial release

********************************************************************/
#ifndef OLED_H
#define OLED_H

/******** Include files **********************/
#include "Compiler.h"
#include "GenericTypeDefs.h"
/*********************************************/

void WriteCommand(BYTE cmd);
void WriteData(BYTE data);
void FillDisplay(unsigned char data);
void oledPutString(unsigned char *ptr,unsigned char page, unsigned char col);
void oledPutNegString(unsigned char *ptr,unsigned char page, unsigned char col);
void oledPutROMString(rom unsigned char *ptr,unsigned char page, unsigned char col);
void oledPutROMNegString(rom unsigned char *ptr,unsigned char page, unsigned char col);
void oledPutImage(rom unsigned char *ptr, unsigned char sizex, unsigned char sizey, unsigned char startx, unsigned char starty);

/*********************************************/
extern ROM BYTE g_pucFont[95][5];
/*********************************************/

// Define this to implement ClearDevice function in the driver.
#define USE_DRV_CLEARDEVICE

/*********************************************************************
* Overview: Horizontal and vertical screen size.
*********************************************************************/
#ifdef  USE_PORTRAIT
#error THE DRIVER DOES SUPPORT PORTRAIT MODE
#else

// Defines the display offset in x direction. Dependent on the display 
// used and how it is connected.
#define OFFSET				2

// Defines the horizontal screen size. Dependent on the display glass used.
#define SCREEN_HOR_SIZE    128

// Defines the vertical screen size. Dependent on the display glass used.
#define SCREEN_VER_SIZE    64
#endif
/*********************************************************************
* Overview: Clipping region control codes to be used with SetClip(...)
*           function. 
*********************************************************************/
#define CLIP_DISABLE       0 	// Disables clipping.
#define CLIP_ENABLE        1	// Enables clipping.

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
#define SSON				1	// screen saver is turned on
#define	SSOFF				0	// screen saver is turned off

/*********************************************************************
* Overview: Color definitions.
*********************************************************************/

#define BLACK              (WORD)0b00000000
#define WHITE              (WORD)0b11111111

/*********************************************************************
* Overview: Emboss size set for 3-D effect
*********************************************************************/

#define GOL_EMBOSS_SIZE                 1		



// Memory pitch for line
#define LINE_MEM_PITCH		0x100 

// Definitions for reset pin
#define RST_TRIS_BIT       TRISDbits.TRISD2
#define RST_LAT_BIT        LATDbits.LATD2

// Color
extern BYTE _color;

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

/*********************************************************************
* Macros:  PMPDelay()
*
* Overview: Delays execution for PMP cycle time. This is dependent on 
*			processor clock. 
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
//#if (GetSystemClock() == 32000000)
// at 32 MHZ 1 nop is at 32.25 ns. Controller needs 300 ns minimum delay from
// last CS valid to next CS valid. Thus we have the 7 nops (227.5 ns).
// PMP access at this clock frequency is 3 cycles (97.5 ns).
// 		therefore: 227.5 + 97.5 > 300 
//   			   verify: 	300 - 97.5 = 202.5
//				   			202.5/32.5 = 6.23 thus we can use 7 nops.	 				
#define PMPDelay()  Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop();  

//#else
//
//#define PMPDelay()  Nop(); 
//
//#endif

/*********************************************************************
* Macros:  PMPWaitBusy()
*
* Overview: Delays execution for PMP cycle time.
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
//#ifdef __PIC24F__
#define PMPWaitBusy()  Nop(); 
//#else
//#error CONTROLLER IS NOT SUPPORTED
//#endif

/*********************************************************************
* Macros:  WriteData(writeByte)
*
* Overview: Writes data.
*
* PreCondition: none
*
* Input: writeByte - Data byte to be written.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
//#define WriteData(writeByte) 	PMADDR=0x4001;PMPWaitBusy();PMDIN1=writeByte;PMPDelay();

/*********************************************************************
* Macros:  ReadData(readByte)
*
* Overview: Reads a byte from the display buffer. This assumes that the 
*			page value, lower and higher column address pointers are already set.
*
* PreCondition: Page, lower and higher column address pointers are already set.
*
* Input: none.
*
* Output: ReadByte - data read from the display buffer.
*
* Side Effects: none
*
********************************************************************/
#define ReadData(readByte)		    TRISD = 0xFF;oledD_C = 1;Nop();oledRD = 0;Nop();oledRD = 1;Nop();oledRD = 0;Nop();readByte = PORTD;

/*********************************************************************
* Macros:  SetAddress(lowerAddr,higherAddr)
*
* Overview: Sets the page and the lower and higher address pointer
*			of the display buffer. All parameters should be pre-calculated.
*
* PreCondition: none
*
* Input: page - Page value for the address.
*		 lowerAddr - Lower column address.
*		 higherAddr - Higher column address.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetAddress(page,lowerAddr,higherAddr)\
	WriteCommand(page);\
	WriteCommand(lowerAddr);\
	WriteCommand(higherAddr);

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
* Side Effects: none
*
********************************************************************/
#define GetMaxX() (SCREEN_HOR_SIZE-1)

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
* Side Effects: none
*
********************************************************************/
#define GetMaxY() (SCREEN_VER_SIZE-1)

/*********************************************************************
* Macros:  SetColor(color)
*
* Overview: Sets current drawing color.
*
* PreCondition: none
*
* Input: color - Color coded in 5:6:5 RGB format.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetColor(color) _color = color;

/*********************************************************************
* Macros:  GetColor()
*
* Overview: Returns current drawing color.
*
* PreCondition: none
*
* Input: none
*
* Output: Color coded in 5:6:5 RGB format.
*
* Side Effects: none
*
********************************************************************/
#define GetColor() _color

/*********************************************************************
* Macros:  SetActivePage(page)
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
#define SetActivePage(page)

/*********************************************************************
* Macros: SetVisualPage(page)
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
#define SetVisualPage(page)

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
* Function: WORD GetPixel(SHORT x, SHORT y)
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
BYTE GetPixel(SHORT x, SHORT y);

/*********************************************************************
* Macros: SetClipRgn(left, top, right, bottom)
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
#define SetClipRgn(left,top,right,bottom) _clipLeft=left; _clipTop=top; _clipRight=right; _clipBottom=bottom;

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
* Macros: SetClip(control)
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
#define SetClip(control) _clipRgn=control;

/*********************************************************************
* Macros: IsDeviceBusy()
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
#define IsDeviceBusy()  0

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

/*********************************************************************
* Function:  void  DelayMs(WORD time)
*
* Overview: Delays execution on time specified in milliseconds.
*           The delay is correct only for 16MIPS.
*
* PreCondition: none
*
* Input: time - Delay in milliseconds.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void DelayMs(WORD time);

#endif // OLED_H
