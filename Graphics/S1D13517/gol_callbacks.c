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
#include "gfx_strings.h"
#include "gfx_screens.h"
#include "gfx_schemes.h"
#include "gol_ids.h"
#include "timer_tick.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define GFX_BlockUntilFinished(function)   while(!function)

/*****************************************************************************
 * SECTION: Externs
 *****************************************************************************/
extern BYTE backgroundScheme;
/*****************************************************************************
 * SECTION: Variables
 *****************************************************************************/

/************************************************************************
 Function: WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)

 Overview: This function must be implemented by user. GOLMsg() function 
 		   calls it each time the valid message for the object received.
           
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 

 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{

    // Process messages for demo screens. Each of the
    // demo screens will have its own corresponding message callback
    // functions.
    switch(screenState)
    {

       	case DISPLAY_MAIN:
            return (MsgMainScreen(objMsg, pObj));
    
        case DISPLAY_PERFORMANCE:
        case DISPLAY_GRADIENT:
        case DISPLAY_ALPHABLEND:
        case DISPLAY_24BPPCOLOR:
        case DISPLAY_SPEED:
            return (MsgPerformanceScreen(objMsg, pObj));

        case DISPLAY_CONFIG:
            return (MsgConfigScreen(objMsg, pObj));

        case DISPLAY_COMFORT:
            return (MsgComfortScreen(objMsg, pObj));

        case DISPLAY_LIGHTING:
            return (MsgLightingScreen(objMsg, pObj));

        case DISPLAY_ENERGY:
            return (MsgPanelScreen(objMsg, pObj));

        case DISPLAY_SECURITY:
            return (MsgSecurityScreen(objMsg, pObj, pMsg));
        
        case DISPLAY_INFO:
        	return (MsgMainScreen(objMsg, pObj));  
			
        default:
            return (1); // process message by default
    }
}

/************************************************************************
 Function: WORD GOLDrawCallback()

 Overview: This function must be implemented by user. GOLDraw() function 
 		   calls it each time when GOL objects drawing is completed. User 
 		   drawing should be done here. GOL will not change color, line 
 		   type and clipping region settings while  this function 
 		   returns zero.

 Input: none

 Output: If the function returns non-zero the draw control will be passed 
		to GOL. GOLDraw() can proceed and re-draw objects that needs 
		to be redrawn.
************************************************************************/
WORD GOLDrawCallback(void)
{
    static DWORD    timeClock = 0;
    ANALOGCLOCK *pAc;

    switch(screenState)
    {

        case CREATE_BACKGROUND:
            screenState = DISPLAY_BACKGROUND;     // switch to next state
            return (1);

        case DISPLAY_BACKGROUND: 
            CreatePreLoadScreen();
            screenState = GOL_BACKGROUNDDRAW;
            return(1);

        case GOL_BACKGROUNDDRAW:      //Draw Background Buffers
        	return PreLoadDrawScreens();

        case CREATE_MAIN:
            GFX_DRIVER_HideLayer(1);
            SwitchOnDoubleBuffering();  
            CreateMainScreen();
            screenState = DISPLAY_MAIN;                                 // switch to next state
            timeClock =0;
            return (1);                                                 // draw objects created

        case DISPLAY_MAIN:

            if(timeClock == 0)     //Make sure clock is actually drawn 
            {
                pAc=(ANALOGCLOCK *)GOLFindObject(MAIN_SCREEN_ID_ANALOG_CLOCK);
                SetState(pAc, AC_DRAW);
                timeClock = TimerTick_GetTick();
            }

            if(TimerTick_GetTick() - timeClock >= 500)
            {
                timeClock = TimerTick_GetTick();
                pAc=(ANALOGCLOCK *)GOLFindObject(MAIN_SCREEN_ID_ANALOG_CLOCK);
                SetState(pAc, AC_TICK);
            }
 
            return (1);     
                        
        case CREATE_INFO:                     //Use PIP for this screen
            SwitchOffDoubleBuffering();
            GFX_DRIVER_CreateLayer(1,1,164,0,GetMaxX()-164,GetMaxY());
            GFX_DRIVER_LayerStartAddress(1,6,160,0);
            GFX_DRIVER_ShowLayer(1);
        	CreateInfo(1);
            screenState = DISPLAY_INFO;     // switch to next state
            return (1);                                                

  #ifndef __PIC32MX                       
 
       case CREATE_UPLOAD:
            UploadExternalMemory();
            screenState = DISPLAY_UPLOAD;
            return (1);

        case DISPLAY_UPLOAD:
            #if defined(ProgramFlash)
            ProgramFlash();
            #endif
            return (1);
  #endif
        case CREATE_PERFORMANCE:
            GFX_DRIVER_HideLayer(1); 
            SwitchOnDoubleBuffering(); 
            CreatePerformanceScreen();                                       // create window and radio buttons
            screenState = DISPLAY_PERFORMANCE;                         // switch to next state
            return (1);                                                 // draw objects created

        case DISPLAY_PERFORMANCE:
        	screenState = CREATE_24BPPCOLOR;
            return (1);                                                 // redraw objects if needed

		case CREATE_24BPPCOLOR:
			CreateColorDepthScreen();
            screenState = DISPLAY_24BPPCOLOR;                        // switch to next state
            return (1);                                                 // draw objects created

		case CREATE_SPEED:
			CreateSpeed();
            screenState = DISPLAY_SPEED;                       // switch to next state
            return (1);                                                 // draw objects created
	           
        case CREATE_CONFIG:
            GFX_DRIVER_HideLayer(1);
            SwitchOnDoubleBuffering();  
            CreateConfigScreen();                                         // create window, group box, static text
            screenState = DISPLAY_CONFIG;                           // switch to next state
            return (1);                                                 // draw objects created

        case CREATE_COMFORT:
            CreateComfortScreen();                     // create window and sliders
            screenState = DISPLAY_COMFORT;   // switch to next state
            return (1);                         // draw objects created

        case CREATE_SCROLLING:           
            
            GFX_DRIVER_HideLayer(1);    
            SwitchOnDoubleBuffering();
            GFXTransition(0,0,GetMaxX(),GetMaxY(),
                       PUSH,GFXGetPageOriginAddress(2),GFXGetPageOriginAddress(0),
                       10,64,LEFT_TO_RIGHT);

            GFXTransition(0,0,GetMaxX(),GetMaxY(),
                       PUSH,GFXGetPageOriginAddress(3),GFXGetPageOriginAddress(0),
                       10,40,TOP_TO_BOTTOM);

            GFXTransition(0,0,GetMaxX(),GetMaxY(),
                       PUSH,GFXGetPageOriginAddress(4),GFXGetPageOriginAddress(0),
                       10,8,RIGHT_TO_LEFT);


            GFXTransition(0,0,GetMaxX(),GetMaxY(),
                       PUSH,GFXGetPageOriginAddress(1),GFXGetPageOriginAddress(0),
                       10,40,BOTTOM_TO_TOP);

            CopyPageWindow( 2, GetDrawBufferAddress(),       
                           0, 0, 0, 0, 
                        GetMaxX(), GetMaxY());

            pAc=(ANALOGCLOCK *)GOLFindObject(MAIN_SCREEN_ID_ANALOG_CLOCK);
            AcSetSecond(pAc,0);

            screenState = DISPLAY_MAIN;
            return (1);                         // draw objects created

        case CREATE_LIGHTING:
            CreateLightingScreen();
            screenState = DISPLAY_LIGHTING;  // switch to next state
            return (1);                         // draw objects created

        case DISPLAY_LIGHTING: 
            DisplayLightingScreen();
            return (1);                         // redraw objects if needed

        case CREATE_SECURITY:
            CreateSecurityScreen();                    // create edit box test screen
            screenState = DISPLAY_SECURITY;      // switch to next state
            return (1);                         // draw objects created

        case CREATE_ENERGY:
            CreateEnergyScreen();                    // create list box test screen
            screenState = DISPLAY_ENERGY;      // switch to next state
            return (1);                         // draw objects created

        case CREATE_GRADIENT:
            CreateGradientScreen();                  // create window
            screenState = DISPLAY_GRADIENT;   // switch to next state
            return (1);                         // draw objects created

        case DISPLAY_SCROLLING:
        case DISPLAY_SECURITY:
        case DISPLAY_ENERGY:
        case DISPLAY_GRADIENT:
        case DISPLAY_COMFORT:
        case DISPLAY_SPEED:
	    case DISPLAY_24BPPCOLOR:
        case DISPLAY_CONFIG:
        case DISPLAY_INFO:
            return (1);                         // redraw objects if needed

        case DISPLAY_ALPHABLEND:
        	DisplayAlphaBlendScreen(); 
            return (1);                         // redraw objects if needed           

        default:
            break;
    }

    return (1); // release drawing control to GOL
}
