/*********************************************************************
 * Microchip Graphic Library Demo Application
 * The header file used to declare strings used in the demo.
 *********************************************************************
 * FileName:        MainDemoStrings.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC24/PIC30/PIC32
 * Compiler:        C30/C32
 * Company:         Microchip Technology, Inc.
 *
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
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Paolo A. Tamayo		03/10/2008
 ********************************************************************/
#ifndef MAINDEMOSTRINGS_H
    #define MAINDEMOSTRINGS_H

    #include "Graphics/Graphics.h"

XCHAR       StaticTextLstStr[] = {0};

const XCHAR NewGameStr[] = {'N','e','w',0};
const XCHAR ExitStr[] = {'E','x','i','t',0};
const XCHAR ScaleStr[] = {'S','c','a','l','e',0};
const XCHAR SetStr[] = {'S','e','t',0};
const XCHAR LeftArrowStr[] = {0x1c,0};
const XCHAR RightArrowStr[] = {0x1d,0};
const XCHAR UpArrowStr[] = {0x1e,0};
const XCHAR DownArrowStr[] = {0x1f,0};
const XCHAR MeterStr[] = {'R','P','M',0};
const XCHAR ExtStr[] = {'E','x','t',0};
const XCHAR EcgStr[] = {'E','C','G',0};
const XCHAR LabelStr[] = {'L','a','b','e','l',0};
const XCHAR NoiseStr[] = {'N','o','i','s','e',0};
const XCHAR CleanStr[] = {'C','l','e','a','n',0};
const XCHAR RateStr[] = {'H','e','a','r','t',' ','R','a','t','e',0};
const XCHAR AmplitudeStr[] = {'A','m','p','l','i','t','u','d','e',0};
const XCHAR mVStr[] = {'m','V',0};
const XCHAR BeatsStr[] = {'B','e','a','t','s',0};

const XCHAR DemoStr[] = {'D','e','m','o',0};
#endif //MAINDEMOSTRINGS_H
