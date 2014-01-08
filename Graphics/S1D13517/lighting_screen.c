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
#include "gfx_schemes.h"
#include "gfx_screens.h"
#include "gol_ids.h"
#include "timer_tick.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define LIGHTING_SCREEN_SIZE_X  (GetMaxX() - (GetMaxX() >> 2) - 100)
#define LIGHTING_SCREEN_SIZE_Y  (GetMaxY()  - 120)
#define LIGHTING_SCREEN_START   (GetMaxX() - LIGHTING_SCREEN_SIZE_X - 50)
#define LIGHTING_SCREEN_END     (GetMaxX() -50)

#define GFX_BlockUntilFinished(function)   while(!function)
/*****************************************************************************
 * SECTION: Variables
 *****************************************************************************/

/*****************************************************************************
 * void CreateLightingScreen(void)
 *
 * NOTE:  The lighting demo is not available when using the PIC24FJ256GB210.
 *        The demo requires loading an image from the external memory, the Epson
 *        S1D13517 () has some routing conflicts which make loading the image
 *        difficult and not robust.  For these reasons, the lighting demo has
 *        been take out for a better out of box experience.
 *****************************************************************************/
void CreateLightingScreen(void)
{

    GOL_SCHEME *currentScheme;

    currentScheme = GFX_SchemeGetCurrentScheme();

    SetColor(currentScheme->Color0);

    while(!FillBevel((GetMaxX() >> 2) + 20,90 ,GetMaxX() - 10, GetMaxY()-10,5));				     	

	while(!AlphaBlendWindow(GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
					 GFX_PAGE1, (GetMaxX() >> 2) + 15, 85,
					 GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
				     (GetMaxX())-((GetMaxX() >> 2) + 15), 
				     GetMaxY() - 90,  	
				     GFX_SchemeGetDefaultScheme()->AlphaValue));

    SetState(GOLFindObject(PANEL_SCREEN_ID_LIGHTING_BUT), BTN_DISABLED);

    if(GetState(GOLFindObject(PANEL_SCREEN_ID_COMFORT_BUT),BTN_DISABLED))
    {
        ClrState(GOLFindObject(PANEL_SCREEN_ID_COMFORT_BUT), BTN_DISABLED);
        SetState(GOLFindObject(PANEL_SCREEN_ID_COMFORT_BUT),BTN_DRAW);
    }

    if(GetState(GOLFindObject(PANEL_SCREEN_ID_SECURITY_BUT),BTN_DISABLED))
    {
        ClrState(GOLFindObject(PANEL_SCREEN_ID_SECURITY_BUT), BTN_DISABLED);
        SetState(GOLFindObject(PANEL_SCREEN_ID_SECURITY_BUT),BTN_DRAW);
    }

    if(GetState(GOLFindObject(PANEL_SCREEN_ID_ENERGY_USAGE_BUT),BTN_DISABLED))
    { 
        ClrState(GOLFindObject(PANEL_SCREEN_ID_ENERGY_USAGE_BUT), BTN_DISABLED);
        SetState(GOLFindObject(PANEL_SCREEN_ID_ENERGY_USAGE_BUT),BTN_DRAW);
    }

/***
 * See above note in the function comment for more information
 **/
    #ifndef __PIC24FJ256GB210__
    GFX_BlockUntilFinished(PutImage((GetMaxX() >> 2)+40, 90+10, (void *) &House, IMAGE_NORMAL));
    #endif

}
/*****************************************************************************
 * void DisplayLightingScreen(void)
 *****************************************************************************/
void DisplayLightingScreen(void)
{
    static DWORD prevTick = 0;
    static BYTE blink = 0;

    if((TimerTick_GetTick() - prevTick) > 1000)
    {
	    prevTick = TimerTick_GetTick();
	    blink ^=1;
	    
        if(blink == 1)
	       { SetColor(GREEN);}
	    else
	        SetColor(WHITE);

	    FillCircle(LIGHTING_SCREEN_START + 65, 140, 5);           //Bed 3
	    FillCircle(LIGHTING_SCREEN_START+290, 160, 5);            //Terrace
	    FillCircle(LIGHTING_SCREEN_START+250, GetMaxY() - 80, 5);  //Living
	    FillCircle(LIGHTING_SCREEN_START + 65, GetMaxY() - 200, 5);  //Bed 4
        UpdateDisplayNow();
    }
    
}
/*****************************************************************************
 * WORD MsgLightingScreen(WORD objMsg, OBJ_HEADER *pObj)
 *****************************************************************************/
WORD MsgLightingScreen(WORD objMsg, OBJ_HEADER *pObj)
{
    return(MsgPanelScreen(objMsg, pObj));
}
