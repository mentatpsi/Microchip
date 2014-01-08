/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        DemoSelection.c
 * Dependencies:    MainDemo.h, DemoSelection.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software LicenseJpeg
 * Agreement
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok
 * Paolo Tamayo         03/10/08    ...
 * Microchip			04/14/10    added macro for tick delay measurement
 *****************************************************************************/
#include "MainDemo.h"

#ifndef MEB_BOARD
#define ENABLE_PICTURES_DEMO
#endif

// images used
#define ICON1P  RGBLighting_mono_72x72
#define ICON1   RGBLighting_4bpp_72x72
#define ICON2P  Language_mono_72x72
#define ICON2   Language_4bpp_72x72
#define ICON3P  PCGaming2_mono_72x72
#define ICON3   PCGaming2_4bpp_72x72
#define ICON4P  Animation_mono_72x72
#define ICON4   Animation_4bpp_72x72
#define ICON5P  DrawingScreen_mono_72x72
#define ICON5   DrawingScreen_4bpp_72x72
#define ICON6P  ECG_mono_72x72
#define ICON6   ECG_4bpp_72x72
#define ICON7P  DateAndTime_mono_72x72
#define ICON7   DateAndTime_4bpp_72x72
#define ICON8P  FolderPhoto_mono_72x72
#define ICON8   FolderPhoto_4bpp_72x72
#define ICON9P  FolderPhoto_mono_72x72
#define ICON9   FolderPhoto_4bpp_72x72
#ifdef ENABLE_USB_MSD_DEMO
    #define ICON10P FolderPhoto_mono_72x72
    #define ICON10  FolderPhoto_4bpp_72x72
#endif
#ifdef ENABLE_SD_MSD_DEMO
    #define ICON11P FolderPhoto_mono_72x72
    #define ICON11  FolderPhoto_4bpp_72x72
#endif

// strings used
extern XCHAR                LeftArrowStr[];
extern XCHAR                RightArrowStr[];
extern XCHAR                UpArrowStr[];
extern XCHAR                DownArrowStr[];
extern XCHAR                DemoStr[];

extern BYTE                 usbMSDStatus;
BYTE                        sdCardStatus;

#ifdef ENABLE_DEMO_MODE

// demo mode
extern BOOL     gEnableDemoFlag;
extern WORD     demoCount;
#endif

// local function prototypes
void            CreateDemoSelectionPage0(void);
void            CreateDemoSelectionPage1(void);

// global tick counter
extern DWORD    tick;
DWORD           demoTick;

// current page number
char            page = 0;

// last page number
#define DEMOSEL_LASTPAGE        1
#define WAIT_UNTIL_FINISH(x)    while(!x)

#define SELECTDELAY 	100000 // used to remove the focus on objects when not touched for a long time.

    /************************************************************************
 Function: CreateDemoSelection()
                                                                       
 Overview: Creates the menu of small demos each represented by an icon.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
    void CreateDemoSelection(void)
{
    static char prevPage = 0;

    SetColor(SCREEN_BACKGROUND_COLOR);
    ClearDevice();

    prevPage = page;
    switch(page)
    {
        case 0:     CreateDemoSelectionPage0(); break;
        case 1:     CreateDemoSelectionPage1(); break;
        default:    page = 0; CreateDemoSelectionPage0();
    }
}

const XCHAR iconLightingStr[] = {'L','i','g','h','t','i','n','g',0};
const XCHAR iconLanguageStr[] = {'L','a','n','g','u','a','g','e',0};
const XCHAR iconGameStr[] = {'G','a','m','e',0};
const XCHAR iconAnimationStr[] = {'A','n','i','m','a','t','i','o','n',0};
const XCHAR iconDrawingStr[] = {'D','r','a','w','i','n','g',0};
const XCHAR iconECGStr[] = {'E','c','g',0};
const XCHAR iconDateTimeStr[] = {'C','l','o','c','k',0};
const XCHAR iconPixFolderStr[] = {'E','x','t','e','r','n','a','l',' ','F','l','a','s','h',0};
const XCHAR iconPix2FolderStr[] = {'G','r','a','p','h',' ','D','e','m','o',0};
const XCHAR iconPix3FolderStr[] = {'T','h','u','m','b',' ','D','r','i','v','e',0};
#ifdef ENABLE_SD_MSD_DEMO
const XCHAR iconPix4FolderStr[] = {'S','D',' ','C','a','r','d',' ','D','e','m','o',0};
#endif
#define ICONBTNINITSTATE    (BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL)

/************************************************************************
 Function: CreateDemoSelectionPage0()
                                                                       
 Overview: Creates page 0 of the menu of small demos each represented 
 		   by an icon.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateDemoSelectionPage0(void)
{
    GOLFree();                      // free memory for the objects in the previous linked list and start new list

    // USB HID or Mood Lighting demo icon
    BtnCreate
    (
        ID_BUTTON1,                 // button ID
        IconLeft(0),                // button dimensions
        IconTop(0),
        IconRight(0),
        IconBottom(0),
        0,                          // set no radius
        ICONBTNINITSTATE,
        (void *) &ICON1,            // bitmap that overlaps the button
        (XCHAR *)iconLightingStr,   // icon label
        iconScheme
    );                              // use alternate scheme

    // Language demo icon
    BtnCreate
    (
        ID_BUTTON2,
        IconLeft(1),
        IconTop(0),
        IconRight(1),
        IconBottom(0),
        0,
        ICONBTNINITSTATE,
        (void *) &ICON2,
        (XCHAR *)iconLanguageStr,
        iconScheme
    );

    // Game demo icon
    BtnCreate
    (
        ID_BUTTON3,
        IconLeft(2),
        IconTop(0),
        IconRight(2),
        IconBottom(0),
        0,
        ICONBTNINITSTATE,
        (void *) &ICON3,
        (XCHAR *)iconGameStr,
        iconScheme
    );

    // Animation demo icon
    BtnCreate
    (
        ID_BUTTON4,
        IconLeft(0),
        IconTop(1),
        IconRight(0),
        IconBottom(1),
        0,
        ICONBTNINITSTATE,
        (void *) &ICON4,
        (XCHAR *)iconAnimationStr,
        iconScheme
    );

    // Drawing demo icon
    BtnCreate
    (
        ID_BUTTON5,
        IconLeft(1),
        IconTop(1),
        IconRight(1),
        IconBottom(1),
        0,
        ICONBTNINITSTATE,
        (void *) &ICON5,
        (XCHAR *)iconDrawingStr,
        iconScheme
    );

    // ECG demo icon
    BtnCreate
    (
        ID_BUTTON6,
        IconLeft(2),
        IconTop(1),
        IconRight(2),
        IconBottom(1),
        0,
        ICONBTNINITSTATE,
        (void *) &ICON6,
        (XCHAR *)iconECGStr,
        iconScheme
    );

    #ifdef ENABLE_DEMO_MODE
    CreateCtrlButtons(DemoStr, NULL, LeftArrowStr, RightArrowStr);
    if(gEnableDemoFlag == TRUE)
        SetState((BUTTON *)GOLFindObject(ID_BUTTON_A), BTN_DRAW | BTN_TOGGLE | BTN_PRESSED);
    else
        SetState((BUTTON *)GOLFindObject(ID_BUTTON_A), BTN_DRAW | BTN_TOGGLE);
    #else
    CreateCtrlButtons(LeftArrowStr, RightArrowStr, UpArrowStr, DownArrowStr);
    #endif
}

/************************************************************************
 Function: CreateDemoSelectionPage1()
                                                                       
 Overview: Creates page 1 of the menu of small demos each represented 
 		   by an icon.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateDemoSelectionPage1(void)
{
    GOLFree();                      // free memory for the objects in the previous linked list and start new list

    // Date and Time demo icon
    BtnCreate
    (
        ID_BUTTON7,                 // button ID
        IconLeft(0),                // dimension of the button
        IconTop(0),
        IconRight(0),
        IconBottom(0),
        0,                          // no radius
        ICONBTNINITSTATE,
        (void *) &ICON7,            // bitmap which overlaps the button
        (XCHAR *)iconDateTimeStr,   // icon label
        iconScheme
    );                              // use alternate scheme

    // Graph Show demo icon
    BtnCreate
    (
        ID_BUTTON8,                 // button ID
        IconLeft(1),                // dimension of the button
        IconTop(0),
        IconRight(1),
        IconBottom(0),
        0,                          // no radius
        ICONBTNINITSTATE,
        (void *) &ICON8,            // bitmap which overlaps the button
        (XCHAR *)iconPix2FolderStr, // icon label
        iconScheme
    );                              // use alternate scheme
    #ifdef ENABLE_PICTURES_DEMO
    // External Flash Memory demo icon
    BtnCreate
    (
        ID_BUTTON9,
        IconLeft(2),
        IconTop(0),
        IconRight(2),
        IconBottom(0),
        0,
        ICONBTNINITSTATE,
        (void *) &ICON9,
        (XCHAR *)iconPixFolderStr,
        iconScheme
    );
    #endif
    #ifdef ENABLE_USB_MSD_DEMO

    // USB MSD demo icon
    BtnCreate
    (
        ID_BUTTON10,
        IconLeft(0),
        IconTop(1),
        IconRight(0),
        IconBottom(1),
        0,
        ICONBTNINITSTATE,
        (void *) &ICON10,
        (XCHAR *)iconPix3FolderStr,
        iconScheme
    );
    #endif
    #ifdef ENABLE_SD_MSD_DEMO

    // USB MSD demo icon
    BtnCreate
    (
        ID_BUTTON11,
        IconLeft(1),
        IconTop(1),
        IconRight(1),
        IconBottom(1),
        0,
        ICONBTNINITSTATE,
        (void *) &ICON11,
        (XCHAR *)iconPix4FolderStr,
        iconScheme
    );
    #endif
    #ifdef ENABLE_DEMO_MODE
    CreateCtrlButtons(DemoStr, NULL, LeftArrowStr, RightArrowStr);
    if(gEnableDemoFlag == TRUE)
        SetState((BUTTON *)GOLFindObject(ID_BUTTON_A), BTN_DRAW | BTN_TOGGLE | BTN_PRESSED);
    else
        SetState((BUTTON *)GOLFindObject(ID_BUTTON_A), BTN_DRAW | BTN_TOGGLE);
    #else
    CreateCtrlButtons(LeftArrowStr, RightArrowStr, UpArrowStr, DownArrowStr);
    #endif
}

/************************************************************************
 Function: CheckDemoStatus()
                                                                       
 Overview: Checks the hardware components of a demo. If hardware is 
 		   not present demo icon will be disabled.
 		                                                                          
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CheckDemoStatus(void)
{
    #if defined(ENABLE_USB_MSD_DEMO) | defined(ENABLE_SD_MSD_DEMO)

    BUTTON      *pBtn;
    #endif
    #ifdef USE_FOCUS
    OBJ_HEADER  *pObj;
    #endif
    #ifdef USE_FOCUS

    // this removes the focus rectangle on an object that is selected.
    // this is only enabled when DISPLAY_DEMOSELECTION is on
    pObj = GOLGetFocus();
    if(pObj != NULL)
    {
        if(tick - demoTick > SELECTDELAY)
        {

            // reset the focus of the currenty focused
            ClrState(pObj, FOCUSED);
            SetState(pObj, DRAW_FOCUS);

            // get the global pointer to the focused object and reset to NULL        		
            GOLGetFocus() = NULL;
        }
    }
    else
        demoTick = tick;
    #endif // #ifdef USE_FOCUS   	
    #ifdef ENABLE_USB_MSD_DEMO
    if(page == 1)
    {                           // check if we are in the correct page
        MonitorDriveMedia();    // update the USB MSD media status
        usbMSDStatus = USBHostMSDSCSIMediaDetect();     // update the status flag
        pBtn = (BUTTON *)GOLFindObject(ID_BUTTON10);    // get the pointer to the icon
        if(usbMSDStatus == TRUE)
        {       // check if the media is attached
            if(GetState(pBtn, BTN_DISABLED) == BTN_DISABLED)
            {   // enable the button/icon so user can select
                BtnSetBitmap(pBtn, (void *) &ICON10);
                ClrState(pBtn, BTN_DISABLED);
                SetState(pBtn, BTN_DRAW);   // redraw the button to update the state and icon used
            }
        }
        else if(usbMSDStatus == FALSE)
        {       // check if media is not attached
            if(GetState(pBtn, BTN_DISABLED) != BTN_DISABLED)
            {   // disable the button/icon, user cannot select
                BtnSetBitmap(pBtn, (void *) &ICON10P);
                SetState(pBtn, BTN_DISABLED | BTN_DRAW);    // redraw the button to update the state and icon used
            }
        }
    }

    #endif
    #ifdef ENABLE_SD_MSD_DEMO
    if(page == 1)
    {                           // check if we are in the correct page
        MonitorDriveMedia();    // update the SD MSD media status
        sdCardStatus = MDD_MediaDetect();

        pBtn = (BUTTON *)GOLFindObject(ID_BUTTON11);    // get the pointer to the icon
        if(sdCardStatus == TRUE)
        {       // check if the media is attached
            if(GetState(pBtn, BTN_DISABLED) == BTN_DISABLED)
            {   // enable the button/icon so user can select
                BtnSetBitmap(pBtn, (void *) &ICON11);
                ClrState(pBtn, BTN_DISABLED);
                SetState(pBtn, BTN_DRAW);   // redraw the button to update the state and icon used
            }
        }
        else if(sdCardStatus == FALSE)
        {       // check if media is not attached
            if(GetState(pBtn, BTN_DISABLED) != BTN_DISABLED)
            {   // disable the button/icon, user cannot select
                BtnSetBitmap(pBtn, (void *) &ICON11P);
                SetState(pBtn, BTN_DISABLED | BTN_DRAW);    // redraw the button to update the state and icon used
            }
        }
    }

    #endif
}

/************************************************************************
 Function: WORD MsgDemoSelection(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the demo selection. Based on user selection
 		   A demo is enabled by switching the screen state. Icons used
 		   changes from colored version to monochrome version when pressed. 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgDemoSelection(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_BUTTON1:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON1P);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON1);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON1);
                screenState = CREATE_RGB;
            }

            break;

        case ID_BUTTON2:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON2P);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON2);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON2);
                screenState = CREATE_MULTIFONT;
            }

            break;

        case ID_BUTTON3:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON3P);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON3);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON3);
                screenState = CREATE_SNAKE;
            }

            break;

        case ID_BUTTON4:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON4P);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON4);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON4);
                screenState = CREATE_ANIMATION;
            }
            
            break;
            
        case ID_BUTTON5:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON5P);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON5);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON5);
                screenState = CREATE_SCALE;
            }
            break;

        case ID_BUTTON6:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON6P);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON6);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON6);
                screenState = CREATE_ECG;
            }

            break;

        case ID_BUTTON7:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON7P);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON7);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON7);
                screenState = CREATE_TIMEDATE;
            }

            break;

        case ID_BUTTON8:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON8P);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON8);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON8);
                screenState = CREATE_GRAPHSHOW;
            }

            break;

            #ifdef ENABLE_PICTURES_DEMO

        case ID_BUTTON9:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON9P);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON9);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON9);
                screenState = CREATE_SLIDESHOW;
            }

            break;
            #endif
            #ifdef ENABLE_USB_MSD_DEMO

        case ID_BUTTON10:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON10P);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON10);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON10);
                screenState = CREATE_JPEGDEMO;
            }

            break;
            #endif
            #ifdef ENABLE_SD_MSD_DEMO

        case ID_BUTTON11:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON11P);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON11);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON11);
                screenState = CREATE_SDCARDDEMO;
            }

            break;
            #endif
            #ifdef ENABLE_DEMO_MODE

        case ID_BUTTON_A:
            if((objMsg == BTN_MSG_PRESSED) && (!GetState(pObj, BTN_PRESSED)))
            {
                gEnableDemoFlag = TRUE;
                if(page == 0)
                    demoCount = 3;
                else
                    demoCount = 0;
            }
            else
            {
                gEnableDemoFlag = FALSE;
            }

            BtnMsgDefault(objMsg, (BUTTON *)pObj, pMsg);
            break;

            //return 0;
            #else

        case ID_BUTTON_A:
            #endif

        case ID_BUTTON_C:
            if(objMsg == BTN_MSG_RELEASED)
            {
                page--;
                if(page < 0)
                    page = DEMOSEL_LASTPAGE;
                screenState = CREATE_DEMOSELECTION;
            }

            break;

            #ifndef ENABLE_DEMO_MODE

        case ID_BUTTON_B:
            #endif

        case ID_BUTTON_D:
            if(objMsg == BTN_MSG_RELEASED)
            {
                page++;
                if(page > DEMOSEL_LASTPAGE)
                    page = 0;
                screenState = CREATE_DEMOSELECTION;
            }

            break;

        default:
            return (1); // process by default
    }

    #ifdef USE_FOCUS

    // if a button object changed in state reset the focus timeout counter.
    if((objMsg == BTN_MSG_RELEASED) || (objMsg == BTN_MSG_PRESSED) || (objMsg == BTN_MSG_CANCELPRESS))
    {
        demoTick = tick;
    }

    #endif // #ifdef USE_FOCUS   	
    return (1);
}

/************************************************************************
 Function: void InitDemoSelectStyleScheme(GOL_SCHEME *pScheme)
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void InitDemoSelectStyleScheme(GOL_SCHEME *pScheme)
{
    pScheme->EmbossDkColor = SCREEN_BACKGROUND_COLOR;
    pScheme->EmbossLtColor = SCREEN_BACKGROUND_COLOR;
    pScheme->pFont = (void *) &GOLSmallFont;
    pScheme->TextColor0 = BLACK;
    pScheme->ColorDisabled = SCREEN_BACKGROUND_COLOR;
    pScheme->Color0 = SCREEN_BACKGROUND_COLOR;
    pScheme->Color1 = SCREEN_BACKGROUND_COLOR;
}
