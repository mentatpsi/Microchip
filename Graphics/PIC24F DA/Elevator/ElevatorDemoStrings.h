/*********************************************************************
 * Module for Microchip Graphics Library
 * This file contains compile time options for the Graphics Library. 
 *********************************************************************
 * FileName:        ElevatorDemoStrings.h
 * Dependencies:    none
 * Processor:       PIC24F 
 * Compiler:        C30 V3.30
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
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
 * Date             Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/07/2011       ...
 ********************************************************************/
#ifndef _ELEVATORDEMOSTRINGS_H
    #define _ELEVATORDEMOSTRINGS_H

#include  <Graphics/Graphics.h>
#include  <ElevatorResources.h>

    const XCHAR RunTimeErrorStr[] = {'R','u','n','t','i','m','e',' ','E','r','r','o','r',0.};
    const XCHAR EnterFlashString[] = {'E','n','t','e','r','i','n','g',' ','F','l','a','s','h',0}; 
    const XCHAR ProgrammingModeString[] = {'P','r','o','g','r','a','m','m','i','n','g',' ','M','o','d','e',0}; 
    const XCHAR InvalidExternalString[] = {'E','x','t','e','r','n','a','l',' ','d','a','t','a',' ','i','n','v','a','l','i','d','.',0};
    const XCHAR PressToContinueString1[] = {'P','r','e','s','s',' ','S','2',' ','t','h','e','n',0};
    const XCHAR PressToContinueString2[] = {'s','e','n','d',' ','d','a','t','a',' ','u','s','i','n','g',0};
    const XCHAR ExternalMemoryProgString1[] = {'"','E','x','t','e','r','n','a','l',' ','M','e','m','o','r','y',0};
    const XCHAR ExternalMemoryProgString2[] = {'P','r','o','g','r','a','m','m','e','r','"',' ','u','t','i','l','i','t','y',0};
#ifdef USE_COMM_PKT_MEDIA_USB
    const XCHAR ViaString[] = {'v','i','a',' ','U','S','B','.','.','.',0};
#else
    const XCHAR ViaString[] =  {'v','i','a',' ','U','A','R','T','.','.','.',0};
#endif
    const XCHAR ParallelFlashNotAccessibleString[] = {'F','l','a','s','h',' ','n','o','t',' ','a','c','c','e','s','s','i','b','l','e',0};


    // this portion implements the multi-language support
    // this is highely dependent on the strings included in the font filter file
    // used when generating the font tables for the messages in the demo.
    enum
    {
        _strMaintain = 0,
        _strOverLoad, 
        _strTimeOut, 
        _strRunTime, 
        _strPause, 
        _strHalt, 
        _strError, 
        _strNonStop, 
        _strWait, 
        _strClosing, 
        _strOpening, 
    };
    
    const XCHAR* eStrings[] = 
    {
        strMaintainChn,
        strOverLoadChn,
        strTimeOutChn,
        strRunTimeChn,
        strPauseChn,
        strHaltChn,
        strErrorChn,
        strNonStopChn,
        strWaitChn,
        strClosingChn,
        strOpeningChn
    };
    
    const XCHAR* cStrings[] = 
    {
        strMaintainEng,
        strOverLoadEng,
        strTimeOutEng,
        strRunTimeEng,
        strPauseEng,
        strHaltEng,
        strErrorEng,
        strNonStopEng,
        strWaitEng,
        strClosingEng,
        strOpeningEng
    };

    const XCHAR** Strings[] = {eStrings, cStrings};

    #define MaintainStr      (XCHAR*)Strings[ElevatorStruct.currLang][_strMaintain    ]
    #define OverLoadStr      (XCHAR*)Strings[ElevatorStruct.currLang][_strOverLoad    ]
    #define TimeOutStr       (XCHAR*)Strings[ElevatorStruct.currLang][_strTimeOut     ]
    #define RunTimeStr       (XCHAR*)Strings[ElevatorStruct.currLang][_strRunTime     ]
    #define PauseStr         (XCHAR*)Strings[ElevatorStruct.currLang][_strPause       ]
    #define HaltStr          (XCHAR*)Strings[ElevatorStruct.currLang][_strHalt        ]
    #define ErrorStr         (XCHAR*)Strings[ElevatorStruct.currLang][_strError       ]
    #define NonStopStr       (XCHAR*)Strings[ElevatorStruct.currLang][_strNonStop     ]
    #define WaitStr          (XCHAR*)Strings[ElevatorStruct.currLang][_strWait        ]
    #define ClosingStr       (XCHAR*)Strings[ElevatorStruct.currLang][_strClosing     ]
    #define OpeningStr       (XCHAR*)Strings[ElevatorStruct.currLang][_strOpening     ]



#endif // #ifndef _ELEVATORDEMOSTRINGS_H

