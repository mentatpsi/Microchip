/*****************************************************************************
 * FileName:        HelloWorldFonts XC32.c
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
 * RussianFont - Height: 25 pixels, 1 bit per pixel, Range: ' ' to '?'
 * ChineseFont - Height: 22 pixels, 1 bit per pixel, Range: '!' to '?'
 * JapaneseFont - Height: 25 pixels, 1 bit per pixel, Range: '!' to '?'
 * KoreanFont - Height: 23 pixels, 1 bit per pixel, Range: '!' to '?'
 * HindiFont - Height: 31 pixels, 1 bit per pixel, Range: ' ' to '?'
 * ThaiFont - Height: 29 pixels, 1 bit per pixel, Range: '!' to 'â'
 *****************************************************************************/

/*****************************************************************************
 * SECTION:  RussianFont
 *****************************************************************************/

const XCHAR RussianStr[] = {   0x0022, 0x0026, 0x0029, 0x0024, 0x0025, 0x002A, 0x002B, 0x0025, 0x002C, 0x0028, 0x0020, 0x0023, 0x0027, 0x0029, 0x0020, 0x0021, 0x0000    };    // In Russian

/*****************************************************************************
 * SECTION:  ChineseFont
 *****************************************************************************/

const XCHAR ChineseStr[] = {   0x0022, 0x0023, 0x0021, 0x0024, 0x0020, 0x0000    };    // In Chinese

/*****************************************************************************
 * SECTION:  JapaneseFont
 *****************************************************************************/

const XCHAR JapaneseStr[] = {   0x0027, 0x0024, 0x0022, 0x0028, 0x0021, 0x0028, 0x0025, 0x0023, 0x0026, 0x0020, 0x0000    };    // In Japanese

/*****************************************************************************
 * SECTION:  KoreanFont
 *****************************************************************************/

const XCHAR KoreanStr[] = {   0x0027, 0x0023, 0x002B, 0x0026, 0x0029, 0x0021, 0x0026, 0x0022, 0x002A, 0x0028, 0x0024, 0x0025, 0x0020, 0x0000    };    // In Korean

/*****************************************************************************
 * SECTION:  HindiFont
 *****************************************************************************/

const XCHAR HindiStr[] = {   0x0024, 0x0023, 0x0028, 0x0025, 0x0020, 0x0027, 0x0020, 0x0022, 0x0026, 0x0029, 0x0026, 0x0021, 0x0000    };    // Hindi Font: Jaipur

/*****************************************************************************
 * SECTION:  ThaiFont
 *****************************************************************************/

const XCHAR ThaiStr[] = {   0x0026, 0x0025, 0x0027, 0x0026, 0x0023, 0x0029, 0x0022, 0x0028, 0x0025, 0x002A, 0x0024, 0x0021, 0x0020, 0x0000    };    // Thai Font: DB Thai Text

/*****************************************************************************
 * SECTION:  Fonts
 *****************************************************************************/

/*********************************
 * TTF Font File Structure
 * Label: RussianFont
 * Description:  Height: 25 pixels, 1 bit per pixel, Range: ' ' to '?'
 * Comment Block:
 * http://www.gnu.org/copyleft/gpl.html 
 *  
 * 		    GNU GENERAL PUBLIC LICENSE 
 * 		       Version 2, June 1991 
 *  
 * Copyright (C) 1989, 1991 Free Software Foundation, Inc. 
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA 
 * Everyone is permitted to copy and distribute verbatim copies 
 * of this license document, but changing it is not allowed. 
 *  
 * 			    Preamble 
 *  
 * The licenses for most software are designed to take away your 
 * freedom to share and change it.  By contrast, the GNU General Public 
 * License is intended to guarantee your freedom to share and change free 
 * software--to make sure the software is free for all its users.  This 
 * General Public License applies to most of the Free Software 
 * Foundation's software and to any other program whose authors commit to 
 * using it.  (Some other Free Software Foundation software is covered by 
 * the GNU Lesser General Public License instead.)  You can apply it to 
 * your programs, too. 
 *  
 * When we speak of free software, we are referring to freedom, not 
 * price.  Our General Public Licenses are designed to make sure that you 
 * have the freedom to distribute copies of free software (and charge for 
 * this service if you wish), that you receive source code or can get it 
 * if you want it, that you can change the software or use pieces of it 
 * in new free programs; and that you know you can do these things. 
 *  
 * To protect your rights, we need to make restrictions that forbid 
 * anyone to deny you these rights or to ask you to surrender the rights. 
 * These restrictions translate to certain responsibilities for you if you 
 * distribute copies of the software, or if you modify it. 
 *  
 * For example, if you distribute copies of such a program, whether 
 * gratis or for a fee, you must give the recipients all the rights that 
 * you have.  You must make sure that they, too, receive or can get the 
 * source code.  And you must show them these terms so they know their 
 * rights. 
 *  
 * We protect your rights with two steps: (1) copyright the software, and 
 * (2) offer you this license which gives you legal permission to copy, 
 * distribute and/or modify the software. 
 *  
 * Also, for each author's protection and ours, we want to make certain 
 * that everyone understands that there is no warranty for this free 
 * software.  If the software is modified by someone else and passed on, we 
 * want its recipients to know that what they have is not the original, so 
 * that any problems introduced by others will not reflect on the original 
 * authors' reputations. 
 *  
 * Finally, any free program is threatened constantly by software 
 * patents.  We wish to avoid the danger that redistributors of a free 
 * program will individually obtain patent licenses, in effect making the 
 * program proprietary.  To prevent this, we have made it clear that any 
 * patent must be licensed for everyone's free use or not licensed at all. 
 *  
 * The precise terms and conditions for copying, distribution and 
 * modification follow. 
 *  
 * 		    GNU GENERAL PUBLIC LICENSE 
 *    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION 
 *  
 * 0. This License applies to any program or other work which contains 
 * a notice placed by the copyright holder saying it may be distributed 
 * under the terms of this General Public License.  The "Program", below, 
 * refers to any such program or work, and a "work based on the Program" 
 * means either the Program or any derivative work under copyright law: 
 * that is to say, a work containing the Program or a portion of it, 
 * either verbatim or with modifications and/or translated into another 
 * language.  (Hereinafter, translation is included without limitation in 
 * the term "modification".)  Each licensee is addressed as "you". 
 *  
 * Activities other than copying, distribution and modification are not 
 * covered by this License; they are outside its scope.  The act of 
 * running the Program is not restricted, and the output from the Program 
 * is covered only if its contents constitute a work based on the 
 * Program (independent of having been made by running the Program). 
 * Whether that is true depends on what the Program does. 
 *  
 * 1. You may copy and distribute verbatim copies of the Program's 
 * source code as you receive it, in any medium, provided that you 
 * conspicuously and appropriately publish on each copy an appropriate 
 * copyright notice and disclaimer of warranty; keep intact all the 
 * notices that refer to this License and to the absence of any warranty; 
 * and give any other recipients of the Program a copy of this License 
 * along with the Program. 
 *  
 * You may charge a fee for the physical act of transferring a copy, and 
 * you may at your option offer warranty protection in exchange for a fee. 
 *  
 * 2. You may modify your copy or copies of the Program or any portion 
 * of it, thus forming a work based on the Program, and copy and 
 * distribute such modifications or work under the terms of Section 1 
 * above, provided that you also meet all of these conditions: 
 *  
 *     a) You must cause the modified files to carry prominent notices 
 *     stating that you changed the files and the date of any change. 
 *  
 *     b) You must cause any work that you distribute or publish, that in 
 *     whole or in part contains or is derived from the Program or any 
 *     part thereof, to be licensed as a whole at no charge to all third 
 *     parties under the terms of this License. 
 *  
 *     c) If the modified program normally reads commands interactively 
 *     when run, you must cause it, when started running for such 
 *     interactive use in the most ordinary way, to print or display an 
 *     announcement including an appropriate copyright notice and a 
 *     notice that there is no warranty (or else, saying that you provide 
 *     a warranty) and that users may redistribute the program under 
 *     these conditions, and telling the user how to view a copy of this 
 *     License.  (Exception: if the Program itself is interactive but 
 *     does not normally print such an announcement, your work based on 
 *     the Program is not required to print an announcement.) 
 *  
 * These requirements apply to the modified work as a whole.  If 
 * identifiable sections of that work are not derived from the Program, 
 * and can be reasonably considered independent and separate works in 
 * themselves, then this License, and its terms, do not apply to those 
 * sections when you distribute them as separate works.  But when you 
 * distribute the same sections as part of a whole which is a work based 
 * on the Program, the distribution of the whole must be on the terms of 
 * this License, whose permissions for other licensees extend to the 
 * entire whole, and thus to each and every part regardless of who wrote it. 
 *  
 * Thus, it is not the intent of this section to claim rights or contest 
 * your rights to work written entirely by you; rather, the intent is to 
 * exercise the right to control the distribution of derivative or 
 * collective works based on the Program. 
 *  
 * In addition, mere aggregation of another work not based on the Program 
 * with the Program (or with a work based on the Program) on a volume of 
 * a storage or distribution medium does not bring the other work under 
 * the scope of this License. 
 *  
 * 3. You may copy and distribute the Program (or a work based on it, 
 * under Section 2) in object code or executable form under the terms of 
 * Sections 1 and 2 above provided that you also do one of the following: 
 *  
 *     a) Accompany it with the complete corresponding machine-readable 
 *     source code, which must be distributed under the terms of Sections 
 *     1 and 2 above on a medium customarily used for software interchange; or, 
 *  
 *     b) Accompany it with a written offer, valid for at least three 
 *     years, to give any third party, for a charge no more than your 
 *     cost of physically performing source distribution, a complete 
 *     machine-readable copy of the corresponding source code, to be 
 *     distributed under the terms of Sections 1 and 2 above on a medium 
 *     customarily used for software interchange; or, 
 *  
 *     c) Accompany it with the information you received as to the offer 
 *     to distribute corresponding source code.  (This alternative is 
 *     allowed only for noncommercial distribution and only if you 
 *     received the program in object code or executable form with such 
 *     an offer, in accord with Subsection b above.) 
 *  
 * The source code for a work means the preferred form of the work for 
 * making modifications to it.  For an executable work, complete source 
 * code means all the source code for all modules it contains, plus any 
 * associated interface definition files, plus the scripts used to 
 * control compilation and installation of the executable.  However, as a 
 * special exception, the source code distributed need not include 
 * anything that is normally distributed (in either source or binary 
 * form) with the major components (compiler, kernel, and so on) of the 
 * operating system on which the executable runs, unless that component 
 * itself accompanies the executable. 
 *  
 * If distribution of executable or object code is made by offering 
 * access to copy from a designated place, then offering equivalent 
 * access to copy the source code from the same place counts as 
 * distribution of the source code, even though third parties are not 
 * compelled to copy the source along with the object code. 
 *  
 * 4. You may not copy, modify, sublicense, or distribute the Program 
 * except as expressly provided under this License.  Any attempt 
 * otherwise to copy, modify, sublicense or distribute the Program is 
 * void, and will automatically terminate your rights under this License. 
 * However, parties who have received copies, or rights, from you under 
 * this License will not have their licenses terminated so long as such 
 * parties remain in full compliance. 
 *  
 * 5. You are not required to accept this License, since you have not 
 * signed it.  However, nothing else grants you permission to modify or 
 * distribute the Program or its derivative works.  These actions are 
 * prohibited by law if you do not accept this License.  Therefore, by 
 * modifying or distributing the Program (or any work based on the 
 * Program), you indicate your acceptance of this License to do so, and 
 * all its terms and conditions for copying, distributing or modifying 
 * the Program or works based on it. 
 *  
 * 6. Each time you redistribute the Program (or any work based on the 
 * Program), the recipient automatically receives a license from the 
 * original licensor to copy, distribute or modify the Program subject to 
 * these terms and conditions.  You may not impose any further 
 * restrictions on the recipients' exercise of the rights granted herein. 
 * You are not responsible for enforcing compliance by third parties to 
 * this License. 
 *  
 * 7. If, as a consequence of a court judgment or allegation of patent 
 * infringement or for any other reason (not limited to patent issues), 
 * conditions are imposed on you (whether by court order, agreement or 
 * otherwise) that contradict the conditions of this License, they do not 
 * excuse you from the conditions of this License.  If you cannot 
 * distribute so as to satisfy simultaneously your obligations under this 
 * License and any other pertinent obligations, then as a consequence you 
 * may not distribute the Program at all.  For example, if a patent 
 * license would not permit royalty-free redistribution of the Program by 
 * all those who receive copies directly or indirectly through you, then 
 * the only way you could satisfy both it and this License would be to 
 * refrain entirely from distribution of the Program. 
 *  
 * If any portion of this section is held invalid or unenforceable under 
 * any particular circumstance, the balance of the section is intended to 
 * apply and the section as a whole is intended to apply in other 
 * circumstances. 
 *  
 * It is not the purpose of this section to induce you to infringe any 
 * patents or other property right claims or to contest validity of any 
 * such claims; this section has the sole purpose of protecting the 
 * integrity of the free software distribution system, which is 
 * implemented by public license practices.  Many people have made 
 * generous contributions to the wide range of software distributed 
 * through that system in reliance on consistent application of that 
 * system; it is up to the author/donor to decide if he or she is willing 
 * to distribute software through any other system and a licensee cannot 
 * impose that choice. 
 *  
 * This section is intended to make thoroughly clear what is believed to 
 * be a consequence of the rest of this License. 
 *  
 * 8. If the distribution and/or use of the Program is restricted in 
 * certain countries either by patents or by copyrighted interfaces, the 
 * original copyright holder who places the Program under this License 
 * may add an explicit geographical distribution limitation excluding 
 * those countries, so that distribution is permitted only in or among 
 * countries not thus excluded.  In such case, this License incorporates 
 * the limitation as if written in the body of this License. 
 *  
 * 9. The Free Software Foundation may publish revised and/or new versions 
 * of the General Public License from time to time.  Such new versions will 
 * be similar in spirit to the present version, but may differ in detail to 
 * address new problems or concerns. 
 *  
 * Each version is given a distinguishing version number.  If the Program 
 * specifies a version number of this License which applies to it and "any 
 * later version", you have the option of following the terms and conditions 
 * either of that version or of any later version published by the Free 
 * Software Foundation.  If the Program does not specify a version number of 
 * this License, you may choose any version ever published by the Free Software 
 * Foundation. 
 *  
 * 10. If you wish to incorporate parts of the Program into other free 
 * programs whose distribution conditions are different, write to the author 
 * to ask for permission.  For software which is copyrighted by the Free 
 * Software Foundation, write to the Free Software Foundation; we sometimes 
 * make exceptions for this.  Our decision will be guided by the two goals 
 * of preserving the free status of all derivatives of our free software and 
 * of promoting the sharing and reuse of software generally. 
 *  
 * 			    NO WARRANTY 
 *  
 * 11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY 
 * FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN 
 * OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES 
 * PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED 
 * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS 
 * TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE 
 * PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, 
 * REPAIR OR CORRECTION. 
 *  
 * 12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING 
 * WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR 
 * REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, 
 * INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING 
 * OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED 
 * TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY 
 * YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER 
 * PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGES. 
 *  
 * 		     END OF TERMS AND CONDITIONS 
 *  
 * 	    How to Apply These Terms to Your New Programs 
 *  
 *   If you develop a new program, and you want it to be of the greatest 
 * possible use to the public, the best way to achieve this is to make it 
 * free software which everyone can redistribute and change under these terms. 
 *  
 *   To do so, attach the following notices to the program.  It is safest 
 * to attach them to the start of each source file to most effectively 
 * convey the exclusion of warranty; and each file should have at least 
 * the "copyright" line and a pointer to where the full notice is found. 
 *  
 *     <one line to give the program's name and a brief idea of what it does.> 
 *     Copyright (C) <year>  <name of author> 
 *  
 *     This program is free software; you can redistribute it and/or modify 
 *     it under the terms of the GNU General Public License as published by 
 *     the Free Software Foundation; either version 2 of the License, or 
 *     (at your option) any later version. 
 *  
 *     This program is distributed in the hope that it will be useful, 
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *     GNU General Public License for more details. 
 *  
 *     You should have received a copy of the GNU General Public License 
 *     along with this program; if not, write to the Free Software 
 *     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 
 *  USA 
 *  
 *  
 * Also add information on how to contact you by electronic and paper mail. 
 *  
 * If the program is interactive, make it output a short notice like this 
 * when it starts in an interactive mode: 
 *  
 *     Gnomovision version 69, Copyright (C) year name of author 
 *     Gnomovision comes with ABSOLUTELY NO WARRANTY; for details type `show w'. 
 *     This is free software, and you are welcome to redistribute it 
 *     under certain conditions; type `show c' for details. 
 *  
 * The hypothetical commands `show w' and `show c' should show the appropriate 
 * parts of the General Public License.  Of course, the commands you use may 
 * be called something other than `show w' and `show c'; they could even be 
 * mouse-clicks or menu items--whatever suits your program. 
 *  
 * You should also get your employer (if you work as a programmer) or your 
 * school, if any, to sign a "copyright disclaimer" for the program, if 
 * necessary.  Here is a sample; alter the names: 
 *  
 *   Yoyodyne, Inc., hereby disclaims all copyright interest in the program 
 *   `Gnomovision' (which makes passes at compilers) written by James Hacker. 
 *  
 *   <signature of Ty Coon>, 1 April 1989 
 *   Ty Coon, President of Vice 
 *  
 * This General Public License does not permit incorporating your program into 
 * proprietary programs.  If your program is a subroutine library, you may 
 * consider it more useful to permit linking proprietary applications with the 
 * library.  If this is what you want to do, use the GNU Lesser General 
 * Public License instead of this License. 
 ***********************************/

extern const char __RussianFont[] __attribute__((aligned(2)));

const FONT_FLASH RussianFont =
{
    (FLASH | COMP_NONE),
    (GFX_FONT_SPACE char *)__RussianFont
};

const char __RussianFont[] __attribute__((aligned(2))) =
{
/****************************************
 * Font header
 ****************************************/
    0x00,           // Font ID
    0x00,           // Font information:  1 bit per pixel, Characters zero degress rotation
    0x20, 0x00,     // First Character
    0x2C, 0x00,     // Last Character
    0x19, 0x00,     // Height
/****************************************
 * Font Glyph Table
 ****************************************/
    0x05,              // width of the glyph
    0x3C, 0x00, 0x00,       // Character - 32, offset (0x0000003C)
    0x04,              // width of the glyph
    0x55, 0x00, 0x00,       // Character - 33, offset (0x00000055)
    0x0D,              // width of the glyph
    0x6E, 0x00, 0x00,       // Character - 1047, offset (0x0000006E)
    0x12,              // width of the glyph
    0xA0, 0x00, 0x00,       // Character - 1052, offset (0x000000A0)
    0x0B,              // width of the glyph
    0xEB, 0x00, 0x00,       // Character - 1072, offset (0x000000EB)
    0x0B,              // width of the glyph
    0x1D, 0x01, 0x00,       // Character - 1074, offset (0x0000011D)
    0x0B,              // width of the glyph
    0x4F, 0x01, 0x00,       // Character - 1076, offset (0x0000014F)
    0x0C,              // width of the glyph
    0x81, 0x01, 0x00,       // Character - 1080, offset (0x00000181)
    0x0C,              // width of the glyph
    0xB3, 0x01, 0x00,       // Character - 1081, offset (0x000001B3)
    0x0B,              // width of the glyph
    0xE5, 0x01, 0x00,       // Character - 1088, offset (0x000001E5)
    0x09,              // width of the glyph
    0x17, 0x02, 0x00,       // Character - 1089, offset (0x00000217)
    0x0A,              // width of the glyph
    0x49, 0x02, 0x00,       // Character - 1090, offset (0x00000249)
    0x0A,              // width of the glyph
    0x7B, 0x02, 0x00,       // Character - 1091, offset (0x0000027B)
/***********************************
 * Font Characters
 ***********************************/
/***********************************
 * Character - 32
 ***********************************/
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         

/***********************************
 * Character - 33
 ***********************************/
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x06,         //  **     
    0x06,         //  **     
    0x06,         //  **     
    0x06,         //  **     
    0x06,         //  **     
    0x06,         //  **     
    0x06,         //  **     
    0x02,         //  *      
    0x02,         //  *      
    0x02,         //  *      
    0x00,         //         
    0x06,         //  **     
    0x0E,         //  ***    
    0x0E,         //  ***    
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         

/***********************************
 * Character - 1047
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xE4, 0x01,         //   *  ****       
    0x1C, 0x03,         //   ***   **      
    0x0C, 0x06,         //   **     **     
    0x04, 0x06,         //   *      **     
    0x04, 0x06,         //   *      **     
    0x00, 0x06,         //          **     
    0x80, 0x03,         //        ***      
    0xF0, 0x01,         //     *****       
    0x00, 0x07,         //         ***     
    0x0C, 0x0E,         //   **     ***    
    0x0E, 0x0E,         //  ***     ***    
    0x0E, 0x0E,         //  ***     ***    
    0x02, 0x0E,         //  *       ***    
    0x04, 0x07,         //   *     ***     
    0xF8, 0x01,         //    ******       
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 1052
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x3E, 0xE0, 0x03,         //  *****       *****      
    0x38, 0xE0, 0x00,         //    ***       ***        
    0x78, 0xE0, 0x00,         //    ****      ***        
    0x68, 0xD0, 0x00,         //    * **     * **        
    0x68, 0xD0, 0x00,         //    * **     * **        
    0xE8, 0xD0, 0x00,         //    * ***    * **        
    0xC8, 0xC8, 0x00,         //    *  **   *  **        
    0xC8, 0xC8, 0x00,         //    *  **   *  **        
    0xC8, 0xC9, 0x00,         //    *  ***  *  **        
    0x88, 0xC5, 0x00,         //    *   ** *   **        
    0x88, 0xC5, 0x00,         //    *   ** *   **        
    0x08, 0xC7, 0x00,         //    *    ***   **        
    0x08, 0xC3, 0x00,         //    *    **    **        
    0x1C, 0xC3, 0x00,         //   ***   **    **        
    0x3E, 0xF2, 0x03,         //  *****   *  ******      
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 1072
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xF8, 0x00,         //    *****        
    0x86, 0x01,         //  **    **       
    0x86, 0x01,         //  **    **       
    0x86, 0x01,         //  **    **       
    0xF0, 0x01,         //     *****       
    0x8C, 0x01,         //   **   **       
    0x86, 0x01,         //  **    **       
    0x83, 0x01,         // **     **       
    0xC6, 0x05,         //  **   *** *     
    0x3E, 0x03,         //  *****  **      
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 1074
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xFF, 0x01,         // *********       
    0x8C, 0x01,         //   **   **       
    0x0C, 0x01,         //   **    *       
    0x8C, 0x01,         //   **   **       
    0x7C, 0x00,         //   *****         
    0x8C, 0x03,         //   **   ***      
    0x0C, 0x03,         //   **    **      
    0x0C, 0x03,         //   **    **      
    0xFF, 0x01,         // *********       
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 1076
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xFC, 0x03,         //   ********      
    0x98, 0x01,         //    **  **       
    0x88, 0x01,         //    *   **       
    0x88, 0x01,         //    *   **       
    0x88, 0x01,         //    *   **       
    0x88, 0x01,         //    *   **       
    0x84, 0x01,         //   *    **       
    0x84, 0x01,         //   *    **       
    0xFF, 0x03,         // **********      
    0x03, 0x03,         // **      **      
    0x01, 0x02,         // *        *      
    0x01, 0x02,         // *        *      
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 1080
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xFF, 0x0F,         // ************    
    0x0C, 0x03,         //   **    **      
    0x8C, 0x03,         //   **   ***      
    0xCC, 0x03,         //   **  ****      
    0x6C, 0x03,         //   ** ** **      
    0x3C, 0x03,         //   ****  **      
    0x1C, 0x03,         //   ***   **      
    0x0C, 0x03,         //   **    **      
    0xFF, 0x0F,         // ************    
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 1081
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x98, 0x01,         //    **  **       
    0x98, 0x01,         //    **  **       
    0xF0, 0x00,         //     ****        
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xFF, 0x0F,         // ************    
    0x0C, 0x03,         //   **    **      
    0x8C, 0x03,         //   **   ***      
    0xCC, 0x03,         //   **  ****      
    0x6C, 0x03,         //   ** ** **      
    0x3C, 0x03,         //   ****  **      
    0x1C, 0x03,         //   ***   **      
    0x0C, 0x03,         //   **    **      
    0xFF, 0x0F,         // ************    
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 1088
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xEF, 0x00,         // **** ***        
    0x9C, 0x01,         //   ***  **       
    0x0C, 0x03,         //   **    **      
    0x0C, 0x03,         //   **    **      
    0x0C, 0x03,         //   **    **      
    0x0C, 0x03,         //   **    **      
    0x0C, 0x03,         //   **    **      
    0x0C, 0x03,         //   **    **      
    0x9C, 0x01,         //   ***  **       
    0xEC, 0x00,         //   ** ***        
    0x0C, 0x00,         //   **            
    0x0C, 0x00,         //   **            
    0x0C, 0x00,         //   **            
    0x1F, 0x00,         // *****           
    0x00, 0x00,         //                 

/***********************************
 * Character - 1089
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xF0, 0x00,         //     ****        
    0x8C, 0x01,         //   **   **       
    0xC4, 0x01,         //   *   ***       
    0x86, 0x01,         //  **    **       
    0x06, 0x00,         //  **             
    0x06, 0x00,         //  **             
    0x06, 0x00,         //  **             
    0x06, 0x01,         //  **     *       
    0x8C, 0x01,         //   **   **       
    0x78, 0x00,         //    ****         
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 1090
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xFE, 0x01,         //  ********       
    0x33, 0x03,         // **  **  **      
    0x31, 0x02,         // *   **   *      
    0x31, 0x02,         // *   **   *      
    0x30, 0x00,         //     **          
    0x30, 0x00,         //     **          
    0x30, 0x00,         //     **          
    0x30, 0x00,         //     **          
    0xFC, 0x00,         //   ******        
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 1091
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xCF, 0x03,         // ****  ****      
    0x84, 0x01,         //   *    **       
    0x8C, 0x00,         //   **   *        
    0x8C, 0x00,         //   **   *        
    0x48, 0x00,         //    *  *         
    0x58, 0x00,         //    ** *         
    0x30, 0x00,         //     **          
    0x30, 0x00,         //     **          
    0x30, 0x00,         //     **          
    0x10, 0x00,         //     *           
    0x13, 0x00,         // **  *           
    0x0B, 0x00,         // ** *            
    0x07, 0x00,         // ***             
    0x00, 0x00,         //                 

};

/*********************************
 * TTF Font File Structure
 * Label: ChineseFont
 * Description:  Height: 22 pixels, 1 bit per pixel, Range: '!' to '?'
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

extern const char __ChineseFont[] __attribute__((aligned(2)));

const FONT_FLASH ChineseFont =
{
    (FLASH | COMP_NONE),
    (GFX_FONT_SPACE char *)__ChineseFont
};

const char __ChineseFont[] __attribute__((aligned(2))) =
{
/****************************************
 * Font header
 ****************************************/
    0x00,           // Font ID
    0x00,           // Font information:  1 bit per pixel, Characters zero degress rotation
    0x20, 0x00,     // First Character
    0x24, 0x00,     // Last Character
    0x16, 0x00,     // Height
/****************************************
 * Font Glyph Table
 ****************************************/
    0x07,              // width of the glyph
    0x1C, 0x00, 0x00,       // Character - 33, offset (0x0000001C)
    0x15,              // width of the glyph
    0x32, 0x00, 0x00,       // Character - 19990, offset (0x00000032)
    0x15,              // width of the glyph
    0x74, 0x00, 0x00,       // Character - 20320, offset (0x00000074)
    0x15,              // width of the glyph
    0xB6, 0x00, 0x00,       // Character - 22909, offset (0x000000B6)
    0x15,              // width of the glyph
    0xF8, 0x00, 0x00,       // Character - 30028, offset (0x000000F8)
/***********************************
 * Font Characters
 ***********************************/
/***********************************
 * Character - 33
 ***********************************/
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x1C,         //   ***   
    0x1C,         //   ***   
    0x1C,         //   ***   
    0x1C,         //   ***   
    0x1C,         //   ***   
    0x18,         //    **   
    0x18,         //    **   
    0x0C,         //   **    
    0x0C,         //   **    
    0x0C,         //   **    
    0x0C,         //   **    
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x1C,         //   ***   
    0x1C,         //   ***   
    0x00,         //         
    0x00,         //         
    0x00,         //         

/***********************************
 * Character - 19990
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x63, 0x00,         //         **   **         
    0x00, 0xCE, 0x01,         //          ***  ***       
    0x70, 0xC6, 0x00,         //     ***  **   **        
    0x30, 0xC6, 0x00,         //     **   **   **        
    0x30, 0xC6, 0x00,         //     **   **   **        
    0x30, 0xC6, 0x06,         //     **   **   ** **     
    0xFE, 0xFF, 0x0B,         //  ***************** *    
    0x30, 0xC6, 0x00,         //     **   **   **        
    0x30, 0xC6, 0x00,         //     **   **   **        
    0x30, 0xC6, 0x00,         //     **   **   **        
    0x30, 0xC6, 0x00,         //     **   **   **        
    0x30, 0xC6, 0x00,         //     **   **   **        
    0x30, 0xFE, 0x00,         //     **   *******        
    0x30, 0xC6, 0x00,         //     **   **   **        
    0x30, 0x00, 0x00,         //     **                  
    0x30, 0x00, 0x00,         //     **                  
    0x30, 0x00, 0x07,         //     **          ***     
    0xF8, 0xFF, 0x01,         //    **************       
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 20320
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x60, 0x0C, 0x00,         //      **   **            
    0xE0, 0x1C, 0x00,         //      ***  ***           
    0x60, 0x0C, 0x00,         //      **   **            
    0x60, 0x06, 0x00,         //      **  **             
    0x30, 0x06, 0x0E,         //     **   **      ***    
    0x30, 0xFE, 0x07,         //     **   **********     
    0x78, 0x33, 0x03,         //    **** **  **  **      
    0x2C, 0xB1, 0x01,         //   ** *  *   ** **       
    0xAC, 0x21, 0x00,         //   ** * **    *          
    0x26, 0xAC, 0x00,         //  **  *    ** * *        
    0x36, 0xA6, 0x01,         //  ** **   **  * **       
    0x30, 0x36, 0x03,         //     **   ** **  **      
    0x30, 0x33, 0x06,         //     **  **  **   **     
    0x30, 0x33, 0x0E,         //     **  **  **   ***    
    0xB0, 0x31, 0x0C,         //     ** **   **    **    
    0xB0, 0x30, 0x06,         //     ** *    **   **     
    0x30, 0x30, 0x00,         //     **      **          
    0x30, 0x3E, 0x00,         //     **   *****          
    0x30, 0x18, 0x00,         //     **     **           
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 22909
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x60, 0x00, 0x00,         //      **                 
    0x70, 0x00, 0x07,         //     ***         ***     
    0x30, 0xFF, 0x07,         //     **  ***********     
    0x30, 0x80, 0x01,         //     **         **       
    0xB6, 0xC1, 0x00,         //  ** ** **     **        
    0xFC, 0x63, 0x00,         //   ********   **         
    0xB0, 0xE1, 0x00,         //     ** **    ***        
    0x98, 0x61, 0x00,         //    **  **    **         
    0x98, 0x61, 0x06,         //    **  **    **  **     
    0xD8, 0xFE, 0x0F,         //    ** ** ***********    
    0xD8, 0x60, 0x00,         //    ** **     **         
    0x8C, 0x60, 0x00,         //   **   *     **         
    0xB8, 0x60, 0x00,         //    *** *     **         
    0xE0, 0x60, 0x00,         //      ***     **         
    0xB0, 0x61, 0x00,         //     ** **    **         
    0x98, 0x61, 0x00,         //    **  **    **         
    0x8C, 0x63, 0x00,         //   **   ***   **         
    0x06, 0x78, 0x00,         //  **        ****         
    0x00, 0x30, 0x00,         //             **          
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 30028
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x18, 0x80, 0x01,         //    **          **       
    0xF0, 0xFF, 0x03,         //     **************      
    0x30, 0x86, 0x01,         //     **   **    **       
    0x30, 0x86, 0x01,         //     **   **    **       
    0xF0, 0xFF, 0x01,         //     *************       
    0x30, 0x86, 0x01,         //     **   **    **       
    0x30, 0x86, 0x01,         //     **   **    **       
    0xF0, 0xFF, 0x01,         //     *************       
    0x00, 0x97, 0x00,         //         *** *  *        
    0x80, 0x31, 0x00,         //        **   **          
    0xC0, 0xE0, 0x00,         //       **     ***        
    0xE0, 0x71, 0x0F,         //      ****   *** ****    
    0xB8, 0x33, 0x06,         //    *** ***  **   **     
    0x8E, 0x31, 0x00,         //  ***   **   **          
    0x80, 0x31, 0x00,         //        **   **          
    0xC0, 0x30, 0x00,         //       **    **          
    0xE0, 0x30, 0x00,         //      ***    **          
    0x30, 0x30, 0x00,         //     **      **          
    0x1C, 0x30, 0x00,         //   ***       **          
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

};

/*********************************
 * TTF Font File Structure
 * Label: JapaneseFont
 * Description:  Height: 25 pixels, 1 bit per pixel, Range: '!' to '?'
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

extern const char __JapaneseFont[] __attribute__((aligned(2)));

const FONT_FLASH JapaneseFont =
{
    (FLASH | COMP_NONE),
    (GFX_FONT_SPACE char *)__JapaneseFont
};

const char __JapaneseFont[] __attribute__((aligned(2))) =
{
/****************************************
 * Font header
 ****************************************/
    0x00,           // Font ID
    0x00,           // Font information:  1 bit per pixel, Characters zero degress rotation
    0x20, 0x00,     // First Character
    0x28, 0x00,     // Last Character
    0x19, 0x00,     // Height
/****************************************
 * Font Glyph Table
 ****************************************/
    0x08,              // width of the glyph
    0x2C, 0x00, 0x00,       // Character - 33, offset (0x0000002C)
    0x17,              // width of the glyph
    0x45, 0x00, 0x00,       // Character - 12371, offset (0x00000045)
    0x17,              // width of the glyph
    0x90, 0x00, 0x00,       // Character - 12373, offset (0x00000090)
    0x17,              // width of the glyph
    0xDB, 0x00, 0x00,       // Character - 12385, offset (0x000000DB)
    0x17,              // width of the glyph
    0x26, 0x01, 0x00,       // Character - 12394, offset (0x00000126)
    0x17,              // width of the glyph
    0x71, 0x01, 0x00,       // Character - 12395, offset (0x00000171)
    0x17,              // width of the glyph
    0xBC, 0x01, 0x00,       // Character - 12399, offset (0x000001BC)
    0x17,              // width of the glyph
    0x07, 0x02, 0x00,       // Character - 12415, offset (0x00000207)
    0x17,              // width of the glyph
    0x52, 0x02, 0x00,       // Character - 12435, offset (0x00000252)
/***********************************
 * Font Characters
 ***********************************/
/***********************************
 * Character - 33
 ***********************************/
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x18,         //    **   
    0x18,         //    **   
    0x18,         //    **   
    0x18,         //    **   
    0x18,         //    **   
    0x18,         //    **   
    0x18,         //    **   
    0x18,         //    **   
    0x18,         //    **   
    0x18,         //    **   
    0x18,         //    **   
    0x18,         //    **   
    0x18,         //    **   
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x3C,         //   ****  
    0x1C,         //   ***   
    0x00,         //         
    0x00,         //         
    0x00,         //         

/***********************************
 * Character - 12371
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x60, 0x00, 0x00,         //      **                 
    0xC0, 0xFF, 0x01,         //       ***********       
    0x00, 0x30, 0x00,         //             **          
    0x00, 0x18, 0x00,         //            **           
    0x00, 0x0E, 0x00,         //          ***            
    0x00, 0x03, 0x00,         //         **              
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0xC0, 0x00, 0x00,         //       **                
    0x60, 0x00, 0x00,         //      **                 
    0x60, 0x00, 0x00,         //      **                 
    0x60, 0x00, 0x00,         //      **                 
    0x60, 0x00, 0x00,         //      **                 
    0xC0, 0xFF, 0x01,         //       ***********       
    0x00, 0x1E, 0x03,         //          ****   **      
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 12373
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x80, 0x07, 0x00,         //        ****             
    0x00, 0x06, 0x00,         //          **             
    0x00, 0x0E, 0x00,         //          ***            
    0x00, 0xEC, 0x00,         //           ** ***        
    0x00, 0x3C, 0x00,         //           ****          
    0x00, 0x1E, 0x00,         //          ****           
    0xE0, 0x1B, 0x00,         //      ***** **           
    0x00, 0x30, 0x00,         //             **          
    0x00, 0x20, 0x00,         //              *          
    0x00, 0x6C, 0x00,         //           ** **         
    0x80, 0xF7, 0x00,         //        **** ****        
    0xC0, 0xC0, 0x00,         //       **      **        
    0xE0, 0x00, 0x00,         //      ***                
    0x60, 0x00, 0x00,         //      **                 
    0xC0, 0x00, 0x00,         //       **                
    0xC0, 0x01, 0x00,         //       ***               
    0x80, 0x7F, 0x00,         //        ********         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 12385
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x03, 0x00,         //         **              
    0x00, 0x0E, 0x00,         //          ***            
    0x00, 0x0E, 0x00,         //          ***            
    0x00, 0xE6, 0x00,         //          **  ***        
    0x30, 0x3E, 0x00,         //     **   *****          
    0xE0, 0x07, 0x00,         //      ******             
    0x00, 0x03, 0x00,         //         **              
    0x00, 0x03, 0x00,         //         **              
    0x80, 0xF1, 0x00,         //        **   ****        
    0x80, 0x9D, 0x01,         //        ** ***  **       
    0x80, 0x07, 0x03,         //        ****     **      
    0xC0, 0x01, 0x03,         //       ***       **      
    0x80, 0x01, 0x03,         //        **       **      
    0x00, 0x00, 0x03,         //                 **      
    0x00, 0x80, 0x03,         //                ***      
    0x00, 0xC0, 0x01,         //               ***       
    0x00, 0xE0, 0x00,         //              ***        
    0x80, 0x3F, 0x00,         //        *******          
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 12394
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x06, 0x00,         //          **             
    0x00, 0x0C, 0x00,         //           **            
    0x00, 0x06, 0x00,         //          **             
    0x30, 0x3E, 0x00,         //     **   *****          
    0xE0, 0x0F, 0x00,         //      *******            
    0x00, 0xE3, 0x03,         //         **   *****      
    0x80, 0x81, 0x0F,         //        **      *****    
    0x80, 0xE1, 0x00,         //        **    ***        
    0xC0, 0x60, 0x00,         //       **     **         
    0xC0, 0x60, 0x00,         //       **     **         
    0x60, 0x60, 0x00,         //      **      **         
    0x60, 0x60, 0x00,         //      **      **         
    0x30, 0x7E, 0x00,         //     **   ******         
    0xB0, 0xE3, 0x01,         //     ** ***   ****       
    0x80, 0x61, 0x03,         //        **    ** **      
    0x00, 0x73, 0x03,         //         **  *** **      
    0x00, 0x3E, 0x00,         //          *****          
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 12395
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x70, 0x00, 0x00,         //     ***                 
    0x60, 0x00, 0x00,         //      **                 
    0xE0, 0xDE, 0x07,         //      *** **** *****     
    0x60, 0x70, 0x00,         //      **     ***         
    0x30, 0x00, 0x00,         //     **                  
    0x30, 0x00, 0x00,         //     **                  
    0x30, 0x00, 0x00,         //     **                  
    0x30, 0x00, 0x00,         //     **                  
    0x8C, 0x00, 0x00,         //   **   *                
    0x6C, 0x00, 0x00,         //   ** **                 
    0x6C, 0x06, 0x00,         //   ** **  **             
    0x7C, 0x06, 0x00,         //   *****  **             
    0x3C, 0x0C, 0x00,         //   ****    **            
    0x30, 0xF8, 0x07,         //     **     ********     
    0x30, 0x00, 0x00,         //     **                  
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 12399
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x30, 0xF0, 0x00,         //     **      ****        
    0x60, 0xC0, 0x01,         //      **       ***       
    0x60, 0xC0, 0x00,         //      **       **        
    0x60, 0xC0, 0x06,         //      **       ** **     
    0x30, 0xF7, 0x01,         //     **  *** *****       
    0x30, 0xDC, 0x00,         //     **    *** **        
    0x30, 0xC0, 0x00,         //     **        **        
    0x30, 0xC0, 0x00,         //     **        **        
    0x18, 0xC0, 0x00,         //    **         **        
    0xD8, 0xC0, 0x00,         //    ** **      **        
    0x58, 0xC0, 0x00,         //    ** *       **        
    0x38, 0xFC, 0x00,         //    ***    ******        
    0x38, 0xC7, 0x07,         //    ***  ***   *****     
    0x38, 0xC3, 0x0E,         //    ***  **    ** ***    
    0x30, 0xFE, 0x0C,         //     **   *******  **    
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 12415
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x30, 0x00,         //             **          
    0xC0, 0x78, 0x00,         //       **   ****         
    0x80, 0x2F, 0x00,         //        ***** *          
    0x00, 0x38, 0x00,         //            ***          
    0x00, 0x18, 0x00,         //            **           
    0x00, 0xD8, 0x00,         //            ** **        
    0x00, 0x8C, 0x03,         //           **   ***      
    0x00, 0x0C, 0x03,         //           **    **      
    0x00, 0x06, 0x03,         //          **     **      
    0xE0, 0x7F, 0x01,         //      ********** *       
    0x30, 0xC3, 0x01,         //     **  **    ***       
    0xB8, 0xC1, 0x07,         //    *** **     *****     
    0x98, 0xC0, 0x0E,         //    **  *      ** ***    
    0xD8, 0x60, 0x18,         //    ** **     **    **   
    0x38, 0x30, 0x00,         //    ***      **          
    0x00, 0x1C, 0x00,         //           ***           
    0x00, 0x07, 0x00,         //         ***             
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 12435
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x03, 0x00,         //         **              
    0x00, 0x0E, 0x00,         //          ***            
    0x00, 0x0C, 0x00,         //           **            
    0x00, 0x06, 0x00,         //          **             
    0x00, 0x07, 0x00,         //         ***             
    0x00, 0x03, 0x00,         //         **              
    0x80, 0x01, 0x00,         //        **               
    0x80, 0x01, 0x00,         //        **               
    0xC0, 0x1C, 0x00,         //       **  ***           
    0xE0, 0x3B, 0x00,         //      ***** ***          
    0xE0, 0x30, 0x0C,         //      ***    **    **    
    0x70, 0x30, 0x0C,         //     ***     **    **    
    0x30, 0x18, 0x06,         //     **     **    **     
    0x38, 0x18, 0x06,         //    ***     **    **     
    0x18, 0x18, 0x03,         //    **      **   **      
    0x18, 0xB8, 0x01,         //    **      *** **       
    0x18, 0x60, 0x00,         //    **        **         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

};

/*********************************
 * TTF Font File Structure
 * Label: KoreanFont
 * Description:  Height: 23 pixels, 1 bit per pixel, Range: '!' to '?'
 * Comment Block:
 * http://www.gnu.org/copyleft/gpl.html 
 *  
 * 		    GNU GENERAL PUBLIC LICENSE 
 * 		       Version 2, June 1991 
 *  
 * Copyright (C) 1989, 1991 Free Software Foundation, Inc. 
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA 
 * Everyone is permitted to copy and distribute verbatim copies 
 * of this license document, but changing it is not allowed. 
 *  
 * 			    Preamble 
 *  
 * The licenses for most software are designed to take away your 
 * freedom to share and change it.  By contrast, the GNU General Public 
 * License is intended to guarantee your freedom to share and change free 
 * software--to make sure the software is free for all its users.  This 
 * General Public License applies to most of the Free Software 
 * Foundation's software and to any other program whose authors commit to 
 * using it.  (Some other Free Software Foundation software is covered by 
 * the GNU Lesser General Public License instead.)  You can apply it to 
 * your programs, too. 
 *  
 * When we speak of free software, we are referring to freedom, not 
 * price.  Our General Public Licenses are designed to make sure that you 
 * have the freedom to distribute copies of free software (and charge for 
 * this service if you wish), that you receive source code or can get it 
 * if you want it, that you can change the software or use pieces of it 
 * in new free programs; and that you know you can do these things. 
 *  
 * To protect your rights, we need to make restrictions that forbid 
 * anyone to deny you these rights or to ask you to surrender the rights. 
 * These restrictions translate to certain responsibilities for you if you 
 * distribute copies of the software, or if you modify it. 
 *  
 * For example, if you distribute copies of such a program, whether 
 * gratis or for a fee, you must give the recipients all the rights that 
 * you have.  You must make sure that they, too, receive or can get the 
 * source code.  And you must show them these terms so they know their 
 * rights. 
 *  
 * We protect your rights with two steps: (1) copyright the software, and 
 * (2) offer you this license which gives you legal permission to copy, 
 * distribute and/or modify the software. 
 *  
 * Also, for each author's protection and ours, we want to make certain 
 * that everyone understands that there is no warranty for this free 
 * software.  If the software is modified by someone else and passed on, we 
 * want its recipients to know that what they have is not the original, so 
 * that any problems introduced by others will not reflect on the original 
 * authors' reputations. 
 *  
 * Finally, any free program is threatened constantly by software 
 * patents.  We wish to avoid the danger that redistributors of a free 
 * program will individually obtain patent licenses, in effect making the 
 * program proprietary.  To prevent this, we have made it clear that any 
 * patent must be licensed for everyone's free use or not licensed at all. 
 *  
 * The precise terms and conditions for copying, distribution and 
 * modification follow. 
 *  
 * 		    GNU GENERAL PUBLIC LICENSE 
 *    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION 
 *  
 * 0. This License applies to any program or other work which contains 
 * a notice placed by the copyright holder saying it may be distributed 
 * under the terms of this General Public License.  The "Program", below, 
 * refers to any such program or work, and a "work based on the Program" 
 * means either the Program or any derivative work under copyright law: 
 * that is to say, a work containing the Program or a portion of it, 
 * either verbatim or with modifications and/or translated into another 
 * language.  (Hereinafter, translation is included without limitation in 
 * the term "modification".)  Each licensee is addressed as "you". 
 *  
 * Activities other than copying, distribution and modification are not 
 * covered by this License; they are outside its scope.  The act of 
 * running the Program is not restricted, and the output from the Program 
 * is covered only if its contents constitute a work based on the 
 * Program (independent of having been made by running the Program). 
 * Whether that is true depends on what the Program does. 
 *  
 * 1. You may copy and distribute verbatim copies of the Program's 
 * source code as you receive it, in any medium, provided that you 
 * conspicuously and appropriately publish on each copy an appropriate 
 * copyright notice and disclaimer of warranty; keep intact all the 
 * notices that refer to this License and to the absence of any warranty; 
 * and give any other recipients of the Program a copy of this License 
 * along with the Program. 
 *  
 * You may charge a fee for the physical act of transferring a copy, and 
 * you may at your option offer warranty protection in exchange for a fee. 
 *  
 * 2. You may modify your copy or copies of the Program or any portion 
 * of it, thus forming a work based on the Program, and copy and 
 * distribute such modifications or work under the terms of Section 1 
 * above, provided that you also meet all of these conditions: 
 *  
 *     a) You must cause the modified files to carry prominent notices 
 *     stating that you changed the files and the date of any change. 
 *  
 *     b) You must cause any work that you distribute or publish, that in 
 *     whole or in part contains or is derived from the Program or any 
 *     part thereof, to be licensed as a whole at no charge to all third 
 *     parties under the terms of this License. 
 *  
 *     c) If the modified program normally reads commands interactively 
 *     when run, you must cause it, when started running for such 
 *     interactive use in the most ordinary way, to print or display an 
 *     announcement including an appropriate copyright notice and a 
 *     notice that there is no warranty (or else, saying that you provide 
 *     a warranty) and that users may redistribute the program under 
 *     these conditions, and telling the user how to view a copy of this 
 *     License.  (Exception: if the Program itself is interactive but 
 *     does not normally print such an announcement, your work based on 
 *     the Program is not required to print an announcement.) 
 *  
 * These requirements apply to the modified work as a whole.  If 
 * identifiable sections of that work are not derived from the Program, 
 * and can be reasonably considered independent and separate works in 
 * themselves, then this License, and its terms, do not apply to those 
 * sections when you distribute them as separate works.  But when you 
 * distribute the same sections as part of a whole which is a work based 
 * on the Program, the distribution of the whole must be on the terms of 
 * this License, whose permissions for other licensees extend to the 
 * entire whole, and thus to each and every part regardless of who wrote it. 
 *  
 * Thus, it is not the intent of this section to claim rights or contest 
 * your rights to work written entirely by you; rather, the intent is to 
 * exercise the right to control the distribution of derivative or 
 * collective works based on the Program. 
 *  
 * In addition, mere aggregation of another work not based on the Program 
 * with the Program (or with a work based on the Program) on a volume of 
 * a storage or distribution medium does not bring the other work under 
 * the scope of this License. 
 *  
 * 3. You may copy and distribute the Program (or a work based on it, 
 * under Section 2) in object code or executable form under the terms of 
 * Sections 1 and 2 above provided that you also do one of the following: 
 *  
 *     a) Accompany it with the complete corresponding machine-readable 
 *     source code, which must be distributed under the terms of Sections 
 *     1 and 2 above on a medium customarily used for software interchange; or, 
 *  
 *     b) Accompany it with a written offer, valid for at least three 
 *     years, to give any third party, for a charge no more than your 
 *     cost of physically performing source distribution, a complete 
 *     machine-readable copy of the corresponding source code, to be 
 *     distributed under the terms of Sections 1 and 2 above on a medium 
 *     customarily used for software interchange; or, 
 *  
 *     c) Accompany it with the information you received as to the offer 
 *     to distribute corresponding source code.  (This alternative is 
 *     allowed only for noncommercial distribution and only if you 
 *     received the program in object code or executable form with such 
 *     an offer, in accord with Subsection b above.) 
 *  
 * The source code for a work means the preferred form of the work for 
 * making modifications to it.  For an executable work, complete source 
 * code means all the source code for all modules it contains, plus any 
 * associated interface definition files, plus the scripts used to 
 * control compilation and installation of the executable.  However, as a 
 * special exception, the source code distributed need not include 
 * anything that is normally distributed (in either source or binary 
 * form) with the major components (compiler, kernel, and so on) of the 
 * operating system on which the executable runs, unless that component 
 * itself accompanies the executable. 
 *  
 * If distribution of executable or object code is made by offering 
 * access to copy from a designated place, then offering equivalent 
 * access to copy the source code from the same place counts as 
 * distribution of the source code, even though third parties are not 
 * compelled to copy the source along with the object code. 
 *  
 * 4. You may not copy, modify, sublicense, or distribute the Program 
 * except as expressly provided under this License.  Any attempt 
 * otherwise to copy, modify, sublicense or distribute the Program is 
 * void, and will automatically terminate your rights under this License. 
 * However, parties who have received copies, or rights, from you under 
 * this License will not have their licenses terminated so long as such 
 * parties remain in full compliance. 
 *  
 * 5. You are not required to accept this License, since you have not 
 * signed it.  However, nothing else grants you permission to modify or 
 * distribute the Program or its derivative works.  These actions are 
 * prohibited by law if you do not accept this License.  Therefore, by 
 * modifying or distributing the Program (or any work based on the 
 * Program), you indicate your acceptance of this License to do so, and 
 * all its terms and conditions for copying, distributing or modifying 
 * the Program or works based on it. 
 *  
 * 6. Each time you redistribute the Program (or any work based on the 
 * Program), the recipient automatically receives a license from the 
 * original licensor to copy, distribute or modify the Program subject to 
 * these terms and conditions.  You may not impose any further 
 * restrictions on the recipients' exercise of the rights granted herein. 
 * You are not responsible for enforcing compliance by third parties to 
 * this License. 
 *  
 * 7. If, as a consequence of a court judgment or allegation of patent 
 * infringement or for any other reason (not limited to patent issues), 
 * conditions are imposed on you (whether by court order, agreement or 
 * otherwise) that contradict the conditions of this License, they do not 
 * excuse you from the conditions of this License.  If you cannot 
 * distribute so as to satisfy simultaneously your obligations under this 
 * License and any other pertinent obligations, then as a consequence you 
 * may not distribute the Program at all.  For example, if a patent 
 * license would not permit royalty-free redistribution of the Program by 
 * all those who receive copies directly or indirectly through you, then 
 * the only way you could satisfy both it and this License would be to 
 * refrain entirely from distribution of the Program. 
 *  
 * If any portion of this section is held invalid or unenforceable under 
 * any particular circumstance, the balance of the section is intended to 
 * apply and the section as a whole is intended to apply in other 
 * circumstances. 
 *  
 * It is not the purpose of this section to induce you to infringe any 
 * patents or other property right claims or to contest validity of any 
 * such claims; this section has the sole purpose of protecting the 
 * integrity of the free software distribution system, which is 
 * implemented by public license practices.  Many people have made 
 * generous contributions to the wide range of software distributed 
 * through that system in reliance on consistent application of that 
 * system; it is up to the author/donor to decide if he or she is willing 
 * to distribute software through any other system and a licensee cannot 
 * impose that choice. 
 *  
 * This section is intended to make thoroughly clear what is believed to 
 * be a consequence of the rest of this License. 
 *  
 * 8. If the distribution and/or use of the Program is restricted in 
 * certain countries either by patents or by copyrighted interfaces, the 
 * original copyright holder who places the Program under this License 
 * may add an explicit geographical distribution limitation excluding 
 * those countries, so that distribution is permitted only in or among 
 * countries not thus excluded.  In such case, this License incorporates 
 * the limitation as if written in the body of this License. 
 *  
 * 9. The Free Software Foundation may publish revised and/or new versions 
 * of the General Public License from time to time.  Such new versions will 
 * be similar in spirit to the present version, but may differ in detail to 
 * address new problems or concerns. 
 *  
 * Each version is given a distinguishing version number.  If the Program 
 * specifies a version number of this License which applies to it and "any 
 * later version", you have the option of following the terms and conditions 
 * either of that version or of any later version published by the Free 
 * Software Foundation.  If the Program does not specify a version number of 
 * this License, you may choose any version ever published by the Free Software 
 * Foundation. 
 *  
 * 10. If you wish to incorporate parts of the Program into other free 
 * programs whose distribution conditions are different, write to the author 
 * to ask for permission.  For software which is copyrighted by the Free 
 * Software Foundation, write to the Free Software Foundation; we sometimes 
 * make exceptions for this.  Our decision will be guided by the two goals 
 * of preserving the free status of all derivatives of our free software and 
 * of promoting the sharing and reuse of software generally. 
 *  
 * 			    NO WARRANTY 
 *  
 * 11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY 
 * FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN 
 * OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES 
 * PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED 
 * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS 
 * TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE 
 * PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, 
 * REPAIR OR CORRECTION. 
 *  
 * 12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING 
 * WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR 
 * REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, 
 * INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING 
 * OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED 
 * TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY 
 * YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER 
 * PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGES. 
 *  
 * 		     END OF TERMS AND CONDITIONS 
 *  
 * 	    How to Apply These Terms to Your New Programs 
 *  
 *   If you develop a new program, and you want it to be of the greatest 
 * possible use to the public, the best way to achieve this is to make it 
 * free software which everyone can redistribute and change under these terms. 
 *  
 *   To do so, attach the following notices to the program.  It is safest 
 * to attach them to the start of each source file to most effectively 
 * convey the exclusion of warranty; and each file should have at least 
 * the "copyright" line and a pointer to where the full notice is found. 
 *  
 *     <one line to give the program's name and a brief idea of what it does.> 
 *     Copyright (C) <year>  <name of author> 
 *  
 *     This program is free software; you can redistribute it and/or modify 
 *     it under the terms of the GNU General Public License as published by 
 *     the Free Software Foundation; either version 2 of the License, or 
 *     (at your option) any later version. 
 *  
 *     This program is distributed in the hope that it will be useful, 
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *     GNU General Public License for more details. 
 *  
 *     You should have received a copy of the GNU General Public License 
 *     along with this program; if not, write to the Free Software 
 *     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 
 *  USA 
 *  
 *  
 * Also add information on how to contact you by electronic and paper mail. 
 *  
 * If the program is interactive, make it output a short notice like this 
 * when it starts in an interactive mode: 
 *  
 *     Gnomovision version 69, Copyright (C) year name of author 
 *     Gnomovision comes with ABSOLUTELY NO WARRANTY; for details type `show w'. 
 *     This is free software, and you are welcome to redistribute it 
 *     under certain conditions; type `show c' for details. 
 *  
 * The hypothetical commands `show w' and `show c' should show the appropriate 
 * parts of the General Public License.  Of course, the commands you use may 
 * be called something other than `show w' and `show c'; they could even be 
 * mouse-clicks or menu items--whatever suits your program. 
 *  
 * You should also get your employer (if you work as a programmer) or your 
 * school, if any, to sign a "copyright disclaimer" for the program, if 
 * necessary.  Here is a sample; alter the names: 
 *  
 *   Yoyodyne, Inc., hereby disclaims all copyright interest in the program 
 *   `Gnomovision' (which makes passes at compilers) written by James Hacker. 
 *  
 *   <signature of Ty Coon>, 1 April 1989 
 *   Ty Coon, President of Vice 
 *  
 * This General Public License does not permit incorporating your program into 
 * proprietary programs.  If your program is a subroutine library, you may 
 * consider it more useful to permit linking proprietary applications with the 
 * library.  If this is what you want to do, use the GNU Lesser General 
 * Public License instead of this License. 
 ***********************************/

extern const char __KoreanFont[] __attribute__((aligned(2)));

const FONT_FLASH KoreanFont =
{
    (FLASH | COMP_NONE),
    (GFX_FONT_SPACE char *)__KoreanFont
};

const char __KoreanFont[] __attribute__((aligned(2))) =
{
/****************************************
 * Font header
 ****************************************/
    0x00,           // Font ID
    0x00,           // Font information:  1 bit per pixel, Characters zero degress rotation
    0x20, 0x00,     // First Character
    0x2B, 0x00,     // Last Character
    0x17, 0x00,     // Height
/****************************************
 * Font Glyph Table
 ****************************************/
    0x05,              // width of the glyph
    0x38, 0x00, 0x00,       // Character - 33, offset (0x00000038)
    0x06,              // width of the glyph
    0x4F, 0x00, 0x00,       // Character - 46, offset (0x0000004F)
    0x13,              // width of the glyph
    0x66, 0x00, 0x00,       // Character - 44228, offset (0x00000066)
    0x13,              // width of the glyph
    0xAB, 0x00, 0x00,       // Character - 45397, offset (0x000000AB)
    0x13,              // width of the glyph
    0xF0, 0x00, 0x00,       // Character - 47084, offset (0x000000F0)
    0x13,              // width of the glyph
    0x35, 0x01, 0x00,       // Character - 48516, offset (0x00000135)
    0x13,              // width of the glyph
    0x7A, 0x01, 0x00,       // Character - 49464, offset (0x0000017A)
    0x13,              // width of the glyph
    0xBF, 0x01, 0x00,       // Character - 50504, offset (0x000001BF)
    0x13,              // width of the glyph
    0x04, 0x02, 0x00,       // Character - 50668, offset (0x00000204)
    0x13,              // width of the glyph
    0x49, 0x02, 0x00,       // Character - 50836, offset (0x00000249)
    0x13,              // width of the glyph
    0x8E, 0x02, 0x00,       // Character - 51064, offset (0x0000028E)
    0x13,              // width of the glyph
    0xD3, 0x02, 0x00,       // Character - 54616, offset (0x000002D3)
/***********************************
 * Font Characters
 ***********************************/
/***********************************
 * Character - 33
 ***********************************/
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x0E,         //  ***    
    0x0E,         //  ***    
    0x0E,         //  ***    
    0x0E,         //  ***    
    0x0E,         //  ***    
    0x0E,         //  ***    
    0x0E,         //  ***    
    0x06,         //  **     
    0x06,         //  **     
    0x00,         //         
    0x00,         //         
    0x0E,         //  ***    
    0x0E,         //  ***    
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         

/***********************************
 * Character - 46
 ***********************************/
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x0E,         //  ***    
    0x0E,         //  ***    
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         

/***********************************
 * Character - 44228
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0xF0, 0x00,         //             ****        
    0x00, 0xDE, 0x00,         //          **** **        
    0x00, 0xD8, 0x00,         //            ** **        
    0x00, 0xD8, 0x00,         //            ** **        
    0xFC, 0xDB, 0x00,         //   ******** ** **        
    0x80, 0xDF, 0x00,         //        ****** **        
    0x80, 0xD9, 0x00,         //        **  ** **        
    0xC0, 0xD9, 0x00,         //       ***  ** **        
    0xC0, 0xD8, 0x00,         //       **   ** **        
    0x60, 0xDF, 0x00,         //      ** ***** **        
    0x30, 0xD8, 0x00,         //     **     ** **        
    0x30, 0xD8, 0x00,         //     **     ** **        
    0x1C, 0xD8, 0x00,         //   ***      ** **        
    0x00, 0xD8, 0x00,         //            ** **        
    0x00, 0xD8, 0x00,         //            ** **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 45397
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x30, 0x00,         //             **          
    0x00, 0xE0, 0x00,         //              ***        
    0x0C, 0xC0, 0x00,         //   **          **        
    0x38, 0xC0, 0x00,         //    ***        **        
    0x30, 0xFC, 0x00,         //     **    ******        
    0x30, 0xC0, 0x00,         //     **        **        
    0x30, 0xC6, 0x00,         //     **   **   **        
    0xB0, 0xFB, 0x00,         //     ** *** *****        
    0xF0, 0xC0, 0x00,         //     ****      **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xD8, 0x00,         //            ** **        
    0x00, 0x7E, 0x00,         //          ******         
    0x00, 0xE3, 0x00,         //         **   ***        
    0x00, 0xC3, 0x00,         //         **    **        
    0x00, 0xE7, 0x00,         //         ***  ***        
    0x00, 0x3E, 0x00,         //          *****          
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 47084
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x70, 0x00,         //             ***         
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xC0, 0x00,         //               **        
    0xF8, 0xC7, 0x00,         //    ********   **        
    0x00, 0xC3, 0x00,         //         **    **        
    0x00, 0xC3, 0x00,         //         **    **        
    0x00, 0xC3, 0x00,         //         **    **        
    0xF8, 0xFF, 0x00,         //    *************        
    0x30, 0xC0, 0x00,         //     **        **        
    0x30, 0xC0, 0x00,         //     **        **        
    0x30, 0xCE, 0x00,         //     **   ***  **        
    0xF0, 0xC3, 0x00,         //     ******    **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 48516
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x30, 0x00,         //             **          
    0x70, 0x78, 0x00,         //     ***    ****         
    0x60, 0x60, 0x00,         //      **      **         
    0xE0, 0x7F, 0x00,         //      **********         
    0x60, 0x60, 0x00,         //      **      **         
    0x60, 0x60, 0x00,         //      **      **         
    0xE0, 0x7F, 0x00,         //      **********         
    0x60, 0x60, 0x00,         //      **      **         
    0x00, 0x00, 0x00,         //                         
    0xFE, 0xFF, 0x03,         //  *****************      
    0x00, 0x06, 0x00,         //          **             
    0x78, 0x06, 0x00,         //    ****  **             
    0x60, 0x06, 0x00,         //      **  **             
    0x60, 0x06, 0x00,         //      **  **             
    0x60, 0x00, 0x00,         //      **                 
    0xC0, 0x7F, 0x00,         //       *********         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 49464
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0xE0, 0x00,         //              ***        
    0x00, 0xCE, 0x00,         //          ***  **        
    0x00, 0xD8, 0x00,         //            ** **        
    0xE0, 0xD8, 0x00,         //      ***   ** **        
    0xC0, 0xD9, 0x00,         //       ***  ** **        
    0xC0, 0xD8, 0x00,         //       **   ** **        
    0xE0, 0xD8, 0x00,         //      ***   ** **        
    0xE0, 0xDE, 0x00,         //      *** **** **        
    0xB0, 0xD9, 0x00,         //     ** **  ** **        
    0x18, 0xDB, 0x00,         //    **   ** ** **        
    0x0C, 0xDB, 0x00,         //   **    ** ** **        
    0x06, 0xD8, 0x00,         //  **        ** **        
    0x00, 0xD8, 0x00,         //            ** **        
    0x00, 0xD8, 0x00,         //            ** **        
    0x00, 0xD8, 0x00,         //            ** **        
    0x00, 0xD8, 0x00,         //            ** **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 50504
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x38, 0x00,         //            ***          
    0x00, 0x60, 0x00,         //              **         
    0xC0, 0x60, 0x00,         //       **     **         
    0xE0, 0x61, 0x00,         //      ****    **         
    0x38, 0x67, 0x00,         //    ***  ***  **         
    0x18, 0x66, 0x00,         //    **    **  **         
    0x18, 0x66, 0x00,         //    **    **  **         
    0x38, 0xE3, 0x07,         //    ***  **   ******     
    0xF0, 0x61, 0x00,         //     *****    **         
    0x00, 0x60, 0x00,         //              **         
    0x00, 0x60, 0x00,         //              **         
    0xE0, 0x60, 0x00,         //      ***     **         
    0xC0, 0x60, 0x00,         //       **     **         
    0xC0, 0x00, 0x00,         //       **                
    0xC0, 0x00, 0x00,         //       **                
    0x80, 0xFF, 0x00,         //        *********        
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 50668
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x30, 0x00,         //             **          
    0x00, 0xE0, 0x00,         //              ***        
    0x00, 0xC0, 0x00,         //               **        
    0xC0, 0xC0, 0x00,         //       **      **        
    0xE0, 0xC1, 0x00,         //      ****     **        
    0x38, 0xFB, 0x00,         //    ***  ** *****        
    0x0C, 0xC6, 0x00,         //   **     **   **        
    0x0C, 0xC6, 0x00,         //   **     **   **        
    0x0C, 0xC6, 0x00,         //   **     **   **        
    0x18, 0xFB, 0x00,         //    **   ** *****        
    0xF0, 0xC1, 0x00,         //     *****     **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 50836
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x07, 0x00,         //         ***             
    0x80, 0x0F, 0x00,         //        *****            
    0xC0, 0x18, 0x00,         //       **   **           
    0x60, 0x30, 0x00,         //      **     **          
    0x60, 0x30, 0x00,         //      **     **          
    0x60, 0x30, 0x00,         //      **     **          
    0xC0, 0x18, 0x00,         //       **   **           
    0x80, 0x0F, 0x00,         //        *****            
    0xE0, 0x1C, 0x00,         //      ***  ***           
    0xC0, 0x19, 0x00,         //       ***  **           
    0x80, 0x19, 0x00,         //        **  **           
    0x80, 0x19, 0x00,         //        **  **           
    0xFE, 0xFF, 0x01,         //  ****************       
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 51064
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x70, 0x00,         //             ***         
    0x00, 0xC0, 0x00,         //               **        
    0x80, 0xC1, 0x00,         //        **     **        
    0xE0, 0xC3, 0x00,         //      *****    **        
    0x30, 0xC7, 0x00,         //     **  ***   **        
    0x18, 0xC6, 0x00,         //    **    **   **        
    0x18, 0xC6, 0x00,         //    **    **   **        
    0x38, 0xC7, 0x00,         //    ***  ***   **        
    0xE0, 0xC3, 0x00,         //      *****    **        
    0x00, 0xC0, 0x00,         //               **        
    0x00, 0xC0, 0x00,         //               **        
    0xC0, 0xC0, 0x00,         //       **      **        
    0x80, 0xC1, 0x00,         //        **     **        
    0x80, 0x01, 0x00,         //        **               
    0x80, 0x01, 0x00,         //        **               
    0x80, 0xFF, 0x01,         //        **********       
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

/***********************************
 * Character - 54616
 ***********************************/
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x18, 0x00,         //            **           
    0x00, 0x70, 0x00,         //             ***         
    0xF0, 0x63, 0x00,         //     ******   **         
    0x00, 0x60, 0x00,         //              **         
    0x00, 0x40, 0x00,         //               *         
    0xFE, 0x5F, 0x00,         //  ************ *         
    0x80, 0x41, 0x00,         //        **     *         
    0xE0, 0x63, 0x00,         //      *****   **         
    0x30, 0xE6, 0x07,         //     **   **  ******     
    0x18, 0x66, 0x00,         //    **    **  **         
    0x18, 0x66, 0x00,         //    **    **  **         
    0x30, 0x67, 0x00,         //     **  ***  **         
    0xE0, 0x63, 0x00,         //      *****   **         
    0x00, 0x60, 0x00,         //              **         
    0x00, 0x60, 0x00,         //              **         
    0x00, 0x60, 0x00,         //              **         
    0x00, 0x60, 0x00,         //              **         
    0x00, 0x00, 0x00,         //                         
    0x00, 0x00, 0x00,         //                         

};

/*********************************
 * TTF Font File Structure
 * Label: HindiFont
 * Description:  Height: 31 pixels, 1 bit per pixel, Range: ' ' to '?'
 ***********************************/

extern const char __HindiFont[] __attribute__((aligned(2)));

const FONT_FLASH HindiFont =
{
    (FLASH | COMP_NONE),
    (GFX_FONT_SPACE char *)__HindiFont
};

const char __HindiFont[] __attribute__((aligned(2))) =
{
/****************************************
 * Font header
 ****************************************/
    0x00,           // Font ID
    0x01,           // Font information:  Extended Glyph Entry, 1 bit per pixel, Characters zero degress rotation
    0x20, 0x00,     // First Character
    0x29, 0x00,     // Last Character
    0x1F, 0x00,     // Height
/****************************************
 * Font Glyph Table
 ****************************************/
    0x80, 0x00, 0x00, 0x00,       // Character - 32, offset (0x00000080)
    0x04, 0x00,              // cursor advance
    0x00, 0x00,              // width of the glyph
    0x00, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0x80, 0x00, 0x00, 0x00,       // Character - 33, offset (0x00000080)
    0x08, 0x00,              // cursor advance
    0x01, 0x00,              // width of the glyph
    0x05, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0x9F, 0x00, 0x00, 0x00,       // Character - 105, offset (0x0000009F)
    0x07, 0x00,              // cursor advance
    0x0B, 0x00,              // width of the glyph
    0x00, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0xDD, 0x00, 0x00, 0x00,       // Character - 109, offset (0x000000DD)
    0x0C, 0x00,              // cursor advance
    0x0C, 0x00,              // width of the glyph
    0x00, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0x1B, 0x01, 0x00, 0x00,       // Character - 110, offset (0x0000011B)
    0x0B, 0x00,              // cursor advance
    0x0B, 0x00,              // width of the glyph
    0x00, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0x59, 0x01, 0x00, 0x00,       // Character - 116, offset (0x00000159)
    0x0B, 0x00,              // cursor advance
    0x0B, 0x00,              // width of the glyph
    0x00, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0x97, 0x01, 0x00, 0x00,       // Character - 118, offset (0x00000197)
    0x0B, 0x00,              // cursor advance
    0x0B, 0x00,              // width of the glyph
    0x00, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0xD5, 0x01, 0x00, 0x00,       // Character - 119, offset (0x000001D5)
    0x00, 0x00,              // cursor advance
    0x07, 0x00,              // width of the glyph
    0xF4, 0xFF,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0xF4, 0x01, 0x00, 0x00,       // Character - 223, offset (0x000001F4)
    0x08, 0x00,              // cursor advance
    0x08, 0x00,              // width of the glyph
    0x00, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0x13, 0x02, 0x00, 0x00,       // Character - 8776, offset (0x00000213)
    0x09, 0x00,              // cursor advance
    0x0A, 0x00,              // width of the glyph
    0x00, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
/***********************************
 * Font Characters
 ***********************************/
/***********************************
 * Character - 32
 ***********************************/
// This character does not contain any raster information

/***********************************
 * Character - 33
 ***********************************/
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x01,         // *       
    0x01,         // *       
    0x01,         // *       
    0x01,         // *       
    0x01,         // *       
    0x01,         // *       
    0x01,         // *       
    0x01,         // *       
    0x01,         // *       
    0x00,         //         
    0x00,         //         
    0x01,         // *       
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         

/***********************************
 * Character - 105
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xF0, 0x00,         //     ****        
    0x88, 0x01,         //    *   **       
    0x04, 0x02,         //   *      *      
    0x04, 0x04,         //   *       *     
    0x04, 0x00,         //   *             
    0x04, 0x00,         //   *             
    0x7F, 0x00,         // *******         
    0x08, 0x00,         //    *            
    0x08, 0x00,         //    *            
    0x08, 0x00,         //    *            
    0x08, 0x00,         //    *            
    0x08, 0x00,         //    *            
    0x08, 0x00,         //    *            
    0x08, 0x00,         //    *            
    0x08, 0x00,         //    *            
    0x08, 0x00,         //    *            
    0x08, 0x00,         //    *            
    0x08, 0x00,         //    *            
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 109
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xFF, 0x0F,         // ************    
    0x08, 0x01,         //    *    *       
    0x08, 0x01,         //    *    *       
    0x08, 0x01,         //    *    *       
    0x08, 0x01,         //    *    *       
    0x08, 0x01,         //    *    *       
    0xFE, 0x01,         //  ********       
    0x0C, 0x01,         //   **    *       
    0x08, 0x01,         //    *    *       
    0x00, 0x01,         //         *       
    0x00, 0x01,         //         *       
    0x00, 0x01,         //         *       
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 110
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xFF, 0x07,         // ***********     
    0x80, 0x00,         //        *        
    0x80, 0x00,         //        *        
    0x80, 0x00,         //        *        
    0xFE, 0x00,         //  *******        
    0x87, 0x00,         // ***    *        
    0x86, 0x00,         //  **    *        
    0x86, 0x00,         //  **    *        
    0x84, 0x00,         //   *    *        
    0x80, 0x00,         //        *        
    0x80, 0x00,         //        *        
    0x80, 0x00,         //        *        
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 116
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xFF, 0x07,         // ***********     
    0x80, 0x00,         //        *        
    0x80, 0x00,         //        *        
    0x80, 0x00,         //        *        
    0xFE, 0x00,         //  *******        
    0x83, 0x00,         // **     *        
    0x81, 0x00,         // *      *        
    0x81, 0x00,         // *      *        
    0x82, 0x00,         //  *     *        
    0x86, 0x00,         //  **    *        
    0x8C, 0x00,         //   **   *        
    0x88, 0x00,         //    *   *        
    0x10, 0x00,         //     *           
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 118
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xFF, 0x07,         // ***********     
    0x80, 0x00,         //        *        
    0x80, 0x00,         //        *        
    0xBC, 0x00,         //   **** *        
    0xC2, 0x00,         //  *    **        
    0x81, 0x00,         // *      *        
    0xC1, 0x00,         // *     **        
    0xF2, 0x00,         //  *  ****        
    0xBC, 0x00,         //   **** *        
    0x80, 0x00,         //        *        
    0x80, 0x00,         //        *        
    0x80, 0x00,         //        *        
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 119
 ***********************************/
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x0E,         //  ***    
    0x30,         //     **  
    0x20,         //      *  
    0x40,         //       * 
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         

/***********************************
 * Character - 223
 ***********************************/
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0xFF,         // ********
    0x08,         //    *    
    0x18,         //    **   
    0x18,         //    **   
    0x08,         //    *    
    0x1F,         // *****   
    0xFF,         // ********
    0x06,         //  **     
    0x04,         //   *     
    0x08,         //    *    
    0x10,         //     *   
    0x20,         //      *  
    0x40,         //       * 
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         

/***********************************
 * Character - 8776
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xFF, 0x03,         // **********      
    0x28, 0x00,         //    * *          
    0x44, 0x00,         //   *   *         
    0x44, 0x00,         //   *   *         
    0x44, 0x00,         //   *   *         
    0x6C, 0x00,         //   ** **         
    0x58, 0x00,         //    ** *         
    0x24, 0x00,         //   *  *          
    0x2C, 0x00,         //   ** *          
    0x18, 0x00,         //    **           
    0x20, 0x00,         //      *          
    0x40, 0x00,         //       *         
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

};

/*********************************
 * TTF Font File Structure
 * Label: ThaiFont
 * Description:  Height: 29 pixels, 1 bit per pixel, Range: '!' to 'â'
 ***********************************/

extern const char __ThaiFont[] __attribute__((aligned(2)));

const FONT_FLASH ThaiFont =
{
    (FLASH | COMP_NONE),
    (GFX_FONT_SPACE char *)__ThaiFont
};

const char __ThaiFont[] __attribute__((aligned(2))) =
{
/****************************************
 * Font header
 ****************************************/
    0x00,           // Font ID
    0x01,           // Font information:  Extended Glyph Entry, 1 bit per pixel, Characters zero degress rotation
    0x20, 0x00,     // First Character
    0x2A, 0x00,     // Last Character
    0x1D, 0x00,     // Height
/****************************************
 * Font Glyph Table
 ****************************************/
    0x8C, 0x00, 0x00, 0x00,       // Character - 33, offset (0x0000008C)
    0x08, 0x00,              // cursor advance
    0x02, 0x00,              // width of the glyph
    0x04, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0xA9, 0x00, 0x00, 0x00,       // Character - 161, offset (0x000000A9)
    0x11, 0x00,              // cursor advance
    0x0E, 0x00,              // width of the glyph
    0x01, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0xE3, 0x00, 0x00, 0x00,       // Character - 170, offset (0x000000E3)
    0x12, 0x00,              // cursor advance
    0x10, 0x00,              // width of the glyph
    0x01, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0x1D, 0x01, 0x00, 0x00,       // Character - 180, offset (0x0000011D)
    0x12, 0x00,              // cursor advance
    0x0E, 0x00,              // width of the glyph
    0x01, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0x57, 0x01, 0x00, 0x00,       // Character - 197, offset (0x00000157)
    0x11, 0x00,              // cursor advance
    0x0D, 0x00,              // width of the glyph
    0x01, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0x91, 0x01, 0x00, 0x00,       // Character - 199, offset (0x00000191)
    0x10, 0x00,              // cursor advance
    0x0C, 0x00,              // width of the glyph
    0x01, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0xCB, 0x01, 0x00, 0x00,       // Character - 202, offset (0x000001CB)
    0x11, 0x00,              // cursor advance
    0x0E, 0x00,              // width of the glyph
    0x01, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0x05, 0x02, 0x00, 0x00,       // Character - 209, offset (0x00000205)
    0x00, 0x00,              // cursor advance
    0x0E, 0x00,              // width of the glyph
    0xF6, 0xFF,              // x-position is adjusted
    0xFC, 0xFF,              // y-position is adjusted
    0x47, 0x02, 0x00, 0x00,       // Character - 210, offset (0x00000247)
    0x0E, 0x00,              // cursor advance
    0x0B, 0x00,              // width of the glyph
    0x01, 0x00,              // x-position is adjusted
    0x00, 0x00,              // y-position is adjusted
    0x81, 0x02, 0x00, 0x00,       // Character - 213, offset (0x00000281)
    0x02, 0x00,              // cursor advance
    0x0E, 0x00,              // width of the glyph
    0xF1, 0xFF,              // x-position is adjusted
    0xFA, 0xFF,              // y-position is adjusted
    0xC7, 0x02, 0x00, 0x00,       // Character - 226, offset (0x000002C7)
    0x0A, 0x00,              // cursor advance
    0x0C, 0x00,              // width of the glyph
    0xFF, 0xFF,              // x-position is adjusted
    0xF8, 0xFF,              // y-position is adjusted
/***********************************
 * Font Characters
 ***********************************/
/***********************************
 * Character - 33
 ***********************************/
    0x00,         //         
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x00,         //         
    0x00,         //         
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x03,         // **      
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         
    0x00,         //         

/***********************************
 * Character - 161
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xE0, 0x07,         //      ******     
    0xF8, 0x0F,         //    *********    
    0x1C, 0x1C,         //   ***     ***   
    0x06, 0x38,         //  **        ***  
    0x19, 0x30,         // *  **       **  
    0x70, 0x30,         //     ***     **  
    0x18, 0x30,         //    **       **  
    0x0C, 0x30,         //   **        **  
    0x0C, 0x30,         //   **        **  
    0x0C, 0x30,         //   **        **  
    0x0C, 0x30,         //   **        **  
    0x0C, 0x30,         //   **        **  
    0x0C, 0x30,         //   **        **  
    0x0C, 0x30,         //   **        **  
    0x0C, 0x30,         //   **        **  
    0x0C, 0x30,         //   **        **  
    0x0C, 0x30,         //   **        **  
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 170
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x40,         //               * 
    0x3C, 0xE0,         //   ****       ***
    0xFE, 0x70,         //  *******    *** 
    0xDF, 0x38,         // ***** **   ***  
    0x93, 0x1F,         // **  *  ******   
    0x91, 0x1F,         // *   *  ******   
    0x93, 0x39,         // **  *  **  ***  
    0xDF, 0x30,         // ***** **    **  
    0xCE, 0x30,         //  ***  **    **  
    0x60, 0x30,         //      **     **  
    0x60, 0x30,         //      **     **  
    0x60, 0x30,         //      **     **  
    0x60, 0x30,         //      **     **  
    0x60, 0x30,         //      **     **  
    0x60, 0x30,         //      **     **  
    0x60, 0x30,         //      **     **  
    0xF0, 0x3F,         //     **********  
    0xF0, 0x1F,         //     *********   
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 180
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xF0, 0x07,         //     *******     
    0xFC, 0x0F,         //   **********    
    0x1E, 0x1C,         //  ****     ***   
    0x06, 0x38,         //  **        ***  
    0xC7, 0x31,         // ***   ***   **  
    0xE3, 0x33,         // **   *****  **  
    0x63, 0x32,         // **   **  *  **  
    0x23, 0x32,         // **   *   *  **  
    0x63, 0x33,         // **   ** **  **  
    0xC6, 0x31,         //  **   ***   **  
    0x86, 0x30,         //  **    *    **  
    0xCC, 0x30,         //   **  **    **  
    0x78, 0x30,         //    ****     **  
    0x78, 0x30,         //    ****     **  
    0x38, 0x30,         //    ***      **  
    0x30, 0x30,         //     **      **  
    0x38, 0x30,         //    ***      **  
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 197
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xF0, 0x03,         //     ******      
    0xFC, 0x07,         //   *********     
    0x0E, 0x0E,         //  ***     ***    
    0x03, 0x1C,         // **        ***   
    0x01, 0x18,         // *          **   
    0x78, 0x18,         //    ****    **   
    0xFC, 0x18,         //   ******   **   
    0xCC, 0x19,         //   **  ***  **   
    0x86, 0x1B,         //  **    *** **   
    0x06, 0x1B,         //  **     ** **   
    0x06, 0x1E,         //  **      ****   
    0x3E, 0x1E,         //  *****   ****   
    0x7E, 0x1C,         //  ******   ***   
    0x46, 0x1C,         //  **   *   ***   
    0x46, 0x1C,         //  **   *   ***   
    0x7C, 0x18,         //   *****    **   
    0x38, 0x18,         //    ***     **   
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 199
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xF8, 0x01,         //    ******       
    0xFC, 0x03,         //   ********      
    0x0E, 0x07,         //  ***    ***     
    0x03, 0x0E,         // **       ***    
    0x01, 0x0C,         // *         **    
    0x00, 0x0C,         //           **    
    0x00, 0x0C,         //           **    
    0x00, 0x0C,         //           **    
    0x00, 0x0C,         //           **    
    0x00, 0x0C,         //           **    
    0x00, 0x0C,         //           **    
    0x80, 0x0F,         //        *****    
    0xC0, 0x0F,         //       ******    
    0xC0, 0x0C,         //       **  **    
    0xC0, 0x0C,         //       **  **    
    0x80, 0x0F,         //        *****    
    0x80, 0x07,         //        ****     
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 202
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x10,         //             *   
    0xF0, 0x39,         //     *****  ***  
    0xFC, 0x1B,         //   ******** **   
    0x0E, 0x0E,         //  ***     ***    
    0x03, 0x1E,         // **       ****   
    0x02, 0x1A,         //  *       * **   
    0x78, 0x19,         //    **** *  **   
    0xFC, 0x19,         //   *******  **   
    0x8C, 0x19,         //   **   **  **   
    0x06, 0x1B,         //  **     ** **   
    0x06, 0x1B,         //  **     ** **   
    0x06, 0x1E,         //  **      ****   
    0x3E, 0x1E,         //  *****   ****   
    0x7E, 0x1C,         //  ******   ***   
    0x46, 0x1C,         //  **   *   ***   
    0x46, 0x18,         //  **   *    **   
    0x7C, 0x18,         //   *****    **   
    0x38, 0x18,         //    ***     **   
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 209
 ***********************************/
    0x0E, 0x10,         //  ***        *   
    0x1B, 0x30,         // ** **       **  
    0x31, 0x38,         // *   **     ***  
    0x13, 0x1C,         // **  *     ***   
    0x1F, 0x0E,         // *****    ***    
    0xFE, 0x07,         //  **********     
    0xF8, 0x01,         //    ******       
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 210
 ***********************************/
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0xF8, 0x00,         //    *****        
    0xFE, 0x03,         //  *********      
    0x86, 0x03,         //  **    ***      
    0x03, 0x06,         // **       **     
    0x01, 0x06,         // *        **     
    0x00, 0x06,         //          **     
    0x00, 0x06,         //          **     
    0x00, 0x06,         //          **     
    0x00, 0x06,         //          **     
    0x00, 0x06,         //          **     
    0x00, 0x06,         //          **     
    0x00, 0x06,         //          **     
    0x00, 0x06,         //          **     
    0x00, 0x06,         //          **     
    0x00, 0x06,         //          **     
    0x00, 0x06,         //          **     
    0x00, 0x06,         //          **     
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 213
 ***********************************/
    0x00, 0x18,         //            **   
    0x00, 0x18,         //            **   
    0xF0, 0x19,         //     *****  **   
    0xFC, 0x1B,         //   ******** **   
    0x0E, 0x1E,         //  ***     ****   
    0xFE, 0x1D,         //  ******** ***   
    0xFF, 0x1F,         // *************   
    0x00, 0x1C,         //           ***   
    0x00, 0x20,         //              *  
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

/***********************************
 * Character - 226
 ***********************************/
    0x78, 0x00,         //    ****         
    0xFE, 0x07,         //  **********     
    0x87, 0x0F,         // ***    *****    
    0x1E, 0x00,         //  ****           
    0x38, 0x00,         //    ***          
    0x70, 0x00,         //     ***         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0x60, 0x00,         //      **         
    0xE0, 0x03,         //      *****      
    0xE0, 0x07,         //      ******     
    0x60, 0x06,         //      **  **     
    0x60, 0x06,         //      **  **     
    0xE0, 0x07,         //      ******     
    0xC0, 0x03,         //       ****      
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 
    0x00, 0x00,         //                 

};

