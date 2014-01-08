/******************************************************************************
* This module allows displaying JPEG files from internal and external memories
*******************************************************************************
* FileName:        JPEG.c
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

Author                 Date           Comments
--------------------------------------------------------------------------------
Pradeep Budagutta    25-Jun-2008    First release
Anton Alkhimenok     05-May-2009    support for JPEG files from external memory
*******************************************************************************/
#include "JPEGImage.h"

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/
DWORD               _jpegImageIndex;    // pointer to current location in image
IMG_FILE_SYSTEM_API _jpegFileApi;       // structure with data access functions pointers

/*******************************************************************************
    LOCAL FUNCTIONS PROTOTYPES
*******************************************************************************/
size_t              JPEGfread(void *ptr, size_t size, size_t n, void *stream);  // read form image
int                 JPEGfseek(void *stream, long offset, int whence);           // seek in image
int                 JPEGfeof(void *stream); // eof of image

/*******************************************************************************
Function:       void JPEGInit()

Precondition:   None

Overview:       This function initializes JPEG decoder

Input:          None

Output:         None
*******************************************************************************/
void JPEGInit(void)
{
    _jpegFileApi.pFread = JPEGfread;
    _jpegFileApi.pFseek = JPEGfseek;
    _jpegFileApi.pFtell = NULL; // not used by JPEG decoder;
    _jpegFileApi.pFeof = JPEGfeof;
}

/*******************************************************************************
Function:       size_t JPEGfread(void *ptr, size_t size, size_t n, void* file)

Precondition:   None

Overview:       This function emulates file system's fread() on constant data
                in internal flash or external memory

Input:          Data buffer, size of data chunk, size of data type, file pointer

Output:         Number of bytes copied
*******************************************************************************/
size_t JPEGfread(void *ptr, size_t size, size_t n, void *file)
{
    size_t      index;
    FLASH_BYTE  *pFile;
    #ifdef USE_JPEG_EXTERNAL
    DWORD       address;
    #endif
    SHORT type;

    type = *((GFX_RESOURCE *)file);

    if((type & 0x0FF0) != IMAGE_JPEG)
        return 0;

    
    switch(type & GFX_MEM_MASK)
    {
    #ifdef USE_JPEG_FLASH

        case FLASH:
            pFile = ((IMAGE_FLASH *)file)->address;

            BYTE    *bptr = (BYTE *)ptr;
            for(index = 0; index < size * n; index++)
            {
                bptr[index] = (BYTE) pFile[_jpegImageIndex];
                _jpegImageIndex++;
            }

            break;
    #endif
    #ifdef USE_JPEG_EXTERNAL

        case EXTERNAL:
            address = _jpegImageIndex + ((IMAGE_EXTERNAL *)file)->address;
            ReadArray(address, (BYTE *)ptr, size * n);  // macro calling the ReadArray function from the external memory driver

            // see JPEGImage.h
            index = size * n;
            _jpegImageIndex += index;
            break;
    #endif

        default:
            return (0);
    }

    return (index);
}

/*******************************************************************************
Function:       int JPEGfseek(void* stream, long offset, int whence)

Precondition:   None

Overview:       This function emulates file system's fseek() on constant data
                in internal flash or external memory. JPEG Decoder uses ONLY absolute and relative offsets.

Input:          File pointer, offset and the referance position

Output:         TRUE if success
*******************************************************************************/
int JPEGfseek(void *stream, long offset, int whence)
{
    if(whence == 0)
    {
        _jpegImageIndex = offset;
    }
    else if(whence == 1)
    {
        _jpegImageIndex += offset;
    }
    else
    {
        return (FALSE); // JPEG Decoder uses ONLY absolute and relative offsets
    }

    return (TRUE);
}

/*******************************************************************************
Function:       int JPEGfeof(void *stream)

Precondition:   None

Overview:       This function emulates file system's feof() on constant data
                in Flash. JPEG Decoder doesn't use EOF.

Input:          File pointer

Output:         True if end of file is reached. JPEG Decoder doesn't use EOF.
                FALSE is returned.
*******************************************************************************/
int JPEGfeof(void *stream)
{
    return (FALSE); // JPEG Decoder doesn't use EOF
}

/*******************************************************************************
Function:       void* JPEGfopen(void* jpegImage)

Precondition:   None

Overview:       This function sets up the variables to emulate a file
                using constant data in internal flash or external memory

Input:          pointer to JPEG file image

Output:         pointer to JPEG file image
*******************************************************************************/
void *JPEGfopen(void *jpegImage)
{
    _jpegImageIndex = 0;
    return (jpegImage);
}
/*******************************************************************************
Function:       void JPEG_PixelOutput (IMG_PIXEL_XY_RGB_888 * pPix)
*******************************************************************************/
void JPEG_PixelOutput (IMG_PIXEL_XY_RGB_888 * pPix)
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
