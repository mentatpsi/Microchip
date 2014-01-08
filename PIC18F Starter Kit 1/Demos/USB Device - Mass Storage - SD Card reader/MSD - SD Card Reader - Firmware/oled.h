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
void oledPutStringInvertOption(unsigned char *ptr,unsigned char page, unsigned char col, BOOL invert);
void oledPutROMStringInvertOption(rom unsigned char *ptr,unsigned char page, unsigned char col, BOOL invert);
void oledPutImage(rom unsigned char *ptr, unsigned char sizex, unsigned char sizey, unsigned char startx, unsigned char starty);

/*********************************************/
extern ROM BYTE g_pucFont[][6];
extern ROM unsigned char folder_image[];
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

#define oledPutROMString(ptr,page,col) oledPutROMStringInvertOption(ptr,page,col,FALSE)
#define oledPutString(ptr,page,col) oledPutStringInvertOption(ptr,page,col,FALSE)


#endif // OLED_H
