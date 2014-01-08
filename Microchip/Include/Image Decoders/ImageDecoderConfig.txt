#ifndef __IMAGEDECODERCONFIG_H__
#define __IMAGEDECODERCONFIG_H__

/******************************************************************************
* FileName:        ImageDecoderConfig.h
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
Pradeep Budagutta    06-Jun-2008    First release
*******************************************************************************/

/* This is used to configure the image decoder */

/************* User configuration start *************/

/* Comment out the image formats which are not required */
#define IMG_SUPPORT_BMP
#define IMG_SUPPORT_JPEG
#define IMG_SUPPORT_GIF

/* Comment out if output has to be given through a callback function */

/* If defined, the code is optimized to use only the graphics driver, only 16-bit-565-color-format is supported */
#define IMG_USE_ONLY_565_GRAPHICS_DRIVER_FOR_OUTPUT

/* If the above define is commented out (Graphics driver is not included by default), then the below defines has to be set by the user */
#ifndef IMG_USE_ONLY_565_GRAPHICS_DRIVER_FOR_OUTPUT
        #define DISP_HOR_RESOLUTION       320
        #define DISP_VER_RESOLUTION       240
#endif

/* If defined, the code is optimized to use only the MDD file system */
#define IMG_USE_ONLY_MDD_FILE_SYSTEM_FOR_INPUT

/* If defined, the a loop callback function is called in every decoding loop so that application can do maintainance activities such as getting data, updating display, etc... */
#define IMG_SUPPORT_IMAGE_DECODER_LOOP_CALLBACK

/************* User configuration end *************/

#endif
