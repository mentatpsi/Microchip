/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        main_screen.c
 * Dependencies:    See Includes Section
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 *
 * Copyright © 2011 Microchip Technology Inc.  All rights reserved.
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
 *****************************************************************************/
/*****************************************************************************
 * SECTION: Includes
 *****************************************************************************/
#include "Graphics/Graphics.h"

/*****************************************************************************
 * SECTION: Strings
 *****************************************************************************/
const XCHAR ErrMsgStandard[]            = { 'U','S','B',' ','E','r','r','o','r',0 };
const XCHAR ErrNotSupported[]           = { 'n','o','t',' ','s','u','p','p','o','r','t','e','d','!',0 };
const XCHAR ErrMsgFailedStr[]           = { 'F','a','i','l','e','d',0 };
const XCHAR ErrMsgHUBAttachedStr[]      = { 'H','U','B',0};
const XCHAR ErrMsgUDAttachedStr[]       = { 'D','e','v','i','c','e',0};
const XCHAR ErrMsgEnumerationStr[]      = { 'E','n','u','m','e','r','a','t','i','o','n',0};
const XCHAR ErrMsgClientInitStr[]       = { 'C','l','i','e','n','t',' ','I','n','i','t','i','a','l','i','z','a','t','i','o','n',0};
const XCHAR ErrMsgOutofMemoryStr[]      = { 'O','u','t',' ','o','f',' ','M','e','m','o','r','y',0};
const XCHAR ErrMsgUnpecifiedErrStr[]    = { 'U','n','s','p','e','c','i','f','i','e','d',0};
const XCHAR ExitStr[]                   = { 'E','x','i','t',0};
const XCHAR MsgTouchToProceedStr[]      = { 'T','o','u','c','h',' ','t','o',' ','p','r','o','c','e','e','d',0};
const XCHAR ButtonStr2[]                = { 'A','l','p','h','a',' ','B','l','e','n','d','e','d',' ','B' ,'u' ,'t' ,'t' ,'o' ,'n' ,'s', '\0' };
const XCHAR PanelDemoStr[]              = { 'P' ,'a' ,'n' ,'e' ,'l',' ','D','e' ,'m' ,'o', '\0' };
const XCHAR SlideShowStr[]              = { 'S' ,'l' ,'i' ,'d' ,'e' ,' ' ,'S','h','o','w', '\0' };
const XCHAR PerformanceStr[]            = { 'P' ,'e' ,'r' ,'f' ,'o' ,'r' ,'m','a','n','c','e', '\0' };
const XCHAR ConfigStr[]                 = { 'C' ,'o' ,'n' ,'f' ,'i' ,'g','\0' };
const XCHAR InfoStr[]                   = { 'I' ,'n' ,'f' ,'o' ,'\0' };
const XCHAR uploadStr[]                 = { 'U' ,'p' ,'l' ,'o' ,'a', 'd', '\0' };
const XCHAR uploadMemoryStr[]           = { 'U' ,'p' ,'l' ,'o' ,'a', 'd', ' ', 'M', 'e', 'm', 'o', 'r', 'y', '.', '.', '.', '\0' };
const XCHAR AutoDemoStr[]               = { 'A','u','t','o',' ','D','e','m','o','\0' };
const XCHAR ColorStr[]                  = { '2','4','B','P','P','C' ,'o' ,'l' ,'o' ,'r','\0' };
const XCHAR SpeedStr[]                  = { 'S' ,'p' ,'e' ,'e' ,'d' ,'\0' };
const XCHAR GradientStr[]               = { 'G' ,'r' ,'a' ,'d' ,'i' ,'e' ,'n' ,'t' ,'s' ,'\0' };
const XCHAR BlendingStr[]               = { 'A' ,'l' ,'p' ,'h' ,'a' ,'B','l','e','n','d','i','n','g','\0' };
const XCHAR BackgroundStr[]             = { 'B','a','c','k','g','r' ,'o' ,'u' ,'n' ,'d','\0' };
const XCHAR AlphaValueStr[]             = { 'A','l','p','h','a','\0' };
const XCHAR PalletteStr[]               = { 'P','a','l','l','e','t','t','e','\0' };
const XCHAR BacklightStr[]              = { 'B','a','c','k','l','i','g','h','t','\0' };
const XCHAR TouchStr[]                  = { 'T' ,'o' ,'u' ,'c' ,'h' ,' ' ,'C' ,'a' ,'l','i','b','r','a','t','e' ,'\0' };
const XCHAR PlusStr[]                   = { '+','\0' };
const XCHAR MinusStr[]                  = { '-' ,'\0' };
const XCHAR LightingStr[]               = { 'L' ,'i' ,'g' ,'h' ,'t' ,'i' ,'n','g','\0' };
const XCHAR SecurityStr[]               = { 'S' ,'e' ,'c' ,'u' ,'r' ,'i' ,'t','y','\0' };
const XCHAR EnergyUsageStr[]            = { 'E' ,'n' ,'e' ,'r' ,'g' ,'y',' ','U','s','a','g','e','\0' };
const XCHAR ComfortStr[]                = { 'C' ,'o' ,'m' ,'f' ,'o','r','t','\0' };
const XCHAR AUTOStr[]                   = { 'A','U','T','O','\0' };
const XCHAR HEATStr[]                   = { 'H' ,'E','A','T','\0' };
const XCHAR COOLStr[]                   = { 'C','O','O','L','\0' };
const XCHAR OFFStr[]                    = { 'O','F','F','\0' };
const XCHAR ONStr[]                     = { 'O','N','\0' };
const XCHAR UpstairsStr[]               = { 'U','p','s','t','a','i','r','s','\0' };
const XCHAR DownstairsStr[]             = { 'D','o','w','n','s','t','a','i','r','s','\0' };
const XCHAR TemperatureStr[]            = { 'T','e','m','p','e','r','a','t','u','r','e','\0' };
const XCHAR SetpointStr[]               = { 'S','e','t','p','o','i','n','t','\0' };
const XCHAR HeatStr[]                   = { 'H','e','a','t','\0' };
const XCHAR CoolStr[]                   = { 'C','o','o','l','\0' };
const XCHAR SystemModeStr[]             = { 'S','y','s','t','e','m',' ','M','o','d','e','\0' };
const XCHAR FanModeStr[]                = { 'F' ,'a','n',' ','M','o','d','e','\0' };
const XCHAR RghtStr[]                   = { '>' ,'>','>','>','\0' };
const XCHAR LftStr[]                    = { '<','<','<','<','\0' };
const XCHAR DwnStr[]                    = { 'V' ,'V','V','V','\0' };
const XCHAR UPStr[]                     = { '^','^','^','^','\0' };
const XCHAR EnterStr[]                  = { 'E' ,'n','t','e','r','\0' };
const XCHAR ClearStr[]                  = { 'C','l','e','a','r','\0' };
const XCHAR D20Str[]                    = { '2' ,'3','\0' };
const XCHAR M230Str[]                   = { '2','3','1','\0' };
const XCHAR kwhStr[]                    = { 'k' ,'W','h','\0' };
const XCHAR DaysStr[]                   = { 'D' ,'a','y','s','\0' };
const XCHAR EnterPINStr[]               = { 'E' ,'n','t','e','r',' ','P','I','N','\0' };
const XCHAR SecurityOptionsStr[]        = { 'S','e','c','u','r','i','t','y',' ','O','p','t','i','o','n','s','\0' };
const XCHAR DisarmedStr[]               = { 'D' ,'i','s','a','r','m','e','d','\0' };
const XCHAR ArmedStr[]                  = { 'A','r','m','e','d','\0' };
const XCHAR SleepingStr[]               = { 'S' ,'l','e','e','p','i','n','g','\0' };
const XCHAR VacationStr[]               = { 'V','a','c','a','t','i','o','n','\0' };
const XCHAR EntertainingStr[]           = { 'E' ,'n','t','e','r','a','i','n','i','n','g','\0' };
const XCHAR TodaysUsageStr[]            = { 'T','o','d','a','y','s',' ','U','s','a','g','e','\0' };
const XCHAR MonthlyUsageStr[]           = { 'M','o','n','t','h','l','y',' ','U','s','a','g','e','\0' };
const XCHAR UsageHistoryStr[]           = { 'U','s','a','g','e',' ','H','i','s','t','o','r','y','\0' };
const XCHAR C24bppStr[]                 = { '*',' ','S','u','p','p','o','r','t',' ','f','o','r',' ','2','4','b','p','p',' ','c','o','l','o','r',' ','d','e','p','t','h','.',0};
const XCHAR C24bppStr2[]                = { ' ',' ','1','6','.','7',' ','M','i','l','l','i','o','n',' ','m','o','r','e',' ','c','o','l','o','r','s','!',0};
const XCHAR FadingStr[]                 = { 'F' ,'a' ,'d' ,'i' ,'n' ,'g' ,' ' ,'U' ,'s','i','n','g',' ','A','l','p','h','a',' ','B','l','e','n','d','i','n','g','\0' };
const XCHAR Var1Name[]                  = { 'V','a','r','1','\0' };
const XCHAR spaceStr[]                  = { ' ','\0'};
const XCHAR loading[]                   = { 'L','o','a','d','i','n','g','.','.','.',0};
const XCHAR InformationStr[]            = { ' ',' ','D','E','M','O',' ','F','E','A','T','U','R','E','S',':','\n',
									        '\n',
                                            '-',' ','U','p',' ','t','o',' ','2','4','b','p','p',' ','c','o','l','o','r',' ','d','e','p','t','h','\n', 
                                            '-',' ','A','l','p','h','a',' ','b','l','e','n','d','i','n','g','\n',  
                                            '-',' ','C','o','l','o','r',' ','g','r','a','d','i','e','n','t','\n',
                                            '-',' ','s','c','r','o','l','l','i','n','g','\n',
                                            '-',' ','M','u','l','t','i','-','b','u','f','f','e','r','i','n','g','\n',
                                            '-',' ','T','r','a','n','s','p','a','r','e','c','y','\n',
                                            '-',' ','S','c','r','e','e','n',' ','r','o','t','a','t','i','o','n','\n',
                                            '-',' ','B','a','c','k','l','i','g','h','t',' ','c','o','n','t','r','o','l','\n',
                                            '-',' ','P','I','P',' ','S','u','p','p','o','r','t','\n',
                                            '-',' ','O','n','-','c','h','i','p',' ','1','6','b','i','t',' ','P','a','r','a','l','l','e','l',' ','M','a','s','t','e','r',' ','P','o','r','t','\n', 
                                            '-',' ','E','p','s','o','n',' ','S','1','D','1','3','5','1','7',' ','d','i','s','p','l','a','y',' ','c','o','n','t','r','o','l','l','e','r','\0'};

