/*********************************************************************
 * Microchip Graphic Library Demo Application
 * The header file joins all header files used in the project.
 *********************************************************************
 * FileName:        MainDemo.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:        C30/C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
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
 * Date                 Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 12/05/2011           Initial version
 ********************************************************************/
#ifndef _MAINDEMO_H
    #define _MAINDEMO_H

////////////////////////////// INCLUDES //////////////////////////////
    #include "Compiler.h"
    #include "GenericTypeDefs.h"
    #include "Graphics/Graphics.h"
    #include "drv_spi.h"
    #include "SST25VF016.h"
    #include "TouchScreen.h"
    #include "Resources.h"
    #include "TimeDelay.h"
    #include "HardwareProfile.h"
    #include "Graphics/mchpGfxDrv.h"

    #define BLACK_SCREEN                (1)

    #define SetBlackScreen()            (_DPTEST = BLACK_SCREEN)
    #define SetNormalScreen()           (_DPTEST = 0)              

    #define SCR1_BTNID                  0xF1 
    #define SCR2_BTNID                  0xF2 
    #define SCR1_PICTID                 0xB1
    #define SCR2_PICTID                 0xB2

    #define SCR1_PICT_WIDTH             200
    #define SCR1_PICT_HEIGHT            150

    #if (COLOR_DEPTH == 1)

        #define SCR1_IMAGE              ((void*)&mchp)
        #define SCR1_BTN_IMAGE          ((void*)&HomeMono1BPP)
        #define SCR1_BTN_IMAGE_PRESSED  ((void*)&HomePressedMono1BPP)

        #define SCR2_IMAGE              ((void*)&mchpGraphics)
        #define SCR2_BTN_IMAGE          ((void*)&SettingsMono1BPP)
        #define SCR2_BTN_IMAGE_PRESSED  ((void*)&SettingsPressedMono1BPP)
#ifdef USE_COMP_IPU
        #define LABEL_TEXT                ("1 BPP - Compressed")
#else
        #define LABEL_TEXT                ("1 BPP")
#endif
        #define SCR1_PALETTE            (Screen_1BPP_Palette)
        #define SCR2_PALETTE            (Screen_1BPP_Palette)

    #elif (COLOR_DEPTH == 4)    

        #define SCR1_IMAGE              ((void*)&Scene02Mono_4BPP)
        #define SCR1_BTN_IMAGE          ((void*)&HomeMono4BPP)
        #define SCR1_BTN_IMAGE_PRESSED  ((void*)&HomePressedMono4BPP)

        #define SCR2_IMAGE              ((void*)&Scene05Mono_4BPP)
        #define SCR2_BTN_IMAGE          ((void*)&SettingsMono4BPP)
        #define SCR2_BTN_IMAGE_PRESSED  ((void*)&SettingsPressedMono4BPP)

#ifdef USE_COMP_IPU
        #define LABEL_TEXT              ("4 BPP - Compressed")
#else
        #define LABEL_TEXT              ("4 BPP")
#endif

        #define SCR1_PALETTE            (Screen1_4BPP_Palette)
        #define SCR2_PALETTE            (Screen2_4BPP_Palette)

    #elif (COLOR_DEPTH == 8)    
    
        #define SCR1_IMAGE              ((void*)&Scene02_8BPP)
        #define SCR1_BTN_IMAGE          ((void*)&Home8BPP)
        #define SCR1_BTN_IMAGE_PRESSED  ((void*)&HomePressed8BPP)

        #define SCR2_IMAGE              ((void*)&Scene05_8BPP)
        #define SCR2_BTN_IMAGE          ((void*)&Settings8BPP)
        #define SCR2_BTN_IMAGE_PRESSED  ((void*)&SettingsPressed8BPP)

#ifdef USE_COMP_IPU
        #define LABEL_TEXT              ("8 BPP - Compressed")
#else
        #define LABEL_TEXT              ("8 BPP")
#endif

        #define SCR1_PALETTE            (Screen1_8BPP_Palette)
        #define SCR2_PALETTE            (Screen2_8BPP_Palette)

    #endif

    #define TEXT_COLOR                  WHITE

    #define TEXT_WIDTH                  (GetTextWidth(LABEL_TEXT, (void *) &FONTDEFAULT)) 
    #define TEXT_HEIGHT                 (GetTextHeight((void *) &FONTDEFAULT))

    #define BTN_WIDTH                   80
    #define BTN_HEIGHT                  79

    #define PICT_WIDTH                  187
    #define PICT_HEIGHT                 140

    #define VERTICAL_MARGIN             ((GetMaxY() - PICT_HEIGHT - BTN_HEIGHT - TEXT_HEIGHT) >> 2) 

    #define TEXT_POSX                   ((GetMaxX() - TEXT_WIDTH) >> 1)
    #define TEXT_POSY                   (VERTICAL_MARGIN)

    #define SCR1_PICTX                  ((GetMaxX() - PICT_WIDTH) >> 1)
    #define SCR1_PICTY                  (TEXT_POSY + TEXT_HEIGHT + VERTICAL_MARGIN)

    #define SCR2_PICTX                  ((GetMaxX() - PICT_WIDTH) >> 1)
    #define SCR2_PICTY                  (TEXT_POSY + TEXT_HEIGHT + VERTICAL_MARGIN)

    #define SCR1_BTNX                   ((GetMaxX() - BTN_WIDTH) >> 1)
    #define SCR1_BTNY                   (SCR1_PICTY + PICT_HEIGHT + VERTICAL_MARGIN)

    #define SCR2_BTNX                   ((GetMaxX() - BTN_WIDTH) >> 1)
    #define SCR2_BTNY                   (SCR2_PICTY + PICT_HEIGHT + VERTICAL_MARGIN)


#endif //#ifndef _MAINDEMO_H
