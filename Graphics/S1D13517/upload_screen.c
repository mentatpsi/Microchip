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
#include "gfx_schemes.h"
#include "gfx_strings.h"
#include "gfx_screens.h"
#include "gol_ids.h"
#include "MainDemo.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define GFX_BlockUntilFinished(function)   while(!function)

/*****************************************************************************
 * void UploadExternalMemory(void)
 *****************************************************************************/
void UploadExternalMemory(void)
{
   	GOL_SCHEME      *currentScheme;
    GOL_SCHEME      *defaultScheme;

    currentScheme = GFX_SchemeGetCurrentScheme();
    defaultScheme = GFX_SchemeGetDefaultScheme();

	SetColor(currentScheme->Color0);	 

	FillBevel((GetMaxX() >> 2)+20,10 ,(GetMaxX() - 10), 60,5);
	PutImage((GetMaxX() >> 2)+20, 10, (void *) &help_faq, IMAGE_NORMAL);
	FillBevel((GetMaxX() >> 2) + 20,90 ,GetMaxX() - 10, GetMaxY()-10,5);
     
	while(!AlphaBlendWindow(GetDestinationPage(), (GetMaxX() >> 2)+15, 5,
					GFX_PAGE1, (GetMaxX() >> 2)+15, 5,
					GetDestinationPage(), (GetMaxX() >> 2)+15, 5,
				     (GetMaxX()) - ((GetMaxX() >> 2)+20), 
				     60,  	
				     defaultScheme->AlphaValue));
				     	

	while(!AlphaBlendWindow(GetDestinationPage(), (GetMaxX() >> 2) + 15, 85,
					GFX_PAGE1, (GetMaxX() >> 2) + 15, 85,
					GetDestinationPage(), (GetMaxX() >> 2) + 15, 85,
				     (GetMaxX())-((GetMaxX() >> 2) + 15), 
				     GetMaxY() - 90,  	
				     defaultScheme->AlphaValue));
	
    SetActivePage(GetDestinationPage());	
				     
		     				     
	SetColor(RGB565CONVERT(255, 102, 0));
    SetFont((void *) &FONTDEFAULT);

    MoveTo((GetMaxX() >> 2)+100, 20);
    GFX_BlockUntilFinished(OutText((XCHAR *)uploadMemoryStr));
	

	SetState(GOLFindObject(MAIN_SCREEN_ID_UPLOAD_BUT),BTN_DRAW|BTN_DISABLED);
      	
}
