/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        GraphicsUserInterface.c
 * Dependencies:    GraphicsUserInterface.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30 V3.00
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software LicenseJpeg
 * Agreement
 *
 * Copyright © 2010 Microchip Technology Inc.  All rights reserved.
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
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/02/10    ...
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "GraphicsUserInterface.h"

#define FORCE_UPDATE 1

// global tick counter
extern  DWORD   tick;

// variables for the strings used.
        XCHAR   CurrentTimeStr[16];

const   XCHAR   AMTime[] = {'A','M',0};
const   XCHAR   PMTime[] = {'P','M',0};

// displayed strings for Remote Control Mode and Version number
        XCHAR   UnivRemoteStr[50];
        XCHAR   UnivRemoteVersionStr[10];


        XCHAR   *DayNightItems[2] = { 	(XCHAR*)AMTime,
    							    	(XCHAR*)PMTime};

        MIWI_TICK    processingTick;

/////////////////////////////////////////////////////////////////////////////
//                       EXTERN FUNCTIONS
/////////////////////////////////////////////////////////////////////////////
extern BOOL RF4CEControllerPairing(void);

/////////////////////////////////////////////////////////////////////////////
//                       GLOBAL VARIABLES FOR DEMO
/////////////////////////////////////////////////////////////////////////////
SCREEN_STATES   screenState = CREATE_HOME;           
GOL_SCHEME		*pTVRemoteStyleScheme, *pWatchDiscStyleScheme, *pFavMenuScheme, *pSettingsScheme, *pSettingsScheme2, *pSettingsScheme3;
GOL_SCHEME		*pRemoteScheme, *pVersionScheme, *pMenuButtonScheme, *pBatStyleScheme, *pTimeDateStyleScheme,  *pTimeSelectScheme;

/////////////////////////////////////////////////////////////////////////////
//                       LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void CreateProgramExternalFlash(void);

/************************************************************************
 Function: CreateCommonObjects()
                                                                       
 Overview: Creates the common objects present in all screens
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateCommonObjects(void)
{
    WORD timeStartX;
	WORD timeWidth, textHeight;

	SetColor(BACKGROUND_COLOR);
	ClearDevice();

    BtnCreate
    (
        ID_MCHPICON, 
        MCHP_LOGO_STARTX, MCHP_LOGO_STARTY, 
        MCHP_LOGO_STARTX + (MCHP_LOGO_WIDTH - 5), 
        MCHP_LOGO_STARTY + (MCHP_LOGO_HEIGHT - 5), 
        0, BTN_DRAW, 
        (void *) &MICROCHIP_LOGO, NULL, 
        pBatStyleScheme 
    );

	PbCreate
	(
		 ID_BATLEVEL,
         BAT_STARTX, BAT_STARTY,
         BAT_STARTX + BAT_WIDTH,
         BAT_STARTY + BAT_HEIGHT,
         PB_DRAW | PB_NOPROGRESS,
         GetBatteryLevel(),
         GetBatteryRange(),
         pBatStyleScheme
	);	

	// calculate the positions of the date and time info
    strcpy(CurrentTimeStr, "Mon 10:10 AM ");

	timeWidth = GetTextWidth(CurrentTimeStr, (void *)&TIMEDATEFONT);
	textHeight = GetTextHeight((void *)&TIMEDATEFONT);	

    timeStartX = BAT_STARTX - timeWidth - 5;

    StCreate
    (
        ID_TIME,             
        timeStartX,
        DATE_TIME_STARTY,                        
        timeStartX + timeWidth,
        DATE_TIME_STARTY + textHeight,
        ST_DRAW, 
        CurrentTimeStr,
        pTimeDateStyleScheme
    );               	

    // This is the battery primitives
	SetColor(TEXTCOLOR_UNPRESSED);
    // draw the outline first
	while(!Bar(	BAT_STARTX - 1, BAT_STARTY - 1, 
				BAT_STARTX + BAT_WIDTH + 1, BAT_STARTY + BAT_HEIGHT + 1));
	// draw the battery head
    while(!Bar(	BAT_STARTX - 3, BAT_STARTY + ((BAT_HEIGHT>>1)-1),
                BAT_STARTX - 2, BAT_STARTY + ((BAT_HEIGHT>>1)+1)));

    UpdateTime(FORCE_UPDATE);
}

/************************************************************************
 Function: CreateHomeScreen()
                                                                       
 Overview: Creates the home screen of the demo.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateHomeScreen(void)
{

    GOLFree();                      // free memory for the objects in the previous linked list and start new list

    HideScreenRendering();

	CreateCommonObjects();

    BtnCreate
    (
        ID_TVICON,                 
        IconLeft(0),                
        IconTop(0),
        IconRight(0),
        IconBottom(0),
        0,                          
        BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL,
        (void *) &TVICON,            
        NULL,
        pRemoteScheme
    );                              

    BtnCreate
    (
        ID_FAVICON,                 
        IconLeft(1),                
        IconTop(0),
        IconRight(1),
        IconBottom(0),
        0,                          
        BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL,
        (void *) &FAVICON,            
        NULL,
        pRemoteScheme
    );                              

    BtnCreate
    (
        ID_INTICON,                 
        IconLeft(0),                
        IconTop(1),
        IconRight(0),
        IconBottom(1),
        0,                          
        BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL,
        (void *) &INTERICON,            
        NULL,
        pRemoteScheme
    );                              

    BtnCreate
    (
        ID_PDICON,                 
        IconLeft(1),                
        IconTop(1),
        IconRight(1),
        IconBottom(1),
        0,                          
        BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL,
        (void *) &PDICON,            
        NULL,
        pRemoteScheme
    );  

    BtnCreate
    (
        ID_SETICON,                 
        IconLeft(0),                
        IconTop(2),
        IconRight(0),
        IconBottom(2),
        0,                          
        BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL,
        (void *) &SETICON,            
        NULL, 
        pRemoteScheme
    );  

    BtnCreate
    (
        ID_REMOTEMODE,                 
        IconLeft(1),                
        IconTop(2),
        IconRight(1),
        IconBottom(2),
        0,                          
        //BTN_DRAW | BTN_NOPANEL | BTN_PRESSED,
        BTN_DRAW,
        NULL,            
        UnivRemoteStr, 
        pVersionScheme
    );  
    
}

/************************************************************************
 Function: void SetRemoteControlModeStrings(REMOTE_CONTROL_RUN_MODE mode)
                                                                       
 Overview: This sets the GUI strings to display the current build time 
 		   selected run mode. In real applications this will not be 
           needed since the application will run in the actual
           RF4CE or other radio modes selected (or supported by this demo).
 		                                                                          
 Input: mode - the run mode ID selected. See REMOTE_CONTROL_RUN_MODE
               definition for details. 
                                                                      
 Output: none
************************************************************************/
void SetRemoteControlModeStrings(REMOTE_CONTROL_RUN_MODE mode)
{
    switch (mode)
    {
        case REMOTE_RF4CE_MODE:
            sprintf(UnivRemoteStr, "Universal\r\nRemote\r\nRF4CE Mode"); 
            break;
        case REMOTE_NORF_MODE:
            sprintf(UnivRemoteStr, "Universal\r\nRemote\r\nNo RF Mode"); 
            break;
        default:
            sprintf(UnivRemoteStr, "Universal\r\nRemote"); 
            break;
    }    
    sprintf(UnivRemoteVersionStr, "ver. %3.2f", REMOTE_FIRMWARE_VERSION); 
}

/************************************************************************
 Function: WORD MsgHomeSelectionCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the demo selection. Based on user selection
 		   A demo is enabled by switching the screen state. Icons used
 		   changes from colored version to monochrome version when pressed. 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgHomeSelectionCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_TVICON:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &TVICONPRESS);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &TVICON);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &TVICON);
                screenState = CREATE_WATCHTV;
            }
            break;

        case ID_FAVICON:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &FAVICONPRESS);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &FAVICON);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &FAVICON);
                screenState = CREATE_FAVORITES;
            }
            break;

        case ID_INTICON:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &INTERICONPRESS);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &INTERICON);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &INTERICON);
                screenState = CREATE_INTERNET;
            }
            break;

        case ID_PDICON:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &PDICONPRESS);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &PDICON);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &PDICON);
                screenState = CREATE_PLAYDISC;
            }
            break;

        case ID_SETICON:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &SETICONPRESS);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &SETICON);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &SETICON);
                screenState = CREATE_RF_SETTINGS;
            }
            break;

        case ID_REMOTEMODE:
            if(objMsg == BTN_MSG_PRESSED)
            {           
                BtnSetText((BUTTON*)pObj, UnivRemoteVersionStr);
            }
            else if((objMsg == BTN_MSG_CANCELPRESS) || (objMsg == BTN_MSG_RELEASED))
            {
                BtnSetText((BUTTON*)pObj, UnivRemoteStr);
            }
            SetState(pObj, BTN_PRESSED);
            break;

        default:
        	break;
    }
	
    return (1);
}

/************************************************************************
 Function: void InitializeGUI(void)
                                                                       
 Overview: This function initializes sub modules used in the GUI.
 		                                                                          
 Input: none.
                                                                       
 Output: none
************************************************************************/
void InitializeGUI(void)
{

	// initialize the real time clock
  	RTCCInit();
  	RTCCProcessEvents();
		
	//make sure the strings are terminated
	_time_str[13] = 0;
	_date_str[13] = 0;
    CurrentTimeStr[13] = 0;

	// create and initialize style schemes

	pRemoteScheme = GOLCreateScheme();      	// used for the generic scheme 	   		

    pRemoteScheme->TextColor0 = TEXTCOLOR_UNPRESSED;
    pRemoteScheme->TextColor1 =  REMOTE_YELLOW;
    pRemoteScheme->CommonBkColor = REMOTE_GRAY;
    pRemoteScheme->EmbossDkColor = KEYPAD_DARKEMBOSS;
    pRemoteScheme->EmbossLtColor = KEYPAD_LIGHTEMBOSS;
    pRemoteScheme->Color0 = KEYPAD_BACKGROUND;
    pRemoteScheme->Color1 = REMOTE_BLACK;

	pVersionScheme = GOLCreateScheme();      	// used for the generic scheme 	   		

    pVersionScheme->TextColor1 = REMOTE_YELLOW;
    pVersionScheme->TextColor0 =  REMOTE_YELLOW;
    pVersionScheme->CommonBkColor = REMOTE_BLACK;
    pVersionScheme->EmbossDkColor = REMOTE_BLACK;
    pVersionScheme->EmbossLtColor = REMOTE_BLACK;
    pVersionScheme->Color0 = REMOTE_BLACK;
    pVersionScheme->Color1 = REMOTE_BLACK;

	pMenuButtonScheme = GOLCreateScheme();  	// used for menu buttons that has text

    pMenuButtonScheme->TextColor0 = REMOTE_BLACK;
    pMenuButtonScheme->TextColor1 = REMOTE_WHITE;
    pMenuButtonScheme->CommonBkColor = BACKGROUND_COLOR;
    pMenuButtonScheme->pFont = (void*)&REMOTEFONT_14;

	pTimeDateStyleScheme = GOLCreateScheme();	// used for the time and date display 

	pTimeDateStyleScheme->TextColor0 = TEXTCOLOR_UNPRESSED;
    pTimeDateStyleScheme->TextColor1 = REMOTE_GRAY;
    pTimeDateStyleScheme->CommonBkColor = BACKGROUND_COLOR;
    pTimeDateStyleScheme->pFont = (void*)&TIMEDATEFONT;

	pBatStyleScheme = GOLCreateScheme();	 	// used for the battery level indicator

    pBatStyleScheme->EmbossDkColor = REMOTE_BLACK;
    pBatStyleScheme->EmbossLtColor = REMOTE_BLACK;
    pBatStyleScheme->Color0 = BATTERY_LEVEL_COLOR;
    pBatStyleScheme->Color1 = REMOTE_BLACK; 
    
    // set the font to NULL so the progress bar used will not show
    // any text on the progress bar
    pBatStyleScheme->pFont = NULL; 

    pTVRemoteStyleScheme = GOLCreateScheme();   // watch TV button schemes

    pTVRemoteStyleScheme->EmbossDkColor = KEYPAD_DARKEMBOSS;
    pTVRemoteStyleScheme->EmbossLtColor = KEYPAD_LIGHTEMBOSS;
    pTVRemoteStyleScheme->TextColor0 = KEYPAD_TEXTCOLOR0;
    pTVRemoteStyleScheme->TextColor1 = REMOTE_YELLOW; 
    pTVRemoteStyleScheme->Color0 = KEYPAD_BACKGROUND;
    pTVRemoteStyleScheme->Color1 = REMOTE_BLACK;
    pTVRemoteStyleScheme->pFont = (void*)&TVREMOTEFONT;

    pWatchDiscStyleScheme = GOLCreateScheme();   // watch TV button schemes

    pWatchDiscStyleScheme->EmbossDkColor = KEYPAD_DARKEMBOSS;
    pWatchDiscStyleScheme->EmbossLtColor = KEYPAD_LIGHTEMBOSS;
    pWatchDiscStyleScheme->TextColor0 = KEYPAD_TEXTCOLOR0;
    pWatchDiscStyleScheme->TextColor1 = REMOTE_YELLOW; 
    pWatchDiscStyleScheme->Color0 = KEYPAD_BACKGROUND;
    pWatchDiscStyleScheme->Color1 = REMOTE_BLACK;
    pWatchDiscStyleScheme->pFont = (void*)&FAVMENUFONT;
    pWatchDiscStyleScheme->TextColorDisabled = REMOTE_BLACK;
    pWatchDiscStyleScheme->ColorDisabled = REMOTE_GRAY;
    pWatchDiscStyleScheme->CommonBkColor = BACKGROUND_COLOR;

    pFavMenuScheme = GOLCreateScheme();   // favorite menu schemes

    pFavMenuScheme->EmbossDkColor = KEYPAD_DARKEMBOSS;
    pFavMenuScheme->EmbossLtColor = KEYPAD_LIGHTEMBOSS;
    pFavMenuScheme->TextColor0 = KEYPAD_TEXTCOLOR0;
    pFavMenuScheme->TextColor1 = REMOTE_YELLOW; 
    pFavMenuScheme->Color0 = KEYPAD_BACKGROUND;
    pFavMenuScheme->Color1 = REMOTE_BLACK;
    pFavMenuScheme->pFont = (void*)&FAVMENUFONT;

    pSettingsScheme = GOLCreateScheme();   // Settings menu schemes 

    pSettingsScheme->EmbossDkColor = REMOTE_GRAY64;
    pSettingsScheme->EmbossLtColor = TEXTCOLOR_UNPRESSED;
    pSettingsScheme->TextColor0 = REMOTE_BLACK; 
    pSettingsScheme->TextColor1 = REMOTE_WHITE;
    pSettingsScheme->Color0 = REMOTE_BLACK;
    pSettingsScheme->Color1 = REMOTE_WHITE;
    pSettingsScheme->pFont = (void*)&TIMEDATEFONT;

    pSettingsScheme2 = GOLCreateScheme();   // Settings menu schemes 2

    pSettingsScheme2->EmbossDkColor = KEYPAD_LIGHTEMBOSS;
    pSettingsScheme2->EmbossLtColor = KEYPAD_LIGHTEMBOSS;
    pSettingsScheme2->TextColor0 = REMOTE_WHITE;
    pSettingsScheme2->TextColor1 = REMOTE_YELLOW;
    pSettingsScheme2->Color0 = KEYPAD_BACKGROUND;
    pSettingsScheme2->Color1 = REMOTE_BLACK;
    pSettingsScheme2->CommonBkColor = REMOTE_GRAY;
    pSettingsScheme2->pFont = (void*)&FAVMENUFONT;
    pSettingsScheme2->ColorDisabled = REMOTE_GRAY;
    pSettingsScheme2->TextColorDisabled = KEYPAD_LIGHTEMBOSS;

    pTimeSelectScheme = GOLCreateScheme();   

    pTimeSelectScheme->EmbossDkColor = KEYPAD_DARKEMBOSS;
    pTimeSelectScheme->EmbossLtColor = KEYPAD_LIGHTEMBOSS;
    pTimeSelectScheme->TextColor0 = REMOTE_YELLOW;;
    pTimeSelectScheme->TextColor1 = KEYPAD_TEXTCOLOR1;
    pTimeSelectScheme->Color0 = REMOTE_BLACK;
    pTimeSelectScheme->Color1 = REMOTE_BLACK;
    pTimeSelectScheme->CommonBkColor = REMOTE_GRAY;
    pTimeSelectScheme->pFont = (void*)&FAVMENUFONT;

    pSettingsScheme3 = GOLCreateScheme();   // Settings menu schemes 3

    pSettingsScheme3->EmbossDkColor = KEYPAD_DARKEMBOSS;
    pSettingsScheme3->EmbossLtColor = KEYPAD_LIGHTEMBOSS;
    pSettingsScheme3->TextColor0 = KEYPAD_TEXTCOLOR0;
    pSettingsScheme3->TextColor1 = REMOTE_YELLOW; 
    pSettingsScheme3->Color0 = KEYPAD_BACKGROUND;
    pSettingsScheme3->Color1 = REMOTE_BLACK;
    pSettingsScheme3->CommonBkColor = REMOTE_GRAY;
    pSettingsScheme3->pFont = (void*)&FAVMENUFONT;


}

/************************************************************************
 Function: void UpdateTime(WORD force)
                                                                       
 Overview: This function updates the date and time variables.
 		                                                                          
 Input: none.
                                                                       
 Output: none
************************************************************************/
void UpdateTime(WORD force)
{
   	static char previousChar;
    char changeIndicator;    
	OBJ_HEADER  *pObject;
    char currentHour;
    char hourStr[3];
	
	RTCCProcessEvents();

    changeIndicator = _time_str[8];

    if ((previousChar != changeIndicator) || (force == FORCE_UPDATE))
    {
    	previousChar = changeIndicator;

        if ((pObject = GOLFindObject(ID_TIME)) != NULL)
        {
            strncpy(CurrentTimeStr, _time_str, 9);
           
            // change hour to AM/PM format
            currentHour = mRTCCGetBinHour();

            if (hour >= 12) 
                strncpy(&CurrentTimeStr[10], DayNightItems[1], 3);
            else
                strncpy(&CurrentTimeStr[10], DayNightItems[0], 3);

            if (currentHour > 12)
            {
                currentHour = currentHour - 12;
                
                // now set the hour in the string
                NumberToString(currentHour, hourStr, 2, 0);
                CurrentTimeStr[4] = hourStr[0];
                CurrentTimeStr[5] = hourStr[1];
                
            }
            
        	StSetText((STATICTEXT *)pObject, CurrentTimeStr);
        	SetState( pObject, ST_DRAW );
		}
 
        if ((pObject = GOLFindObject(ID_BATLEVEL)) != NULL)
        {
            PbSetPos( (PROGRESSBAR*)pObject, GetBatteryLevel());
            SetState( pObject, PB_DRAW_BAR );
        }
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
    BOOL status;
	
	// update the time and date variables
	UpdateTime(0);
	
	switch(screenState)
    {
        case CREATE_HOME:
            CreateHomeScreen();
            TransparentColorDisable(); 
            screenState = DISPLAY_HOME;    
            break;

        case CREATE_WATCHTV:
        	CreateWatchTVScreen();
            // enable the transparent color feature
            TransparentColorEnable(TRANSPARENT_COLOR);
            screenState = DISPLAY_WATCHTV;    
            break;
            
        case CREATE_RF_SETTINGS:
        	CreateRFSettingsScreen();
            // enable the transparent color feature
            TransparentColorEnable(TRANSPARENT_COLOR);
            screenState = DISPLAY_RF_SETTINGS;    
            break;


        case CREATE_TIME_SETTINGS:
        	CreateTimeSettingsScreen();
            processingTick = RF4CE_TickGet();
            // enable the transparent color feature
            TransparentColorEnable(TRANSPARENT_COLOR);
            screenState = DISPLAY_TIME_SETTINGS;    
            break;

        case CREATE_PLAYDISC:
        	CreatePlayDiscScreen();
            // enable the transparent color feature
            TransparentColorEnable(TRANSPARENT_COLOR);
            screenState = DISPLAY_PLAYDISC;    
            break;
            
        case CREATE_FAVORITES:
        	CreateFavoritesScreen();
            // enable the transparent color feature
            TransparentColorEnable(TRANSPARENT_COLOR);
            screenState = DISPLAY_FAVORITES;    
            break;
            
        case CREATE_INTERNET:
            // enable the transparent color feature
            TransparentColorEnable(TRANSPARENT_COLOR);
        	CreateInternetScreen();
            screenState = DISPLAY_INTERNET;    
            break;
           
        case RF_PAIRING_MODE:
            // if RF4CEControllerPairing() is successful it will also 
            // set the active device to the newly paired device 
            status = RF4CEControllerPairing();
            processingTick = RF4CE_TickGet();

            // show results of the pairing first
            CreatePairingResults(status);
            DelayMs(1500);

            UpdatePairTableLEDStatus();

            // Due to primitive objects in the screen of RF Settings, a complete 
            // redraw of the screen is required. If all are Widgets then it is possible
            // to just call  GOLRedrawRec(0, 0, GetMaxX(), GetMaxY());
            // and set state to 
            // screenState = DISPLAY_RF_SETTINGS;   

            screenState = CREATE_RF_SETTINGS;   
            break;

        case CREATE_PROGRAM_FLASH:
            CreateProgramExternalFlash();                  
            screenState = DISPLAY_PROGRAM_FLASH_MENU;    
            break;

        case DISPLAY_PROGRAM_FLASH_ACTUAL:

            // perform a external flash memory erase and a system reset
            // after reset, the check on validity of the external flash
            // will be launched and this will cause the programming
            // mode to be called. Hex data for the external flash
            // should then be sent to the board.
            PowerSaveBacklightOff();
            ChipErase();
            Reset();
            break;

        case CREATE_MANAGE_PAIR_TABLE_MODE:
            CreateManagePairTableMode();                  
            screenState = DISPLAY_MANAGE_PAIR_TABLE_MODE;    
            break;

        case DISPLAY_TIME_SETTINGS:
            TimeSettingsDrawCallback();
            break;

        case DISPLAY_HOME:
        case DISPLAY_WATCHTV:
        case DISPLAY_RF_SETTINGS:
        case DISPLAY_PLAYDISC:
        case DISPLAY_FAVORITES:
        case DISPLAY_INTERNET:
        case DISPLAY_PROGRAM_FLASH_MENU:
        case DISPLAY_MANAGE_PAIR_TABLE_MODE:
        default:
            break;
    }

    return (1); 

}


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
	WORD status = 0;
	
    switch(screenState)
    {
        case DISPLAY_HOME:
            status = (MsgHomeSelectionCallback(objMsg, pObj, pMsg));
			break;

        case DISPLAY_WATCHTV:
            status = (MsgWatchTVCallback(objMsg, pObj, pMsg));
			break;

        case DISPLAY_RF_SETTINGS:
            status = (MsgRFSettingsCallback(objMsg, pObj, pMsg));
			break;

        case DISPLAY_TIME_SETTINGS:
            status = (MsgTimeSettingsCallback(objMsg, pObj, pMsg));
			break;

        case DISPLAY_PLAYDISC:
            status = (MsgPlayDiscCallback(objMsg, pObj, pMsg));
			break;

        case DISPLAY_FAVORITES:
            status = (MsgFavoritesCallback(objMsg, pObj, pMsg));
			break;

        case DISPLAY_INTERNET:
            status = (MsgInternetCallback(objMsg, pObj, pMsg));
			break;

        case DISPLAY_PROGRAM_FLASH_MENU:
            status = (MsgProgramExternalFlashCallback(objMsg, pObj, pMsg));
			break;

        case DISPLAY_MANAGE_PAIR_TABLE_MODE:
            status = (MsgManagePairTableCallback(objMsg, pObj, pMsg));
            break;

        default:
            break;
            
    }	
	return (status);
}	

/************************************************************************************
* Function: NumberToString(WORD Value, XCHAR *pText, BYTE NoOfDigits, BYTE DotPos )
*
* Notes: convert the number to string
*
*************************************************************************************/
void NumberToString(WORD Value, XCHAR *pText, BYTE NoOfDigits, BYTE DotPos)

{
    BYTE    i;
    BYTE    bIndex;

    for(bIndex = 0; bIndex < NoOfDigits; bIndex++)
    {
        pText[NoOfDigits - bIndex - 1] = '0' + (Value % 10);
        Value /= 10;
    }

    if(DotPos != 0 && DotPos <= NoOfDigits)
    {
        for(i = 0; i < DotPos; i++)
        {
            pText[NoOfDigits - i] = pText[NoOfDigits - 1 - i];
        }

        pText[NoOfDigits - DotPos] = '.';
        pText[NoOfDigits + 1] = '\0';
    }

 
    //If dot position is 0 or greater than number of digits, then don't put dot in the display
    else
    {
        pText[NoOfDigits] = '\0';
    }
}

 



