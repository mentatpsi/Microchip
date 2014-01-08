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
#include "gfx_strings.h"
#include "gfx_screens.h"
#include "gol_ids.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define CONFIG_BUTTON_HEIGHT    (GetMaxY() / 9)
#define CONFIG_BUTTON_RADIUS    7

#define GFX_BlockUntilFinished(function)   while(!function)
/*****************************************************************************
 * SECTION: Variables
 *****************************************************************************/
static short alphalevel = 50;       // This is 50%
static GOL_IDS configButtonId;
/*****************************************************************************
 * void CreateConfigScreen(void)
 *****************************************************************************/
void CreateConfigScreen(void)
{
    GOL_SCHEME *currentScheme;
    
    currentScheme = GFX_SchemeGetCurrentScheme();

    configButtonId = CONFIG_SCREEN_ID_BACKGROUND_BUT;

    SetColor(currentScheme->Color0);	 

    FillBevel((GetMaxX() >> 2)+20,10 ,(GetMaxX() - 10), 60,5);
    PutImage((GetMaxX() >> 2)+20, 10, (void *) &preferences_system, IMAGE_NORMAL);
    FillBevel((GetMaxX() >> 2) + 20,90 ,GetMaxX() - 10, GetMaxY()-10,5);

    while(!AlphaBlendWindow(GetDrawBufferAddress(), (GetMaxX() >> 2)+15, 5,
			     GFX_PAGE1, (GetMaxX() >> 2)+15, 5,
			     GetDrawBufferAddress(), (GetMaxX() >> 2)+15, 5,
		         (GetMaxX()) - ((GetMaxX() >> 2)+20), 
		         60,  	
		         GFX_SchemeGetDefaultScheme()->AlphaValue));
    		     	

    while(!AlphaBlendWindow(GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
			     GFX_PAGE1, (GetMaxX() >> 2) + 15, 85,
			     GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
		         (GetMaxX())-((GetMaxX() >> 2) + 15), 
		         GetMaxY() - 90,  	
		         GFX_SchemeGetDefaultScheme()->AlphaValue));		        

    SetColor(RGB565CONVERT(255, 102, 0));
    SetFont((void *) &FONTDEFAULT);

    MoveTo((GetMaxX() >> 2)+80, 20);
    GFX_BlockUntilFinished(OutText((XCHAR *)ConfigStr));
                            
    BtnCreate
    (
        CONFIG_SCREEN_ID_BACKGROUND_BUT,                         // button ID
        (GetMaxX() >> 2)+100,
        (CONFIG_BUTTON_HEIGHT *3) + 10,//-40,
        (GetMaxX() >> 2)+360,
        (CONFIG_BUTTON_HEIGHT << 2) + 10,//-40,                 // dimension
        CONFIG_BUTTON_RADIUS,
        BTN_DRAW,                           // will be dislayed after creation
        NULL,           // use bitmap
        (XCHAR *)BackgroundStr,                   // "HOME", 	    // text
        currentScheme
    );                                      // alternative GOL scheme
    BtnCreate
    (
        CONFIG_SCREEN_ID_ALPHA_BUT,                         // button ID
        (GetMaxX() >> 2)+100,
        (CONFIG_BUTTON_HEIGHT << 2) + 20,//-40,
        (GetMaxX() >> 2)+360,
        (CONFIG_BUTTON_HEIGHT * 5) + 20,//-40,                 // dimension
        CONFIG_BUTTON_RADIUS,                                 // set radius
        BTN_DRAW ,              // draw a vertical capsule button
        NULL, // no bitmap
        (XCHAR *)AlphaValueStr,                    // "LO",       	// text
        currentScheme
    );                          // use alternate scheme
    BtnCreate
    (
        CONFIG_SCREEN_ID_PALETTE_BUT,             // button ID
        (GetMaxX() >> 2)+100,
        (CONFIG_BUTTON_HEIGHT *5) + 30,//-40,
        (GetMaxX() >> 2)+360,
        (CONFIG_BUTTON_HEIGHT * 6) +  30,//-40,      // dimension
        CONFIG_BUTTON_RADIUS,                     // set radius
        BTN_DRAW,               // draw a vertical capsule button
        NULL,                   // no bitmap
        (XCHAR *)PalletteStr,         // "ON",		// text
        currentScheme
    );                          // use alternate scheme

    BtnCreate
    (
        CONFIG_SCREEN_ID_BACKLIGHT_BUT,             // button ID
        (GetMaxX() >> 2)+100,
        (CONFIG_BUTTON_HEIGHT *6) + 40,//-40,
        (GetMaxX() >> 2)+360,
        (CONFIG_BUTTON_HEIGHT * 7) +  40,//-40,      // dimension
        CONFIG_BUTTON_RADIUS,                     // set radius
        BTN_DRAW,               // draw a vertical capsule button
        NULL,                   // no bitmap
        (XCHAR *)BacklightStr,         // "ON",		// text
        currentScheme
    );                          // use alternate scheme

    BtnCreate
    (
        CONFIG_SCREEN_ID_PLUS_BUT,             // button ID
         (GetMaxX() >> 2)+120,
        (CONFIG_BUTTON_HEIGHT * 7) +  10+40,
        (GetMaxX() >> 2)+220,
        (CONFIG_BUTTON_HEIGHT * 8) + 10+40,     // dimension
        CONFIG_BUTTON_RADIUS,                     // set radius
        BTN_DRAW, // draw a vertical capsule button
        (void *) &list_add,//NULL,                       // no bitmap
        NULL,                    // "+",      	// text
        currentScheme
    );                             // use alternate scheme 

    BtnCreate
    (
        CONFIG_SCREEN_ID_MINUS_BUT,             // button ID
         (GetMaxX() >> 2)+240,
        (CONFIG_BUTTON_HEIGHT * 7) +  10+40,
        (GetMaxX() >> 2)+340,
        (CONFIG_BUTTON_HEIGHT * 8) +  10+40,     // dimension
        CONFIG_BUTTON_RADIUS,                     // set radius
        BTN_DRAW, // draw a vertical capsule button
        (void *) &list_remove,                       // no bitmap
        NULL,                    // "-",      	// text
        currentScheme
    );                             // use alternate scheme 
}
/*****************************************************************************
 * WORD MsgConfigScreen(WORD objMsg, OBJ_HEADER *pObj)
 *****************************************************************************/
WORD MsgConfigScreen(WORD objMsg, OBJ_HEADER *pObj)
{
    GOL_SCHEME      *currentScheme;
    SHORT           change;
    static SHORT    dimamount =100;
    GFX_SCHEMES     schemeIdx;

    currentScheme = GFX_SchemeGetCurrentScheme();

    MsgMainScreen(objMsg, pObj);   // This function gets the messages from the other screen
    

    switch(GetObjID(pObj))
    {

        case CONFIG_SCREEN_ID_PLUS_BUT:
        case CONFIG_SCREEN_ID_MINUS_BUT:

            if(objMsg == BTN_MSG_RELEASED)
            {
                if(GetObjID(pObj) == CONFIG_SCREEN_ID_PLUS_BUT)  // Plus
                    change = 1;
                else 
                    change = -1;

                switch(configButtonId)
                {

                    case CONFIG_SCREEN_ID_BACKGROUND_BUT: //Background
                        if(change == 1)
                            SetPreLoadBackGroundScheme(PRE_LOAD_BACKGROUND_SCHEME_RAINBOW);
                        else if(change == -1)
                            SetPreLoadBackGroundScheme(PRE_LOAD_BACKGROUND_SCHEME_BLUE);
        
                        SetPreLoadPage(PRE_LOAD_PAGE_1);
                        screenState = CREATE_BACKGROUND;
                        break;
                      
                    case CONFIG_SCREEN_ID_ALPHA_BUT: //Alpha
                        if(change == -1)
                            alphalevel += 20;
                        else if(change == 1)
                            alphalevel -= 20;
                        
                        if(alphalevel > 100) 
                            alphalevel = 100;
                        else if(alphalevel < 0) 
                            alphalevel = 0;  
                        
                        GFX_SchemeSetAlpha(alphalevel);

                        SetState(GOLFindObject(CONFIG_SCREEN_ID_BACKGROUND_BUT), BTN_DRAW);
                        SetState(GOLFindObject(CONFIG_SCREEN_ID_ALPHA_BUT), BTN_DRAW);
                        SetState(GOLFindObject(CONFIG_SCREEN_ID_PALETTE_BUT), BTN_DRAW);
                        break;
                      
                    case CONFIG_SCREEN_ID_PALETTE_BUT: //Pallette
                        schemeIdx = GFX_SchemeGetCurrentSchemeIdx();

                        schemeIdx += change;
                        
                        if(schemeIdx >= NUM_GFX_SCHEMES) 
                            schemeIdx = GFX_SCHEME_1;
                        else if(schemeIdx < GFX_SCHEME_1) 
                            schemeIdx = NUM_GFX_SCHEMES - 1;

                        GFX_SchemeSetCurrentSchemeIdx(schemeIdx);
                        currentScheme = GFX_SchemeGetCurrentScheme();

                        GOLSetScheme(GOLFindObject(CONFIG_SCREEN_ID_BACKGROUND_BUT), currentScheme);
                        SetState(GOLFindObject(CONFIG_SCREEN_ID_BACKGROUND_BUT), BTN_DRAW);
                        
                        GOLSetScheme(GOLFindObject(CONFIG_SCREEN_ID_ALPHA_BUT), currentScheme);
                        SetState(GOLFindObject(CONFIG_SCREEN_ID_ALPHA_BUT), BTN_DRAW);
                        
                        GOLSetScheme(GOLFindObject(CONFIG_SCREEN_ID_PALETTE_BUT), currentScheme);
                        SetState(GOLFindObject(CONFIG_SCREEN_ID_PALETTE_BUT), BTN_DRAW);
                        break;

                     case CONFIG_SCREEN_ID_BACKLIGHT_BUT: //Backlight
                        if(change == -1)
                            dimamount -= 5;
                        else if(change == 1)
                            dimamount += 5;
                        
                        if(dimamount > 100) 
                            dimamount = 100;
                        else if(dimamount < 0) 
                            dimamount = 0; 

                         DisplayBrightness(dimamount);    
                        break;

                    default:
                        break;
                }
            }             
            return (1);  
    
        case CONFIG_SCREEN_ID_BACKGROUND_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                ClrState(GOLFindObject(CONFIG_SCREEN_ID_PALETTE_BUT), BTN_DISABLED); 
                SetState(GOLFindObject(CONFIG_SCREEN_ID_PALETTE_BUT), BTN_DRAW);
                ClrState(GOLFindObject(CONFIG_SCREEN_ID_ALPHA_BUT), BTN_DISABLED); 
                SetState(GOLFindObject(CONFIG_SCREEN_ID_ALPHA_BUT), BTN_DRAW); 
                ClrState(GOLFindObject(CONFIG_SCREEN_ID_BACKLIGHT_BUT), BTN_DISABLED); 
                SetState(GOLFindObject(CONFIG_SCREEN_ID_BACKLIGHT_BUT), BTN_DRAW); 
                SetState(pObj, BTN_DRAW|BTN_DISABLED);
                configButtonId = CONFIG_SCREEN_ID_BACKGROUND_BUT;             
            }
            return (1);    

        case CONFIG_SCREEN_ID_ALPHA_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                ClrState(GOLFindObject(CONFIG_SCREEN_ID_BACKGROUND_BUT), BTN_DISABLED); 
                SetState(GOLFindObject(CONFIG_SCREEN_ID_BACKGROUND_BUT), BTN_DRAW);
                ClrState(GOLFindObject(CONFIG_SCREEN_ID_PALETTE_BUT), BTN_DISABLED); 
                SetState(GOLFindObject(CONFIG_SCREEN_ID_PALETTE_BUT), BTN_DRAW);
                ClrState(GOLFindObject(CONFIG_SCREEN_ID_BACKLIGHT_BUT), BTN_DISABLED); 
                SetState(GOLFindObject(CONFIG_SCREEN_ID_BACKLIGHT_BUT), BTN_DRAW); 
                SetState(pObj, BTN_DRAW|BTN_DISABLED);
                configButtonId = CONFIG_SCREEN_ID_ALPHA_BUT;             
            }
            return (1);    

        case CONFIG_SCREEN_ID_PALETTE_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                ClrState(GOLFindObject(CONFIG_SCREEN_ID_BACKGROUND_BUT), BTN_DISABLED); 
                SetState(GOLFindObject(CONFIG_SCREEN_ID_BACKGROUND_BUT), BTN_DRAW);
                ClrState(GOLFindObject(CONFIG_SCREEN_ID_ALPHA_BUT), BTN_DISABLED); 
                SetState(GOLFindObject(CONFIG_SCREEN_ID_ALPHA_BUT), BTN_DRAW);
                ClrState(GOLFindObject(CONFIG_SCREEN_ID_BACKLIGHT_BUT), BTN_DISABLED); 
                SetState(GOLFindObject(CONFIG_SCREEN_ID_BACKLIGHT_BUT), BTN_DRAW);  
                SetState(pObj, BTN_DRAW|BTN_DISABLED);  
                configButtonId = CONFIG_SCREEN_ID_PALETTE_BUT;          
            }
            return (1); 

        case CONFIG_SCREEN_ID_BACKLIGHT_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                ClrState(GOLFindObject(CONFIG_SCREEN_ID_BACKGROUND_BUT), BTN_DISABLED); 
                SetState(GOLFindObject(CONFIG_SCREEN_ID_BACKGROUND_BUT), BTN_DRAW);
                ClrState(GOLFindObject(CONFIG_SCREEN_ID_ALPHA_BUT), BTN_DISABLED); 
                SetState(GOLFindObject(CONFIG_SCREEN_ID_ALPHA_BUT), BTN_DRAW); 
                ClrState(GOLFindObject(CONFIG_SCREEN_ID_PALETTE_BUT), BTN_DISABLED); 
                SetState(GOLFindObject(CONFIG_SCREEN_ID_PALETTE_BUT), BTN_DRAW); 
                SetState(pObj, BTN_DRAW|BTN_DISABLED);  
                configButtonId = CONFIG_SCREEN_ID_BACKLIGHT_BUT;          
            }                          

        default:
            return (1);                             // process by default

    }
}
