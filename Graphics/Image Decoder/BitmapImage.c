/******************************************************************************
* This module allows displaying Bitmap files from internal and external memories
*******************************************************************************
* FileName:        BitmapImage.c
* Dependencies:    Image decoding library
* Processor:       PIC24F, PIC24H, dsPIC, PIC32
* Compiler:        C30 v2.01/C32 v0.00.18
* Company:         Microchip Technology, Inc.

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
 *******************************************************************************/
#include "BitmapImage.h"

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/
DWORD               bitmapImageIndex;    // pointer to current location in image
IMG_FILE_SYSTEM_API bitmapFileApi;       // structure with data access functions pointers

/*******************************************************************************
    LOCAL FUNCTIONS PROTOTYPES
*******************************************************************************/
size_t              BitmapImage_fread(void *ptr, size_t size, size_t n, void *stream);  // read form image
int                 BitmapImage_fseek(void *stream, long offset, int whence);           // seek in image
int                 BitmapImage_feof(void *stream); // eof of image
long                BitmapImage_ftell(void *fo);

/*******************************************************************************
Function:       void BitmapImageInit()

Precondition:   None

Overview:       This function initializes bitmap image file pointers

Input:          None

Output:         None
*******************************************************************************/
void BitmapImageInit(void)
{
    bitmapFileApi.pFread = BitmapImage_fread;
    bitmapFileApi.pFseek = BitmapImage_fseek;
    bitmapFileApi.pFtell = BitmapImage_ftell;
    bitmapFileApi.pFeof = BitmapImage_feof;
}

/*******************************************************************************
Function:       size_t BitmapImage_fread(void *ptr, size_t size, size_t n, void* file)

Precondition:   None

Overview:       This function emulates file system's fread() on constant data
                in internal flash or external memory

Input:          Data buffer, size of data chunk, size of data type, file pointer

Output:         Number of bytes copied
*******************************************************************************/
size_t BitmapImage_fread(void *ptr, size_t size, size_t n, void *file)
{
    size_t      index;
    FLASH_BYTE  *pFile;
    #ifdef USE_BITMAP_IMAGE_EXTERNAL
    DWORD       address;
    #endif
    SHORT type;

    type = *((GFX_RESOURCE *)file);

    if((type & 0x0FF0) != IMAGE_JPEG)       // This is a place holder until there is an IMAGE_BINARY
        return 0;

    
    switch(type & GFX_MEM_MASK)
    {
    #ifdef USE_BITMAP_IMAGE_FLASH

        case FLASH:
            pFile = ((IMAGE_FLASH *)file)->address;

            BYTE    *bptr = (BYTE *)ptr;
            for(index = 0; index < size * n; index++)
            {
                bptr[index] = (BYTE) pFile[bitmapImageIndex];
                bitmapImageIndex++;
            }

            break;
    #endif
    #ifdef USE_BITMAP_IMAGE_EXTERNAL

        case EXTERNAL:
            address = bitmapImageIndex + ((IMAGE_EXTERNAL *)file)->address;
            ReadArray(address, (BYTE *)ptr, size * n);  // macro calling the ReadArray function from the external memory driver

            // see JPEGImage.h
            index = size * n;
            bitmapImageIndex += index;
            break;
    #endif

        default:
            return (0);
    }

    return (index);
}

/*******************************************************************************
Function:       int BitmapImage_fseek(void* stream, long offset, int whence)

Precondition:   None

Overview:       This function emulates file system's fseek() on constant data
                in internal flash or external memory. JPEG Decoder uses ONLY absolute and relative offsets.

Input:          File pointer, offset and the referance position

Output:         TRUE if success
*******************************************************************************/
int BitmapImage_fseek(void *stream, long offset, int whence)
{
    if(whence == 0)
    {
        bitmapImageIndex = offset;
    }
    else if(whence == 1)
    {
        bitmapImageIndex += offset;
    }
    else
    {
        return (FALSE); // JPEG Decoder uses ONLY absolute and relative offsets
    }

    return (TRUE);
}

/*******************************************************************************
Function:       long BitmapImage_ftell(void *fo)

Precondition:   None

Overview:       This function emulates file system's ftell() on constant data
                in Flash 

Input:          File pointer

Output:         Pointer position
*******************************************************************************/
long BitmapImage_ftell(void *fo)
{
    return (bitmapImageIndex);
}
/*******************************************************************************
Function:       int BitmapImage_feof(void *stream)

Precondition:   None

Overview:       This function emulates file system's feof() on constant data
                in Flash. JPEG Decoder doesn't use EOF.

Input:          File pointer

Output:         True if end of file is reached. JPEG Decoder doesn't use EOF.
                FALSE is returned.
*******************************************************************************/
int BitmapImage_feof(void *stream)
{
    return (FALSE); 
}

/*******************************************************************************
Function:       void* BitmapImage_fopen(void* jpegImage)

Precondition:   None

Overview:       This function sets up the variables to emulate a file
                using constant data in internal flash or external memory

Input:          pointer to JPEG file image

Output:         pointer to JPEG file image
*******************************************************************************/
void *BitmapImage_fopen(void *bitmapImage)
{
    bitmapImageIndex = 0;
    return (bitmapImage);
}
/*******************************************************************************
Function:       void BitmapImage_PixelOutput (IMG_PIXEL_XY_RGB_888 * pPix)
*******************************************************************************/
void BitmapImage_PixelOutput (IMG_PIXEL_XY_RGB_888 * pPix)
{
    SetColor(RGB565CONVERT(pPix->R, pPix->G, pPix->B));
    if(pPix->X <= GetMaxX() && pPix->Y <= GetMaxY())
    {
        PutPixel(pPix->X, pPix->Y);
    }

    if(pPix->X > GetMaxX() && pPix->Y > GetMaxY())
    {
        ImageAbort();
    }
}
