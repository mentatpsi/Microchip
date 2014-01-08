/*****************************************************************************
 * FileName:        InternalResourceXC16_1BPPModeIPU.c
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
 * SECTION:  Graphics Bitmap Padding Check
 *****************************************************************************/
#ifdef USE_BITMAP_NO_PADDING_LINE
#error These bitmap resources have been converted with padding of the horizontal lines, but USE_BITMAP_NO_PADDING_LINE is defined in GraphicsConfig.h.
#endif

/*****************************************************************************
 * SECTION: IPU Buffer Size Check
 *****************************************************************************/
#if (GFX_COMPRESSED_BUFFER_SIZE < 378)
#warning The GFX_COMPRESSED_BUFFER_SIZE defined in hardware profile is smaller than the compressed size of the IPU resources.
#endif

#if (GFX_DECOMPRESSED_BUFFER_SIZE < 2530)
#warning The GFX_DECOMPRESSED_BUFFER_SIZE defined in hardware profile is smaller than the decompressed size of the IPU resources.
#endif

/*****************************************************************************
 * Converted Resources
 * -------------------
 *
 * Bitmaps
 * -------
 * SettingsPressedMono1BPP - 81x79 pixels, 1-bits per pixel
 * HomeMono1BPP - 80x79 pixels, 1-bits per pixel
 * HomePressedMono1BPP - 80x79 pixels, 1-bits per pixel
 * SettingsMono1BPP - 81x79 pixels, 1-bits per pixel
 * mchpGraphics - 140x140 pixels, 1-bits per pixel
 * mchp - 140x140 pixels, 1-bits per pixel
 *
 * Palettes
 * --------
 * Screen_1BPP_Palette - 2 Colors
 *
 * Fonts
 * -----
 * DroidSans_Bold_14 - Height: 17 pixels, 1 bit per pixel, Range: ' ' to '~'
 *****************************************************************************/

/*****************************************************************************
 * SECTION:  Fonts
 *****************************************************************************/

/*********************************
 * TTF Font File Structure
 * Label: DroidSans_Bold_14
 * Description:  Height: 17 pixels, 1 bit per pixel, Range: ' ' to '~'
 * Comment Block:
 *  
 *                                  Apache License 
 *                            Version 2.0, January 2004 
 *                         http://www.apache.org/licenses/ 
 *  
 *    TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION 
 *  
 *    1. Definitions. 
 *  
 *       "License" shall mean the terms and conditions for use, reproduction, 
 *       and distribution as defined by Sections 1 through 9 of this document. 
 *  
 *       "Licensor" shall mean the copyright owner or entity authorized by 
 *       the copyright owner that is granting the License. 
 *  
 *       "Legal Entity" shall mean the union of the acting entity and all 
 *       other entities that control, are controlled by, or are under common 
 *       control with that entity. For the purposes of this definition, 
 *       "control" means (i) the power, direct or indirect, to cause the 
 *       direction or management of such entity, whether by contract or 
 *       otherwise, or (ii) ownership of fifty percent (50%) or more of the 
 *       outstanding shares, or (iii) beneficial ownership of such entity. 
 *  
 *       "You" (or "Your") shall mean an individual or Legal Entity 
 *       exercising permissions granted by this License. 
 *  
 *       "Source" form shall mean the preferred form for making modifications, 
 *       including but not limited to software source code, documentation 
 *       source, and configuration files. 
 *  
 *       "Object" form shall mean any form resulting from mechanical 
 *       transformation or translation of a Source form, including but 
 *       not limited to compiled object code, generated documentation, 
 *       and conversions to other media types. 
 *  
 *       "Work" shall mean the work of authorship, whether in Source or 
 *       Object form, made available under the License, as indicated by a 
 *       copyright notice that is included in or attached to the work 
 *       (an example is provided in the Appendix below). 
 *  
 *       "Derivative Works" shall mean any work, whether in Source or Object 
 *       form, that is based on (or derived from) the Work and for which the 
 *       editorial revisions, annotations, elaborations, or other modifications 
 *       represent, as a whole, an original work of authorship. For the purposes 
 *       of this License, Derivative Works shall not include works that remain 
 *       separable from, or merely link (or bind by name) to the interfaces of, 
 *       the Work and Derivative Works thereof. 
 *  
 *       "Contribution" shall mean any work of authorship, including 
 *       the original version of the Work and any modifications or additions 
 *       to that Work or Derivative Works thereof, that is intentionally 
 *       submitted to Licensor for inclusion in the Work by the copyright owner 
 *       or by an individual or Legal Entity authorized to submit on behalf of 
 *       the copyright owner. For the purposes of this definition, "submitted" 
 *       means any form of electronic, verbal, or written communication sent 
 *       to the Licensor or its representatives, including but not limited to 
 *       communication on electronic mailing lists, source code control systems, 
 *       and issue tracking systems that are managed by, or on behalf of, the 
 *       Licensor for the purpose of discussing and improving the Work, but 
 *       excluding communication that is conspicuously marked or otherwise 
 *       designated in writing by the copyright owner as "Not a Contribution." 
 *  
 *       "Contributor" shall mean Licensor and any individual or Legal Entity 
 *       on behalf of whom a Contribution has been received by Licensor and 
 *       subsequently incorporated within the Work. 
 *  
 *    2. Grant of Copyright License. Subject to the terms and conditions of 
 *       this License, each Contributor hereby grants to You a perpetual, 
 *       worldwide, non-exclusive, no-charge, royalty-free, irrevocable 
 *       copyright license to reproduce, prepare Derivative Works of, 
 *       publicly display, publicly perform, sublicense, and distribute the 
 *       Work and such Derivative Works in Source or Object form. 
 *  
 *    3. Grant of Patent License. Subject to the terms and conditions of 
 *       this License, each Contributor hereby grants to You a perpetual, 
 *       worldwide, non-exclusive, no-charge, royalty-free, irrevocable 
 *       (except as stated in this section) patent license to make, have made, 
 *       use, offer to sell, sell, import, and otherwise transfer the Work, 
 *       where such license applies only to those patent claims licensable 
 *       by such Contributor that are necessarily infringed by their 
 *       Contribution(s) alone or by combination of their Contribution(s) 
 *       with the Work to which such Contribution(s) was submitted. If You 
 *       institute patent litigation against any entity (including a 
 *       cross-claim or counterclaim in a lawsuit) alleging that the Work 
 *       or a Contribution incorporated within the Work constitutes direct 
 *       or contributory patent infringement, then any patent licenses 
 *       granted to You under this License for that Work shall terminate 
 *       as of the date such litigation is filed. 
 *  
 *    4. Redistribution. You may reproduce and distribute copies of the 
 *       Work or Derivative Works thereof in any medium, with or without 
 *       modifications, and in Source or Object form, provided that You 
 *       meet the following conditions: 
 *  
 *       (a) You must give any other recipients of the Work or 
 *           Derivative Works a copy of this License; and 
 *  
 *       (b) You must cause any modified files to carry prominent notices 
 *           stating that You changed the files; and 
 *  
 *       (c) You must retain, in the Source form of any Derivative Works 
 *           that You distribute, all copyright, patent, trademark, and 
 *           attribution notices from the Source form of the Work, 
 *           excluding those notices that do not pertain to any part of 
 *           the Derivative Works; and 
 *  
 *       (d) If the Work includes a "NOTICE" text file as part of its 
 *           distribution, then any Derivative Works that You distribute must 
 *           include a readable copy of the attribution notices contained 
 *           within such NOTICE file, excluding those notices that do not 
 *           pertain to any part of the Derivative Works, in at least one 
 *           of the following places: within a NOTICE text file distributed 
 *           as part of the Derivative Works; within the Source form or 
 *           documentation, if provided along with the Derivative Works; or, 
 *           within a display generated by the Derivative Works, if and 
 *           wherever such third-party notices normally appear. The contents 
 *           of the NOTICE file are for informational purposes only and 
 *           do not modify the License. You may add Your own attribution 
 *           notices within Derivative Works that You distribute, alongside 
 *           or as an addendum to the NOTICE text from the Work, provided 
 *           that such additional attribution notices cannot be construed 
 *           as modifying the License. 
 *  
 *       You may add Your own copyright statement to Your modifications and 
 *       may provide additional or different license terms and conditions 
 *       for use, reproduction, or distribution of Your modifications, or 
 *       for any such Derivative Works as a whole, provided Your use, 
 *       reproduction, and distribution of the Work otherwise complies with 
 *       the conditions stated in this License. 
 *  
 *    5. Submission of Contributions. Unless You explicitly state otherwise, 
 *       any Contribution intentionally submitted for inclusion in the Work 
 *       by You to the Licensor shall be under the terms and conditions of 
 *       this License, without any additional terms or conditions. 
 *       Notwithstanding the above, nothing herein shall supersede or modify 
 *       the terms of any separate license agreement you may have executed 
 *       with Licensor regarding such Contributions. 
 *  
 *    6. Trademarks. This License does not grant permission to use the trade 
 *       names, trademarks, service marks, or product names of the Licensor, 
 *       except as required for reasonable and customary use in describing the 
 *       origin of the Work and reproducing the content of the NOTICE file. 
 *  
 *    7. Disclaimer of Warranty. Unless required by applicable law or 
 *       agreed to in writing, Licensor provides the Work (and each 
 *       Contributor provides its Contributions) on an "AS IS" BASIS, 
 *       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
 *       implied, including, without limitation, any warranties or conditions 
 *       of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A 
 *       PARTICULAR PURPOSE. You are solely responsible for determining the 
 *       appropriateness of using or redistributing the Work and assume any 
 *       risks associated with Your exercise of permissions under this License. 
 *  
 *    8. Limitation of Liability. In no event and under no legal theory, 
 *       whether in tort (including negligence), contract, or otherwise, 
 *       unless required by applicable law (such as deliberate and grossly 
 *       negligent acts) or agreed to in writing, shall any Contributor be 
 *       liable to You for damages, including any direct, indirect, special, 
 *       incidental, or consequential damages of any character arising as a 
 *       result of this License or out of the use or inability to use the 
 *       Work (including but not limited to damages for loss of goodwill, 
 *       work stoppage, computer failure or malfunction, or any and all 
 *       other commercial damages or losses), even if such Contributor 
 *       has been advised of the possibility of such damages. 
 *  
 *    9. Accepting Warranty or Additional Liability. While redistributing 
 *       the Work or Derivative Works thereof, You may choose to offer, 
 *       and charge a fee for, acceptance of support, warranty, indemnity, 
 *       or other liability obligations and/or rights consistent with this 
 *       License. However, in accepting such obligations, You may act only 
 *       on Your own behalf and on Your sole responsibility, not on behalf 
 *       of any other Contributor, and only if You agree to indemnify, 
 *       defend, and hold each Contributor harmless for any liability 
 *       incurred by, or claims asserted against, such Contributor by reason 
 *       of your accepting any such warranty or additional liability. 
 *  
 *    END OF TERMS AND CONDITIONS 
 *  
 *    APPENDIX: How to apply the Apache License to your work. 
 *  
 *       To apply the Apache License to your work, attach the following 
 *       boilerplate notice, with the fields enclosed by brackets "[]" 
 *       replaced with your own identifying information. (Don't include 
 *       the brackets!)  The text should be enclosed in the appropriate 
 *       comment syntax for the file format. We also recommend that a 
 *       file or class name and description of purpose be included on the 
 *       same "printed page" as the copyright notice for easier 
 *       identification within third-party archives. 
 *  
 *    Copyright [yyyy] [name of copyright owner] 
 *  
 *    Licensed under the Apache License, Version 2.0 (the "License"); 
 *    you may not use this file except in compliance with the License. 
 *    You may obtain a copy of the License at 
 *  
 *        http://www.apache.org/licenses/LICENSE-2.0 
 *  
 *    Unless required by applicable law or agreed to in writing, software 
 *    distributed under the License is distributed on an "AS IS" BASIS, 
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 *    See the License for the specific language governing permissions and 
 *    limitations under the License. 
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
/*****************************************************************************
 * SECTION:  BITMAPS
 *****************************************************************************/

/*********************************
 * Bitmap Structure
 * Label: SettingsPressedMono1BPP
 * Description:  81x79 pixels, 1-bits per pixel
 ***********************************/
#if defined (__C30__) && (__C30_VERSION__ <= 325)
asm(".section .const, psv");
#else
asm(".section .const, psv, page");
#endif


extern const char __SettingsPressedMono1BPP[] __attribute__((space(prog), aligned(2)));

const GFX_IMAGE_HEADER SettingsPressedMono1BPP =
{
    (FLASH | IMAGE_MBITMAP | COMP_IPU),
    0,
    {    .progByteAddress = (FLASH_BYTE *) __SettingsPressedMono1BPP    },
    81,
    79,
    358,
    879,
    1
};

asm(".section .text, code");
asm(".global ___SettingsPressedMono1BPP");
asm(".align 2");
asm("___SettingsPressedMono1BPP:");
asm(".byte 0x63, 0x60, 0xF4, 0x67, 0x08, 0x64, 0x60, 0x60, 0x58, 0xFC, 0x8F, 0x81, 0xEE, 0xE0, 0x8F, 0x3D");
asm(".byte 0x9C, 0xC9, 0xA2, 0x00, 0x67, 0x7E, 0xD3, 0x47, 0x30, 0xF3, 0x31, 0x99, 0x09, 0x0C, 0x9F, 0xCF");
asm(".byte 0x33, 0xB0, 0x81, 0x99, 0x1F, 0x0E, 0xFE, 0xBE, 0xDF, 0xCE, 0x0F, 0x66, 0x4A, 0xD4, 0xFF, 0x90");
asm(".byte 0xFF, 0x27, 0x01, 0x66, 0xFA, 0xC8, 0xFC, 0xFF, 0x6F, 0x61, 0x04, 0x66, 0x3E, 0x3B, 0xF8, 0xFF");
asm(".byte 0x7F, 0x73, 0x3A, 0x98, 0xF9, 0xF9, 0x3F, 0x10, 0x9C, 0x07, 0xB1, 0x1A, 0x7E, 0x82, 0x98, 0xF3");
asm(".byte 0x19, 0x41, 0xCC, 0xBF, 0x20, 0xA6, 0x3D, 0x88, 0xC9, 0x00, 0x66, 0xEE, 0x07, 0xAB, 0x85, 0x28");
asm(".byte 0x00, 0x33, 0x7F, 0x81, 0x98, 0xF1, 0x60, 0xE6, 0xA7, 0xFF, 0xF6, 0x3F, 0xFE, 0xFB, 0x83, 0x99");
asm(".byte 0x5F, 0xFE, 0xB3, 0x37, 0xFC, 0x87, 0x38, 0xE8, 0xDB, 0xFF, 0x83, 0xEC, 0xFF, 0x20, 0xCC, 0x4F");
asm(".byte 0xF5, 0x0F, 0xEC, 0xFF, 0x40, 0x14, 0xFC, 0xB6, 0x3F, 0xF0, 0xFF, 0xC3, 0x7D, 0x08, 0x53, 0xFE");
asm(".byte 0xC0, 0xFF, 0x8F, 0xF7, 0x21, 0x16, 0xF3, 0x33, 0xFC, 0x7B, 0x0C, 0xB6, 0xF8, 0xC3, 0x5F, 0x76");
asm(".byte 0x86, 0x3F, 0xC7, 0xED, 0x41, 0x6E, 0xAF, 0xF8, 0x03, 0x62, 0xD6, 0xDB, 0x00, 0x99, 0x1C, 0xFF");
asm(".byte 0x99, 0x19, 0x7E, 0xF4, 0xFF, 0x07, 0x05, 0xC5, 0x0B, 0x08, 0x53, 0x1D, 0xCC, 0x4C, 0x06, 0x31");
asm(".byte 0xF5, 0xC1, 0xCC, 0xCF, 0x0C, 0x3F, 0xE6, 0xC3, 0x98, 0x8C, 0x08, 0x26, 0xF3, 0x1F, 0x18, 0xF3");
asm(".byte 0x31, 0xFF, 0x3F, 0x38, 0xF3, 0xFF, 0x7F, 0x88, 0xB6, 0x13, 0xFF, 0x9F, 0x03, 0xFD, 0xF3, 0x5F");
asm(".byte 0x19, 0x14, 0x4E, 0xFF, 0x8E, 0x03, 0x3D, 0x54, 0x0F, 0x0A, 0xA9, 0x07, 0x7F, 0x40, 0x4C, 0x7B");
asm(".byte 0x76, 0x70, 0x38, 0xF4, 0x83, 0x3C, 0x0F, 0x0E, 0x87, 0x9F, 0xFC, 0xF0, 0x70, 0xF8, 0x2D, 0xFF");
asm(".byte 0x0F, 0x16, 0x0E, 0x9F, 0xEC, 0xFF, 0xC0, 0xC2, 0xE1, 0x5B, 0xFD, 0x87, 0xFF, 0xF0, 0x70, 0x60");
asm(".byte 0x7C, 0x00, 0x0F, 0x07, 0xF6, 0x03, 0x50, 0x51, 0x70, 0x40, 0xDD, 0x47, 0x84, 0xFA, 0x7C, 0x44");
asm(".byte 0xA0, 0x82, 0xAD, 0x68, 0xF8, 0x83, 0x08, 0xF5, 0x9F, 0x08, 0x26, 0x92, 0xDA, 0x67, 0x87, 0x81");
asm(".byte 0xF1, 0x06, 0x31, 0xEC, 0x8C, 0x04, 0x30, 0x36, 0xD5, 0x21, 0x71, 0x5C, 0xF6, 0xC7, 0x3E, 0x4F");
asm(".byte 0x00, 0xCC, 0xDC, 0x70, 0xF0, 0xE7, 0xFC, 0x66, 0x4E, 0x68, 0x7A, 0xF8, 0x7D, 0x1F, 0x9A, 0x1E");
asm(".byte 0x80, 0xD1, 0x15, 0x0F, 0x4F, 0x30, 0x9F, 0x10, 0xCC, 0x2F, 0xFE, 0x70, 0x26, 0x52, 0x3A, 0xFB");
asm(".byte 0x81, 0x48, 0x7D, 0x74, 0x04, 0x00");
#if defined (__C30__) && (__C30_VERSION__ <= 325)
asm(".section .const, psv");
#else
asm(".section .const, psv, page");
#endif

/*********************************
 * Bitmap Structure
 * Label: HomeMono1BPP
 * Description:  80x79 pixels, 1-bits per pixel
 ***********************************/
#if defined (__C30__) && (__C30_VERSION__ <= 325)
asm(".section .const, psv");
#else
asm(".section .const, psv, page");
#endif


extern const char __HomeMono1BPP[] __attribute__((space(prog), aligned(2)));

const GFX_IMAGE_HEADER HomeMono1BPP =
{
    (FLASH | IMAGE_MBITMAP | COMP_IPU),
    0,
    {    .progByteAddress = (FLASH_BYTE *) __HomeMono1BPP    },
    80,
    79,
    241,
    800,
    1
};

asm(".section .text, code");
asm(".global ___HomeMono1BPP");
asm(".align 2");
asm("___HomeMono1BPP:");
asm(".byte 0x63, 0x60, 0xF4, 0x67, 0x08, 0x60, 0x60, 0x60, 0x58, 0xFC, 0x8F, 0x81, 0xA6, 0xE0, 0x00, 0x33");
asm(".byte 0x8C, 0x95, 0xC0, 0x06, 0x63, 0x4D, 0xE0, 0x84, 0xB1, 0xCE, 0x18, 0xC3, 0x58, 0xCF, 0xD2, 0x61");
asm(".byte 0xAC, 0xCF, 0xE7, 0xA1, 0x8C, 0x86, 0x9F, 0xF3, 0x19, 0xA1, 0x86, 0xD4, 0xD8, 0x41, 0x8D, 0x49");
asm(".byte 0xB0, 0xAB, 0x81, 0x1A, 0x33, 0x41, 0xFE, 0x07, 0xD4, 0x98, 0x13, 0xFC, 0x1F, 0x84, 0x21, 0xAC");
asm(".byte 0x27, 0xEC, 0x0F, 0xD4, 0x21, 0xAC, 0x4F, 0xCC, 0x07, 0xFC, 0xA1, 0x86, 0x30, 0x36, 0x40, 0x8C");
asm(".byte 0x39, 0xF0, 0x87, 0x81, 0xC1, 0x1E, 0x6C, 0x4C, 0x42, 0x1D, 0x03, 0x43, 0x1D, 0xD8, 0x18, 0x03");
asm(".byte 0x7B, 0x06, 0x86, 0x3F, 0x3C, 0x20, 0xD6, 0x0C, 0x7E, 0x06, 0x86, 0x0F, 0x92, 0x60, 0x37, 0xB1");
asm(".byte 0x33, 0x30, 0x3C, 0x30, 0x86, 0x18, 0x02, 0xD4, 0x05, 0x36, 0xE6, 0x27, 0xD0, 0x84, 0x86, 0xF9");
asm(".byte 0x20, 0xE3, 0xFE, 0x80, 0xB8, 0xF6, 0x40, 0xAE, 0x43, 0x1D, 0x88, 0x55, 0xC7, 0xC4, 0xC0, 0xA0");
asm(".byte 0x60, 0x0F, 0x62, 0xFD, 0x61, 0x01, 0xB9, 0x09, 0xC4, 0x02, 0xB9, 0xEB, 0x04, 0x3F, 0x88, 0x05");
asm(".byte 0x72, 0xD7, 0x13, 0x76, 0x10, 0x0B, 0xE4, 0xAE, 0x6F, 0x60, 0xCB, 0x0F, 0xE4, 0x33, 0x30, 0x7C");
asm(".byte 0x81, 0x38, 0x52, 0x9F, 0x81, 0xA1, 0x06, 0xC2, 0xB2, 0x03, 0xAA, 0x05, 0x3B, 0xB2, 0x01, 0xA4");
asm(".byte 0x0F, 0xC2, 0x02, 0x11, 0x8C, 0xF3, 0xD8, 0x89, 0x63, 0x21, 0x74, 0x20, 0xEB, 0xFD, 0x61, 0x4F");
asm(".byte 0x0C, 0x0B, 0xE8, 0xE2, 0x81, 0x65, 0x31, 0xC3, 0x59, 0x4C, 0x20, 0xD6, 0x01, 0x38, 0xCB, 0x01");
asm(".byte 0xCA, 0x02, 0x86, 0x36, 0x88, 0xB0, 0x81, 0x13, 0x0F, 0xFE, 0x03, 0x01, 0x3B, 0x03, 0x8D, 0x01");
asm(".byte 0x00");
#if defined (__C30__) && (__C30_VERSION__ <= 325)
asm(".section .const, psv");
#else
asm(".section .const, psv, page");
#endif

/*********************************
 * Bitmap Structure
 * Label: HomePressedMono1BPP
 * Description:  80x79 pixels, 1-bits per pixel
 ***********************************/
#if defined (__C30__) && (__C30_VERSION__ <= 325)
asm(".section .const, psv");
#else
asm(".section .const, psv, page");
#endif


extern const char __HomePressedMono1BPP[] __attribute__((space(prog), aligned(2)));

const GFX_IMAGE_HEADER HomePressedMono1BPP =
{
    (FLASH | IMAGE_MBITMAP | COMP_IPU),
    0,
    {    .progByteAddress = (FLASH_BYTE *) __HomePressedMono1BPP    },
    80,
    79,
    224,
    800,
    1
};

asm(".section .text, code");
asm(".global ___HomePressedMono1BPP");
asm(".align 2");
asm("___HomePressedMono1BPP:");
asm(".byte 0x63, 0x60, 0xF4, 0x67, 0x08, 0x60, 0x60, 0x60, 0x58, 0xFC, 0x8F, 0x81, 0xA6, 0xE0, 0x00, 0x33");
asm(".byte 0x8C, 0x95, 0xC0, 0x06, 0x63, 0x4D, 0xE0, 0x84, 0xB1, 0xCE, 0x18, 0xC3, 0x58, 0xCF, 0xD2, 0x61");
asm(".byte 0xAC, 0xCF, 0xE7, 0xA1, 0x8C, 0x86, 0x9F, 0xF3, 0x19, 0xA1, 0x86, 0xFC, 0xB1, 0x87, 0x1A, 0x93");
asm(".byte 0xF0, 0xAF, 0x1E, 0x6A, 0xCC, 0x84, 0xFF, 0xFF, 0xA1, 0xC6, 0x9C, 0xF8, 0xFF, 0x5F, 0x18, 0xC2");
asm(".byte 0x7A, 0xF2, 0xFF, 0xBF, 0x3A, 0x84, 0xF5, 0xE9, 0xFF, 0x7F, 0x7F, 0xA8, 0x21, 0xFF, 0xFF, 0x43");
asm(".byte 0x8C, 0x39, 0xF0, 0xE7, 0xFF, 0x7F, 0x88, 0x31, 0x09, 0xFF, 0xFE, 0xFF, 0x87, 0x18, 0x63, 0xF0");
asm(".byte 0x1F, 0x08, 0x78, 0x40, 0xAC, 0x19, 0x20, 0x96, 0x24, 0xD8, 0x4D, 0x20, 0x96, 0x31, 0xD4, 0x10");
asm(".byte 0xA8, 0x31, 0x3F, 0x41, 0xAC, 0xF9, 0x20, 0xE3, 0xFE, 0x80, 0x58, 0xF6, 0x40, 0x03, 0x1D, 0xFE");
asm(".byte 0x81, 0x58, 0xF5, 0x4C, 0x0C, 0x0C, 0x0A, 0xFF, 0xC1, 0x80, 0x05, 0xEC, 0x26, 0x10, 0xE0, 0x04");
asm(".byte 0xBB, 0x09, 0x04, 0x84, 0xC1, 0x6E, 0x02, 0x01, 0xA0, 0xBB, 0xBE, 0x41, 0x58, 0xF9, 0x0C, 0x0C");
asm(".byte 0x5F, 0x20, 0x2C, 0x7D, 0x06, 0x86, 0x3F, 0x10, 0x96, 0x3D, 0x03, 0xC3, 0x07, 0x08, 0x8B, 0x1F");
asm(".byte 0x68, 0x09, 0x84, 0x05, 0xB2, 0xF7, 0x7F, 0xE2, 0x0F, 0xE2, 0x58, 0x08, 0x1D, 0xC8, 0x7A, 0xD9");
asm(".byte 0x0F, 0x10, 0xC3, 0xFA, 0xFF, 0x7F, 0xD0, 0xB0, 0xFE, 0xA1, 0xB2, 0xEA, 0xA1, 0x2C, 0x60, 0x38");
asm(".byte 0x83, 0x58, 0x7F, 0xA0, 0x41, 0xC5, 0xC0, 0xF0, 0x00, 0xC4, 0x62, 0x67, 0xA0, 0x31, 0x00, 0x00");
#if defined (__C30__) && (__C30_VERSION__ <= 325)
asm(".section .const, psv");
#else
asm(".section .const, psv, page");
#endif

/*********************************
 * Bitmap Structure
 * Label: SettingsMono1BPP
 * Description:  81x79 pixels, 1-bits per pixel
 ***********************************/
#if defined (__C30__) && (__C30_VERSION__ <= 325)
asm(".section .const, psv");
#else
asm(".section .const, psv, page");
#endif


extern const char __SettingsMono1BPP[] __attribute__((space(prog), aligned(2)));

const GFX_IMAGE_HEADER SettingsMono1BPP =
{
    (FLASH | IMAGE_MBITMAP | COMP_IPU),
    0,
    {    .progByteAddress = (FLASH_BYTE *) __SettingsMono1BPP    },
    81,
    79,
    363,
    879,
    1
};

asm(".section .text, code");
asm(".global ___SettingsMono1BPP");
asm(".align 2");
asm("___SettingsMono1BPP:");
asm(".byte 0x63, 0x60, 0xF4, 0x67, 0x08, 0x64, 0x60, 0x60, 0x58, 0xFC, 0x8F, 0x81, 0xEE, 0xE0, 0x8F, 0x3D");
asm(".byte 0x9C, 0xC9, 0xA2, 0x00, 0x67, 0xB2, 0x21, 0x31, 0x13, 0x30, 0x99, 0x09, 0x0C, 0xCC, 0x07, 0x18");
asm(".byte 0xD8, 0xC0, 0xCC, 0x0F, 0x07, 0x99, 0x0F, 0xB4, 0xF3, 0x83, 0x99, 0x12, 0xF5, 0x0C, 0x0C, 0xFF");
asm(".byte 0x24, 0xC0, 0x4C, 0x1E, 0x19, 0x06, 0x06, 0x0B, 0x03, 0x88, 0x26, 0x88, 0x06, 0x10, 0x60, 0x06");
asm(".byte 0x11, 0x07, 0x40, 0x44, 0x03, 0x23, 0x9C, 0x04, 0x12, 0x3F, 0x80, 0x1A, 0xC1, 0x02, 0x8C, 0x0C");
asm(".byte 0xFF, 0xFF, 0x33, 0x36, 0x80, 0xD5, 0x32, 0x36, 0xFC, 0xFF, 0xCF, 0x0E, 0x61, 0x32, 0x3D, 0xF8");
asm(".byte 0xFF, 0x9F, 0xDF, 0x01, 0xC2, 0xFC, 0x60, 0xFF, 0xC3, 0x1E, 0xC2, 0x64, 0xF9, 0xC1, 0xDE, 0x60");
asm(".byte 0x0F, 0x31, 0x97, 0xED, 0xCF, 0x41, 0xF6, 0x3A, 0x08, 0x93, 0xA9, 0xE6, 0x81, 0xFD, 0x1F, 0x88");
asm(".byte 0x02, 0x66, 0x3B, 0x87, 0x07, 0x1F, 0x0E, 0x42, 0x98, 0xF2, 0x07, 0x1A, 0x3F, 0x1E, 0x84, 0x58");
asm(".byte 0xC9, 0xCF, 0xC0, 0xF4, 0xB8, 0x19, 0x64, 0xDB, 0x87, 0x46, 0x76, 0x06, 0x96, 0x63, 0xCC, 0x20");
asm(".byte 0xB7, 0x57, 0x34, 0xB0, 0x33, 0xF0, 0x1C, 0x61, 0xB6, 0x01, 0x32, 0x39, 0x1A, 0x98, 0x19, 0x38");
asm(".byte 0x7A, 0xD8, 0x15, 0x60, 0xCC, 0x0E, 0x08, 0xF3, 0x40, 0x32, 0x82, 0xF9, 0x99, 0x41, 0x62, 0x06");
asm(".byte 0x94, 0xB9, 0x99, 0x51, 0x62, 0x02, 0x94, 0x69, 0xCC, 0xCC, 0x03, 0x63, 0x2A, 0xF3, 0xB1, 0x41");
asm(".byte 0x15, 0x34, 0x28, 0xFF, 0x61, 0x86, 0x6A, 0x6B, 0x50, 0x67, 0x60, 0x98, 0xC0, 0x0C, 0x62, 0x4A");
asm(".byte 0x34, 0xB8, 0x33, 0x30, 0x3C, 0x60, 0x06, 0x85, 0xD4, 0x03, 0x06, 0x30, 0x93, 0x1D, 0xEC, 0xB2");
asm(".byte 0x7E, 0x60, 0xE0, 0x34, 0x43, 0xC2, 0x81, 0x9F, 0x91, 0xA1, 0xA1, 0x11, 0xE2, 0x74, 0x39, 0x60");
asm(".byte 0x00, 0x42, 0xC3, 0xC1, 0x86, 0x87, 0x81, 0x01, 0xE2, 0x37, 0xB6, 0x9A, 0x0F, 0xF2, 0x0C, 0xB0");
asm(".byte 0x70, 0x60, 0x4C, 0x80, 0x86, 0x2F, 0xDB, 0x07, 0xF6, 0x03, 0x50, 0x51, 0xA6, 0x07, 0xF6, 0x3F");
asm(".byte 0x18, 0x0F, 0x40, 0x4C, 0x68, 0x40, 0x04, 0x2A, 0xC3, 0x3F, 0x20, 0x13, 0x1C, 0xEA, 0x0C, 0x0C");
asm(".byte 0x3F, 0xEC, 0xE1, 0xA1, 0x0E, 0x27, 0xC1, 0x31, 0xD4, 0x80, 0x88, 0x37, 0x88, 0x61, 0x3C, 0x40");
asm(".byte 0xA7, 0x5A, 0x28, 0x80, 0x99, 0x12, 0x65, 0x0C, 0x0C, 0x79, 0x02, 0x60, 0xE6, 0x86, 0x83, 0x8C");
asm(".byte 0x0D, 0xCD, 0x9C, 0x60, 0x26, 0x52, 0x7A, 0x00, 0x5A, 0xED, 0xC0, 0xC0, 0x80, 0xC9, 0x64, 0x41");
asm(".byte 0x62, 0x2A, 0xC0, 0x99, 0x3F, 0x10, 0xA9, 0x8F, 0x8E, 0x00, 0x00");
#if defined (__C30__) && (__C30_VERSION__ <= 325)
asm(".section .const, psv");
#else
asm(".section .const, psv, page");
#endif

/*********************************
 * Bitmap Structure
 * Label: mchpGraphics
 * Description:  140x140 pixels, 1-bits per pixel
 ***********************************/
#if defined (__C30__) && (__C30_VERSION__ <= 325)
asm(".section .const, psv");
#else
asm(".section .const, psv, page");
#endif


extern const char __mchpGraphics[] __attribute__((space(prog), aligned(2)));

const GFX_IMAGE_HEADER mchpGraphics =
{
    (FLASH | IMAGE_MBITMAP | COMP_IPU),
    0,
    {    .progByteAddress = (FLASH_BYTE *) __mchpGraphics    },
    140,
    140,
    377,
    2530,
    1
};

asm(".section .text, code");
asm(".global ___mchpGraphics");
asm(".align 2");
asm("___mchpGraphics:");
asm(".byte 0x63, 0x60, 0xEC, 0x61, 0xE8, 0x61, 0x60, 0x60, 0xF8, 0x8F, 0x09, 0xF8, 0x47, 0x45, 0x86, 0x81");
asm(".byte 0x08, 0x33, 0x43, 0x03, 0xF3, 0xFF, 0x7A, 0x06, 0x06, 0x3E, 0x06, 0x86, 0x7F, 0xC4, 0x8B, 0x3C");
asm(".byte 0xFE, 0xDF, 0x0F, 0x64, 0xFC, 0xF9, 0xD1, 0xFF, 0xDF, 0x9E, 0x14, 0x11, 0x86, 0xBE, 0xC7, 0xEC");
asm(".byte 0x0C, 0x40, 0x11, 0x66, 0x0B, 0xD2, 0x44, 0x98, 0x8F, 0xFF, 0x67, 0xFE, 0x4F, 0xBA, 0xC8, 0x81");
asm(".byte 0x8F, 0xFF, 0x8F, 0x93, 0x28, 0xF2, 0xBF, 0x5F, 0xDE, 0xBE, 0xEF, 0xFF, 0x01, 0x14, 0x5F, 0x10");
asm(".byte 0x14, 0x01, 0x86, 0xCF, 0x63, 0x8B, 0xBE, 0xC7, 0x16, 0x28, 0x21, 0x46, 0x50, 0x04, 0x08, 0xE4");
asm(".byte 0x8F, 0x37, 0xCA, 0x1F, 0x40, 0x89, 0x1D, 0x82, 0x22, 0xCC, 0x07, 0xFA, 0x1F, 0xFF, 0x6F, 0x64");
asm(".byte 0xFE, 0xFF, 0x11, 0xEE, 0x66, 0x62, 0x44, 0xE4, 0xED, 0xEB, 0x18, 0x80, 0x31, 0x78, 0x80, 0xF1");
asm(".byte 0x81, 0x3D, 0x29, 0x22, 0x16, 0x0D, 0xA0, 0x18, 0xB4, 0x00, 0x7A, 0xF6, 0x3F, 0xF1, 0x22, 0xC0");
asm(".byte 0xA4, 0xF2, 0x07, 0x18, 0x83, 0x07, 0x18, 0x19, 0xE0, 0xE1, 0x43, 0x84, 0xC8, 0x63, 0xF6, 0x06");
asm(".byte 0xF9, 0xE3, 0x40, 0x37, 0xFC, 0x93, 0xFF, 0xF1, 0x9F, 0x78, 0x11, 0xE6, 0x1F, 0xF5, 0x0F, 0x0E");
asm(".byte 0x7C, 0x94, 0x3F, 0xDE, 0xFF, 0x9F, 0xFD, 0x1F, 0xF1, 0x22, 0xC0, 0x58, 0x00, 0x3A, 0x58, 0x1E");
asm(".byte 0x18, 0xFB, 0x0C, 0x24, 0x88, 0xFC, 0x3F, 0x5E, 0xF8, 0xC7, 0xE2, 0x03, 0xC8, 0xB3, 0xC7, 0xFF");
asm(".byte 0x13, 0x2F, 0x22, 0x7F, 0x80, 0xF1, 0xCF, 0x01, 0x60, 0x4A, 0x2B, 0x78, 0x40, 0x8A, 0xC8, 0x7F");
asm(".byte 0xFB, 0x0F, 0x8F, 0xED, 0x0B, 0x40, 0x46, 0x1D, 0x20, 0x41, 0xE4, 0xF1, 0xF1, 0x7E, 0x79, 0x86");
asm(".byte 0x3A, 0x86, 0x1F, 0x85, 0x32, 0x3F, 0x48, 0x10, 0xF9, 0xCF, 0x50, 0xC0, 0x60, 0x5F, 0x27, 0x03");
asm(".byte 0x4C, 0xD2, 0xA4, 0x88, 0xC8, 0xB3, 0x33, 0x30, 0xB0, 0x7F, 0x60, 0xB6, 0x60, 0x60, 0x20, 0x45");
asm(".byte 0x04, 0x1C, 0xB0, 0xC0, 0x8C, 0xF0, 0x8F, 0x19, 0x29, 0x17, 0x10, 0x14, 0x01, 0xE6, 0x1D, 0x79");
asm(".byte 0x86, 0x0F, 0xCC, 0xEC, 0xA0, 0x3C, 0x4E, 0xBC, 0x08, 0xB8, 0xFC, 0x01, 0x1A, 0xF5, 0xEF, 0x31");
asm(".byte 0xFB, 0x3F, 0x12, 0x44, 0x18, 0xFA, 0xFE, 0x03, 0xE3, 0xDD, 0x9E, 0x81, 0x01, 0x1E, 0xAA, 0xC4");
asm(".byte 0x89, 0x30, 0x03, 0x8B, 0xC7, 0x03, 0x1F, 0x51, 0xCA, 0x16, 0x22, 0x44, 0x1E, 0x1F, 0xFF, 0xF8");
asm(".byte 0xF8, 0x78, 0xFD, 0x1F, 0x92, 0x44, 0xFE, 0xF7, 0x3F, 0x3E, 0x00, 0xCC, 0x71, 0x7C, 0x32, 0x48");
asm(".byte 0x3E, 0x25, 0x28, 0x02, 0x2A, 0x7F, 0xEC, 0xF9, 0xFE, 0x30, 0x80, 0x72, 0x10, 0xF1, 0x22, 0xA8");
asm(".byte 0x60, 0x54, 0x64, 0x18, 0x8A, 0x30, 0x30, 0x00, 0x00");
#if defined (__C30__) && (__C30_VERSION__ <= 325)
asm(".section .const, psv");
#else
asm(".section .const, psv, page");
#endif

/*********************************
 * Bitmap Structure
 * Label: mchp
 * Description:  140x140 pixels, 1-bits per pixel
 ***********************************/
#if defined (__C30__) && (__C30_VERSION__ <= 325)
asm(".section .const, psv");
#else
asm(".section .const, psv, page");
#endif


extern const char __mchp[] __attribute__((space(prog), aligned(2)));

const GFX_IMAGE_HEADER mchp =
{
    (FLASH | IMAGE_MBITMAP | COMP_IPU),
    0,
    {    .progByteAddress = (FLASH_BYTE *) __mchp    },
    140,
    140,
    262,
    2530,
    1
};

asm(".section .text, code");
asm(".global ___mchp");
asm(".align 2");
asm("___mchp:");
asm(".byte 0x63, 0x60, 0xEC, 0x61, 0xE8, 0x61, 0x60, 0x60, 0xF8, 0x8F, 0x09, 0xF8, 0x47, 0x45, 0x68, 0x23");
asm(".byte 0xC2, 0x0F, 0x0C, 0x6F, 0x7E, 0x86, 0x0F, 0xFC, 0x60, 0x06, 0x25, 0x22, 0x60, 0x04, 0x14, 0xE5");
asm(".byte 0x47, 0xE2, 0x92, 0x23, 0xC2, 0xCF, 0xC0, 0xFF, 0xFF, 0xC3, 0x87, 0x0F, 0xFC, 0x60, 0x06, 0x85");
asm(".byte 0x22, 0xFC, 0xD4, 0x12, 0x01, 0xB9, 0x90, 0x42, 0x11, 0x10, 0xFA, 0xF0, 0x01, 0xD5, 0xE3, 0xE4");
asm(".byte 0x88, 0x30, 0x80, 0x4D, 0x05, 0x19, 0x8C, 0x1C, 0x17, 0x24, 0x8B, 0x80, 0xA3, 0xFF, 0x03, 0xFF");
asm(".byte 0x07, 0x06, 0x94, 0xB4, 0x41, 0xBA, 0x08, 0x90, 0x0D, 0x74, 0x21, 0xC8, 0x64, 0xB8, 0x0B, 0xC9");
asm(".byte 0x13, 0x01, 0x1A, 0x07, 0x4C, 0x4F, 0x40, 0xCF, 0x32, 0x30, 0x7C, 0xA0, 0x40, 0xE4, 0xFF, 0x07");
asm(".byte 0x60, 0xE0, 0x7F, 0x00, 0xA1, 0x0F, 0x1F, 0xF8, 0x29, 0x11, 0x01, 0x09, 0x01, 0x49, 0x86, 0xFF");
asm(".byte 0x1F, 0xE0, 0x21, 0x46, 0x8E, 0x08, 0xD0, 0x48, 0x60, 0x4E, 0xF9, 0x0F, 0x0E, 0x32, 0x7E, 0x8A");
asm(".byte 0x44, 0x18, 0x3E, 0x80, 0x93, 0x13, 0x28, 0x41, 0x50, 0x24, 0x02, 0x34, 0xF3, 0x03, 0x38, 0x8D");
asm(".byte 0xC3, 0x5D, 0x48, 0x96, 0x08, 0x98, 0x0D, 0x32, 0x19, 0x9E, 0x79, 0xC9, 0x14, 0x81, 0x14, 0x25");
asm(".byte 0x20, 0xF2, 0x03, 0x05, 0x22, 0x90, 0xE4, 0x08, 0x0A, 0x7D, 0x98, 0x4F, 0xC9, 0x13, 0x81, 0x66");
asm(".byte 0x10, 0x10, 0xF8, 0xC0, 0x4F, 0x89, 0x08, 0x08, 0x31, 0x80, 0x82, 0x8D, 0xFF, 0x3F, 0x45, 0x22");
asm(".byte 0xE0, 0xE0, 0x62, 0x00, 0x59, 0xF0, 0xE1, 0x3F, 0xA5, 0x22, 0x1F, 0xC0, 0x02, 0xB0, 0x44, 0x45");
asm(".byte 0xB6, 0x08, 0x38, 0xBF, 0x7D, 0x00, 0x25, 0x50, 0x0A, 0x44, 0xC0, 0x82, 0xA0, 0xC0, 0xA2, 0x50");
asm(".byte 0x04, 0x52, 0xB2, 0x01, 0x85, 0xFF, 0x23, 0xC5, 0x05, 0xE9, 0x22, 0xE4, 0xD5, 0x9E, 0xA3, 0x22");
asm(".byte 0xA4, 0x89, 0x30, 0x30, 0x00, 0x00");
#if defined (__C30__) && (__C30_VERSION__ <= 325)
asm(".section .const, psv");
#else
asm(".section .const, psv, page");
#endif

/*****************************************************************************
 * SECTION:  Palettes
 *****************************************************************************/

/*********************************
 * PALETTE Structure
 * Label: Screen_1BPP_Palette
 * Description:  2 Colors
 ***********************************/

extern const PALETTE_ENTRY Screen_1BPP_Palette_Data[];

const PALETTE_FLASH Screen_1BPP_Palette =
{
    (FLASH | COMP_NONE),
    {
        0x0000,        // id
        2             // number of entries
    },
    (void *)Screen_1BPP_Palette_Data       // entry address
};

const PALETTE_ENTRY Screen_1BPP_Palette_Data[] =
{
/***********************************
 * Color Palette Entries
 **********************************/
    { .value = 0x0000},
    { .value = 0xFEA3},
};

