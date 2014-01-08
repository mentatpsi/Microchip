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
 * Section: Includes
 *****************************************************************************/
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"

/*****************************************************************************
 * Section: Defines
 *****************************************************************************/
#ifdef __PIC32MX__
// for a system clock of 80 MHz
#define TIMER_TICK_PERIOD 		    (GetSystemClock()/300000) 
#define __TIMER_TICK_ISR            __ISR(_TIMER_3_VECTOR, ipl4)
#else
// for a system clock of 32 MHz
#define TIMER_TICK_SAMPLE_PERIOD    200 
#define TIMER_TICK_PERIOD			(GetPeripheralClock() * TIMER_TICK_SAMPLE_PERIOD) / 4000000
#define __TIMER_TICK_ISR            __attribute__((interrupt, shadow, auto_psv))
#endif

#define TIMER_TICK_PRESCALER	    16
#define TimerTickInterrupt          _T3Interrupt
/*****************************************************************************
 * Section: Variables
 *****************************************************************************/
extern void TouchProcessTouch(void);
extern void TouchDetectPosition(void);
/*****************************************************************************
 * Section: Variables
 *****************************************************************************/
static DWORD tickscaler;
volatile DWORD tick;

/*********************************************************************
* Function: void TickInit(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: sets tick timer
*
* Note: none
*
********************************************************************/
void TickInit(void)
{

    // Initialize Timer4
    #ifdef __PIC32MX__
    OpenTimer3(T3_ON | T3_PS_1_8, TIMER_TICK_PERIOD);  // enable timer to run for approximately 100 us
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_1); // Enable interrupt
    #else
    TMR3 = 0;
    PR3 = TIMER_TICK_PERIOD;							//(for approximately 100 us)
    IFS0bits.T3IF = 0;  						//Clear flag
    IEC0bits.T3IE = 1;  						//Enable interrupt
    T3CONbits.TON = 1;  						//Run timer
    #endif
    
    tickscaler = 0;

}

/*********************************************************************
* Function: Timer3 ISR
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: increments tick counter. Tick is approx. 1 ms.
*
* Note: none
*
********************************************************************/
void __TIMER_TICK_ISR TimerTickInterrupt(void)
{
    TMR3 = 0;
	if (tickscaler > TIMER_TICK_PRESCALER)
	{
    	tickscaler = 0;
    	tick++;
 	} 
 	else
    {
    	tickscaler++;  	
    }

    TouchDetectPosition();

    // Clear flag
    #ifdef __PIC32MX__
    mT3ClearIntFlag();
    #else
    IFS0bits.T3IF = 0;
    #endif    
}
