/*****************************************************************************
 * Microchip Graphics Library Demo Application
 *****************************************************************************
 * FileName:        RemoteSettingSInterface.c
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
 * 09/21/11    ...
 *****************************************************************************/
#include "GraphicsUserInterface.h"
#include "led.h"

#define STATUSDIST      (25)
#define STATUSRADIUS    (6)
#define STATUSXSTART    ((GetMaxX()>>1))
#define STATUSYSTART    ((GetMaxY()>>2))
#define STATUSMIDPTS    (STATUSDIST*(.707))


const XCHAR             January[] = {'J','a','n',0};
const XCHAR             February[] = {'F','e','b',0};
const XCHAR             March[] = {'M','a','r',0};
const XCHAR             April[] = {'A','p','r',0};
const XCHAR             May[] = {'M','a','y',0};
const XCHAR             June[] = {'J','u','n',0};
const XCHAR             July[] = {'J','u','l',0};
const XCHAR             August[] = {'A','u','g',0};
const XCHAR             September[] = {'S','e','p',0};
const XCHAR             October[] = {'O','c','t',0};
const XCHAR             November[] = {'N','o','v',0};
const XCHAR             December[] = {'D','e','c',0};

const XCHAR             ClearPairTableStr[] = "Clear\nAll";
const XCHAR             ClearSelectedTableEntryStr[] = "Clear\nSelected";
const XCHAR             ProceedStr[] = "Proceed";
const XCHAR             CancelStr[] = "Cancel";

BYTE  daysPerMonth[12] = { 31, 28, 31, 30, 31, 30,
        				   31, 31, 30, 31, 30, 31 };

char    SetMonthStr[4];
char    SetDayStr[3];
char    SetYearStr[5];
char    SetHourStr[3];
char    SetMinuteStr[3];
char    SetSecondStr[3];

WORD    selection = ID_ST_MONTH;

XCHAR   *MoItems[13] = { NULL,
                        (XCHAR*)January,
    					(XCHAR*)February,
    					(XCHAR*)March,
    					(XCHAR*)April,
    					(XCHAR*)May,
    					(XCHAR*)June,
    					(XCHAR*)July,
    					(XCHAR*)August,
    					(XCHAR*)September,
    					(XCHAR*)October,
    					(XCHAR*)November,
    					(XCHAR*)December};

typedef struct 
{
    WORD xPos;
    WORD yPos;
} PROCESSING_PNT;	

const PROCESSING_PNT positions[] =    {
         	                            {STATUSXSTART,				STATUSYSTART-STATUSDIST},
    								    {STATUSXSTART+STATUSMIDPTS, STATUSYSTART-STATUSMIDPTS},
    								    {STATUSXSTART+STATUSDIST, 	STATUSYSTART},
    								    {STATUSXSTART+STATUSMIDPTS, STATUSYSTART+STATUSMIDPTS},
    								    {STATUSXSTART, 				STATUSYSTART+STATUSDIST},
    								    {STATUSXSTART-STATUSMIDPTS, STATUSYSTART+STATUSMIDPTS},
    								    {STATUSXSTART-STATUSDIST, 	STATUSYSTART},
    								    {STATUSXSTART-STATUSMIDPTS, STATUSYSTART-STATUSMIDPTS}
							        };

#define NUM_PROCESSING_POS          (sizeof(positions) / sizeof(PROCESSING_PNT))	

signed char     hour, minute, second, day, month;
        SHORT   year;
        XCHAR   macAddrStr[60];
        XCHAR   destAddrStr[MAX_PAIR_SIZE][33];
        WORD    processing = 0;															

// prototypes for internal functions
void CreateSelectButtons();
void FillRFSettingsWidgets(LISTBOX *pLb, STATICTEXT *pSt);
void ShowMessage(XCHAR *pString, void *pFont);

/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
/* ------------------------------------------------------------------------------------- */
//  Create Settings Functions
/* ------------------------------------------------------------------------------------- */
/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */

/************************************************************************
 Function: CreateRFSettingsScreen()
                                                                       
 Overview: Creates the RF Settings screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateRFSettingsScreen(void)
{
    LISTBOX             *pLb;
    STATICTEXT          *pSt;
    WORD                listBoxHeight, pairClearButtonStartY;

    static XCHAR        ClearDevicesStr[] = "Clear\nPairing...";
    static XCHAR        PairDeviceStr[] = "Pair\nDevice...";

    GOLFree();                      // free memory for the objects in the previous linked list and start new list

    HideScreenRendering();

	CreateCommonObjects();

	SetColor(REMOTE_GRAY);
    while(!Bar(0, TOP_MARGIN, GetMaxX(), GetMaxY()));

    listBoxHeight = GetTextHeight(pRemoteScheme->pFont) * (MAX_PAIR_SIZE+1);
    pairClearButtonStartY = (PAIR_LIST_ORIGIN_Y + listBoxHeight) + ((KPTop(3)-(PAIR_LIST_ORIGIN_Y + listBoxHeight) - PAIRCLEAR_HEIGHT)>>1);

    pLb = LbCreate
    (
        ID_PAIR_LIST, 
        PAIR_LIST_ORIGIN_X, PAIR_LIST_ORIGIN_Y,
        PAIR_LIST_ORIGIN_X + PAIR_LIST_WIDTH,
        PAIR_LIST_ORIGIN_Y + listBoxHeight,
        LB_DRAW | LB_SINGLE_SEL | LB_CENTER_ALIGN,
        NULL,
        pRemoteScheme
    );                                          // use alternate scheme

   	pSt = StCreate
    (
        ID_ST_MACADDR,             
        MACADDR_ORIGIN_X, MACADDR_ORIGIN_Y,                        
        MACADDR_ORIGIN_X + MACADDR_WIDTH, MACADDR_ORIGIN_Y + MACADDR_HEIGHT, 
        ST_DRAW | ST_CENTER_ALIGN, 
        NULL,
        pRemoteScheme 
    );        

    // fill the list box and the static text with data
    FillRFSettingsWidgets(pLb, pSt);

    BtnCreate
    (
        ID_PAIR_DEVICE,                 
        PAIR_START_X,                
        pairClearButtonStartY,
        PAIR_START_X + PAIRCLEAR_WIDTH,
        pairClearButtonStartY + PAIRCLEAR_HEIGHT,
        0,                          
        BTN_DRAW,
        NULL,           
        (XCHAR *)PairDeviceStr,
        pWatchDiscStyleScheme
    );                              
    
    BtnCreate
    (
        ID_ERASE_PAIR_TABLE,                 
        CLEAR_START_X,                
        pairClearButtonStartY,
        CLEAR_START_X + PAIRCLEAR_WIDTH,
        pairClearButtonStartY + PAIRCLEAR_HEIGHT,
        0,                          
        BTN_DRAW,
        NULL,           
        (XCHAR *)ClearDevicesStr,
        pWatchDiscStyleScheme
    );          

    CreateSelectButtons();

}

/************************************************************************
 Function: CreateTimeSettingsScreen()
                                                                       
 Overview: Creates the Settings screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateTimeSettingsScreen(void)
{
	WORD         tempValue;

    static XCHAR plusStr[] = {0x3C, 0};     // increment symbol
    static XCHAR minusStr[] = {0x3E, 0};    // decrement symbol
	
    // free memory for the objects in the previous linked list and start new list
    GOLFree();                              

    // set the default selection to the month item
    selection = ID_ST_MONTH;

    HideScreenRendering();

	CreateCommonObjects();

	SetColor(REMOTE_GRAY);
    while(!Bar(0, TOP_MARGIN, GetMaxX(), GetMaxY()));

	// *********************************************************** 	
	// Set up the month, day and year selection
	// *********************************************************** 	

	// Now update the date and time if needed
    RTCCProcessEvents();
    hour    = mRTCCGetBinHour();
    minute  = mRTCCGetBinMin();
    second  = mRTCCGetBinSec();
    day     = mRTCCGetBinDay();
    month   = mRTCCGetBinMonth();
    year    = mRTCCGetBinYear();

    CreateSelectButtons();

	UpdateDateTimeStrings();	
	
	tempValue = mRTCCGetBinMonth();
	EbCreate
    (
        ID_ST_MONTH,             
        SETLeft(0),
        SETTop(0),                        
        SETRight(0),
        SETBottom(0),
        EB_DRAW | EB_CENTER_ALIGN, 
        MoItems[tempValue],
        5,
        pTimeSelectScheme 
    );               	

	tempValue = mRTCCGetBinDay();
	EbCreate
    (
        ID_ST_DAY,             
        SETLeft(1),
        SETTop(0),                        
        SETRight(1),
        SETBottom(0),
        EB_DRAW | EB_CENTER_ALIGN, 
        SetDayStr,
        5,
        pSettingsScheme2 
    );      

	EbCreate
    (
        ID_ST_YEAR,             
        SETLeft(2),
        SETTop(0),                        
        SETRight(2),
        SETBottom(0),
        EB_DRAW | EB_CENTER_ALIGN,
        SetYearStr,
        5,
        pSettingsScheme2 
    );               	

	EbCreate
    (
        ID_ST_HOUR,             
        SETLeft(0),
        SETTop(1),                        
        SETRight(0),
        SETBottom(1),
        EB_DRAW | EB_CENTER_ALIGN,
        SetHourStr,
        5,
        pSettingsScheme2
    );               	

	EbCreate
    (
        ID_ST_MINUTE,             
        SETLeft(1),
        SETTop(1),                        
        SETRight(1),
        SETBottom(1),
        EB_DRAW | EB_CENTER_ALIGN, 
        SetMinuteStr,
        5,
        pSettingsScheme2
    );               	

	EbCreate
    (
        ID_ST_SECOND,             
        SETLeft(2),
        SETTop(1),                        
        SETRight(2),
        SETBottom(1),
        EB_DRAW | EB_CENTER_ALIGN,
        SetSecondStr,
        5,
        pSettingsScheme2
    );               	

    BtnCreate
    (
        ID_UP_BTN,
        SETLeft(0) + (SET_WIDTH>>1),                
        SETTop(2)+1,
        SETRight(0) + (SET_WIDTH>>1),
        SETBottom(2),
        0,                          
        BTN_DRAW, 
        NULL, 
        plusStr,
        pSettingsScheme2 
    ); 

    BtnCreate
    (
        ID_DOWN_BTN,
        SETLeft(1) + (SET_WIDTH>>1),                
        SETTop(2)+1,
        SETRight(1) + (SET_WIDTH>>1),
        SETBottom(2),
        0,                          
        BTN_DRAW, 
        NULL, 
        minusStr,
        pSettingsScheme2 
    ); 

}	

/************************************************************************
 Function: FillRFSettingsWidgets(LISTBOX *pLb, STATICTEXT *pSt)
                                                                       
 Overview: Fills the list box and static text data for the 
           mac address of the remote and the paired devices.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void FillRFSettingsWidgets(LISTBOX *pLb, STATICTEXT *pSt)
{
    int                 i;
    BYTE                rf4ceNodeAddr[8], activeDev;
    WORD                status;

    if (pSt != NULL)
    {
        // get remote address
        RF4CE_CB_Get64BitNodeAddress(rf4ceNodeAddr);
    
        sprintf(macAddrStr,"Remote Control Address:\n%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",\
                        (WORD)rf4ceNodeAddr[0], (WORD)rf4ceNodeAddr[1], \
                        (WORD)rf4ceNodeAddr[2], (WORD)rf4ceNodeAddr[3], \
                        (WORD)rf4ceNodeAddr[4], (WORD)rf4ceNodeAddr[5], \
                        (WORD)rf4ceNodeAddr[6], (WORD)rf4ceNodeAddr[7]);
    
        StSetText(pSt, macAddrStr);       	
    }

    // check which entry is active
    if (RF4CEControllerCheckPairTable(&activeDev) != 0)
    {
        if (activeDev != RF4CEControllerGetActiveDevice())
            activeDev = RF4CEControllerGetActiveDevice();
    }
    else
        activeDev = 0xFF;

    for (i = 0; i < MAX_PAIR_SIZE; i++)
    {
        if (RF4CEControllerCheckPairTableEntry(i) == TRUE)
        {
            sprintf(destAddrStr[i],"Dev %d:%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",\
                            (WORD)(i+1),\
                            (WORD)PairTable[i].DstIEEEAddress[7],\
                            (WORD)PairTable[i].DstIEEEAddress[6],\
                            (WORD)PairTable[i].DstIEEEAddress[5],\
                            (WORD)PairTable[i].DstIEEEAddress[4],\
                            (WORD)PairTable[i].DstIEEEAddress[3],\
                            (WORD)PairTable[i].DstIEEEAddress[2],\
                            (WORD)PairTable[i].DstIEEEAddress[1],\
                            (WORD)PairTable[i].DstIEEEAddress[0]);
        }
        else
        {
            sprintf(destAddrStr[i],"--empty--");
        }
        if (activeDev == i)
            status = LB_STS_SELECTED;
        else
            status = 0;

        LbAddItem(pLb, NULL, destAddrStr[i], NULL, status, 0);

    }

}

/************************************************************************
 Function: CreateSelectButtons()
                                                                       
 Overview: Creates the buttons that selects between the RF settings, 
           Time settings and the Home button.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateSelectButtons()
{

	static XCHAR PairRFStr[] = "Set\nRF";
	static XCHAR SetStr[] = "Set\nTime";

	// *********************************************************** 	
	// Set up pair, set and home buttons
	// *********************************************************** 	

    BtnCreate
    (
        ID_RF_SETTINGS,                 
        KPLeft(0),
        KPTop(3),
        KPRight(0),
        KPBottom(3),
        0,                          
        BTN_DRAW,
        NULL,            
        PairRFStr,
        pSettingsScheme3
    );                     

    BtnCreate
    (
        ID_TIME_SETTINGS,
        KPLeft(1),
        KPTop(3),
        KPRight(1),
        KPBottom(3),
        0,                          
        BTN_DRAW,
        NULL, 
        SetStr,
        pSettingsScheme3
    );                     

    BtnCreate
    (
        ID_HOME,                 
        KPLeft(2),                
        KPTop(3),
        KPRight(2),
        KPBottom(3),
        0,                          
        BTN_DRAW,
        (void *) &ICON_HOME,            
        NULL,
        pSettingsScheme3
    );                     

}

/************************************************************************
 Function: CreateProgramExternalFlash()
                                                                       
 Overview: Creates the flash program screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateProgramExternalFlash(void)
{
    WORD textHeight;
    void *pFont;

    GOLFree();                      // free memory for the objects in the previous linked list and start new list

    pFont = (void* )&REMOTEFONT_14;
    textHeight = GetTextHeight(pFont);
    SetColor(REMOTE_BLACK);
    ClearDevice();
    SetColor(REMOTE_WHITE);
	SetFont(pFont);
	OutTextXY(10,10,"Erase all external flash data?"); 
    OutTextXY(10,10+textHeight,"This will erase:");
    OutTextXY(10,10+(textHeight*2),"- touch calibration");
    OutTextXY(10,10+(textHeight*3),"- pairing table"); 
    OutTextXY(10,10+(textHeight*4),"- graphics resources");
    OutTextXY(10,10+(textHeight*5),"then reset the board &"); 
    OutTextXY(10,10+(textHeight*6),"prompt programming mode.");
    SetColor(REMOTE_YELLOW);
	OutTextXY(10,10+(textHeight*8),"Do you want to proceed?");

    BtnCreate
    (
        ID_PGM_YES,                 
        PROCEED_START_X,                
        PROCEED_START_Y,
        PROCEED_START_X + PROCAN_WIDTH,
        PROCEED_START_Y + PROCAN_HEIGHT,
        0,                          
        BTN_DRAW,
        NULL,           
        (XCHAR *)ProceedStr,
        pWatchDiscStyleScheme
    );                              

    BtnCreate
    (
        ID_PGM_NO,                 
        CANCEL_START_X,                
        CANCEL_START_Y,
        CANCEL_START_X + PROCAN_WIDTH,
        CANCEL_START_Y + PROCAN_HEIGHT,
        0,                          
        BTN_DRAW,
        NULL,           
        (XCHAR *)CancelStr,
        pWatchDiscStyleScheme
    );          
    
}

/************************************************************************
 Function: CreatePairingResults(BOOL status)
                                                                       
 Overview: Creates the pairing results screen.
                                                                       
 Input: status - results of the pairing (based on RF4CE stack pairing status), 
                 TRUE - success 
                 FALSE - not successful pairing

 Output: none
************************************************************************/
void CreatePairingResults(BOOL status)
{
    XCHAR resultsSuccessStr[] = "Pairing Success!";
    XCHAR resultsFailedStr[] = "Pairing Failed!";
    XCHAR *pStr;

    if (status == TRUE)
    {
        pStr = resultsSuccessStr;
    }
    else
    {
        pStr = resultsFailedStr;
    }

    ShowMessage(pStr, (void* )&REMOTEFONT_14);

}

/************************************************************************
 Function: CreateManagePairTableMode()
                                                                       
 Overview: Creates the flash program screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateManagePairTableMode(void)
{
    LISTBOX *pLb;

    GOLFree();                      // free memory for the objects in the previous linked list and start new list

    SetColor(REMOTE_BLACK);
    ClearDevice();

    pLb = LbCreate
    (
        ID_PAIR_LIST, 
        ERASE_LIST_START_X,
        ERASE_LIST_START_Y,
        GetMaxX(),
        ERASE_LIST_START_Y + ERASERF_HEIGHT,        
        LB_DRAW | LB_SINGLE_SEL | LB_CENTER_ALIGN,
        NULL,
        pRemoteScheme
    );                                          // use alternate scheme

    FillRFSettingsWidgets(pLb, NULL);

    // check if the table is empty
    
    BtnCreate
    (
        ID_ERASE_CANCEL,                 
        ERASE_CANCEL_START_X,                
        ERASE_CANCEL_START_Y,
        ERASE_CANCEL_START_X + ERASERF_WIDTH,
        ERASE_CANCEL_START_Y + ERASERF_HEIGHT,
        0,                          
        BTN_DRAW,
        NULL,           
        (XCHAR *)CancelStr, 
        pWatchDiscStyleScheme
    );                              

    BtnCreate
    (
        ID_ERASE_TABLE,                 
        ERASE_TABLE_START_X,                
        ERASE_TABLE_START_Y,
        ERASE_TABLE_START_X + ERASERF_WIDTH,
        ERASE_TABLE_START_Y + ERASERF_HEIGHT,
        0,                          
        BTN_DRAW,
        NULL,           
        (XCHAR *)ClearPairTableStr, 
        pWatchDiscStyleScheme
    );                              
#ifdef ENABLE_PAIR_ENTRY_DELETION
    BtnCreate
    (
        ID_ERASE_ENTRY,                 
        ERASE_ENTRY_START_X,                
        ERASE_ENTRY_START_Y,
        ERASE_ENTRY_START_X + ERASERF_WIDTH,
        ERASE_ENTRY_START_Y + ERASERF_HEIGHT,
        0,                          
        BTN_DRAW,
        NULL,           
        (XCHAR *)ClearSelectedTableEntryStr,
        pWatchDiscStyleScheme
    );          
#endif

}

/************************************************************************
 Function: void UpdatePairTableLEDStatus()
                                                                       
 Overview: Checks the pair table if there is a valid entry.
           When there is none it will start the LED blinking to
           indicate to user there is no pairing.  
                                                                       
 Input: None
                                                                       
 Output: None
         
************************************************************************/
void UpdatePairTableLEDStatus()
{
    int i;

    for (i = 0 ; i < MAX_PAIR_SIZE; i++)
    {
        if (RF4CEControllerCheckPairTableEntry(i) == TRUE)
        {
            LEDOff(RED1);
            return;
        }
    }

    LEDBlink(RED1);
    RF4CEControllerSetActiveDevice(0xFF); 

    return;
}

/************************************************************************
 Function: void ShowMessage(XCHAR *pString, void *pFont)
                                                                       
 Overview: A generic message screen.
                                                                       
 Input: pString - Pointer to the string to be displayed. 
        pFont - Pointer to the font to be used.
                                                                  
                                                                       
 Output: None
         
************************************************************************/
void ShowMessage(XCHAR *pString, void *pFont)
{
    WORD stx, sty;

    SetColor(REMOTE_BLACK);
    ClearDevice();

    SetColor(REMOTE_YELLOW);
	SetFont(pFont);
    
    stx = (GetMaxX() - GetTextWidth(pString, pFont)) >> 1;
    sty = (GetMaxY() - GetTextHeight(pFont)) >> 1;

	while(!OutTextXY(stx, sty, pString));

}


/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
/* ------------------------------------------------------------------------------------- */
//  Message Callback Functions
/* ------------------------------------------------------------------------------------- */
/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */

/************************************************************************
 Function: WORD MsgTimeSettingsCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the Time settings screen
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgTimeSettingsCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
	WORD setStatus = 0, updateStrings = 0, changeObjID = 0;
    static signed char change;
    static signed char dayMax;

	static RF4CE_TICK currTick; 

    OBJ_HEADER *pObjTmp;

    switch(GetObjID(pObj))
    {

        case ID_ST_MONTH:
        case ID_ST_DAY:
        case ID_ST_YEAR:
	    case ID_ST_HOUR:
	    case ID_ST_MINUTE:
	    case ID_ST_SECOND:

            if (objMsg == EB_MSG_TOUCHSCREEN)
            {
                if (selection != GetObjID(pObj))
                {
                    // redraw current selection to unselect
                    if ((pObjTmp = GOLFindObject(selection)) != NULL)
                    {
                        GOLSetScheme(pObjTmp, pSettingsScheme2);
                        SetState(pObjTmp, DRAW); 
                    }
                    selection = GetObjID(pObj);
                    pObjTmp = GOLFindObject(selection);
                    GOLSetScheme(pObjTmp, pTimeSelectScheme);
                    SetState(pObjTmp, DRAW);    
                }
            
                // enable the up and down buttons if not yet enabled
                pObjTmp = GOLFindObject(ID_UP_BTN);
                if (GetState(pObjTmp, BTN_DISABLED) == BTN_DISABLED)
                {
                    ClrState(pObjTmp, BTN_DISABLED);    
                    SetState(pObjTmp, DRAW);    
                }
                pObjTmp = GOLFindObject(ID_DOWN_BTN);
                if (GetState(pObjTmp, BTN_DISABLED) == BTN_DISABLED)
                {
                    ClrState(pObjTmp, BTN_DISABLED);    
                    SetState(pObjTmp, DRAW);    
                }

            }
            break;

        case ID_UP_BTN:
        case ID_DOWN_BTN:
            if(objMsg == BTN_MSG_RELEASED)
            {
                if (GetObjID(pObj) == ID_DOWN_BTN)
                    change = 1;
                else  
                    change = -1;
                updateStrings = 1;
                changeObjID = selection;
            }
            break;

		case ID_RF_SETTINGS:	
            if(objMsg == BTN_MSG_RELEASED)
            {
                selection = 0;
	            setStatus = 1;
                screenState = CREATE_RF_SETTINGS;
            }
            break;

        case ID_MCHPICON:
            // this is the hidden flash programming launcher, the touch on the Microchip Icon
            // must be steady to launch the hidden flash programmer screen.
            if (objMsg == BTN_MSG_STILLPRESSED) 
            {
                // checks if the flash is to be programmed
                // checks for 3 seconds.
                currTick = RF4CE_TickGet();
                if(RF4CE_TickGetDiff(currTick, processingTick) > (ONE_SECOND * 3))
                { 
                    processingTick = currTick;
                    screenState = CREATE_PROGRAM_FLASH;
                }
            }
            else if ((objMsg == BTN_MSG_PRESSED) || (objMsg == BTN_MSG_RELEASED))                
            {
                processingTick = RF4CE_TickGet();
            }
         
            break;

		case ID_TIME_SETTINGS:	
            return 0;

        case ID_HOME:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON_PRESS_HOME);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON_HOME);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON_HOME);
                selection = ID_ST_MONTH;
	            setStatus = 1;
                screenState = CREATE_HOME;
            }
            break;

		default:
			break;
	}

	if (updateStrings)
	{
	
	    dayMax  = daysPerMonth[month-1];
	
	    // February has one day more for a leap year, unless it is on the thousands
	    if ((month == 2) && ((year % 4) == 0) && (year != 0))
	    {
	        dayMax ++;
	    }
	
	    // Change the appropriate setting.  Allow the controls to roll over.
        switch(changeObjID)
	    {
            case ID_ST_DAY:
	            day += change;
	            if (day < 1)
	            {
	                day = dayMax;
	            }
	            if (day > dayMax)
	            {
	                day = 1;
	            }
				NumberToString(day, SetDayStr, 2, 0);
				StSetText((STATICTEXT *)GOLFindObject(ID_ST_DAY), SetDayStr);
				SetState((GOLFindObject(ID_ST_DAY)), ST_DRAW );
	            setStatus = 1;
	            break;
	
            case ID_ST_MONTH:
	            month += change;
	            if (month < 1)
	            {
	                month = 12;
	            }
	            if (month > 12)
	            {
	                month = 1;
	            }
				StSetText((STATICTEXT *)GOLFindObject(ID_ST_MONTH), MoItems[month]);
				SetState((GOLFindObject(ID_ST_MONTH)), ST_DRAW );
	            setStatus = 1;
	            break;
	
            case ID_ST_YEAR:
	            year += change;
	            if (year < 0)
	            {
	                year = 99;
	            }
	            if (year > 99)
	            {
	                year = 0;
	            }
				NumberToString((2000+year), SetYearStr, 4, 0);
				StSetText((STATICTEXT *)GOLFindObject(ID_ST_YEAR), SetYearStr);
				SetState((GOLFindObject(ID_ST_YEAR)), ST_DRAW );
	            setStatus = 1;
	            break;
	
            case ID_ST_HOUR:
	            hour += change;
	            if (hour < 0)
	            {
	                hour = 23;
	            }
	            if (hour > 23)
	            {
	                hour = 0;
	            }
				NumberToString(hour, SetHourStr, 2, 0);
				StSetText((STATICTEXT *)GOLFindObject(ID_ST_HOUR), SetHourStr);
				SetState((GOLFindObject(ID_ST_HOUR)), ST_DRAW );
	            setStatus = 1;
	            break;
	
            case ID_ST_MINUTE:
	            minute += change;
	            if (minute < 0)
	            {
	                minute = 59;
	            }
	            if (minute > 59)
	            {
	                minute = 0;
	            }
				NumberToString(minute, SetMinuteStr, 2, 0);
				StSetText((STATICTEXT *)GOLFindObject(ID_ST_MINUTE), SetMinuteStr);
				SetState((GOLFindObject(ID_ST_MINUTE)), ST_DRAW );
	            setStatus = 1;
	            break;

            case ID_ST_SECOND:
	            second += change;
	            if (second < 0)
	            {
	                second = 59;
	            }
	            if (second > 59)
	            {
	                second = 0;
	            }
				NumberToString(second, SetSecondStr, 2, 0);
				StSetText((STATICTEXT *)GOLFindObject(ID_ST_SECOND), SetSecondStr);
				SetState((GOLFindObject(ID_ST_SECOND)), ST_DRAW );
	            setStatus = 1;
	            break;
	
	        default:
	            return 1;
	    }
	}
    changeObjID = 0;


	if (setStatus)
    {
        mRTCCOff();
        RTCCSetBinHour( hour );
        RTCCSetBinMin( minute );
        RTCCSetBinSec( second );
        RTCCSetBinMonth( month );
        RTCCSetBinYear( year );
        RTCCSetBinDay( day );
        RTCCCalculateWeekDay();
        RTCCSet();                      // Copy the new values to the RTCC registers
	    
	    UpdateTime(1);               	// Update the time on the display.	
	}    

	return 1;
}

/************************************************************************
 Function: WORD TimeSettingsDrawCallback(void)
 
 Overview: This function is called by GOLDraw() function when the state 
 		   is Time and Date settings.

 Input: none

 Output: If the function returns non-zero the draw control will be passed 
		to GOL. GOLDraw() can proceed and re-draw objects that needs 
		to be redrawn.
************************************************************************/
WORD TimeSettingsDrawCallback(void)
{
    WORD temp;

    temp  = mRTCCGetBinSec();
    if (temp != second)
    {
       	// Now update the date and time 
        RTCCProcessEvents();
       	UpdateDateTimeStrings();	        

        {
            second = temp;
    	    SetState((GOLFindObject(ID_ST_SECOND)), ST_DRAW );
    		StSetText((STATICTEXT *)GOLFindObject(ID_ST_SECOND), SetSecondStr);
        }

        temp = mRTCCGetBinMin();
        if (temp != minute)
        {
            minute = temp;
        	SetState((GOLFindObject(ID_ST_MINUTE)), ST_DRAW );
    		StSetText((STATICTEXT *)GOLFindObject(ID_ST_MINUTE), SetMinuteStr);
        }

        temp = mRTCCGetBinHour();
        if (temp != hour)
        {
            hour = temp;
        	SetState((GOLFindObject(ID_ST_HOUR)), ST_DRAW );
    		StSetText((STATICTEXT *)GOLFindObject(ID_ST_HOUR), SetHourStr);
        }

        temp = mRTCCGetBinDay();
        if (temp != day)
        {
            day = temp;
        	SetState((GOLFindObject(ID_ST_DAY)), ST_DRAW );
    		StSetText((STATICTEXT *)GOLFindObject(ID_ST_DAY), SetDayStr);
        }

        temp = mRTCCGetBinMonth();
        if (temp != month)
        {
            month = temp;
        	SetState((GOLFindObject(ID_ST_MONTH)), ST_DRAW );
    		StSetText((STATICTEXT *)GOLFindObject(ID_ST_MONTH), SetMonthStr);
        }

        temp = mRTCCGetBinYear();
        if (temp != year)
        {
            year = temp;
        	SetState((GOLFindObject(ID_ST_YEAR)), ST_DRAW );
    		StSetText((STATICTEXT *)GOLFindObject(ID_ST_YEAR), SetYearStr);
        }
    }

    return 1;
}

/************************************************************************
 Function: WORD MsgRFSettingsCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the RF settings screen
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgRFSettingsCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    BYTE index;
	
    switch(GetObjID(pObj))
    {

		case ID_PAIR_DEVICE:	
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = RF_PAIRING_MODE;
            }
            break;

		case ID_ERASE_PAIR_TABLE:	
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_MANAGE_PAIR_TABLE_MODE;
            }
            break;

		case ID_RF_SETTINGS:	
            // do nothing since we already in the RF Settings Screen
            break;

		case ID_TIME_SETTINGS:	
            if(objMsg == BTN_MSG_RELEASED)
            {
                // change the screen to show set time screen
                screenState = CREATE_TIME_SETTINGS;
            }
            break;

        case ID_HOME:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON_PRESS_HOME);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON_HOME);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON_HOME);
                screenState = CREATE_HOME;
            }
            break;
        
        case ID_PAIR_LIST:
            if (objMsg == LB_MSG_TOUCHSCREEN)
            {
                // filter the touch actions and only select active entries
                if((pMsg->uiEvent == EVENT_MOVE) || (pMsg->uiEvent == EVENT_PRESS))
                {
                    // now convert the message since EVENT_MOVE is not processed in ListBox.
                    pMsg->uiEvent = EVENT_PRESS;      

                    // call the message default directly here so we can control
                    // the rendering of the listbox.
                    LbMsgDefault(objMsg, pObj, pMsg);

                    index = LbGetFocusedItem((LISTBOX*)pObj);
    
                    // check if the index is a paired entry or is already set as active
                    if((PairTable[index].NodeCap.Val == 0xFF) || (PairTable[index].NodeCap.Val == 0) || (RF4CEControllerGetActiveDevice() == index))
                    {
                        // it is not paired so reset all rendering items
                        ClrState(pObj, LB_DRAW | LB_DRAW_FOCUS | LB_DRAW_ITEMS);
                    }
                    else
                    {      
                        // now set the entry that can be selected as the active device
                        RF4CEControllerSetActiveDevice(index);
                    }

                    // return 0 since we already processed the listbox
                    return 0;
                }
            }
            break;

		default:
			break;
	}

	return 1;
}

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
WORD MsgProgramExternalFlashCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_PGM_YES:
            if(objMsg == BTN_MSG_RELEASED)
            {
                // remove the buttons since they will not respond to touches anymore
                SetState(GOLFindObject(ID_PGM_YES), BTN_HIDE);
                SetState(GOLFindObject(ID_PGM_NO), BTN_HIDE);
                screenState = DISPLAY_PROGRAM_FLASH_ACTUAL;
            }
            break;
        case ID_PGM_NO:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_TIME_SETTINGS;
            }
            break;
    }
    return 1;
}

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
WORD MsgManagePairTableCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
#ifdef ENABLE_PAIR_ENTRY_DELETION
   BYTE index;
   BYTE response;
#endif

    switch(GetObjID(pObj))
    {
        case ID_ERASE_TABLE:
            if(objMsg == BTN_MSG_RELEASED)
            {
                RF4CE_Start(FALSE);

                UpdatePairTableLEDStatus();

                ShowMessage("Pairing Table Erased", pMenuButtonScheme->pFont);
                DelayMs(1000);
                screenState = CREATE_RF_SETTINGS;
            }
            break;
#ifdef ENABLE_PAIR_ENTRY_DELETION
        case ID_ERASE_ENTRY:
            if(objMsg == BTN_MSG_RELEASED)
            {
                if ((response = RF4CE_UnPair(RF4CEControllerGetActiveDevice())) != SUCCESS)
                {
                    ShowMessage("Pair Entry Erase Failed", pMenuButtonScheme->pFont);
                }
                else
                {
                    ShowMessage("Pair Entry Erased", pMenuButtonScheme->pFont);
                }

                // if the table still has a valid entry, then set
                // the first one as active else set the active to be none.
                if (RF4CEControllerCheckPairTable(&index) != 0)
                    UpdatePairTableLEDStatus();
                    RF4CEControllerSetActiveDevice(index);
                else
                {
                    // since nothing is paired, initialize to no pairing to reset everything
                    RF4CE_Start(FALSE);
                    RF4CEControllerSetActiveDevice(0xFF);
                }
                DelayMs(1000);
                screenState = CREATE_RF_SETTINGS;
            }
            break;

        case ID_PAIR_LIST:
            if (objMsg == LB_MSG_TOUCHSCREEN)
            {
                // filter the touch actions and only select active entries
                if((pMsg->uiEvent == EVENT_MOVE) || (pMsg->uiEvent == EVENT_PRESS))
                {
                    // now convert the message since EVENT_MOVE is not processed in ListBox.
                    pMsg->uiEvent = EVENT_PRESS;      

                    // call the message default directly here so we can control
                    // the rendering of the listbox.
                    LbMsgDefault(objMsg, pObj, pMsg);

                    index = LbGetFocusedItem((LISTBOX*)pObj);
    
                    // check if the index is a paired entry or is already set as active
                    if((PairTable[index].NodeCap.Val == 0xFF) || (PairTable[index].NodeCap.Val == 0) || (RF4CEControllerGetActiveDevice() == index))
                    {
                        // it is not paired so reset all rendering items
                        ClrState(pObj, LB_DRAW | LB_DRAW_FOCUS | LB_DRAW_ITEMS);
                    }
                    else
                    {      
                        // now set the entry that can be selected as the active device
                        RF4CEControllerSetActiveDevice(index);
                    }

                    // return 0 since we already processed the listbox
                    return 0;
                }
            }
            break;
#else
        case ID_PAIR_LIST:
            // ignore all messages to the list box.
            if (objMsg == LB_MSG_TOUCHSCREEN)
            {
                return 0;
            }
            break;

#endif
        case ID_ERASE_CANCEL:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_RF_SETTINGS;
            }
            break;
        
    }
    return 1;
}

/************************************************************************
 Function: UpdateDateTimeStrings(void)
                                                                       
 Overview: Forces the update of the time string variables.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void UpdateDateTimeStrings(void)
{
	strncpy(SetMonthStr, _date_str,3);
	SetMonthStr[3] = 0;
	strncpy(SetDayStr, &_date_str[4],2);
	SetDayStr[2] = 0;
	strncpy(SetYearStr, &_date_str[8],4);
	SetYearStr[4] = 0;

	strncpy(SetHourStr, &_time_str[4],2);
	SetHourStr[2] = 0;
	strncpy(SetMinuteStr, &_time_str[7],2);
	SetMinuteStr[2] = 0;
	strncpy(SetSecondStr, &_time_str[10],2);
	SetSecondStr[2] = 0;

}	

/************************************************************************
 Function: RF4CEDiscoveryCallBack(void)
                                                                       
 Overview: Callback function to show that RF4CE discovery is in process.
           This will show some rotating filled circle on the screen
           to indicate that hardware is busy.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void RF4CEDiscoveryCallBack(void)
{
	RF4CE_TICK currTick = RF4CE_TickGet();

    if(RF4CE_TickGetDiff(currTick, processingTick) > (ONE_SECOND / 4))
    { 
        SetColor(REMOTE_BLACK);
	    while(!FillCircle(positions[processing].xPos,positions[processing].yPos,STATUSRADIUS));

        processing++;
        if(processing >= NUM_PROCESSING_POS)
            processing = 0;
    
    	SetColor(REMOTE_YELLOW);
    	while(!FillCircle(positions[processing].xPos,positions[processing].yPos,STATUSRADIUS));

        processingTick = currTick;
    }
}


