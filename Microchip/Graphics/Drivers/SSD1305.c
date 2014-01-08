/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Solomon Systech SSD1305 LCD controller driver
  *****************************************************************************
 * FileName:        SSD1305.c
 * Processor:       PIC24
 * Compiler:       	MPLAB C30
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 12/28/11	    Ported from SSD1303/SSH1101A driver
 *****************************************************************************/
#include "HardwareProfile.h"

#if defined(USE_GFX_DISPLAY_CONTROLLER_SSD1305)

#include "Compiler.h"
#include "TimeDelay.h"
#include "Graphics/DisplayDriver.h"
#include "Graphics/SSD1305.h"
#include "Graphics/gfxtcon.h"
#include "Graphics/Primitive.h"

#if defined (USE_GFX_PMP)
    #include "Graphics/gfxpmp.h"
#elif defined (USE_GFX_EPMP)
    #include "Graphics/gfxepmp.h"
#endif

// Clipping region control
SHORT       _clipRgn;

// Clipping region borders
SHORT       _clipLeft;
SHORT       _clipTop;
SHORT       _clipRight;
SHORT       _clipBottom;

// Color
GFX_COLOR   _color;
#ifdef USE_TRANSPARENT_COLOR
GFX_COLOR   _colorTransparent;
SHORT       _colorTransparentEnable;
#endif

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
#define SetAddress(page, lowerAddr, higherAddr) \
	DisplaySetCommand(); \
    DeviceWrite(page); \
    DeviceWrite(lowerAddr); \
    DeviceWrite(higherAddr); \
	DisplaySetData();


/*********************************************************************
* Function:  void ResetDevice()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: resets LCD, initializes PMP
*
* Note: none
*
********************************************************************/
void ResetDevice(void)
{
    DisplayVCCSignalConfig();
    DisplayVCCSignalDisable();

	// Initialize the device
	DriverInterfaceInit();

    DelayMs(500);
    DisplayVCCSignalEnable();


    DisplayEnable();
	DisplaySetCommand();

    /*Display off*/
    DeviceWrite(0xAE);
    /*Display Memory Address mode*/
    DeviceWrite(0x20);
    DeviceWrite(0x02);      // Page Addressing Mode

    /*Display Column Address*/
    DeviceWrite(0x21);
    DeviceWrite(0x00);
    DeviceWrite(0x83);      
    /*Display Page Address*/
    DeviceWrite(0x22);
    DeviceWrite(0x00);
    DeviceWrite(0x07);

    /*Display Start line*/
    DeviceWrite(0x40);

    /*Display Segment Re_map mode*/
#if (DISP_ORIENTATION == 0)
    DeviceWrite(0xA1);
#elif (DISP_ORIENTATION == 180)
    DeviceWrite(0xA0);
#endif
    /*Display Entire display ON*/
    DeviceWrite(0xA4);      // output follows RAM content

    /*Set Display Normal/Inverse mode*/
    DeviceWrite(0xA6);

    /*Display COM Output Scan Direction*/
#if (DISP_ORIENTATION == 0)
    DeviceWrite(0xC0);
#elif (DISP_ORIENTATION == 180)
    DeviceWrite(0xC8);
#endif
    /*Display Page start Address of page mode*/
    DeviceWrite(0xB0);

    /*Display Multiplex Ratio*/
    DeviceWrite(0xA8);
    DeviceWrite(0x1F);

    /*Display offset*/
    DeviceWrite(0xD3);
    DeviceWrite(0x00);      // no vertical offset

    /*Display Frame Frequence*/
    DeviceWrite(0xD5);
    DeviceWrite(0x51);       
    /*Display Area color mode ON/OFF*/
    DeviceWrite(0xD8);
    DeviceWrite(0x00);      // Monochrome Mode
    /*Display COM Configuration*/
    DeviceWrite(0xDA);
    DeviceWrite(0x12);
    /*Display Bank0 of Contrast*/
    DeviceWrite(0x81);
    DeviceWrite(0x3F);
    /*Display Color Banks of Contrast*/
    DeviceWrite(0x82);
    DeviceWrite(0x00);
    
    /*Display Pre_charge period*/
    DeviceWrite(0xD9);
    DeviceWrite(0x22);
    /*Display VComh Deselect Level*/
    DeviceWrite(0xDB);
    DeviceWrite(0x25);
    /*Display Internal DCDC Disable*/
    DeviceWrite(0xAD);
    DeviceWrite(0x8E);
    /*Display Delay 100ms*/
    DelayMs(200);
    /*Display ON*/
    DeviceWrite(0xAF);

    DelayMs(1);

    DisplayDisable();
	DisplaySetData();
}

#ifdef USE_TRANSPARENT_COLOR
/*********************************************************************
* Function:  void TransparentColorEnable(GFX_COLOR color)
*
* Overview: Sets current transparent color.
*
* PreCondition: none
*
* Input: color - Color value chosen.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void TransparentColorEnable(GFX_COLOR color)
{
    _colorTransparent = color;    
    _colorTransparentEnable = TRANSPARENT_COLOR_ENABLE;

}
#endif

/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates
*
* Output: none
*
* Side Effects: none
*
* Overview: puts pixel
*
* Note: none
*
********************************************************************/
void PutPixel(SHORT x, SHORT y)
{
    BYTE    page, add, lAddr, hAddr;
    BYTE    mask, display;

    // check if point is in clipping region
    if(_clipRgn)
    {
        if(x < _clipLeft)
            return;
        if(x > _clipRight)
            return;
        if(y < _clipTop)
            return;
        if(y > _clipBottom)
            return;
    }

    // Assign a page address
    if(y < 8)
        page = 0xB0;
    else if(y < 16)
        page = 0xB1;
    else if(y < 24)
        page = 0xB2;
    else if(y < 32)
        page = 0xB3;
    else if(y < 40)
        page = 0xB4;
    else if(y < 48)
        page = 0xB5;
    else if(y < 56)
        page = 0xB6;
    else
        page = 0xB7;

    add = x + GRAM_COLUMN_OFFSET; 
    lAddr = 0x0F & add;             // Low address
    hAddr = 0x10 | (add >> 4);      // High address

    // Calculate mask from rows basically do a y%8 and remainder is bit position
    add = y >> 3;                   // Divide by 8
    add <<= 3;                      // Multiply by 8
    add = y - add;                  // Calculate bit position
    mask = 1 << add;                // Left shift 1 by bit position
    DisplayEnable();

    SetAddress(page, lAddr, hAddr); // Set the address (sets the page,
                                    // lower and higher column address pointers)
	DisplaySetCommand();
    DeviceWrite(0xE0);              // enter read-modify-write so we do not need to re-issue address
  	DisplaySetData();

    display = SingleDeviceRead();	// Read to initiate Read transaction on PMP and dummy read
    								// (requirement for data synchronization in the controller)
    display = SingleDeviceRead();	// Read again as a requirement for data synchronization in the display controller
    display = SingleDeviceRead();	// Read actual data from from display buffer

    if(_color > 0)                  // If non-zero for pixel on
        display |= mask;            // or in mask
    else
        // If 0 for pixel off
        display &= ~mask;           // and with inverted mask

    // lower and higher column address pointers)
    DeviceWrite(display);           // restore the byte with manipulated bit

	DisplaySetCommand();
    DeviceWrite(0xE0);              // exit read modify write, SSD1305 address pointer is set 
                                    // back to the last address set
  	DisplaySetData();

    DisplayDisable();
}

/*********************************************************************
* Function: BYTE GetPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates 
*
* Output: pixel color
*
* Side Effects: none
*
* Overview: return pixel color at x,y position
*
* Note: none
*
********************************************************************/
BYTE GetPixel(SHORT x, SHORT y)
{
    BYTE    page, add, lAddr, hAddr;
    BYTE    mask, temp, display;

    // check if point is in clipping region
    if(_clipRgn)
    {
        if(x < _clipLeft)
            return (0);
        if(x > _clipRight)
            return (0);
        if(y < _clipTop)
            return (0);
        if(y > _clipBottom)
            return (0);
    }

    // Assign a page address
    if(y < 8)
        page = 0xB0;
    else if(y < 16)
        page = 0xB1;
    else if(y < 24)
        page = 0xB2;
    else if(y < 32)
        page = 0xB3;
    else if(y < 40)
        page = 0xB4;
    else if(y < 48)
        page = 0xB5;
    else if(y < 56)
        page = 0xB6;
    else
        page = 0xB7;

    add = x + GRAM_COLUMN_OFFSET;
    lAddr = 0x0F & add;             // Low address
    hAddr = 0x10 | (add >> 4);      // High address

    // Calculate mask from rows basically do a y%8 and remainder is bit position
    temp = y >> 3;                  // Divide by 8
    temp <<= 3;                     // Multiply by 8
    temp = y - temp;                // Calculate bit position
    mask = 1 << temp;               // Left shift 1 by bit position
    DisplayEnable();

    SetAddress(page, lAddr, hAddr); // Set the address (sets the page,
                                    // lower and higher column address pointers)
    display = SingleDeviceRead();	// Read to initiate Read transaction on PMP
                                    // Dummy Read (requirement for data synchronization in the controller)
    display = DeviceRead();         // Read data from display buffer
    DisplayDisable();
    if ((display & mask) == 0)      // mask all other bits check if the value is 0 or greater than 1
        return 0;                   // if == 0 then the pixel value is 0
    else
        return 1;                   // if > 1 then the pixel value is 1
}

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
WORD IsDeviceBusy(void)
{  
    return (0);
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
* Overview: clears screen with current color 
*
* Note: none
*
********************************************************************/
void ClearDevice(void)
{
    BYTE    i, j;
    
    // since this driver is a 1BPP driver and interface is byte wide.
    BYTE    colorWord;
    
    if (_color)
        colorWord = 0xFF;
    else
        colorWord = 0x00;

    DisplayEnable();
    for(i = 0xB0; i < 0xB8; i++)
    {   // Go through all 8 pages since we do not know 
        // where the display buffer is mapped, 
        SetAddress(i, 0x00, 0x10);
        for(j = 0; j < 132; j++)
        {   // Write to all pixels 
            DeviceWrite(colorWord);
        }
    }

    DisplayDisable();
}

/*********************************************************************
* Function: void GFX_Horizontal_Scroll(
*                   SSD1305_SCROLL_DIR dir, 
*                   SSD1305_SCROLL_OFFSET offset, 
*                   SSD1305_SCROLL_PAGE pageStart, 
*                   SSD1305_SCROLL_PAGE pageEnd, 
*                   SSD1305_SCROLL_INTERVAL frameInterval)
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
void SSD1305_Horizontal_Scroll(SSD1305_SCROLL_DIR dir, SSD1305_SCROLL_OFFSET offset, 
                    SSD1305_SCROLL_PAGE pageStart, SSD1305_SCROLL_PAGE pageEnd, 
                    SSD1305_SCROLL_INTERVAL frameInterval)
{
    DisplayEnable();
	DisplaySetCommand();

    DeviceWrite(0x26 | dir);
    DeviceWrite(offset);
    DeviceWrite(pageStart);
    DeviceWrite(frameInterval);
    DeviceWrite(pageEnd);
    DeviceWrite(0x2F);          // start scrolling

    DisplayDisable();
    
}

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
                                SSD1305_SCROLL_INTERVAL frameInterval)
{
    DisplayEnable();
	DisplaySetCommand();

    DeviceWrite(0xA3);
    DeviceWrite(fixedRowsStart);
    DeviceWrite(noScrollRows);
    
    DeviceWrite(0x29);          // set vertical and horizontal scrolling
    DeviceWrite(0x00);          // no horizontal scrolling
    DeviceWrite(0x00);          // dummy for page start
    DeviceWrite(frameInterval);
    DeviceWrite(0x00);          // dymmy for page end

    DeviceWrite(offset);        

    DeviceWrite(0x2F);          // start scrolling

    DisplayDisable();
    
}

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
void SSD1305_Scroll_Stop()
{
    DisplayEnable();
	DisplaySetCommand();

    DeviceWrite(0x2E);          // stop scrolling

    DisplayDisable();

}


#endif // #if defined(USE_GFX_DISPLAY_CONTROLLER_SSD1305)

