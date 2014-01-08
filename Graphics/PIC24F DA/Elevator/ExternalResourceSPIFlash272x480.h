/*****************************************************************************
 * FileName:        ExternalResourceSPIFlash272x480.h
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

#ifndef EXTERNALRESOURCESPIFLASH272X480_H_FILE
#define EXTERNALRESOURCESPIFLASH272X480_H_FILE
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
 * This is a unique marker to verify the external memory data
 * The marker is a total of 8 bytes in length starting at the location GRC_CRC32_EXTERNAL_ADDR.
 * The first four bytes, starting at GRC_CRC32_EXTERNAL_ADDR, will always equal 'M', 'C', 'H', 'P'.
 * The next four byte is the generated 32-bit CRC.  The application can compare the value read from the
 * external memory to the value of GRC_CRC32_EXTERNAL_MARKER to verify that the data is valid.
 *****************************************************************************/
#define GRC_CRC32_EXTERNAL_MARKER 0x900BC363ul
#define GRC_CRC32_EXTERNAL_ADDR 0x00047AC0ul

/*****************************************************************************
 * SECTION:  BITMAPS
 *****************************************************************************/

/*********************************
 * Bitmap Structure
 * Label: Background272x480
 * Description:  272x480 pixels, 16-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL Background272x480;
#define Background272x480_WIDTH     (272)
#define Background272x480_HEIGHT    (480)
#define Background272x480_SIZE      (261126)
/*********************************
 * Bitmap Structure
 * Label: arrowUpComposite
 * Description:  100x230 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL arrowUpComposite;
#define arrowUpComposite_WIDTH     (100)
#define arrowUpComposite_HEIGHT    (230)
#define arrowUpComposite_SIZE      (11538)
/*********************************
 * Bitmap Structure
 * Label: arrowDownComposite
 * Description:  100x230 pixels, 4-bits per pixel
 ***********************************/
extern const IMAGE_EXTERNAL arrowDownComposite;
#define arrowDownComposite_WIDTH     (100)
#define arrowDownComposite_HEIGHT    (230)
#define arrowDownComposite_SIZE      (11538)
/*****************************************************************************
 * SECTION:  Fonts
 *****************************************************************************/

/*********************************
 * TTF Font File Structure
 * Label: fireflysung_40
 * Description:  Height: 44 pixels, 1 bit per pixel, Range: ' ' to '?'
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
extern const FONT_EXTERNAL fireflysung_40;
#define fireflysung_40_SIZE    (9328)
/*****************************************************************************
 * SECTION:  fireflysung_40
 *****************************************************************************/

extern const XCHAR strMaintainChn[];
extern const XCHAR strOverLoadChn[];
extern const XCHAR strTimeOutChn[];
extern const XCHAR strRunTimeChn[];
extern const XCHAR strPauseChn[];
extern const XCHAR strHaltChn[];
extern const XCHAR strErrorChn[];
extern const XCHAR strNonStopChn[];
extern const XCHAR strWaitChn[];
extern const XCHAR strClosingChn[];
extern const XCHAR strOpeningChn[];
extern const XCHAR strMaintainEng[];
extern const XCHAR strOverLoadEng[];
extern const XCHAR strTimeOutEng[];
extern const XCHAR strRunTimeEng[];
extern const XCHAR strPauseEng[];
extern const XCHAR strHaltEng[];
extern const XCHAR strErrorEng[];
extern const XCHAR strNonStopEng[];
extern const XCHAR strWaitEng[];
extern const XCHAR strClosingEng[];
extern const XCHAR strOpeningEng[];
#endif

