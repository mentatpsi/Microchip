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

#ifndef STOPWATCH_HEADER_FILE
#define STOPWATCH_HEADER_FILE

/*****************************************************************************
 * SECTION: Includes
 *****************************************************************************/
#ifdef __PIC32MX
    #include <plib.h>
    #include <p32xxxx.h>
#else
    #include <p24Fxxxx.h>
#endif
#include "GenericTypeDefs.h"
/*****************************************************************************
 * SECTION: Inline Functions
 *****************************************************************************/
extern inline void __attribute__((always_inline)) Stopwatch_Start(void)
{
#ifdef __PIC32MX
    WriteCoreTimer(0);
#else
    T4CONbits.T45 = 1;
    T4CONbits.TON = 0;
    TMR4 = 0;
    TMR5 = 0;
    T4CONbits.TON = 1;
#endif
}

extern inline DWORD __attribute__((always_inline)) Stopwatch_Stop(void)
{
    DWORD timerTicks;

#ifdef __PIC32MX
    timerTicks = 2*ReadCoreTimer();
#else

    T4CONbits.TON = 0;
    timerTicks = (DWORD)TMR5;
    timerTicks <<= 16;
    timerTicks |= (DWORD)TMR4;
#endif
    return timerTicks;
}
#endif
