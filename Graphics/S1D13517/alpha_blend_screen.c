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
#include "MainDemo.h"
#include "gfx_screens.h"
#include "gol_ids.h"
#include "TimeDelay.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define ALPHA_BLEND_DELAY_MS    (10)
/*****************************************************************************
 * SECTION: Variables
 *****************************************************************************/
static BYTE alphaBlendPercentage = 0;
/*****************************************************************************
 * void DisplayAlphaBlendScreen(void)
 *****************************************************************************/
void DisplayAlphaBlendScreen(void)
{
	
	

	while(!AlphaBlendWindow(1, (GetMaxX() >> 2) + 15, 85,
					 IMAGE2_PINGPONG, (GetMaxX() >> 2) + 15, 85,
					 GetFrameBufferAddress(), (GetMaxX() >> 2) + 15, 85,
				     (GetMaxX())-((GetMaxX() >> 2) + 20), 
				     GetMaxY() - 90,  	
				     alphaBlendPercentage)); 
				    
	DelayMs(ALPHA_BLEND_DELAY_MS);		
					          
	if(++alphaBlendPercentage > 99)       
 		alphaBlendPercentage = 0;    
	     				
}
