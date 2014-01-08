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
#include "stopwatch.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define PERFORMANCE_BUTTON_HEIGHT    (GetMaxY() / 9)
#define PERFORMANCE_BUTTON_RADIUS    7

#define GFX_BlockUntilFinished(function)   while(!function)
/*****************************************************************************
 * SECTION: Local Function Prototypes
 *****************************************************************************/
float FPS_PPS(void);
void CharStringToXCharString(XCHAR *xstring, char *string);
/*****************************************************************************
 * void CreatePerformanceScreen(BYTE drawFlag)
 *****************************************************************************/
void CreatePerformanceScreen(void)
{
    GOL_SCHEME *currentScheme;

    currentScheme = GFX_SchemeGetCurrentScheme();

   	SetColor(ORANGE);                       // Orange
   	Bar((GetMaxX() >> 2), 0 , (GetMaxX() >> 2) + 2, GetMaxY());

    SetColor(currentScheme->Color0);

	while(!FillBevel((GetMaxX() >> 2)+20,10 ,(GetMaxX() - 10), 60,5));
	PutImage((GetMaxX() >> 2)+20, 10, (void *) &appointment_new, IMAGE_NORMAL);
	while(!FillBevel((GetMaxX() >> 2) + 20,90 ,GetMaxX() - 10, GetMaxY()-10,5));
 
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
    GFX_BlockUntilFinished(OutText((XCHAR *)PerformanceStr));

    GOLFree();                              // free memory for the objects in the previous linked list and start new list

    BtnCreate
    (
        PERFORMANCE_SCREEN_ID_COLOR_BUT,                         // button ID
        5,
        PERFORMANCE_BUTTON_HEIGHT << 1,
        (GetMaxX() >> 2) - 5,
        PERFORMANCE_BUTTON_HEIGHT *3,                                // dimension
        PERFORMANCE_BUTTON_RADIUS,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)ColorStr,                 // "Button",     	// text
        currentScheme
    ); 
                                     // use alternate scheme
    BtnCreate
    (
        PERFORMANCE_SCREEN_ID_SPEED_BUT,                         // button ID
        5,
        (PERFORMANCE_BUTTON_HEIGHT *3) + 10,
        (GetMaxX() >> 2) - 5,
        (PERFORMANCE_BUTTON_HEIGHT << 2) + 10,                 // dimension
        PERFORMANCE_BUTTON_RADIUS,
        BTN_DRAW,                           // will be dislayed after creation
        NULL,           // use bitmap
        (XCHAR *)SpeedStr,                   // "HOME", 	    // text
        currentScheme
    );                                      // alternative GOL scheme
    BtnCreate
    (
        PERFORMANCE_SCREEN_ID_GRADIENT_BUT,                         // button ID
        5,
        (PERFORMANCE_BUTTON_HEIGHT << 2) + 20,
        (GetMaxX() >> 2) - 5,
        (PERFORMANCE_BUTTON_HEIGHT * 5) + 20,                 // dimension
        PERFORMANCE_BUTTON_RADIUS,                                 // set radius
        BTN_DRAW ,              // draw a vertical capsule button
        NULL, // no bitmap
        (XCHAR *)GradientStr,                    // "LO",       	// text
        currentScheme
    );                          // use alternate scheme
    BtnCreate
    (
        PERFORMANCE_SCREEN_ID_BLENDING_BUT,             // button ID
        5,
        (PERFORMANCE_BUTTON_HEIGHT *5) + 30,
        (GetMaxX() >> 2) - 5,
        (PERFORMANCE_BUTTON_HEIGHT * 6) +  30,      // dimension
        PERFORMANCE_BUTTON_RADIUS,                     // set radius
        BTN_DRAW,               // draw a vertical capsule button
        NULL,                   // no bitmap
        (XCHAR *)BlendingStr,         // "ON",		// text
        currentScheme
    );                          // use alternate scheme
    BtnCreate
    (
        PERFORMANCE_SCREEN_ID_EXIT_BUT,             // button ID
        5,
        (PERFORMANCE_BUTTON_HEIGHT * 6) +  40,
        (GetMaxX() >> 2) - 5,
        (PERFORMANCE_BUTTON_HEIGHT * 7) +  40,     // dimension
        PERFORMANCE_BUTTON_RADIUS,                     // set radius
        BTN_DRAW, // draw a vertical capsule button
        NULL,  // no bitmap
        (XCHAR *)ExitStr,                    // "OFF",      	// text
        currentScheme
    );                      // use alternate scheme 	
 
}
/*****************************************************************************
 * void CreatePerformanceScreen(void)
 *****************************************************************************/
WORD MsgPerformanceScreen(WORD objMsg, OBJ_HEADER *pObj)
{

    switch(GetObjID(pObj))
    {
	    
	    case PERFORMANCE_SCREEN_ID_COLOR_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            	screenState = CREATE_24BPPCOLOR;

            return (1);                 // process by default
            
 
 	    case PERFORMANCE_SCREEN_ID_SPEED_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            	screenState = CREATE_SPEED;
			
            return (1);                 // process by default

        case PERFORMANCE_SCREEN_ID_GRADIENT_BUT:   //Gradients
            if(objMsg == BTN_MSG_RELEASED)
            {          
                SetColor(GFX_SchemeGetCurrentScheme()->Color0);		 	 	     				     
                SetColor(RGB565CONVERT(255, 102, 0));
                SetFont((void *) &FONTDEFAULT);

                MoveTo((GetMaxX() >> 2)+80, 20);
                GFX_BlockUntilFinished(OutText((XCHAR *)PerformanceStr));
				 	 
                screenState = CREATE_GRADIENT; 

            }    
            return (1);        

        case PERFORMANCE_SCREEN_ID_BLENDING_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {          
                SetActivePage(IMAGE2_PINGPONG);        //Draw Image to be shown fading in
                SetColor(WHITE);
                BevelGradient((GetMaxX() >> 2) + 20,90,GetMaxX() - 10,GetMaxY()-10,5,BRIGHTBLUE,BRIGHTRED,100,GRAD_RIGHT);
                SetColor(RGB565CONVERT(255, 102, 0));
                SetFont((void *) &FONTDEFAULT);
                OutTextXY((GetMaxX() >> 2) + 60,150,(XCHAR *)FadingStr);
                SetActivePage(GetDrawBufferAddress());
              
                screenState = DISPLAY_ALPHABLEND;
            }    
            return (1);        
                       
        case PERFORMANCE_SCREEN_ID_EXIT_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {
      
                while(!AlphaBlendWindow(GetDrawBufferAddress(), 0, 0,
                                 GetDrawBufferAddress(), 0, 0,
                                 GFX_PAGE4, 0, 0,
                                 GetMaxX(), 
                                 GetMaxY(),   	
                                 GFX_SchemeGetDefaultScheme()->AlphaValue));   

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

            }
            return (1);                             // process by default

        default:
            return (1);                             // process by default
    }
}
/*****************************************************************************
 * void CreateColorDepthScreen(void)
 *****************************************************************************/
void CreateColorDepthScreen(void)
{
     
    SetColor(GFX_SchemeGetCurrentScheme()->Color0);	 	 

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
    GFX_BlockUntilFinished(OutText((XCHAR *)PerformanceStr));


    MoveTo((GetMaxX() >> 2) + 25,95);
    GFX_BlockUntilFinished(OutText((XCHAR *)C24bppStr));	
    MoveTo((GetMaxX() >> 2) + 25,120);
    GFX_BlockUntilFinished(OutText((XCHAR *)C24bppStr2));			    
			 	
}
/*****************************************************************************
 * void CreateSpeed(void)
 *****************************************************************************/
void CreateSpeed(void)
{
	XCHAR  wtext[50];
	char	text[50];
	float fps = 0;

	long cTmrCount=0;
	static const SHORT polyPoints[] = 
    {
	    (GetMaxX()+1)/2,    (GetMaxY()+1)/4,
	    (GetMaxX()+1)*3/4,  (GetMaxY()+1)/2,
	    (GetMaxX()+1)/2,    (GetMaxY()+1)*3/4,
	    (GetMaxX()+1)/4,    (GetMaxY()+1)/2,
	    (GetMaxX()+1)/2,    (GetMaxY()+1)/4,
	};


    SetColor(GFX_SchemeGetCurrentScheme()->Color0);

	while(!FillBevel((GetMaxX() >> 2) + 20,90 ,GetMaxX() - 10, GetMaxY()-10,5));
     
	while(!AlphaBlendWindow(GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
					 GFX_PAGE1, (GetMaxX() >> 2) + 15, 85,
					 GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
				     (GetMaxX())-((GetMaxX() >> 2) + 15), 
				     GetMaxY() - 90,  	
				     GFX_SchemeGetDefaultScheme()->AlphaValue));

    SetColor(RGB565CONVERT(255, 102, 0)); //Text Font Color
	fps = FPS_PPS();
	
	sprintf(text, "%s%d %s", "Width: ", GetMaxX()+1, " pixels");
	MoveTo((GetMaxX() >> 2) + 70,100);
	CharStringToXCharString(wtext,text);
	GFX_BlockUntilFinished(OutText((XCHAR *)wtext));		
	
	sprintf(text, "%s%d %s", "Height: ", GetMaxY()+1, " pixels");
	MoveTo((GetMaxX() >> 2) + 70,150);
	CharStringToXCharString(wtext,text);
	GFX_BlockUntilFinished(OutText((XCHAR *)wtext));				
	
	sprintf(text, "%s%1.3f %s", "Frames Per Second: ", (double)fps, "FPS");
	MoveTo((GetMaxX() >> 2) + 70,200);
	CharStringToXCharString(wtext,text);
	GFX_BlockUntilFinished(OutText((XCHAR *)wtext));
	
	sprintf(text, "%s%ld %s", "Pixels Per Second: ", (long)(fps * (float)GetMaxX() * (float)GetMaxY()), "PPS");
	MoveTo((GetMaxX() >> 2) + 70,250);
	CharStringToXCharString(wtext,text);
	GFX_BlockUntilFinished(OutText((XCHAR *)wtext));
	
	SetActivePage(IMAGE1_PINGPONG);
	
    Stopwatch_Start();
	DrawPoly(4, (SHORT *)polyPoints);
	cTmrCount = Stopwatch_Stop();

	SetActivePage(GetDrawBufferAddress());
	
    sprintf(text, "%s%1.3f", "Polygons Per Second: ", (double)((GetSystemClock()/(double)cTmrCount)));
	MoveTo((GetMaxX() >> 2) + 70,300);
	CharStringToXCharString(wtext,text);

	GFX_BlockUntilFinished(OutText((XCHAR *)wtext));	

}

/*****************************************************************************
 * float FPS_PPS(void)
 *****************************************************************************/
float FPS_PPS(void)
{
	DWORD cTmrCount=0;
	float framespersec = 0;
   
    SetActivePage(IMAGE1_PINGPONG);
    Stopwatch_Start();
    ClearDevice();
	cTmrCount = Stopwatch_Stop();
	framespersec = ((float)GetSystemClock() / (float)cTmrCount);
	
	SetActivePage(GetDrawBufferAddress());
	
	return(framespersec);	

}
/*****************************************************************************
 * void CharStringToXCharString(XCHAR *wtext, char *text)
 *****************************************************************************/
void CharStringToXCharString(XCHAR *xstring, char *string)
{
    while(*string != '\0')
    {
        *xstring = (XCHAR)*string;
        xstring++;
        string++;
    }

    *xstring = (XCHAR)*string;
}
