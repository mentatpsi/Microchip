/*****************************************************************************
 * FileName:        ExternalResource.h
 * Processor:       PIC24F, PIC24H, dsPIC
 * Compiler:        MPLAB C30/XC16 (see release notes for tested revision)
 * Linker:          MPLAB LINK30/XC16
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright(c) 2012 Microchip Technology Inc.  All rights reserved.
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
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * AUTO-GENERATED CODE:  Graphics Resource Converter version: 3.28.15
 *****************************************************************************/

#ifndef EXTERNALRESOURCE_H_FILE
#define EXTERNALRESOURCE_H_FILE
/*****************************************************************************
 * SECTION:  Includes
 *****************************************************************************/
#include <Graphics/Graphics.h>
#include "HardwareProfile.h"

/*****************************************************************************
 * SECTION:  Graphics Library Firmware Check
 *****************************************************************************/
#if(GRAPHICS_LIBRARY_VERSION != 0x0306)
#warning "It is suggested to use Graphics Library version 3.06 with this version of GRC."
#endif




/*****************************************************************************
 * This is a unique marker to verify the external memory data
 * The marker is a total of 8 bytes in length starting at the location GRC_CRC32_EXTERNAL_ADDR.
 * The first four bytes, starting at GRC_CRC32_EXTERNAL_ADDR, will always equal 'M', 'C', 'H', 'P'.
 * The next four byte is the generated 32-bit CRC.  The application can compare the value read from the
 * external memory to the value of GRC_CRC32_EXTERNAL_MARKER to verify that the data is valid.
 *****************************************************************************/
#define GRC_CRC32_EXTERNAL_MARKER 0x84FD3EE9ul
#define GRC_CRC32_EXTERNAL_ADDR 0x000103F0ul

/*****************************************************************************
 * SECTION:  JPEGS
 *****************************************************************************/

/*********************************
 * JPEG Structure
 * Label: Scene01
 * Description:  320x240 pixels
 ***********************************/
extern const IMAGE_EXTERNAL Scene01;
#define Scene01_WIDTH     (320)
#define Scene01_HEIGHT    (240)
#define Scene01_SIZE      (29068)
/*********************************
 * JPEG Structure
 * Label: Scene02
 * Description:  320x240 pixels
 ***********************************/
extern const IMAGE_EXTERNAL Scene02;
#define Scene02_WIDTH     (320)
#define Scene02_HEIGHT    (240)
#define Scene02_SIZE      (37463)
#endif

