/*********************************************************************
 * Microchip Graphic Library Demo Application
 * The header file joins all header files used in the project.
 *********************************************************************
 * FileName:        GoogleMapMenu.h
 * Dependencies:    MainDemo.h
 * Processor:       PIC24/PIC32
 * Compiler:        C30/C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright © 2007 Microchip Technology Inc.  All rights reserved.
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 03/10/08    ...
 ********************************************************************/
#ifndef GOOGLEMAPMENU_H
    #define GOOGLEMAPMENU_H
	
	#include "Graphics/Graphics.h"
	#include "MainDemo.h"

/////////////////////////////////////////////////////////////////////////////
//                            MACROS
/////////////////////////////////////////////////////////////////////////////
// zoom buttons
#define ZOOMIN_ID		0x0A04
#define ZOOMOUT_ID		0x0A05

// zoom slider 
#define ZOOMSLIDE_ID	0x0A06

// menu buttons
#define MAP_ID			0x0A07
#define SAT_ID			0x0A08
#define TER_ID			0x0A09
#define ENTER_ID		0x0A0A

// list box for the presaved addresses
#define LIST_ID			0x0A0B

// Ghost Static Text (to detect touch on the map area)
#define MAPAREAST_ID	0x0A0C

// keyboard object
#define KEYBOARD_ID		0x0A0D

// Keyboard Macros
// macros used to toggle between keyboard key set
#define SHOWUPPERKEYS		0xCAFA
#define SHOWLOWERKEYS		0xCAFB
#define SHOWSYMBOLS		 	0xCAFC
// defines the number of keys of the keyboard
#define KEYBOARDWIDTH		11
#define KEYBOARDHEIGHT		4
// defines the size of the buffer to be used
#define TEBUFFERSIZE    	100
// defines the indeces of the special command keys
#define DELCHKEYINDEX   	10				// index number of the enter key
#define SPACEKEYINDEX   	33              // index number of the enter key
#define CLEARKEYINDEX   	43              // index number of the enter key
#define SHIFTMODEKEYINDEX1  32              // index number of the mode key
#define SHIFTMODEKEYINDEX2  22              // index number of the mode key



// nacros for the widget positions in the screen
// dimensions are dependent on the way the items 
// are arranged on the screen

// menus
#define GOOGLEMENUWIDTH		(GetMaxX()/4) //((GetMaxX()-ZOOMSLIDEWIDTH)/4)
#define GOOGLEMENUHEIGHT	30

#define ZOOMIN_X			(GetMaxX()-ZOOMBUTTONWIDTH) 
#define ZOOMIN_Y			0 //(PANDOWN_Y+(PANBUTTONHEIGHT*2)+2)

#define ZOOMBUTTONWIDTH		32
#define ZOOMBUTTONHEIGHT	40

#define ZOOMSLIDEHEIGHT		(GetMaxY()-1-(ZOOMBUTTONHEIGHT<<1)-GOOGLEMENUHEIGHT-3)
#define ZOOMSLIDEWIDTH		(ZOOMBUTTONWIDTH) 

#define ZOOMSLIDE_X			(GetMaxX()-ZOOMBUTTONWIDTH) 
#define ZOOMSLIDE_Y			(ZOOMIN_Y+ZOOMBUTTONHEIGHT+1)

#define ZOOMOUT_X			(GetMaxX()-ZOOMBUTTONWIDTH) 
#define ZOOMOUT_Y			(ZOOMSLIDE_Y+ZOOMSLIDEHEIGHT+1)

#define MAP_X				0 
#define MAP_Y				(GetMaxY()-(GOOGLEMENUHEIGHT)-1)
#define SAT_X				(MAP_X+GOOGLEMENUWIDTH+1)
#define SAT_Y				(MAP_Y)
#define TER_X				(SAT_X+GOOGLEMENUWIDTH+1)
#define TER_Y				(MAP_Y)
#define ENTER_X				(TER_X+GOOGLEMENUWIDTH+1)
#define ENTER_Y				(MAP_Y)
#define MENU_BTN_RADIUS		(GOOGLEMENUHEIGHT>>1)

// Map Area dimensions
#define MAPAREA_X1			10
#define MAPAREA_Y1			10
#define MAPAREA_X2			(GetMaxX()-ZOOMBUTTONWIDTH-10)
#define MAPAREA_Y2			(GetMaxY()-GOOGLEMENUHEIGHT-10)

// Keyboard dimensions
#define KEYBOARD_WIDTH		(GetMaxX()) 
#define KEYBOARD_KEY_WIDTH	(KEYBOARD_WIDTH/11)
#define KEYBOARD_HEIGHT		(GetMaxY()-(GOOGLEMENUHEIGHT)-2) 
#define KEYBOARD_KEY_HEIGHT	(KEYBOARD_KEY_WIDTH)
#define KEYBOARD_X			0		
#define KEYBOARD_Y			0	

// list box dimension
#define ADDLIST_WIDTH		KEYBOARD_WIDTH
#define ADDLIST_HEIGHT		(GetMaxY()-(GOOGLEMENUHEIGHT)-2) 
#define ADDLIST_X			0		
#define ADDLIST_Y			0	

// defines for new user requests status bits
#define NEWADDRESSREQUEST	0x01
#define NEWZOOMREQUEST		0x02
#define NEWMAPTYPEREQUEST	0x04

#define DECODING_MAP_IMAGE_ENABLED  0x01
#define DECODING_MAP_IMAGE_IDLE     0x00

// defines for menu status
#define NORMALMENU			0
#define ALTERNATEMENU		1

// structure defining the current user download request
typedef struct
{
	unsigned char newItemsFlag;	// status bits for each entry in this structure. when a bit is set new request from the user is available
								// bit 0 - for new szCenter, bit 1 - for new vZoom etc...
	unsigned char *szCenter;	// Pointer to a statically allocated string indicating the map center position.  Ex: "2355 W. Chandler Blvd., Chandler, AZ" or "33.304524,-111.882024" where 33.304524 and -111.882024 are longitude and latitude coordinates.
	unsigned char vZoom;		// Zoom level from 0 (whole world in a single map) to 21+ (see if someone is bald or not)
	unsigned char vMapType;		// Map type requested.  Can be either GOOGLE_STATIC_MAP_ROADMAP, GOOGLE_STATIC_MAP_SATELLITE, GOOGLE_STATIC_MAP_HYBRID, or GOOGLE_STATIC_MAP_TERRAIN.
	// &PAT edits
	int 	      reqState;
} USER_INPUT_REQUEST;

// style scheme for Google Map Menu
extern GOL_SCHEME       	*pGoogleMapMenuScheme1, *pGoogleMapMenuScheme2; 

// 
extern USER_INPUT_REQUEST	CurrentDownloadRequest;


/************************************************************************
 Function: WORD CreateGoogleMapScreen(void)
                                                                       
 Overview: Creates the menu of small demos each represented by an icon.
 		                                                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
void    CreateGoogleMapScreen(void);

/************************************************************************
 Function: WORD GoogleMapMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the demo selection. Based on user selection
 		   A demo is enabled by switching the screen state. Icons used
 		   changes from colored version to monochrome version when pressed. 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD    GoogleMapMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

WORD GoogleMapDrawCallback();
void CreateGoogleMapScreen(void);

/************************************************************************
 Function: void InitGoogleMapStyleScheme()
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void    InitGoogleMapStyleScheme();


#endif //GOOGLEMAPMENU_H
