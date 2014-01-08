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
#include "gfx_schemes.h"
#include "gfx_strings.h"
#include "gfx_screens.h"
#include "gol_ids.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define PANNEL_BUTTON_HEIGHT    (GetMaxY() / 9)
#define PANNEL_BUTTON_RADIUS    7

#define GFX_BlockUntilFinished(function)   while(!function)
/*****************************************************************************
 * SECTION: Externs
 *****************************************************************************/

/*****************************************************************************
 * void CreatePanelScreen(void)
 *
 * NOTE:  The lighting demo is not available when using the PIC24FJ256GB210.
 *        The demo requires loading an image from the external memory, the Epson
 *        S1D13517 () has some routing conflicts which make loading the image
 *        difficult and not robust.  For these reasons, the lighting demo has
 *        been take out for a better out of box experience.
 *****************************************************************************/
void CreatePanelScreen(void)
{
    GOL_SCHEME *currentScheme;

    currentScheme = GFX_SchemeGetCurrentScheme();

    SetColor(ORANGE);                       // Orange
    while(!Bar((GetMaxX() >> 2), 0 , (GetMaxX() >> 2) + 2, GetMaxY()));

    GOLFree();                              // free memory for the objects in the previous linked list and start new list

    BtnCreate
    (
        PANEL_SCREEN_ID_COMFORT_BUT,                         // button ID
/***
 * See above note in the function comment for more information
 **/
#ifndef __PIC24FJ256GB210__
        5,
        PANNEL_BUTTON_HEIGHT << 1,
        (GetMaxX() >> 2) - 5,
        PANNEL_BUTTON_HEIGHT *3,                                // dimension
#else
        5,
        (PANNEL_BUTTON_HEIGHT *3) + 10,
        (GetMaxX() >> 2) - 5,
        (PANNEL_BUTTON_HEIGHT << 2) + 10,                 // dimension
#endif
        PANNEL_BUTTON_RADIUS,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)ComfortStr,                 // "Button",     	// text
        currentScheme
    ); 
                                     // use alternate scheme
/***
 * See above note in the function comment for more information
 **/
    #ifndef __PIC24FJ256GB210__
    BtnCreate
    (
        PANEL_SCREEN_ID_LIGHTING_BUT,                         // button ID
        5,
        (PANNEL_BUTTON_HEIGHT *3) + 10,
        (GetMaxX() >> 2) - 5,
        (PANNEL_BUTTON_HEIGHT << 2) + 10,                 // dimension
        PANNEL_BUTTON_RADIUS,
        BTN_DRAW,                           // will be dislayed after creation
        NULL,           // use bitmap
        (XCHAR *)LightingStr,                   // "HOME", 	    // text
        currentScheme
    );                                      // alternative GOL scheme
    #endif

    BtnCreate
    (
        PANEL_SCREEN_ID_SECURITY_BUT,                         // button ID
        5,
        (PANNEL_BUTTON_HEIGHT << 2) + 20,
        (GetMaxX() >> 2) - 5,
        (PANNEL_BUTTON_HEIGHT * 5) + 20,                 // dimension
        PANNEL_BUTTON_RADIUS,                                 // set radius
        BTN_DRAW ,              // draw a vertical capsule button
        NULL, // no bitmap
        (XCHAR *)SecurityStr,                    // "LO",       	// text
        currentScheme
    );                          // use alternate scheme
    BtnCreate
    (
        PANEL_SCREEN_ID_ENERGY_USAGE_BUT,             // button ID
        5,
        (PANNEL_BUTTON_HEIGHT *5) + 30,
        (GetMaxX() >> 2) - 5,
        (PANNEL_BUTTON_HEIGHT * 6) +  30,      // dimension
        PANNEL_BUTTON_RADIUS,                     // set radius
        BTN_DRAW,               // draw a vertical capsule button
        NULL,                   // no bitmap
        (XCHAR *)EnergyUsageStr,         // "ON",		// text
        currentScheme
    );                          // use alternate scheme
    BtnCreate
    (
        PANEL_SCREEN_ID_EXIT_BUT,             // button ID
        5,
        (PANNEL_BUTTON_HEIGHT * 6) +  40,
        (GetMaxX() >> 2) - 5,
        (PANNEL_BUTTON_HEIGHT * 7) +  40,     // dimension
        PANNEL_BUTTON_RADIUS,                     // set radius
        BTN_DRAW, // draw a vertical capsule button
        NULL,  // no bitmap
        (XCHAR *)ExitStr,                    // "OFF",      	// text
        currentScheme
    );                      // use alternate scheme 	

 	SetColor(currentScheme->Color0);

	while(!FillBevel((GetMaxX() >> 2)+20,10 ,(GetMaxX() - 10), 60,5));
	while(!PutImage((GetMaxX() >> 2)+20, 10, (void *) &appointment_new, IMAGE_NORMAL));
	while(!FillBevel((GetMaxX() >> 2) + 20,90 ,GetMaxX() - 10, GetMaxY()-10,5));

	while(!AlphaBlendWindow(GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
					 GFX_PAGE1, (GetMaxX() >> 2) + 15, 85,
					 GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
				     (GetMaxX())-((GetMaxX() >> 2) + 15), 
				     GetMaxY() - 90,  	
				     GFX_SchemeGetDefaultScheme()->AlphaValue));

	SetColor(RGB565CONVERT(255, 102, 0));
    SetFont((void *) &FONTDEFAULT);

    MoveTo((GetMaxX() >> 2)+80, 20);
    GFX_BlockUntilFinished(OutText((XCHAR *)PanelDemoStr));
 	 
}
/*****************************************************************************
 * void CreatePanelScreen(void)
 *****************************************************************************/
WORD MsgPanelScreen(WORD objMsg, OBJ_HEADER *pObj)
{
    switch(GetObjID(pObj))
    {
        case PANEL_SCREEN_ID_COMFORT_BUT:
            if((objMsg == BTN_MSG_RELEASED))
            {
                screenState = CREATE_COMFORT; 
            }
            return (1); // process by default

        case PANEL_SCREEN_ID_LIGHTING_BUT:
            if((objMsg == BTN_MSG_RELEASED))
            {
                screenState = CREATE_LIGHTING; 
            }            
            return (1); // process by default

        case PANEL_SCREEN_ID_SECURITY_BUT:
            if((objMsg == BTN_MSG_RELEASED))
            {
                screenState = CREATE_SECURITY; 
            }            
            return (1); // process by default
            
        case PANEL_SCREEN_ID_ENERGY_USAGE_BUT:
            if((objMsg == BTN_MSG_RELEASED))
            {
                screenState = CREATE_ENERGY; 
            }
            return (1); // process by default
            

        case PANEL_SCREEN_ID_EXIT_BUT:
            if((objMsg == BTN_MSG_RELEASED))
            {
                GFXTransition(0,0,(GetMaxX() >> 2) + 10,GetMaxY(),
                           PUSH,GFXGetPageOriginAddress(1),GFXGetPageOriginAddress(0),
                           2,8,RIGHT_TO_LEFT);       

                GFXTransition(0,0,(GetMaxX() >> 2) + 10,GetMaxY(),
                           SLIDE,GFXGetPageOriginAddress(GFX_PAGE2),GFXGetPageOriginAddress(0),
                          2,8,LEFT_TO_RIGHT);  

                CopyPageWindow( 1, GetDrawBufferAddress(),       
                           0, 0, 0, 0, 
                        GetMaxX(), GetMaxY());

                screenState = CREATE_MAIN; 

                return (0);                             // process by default
            }
            return (1);

        default:
            return (0);                             // process by default
    }
}
