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
//#include "icons.h"
#include "MainDemo.h"//** Remove when complete!!!!!!!!!
#include "gfx_schemes.h"
#include "gfx_strings.h"
#include "gfx_screens.h"
#include "gol_ids.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define COMFORT_SCREEN_SIZE_X  (GetMaxX() - (GetMaxX() >> 2) - 100)
#define COMFORT_SCREEN_SIZE_Y  (GetMaxY()  - 120)
#define COMFORT_SCREEN_START   (GetMaxX() - COMFORT_SCREEN_SIZE_X - 50)
#define COMFORT_SCREEN_END     (GetMaxX() -50)
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
XCHAR SetHeatStr[]      = { '7','2','\0'};                   //Temperature that can be changed
XCHAR SetCoolStr[]      = { '7','8','\0'};                   //Temperature that can be changed
XCHAR STemperatureStr[] = { '7','5','\0'};                   //Temperature that can be changed
XCHAR *Level[2]         = { (XCHAR *)UpstairsStr,
                            (XCHAR *)DownstairsStr
                          };
XCHAR *LevelTemp[2]     = { (XCHAR *)SetHeatStr,
                            (XCHAR *)SetCoolStr
                          };

/*****************************************************************************
 * SECTION: Externs
 *****************************************************************************/
extern const FONT_FLASH     Large;          // equal width font

/*****************************************************************************
 * void CreateComfortScreen(void)
 *****************************************************************************/
void CreateComfortScreen(void)
{
    GOL_SCHEME *currentScheme;
    GOL_SCHEME *currentTemperatureScheme;
    GOL_SCHEME *defaultScheme;

    currentScheme               = GFX_SchemeGetCurrentScheme();
    currentTemperatureScheme    = GFX_SchemeGetCurrentTScheme();
    defaultScheme               = GFX_SchemeGetDefaultScheme();

    CreatePanelScreen();                    // This adds the widgets seen on the left of the screen
 
    SetState(GOLFindObject(PANEL_SCREEN_ID_COMFORT_BUT),BTN_DISABLED);

    SetFont((void *) &FONTDEFAULT);
    SetColor(RGB565CONVERT(255, 102, 0));//WHITE);
    OutTextXY(COMFORT_SCREEN_START + 150,90,(XCHAR *)Level[0]);
    SetFont((void *) &Large);
    OutTextXY(COMFORT_SCREEN_START + 10,190,(XCHAR *)LevelTemp[0]);

    SetFont((void *) &FONTDEFAULT);
    OutTextXY(COMFORT_SCREEN_START - 5,(GetMaxY()>>1) - 110,(XCHAR *)TemperatureStr);
    OutTextXY(COMFORT_SCREEN_END - 145,(GetMaxY()>>1) - 110,(XCHAR *)SetpointStr);
    OutTextXY(COMFORT_SCREEN_END - 140,(GetMaxY()>>1) - 80,(XCHAR *)HeatStr);
    OutTextXY(COMFORT_SCREEN_END - 140,(GetMaxY()>>1) ,(XCHAR *)CoolStr);

    StCreate
    (
        COMFORT_SCREEN_ID_COOL_STATIC_TXT,             // ID
        COMFORT_SCREEN_END - 135,
        (GetMaxY()>>1) + 25,
        COMFORT_SCREEN_END - 100,
         (GetMaxY()>>1) + 60,                                // dimension
        ST_DRAW,   // will be dislayed, has frame and center aligned
        (XCHAR *)SetCoolStr,                            // text is from time value
        currentTemperatureScheme
    );                  // alternate scheme

    StCreate
    (
        COMFORT_SCREEN_ID_HEAT_STATIC_TXT,             // ID
        COMFORT_SCREEN_END - 135,
        (GetMaxY()>>1) - 55,
        COMFORT_SCREEN_END - 100,
         (GetMaxY()>>1) - 21,                                // dimension
        ST_DRAW,   // will be dislayed, has frame and center aligned
        (XCHAR *)SetHeatStr,                            // text is from time value
        currentTemperatureScheme
    );                  // alternate scheme


    BtnCreate    //Left Arrow
    (
        COMFORT_SCREEN_ID_LEFT_ARROW_BUT,                         // button ID
        COMFORT_SCREEN_START + 10,
        70+20,
        COMFORT_SCREEN_START + 110,
        110+20,                                // dimension
        0,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)LftStr,                 // "Button",     	// text
        currentScheme//ComfortScheme
    ); 

    BtnCreate    //Right Arrow
    (
        COMFORT_SCREEN_ID_RIGHT_ARROW_BUT,                         // button ID
        COMFORT_SCREEN_END - 110,
        70+20,
        COMFORT_SCREEN_END - 10,
        110+20,                                // dimension
        0,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)RghtStr,                 // "Button",     	// text
        currentScheme//ComfortScheme
    ); 

    
    BtnCreate         //AUto
    (
        COMFORT_SCREEN_ID_AUTO_BUT,                         // button ID
        COMFORT_SCREEN_START - 5,
        GetMaxY()-140,
        COMFORT_SCREEN_START + 115,
        GetMaxY()-100,                                // dimension
        0,                                 // set radius
        BTN_DRAW|BTN_DISABLED,               // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)AUTOStr,                 // "Button",     	// text
        currentScheme//ComfortScheme
    ); 
   
    BtnCreate      //HEat
    (
        COMFORT_SCREEN_ID_HEAT_BUT,                         // button ID
        COMFORT_SCREEN_START - 5,
        GetMaxY() - 95,
        COMFORT_SCREEN_START + 115,
        GetMaxY() - 55,                                // dimension
        0,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)HEATStr,                 // "Button",     	// text
        currentScheme//ComfortScheme
    ); 
   
    BtnCreate       //OFF
    (
        COMFORT_SCREEN_ID_OFF_BUT,                         // button ID
        COMFORT_SCREEN_START + 117,
         GetMaxY()-140,
        COMFORT_SCREEN_START + 237,
         GetMaxY()-100,                                // dimension
        0,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)OFFStr,                 // "Button",     	// text
        currentScheme//ComfortScheme
    ); 
   
    BtnCreate       //COOL
    (
        COMFORT_SCREEN_ID_COOL_BUT,                         // button ID
        COMFORT_SCREEN_START + 117,
        GetMaxY() - 95,
        COMFORT_SCREEN_START + 237,
        GetMaxY() - 55,                                // dimension
        0,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)COOLStr,                 // "Button",     	// text
        currentScheme//ComfortScheme
    ); 

    GbCreate           //System Mode
    (
        COMFORT_SCREEN_ID_SYSTEM_MODE_GROUP_BOX,               // ID
        COMFORT_SCREEN_START - 6,
        GetMaxY()-170,
        COMFORT_SCREEN_START + 238,
		GetMaxY()-98,                                 // dimension
        GB_DRAW | GB_CENTER_ALIGN,  // will be dislayed after creation
        (XCHAR *)SystemModeStr,      // "Selection"
        currentTemperatureScheme//alt4Scheme
    );                              // alternate scheme
    GbCreate           //System Mode
    (
        COMFORT_SCREEN_ID_FAN_MODE_GROUP_BOX,               // ID
        COMFORT_SCREEN_END - 116,
        GetMaxY()-170,
        COMFORT_SCREEN_END + 6,
		GetMaxY()-98,                                 // dimension
        GB_DRAW | GB_CENTER_ALIGN,  // will be dislayed after creation
        (XCHAR *)FanModeStr,          // "Selection"
        currentTemperatureScheme
    );                              // alternate scheme
    
    BtnCreate        //ON
    (
        COMFORT_SCREEN_ID_FAN_ON_BUT,                         // button ID
        COMFORT_SCREEN_END - 115,
         GetMaxY()-140,
        COMFORT_SCREEN_END + 5,
         GetMaxY()-100,                                // dimension
        0,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)ONStr,                 // "Button",     	// text
        currentScheme
    ); 
   
    BtnCreate           //AUTO
    (
        COMFORT_SCREEN_ID_FAN_AUTO_BUT,                         // button ID
        COMFORT_SCREEN_END - 115,
        GetMaxY() - 95,
        COMFORT_SCREEN_END + 5,
        GetMaxY() - 55,                                // dimension
        0,                                 // set radius
        BTN_DRAW|BTN_DISABLED,                           // draw a beveled button
        NULL,                               // no bitmap
        (XCHAR *)AUTOStr,                 // "Button",     	// text
        currentScheme
    ); 

    BtnCreate          //Minus
    (
        COMFORT_SCREEN_ID_COOL_MINUS_BUT,                         // button ID
        COMFORT_SCREEN_END - 235,
         (GetMaxY()>>1) + 1,
        COMFORT_SCREEN_END - 175,
         (GetMaxY()>>1) + 60,                                // dimension
        0,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        (void *) &list_remove,//NULL,                               // no bitmap
        NULL,                            	// text
        currentScheme
    ); 
   
    BtnCreate            //Minus
    (
        COMFORT_SCREEN_ID_HEAT_MINUS_BUT,                         // button ID
        COMFORT_SCREEN_END - 235,
        (GetMaxY()>>1) - 80,
        COMFORT_SCREEN_END - 175,
        (GetMaxY()>>1) - 21,                                // dimension
        0,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        (void *) &list_remove,//NULL,                               // no bitmap
        NULL,//(XCHAR *)MinusStr,                 // "Button",     	// text
        currentScheme
    ); 

    BtnCreate       //Plus
    (
        COMFORT_SCREEN_ID_COOL_PLUS_BUT,                         // button ID
        COMFORT_SCREEN_END - 55,
        (GetMaxY()>>1) + 1,
        COMFORT_SCREEN_END + 5,
        (GetMaxY()>>1) + 60,                                // dimension
        0,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        (void *) &list_add,//NULL,                               // no bitmap
        NULL,//(XCHAR *)PlusStr,                 // "Button",     	// text
        currentScheme
    ); 
   
    BtnCreate           //Plus
    (
        COMFORT_SCREEN_ID_HEAT_PLUS_BUT,                         // button ID
        COMFORT_SCREEN_END - 55,
        (GetMaxY()>>1) - 80,
        COMFORT_SCREEN_END + 5,
        (GetMaxY()>>1) - 21,                                // dimension
        0,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        (void *) &list_add,//NULL,                               // no bitmap
        NULL,//(XCHAR *)PlusStr,                 // "Button",     	// text
        currentScheme
    ); 

}
/*****************************************************************************
 * WORD MsgComfortScreen(WORD objMsg, OBJ_HEADER *pObj)
 *****************************************************************************/
WORD MsgComfortScreen(WORD objMsg, OBJ_HEADER *pObj)
{
    WORD    id;
    static int HeatTemp = 72;
    static int CoolTemp = 78;
    static int CurrentLevel = 0;
    GOL_SCHEME *currentScheme;

    currentScheme = GFX_SchemeGetCurrentScheme();

    MsgPanelScreen(objMsg, pObj);  //Adds UI from PanelScreen

    switch(id = GetObjID(pObj))
    {

         case COMFORT_SCREEN_ID_LEFT_ARROW_BUT:  
         case COMFORT_SCREEN_ID_RIGHT_ARROW_BUT:    
            if(objMsg == BTN_MSG_RELEASED)
            {
               CurrentLevel ^=1;
               SetColor(currentScheme->Color0);   //Background Color
               Bar(COMFORT_SCREEN_START + 135,90,COMFORT_SCREEN_START + 255,120);
               
                          
               while(!AlphaBlendWindow(1, COMFORT_SCREEN_START + 140, 90,
							    GFX_PAGE1, COMFORT_SCREEN_START + 140, 90,
							    1, COMFORT_SCREEN_START + 140, 90,
						        120, 
						        30,   	
						        GFX_SchemeGetDefaultScheme()->AlphaValue)); 
 
               SetColor(RGB565CONVERT(40, 84, 217));
               Bar(COMFORT_SCREEN_START - 5,(GetMaxY()>>1) - 80 , COMFORT_SCREEN_START + 115, (GetMaxY()>>1) + 60);
               SetColor(WHITE); 
               SetFont((void *) &FONTDEFAULT);
               OutTextXY(COMFORT_SCREEN_START + 150,90,(XCHAR *)Level[CurrentLevel]);
               SetFont((void *) &Large);
               OutTextXY(COMFORT_SCREEN_START + 10,190,(XCHAR *)LevelTemp[CurrentLevel]);
               
            }

            return (1);                             // process by default


        case COMFORT_SCREEN_ID_COOL_MINUS_BUT:    //Minus
            if(objMsg == BTN_MSG_RELEASED)
            {
                CoolTemp--;
        
                SetCoolStr[0] = (CoolTemp/10) + 0x30;  //Convert Int to String
                SetCoolStr[1] = (CoolTemp%10) + 0x30;  //Convert Int to String
                SetState(GOLFindObject(COMFORT_SCREEN_ID_COOL_STATIC_TXT),ST_DRAW);
            }

            return (1);                             // process by default

        case COMFORT_SCREEN_ID_HEAT_MINUS_BUT:    //Minus
            if(objMsg == BTN_MSG_RELEASED)
            {
                HeatTemp--;
                SetHeatStr[0] = (HeatTemp/10) + 0x30;  //Convert Int to String
                SetHeatStr[1] = (HeatTemp%10) + 0x30;  //Convert Int to String
                SetState(GOLFindObject(COMFORT_SCREEN_ID_HEAT_STATIC_TXT),ST_DRAW);
            }

            return (1);                             // process by default

       case COMFORT_SCREEN_ID_COOL_PLUS_BUT:    //Plus
            if(objMsg == BTN_MSG_RELEASED)
            {
                CoolTemp++;
                SetCoolStr[0] = (CoolTemp/10) + 0x30;  //Convert Int to String
                SetCoolStr[1] = (CoolTemp%10) + 0x30;  //Convert Int to String
                SetState(GOLFindObject(COMFORT_SCREEN_ID_COOL_STATIC_TXT),ST_DRAW);
            }

            return (1);                             // process by default

        case COMFORT_SCREEN_ID_HEAT_PLUS_BUT:    //Plus
            if(objMsg == BTN_MSG_RELEASED)
            {
                HeatTemp++;
                SetHeatStr[0] = (HeatTemp/10) + 0x30;  //Convert Int to String
                SetHeatStr[1] = (HeatTemp%10) + 0x30;  //Convert Int to String
                SetState(GOLFindObject(COMFORT_SCREEN_ID_HEAT_STATIC_TXT),ST_DRAW);
            }

            return (1);                             // process by default


        case COMFORT_SCREEN_ID_AUTO_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                SetState(pObj,BTN_DISABLED|BTN_DRAW);
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_HEAT_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_HEAT_BUT),BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_OFF_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_OFF_BUT),BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_COOL_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_COOL_BUT),BTN_DRAW);
            }

            return (1);                             // process by default

        case COMFORT_SCREEN_ID_HEAT_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                SetState(pObj,BTN_DISABLED|BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_AUTO_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_AUTO_BUT),BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_OFF_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_OFF_BUT),BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_COOL_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_COOL_BUT),BTN_DRAW);
            }

            return (1);                             // process by default

        case COMFORT_SCREEN_ID_OFF_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                SetState(pObj,BTN_DISABLED|BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_AUTO_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_AUTO_BUT),BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_HEAT_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_HEAT_BUT),BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_COOL_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_COOL_BUT),BTN_DRAW);
            }

            return (1);                             // process by default

        case COMFORT_SCREEN_ID_COOL_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                SetState(pObj,BTN_DISABLED|BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_AUTO_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_AUTO_BUT),BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_HEAT_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_HEAT_BUT),BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_OFF_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_OFF_BUT),BTN_DRAW);
            }

            return (1);                             // process by default

       
        case COMFORT_SCREEN_ID_FAN_ON_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                SetState(pObj,BTN_DISABLED|BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_FAN_AUTO_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_FAN_AUTO_BUT),BTN_DRAW);
            }

            return (1);                             // process by default

        case COMFORT_SCREEN_ID_FAN_AUTO_BUT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                SetState(pObj,BTN_DISABLED|BTN_DRAW);
                
                ClrState(GOLFindObject(COMFORT_SCREEN_ID_FAN_ON_BUT),BTN_DISABLED); 
                SetState(GOLFindObject(COMFORT_SCREEN_ID_FAN_ON_BUT),BTN_DRAW);

            }

            return (1);                             // process by default

        default:
            return (1);                             // process by default
    }
}
