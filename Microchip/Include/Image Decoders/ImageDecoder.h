#ifndef __IMAGEDECODER_H__
#define __IMAGEDECODER_H__

/******************************************************************************
* FileName:        ImageDecoder.h
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

#include "GenericTypeDefs.h"
#include "stddef.h"

/************* User configuration start *************/
#include "ImageDecoderConfig.h"
/************* User configuration end *************/

/*********************************************************************
* Overview: Typedefs of the used File System APIs
*********************************************************************/
typedef size_t (*FileRead)(void *ptr, size_t size, size_t n, void *stream);
typedef int    (*FileSeek)(void *stream, long offset, int whence);
typedef long   (*FileTell)(void *fo);
typedef int    (*FileFeof)(void *stream);

/*********************************************************************
* Overview: IMG_FileSystemAPI holds function pointers to the used
*           File System APIs
*********************************************************************/
typedef struct _IMG_FILE_SYSTEM_API
{
     FileRead pFread;
     FileSeek pFseek;
     FileTell pFtell;
     FileFeof pFeof;
} IMG_FILE_SYSTEM_API;

/*********************************************************************
* Overview: IMG_ImageFileFormat specifies all the supported
*           image file formats
*********************************************************************/
typedef enum _IMG_FILE_FORMAT
{
     IMG_NONE = 0,
     IMG_BMP,                /* Bitmap image */
     IMG_JPEG,               /* JPEG image */
     IMG_GIF                 /* GIF image */
} IMG_FILE_FORMAT;

/*********************************************************************
* Overview: IMG_PixelRgb holds the RGB information of a pixel in BYTES
*********************************************************************/
typedef struct _IMG_PIXEL_XY_RGB_888
{
     WORD X;
     WORD Y;
     BYTE R;
     BYTE G;
     BYTE B;
} IMG_PIXEL_XY_RGB_888;

/*********************************************************************
* Overview: IMG_PixelOutput is a callback function which receives the
*           color information of the output pixel
*********************************************************************/
typedef void (*IMG_PIXEL_OUTPUT)(IMG_PIXEL_XY_RGB_888 *pPix);

/*********************************************************************
* Overview: IMG_LoopCallback is a callback function which is called
*           in every loop of the decoding cycle so that user
*           application can do some maintainance activities
*********************************************************************/
typedef void (*IMG_LOOP_CALLBACK)(void);

/* The global variables which define the image position and size */
#ifndef __IMAGEDECODER_C__
  extern BYTE IMG_blAbortImageDecoding;
  extern WORD IMG_wStartX;
  extern WORD IMG_wStartY;
  extern WORD IMG_wWidth;
  extern WORD IMG_wHeight;
  extern WORD IMG_wImageWidth;
  extern WORD IMG_wImageHeight;
  extern BYTE IMG_bDownScalingFactor;
  extern BYTE IMG_bAlignCenter;
 
 #ifndef IMG_USE_ONLY_565_GRAPHICS_DRIVER_FOR_OUTPUT
  extern IMG_PIXEL_OUTPUT IMG_pPixelOutput;
  extern IMG_PIXEL_XY_RGB_888 IMG_PixelXYColor;
 #endif

 #ifndef IMG_USE_ONLY_MDD_FILE_SYSTEM_FOR_INPUT
  extern IMG_FILE_SYSTEM_API *IMG_pFileAPIs;
 #endif
 
 #ifdef IMG_SUPPORT_IMAGE_DECODER_LOOP_CALLBACK
  extern IMG_LOOP_CALLBACK  IMG_pLoopCallbackFn;
 #endif
#endif

#ifdef IMG_USE_ONLY_MDD_FILE_SYSTEM_FOR_INPUT

 #include <MDD File System/FSIO.h>

 #define IMG_FILE         FSFILE
 #define IMG_FREAD        FSfread
 #define IMG_FSEEK        FSfseek
 #define IMG_FTELL        FSftell
 #define IMG_FEOF         FSfeof

// added by Paolo 9/10/08 for MultiApp Demo
 #define IMG_FOPEN        FSfopen
 #define IMG_FCLOSE       FSfclose

#else

 #define IMG_FILE         void
 #define IMG_FREAD        IMG_pFileAPIs->pFread
 #define IMG_FSEEK        IMG_pFileAPIs->pFseek
 #define IMG_FTELL        IMG_pFileAPIs->pFtell
 #define IMG_FEOF         IMG_pFileAPIs->pFeof

#endif

#ifndef __IMAGEDECODER_C__

  extern IMG_FILE *IMG_pCurrentFile;

#endif

/* The individual image decoders use these defines instead of directly using those provided in the Display driver file */
#define IMG_SCREEN_WIDTH         (GetMaxX()+1)
#define IMG_SCREEN_HEIGHT        (GetMaxY()+1)

#ifdef IMG_USE_ONLY_565_GRAPHICS_DRIVER_FOR_OUTPUT
       #include "Graphics/Graphics.h"
       #define IMG_vSetColor(r, g, b)   SetColor(RGB565CONVERT((r), (g), (b)))
       #define IMG_vSetColor565(color)  SetColor(color)
       #define _PutPixel(x, y)          PutPixel(x, y)
#else
       #define IMG_vSetColor(r, g, b)   IMG_PixelXYColor.R = r; IMG_PixelXYColor.G = g; IMG_PixelXYColor.B = b;
       #define IMG_vSetColor565(color)  IMG_PixelXYColor.R = ((color)>>11)&0x1F; IMG_PixelXYColor.G = ((color)>>5)&0x3F; IMG_PixelXYColor.B = (color)&0x1F;
       #define _PutPixel(x, y)          if(IMG_pPixelOutput != NULL) { IMG_PixelXYColor.X = x; IMG_PixelXYColor.Y = y; IMG_pPixelOutput(&IMG_PixelXYColor); }
       #ifndef RGB565CONVERT
            #define RGB565CONVERT(r, g, b) (((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))
       #endif
#endif

#define IMG_vPutPixel(x, y)      if(IMG_bDownScalingFactor <= 1)\
                                 {                          \
                                     if((x) < IMG_wImageWidth && (y) < IMG_wImageHeight) { _PutPixel(IMG_wStartX + (x), IMG_wStartY + (y)); }\
                                 }                          \
                                 else if((x)%IMG_bDownScalingFactor == 0 && (y)%IMG_bDownScalingFactor == 0)      \
                                 {                          \
                                     if((x) < IMG_wImageWidth && (y) < IMG_wImageHeight) { _PutPixel(IMG_wStartX + ((x)/IMG_bDownScalingFactor), IMG_wStartY + ((y)/IMG_bDownScalingFactor)); }\
                                 }        


#ifdef IMG_SUPPORT_IMAGE_DECODER_LOOP_CALLBACK
       #define IMG_vLoopCallback() if(IMG_pLoopCallbackFn != NULL) { IMG_pLoopCallbackFn(); }
#else
       #define IMG_vLoopCallback()
#endif

#define IMG_DECODE_ABORTED 0xFF
#define IMG_vCheckAndAbort()                \
        if(IMG_blAbortImageDecoding == 1)   \
        {                                   \
              IMG_blAbortImageDecoding = 0; \
              return IMG_DECODE_ABORTED;    \
        }


#ifdef IMG_SUPPORT_BMP
 #include "BmpDecoder.h"
#endif

#ifdef IMG_SUPPORT_JPEG
 #include "JpegDecoder.h"
#endif

#ifdef IMG_SUPPORT_GIF
 #include "GifDecoder.h"
#endif

/* Function prototypes */
/* User uses only these three functions */

/*********************************************************************
* Function: void ImageDecoderInit(void)
*
* Overview: This function initializes the global variables to 0
*           and then initializes the driver.
*           This must be called once before any other function of
*           the library is called
*
* Input: None
*
* Output: None
*
* Example:
*   <PRE> 
*	void main(void)
*	{
*		ImageInit();
*       ...
*	}
*	</PRE> 
*
* Side Effects: The graphics driver will be reset
*
********************************************************************/
void ImageDecoderInit(void);

/*********************************************************************
* Function: void ImageLoopCallbackRegister(IMG_LOOP_CALLBACK pLoopCallbackFn)
*
* Overview: This function registers the loop callback function so that
*           the decoder calls this function in every decoding loop.
*           This can be used by the application program to do
*           maintainance activities such as fetching data, updating
*           the display, etc...
*
* Input: pLoopCallbackFn -> Loop callback function pointer
*
* Output: None
*
* Example:
*   <PRE> 
*   void Mainantance(void)
*   {
*       ...
*   }
*
*	void main(void)
*	{
*		ImageInit();
*       ImageLoopCallbackRegister(Mainantance);       
*       ...
*	}
*	</PRE> 
*
* Side Effects: The graphics driver will be reset
*
********************************************************************/
void ImageLoopCallbackRegister(IMG_LOOP_CALLBACK pFn);

/*********************************************************************
* Function: BYTE ImageDecode(IMG_FILE *pImageFile, IMG_FILE_FORMAT eImgFormat, WORD wStartx, WORD wStarty, WORD wWidth, WORD wHeight, WORD wFlags, IMG_FILE_SYSTEM_API *pFileAPIs, IMG_PIXEL_OUTPUT pPixelOutput)
*
* Overview: This function decodes and displays the image on the screen
*
* Input: pImageFile -> The image file pointer
*        eImgFormat -> Type of image to be decoded
*        wStartx  -> Starting x position for the image to be displayed
*        wStarty  -> Starting y position for the image to be displayed
*        wWidth   -> The width into which the image to be displayed/compressed
*                    (This is not the width of the image)
*        wHeight  -> The height into which the image to be displayed/compressed
*                    (This is not the height of the image)
*        wFlags   -> If bit 0 is set, the image would be center aligned into the area
*                         specified by wStartx, wStarty, wWidth and wHeight
*                 -> If bit 1 is set, the image would be downscaled if required to fit
*                         into the area specified by wStartx, wStarty, wWidth
*                         and wHeight
*        pFileAPIs     -> The pointer to a structure which has function pointers
*                         to the File System APIs
*        pPixelOutput  -> The function to output (x, y) coordinates and the color
*
* Output: Error code -> 0 means no error
*
* Example:
*   <PRE> 
*	void main(void)
*	{
*		IMG_FILE pImageFile;
*       IMG_vInitialize();
*       pImageFile = IMG_FOPEN("Image.jpg", "r");
*       if(pImageFile == NULL)
*       {
*                 <- Error handling ->
*       }
*       IMG_bDecode(pImageFile, IMG_JPEG, 0, 0, 320, 240, 0, NULL, NULL);
*       IMG_FCLOSE(pImageFile);
*       while(1);
*	}
*	</PRE> 
*
* Side Effects: None
*
********************************************************************/
BYTE ImageDecode(IMG_FILE *pImageFile, IMG_FILE_FORMAT eImgFormat,
                 WORD wStartx, WORD wStarty, WORD wWidth, WORD wHeight,
                 WORD wFlags, IMG_FILE_SYSTEM_API *pFileAPIs,
                 IMG_PIXEL_OUTPUT pPixelOutput);

/* Flags */
#define IMG_ALIGN_CENTER 0x0001
#define IMG_DOWN_SCALE   0x0002


/*********************************************************************
* Function: BYTE ImageFullScreenDecode(IMG_FILE *pImageFile, IMG_FILE_FORMAT eImgFormat, IMG_FILE_SYSTEM_API pFileAPIs, IMG_PIXEL_OUTPUT pPixelOutput)
*
* Overview: This function decodes and displays the image on the screen in
*           fullscreen mode with center aligned and downscaled if required
*
* Input: pImageFile -> The image file pointer
*        eImgFormat -> Type of image to be decoded
*        pFileAPIs     -> The pointer to a structure which has function pointers
*                         to the File System APIs
*        pPixelOutput -> The function to output (x, y) coordinates and the color
*
* Output: Error code -> 0 means no error
*
* Example:
*   <PRE> 
*	void main(void)
*	{
*		IMG_FILE pImageFile;
*       IMG_vInitialize();
*       pImageFile = IMG_FOPEN("Image.jpg", "r");
*       if(pImageFile == NULL)
*       {
*                 <- Error handling ->
*       }
*       IMG_bFullScreenDecode(pImageFile, IMG_JPEG, NULL, NULL);
*       IMG_FCLOSE(pImageFile);
*       while(1);
*	}
*	</PRE> 
*
* Side Effects: None
*
********************************************************************/
#define ImageFullScreenDecode(pImageFile, eImgFormat, pFileAPIs, pPixelOutput) \
        ImageDecode(pImageFile, eImgFormat, 0, 0, IMG_SCREEN_WIDTH, IMG_SCREEN_HEIGHT, (IMG_ALIGN_CENTER | IMG_DOWN_SCALE), pFileAPIs, pPixelOutput)

/*********************************************************************
* Function: BYTE ImageDecodeTask(void)
*
* Overview: This function completes one small part of the image decode function
*
* Input: None
*
* Output: Status code - '1' means decoding is completed
*                     - '0' means decoding is not yet completed, call this function again
*
* Example:
*   <PRE>
*
*       IMG_bFullScreenDecode(pImageFile, IMG_JPEG, NULL, NULL);
*       while(!ImageDecodeTask());
*
*	</PRE> 
*
* Side Effects: None
*
********************************************************************/
BYTE ImageDecodeTask(void);

/*********************************************************************
* Function: void ImageAbort(void)
*
* Overview: This function sets the Image Decoder's Abort flag so that
*           decoding aborts in the next decoding loop.
*
* Input: None
*
* Output: None
*
* Example:
*   <PRE>
*
*   void callback(void);
*	void main(void)
*	{
*		IMG_FILE pImageFile;
*       IMG_vInitialize();
*       ImageLoopCallbackRegister(callback);
*       pImageFile = IMG_FOPEN("Image.jpg", "r");
*       if(pImageFile == NULL)
*       {
*                 <- Error handling ->
*       }
*       IMG_bFullScreenDecode(pImageFile, IMG_JPEG, NULL, NULL);
*       IMG_FCLOSE(pImageFile);
*       while(1);
*	}
*
*   void callback(void)
*   {
*       if(<- check for abort condition ->)
*       {
*           ImageAbort();
*       }
*   }
*
*	</PRE> 
*
* Side Effects: None
*
********************************************************************/
#define ImageAbort() IMG_blAbortImageDecoding = 1;

/********* This is not for the user *********/
/* This is used by the individual decoders */
void IMG_vSetboundaries(void);
/********* This is not for the user *********/
#endif
