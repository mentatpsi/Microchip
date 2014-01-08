/*****************************************************************************
 * FileName:        Resources_8BPPMode.h
 * Processor:       PIC24F
 * Compiler:        MPLAB C30 (see release notes for tested revision)
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright � 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
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
 * Date            Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 12/05/2007      Initial Version
 *****************************************************************************/

#ifndef _RESOURCES_H_FILE
#define _RESOURCES_H_FILE

#include <Graphics/Graphics.h>

#ifdef USE_COMP_IPU
    #if (COLOR_DEPTH == 1)
        #include "InternalResourceXC16_1BPPModeIPU.h"
    #elif (COLOR_DEPTH == 4)    
        #include "InternalResourceXC16_4BPPModeIPU.h"
    #elif (COLOR_DEPTH == 8)    
        #include "InternalResourceXC16_8BPPModeIPU.h"
    #endif
#else
    #if (COLOR_DEPTH == 1)
        #include "InternalResourceXC16_1BPPMode.h"
    #elif (COLOR_DEPTH == 4)    
        #include "InternalResourceXC16_4BPPMode.h"
    #elif (COLOR_DEPTH == 8)    
        #include "InternalResourceXC16_8BPPMode.h"
    #endif
#endif

#endif //#ifndef _RESOURCES_H_FILE
