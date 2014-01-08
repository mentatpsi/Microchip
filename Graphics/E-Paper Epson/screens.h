/*********************************************************************
 *
 *	Screen used in the E-Paper demo
 *
 *********************************************************************
 * FileName:        screens.h
 * Dependencies:    None
 * Processor:       PIC24, dsPIC, PIC32
 * Compiler:        Microchip C30 and C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (c) 2011 Microchip Technology Inc.  All rights 
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
 * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product ("Device") which is 
 *      integrated into Licensee�s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and 
 *      ENC28J60.h ported to a non-Microchip device used in 
 *      conjunction with a Microchip ethernet controller for the 
 *      sole purpose of interfacing with the ethernet controller. 
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT 
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL 
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF 
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS 
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT 
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 ********************************************************************/

#ifndef SCREENS_H
#define SCREENS_H

/**************************************************
 * Screen states:  Used by the GOL draw callback 
 * function.
 *************************************************/
typedef enum
{
    DEMO_NEXT_PRESS = 0x8000,
    DEMO_NEXT_UPDATE = 0x4000,
    DEMO_NEXT_WAIT_RELEASE = 0x2000,

    DEMO_BACK_PRESS = 0x1000,
    DEMO_BACK_UPDATE = 0x0800,
    DEMO_BACK_WAIT_RELEASE = 0x0400,

    DEMO_FONT_SCREEN_CREATE = 0,
    DEMO_FONT_SCREEN_UPDATE,
    DEMO_FONT_SCREEN_WAIT,

    DEMO_FOREIGN_FONT_SCREEN_CREATE,
    DEMO_FOREIGN_FONT_SCREEN_UPDATE,
    DEMO_FOREIGN_FONT_SCREEN_WAIT,

    DEMO_PICTURE_SCREEN_CREATE,
    DEMO_PICTURE_SCREEN_UPDATE,
    DEMO_PICTURE_SCREEN_WAIT,

    DEMO_WIDGETS_SCREEN_CREATE,
    DEMO_WIDGETS_SCREEN_UPDATE,
    DEMO_WIDGETS_SCREEN_WAIT,

    DEMO_CHART_SCREEN_CREATE,
    DEMO_CHART_SCREEN_UPDATE,
    DEMO_CHART_SCREEN_WAIT,
    
    DEMO_QR_SCREEN_CREATE,
    DEMO_QR_SCREEN_UPDATE,
    DEMO_QR_SCREEN_WAIT,

    DEMO_EKG_SCREEN_CREATE,
	DEMO_EKG_SCREEN_UPDATE,
	DEMO_EKG_SCREEN_WAIT,    

	DEMO_LISTBOX_SCREEN_CREATE,
	DEMO_LISTBOX_SCREEN_UPDATE,
	DEMO_LISTBOX_SCREEN_WAIT,
	
	DEMO_PIP_SCREEN_CREATE,
	DEMO_PIP_SCREEN_RUN,
	DEMO_PIP_SCREEN_WAIT,	
	
	DEMO_INTRO_SCREEN_CREATE,
	DEMO_INTRO_SCREEN_UPDATE,
	DEMO_INTRO_SCREEN_WAIT
	
}DEMO_SCREENS;


/**************************************************
 * Function Protoypes
 *************************************************/
void WidgetScreen( WORD create );
void ChartScreen( WORD create );
void CreateIntroductionScreen(void);
void CreateForeignFontScreen(void);
void CreateFontScreen(void);
void CreatePictureScreen(void);
void LoadPicture(void);
void CreatePotScreen(void);
WORD CreatePotPanel(void);
WORD GetPotSamples(WORD number);
void GraphPotentiometer(void);
void CreateEKGScreen(void);
WORD GetEKGSamples(void);
void GraphEKG(void);
void LoadIntroScreen(void);
void CreateMainDemoPage(void);
void LoadFontScreen(void);
void LoadForeignFontScreen(void);
void CreateWidgets( void );
void DeleteWidgets( void );
void CreateChart( void );
void DeleteChart( void );
void ResetEKG(void);
void UpdateListBox( BOOL up, BOOL down );
WORD ListBoxSelected( void );
void DeleteListBox( void );
void LoadQRCode(void);

#endif

