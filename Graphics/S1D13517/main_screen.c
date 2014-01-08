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
#define MAIN_SCREEN_BUTTON_HEIGHT       (GetMaxY() / 10)
#define MAIN_SCREEN_BUTTON_RADIUS       7
#define MAIN_SCRREN_DISPLAY_BUFFER      GFX_PAGE0

#define GFX_BlockUntilFinished(function)   while(!function)

/*****************************************************************************
 * SECTION: Constants
 *****************************************************************************/
const XCHAR mainScreenStr[] = { 'H', 'o', 'm', 'e', '\0' };    
/*****************************************************************************
 * void CreateMainScreen(void)
 *****************************************************************************/
void CreateMainScreen(void)
{
    GOL_SCHEME *currentScheme;

    currentScheme = GFX_SchemeGetCurrentScheme();

    while(!AlphaBlendWindow(GFX_PAGE1, (GetMaxX() >> 2), 0,
   	 		      GFX_PAGE1, (GetMaxX() >> 2), 0,
	 		      MAIN_SCRREN_DISPLAY_BUFFER, (GetMaxX() >> 2), 0,
	              GetMaxX()-(GetMaxX() >> 2), 
	    	      GetMaxY(),   	
	    	      GFX_SchemeGetDefaultScheme()->AlphaValue));						        

   	SetColor(ORANGE);                       // Orange
   	while(!Bar((GetMaxX() >> 2), 0 , (GetMaxX() >> 2) + 2, GetMaxY()));

    GOLFree();                              // free memory for the objects in the previous linked list and start new list

    CopyPageWindow( 1, GetDrawBufferAddress(),       
                           0, 0, 0, 0, 
                        (GetMaxX() >> 2) - 5, GetMaxY());

    BtnCreate
    (
        MAIN_SCREEN_ID_PANEL_BUT,                         // button ID
        5,
        MAIN_SCREEN_BUTTON_HEIGHT << 1,
        (GetMaxX() >> 2) - 5,
        MAIN_SCREEN_BUTTON_HEIGHT *3,                                // dimension
        MAIN_SCREEN_BUTTON_RADIUS,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)PanelDemoStr,                 // "Button",     	// text
        currentScheme
    ); 
    BtnCreate
    (
        MAIN_SCREEN_ID_SLIDE_SHOW_BUT,                         // button ID
        5,
        (MAIN_SCREEN_BUTTON_HEIGHT *3) + 10,
        (GetMaxX() >> 2) - 5,
        (MAIN_SCREEN_BUTTON_HEIGHT << 2) + 10,                 // dimension
        MAIN_SCREEN_BUTTON_RADIUS,
        BTN_DRAW,                           // will be dislayed after creation
        NULL,           // use bitmap
        (XCHAR *)SlideShowStr,                   // "HOME", 	    // text
        currentScheme
    );                                      // alternative GOL scheme
    BtnCreate
    (
        MAIN_SCREEN_ID_PERFORMANCE_BUT,                         // button ID
        5,
        (MAIN_SCREEN_BUTTON_HEIGHT << 2) + 20,
        (GetMaxX() >> 2) - 5,
        (MAIN_SCREEN_BUTTON_HEIGHT * 5) + 20,                 // dimension
        MAIN_SCREEN_BUTTON_RADIUS,                                 // set radius
        BTN_DRAW ,              // draw a vertical capsule button
        NULL, // no bitmap
        (XCHAR *)PerformanceStr,                    // "LO",       	// text
        currentScheme
    );                          // use alternate scheme
    BtnCreate
    (
        MAIN_SCREEN_ID_CONFIG_BUT,             // button ID
        5,
        (MAIN_SCREEN_BUTTON_HEIGHT * 5) + 30,
        (GetMaxX() >> 2) - 5,
        (MAIN_SCREEN_BUTTON_HEIGHT * 6) +  30,      // dimension
        MAIN_SCREEN_BUTTON_RADIUS,                // set radius
        BTN_DRAW,                     // draw a vertical capsule button
        NULL,                         // no bitmap
        (XCHAR *)ConfigStr,           // "ON",		// text
        currentScheme
    );                          // use alternate scheme
    BtnCreate
    (
        MAIN_SCREEN_ID_INFO_BUT,             // button ID
        5,
        (MAIN_SCREEN_BUTTON_HEIGHT * 6) +  40,
        (GetMaxX() >> 2) - 5,
        (MAIN_SCREEN_BUTTON_HEIGHT * 7) +  40,     // dimension
        MAIN_SCREEN_BUTTON_RADIUS,                     // set radius
        BTN_DRAW, // draw a vertical capsule button
        NULL,  // no bitmap
        (XCHAR *)InfoStr,                    // "OFF",      	// text
        currentScheme
    );                      // use alternate scheme 

    AcCreate
    (
        MAIN_SCREEN_ID_ANALOG_CLOCK,                         // button ID
        (GetMaxX() >> 2) + 20, 
        90, 
        GetMaxX() - 10,
        GetMaxY()-10,
        5,  //Set Hour 
        30, //Set Minute  
        150, 
        TRUE,
        AC_DRAW | AC_DISABLED,                           // draw a beveled button
        NULL,
        currentScheme
    );
  
	SetColor(currentScheme->Color0);	
   
	while(!FillBevel((GetMaxX() >> 2)+20,10 ,(GetMaxX() - 10), 60,5));
	while(!PutImage((GetMaxX() >> 2)+20, 10, (void *) &file_browser, IMAGE_NORMAL));
     
	while(!AlphaBlendWindow(GetDrawBufferAddress(), (GetMaxX() >> 2)+15, 5,
					 GFX_PAGE1, (GetMaxX() >> 2)+15, 5,
					 GetDrawBufferAddress(), (GetMaxX() >> 2)+15, 5,
				     (GetMaxX()) - ((GetMaxX() >> 2)+20), 
				     60,  	
				     GFX_SchemeGetDefaultScheme()->AlphaValue));	
				     		     				  
	SetColor(RGB565CONVERT(255, 102, 0));
    SetFont((void *) &FONTDEFAULT);

    MoveTo((GetMaxX() >> 2) + 80, 20);
    GFX_BlockUntilFinished(OutText((XCHAR *)mainScreenStr));
 	
}
/*****************************************************************************
 * WORD MsgMainScreen(WORD objMsg, OBJ_HEADER *pObj)
 *****************************************************************************/
WORD MsgMainScreen(WORD objMsg, OBJ_HEADER *pObj)
{

    switch(GetObjID(pObj))
    {   
        case MAIN_SCREEN_ID_PANEL_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {  
               GFXTransition(0,0,(GetMaxX() >> 2) + 10,GetMaxY(),
                       PUSH,GFXGetPageOriginAddress(1),GFXGetPageOriginAddress(0),
                       2,8,RIGHT_TO_LEFT);

               GFXTransition(0,0,(GetMaxX() >> 2) + 10,GetMaxY(),
                       SLIDE,GFXGetPageOriginAddress(3),GFXGetPageOriginAddress(0),
                       2,8,LEFT_TO_RIGHT);

                CopyPageWindow( 1, GetDrawBufferAddress(),       
                           0, 0, 0, 0, 
                        GetMaxX(), GetMaxY());

				screenState = CREATE_COMFORT;      
            
                return (0);                 // process by default

            }
            return (1);                 // process by default

         case MAIN_SCREEN_ID_SLIDE_SHOW_BUT:

            if(objMsg == BTN_MSG_RELEASED)
            {         
                      screenState = CREATE_SCROLLING;            
            }
            return (1);                 // process by default

        case MAIN_SCREEN_ID_CONFIG_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                 
		         ClrState(GOLFindObject(MAIN_SCREEN_ID_INFO_BUT),BTN_DISABLED);
                 SetState(GOLFindObject(MAIN_SCREEN_ID_INFO_BUT),BTN_DRAW);
                 SetState(GOLFindObject(MAIN_SCREEN_ID_CONFIG_BUT),BTN_DRAW|BTN_DISABLED);
                 screenState = CREATE_CONFIG;
            }

            return (1);                 // Do not process by default

        case MAIN_SCREEN_ID_INFO_BUT:    //Info
            if(objMsg == BTN_MSG_RELEASED)
            {
				 screenState = CREATE_INFO;  
            }
            return (1);                 // Do not process by default

        case MAIN_SCREEN_ID_UPLOAD_BUT:    //Upload

            if(objMsg == BTN_MSG_RELEASED)
            {
				 screenState = CREATE_UPLOAD;  
            }
            return (1);                 // Do not process by default

        case MAIN_SCREEN_ID_PERFORMANCE_BUT:   //Performance Screen
            if((objMsg == BTN_MSG_RELEASED))
            {
               GFXTransition(0,0,(GetMaxX() >> 2) + 10,GetMaxY(),
                       PUSH,GFXGetPageOriginAddress(1),GFXGetPageOriginAddress(0),
                       2,8,RIGHT_TO_LEFT);

               GFXTransition(0,0,(GetMaxX() >> 2) + 10,GetMaxY(),
                       SLIDE,GFXGetPageOriginAddress(4),GFXGetPageOriginAddress(0),
                       2,8,LEFT_TO_RIGHT); 

                CopyPageWindow( 1, GetDrawBufferAddress(),       
                           0, 0, 0, 0, 
                        GetMaxX(), GetMaxY());

               screenState = CREATE_PERFORMANCE;  
   
               return (0);                 // process by default
            }
            return (1);                 // process by default

        default:
            return (1);                 // process by default
    }
}
