/*********************************************************************
 * Microchip Graphic Library Demo Application
 * The header file joins all header files used in the project.
 *********************************************************************
 * FileName:        PIC24F_DA Demo.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:        C30/C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (c) 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 03/22/11     ...
 ********************************************************************/
#ifndef _PIC24FDADEMO_H
    #define _PIC24FDADEMO_H

////////////////////////////// INCLUDES //////////////////////////////
    #include "Compiler.h"
    #include "GenericTypeDefs.h"
    #include "TimeDelay.h"
    #include "Graphics/Graphics.h"
    #include "Graphics/mchpGfxDrv.h"
    #include "UART.h"
    #include "SST39LF400.h"    
    #include "FlashProgrammer.h"   
    #include "InternalResourceXC16.h"
    #include "ExternalResource.h"    
    #include "TouchScreen.h"
    #include "Potentiometer.h"
    #include "Graphics/gfxepmp.h"
    
    #include "Graphics/mchpGfxDrv.h"

    #include "FishFinderDemo.h"
    #include "Animation.h"

    #include "USB/usb.h"
    #include "USB/usb_function_generic.h"

//    #define ENABLE_DEBUG_DEMO

/////////////////////////////////////////////////////////////////////////////
//                              DEMO MACROS
/////////////////////////////////////////////////////////////////////////////
    #define monofontsmall               VeraMono_17
    #define monofont                    VeraMono_28
 
    #define ID_BUTTON_A                 0x00F0
    #define ID_BUTTON_B                 0x00F1
    #define ID_BUTTON_C                 0x00F2
    #define ID_BUTTON_D                 0x00F3

    #define ID_PICTURE1                 0x00F5
    #define ID_PICTURE2                 0x00F6
    #define ID_PICTURE3                 0x00F7
    #define ID_PICTURE4                 0x00F8

    #define ID_METER1                   0x00F9

    #define TEST_ICON_SIZE              15000ul
    #define INTERNAL_ADDRESS	        0xFA00ul 

    #define CHECK_DELAY                 (100)
    #define ICON_MAX_COUNT              (6)

    #define ICON_TRANSPARENTCOLOR       RGBConvert(0x0D,0x0D,0x0D)  
    #define SCREEN_BACKGROUND_COLOR     (BLACK) 
    
    #define ICON_WIDTH                  (icon1_WIDTH)
    #define ICON_HEIGHT                 (icon1_HEIGHT)
    #define ICON_LARGE_WIDTH            (icon1_large_WIDTH)
    #define ICON_LARGE_HEIGHT           (icon1_large_HEIGHT)
    
    #define DRAW_NOT_SELECTABLE         (0)
    #define DRAW_SELECTABLE             (1)

    #define SLIDE_DELTA                 (1)                         // NOTE: this does not work when value is not 1
    #define ICON_ADDRESS_DELTA          ((DWORD)((GetMaxY() + 1)*(ICON_WIDTH + ICON_SPACING)))

    #define TOUCH_SLIDE_THRESHOLD       (5) // used to initially detect a drag
    
    #define CENTER_IN_Y                 (GetMaxY() >> 1)    
    #define CENTER_IN_X                 (GetMaxX() >> 1)    

    #define ICON_VISIBLE_COUNT          (3)
    #define ICON_SPACING                ((GetMaxX() - (ICON_WIDTH * ICON_VISIBLE_COUNT))/ICON_VISIBLE_COUNT)
    
    #define SWIPE_MAGNITUDE_DELTA       (20)

    // hardware buttons
	#define GetHWButtonSelect()    	    GetHWButtonCenter()

    // Control buttons dimension
    #define CTRLBTN_XINDENT         0
    #define CTRLBTN_HEIGHT          35
    #define CTRLBTN_WIDTH           (((GetMaxX() + 1) - (CTRLBTN_XINDENT * 2)) / 4)
    #define CtrlBtnTop()            (GetMaxY() - CTRLBTN_HEIGHT)
    #define CtrlBtnBottom()         GetMaxY()
    #define CtrlBtnLeft(column)     (((column + 1) * CTRLBTN_XINDENT) + (column * CTRLBTN_WIDTH))
    #define CtrlBtnRight(column)    ((column + 1) * (CTRLBTN_XINDENT + CTRLBTN_WIDTH))


    typedef enum
    {
        GEST_IDLE = 0xCA00,
        GEST_PRESS,
        GEST_RELEASE,
        GEST_MOVE,
        GEST_SWIPE,
        
    } GESTURE_TYPES;   

    typedef enum
    {
        SLIDE_TO_NOWHERE = 0xCB00,
        SLIDE_TO_LEFT,
        SLIDE_TO_RIGHT,
        
    } SLIDE_DIR;

    typedef enum
    {
        SWIPE_MAG_NONE = 0xCC00,        // x change < (SWIPE_MAGNITUDE_DELTA)
        SWIPE_MAG_LOW,                  // (SWIPE_MAGNITUDE_DELTA)   > x change <= (SWIPE_MAGNITUDE_DELTA*2)
        SWIPE_MAG_MEDIUM,               // (SWIPE_MAGNITUDE_DELTA*2) > x change <= (SWIPE_MAGNITUDE_DELTA*3)
        SWIPE_MAG_LARGE,                // (SWIPE_MAGNITUDE_DELTA*3) > x change 
        
    } SWIPE_MAG;

    /////////////////////////////////////////////////////////////////////////////
    //                            DEMO STATES
    /////////////////////////////////////////////////////////////////////////////
    typedef enum
    {
        CREATE_MAINMENU = 0,
        DISPLAY_MAINMENU,
        CREATE_FISHFINDER,
        DISPLAY_FISHFINDER,
        CREATE_ANIMATION,
        DISPLAY_ANIMATION,
    } SCREEN_STATES;

/////////////////////////////////////////////////////////////////////////////
//                              DATA TYPES
/////////////////////////////////////////////////////////////////////////////

    typedef struct
    {
        WORD        IconCount;                      // number of icons on the screen
        void        *pIcons[ICON_MAX_COUNT];
        void        *pIconsLarge[ICON_MAX_COUNT];
        SHORT       IconLeft[ICON_MAX_COUNT];
        SHORT       IconTop[ICON_MAX_COUNT];
        SHORT       IconRight[ICON_MAX_COUNT];
        SHORT       IconBottom[ICON_MAX_COUNT];
        SHORT       IconSelected;
        WORD        visibleStart;                   //
        WORD        visibleEnd;                     //
        WORD        currentStart;                   //
        WORD        currentEnd;                     //
    } ICON_MANAGER;

    typedef struct
    {
        GESTURE_TYPES   type;                   // defines the type of gestures
        SWIPE_MAG       swipeMagnitude;         // determines if the swipe is long and fast
        SLIDE_DIR       slideDir;               // determines if the swipe is left to right or vice versa
        WORD            currPos;                // used as a variable to track the position of the touch (in the x direction)
        WORD            prevPos;                // used as a variable to track previous position of the touch (in the x direction)
        WORD            touchMoveThreshold;
        SHORT           delta;                  // used track how many pixels we need to move the display buffer
    } USER_GESTURE;

// current state of main demo state mashine
extern SCREEN_STATES   screenState;               
extern GOL_SCHEME      *altScheme;              // alternative style scheme
extern GOL_SCHEME      *menuScheme;             // menu style scheme
extern GOL_SCHEME      *meterScheme;            // meter style scheme

void CreateCtrlButtons(XCHAR *pTextA, XCHAR *pTextB, XCHAR *pTextC, XCHAR *pTextD);

#endif //_PIC24FDADEMO_H
