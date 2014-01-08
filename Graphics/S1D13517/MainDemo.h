/*********************************************************************
 * Microchip Graphic Library Demo Application
 * The header file joins all header files used in the project.
 *********************************************************************
 * FileName:        MainDemo.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC24/PIC32
 * Compiler:        C30/C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright © 2007 Microchip Technology Inc.  All rights reserved.
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
 * Anton Alkhimenok
 * Paolo Tamayo         03/10/08    ...
 ********************************************************************/
#ifndef _MAINDEMO_H
#define _MAINDEMO_H

/*****************************************************************************
 * SECTION: Includes
 *****************************************************************************/
#ifdef __PIC32MX
    #include <plib.h>
    #include <p32xxxx.h>
#else
    #include <p24Fxxxx.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Graphics/DisplayDriver.h"
#include "GenericTypeDefs.h"
#include "Graphics/Graphics.h"
#include "HardwareProfile.h"
#include "Graphics/S1D13517.h"
#include "SST25VF016.h"
#include "EEPROM.h"
#include "Beep.h"
#include "Graphics/Transitions.h"
#include "FlashProgrammer.h"
#if defined (__C30__) || defined(__XC16__)
    #include "ExternalResource.h"
    #include "InternalResourceXC16.h"
#else
    #include "InternalResourceXC32.h"
#endif

/*****************************************************************************
 * SECTION: External memory Comm packet size
 *****************************************************************************/
//#define COMM_PKT_RX_MAX_SIZE (1028)

/*****************************************************************************
 * SECTION: DEMO STATES
 *****************************************************************************/
typedef enum
{
    CREATE_DEMOSELECTION    = 0,
    DISPLAY_DEMOSELECTION,
    CREATE_BACKGROUND,
    DISPLAY_BACKGROUND,    
    CREATE_MAIN,
    DISPLAY_MAIN,
    CREATE_PERFORMANCE,
    DISPLAY_PERFORMANCE,
    CREATE_CONFIG,
    DISPLAY_CONFIG,
    CREATE_24BPPCOLOR,
    DISPLAY_24BPPCOLOR,
    CREATE_SPEED,
    DISPLAY_SPEED,
    CREATE_COMFORT,
    DISPLAY_COMFORT,
    CREATE_LIGHTING,
    DISPLAY_LIGHTING,
    CREATE_ENERGY,
    DISPLAY_ENERGY,
    CREATE_SECURITY,
    DISPLAY_SECURITY,
    CREATE_SCROLLING,
    DISPLAY_SCROLLING,
    CREATE_GRADIENT,
    DISPLAY_GRADIENT,
    CREATE_ALPHABLEND,
    DISPLAY_ALPHABLEND,
    CREATE_INFO,
    DISPLAY_INFO,
    CREATE_UPLOAD,
    DISPLAY_UPLOAD,
    GOL_BACKGROUNDDRAW
} SCREEN_STATES;
extern SCREEN_STATES    screenState;    // current state of main demo state mashine



/*****************************************************************************
 * SECTION: Graphics Page Defines
 *****************************************************************************/
#define DISPLAY_BUFFER	GFX_PAGE0	
#define DOUBLE_BUFFER	GFX_PAGE8	
#define IMAGE1_PINGPONG GFX_PAGE7
#define IMAGE2_PINGPONG GFX_PAGE8

 
/*****************************************************************************
 * SECTION: Function Prototypes
 *****************************************************************************/
WORD    ExternalMemoryCallback(IMAGE_EXTERNAL *memory, LONG offset, WORD nCount, void *buffer);

#endif // _MAINDEMO_H
