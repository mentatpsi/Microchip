/*********************************************************************
 * Microchip Graphic Library Demo Application
 * This is the header file for the RGB Mood Lighting Demo. The demo 
 * can still be executed even if the HID Device hardware is not 
 * connected. It will just show the GUI interface of the demo.
 *********************************************************************
 * FileName:        RGBDemo.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC24/PIC30/PIC32
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Paolo A. Tamayo		03/10/2008
 ********************************************************************/
#ifndef _RGBDEMO_H
    #define _RGBDEMO_H

////////////////////////////// INCLUDES //////////////////////////////
    #ifdef __PIC32MX
        #include <plib.h>
    #elif defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__PIC24E__)
    	#include <p24Exxxx.h>
    #else
        #include <p24Fxxxx.h>
    #endif
    #include "GenericTypeDefs.h"
    #include "Graphics/Graphics.h"

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT'S IDs
/////////////////////////////////////////////////////////////////////////////
// WARNING! Do not edit the IDs without checking if it affects the operation.
// Edit boxes and sliders IDs are used for optimized code (see ShowCurrentColor()
// function).
    #define ID_UPBTN1       0xF010                          // Up Button 1 ID
    #define ID_UPBTN2       0xF011                          // Up Button 2 ID
    #define ID_UPBTN3       0xF012                          // Up Button 3 ID
    #define ID_DNBTN1       0xF013                          // Down Button 1 ID
    #define ID_DNBTN2       0xF014                          // Down Button 2 ID
    #define ID_DNBTN3       0xF015                          // Down Button 3 ID
    #define ID_HIDDENSTXT   0xF020                          // Static text ID
    #define ID_SLD1         0xF030                          // Slider 1 ID
    #define ID_SLD2         0xF031                          // Slider 2 ID
    #define ID_SLD3         0xF032                          // Slider 3 ID
    #define ID_SLD4         0xF033                          // Slider 4 ID
    #define ID_EB1          0xF040                          // Edit Box 1 ID
    #define ID_EB2          0xF041                          // Edit Box 2 ID
    #define ID_EB3          0xF042                          // Edit Box 3 ID
    #define MAXCHARSIZE     3
    #define DEFAULTTICKDIFF 200
    #define CHANGEDELAY     25
    #define MINTICKDELAY    20

// preset colors delay
    #define PRESETTICKDELAY 2000 //600

// color fading delay
    #define FADECOLORDELAY  40
    #define FADECOLORDELTA  2

    #define REDMAX          255
    #define GREENMAX        255
    #define BLUEMAX         255

    #ifndef TRUE
        #define TRUE    1
        #define FALSE   0
    #endif
    #define DRAWPALCOLOR        0
    #define GETPALCOLOR         1

    #define DRAW_INTENSITY      0xFF30                      // redraw the intensity level
    #define UPDATE_INTENSITY    0xFF31                      // update only the intensity level

// color pointer triangle defines
    #define NEW_COLORPOINTER    0xF001
    #define HIDE_COLORPOINTER   0xF002
    #define SHOW_COLORPOINTER   0xF003
    #define COLORPOINTERDELAY   800

    #define RGBBACKGROUND       BLACK                       //SCREEN_BACKGROUND_COLOR
    #define DIVISION            (PLTWIDTH / 6)
    #define RDELTA              (REDMAX / DIVISION)
    #define GDELTA              (GREENMAX / DIVISION)
    #define BDELTA              (BLUEMAX / DIVISION)

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT DIMENSIONS DEFINES
/////////////////////////////////////////////////////////////////////////////
// Menu buttons
    #define MENUBTNWIDTH    35                      			// width of buttons
    #define MENUBTNHEIGHT   35	                    			// height of buttons

// color palette dimension
    #define PLTWIDTH    (GetMaxX())                  			// palette width
    #define PLTHEIGHT   55                           			// palette height
    #define PLTXPOS     0                            			// palette left/top pos
    #define PLTYPOS     10

	#define BTNWIDTH	((GetMaxX())/9)							// width of buttons
// 	#define BTNHEIGHT	BTNWIDTH							// height of buttons
 	#define BTNHEIGHT	((GetMaxY()-MENUBTNHEIGHT-PLTHEIGHT-42)/3)	// height of buttons
	#define DIVWIDTH	(PLTWIDTH/8)							// factor used to layout controls

	#define SLDWIDTH	(GetMaxX()-((BTNWIDTH*5)+(BTNWIDTH>>1)+26))  // 26 is the total of the margins of each object


// current color bar dimensions
//#define 	CURRBARXPOS		(PLTXPOS)
    #define CURRBARXPOS     (0)
    #define CURRBARYPOS     ((PLTYPOS) + (PLTHEIGHT) + 8)
    #define CURRBARWIDTH    (BTNWIDTH)
    #define CURRBARHEIGHT   ((BTN3DNYPOS + BTNHEIGHT) - (BTN1DNYPOS))

// edit box dimensions
    #define EBHEIGHT    (BTNHEIGHT)                         // edit box height
    #define EBWIDTH     ((BTNWIDTH) + (BTNWIDTH >> 1))      // edit Box width
    #define EBXPOS      (CURRBARXPOS) + (CURRBARWIDTH) + 7  // edit box x position
    #define EB1YPOS     (CURRBARYPOS)                       // edit box1 y position
    #define EB2YPOS     (EB1YPOS) + (EBHEIGHT) + 8          // edit box2 y position
    #define EB3YPOS     (EB2YPOS) + (EBHEIGHT) + 8          // edit box3 y position

// Up buttons dimensions
    #define BTN1DNYPOS  EB1YPOS                             // down button1 y position
    #define BTN2DNYPOS  (BTN1DNYPOS + BTNHEIGHT + 8)        // down button2 y position
    #define BTN3DNYPOS  (BTN2DNYPOS + BTNHEIGHT + 8)        // down button3 y position
    #define BTNDNXPOS   (EBXPOS + EBWIDTH + 7)              // button down x position

// slider/scrollbar dimensions
    #define SLDHEIGHT   BTNHEIGHT //BTNWIDTH                            // slider height
    //#define SLDWIDTH    100                         // slider width
    #define SLD1YPOS    BTN1DNYPOS                  // slider1 y pos
    #define SLD2YPOS    BTN2DNYPOS                  // slider2 y pos
    #define SLD3YPOS    BTN3DNYPOS                  // slider3 y pos
    #define SLDXPOS     (BTNDNXPOS + BTNWIDTH + 3)  // slider x pos

// Down buttons dimensions
    #define BTN1UPYPOS  BTN1DNYPOS                  // up button1 position
    #define BTN2UPYPOS  BTN2DNYPOS                  // up button2 position
    #define BTN3UPYPOS  BTN3DNYPOS                  // up button3 position
    #define BTNUPXPOS   (SLDXPOS + SLDWIDTH + 3)    // down button x position

// intensity slider position
    #define SLD4HEIGHT  (CURRBARHEIGHT)             // slider 4 height
    #define SLD4WIDTH   ((BTNWIDTH - 1))            // slider 4 width
    #define SLD4YPOS    (BTN1DNYPOS)                // slider 4 y pos
    #define SLD4XPOS    (BTNUPXPOS + BTNWIDTH + 8)  // slider 4 x pos

// Exit button dimensions
    #define EXTBTNXPOS      (PLTXPOS)               // exit button x position
    #define EXTBTNYPOS      (GetMaxY() - (MENUBTNHEIGHT))   // exit button y position
    #define EXTBTNWIDTH     (MENUBTNWIDTH << 1)             // exit button width
    #define EXTBTNHEIGHT    (MENUBTNHEIGHT)                 // exit button height

// Preset Buttons
    #define PRE1BTNXPOS     (EXTBTNXPOS + EXTBTNWIDTH + 5)  // preset1 button x position
    #define PRE1BTNYPOS     (GetMaxY() - (BTNHEIGHT))       // preset1 button y position
    #define PRE1BTNWIDTH    (EXTBTNWIDTH)                   // preset1 button width
    #define PRE1BTNHEIGHT   (MENUBTNHEIGHT)                 // preset1 button height
    #define PRE2BTNXPOS     (PRE1BTNXPOS + EXTBTNWIDTH + 5) // preset2 button x position
    #define PRE2BTNYPOS     (PRE1BTNYPOS)                   // preset2 button y position
    #define PRE2BTNWIDTH    (EXTBTNWIDTH)                   // preset2 button width
    #define PRE2BTNHEIGHT   (MENUBTNHEIGHT)                 // preset2 button height
    #define FADEBTNXPOS     (PRE2BTNXPOS + EXTBTNWIDTH + 5) // fade button x position
    #define FADEBTNYPOS     (PRE1BTNYPOS)                   // fade button y position
    #define FADEBTNWIDTH    (EXTBTNWIDTH)                   // fade button width
    #define FADEBTNHEIGHT   (MENUBTNHEIGHT)                 // fade button height

/************************************************************************
 Function: WORD CreateRGBDemo(void)
                                                                       
 Overview: Creates the RGB Mood Lighting demo screen.
  		                                         
 Input: none
                                                                       
 Output: none.
************************************************************************/
void    CreateRGBDemo(void);

/************************************************************************
 Function: WORD RGBDemoMsgCallback(WORD objMsg, OBJ_HEADER* pObj, 
 								   GOL_MSG* pMsg)

 Overview: This function is called by GOLMsg() function when in this 
 		   particular demo each time the valid message is received for the 
  		   objects in the demo.
           
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 

 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD    RGBDemoMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: WORD RGBDemoDrawCallback(void)
 
 Overview: This function is called by GOLDraw() function when the state 
 		   is in this particular demo.

 Input: none

 Output: If the function returns non-zero the draw control will be passed 
		to GOL. GOLDraw() can proceed and re-draw objects that needs 
		to be redrawn.
************************************************************************/
WORD    RGBDemoDrawCallback(void);
#endif
