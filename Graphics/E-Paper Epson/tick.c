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
 *****************************************************************************/

/*****************************************************************************
 * SECTION: Includes
 *****************************************************************************/
#include "Graphics/Graphics.h"
#include "MainDemo.h"
/*****************************************************************************
 * SECTION: externs
 *****************************************************************************/
#if !defined( ONE_CYCLE_DRAWING	)
BOOL g_UpdateNow;
#endif

/*********************************************************************
 * Section: Tick Delay
 *********************************************************************/
#define SAMPLE_PERIOD       2000 // us
#define TICK_PERIOD			(GetPeripheralClock() * SAMPLE_PERIOD) / 4000000

volatile WORD tick;
/*****************************************************************************
 * void CreateFontScreen(void)
 *****************************************************************************/
void InitTick(void)
{
    #ifdef __PIC32MX__
    OpenTimer4(T4_ON | T4_PS_1_8, TICK_PERIOD);
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_1);
    #else
    TMR4 = 0;
    PR4 = TICK_PERIOD;
    IFS1bits.T4IF = 0;  //Clear flag
    IEC1bits.T4IE = 1;  //Enable interrupt
    T4CONbits.TON = 1;  //Run timer
    #endif
}

/*****************************************************************************
 * void CreateFontScreen(void)
 *****************************************************************************/
#ifdef __PIC32MX__
    #define __T4_ISR    __ISR(_TIMER_4_VECTOR, ipl1)
#else
    #define __T4_ISR    __attribute__((interrupt, shadow, auto_psv))
#endif

void __T4_ISR _T4Interrupt(void)
{	
    // Clear flag
    #ifdef __PIC32MX__
    mT4ClearIntFlag();
    #else
    IFS1bits.T4IF = 0;
    #endif

    tick++;
    
    btnS1 = GetHWButtonS1();
    btnS2 = GetHWButtonS2();
    btnS3 = GetHWButtonS3();    
    
    if ( g_AutoMode )
    {   
	    static BYTE IsPress = FALSE;
	    
	    if ( btnS1 != HW_BUTTON_PRESS && btnS2 != HW_BUTTON_PRESS && btnS3 != HW_BUTTON_PRESS )
	    {
		   static unsigned twice;
		   
		   if ( IsPress && AutoModeCnt > 0xff )
		   {
				g_AutoMode = FALSE;	
		    	IsPress = FALSE;
		   }
		   
		   		   
		   twice ^= 1;
		   if ( twice )
			   if( ++AutoModeCnt > 0xfffe )
			   {
			      AutoModeCnt = 0;
			      btnS3 = HW_BUTTON_PRESS;
			   } 			     
		}	
		else
		{
			IsPress = TRUE;	
			AutoModeCnt = 0;
		}	
	}
	
#if !defined( ONE_CYCLE_DRAWING	)
	// EPD Update based on DelayUpdateCnt dalay.
	// The counter value for DelayUpdateCnt is a time that would be enough to draw any possible scene or picture. 
	static WORD DelayUpdateCnt;
	
	if ( GFX_DRIVER_IsUpdateRequested() )
	{
		DelayUpdateCnt = 0x00FF;
	}
	else if ( DelayUpdateCnt )
	{
		if ( --DelayUpdateCnt )
			return;
		else	
			g_UpdateNow = TRUE;
	}
#endif		
}

