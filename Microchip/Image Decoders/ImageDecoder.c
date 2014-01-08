#define __IMAGEDECODER_C__
/******************************************************************************

* FileName:        ImageDecoder.c
* Dependencies:    project requires File System library
* Processor:       PIC24/dsPIC30/dsPIC33/PIC32MX
* Compiler:        C30 v2.01/C32 v0.00.18
* Company:         Microchip Technology, Inc.

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

Author                 Date           Comments
--------------------------------------------------------------------------------
Pradeep Budagutta    03-Mar-2008    First release
*******************************************************************************/

/* This is used as a common header for all image decoders.
   This contains interfaces to sheild the image decoders
   from project specific information like reading from
   USB drive or from sd card, etc... */

#include "Image Decoders/ImageDecoder.h"

/**************************/
/**** GLOBAL VARIABLES ****/
/**************************/

IMG_FILE *IMG_pCurrentFile;

WORD IMG_wStartX;
WORD IMG_wStartY;
WORD IMG_wWidth;
WORD IMG_wHeight;
WORD IMG_wImageWidth;
WORD IMG_wImageHeight;
BYTE IMG_bDownScalingFactor;
BYTE IMG_bAlignCenter;
BYTE IMG_blAbortImageDecoding;

#ifndef IMG_USE_ONLY_565_GRAPHICS_DRIVER_FOR_OUTPUT
IMG_PIXEL_OUTPUT IMG_pPixelOutput;
IMG_PIXEL_XY_RGB_888 IMG_PixelXYColor;
#endif

#ifndef IMG_USE_ONLY_MDD_FILE_SYSTEM_FOR_INPUT
IMG_FILE_SYSTEM_API *IMG_pFileAPIs;
#endif

#ifdef IMG_SUPPORT_IMAGE_DECODER_LOOP_CALLBACK
IMG_LOOP_CALLBACK  IMG_pLoopCallbackFn;
#endif

/**************************/
/*******************************************************************************
Function:       void ImageDecoderInit(void)

Precondition:   None

Overview:       This function resets the variables and initializes the display

Input:          None

Output:         None
*******************************************************************************/
void ImageDecoderInit(void)
{
     IMG_wStartX = 0;
     IMG_wStartY = 0;
     IMG_wWidth  = 0;
     IMG_wHeight = 0;
     IMG_wImageWidth = 0;
     IMG_wImageHeight = 0;

   #ifndef IMG_USE_ONLY_565_GRAPHICS_DRIVER_FOR_OUTPUT
     IMG_pPixelOutput = NULL;
     IMG_PixelXYColor.X = 0;
     IMG_PixelXYColor.Y = 0;
     IMG_PixelXYColor.R = 0;
     IMG_PixelXYColor.G = 0;
     IMG_PixelXYColor.B = 0;
   #endif

   #ifndef IMG_USE_ONLY_MDD_FILE_SYSTEM_FOR_INPUT
     IMG_pFileAPIs = NULL;
   #endif

   #ifdef IMG_SUPPORT_IMAGE_DECODER_LOOP_CALLBACK
     IMG_pLoopCallbackFn = NULL;
   #endif
    IMG_blAbortImageDecoding = 0;

#ifdef IMG_USE_NON_BLOCKING_DECODING
    IMG_pCurrentFile = NULL;
#endif
}

/*******************************************************************************
Function:       void ImageLoopCallbackRegister(IMG_LOOP_CALLBACK pLoopCallbackFn)

Precondition:   None

Overview:       This function registers the loop callback function

Input:          Loop callback function pointer

Output:         None
*******************************************************************************/
void ImageLoopCallbackRegister(IMG_LOOP_CALLBACK pLoopCallbackFn)
{
   #ifdef IMG_SUPPORT_IMAGE_DECODER_LOOP_CALLBACK
     IMG_pLoopCallbackFn = pLoopCallbackFn;
   #endif
}

/*******************************************************************************
Function:       BYTE ImageDecode(IMG_FILE *pImageFile, IMG_FILE_FORMAT eImgFormat, WORD wStartx, WORD wStarty, WORD wWidth, WORD wHeight, WORD wFlags, IMG_FILE_SYSTEM_API *pFileAPIs, IMG_PIXEL_OUTPUT pPixelOutput)

Precondition:   None

Overview:       This function uses the approperiate image decoding function to
                decode and display the image

Input:          File pointer, Kind of image, Image position and boundaries, If center alignment and downscaling to fit into the display is required, File System API pointer and function to output the decoded pixels

Output:         Error code - '0' means no error
*******************************************************************************/
BYTE ImageDecode(IMG_FILE *pImageFile, IMG_FILE_FORMAT eImgFormat,
                 WORD wStartx, WORD wStarty, WORD wWidth, WORD wHeight,
                 WORD wFlags, IMG_FILE_SYSTEM_API *pFileAPIs,
                 IMG_PIXEL_OUTPUT pPixelOutput)
{
     BYTE bRetVal = 0;

     IMG_wStartX = wStartx;
     IMG_wStartY = wStarty;
     IMG_wWidth  = wWidth;
     IMG_wHeight = wHeight;
     IMG_wImageWidth = 0;
     IMG_wImageHeight = 0;

     IMG_bDownScalingFactor = (wFlags & IMG_DOWN_SCALE)? 1: 0;
     IMG_bAlignCenter = (wFlags & IMG_ALIGN_CENTER)? 1: 0;
     
   #ifndef IMG_USE_ONLY_565_GRAPHICS_DRIVER_FOR_OUTPUT
     IMG_pPixelOutput = pPixelOutput;
   #endif

   #ifndef IMG_USE_ONLY_MDD_FILE_SYSTEM_FOR_INPUT
     IMG_pFileAPIs = pFileAPIs;
   #endif

     switch(eImgFormat)
     {
      #ifdef IMG_SUPPORT_BMP
       case IMG_BMP: bRetVal = BMP_bDecode(pImageFile);
                     break;
      #endif
      #ifdef IMG_SUPPORT_JPEG
       case IMG_JPEG: bRetVal = JPEG_bDecode(pImageFile, TRUE);
                      IMG_pCurrentFile = pImageFile;
                     break;
      #endif
      #ifdef IMG_SUPPORT_GIF
       case IMG_GIF: bRetVal = GIF_bDecode(pImageFile);
                     break;
      #endif
       default:      bRetVal = 0xFF;
     }

     return(bRetVal);
}


/*******************************************************************************
Function:       BYTE ImageDecodeTask(void)

Precondition:   ImageDecode() must have been called

Overview:       This function completes one small part of the image decode function

Input:          None

Output:         Status code - '1' means decoding is completed
                            - '0' means decoding is not yet completed, call this function again
*******************************************************************************/
BYTE ImageDecodeTask(void)
{
	#ifdef IMG_USE_NON_BLOCKING_DECODING
	{
	    if(IMG_pCurrentFile != NULL) // At present, supports only JPEG
	    {
	        return JPEG_bDecode(IMG_pCurrentFile, FALSE);
	    }
	}
	#endif
	    
    return 1;
}


/*******************************************************************************
Function:       BYTE IMG_vSetboundaries(void)

Precondition:   None

Overview:       This function sets the boundaries and scaling factor. THIS IS
                NOT FOR THE USER.

Input:          None

Output:         None
*******************************************************************************/
void IMG_vSetboundaries(void)
{
     if(IMG_bDownScalingFactor > 0)
     {
         WORD wXScalingFactor = IMG_wImageWidth / IMG_wWidth;
         WORD wYScalingFactor = IMG_wImageHeight / IMG_wHeight;

         if(wXScalingFactor * IMG_wWidth < IMG_wImageWidth)
         {
             wXScalingFactor++;
         }
         if(wYScalingFactor * IMG_wHeight < IMG_wImageHeight)
         {
             wYScalingFactor++;
         }

         IMG_bDownScalingFactor = (BYTE)(wXScalingFactor > wYScalingFactor)? wXScalingFactor: wYScalingFactor;

         if(IMG_bDownScalingFactor <= 1)
         {
             IMG_bDownScalingFactor = 0;
         }
     }

     if(IMG_bAlignCenter > 0)
     {
         BYTE bDownScalingFactor = (IMG_bDownScalingFactor <= 1)? 1: IMG_bDownScalingFactor;
         if(IMG_wWidth > (IMG_wImageWidth / bDownScalingFactor))
         {
             IMG_wStartX += (IMG_wWidth - (IMG_wImageWidth / bDownScalingFactor)) / 2;
         }
         if(IMG_wHeight > (IMG_wImageHeight / bDownScalingFactor))
         {
             IMG_wStartY += (IMG_wHeight - (IMG_wImageHeight / bDownScalingFactor)) / 2;
         }
     }
}

#undef __IMAGEDECODER_C__
