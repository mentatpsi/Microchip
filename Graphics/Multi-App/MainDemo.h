/*********************************************************************
 * Microchip Graphic Library Demo Application
 * The header file joins all header files used in the project.
 *********************************************************************
 * FileName:        MainDemo.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC24/PIC32
 * Compiler:        C30/C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright ? 2007 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED ?AS IS? WITHOUT WARRANTY OF ANY
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok
 * Paolo Tamayo         03/10/08    ...
 ********************************************************************/
#ifndef _MAINDEMO_H
    #define _MAINDEMO_H


/////////////////////////////////////////////////////////////////////////////
//                            ADDITIONAL COLORS USED
/////////////////////////////////////////////////////////////////////////////
    #define GRAY20      RGBConvert(51, 51, 51)
    #define GRAY40      RGBConvert(102, 102, 102)
    #define GRAY80      RGBConvert(204, 204, 204)
    #define GRAY90      RGBConvert(229, 229, 229)
    #define GRAY95      RGBConvert(242, 242, 242)
    #define RED4        RGBConvert(139, 0, 0)
    #define FIREBRICK1  RGBConvert(255, 48, 48)
    #define DARKGREEN   RGBConvert(0, 100, 0)
    #define PALEGREEN   RGBConvert(152, 251, 152)
    #define LIGHTYELLOW RGBConvert(238, 221, 130)
    #define GOLD        RGBConvert(255, 215, 0)
    #define DARKORANGE  RGBConvert(255, 140, 0)


////////////////////////////// INCLUDES //////////////////////////////
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include "Compiler.h"

    #include "GenericTypeDefs.h"
    #include "HardwareProfile.h"
    #include "TimeDelay.h"
    #include "Graphics/Graphics.h"

    #if defined(__C30__) || defined (__XC16__)
        #if defined(__PIC24FJ256GB110__) || defined(__PIC24FJ256DA210__) || defined(__PIC24FJ256GB210__)
            #include "usb_config.h"
            #include "USB/usb.h"
            #include "USB/usb_host_msd.h"
            #include "USB/usb_host_hid_parser.h"
            #include "USB/usb_host_hid.h"
        #endif
        #include "MDD File System/FSIO.h"
    #elif defined(__PIC32MX__)
        #if defined(__32MX460F512L__) || defined(__32MX795F512L__)


            #include "usb_config.h"
            #include "USB/usb.h"
            #include "USB/usb_host_msd.h"
            #include "USB/usb_host_hid_parser.h"
            #include "USB/usb_host_hid.h"
        #endif
        #include "MDD File System/FSIO.h"
    #endif
#if defined(__PIC24FJ256DA210__) 
    #include "Graphics/mchpGfxDrv.h"
#endif
    #include "SST39VF040.h"
    #include "SST25VF016.h"
    #include "EEPROM.h"
    #include "TouchScreen.h"
    #include "Potentiometer.h"
    #include "Beep.h"
#ifdef USE_FOCUS
    #include "SideButtons.h"
#endif
    #include "SlideShowDemo.h"
    #include "rtcc.h"
#if (defined(EXPLORER_16) || defined(PIC24FJ256DA210_DEV_BOARD)) && defined (USE_COMM_PKT_MEDIA_SERIAL_PORT)
    #include "FlashProgrammer.h"
#endif
    #include "cpld.h"
#if defined(EXPLORER_16) || defined(PIC24FJ256DA210_DEV_BOARD)
	#include "ExternalResource.h"
#endif
#if defined(__C30__) || defined(__XC16__)
	#if defined(dsPIC33E_SK)
    	#include "InternalResourceSet2XC16.h"
    #endif
	#include "InternalResourceXC16.h"
#elif defined(__PIC32MX__)
    #if !defined(EXPLORER_16)
    	#include "InternalResourceSet2XC32.h"
    #endif
	#include "InternalResourceXC32.h"
#endif

    #include "ExternalMemory.h"
	
/////////////////////////////////////////////////////////////////////////////
//                             FONTS USED
/////////////////////////////////////////////////////////////////////////////
extern const FONT_FLASH GOLFontDefault; // default GOL font
extern const FONT_FLASH GOLMediumFont;  // medium font
extern const FONT_FLASH GOLSmallFont;   // small font

/////////////////////////////////////////////////////////////////////////////
//                            DEMO STATES
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CREATE_DEMOSELECTION    = 0,
    DISPLAY_DEMOSELECTION,
    CREATE_RGB,
    DISPLAY_RGB,
    CREATE_MULTIFONT,
    DISPLAY_MULTIFONT,
    CREATE_TIMEDATE,
    DISPLAY_TIMEDATE,
    CREATE_SCALE,
    DISPLAY_SCALE,
    CREATE_ECG,
    DISPLAY_ECG,
    BOX_DRAW_ECG,
    CREATE_GAME,
    DISPLAY_GAME,
    CREATE_ANIMATION,
    DISPLAY_ANIMATION,
    CREATE_SNAKE,
    DISPLAY_SNAKE,
    CREATE_SNAKE_SCORE,
    DISPLAY_SNAKE_SCORE,
    CREATE_SLIDESHOW,
    DISPLAY_SLIDESHOW,
        #ifdef ENABLE_USB_MSD_DEMO
    CREATE_JPEGDEMO,
    DISPLAY_JPEGDEMO,
        #endif
        #ifdef ENABLE_SD_MSD_DEMO
    CREATE_SDCARDDEMO,
    DISPLAY_SDCARDDEMO,
        #endif
    CREATE_GRAPHSHOW,
    DISPLAY_GRAPHSHOW,
    DEMO_MODE,
} SCREEN_STATES;

/////////////////////////////////////////////////////////////////////////////
//                       GLOBAL VARIABLES FOR DEMO
/////////////////////////////////////////////////////////////////////////////
extern SCREEN_STATES    screenState;    // current state of main demo state mashine
extern GOL_SCHEME       *altScheme;     // alternative style scheme
extern GOL_SCHEME       *iconScheme;    // alternative style scheme for icons
extern GOL_SCHEME       *meterScheme;   // meter scheme
extern GOL_SCHEME       *navScheme;     // style scheme for the navigation
extern GOL_SCHEME       *gridScheme;    // alternative grid style scheme
extern GOL_SCHEME       *graphScheme;   // style scheme for graph demo

// Control buttons dimension
    #define CTRLBTN_XINDENT         0
    #define CTRLBTN_HEIGHT          35
    #define CTRLBTN_WIDTH           (((GetMaxX() + 1) - (CTRLBTN_XINDENT * 2)) / 4)
    #define CtrlBtnTop()            (GetMaxY() - CTRLBTN_HEIGHT)
    #define CtrlBtnBottom()         GetMaxY()
    #define CtrlBtnLeft(column)     (((column + 1) * CTRLBTN_XINDENT) + (column * CTRLBTN_WIDTH))
    #define CtrlBtnRight(column)    ((column + 1) * (CTRLBTN_XINDENT + CTRLBTN_WIDTH))

// Screen background color
    #define SCREEN_BACKGROUND_COLOR RGBConvert(103, 195, 231)

WORD    ExternalMemoryCallback(IMAGE_EXTERNAL *memory, LONG offset, WORD nCount, void *buffer);

/************************************************************************
 Function: void CreateCtrlButtons(XCHAR *pTextA, XCHAR *pTextB, 
 								  XCHAR *pTextC, XCHAR *pTextD)

 Overview: Creates the navigation and control buttons of the menu.
 
 Input: pTextA - pointer to button A label.
		pTextB - pointer to button B label.
		pTextC - pointer to button C label.
		pTextB - pointer to button D label.

 Output: none
************************************************************************/
void    CreateCtrlButtons(XCHAR *pTextA, XCHAR *pTextB, XCHAR *pTextC, XCHAR *pTextD);

/*********************************************************************
* Function: void ShowError(XCHAR *pStr)
*
* PreCondition: none
*
* Input:  pStr - pointer to the error message
*
* Output: none
*
* Side Effects: none
*
* Overview: This function shows the equivalent blue screen of death.
*
* Note: none
*
********************************************************************/
void ShowError(XCHAR *pStr);

// Macros to enable demos and features
#if !defined (__PIC24FJ128GA010__) 		// disable this mode when using PIC24FJ128GA010 because of limited flash space
    #define ENABLE_DEMO_MODE            // enables the automatic demo mode
#endif    
    #include "ID.h"
    #include "DemoSelection.h"
    #include "RGBDemo.h"
    #include "MultiFontsDemo.h"
    #include "TimeDateDemo.h"
    #include "Animation.h"
    #include "DrawTool.h"
    #include "ECG.h"
    #include "Games.h"
    #include "JPEGDemo.h"
    #include "DemoSteps.h"
    #include "GraphShow.h"
#endif // _MAINDEMO_H
