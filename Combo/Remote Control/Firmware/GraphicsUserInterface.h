/*********************************************************************
 * Microchip Graphic Library Demo Application
 * The header file joins all header files used in the project.
 *********************************************************************
 * FileName:        GraphicsUserInterface.h
 * Dependencies:    
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
 * 01/09/12    - Renamed resources files to ExternalResource.c (h) 
 *             - Renamed RemoteGUIFonts.c (h) to InternalResourceC30.c (h)
 *               & added in the palette resource.
 ********************************************************************/
#ifndef GRAPHICSUSERINTERFACE_H
    #define GRAPHICSUSERINTERFACE_H
	
    #include "Compiler.h"
    #include "GenericTypeDefs.h"
	#include "Graphics/Graphics.h"
	#include "ExternalResource.h"
	#include "InternalResourceXC16.h"
	#include "rtcc.h"
    #include "ExternalMemory.h"
    #include "TimeDelay.h"
    #include "NVM.h"
	#include "RF4CEController.h"
    #include "WirelessProtocols/SymbolTime.h"
    #include "battery.h"
    #include "sleep.h"

    // firmware version
    #define REMOTE_FIRMWARE_VERSION    1.00
	#define HIDE_PAINTING_EFFECT

    //#define ENABLE_PAIR_ENTRY_DELETION

/////////////////////////////////////////////////////////////////////////////
//                            MACROS
/////////////////////////////////////////////////////////////////////////////
	// macroses for home screen icon positions
	// when increasing the number of demos, increase the rows and columns too
    #define ICON_COLUMNSNUM         2	
    #define ICON_ROWSNUM            3
    #define ICON_WIDTH              80 
    #define ICON_HEIGHT             79 
    #define ICON_BITMAP_WIDTH       80
    #define ICON_BITMAP_HEIGHT      79 
    
    #define TOP_MARGIN			    38
    
    #define ICON_XINDENT            ((GetMaxX() - ICON_COLUMNSNUM * ICON_WIDTH) / (ICON_COLUMNSNUM + 1))
    #define ICON_YINDENT            ((GetMaxY() - TOP_MARGIN - (ICON_ROWSNUM * ICON_HEIGHT)) / (ICON_ROWSNUM + 1))
    #define IconTop(row)            ((((row + 1) * ICON_YINDENT) + (row * ICON_HEIGHT)) + TOP_MARGIN) 
    #define IconBottom(row)         (((row + 1) * (ICON_YINDENT + ICON_HEIGHT))  + TOP_MARGIN) 
    #define IconLeft(column)        (((column + 1) * ICON_XINDENT) + (column * ICON_WIDTH))
    #define IconRight(column)       ((column + 1) * (ICON_XINDENT + ICON_WIDTH))

    // Microchip logo location
    #define MCHP_LOGO_STARTX        10
    #define MCHP_LOGO_STARTY        3

    #define MCHP_LOGO_WIDTH         35
    #define MCHP_LOGO_HEIGHT        33


	// Date and time start locations
	#define DATE_TIME_STARTY	    10

	// Battery dimensions
	#define BAT_STARTX			    (GetMaxX() - 30)
	#define BAT_STARTY			    (DATE_TIME_STARTY + 3)
	#define BAT_WIDTH			    21
	#define BAT_HEIGHT			    10

    /* ******************************************************************* */
    /*          DEFINES FOR WATCH TV CHANNEL SELECTION SCREEN              */
    /* ******************************************************************* */
    #define KP_COLUMNSNUM     	    3	
    #define KP_ROWSNUM        	    4
    #define KP_WIDTH          	    ((GetMaxX()+ 1)/3) //40 
    #define KP_HEIGHT         	    (((GetMaxY() + 1)-TOP_MARGIN)/4) //47 

    #define KP_XINDENT        	    (((GetMaxX() + 1) - KP_COLUMNSNUM * KP_WIDTH) / (KP_COLUMNSNUM + 1))
    #define KP_YINDENT        	    (((GetMaxY() + 1) - TOP_MARGIN - (KP_ROWSNUM * KP_HEIGHT)) / (KP_ROWSNUM + 1))
    #define KPTop(row)              ((((row + 1) * KP_YINDENT) + (row * KP_HEIGHT)) + TOP_MARGIN) 
    #define KPBottom(row)           (((row + 1) * (KP_YINDENT + KP_HEIGHT))  + TOP_MARGIN) 
    #define KPLeft(column)          (((column + 1) * KP_XINDENT) + (column * KP_WIDTH))
    #define KPRight(column)         ((column + 1) * (KP_XINDENT + KP_WIDTH))

    /* ******************************************************************* */
    /*                   DEFINES FOR SETTINGS SCREEN                       */
    /* ******************************************************************* */

    #define MACADDR_ORIGIN_X        (0)
    #define MACADDR_ORIGIN_Y        (TOP_MARGIN + 5)
    #define MACADDR_WIDTH           (GetMaxX() + 1) 
    #define MACADDR_HEIGHT          (36)
    
    #define PAIR_LIST_ORIGIN_X      (0)
    #define PAIR_LIST_ORIGIN_Y      (MACADDR_ORIGIN_Y + MACADDR_HEIGHT + 1)
    #define PAIR_LIST_WIDTH         (GetMaxX() + 1) 
    #define PAIR_LIST_HEIGHT        ((GetMaxY() + 1 - (KPBottom(3) - KPTop(3)) - PAIR_LIST_ORIGIN_Y) - 1)

    #define SET_COLUMNSNUM     	    3	
    #define SET_ROWSNUM        	    3
    #define SET_XSTART              25 
    #define SET_YSTART         	    25 
    #define SET_WIDTH          	    ((GetMaxX() - (SET_XSTART<<1) + 1)/SET_COLUMNSNUM)             
    #define SET_HEIGHT         	    (((GetMaxY() - (SET_YSTART<<1) + 1) - TOP_MARGIN - (KPBottom(3) - KPTop(3)))/SET_ROWSNUM)  

    #define SET_XINDENT        	    (((GetMaxX() - (SET_XSTART<<1)) - SET_COLUMNSNUM * SET_WIDTH) / (SET_COLUMNSNUM + 1))
    #define SET_YINDENT        	    (((GetMaxY() - (SET_XSTART<<1))- TOP_MARGIN - (KPBottom(3) - KPTop(3)) - (SET_ROWSNUM * SET_HEIGHT)) / (SET_ROWSNUM + 1))

    #define SETTop(row)             ((SET_YSTART) + ((((row + 1) * SET_YINDENT) + (row * SET_HEIGHT)) + TOP_MARGIN)) 
    #define SETBottom(row)          ((SET_YSTART) + (((row + 1) * (SET_YINDENT + SET_HEIGHT))  + TOP_MARGIN)) 
    #define SETLeft(column)         ((SET_XSTART) + (((column + 1) * SET_XINDENT) + (column * SET_WIDTH)))
    #define SETRight(column)        ((SET_XSTART) + ((column + 1) * (SET_XINDENT + SET_WIDTH)))

    #define PAIR_START_X            (GetMaxX()>>4)
    #define CLEAR_START_X           ((GetMaxX()>>4) + (GetMaxX()>>1)) 
    #define PAIRCLEAR_START_Y       (PAIR_LIST_ORIGIN_Y + PAIR_LIST_HEIGHT + 10)

    #define PAIRCLEAR_WIDTH         ((GetMaxX()>>1) - ((GetMaxX()>>4)<<1))  
    #define PAIRCLEAR_HEIGHT        ((GetMaxY()>>2) - 20) 

    /* ******************************************************************* */
    /*                     DEFINES FOR PLAY DISC SCREEN                    */
    /* ******************************************************************* */
    #define PD_COLUMNSNUM     	    3	
    #define PD_ROWSNUM        	    4
    #define PD_WIDTH          	    55 
    #define PD_HEIGHT         	    55 

    #define PD_XINDENT        	    ((GetMaxX() - PD_COLUMNSNUM * PD_WIDTH) / (PD_COLUMNSNUM + 1))
    #define PD_YINDENT        	    ((GetMaxY() - TOP_MARGIN - (PD_ROWSNUM * PD_HEIGHT)) / (PD_ROWSNUM + 1))
    #define PDTop(row)              ((((row + 1) * PD_YINDENT) + (row * PD_HEIGHT)) + TOP_MARGIN) 
    #define PDBottom(row)           (((row + 1) * (PD_YINDENT + PD_HEIGHT))  + TOP_MARGIN) 
    #define PDLeft(column)          (((column + 1) * PD_XINDENT) + (column * PD_WIDTH))
    #define PDRight(column)         ((column + 1) * (PD_XINDENT + PD_WIDTH))

    /* ******************************************************************* */
    /*                     DEFINES FOR FAVORITES SCREEN                    */
    /* ******************************************************************* */
    #define FAV_COLUMNSNUM     	    1	
    #define FAV_ROWSNUM        	    5
    #define FAV_WIDTH          	    (GetMaxX() + 1) 
    #define FAV_HEIGHT         	    (((GetMaxY() - (KPBottom(3) - KPTop(3)) - TOP_MARGIN) - 1)/FAV_ROWSNUM)

    #define FAV_XINDENT        	    ((GetMaxX() - FAV_COLUMNSNUM * FAV_WIDTH) / (FAV_COLUMNSNUM + 1))
    #define FAV_YINDENT        	    ((GetMaxY() - (KPBottom(3) - KPTop(3)) - TOP_MARGIN - (FAV_ROWSNUM * FAV_HEIGHT)) / (FAV_ROWSNUM + 1))
    #define FAVTop(row)             ((((row + 1) * FAV_YINDENT) + (row * FAV_HEIGHT)) + TOP_MARGIN) 
    #define FAVBottom(row)          (((row + 1) * (FAV_YINDENT + FAV_HEIGHT))  + TOP_MARGIN) 
    #define FAVLeft(column)         (((column + 1) * FAV_XINDENT) + (column * FAV_WIDTH))
    #define FAVRight(column)        ((column + 1) * (FAV_XINDENT + FAV_WIDTH))

    /* ******************************************************************* */
    /*                     DEFINES FOR INTERNET SCREEN                      */
    /* ******************************************************************* */
	#define LEFT_MARGIN			    10
	#define GLOBE_WIDTH			    ICON_BITMAP_WIDTH
	#define INTER_MODELeft		    LEFT_MARGIN + 5 + GLOBE_WIDTH
	#define INTER_MODETop		    TOP_MARGIN
	#define INTER_MODERight		    GetMaxX() - 10
	#define INTER_MODEBottom	    TOP_MARGIN + GLOBE_WIDTH

	#define INTER_INSTRLeft		    LEFT_MARGIN + 5
	#define INTER_INSTRTop		    INTER_MODEBottom + 10
	#define INTER_INSTRRight	    GetMaxX() - 10
	#define INTER_INSTRBottom	    GetMaxY() - 60 

	// Note: the reminaing 3 icons (Up arrow, down arrow and the home icon) are placed using the Settings Screen

	/* ***************************************** */
	// Main Home Screen
	/* ***************************************** */
	// IDs
    #define ID_MCHPICON             0x10
    
	#define ID_MAINBACKGROUND       0x50
	#define ID_BATLEVEL			    0x60
	#define ID_TVICON               0xC0
    #define ID_FAVICON              0xC1
    #define ID_INTICON              0xC2
    #define ID_PDICON               0xC3
    #define ID_SETICON              0xC4
    #define ID_UNIVICON             0xC5
    #define ID_REMOTEMODE		    0xC6
    
    #define ID_DATE				    0xC7
    #define ID_TIME				    0xC8
    
	// Icons
 	#define TVICON				    TV 
 	#define TVICONPRESS			    TVPressed
 	#define FAVICON 			    Favorites 
 	#define FAVICONPRESS		    FavoritesPressed
	#define INTERICON		 	    Internet
	#define INTERICONPRESS 		    InternetPressed
 	#define PDICON				    Play_Disc
 	#define PDICONPRESS 		    Play_DiscPressed
 	#define SETICON 			    Settings 
 	#define SETICONPRESS 		    SettingsPressed
    
    // Microchip logo
    #define MICROCHIP_LOGO          MICH4CBsmall

    // RF4CE presence Icon 
    #define RF4CE_ICON              RF4CEIcon

	/* ***************************************** */
	// Watch TV Screen
	/* ***************************************** */
	// IDs
    #define ID_KP_0                 0x0
    #define ID_KP_1                 0x1
    #define ID_KP_2                 0x2
    #define ID_KP_3                 0x3
    #define ID_KP_4                 0x4
    #define ID_KP_5                 0x5
    #define ID_KP_6                 0x6
    #define ID_KP_7                 0x7
    #define ID_KP_8                 0x8
    #define ID_KP_9                 0x9
    #define ID_KP_FAV               0xA
    #define ID_HOME          	    0xB

	// Icons
 	#define KPICON_FAV			    Smaller_Favorites
 	#define KPICON_PRESS_FAV	    Smaller_FavoritesPressed
 	#define ICON_HOME			    HomeIconSmall
 	#define ICON_PRESS_HOME		    HomeIconSmallPressed

	/* ***************************************** */
	// Settings Screen
	/* ***************************************** */
	// IDs
	#define ID_RF_SETTINGS		    0x1AC
	#define ID_TIME_SETTINGS        0x1AD
	#define ID_SET_TIME_VALUES      0x1AE

	#define ID_ST_MONTH			    0x1AF
	#define ID_ST_DAY			    0x1B0
	#define ID_ST_YEAR			    0x1B1
	#define ID_ST_HOUR			    0x1B2
	#define ID_ST_MINUTE		    0x1B3
	#define ID_ST_SECOND		    0x1B4

    #define ID_ERASE_PAIR_TABLE     0x1B5
    #define ID_PAIR_DEVICE          0x1B6
        
    #define ID_UP_BTN               0x200
    #define ID_DOWN_BTN             0x201
    #define ID_SELECT_TIME_BTN      0x202

	// Icons
 	#define SETICON_UP				Home 
 	#define SETICON_UP_PRESS		Home 
 	#define SETICON_DOWN			Home 
 	#define SETICON_DOWN_PRESS		Home 
 	#define SETICON_VALUE			Home 
 	#define SETICON_VALUE_PRESS		Home 
 	#define SETICON_PAIRRF			Home 
 	#define SETICON_PAIRRF_PRESS 	Home 


	/* ***************************************** */
	// Play Disc Screen
	/* ***************************************** */
	// IDs
	#define ID_FAST_RW			    0x90	
	#define ID_FAST_FW			    0x91	
	#define ID_SLOW_RW			    0x92	
	#define ID_SLOW_FW			    0x93	
	#define ID_PLAY_PAUSE		    0x94	
	#define ID_STOP			        0x95	
	#define ID_PLAY_INC		    	0x96	
	#define ID_SETUP_PLAYER		    0x97	
	#define ID_DISC_MENU		    0x98	

    #define ID_PAIR_LIST            0x0100
    #define ID_ST_MACADDR           0x0101

	// Icons
	#define PDICON_FAST_RW			button1
	#define PDICON_FAST_RW_PRESS	button1press
	#define PDICON_FAST_FW			button2
	#define PDICON_FAST_FW_PRESS	button2press
	#define PDICON_SLOW_RW			button3
	#define PDICON_SLOW_RW_PRESS	button3press
	#define PDICON_PLAY_PAUSE		button4
	#define PDICON_PLAY_PAUSE_PRESS	button4press
	#define PDICON_SLOW_FW			button5
	#define PDICON_SLOW_FW_PRESS	button5press
	#define PDICON_STOP				button6
	#define PDICON_STOP_PRESS		button6press
	#define PDICON_PLAY_INC			button7
	#define PDICON_PLAY_INC_PRESS	button7press

	/* ***************************************** */
	// Favorites Screen
	/* ***************************************** */
	// IDs
	#define ID_FAV_1			    0x81	
	#define ID_FAV_2			    0x82	
	#define ID_FAV_3			    0x83	
	#define ID_FAV_4			    0x84	
	#define ID_FAV_5			    0x85	
	#define ID_FAV_6			    0x86	
    #define ID_FAV_UP			    0x87	
	#define ID_FAV_DOWN			    0x88	

	// Icons
	#define FAVICON_ITEM		    favoritesplain
	#define FAVICON_ITEM_PRESS	    favoritesgreen
 	#define FAVICON_UP			    favuparrow
 	#define FAVICON_UP_PRESS	    favuparrowpress
 	#define FAVICON_DOWN		    favdownarrow
 	#define FAVICON_DOWN_PRESS	    favdownarrowpress

	/* ***************************************** */
	// Internet Screen
	/* ***************************************** */
	// IDs
	#define ID_INT_MODE			    0x70	
	#define ID_INT_INTR             0x71	

	// Icons
	#define INTICON_GLOBE		    Internet
	
	// common used colors
	#define BATTERY_LEVEL_COLOR     251	// this is BRIGHTGREEN
	#define REMOTE_WHITE	        249	// this is white
	#define REMOTE_BLACK	        0	// this is black
	#define REMOTE_GRAY	            254	// this is common gray color

	#define REMOTE_YELLOW           252	// this is used in setting time and date
	#define REMOTE_GRAY64           69	// this is used in setting time and date

    #define TEXTCOLOR_UNPRESSED     253

	#define TRANSPARENT_COLOR	    0	// this is the orange transparent color
	#define BACKGROUND_COLOR	    0   // this is the pure color for the background

    #define KEYPAD_BACKGROUND       REMOTE_GRAY 
    #define KEYPAD_LIGHTEMBOSS      22
    #define KEYPAD_DARKEMBOSS       REMOTE_BLACK
    #define KEYPAD_TEXTCOLOR0       TEXTCOLOR_UNPRESSED
    #define KEYPAD_TEXTCOLOR1       REMOTE_WHITE 
	
	/* ***************************************** */
	// Program the Flash Screen or Erase Pair Table
	/* ***************************************** */
	#define ID_PGM_YES			    0x0FF0	
	#define ID_PGM_NO               0x0FF1	
	#define ID_ERASE_TABLE		    0x0FF2	
	#define ID_ERASE_ENTRY		    0x0FF3	
	#define ID_ERASE_CANCEL         0x0FF4	

    #define ERASERF_WIDTH            (GetMaxX()/3) 
    #define ERASERF_HEIGHT           (GetMaxY()>>2) 

    #define ERASE_LIST_START_X      (0)
    #define ERASE_LIST_START_Y      (ERASERF_HEIGHT>>2)

#ifdef ENABLE_PAIR_ENTRY_DELETION
    #define ERASE_ENTRY_START_X     (ERASERF_WIDTH/3)
    #define ERASE_ENTRY_START_Y     (ERASE_LIST_START_Y + ERASERF_HEIGHT + (ERASERF_HEIGHT>>2)) 

    #define ERASE_TABLE_START_X     (ERASE_ENTRY_START_X + ERASERF_WIDTH + (ERASERF_WIDTH/3))
    #define ERASE_TABLE_START_Y     (ERASE_LIST_START_Y + ERASERF_HEIGHT + (ERASERF_HEIGHT>>2)) 

    #define ERASE_CANCEL_START_X    ((GetMaxX()>>1)-(ERASERF_WIDTH>>1))
    #define ERASE_CANCEL_START_Y    (ERASE_ENTRY_START_Y + ERASERF_HEIGHT + (ERASERF_HEIGHT>>2))
#else
    #define ERASE_TABLE_START_X     (ERASERF_WIDTH/3)
    #define ERASE_TABLE_START_Y     (ERASE_LIST_START_Y + ERASERF_HEIGHT + (ERASERF_HEIGHT>>2)) 

    #define ERASE_CANCEL_START_X    (ERASE_TABLE_START_X + ERASERF_WIDTH + (ERASERF_WIDTH/3))
    #define ERASE_CANCEL_START_Y    (ERASE_LIST_START_Y + ERASERF_HEIGHT + (ERASERF_HEIGHT>>2)) 
#endif

    #define PROCAN_WIDTH            ((GetMaxX()>>1) - ((GetMaxX()>>4)<<1))  
    #define PROCAN_HEIGHT           (GetMaxY()>>2) 

    #define PROCEED_START_X         (GetMaxX()>>4)
    #define PROCEED_START_Y         (GetMaxY()>>1)

    #define CANCEL_START_X          ((GetMaxX()>>4) + (GetMaxX()>>1)) 
    #define CANCEL_START_Y          (GetMaxY()>>1)

/////////////////////////////////////////////////////////////////////////////
//            FONTS (the default font is also set in GraphicsConfig.h)
/////////////////////////////////////////////////////////////////////////////
    #define SmallFonts              DroidSansMono_11 
    #define REMOTEFONT_14		    DroidSansMono_11 
	#define TIMEDATEFONT		    DroidSans_Bold_14
	#define TVREMOTEFONT            DroidSansMono_36 
	#define FAVMENUFONT             DroidSans_18 

/////////////////////////////////////////////////////////////////////////////
//                            DEMO STATES
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CREATE_HOME          = 0xCA00,
    DISPLAY_HOME,
    CREATE_WATCHTV,
    DISPLAY_WATCHTV,
    CREATE_FAVORITES,
    DISPLAY_FAVORITES,
    CREATE_INTERNET,
    DISPLAY_INTERNET,
    CREATE_PLAYDISC,
    DISPLAY_PLAYDISC,
    CREATE_TIME_SETTINGS,
    DISPLAY_TIME_SETTINGS,
    CREATE_RF_SETTINGS,
    DISPLAY_RF_SETTINGS,
    RF_PAIRING_MODE,
    CREATE_PROGRAM_FLASH, 
    DISPLAY_PROGRAM_FLASH_MENU,
    DISPLAY_PROGRAM_FLASH_ACTUAL,
    CREATE_MANAGE_PAIR_TABLE_MODE,
    DISPLAY_MANAGE_PAIR_TABLE_MODE
} SCREEN_STATES;

extern WORD                 channel;
extern SCREEN_STATES        screenState;           

extern GOL_SCHEME		    *pTVRemoteStyleScheme, *pWatchDiscStyleScheme, *pFavMenuScheme;
extern GOL_SCHEME		    *pSettingsScheme, *pSettingsScheme2, *pSettingsScheme3, *pTimeSelectScheme;
extern GOL_SCHEME		    *pRemoteScheme, *pMenuButtonScheme, *pBatStyleScheme, *pTimeDateStyleScheme;

extern signed char          hour, minute, second, day, month;
extern SHORT                year;

extern MIWI_TICK            processingTick;

#if defined (HIDE_PAINTING_EFFECT)
#define HideScreenRendering()	{   while(!_VMRGN); _DPTEST = 1;}
#define ShowScreenRendering()	{   if (_DPTEST == 1) _DPTEST = 0;}
#else
#define HideScreenRendering()	
#define ShowScreenRendering()	
#endif

/************************************************************************
 Function: CreateCommonObjects()
                                                                       
 Overview: Creates the common objects present in all screens
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateCommonObjects(void);

/************************************************************************
 Function: void InitializeGUI(void)
                                                                       
 Overview: This function initializes sub modules used in the GUI.
 		                                                                          
 Input: none.
                                                                       
 Output: none
************************************************************************/
void InitializeGUI(void);

/************************************************************************
 Function: WORD CreateHomeScreen(void)
                                                                       
 Overview: Creates the home screen menu.
 		                                                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
void    CreateHomeScreen(void);

/************************************************************************
 Function: CreateInternetScreen()
                                                                       
 Overview: Creates the favorites screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateInternetScreen(void);

/************************************************************************
 Function: WORD MsgInternetCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the play disc screen 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgInternetCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: CreateFavoritesScreen()
                                                                       
 Overview: Creates the favorites screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateFavoritesScreen(void);

/************************************************************************
 Function: WORD MsgFavoritesCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the play disc screen 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgFavoritesCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: CreatePlayDiscScreen()
                                                                       
 Overview: Creates the Watch TV screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreatePlayDiscScreen(void);

/************************************************************************
 Function: WORD MsgPlayDiscCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the play disc screen 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgPlayDiscCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: CreateWatchTVScreen()
                                                                       
 Overview: Creates the TV channel selection screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateWatchTVScreen(void);

/************************************************************************
 Function: WORD MsgWatchTVCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the watch TV menu
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgWatchTVCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: WORD MsgRFSettingsCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the RF settings screen
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgRFSettingsCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: WORD MsgTimeSettingsCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the Time settings screen
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgTimeSettingsCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: WORD TimeSettingsDrawCallback(void)
 
 Overview: This function is called by GOLDraw() function when the state 
 		   is Time and Date settings.

 Input: none

 Output: If the function returns non-zero the draw control will be passed 
		to GOL. GOLDraw() can proceed and re-draw objects that needs 
		to be redrawn.
************************************************************************/
WORD TimeSettingsDrawCallback(void);

/************************************************************************
 Function: CreateTimeSettingsScreen()
                                                                       
 Overview: Creates the Settings screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateTimeSettingsScreen(void);

/************************************************************************
 Function: CreateRFSettingsScreen()
                                                                       
 Overview: Creates the RF Settings screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateRFSettingsScreen(void);

/************************************************************************
 Function: WORD MsgProgramExternalFlashCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the demo selection. Based on user selection
 		   A demo is enabled by switching the screen state. Icons used
 		   changes from colored version to monochrome version when pressed. 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgProgramExternalFlashCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: WORD MsgManagePairTableCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the demo selection. Based on user selection
 		   A demo is enabled by switching the screen state. Icons used
 		   changes from colored version to monochrome version when pressed. 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgManagePairTableCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: WORD MsgDemoSelection(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
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
WORD    MsgDemoSelectionCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: void InitDemoSelectStyleScheme(GOL_SCHEME *pScheme)
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void    InitDemoSelectStyleScheme(GOL_SCHEME *pScheme);

/************************************************************************
 Function: CreateManagePairTableMode()
                                                                       
 Overview: Creates the flash program screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateManagePairTableMode(void);

/************************************************************************
 Function: CreatePairingResults(BOOL status)
                                                                       
 Overview: Creates the pairing results screen.
                                                                       
 Input: status - results of the pairing (based on RF4CE stack pairing status), 
                 TRUE - success 
                 FALSE - not successful pairing

 Output: none
************************************************************************/
void CreatePairingResults(BOOL status);

/************************************************************************
 Function: void UpdatePairTableLEDStatus()
                                                                       
 Overview: Checks the pair table if there is a valid entry.
           When there is none it will start the LED blinking to
           indicate to user there is no pairing.  
                                                                       
 Input: None
                                                                       
 Output: None
         
************************************************************************/
void UpdatePairTableLEDStatus();

/************************************************************************
 Function: UpdateDateTimeStrings(void)
                                                                       
 Overview: Forces the update of the time string variables.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void UpdateDateTimeStrings(void);

/************************************************************************
 Function: void UpdateTime(WORD force)
                                                                       
 Overview: This function updates the date and time variables.
 		                                                                          
 Input: none.
                                                                       
 Output: none
************************************************************************/
void UpdateTime(WORD force);

/************************************************************************************
* Function: NumberToString(WORD Value, XCHAR *pText, BYTE NoOfDigits, BYTE DotPos )
*
* Notes: convert the number to string
*
*************************************************************************************/
void NumberToString(WORD Value, XCHAR *pText, BYTE NoOfDigits, BYTE DotPos);

/************************************************************************************
* Function: SetRemoteControlModeStrings(REMOTE_CONTROL_RUN_MODE mode)
*
* Notes: Sets the GUI strings to the selected run mode.
*
*************************************************************************************/
void SetRemoteControlModeStrings(REMOTE_CONTROL_RUN_MODE mode);


#endif //GRAPHICSUSERINTERFACE
