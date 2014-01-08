/*****************************************************************************
 * FileName:        InternalResourceXC32.h
 * Processor:       PIC32MX
 * Compiler:        MPLAB C32/XC32 (see release notes for tested revision)
 * Linker:          MPLAB LINK32/XC32
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

#ifndef INTERNALRESOURCEXC32_H_FILE
#define INTERNALRESOURCEXC32_H_FILE
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
 * This is an error check for the color depth
 *****************************************************************************/
#if (COLOR_DEPTH > 16)
#error "Color Depth needs to be 16 to correctly use these resources"
#endif



/*****************************************************************************
 * SECTION:  BITMAPS
 *****************************************************************************/

/*********************************
 * Bitmap Structure
 * Label: Engine4
 * Description:  73x55 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER Engine4;
#define Engine4_WIDTH     (73)
#define Engine4_HEIGHT    (55)
#define Engine4_SIZE      (2073)
/*********************************
 * Bitmap Structure
 * Label: greenphone
 * Description:  48x35 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER greenphone;
#define greenphone_WIDTH     (48)
#define greenphone_HEIGHT    (35)
#define greenphone_SIZE      (878)
/*********************************
 * Bitmap Structure
 * Label: intro
 * Description:  158x55 pixels, 8-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER intro;
#define intro_WIDTH     (158)
#define intro_HEIGHT    (55)
#define intro_SIZE      (9208)
/*********************************
 * Bitmap Structure
 * Label: mchpIcon
 * Description:  32x32 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH mchpIcon;
#define mchpIcon_WIDTH     (32)
#define mchpIcon_HEIGHT    (32)
#define mchpIcon_SIZE      (138)
/*********************************
 * Bitmap Structure
 * Label: mchpLogo
 * Description:  152x40 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER mchpLogo;
#define mchpLogo_WIDTH     (152)
#define mchpLogo_HEIGHT    (40)
#define mchpLogo_SIZE      (3078)
/*********************************
 * Bitmap Structure
 * Label: arrowDown
 * Description:  18x26 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_FLASH arrowDown;
#define arrowDown_WIDTH     (18)
#define arrowDown_HEIGHT    (26)
#define arrowDown_SIZE      (272)
/*********************************
 * Bitmap Structure
 * Label: arrowUp
 * Description:  18x26 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_FLASH arrowUp;
#define arrowUp_WIDTH     (18)
#define arrowUp_HEIGHT    (26)
#define arrowUp_SIZE      (272)
/*********************************
 * Bitmap Structure
 * Label: bulboff
 * Description:  120x44 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER bulboff;
#define bulboff_WIDTH     (120)
#define bulboff_HEIGHT    (44)
#define bulboff_SIZE      (2678)
/*********************************
 * Bitmap Structure
 * Label: bulbon
 * Description:  120x44 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER bulbon;
#define bulbon_WIDTH     (120)
#define bulbon_HEIGHT    (44)
#define bulbon_SIZE      (2678)
/*********************************
 * Bitmap Structure
 * Label: Engine1
 * Description:  73x55 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER Engine1;
#define Engine1_WIDTH     (73)
#define Engine1_HEIGHT    (55)
#define Engine1_SIZE      (2073)
/*********************************
 * Bitmap Structure
 * Label: Engine2
 * Description:  73x55 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER Engine2;
#define Engine2_WIDTH     (73)
#define Engine2_HEIGHT    (55)
#define Engine2_SIZE      (2073)
/*********************************
 * Bitmap Structure
 * Label: Engine3
 * Description:  73x55 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER Engine3;
#define Engine3_WIDTH     (73)
#define Engine3_HEIGHT    (55)
#define Engine3_SIZE      (2073)
/*********************************
 * Bitmap Structure
 * Label: redphone
 * Description:  48x35 pixels, 4-bits per pixel
 ***********************************/
extern const GFX_IMAGE_HEADER redphone;
#define redphone_WIDTH     (48)
#define redphone_HEIGHT    (35)
#define redphone_SIZE      (878)
/*********************************
 * Bitmap Structure
 * Label: mchpIcon0
 * Description:  32x32 pixels, 1-bits per pixel
 ***********************************/
extern const IMAGE_FLASH mchpIcon0;
#define mchpIcon0_WIDTH     (32)
#define mchpIcon0_HEIGHT    (32)
#define mchpIcon0_SIZE      (138)
/*****************************************************************************
 * SECTION:  Fonts
 *****************************************************************************/

/*********************************
 * TTF Font File Structure
 * Label: monofont
 * Description:  Height: 20 pixels, 1 bit per pixel, Range: '.' to '9'
 * Comment Block:
 * Created from Bitstream Vera Fonts 
 *  
 * Copyright (c) 2003, Bitstream Inc. 
 * All Rights Reserved. Bitstream 
 * Vera is a trademark of Bitstream, Inc. 
 *  
 * Permission is hereby granted, free of charge, to any person obtaining 
 * a copy of the fonts accompanying this license ("Fonts") and associated 
 * documentation files (the "Font Software"), to reproduce and distribute 
 * the Font Software, including without limitation the rights to use, 
 * copy, merge, publish, distribute, and/or sell copies of the Font 
 * Software, and to permit persons to whom the Font Software is furnished 
 * to do so, subject to the following conditions: 
 *  
 * The above copyright and trademark notices and this permission notice 
 * shall be included in all copies of one or more of the Font Software 
 * typefaces. 
 *  
 * The Font Software may be modified, altered, or added to, and in 
 * particular the designs of glyphs or characters in the Fonts may be 
 * modified and additional glyphs or characters may be added to the 
 * Fonts, only if the fonts are renamed to names not containing either 
 * the words "Bitstream" or the word "Vera". 
 *  
 * This License becomes null and void to the extent applicable to Fonts 
 * or Font Software that has been modified and is distributed under the 
 * "Bitstream Vera" names. 
 *  
 * The Font Software may be sold as part of a larger software package but 
 * no copy of one or more of the Font Software typefaces may be sold by 
 * itself. 
 * 
 * THE FONT SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT 
 * OF COPYRIGHT, PATENT, TRADEMARK, OR OTHER RIGHT. IN NO EVENT SHALL 
 * BITSTREAM OR THE GNOME FOUNDATION BE LIABLE FOR ANY CLAIM, DAMAGES OR 
 * OTHER LIABILITY, INCLUDING ANY GENERAL, SPECIAL, INDIRECT, INCIDENTAL, 
 * OR CONSEQUENTIAL DAMAGES, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 * OTHERWISE, ARISING FROM, OUT OF THE USE OR INABILITY TO USE THE FONT 
 * SOFTWARE OR FROM OTHER DEALINGS IN THE FONT SOFTWARE. 
 *  
 * Except as contained in this notice, the names of Gnome, the Gnome 
 * Foundation, and Bitstream Inc., shall not be used in advertising or 
 * otherwise to promote the sale, use or other dealings in this Font 
 * Software without prior written authorization from the Gnome Foundation 
 * or Bitstream Inc., respectively. For further information, contact: 
 * fonts at gnome dot org. 
 *  
 ***********************************/
extern const FONT_FLASH monofont;
#define monofont_SIZE    (536)
/*********************************
 * TTF Font File Structure
 * Label: GOLSmallFont
 * Description:  Height: 15 pixels, 1 bit per pixel, Range: ' ' to '?'
 * Comment Block:
 * ========================================================================== 
 *  
 * Created from AR PL New Sung font (fireflysung.ttf). 
 *  
 * AR PL New Sung font is 
 *     Copyright (c) 1994-1999, Arphic Technology Co., Ltd. 
 *     Copyright (C) 1999-2004, Firefly and Arphic Technology Co., Ltd. 
 * All Rights Reserved. 
 *  
 * NOTICE: This Truetype font contains embedded bitmap fonts made 
 *     by firefly and is released as a whole 
 *     under the ARPHIC PUBLIC LICENSE. 
 *     There are also seperate bitmap fonts made by Firefly and 
 *     released under the GENERAL PUBLIC LICENSE (GPL): 
 *     http://www.study-area.org/apt/firefly-font/bitmaps/ 
 *  
 * Copyright: 
 *     (Copied from 'license/english/ARPHICPL.TXT'. 
 *     See 'license/big5/ARPHICPL.TXT' or 'license/gb/ARPHICPL.TXT' 
 *     for the Chinese version.) 
 *  
 * ========================================================================== 
 *  
 * ARPHIC PUBLIC LICENSE 
 *  
 * Copyright (C) 1999 Arphic Technology Co., Ltd. 
 * 11Fl. No.168, Yung Chi Rd., Taipei, 110 Taiwan 
 * All rights reserved except as specified below. 
 *  
 * Everyone is permitted to copy and distribute verbatim copies of this 
 * license document, but changing it is forbidden. 
 *  
 * Preamble 
 *  
 *    The licenses for most software are designed to take away your freedom 
 * to share and change it. By contrast, the ARPHIC PUBLIC LICENSE 
 * specifically permits and encourages you to use this software, provided 
 * that you give the recipients all the rights that we gave you and make 
 * sure they can get the modifications of this software. 
 *  
 * Legal Terms 
 *  
 * 0. Definitions: 
 *    Throughout this License, "Font" means the TrueType fonts "AR PL 
 * Mingti2L Big5", "AR PL KaitiM Big5" (BIG-5 character set) and "AR PL 
 * SungtiL GB", "AR PL KaitiM GB" (GB character set) which are originally 
 * distributed by Arphic, and the derivatives of those fonts created 
 * through any modification including modifying glyph, reordering glyph, 
 * converting format, changing font name, or adding/deleting some 
 * characters in/from glyph table. 
 *  
 *    "PL" means "Public License". 
 *  
 *    "Copyright Holder" means whoever is named in the copyright or 
 * copyrights for the Font. 
 *  
 *    "You" means the licensee, or person copying, redistributing or 
 * modifying the Font. 
 *  
 *    "Freely Available" means that you have the freedom to copy or modify 
 * the Font as well as redistribute copies of the Font under the same 
 * conditions you received, not price. If you wish, you can charge for this 
 * service. 
 *  
 * 1. Copying & Distribution 
 *    You may copy and distribute verbatim copies of this Font in any 
 * medium, without restriction, provided that you retain this license file 
 * (ARPHICPL.TXT) unaltered in all copies. 
 *  
 * 2. Modification 
 *    You may otherwise modify your copy of this Font in any way, including 
 * modifying glyph, reordering glyph, converting format, changing font 
 * name, or adding/deleting some characters in/from glyph table, and copy 
 * and distribute such modifications under the terms of Section 1 above, 
 * provided that the following conditions are met: 
 *  
 *    a) You must insert a prominent notice in each modified file stating 
 * how and when you changed that file. 
 *  
 *    b) You must make such modifications Freely Available as a whole to 
 * all third parties under the terms of this License, such as by offering 
 * access to copy the modifications from a designated place, or 
 * distributing the modifications on a medium customarily used for software 
 * interchange. 
 *  
 *    c) If the modified fonts normally reads commands interactively when 
 * run, you must cause it, when started running for such interactive use in 
 * the most ordinary way, to print or display an announcement including an 
 * appropriate copyright notice and a notice that there is no warranty (or 
 * else, saying that you provide a warranty) and that users may 
 * redistribute the Font under these conditions, and telling the user how 
 * to view a copy of this License. 
 *  
 *    These requirements apply to the modified work as a whole. If 
 * identifiable sections of that work are not derived from the Font, and 
 * can be reasonably considered independent and separate works in 
 * themselves, then this License and its terms, do not apply to those 
 * sections when you distribute them as separate works. Therefore, mere 
 * aggregation of another work not based on the Font with the Font on a 
 * volume of a storage or distribution medium does not bring the other work 
 * under the scope of this License. 
 *  
 * 3. Condition Subsequent 
 *    You may not copy, modify, sublicense, or distribute the Font except 
 * as expressly provided under this License. Any attempt otherwise to copy, 
 * modify, sublicense or distribute the Font will automatically 
 * retroactively void your rights under this License. However, parties who 
 * have received copies or rights from you under this License will keep 
 * their licenses valid so long as such parties remain in full compliance. 
 *  
 * 4. Acceptance 
 *    You are not required to accept this License, since you have not 
 * signed it. However, nothing else grants you permission to copy, modify, 
 * sublicense or distribute the Font. These actions are prohibited by law 
 * if you do not accept this License. Therefore, by copying, modifying, 
 * sublicensing or distributing the Font, you indicate your acceptance of 
 * this License and all its terms and conditions. 
 *  
 * 5. Automatic Receipt 
 *    Each time you redistribute the Font, the recipient automatically 
 * receives a license from the original licensor to copy, distribute or 
 * modify the Font subject to these terms and conditions. You may not 
 * impose any further restrictions on the recipients' exercise of the 
 * rights granted herein. You are not responsible for enforcing compliance 
 * by third parties to this License. 
 *  
 * 6. Contradiction 
 *    If, as a consequence of a court judgment or allegation of patent 
 * infringement or for any other reason (not limited to patent issues), 
 * conditions are imposed on you (whether by court order, agreement or 
 * otherwise) that contradict the conditions of this License, they do not 
 * excuse you from the conditions of this License. If you cannot distribute 
 * so as to satisfy simultaneously your obligations under this License and 
 * any other pertinent obligations, then as a consequence you may not 
 * distribute the Font at all. For example, if a patent license would not 
 * permit royalty-free redistribution of the Font by all those who receive 
 * copies directly or indirectly through you, then the only way you could 
 * satisfy both it and this License would be to refrain entirely from 
 * distribution of the Font. 
 *  
 *    If any portion of this section is held invalid or unenforceable under 
 * any particular circumstance, the balance of the section is intended to 
 * apply and the section as a whole is intended to apply in other 
 * circumstances. 
 *  
 * 7. NO WARRANTY 
 *    BECAUSE THE FONT IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY FOR 
 * THE FONT, TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT WHEN 
 * OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS OR OTHER PARTIES 
 * PROVIDE THE FONT "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS 
 * TO THE QUALITY AND PERFORMANCE OF THE FONT IS WITH YOU. SHOULD THE FONT 
 * PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR 
 * OR CORRECTION. 
 *  
 * 8. DAMAGES WAIVER 
 *    UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING, IN NO 
 * EVENT WILL ANY COPYRIGHTT HOLDERS, OR OTHER PARTIES WHO MAY COPY, MODIFY 
 * OR REDISTRIBUTE THE FONT AS PERMITTED ABOVE, BE LIABLE TO YOU FOR ANY 
 * DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL, SPECIAL OR EXEMPLARY 
 * DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE FONT (INCLUDING 
 * BUT NOT LIMITED TO PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF 
 * USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION), EVEN IF SUCH HOLDERS OR 
 * OTHER PARTIES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
 *  
 * ====================================================================== 
 ***********************************/
extern const FONT_FLASH GOLSmallFont;
#define GOLSmallFont_SIZE    (3913)
/*********************************
 * TTF Font File Structure
 * Label: GOLFontDefault
 * Description:  Height: 17 pixels, 1 bit per pixel, Range: ' ' to '?'
 * Comment Block:
 * ========================================================================== 
 *  
 * Created from AR PL New Sung font (fireflysung.ttf). 
 *  
 * AR PL New Sung font is 
 *     Copyright (c) 1994-1999, Arphic Technology Co., Ltd. 
 *     Copyright (C) 1999-2004, Firefly and Arphic Technology Co., Ltd. 
 * All Rights Reserved. 
 *  
 * NOTICE: This Truetype font contains embedded bitmap fonts made 
 *     by firefly and is released as a whole 
 *     under the ARPHIC PUBLIC LICENSE. 
 *     There are also seperate bitmap fonts made by Firefly and 
 *     released under the GENERAL PUBLIC LICENSE (GPL): 
 *     http://www.study-area.org/apt/firefly-font/bitmaps/ 
 *  
 * Copyright: 
 *     (Copied from 'license/english/ARPHICPL.TXT'. 
 *     See 'license/big5/ARPHICPL.TXT' or 'license/gb/ARPHICPL.TXT' 
 *     for the Chinese version.) 
 *  
 * ========================================================================== 
 *  
 * ARPHIC PUBLIC LICENSE 
 *  
 * Copyright (C) 1999 Arphic Technology Co., Ltd. 
 * 11Fl. No.168, Yung Chi Rd., Taipei, 110 Taiwan 
 * All rights reserved except as specified below. 
 *  
 * Everyone is permitted to copy and distribute verbatim copies of this 
 * license document, but changing it is forbidden. 
 *  
 * Preamble 
 *  
 *    The licenses for most software are designed to take away your freedom 
 * to share and change it. By contrast, the ARPHIC PUBLIC LICENSE 
 * specifically permits and encourages you to use this software, provided 
 * that you give the recipients all the rights that we gave you and make 
 * sure they can get the modifications of this software. 
 *  
 * Legal Terms 
 *  
 * 0. Definitions: 
 *    Throughout this License, "Font" means the TrueType fonts "AR PL 
 * Mingti2L Big5", "AR PL KaitiM Big5" (BIG-5 character set) and "AR PL 
 * SungtiL GB", "AR PL KaitiM GB" (GB character set) which are originally 
 * distributed by Arphic, and the derivatives of those fonts created 
 * through any modification including modifying glyph, reordering glyph, 
 * converting format, changing font name, or adding/deleting some 
 * characters in/from glyph table. 
 *  
 *    "PL" means "Public License". 
 *  
 *    "Copyright Holder" means whoever is named in the copyright or 
 * copyrights for the Font. 
 *  
 *    "You" means the licensee, or person copying, redistributing or 
 * modifying the Font. 
 *  
 *    "Freely Available" means that you have the freedom to copy or modify 
 * the Font as well as redistribute copies of the Font under the same 
 * conditions you received, not price. If you wish, you can charge for this 
 * service. 
 *  
 * 1. Copying & Distribution 
 *    You may copy and distribute verbatim copies of this Font in any 
 * medium, without restriction, provided that you retain this license file 
 * (ARPHICPL.TXT) unaltered in all copies. 
 *  
 * 2. Modification 
 *    You may otherwise modify your copy of this Font in any way, including 
 * modifying glyph, reordering glyph, converting format, changing font 
 * name, or adding/deleting some characters in/from glyph table, and copy 
 * and distribute such modifications under the terms of Section 1 above, 
 * provided that the following conditions are met: 
 *  
 *    a) You must insert a prominent notice in each modified file stating 
 * how and when you changed that file. 
 *  
 *    b) You must make such modifications Freely Available as a whole to 
 * all third parties under the terms of this License, such as by offering 
 * access to copy the modifications from a designated place, or 
 * distributing the modifications on a medium customarily used for software 
 * interchange. 
 *  
 *    c) If the modified fonts normally reads commands interactively when 
 * run, you must cause it, when started running for such interactive use in 
 * the most ordinary way, to print or display an announcement including an 
 * appropriate copyright notice and a notice that there is no warranty (or 
 * else, saying that you provide a warranty) and that users may 
 * redistribute the Font under these conditions, and telling the user how 
 * to view a copy of this License. 
 *  
 *    These requirements apply to the modified work as a whole. If 
 * identifiable sections of that work are not derived from the Font, and 
 * can be reasonably considered independent and separate works in 
 * themselves, then this License and its terms, do not apply to those 
 * sections when you distribute them as separate works. Therefore, mere 
 * aggregation of another work not based on the Font with the Font on a 
 * volume of a storage or distribution medium does not bring the other work 
 * under the scope of this License. 
 *  
 * 3. Condition Subsequent 
 *    You may not copy, modify, sublicense, or distribute the Font except 
 * as expressly provided under this License. Any attempt otherwise to copy, 
 * modify, sublicense or distribute the Font will automatically 
 * retroactively void your rights under this License. However, parties who 
 * have received copies or rights from you under this License will keep 
 * their licenses valid so long as such parties remain in full compliance. 
 *  
 * 4. Acceptance 
 *    You are not required to accept this License, since you have not 
 * signed it. However, nothing else grants you permission to copy, modify, 
 * sublicense or distribute the Font. These actions are prohibited by law 
 * if you do not accept this License. Therefore, by copying, modifying, 
 * sublicensing or distributing the Font, you indicate your acceptance of 
 * this License and all its terms and conditions. 
 *  
 * 5. Automatic Receipt 
 *    Each time you redistribute the Font, the recipient automatically 
 * receives a license from the original licensor to copy, distribute or 
 * modify the Font subject to these terms and conditions. You may not 
 * impose any further restrictions on the recipients' exercise of the 
 * rights granted herein. You are not responsible for enforcing compliance 
 * by third parties to this License. 
 *  
 * 6. Contradiction 
 *    If, as a consequence of a court judgment or allegation of patent 
 * infringement or for any other reason (not limited to patent issues), 
 * conditions are imposed on you (whether by court order, agreement or 
 * otherwise) that contradict the conditions of this License, they do not 
 * excuse you from the conditions of this License. If you cannot distribute 
 * so as to satisfy simultaneously your obligations under this License and 
 * any other pertinent obligations, then as a consequence you may not 
 * distribute the Font at all. For example, if a patent license would not 
 * permit royalty-free redistribution of the Font by all those who receive 
 * copies directly or indirectly through you, then the only way you could 
 * satisfy both it and this License would be to refrain entirely from 
 * distribution of the Font. 
 *  
 *    If any portion of this section is held invalid or unenforceable under 
 * any particular circumstance, the balance of the section is intended to 
 * apply and the section as a whole is intended to apply in other 
 * circumstances. 
 *  
 * 7. NO WARRANTY 
 *    BECAUSE THE FONT IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY FOR 
 * THE FONT, TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT WHEN 
 * OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS OR OTHER PARTIES 
 * PROVIDE THE FONT "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS 
 * TO THE QUALITY AND PERFORMANCE OF THE FONT IS WITH YOU. SHOULD THE FONT 
 * PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR 
 * OR CORRECTION. 
 *  
 * 8. DAMAGES WAIVER 
 *    UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING, IN NO 
 * EVENT WILL ANY COPYRIGHTT HOLDERS, OR OTHER PARTIES WHO MAY COPY, MODIFY 
 * OR REDISTRIBUTE THE FONT AS PERMITTED ABOVE, BE LIABLE TO YOU FOR ANY 
 * DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL, SPECIAL OR EXEMPLARY 
 * DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE FONT (INCLUDING 
 * BUT NOT LIMITED TO PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF 
 * USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION), EVEN IF SUCH HOLDERS OR 
 * OTHER PARTIES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
 *  
 * ====================================================================== 
 ***********************************/
extern const FONT_FLASH GOLFontDefault;
#define GOLFontDefault_SIZE    (6289)
/*****************************************************************************
 * SECTION:  GOLSmallFont
 *****************************************************************************/

extern const XCHAR cOnBulbStr[];
extern const XCHAR cOffBulbStr[];
extern const XCHAR cSelectionStr[];
extern const XCHAR cAlignmentStr[];
extern const XCHAR cReturnStr[];
extern const XCHAR cMeter1Str[];
extern const XCHAR cMeter2Str[];
extern const XCHAR cValueLabelStr[];
extern const XCHAR cMoreStr[];
extern const XCHAR cBackStr[];
extern const XCHAR cGroup1Str[];
extern const XCHAR cGroup2Str[];
extern const XCHAR cSetTimeDateStr[];
extern const XCHAR cNormalLoadStr[];
extern const XCHAR cLightLoadStr[];
extern const XCHAR cHeavyLoadStr[];
extern const XCHAR cUseR6PotStr[];
extern const XCHAR cScanModeStr[];
extern const XCHAR cScrSelList1[];
extern const XCHAR cScrSelList2[];
extern const XCHAR cScrSelListLang[];
extern const XCHAR cLanguage[];
extern const XCHAR eOnBulbStr[];
extern const XCHAR eOffBulbStr[];
extern const XCHAR eSelectionStr[];
extern const XCHAR eAlignmentStr[];
extern const XCHAR eReturnStr[];
extern const XCHAR eMeter1Str[];
extern const XCHAR eMeter2Str[];
extern const XCHAR eValueLabelStr[];
extern const XCHAR eMoreStr[];
extern const XCHAR eBackStr[];
extern const XCHAR eGroup1Str[];
extern const XCHAR eGroup2Str[];
extern const XCHAR eSetTimeDateStr[];
extern const XCHAR eNormalLoadStr[];
extern const XCHAR eLightLoadStr[];
extern const XCHAR eHeavyLoadStr[];
extern const XCHAR eUseR6PotStr[];
extern const XCHAR eScanModeStr[];
extern const XCHAR eScrSelList1[];
extern const XCHAR eScrSelList2[];
extern const XCHAR eScrSelListLang[];
extern const XCHAR eLanguage[];
/*****************************************************************************
 * SECTION:  GOLFontDefault
 *****************************************************************************/

extern const XCHAR cTouchScreenStr[];
extern const XCHAR cButtonStr[];
extern const XCHAR cCheckBoxStr[];
extern const XCHAR cRadioButtonStr[];
extern const XCHAR cGroupBoxStr[];
extern const XCHAR cStaticTextStr[];
extern const XCHAR cSliderStr[];
extern const XCHAR cProgressBarStr[];
extern const XCHAR cListBoxStr[];
extern const XCHAR cEditBoxStr[];
extern const XCHAR cMeterStr[];
extern const XCHAR cDialStr[];
extern const XCHAR cPictureStr[];
extern const XCHAR cCustomStr[];
extern const XCHAR cSignatureStr[];
extern const XCHAR cPlottingStr[];
extern const XCHAR cEcgStr[];
extern const XCHAR cHomeStr[];
extern const XCHAR cHomeLinesStr[];
extern const XCHAR cLowStr[];
extern const XCHAR cHighStr[];
extern const XCHAR cOnStr[];
extern const XCHAR cOffStr[];
extern const XCHAR cEnableStr[];
extern const XCHAR cDisabledStr[];
extern const XCHAR cTextLeftStr[];
extern const XCHAR cTextBottomStr[];
extern const XCHAR cRb1Str[];
extern const XCHAR cRb2Str[];
extern const XCHAR cRb3Str[];
extern const XCHAR cRb4Str[];
extern const XCHAR cRb5Str[];
extern const XCHAR cRb6Str[];
extern const XCHAR cStaticTextLstStr[];
extern const XCHAR cLeftStr[];
extern const XCHAR cCenterStr[];
extern const XCHAR cRightStr[];
extern const XCHAR cPIC24SpeedStr[];
extern const XCHAR cQuestionStr[];
extern const XCHAR cSingleStr[];
extern const XCHAR cAlignCenterStr[];
extern const XCHAR cListboxLstStr[];
extern const XCHAR cCallingStr[];
extern const XCHAR cHoldingStr[];
extern const XCHAR cHoldStr[];
extern const XCHAR cAccelStr[];
extern const XCHAR cDecelStr[];
extern const XCHAR cScaleStr[];
extern const XCHAR cAnimateStr[];
extern const XCHAR cLeftArrowStr[];
extern const XCHAR cRightArrowStr[];
extern const XCHAR cUpArrowStr[];
extern const XCHAR cDownArrowStr[];
extern const XCHAR cExitStr[];
extern const XCHAR cSetDateTimeStr[];
extern const XCHAR eTouchScreenStr[];
extern const XCHAR eButtonStr[];
extern const XCHAR eCheckBoxStr[];
extern const XCHAR eRadioButtonStr[];
extern const XCHAR eGroupBoxStr[];
extern const XCHAR eStaticTextStr[];
extern const XCHAR eSliderStr[];
extern const XCHAR eProgressBarStr[];
extern const XCHAR eListBoxStr[];
extern const XCHAR eEditBoxStr[];
extern const XCHAR eMeterStr[];
extern const XCHAR eDialStr[];
extern const XCHAR ePictureStr[];
extern const XCHAR eCustomStr[];
extern const XCHAR eSignatureStr[];
extern const XCHAR ePlottingStr[];
extern const XCHAR eEcgStr[];
extern const XCHAR eHomeStr[];
extern const XCHAR eHomeLinesStr[];
extern const XCHAR eLowStr[];
extern const XCHAR eHighStr[];
extern const XCHAR eOnStr[];
extern const XCHAR eOffStr[];
extern const XCHAR eEnableStr[];
extern const XCHAR eDisabledStr[];
extern const XCHAR eTextLeftStr[];
extern const XCHAR eTextBottomStr[];
extern const XCHAR eRb1Str[];
extern const XCHAR eRb2Str[];
extern const XCHAR eRb3Str[];
extern const XCHAR eRb4Str[];
extern const XCHAR eRb5Str[];
extern const XCHAR eRb6Str[];
extern const XCHAR eStaticTextLstStr[];
extern const XCHAR eLeftStr[];
extern const XCHAR eCenterStr[];
extern const XCHAR eRightStr[];
extern const XCHAR ePIC24SpeedStr[];
extern const XCHAR eQuestionStr[];
extern const XCHAR eSingleStr[];
extern const XCHAR eAlignCenterStr[];
extern const XCHAR eListboxLstStr[];
extern const XCHAR eCallingStr[];
extern const XCHAR eHoldingStr[];
extern const XCHAR eHoldStr[];
extern const XCHAR eAccelStr[];
extern const XCHAR eDecelStr[];
extern const XCHAR eScaleStr[];
extern const XCHAR eAnimateStr[];
extern const XCHAR eLeftArrowStr[];
extern const XCHAR eRightArrowStr[];
extern const XCHAR eUpArrowStr[];
extern const XCHAR eDownArrowStr[];
extern const XCHAR eExitStr[];
extern const XCHAR eSetDateTimeStr[];
#endif

