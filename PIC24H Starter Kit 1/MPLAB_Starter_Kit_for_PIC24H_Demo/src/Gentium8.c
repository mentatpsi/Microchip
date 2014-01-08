/*****************************************************************************
 * FileName:        Gentium8.c
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
 * Fonts
 * -----
 * GOLFontDefault - Height: 14 pixels, range: '' to ''
 *****************************************************************************/

/*****************************************************************************
 * SECTION:  Fonts
 *****************************************************************************/

/*********************************
 * FNT Font File Structure
 * Label: GOLFontDefault
 * Description:  Height: 14 pixels, range: '' to ''
 ***********************************/
#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
extern const char __GOLFontDefault[] __attribute__((space(prog), aligned(2)));
#else
extern const char __GOLFontDefault[] __attribute__((aligned(2)));
#endif

const FONT_FLASH GOLFontDefault =
{
    (FLASH | COMP_NONE),
    (GFX_FONT_SPACE char *)__GOLFontDefault
};

#ifdef USE_GFX_FONT_IN_PROGRAM_SECTION
asm(".section .text, code");
#else
asm(".section .const, psv, page");
#endif
asm(".global ___GOLFontDefault");
asm(".align 2");
asm("___GOLFontDefault:");
/****************************************
 * Font header
 ****************************************/
asm(".byte 0x00");           // Font ID
asm(".byte 0x00");           // Font information:  1 bit per pixel, Characters zero degress rotation
asm(".byte 0x1C, 0x00");     // First Character
asm(".byte 0x7F, 0x00");     // Last Character
asm(".byte 0x0E, 0x00");     // Height
/****************************************
 * Font Glyph Table
 ****************************************/
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x98, 0x01, 0x00");       // Character - 28, offset (0x00000198)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xA6, 0x01, 0x00");       // Character - 29, offset (0x000001A6)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0xB4, 0x01, 0x00");       // Character - 30, offset (0x000001B4)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0xD0, 0x01, 0x00");       // Character - 31, offset (0x000001D0)
asm(".byte 0x02");              // width of the glyph
asm(".byte 0xEC, 0x01, 0x00");       // Character - 32, offset (0x000001EC)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0xFA, 0x01, 0x00");       // Character - 33, offset (0x000001FA)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x08, 0x02, 0x00");       // Character - 34, offset (0x00000208)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x16, 0x02, 0x00");       // Character - 35, offset (0x00000216)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x24, 0x02, 0x00");       // Character - 36, offset (0x00000224)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0x32, 0x02, 0x00");       // Character - 37, offset (0x00000232)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x40, 0x02, 0x00");       // Character - 38, offset (0x00000240)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x4E, 0x02, 0x00");       // Character - 39, offset (0x0000024E)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x5C, 0x02, 0x00");       // Character - 40, offset (0x0000025C)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x6A, 0x02, 0x00");       // Character - 41, offset (0x0000026A)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x78, 0x02, 0x00");       // Character - 42, offset (0x00000278)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x86, 0x02, 0x00");       // Character - 43, offset (0x00000286)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x94, 0x02, 0x00");       // Character - 44, offset (0x00000294)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0xA2, 0x02, 0x00");       // Character - 45, offset (0x000002A2)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0xB0, 0x02, 0x00");       // Character - 46, offset (0x000002B0)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xBE, 0x02, 0x00");       // Character - 47, offset (0x000002BE)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xCC, 0x02, 0x00");       // Character - 48, offset (0x000002CC)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xDA, 0x02, 0x00");       // Character - 49, offset (0x000002DA)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xE8, 0x02, 0x00");       // Character - 50, offset (0x000002E8)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xF6, 0x02, 0x00");       // Character - 51, offset (0x000002F6)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x04, 0x03, 0x00");       // Character - 52, offset (0x00000304)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x12, 0x03, 0x00");       // Character - 53, offset (0x00000312)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x20, 0x03, 0x00");       // Character - 54, offset (0x00000320)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x2E, 0x03, 0x00");       // Character - 55, offset (0x0000032E)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x3C, 0x03, 0x00");       // Character - 56, offset (0x0000033C)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x4A, 0x03, 0x00");       // Character - 57, offset (0x0000034A)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x58, 0x03, 0x00");       // Character - 58, offset (0x00000358)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x66, 0x03, 0x00");       // Character - 59, offset (0x00000366)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x74, 0x03, 0x00");       // Character - 60, offset (0x00000374)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x82, 0x03, 0x00");       // Character - 61, offset (0x00000382)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x90, 0x03, 0x00");       // Character - 62, offset (0x00000390)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x9E, 0x03, 0x00");       // Character - 63, offset (0x0000039E)
asm(".byte 0x0A");              // width of the glyph
asm(".byte 0xAC, 0x03, 0x00");       // Character - 64, offset (0x000003AC)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xC8, 0x03, 0x00");       // Character - 65, offset (0x000003C8)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xD6, 0x03, 0x00");       // Character - 66, offset (0x000003D6)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xE4, 0x03, 0x00");       // Character - 67, offset (0x000003E4)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xF2, 0x03, 0x00");       // Character - 68, offset (0x000003F2)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x00, 0x04, 0x00");       // Character - 69, offset (0x00000400)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x0E, 0x04, 0x00");       // Character - 70, offset (0x0000040E)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x1C, 0x04, 0x00");       // Character - 71, offset (0x0000041C)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x2A, 0x04, 0x00");       // Character - 72, offset (0x0000042A)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x38, 0x04, 0x00");       // Character - 73, offset (0x00000438)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x46, 0x04, 0x00");       // Character - 74, offset (0x00000446)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x54, 0x04, 0x00");       // Character - 75, offset (0x00000454)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x62, 0x04, 0x00");       // Character - 76, offset (0x00000462)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x70, 0x04, 0x00");       // Character - 77, offset (0x00000470)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x8C, 0x04, 0x00");       // Character - 78, offset (0x0000048C)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x9A, 0x04, 0x00");       // Character - 79, offset (0x0000049A)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xA8, 0x04, 0x00");       // Character - 80, offset (0x000004A8)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xB6, 0x04, 0x00");       // Character - 81, offset (0x000004B6)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xC4, 0x04, 0x00");       // Character - 82, offset (0x000004C4)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xD2, 0x04, 0x00");       // Character - 83, offset (0x000004D2)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xE0, 0x04, 0x00");       // Character - 84, offset (0x000004E0)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xEE, 0x04, 0x00");       // Character - 85, offset (0x000004EE)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0xFC, 0x04, 0x00");       // Character - 86, offset (0x000004FC)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x0A, 0x05, 0x00");       // Character - 87, offset (0x0000050A)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x26, 0x05, 0x00");       // Character - 88, offset (0x00000526)
asm(".byte 0x07");              // width of the glyph
asm(".byte 0x34, 0x05, 0x00");       // Character - 89, offset (0x00000534)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x42, 0x05, 0x00");       // Character - 90, offset (0x00000542)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x50, 0x05, 0x00");       // Character - 91, offset (0x00000550)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x5E, 0x05, 0x00");       // Character - 92, offset (0x0000055E)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x6C, 0x05, 0x00");       // Character - 93, offset (0x0000056C)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x7A, 0x05, 0x00");       // Character - 94, offset (0x0000057A)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x88, 0x05, 0x00");       // Character - 95, offset (0x00000588)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x96, 0x05, 0x00");       // Character - 96, offset (0x00000596)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xA4, 0x05, 0x00");       // Character - 97, offset (0x000005A4)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xB2, 0x05, 0x00");       // Character - 98, offset (0x000005B2)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xC0, 0x05, 0x00");       // Character - 99, offset (0x000005C0)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xCE, 0x05, 0x00");       // Character - 100, offset (0x000005CE)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xDC, 0x05, 0x00");       // Character - 101, offset (0x000005DC)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0xEA, 0x05, 0x00");       // Character - 102, offset (0x000005EA)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xF8, 0x05, 0x00");       // Character - 103, offset (0x000005F8)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x06, 0x06, 0x00");       // Character - 104, offset (0x00000606)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x14, 0x06, 0x00");       // Character - 105, offset (0x00000614)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x22, 0x06, 0x00");       // Character - 106, offset (0x00000622)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x30, 0x06, 0x00");       // Character - 107, offset (0x00000630)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x3E, 0x06, 0x00");       // Character - 108, offset (0x0000063E)
asm(".byte 0x09");              // width of the glyph
asm(".byte 0x4C, 0x06, 0x00");       // Character - 109, offset (0x0000064C)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x68, 0x06, 0x00");       // Character - 110, offset (0x00000668)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x76, 0x06, 0x00");       // Character - 111, offset (0x00000676)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x84, 0x06, 0x00");       // Character - 112, offset (0x00000684)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0x92, 0x06, 0x00");       // Character - 113, offset (0x00000692)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0xA0, 0x06, 0x00");       // Character - 114, offset (0x000006A0)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0xAE, 0x06, 0x00");       // Character - 115, offset (0x000006AE)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0xBC, 0x06, 0x00");       // Character - 116, offset (0x000006BC)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xCA, 0x06, 0x00");       // Character - 117, offset (0x000006CA)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0xD8, 0x06, 0x00");       // Character - 118, offset (0x000006D8)
asm(".byte 0x08");              // width of the glyph
asm(".byte 0xE6, 0x06, 0x00");       // Character - 119, offset (0x000006E6)
asm(".byte 0x06");              // width of the glyph
asm(".byte 0xF4, 0x06, 0x00");       // Character - 120, offset (0x000006F4)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x02, 0x07, 0x00");       // Character - 121, offset (0x00000702)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x10, 0x07, 0x00");       // Character - 122, offset (0x00000710)
asm(".byte 0x04");              // width of the glyph
asm(".byte 0x1E, 0x07, 0x00");       // Character - 123, offset (0x0000071E)
asm(".byte 0x03");              // width of the glyph
asm(".byte 0x2C, 0x07, 0x00");       // Character - 124, offset (0x0000072C)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x3A, 0x07, 0x00");       // Character - 125, offset (0x0000073A)
asm(".byte 0x05");              // width of the glyph
asm(".byte 0x48, 0x07, 0x00");       // Character - 126, offset (0x00000748)
asm(".byte 0x02");              // width of the glyph
asm(".byte 0x56, 0x07, 0x00");       // Character - 127, offset (0x00000756)
/***********************************
 * Font Characters
 ***********************************/
/***********************************
 * Character - 28
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x30");        //     **  
asm(".byte 0x38");        //    ***  
asm(".byte 0x0C");        //   **    
asm(".byte 0x06");        //  **     
asm(".byte 0x02");        //  *      
asm(".byte 0x06");        //  **     
asm(".byte 0x0C");        //   **    
asm(".byte 0x38");        //    ***  
asm(".byte 0x30");        //     **  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 29
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x0E");        //  ***    
asm(".byte 0x18");        //    **   
asm(".byte 0x30");        //     **  
asm(".byte 0x20");        //      *  
asm(".byte 0x30");        //     **  
asm(".byte 0x18");        //    **   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 30
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x44, 0x00");        //   *   *         
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 31
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0x44, 0x00");        //   *   *         
asm(".byte 0x6C, 0x00");        //   ** **         
asm(".byte 0x38, 0x00");        //    ***          
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x10, 0x00");        //     *           
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

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

/***********************************
 * Character - 33
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 34
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
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
asm(".byte 0x14");        //   * *   
asm(".byte 0x0C");        //   **    
asm(".byte 0x1E");        //  ****   
asm(".byte 0x0A");        //  * *    
asm(".byte 0x1E");        //  ****   
asm(".byte 0x07");        // ***     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 36
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0E");        //  ***    
asm(".byte 0x1D");        // * ***   
asm(".byte 0x05");        // * *     
asm(".byte 0x06");        //  **     
asm(".byte 0x1C");        //   ***   
asm(".byte 0x15");        // * * *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x04");        //   *     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 37
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x66");        //  **  ** 
asm(".byte 0x29");        // *  * *  
asm(".byte 0x19");        // *  **   
asm(".byte 0x6E");        //  *** ** 
asm(".byte 0x54");        //   * * * 
asm(".byte 0x54");        //   * * * 
asm(".byte 0x62");        //  *   ** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 38
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x18");        //    **   
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x7C");        //   ***** 
asm(".byte 0x2F");        // **** *  
asm(".byte 0x39");        // *  ***  
asm(".byte 0x11");        // *   *   
asm(".byte 0x6E");        //  *** ** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 39
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
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

/***********************************
 * Character - 40
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 41
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 42
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x04");        //   *     
asm(".byte 0x1F");        // *****   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x05");        // * *     
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
asm(".byte 0x00");        //         
asm(".byte 0x04");        //   *     
asm(".byte 0x1F");        // *****   
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
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
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
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
asm(".byte 0x07");        // ***     
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
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 47
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x10");        //     *   
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x01");        // *       
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 48
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x09");        // *  *    
asm(".byte 0x09");        // *  *    
asm(".byte 0x09");        // *  *    
asm(".byte 0x09");        // *  *    
asm(".byte 0x09");        // *  *    
asm(".byte 0x06");        //  **     
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
asm(".byte 0x04");        //   *     
asm(".byte 0x07");        // ***     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x0E");        //  ***    
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
asm(".byte 0x18");        //    **   
asm(".byte 0x16");        //  ** *   
asm(".byte 0x10");        //     *   
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x12");        //  *  *   
asm(".byte 0x1E");        //  ****   
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
asm(".byte 0x0C");        //   **    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x08");        //    *    
asm(".byte 0x0C");        //   **    
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x0E");        //  ***    
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
asm(".byte 0x08");        //    *    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x09");        // *  *    
asm(".byte 0x1F");        // *****   
asm(".byte 0x08");        //    *    
asm(".byte 0x1C");        //   ***   
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
asm(".byte 0x1C");        //   ***   
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x1E");        //  ****   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x0E");        //  ***    
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
asm(".byte 0x18");        //    **   
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x0E");        //  ***    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x04");        //   *     
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
asm(".byte 0x1E");        //  ****   
asm(".byte 0x0A");        //  * *    
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
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
asm(".byte 0x0F");        // ****    
asm(".byte 0x09");        // *  *    
asm(".byte 0x09");        // *  *    
asm(".byte 0x0F");        // ****    
asm(".byte 0x09");        // *  *    
asm(".byte 0x09");        // *  *    
asm(".byte 0x06");        //  **     
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
asm(".byte 0x04");        //   *     
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0E");        //  ***    
asm(".byte 0x04");        //   *     
asm(".byte 0x03");        // **      
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
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
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
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
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
asm(".byte 0x18");        //    **   
asm(".byte 0x06");        //  **     
asm(".byte 0x03");        // **      
asm(".byte 0x1C");        //   ***   
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
asm(".byte 0x1F");        // *****   
asm(".byte 0x0F");        // ****    
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
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x0C");        //   **    
asm(".byte 0x1C");        //   ***   
asm(".byte 0x03");        // **      
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
asm(".byte 0x06");        //  **     
asm(".byte 0x09");        // *  *    
asm(".byte 0x09");        // *  *    
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
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
asm(".byte 0x78, 0x00");        //    ****         
asm(".byte 0x86, 0x00");        //  **    *        
asm(".byte 0x39, 0x01");        // *  ***  *       
asm(".byte 0x25, 0x01");        // * *  *  *       
asm(".byte 0x25, 0x01");        // * *  *  *       
asm(".byte 0x25, 0x01");        // * *  *  *       
asm(".byte 0xFB, 0x00");        // ** *****        
asm(".byte 0x86, 0x00");        //  **    *        
asm(".byte 0x7C, 0x00");        //   *****         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 65
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x0C");        //   **    
asm(".byte 0x0C");        //   **    
asm(".byte 0x12");        //  *  *   
asm(".byte 0x1E");        //  ****   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x33");        // **  **  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 66
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1F");        // *****   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x1E");        //  ****   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1F");        // *****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 67
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x01");        // *       
asm(".byte 0x01");        // *       
asm(".byte 0x01");        // *       
asm(".byte 0x01");        // *       
asm(".byte 0x13");        // **  *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 68
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1F");        // *****   
asm(".byte 0x32");        //  *  **  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x12");        //  *  *   
asm(".byte 0x1F");        // *****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 69
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1F");        // *****   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x02");        //  *      
asm(".byte 0x0E");        //  ***    
asm(".byte 0x02");        //  *      
asm(".byte 0x12");        //  *  *   
asm(".byte 0x1F");        // *****   
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
asm(".byte 0x1F");        // *****   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x02");        //  *      
asm(".byte 0x0E");        //  ***    
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x07");        // ***     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 71
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x03");        // **      
asm(".byte 0x01");        // *       
asm(".byte 0x39");        // *  ***  
asm(".byte 0x11");        // *   *   
asm(".byte 0x11");        // *   *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 72
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x77");        // *** *** 
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x3E");        //  *****  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x77");        // *** *** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 73
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x07");        // ***     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x07");        // ***     
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
asm(".byte 0x07");        // ***     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x01");        // *       
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 75
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x37");        // *** **  
asm(".byte 0x0A");        //  * *    
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x0A");        //  * *    
asm(".byte 0x12");        //  *  *   
asm(".byte 0x37");        // *** **  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 76
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x07");        // ***     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x12");        //  *  *   
asm(".byte 0x1F");        // *****   
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
asm(".byte 0xC3, 0x00");        // **    **        
asm(".byte 0xC6, 0x00");        //  **   **        
asm(".byte 0xA6, 0x00");        //  **  * *        
asm(".byte 0xAA, 0x00");        //  * * * *        
asm(".byte 0x9A, 0x00");        //  * **  *        
asm(".byte 0x92, 0x00");        //  *  *  *        
asm(".byte 0xD3, 0x00");        // **  * **        
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 78
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x73");        // **  *** 
asm(".byte 0x22");        //  *   *  
asm(".byte 0x26");        //  **  *  
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x2A");        //  * * *  
asm(".byte 0x32");        //  *  **  
asm(".byte 0x27");        // ***  *  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 79
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x33");        // **  **  
asm(".byte 0x21");        // *    *  
asm(".byte 0x21");        // *    *  
asm(".byte 0x21");        // *    *  
asm(".byte 0x13");        // **  *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 80
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0F");        // ****    
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x07");        // ***     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 81
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1C");        //   ***   
asm(".byte 0x33");        // **  **  
asm(".byte 0x21");        // *    *  
asm(".byte 0x21");        // *    *  
asm(".byte 0x21");        // *    *  
asm(".byte 0x13");        // **  *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x10");        //     *   
asm(".byte 0x60");        //      ** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 82
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0F");        // ****    
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x37");        // *** **  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 83
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x02");        //  *      
asm(".byte 0x04");        //   *     
asm(".byte 0x08");        //    *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x06");        //  **     
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
asm(".byte 0x1F");        // *****   
asm(".byte 0x15");        // * * *   
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x0E");        //  ***    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 85
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x77");        // *** *** 
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x22");        //  *   *  
asm(".byte 0x1C");        //   ***   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 86
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x63");        // **   ** 
asm(".byte 0x22");        //  *   *  
asm(".byte 0x12");        //  *  *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 87
 ***********************************/
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x93, 0x01");        // **  *  **       
asm(".byte 0x92, 0x00");        //  *  *  *        
asm(".byte 0xAA, 0x00");        //  * * * *        
asm(".byte 0xAA, 0x00");        //  * * * *        
asm(".byte 0xAA, 0x00");        //  * * * *        
asm(".byte 0x44, 0x00");        //   *   *         
asm(".byte 0x44, 0x00");        //   *   *         
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 88
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x37");        // *** **  
asm(".byte 0x12");        //  *  *   
asm(".byte 0x0C");        //   **    
asm(".byte 0x08");        //    *    
asm(".byte 0x14");        //   * *   
asm(".byte 0x22");        //  *   *  
asm(".byte 0x77");        // *** *** 
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 89
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x63");        // **   ** 
asm(".byte 0x14");        //   * *   
asm(".byte 0x14");        //   * *   
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x1C");        //   ***   
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
asm(".byte 0x1E");        //  ****   
asm(".byte 0x09");        // *  *    
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x12");        //  *  *   
asm(".byte 0x1F");        // *****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 91
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x06");        //  **     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 92
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x01");        // *       
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x08");        //    *    
asm(".byte 0x10");        //     *   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 93
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x07");        // ***     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x07");        // ***     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 94
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x04");        //   *     
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x12");        //  *  *   
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
asm(".byte 0x1F");        // *****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 96
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
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

/***********************************
 * Character - 97
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0E");        //  ***    
asm(".byte 0x09");        // *  *    
asm(".byte 0x0E");        //  ***    
asm(".byte 0x09");        // *  *    
asm(".byte 0x1F");        // *****   
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 98
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x0E");        //  ***    
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x00");        //         
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
asm(".byte 0x0E");        //  ***    
asm(".byte 0x09");        // *  *    
asm(".byte 0x01");        // *       
asm(".byte 0x09");        // *  *    
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 100
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x18");        //    **   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x1E");        //  ****   
asm(".byte 0x11");        // *   *   
asm(".byte 0x11");        // *   *   
asm(".byte 0x11");        // *   *   
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
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
asm(".byte 0x0E");        //  ***    
asm(".byte 0x09");        // *  *    
asm(".byte 0x0F");        // ****    
asm(".byte 0x09");        // *  *    
asm(".byte 0x06");        //  **     
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 102
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x07");        // ***     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x07");        // ***     
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
asm(".byte 0x3E");        //  *****  
asm(".byte 0x11");        // *   *   
asm(".byte 0x11");        // *   *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x02");        //  *      
asm(".byte 0x3D");        // * ****  
asm(".byte 0x21");        // *    *  
asm(".byte 0x1E");        //  ****   
asm(".byte 0x00");        //         

/***********************************
 * Character - 104
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x1A");        //  * **   
asm(".byte 0x16");        //  ** *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x3F");        // ******  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 105
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x07");        // ***     
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
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x01");        // *       
asm(".byte 0x00");        //         

/***********************************
 * Character - 107
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x1A");        //  * **   
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
asm(".byte 0x0A");        //  * *    
asm(".byte 0x37");        // *** **  
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 108
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x03");        // **      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x07");        // ***     
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
asm(".byte 0xDA, 0x00");        //  * ** **        
asm(".byte 0xB7, 0x00");        // *** ** *        
asm(".byte 0x92, 0x00");        //  *  *  *        
asm(".byte 0x92, 0x00");        //  *  *  *        
asm(".byte 0xFF, 0x01");        // *********       
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 
asm(".byte 0x00, 0x00");        //                 

/***********************************
 * Character - 110
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1A");        //  * **   
asm(".byte 0x17");        // *** *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x3F");        // ******  
asm(".byte 0x00");        //         
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
asm(".byte 0x0E");        //  ***    
asm(".byte 0x11");        // *   *   
asm(".byte 0x11");        // *   *   
asm(".byte 0x11");        // *   *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x00");        //         
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
asm(".byte 0x1B");        // ** **   
asm(".byte 0x16");        //  ** *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x0E");        //  ***    
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x07");        // ***     
asm(".byte 0x00");        //         

/***********************************
 * Character - 113
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x1E");        //  ****   
asm(".byte 0x11");        // *   *   
asm(".byte 0x11");        // *   *   
asm(".byte 0x11");        // *   *   
asm(".byte 0x1E");        //  ****   
asm(".byte 0x10");        //     *   
asm(".byte 0x10");        //     *   
asm(".byte 0x38");        //    ***  
asm(".byte 0x00");        //         

/***********************************
 * Character - 114
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0B");        // ** *    
asm(".byte 0x06");        //  **     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x07");        // ***     
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
asm(".byte 0x02");        //  *      
asm(".byte 0x06");        //  **     
asm(".byte 0x04");        //   *     
asm(".byte 0x06");        //  **     
asm(".byte 0x06");        //  **     
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
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x0F");        // ****    
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x0E");        //  ***    
asm(".byte 0x00");        //         
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
asm(".byte 0x1B");        // ** **   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x12");        //  *  *   
asm(".byte 0x3E");        //  *****  
asm(".byte 0x00");        //         
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
asm(".byte 0x13");        // **  *   
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0C");        //   **    
asm(".byte 0x04");        //   *     
asm(".byte 0x00");        //         
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
asm(".byte 0xCB");        // ** *  **
asm(".byte 0x5A");        //  * ** * 
asm(".byte 0x5A");        //  * ** * 
asm(".byte 0x36");        //  ** **  
asm(".byte 0x26");        //  **  *  
asm(".byte 0x00");        //         
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
asm(".byte 0x1B");        // ** **   
asm(".byte 0x0A");        //  * *    
asm(".byte 0x04");        //   *     
asm(".byte 0x0A");        //  * *    
asm(".byte 0x1B");        // ** **   
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
asm(".byte 0x13");        // **  *   
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0C");        //   **    
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x03");        // **      
asm(".byte 0x00");        //         

/***********************************
 * Character - 122
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x0E");        //  ***    
asm(".byte 0x05");        // * *     
asm(".byte 0x04");        //   *     
asm(".byte 0x0A");        //  * *    
asm(".byte 0x0F");        // ****    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 123
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x08");        //    *    
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x08");        //    *    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 124
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 125
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x04");        //   *     
asm(".byte 0x04");        //   *     
asm(".byte 0x02");        //  *      
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
asm(".byte 0x16");        //  ** *   
asm(".byte 0x0D");        // * **    
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

/***********************************
 * Character - 127
 ***********************************/
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x02");        //  *      
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         
asm(".byte 0x00");        //         

asm(".section .const, psv, page");
