/*****************************************************************************
 * FileName:        InternalResourceXC16.c
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
 * Converted Resources
 * -------------------
 *
 *
 * Palettes
 * --------
 * RemoteControlPalette - 256 Colors
 *
 * Fonts
 * -----
 * DroidSansMono_11 - Height: 15 pixels, 1 bit per pixel, Range: ' ' to '~'
 * DroidSansMono_22 - Height: 30 pixels, 1 bit per pixel, Range: ' ' to '~'
 * DroidSans_18 - Height: 22 pixels, 1 bit per pixel, Range: ' ' to '~'
 * DroidSans_Bold_14 - Height: 17 pixels, 1 bit per pixel, Range: ' ' to '~'
 * DroidSansMono_36 - Height: 48 pixels, 1 bit per pixel, Range: '+' to '9'
 *****************************************************************************/

/*****************************************************************************
 * SECTION:  Fonts
 *****************************************************************************/

/*********************************
 * TTF Font File Structure
 * Label: DroidSansMono_11
 * Description:  Height: 15 pixels, 1 bit per pixel, Range: ' ' to '~'
 ***********************************/
#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
extern const char __DroidSansMono_11[] __attribute__((space(prog), aligned(2)));
#else
extern const char __DroidSansMono_11[] __attribute__((aligned(2)));
#endif

const FONT_FLASH DroidSansMono_11 =
{
    (FLASH | COMP_NONE),
    (GFX_FONT_SPACE char *)__DroidSansMono_11
};

#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
asm(".section .text, code");
#else
asm(".section .const, psv, page");
#endif
asm(".global ___DroidSansMono_11");
asm(".align 2");
asm("___DroidSansMono_11:");
/****************************************
 * Font header
 ****************************************/
asm(".byte 0x00");           // Font ID
asm(".byte 0x00");           // Font information:  1 bit per pixel, Characters zero degress rotation
asm(".byte 0x20, 0x00");     // First Character
asm(".byte 0x7E, 0x00");     // Last Character
asm(".byte 0x0F, 0x00");     // Height
/****************************************
 * Font Glyph Table
 ****************************************/
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x84, 0x01, 0x00");       // Character - 32, offset (0x00000184)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x93, 0x01, 0x00");       // Character - 33, offset (0x00000193)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xA2, 0x01, 0x00");       // Character - 34, offset (0x000001A2)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xB1, 0x01, 0x00");       // Character - 35, offset (0x000001B1)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xC0, 0x01, 0x00");       // Character - 36, offset (0x000001C0)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xCF, 0x01, 0x00");       // Character - 37, offset (0x000001CF)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xDE, 0x01, 0x00");       // Character - 38, offset (0x000001DE)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xED, 0x01, 0x00");       // Character - 39, offset (0x000001ED)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xFC, 0x01, 0x00");       // Character - 40, offset (0x000001FC)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x0B, 0x02, 0x00");       // Character - 41, offset (0x0000020B)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x1A, 0x02, 0x00");       // Character - 42, offset (0x0000021A)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x29, 0x02, 0x00");       // Character - 43, offset (0x00000229)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x38, 0x02, 0x00");       // Character - 44, offset (0x00000238)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x47, 0x02, 0x00");       // Character - 45, offset (0x00000247)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x56, 0x02, 0x00");       // Character - 46, offset (0x00000256)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x65, 0x02, 0x00");       // Character - 47, offset (0x00000265)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x74, 0x02, 0x00");       // Character - 48, offset (0x00000274)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x83, 0x02, 0x00");       // Character - 49, offset (0x00000283)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x92, 0x02, 0x00");       // Character - 50, offset (0x00000292)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xA1, 0x02, 0x00");       // Character - 51, offset (0x000002A1)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xB0, 0x02, 0x00");       // Character - 52, offset (0x000002B0)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xBF, 0x02, 0x00");       // Character - 53, offset (0x000002BF)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xCE, 0x02, 0x00");       // Character - 54, offset (0x000002CE)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xDD, 0x02, 0x00");       // Character - 55, offset (0x000002DD)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xEC, 0x02, 0x00");       // Character - 56, offset (0x000002EC)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xFB, 0x02, 0x00");       // Character - 57, offset (0x000002FB)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x0A, 0x03, 0x00");       // Character - 58, offset (0x0000030A)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x19, 0x03, 0x00");       // Character - 59, offset (0x00000319)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x28, 0x03, 0x00");       // Character - 60, offset (0x00000328)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x37, 0x03, 0x00");       // Character - 61, offset (0x00000337)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x46, 0x03, 0x00");       // Character - 62, offset (0x00000346)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x55, 0x03, 0x00");       // Character - 63, offset (0x00000355)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x64, 0x03, 0x00");       // Character - 64, offset (0x00000364)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x73, 0x03, 0x00");       // Character - 65, offset (0x00000373)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x82, 0x03, 0x00");       // Character - 66, offset (0x00000382)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x91, 0x03, 0x00");       // Character - 67, offset (0x00000391)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xA0, 0x03, 0x00");       // Character - 68, offset (0x000003A0)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xAF, 0x03, 0x00");       // Character - 69, offset (0x000003AF)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xBE, 0x03, 0x00");       // Character - 70, offset (0x000003BE)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xCD, 0x03, 0x00");       // Character - 71, offset (0x000003CD)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xDC, 0x03, 0x00");       // Character - 72, offset (0x000003DC)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xEB, 0x03, 0x00");       // Character - 73, offset (0x000003EB)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xFA, 0x03, 0x00");       // Character - 74, offset (0x000003FA)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x09, 0x04, 0x00");       // Character - 75, offset (0x00000409)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x18, 0x04, 0x00");       // Character - 76, offset (0x00000418)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x27, 0x04, 0x00");       // Character - 77, offset (0x00000427)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x36, 0x04, 0x00");       // Character - 78, offset (0x00000436)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x45, 0x04, 0x00");       // Character - 79, offset (0x00000445)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x54, 0x04, 0x00");       // Character - 80, offset (0x00000454)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x63, 0x04, 0x00");       // Character - 81, offset (0x00000463)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x72, 0x04, 0x00");       // Character - 82, offset (0x00000472)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x81, 0x04, 0x00");       // Character - 83, offset (0x00000481)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x90, 0x04, 0x00");       // Character - 84, offset (0x00000490)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x9F, 0x04, 0x00");       // Character - 85, offset (0x0000049F)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xAE, 0x04, 0x00");       // Character - 86, offset (0x000004AE)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xBD, 0x04, 0x00");       // Character - 87, offset (0x000004BD)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xCC, 0x04, 0x00");       // Character - 88, offset (0x000004CC)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xDB, 0x04, 0x00");       // Character - 89, offset (0x000004DB)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xEA, 0x04, 0x00");       // Character - 90, offset (0x000004EA)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xF9, 0x04, 0x00");       // Character - 91, offset (0x000004F9)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x08, 0x05, 0x00");       // Character - 92, offset (0x00000508)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x17, 0x05, 0x00");       // Character - 93, offset (0x00000517)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x26, 0x05, 0x00");       // Character - 94, offset (0x00000526)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x35, 0x05, 0x00");       // Character - 95, offset (0x00000535)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x44, 0x05, 0x00");       // Character - 96, offset (0x00000544)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x53, 0x05, 0x00");       // Character - 97, offset (0x00000553)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x62, 0x05, 0x00");       // Character - 98, offset (0x00000562)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x71, 0x05, 0x00");       // Character - 99, offset (0x00000571)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x80, 0x05, 0x00");       // Character - 100, offset (0x00000580)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x8F, 0x05, 0x00");       // Character - 101, offset (0x0000058F)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x9E, 0x05, 0x00");       // Character - 102, offset (0x0000059E)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xAD, 0x05, 0x00");       // Character - 103, offset (0x000005AD)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xBC, 0x05, 0x00");       // Character - 104, offset (0x000005BC)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xCB, 0x05, 0x00");       // Character - 105, offset (0x000005CB)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xDA, 0x05, 0x00");       // Character - 106, offset (0x000005DA)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xE9, 0x05, 0x00");       // Character - 107, offset (0x000005E9)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xF8, 0x05, 0x00");       // Character - 108, offset (0x000005F8)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x07, 0x06, 0x00");       // Character - 109, offset (0x00000607)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x16, 0x06, 0x00");       // Character - 110, offset (0x00000616)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x25, 0x06, 0x00");       // Character - 111, offset (0x00000625)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x34, 0x06, 0x00");       // Character - 112, offset (0x00000634)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x43, 0x06, 0x00");       // Character - 113, offset (0x00000643)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x52, 0x06, 0x00");       // Character - 114, offset (0x00000652)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x61, 0x06, 0x00");       // Character - 115, offset (0x00000661)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x70, 0x06, 0x00");       // Character - 116, offset (0x00000670)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x7F, 0x06, 0x00");       // Character - 117, offset (0x0000067F)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x8E, 0x06, 0x00");       // Character - 118, offset (0x0000068E)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x9D, 0x06, 0x00");       // Character - 119, offset (0x0000069D)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xAC, 0x06, 0x00");       // Character - 120, offset (0x000006AC)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xBB, 0x06, 0x00");       // Character - 121, offset (0x000006BB)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xCA, 0x06, 0x00");       // Character - 122, offset (0x000006CA)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xD9, 0x06, 0x00");       // Character - 123, offset (0x000006D9)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xE8, 0x06, 0x00");       // Character - 124, offset (0x000006E8)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xF7, 0x06, 0x00");       // Character - 125, offset (0x000006F7)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x06, 0x07, 0x00");       // Character - 126, offset (0x00000706)
/***********************************
 * Font Characters
 ***********************************/
/***********************************
 * Character - 32
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 33
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 34
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 35
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x3F");        // ******  
asm(".byte 0x14");        //   * *   
asm(".byte 0x0A");        //  * *    
asm(".byte 0x3F");        // ******  
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 36
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x3C");        //   ****  
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0C");        //   **    
asm(".byte 0x38");        //    ***  
asm(".byte 0x28");        //    * *  
asm(".byte 0x28");        //    * *  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 37
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x22");        //  *   *  
asm(".byte 0x15");        // * * *   
asm(".byte 0x15");        // * * *   
asm(".byte 0x0A");        //  * *    
asm(".byte 0x28");        //    * *  
asm(".byte 0x54");        //   * * * 
asm(".byte 0x54");        //   * * * 
asm(".byte 0x22");        //  *   *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 38
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x0C");        //   **    
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x32");        //  *  **  
asm(".byte 0x32");        //  *  **  
asm(".byte 0x7C");        //   ***** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 39
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 40
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x10");        //     *   
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x08");        //    *    
asm(".byte 0x10");        //     *   
asm(".byte 0x00");        //         

/***********************************
 * Character - 41
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x04");        //   *     
asm(".byte 0x08");        //    *    
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x00");        //         

/***********************************
 * Character - 42
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x3E");        //  *****  
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 43
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x3E");        //  *****  
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 44
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 45
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 46
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 47
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x20");        //      *  
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 48
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 49
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0C");        //   **    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 50
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x20");        //      *  
asm(".byte 0x20");        //      *  
asm(".byte 0x20");        //      *  
asm(".byte 0x10");        //     *   
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 51
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x20");        //      *  
asm(".byte 0x20");        //      *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x20");        //      *  
asm(".byte 0x20");        //      *  
asm(".byte 0x20");        //      *  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 52
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x10");        //     *   
asm(".byte 0x18");        //    **   
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x3E");        //  *****  
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 53
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x1E");        //  ****   
asm(".byte 0x20");        //      *  
asm(".byte 0x20");        //      *  
asm(".byte 0x20");        //      *  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 54
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x38");        //    ***  
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x1E");        //  ****   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 55
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x20");        //      *  
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 56
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 57
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x20");        //      *  
asm(".byte 0x10");        //     *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 58
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 59
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 60
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x20");        //      *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x02");        //  *      
asm(".byte 0x1C");        //   ***   
asm(".byte 0x20");        //      *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 61
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 62
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x1C");        //   ***   
asm(".byte 0x20");        //      *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 63
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0E");        //  ***    
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x00");        //         
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 64
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x59");        // *  ** * 
asm(".byte 0x55");        // * * * * 
asm(".byte 0x55");        // * * * * 
asm(".byte 0x55");        // * * * * 
asm(".byte 0x2D");        // * ** *  
asm(".byte 0x02");        //  *      
asm(".byte 0x3C");        //   ****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 65
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x41");        // *     * 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 66
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 67
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x04");        //   *     
asm(".byte 0x3C");        //   ****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 68
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x12");        //  *  *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 69
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x3E");        //  *****  
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 70
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x3E");        //  *****  
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 71
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x32");        //  *  **  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x24");        //   *  *  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 72
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 73
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 74
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 75
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x22");        //  *   *  
asm(".byte 0x12");        //  *  *   
asm(".byte 0x0A");        //  * *    
asm(".byte 0x06");        //  **     
asm(".byte 0x0A");        //  * *    
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 76
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 77
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x36");        //  ** **  
asm(".byte 0x36");        //  ** **  
asm(".byte 0x36");        //  ** **  
asm(".byte 0x36");        //  ** **  
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 78
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x22");        //  *   *  
asm(".byte 0x26");        //  **  *  
asm(".byte 0x26");        //  **  *  
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x32");        //  *  **  
asm(".byte 0x32");        //  *  **  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 79
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 80
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 81
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x10");        //     *   
asm(".byte 0x20");        //      *  
asm(".byte 0x00");        //         

/***********************************
 * Character - 82
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x42");        //  *    * 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 83
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x0C");        //   **    
asm(".byte 0x30");        //     **  
asm(".byte 0x20");        //      *  
asm(".byte 0x20");        //      *  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 84
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 85
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 86
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x41");        // *     * 
asm(".byte 0x41");        // *     * 
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 87
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x41");        // *     * 
asm(".byte 0x41");        // *     * 
asm(".byte 0x49");        // *  *  * 
asm(".byte 0x49");        // *  *  * 
asm(".byte 0x55");        // * * * * 
asm(".byte 0x36");        //  ** **  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 88
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x41");        // *     * 
asm(".byte 0x22");        //  *   *  
asm(".byte 0x14");        //   * *   
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x14");        //   * *   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x41");        // *     * 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 89
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x41");        // *     * 
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x14");        //   * *   
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 90
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x20");        //      *  
asm(".byte 0x10");        //     *   
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 91
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x1C");        //   ***   
asm(".byte 0x00");        //         

/***********************************
 * Character - 92
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x20");        //      *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 93
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x1C");        //   ***   
asm(".byte 0x00");        //         

/***********************************
 * Character - 94
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 95
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x7F");        // ******* 
asm(".byte 0x00");        //         

/***********************************
 * Character - 96
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x10");        //     *   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 97
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x20");        //      *  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 98
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x1E");        //  ****   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 99
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x3C");        //   ****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 100
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x20");        //      *  
asm(".byte 0x20");        //      *  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 101
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x3C");        //   ****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 102
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x38");        //    ***  
asm(".byte 0x08");        //    *    
asm(".byte 0x3E");        //  *****  
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 103
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x7C");        //   ***** 
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x02");        //  *      
asm(".byte 0x3E");        //  *****  
asm(".byte 0x42");        //  *    * 
asm(".byte 0x42");        //  *    * 
asm(".byte 0x3C");        //   ****  

/***********************************
 * Character - 104
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x1E");        //  ****   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 105
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x0C");        //   **    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 106
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x10");        //     *   
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x0E");        //  ***    

/***********************************
 * Character - 107
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x22");        //  *   *  
asm(".byte 0x12");        //  *  *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x12");        //  *  *   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x42");        //  *    * 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 108
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0C");        //   **    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 109
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 110
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 111
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 112
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      

/***********************************
 * Character - 113
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x20");        //      *  
asm(".byte 0x20");        //      *  
asm(".byte 0x20");        //      *  

/***********************************
 * Character - 114
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 115
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x02");        //  *      
asm(".byte 0x0E");        //  ***    
asm(".byte 0x30");        //     **  
asm(".byte 0x20");        //      *  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 116
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x3E");        //  *****  
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x38");        //    ***  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 117
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 118
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 119
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x49");        // *  *  * 
asm(".byte 0x55");        // * * * * 
asm(".byte 0x55");        // * * * * 
asm(".byte 0x55");        // * * * * 
asm(".byte 0x32");        //  *  **  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 120
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x22");        //  *   *  
asm(".byte 0x14");        //   * *   
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x14");        //   * *   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 121
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x18");        //    **   
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x06");        //  **     

/***********************************
 * Character - 122
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x10");        //     *   
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 123
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x30");        //     **  
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x06");        //  **     
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x30");        //     **  
asm(".byte 0x00");        //         

/***********************************
 * Character - 124
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    

/***********************************
 * Character - 125
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x30");        //     **  
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         

/***********************************
 * Character - 126
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0E");        //  ***    
asm(".byte 0x38");        //    ***  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

asm(".section .const, psv, page");
/*********************************
 * TTF Font File Structure
 * Label: DroidSansMono_22
 * Description:  Height: 30 pixels, 1 bit per pixel, Range: ' ' to '~'
 ***********************************/
#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
extern const char __DroidSansMono_22[] __attribute__((space(prog), aligned(2)));
#else
extern const char __DroidSansMono_22[] __attribute__((aligned(2)));
#endif

const FONT_FLASH DroidSansMono_22 =
{
    (FLASH | COMP_NONE),
    (GFX_FONT_SPACE char *)__DroidSansMono_22
};

#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
asm(".section .text, code");
#else
asm(".section .const, psv, page");
#endif
asm(".global ___DroidSansMono_22");
asm(".align 2");
asm("___DroidSansMono_22:");
/****************************************
 * Font header
 ****************************************/
asm(".byte 0x00");           // Font ID
asm(".byte 0x00");           // Font information:  1 bit per pixel, Characters zero degress rotation
asm(".byte 0x20, 0x00");     // First Character
asm(".byte 0x7E, 0x00");     // Last Character
asm(".byte 0x1E, 0x00");     // Height
/****************************************
 * Font Glyph Table
 ****************************************/
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x84, 0x01, 0x00");       // Character - 32, offset (0x00000184)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xC0, 0x01, 0x00");       // Character - 33, offset (0x000001C0)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xFC, 0x01, 0x00");       // Character - 34, offset (0x000001FC)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x38, 0x02, 0x00");       // Character - 35, offset (0x00000238)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x74, 0x02, 0x00");       // Character - 36, offset (0x00000274)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xB0, 0x02, 0x00");       // Character - 37, offset (0x000002B0)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xEC, 0x02, 0x00");       // Character - 38, offset (0x000002EC)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x28, 0x03, 0x00");       // Character - 39, offset (0x00000328)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x64, 0x03, 0x00");       // Character - 40, offset (0x00000364)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xA0, 0x03, 0x00");       // Character - 41, offset (0x000003A0)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xDC, 0x03, 0x00");       // Character - 42, offset (0x000003DC)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x18, 0x04, 0x00");       // Character - 43, offset (0x00000418)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x54, 0x04, 0x00");       // Character - 44, offset (0x00000454)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x90, 0x04, 0x00");       // Character - 45, offset (0x00000490)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xCC, 0x04, 0x00");       // Character - 46, offset (0x000004CC)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x08, 0x05, 0x00");       // Character - 47, offset (0x00000508)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x44, 0x05, 0x00");       // Character - 48, offset (0x00000544)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x80, 0x05, 0x00");       // Character - 49, offset (0x00000580)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xBC, 0x05, 0x00");       // Character - 50, offset (0x000005BC)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xF8, 0x05, 0x00");       // Character - 51, offset (0x000005F8)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x34, 0x06, 0x00");       // Character - 52, offset (0x00000634)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x70, 0x06, 0x00");       // Character - 53, offset (0x00000670)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xAC, 0x06, 0x00");       // Character - 54, offset (0x000006AC)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xE8, 0x06, 0x00");       // Character - 55, offset (0x000006E8)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x24, 0x07, 0x00");       // Character - 56, offset (0x00000724)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x60, 0x07, 0x00");       // Character - 57, offset (0x00000760)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x9C, 0x07, 0x00");       // Character - 58, offset (0x0000079C)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xD8, 0x07, 0x00");       // Character - 59, offset (0x000007D8)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x14, 0x08, 0x00");       // Character - 60, offset (0x00000814)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x50, 0x08, 0x00");       // Character - 61, offset (0x00000850)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x8C, 0x08, 0x00");       // Character - 62, offset (0x0000088C)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xC8, 0x08, 0x00");       // Character - 63, offset (0x000008C8)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x04, 0x09, 0x00");       // Character - 64, offset (0x00000904)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x40, 0x09, 0x00");       // Character - 65, offset (0x00000940)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x7C, 0x09, 0x00");       // Character - 66, offset (0x0000097C)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xB8, 0x09, 0x00");       // Character - 67, offset (0x000009B8)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xF4, 0x09, 0x00");       // Character - 68, offset (0x000009F4)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x30, 0x0A, 0x00");       // Character - 69, offset (0x00000A30)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x6C, 0x0A, 0x00");       // Character - 70, offset (0x00000A6C)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xA8, 0x0A, 0x00");       // Character - 71, offset (0x00000AA8)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xE4, 0x0A, 0x00");       // Character - 72, offset (0x00000AE4)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x20, 0x0B, 0x00");       // Character - 73, offset (0x00000B20)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x5C, 0x0B, 0x00");       // Character - 74, offset (0x00000B5C)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x98, 0x0B, 0x00");       // Character - 75, offset (0x00000B98)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xD4, 0x0B, 0x00");       // Character - 76, offset (0x00000BD4)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x10, 0x0C, 0x00");       // Character - 77, offset (0x00000C10)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x4C, 0x0C, 0x00");       // Character - 78, offset (0x00000C4C)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x88, 0x0C, 0x00");       // Character - 79, offset (0x00000C88)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xC4, 0x0C, 0x00");       // Character - 80, offset (0x00000CC4)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x00, 0x0D, 0x00");       // Character - 81, offset (0x00000D00)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x3C, 0x0D, 0x00");       // Character - 82, offset (0x00000D3C)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x78, 0x0D, 0x00");       // Character - 83, offset (0x00000D78)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xB4, 0x0D, 0x00");       // Character - 84, offset (0x00000DB4)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xF0, 0x0D, 0x00");       // Character - 85, offset (0x00000DF0)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x2C, 0x0E, 0x00");       // Character - 86, offset (0x00000E2C)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x68, 0x0E, 0x00");       // Character - 87, offset (0x00000E68)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xA4, 0x0E, 0x00");       // Character - 88, offset (0x00000EA4)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xE0, 0x0E, 0x00");       // Character - 89, offset (0x00000EE0)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x1C, 0x0F, 0x00");       // Character - 90, offset (0x00000F1C)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x58, 0x0F, 0x00");       // Character - 91, offset (0x00000F58)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x94, 0x0F, 0x00");       // Character - 92, offset (0x00000F94)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xD0, 0x0F, 0x00");       // Character - 93, offset (0x00000FD0)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x0C, 0x10, 0x00");       // Character - 94, offset (0x0000100C)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x48, 0x10, 0x00");       // Character - 95, offset (0x00001048)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x84, 0x10, 0x00");       // Character - 96, offset (0x00001084)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xC0, 0x10, 0x00");       // Character - 97, offset (0x000010C0)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xFC, 0x10, 0x00");       // Character - 98, offset (0x000010FC)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x38, 0x11, 0x00");       // Character - 99, offset (0x00001138)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x74, 0x11, 0x00");       // Character - 100, offset (0x00001174)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xB0, 0x11, 0x00");       // Character - 101, offset (0x000011B0)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xEC, 0x11, 0x00");       // Character - 102, offset (0x000011EC)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x28, 0x12, 0x00");       // Character - 103, offset (0x00001228)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x64, 0x12, 0x00");       // Character - 104, offset (0x00001264)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xA0, 0x12, 0x00");       // Character - 105, offset (0x000012A0)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xDC, 0x12, 0x00");       // Character - 106, offset (0x000012DC)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x18, 0x13, 0x00");       // Character - 107, offset (0x00001318)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x54, 0x13, 0x00");       // Character - 108, offset (0x00001354)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x90, 0x13, 0x00");       // Character - 109, offset (0x00001390)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xCC, 0x13, 0x00");       // Character - 110, offset (0x000013CC)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x08, 0x14, 0x00");       // Character - 111, offset (0x00001408)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x44, 0x14, 0x00");       // Character - 112, offset (0x00001444)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x80, 0x14, 0x00");       // Character - 113, offset (0x00001480)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xBC, 0x14, 0x00");       // Character - 114, offset (0x000014BC)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xF8, 0x14, 0x00");       // Character - 115, offset (0x000014F8)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x34, 0x15, 0x00");       // Character - 116, offset (0x00001534)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x70, 0x15, 0x00");       // Character - 117, offset (0x00001570)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xAC, 0x15, 0x00");       // Character - 118, offset (0x000015AC)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xE8, 0x15, 0x00");       // Character - 119, offset (0x000015E8)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x24, 0x16, 0x00");       // Character - 120, offset (0x00001624)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x60, 0x16, 0x00");       // Character - 121, offset (0x00001660)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x9C, 0x16, 0x00");       // Character - 122, offset (0x0000169C)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xD8, 0x16, 0x00");       // Character - 123, offset (0x000016D8)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x14, 0x17, 0x00");       // Character - 124, offset (0x00001714)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x50, 0x17, 0x00");       // Character - 125, offset (0x00001750)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x8C, 0x17, 0x00");       // Character - 126, offset (0x0000178C)
/***********************************
 * Font Characters
 ***********************************/
/***********************************
 * Character - 32
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 33
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x40, 0x00");        //       *         
asm(".byte 0x40, 0x00");        //       *         
asm(".byte 0x40, 0x00");        //       *         
asm(".byte 0x40, 0x00");        //       *         
asm(".byte 0x40, 0x00");        //       *         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 34
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x38, 0x07");        //    ***  ***     
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 35
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x30, 0x06");        //     **   **     
asm(".byte 0x30, 0x06");        //     **   **     
asm(".byte 0x30, 0x06");        //     **   **     
asm(".byte 0x10, 0x02");        //     *    *      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0x18, 0x01");        //    **   *       
asm(".byte 0x08, 0x01");        //    *    *       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0xFF, 0x07");        // ***********     
asm(".byte 0xFF, 0x07");        // ***********     
asm(".byte 0x84, 0x00");        //   *    *        
asm(".byte 0x84, 0x00");        //   *    *        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 36
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x20, 0x00");        //      *          
asm(".byte 0x20, 0x00");        //      *          
asm(".byte 0xF8, 0x03");        //    *******      
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0x26, 0x00");        //  **  *          
asm(".byte 0x26, 0x00");        //  **  *          
asm(".byte 0x26, 0x00");        //  **  *          
asm(".byte 0x2C, 0x00");        //   ** *          
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0xE0, 0x01");        //      ****       
asm(".byte 0x20, 0x07");        //      *  ***     
asm(".byte 0x20, 0x06");        //      *   **     
asm(".byte 0x20, 0x06");        //      *   **     
asm(".byte 0x20, 0x07");        //      *  ***     
asm(".byte 0xFE, 0x03");        //  *********      
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x20, 0x00");        //      *          
asm(".byte 0x20, 0x00");        //      *          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 37
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x1E, 0x06");        //  ****    **     
asm(".byte 0x33, 0x02");        // **  **   *      
asm(".byte 0x33, 0x03");        // **  **  **      
asm(".byte 0xB3, 0x01");        // **  ** **       
asm(".byte 0xB3, 0x01");        // **  ** **       
asm(".byte 0xF3, 0x00");        // **  ****        
asm(".byte 0xDE, 0x00");        //  **** **        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xB0, 0x07");        //     ** ****     
asm(".byte 0xF0, 0x0C");        //     ****  **    
asm(".byte 0xD8, 0x0C");        //    ** **  **    
asm(".byte 0xD8, 0x0C");        //    ** **  **    
asm(".byte 0xCC, 0x0C");        //   **  **  **    
asm(".byte 0xC4, 0x0C");        //   *   **  **    
asm(".byte 0x86, 0x07");        //  **    ****     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 38
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0xD8, 0x00");        //    ** **        
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x7C, 0x0C");        //   *****   **    
asm(".byte 0xEC, 0x0C");        //   ** ***  **    
asm(".byte 0xC6, 0x05");        //  **   *** *     
asm(".byte 0x86, 0x07");        //  **    ****     
asm(".byte 0x06, 0x07");        //  **     ***     
asm(".byte 0x0E, 0x07");        //  ***    ***     
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0x78, 0x1C");        //    ****   ***   
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 39
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 40
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 41
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 42
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x40, 0x00");        //       *         
asm(".byte 0x40, 0x00");        //       *         
asm(".byte 0x44, 0x0C");        //   *   *   **    
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xA0, 0x00");        //      * *        
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x10, 0x01");        //     *   *       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 43
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xFE, 0x07");        //  **********     
asm(".byte 0xFE, 0x07");        //  **********     
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 44
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 45
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x03");        //    *******      
asm(".byte 0xF8, 0x03");        //    *******      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 46
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 47
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x01");        //         *       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 48
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0xF8, 0x01");        //    ******       
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0xF8, 0x01");        //    ******       
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 49
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xDC, 0x00");        //   *** **        
asm(".byte 0xC8, 0x00");        //    *  **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 50
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0xF8, 0x03");        //    *******      
asm(".byte 0x08, 0x07");        //    *    ***     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 51
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0x08, 0x07");        //    *    ***     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0xF0, 0x03");        //     ******      
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 52
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x80, 0x03");        //        ***      
asm(".byte 0xC0, 0x03");        //       ****      
asm(".byte 0xC0, 0x03");        //       ****      
asm(".byte 0x60, 0x03");        //      ** **      
asm(".byte 0x30, 0x03");        //     **  **      
asm(".byte 0x30, 0x03");        //     **  **      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x08, 0x03");        //    *    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 53
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x03");        //    *******      
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 54
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x03");        //      *****      
asm(".byte 0xF0, 0x03");        //     ******      
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0xF6, 0x01");        //  ** *****       
asm(".byte 0xFE, 0x03");        //  *********      
asm(".byte 0x0E, 0x07");        //  ***    ***     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0xF8, 0x03");        //    *******      
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 55
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFE, 0x07");        //  **********     
asm(".byte 0xFE, 0x07");        //  **********     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 56
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0xF8, 0x01");        //    ******       
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0xB8, 0x01");        //    *** **       
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0xF8, 0x01");        //    ******       
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x07");        //  **     ***     
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0xF8, 0x01");        //    ******       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 57
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x06, 0x02");        //  **      *      
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x0E, 0x07");        //  ***    ***     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xF8, 0x06");        //    ***** **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 58
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 59
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 60
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x04");        //           *     
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0xC0, 0x01");        //       ***       
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0xC0, 0x01");        //       ***       
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0x00, 0x04");        //           *     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 61
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 62
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x04, 0x00");        //   *             
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0xC0, 0x01");        //       ***       
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0xC0, 0x01");        //       ***       
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x04, 0x00");        //   *             
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 63
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x01");        //    ******       
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x40, 0x00");        //       *         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 64
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x38, 0x07");        //    ***  ***     
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x06, 0x08");        //  **        *    
asm(".byte 0x02, 0x18");        //  *         **   
asm(".byte 0xE3, 0x1B");        // **   ***** **   
asm(".byte 0x33, 0x1B");        // **  **  ** **   
asm(".byte 0x1B, 0x1B");        // ** **   ** **   
asm(".byte 0x1B, 0x1B");        // ** **   ** **   
asm(".byte 0x1B, 0x1B");        // ** **   ** **   
asm(".byte 0x1B, 0x1B");        // ** **   ** **   
asm(".byte 0xBB, 0x0A");        // ** *** * * *    
asm(".byte 0xF3, 0x0E");        // **  **** ***    
asm(".byte 0x02, 0x00");        //  *              
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x38, 0x07");        //    ***  ***     
asm(".byte 0xF0, 0x03");        //     ******      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 65
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xA0, 0x00");        //      * *        
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0x10, 0x01");        //     *   *       
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x08, 0x02");        //    *     *      
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x04, 0x04");        //   *       *     
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x03, 0x18");        // **         **   
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 66
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFE, 0x03");        //  *********      
asm(".byte 0xFE, 0x07");        //  **********     
asm(".byte 0x06, 0x0E");        //  **      ***    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0xFE, 0x03");        //  *********      
asm(".byte 0xFE, 0x07");        //  **********     
asm(".byte 0x06, 0x0E");        //  **      ***    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0E");        //  **      ***    
asm(".byte 0xFE, 0x07");        //  **********     
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 67
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x07");        //      ******     
asm(".byte 0xF0, 0x0F");        //     ********    
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x0E, 0x00");        //  ***            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0xF8, 0x0F");        //    *********    
asm(".byte 0xE0, 0x0F");        //      *******    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 68
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x86, 0x03");        //  **    ***      
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 69
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 70
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 71
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x07");        //      ******     
asm(".byte 0xF8, 0x07");        //    ********     
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x86, 0x0F");        //  **    *****    
asm(".byte 0x86, 0x0F");        //  **    *****    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0E, 0x0C");        //  ***      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x1C, 0x0C");        //   ***     **    
asm(".byte 0xF8, 0x0F");        //    *********    
asm(".byte 0xE0, 0x07");        //      ******     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 72
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 73
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 74
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 75
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0x5C, 0x00");        //   *** *         
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 76
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 77
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0E, 0x0E");        //  ***     ***    
asm(".byte 0x0E, 0x0E");        //  ***     ***    
asm(".byte 0x0E, 0x0E");        //  ***     ***    
asm(".byte 0x0E, 0x0E");        //  ***     ***    
asm(".byte 0x16, 0x0D");        //  ** *   * **    
asm(".byte 0x16, 0x0D");        //  ** *   * **    
asm(".byte 0x16, 0x0D");        //  ** *   * **    
asm(".byte 0x16, 0x0D");        //  ** *   * **    
asm(".byte 0xB6, 0x0D");        //  ** ** ** **    
asm(".byte 0xA6, 0x0C");        //  **  * *  **    
asm(".byte 0xA6, 0x0C");        //  **  * *  **    
asm(".byte 0xA6, 0x0C");        //  **  * *  **    
asm(".byte 0xE6, 0x0C");        //  **  ***  **    
asm(".byte 0xE6, 0x0C");        //  **  ***  **    
asm(".byte 0x46, 0x0C");        //  **   *   **    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 78
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0E, 0x0C");        //  ***      **    
asm(".byte 0x0E, 0x0C");        //  ***      **    
asm(".byte 0x1E, 0x0C");        //  ****     **    
asm(".byte 0x16, 0x0C");        //  ** *     **    
asm(".byte 0x36, 0x0C");        //  ** **    **    
asm(".byte 0x66, 0x0C");        //  **  **   **    
asm(".byte 0x66, 0x0C");        //  **  **   **    
asm(".byte 0xC6, 0x0C");        //  **   **  **    
asm(".byte 0xC6, 0x0C");        //  **   **  **    
asm(".byte 0x86, 0x0D");        //  **    ** **    
asm(".byte 0x06, 0x0D");        //  **     * **    
asm(".byte 0x06, 0x0F");        //  **     ****    
asm(".byte 0x06, 0x0E");        //  **      ***    
asm(".byte 0x06, 0x0E");        //  **      ***    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 79
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0xF8, 0x03");        //    *******      
asm(".byte 0x1C, 0x07");        //   ***   ***     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x1C, 0x07");        //   ***   ***     
asm(".byte 0xF8, 0x03");        //    *******      
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 80
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x0C, 0x0E");        //   **     ***    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 81
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0xF8, 0x03");        //    *******      
asm(".byte 0x1C, 0x07");        //   ***   ***     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x1C, 0x07");        //   ***   ***     
asm(".byte 0xF8, 0x03");        //    *******      
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0x00, 0x0E");        //          ***    
asm(".byte 0x00, 0x04");        //           *     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 82
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x0C, 0x0E");        //   **     ***    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0x8C, 0x03");        //   **   ***      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x0E");        //   **     ***    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x18");        //   **       **   
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 83
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x03");        //     ******      
asm(".byte 0xF8, 0x07");        //    ********     
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0xE0, 0x03");        //      *****      
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0xF8, 0x01");        //    ******       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 84
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFE, 0x07");        //  **********     
asm(".byte 0xFE, 0x07");        //  **********     
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 85
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 86
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x03, 0x18");        // **         **   
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0xA0, 0x00");        //      * *        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 87
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x03, 0x18");        // **         **   
asm(".byte 0x03, 0x18");        // **         **   
asm(".byte 0x03, 0x18");        // **         **   
asm(".byte 0x02, 0x18");        //  *         **   
asm(".byte 0x02, 0x08");        //  *         *    
asm(".byte 0x46, 0x08");        //  **   *    *    
asm(".byte 0xE6, 0x0C");        //  **  ***  **    
asm(".byte 0xA6, 0x0C");        //  **  * *  **    
asm(".byte 0xA6, 0x0C");        //  **  * *  **    
asm(".byte 0xA6, 0x0C");        //  **  * *  **    
asm(".byte 0xB4, 0x0D");        //   * ** ** **    
asm(".byte 0x14, 0x05");        //   * *   * *     
asm(".byte 0x14, 0x05");        //   * *   * *     
asm(".byte 0x14, 0x05");        //   * *   * *     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 88
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0xB8, 0x01");        //    *** **       
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0x98, 0x03");        //    **  ***      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x0C, 0x07");        //   **    ***     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x06, 0x0E");        //  **      ***    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 89
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x03, 0x0C");        // **        **    
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x98, 0x01");        //    **  **       
asm(".byte 0x98, 0x01");        //    **  **       
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 90
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xC0, 0x01");        //       ***       
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 91
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x03");        //      *****      
asm(".byte 0xE0, 0x03");        //      *****      
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xE0, 0x03");        //      *****      
asm(".byte 0xE0, 0x03");        //      *****      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 92
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x00, 0x01");        //         *       
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 93
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 94
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x40, 0x00");        //       *         
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xA0, 0x00");        //      * *        
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0x10, 0x01");        //     *   *       
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x08, 0x02");        //    *     *      
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x04, 0x04");        //   *       *     
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 95
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFF, 0x1F");        // *************   
asm(".byte 0xFF, 0x1F");        // *************   
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 96
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 97
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x01");        //    ******       
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0xF0, 0x07");        //     *******     
asm(".byte 0x3C, 0x06");        //   ****   **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x07");        //  **     ***     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x78, 0x06");        //    ****  **     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 98
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xEC, 0x03");        //   ** *****      
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x1C, 0x06");        //   ***    **     
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x1C, 0x06");        //   ***    **     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xEC, 0x03");        //   ** *****      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 99
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xE0, 0x07");        //      ******     
asm(".byte 0xF0, 0x07");        //     *******     
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0xF0, 0x07");        //     *******     
asm(".byte 0xE0, 0x07");        //      ******     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 100
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0xF0, 0x06");        //     **** **     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x0C, 0x07");        //   **    ***     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x0C, 0x07");        //   **    ***     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xF8, 0x06");        //    ***** **     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 101
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0xF8, 0x07");        //    ********     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xF8, 0x07");        //    ********     
asm(".byte 0xF0, 0x07");        //     *******     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 102
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x80, 0x0F");        //        *****    
asm(".byte 0xC0, 0x0F");        //       ******    
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 103
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x0F");        //     ********    
asm(".byte 0xB8, 0x03");        //    *** ***      
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0xF8, 0x07");        //    ********     
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xF8, 0x01");        //    ******       
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 104
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xEC, 0x01");        //   ** ****       
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0x1C, 0x06");        //   ***    **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 105
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 106
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0xC0, 0x01");        //       ***       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 107
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x0E");        //   **     ***    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 108
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xFC, 0x0F");        //   **********    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 109
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x36, 0x03");        //  ** **  **      
asm(".byte 0xFE, 0x07");        //  **********     
asm(".byte 0x66, 0x06");        //  **  **  **     
asm(".byte 0x66, 0x06");        //  **  **  **     
asm(".byte 0x66, 0x06");        //  **  **  **     
asm(".byte 0x66, 0x06");        //  **  **  **     
asm(".byte 0x66, 0x06");        //  **  **  **     
asm(".byte 0x66, 0x06");        //  **  **  **     
asm(".byte 0x66, 0x06");        //  **  **  **     
asm(".byte 0x66, 0x06");        //  **  **  **     
asm(".byte 0x66, 0x06");        //  **  **  **     
asm(".byte 0x66, 0x06");        //  **  **  **     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 110
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xEC, 0x01");        //   ** ****       
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0x1C, 0x06");        //   ***    **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 111
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0xF8, 0x03");        //    *******      
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0xF8, 0x03");        //    *******      
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 112
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xEC, 0x03");        //   ** *****      
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x1C, 0x06");        //   ***    **     
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x0C, 0x0C");        //   **      **    
asm(".byte 0x1C, 0x06");        //   ***    **     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xEC, 0x03");        //   ** *****      
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 113
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x06");        //    ***** **     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x0C, 0x07");        //   **    ***     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x0C, 0x07");        //   **    ***     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xF8, 0x06");        //    ***** **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 114
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x98, 0x07");        //    **  ****     
asm(".byte 0xF8, 0x07");        //    ********     
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 115
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x03");        //     ******      
asm(".byte 0xF8, 0x07");        //    ********     
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0xC0, 0x03");        //       ****      
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0xF8, 0x01");        //    ******       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 116
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x40, 0x00");        //       *         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xE0, 0x07");        //      ******     
asm(".byte 0xC0, 0x07");        //       *****     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 117
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x07");        //   **    ***     
asm(".byte 0xF8, 0x07");        //    ********     
asm(".byte 0xF0, 0x06");        //     **** **     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 118
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0xA0, 0x00");        //      * *        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 119
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x43, 0x18");        // **    *    **   
asm(".byte 0xE3, 0x18");        // **   ***   **   
asm(".byte 0xA2, 0x08");        //  *   * *   *    
asm(".byte 0xA2, 0x08");        //  *   * *   *    
asm(".byte 0xA6, 0x0C");        //  **  * *  **    
asm(".byte 0xB6, 0x0D");        //  ** ** ** **    
asm(".byte 0xB6, 0x0D");        //  ** ** ** **    
asm(".byte 0x14, 0x05");        //   * *   * *     
asm(".byte 0x14, 0x05");        //   * *   * *     
asm(".byte 0x14, 0x05");        //   * *   * *     
asm(".byte 0x1C, 0x07");        //   ***   ***     
asm(".byte 0x08, 0x02");        //    *     *      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 120
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0x30, 0x03");        //     **  **      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 121
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x10, 0x03");        //     *   **      
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0xB0, 0x01");        //     ** **       
asm(".byte 0xA0, 0x00");        //      * *        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x3E, 0x00");        //  *****          
asm(".byte 0x1E, 0x00");        //  ****           
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 122
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 123
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0x80, 0x07");        //        ****     
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x80, 0x07");        //        ****     
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 124
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 125
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x3C, 0x00");        //   ****          
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xC0, 0x01");        //       ***       
asm(".byte 0x00, 0x07");        //         ***     
asm(".byte 0xC0, 0x01");        //       ***       
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x3C, 0x00");        //   ****          
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 126
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x80, 0x03");        //        ***      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

asm(".section .const, psv, page");
/*********************************
 * TTF Font File Structure
 * Label: DroidSans_18
 * Description:  Height: 22 pixels, 1 bit per pixel, Range: ' ' to '~'
 ***********************************/
#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
extern const char __DroidSans_18[] __attribute__((space(prog), aligned(2)));
#else
extern const char __DroidSans_18[] __attribute__((aligned(2)));
#endif

const FONT_FLASH DroidSans_18 =
{
    (FLASH | COMP_NONE),
    (GFX_FONT_SPACE char *)__DroidSans_18
};

#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
asm(".section .text, code");
#else
asm(".section .const, psv, page");
#endif
asm(".global ___DroidSans_18");
asm(".align 2");
asm("___DroidSans_18:");
/****************************************
 * Font header
 ****************************************/
asm(".byte 0x00");           // Font ID
asm(".byte 0x00");           // Font information:  1 bit per pixel, Characters zero degress rotation
asm(".byte 0x20, 0x00");     // First Character
asm(".byte 0x7E, 0x00");     // Last Character
asm(".byte 0x16, 0x00");     // Height
/****************************************
 * Font Glyph Table
 ****************************************/
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x84, 0x01, 0x00");       // Character - 32, offset (0x00000184)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x9A, 0x01, 0x00");       // Character - 33, offset (0x0000019A)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xB0, 0x01, 0x00");       // Character - 34, offset (0x000001B0)
asm(".byte 0x0C");              // width of the glyph
asm(".byte 0xC6, 0x01, 0x00");       // Character - 35, offset (0x000001C6)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xF2, 0x01, 0x00");       // Character - 36, offset (0x000001F2)
asm(".byte 0x0F");              // width of the glyph
asm(".byte 0x1E, 0x02, 0x00");       // Character - 37, offset (0x0000021E)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x4A, 0x02, 0x00");       // Character - 38, offset (0x0000024A)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x76, 0x02, 0x00");       // Character - 39, offset (0x00000276)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x8C, 0x02, 0x00");       // Character - 40, offset (0x0000028C)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xA2, 0x02, 0x00");       // Character - 41, offset (0x000002A2)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xB8, 0x02, 0x00");       // Character - 42, offset (0x000002B8)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xE4, 0x02, 0x00");       // Character - 43, offset (0x000002E4)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x10, 0x03, 0x00");       // Character - 44, offset (0x00000310)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x26, 0x03, 0x00");       // Character - 45, offset (0x00000326)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x3C, 0x03, 0x00");       // Character - 46, offset (0x0000033C)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x52, 0x03, 0x00");       // Character - 47, offset (0x00000352)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x68, 0x03, 0x00");       // Character - 48, offset (0x00000368)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x94, 0x03, 0x00");       // Character - 49, offset (0x00000394)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xC0, 0x03, 0x00");       // Character - 50, offset (0x000003C0)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xEC, 0x03, 0x00");       // Character - 51, offset (0x000003EC)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x18, 0x04, 0x00");       // Character - 52, offset (0x00000418)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x44, 0x04, 0x00");       // Character - 53, offset (0x00000444)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x70, 0x04, 0x00");       // Character - 54, offset (0x00000470)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x9C, 0x04, 0x00");       // Character - 55, offset (0x0000049C)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xC8, 0x04, 0x00");       // Character - 56, offset (0x000004C8)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xF4, 0x04, 0x00");       // Character - 57, offset (0x000004F4)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x20, 0x05, 0x00");       // Character - 58, offset (0x00000520)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x36, 0x05, 0x00");       // Character - 59, offset (0x00000536)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x4C, 0x05, 0x00");       // Character - 60, offset (0x0000054C)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x78, 0x05, 0x00");       // Character - 61, offset (0x00000578)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xA4, 0x05, 0x00");       // Character - 62, offset (0x000005A4)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xD0, 0x05, 0x00");       // Character - 63, offset (0x000005D0)
asm(".byte 0x10");              // width of the glyph
asm(".byte 0xE6, 0x05, 0x00");       // Character - 64, offset (0x000005E6)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0x12, 0x06, 0x00");       // Character - 65, offset (0x00000612)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0x3E, 0x06, 0x00");       // Character - 66, offset (0x0000063E)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0x6A, 0x06, 0x00");       // Character - 67, offset (0x0000066A)
asm(".byte 0x0C");              // width of the glyph
asm(".byte 0x96, 0x06, 0x00");       // Character - 68, offset (0x00000696)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xC2, 0x06, 0x00");       // Character - 69, offset (0x000006C2)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0xEE, 0x06, 0x00");       // Character - 70, offset (0x000006EE)
asm(".byte 0x0C");              // width of the glyph
asm(".byte 0x1A, 0x07, 0x00");       // Character - 71, offset (0x0000071A)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x46, 0x07, 0x00");       // Character - 72, offset (0x00000746)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x72, 0x07, 0x00");       // Character - 73, offset (0x00000772)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x88, 0x07, 0x00");       // Character - 74, offset (0x00000788)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x9E, 0x07, 0x00");       // Character - 75, offset (0x0000079E)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0xCA, 0x07, 0x00");       // Character - 76, offset (0x000007CA)
asm(".byte 0x10");              // width of the glyph
asm(".byte 0xF6, 0x07, 0x00");       // Character - 77, offset (0x000007F6)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x22, 0x08, 0x00");       // Character - 78, offset (0x00000822)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x4E, 0x08, 0x00");       // Character - 79, offset (0x0000084E)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0x7A, 0x08, 0x00");       // Character - 80, offset (0x0000087A)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0xA6, 0x08, 0x00");       // Character - 81, offset (0x000008A6)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0xD2, 0x08, 0x00");       // Character - 82, offset (0x000008D2)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0xFE, 0x08, 0x00");       // Character - 83, offset (0x000008FE)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x2A, 0x09, 0x00");       // Character - 84, offset (0x0000092A)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x56, 0x09, 0x00");       // Character - 85, offset (0x00000956)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x82, 0x09, 0x00");       // Character - 86, offset (0x00000982)
asm(".byte 0x10");              // width of the glyph
asm(".byte 0xAE, 0x09, 0x00");       // Character - 87, offset (0x000009AE)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xDA, 0x09, 0x00");       // Character - 88, offset (0x000009DA)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x06, 0x0A, 0x00");       // Character - 89, offset (0x00000A06)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x32, 0x0A, 0x00");       // Character - 90, offset (0x00000A32)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x5E, 0x0A, 0x00");       // Character - 91, offset (0x00000A5E)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x74, 0x0A, 0x00");       // Character - 92, offset (0x00000A74)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x8A, 0x0A, 0x00");       // Character - 93, offset (0x00000A8A)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xA0, 0x0A, 0x00");       // Character - 94, offset (0x00000AA0)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xCC, 0x0A, 0x00");       // Character - 95, offset (0x00000ACC)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xE2, 0x0A, 0x00");       // Character - 96, offset (0x00000AE2)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x0E, 0x0B, 0x00");       // Character - 97, offset (0x00000B0E)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0x3A, 0x0B, 0x00");       // Character - 98, offset (0x00000B3A)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x66, 0x0B, 0x00");       // Character - 99, offset (0x00000B66)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0x7C, 0x0B, 0x00");       // Character - 100, offset (0x00000B7C)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xA8, 0x0B, 0x00");       // Character - 101, offset (0x00000BA8)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xD4, 0x0B, 0x00");       // Character - 102, offset (0x00000BD4)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xEA, 0x0B, 0x00");       // Character - 103, offset (0x00000BEA)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0x16, 0x0C, 0x00");       // Character - 104, offset (0x00000C16)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x42, 0x0C, 0x00");       // Character - 105, offset (0x00000C42)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x58, 0x0C, 0x00");       // Character - 106, offset (0x00000C58)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x6E, 0x0C, 0x00");       // Character - 107, offset (0x00000C6E)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x9A, 0x0C, 0x00");       // Character - 108, offset (0x00000C9A)
asm(".byte 0x10");              // width of the glyph
asm(".byte 0xB0, 0x0C, 0x00");       // Character - 109, offset (0x00000CB0)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0xDC, 0x0C, 0x00");       // Character - 110, offset (0x00000CDC)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x08, 0x0D, 0x00");       // Character - 111, offset (0x00000D08)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0x34, 0x0D, 0x00");       // Character - 112, offset (0x00000D34)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0x60, 0x0D, 0x00");       // Character - 113, offset (0x00000D60)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x8C, 0x0D, 0x00");       // Character - 114, offset (0x00000D8C)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0xA2, 0x0D, 0x00");       // Character - 115, offset (0x00000DA2)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xCE, 0x0D, 0x00");       // Character - 116, offset (0x00000DCE)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0xE4, 0x0D, 0x00");       // Character - 117, offset (0x00000DE4)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x10, 0x0E, 0x00");       // Character - 118, offset (0x00000E10)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x3C, 0x0E, 0x00");       // Character - 119, offset (0x00000E3C)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x68, 0x0E, 0x00");       // Character - 120, offset (0x00000E68)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x94, 0x0E, 0x00");       // Character - 121, offset (0x00000E94)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xC0, 0x0E, 0x00");       // Character - 122, offset (0x00000EC0)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xD6, 0x0E, 0x00");       // Character - 123, offset (0x00000ED6)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xEC, 0x0E, 0x00");       // Character - 124, offset (0x00000EEC)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x18, 0x0F, 0x00");       // Character - 125, offset (0x00000F18)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x2E, 0x0F, 0x00");       // Character - 126, offset (0x00000F2E)
/***********************************
 * Font Characters
 ***********************************/
/***********************************
 * Character - 32
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 33
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 34
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x36");        //  ** **  
asm(".byte 0x36");        //  ** **  
asm(".byte 0x36");        //  ** **  
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 35
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x10, 0x02");        //     *    *      
asm(".byte 0x10, 0x02");        //     *    *      
asm(".byte 0x10, 0x03");        //     *   **      
asm(".byte 0x18, 0x01");        //    **   *       
asm(".byte 0xFE, 0x0F");        //  ***********    
asm(".byte 0x08, 0x01");        //    *    *       
asm(".byte 0x08, 0x01");        //    *    *       
asm(".byte 0x08, 0x01");        //    *    *       
asm(".byte 0xFF, 0x07");        // ***********     
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x00");        //   **   *        
asm(".byte 0x84, 0x00");        //   *    *        
asm(".byte 0x84, 0x00");        //   *    *        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 36
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0xDE, 0x00");        //  **** **        
asm(".byte 0x16, 0x00");        //  ** *           
asm(".byte 0x16, 0x00");        //  ** *           
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0xD0, 0x00");        //     * **        
asm(".byte 0x90, 0x01");        //     *  **       
asm(".byte 0x90, 0x01");        //     *  **       
asm(".byte 0xD6, 0x00");        //  ** * **        
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 37
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x1C, 0x0C");        //   ***     **    
asm(".byte 0x36, 0x04");        //  ** **    *     
asm(".byte 0x22, 0x06");        //  *   *   **     
asm(".byte 0x22, 0x02");        //  *   *   *      
asm(".byte 0x22, 0x01");        //  *   *  *       
asm(".byte 0xB6, 0x01");        //  ** ** **       
asm(".byte 0x9C, 0x1C");        //   ***  *  ***   
asm(".byte 0xC0, 0x36");        //       ** ** **  
asm(".byte 0x40, 0x22");        //       *  *   *  
asm(".byte 0x20, 0x22");        //      *   *   *  
asm(".byte 0x30, 0x22");        //     **   *   *  
asm(".byte 0x10, 0x36");        //     *    ** **  
asm(".byte 0x18, 0x1C");        //    **     ***   
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 38
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xDC, 0x01");        //   *** ***       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x6C, 0x0C");        //   ** **   **    
asm(".byte 0xC6, 0x0C");        //  **   **  **    
asm(".byte 0x86, 0x07");        //  **    ****     
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0xCC, 0x07");        //   **  *****     
asm(".byte 0xF8, 0x0C");        //    *****  **    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 39
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 40
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x18");        //    **   
asm(".byte 0x08");        //    *    
asm(".byte 0x0C");        //   **    
asm(".byte 0x04");        //   *     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x04");        //   *     
asm(".byte 0x0C");        //   **    
asm(".byte 0x08");        //    *    
asm(".byte 0x18");        //    **   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 41
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x02");        //  *      
asm(".byte 0x06");        //  **     
asm(".byte 0x04");        //   *     
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x04");        //   *     
asm(".byte 0x06");        //  **     
asm(".byte 0x02");        //  *      
asm(".byte 0x03");        // **      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 42
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 43
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 44
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x03");        // **      
asm(".byte 0x01");        // *       
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 45
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 46
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 47
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x60");        //      ** 
asm(".byte 0x20");        //      *  
asm(".byte 0x30");        //     **  
asm(".byte 0x30");        //     **  
asm(".byte 0x10");        //     *   
asm(".byte 0x18");        //    **   
asm(".byte 0x08");        //    *    
asm(".byte 0x0C");        //   **    
asm(".byte 0x04");        //   *     
asm(".byte 0x06");        //  **     
asm(".byte 0x02");        //  *      
asm(".byte 0x03");        // **      
asm(".byte 0x03");        // **      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 48
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x84, 0x00");        //   *    *        
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x84, 0x00");        //   *    *        
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 49
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0x64, 0x00");        //   *  **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 50
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0xEE, 0x00");        //  *** ***        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 51
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0xCE, 0x01");        //  ***  ***       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 52
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0xD8, 0x00");        //    ** **        
asm(".byte 0xC8, 0x00");        //    *  **        
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC2, 0x00");        //  *    **        
asm(".byte 0xC3, 0x00");        // **    **        
asm(".byte 0xFF, 0x01");        // *********       
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 53
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xE6, 0x00");        //  **  ***        
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 54
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x00");        //     ****        
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x04, 0x00");        //   *             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x76, 0x00");        //  ** ***         
asm(".byte 0xCE, 0x00");        //  ***  **        
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x84, 0x01");        //   *    **       
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 55
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x00");        //        *        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x40, 0x00");        //       *         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x20, 0x00");        //      *          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 56
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0xCE, 0x01");        //  ***  ***       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 57
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x86, 0x00");        //  **    *        
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0xCC, 0x01");        //   **  ***       
asm(".byte 0xB8, 0x01");        //    *** **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x00");        //        *        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x3C, 0x00");        //   ****          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 58
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 59
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x03");        // **      
asm(".byte 0x01");        // *       
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 60
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x01");        //         *       
asm(".byte 0xC0, 0x01");        //       ***       
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0xC0, 0x01");        //       ***       
asm(".byte 0x00, 0x01");        //         *       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 61
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 62
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x02, 0x00");        //  *              
asm(".byte 0x0E, 0x00");        //  ***            
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x0E, 0x00");        //  ***            
asm(".byte 0x02, 0x00");        //  *              
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 63
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x33");        // **  **  
asm(".byte 0x60");        //      ** 
asm(".byte 0x60");        //      ** 
asm(".byte 0x60");        //      ** 
asm(".byte 0x60");        //      ** 
asm(".byte 0x30");        //     **  
asm(".byte 0x18");        //    **   
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x00");        //         
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 64
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xC0, 0x07");        //       *****     
asm(".byte 0x30, 0x18");        //     **     **   
asm(".byte 0x18, 0x30");        //    **       **  
asm(".byte 0x0C, 0x20");        //   **         *  
asm(".byte 0x84, 0x67");        //   *    ****  ** 
asm(".byte 0x46, 0x44");        //  **   *   *   * 
asm(".byte 0x62, 0x44");        //  *   **   *   * 
asm(".byte 0x22, 0x44");        //  *   *    *   * 
asm(".byte 0x22, 0x66");        //  *   *   **  ** 
asm(".byte 0x62, 0x26");        //  *   **  **  *  
asm(".byte 0xC6, 0x19");        //  **   ***  **   
asm(".byte 0x04, 0x00");        //   *             
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x38, 0x0C");        //    ***    **    
asm(".byte 0xE0, 0x07");        //      ******     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 65
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x20, 0x00");        //      *          
asm(".byte 0x70, 0x00");        //     ***         
asm(".byte 0x50, 0x00");        //     * *         
asm(".byte 0xD8, 0x00");        //    ** **        
asm(".byte 0xD8, 0x00");        //    ** **        
asm(".byte 0x88, 0x00");        //    *   *        
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0xFE, 0x03");        //  *********      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x02, 0x02");        //  *       *      
asm(".byte 0x03, 0x06");        // **       **     
asm(".byte 0x03, 0x06");        // **       **     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 66
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x01");        //   **    *       
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 67
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x38, 0x03");        //    ***  **      
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x38, 0x03");        //    ***  **      
asm(".byte 0xF0, 0x03");        //     ******      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 68
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0xCC, 0x01");        //   **  ***       
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 69
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 70
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 71
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x03");        //     ******      
asm(".byte 0x38, 0x02");        //    ***   *      
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x04, 0x00");        //   *             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x86, 0x07");        //  **    ****     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x06, 0x06");        //  **      **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x38, 0x06");        //    ***   **     
asm(".byte 0xF0, 0x03");        //     ******      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 72
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0xFC, 0x07");        //   *********     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 73
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 74
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x0C");        //   **    
asm(".byte 0x07");        // ***     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 75
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x00");        //   **   *        
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x3C, 0x00");        //   ****          
asm(".byte 0x3C, 0x00");        //   ****          
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x0C, 0x01");        //   **    *       
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 76
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 77
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x1C, 0x38");        //   ***      ***  
asm(".byte 0x14, 0x38");        //   * *      ***  
asm(".byte 0x1C, 0x38");        //   ***      ***  
asm(".byte 0x3C, 0x3C");        //   ****    ****  
asm(".byte 0x2C, 0x34");        //   ** *    * **  
asm(".byte 0x2C, 0x34");        //   ** *    * **  
asm(".byte 0x6C, 0x36");        //   ** **  ** **  
asm(".byte 0x6C, 0x32");        //   ** **  *  **  
asm(".byte 0x4C, 0x32");        //   **  *  *  **  
asm(".byte 0xCC, 0x33");        //   **  ****  **  
asm(".byte 0xCC, 0x31");        //   **  ***   **  
asm(".byte 0x8C, 0x31");        //   **   **   **  
asm(".byte 0x8C, 0x31");        //   **   **   **  
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 78
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x1C, 0x06");        //   ***    **     
asm(".byte 0x1C, 0x06");        //   ***    **     
asm(".byte 0x3C, 0x06");        //   ****   **     
asm(".byte 0x2C, 0x06");        //   ** *   **     
asm(".byte 0x6C, 0x06");        //   ** **  **     
asm(".byte 0x4C, 0x06");        //   **  *  **     
asm(".byte 0xCC, 0x06");        //   **  ** **     
asm(".byte 0x8C, 0x06");        //   **   * **     
asm(".byte 0x8C, 0x07");        //   **   ****     
asm(".byte 0x0C, 0x07");        //   **    ***     
asm(".byte 0x0C, 0x07");        //   **    ***     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 79
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 80
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0xCC, 0x01");        //   **  ***       
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 81
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x06, 0x0C");        //  **       **    
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x00, 0x03");        //         **      
asm(".byte 0x00, 0x06");        //          **     
asm(".byte 0x00, 0x02");        //          *      
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 82
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0x4C, 0x00");        //   **  *         
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 83
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x4E, 0x00");        //  ***  *         
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x66, 0x00");        //  **  **         
asm(".byte 0x3E, 0x00");        //  *****          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 84
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFF, 0x03");        // **********      
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 85
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 86
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x03, 0x03");        // **      **      
asm(".byte 0x03, 0x03");        // **      **      
asm(".byte 0x02, 0x01");        //  *      *       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x84, 0x00");        //   *    *        
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x48, 0x00");        //    *  *         
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 87
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x83, 0xC1");        // **     **     **
asm(".byte 0x83, 0xC1");        // **     **     **
asm(".byte 0xC3, 0x41");        // **    ***     * 
asm(".byte 0xC6, 0x63");        //  **   ****   ** 
asm(".byte 0x46, 0x62");        //  **   *  *   ** 
asm(".byte 0x46, 0x62");        //  **   *  *   ** 
asm(".byte 0x64, 0x22");        //   *  **  *   *  
asm(".byte 0x6C, 0x36");        //   ** **  ** **  
asm(".byte 0x2C, 0x34");        //   ** *    * **  
asm(".byte 0x28, 0x14");        //    * *    * *   
asm(".byte 0x28, 0x14");        //    * *    * *   
asm(".byte 0x38, 0x1C");        //    ***    ***   
asm(".byte 0x18, 0x08");        //    **      *    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 88
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x03, 0x03");        // **      **      
asm(".byte 0x06, 0x01");        //  **     *       
asm(".byte 0x84, 0x01");        //   *    **       
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x48, 0x00");        //    *  *         
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x82, 0x01");        //  *     **       
asm(".byte 0x03, 0x03");        // **      **      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 89
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x03, 0x03");        // **      **      
asm(".byte 0x02, 0x01");        //  *      *       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x84, 0x00");        //   *    *        
asm(".byte 0xCC, 0x00");        //   **  **        
asm(".byte 0x68, 0x00");        //    * **         
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 90
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x20, 0x00");        //      *          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 91
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 92
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x03");        // **      
asm(".byte 0x02");        //  *      
asm(".byte 0x06");        //  **     
asm(".byte 0x04");        //   *     
asm(".byte 0x0C");        //   **    
asm(".byte 0x08");        //    *    
asm(".byte 0x18");        //    **   
asm(".byte 0x10");        //     *   
asm(".byte 0x30");        //     **  
asm(".byte 0x30");        //     **  
asm(".byte 0x20");        //      *  
asm(".byte 0x60");        //      ** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 93
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0F");        // ****    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0F");        // ****    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 94
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x58, 0x00");        //    ** *         
asm(".byte 0xC8, 0x00");        //    *  **        
asm(".byte 0x8C, 0x00");        //   **   *        
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x02, 0x01");        //  *      *       
asm(".byte 0x03, 0x03");        // **      **      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 95
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x7F");        // ******* 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 96
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x20, 0x00");        //      *          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 97
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0xE6, 0x00");        //  **  ***        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0xCE, 0x00");        //  ***  **        
asm(".byte 0xC3, 0x00");        // **    **        
asm(".byte 0xC3, 0x00");        // **    **        
asm(".byte 0xA7, 0x00");        // ***  * *        
asm(".byte 0x9E, 0x00");        //  ****  *        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 98
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0xF6, 0x00");        //  ** ****        
asm(".byte 0x8E, 0x01");        //  ***   **       
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x8E, 0x01");        //  ***   **       
asm(".byte 0xF2, 0x00");        //  *  ****        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 99
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x7C");        //   ***** 
asm(".byte 0x6E");        //  *** ** 
asm(".byte 0x03");        // **      
asm(".byte 0x03");        // **      
asm(".byte 0x03");        // **      
asm(".byte 0x03");        // **      
asm(".byte 0x03");        // **      
asm(".byte 0x03");        // **      
asm(".byte 0x6E");        //  *** ** 
asm(".byte 0x7C");        //   ***** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 100
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0xBC, 0x01");        //   **** **       
asm(".byte 0xC6, 0x01");        //  **   ***       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0xC6, 0x01");        //  **   ***       
asm(".byte 0x3C, 0x01");        //   ****  *       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 101
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0xCE, 0x00");        //  ***  **        
asm(".byte 0x82, 0x01");        //  *     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0xFF, 0x01");        // *********       
asm(".byte 0x03, 0x00");        // **              
asm(".byte 0x03, 0x00");        // **              
asm(".byte 0x02, 0x00");        //  *              
asm(".byte 0xCE, 0x01");        //  ***  ***       
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 102
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x78");        //    **** 
asm(".byte 0x4C");        //   **  * 
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x3F");        // ******  
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 103
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xCE, 0x00");        //  ***  **        
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x02, 0x00");        //  *              
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xC6, 0x01");        //  **   ***       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0xC3, 0x00");        // **    **        
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 104
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0xF6, 0x00");        //  ** ****        
asm(".byte 0xCE, 0x01");        //  ***  ***       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 105
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 106
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x04");        //   *     
asm(".byte 0x07");        // ***     
asm(".byte 0x00");        //         

/***********************************
 * Character - 107
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x66, 0x00");        //  **  **         
asm(".byte 0x36, 0x00");        //  ** **          
asm(".byte 0x1A, 0x00");        //  * **           
asm(".byte 0x3E, 0x00");        //  *****          
asm(".byte 0x66, 0x00");        //  **  **         
asm(".byte 0x66, 0x00");        //  **  **         
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 108
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 109
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x72, 0x1E");        //  *  ***  ****   
asm(".byte 0xCE, 0x3B");        //  ***  **** ***  
asm(".byte 0x86, 0x31");        //  **    **   **  
asm(".byte 0x86, 0x30");        //  **    *    **  
asm(".byte 0x86, 0x30");        //  **    *    **  
asm(".byte 0x86, 0x30");        //  **    *    **  
asm(".byte 0x86, 0x30");        //  **    *    **  
asm(".byte 0x86, 0x30");        //  **    *    **  
asm(".byte 0x86, 0x30");        //  **    *    **  
asm(".byte 0x86, 0x30");        //  **    *    **  
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 110
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF2, 0x00");        //  *  ****        
asm(".byte 0xCE, 0x01");        //  ***  ***       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 111
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x82, 0x01");        //  *     **       
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 112
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF2, 0x00");        //  *  ****        
asm(".byte 0x8E, 0x01");        //  ***   **       
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x8E, 0x01");        //  ***   **       
asm(".byte 0xF6, 0x00");        //  ** ****        
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 113
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x3C, 0x01");        //   ****  *       
asm(".byte 0xC6, 0x01");        //  **   ***       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0xC6, 0x01");        //  **   ***       
asm(".byte 0xBC, 0x01");        //   **** **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x80, 0x01");        //        **       
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 114
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x32");        //  *  **  
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 115
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x3C, 0x00");        //   ****          
asm(".byte 0x66, 0x00");        //  **  **         
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x60, 0x00");        //      **         
asm(".byte 0x62, 0x00");        //  *   **         
asm(".byte 0x3E, 0x00");        //  *****          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 116
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x3F");        // ******  
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x3C");        //   ****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 117
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0xCE, 0x01");        //  ***  ***       
asm(".byte 0x3C, 0x01");        //   ****  *       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 118
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x82, 0x00");        //  *     *        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x44, 0x00");        //   *   *         
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x28, 0x00");        //    * *          
asm(".byte 0x28, 0x00");        //    * *          
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 119
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x63, 0x08");        // **   **    *    
asm(".byte 0x63, 0x0C");        // **   **   **    
asm(".byte 0xE3, 0x0C");        // **   ***  **    
asm(".byte 0xF2, 0x04");        //  *  ****  *     
asm(".byte 0x92, 0x04");        //  *  *  *  *     
asm(".byte 0x96, 0x06");        //  ** *  * **     
asm(".byte 0x9E, 0x07");        //  ****  ****     
asm(".byte 0x9E, 0x07");        //  ****  ****     
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x0C, 0x03");        //   **    **      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 120
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xC3, 0x00");        // **    **        
asm(".byte 0x66, 0x00");        //  **  **         
asm(".byte 0x24, 0x00");        //   *  *          
asm(".byte 0x3C, 0x00");        //   ****          
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x3C, 0x00");        //   ****          
asm(".byte 0x24, 0x00");        //   *  *          
asm(".byte 0x66, 0x00");        //  **  **         
asm(".byte 0xC3, 0x00");        // **    **        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 121
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x82, 0x00");        //  *     *        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC4, 0x00");        //   *   **        
asm(".byte 0x4C, 0x00");        //   **  *         
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x28, 0x00");        //    * *          
asm(".byte 0x28, 0x00");        //    * *          
asm(".byte 0x30, 0x00");        //     **          
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x18, 0x00");        //    **           
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x07, 0x00");        // ***             
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 122
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x7F");        // ******* 
asm(".byte 0x60");        //      ** 
asm(".byte 0x30");        //     **  
asm(".byte 0x10");        //     *   
asm(".byte 0x18");        //    **   
asm(".byte 0x0C");        //   **    
asm(".byte 0x04");        //   *     
asm(".byte 0x06");        //  **     
asm(".byte 0x03");        // **      
asm(".byte 0x7F");        // ******* 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 123
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x30");        //     **  
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x08");        //    *    
asm(".byte 0x06");        //  **     
asm(".byte 0x0C");        //   **    
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x30");        //     **  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 124
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 125
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x04");        //   *     
asm(".byte 0x18");        //    **   
asm(".byte 0x0C");        //   **    
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x03");        // **      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 126
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x1C, 0x00");        //   ***           
asm(".byte 0x32, 0x01");        //  *  **  *       
asm(".byte 0xE0, 0x00");        //      ***        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

asm(".section .const, psv, page");
/*********************************
 * TTF Font File Structure
 * Label: DroidSans_Bold_14
 * Description:  Height: 17 pixels, 1 bit per pixel, Range: ' ' to '~'
 ***********************************/
#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
extern const char __DroidSans_Bold_14[] __attribute__((space(prog), aligned(2)));
#else
extern const char __DroidSans_Bold_14[] __attribute__((aligned(2)));
#endif

const FONT_FLASH DroidSans_Bold_14 =
{
    (FLASH | COMP_NONE),
    (GFX_FONT_SPACE char *)__DroidSans_Bold_14
};

#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
asm(".section .text, code");
#else
asm(".section .const, psv, page");
#endif
asm(".global ___DroidSans_Bold_14");
asm(".align 2");
asm("___DroidSans_Bold_14:");
/****************************************
 * Font header
 ****************************************/
asm(".byte 0x00");           // Font ID
asm(".byte 0x00");           // Font information:  1 bit per pixel, Characters zero degress rotation
asm(".byte 0x20, 0x00");     // First Character
asm(".byte 0x7E, 0x00");     // Last Character
asm(".byte 0x11, 0x00");     // Height
/****************************************
 * Font Glyph Table
 ****************************************/
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x84, 0x01, 0x00");       // Character - 32, offset (0x00000184)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x95, 0x01, 0x00");       // Character - 33, offset (0x00000195)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xA6, 0x01, 0x00");       // Character - 34, offset (0x000001A6)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0xB7, 0x01, 0x00");       // Character - 35, offset (0x000001B7)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xD9, 0x01, 0x00");       // Character - 36, offset (0x000001D9)
asm(".byte 0x0C");              // width of the glyph
asm(".byte 0xEA, 0x01, 0x00");       // Character - 37, offset (0x000001EA)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x0C, 0x02, 0x00");       // Character - 38, offset (0x0000020C)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x2E, 0x02, 0x00");       // Character - 39, offset (0x0000022E)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x3F, 0x02, 0x00");       // Character - 40, offset (0x0000023F)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x50, 0x02, 0x00");       // Character - 41, offset (0x00000250)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x61, 0x02, 0x00");       // Character - 42, offset (0x00000261)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x72, 0x02, 0x00");       // Character - 43, offset (0x00000272)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x83, 0x02, 0x00");       // Character - 44, offset (0x00000283)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x94, 0x02, 0x00");       // Character - 45, offset (0x00000294)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0xA5, 0x02, 0x00");       // Character - 46, offset (0x000002A5)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xB6, 0x02, 0x00");       // Character - 47, offset (0x000002B6)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xC7, 0x02, 0x00");       // Character - 48, offset (0x000002C7)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xD8, 0x02, 0x00");       // Character - 49, offset (0x000002D8)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xE9, 0x02, 0x00");       // Character - 50, offset (0x000002E9)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xFA, 0x02, 0x00");       // Character - 51, offset (0x000002FA)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x0B, 0x03, 0x00");       // Character - 52, offset (0x0000030B)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x1C, 0x03, 0x00");       // Character - 53, offset (0x0000031C)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x2D, 0x03, 0x00");       // Character - 54, offset (0x0000032D)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x3E, 0x03, 0x00");       // Character - 55, offset (0x0000033E)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x4F, 0x03, 0x00");       // Character - 56, offset (0x0000034F)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x60, 0x03, 0x00");       // Character - 57, offset (0x00000360)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x71, 0x03, 0x00");       // Character - 58, offset (0x00000371)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x82, 0x03, 0x00");       // Character - 59, offset (0x00000382)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x93, 0x03, 0x00");       // Character - 60, offset (0x00000393)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xA4, 0x03, 0x00");       // Character - 61, offset (0x000003A4)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xB5, 0x03, 0x00");       // Character - 62, offset (0x000003B5)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xC6, 0x03, 0x00");       // Character - 63, offset (0x000003C6)
asm(".byte 0x0C");              // width of the glyph
asm(".byte 0xD7, 0x03, 0x00");       // Character - 64, offset (0x000003D7)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0xF9, 0x03, 0x00");       // Character - 65, offset (0x000003F9)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x1B, 0x04, 0x00");       // Character - 66, offset (0x0000041B)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x3D, 0x04, 0x00");       // Character - 67, offset (0x0000043D)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x5F, 0x04, 0x00");       // Character - 68, offset (0x0000045F)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x81, 0x04, 0x00");       // Character - 69, offset (0x00000481)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x92, 0x04, 0x00");       // Character - 70, offset (0x00000492)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xA3, 0x04, 0x00");       // Character - 71, offset (0x000004A3)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xC5, 0x04, 0x00");       // Character - 72, offset (0x000004C5)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xE7, 0x04, 0x00");       // Character - 73, offset (0x000004E7)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xF8, 0x04, 0x00");       // Character - 74, offset (0x000004F8)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x09, 0x05, 0x00");       // Character - 75, offset (0x00000509)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x2B, 0x05, 0x00");       // Character - 76, offset (0x0000052B)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x3C, 0x05, 0x00");       // Character - 77, offset (0x0000053C)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x5E, 0x05, 0x00");       // Character - 78, offset (0x0000055E)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0x80, 0x05, 0x00");       // Character - 79, offset (0x00000580)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0xA2, 0x05, 0x00");       // Character - 80, offset (0x000005A2)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0xC4, 0x05, 0x00");       // Character - 81, offset (0x000005C4)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0xE6, 0x05, 0x00");       // Character - 82, offset (0x000005E6)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x08, 0x06, 0x00");       // Character - 83, offset (0x00000608)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x19, 0x06, 0x00");       // Character - 84, offset (0x00000619)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0x2A, 0x06, 0x00");       // Character - 85, offset (0x0000062A)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x4C, 0x06, 0x00");       // Character - 86, offset (0x0000064C)
asm(".byte 0x0D");              // width of the glyph
asm(".byte 0x6E, 0x06, 0x00");       // Character - 87, offset (0x0000066E)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x90, 0x06, 0x00");       // Character - 88, offset (0x00000690)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xB2, 0x06, 0x00");       // Character - 89, offset (0x000006B2)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xC3, 0x06, 0x00");       // Character - 90, offset (0x000006C3)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xD4, 0x06, 0x00");       // Character - 91, offset (0x000006D4)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xE5, 0x06, 0x00");       // Character - 92, offset (0x000006E5)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xF6, 0x06, 0x00");       // Character - 93, offset (0x000006F6)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x07, 0x07, 0x00");       // Character - 94, offset (0x00000707)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x18, 0x07, 0x00");       // Character - 95, offset (0x00000718)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x29, 0x07, 0x00");       // Character - 96, offset (0x00000729)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x3A, 0x07, 0x00");       // Character - 97, offset (0x0000073A)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x4B, 0x07, 0x00");       // Character - 98, offset (0x0000074B)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x6D, 0x07, 0x00");       // Character - 99, offset (0x0000076D)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x7E, 0x07, 0x00");       // Character - 100, offset (0x0000077E)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xA0, 0x07, 0x00");       // Character - 101, offset (0x000007A0)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xB1, 0x07, 0x00");       // Character - 102, offset (0x000007B1)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xC2, 0x07, 0x00");       // Character - 103, offset (0x000007C2)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0xD3, 0x07, 0x00");       // Character - 104, offset (0x000007D3)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0xF5, 0x07, 0x00");       // Character - 105, offset (0x000007F5)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x06, 0x08, 0x00");       // Character - 106, offset (0x00000806)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x17, 0x08, 0x00");       // Character - 107, offset (0x00000817)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x28, 0x08, 0x00");       // Character - 108, offset (0x00000828)
asm(".byte 0x0E");              // width of the glyph
asm(".byte 0x39, 0x08, 0x00");       // Character - 109, offset (0x00000839)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x5B, 0x08, 0x00");       // Character - 110, offset (0x0000085B)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x7D, 0x08, 0x00");       // Character - 111, offset (0x0000087D)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x9F, 0x08, 0x00");       // Character - 112, offset (0x0000089F)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0xC1, 0x08, 0x00");       // Character - 113, offset (0x000008C1)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xE3, 0x08, 0x00");       // Character - 114, offset (0x000008E3)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xF4, 0x08, 0x00");       // Character - 115, offset (0x000008F4)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x05, 0x09, 0x00");       // Character - 116, offset (0x00000905)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x16, 0x09, 0x00");       // Character - 117, offset (0x00000916)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x38, 0x09, 0x00");       // Character - 118, offset (0x00000938)
asm(".byte 0x0B");              // width of the glyph
asm(".byte 0x49, 0x09, 0x00");       // Character - 119, offset (0x00000949)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x6B, 0x09, 0x00");       // Character - 120, offset (0x0000096B)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x7C, 0x09, 0x00");       // Character - 121, offset (0x0000097C)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x8D, 0x09, 0x00");       // Character - 122, offset (0x0000098D)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x9E, 0x09, 0x00");       // Character - 123, offset (0x0000099E)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xAF, 0x09, 0x00");       // Character - 124, offset (0x000009AF)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xC0, 0x09, 0x00");       // Character - 125, offset (0x000009C0)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xD1, 0x09, 0x00");       // Character - 126, offset (0x000009D1)
/***********************************
 * Font Characters
 ***********************************/
/***********************************
 * Character - 32
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 33
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 34
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x36");        //  ** **  
asm(".byte 0x36");        //  ** **  
asm(".byte 0x36");        //  ** **  
asm(".byte 0x36");        //  ** **  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 35
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x68, 0x00");        //    * **         
asm(".byte 0xFF, 0x00");        // ********        
asm(".byte 0xFF, 0x00");        // ********        
asm(".byte 0x2C, 0x00");        //   ** *          
asm(".byte 0x34, 0x00");        //   * **          
asm(".byte 0xFF, 0x00");        // ********        
asm(".byte 0xFF, 0x00");        // ********        
asm(".byte 0x16, 0x00");        //  ** *           
asm(".byte 0x1E, 0x00");        //  ****           
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 36
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x3E");        //  *****  
asm(".byte 0x3F");        // ******  
asm(".byte 0x0B");        // ** *    
asm(".byte 0x1F");        // *****   
asm(".byte 0x7C");        //   ***** 
asm(".byte 0x68");        //    * ** 
asm(".byte 0x7F");        // ******* 
asm(".byte 0x3F");        // ******  
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 37
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0E, 0x01");        //  ***    *       
asm(".byte 0x9B, 0x00");        // ** **  *        
asm(".byte 0xDB, 0x00");        // ** ** **        
asm(".byte 0x5B, 0x00");        // ** ** *         
asm(".byte 0xFB, 0x03");        // ** *******      
asm(".byte 0xEE, 0x06");        //  *** *** **     
asm(".byte 0xD0, 0x06");        //     * ** **     
asm(".byte 0xD8, 0x06");        //    ** ** **     
asm(".byte 0xC8, 0x06");        //    *  ** **     
asm(".byte 0x84, 0x03");        //   *    ***      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 38
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x3C, 0x03");        //   ****  **      
asm(".byte 0xF6, 0x01");        //  ** *****       
asm(".byte 0xE6, 0x01");        //  **  ****       
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0xFC, 0x03");        //   ********      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 39
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 40
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x18");        //    **   
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x18");        //    **   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 41
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x06");        //  **     
asm(".byte 0x04");        //   *     
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x04");        //   *     
asm(".byte 0x06");        //  **     
asm(".byte 0x03");        // **      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 42
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x3C");        //   ****  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x24");        //   *  *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 43
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x7F");        // ******* 
asm(".byte 0x7F");        // ******* 
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 44
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x02");        //  *      
asm(".byte 0x03");        // **      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 45
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0F");        // ****    
asm(".byte 0x0F");        // ****    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 46
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 47
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x30");        //     **  
asm(".byte 0x30");        //     **  
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x03");        // **      
asm(".byte 0x03");        // **      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 48
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x3E");        //  *****  
asm(".byte 0x63");        // **   ** 
asm(".byte 0x63");        // **   ** 
asm(".byte 0x63");        // **   ** 
asm(".byte 0x63");        // **   ** 
asm(".byte 0x63");        // **   ** 
asm(".byte 0x63");        // **   ** 
asm(".byte 0x3E");        //  *****  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 49
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x18");        //    **   
asm(".byte 0x1E");        //  ****   
asm(".byte 0x1F");        // *****   
asm(".byte 0x1A");        //  * **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 50
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x7F");        // ******* 
asm(".byte 0x62");        //  *   ** 
asm(".byte 0x60");        //      ** 
asm(".byte 0x30");        //     **  
asm(".byte 0x38");        //    ***  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x06");        //  **     
asm(".byte 0x7F");        // ******* 
asm(".byte 0x7F");        // ******* 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 51
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x7F");        // ******* 
asm(".byte 0x60");        //      ** 
asm(".byte 0x60");        //      ** 
asm(".byte 0x1C");        //   ***   
asm(".byte 0x78");        //    **** 
asm(".byte 0x60");        //      ** 
asm(".byte 0x60");        //      ** 
asm(".byte 0x7F");        // ******* 
asm(".byte 0x1F");        // *****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 52
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x30");        //     **  
asm(".byte 0x38");        //    ***  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x36");        //  ** **  
asm(".byte 0x33");        // **  **  
asm(".byte 0x7F");        // ******* 
asm(".byte 0x7F");        // ******* 
asm(".byte 0x30");        //     **  
asm(".byte 0x30");        //     **  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 53
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x3E");        //  *****  
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x60");        //      ** 
asm(".byte 0x60");        //      ** 
asm(".byte 0x7F");        // ******* 
asm(".byte 0x1F");        // *****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 54
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x06");        //  **     
asm(".byte 0x03");        // **      
asm(".byte 0x3F");        // ******  
asm(".byte 0x7F");        // ******* 
asm(".byte 0x63");        // **   ** 
asm(".byte 0x63");        // **   ** 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x3C");        //   ****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 55
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x7F");        // ******* 
asm(".byte 0x7F");        // ******* 
asm(".byte 0x30");        //     **  
asm(".byte 0x30");        //     **  
asm(".byte 0x38");        //    ***  
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 56
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x7F");        // ******* 
asm(".byte 0x63");        // **   ** 
asm(".byte 0x77");        // *** *** 
asm(".byte 0x3E");        //  *****  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x73");        // **  *** 
asm(".byte 0x63");        // **   ** 
asm(".byte 0x7F");        // ******* 
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 57
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x3F");        // ******  
asm(".byte 0x63");        // **   ** 
asm(".byte 0x63");        // **   ** 
asm(".byte 0x7F");        // ******* 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x60");        //      ** 
asm(".byte 0x30");        //     **  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 58
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 59
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x02");        //  *      
asm(".byte 0x03");        // **      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 60
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x40");        //       * 
asm(".byte 0x70");        //     *** 
asm(".byte 0x1C");        //   ***   
asm(".byte 0x06");        //  **     
asm(".byte 0x1C");        //   ***   
asm(".byte 0x70");        //     *** 
asm(".byte 0x40");        //       * 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 61
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x00");        //         
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 62
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x0E");        //  ***    
asm(".byte 0x38");        //    ***  
asm(".byte 0x60");        //      ** 
asm(".byte 0x38");        //    ***  
asm(".byte 0x0E");        //  ***    
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 63
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0E");        //  ***    
asm(".byte 0x1F");        // *****   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x0C");        //   **    
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 64
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x18, 0x03");        //    **   **      
asm(".byte 0x0C, 0x06");        //   **     **     
asm(".byte 0xE6, 0x06");        //  **  *** **     
asm(".byte 0xB6, 0x06");        //  ** ** * **     
asm(".byte 0xB6, 0x06");        //  ** ** * **     
asm(".byte 0xF6, 0x06");        //  ** **** **     
asm(".byte 0xE6, 0x03");        //  **  *****      
asm(".byte 0x04, 0x00");        //   *             
asm(".byte 0x1C, 0x01");        //   ***   *       
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 65
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x28, 0x00");        //    * *          
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 66
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 67
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x0E, 0x00");        //  ***            
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 68
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x3E, 0x00");        //  *****          
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xC6, 0x01");        //  **   ***       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0x3E, 0x00");        //  *****          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 69
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 70
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 71
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF0, 0x01");        //     *****       
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0x0C, 0x00");        //   **            
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0xE6, 0x01");        //  **  ****       
asm(".byte 0xE6, 0x01");        //  **  ****       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x8E, 0x01");        //  ***   **       
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0xF8, 0x01");        //    ******       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 72
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0xFE, 0x01");        //  ********       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 73
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x1E");        //  ****   
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x1E");        //  ****   
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 74
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x1F");        // *****   
asm(".byte 0x0F");        // ****    
asm(".byte 0x00");        //         

/***********************************
 * Character - 75
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x66, 0x00");        //  **  **         
asm(".byte 0x36, 0x00");        //  ** **          
asm(".byte 0x1E, 0x00");        //  ****           
asm(".byte 0x3E, 0x00");        //  *****          
asm(".byte 0x76, 0x00");        //  ** ***         
asm(".byte 0x66, 0x00");        //  **  **         
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 76
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 77
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x0E, 0x0E");        //  ***     ***    
asm(".byte 0x0E, 0x0E");        //  ***     ***    
asm(".byte 0x1E, 0x0F");        //  ****   ****    
asm(".byte 0x1E, 0x0F");        //  ****   ****    
asm(".byte 0x16, 0x0D");        //  ** *   * **    
asm(".byte 0xB6, 0x0D");        //  ** ** ** **    
asm(".byte 0xB6, 0x0D");        //  ** ** ** **    
asm(".byte 0xA6, 0x0C");        //  **  * *  **    
asm(".byte 0xE6, 0x0C");        //  **  ***  **    
asm(".byte 0x66, 0x0C");        //  **  **   **    
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 78
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x8E, 0x01");        //  ***   **       
asm(".byte 0x9E, 0x01");        //  ****  **       
asm(".byte 0x9E, 0x01");        //  ****  **       
asm(".byte 0xB6, 0x01");        //  ** ** **       
asm(".byte 0xB6, 0x01");        //  ** ** **       
asm(".byte 0xE6, 0x01");        //  **  ****       
asm(".byte 0xC6, 0x01");        //  **   ***       
asm(".byte 0xC6, 0x01");        //  **   ***       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 79
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0x8E, 0x03");        //  ***   ***      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x8E, 0x03");        //  ***   ***      
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 80
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0x3E, 0x00");        //  *****          
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 81
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0x8E, 0x03");        //  ***   ***      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x06, 0x03");        //  **     **      
asm(".byte 0x8E, 0x03");        //  ***   ***      
asm(".byte 0xFC, 0x01");        //   *******       
asm(".byte 0xF8, 0x00");        //    *****        
asm(".byte 0xC0, 0x01");        //       ***       
asm(".byte 0x80, 0x03");        //        ***      
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 82
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0x3E, 0x00");        //  *****          
asm(".byte 0x66, 0x00");        //  **  **         
asm(".byte 0xE6, 0x00");        //  **  ***        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 83
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x1E");        //  ****   
asm(".byte 0x3C");        //   ****  
asm(".byte 0x30");        //     **  
asm(".byte 0x30");        //     **  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 84
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0xFF");        // ********
asm(".byte 0xFF");        // ********
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 85
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0x86, 0x01");        //  **    **       
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 86
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 87
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x43, 0x18");        // **    *    **   
asm(".byte 0xE3, 0x18");        // **   ***   **   
asm(".byte 0xE3, 0x18");        // **   ***   **   
asm(".byte 0xA6, 0x0C");        //  **  * *  **    
asm(".byte 0xB6, 0x0D");        //  ** ** ** **    
asm(".byte 0xB6, 0x0D");        //  ** ** ** **    
asm(".byte 0xB4, 0x05");        //   * ** ** *     
asm(".byte 0x14, 0x05");        //   * *   * *     
asm(".byte 0x1C, 0x07");        //   ***   ***     
asm(".byte 0x1C, 0x07");        //   ***   ***     
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 88
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xC7, 0x01");        // ***   ***       
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0xEE, 0x00");        //  *** ***        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x83, 0x01");        // **     **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 89
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0xC3");        // **    **
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x3C");        //   ****  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 90
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x7F");        // ******* 
asm(".byte 0x7F");        // ******* 
asm(".byte 0x30");        //     **  
asm(".byte 0x38");        //    ***  
asm(".byte 0x18");        //    **   
asm(".byte 0x0C");        //   **    
asm(".byte 0x0E");        //  ***    
asm(".byte 0x06");        //  **     
asm(".byte 0x7F");        // ******* 
asm(".byte 0x7F");        // ******* 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 91
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0E");        //  ***    
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x0E");        //  ***    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 92
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x03");        // **      
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x30");        //     **  
asm(".byte 0x30");        //     **  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 93
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0E");        //  ***    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0E");        //  ***    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 94
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x1C");        //   ***   
asm(".byte 0x14");        //   * *   
asm(".byte 0x36");        //  ** **  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x63");        // **   ** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 95
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3F");        // ******  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 96
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x18");        //    **   
asm(".byte 0x30");        //     **  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 97
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x7C");        //   ***** 
asm(".byte 0x60");        //      ** 
asm(".byte 0x7C");        //   ***** 
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x6C");        //   ** ** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 98
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x76, 0x00");        //  ** ***         
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0x76, 0x00");        //  ** ***         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 99
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x3E");        //  *****  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 100
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xDC, 0x00");        //   *** **        
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xDC, 0x00");        //   *** **        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 101
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x7C");        //   ***** 
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x7C");        //   ***** 
asm(".byte 0x78");        //    **** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 102
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x1E");        //  ****   
asm(".byte 0x06");        //  **     
asm(".byte 0x0F");        // ****    
asm(".byte 0x0F");        // ****    
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 103
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0xFC");        //   ******
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x3C");        //   ****  
asm(".byte 0x06");        //  **     
asm(".byte 0x7E");        //  ****** 
asm(".byte 0xFE");        //  *******
asm(".byte 0xC6");        //  **   **
asm(".byte 0xE6");        //  **  ***
asm(".byte 0x7C");        //   ***** 
asm(".byte 0x00");        //         

/***********************************
 * Character - 104
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x76, 0x00");        //  ** ***         
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 105
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 106
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x00");        //         
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0F");        // ****    
asm(".byte 0x07");        // ***     
asm(".byte 0x00");        //         

/***********************************
 * Character - 107
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0xE6");        //  **  ***
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x36");        //  ** **  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x3E");        //  *****  
asm(".byte 0x36");        //  ** **  
asm(".byte 0x66");        //  **  ** 
asm(".byte 0xC6");        //  **   **
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 108
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 109
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x76, 0x0E");        //  ** ***  ***    
asm(".byte 0xFE, 0x1F");        //  ************   
asm(".byte 0xC6, 0x18");        //  **   **   **   
asm(".byte 0xC6, 0x18");        //  **   **   **   
asm(".byte 0xC6, 0x18");        //  **   **   **   
asm(".byte 0xC6, 0x18");        //  **   **   **   
asm(".byte 0xC6, 0x18");        //  **   **   **   
asm(".byte 0xC6, 0x18");        //  **   **   **   
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 110
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x76, 0x00");        //  ** ***         
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 111
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 112
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x76, 0x00");        //  ** ***         
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x7E, 0x00");        //  ******         
asm(".byte 0x76, 0x00");        //  ** ***         
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x06, 0x00");        //  **             
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 113
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xDC, 0x00");        //   *** **        
asm(".byte 0xFC, 0x00");        //   ******        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xDC, 0x00");        //   *** **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0xC0, 0x00");        //       **        
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 114
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x36");        //  ** **  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x0E");        //  ***    
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 115
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3C");        //   ****  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x06");        //  **     
asm(".byte 0x1E");        //  ****   
asm(".byte 0x38");        //    ***  
asm(".byte 0x30");        //     **  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 116
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x0C");        //   **    
asm(".byte 0x3E");        //  *****  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x3C");        //   ****  
asm(".byte 0x38");        //    ***  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 117
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xFE, 0x00");        //  *******        
asm(".byte 0xDC, 0x00");        //   *** **        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 118
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0xC3");        // **    **
asm(".byte 0xC3");        // **    **
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x3C");        //   ****  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x18");        //    **   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 119
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x23, 0x06");        // **   *   **     
asm(".byte 0x53, 0x06");        // **  * *  **     
asm(".byte 0x53, 0x02");        // **  * *  *      
asm(".byte 0x56, 0x03");        //  ** * * **      
asm(".byte 0xDE, 0x03");        //  **** ****      
asm(".byte 0xDE, 0x03");        //  **** ****      
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x8C, 0x01");        //   **   **       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 120
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0xE7");        // ***  ***
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x3C");        //   ****  
asm(".byte 0x18");        //    **   
asm(".byte 0x38");        //    ***  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x66");        //  **  ** 
asm(".byte 0xC3");        // **    **
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 121
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0xC3");        // **    **
asm(".byte 0xE7");        // ***  ***
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x7C");        //   ***** 
asm(".byte 0x3C");        //   ****  
asm(".byte 0x3C");        //   ****  
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x0F");        // ****    
asm(".byte 0x07");        // ***     
asm(".byte 0x00");        //         

/***********************************
 * Character - 122
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x3E");        //  *****  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x3E");        //  *****  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 123
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x18");        //    **   
asm(".byte 0x1C");        //   ***   
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x07");        // ***     
asm(".byte 0x0F");        // ****    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x1C");        //   ***   
asm(".byte 0x18");        //    **   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 124
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x18");        //    **   
asm(".byte 0x00");        //         

/***********************************
 * Character - 125
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x07");        // ***     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x1C");        //   ***   
asm(".byte 0x1E");        //  ****   
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x07");        // ***     
asm(".byte 0x03");        // **      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 126
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0C");        //   **    
asm(".byte 0x7E");        //  ****** 
asm(".byte 0x30");        //     **  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

asm(".section .const, psv, page");
/*********************************
 * TTF Font File Structure
 * Label: DroidSansMono_36
 * Description:  Height: 48 pixels, 1 bit per pixel, Range: '+' to '9'
 ***********************************/
#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
extern const char __DroidSansMono_36[] __attribute__((space(prog), aligned(2)));
#else
extern const char __DroidSansMono_36[] __attribute__((aligned(2)));
#endif

const FONT_FLASH DroidSansMono_36 =
{
    (FLASH | COMP_NONE),
    (GFX_FONT_SPACE char *)__DroidSansMono_36
};

#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
asm(".section .text, code");
#else
asm(".section .const, psv, page");
#endif
asm(".global ___DroidSansMono_36");
asm(".align 2");
asm("___DroidSansMono_36:");
/****************************************
 * Font header
 ****************************************/
asm(".byte 0x00");           // Font ID
asm(".byte 0x00");           // Font information:  1 bit per pixel, Characters zero degress rotation
asm(".byte 0x2B, 0x00");     // First Character
asm(".byte 0x39, 0x00");     // Last Character
asm(".byte 0x30, 0x00");     // Height
/****************************************
 * Font Glyph Table
 ****************************************/
asm(".byte 0x16");              // width of the glyph
asm(".byte 0x44, 0x00, 0x00");       // Character - 43, offset (0x00000044)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0xD4, 0x00, 0x00");       // Character - 44, offset (0x000000D4)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0x64, 0x01, 0x00");       // Character - 45, offset (0x00000164)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0xF4, 0x01, 0x00");       // Character - 46, offset (0x000001F4)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0x84, 0x02, 0x00");       // Character - 47, offset (0x00000284)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0x14, 0x03, 0x00");       // Character - 48, offset (0x00000314)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0xA4, 0x03, 0x00");       // Character - 49, offset (0x000003A4)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0x34, 0x04, 0x00");       // Character - 50, offset (0x00000434)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0xC4, 0x04, 0x00");       // Character - 51, offset (0x000004C4)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0x54, 0x05, 0x00");       // Character - 52, offset (0x00000554)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0xE4, 0x05, 0x00");       // Character - 53, offset (0x000005E4)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0x74, 0x06, 0x00");       // Character - 54, offset (0x00000674)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0x04, 0x07, 0x00");       // Character - 55, offset (0x00000704)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0x94, 0x07, 0x00");       // Character - 56, offset (0x00000794)
asm(".byte 0x16");              // width of the glyph
asm(".byte 0x24, 0x08, 0x00");       // Character - 57, offset (0x00000824)
/***********************************
 * Font Characters
 ***********************************/
/***********************************
 * Character - 43
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0xFC, 0xFF, 0x03");        //   ****************      
asm(".byte 0xFC, 0xFF, 0x03");        //   ****************      
asm(".byte 0xFC, 0xFF, 0x03");        //   ****************      
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 44
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x3C, 0x00");        //           ****          
asm(".byte 0x00, 0x3C, 0x00");        //           ****          
asm(".byte 0x00, 0x1E, 0x00");        //          ****           
asm(".byte 0x00, 0x1E, 0x00");        //          ****           
asm(".byte 0x00, 0x1E, 0x00");        //          ****           
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x07, 0x00");        //         ***             
asm(".byte 0x00, 0x07, 0x00");        //         ***             
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 45
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0xE0, 0xFF, 0x01");        //      ************       
asm(".byte 0xE0, 0xFF, 0x01");        //      ************       
asm(".byte 0xE0, 0xFF, 0x01");        //      ************       
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 46
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x1F, 0x00");        //         *****           
asm(".byte 0x00, 0x1F, 0x00");        //         *****           
asm(".byte 0x00, 0x1F, 0x00");        //         *****           
asm(".byte 0x00, 0x1F, 0x00");        //         *****           
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 47
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0xC0, 0x01");        //               ***       
asm(".byte 0x00, 0xC0, 0x01");        //               ***       
asm(".byte 0x00, 0xE0, 0x01");        //              ****       
asm(".byte 0x00, 0xE0, 0x00");        //              ***        
asm(".byte 0x00, 0xE0, 0x00");        //              ***        
asm(".byte 0x00, 0x70, 0x00");        //             ***         
asm(".byte 0x00, 0x70, 0x00");        //             ***         
asm(".byte 0x00, 0x38, 0x00");        //            ***          
asm(".byte 0x00, 0x38, 0x00");        //            ***          
asm(".byte 0x00, 0x3C, 0x00");        //           ****          
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0F, 0x00");        //         ****            
asm(".byte 0x00, 0x07, 0x00");        //         ***             
asm(".byte 0x00, 0x07, 0x00");        //         ***             
asm(".byte 0x80, 0x03, 0x00");        //        ***              
asm(".byte 0x80, 0x03, 0x00");        //        ***              
asm(".byte 0xC0, 0x01, 0x00");        //       ***               
asm(".byte 0xC0, 0x01, 0x00");        //       ***               
asm(".byte 0xE0, 0x01, 0x00");        //      ****               
asm(".byte 0xE0, 0x00, 0x00");        //      ***                
asm(".byte 0xE0, 0x00, 0x00");        //      ***                
asm(".byte 0x70, 0x00, 0x00");        //     ***                 
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 48
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x3F, 0x00");        //         ******          
asm(".byte 0xC0, 0x7F, 0x00");        //       *********         
asm(".byte 0xC0, 0xFF, 0x00");        //       **********        
asm(".byte 0xE0, 0xE1, 0x01");        //      ****    ****       
asm(".byte 0xF0, 0xC0, 0x03");        //     ****      ****      
asm(".byte 0x70, 0x80, 0x03");        //     ***        ***      
asm(".byte 0x70, 0x80, 0x03");        //     ***        ***      
asm(".byte 0x78, 0x80, 0x07");        //    ****        ****     
asm(".byte 0x78, 0x80, 0x07");        //    ****        ****     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x78, 0x80, 0x07");        //    ****        ****     
asm(".byte 0x70, 0x80, 0x07");        //     ***        ****     
asm(".byte 0x70, 0x80, 0x03");        //     ***        ***      
asm(".byte 0x70, 0x80, 0x03");        //     ***        ***      
asm(".byte 0xF0, 0xC0, 0x03");        //     ****      ****      
asm(".byte 0xE0, 0xE1, 0x01");        //      ****    ****       
asm(".byte 0xC0, 0xFF, 0x00");        //       **********        
asm(".byte 0x80, 0x7F, 0x00");        //        ********         
asm(".byte 0x00, 0x3F, 0x00");        //         ******          
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 49
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1F, 0x00");        //         *****           
asm(".byte 0x80, 0x1F, 0x00");        //        ******           
asm(".byte 0xC0, 0x1F, 0x00");        //       *******           
asm(".byte 0xE0, 0x1D, 0x00");        //      **** ***           
asm(".byte 0x70, 0x1C, 0x00");        //     ***   ***           
asm(".byte 0x20, 0x1C, 0x00");        //      *    ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 50
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x80, 0x3F, 0x00");        //        *******          
asm(".byte 0xE0, 0xFF, 0x00");        //      ***********        
asm(".byte 0xF0, 0xFF, 0x01");        //     *************       
asm(".byte 0x70, 0xE0, 0x01");        //     ***      ****       
asm(".byte 0x20, 0xC0, 0x03");        //      *        ****      
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0xC0, 0x03");        //               ****      
asm(".byte 0x00, 0xC0, 0x01");        //               ***       
asm(".byte 0x00, 0xE0, 0x01");        //              ****       
asm(".byte 0x00, 0xE0, 0x00");        //              ***        
asm(".byte 0x00, 0x70, 0x00");        //             ***         
asm(".byte 0x00, 0x78, 0x00");        //            ****         
asm(".byte 0x00, 0x3C, 0x00");        //           ****          
asm(".byte 0x00, 0x1E, 0x00");        //          ****           
asm(".byte 0x00, 0x0F, 0x00");        //         ****            
asm(".byte 0x80, 0x07, 0x00");        //        ****             
asm(".byte 0xC0, 0x03, 0x00");        //       ****              
asm(".byte 0xC0, 0x01, 0x00");        //       ***               
asm(".byte 0xE0, 0x00, 0x00");        //      ***                
asm(".byte 0x70, 0x00, 0x00");        //     ***                 
asm(".byte 0xF8, 0xFF, 0x07");        //    ****************     
asm(".byte 0xF8, 0xFF, 0x07");        //    ****************     
asm(".byte 0xF8, 0xFF, 0x07");        //    ****************     
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 51
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0xC0, 0x3F, 0x00");        //       ********          
asm(".byte 0xF0, 0xFF, 0x00");        //     ************        
asm(".byte 0xFC, 0xFF, 0x01");        //   ***************       
asm(".byte 0x38, 0xE0, 0x01");        //    ***       ****       
asm(".byte 0x00, 0xC0, 0x03");        //               ****      
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0xC0, 0x03");        //               ****      
asm(".byte 0x00, 0xE0, 0x01");        //              ****       
asm(".byte 0x00, 0xF0, 0x00");        //             ****        
asm(".byte 0xC0, 0x3F, 0x00");        //       ********          
asm(".byte 0xC0, 0x3F, 0x00");        //       ********          
asm(".byte 0xC0, 0xFF, 0x00");        //       **********        
asm(".byte 0x00, 0xE0, 0x03");        //              *****      
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0x80, 0x07");        //                ****     
asm(".byte 0x00, 0x00, 0x07");        //                 ***     
asm(".byte 0x00, 0x00, 0x07");        //                 ***     
asm(".byte 0x00, 0x00, 0x07");        //                 ***     
asm(".byte 0x00, 0x80, 0x07");        //                ****     
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x0C, 0xE0, 0x03");        //   **         *****      
asm(".byte 0xFC, 0xFF, 0x01");        //   ***************       
asm(".byte 0xFC, 0xFF, 0x00");        //   **************        
asm(".byte 0xF0, 0x1F, 0x00");        //     *********           
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 52
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0xF0, 0x00");        //             ****        
asm(".byte 0x00, 0xF0, 0x00");        //             ****        
asm(".byte 0x00, 0xF8, 0x00");        //            *****        
asm(".byte 0x00, 0xFC, 0x00");        //           ******        
asm(".byte 0x00, 0xEC, 0x00");        //           ** ***        
asm(".byte 0x00, 0xEE, 0x00");        //          *** ***        
asm(".byte 0x00, 0xE7, 0x00");        //         ***  ***        
asm(".byte 0x00, 0xE7, 0x00");        //         ***  ***        
asm(".byte 0x80, 0xE3, 0x00");        //        ***   ***        
asm(".byte 0xC0, 0xE1, 0x00");        //       ***    ***        
asm(".byte 0xC0, 0xE1, 0x00");        //       ***    ***        
asm(".byte 0xE0, 0xE0, 0x00");        //      ***     ***        
asm(".byte 0x70, 0xE0, 0x00");        //     ***      ***        
asm(".byte 0x78, 0xE0, 0x00");        //    ****      ***        
asm(".byte 0x38, 0xE0, 0x00");        //    ***       ***        
asm(".byte 0x1C, 0xE0, 0x00");        //   ***        ***        
asm(".byte 0x0E, 0xE0, 0x00");        //  ***         ***        
asm(".byte 0xFE, 0xFF, 0x0F");        //  *******************    
asm(".byte 0xFE, 0xFF, 0x0F");        //  *******************    
asm(".byte 0xFE, 0xFF, 0x0F");        //  *******************    
asm(".byte 0x00, 0xE0, 0x00");        //              ***        
asm(".byte 0x00, 0xE0, 0x00");        //              ***        
asm(".byte 0x00, 0xE0, 0x00");        //              ***        
asm(".byte 0x00, 0xE0, 0x00");        //              ***        
asm(".byte 0x00, 0xE0, 0x00");        //              ***        
asm(".byte 0x00, 0xE0, 0x00");        //              ***        
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 53
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0xE0, 0xFF, 0x01");        //      ************       
asm(".byte 0xE0, 0xFF, 0x01");        //      ************       
asm(".byte 0xE0, 0xFF, 0x01");        //      ************       
asm(".byte 0xE0, 0x00, 0x00");        //      ***                
asm(".byte 0xE0, 0x00, 0x00");        //      ***                
asm(".byte 0xE0, 0x00, 0x00");        //      ***                
asm(".byte 0x70, 0x00, 0x00");        //     ***                 
asm(".byte 0x70, 0x00, 0x00");        //     ***                 
asm(".byte 0x70, 0x00, 0x00");        //     ***                 
asm(".byte 0x70, 0x00, 0x00");        //     ***                 
asm(".byte 0xF0, 0x3F, 0x00");        //     **********          
asm(".byte 0xF0, 0xFF, 0x00");        //     ************        
asm(".byte 0xE0, 0xFF, 0x01");        //      ************       
asm(".byte 0x00, 0xE0, 0x03");        //              *****      
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0x80, 0x07");        //                ****     
asm(".byte 0x00, 0x00, 0x07");        //                 ***     
asm(".byte 0x00, 0x00, 0x07");        //                 ***     
asm(".byte 0x00, 0x00, 0x07");        //                 ***     
asm(".byte 0x00, 0x00, 0x07");        //                 ***     
asm(".byte 0x00, 0x80, 0x07");        //                ****     
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x18, 0xE0, 0x03");        //    **        *****      
asm(".byte 0xF8, 0xFF, 0x01");        //    **************       
asm(".byte 0xF8, 0xFF, 0x00");        //    *************        
asm(".byte 0xE0, 0x1F, 0x00");        //      ********           
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 54
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0xFC, 0x01");        //           *******       
asm(".byte 0x00, 0xFF, 0x01");        //         *********       
asm(".byte 0x80, 0xFF, 0x01");        //        **********       
asm(".byte 0xC0, 0x03, 0x00");        //       ****              
asm(".byte 0xE0, 0x01, 0x00");        //      ****               
asm(".byte 0xE0, 0x00, 0x00");        //      ***                
asm(".byte 0xF0, 0x00, 0x00");        //     ****                
asm(".byte 0x70, 0x00, 0x00");        //     ***                 
asm(".byte 0x70, 0x00, 0x00");        //     ***                 
asm(".byte 0x78, 0x00, 0x00");        //    ****                 
asm(".byte 0x78, 0x7E, 0x00");        //    ****  ******         
asm(".byte 0x38, 0xFF, 0x01");        //    ***  *********       
asm(".byte 0xB8, 0xFF, 0x01");        //    *** **********       
asm(".byte 0xF8, 0xC0, 0x03");        //    *****      ****      
asm(".byte 0x78, 0x80, 0x07");        //    ****        ****     
asm(".byte 0x78, 0x00, 0x07");        //    ****         ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x70, 0x00, 0x07");        //     ***         ***     
asm(".byte 0x70, 0x80, 0x07");        //     ***        ****     
asm(".byte 0xF0, 0x80, 0x03");        //     ****       ***      
asm(".byte 0xE0, 0xC1, 0x03");        //      ****     ****      
asm(".byte 0xC0, 0xFF, 0x01");        //       ***********       
asm(".byte 0x80, 0xFF, 0x00");        //        *********        
asm(".byte 0x00, 0x3F, 0x00");        //         ******          
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 55
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0xF8, 0xFF, 0x07");        //    ****************     
asm(".byte 0xF8, 0xFF, 0x07");        //    ****************     
asm(".byte 0xF8, 0xFF, 0x07");        //    ****************     
asm(".byte 0x00, 0x80, 0x07");        //                ****     
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0xC0, 0x01");        //               ***       
asm(".byte 0x00, 0xC0, 0x01");        //               ***       
asm(".byte 0x00, 0xE0, 0x01");        //              ****       
asm(".byte 0x00, 0xE0, 0x00");        //              ***        
asm(".byte 0x00, 0xE0, 0x00");        //              ***        
asm(".byte 0x00, 0x70, 0x00");        //             ***         
asm(".byte 0x00, 0x70, 0x00");        //             ***         
asm(".byte 0x00, 0x78, 0x00");        //            ****         
asm(".byte 0x00, 0x38, 0x00");        //            ***          
asm(".byte 0x00, 0x3C, 0x00");        //           ****          
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x1C, 0x00");        //           ***           
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0E, 0x00");        //          ***            
asm(".byte 0x00, 0x0F, 0x00");        //         ****            
asm(".byte 0x00, 0x07, 0x00");        //         ***             
asm(".byte 0x80, 0x07, 0x00");        //        ****             
asm(".byte 0x80, 0x03, 0x00");        //        ***              
asm(".byte 0x80, 0x03, 0x00");        //        ***              
asm(".byte 0xC0, 0x03, 0x00");        //       ****              
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 56
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x3F, 0x00");        //         ******          
asm(".byte 0xC0, 0xFF, 0x00");        //       **********        
asm(".byte 0xE0, 0xFF, 0x01");        //      ************       
asm(".byte 0xE0, 0xC0, 0x01");        //      ***      ***       
asm(".byte 0x70, 0x80, 0x03");        //     ***        ***      
asm(".byte 0x70, 0x80, 0x03");        //     ***        ***      
asm(".byte 0x70, 0x80, 0x03");        //     ***        ***      
asm(".byte 0x70, 0x80, 0x03");        //     ***        ***      
asm(".byte 0xF0, 0xC0, 0x01");        //     ****      ***       
asm(".byte 0xE0, 0xE1, 0x01");        //      ****    ****       
asm(".byte 0xC0, 0xF3, 0x00");        //       ****  ****        
asm(".byte 0x80, 0x7F, 0x00");        //        ********         
asm(".byte 0x00, 0x3F, 0x00");        //         ******          
asm(".byte 0xC0, 0xFF, 0x00");        //       **********        
asm(".byte 0xE0, 0xF1, 0x01");        //      ****   *****       
asm(".byte 0xF0, 0xC0, 0x03");        //     ****      ****      
asm(".byte 0x70, 0x80, 0x03");        //     ***        ***      
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x78, 0x80, 0x07");        //    ****        ****     
asm(".byte 0xF0, 0xC0, 0x03");        //     ****      ****      
asm(".byte 0xF0, 0xFF, 0x01");        //     *************       
asm(".byte 0xE0, 0xFF, 0x00");        //      ***********        
asm(".byte 0x00, 0x3F, 0x00");        //         ******          
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

/***********************************
 * Character - 57
 ***********************************/
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x3F, 0x00");        //         ******          
asm(".byte 0xC0, 0x7F, 0x00");        //       *********         
asm(".byte 0xE0, 0xFF, 0x00");        //      ***********        
asm(".byte 0xF0, 0xE0, 0x01");        //     ****     ****       
asm(".byte 0x70, 0xC0, 0x03");        //     ***       ****      
asm(".byte 0x78, 0x80, 0x03");        //    ****        ***      
asm(".byte 0x38, 0x80, 0x03");        //    ***         ***      
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x00, 0x07");        //    ***          ***     
asm(".byte 0x38, 0x80, 0x07");        //    ***         ****     
asm(".byte 0x78, 0x80, 0x07");        //    ****        ****     
asm(".byte 0xF0, 0xC0, 0x07");        //     ****      *****     
asm(".byte 0xF0, 0x7F, 0x07");        //     *********** ***     
asm(".byte 0xE0, 0x3F, 0x07");        //      *********  ***     
asm(".byte 0x80, 0x9F, 0x07");        //        ******  ****     
asm(".byte 0x00, 0x80, 0x07");        //                ****     
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0x80, 0x03");        //                ***      
asm(".byte 0x00, 0xC0, 0x03");        //               ****      
asm(".byte 0x00, 0xC0, 0x01");        //               ***       
asm(".byte 0x00, 0xE0, 0x01");        //              ****       
asm(".byte 0x00, 0xF0, 0x00");        //             ****        
asm(".byte 0xE0, 0x7F, 0x00");        //      **********         
asm(".byte 0xE0, 0x3F, 0x00");        //      *********          
asm(".byte 0xE0, 0x0F, 0x00");        //      *******            
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         
asm(".byte 0x00, 0x00, 0x00");        //                         

asm(".section .const, psv, page");
/*****************************************************************************
 * SECTION:  Palettes
 *****************************************************************************/

/*********************************
 * PALETTE Structure
 * Label: RemoteControlPalette
 * Description:  256 Colors
 ***********************************/

extern const PALETTE_ENTRY RemoteControlPalette_Data[];

const PALETTE_FLASH RemoteControlPalette =
{
    (FLASH | COMP_NONE),
    {
        0x0000,        // id
        256             // number of entries
    },
    (void *)RemoteControlPalette_Data       // entry address
};

const PALETTE_ENTRY RemoteControlPalette_Data[] =
{
/***********************************
 * Color Palette Entries
 **********************************/
    { .value = 0x0000},
    { .value = 0x0020},
    { .value = 0x0043},
    { .value = 0x0040},
    { .value = 0x0840},
    { .value = 0x0083},
    { .value = 0x0861},
    { .value = 0x1080},
    { .value = 0x1082},
    { .value = 0x0084},
    { .value = 0x10A2},
    { .value = 0x08A2},
    { .value = 0x10A0},
    { .value = 0x10A3},
    { .value = 0x18A0},
    { .value = 0x10C2},
    { .value = 0x00E7},
    { .value = 0x10C4},
    { .value = 0x20E0},
    { .value = 0x18E4},
    { .value = 0x1903},
    { .value = 0x2103},
    { .value = 0x1924},
    { .value = 0x2124},
    { .value = 0x014A},
    { .value = 0x50C2},
    { .value = 0x2125},
    { .value = 0x3140},
    { .value = 0x2925},
    { .value = 0x7882},
    { .value = 0x1948},
    { .value = 0x2942},
    { .value = 0x2965},
    { .value = 0x3145},
    { .value = 0x018D},
    { .value = 0x3983},
    { .value = 0x3185},
    { .value = 0x3184},
    { .value = 0x31A6},
    { .value = 0x41A0},
    { .value = 0x41C2},
    { .value = 0x39C6},
    { .value = 0x29E8},
    { .value = 0x0231},
    { .value = 0x0A30},
    { .value = 0x3A07},
    { .value = 0xC0E2},
    { .value = 0x3A08},
    { .value = 0x4A03},
    { .value = 0x4207},
    { .value = 0x5221},
    { .value = 0x3A26},
    { .value = 0x5A01},
    { .value = 0x4228},
    { .value = 0x1A6E},
    { .value = 0x0295},
    { .value = 0x4249},
    { .value = 0x6A40},
    { .value = 0x5246},
    { .value = 0x5A80},
    { .value = 0x428A},
    { .value = 0xE924},
    { .value = 0x4A8C},
    { .value = 0x4A89},
    { .value = 0x02F7},
    { .value = 0x62A3},
    { .value = 0x5289},
    { .value = 0x52AB},
    { .value = 0x52AA},
    { .value = 0x52CB},
    { .value = 0x3B0E},
    { .value = 0x5AEB},
    { .value = 0x62CA},
    { .value = 0x037B},
    { .value = 0x3B4D},
    { .value = 0x5B0B},
    { .value = 0x5B0C},
    { .value = 0x630C},
    { .value = 0x7303},
    { .value = 0x8B00},
    { .value = 0x6B0A},
    { .value = 0x6329},
    { .value = 0x7340},
    { .value = 0x632C},
    { .value = 0x2376},
    { .value = 0x03BF},
    { .value = 0x3374},
    { .value = 0x732C},
    { .value = 0x03DF},
    { .value = 0x239F},
    { .value = 0x9B40},
    { .value = 0x03FF},
    { .value = 0x8382},
    { .value = 0x736C},
    { .value = 0x041D},
    { .value = 0x53CB},
    { .value = 0x7B89},
    { .value = 0x6B8D},
    { .value = 0x1BFF},
    { .value = 0x6B8E},
    { .value = 0x043F},
    { .value = 0x6BAC},
    { .value = 0xA381},
    { .value = 0x145C},
    { .value = 0x3C80},
    { .value = 0xF2AA},
    { .value = 0x8BC4},
    { .value = 0x73D1},
    { .value = 0xABC0},
    { .value = 0x8BE9},
    { .value = 0x7BEF},
    { .value = 0x9402},
    { .value = 0x3C5A},
    { .value = 0x840F},
    { .value = 0x8BEE},
    { .value = 0x7431},
    { .value = 0x6451},
    { .value = 0x1CBF},
    { .value = 0x8430},
    { .value = 0x4C98},
    { .value = 0x7C51},
    { .value = 0x9C60},
    { .value = 0x942D},
    { .value = 0xBC20},
    { .value = 0x54B6},
    { .value = 0xAC62},
    { .value = 0x6C92},
    { .value = 0xA464},
    { .value = 0x8472},
    { .value = 0x9C68},
    { .value = 0x54D5},
    { .value = 0x6C96},
    { .value = 0x84AA},
    { .value = 0x9470},
    { .value = 0x946F},
    { .value = 0xA486},
    { .value = 0x8C92},
    { .value = 0x9492},
    { .value = 0x451F},
    { .value = 0x9493},
    { .value = 0xBCA2},
    { .value = 0x94D4},
    { .value = 0xD4C0},
    { .value = 0x9509},
    { .value = 0x7D2F},
    { .value = 0xDCC0},
    { .value = 0xBD04},
    { .value = 0x6D73},
    { .value = 0x94F4},
    { .value = 0xBD07},
    { .value = 0xCD00},
    { .value = 0xA513},
    { .value = 0xC522},
    { .value = 0xFC51},
    { .value = 0xA513},
    { .value = 0xA512},
    { .value = 0xA514},
    { .value = 0x5D7F},
    { .value = 0xC540},
    { .value = 0x9D34},
    { .value = 0x757C},
    { .value = 0x857A},
    { .value = 0xB551},
    { .value = 0xC54B},
    { .value = 0xAD55},
    { .value = 0xA575},
    { .value = 0x9578},
    { .value = 0x9599},
    { .value = 0xBD6E},
    { .value = 0x95ED},
    { .value = 0x6DDF},
    { .value = 0xE581},
    { .value = 0xB5E6},
    { .value = 0xB595},
    { .value = 0x7E65},
    { .value = 0xC5E0},
    { .value = 0xAD97},
    { .value = 0xFD60},
    { .value = 0xB5B6},
    { .value = 0xBDB4},
    { .value = 0xFD80},
    { .value = 0xD5C7},
    { .value = 0x9634},
    { .value = 0xDDE3},
    { .value = 0xB5D8},
    { .value = 0xE620},
    { .value = 0xD5ED},
    { .value = 0xFDE0},
    { .value = 0xBE18},
    { .value = 0x8E5F},
    { .value = 0xA63C},
    { .value = 0xEE22},
    { .value = 0xFD96},
    { .value = 0xC618},
    { .value = 0xDE2B},
    { .value = 0xBE6C},
    { .value = 0xFE21},
    { .value = 0xB63B},
    { .value = 0xC636},
    { .value = 0xBE88},
    { .value = 0xC659},
    { .value = 0xE6A0},
    { .value = 0xD6A5},
    { .value = 0xE651},
    { .value = 0xA6BF},
    { .value = 0xE674},
    { .value = 0xDE96},
    { .value = 0xFEA2},
    { .value = 0xCEBB},
    { .value = 0xCEBA},
    { .value = 0xC6F4},
    { .value = 0xBEF9},
    { .value = 0xD6BA},
    { .value = 0xFEC3},
    { .value = 0xDEB8},
    { .value = 0xFEC6},
    { .value = 0xB6FF},
    { .value = 0xFF00},
    { .value = 0xFEE8},
    { .value = 0xFE7A},
    { .value = 0xDEDB},
    { .value = 0xFF0C},
    { .value = 0xFF40},
    { .value = 0xCF5A},
    { .value = 0xC73F},
    { .value = 0xFF11},
    { .value = 0xFF64},
    { .value = 0xDF1C},
    { .value = 0xCF5D},
    { .value = 0xEF18},
    { .value = 0xFF80},
    { .value = 0xEF3A},
    { .value = 0xE73C},
    { .value = 0xCF7F},
    { .value = 0xEF5D},
    { .value = 0xFF57},
    { .value = 0xFFAA},
    { .value = 0xD77F},
    { .value = 0xEF91},
    { .value = 0xFF55},
    { .value = 0xFF90},
    { .value = 0xFFAD},
    { .value = 0xFF93},
    { .value = 0xEF9E},
    { .value = 0xFF9A},
    { .value = 0xF7BD},
    { .value = 0xF7D8},
    { .value = 0xFFD6},
    { .value = 0xFFDC},
    { .value = 0xFFFF},
    { .value = 0x7E65},
    { .value = 0x3C80},
    { .value = 0xFEA2},
    { .value = 0xAD55},
    { .value = 0x10C2},
    { .value = 0xA000},
};

