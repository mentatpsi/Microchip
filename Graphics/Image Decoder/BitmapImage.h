/******************************************************************************
* This module allows displaying JPEG files from internal and external memories
*******************************************************************************
* FileName:        JPEGImage.h
* Dependencies:    Image decoding library, Graphics library
* Processor:       PIC24F, PIC24H, dsPIC, PIC32
* Compiler:        C30 v2.01/C32 v0.00.18
* Company:         Microchip Technology, Inc.

 * Software License Agreement
 *
 * Copyright � 2009 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
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
Anton Alkhimenok     05-May-2009    Modified to support JPEG files in external memory
*******************************************************************************/
#ifndef _BITMAPIMAGE_H
#define _BITMAPIMAGE_H

/************************************************************************
 * Section:  Includes
 *******************************************************************************/
    #include "Graphics/Graphics.h"
    #include "Image Decoders/ImageDecoder.h"
    #include "HardwareProfile.h"

// The module uses an external memory driver to get an JPEG image from the external memory.
// The header file for the driver must be provided HERE:
    #if defined (GFX_PICTAIL_V2)
        #include "SST39VF040.h"     // driver for SST39 parallel flash with JPEG files
    #elif defined (GFX_PICTAIL_V3) || defined (PIC24FJ256DA210_DEV_BOARD) || defined (MEB_BOARD)
        #include "SST25VF016.h"     // driver for SST25 SPI flash with JPEG files
    #else
        #error Hardware used does not have external memory
    #endif

// One function must be implemented in the driver:

/************************************************************************
* Function: void FunctionToReadArrayFromMemory(DWORD address, BYTE* pData, nCount)                                                                     
*
* Overview: this function reads data into buffer specified
*                                                                       
* Input: memory address, pointer to the data buffer, data count
*                                                                       
************************************************************************/

// Definition for the function implemented in the external memory driver must be HERE:
    #if defined (GFX_PICTAIL_V2)
        #if defined(__dsPIC33F__) || defined(__PIC24H__)
            #define SST39PMPInit() \
    {                              \
        while(PMMODEbits.BUSY);    \
        PMMODE = 0x0a49;           \
        PMAEN = 0x0003;            \
        PMCON = 0x9320;            \
    }

        #elif defined(__PIC32MX__)
            #define SST39PMPInit() \
    {                              \
        while(PMMODEbits.BUSY);    \
        PMMODE = 0x0a1a;           \
        PMAEN = 0x0003;            \
        PMCON = 0x9320;            \
    }

        #else
            #define SST39PMPInit() \
    {                              \
        while(PMMODEbits.BUSY);    \
        PMMODE = 0x0a09;           \
        PMAEN = 0x0003;            \
        PMCON = 0x9320;            \
    }

        #endif
        #if defined(__dsPIC33F__) || defined(__PIC24H__)
            #define LCDPMPInit() \
    {                            \
        while(PMMODEbits.BUSY);  \
        PMMODE = 0x0208;         \
        PMAEN = 0x0000;          \
        PMCON = 0x8300;          \
    }

        #else
            #define LCDPMPInit() \
    {                            \
        while(PMMODEbits.BUSY);  \
        PMMODE = 0x0204;         \
        PMAEN = 0x0000;          \
        PMCON = 0x8300;          \
    }

        #endif
        #define ReadArray(address, pData, nCount) \
    SST39PMPInit();                               \
    SST39ReadArray(address, pData, nCount);       \
    LCDPMPInit();
    #else // GFX_PICTAIL_V3 or PIC24FJ256DA210_DEV_BOARD 
        #define ReadArray(address, pData, nCount)   SST25ReadArray(address, pData, nCount);
    #endif
    
    
    #define USE_BITMAP_IMAGE_FLASH          // uncomment this line if bitmap images are in internal flash
    //***#define USE_BITMAP_IMAGE_EXTERNAL       // uncomment this line if bitmap images are in external memory


// Structure with data access functions pointers
extern IMG_FILE_SYSTEM_API  bitmapFileApi;

/*******************************************************************************
Function:       void* BitmapImage_fopen(void* bitmapImage)

Precondition:   None

Overview:       This function sets up the variables to emulate a file
                using constant data in internal flash or external memory

Input:          pointer to JPEG file image

Output:         pointer to JPEG file image
*******************************************************************************/
void                        *BitmapImage_fopen(void *bitmapImage);

/*******************************************************************************
Function:       void JPEGInit()

Precondition:   None

Overview:       This function initializes JPEG decoder

Input:          None

Output:         None
*******************************************************************************/
void                        BitmapImageInit(void);

/*******************************************************************************
Function:       void BitmapImage_PixelOutput (IMG_PIXEL_XY_RGB_888 * pPix)
*******************************************************************************/
void BitmapImage_PixelOutput (IMG_PIXEL_XY_RGB_888 * pPix);

#endif
