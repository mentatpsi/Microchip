/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  UltraChip UC1610 controller driver
 *****************************************************************************
 * FileName:        UC1610.c
 * Dependencies:    Graphics.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2009 Microchip Technology Inc.  All rights reserved.
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
 * 02/25/09     ...
 * 04/12/11     Graphics Library Version 3.00 Support
 * 07/02/12     Modified PutImageXBPPYYY() functions to use new API.
 *****************************************************************************/
#include "HardwareProfile.h"

#if defined (GFX_USE_DISPLAY_CONTROLLER_UC1610) 

#include "Compiler.h"
#include "Graphics/DisplayDriver.h"
#include "Graphics/UC1610.h"
#include "Graphics/Primitive.h"

#if defined (USE_GFX_PMP)
    #include "Graphics/gfxpmp.h"
#elif defined (USE_GFX_EPMP)
    #include "Graphics/gfxepmp.h"
#endif    

// Unsupported Graphics Library Features
#ifdef USE_TRANSPARENT_COLOR
    #warning "This driver does not support the transparent feature on PutImage(). Build will use the PutImage() functions defined in the Primitive.c"
#endif    

//#define USE_PRIMITIVE_PUTIMAGE
#ifndef USE_PRIMITIVE_PUTIMAGE
    #warning "This driver does not support partial putImage feature. To enable partial putimage feature, uncomment the macro USE_PRIMITIVE_PUTIMAGE in this file. This will enable the PutImageXBPPYYY() in the Primitive.c implementation."
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

/////////////////////// LOCAL FUNCTIONS PROTOTYPES ////////////////////////////
#if !defined(USE_TRANSPARENT_COLOR) && !defined(USE_PRIMITIVE_PUTIMAGE)
void    PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void    PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void    PutImage1BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void    PutImage4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
#endif //#if !defined(USE_TRANSPARENT_COLOR) && !defined(USE_PRIMITIVE_PUTIMAGE)


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
    // set Vbias
    DeviceWrite(CMD_BIAS_RATIO_12);
    DeviceWrite(CMD_CONTRAST);
    DeviceWrite(82);

    // set panel loading
    DeviceWrite(CMD_PANEL_LOADING_38NF);

    // set connected COM electrodes
    DeviceWrite(CMD_DISPLAY_START);
    DeviceWrite(4 * (DISP_START_PAGE + 1) - 1);
    DeviceWrite(CMD_DISPLAY_END);
    DeviceWrite(4 * (DISP_END_PAGE + 1) - 1);
    DeviceWrite(CMD_COM_END);
    DeviceWrite(4 * (DISP_END_PAGE + 1) - 1);

    // set line rate
    DeviceWrite(CMD_LINE_RATE_12KLPS);

    DeviceWrite(CMD_MAPPING_MY);

    // inverse color
    DeviceWrite(CMD_INVERSE_ON);

    // set programm window
    DeviceWrite(CMD_WND_PRG_DISABLE);
    DeviceWrite(CMD_START_COLUMN);
    DeviceWrite(DISP_START_COLUMN);
    DeviceWrite(CMD_END_COLUMN);
    DeviceWrite(DISP_END_COLUMN);
    DeviceWrite(CMD_START_PAGE);
    DeviceWrite(DISP_START_PAGE);
    DeviceWrite(CMD_END_PAGE);
    DeviceWrite(DISP_END_PAGE);
    DeviceWrite(CMD_WND_PRG_ENABLE);

    // set autoincrement
    DeviceWrite(CMD_RAM_ADDR_INCR_ON);

    DeviceWrite(CMD_DISPLAY_ON);

	DisplaySetData();

    DisplayDisable();
}

/*********************************************************************
* Function: void ContrastSet(BYTE contrast)
*
* PreCondition: none
*
* Input: contrast value 
*
* Output: none
*
* Side Effects: none
*
* Overview: sets contrast
*
* Note: none
*
********************************************************************/
void ContrastSet(BYTE contrast)
{
    DisplayEnable();
	DisplaySetCommand();
    DeviceWrite(CMD_CONTRAST);
    DeviceWrite(contrast);
	DisplaySetData();
    DisplayDisable();
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
* Input: pixel position
*
* Output: none
*
* Side Effects: none
*
* Overview: puts pixel with current color at given position
*
* Note: none
*
********************************************************************/
void PutPixel(SHORT x, SHORT y)
{
    BYTE    value;

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

    x += DISP_START_COLUMN;

    DisplayEnable();

    // set address
	DisplaySetCommand();
    DeviceWrite(CMD_COLUMN_ADDR_LSB | (x & 0x0f));
    DeviceWrite(CMD_COLUMN_ADDR_MSB | ((x >> 4) & 0x0f));
    DeviceWrite(CMD_PAGE_ADDR | (DISP_START_PAGE + (y >> 2)));
	DisplaySetData();

    // read 4 pixels
    value = DeviceRead();
    value = DeviceRead();

    // set pixel
    switch(y & 0x03)
    {
        case 0: value &= 0b11111100; value |= _color; break;
        case 1: value &= 0b11110011; value |= _color << 2; break;
        case 2: value &= 0b11001111; value |= _color << 4; break;
        case 3: value &= 0b00111111; value |= _color << 6; break;
    }

    // set address
	DisplaySetCommand();
    DeviceWrite(CMD_COLUMN_ADDR_LSB | (x & 0x0f));
    DeviceWrite(CMD_COLUMN_ADDR_MSB | ((x >> 4) & 0x0f));
    DeviceWrite(CMD_PAGE_ADDR | (DISP_START_PAGE + (y >> 2)));
	DisplaySetData();

    // write 4 pixels back
    DeviceWrite(value);

    DisplayDisable();
}

/*********************************************************************
* Function: WORD GetPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: pixel position
*
* Output: pixel color
*
* Side Effects: none
*
* Overview: returns pixel at given position
*
* Note: none
*
********************************************************************/
GFX_COLOR GetPixel(SHORT x, SHORT y)
{
    BYTE    value;

    x += DISP_START_COLUMN;

    DisplayEnable();

    // set address
	DisplaySetCommand();
    DeviceWrite(CMD_COLUMN_ADDR_LSB | (x & 0x0f));
    DeviceWrite(CMD_COLUMN_ADDR_MSB | ((x >> 4) & 0x0f));
    DeviceWrite(CMD_PAGE_ADDR | (DISP_START_PAGE + (y >> 2)));
	DisplaySetData();

    // read 4 pixels
    value = DeviceRead();
    value = DeviceRead();

    DisplayDisable();

    // get pixel
    switch(y & 0x03)
    {
        case 0: break;
        case 1: value >>= 2; break;
        case 2: value >>= 4; break;
        case 3: value >>= 6; break;
    }

    value &= 0x03;
    return ((GFX_COLOR)value);
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
    WORD    counter;
    BYTE    pattern;

    pattern = _color;
    pattern |= pattern << 2;
    pattern |= pattern << 4;

    DisplayEnable();
	DisplaySetCommand();
    DeviceWrite(CMD_COLUMN_ADDR_LSB | (DISP_START_COLUMN & 0x0f));
    DeviceWrite(CMD_COLUMN_ADDR_MSB | ((DISP_START_COLUMN >> 4) & 0x0f));
    DeviceWrite(CMD_PAGE_ADDR | DISP_START_PAGE);
	DisplaySetData();

    for(counter = 0; counter < (DWORD) (GetMaxX() + 1) * (GetMaxY() + 1) / 4; counter++)
    {
        DeviceWrite(pattern);
    }

    DisplayDisable();
}

#if !defined(USE_TRANSPARENT_COLOR) && !defined(USE_PRIMITIVE_PUTIMAGE) 

#ifdef USE_BITMAP_FLASH

/*********************************************************************
* Function: void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - (currently not implemented in this driver)
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
void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    BYTE                temp;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                cx, cy;
    BYTE                stretchX, stretchY;
    BYTE                palette[2];
    BYTE                mask;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    palette[0] = (BYTE) * (flashAddress + 1);
    palette[0] >>= 3;
    palette[0] &= 0x03;
    flashAddress += 2;
    palette[1] = (BYTE) * (flashAddress + 1);
    palette[1] >>= 3;
    palette[1] &= 0x03;
    flashAddress += 2;

    cy = top;
    for(y = 0; y < sizeY; y++)
    {
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            flashAddress = tempFlashAddress;
            cx = left;
            mask = 0;
            for(x = 0; x < sizeX; x++)
            {

                // Read 8 pixels from flash
                if(mask == 0)
                {
                    temp = *flashAddress;
                    flashAddress++;
                    mask = 0x01;
                }

                // Set color
                if(mask & temp)
                {
                    SetColor(palette[1]);
                }
                else
                {
                    SetColor(palette[0]);
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    PutPixel(cx++, cy);
                }

                // Shift to the next pixel
                mask <<= 1;
            }

            cy++;
        }
    }
}

/*********************************************************************
* Function: void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - (currently not implemented in this driver)
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 16 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                cx, cy;
    BYTE                temp;
    register BYTE       stretchX, stretchY;
    BYTE                palette[16];
    WORD                counter;

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
        palette[counter] = (BYTE) * (flashAddress + 1);
        palette[counter] >>= 3;
        palette[counter] &= 0x03;
        flashAddress += 2;
    }

    cy = top;
    for(y = 0; y < sizeY; y++)
    {
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            flashAddress = tempFlashAddress;
            cx = left;
            for(x = 0; x < sizeX; x++)
            {

                // Read 2 pixels from flash
                if(x & 0x0001)
                {

                    // second pixel in byte
                    SetColor(palette[temp >> 4]);
                }
                else
                {
                    temp = *flashAddress;
                    flashAddress++;

                    // first pixel in byte
                    SetColor(palette[temp & 0x0f]);
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    PutPixel(cx++, cy);
                }
            }

            cy++;
        }
    }
}

#endif
#ifdef USE_BITMAP_EXTERNAL

/*********************************************************************
* Function: void PutImage1BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - (currently not implemented in this driver)
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
void PutImage1BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[2];
    BYTE            lineBuffer[((GetMaxX() + 1) / 8) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp;
    BYTE            mask;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            cx, cy;
    BYTE            stretchX, stretchY;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (2 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 2 * sizeof(WORD), palette);

    palette[0] >>= 3;
    palette[0] &= 0x03;
    palette[1] >>= 3;
    palette[1] &= 0x03;

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 2 * sizeof(WORD);

    // Line width in bytes
    byteWidth = bmp.width >> 3;
    if(bmp.width & 0x0007)
        byteWidth++;

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    cy = top;
    for(y = 0; y < sizeY; y++)
    {

        // Get line
        ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;
            cx = left;
            mask = 0;
            for(x = 0; x < sizeX; x++)
            {

                // Read 8 pixels from flash
                if(mask == 0)
                {
                    temp = *pData++;
                    mask = 0x01;
                }

                // Set color
                if(mask & temp)
                {
                    SetColor(palette[1]);
                }
                else
                {
                    SetColor(palette[0]);
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    PutPixel(cx++, cy);
                }

                // Shift to the next pixel
                mask <<= 1;
            }

            cy++;
        }
    }
}

/*********************************************************************
* Function: void PutImage4BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - (currently not implemented in this driver)
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
void PutImage4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            palette[16];
    BYTE            lineBuffer[((GetMaxX() + 1) / 2) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            cx, cy;
    BYTE            stretchX, stretchY;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (16 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 16 * sizeof(WORD), palette);

    for(temp = 0; temp < 16; temp++)
    {
        palette[temp] >>= 3;
        palette[temp] &= 0x03;
    }

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 16 * sizeof(WORD);

    // Line width in bytes
    byteWidth = bmp.width >> 1;
    if(bmp.width & 0x0001)
        byteWidth++;

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    cy = top;
    for(y = 0; y < sizeY; y++)
    {

        // Get line
        ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;
            cx = left;

            for(x = 0; x < sizeX; x++)
            {

                // Read 2 pixels from flash
                if(x & 0x0001)
                {

                    // second pixel in byte
                    SetColor(palette[temp >> 4]);
                }
                else
                {
                    temp = *pData++;

                    // first pixel in byte
                    SetColor(palette[temp & 0x0f]);
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    PutPixel(cx++, cy);
                }
            }

            cy++;
        }
    }
}

#endif

#endif // #if !defined(USE_TRANSPARENT_COLOR) && !defined(USE_PRIMITIVE_PUTIMAGE) 

#endif //#if defined (GFX_USE_DISPLAY_CONTROLLER_UC1610) 

