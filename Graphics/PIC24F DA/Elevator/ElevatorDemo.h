/*********************************************************************
 * Microchip Graphic Library Demo Application
 * The header file joins all header files used in the project.
 *********************************************************************
 * FileName:        ElevatorDemo.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC24F
 * Compiler:        C30
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
 * 11/10/2011   ...
 ********************************************************************/
#ifndef _ELEVATORDEMO_H
    #define _ELEVATORDEMO_H

////////////////////////////// INCLUDES //////////////////////////////
    #include "Compiler.h"
    #include "GenericTypeDefs.h"
    #include "TimeDelay.h"
    #include "Graphics/Graphics.h"
    #include "FlashProgrammer.h"
    #include "SST39LF400.h"    
    #include "SST25VF016.h"    
    #include "ExternalMemory.h"
    #include "FlashProgrammer.h"   

    #include "ElevatorResources.h"
    #include "ElevatorDemoStrings.h"
    #include "TouchScreen.h"
    #include "Graphics/mchpGfxDrv.h"
    #include "USB/usb.h"
    #include "USB/usb_function_generic.h"

/////////////////////////////////////////////////////////////////////////////
//                              DEMO MACROS
/////////////////////////////////////////////////////////////////////////////
    
    // images used
    #define UPARROWIMAGE                    arrowUpComposite
    #define DOWNARROWIMAGE                  arrowDownComposite

#if defined (GFX_USE_DISPLAY_PANEL_PH480272T_005_I11Q) 
    #if (DISP_ORIENTATION == 90)   
        #define BACKGROUND_IMAGE            Background272x480
        #define USE_PORTRAIT_MODE
    #elif (DISP_ORIENTATION == 0)   
        #define USE_PARALLEL_FLASH
        #define BACKGROUND_IMAGE            Background480x272
        #define USE_LANDSCAPE_MODE
    #endif
#endif

    #define ELEVATOR_FONT                   DroidSans_Bold_150
    #define ELEVATOR_MSG_FONT               fireflysung_40


    // colors used
    #define TRANSCOLOR                      RGBConvert(0xf5, 0x26, 0xec) 
    #define FLOORTEXTCOLOR                  RGBConvert(0xf2, 0x83, 0x21)

    #define ELEVATOR_DEMO_BACKGROUND_COLOR  RGBConvert(0x26, 0x71, 0xa3) 

    // other macros which pertains to animation and locations in the screen
    #define ANIMATIONDELAY                  500ul 
    #define FLOORANIMATIONDELAY             20000ul 
    #define ANIMATEINCREMENT                (3)
    #define MINFLOOR                        (0)
    #define MAXFLOOR                        (20)
    #define MAXFLOORCHARS                   (2)
    #define DEFAULTFLOORNUMBER              (10)
    #define LINEOFFSET                      DISP_HOR_RESOLUTION 

    // hidden buffer location 
    // NOTE: double buffering feature of driver is NOT used here
    #define HIDDENBUFFER_START_ADDRESS      (GFX_DISPLAY_BUFFER_START_ADDRESS+GFX_DISPLAY_BUFFER_LENGTH)

//    #define DEBUG_DEMO_MODE

    typedef enum
    {
        CAR_STOPPED = 0,
        CAR_GOING_UP,
        CAR_GOING_DOWN,        

    } ELEVATOR_CAR_STATE;

    typedef enum
    {
        CHINESE = 0,
        ENGLISH,

    } APPS_LANGUAGE;

    #define LANGUAGEUSED                    ENGLISH //CHINESE


/*********************************************************************
* Overview: This structure is specifically created for the elevator 
*       animation. The two arrow images (up and down) are pre-rendered 
*       in the hidden buffer. Depending on the direction of the 
*       elevator car, the arrows are chosen and copied into the 
*       displayed buffer. Only a portion of the image is copied 
*       and that portion is updated to give an effect of animation. 
*       The copying of the smaller portion of the chosen image loops 
*       making the effect of animation.
*
*********************************************************************/
    typedef struct 
    {
        void *pImage;
        // arrow animation image location on the screen (defined by the offsets)
        DWORD arrowSourceOffset;        // offset of the source image from the hidden buffer
                                        // this offset is dynamically changing since only a 
                                        // portion of the original image is copied to the 
                                        // destination (or the displayed buffer)
        DWORD downArrowOffset;          // offset if the down arrow from the hidden buffer
        DWORD upArrowOffset;            // offset if the up arrow from the hidden buffer
        DWORD backgroundSourceOffset;   // offset of the copied background from the hidden buffer        
        DWORD arrowDestOffset;          // offset of the animated image in the displayed buffer
                                        // this value will not change since the location of the  
                                        // copied image in the displayed buffer is static
        // defines animation area 
        WORD animationAreaHeight;       // height of the animated area in the display buffer
        WORD animationAreaWidth;        // width of the animated area in the display buffer

        WORD floorLocation;             // current floor number (which floor the elevator car is currently located)
        WORD floorStatusXStart;         // defines the x pixel position of the floor number on the screen 
        WORD floorStatusYStart;         // defines the y pixel position of the floor number on the screen 
        WORD floorStatusWidth;          // defines the width of the area of the screen where the floor number is located
        WORD floorStatusHeight;         // defines the height of the area of the screen where the floor number is located    
        DWORD floorBkGrndSourceOffset;  // offset of the copied background from the hidden buffer 
                                        // for the floor numbers       
        DWORD floorBkGrndDestOffset;    // offset of the background in the destination buffer 
                                        // (main buffer) for the floor numbers       

        ELEVATOR_CAR_STATE carDir;      // state of the elevator car
        WORD animateLineStart;          // animation starting position
        WORD animateLineStop;           // animation end position 
        SHORT incValue;                 // animation increments 
        SHORT animateLineCurrent;       // current line of animation
        BOOL initAnimationArea;         // flag to indicate that animation area needs initialization
        BOOL updateAnimationArea;       // flag to indicate that animation area needs to be updated
        BOOL updateCurrentFloor;        // flag to indicate the floor number needs to be updated
        BOOL updateMessage;             // flag to indicate the message on the screen needs to be updated

        WORD msgXStart;                 // defines the x pixel position of the message area on the screen 
        WORD msgYStart;                 // defines the y pixel position of the message area on the screen 
        WORD msgWidth;                  // defines the width of the message area of the screen 
        WORD msgHeight;                 // defines the height of the message area of the screen 
        DWORD msgBkGrndSourceOffset;    // offset of the copied background from the hidden buffer 
                                        // for the the messages
        DWORD msgBkGrndDestOffset;      // offset of the background in the destination buffer 
                                        // (main buffer) for the messages 
        APPS_LANGUAGE currLang;
    
    } ELEVATOR_ANIMATION;


#endif //_ELEVATORDEMO
