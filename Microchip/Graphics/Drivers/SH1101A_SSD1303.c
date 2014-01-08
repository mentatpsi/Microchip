/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Sino Wealth Microelectronic SH1101A OLED controller driver  
 *  Solomon Systech SSD1303 LCD controller driver
  *****************************************************************************
 * FileName:        SH1101A_SSD1303.c
 * Processor:       PIC24
 * Compiler:       	MPLAB C30
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 03/10/07	    Original
 * 12/20/07	    Ported to PIC24 Kit
 * 03/26/10		Fixed error on PutPixel() timing (PMP timing).
 * 03/22/11     - Modified for Graphics Library Version 3.00
 *              - Replace color data type from WORD_VAL to GFX_COLOR
 *              - Replace DeviceInit() to DriverInterfaceInit()
 * 12/13/11     Modify to use USE_GFX_DISPLAY_CONTROLLER_SSD1303 and 
 *              USE_GFX_DISPLAY_CONTROLLER_SH1101A labels to distinguish 
 *              between the two displays.
 *****************************************************************************/
#include "HardwareProfile.h"

#if defined(USE_GFX_DISPLAY_CONTROLLER_SH1101A) || defined (USE_GFX_DISPLAY_CONTROLLER_SSD1303)

#include "Compiler.h"
#include "TimeDelay.h"
#include "Graphics/DisplayDriver.h"
#include "Graphics/SH1101A_SSD1303.h"
#include "Graphics/gfxtcon.h"
#include "Graphics/Primitive.h"

#if defined (USE_GFX_PMP)
    #include "Graphics/gfxpmp.h"
#elif defined (USE_GFX_PMP)
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
	// Initialize the device
	DriverInterfaceInit();

    DisplayEnable();
	DisplaySetCommand();

#if defined(USE_GFX_DISPLAY_CONTROLLER_SH1101A) 

    // Setup Display
    DeviceWrite(0xAE);             // turn off the display (AF=ON, AE=OFF)
    DeviceWrite(0xDB);             // set  VCOMH
    DeviceWrite(0x23);
    DeviceWrite(0xD9);             // set  VP
    DeviceWrite(0x22);

    // Re-map
    DeviceWrite(0xA1);             // [A0]:column address 0 is map to SEG0

    // [A1]:column address 131 is map to SEG0
    // COM Output Scan Direction
    DeviceWrite(0xC8);             // C0 is COM0 to COMn, C8 is COMn to COM0

    // COM Pins Hardware Configuration
    DeviceWrite(0xDA);             // set pins hardware configuration
    DeviceWrite(0x12);

    // Multiplex Ratio
    DeviceWrite(0xA8);             // set multiplex ratio
    DeviceWrite(0x3F);             // set to 64 mux

    // Display Clock Divide
    DeviceWrite(0xD5);             // set display clock divide
    DeviceWrite(0xA0);             // set to 100Hz

    // Contrast Control Register
    DeviceWrite(0x81);             // Set contrast control
    DeviceWrite(0x60);             // display 0 ~ 127; 2C

    // Display Offset
    DeviceWrite(0xD3);             // set display offset
    DeviceWrite(0x00);             // no offset

    //Normal or Inverse Display
    DeviceWrite(0xA6);             // Normal display
    DeviceWrite(0xAD);             // Set DC-DC
    DeviceWrite(0x8B);             // 8B=ON, 8A=OFF

    // Display ON/OFF
    DeviceWrite(0xAF);             // AF=ON, AE=OFF
    DelayMs(150);

    // Entire Display ON/OFF
    DeviceWrite(0xA4);             // A4=ON

    // Display Start Line
    DeviceWrite(0x40);             // Set display start line

    // Lower Column Address
    DeviceWrite(0x00 + OFFSET);    // Set lower column address

    // Higher Column Address
    DeviceWrite(0x10);             // Set higher column address
    DelayMs(1);

#elif defined (USE_GFX_DISPLAY_CONTROLLER_SSD1303)

    // Setup Display
    DeviceWrite(0xAE);             // turn off the display (AF=ON, AE=OFF)
    DeviceWrite(0xDB);             // set  VCOMH
    DeviceWrite(0x23);
    DeviceWrite(0xD9);             // set  VP
    DeviceWrite(0x22);
    DeviceWrite(0xAD);             // Set DC-DC
    DeviceWrite(0x8B);             // 8B=ON, 8A=OFF
    DelayMs(1);

    // Display ON/OFF
    DeviceWrite(0xAF);             // AF=ON, AE=OFF

    // Init OLED display using SSD1303 driver
    // Display Clock Divide
    DeviceWrite(0xD5);             // set display clock divide
    DeviceWrite(0xA0);             // set to 100Hz

    // Display Offset
    DeviceWrite(0xD3);             // set display offset
    DeviceWrite(0x00);             // no offset

    // Multiplex Ratio
    DeviceWrite(0xA8);             // set multiplex ratio
    DeviceWrite(0x3F);             // set to 64 mux

    // Display Start Line
    DeviceWrite(0x40);             // Set display start line

    // Re-map
    DeviceWrite(0xA0);             // [A0]:column address 0 is map to SEG0

    // [A1]:column address 131 is map to SEG0
    // COM Output Scan Direction
    DeviceWrite(0xC8);             // C0 is COM0 to COMn, C8 is COMn to COM0

    // COM Pins Hardware Configuration
    DeviceWrite(0xDA);             // set pins hardware configuration
    DeviceWrite(0x12);

    // Contrast Control Register
    DeviceWrite(0x81);             // Set contrast control
    DeviceWrite(0x60);             // display 0 ~ 127; 2C

    // Entire Display ON/OFF
    DeviceWrite(0xA4);             // A4=ON

    //Normal or Inverse Display
    DeviceWrite(0xA6);             // Normal display

#ifdef DISABLE_DC_DC_CONVERTER
	DeviceWrite(0x8A);
#else
    DeviceWrite(0x8B);             // 8B=ON, 8A=OFF 
#endif

    // Lower Column Address
    DeviceWrite(0x00 + OFFSET);    // Set lower column address

    // Higher Column Address
    DeviceWrite(0x10);             // Set higher column address
    DelayMs(1);
#else
    #error The controller is not supported.
#endif

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

    add = x + OFFSET;
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
    display = SingleDeviceRead();	// Read to initiate Read transaction on PMP and dummy read
    								// (requirement for data synchronization in the controller)
    display = SingleDeviceRead();	// Read again as a requirement for data synchronization in the display controller
    display = SingleDeviceRead();	// Read actual data from from display buffer

    if(_color > 0)                  // If non-zero for pixel on
        display |= mask;            // or in mask
    else
        // If 0 for pixel off
        display &= ~mask;           // and with inverted mask
    SetAddress(page, lAddr, hAddr); // Set the address (sets the page,

    // lower and higher column address pointers)
    DeviceWrite(display);             // restore the byte with manipulated bit
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

    add = x + OFFSET;
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

    return (display & mask);        // mask all other bits and return the result	
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
    DisplayEnable();
    for(i = 0xB0; i < 0xB8; i++)
    {       // Go through all 8 pages
        SetAddress(i, 0x00, 0x10);
        for(j = 0; j < 132; j++)
        {   // Write to all 132 bytes
            DeviceWrite(_color);
        }
    }

    DisplayDisable();
}


#endif // #if defined(USE_GFX_DISPLAY_CONTROLLER_SH1101A) || defined (USE_GFX_DISPLAY_CONTROLLER_SSD1303)

