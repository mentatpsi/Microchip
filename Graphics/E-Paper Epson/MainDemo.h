/*********************************************************************
 *
 *	Main demo for the E-Paper Demo
 *
 *********************************************************************
 * FileName:        MainDemo.h
 * Dependencies:    None
 * Processor:       PIC24, dsPIC, PIC32
 * Compiler:        Microchip C30 and C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (c) 2011 Microchip Technology Inc.  All rights 
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
 * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product ("Device") which is 
 *      integrated into Licensee�s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and 
 *      ENC28J60.h ported to a non-Microchip device used in 
 *      conjunction with a Microchip ethernet controller for the 
 *      sole purpose of interfacing with the ethernet controller. 
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT 
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL 
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF 
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS 
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT 
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 ********************************************************************/
#ifndef _MAINDEMO_DEMO_H
#define _MAINDEMO_DEMO_H
    
/**************************************************
 * Includes
 *************************************************/
#if defined(__dsPIC33F__)
    #include <p33Fxxxx.h>
#elif defined(__PIC24H__)
    #include <p24Hxxxx.h>
#elif defined(__PIC32MX__)
    #include <plib.h>
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
#include "Graphics/S1D13522.h"
#include "Graphics/Transitions.h"
#include "gfx_schemes.h"
#include "TimeDelay.h"
#include "SST25VF064.h"

/**************************************************
 * Defines
 *************************************************/
#define NAV_BTN_WIDTH		25
#define ONE_CYCLE_DRAWING

/**************************************************
 * Externs
 *************************************************/
extern volatile BOOL g_AutoMode;
extern volatile WORD AutoModeCnt;
extern volatile HW_BUTTON_STATE btnS1, btnS2, btnS3;
/**************************************************
 * Function Protoypes
 *************************************************/
void PrepareDemoPage(XCHAR *pText);
extern void InitTick(void);
#endif

