/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        info_screen.c
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
#include "gfx_schemes.h"
#include "gfx_strings.h"
#include "gfx_screens.h"
#include "gol_ids.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define GFX_BlockUntilFinished(function)   while(!function)

/*****************************************************************************
 * void CreateInfo(void)
 *****************************************************************************/
void CreateInfo(BYTE pipControl)
{

	GOL_SCHEME *defaultScheme;

    defaultScheme = GFX_SchemeGetDefaultScheme();

	SetColor(GFX_SchemeGetCurrentScheme()->Color0);	 

if(pipControl == 0)
{
	while(!FillBevel((GetMaxX() >> 2)+20,10 ,(GetMaxX() - 10), 60,5));
	PutImage((GetMaxX() >> 2)+20, 10, (void *) &help_faq, IMAGE_NORMAL);
	while(!FillBevel((GetMaxX() >> 2) + 20,90 ,GetMaxX() - 10, GetMaxY()-10,5));
     
	while(!AlphaBlendWindow(GetDrawBufferAddress(), (GetMaxX() >> 2)+15, 5,
					 GFX_PAGE1, (GetMaxX() >> 2)+15, 5,
					 GetDrawBufferAddress(), (GetMaxX() >> 2)+15, 5,
				     (GetMaxX()) - ((GetMaxX() >> 2)+20), 
				     60,  	
				     defaultScheme->AlphaValue));
				     	

	while(!AlphaBlendWindow(GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
					 GFX_PAGE1, (GetMaxX() >> 2) + 15, 85,
					 GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
				     (GetMaxX())-((GetMaxX() >> 2) + 15), 
				     GetMaxY() - 90,  	
				     defaultScheme->AlphaValue));			     	
				     		     				     
	SetColor(RGB565CONVERT(255, 102, 0));
    SetFont((void *) &FONTDEFAULT);

    MoveTo((GetMaxX() >> 2)+100, 20);
    GFX_BlockUntilFinished(OutText((XCHAR *)InfoStr));
			
	StCreate
	(
		INFO_SCREEN_ID_STATIC_TXT,             // ID
		(GetMaxX() >> 2) + 20,
		90,                          // dimensions
		(GetMaxX() >> 2) + 20,
		90,
		ST_UPDATE,          // will be dislayed, and center aligned
		(void *)InformationStr,                            
		defaultScheme
	);  
}	
else
{
	ClrState(GOLFindObject(MAIN_SCREEN_ID_CONFIG_BUT),BTN_DISABLED);
	SetState(GOLFindObject(MAIN_SCREEN_ID_CONFIG_BUT),BTN_DRAW);
	SetState(GOLFindObject(MAIN_SCREEN_ID_INFO_BUT),BTN_DRAW|BTN_DISABLED);
}      	
}
