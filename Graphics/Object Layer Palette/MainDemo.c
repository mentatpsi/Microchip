/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        MainDemo.c
 * Dependencies:    MainDemo.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright (c) 2008 Microchip Technology Inc.  All rights reserved.
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 30/05/07	...
 * 09/20/07    Round buttons demo screen is added
 * 10/05/07    Modified navigation, added Meter, Dial, 
 *	    	   RTCC demos
 * 01/21/08    Updated for v1.1
 * 02/07/08    PIC32 support 
 * 06/25/09    dsPIC & PIC24H support 
 * 06/29/09	   Added demo that shows buttons with 
 *			   multi-line text. See CreateCheckBoxes() demo.
 *****************************************************************************/
#include "MainDemo.h"
#include "MainDemoStrings.h"
#include "TimeDelay.h" 

// Configuration bits
#if defined (__PIC24FJ256DA210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_EC & WUTSEL_LEG & ALTPMP_ALTPMPEN & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
#elif defined(__PIC32MX__)
    #pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF, FCKSM = CSECME, FPBDIV = DIV_1
    #pragma config OSCIOFNC = ON, POSCMOD = XT, FSOSCEN = ON, FNOSC = PRIPLL
    #pragma config CP = OFF, BWP = OFF, PWP = OFF
#endif	

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT'S IDs
/////////////////////////////////////////////////////////////////////////////
#define ID_WINDOW1          10

#define ID_BUTTON1          11
#define ID_BUTTON2          12
#define ID_BUTTON3          13
#define ID_BUTTON4          14
#define ID_BUTTON5          15
#define ID_BUTTON6          16
#define ID_BUTTON7          17

#define ID_BUTTON_BACK      18
#define ID_BUTTON_NEXT      19

#define ID_CHECKBOX1        20
#define ID_CHECKBOX2        21
#define ID_CHECKBOX3        22
#define ID_CHECKBOX4        23
#define ID_CHECKBOX5        24

#define ID_RADIOBUTTON1     30
#define ID_RADIOBUTTON2     31
#define ID_RADIOBUTTON3     32
#define ID_RADIOBUTTON4     33
#define ID_RADIOBUTTON5     34
#define ID_RADIOBUTTON6     35
#define ID_RADIOBUTTON7     36
#define ID_RADIOBUTTON8     37

#define ID_SLIDER1          40
#define ID_SLIDER2          41
#define ID_SLIDER3          42

#define ID_GROUPBOX1        50
#define ID_GROUPBOX2        51
#define ID_STATICTEXT1      52
#define ID_STATICTEXT2      53
#define ID_STATICTEXT3      54

#define ID_PICTURE1         55

#define ID_PROGRESSBAR1     60
#define ID_CUSTOM1          70

#define ID_LISTBOX1         80
#define ID_LISTBOX2         81
#define ID_LISTBOX3         82
#define ID_EDITBOX1         83

#define ID_CALL             91
#define ID_STOPCALL         92
#define ID_HOLD             93
#define ID_BACKSPACE        94
#define ID_POUND            95
#define ID_ASTERISK         96
#define ID_KEYPAD           100                         /* uses 100 to 110 for phone key pad demo */

#define ID_ROUNDDIAL        120
#define ID_METER1           130
#define ID_METER2           131
#define ID_DIGITALMETER1    132
#define ID_DIGITALMETER2    133

#define ID_BUTTONHR_P       150
#define ID_BUTTONHR_M       151
#define ID_BUTTONMN_P       152
#define ID_BUTTONMN_M       153
#define ID_BUTTONSC_P       154
#define ID_BUTTONSC_M       155

#define ID_BUTTON_DATE_UP   180
#define ID_BUTTON_DATE_DN   181

#define ID_BUTTON_MO        186
#define ID_BUTTON_DY        187
#define ID_BUTTON_YR        188
#define ID_BUTTON_RET       189

#define ID_EB_MONTH         200
#define ID_EB_DAY           201
#define ID_EB_YEAR          202

#define ID_EB_HOUR          205
#define ID_EB_MINUTE        206
#define ID_EB_SECOND        207

#define WAIT_UNTIL_FINISH(x)    while(!x)

    /////////////////////////////////////////////////////////////////////////////
    //                            LOCAL PROTOTYPES
    /////////////////////////////////////////////////////////////////////////////
void                        StartScreen();                                              // draws intro screen
void                        CreatePage(XCHAR *pText);                                   // creates the navigation buttons and window for each screen
void                        CreateButtons(void);                                        // creates buttons demo screen
WORD                        MsgButtons(WORD objMsg, OBJ_HEADER *pObj);                  // processes messages for buttons demo screen
void                        CreateRoundButtons(void);                                   // creates rounded buttons demo screen
WORD                        MsgRoundButtons(WORD objMsg, OBJ_HEADER *pObj);             // processes messages for rounded buttons demo screen
void                        CreateCheckBoxes(void);                                     // creates checkboxes demo screen
WORD                        MsgCheckBoxes(WORD objMsg, OBJ_HEADER *pObj);               // processes messages for checkboxes demo screen
void                        CreateRadioButtons(void);                                   // creates radio buttons demo screen
WORD                        MsgRadioButtons(WORD objMsg, OBJ_HEADER *pObj);             // processes messages for radio buttons demo screen
void                        CreateStaticText(void);                                     // creates static text demo screen
WORD                        MsgStaticText(WORD objMsg, OBJ_HEADER *pObj);               // processes messages for static text demo screen
void                        CreatePicture(void);                                        // creates picture demo screen
WORD                        MsgPicture(WORD objMsg, OBJ_HEADER *pObj);                  // processes messages for picture demo screen
void                        CreateSlider(void);                                         // creates slider demo screen
void                        DrawSliderCursor(WORD color, SHORT x, SHORT y);             // draws cursor for slider demo screen
WORD                        MsgSlider(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);    // processes messages for slider demo screen
void                        CreateProgressBar(void);                                    // creates progress bar demo screen
WORD                        MsgProgressBar(WORD objMsg, OBJ_HEADER *pObj);              //  processes messages for progress bar demo screen
void                        CreateSignature(void);                                      // creates signature demo screen
WORD                        PanelSignature(void);                                       // draws box for the signature
WORD                        MsgSignature(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg); // processes messages for signature demo screen
void                        CreatePotentiometer(void);                                  // scrolling graph demo screen
WORD                        PanelPotentiometer(void);                                   // draws box for the scrolling graph
void                        GraphPotentiometer(void);                                   // draws scrolling graph graph
WORD                        GetPotSamples(WORD number);                                 // adds sample from ADC potentiometer channel into temporary buffer

// returns zero if samples number in the buffer is less than defined by parameter
WORD                        MsgPotentiometer(WORD objMsg, OBJ_HEADER *pObj);            // processes messages for ECG demo screen
void                        CreateECG(void);                                            // creates ECG demo screen
WORD                        PanelECG(void);                                             // draws box for the ECG
void                        GraphECG(void);                                             // draws ECG graph
WORD                        GetECGSamples(WORD number);                                 // adds sample from FLASH (can be any source) into temporary buffer

// returns zero if samples number in the buffer is less than defined by parameter
WORD                        MsgECG(WORD objMsg, OBJ_HEADER *pObj);                      // processes messages for ECG demo screen
void                        CreateCustomControl(void);                                  // creates custom control demo screen
WORD                        MsgCustomControl(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg); // processes messages for custom control demo screen
void                        CreateListBox(void);                                        // creates list box demo screen
WORD                        MsgListBox(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);   // processes messages for list box demo screen
void                        CreateEditBox(void);                                        // creates edit box demo screen
WORD                        MsgEditBox(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);   // processes messages for edit box demo screen
void                        CreateDateTime(void);                                       // creates date time demo screen
WORD                        MsgDateTime(WORD objMsg, OBJ_HEADER *pObj);                 // processes messages for date and time demo screen
void                        UpdateRTCCDates(LISTBOX *pLb);                              // update the date edit boxes
WORD                        MsgSetDate(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);   // processes messages for setting dates
void                        ShowPullDownMenu(void);                                     // create objects for pull down menu and draw
WORD                        RemovePullDownMenu(void);                                   // removes pop out pull down menu from screen
void                        updateDateTimeEb(void);                                     // update edit boxes with current date and time values
void                        CreateMeter(void);                                          // creates meter demo screen
void                        UpdateMeter(void);
WORD                        MsgMeter(WORD objMsg, OBJ_HEADER *pObj);                    // processes messages for meter demo screen
void                        CreateDial(void);                                           // creates dial demo screen
WORD                        MsgDial(WORD objMsg, OBJ_HEADER *pObj);                     // processes messages for dial demo screen
void                        ErrorTrap(XCHAR *message);                                  // outputs text message on screen and stop execution
void                        TickInit(void);                                             // starts tick counter
void                        CreatePullDown(void);
WORD                        MsgPullDown(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);  // processes screen pull down menu
void                        DisplayPullDown(void);                                      // refresh the screen when switching screen using pulldown menu
void                        InitializeBoard(void);                                      // Initializes the hardware components including the PIC device


/////////////////////////////////////////////////////////////////////////////

//                            IMAGES USED
/////////////////////////////////////////////////////////////////////////////
// internal flash image
extern const IMAGE_FLASH   intro;
extern const IMAGE_FLASH   mchpLogo;
extern const IMAGE_FLASH   bulbon;
extern const IMAGE_FLASH   bulboff;
extern const IMAGE_FLASH   gradientButton;
extern const IMAGE_FLASH   Engine1;
extern const IMAGE_FLASH   Engine2;
extern const IMAGE_FLASH   Engine3;
extern const IMAGE_FLASH   Engine4;
extern const IMAGE_FLASH   arrowUp;
extern const IMAGE_FLASH   arrowDown;
extern const IMAGE_FLASH   redphone;
extern const IMAGE_FLASH   greenphone;
extern const IMAGE_FLASH   mchpIcon0;
extern const IMAGE_FLASH   mchpIcon;

extern const PALETTE_FLASH _GOL_Palette_Demo;

/////////////////////////////////////////////////////////////////////////////
//                             FONTS USED
/////////////////////////////////////////////////////////////////////////////
extern const FONT_FLASH     GOLFontDefault;                 // default GOL font
extern const FONT_FLASH     GOLMediumFont;                  // medium font
extern const FONT_FLASH     GOLSmallFont;                   // small font
extern const FONT_FLASH     monofont;                       // equal width font
#if defined (__PIC24FJ256DA210__) && defined (USE_FONT_RAM)
extern FONT_RAM	AltGOLFontDefault;
#endif

/////////////////////////////////////////////////////////////////////////////
//                            DEMO STATES
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CREATE_BUTTONS          = 0,
    DISPLAY_BUTTONS,
    CREATE_CHECKBOXES,
    DISPLAY_CHECKBOXES,
    CREATE_RADIOBUTTONS,
    DISPLAY_RADIOBUTTONS,
    CREATE_STATICTEXT,
    DISPLAY_STATICTEXT,
    CREATE_PICTURE,
    DISPLAY_PICTURE,
    CREATE_SLIDER,
    DISPLAY_SLIDER,
    CURSOR_DRAW_SLIDER,
    CREATE_PROGRESSBAR,
    DISPLAY_PROGRESSBAR,
    CREATE_LISTBOX,
    DISPLAY_LISTBOX,
    CREATE_EDITBOX,
    DISPLAY_EDITBOX,
    CREATE_METER,
    DISPLAY_METER,
    CREATE_DIAL,
    DISPLAY_DIAL,
    CREATE_CUSTOMCONTROL,
    DISPLAY_CUSTOMCONTROL,
    CREATE_SIGNATURE,
    DISPLAY_SIGNATURE,
    BOX_DRAW_SIGNATURE,
    CREATE_POT,
    DISPLAY_POT,
    BOX_DRAW_POT,
    CREATE_ECG,
    DISPLAY_ECG,
    BOX_DRAW_ECG,
    CREATE_PULLDOWN,
    DISPLAY_PULLDOWN,

    // these states are for time and date settings 0xF3xx is used here as a
    // state ID to check when date and time are to be updated or not.
    CREATE_DATETIME         = 0xF300,                       // creates the date and time setting menu		
    DISPLAY_DATETIME        = 0xF301,                       // displays the menu for the date and time setting
    DISPLAY_DATE_PDMENU     = 0xF302,                       // displays the pulldown menu to set one of the date items (month, day or year)
    SHOW_DATE_PDMENU        = 0xF303,                       // creates the pulldown menu to set one of the date items (month, day or year)
    HIDE_DATE_PDMENU        = 0xF304                        // hides the pulldown menu to set one of the date items (month, day or year)
} SCREEN_STATES;

/////////////////////////////////////////////////////////////////////////////
//                       GLOBAL VARIABLES FOR DEMO
/////////////////////////////////////////////////////////////////////////////
SCREEN_STATES   screenState = CREATE_BUTTONS;               // current state of main demo state mashine
GOL_SCHEME      *altScheme;                                 // alternative style scheme
GOL_SCHEME      *alt2Scheme;                                // alternative 2 style scheme
GOL_SCHEME      *alt3Scheme;                                // alternative 3 style scheme
GOL_SCHEME      *alt4Scheme;                                // alternative 4 style scheme
GOL_SCHEME      *alt5Scheme;                                // alternative 5 style scheme
GOL_SCHEME      *timeScheme;
GOL_SCHEME      *meterScheme;                               // meter scheme
GOL_SCHEME      *navScheme;                                 // style scheme for the navigation
GOL_SCHEME      *redScheme;                                 // alternative red style scheme
GOL_SCHEME      *greenScheme;                               // alternative green style scheme
GOL_SCHEME      *yellowScheme;                              // alternative yellow style scheme
OBJ_HEADER      *pNavList;                                  // pointer to navigation list
volatile DWORD  tick = 0;                                   // tick counter
char            animate;                                    // switch to control animation for picture demo
OBJ_HEADER      *pGenObj;                                   // pointer to a general purpose object
PICTURE         *pPicture;                                  // pointer to picture object for picture demo
PROGRESSBAR     *pProgressBar;                              // pointer to progress bar object for progress bar demo
SLIDER          *pSlider;                                   // pointer to the slider controlling the animation speed

// DEFINITIONS FOR CUSTOM CONTROL DEMO
#define CC_ORIGIN_X ((GetMaxX() - 180 + 1) / 2)
#define CC_ORIGIN_Y ((40 + GetMaxY() - 175 + 1) / 2)

// GLOBAL DEFINITIONS AND VARIABLES FOR SLIDER DEMO
#define SLD_ORIGIN_X    ((GetMaxX() - 260 + 1) / 2)
#define SLD_ORIGIN_Y    ((40 + GetMaxY() - 180 + 1) / 2)
#define CUR_BAR_SIZE    3                                   // half size of center point for cursor
#define CUR_BRD_LEFT    SLD_ORIGIN_X + 37 + CUR_BAR_SIZE    // cursor area left border
#define CUR_BRD_RIGHT   SLD_ORIGIN_X + 223 - CUR_BAR_SIZE   // cursor area right border
#define CUR_BRD_TOP     SLD_ORIGIN_Y + 2 + CUR_BAR_SIZE     // cursor area top border
#define CUR_BRD_BOTTOM  SLD_ORIGIN_Y + 134 - CUR_BAR_SIZE   // cursor area bottom border
SHORT           x, prevX;                                   // cursor X position
SHORT           y, prevY;                                   // cursor Y position
WORD            cursorUpdate;                               // update the cursor position if set   

// GLOBAL DEFINITIONS AND VARIABLES FOR ECG DEMO
// Array of ECG points to emulate ECG input
const SHORT ecgPoints[] ={
    0x0068,0x0068,0x0068,0x0068,0x0068,0x0069,0x0068,0x0068,0x0068,0x0067,0x0067,0x0066,0x0066,0x0068,0x0068,0x0068,0x0067,0x0066,0x0066,0x0066,0x0066,0x0065,0x0065,0x0065,0x0065,0x0065,0x0065,0x0064,0x0063,0x0063,0x0063,0x0064,
    0x0064,0x0065,0x0065,0x0064,0x0064,0x0064,0x0064,0x0065,0x0066,0x0066,0x0062,0x0061,0x0060,0x0059,0x0055,0x004a,0x0049,0x0047,0x0047,0x0048,0x004a,0x004f,0x0053,0x0056,0x005c,0x005d,0x0060,0x0061,0x0062,0x0064,0x0064,0x0064,
    0x0065,0x0066,0x0067,0x0068,0x0068,0x0069,0x0070,0x0070,0x0055,0x0030,0x0004,0x0028,0x005d,0x006c,0x0069,0x0069,0x0069,0x0069,0x0068,0x0067,0x0065,0x0063,0x0061,0x005e,0x005e,0x0062,0x0064,0x0066,0x0067,0x0068,0x0068,0x006a
};

// Scanning window size
#define ECG_WINDOW_SIZE 8

// ECG data circular buffer size
#define ECG_BUFFER_SIZE 256

// ECG data circular buffer
SHORT   ecgBuffer[ECG_BUFFER_SIZE];

// GLOBAL DEFINITIONS AND VARIABLES FOR PLOTTING POTENTIOMETER DEMO
// Graph shift
#define POT_MOVE_DELTA  2

// Potentiometer circular buffer size
#define POT_BUFFER_SIZE 320

// Potentiometer data circular buffer
SHORT           potBuffer[POT_BUFFER_SIZE];

// Temporary buffer for graph demo screens
SHORT           tempBuffer[10];

// Variables for the pulldown menus
SHORT           pDwnLeft, pDwnTop, pDwnRight, pDwnBottom;

// Variables for date and time
SCREEN_STATES   prevState = CREATE_BUTTONS; // used to mark state where time setting was called	
OBJ_HEADER      *pListSaved;                // temporary storage for object list
WORD            DateItemID;                 // ID of date item being modified (ID_EB_MONTH(DAY)(YEAR))
XCHAR           dateTimeStr[26];            // string variable for date and time display
SHORT           List2Enable = 0, refreshScreen = 0;


/////////////////////////////////////////////////////////////////////////////
// SPI Channel settings
/////////////////////////////////////////////////////////////////////////////

#if defined(SPI_CHANNEL_1_ENABLE) || defined(SPI_CHANNEL_2_ENABLE)

    #if defined (USE_SST25VF016)
        const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
    #endif    
    #ifdef USE_TOUCHSCREEN_AR1020
        const DRV_SPI_INIT_DATA ar1020SpiInit = {AR1020_SPI_CHANNEL,    2,  3, 0, 0, 0, 0};        
    #endif
#endif
/////////////////////////////////////////////////////////////////////////////
// TouchScreen Init Values
/////////////////////////////////////////////////////////////////////////////
#ifdef USE_TOUCHSCREEN_RESISTIVE
#define TOUCH_INIT_VALUES   (NULL)
#endif
#ifdef USE_TOUCHSCREEN_AR1020
#define TOUCH_INIT_VALUES   ((void *)&ar1020SpiInit)
#endif

/////////////////////////////////////////////////////////////////////////////

//                                  MAIN
/////////////////////////////////////////////////////////////////////////////
int main(void)
{
    GOL_MSG msg;                            // GOL message structure to interact with GOL

    #ifdef __PIC32MX__
        INTEnableSystemMultiVectoredInt();
        SYSTEMConfigPerformance(GetSystemClock());  
    #endif

    GOLInit();                              // Initialize graphics library and crete default style scheme for GOL

#if !defined(GFX_PICTAIL_LCC)
    SetPaletteBpp(8);
#endif

    SetPalette((void*)&GOL_Palette_Demo, 0, 256);

    EnablePalette();

    InitializeBoard();

    StartScreen();                                 // Show intro screen and wait for touch

    // create the alternate schemes
    navScheme = GOLCreateScheme();          // alternative scheme for the navigate buttons
    altScheme = GOLCreateScheme();          // create alternative 1 style scheme
    alt2Scheme = GOLCreateScheme();         // create alternative 2 style scheme
    alt3Scheme = GOLCreateScheme();         // create alternative 3 style scheme
    alt4Scheme = GOLCreateScheme();         // create alternative 4 style scheme
    alt5Scheme = GOLCreateScheme();         // create alternative 5 style scheme
    timeScheme = GOLCreateScheme();
    meterScheme = GOLCreateScheme();        // create meter scheme
    redScheme = GOLCreateScheme();          // create red style scheme
    greenScheme = GOLCreateScheme();        // create green style scheme
    yellowScheme = GOLCreateScheme();       // create yellow style scheme
    navScheme->CommonBkColor = 67;  //RGB565CONVERT(212, 237, 247)
    altScheme->Color0 = 47;  //RGB565CONVERT(0x4C, 0x8E, 0xFF);
    altScheme->Color1 = 48;  //RGB565CONVERT(0xFF, 0xBB, 0x4C);
    altScheme->EmbossDkColor = 49;   //RGB565CONVERT(0x1E, 0x00, 0xE5);
    altScheme->EmbossLtColor = 50;   //RGB565CONVERT(0xA9, 0xDB, 0xEF);
    altScheme->ColorDisabled = 51;   //RGB565CONVERT(0xD4, 0xE1, 0xF7);

    altScheme->TextColor1 = BRIGHTBLUE;
    altScheme->TextColor0 = 52;      //RGB565CONVERT(0xFF, 0xBB, 0x4C);
    altScheme->TextColorDisabled = 53;   //RGB565CONVERT(0xB8, 0xB9, 0xBC);

    alt2Scheme->TextColor1 = BRIGHTRED;
    alt2Scheme->TextColor0 = BRIGHTBLUE;
    alt2Scheme->pFont = (void *) &GOLMediumFont;

    alt3Scheme->Color0 = LIGHTBLUE;
    alt3Scheme->Color1 = BRIGHTGREEN;
    alt3Scheme->TextColor0 = BLACK;
    alt3Scheme->TextColor1 = WHITE;
    alt3Scheme->pFont = (void *) &GOLMediumFont;

    alt4Scheme->Color0 = LIGHTBLUE;
    alt4Scheme->Color1 = BRIGHTGREEN;
    alt4Scheme->TextColor0 = BLACK;
    alt4Scheme->TextColor1 = WHITE;
    alt4Scheme->pFont = (void *) &GOLSmallFont;

    alt5Scheme->Color0 = LIGHTBLUE;
    alt5Scheme->Color1 = BRIGHTRED;
    alt5Scheme->TextColor0 = BLACK;
    alt5Scheme->TextColor1 = WHITE;
    alt5Scheme->pFont = (void *) &monofont; //monofont is equal width font, required for digitalmeter widget
    redScheme->Color0 = 54;      //RGB565CONVERT(0xCC, 0x00, 0x00);
    redScheme->Color1 = BRIGHTRED;
    redScheme->EmbossDkColor = RED4;
    redScheme->EmbossLtColor = FIREBRICK1;
    redScheme->TextColor0 = 55;  //RGB565CONVERT(0xC8, 0xD5, 0x85);
    redScheme->TextColor1 = BLACK;

    greenScheme->Color0 = 56;    //RGB565CONVERT(0x23, 0x9E, 0x0A);
    greenScheme->Color1 = BRIGHTGREEN;
    greenScheme->EmbossDkColor = DARKGREEN;
    greenScheme->EmbossLtColor = PALEGREEN;
    greenScheme->TextColor0 = 57;    //RGB565CONVERT(0xDF, 0xAC, 0x83);
    greenScheme->TextColor1 = BLACK;

    yellowScheme->Color0 = BRIGHTYELLOW;
    yellowScheme->Color1 = YELLOW;
    yellowScheme->EmbossDkColor = 58;    //RGB565CONVERT(0xFF, 0x94, 0x4C);
    yellowScheme->EmbossLtColor = 59;    //RGB565CONVERT(0xFD, 0xFF, 0xB2);
    yellowScheme->TextColor0 = 60;       //RGB565CONVERT(0xAF, 0x34, 0xF3);
    yellowScheme->TextColor1 = RED;

    timeScheme->Color0 = BLACK;
    timeScheme->Color1 = WHITE;
    timeScheme->TextColor0 = BRIGHTBLUE;
    timeScheme->TextColor1 = WHITE;
    timeScheme->EmbossDkColor = GRAY20;
    timeScheme->EmbossLtColor = GRAY80;
    timeScheme->pFont = (void *) &GOLSmallFont;

    meterScheme->Color0 = BLACK;
    meterScheme->Color1 = WHITE;
    meterScheme->TextColor0 = BRIGHTBLUE;
    meterScheme->TextColor1 = WHITE;
    meterScheme->EmbossDkColor = GRAY20;
    meterScheme->EmbossLtColor = GRAY80;
    meterScheme->pFont = (void *) &GOLSmallFont;

    while(1)
    {
        if(GOLDraw())
        {                           // Draw GOL objects
            // Drawing is done here, process messages
            TouchGetMsg(&msg);      // Get message from touch screen
            GOLMsg(&msg);           // Process message
            #if !(defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__) || defined(GFX_PICTAIL_LCC))
            SideButtonsMsg(&msg);   // Get message from side buttons
            GOLMsg(&msg);           // Process message
            #endif
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
// Input: objMsg - translated message for the object,
//        pObj - pointer to the object,
//        pMsg - pointer to the non-translated, raw GOL message
// Output: if the function returns non-zero the message will be processed by default
// Overview: this function must be implemented by user. GOLMsg() function calls it each

//           time the valid message for the object received
/////////////////////////////////////////////////////////////////////////////
WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{

    // beep if button is pressed
    if(objMsg == BTN_MSG_PRESSED)
    {
        Beep();
    }
    else
    {
        if(GetObjType(pObj) == OBJ_RADIOBUTTON)
        {
            Beep();
        }
        else
        {
            if(GetObjType(pObj) == OBJ_CHECKBOX)
                Beep();
        }
    }

    if((screenState & 0xF300) != 0xF300)
    {

        // check for time setting press, process only when not setting time and date
        if(objMsg == ST_MSG_SELECTED)
        {

            /* note how the states are arranged in the enumeration, the display state is 
		       right after the create state. So at the time the static text box of the 
		       time is selected, the state is in one of the displays. So a minus one is needed
		       to adjust to go back to the create state of the current page.
		       After the date and time adjust screen is exited, the saved create state will
		       be entered as the next state. So we get the effect of going back to the previous
		       screen after date and time settings are done.
		    */
            if((GetObjID(pObj) == ID_STATICTEXT1) || (GetObjID(pObj) == ID_STATICTEXT2))
            {
                prevState = screenState - 1;        // save the current create state
                screenState = CREATE_DATETIME;      // go to date and time setting screen
                return (1);
            }
        }
    }

    // check if pull down menu is called
    if(GetObjID(pObj) == ID_WINDOW1)
    {
        if((objMsg == WND_MSG_TITLE) && (screenState != DISPLAY_PULLDOWN))
        {

            // check area of press
            if((pMsg->param1 <= 40) && (pMsg->param2 <= 40))
            {
                switch(screenState)
                {

                    // if one of these states we redraw the whole screen since
                    // these screens have customized graphics.
                    case DISPLAY_SLIDER:
                        prevState = CREATE_SLIDER;
                        break;

                    case DISPLAY_CUSTOMCONTROL:
                        prevState = CREATE_CUSTOMCONTROL;
                        break;

                    case DISPLAY_SIGNATURE:
                        prevState = CREATE_SIGNATURE;
                        break;

                    case DISPLAY_POT:
                        prevState = CREATE_POT;
                        break;

                    case DISPLAY_ECG:
                        prevState = CREATE_ECG;
                        break;

                    case DISPLAY_PROGRESSBAR:
                        prevState = CREATE_PROGRESSBAR;
                        break;

                    // pull down is disabled when setting date and time
                    case CREATE_DATETIME:
                    case DISPLAY_DATETIME:
                    case DISPLAY_DATE_PDMENU:
                    case SHOW_DATE_PDMENU:
                    case HIDE_DATE_PDMENU:
                        return (0);

                    default:
                        prevState = screenState;    // save the current create state
                        break;
                }

                screenState = CREATE_PULLDOWN;      // go to show pulldown menu state
                return (1);
            }
        }
    }

    // process messages for demo screens
    switch(screenState)
    {
        case DISPLAY_BUTTONS:
            return (MsgButtons(objMsg, pObj));

        case DISPLAY_CHECKBOXES:
            return (MsgCheckBoxes(objMsg, pObj));

        case DISPLAY_RADIOBUTTONS:
            return (MsgRadioButtons(objMsg, pObj));

        case DISPLAY_STATICTEXT:
            return (MsgStaticText(objMsg, pObj));

        case DISPLAY_PICTURE:
            return (MsgPicture(objMsg, pObj));

        case DISPLAY_SLIDER:
            return (MsgSlider(objMsg, pObj, pMsg));

        case DISPLAY_PROGRESSBAR:
            return (MsgProgressBar(objMsg, pObj));

        // date and time settings display
        case DISPLAY_DATETIME:
            return (MsgDateTime(objMsg, pObj));

        case DISPLAY_DATE_PDMENU:
            return (MsgSetDate(objMsg, pObj, pMsg));

        case CREATE_DATETIME:
        case SHOW_DATE_PDMENU:
        case HIDE_DATE_PDMENU:
            return (0);

        case DISPLAY_METER:
            return (MsgMeter(objMsg, pObj));

        case DISPLAY_DIAL:
            return (MsgDial(objMsg, pObj));

        case DISPLAY_CUSTOMCONTROL:
            return (MsgCustomControl(objMsg, pObj, pMsg));

        case DISPLAY_LISTBOX:
            return (MsgListBox(objMsg, pObj, pMsg));

        case DISPLAY_EDITBOX:
            return (MsgEditBox(objMsg, pObj, pMsg));

        case DISPLAY_SIGNATURE:
            return (MsgSignature(objMsg, pObj, pMsg));

        case DISPLAY_POT:
            return (MsgPotentiometer(objMsg, pObj));

        case DISPLAY_ECG:
            return (MsgECG(objMsg, pObj));

        case DISPLAY_PULLDOWN:
            return (MsgPullDown(objMsg, pObj, pMsg));

        default:

            // process message by default
            return (1);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLDrawCallback()
// Output: if the function returns non-zero the draw control will be passed to GOL
// Overview: this function must be implemented by user. GOLDraw() function calls it each
//           time when GOL objects drawing is completed. User drawing should be done here.
//           GOL will not change color, line type and clipping region settings while

//           this function returns zero.
/////////////////////////////////////////////////////////////////////////////
WORD GOLDrawCallback(void)
{
    static DWORD    prevTick = 0;                           // keeps previous value of tick
    static BYTE     direction = 1;                          // direction switch for progress bar
    static BYTE     arrowPos = 0;                           // arrows pictures position for custom control demo
    static WORD     wDelay = 40;                           // progress bar delay variable
    OBJ_HEADER      *pObj = NULL;                           // used to change text in Window
    SLIDER          *pSld;                                  // used when updating date and time
    LISTBOX         *pLb;                                   // used when updating date and time

    #if !(defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__) || defined(GFX_PICTAIL_LCC))

    static DWORD    prevTime = 0;                           // keeps previous value of time tick
    WORD            i;

	// check first of the objects for the time display is already created.
	if (GOLFindObject(ID_STATICTEXT1) != NULL) {
	    // update the time display
	    if((screenState & 0x0000F300) != 0x0000F300)
	    {                                                       // process only when NOT setting time and date
	        if((tick - prevTime) > 10000)
	        {
	            RTCCProcessEvents();                            // update the date and time string variabes
	            i = 0;
	            while(i < 12)
	            {
	                dateTimeStr[i] = _time_str[i];
	                dateTimeStr[i + 13] = _date_str[i];
	                i++;
	            }
	
	            dateTimeStr[12] = 0x000A;                       // (XCHAR)'\n';
	            dateTimeStr[25] = 0x0000;                       // (XCHAR)'\0';
	            if((pObj = GOLFindObject(ID_STATICTEXT1)))
	            {                                               // get the time display obj pointer
	                StSetText((STATICTEXT *)pObj, dateTimeStr); // now display the new date & time
	                SetState(pObj, ST_DRAW);                    // redraw the time display
	            }
	
	            prevTime = tick;                                // reset tick timer
	        }
	    }
	    else
	    {                               // process only when setting time and date
	        // do not update when pull down menus are on
	        if((screenState != DISPLAY_DATE_PDMENU) && (screenState != HIDE_DATE_PDMENU))
	        {
	            if((tick - prevTime) > 1000)
	            {
	                updateDateTimeEb(); // update edit boxes for date and time settings
	                prevTime = tick;    // reset tick timer
	            }
	        }
	    }
	} // if (GOLFindObject(ID_STATICTEXT1) != NULL) ...     

    #endif
    switch(screenState)
    {
        case CREATE_BUTTONS:
            CreateButtons();        // create window and buttons
            screenState = DISPLAY_BUTTONS;                      // switch to next state
            return (1);                                         // draw objects created

        case DISPLAY_BUTTONS:
            return (1);                                         // redraw objects if needed

        case CREATE_CHECKBOXES:
            CreateCheckBoxes();                                 // create window and check boxes
            screenState = DISPLAY_CHECKBOXES;                   // switch to next state
            return (1);                                         // draw objects created

        case DISPLAY_CHECKBOXES:
            return (1);                                         // redraw objects if needed

        case CREATE_RADIOBUTTONS:
            CreateRadioButtons();                               // create window and radio buttons
            screenState = DISPLAY_RADIOBUTTONS;                 // switch to next state
            return (1);                                         // draw objects created

        case DISPLAY_RADIOBUTTONS:
            return (1);                                         // redraw objects if needed

        case CREATE_STATICTEXT:
            CreateStaticText();                                 // create window, group box, static text

            // and radio buttons for control
            screenState = DISPLAY_STATICTEXT;                   // switch to next state
            return (1);                                         // draw objects created

        case DISPLAY_STATICTEXT:
            if((tick - prevTick) > 40000)
            {
                pObj = GOLFindObject(ID_WINDOW1);
                if(direction)
                {
                    WndSetText((WINDOW *)pObj, GroupBoxStr);    // "Group Box");
                    direction = 0;
                }
                else
                {
                    WndSetText((WINDOW *)pObj, StaticTextStr);  // "Static text");
                    direction = 1;
                }

                SetState(pObj, WND_DRAW_TITLE);
                prevTick = tick;
            }

            return (1);                         // redraw objects if needed

        case CREATE_SLIDER:
            cursorUpdate = 1;
            CreateSlider();                     // create window and sliders
            screenState = CURSOR_DRAW_SLIDER;   // switch to next state
            return (1);                         // draw objects created

        case CURSOR_DRAW_SLIDER:
            DrawSliderCursor(BLACK, x, y);              // draw sliders position cursor
            prevX = x;
            prevY = y;
            screenState = DISPLAY_SLIDER;       // switch to next state

        case DISPLAY_SLIDER:
            if (cursorUpdate)
            {
                // hide previous position
                DrawSliderCursor(altScheme->CommonBkColor, prevX, prevY);      // draw sliders position cursor
                // draw new position
                DrawSliderCursor(BLACK, x, y);              // draw sliders position cursor
                prevX = x;
                prevY = y;
                cursorUpdate = 0;
            }    
            return (1);                         // redraw objects if needed

        case CREATE_PROGRESSBAR:
            CreateProgressBar();                // create window and progress bar
            screenState = DISPLAY_PROGRESSBAR;  // switch to next state
            return (1);                         // draw objects created

        case DISPLAY_PROGRESSBAR:
            if(GetState(pGenObj, BTN_PRESSED))
                wDelay = 0;
            else
                wDelay = 400;

            if((tick - prevTick) > wDelay)
            {
                if(direction)
                {
                    if(pProgressBar->pos == pProgressBar->range)
                        direction = 0;          // change direction
                    else
                        PbSetPos(pProgressBar, PbGetPos(pProgressBar) + 1); // increase
                }
                else
                {
                    if(pProgressBar->pos == 0)
                        direction = 1;  // change direction
                    else
                        PbSetPos(pProgressBar, PbGetPos(pProgressBar) - 1); // decrease
                }

                SetState(pProgressBar, PB_DRAW_BAR);                        // redraw bar only
                prevTick = tick;
            }

            return (1);                         // redraw objects if needed

        case CREATE_EDITBOX:
            CreateEditBox();                    // create edit box test screen
            screenState = DISPLAY_EDITBOX;      // switch to next state
            return (1);                         // draw objects created

        case DISPLAY_EDITBOX:
            return (1);                         // draw objects

        case CREATE_LISTBOX:
            CreateListBox();                    // create list box test screen
            screenState = DISPLAY_LISTBOX;      // switch to next state
            return (1);                         // draw objects created

        case DISPLAY_LISTBOX:

            // this moves the slider and editbox for the date setting to
            // move while the up or down arrow buttons are pressed
            if((tick - prevTick) > 1000)
            {
                pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);
                pSld = (SLIDER *)GOLFindObject(ID_SLIDER1);
                pObj = GOLFindObject(ID_BUTTON1);

                if(GetState(pObj, BTN_PRESSED))
                {
                    LbSetFocusedItem(pLb, LbGetFocusedItem(pLb) - 1);
                    SetState(pLb, LB_DRAW_ITEMS);
                    SldSetPos(pSld, SldGetPos(pSld) + 1);
                    SetState(pSld, SLD_DRAW_THUMB);
                }

                pObj = GOLFindObject(ID_BUTTON2);

                if(GetState(pObj, BTN_PRESSED))
                {
                    LbSetFocusedItem(pLb, LbGetFocusedItem(pLb) + 1);
                    SetState(pLb, LB_DRAW_ITEMS);
                    SldSetPos(pSld, SldGetPos(pSld) - 1);
                    SetState(pSld, SLD_DRAW_THUMB);
                }

                prevTick = tick;
            }

            return (1);                         // draw objects

        case CREATE_DATETIME:
            CreateDateTime();                   // create date and time demo
            screenState = DISPLAY_DATETIME;     // switch to next state
            return (1);                         // draw objects created

        case SHOW_DATE_PDMENU:
            ShowPullDownMenu();
            screenState = DISPLAY_DATE_PDMENU;
            return (1);

        case HIDE_DATE_PDMENU:
            if(RemovePullDownMenu())
                screenState = DISPLAY_DATETIME; // switch to next state
            return (1);

        case DISPLAY_DATE_PDMENU:

            // this moves the slider and editbox for the date setting to
            // move while the up or down arrow buttons are pressed
            if((tick - prevTick) > 1000)
            {
                pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);
                pSld = (SLIDER *)GOLFindObject(ID_SLIDER1);
                pObj = GOLFindObject(ID_BUTTON_DATE_UP);

                if(GetState(pObj, BTN_PRESSED))
                {
                    LbSetFocusedItem(pLb, LbGetFocusedItem(pLb) - 1);
                    SetState(pLb, LB_DRAW_ITEMS);
                    SldSetPos(pSld, SldGetPos(pSld) + 1);
                    SetState(pSld, SLD_DRAW_THUMB);
                }

                pObj = GOLFindObject(ID_BUTTON_DATE_DN);

                if(GetState(pObj, BTN_PRESSED))
                {
                    LbSetFocusedItem(pLb, LbGetFocusedItem(pLb) + 1);
                    SetState(pLb, LB_DRAW_ITEMS);
                    SldSetPos(pSld, SldGetPos(pSld) - 1);
                    SetState(pSld, SLD_DRAW_THUMB);
                }

                prevTick = tick;
            }

            return (1);

        case DISPLAY_DATETIME:

            // Checks if the pull down menus are to be created or not
            pObj = GOLFindObject(ID_BUTTON_MO);
            if(GetState(pObj, BTN_PRESSED))
            {
                screenState = SHOW_DATE_PDMENU; // change state
                return (1);
            }

            pObj = GOLFindObject(ID_BUTTON_YR);
            if(GetState(pObj, BTN_PRESSED))
            {
                screenState = SHOW_DATE_PDMENU; // change state
                return (1);
            }

            pObj = GOLFindObject(ID_BUTTON_DY);
            if(GetState(pObj, BTN_PRESSED))
            {
                screenState = SHOW_DATE_PDMENU; // change state
                return (1);
            }

            // this increments the values for the time settings
            // while the + or - buttons are pressed
            if((tick - prevTick) > 2000)
            {
                pObj = GOLFindObject(ID_BUTTONHR_P);
                if(GetState(pObj, BTN_PRESSED))
                {
                    MsgDateTime(BTN_MSG_PRESSED, pObj);
                }

                pObj = GOLFindObject(ID_BUTTONHR_M);
                if(GetState(pObj, BTN_PRESSED))
                {
                    MsgDateTime(BTN_MSG_PRESSED, pObj);
                }

                pObj = GOLFindObject(ID_BUTTONMN_P);
                if(GetState(pObj, BTN_PRESSED))
                {
                    MsgDateTime(BTN_MSG_PRESSED, pObj);
                }

                pObj = GOLFindObject(ID_BUTTONMN_M);
                if(GetState(pObj, BTN_PRESSED))
                {
                    MsgDateTime(BTN_MSG_PRESSED, pObj);
                }

                pObj = GOLFindObject(ID_BUTTONSC_P);
                if(GetState(pObj, BTN_PRESSED))
                {
                    MsgDateTime(BTN_MSG_PRESSED, pObj);
                }

                pObj = GOLFindObject(ID_BUTTONSC_M);
                if(GetState(pObj, BTN_PRESSED))
                {
                    MsgDateTime(BTN_MSG_PRESSED, pObj);
                }

                prevTick = tick;
            }

            return (1);

        case CREATE_METER:
            CreateMeter();                      // create meter demo
            screenState = DISPLAY_METER;        // switch to next state
            return (1);                         // draw objects created

        case DISPLAY_METER:
            if((tick - prevTick) > 600)
            {
                UpdateMeter();
                prevTick = tick;
            }

            return (1);                         // redraw objects if needed

        case CREATE_DIAL:
            CreateDial();                       // create dial demo
            screenState = DISPLAY_DIAL;         // switch to next state
            return (1);                         // draw objects created

        case DISPLAY_DIAL:
            return (1);                         // redraw objects if needed

        case CREATE_PICTURE:
            CreatePicture();                    // create window, picture control and some check boxes
            screenState = DISPLAY_PICTURE;      // switch to next state
            return (1);                         // draw objects created

        case DISPLAY_PICTURE:
            if(animate)
            {   // if animation is on change picture
                if((tick - prevTick) > (SldGetPos(pSlider) + 40) * 10)
                {
                    switch(animate)
                    {
                        case 1: PictSetBitmap(pPicture, (void *) &Engine1); animate++; break;
                        case 2: PictSetBitmap(pPicture, (void *) &Engine2); animate++; break;
                        case 3: PictSetBitmap(pPicture, (void *) &Engine3); animate++; break;
                        case 4: PictSetBitmap(pPicture, (void *) &Engine4); animate = 1; break;
                    }

                    SetState(pPicture, PICT_DRAW);  // must be redrawn
                    prevTick = tick;
                }
            }

            return (1);                             // redraw objects if needed

        case CREATE_CUSTOMCONTROL:
            CreateCustomControl();                  // create window and custom control
            screenState = DISPLAY_CUSTOMCONTROL;    // switch to next state
            return (1);                             // draw objects created

        case DISPLAY_CUSTOMCONTROL:
            #define SX_ARROW    18
            #define SY_ARROW    26
            if((tick - prevTick) > 20)
            {
                arrowPos++;                         // change arrows' position
                if(arrowPos > 60 - SY_ARROW)
                    arrowPos = 0;

                WAIT_UNTIL_FINISH(PutImage(CC_ORIGIN_X + 0, CC_ORIGIN_Y + 60 - SY_ARROW - arrowPos, (void *) &arrowUp, 1)); // draw arrows
                WAIT_UNTIL_FINISH(PutImage(CC_ORIGIN_X + 0, CC_ORIGIN_Y + 60 + arrowPos, (void *) &arrowDown, 1));
                WAIT_UNTIL_FINISH(PutImage(CC_ORIGIN_X + 180 - SX_ARROW, CC_ORIGIN_Y + 60 - SY_ARROW - arrowPos, (void *) &arrowUp, 1));
                WAIT_UNTIL_FINISH(PutImage(CC_ORIGIN_X + 180 - SX_ARROW, CC_ORIGIN_Y + 60 + arrowPos, (void *) &arrowDown, 1));
                prevTick = tick;
            }

            return (1);                         // redraw objects if needed

        case CREATE_SIGNATURE:
            CreateSignature();                  // create window
            screenState = BOX_DRAW_SIGNATURE;   // switch to next state
            return (1);                         // draw objects created

        case BOX_DRAW_SIGNATURE:
            if(0 == PanelSignature())           // draw box for signature
                return (0);                     // drawing is not completed, don't pass

            // drawing control to GOL, try it again
            screenState = DISPLAY_SIGNATURE;    // switch to next state
            return (1);                         // pass drawing control to GOL, redraw objects if needed

        case DISPLAY_SIGNATURE:
            return (1);                         // redraw objects if needed

        case CREATE_POT:
            #if defined(__dsPIC33F__) || defined(__PIC24H__)
            ADC_POT_TRIS = 1;
            ADC_POT_PCFG = 0;
            #endif
            CreatePotentiometer();              // create window
            screenState = BOX_DRAW_POT;         // switch to next state
            return (1);                         // draw objects created

        case BOX_DRAW_POT:
            if(0 == PanelPotentiometer())       // draw box for potentiometer graph
                return (0);                     // drawing is not completed, don't pass

            // drawing control to GOL, try it again
            screenState = DISPLAY_POT;          // switch to next state
            return (1);                         // pass drawing control to GOL, redraw objects if needed

        case DISPLAY_POT:
            if((tick - prevTick) > 200)
            {
                if(GetPotSamples(POT_MOVE_DELTA))
                    GraphPotentiometer();       // redraw graph
                prevTick = tick;
            }

            return (1);                         // redraw objects if needed

        case CREATE_ECG:
            CreateECG();                        // create window
            screenState = BOX_DRAW_ECG;         // switch to next state
            return (1);                         // draw objects created

        case BOX_DRAW_ECG:
            if(0 == PanelECG())                 // draw box for ECG graph
                return (0);                     // drawing is not completed, don't pass

            // drawing control to GOL, try it again
            screenState = DISPLAY_ECG;          // switch to next state
            return (1);                         // pass drawing control to GOL, redraw objects if needed

        case DISPLAY_ECG:
            if((tick - prevTick) > 10)
            {
                if(GetECGSamples(ECG_WINDOW_SIZE))
                    GraphECG();                 // redraw graph
                prevTick = tick;
            }

            return (1);                         // redraw objects if needed

        case CREATE_PULLDOWN:
            CreatePullDown();
            screenState = DISPLAY_PULLDOWN;
            return (1);

        case DISPLAY_PULLDOWN:
            DisplayPullDown();
            return (1);                         // redraw objects if needed
    }

    return (1); // release drawing control to GOL
}

// Shows intro screen and waits for touch
void StartScreen(void)
{
    #define SS_ORIGIN_X ((GetMaxX() - 316 + 1) / 2)
    #define SS_ORIGIN_Y ((GetMaxY() - 140 + 1) / 2)

    SHORT               counter;

    static const XCHAR  text[] = "Touch screen to continue";

    WORD                i, j, k;
    WORD                ytemp, xtemp, srRes = 0x0001;

    SetColor(BLACK);
    ClearDevice();
    WAIT_UNTIL_FINISH(PutImage((GetMaxX() - GetImageWidth((void *) &intro)) / 2, (GetMaxY() - GetImageHeight((void *) &intro)) / 2 - 21, (void *) &intro, IMAGE_NORMAL));

    for(counter = 0; counter < ((GetMaxX() + 1) - 40); counter++)
    {                               // move Microchip icon
        #ifdef __PIC24FJ256DA210__
            if(counter == 0)
            {
                WAIT_UNTIL_FINISH(PutImage(counter, GetMaxY() - 40, (void *) &mchpIcon0, IMAGE_NORMAL));
            }
            else
            {
                DWORD x = counter;
                DWORD y = GetMaxY() - 40;
                
                GFX_WaitForCommandQueue(4);
                GFX_SetWorkArea1(GFX_DISPLAY_BUFFER_START_ADDRESS);
                GFX_SetWorkArea2(GFX_DISPLAY_BUFFER_START_ADDRESS);

                #if (DISP_ORIENTATION == 0)
                {
                    GFX_RCC_SetSrcOffset((y * DISP_HOR_RESOLUTION) + (x - 1));
                    GFX_RCC_SetDestOffset((y * DISP_HOR_RESOLUTION) + x);
                    GFX_RCC_SetSize(GetImageWidth((void *) &mchpIcon0), GetImageHeight((void *) &mchpIcon0));
                    GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_DISCONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                    DelayMs(1);
                }
                #elif (DISP_ORIENTATION == 90)
                {
                    DWORD t = x;
                    x = y;
                    y = GetMaxX() - t - GetImageWidth((void *) &mchpIcon0) + 1;
                    GFX_RCC_SetSrcOffset(((y + 1) * DISP_HOR_RESOLUTION) + x);
                    GFX_RCC_SetDestOffset((y * DISP_HOR_RESOLUTION) + x);
                    GFX_RCC_SetSize(GetImageHeight((void *) &mchpIcon0), GetImageWidth((void *) &mchpIcon0));
                    GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_DISCONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                    DelayMs(1);
                }
                #else
                    WAIT_UNTIL_FINISH(PutImage(counter, GetMaxY() - 40, (void *) &mchpIcon0, IMAGE_NORMAL));
                #endif
            }
        #else
            WAIT_UNTIL_FINISH(PutImage(counter, GetMaxY() - 40, (void *) &mchpIcon0, IMAGE_NORMAL));
        #endif
    }

    SetColor(BRIGHTRED);
    SetFont((void *) &FONTDEFAULT);

    MoveTo((GetMaxX() - GetTextWidth((XCHAR *)text, (void *) &FONTDEFAULT)) >> 1, SS_ORIGIN_Y + 122);
    WAIT_UNTIL_FINISH(OutText((XCHAR *)text));

    // wait for touch
    while(TouchGetX() == -1);
    Beep();

    SetClipRgn
    (
        SS_ORIGIN_X,
        SS_ORIGIN_Y,
        SS_ORIGIN_X + (GetImageWidth((void *) &intro) << 1),
        SS_ORIGIN_Y + (GetImageHeight((void *) &intro) << 1)
    );
    SetClip(CLIP_ENABLE);

    // random fade effect using a Linear Feedback Shift Register (LFSR)
    SetColor(WHITE);
    for(i = 1800; i > 0; i--)
    {

        // for a 16 bit LFSR variable the taps are at bits: 1, 2, 4, and 15
        srRes = (srRes >> 1) ^ (-(int)(srRes & 1) & 0x8006);
        xtemp = (srRes & 0x00FF) % 40;
        ytemp = (srRes >> 8) % 30;

        // by replicating the white (1x1) bars into 8 areas fading is faster
        for(j = 0; j < 8; j++)
        {
            for(k = 0; k < 8; k++)
                PutPixel(SS_ORIGIN_X + xtemp + (j * 40), ytemp + (k * 30));
        }
    }

    SetClip(CLIP_DISABLE);

    #if !(defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__) || defined(GFX_PICTAIL_LCC))

    // initialize date and time
    mRTCCOff();
    RTCCSetBinHour(RTCC_DEFAULT_HOUR);      // set the hour value
    RTCCSetBinMin(RTCC_DEFAULT_MINUTE);     // set the minute value
    RTCCSetBinSec(RTCC_DEFAULT_SECOND);     // set the second value
    RTCCSetBinDay(RTCC_DEFAULT_DAY);        // set the day value
    RTCCSetBinMonth(RTCC_DEFAULT_MONTH);    // set the month value
    RTCCSetBinYear(RTCC_DEFAULT_YEAR);      // set the year value
    RTCCCalculateWeekDay();         		// calculate the weekday from this new value
    mRTCCSet();                     		// copy the new values to the RTCC registers
    RTCCProcessEvents();            		// update the date and time strings
    #endif
}

/* */
void CreatePage(XCHAR *pText)
{
    OBJ_HEADER  *obj;
#if !(defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__) || defined(GFX_PICTAIL_LCC))
    SHORT       i;
#endif

    WndCreate
    (
        ID_WINDOW1,         // ID
        0,
        0,
        GetMaxX(),
        GetMaxY(),          // dimension
        WND_DRAW,           // will be dislayed after creation
        (void *) &mchpIcon, // icon
        pText,              // set text
        navScheme
    );                      // default GOL scheme
    BtnCreate
    (
        ID_BUTTON_BACK,     // button ID
        0,
        40,                 // left, top corner	
        25,
        GetMaxY(),
        0,                  // right, bottom corner (with radius = 0)
        BTN_DRAW,           // will be dislayed after creation
        NULL,               // no bitmap	
        LeftArrowStr,       // LEFT arrow as text
        navScheme
    );                      // use navigation scheme
    obj = (OBJ_HEADER *)BtnCreate
        (
            ID_BUTTON_NEXT, // button ID
            GetMaxX() - 25,
            40,
            GetMaxX(),
            GetMaxY(),
            0,              // dimension (with radius = 0)
            BTN_DRAW,       // will be dislayed and disabled after creation
            NULL,           // no bitmap
            RightArrowStr,  // RIGHT arrow as text
            navScheme
        );                  // use navigation scheme
#if !(defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__) || defined(GFX_PICTAIL_LCC))
    RTCCProcessEvents();    // update the date and time strings
    i = 0;
    while(i < 12)
    {
        dateTimeStr[i] = (XCHAR) _time_str[i];
        dateTimeStr[i + 13] = (XCHAR) _date_str[i];
        i++;
    }

    dateTimeStr[12] = (XCHAR) '\n';
    dateTimeStr[25] = (XCHAR) '\0';

    StCreate
    (
        ID_STATICTEXT1,     // ID
        GetMaxX() - (GetTextWidth(dateTimeStr, (void *) &GOLSmallFont) + 15),
        3,                  // dimensions
        GetMaxX() - 5,
        3 + (GetTextHeight((void *) &GOLSmallFont) << 1),
        ST_DRAW | ST_FRAME | ST_CENTER_ALIGN,   // will be dislayed, has frame and center aligned
        dateTimeStr,                            // text is from time value
        timeScheme
    );                  // alternate scheme
#endif
    GOLSetFocus(obj);   // set focus for the button
}

/* definitions for the pull down menu demo */
#define BOTTOM_NORMAL   40
#define BOTTOM_DROPPED  220

/*
const char ScreenItems[] =
"Buttons\n" "Checkbox\n" "Radio buttons\n" "Group box\n" "Slider\n" "Progress bar\n" "List box\n" "Edit box\n" "MORE......";
const char ScreenItems2[] =
"BACK......\n" "Meter\n" "Dial\n" "Picture\n" "Custom control\n" "Signature\n" "Plotting\n" "ECG"; 
*/
XCHAR   *ScreenItems = ScrSelList1;
XCHAR   *ScreenItems2 = ScrSelList2;

/* */

void CreatePullDown(void)
{
    OBJ_HEADER  *pFObj;

    // before moving to a new list the current focused object is set to be not focused.
    // this saves us the effort to save the pointer to the object and setting
    // the global pointer to the focused object to that saved object.
    // this application does not need it to be saved.
    pFObj = GOLGetFocus();          // get the focused object
    if(pFObj != NULL)
    {
        ClrState(pFObj, FOCUSED);   // clear the focus state
        SetState(pFObj, DRAW);      // set to be redrawn
    }

    pListSaved = GOLGetList();
    GOLNewList();

    LbCreate
    (
        ID_LISTBOX2,                // ID
        40,
        BOTTOM_NORMAL,              // dimensions
        165,
        BOTTOM_DROPPED,
        LB_SINGLE_SEL | LB_DRAW | LB_FOCUSED,   // will be created with single select and focused
        (XCHAR *)ScreenItems,                   // list of demo screens
        alt2Scheme
    );                  // use alternate scheme

    // This object is a ghost object to enable to get messages that touch is outside
    // the list box. It will never be displayed.
    GbCreate
    (
        ID_GROUPBOX2,   // ID
        0,
        0,
        GetMaxX(),
        GetMaxY(),      // cover the whole screen
        0,              // do not display
        NULL,           // no text
        NULL
    );
}

/* this adds items on the items list of the list box */
void AddItemList(XCHAR *pText, LISTBOX *pLb)
{
    XCHAR   *pointer;
    WORD    ctr;

    if(pText != NULL)
    {
        pointer = pText;
        ctr = 0;
        while(*pointer)
        {
            if(NULL == LbAddItem(pLb, NULL, pointer, NULL, 0, ctr))
                break;
            while(*pointer++ > 31);
            if(*(pointer - 1) == 0)
                break;
            ctr++;
        }
    }
}

/* */
WORD MsgPullDown(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    LISTBOX *pLb2;
    SHORT   item;

    pLb2 = (LISTBOX *)GOLFindObject(ID_LISTBOX2);   // find the pull down object
    if(pMsg->uiEvent == EVENT_RELEASE)
    {                           // state transition must be done
        if(GetObjID(pObj) == ID_LISTBOX2)
        {                       // if released outside list box, go back
            refreshScreen = 1;  // refresh screen only when an item was
        }                       // selected

        return (1);             // if release was done outside the
        // list box, ignore the selection	
    }
    else
    {
        if((pMsg->uiEvent == EVENT_MOVE) || // convert messages to press
        (pMsg->uiEvent == EVENT_PRESS))
        {
            if(GetObjID(pObj) == ID_LISTBOX2)
            {                   // process only for list box(pull down menu)
                pMsg->uiEvent = EVENT_PRESS;                    // now convert the message
                item = LbGetFocusedItem(pLb2);                  // check item selected
                if((!List2Enable) && (item == 8))
                {                                               // if item MORE.... change menu
                    List2Enable = 1;
                    LbDelItemsList(pLb2);                       // remove the current item
                    AddItemList((XCHAR *)ScreenItems2, pLb2);   // set the items list to ScreenItems2
                    SetState(pLb2, LB_DRAW);
                }

                if((List2Enable) && (item == 0))
                {                       // if item BACK.... change back menu
                    List2Enable = 0;    // remove the current item
                    LbDelItemsList(pLb2);
                    AddItemList((XCHAR *)ScreenItems, pLb2);    // set the items list to ScreenItems
                    SetState(pLb2, LB_DRAW);
                }

                return (1);
            }
        }

        return (0);
    }
}

/* */
void DisplayPullDown(void)
{
    LISTBOX *pLb2;
    SHORT   item, left, top, right, bottom;

    if(refreshScreen)
    {
        pLb2 = (LISTBOX *)GOLFindObject(ID_LISTBOX2);   // find the pull down object
        item = LbGetFocusedItem(pLb2);                  // get selected page/screen
        if(!List2Enable)
        {                       // For list 1
            switch(item)
            {
                default:

                case 0:
                    screenState = CREATE_BUTTONS;
                    break;

                case 1:
                    screenState = CREATE_CHECKBOXES;
                    break;

                case 2:
                    screenState = CREATE_RADIOBUTTONS;
                    break;

                case 3:
                    screenState = CREATE_STATICTEXT;
                    break;

                case 4:
                    screenState = CREATE_SLIDER;
                    break;

                case 5:
                    screenState = CREATE_PROGRESSBAR;
                    break;

                case 6:
                    screenState = CREATE_LISTBOX;
                    break;

                case 7:
                    screenState = CREATE_EDITBOX;
                    break;

                case 8:
                    screenState = prevState;
                    break;
            }
        }
        else
        {
            switch(item)
            {                   // For list 2
                default:

                case 0:
                    screenState = prevState;
                    break;

                case 1:
                    screenState = CREATE_METER;
                    break;

                case 2:
                    screenState = CREATE_DIAL;
                    break;

                case 3:
                    screenState = CREATE_PICTURE;
                    break;

                case 4:
                    screenState = CREATE_CUSTOMCONTROL;
                    break;

                case 5:
                    screenState = CREATE_SIGNATURE;
                    break;

                case 6:
                    screenState = CREATE_POT;
                    break;

                case 7:
                    screenState = CREATE_ECG;
                    break;
            }
        }

        left = pLb2->hdr.left;
        top = pLb2->hdr.top;    // save the dimensions
        right = pLb2->hdr.right;
        bottom = pLb2->hdr.bottom;
        SetColor(pLb2->hdr.pGolScheme->CommonBkColor);      // set the color
        WAIT_UNTIL_FINISH(Bar(left, top, right, bottom));   // hide the pull down menu
        GOLFree();              // remove the pull down menu
        GOLSetList(pListSaved); // set active list back to saved list
        GOLRedrawRec(left, top, right, bottom);

        refreshScreen = 0;      // reset the flag to no refresh	
    }
}

/* */
void CreateButtons(void)
{
    #define BTN_ORIGIN_X    ((GetMaxX() - 258 + 1) / 2)
    #define BTN_ORIGIN_Y    ((40 + GetMaxY() - 177 + 1) / 2)

    GOLFree();                              // free memory for the objects in the previous linked list and start new list
    TransparentColorDisable();
    CreatePage(ButtonStr);                  // CreatePage("Buttons");
    BtnCreate
    (
        ID_BUTTON1,                         // button ID
        BTN_ORIGIN_X + 0,
        BTN_ORIGIN_Y + 7,
        BTN_ORIGIN_X + 126,
        BTN_ORIGIN_Y + 57,                  // dimension
        10,                                 // set radius
        BTN_DRAW,                           // draw a beveled button
        NULL,                               // no bitmap
        ButtonStr,                          // "Button",     	// text
        altScheme
    );                                      // use alternate scheme
    BtnCreate
    (
        ID_BUTTON2,                         // button ID
        BTN_ORIGIN_X + 0,
        BTN_ORIGIN_Y + 67,
        BTN_ORIGIN_X + 126,
        BTN_ORIGIN_Y + 117,                 // dimension
        0,
        BTN_DRAW,                           // will be dislayed after creation
        (void *) &gradientButton,           // use bitmap
        HomeStr,                            // "HOME", 	    // text
        altScheme
    );                                      // alternative GOL scheme
    BtnCreate
    (
        ID_BUTTON3,                         // button ID
        BTN_ORIGIN_X + 135,
        BTN_ORIGIN_Y + 7,
        BTN_ORIGIN_X + 185,
        BTN_ORIGIN_Y + 117,                 // dimension
        25,                                 // set radius
        BTN_DRAW | BTN_TOGGLE,              // draw a vertical capsule button
        // that has a toggle behavior
        NULL, // no bitmap
        LowStr,                             // "LO",       	// text
        yellowScheme
    );                          // use alternate scheme
    BtnCreate
    (
        ID_BUTTON4,             // button ID
        BTN_ORIGIN_X + 195,
        BTN_ORIGIN_Y + 0,
        BTN_ORIGIN_X + 255,
        BTN_ORIGIN_Y + 60,      // dimension
        30,                     // set radius
        BTN_DRAW,               // draw a vertical capsule button
        NULL,                   // no bitmap
        OnStr,                  // "ON",		// text
        greenScheme
    );                          // use alternate scheme
    BtnCreate
    (
        ID_BUTTON5,             // button ID
        BTN_ORIGIN_X + 195,
        BTN_ORIGIN_Y + 64,
        BTN_ORIGIN_X + 255,
        BTN_ORIGIN_Y + 124,     // dimension
        30,                     // set radius
        BTN_DRAW | BTN_PRESSED, // draw a vertical capsule button
        // that is initially pressed
        NULL,  // no bitmap
        OffStr,                             // "OFF",      	// text
        redScheme
    );                      // use alternate scheme 	
    BtnCreate
    (
        ID_BUTTON6,         // button ID
        BTN_ORIGIN_X + 0,
        BTN_ORIGIN_Y + 127,
        BTN_ORIGIN_X + 126,
        BTN_ORIGIN_Y + 177, // dimension
        0,
        BTN_DRAW | BTN_TEXTLEFT // will be dislayed after creation with text
        | BTN_TEXTTOP, // left top position
        (void *) &bulboff,          // use bitmap
        OffBulbStr,                 // text
        alt2Scheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_BUTTON7,                 // button ID
        BTN_ORIGIN_X + 132,
        BTN_ORIGIN_Y + 127,
        BTN_ORIGIN_X + 258,
        BTN_ORIGIN_Y + 177,         // dimension
        0,
        BTN_DRAW | BTN_DISABLED,    // will be dislayed and disabled after creation
        NULL,                       // no bitmap
        DisabledStr,                // "Disabled",  // text
        altScheme
    );                              // use alternate scheme
}

/* */
WORD MsgButtons(WORD objMsg, OBJ_HEADER *pObj)
{
    OBJ_HEADER  *pOtherRbtn;

    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_CHECKBOXES;    // goto check box demo screen
            }

            return (1);                             // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_ECG;           // goto ECG demo screen
            }

            return (1);                             // process by default

        case ID_BUTTON3:
            if(objMsg == BTN_MSG_PRESSED)
            {                           // change text and scheme
                BtnSetText((BUTTON *)pObj, HighStr);
            }
            else
            {
                BtnSetText((BUTTON *)pObj, LowStr);
            }

            return (1);                 // process by default

        case ID_BUTTON4:
            if(objMsg == BTN_MSG_PRESSED)
            {
                if(!GetState(pObj, BTN_PRESSED))
                {
                    pOtherRbtn = GOLFindObject(ID_BUTTON5);
                    ClrState(pOtherRbtn, BTN_PRESSED);
                    SetState(pOtherRbtn, BTN_DRAW);
                    SetState(pObj, BTN_PRESSED | BTN_DRAW);
                    GOLSetFocus(pObj);  // set focus for the button
                }
            }

            return (0);                 // Do not process by default

        case ID_BUTTON5:
            if(objMsg == BTN_MSG_PRESSED)
            {
                if(!GetState(pObj, BTN_PRESSED))
                {
                    pOtherRbtn = GOLFindObject(ID_BUTTON4);
                    ClrState(pOtherRbtn, BTN_PRESSED);
                    SetState(pOtherRbtn, BTN_DRAW);
                    SetState(pObj, BTN_PRESSED | BTN_DRAW);
                    GOLSetFocus(pObj);  // set focus for the button
                }
            }

            return (0);                 // Do not process by default

        case ID_BUTTON6:
            if(objMsg == BTN_MSG_PRESSED)
            {                           // change face picture
                BtnSetBitmap(pObj, (void *) &bulbon);
                BtnSetText((BUTTON *)pObj, OnBulbStr);
            }

            if((objMsg == BTN_MSG_RELEASED) || (objMsg == BTN_MSG_CANCELPRESS))
            {
                BtnSetBitmap(pObj, (void *) &bulboff);
                BtnSetText((BUTTON *)pObj, OffBulbStr);
                ClrState(pObj, 0x00F0);
                SetState(pObj, BTN_TEXTTOP | BTN_TEXTLEFT);
            }

            return (1);                 // process by default

        default:
            return (1);                 // process by default
    }
}

/* */
void CreateCheckBoxes(void)
{
    #define CB_ORIGIN_X ((GetMaxX() - 115 + 1) / 2)
    #define CB_ORIGIN_Y ((40 + GetMaxY() - 175 + 1) / 2)

    GOLFree();                  // free memory for the objects in the previous linked list and start new list
    CreatePage(CheckBoxStr);    // CreatePage("Checkboxes");
    CbCreate
    (
        ID_CHECKBOX1,           // ID
        CB_ORIGIN_X + 0,
        CB_ORIGIN_Y + 65,
        CB_ORIGIN_X + 165,
        CB_ORIGIN_Y + 100,      // dimension
        CB_DRAW,                // will be dislayed after creation
        TextLeftStr,            // "Text Left"
        altScheme
    );                          // alternative GOL scheme
    CbCreate
    (
        ID_CHECKBOX2,           // ID
        CB_ORIGIN_X + 0,
        CB_ORIGIN_Y + 105,
        CB_ORIGIN_X + 165,
        CB_ORIGIN_Y + 140,      // dimension
        CB_DRAW,                // will be dislayed and checked after creation
        TextBottomStr,          // "Text Bottom"
        altScheme
    );                          // alternative GOL scheme
    CbCreate
    (
        ID_CHECKBOX3,           // ID
        CB_ORIGIN_X + 0,
        CB_ORIGIN_Y + 145,
        CB_ORIGIN_X + 150,
        CB_ORIGIN_Y + 180,      // dimension
        CB_DRAW | CB_DISABLED,  // will be dislayed and disabled after creation
        DisabledStr,            // "Disabled"
        altScheme
    );                          // alternative GOL scheme
    pGenObj = (OBJ_HEADER *)BtnCreate
        (
            ID_BUTTON1,         // button ID
            CB_ORIGIN_X + 0,
            CB_ORIGIN_Y - 8,
            CB_ORIGIN_X + 115,
            CB_ORIGIN_Y + 62,   // dimension
            10,                 // set radius
            BTN_DRAW,           // draw a beveled button
            NULL,               // no bitmap
            HomeLinesStr,       // "Home1\nHome2" the string has two lines \n is the new line character
            altScheme
        );                      // use alternate scheme
}

/* */
WORD MsgCheckBoxes(WORD objMsg, OBJ_HEADER *pObj)
{
    switch(GetObjID(pObj))
    {
        case ID_CHECKBOX1:
            if(objMsg == CB_MSG_CHECKED)
            {
                SetState(pGenObj, BTN_TEXTLEFT | BTN_DRAW); // set align left and redraw button	
            }
            else
            {
                ClrState(pGenObj, BTN_TEXTLEFT);            // clear all text alignment	
                SetState(pGenObj, BTN_DRAW);                // set align left and redraw button	
            }

            return (1); // process by default

        case ID_CHECKBOX2:
            if(objMsg == CB_MSG_CHECKED)
            {
                SetState(pGenObj, BTN_TEXTBOTTOM | BTN_DRAW);   // set align bottom and redraw button	
            }
            else
            {
                ClrState(pGenObj, BTN_TEXTBOTTOM);              // clear all text alignment	
                SetState(pGenObj, BTN_DRAW);                    // set align left and redraw button	
            }

            return (1);                             // process by default

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_RADIOBUTTONS;  // goto radio buttons screen
            }

            return (1);                             // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_BUTTONS;       // goto round buttons screen
            }

            return (1);                             // process by default

        default:
            return (1);                             // process by default
    }
}

/* */
void CreateRadioButtons(void)
{
    #define RB_ORIGIN_X ((GetMaxX() - 260 + 1) / 2)
    #define RB_ORIGIN_Y ((40 + GetMaxY() - 185 + 1) / 2)

    GOLFree();                                  // free memory for the objects in the previous linked list and start new list
    CreatePage(RadioButtonStr);                 // CreatePage("Radio buttons");
    GbCreate
    (
        ID_GROUPBOX1,                           // ID
        RB_ORIGIN_X + 0,
        RB_ORIGIN_Y + 0,
        RB_ORIGIN_X + 127,
        RB_ORIGIN_Y + 185,                      // dimension
        GB_DRAW | GB_CENTER_ALIGN,              // will be dislayed after creation
        Group1Str,                              // "Group 1"
        alt2Scheme
    );                                          // alternate scheme
    GbCreate
    (
        ID_GROUPBOX2,                           // ID
        RB_ORIGIN_X + 130,
        RB_ORIGIN_Y + 0,
        RB_ORIGIN_X + 260,
        RB_ORIGIN_Y + 185,                      // dimension
        GB_DRAW | GB_CENTER_ALIGN,              // will be dislayed after creation
        Group2Str,                              // "Group 2"
        alt2Scheme
    );                                          // alternate scheme
    RbCreate
    (
        ID_RADIOBUTTON1,                        // ID
        RB_ORIGIN_X + 5,
        RB_ORIGIN_Y + 25,
        RB_ORIGIN_X + 125,
        RB_ORIGIN_Y + 60,                       // dimension
        RB_DRAW | RB_GROUP | RB_CHECKED,        // will be dislayed and checked after creation
        // first button in the group
        Rb1Str,    // "Rb1"
        altScheme
    );                          // alternative GOL scheme
    RbCreate
    (
        ID_RADIOBUTTON2,        // ID
        RB_ORIGIN_X + 5,
        RB_ORIGIN_Y + 65,
        RB_ORIGIN_X + 125,
        RB_ORIGIN_Y + 100,      // dimension
        RB_DRAW,                // will be dislayed
        Rb2Str,                 // "Rb2"
        altScheme
    );                          // alternative GOL scheme
    RbCreate
    (
        ID_RADIOBUTTON3,        // ID
        RB_ORIGIN_X + 5,
        RB_ORIGIN_Y + 105,
        RB_ORIGIN_X + 125,
        RB_ORIGIN_Y + 140,      // dimension
        RB_DRAW,                // will be dislayed after creation
        Rb3Str,                 // "Rb3"
        altScheme
    );                          // alternative GOL scheme
    RbCreate
    (
        ID_RADIOBUTTON4,        // ID
        RB_ORIGIN_X + 5,
        RB_ORIGIN_Y + 145,
        RB_ORIGIN_X + 125,
        RB_ORIGIN_Y + 180,      // dimension
        RB_DRAW | RB_DISABLED,  // will be dislayed and disabled after creation
        DisabledStr,            // "Disabled"
        altScheme
    );                          // alternative GOL scheme
    RbCreate
    (
        ID_RADIOBUTTON5,        // ID
        RB_ORIGIN_X + 135,
        RB_ORIGIN_Y + 25,
        RB_ORIGIN_X + 255,
        RB_ORIGIN_Y + 60,       // dimension
        RB_DRAW | RB_GROUP,     // will be dislayed and focused after creation
        // first button in the group
        Rb4Str,    // "Rb4"
        altScheme
    );                          // alternative GOL scheme
    RbCreate
    (
        ID_RADIOBUTTON6,        // ID
        RB_ORIGIN_X + 135,
        RB_ORIGIN_Y + 65,
        RB_ORIGIN_X + 255,
        RB_ORIGIN_Y + 100,      // dimension
        RB_DRAW | RB_CHECKED,   // will be dislayed and checked after creation
        Rb5Str,                 // "Rb5"
        altScheme
    );                          // alternative GOL scheme
    RbCreate
    (
        ID_RADIOBUTTON7,        // ID
        RB_ORIGIN_X + 135,
        RB_ORIGIN_Y + 105,
        RB_ORIGIN_X + 255,
        RB_ORIGIN_Y + 140,      // dimension
        RB_DRAW,                // will be dislayed after creation
        Rb6Str,                 // "Rb6"
        altScheme
    );                          // alternative GOL scheme
}

/* */
WORD MsgRadioButtons(WORD objMsg, OBJ_HEADER *pObj)
{
    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_STATICTEXT;    // goto static text screen
            }

            return (1);                             // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_CHECKBOXES;    // goto check boxes screen
            }

            return (1);                             // process by default

        default:
            return (1);                             // process by default
    }
}

/* */
void CreateStaticText(void)
{
    #define ST_ORIGIN_X ((GetMaxX() - 260 + 1) / 2)
    #define ST_ORIGIN_Y ((40 + GetMaxY() - 185 + 1) / 2)

    GOLFree();                                  // free memory for the objects in the previous linked list and start new list
    CreatePage(StaticTextStr);                  // CreatePage("Static text");
    GbCreate
    (
        ID_GROUPBOX1,                           // ID
        ST_ORIGIN_X + 0,
        ST_ORIGIN_Y + 0,
        ST_ORIGIN_X + 205,
        ST_ORIGIN_Y + 185,                      // dimension
        GB_DRAW,                                // will be dislayed after creation
        GroupBoxStr,                            // "Group Box"
        NULL
    );                                          // default GOL scheme
    StCreate
    (
        ID_STATICTEXT3,                         // ID
        ST_ORIGIN_X + 5,
        ST_ORIGIN_Y + 35,
        ST_ORIGIN_X + 195,
        ST_ORIGIN_Y + 175,                      // dimension
        ST_DRAW | ST_FRAME,                     // will be dislayed, has frame
        StaticTextLstStr,                       // "Microchip\nGraphics\nLibrary\nStatic Text and\nGroup Box Test.", // multi-line text
        altScheme
    );                                          // use alternate scheme scheme
    RbCreate
    (
        ID_RADIOBUTTON1,                        // ID
        ST_ORIGIN_X + 210,
        ST_ORIGIN_Y + 25,
        ST_ORIGIN_X + 260,
        ST_ORIGIN_Y + 55,                       // dimension
        RB_DRAW | RB_GROUP | RB_CHECKED,        // will be dislayed and checked after creation
        // first button in the group
        LeftStr,   // "Left"
        altScheme
    );                      // use alternate scheme
    RbCreate
    (
        ID_RADIOBUTTON2,    // ID
        ST_ORIGIN_X + 210,
        ST_ORIGIN_Y + 65,
        ST_ORIGIN_X + 260,
        ST_ORIGIN_Y + 95,   // dimension
        RB_DRAW,            // will be dislayed after creation
        CenterStr,          // "Center"
        altScheme
    );                      // use alternate scheme
    RbCreate
    (
        ID_RADIOBUTTON3,    // ID
        ST_ORIGIN_X + 210,
        ST_ORIGIN_Y + 105,
        ST_ORIGIN_X + 260,
        ST_ORIGIN_Y + 135,  // dimension
        RB_DRAW,            // will be dislayed after creation
        RightStr,           // "Right"
        altScheme
    );                      // use alternate scheme
}

/* */
WORD MsgStaticText(WORD objMsg, OBJ_HEADER *pObj)
{
    GROUPBOX    *pGb;
    STATICTEXT  *pSt;

    switch(GetObjID(pObj))
    {
        case ID_RADIOBUTTON1:   // change aligment to left
            pGb = (GROUPBOX *)GOLFindObject(ID_GROUPBOX1);      // get pointer to group box
            ClrState(pGb, GB_RIGHT_ALIGN | GB_CENTER_ALIGN);    // clear right and center alignment states
            SetState(pGb, GB_DRAW);                             // set redraw state
            pSt = (STATICTEXT *)GOLFindObject(ID_STATICTEXT3);  // get pointer to static text
            ClrState(pSt, ST_RIGHT_ALIGN | ST_CENTER_ALIGN);    // clear right and center alignment states
            SetState(pSt, ST_UPDATE);                           // set update state
            return (1); // process by default

        case ID_RADIOBUTTON2:
            pGb = (GROUPBOX *)GOLFindObject(ID_GROUPBOX1);      // get pointer to group box
            ClrState(pGb, GB_RIGHT_ALIGN);                      // clear right alignment state
            SetState(pGb, GB_CENTER_ALIGN | GB_DRAW);           // set center alignment and redraw states
            pSt = (STATICTEXT *)GOLFindObject(ID_STATICTEXT3);  // get pointer to static text
            ClrState(pSt, ST_RIGHT_ALIGN);                      // clear right aligment state
            SetState(pSt, ST_CENTER_ALIGN | ST_UPDATE);         // set center alignment and update  states
            return (1); // process by default

        case ID_RADIOBUTTON3:
            pGb = (GROUPBOX *)GOLFindObject(ID_GROUPBOX1);      // get pointer to group box
            ClrState(pGb, GB_CENTER_ALIGN);                     // clear center alignment state
            SetState(pGb, GB_RIGHT_ALIGN | GB_DRAW);            // set right alignment and redraw state
            pSt = (STATICTEXT *)GOLFindObject(ID_STATICTEXT3);  // get pointer to static text
            ClrState(pSt, ST_CENTER_ALIGN);                     // clear center aligment state
            SetState(pSt, ST_RIGHT_ALIGN | ST_UPDATE);          // set right alignment and update states
            return (1);                             // process by default

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_SLIDER;        // goto slider screen
            }

            return (1);                             // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_RADIOBUTTONS;  // goto radio buttons screen
            }

            return (1);                             // process by default

        default:
            return (1);
    }
}

/* */
void CreateSlider(void)
{
    GOLFree();                              // free memory for the objects in the previous linked list and start new list
    CreatePage(SliderStr);                  // CreatePage("Slider");
    GbCreate
    (
        ID_GROUPBOX1,                       // ID
        SLD_ORIGIN_X + 35,
        SLD_ORIGIN_Y + 0,
        SLD_ORIGIN_X + 225,
        SLD_ORIGIN_Y + 135,                 // dimension
        GB_DRAW,                            // will be dislayed after creation
        NULL,                               // no text
        altScheme
    );                                      // alternative GOL scheme
    SldCreate
    (
        ID_SLIDER1,                         // ID
        SLD_ORIGIN_X + 0,
        SLD_ORIGIN_Y + 140,
        SLD_ORIGIN_X + 260,
        SLD_ORIGIN_Y + 170,                 // dimension
        SLD_DRAW,                           // will be dislayed after creation
        CUR_BRD_RIGHT - CUR_BRD_LEFT,       // range
        10,                                 // page
        (CUR_BRD_RIGHT - CUR_BRD_LEFT) / 2, // pos
        altScheme
    );                      // alternative GOL scheme
    SldCreate
    (
        ID_SLIDER2,         // ID
        SLD_ORIGIN_X + 0,
        SLD_ORIGIN_Y + 0,
        SLD_ORIGIN_X + 30,
        SLD_ORIGIN_Y + 135, // dimension
        SLD_DRAW | SLD_VERTICAL | SLD_DISABLED, // vertical, will be dislayed and disabled after creation
        100,                                // range
        100,                                // page
        50,                                 // pos
        altScheme
    );                                      // alternative GOL scheme
    SldCreate
    (
        ID_SLIDER3,                         // ID
        SLD_ORIGIN_X + 230,
        SLD_ORIGIN_Y + 0,
        SLD_ORIGIN_X + 260,
        SLD_ORIGIN_Y + 135,                 // dimension
        SLD_DRAW | SLD_VERTICAL,            // vertical, will be dislayed after creation
        CUR_BRD_BOTTOM - CUR_BRD_TOP,       // range
        10,                                 // page
        (CUR_BRD_BOTTOM - CUR_BRD_TOP) / 2, // pos
        altScheme
    );  // alternative GOL scheme

    // Set cursor to the middle
    prevX = x = (CUR_BRD_RIGHT + CUR_BRD_LEFT) / 2; // cursor X position
    prevY = y = (CUR_BRD_BOTTOM + CUR_BRD_TOP) / 2; // cursor Y position
}

// Draw sliders' position cursor
void DrawSliderCursor(WORD color, SHORT xPos, SHORT yPos)
{
    SetColor(color);
    WAIT_UNTIL_FINISH(Bar(xPos - CUR_BAR_SIZE, yPos - CUR_BAR_SIZE, xPos + CUR_BAR_SIZE, yPos + CUR_BAR_SIZE));
    WAIT_UNTIL_FINISH(Line(xPos, CUR_BRD_TOP, xPos, CUR_BRD_BOTTOM));
    WAIT_UNTIL_FINISH(Line(CUR_BRD_LEFT, yPos, CUR_BRD_RIGHT, yPos));
}

// Process messages for slider screen
WORD MsgSlider(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    WORD    id;
    SLIDER  *pSld;

    switch(id = GetObjID(pObj))
    {
        case ID_WINDOW1:

            // check if window is touched in cursor box area
            if
            (
                (pMsg->param1 > CUR_BRD_LEFT) &&
                (pMsg->param1 < CUR_BRD_RIGHT) &&
                (pMsg->param2 > CUR_BRD_TOP) &&
                (pMsg->param2 < CUR_BRD_BOTTOM)
            )
            {

                // do some kind of debouncing so the lines on the screen will be stable.
                if ((abs((pMsg->param1 - x)) > 2) || (abs((pMsg->param2 - y)) > 2))
                {
                    // get new cursor x,y
                    x = pMsg->param1;
                    y = pMsg->param2;
                    cursorUpdate = 1;

                    // set new sliders positions
                    pSld = (SLIDER *)GOLFindObject(ID_SLIDER1); // get pointer for horizontal slider
                    SldSetPos(pSld, x - CUR_BRD_LEFT);          // set new position
                    SetState(pSld, SLD_DRAW_THUMB);             // redraw thumb only
                    pSld = (SLIDER *)GOLFindObject(ID_SLIDER3); // get pointer for vertical slider
                    SldSetPos(pSld, CUR_BRD_BOTTOM - y);        // set new position
                    SetState(pSld, SLD_DRAW_THUMB);             // redraw thumb only
                }    
            }

            return (1);                             // process by default

        case ID_SLIDER1:
        case ID_SLIDER3:

			if((objMsg == SLD_MSG_INC) || (objMsg == SLD_MSG_DEC)) 			
            {
	            // process message by default
	            SldMsgDefault(objMsg, (SLIDER *)pObj, pMsg);

	            // get new X position from horizontal slider
	            if(id == ID_SLIDER1)
	            {
	                x = CUR_BRD_LEFT + SldGetPos((SLIDER *)pObj);
	                if(x > CUR_BRD_RIGHT)
	                    x = CUR_BRD_RIGHT;
	            }
	
	            // get new Y position from vertical slider
	            if(id == ID_SLIDER3)
	            {
	                y = CUR_BRD_BOTTOM - SldGetPos((SLIDER *)pObj);
	                if(y < CUR_BRD_TOP)
	                    y = CUR_BRD_TOP;
	            }
	
	            cursorUpdate = 1;            }
            
            return (0);                             // it was processed

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_PROGRESSBAR;   // goto progress bar screen
            }

            return (1);                             // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_STATICTEXT;    // goto static text screen
            }

            return (1);                             // process by default

        default:
            return (1);                             // process by default
    }
}

// Creates progress bar screen
void CreateProgressBar(void)
{
    #define PB_ORIGIN_X ((GetMaxX() - 240 + 1) / 2)
    #define PB_ORIGIN_Y ((40 + GetMaxY() - 130 + 1) / 2)

    GOLFree();                      // free memory for the objects in the previous linked list and start new list
    CreatePage(ProgressBarStr);     // CreatePage("Progress bar");
    pProgressBar = PbCreate
        (
            ID_PROGRESSBAR1,        // ID
            PB_ORIGIN_X + 0,
            PB_ORIGIN_Y + 0,
            PB_ORIGIN_X + 240,
            PB_ORIGIN_Y + 50,       // dimension
            PB_DRAW,                // will be dislayed after creation
            25,                     // position
            50,                     // range
            NULL
        );                          // use default scheme
    StCreate
    (
        ID_STATICTEXT3,             // ID
        PB_ORIGIN_X + 30,
        PB_ORIGIN_Y + 70,
        PB_ORIGIN_X + 185,
        PB_ORIGIN_Y + 130,          // dimension
        ST_DRAW | ST_CENTER_ALIGN,  // display text
        SpeedStr,                   // text
        altScheme
    );                              // use alternate scheme
    pGenObj = (OBJ_HEADER *)BtnCreate
        (
            ID_BUTTON1,             // button ID
            PB_ORIGIN_X + 180,
            PB_ORIGIN_Y + 83,
            PB_ORIGIN_X + 220,
            PB_ORIGIN_Y + 113,      // dimension
            10,                     // set radius
            BTN_DRAW | BTN_TOGGLE,  // draw button
            NULL,                   // no bitmap
            QuestionStr,            // "?",               			// text
            altScheme
        );                          // use alternate scheme
}

// Processes messages for progress bar screen
WORD MsgProgressBar(WORD objMsg, OBJ_HEADER *pObj)
{
    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_LISTBOX;   // goto list box screen
            }

            return (1);                         // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_SLIDER;    // goto slider screen
            }

            return (1);                         // process by default

        default:
            return (1);                         // process by default
    }
}

XCHAR   *pMyItemList = ListboxLstStr;

// creates list box demo screen
void CreateListBox(void)
{
    LISTBOX *pLb;

    #define LB_ORIGIN_X ((GetMaxX() - 260 + 1) / 2)
    #define LB_ORIGIN_Y ((40 + GetMaxY() - 192 + 1) / 2)

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list
    CreatePage(ListBoxStr);                         // CreatePage("List box");
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            LB_ORIGIN_X + 10,
            LB_ORIGIN_Y + 52,
            LB_ORIGIN_X + 220,
            LB_ORIGIN_Y + 192,                      // dimension
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
            pMyItemList,
            altScheme
        );                                          // use alternate scheme
    SldCreate
    (
        ID_SLIDER1,                                 // ID
        LB_ORIGIN_X + 220,
        LB_ORIGIN_Y + 82,
        LB_ORIGIN_X + 250,
        LB_ORIGIN_Y + 160,                          // dimension
        SLD_DRAW | SLD_SCROLLBAR | SLD_VERTICAL,    // vertical, will be dislayed after creation
        LbGetCount(pLb),                            // range
        1,                          // page
        LbGetCount(pLb) - 1,        // pos
        altScheme
    );                              // use alternate scheme
    BtnCreate
    (
        ID_BUTTON1,                 // ID
        LB_ORIGIN_X + 220,
        LB_ORIGIN_Y + 52,
        LB_ORIGIN_X + 250,
        LB_ORIGIN_Y + 82,
        0,                          // dimension (no radius)
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        UpArrowStr,                 // Up Arrow
        altScheme
    );                              // use alternate scheme
    BtnCreate
    (
        ID_BUTTON2,                 // ID
        LB_ORIGIN_X + 220,
        LB_ORIGIN_Y + 162,
        LB_ORIGIN_X + 250,
        LB_ORIGIN_Y + 192,
        0,                          // dimension (no radius)
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        DownArrowStr,               // Down Arrow
        altScheme
    );                              // use alternate scheme
    CbCreate
    (
        ID_CHECKBOX1,               // ID
        LB_ORIGIN_X + 10,
        LB_ORIGIN_Y + 20,
        LB_ORIGIN_X + 110,
        LB_ORIGIN_Y + 45,           // dimension
        CB_DRAW,                    // will be dislayed after creation
        SingleStr,                  // "Single"
        altScheme
    );                              // alternative GOL scheme
    CbCreate
    (
        ID_CHECKBOX2,               // ID
        LB_ORIGIN_X + 140,
        LB_ORIGIN_Y + 20,
        LB_ORIGIN_X + 240,
        LB_ORIGIN_Y + 45,           // dimension
        CB_DRAW,                    // will be dislayed after creation
        AlignCenterStr,             // "Center"
        altScheme
    );                              // alternative GOL scheme
    GbCreate
    (
        ID_GROUPBOX1,               // ID
        LB_ORIGIN_X + 0,
        LB_ORIGIN_Y + 0,
        LB_ORIGIN_X + 127,
        LB_ORIGIN_Y + 50,           // dimension
        GB_DRAW | GB_CENTER_ALIGN,  // will be dislayed after creation
        SelectionStr,               // "Selection"
        alt4Scheme
    );                              // alternate scheme
    GbCreate
    (
        ID_GROUPBOX2,               // ID
        LB_ORIGIN_X + 130,
        LB_ORIGIN_Y + 0,
        LB_ORIGIN_X + 260,
        LB_ORIGIN_Y + 50,           // dimension
        GB_DRAW | GB_CENTER_ALIGN,  // will be dislayed after creation
        AlignmentStr,               // "Alignment"
        alt4Scheme
    );                              // alternate scheme
}

// processes messages for list box demo screen
WORD MsgListBox(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    LISTBOX *pLb;
    SLIDER  *pSld;

    pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);
    pSld = (SLIDER *)GOLFindObject(ID_SLIDER1);

    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // Set new list box position
            SldSetPos(pSld, LbGetCount(pLb) - LbGetFocusedItem(pLb));
            SetState(pSld, SLD_DRAW_THUMB);

            // The message was processed
            return (0);

        case ID_SLIDER1:

			if((objMsg == SLD_MSG_INC) || (objMsg == SLD_MSG_DEC)) 			
            {
	            // Process message by default
	            SldMsgDefault(objMsg, (SLIDER *)pObj, pMsg);
	
	            // Set new list box position
	            if(LbGetFocusedItem(pLb) != LbGetCount(pLb) - SldGetPos(pSld))
	            {
	                LbSetFocusedItem(pLb, LbGetCount(pLb) - SldGetPos(pSld));
	                SetState(pLb, LB_DRAW_ITEMS);
	            }
			} 

            // The message was processed
            return (0);

        case ID_BUTTON1:
            if(objMsg == BTN_MSG_RELEASED)
            {
                LbSetFocusedItem(pLb, LbGetFocusedItem(pLb) - 1);
                SetState(pLb, LB_DRAW_ITEMS);
                SldSetPos(pSld, SldGetPos(pSld) + 1);
                SetState(pSld, SLD_DRAW_THUMB);
            }

            return (1);

        case ID_BUTTON2:
            if(objMsg == BTN_MSG_RELEASED)
            {
                LbSetFocusedItem(pLb, LbGetFocusedItem(pLb) + 1);
                SetState(pLb, LB_DRAW_ITEMS);
                SldSetPos(pSld, SldGetPos(pSld) - 1);
                SetState(pSld, SLD_DRAW_THUMB);
            }

            return (1);

        case ID_CHECKBOX1:
            if(objMsg == CB_MSG_CHECKED)
            {
                SetState(pLb, LB_SINGLE_SEL | LB_DRAW);
                LbChangeSel(pLb, pLb->pFocusItem);
            }
            else
            {
                ClrState(pLb, LB_SINGLE_SEL);
            }

            return (1);

        case ID_CHECKBOX2:
            if(objMsg == CB_MSG_CHECKED)
            {
                SetState(pLb, LB_CENTER_ALIGN);
            }
            else
            {
                ClrState(pLb, LB_CENTER_ALIGN);
            }

            SetState(pLb, LB_DRAW);
            return (1);

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_EDITBOX;       // goto edit box screen
            }

            return (1);                             // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_PROGRESSBAR;   // goto progress bar screen
            }

            return (1);                             // process by default

        default:
            return (1);                             // process by default
    }
}

// Creates the edit box demo screen
void CreateEditBox(void)
{
    static XCHAR    pTempXchar[][2] =  {   {'0',0},
        {'1',0},
        {'2',0},
        {'3',0},
        {'4',0},
        {'5',0},
        {'6',0},
        {'7',0},
        {'8',0},
        {'9',0},
        {'*',0},
        {'#',0}
    };

    GOLFree();          // free memory for the objects in the previous linked list and start new list
    CreatePage(EditBoxStr);

    #define EB_ORIGIN_X ((GetMaxX() - 212 + 1) / 2)
    #define EB_ORIGIN_Y ((40 + GetMaxY() - 195 + 1) / 2)
    #define KEYSTARTX   EB_ORIGIN_X
    #define KEYSTARTY   EB_ORIGIN_Y
    #define KEYSIZEX    53
    #define KEYSIZEY    39
    #define MAXCHARSIZE 17

    TransparentColorEnable(0x44); // this is the index of the background color of the red and green phones.

    EbCreate
    (
        ID_EDITBOX1,                // ID
        KEYSTARTX,
        KEYSTARTY + 1,
        KEYSTARTX + 4 * KEYSIZEX,
        KEYSTARTY + 1 * KEYSIZEY - GOL_EMBOSS_SIZE, // dimension
        EB_DRAW,                    // will be dislayed after creation
        NULL,
        MAXCHARSIZE,
        altScheme
    );                              // default GOL scheme
    BtnCreate
    (
        ID_KEYPAD + 0,              // ID
        KEYSTARTX + 1 * KEYSIZEX,
        KEYSTARTY + 4 * KEYSIZEY,
        KEYSTARTX + 2 * KEYSIZEX,
        KEYSTARTY + 5 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        pTempXchar[0],              // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_KEYPAD + 1,              // ID
        KEYSTARTX + 0 * KEYSIZEX,
        KEYSTARTY + 1 * KEYSIZEY,
        KEYSTARTX + 1 * KEYSIZEX,
        KEYSTARTY + 2 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        pTempXchar[1],              // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_KEYPAD + 2,              // ID
        KEYSTARTX + 1 * KEYSIZEX,
        KEYSTARTY + 1 * KEYSIZEY,
        KEYSTARTX + 2 * KEYSIZEX,
        KEYSTARTY + 2 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        pTempXchar[2],              // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_KEYPAD + 3,              // ID
        KEYSTARTX + 2 * KEYSIZEX,
        KEYSTARTY + 1 * KEYSIZEY,
        KEYSTARTX + 3 * KEYSIZEX,
        KEYSTARTY + 2 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        pTempXchar[3],              // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_KEYPAD + 4,              // ID
        KEYSTARTX + 0 * KEYSIZEX,
        KEYSTARTY + 2 * KEYSIZEY,
        KEYSTARTX + 1 * KEYSIZEX,
        KEYSTARTY + 3 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        pTempXchar[4],              // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_KEYPAD + 5,              // ID
        KEYSTARTX + 1 * KEYSIZEX,
        KEYSTARTY + 2 * KEYSIZEY,
        KEYSTARTX + 2 * KEYSIZEX,
        KEYSTARTY + 3 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        pTempXchar[5],              // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_KEYPAD + 6,              // ID
        KEYSTARTX + 2 * KEYSIZEX,
        KEYSTARTY + 2 * KEYSIZEY,
        KEYSTARTX + 3 * KEYSIZEX,
        KEYSTARTY + 3 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        pTempXchar[6],              // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_KEYPAD + 7,              // ID
        KEYSTARTX + 0 * KEYSIZEX,
        KEYSTARTY + 3 * KEYSIZEY,
        KEYSTARTX + 1 * KEYSIZEX,
        KEYSTARTY + 4 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        pTempXchar[7],              // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_KEYPAD + 8,              // ID
        KEYSTARTX + 1 * KEYSIZEX,
        KEYSTARTY + 3 * KEYSIZEY,
        KEYSTARTX + 2 * KEYSIZEX,
        KEYSTARTY + 4 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        pTempXchar[8],              // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_KEYPAD + 9,              // ID
        KEYSTARTX + 2 * KEYSIZEX,
        KEYSTARTY + 3 * KEYSIZEY,
        KEYSTARTX + 3 * KEYSIZEX,
        KEYSTARTY + 4 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        pTempXchar[9],              // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_ASTERISK,                // ID
        KEYSTARTX + 0 * KEYSIZEX,
        KEYSTARTY + 4 * KEYSIZEY,
        KEYSTARTX + 1 * KEYSIZEX,
        KEYSTARTY + 5 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        pTempXchar[10],             // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_POUND,                   // ID
        KEYSTARTX + 2 * KEYSIZEX,
        KEYSTARTY + 4 * KEYSIZEY,
        KEYSTARTX + 3 * KEYSIZEX,
        KEYSTARTY + 5 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        pTempXchar[11],             // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_BACKSPACE,               // ID
        KEYSTARTX + 3 * KEYSIZEX,
        KEYSTARTY + 1 * KEYSIZEY,
        KEYSTARTX + 4 * KEYSIZEX,
        KEYSTARTY + 2 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        LeftArrowStr,               // Left Arrow
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_CALL,                    // ID
        KEYSTARTX + 3 * KEYSIZEX,
        KEYSTARTY + 2 * KEYSIZEY,
        KEYSTARTX + 4 * KEYSIZEX,
        KEYSTARTY + 3 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        (void *) &greenphone,       // use green phone bitmap
        NULL,                       // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_STOPCALL,                // ID
        KEYSTARTX + 3 * KEYSIZEX,
        KEYSTARTY + 3 * KEYSIZEY,
        KEYSTARTX + 4 * KEYSIZEX,
        KEYSTARTY + 4 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        (void *) &redphone,         // use redphone bitmap
        NULL,                       // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_HOLD,                    // ID
        KEYSTARTX + 3 * KEYSIZEX,
        KEYSTARTY + 4 * KEYSIZEY,
        KEYSTARTX + 4 * KEYSIZEX,
        KEYSTARTY + 5 * KEYSIZEY,   // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap
        HoldStr,                    // "Hld"
        altScheme
    );                              // alternative GOL scheme
}

// Processes messages for the edit box demo screen
WORD MsgEditBox(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    EDITBOX     *pEb;
    SHORT       id;
    XCHAR       temp;
    static char status = 0; // status to check if calling, holding or not
    id = GetObjID(pObj);

    // ignore all touch screen messages to the edit box
    // since we do not want the caret to show up
    if ((id == ID_EDITBOX1) && (pMsg->type == TYPE_TOUCHSCREEN))
    {
        return 0;
    }

    // If number key is pressed
    if(objMsg == BTN_MSG_RELEASED)
    {
        if(id >= ID_KEYPAD)
            if(id < ID_KEYPAD + 10)
            {
                if(!status)
                {
                    pEb = (EDITBOX *)GOLFindObject(ID_EDITBOX1);
                    EbAddChar(pEb, '0' + id - ID_KEYPAD);
                    SetState(pEb, EB_DRAW);
                }

                return (1);
            }

	    switch(id)
	    {
	        case ID_CALL:
                pEb = (EDITBOX *)GOLFindObject(ID_EDITBOX1);
                EbSetText(pEb, (XCHAR *)CallingStr);
                SetState(pEb, EB_DRAW);
                status = 1;
                break;
	
	        case ID_STOPCALL:
                pEb = (EDITBOX *)GOLFindObject(ID_EDITBOX1);
                temp = 0x0000;
                EbSetText(pEb, &temp);
                SetState(pEb, EB_DRAW);
                status = 0;
                break;
	
	        case ID_BACKSPACE:
                if(!status)
                {
                    pEb = (EDITBOX *)GOLFindObject(ID_EDITBOX1);
                    EbDeleteChar(pEb);
                    SetState(pEb, EB_DRAW);
                }
                break;
	
	        case ID_HOLD:
                pEb = (EDITBOX *)GOLFindObject(ID_EDITBOX1);
                if(status == 1)
                {
                    EbSetText(pEb, (XCHAR *)HoldingStr);
                    status = 2;
                }
                else if(status == 2)
                {
                    EbSetText(pEb, (XCHAR *)CallingStr);
                    status = 1;
                }
	
                SetState(pEb, EB_DRAW);
                break;
	
	        case ID_ASTERISK:
	            if(!status)
	            {
                    pEb = (EDITBOX *)GOLFindObject(ID_EDITBOX1);
                    EbAddChar(pEb, (XCHAR) '*');
                    SetState(pEb, EB_DRAW);
	            }
                break;
	
	        case ID_POUND:
	            if(!status)
	            {
                    pEb = (EDITBOX *)GOLFindObject(ID_EDITBOX1);
                    EbAddChar(pEb, (XCHAR) '#');
                    SetState(pEb, EB_DRAW);
	            }
                break;
	
	        case ID_BUTTON_NEXT:
                screenState = CREATE_METER;     // goto meter screen
	            status = 0;
                break;
	
	        case ID_BUTTON_BACK:
                screenState = CREATE_LISTBOX;   // goto list box screen
	            status = 0;
                break;
	
	        default:
                break;
	    }
	} 
	return (1);                         		// process by default
}

/*********************************************************************************/

/*********************************************************************************/

/*********************************************************************************/

/*********************************************************************************/

//			START OF DATE AND TIME SETTING

/*********************************************************************************/
#define DT_ORIGIN_X     ((GetMaxX() - 212 + 1) / 2)
#define DT_ORIGIN_Y     ((40 + GetMaxY() - 195 + 1) / 2)
#define EB1STARTX       30
#define EB2STARTX       122
#define EB3STARTX       200
#define EBSTARTY        55
#define EB4STARTX       40
#define EB5STARTX       125
#define EB6STARTX       210
#define EBSTARTY2       110

#define SELECTBTNWIDTH  22

// Items list
// Months Items list
const XCHAR MonthItems[] = {'J','a','n',0x000A,
    'F','e','b',0x000A,
    'M','a','r',0x000A,
    'A','p','r',0x000A,
    'M','a','y',0x000A,
    'J','u','n',0x000A,
    'J','u','l',0x000A,
    'A','u','g',0x000A,
    'S','e','p',0x000A,
    'O','c','t',0x000A,
    'N','o','v',0x000A,
    'D','e','c',0x0000};

// Days Items list
const XCHAR DayItems[] = {'0','1',0x000A, '0','2',0x000A, '0','3',0x000A, '0','4',0x000A, '0','5',0x000A,
    '0','6',0x000A, '0','7',0x000A, '0','8',0x000A, '0','9',0x000A, '1','0',0x000A,
    '1','1',0x000A, '1','2',0x000A, '1','3',0x000A, '1','4',0x000A, '1','5',0x000A,
    '1','6',0x000A, '1','7',0x000A, '1','8',0x000A, '1','9',0x000A, '2','0',0x000A,
    '2','1',0x000A, '2','2',0x000A, '2','3',0x000A, '2','4',0x000A, '2','5',0x000A,
    '2','6',0x000A, '2','7',0x000A, '2','8',0x000A, '2','9',0x000A, '3','0',0x000A, '3','1',0x0000};

// Year Items list
const XCHAR YearItems[] = {'0','0',0x000A, '0','1',0x000A, '0','2',0x000A, '0','3',0x000A, '0','4',0x000A,
    '0','5',0x000A, '0','6',0x000A, '0','7',0x000A, '0','8',0x000A, '0','9',0x000A,
    '1','0',0x000A, '1','1',0x000A, '1','2',0x000A, '1','3',0x000A, '1','4',0x000A,
    '1','5',0x000A, '1','6',0x000A, '1','7',0x000A, '1','8',0x000A, '1','9',0x000A, '2','0',0x0000};

XCHAR       *DTSetText = SetDateTimeStr;
XCHAR       DTPlusSym[] = {'+',0};
XCHAR       DTMinusSym[] = {'-',0};
XCHAR       *DTDoneText = ExitStr;
XCHAR       *DTDownArrow = DownArrowStr;
XCHAR       *DTUpArrow = UpArrowStr;

// Creates date and time screen
void CreateDateTime(void)
{
    WORD    textWidth, textHeight, tempStrWidth;
    XCHAR   tempStr[] = {'M',0};

    // free memory for the objects in the previous linked list and start new list
    GOLFree();
    RTCCProcessEvents();    // update the global time and date strings

    /* ************************************* */

    /* Create Month, Day and Year Edit Boxes */

    /* ************************************* */
    tempStrWidth = GetTextWidth(tempStr, altScheme->pFont);
    textHeight = GetTextHeight(altScheme->pFont) + (GOL_EMBOSS_SIZE << 1);

    WndCreate
    (
        ID_WINDOW1,         // ID
        0,
        0,
        GetMaxX(),
        GetMaxY(),          // dimension
        WND_DRAW,           // will be dislayed after creation
        (void *) &mchpIcon, // icon
        DTSetText,          // set text
        navScheme
    );                      // use default scheme

    /* *************************** */

    // create month components

    /* *************************** */

    // months has three characters, thus we multiply by three
    textWidth = (tempStrWidth * 3) + (GOL_EMBOSS_SIZE << 1);

    EbCreate
    (
        ID_EB_MONTH,        // ID
        EB1STARTX,
        EBSTARTY,
        EB1STARTX + textWidth,
        EBSTARTY + textHeight,
        EB_DRAW,            // will be dislayed after creation
        NULL,
        3,                  // max characters is 3
        altScheme
    );                      // use alternate scheme
    BtnCreate
    (
        ID_BUTTON_MO,       // button ID
        EB1STARTX + textWidth + 1,
        EBSTARTY,
        EB1STARTX + textWidth + SELECTBTNWIDTH + 1,
        EBSTARTY + textHeight,
        0,                  // draw rectangular button	
        BTN_DRAW,           // will be dislayed after creation
        NULL,               // no bitmap	
        DTDownArrow,        // DOWN arrow as text
        altScheme
    );                      // use alternate scheme

    /* *************************** */

    // create day components

    /* *************************** */

    // day has two characters, thus we multiply by two
    textWidth = (tempStrWidth * 2) + (GOL_EMBOSS_SIZE << 1);

    EbCreate
    (
        ID_EB_DAY,          // ID
        EB2STARTX,
        EBSTARTY,
        EB2STARTX + textWidth,
        EBSTARTY + textHeight,
        EB_DRAW,            // will be dislayed after creation
        NULL,
        2,                  // max characters is 3
        altScheme
    );                      // use alternate scheme
    BtnCreate
    (
        ID_BUTTON_DY,       // button ID
        EB2STARTX + textWidth + 1,
        EBSTARTY,
        EB2STARTX + textWidth + SELECTBTNWIDTH + 1,
        EBSTARTY + textHeight,
        0,                  // draw rectangular button	
        BTN_DRAW,           // will be dislayed after creation
        NULL,               // no bitmap	
        DTDownArrow,        // DOWN arrow as text
        altScheme
    );                      // use alternate scheme

    /* *************************** */

    // create year components

    /* *************************** */

    // year has four characters, thus we multiply by four
    textWidth = (tempStrWidth * 4) + (GOL_EMBOSS_SIZE << 1);

    EbCreate
    (
        ID_EB_YEAR,         // ID
        EB3STARTX,
        EBSTARTY,
        EB3STARTX + textWidth,
        EBSTARTY + textHeight,
        EB_DRAW,            // will be dislayed after creation
        NULL,
        4,                  // max characters is 4
        altScheme
    );                      // use alternate scheme
    BtnCreate
    (
        ID_BUTTON_YR,       // button ID
        EB3STARTX + textWidth + 1,
        EBSTARTY,
        EB3STARTX + textWidth + SELECTBTNWIDTH + 1,
        EBSTARTY + textHeight,
        0,                  // draw rectangular button	
        BTN_DRAW,           // will be dislayed after creation
        NULL,               // no bitmap	
        DTDownArrow,        // DOWN arrow as text
        altScheme
    );                      // use alternate scheme

    /* ****************************************** */

    /* Create Hour, Minute and Seconds Edit Boxes */

    /* ****************************************** */
    RTCCProcessEvents();    // update the time and date
    textHeight = (GetTextHeight(altScheme->pFont) << 1);
    tempStrWidth = GetTextWidth(tempStr, altScheme->pFont);

    /* *************************** */

    // create hour components

    /* *************************** */

    // items here have 2 characters each so we use 2 as multiplier
    textWidth = (tempStrWidth * 2) + (GOL_EMBOSS_SIZE << 1);

    EbCreate
    (
        ID_EB_HOUR,         // ID
        EB4STARTX,
        EBSTARTY2,
        EB4STARTX + textWidth,
        EBSTARTY2 + textHeight,
        EB_DRAW,            // will be dislayed after creation
        NULL,
        2,                  // max characters is 2
        altScheme
    );                      // use alternate scheme
    BtnCreate
    (
        ID_BUTTONHR_P,      // button ID
        EB4STARTX + textWidth + 1,
        EBSTARTY2,
        EB4STARTX + textWidth + SELECTBTNWIDTH + 1,
        EBSTARTY2 + (textHeight >> 1),
        0,                  // draw rectangular button	
        BTN_DRAW,           // will be dislayed after creation
        NULL,               // no bitmap	
        DTPlusSym,          // plus symbol as text
        altScheme
    );                      // use alternate scheme
    BtnCreate
    (
        ID_BUTTONHR_M,      // button ID
        EB4STARTX + textWidth + 1,
        EBSTARTY2 + (textHeight >> 1) + 1,
        EB4STARTX + textWidth + SELECTBTNWIDTH + 1,
        EBSTARTY2 + textHeight,
        0,                  // draw rectangular button	
        BTN_DRAW,           // will be dislayed after creation
        NULL,               // no bitmap	
        DTMinusSym,         // minus symbols as text
        altScheme
    );                      // use alternate scheme

    /* *************************** */

    // create minute components

    /* *************************** */
    EbCreate
    (
        ID_EB_MINUTE,       // ID
        EB5STARTX,
        EBSTARTY2,
        EB5STARTX + textWidth,
        EBSTARTY2 + textHeight,
        EB_DRAW,            // will be dislayed after creation
        NULL,
        2,                  // max characters is 2
        altScheme
    );                      // use alternate scheme
    BtnCreate
    (
        ID_BUTTONMN_P,      // button ID
        EB5STARTX + textWidth + 1,
        EBSTARTY2,
        EB5STARTX + textWidth + SELECTBTNWIDTH + 1,
        EBSTARTY2 + (textHeight >> 1),
        0,                  // draw rectangular button	
        BTN_DRAW,           // will be dislayed after creation
        NULL,               // no bitmap	
        DTPlusSym,          // plus symbol as text
        altScheme
    );                      // use alternate scheme
    BtnCreate
    (
        ID_BUTTONMN_M,      // button ID
        EB5STARTX + textWidth + 1,
        EBSTARTY2 + (textHeight >> 1) + 1,
        EB5STARTX + textWidth + SELECTBTNWIDTH + 1,
        EBSTARTY2 + textHeight,
        0,                  // draw rectangular button	
        BTN_DRAW,           // will be dislayed after creation
        NULL,               // no bitmap	
        DTMinusSym,         // minus symbols as text
        altScheme
    );                      // use alternate scheme

    /* *************************** */

    // create seconds components

    /* *************************** */
    EbCreate
    (
        ID_EB_SECOND,       // ID
        EB6STARTX,
        EBSTARTY2,
        EB6STARTX + textWidth,
        EBSTARTY2 + textHeight,
        EB_DRAW,            // will be dislayed after creation
        NULL,
        2,                  // max characters is 2
        altScheme
    );                      // use alternate scheme
    BtnCreate
    (
        ID_BUTTONSC_P,      // button ID
        EB6STARTX + textWidth + 1,
        EBSTARTY2,
        EB6STARTX + textWidth + SELECTBTNWIDTH + 1,
        EBSTARTY2 + (textHeight >> 1),
        0,                  // draw rectangular button	
        BTN_DRAW,           // will be dislayed after creation
        NULL,               // no bitmap	
        DTPlusSym,          // plus symbol as text
        altScheme
    );                      // use alternate scheme
    BtnCreate
    (
        ID_BUTTONSC_M,      // button ID
        EB6STARTX + textWidth + 1,
        EBSTARTY2 + (textHeight >> 1) + 1,
        EB6STARTX + textWidth + SELECTBTNWIDTH + 1,
        EBSTARTY2 + textHeight,
        0,                  // draw rectangular button	
        BTN_DRAW,           // will be dislayed after creation
        NULL,               // no bitmap	
        DTMinusSym,         // minus symbols as text
        altScheme
    );                      // use alternate scheme

    //-----------------------------------------------------
    // Done Button
    textHeight = GetTextHeight(altScheme->pFont) + (GOL_EMBOSS_SIZE << 1) + 5;

    BtnCreate
    (
        ID_BUTTON4,         // button ID
        EB6STARTX,
        175,
        EB6STARTX + textWidth + SELECTBTNWIDTH,
        175 + textHeight,
        8,                  // draw rectangular button	
        BTN_DRAW,           // will be dislayed after creation
        NULL,               // no bitmap	
        DTDoneText,         // set text
        altScheme
    );                      // use alternate scheme
    updateDateTimeEb();
}

/* */
void updateDateTimeEb(void)
{
    OBJ_HEADER  *obj;
    SHORT       i;

    RTCCProcessEvents();        // update the time and date
    i = 0;
    while(i < 12)
    {
        dateTimeStr[i] = _time_str[i];
        dateTimeStr[i + 13] = _date_str[i];
        i++;
    }

    dateTimeStr[12] = 0x000A;   // (XCHAR)'\n';
    dateTimeStr[25] = 0x0000;   // (XCHAR)'\0';

    // update date edit boxes
    obj = GOLFindObject(ID_EB_MONTH);
    if(obj)
    {
        EbSetText((EDITBOX *)obj, &dateTimeStr[13]);
        SetState(obj, EB_DRAW);
    }

    obj = GOLFindObject(ID_EB_DAY);
    if(obj)
    {
        EbSetText((EDITBOX *)obj, &dateTimeStr[17]);
        SetState(obj, EB_DRAW);
    }

    obj = GOLFindObject(ID_EB_YEAR);
    if(obj)
    {
        EbSetText((EDITBOX *)obj, &dateTimeStr[21]);
        SetState(obj, EB_DRAW);
    }

    // update time edit boxes
    obj = GOLFindObject(ID_EB_HOUR);
    if(obj)
    {
        EbSetText((EDITBOX *)obj, &dateTimeStr[4]);
        SetState(obj, EB_DRAW);
    }

    obj = GOLFindObject(ID_EB_MINUTE);
    if(obj)
    {
        EbSetText((EDITBOX *)obj, &dateTimeStr[7]);
        SetState(obj, EB_DRAW);
    }

    obj = GOLFindObject(ID_EB_SECOND);
    if(obj)
    {
        EbSetText((EDITBOX *)obj, &dateTimeStr[10]);
        SetState(obj, EB_DRAW);
    }
}

/* this will show the pull down menus */
void ShowPullDownMenu(void)
{
    WORD    textWidth = 0, textHeight, tempStrWidth;
    LISTBOX *pLb;
    XCHAR   *pItems = NULL;
    XCHAR   tempString[] = {'M',0};

    pListSaved = GOLGetList();
    GOLNewList();

    textHeight = GetTextHeight(altScheme->pFont) + (GOL_EMBOSS_SIZE << 1);

    // Note: pDwnBottom, pDwnTop, pDwnLeft and pDwnRight are globals that will
    // 		 be used to redraw the area where the pulldown menu covered.
    pDwnBottom = 230;
    pDwnTop = EBSTARTY;

    tempStrWidth = GetTextWidth(tempString, altScheme->pFont);
    switch(DateItemID)
    {
        case ID_EB_MONTH:
            textWidth = (tempStrWidth * 3) + (GOL_EMBOSS_SIZE << 1);
            pDwnLeft = EB1STARTX;
            pDwnRight = EB1STARTX + textWidth + SELECTBTNWIDTH;
            pItems = (XCHAR *)MonthItems;
            break;

        case ID_EB_DAY:
            textWidth = (tempStrWidth * 2) + (GOL_EMBOSS_SIZE << 1);
            pDwnLeft = EB2STARTX;
            pDwnRight = EB2STARTX + textWidth + SELECTBTNWIDTH;
            pItems = (XCHAR *)DayItems;
            break;

        case ID_EB_YEAR:
            textWidth = (tempStrWidth * 4) + (GOL_EMBOSS_SIZE << 1);
            pDwnLeft = EB3STARTX;
            pDwnRight = EB3STARTX + textWidth + SELECTBTNWIDTH;
            pItems = (XCHAR *)YearItems;
            break;
    }

    pLb = (LISTBOX *)LbCreate
        (
            ID_LISTBOX1,                            // List Box ID
            pDwnLeft,                               // left
            pDwnTop + textHeight,                   // top
            pDwnRight - SELECTBTNWIDTH,             // right
            pDwnBottom,                             // bottom dimension
            LB_DRAW | LB_SINGLE_SEL,                // draw after creation, single selection
            pItems,                                 // items list
            altScheme
        );

    SldCreate
    (
        ID_SLIDER1,                                 // Slider ID
        pDwnRight - SELECTBTNWIDTH + 1,
        pDwnTop + (textHeight << 1),
        pDwnRight,
        pDwnBottom - textHeight,
        SLD_DRAW | SLD_SCROLLBAR | SLD_VERTICAL,    // vertical, draw after creation
        LbGetCount(pLb),                            // range
        5,                      // page
        LbGetCount(pLb) - 1,    // pos
        altScheme
    );

    BtnCreate
    (
        ID_BUTTON_DATE_UP,      // up button ID
        pDwnRight - SELECTBTNWIDTH + 1,
        pDwnTop + textHeight,
        pDwnRight,
        pDwnTop + (textHeight << 1),
        0,
        BTN_DRAW,               // draw after creation
        NULL,                   // no bitmap
        DTUpArrow,              // text
        altScheme
    );

    BtnCreate
    (
        ID_BUTTON_DATE_DN,      // down button ID
        pDwnRight - SELECTBTNWIDTH + 1,
        pDwnBottom - textHeight,
        pDwnRight,
        pDwnBottom,
        0,
        BTN_DRAW,               // draw after creation
        NULL,                   // no bitmap
        DTDownArrow,            // text
        altScheme
    );
}

/* */
WORD RemovePullDownMenu(void)
{
    OBJ_HEADER  *pObjTemp;

    // check if objects has been hidden
    pObjTemp = GOLGetList();
    while(pObjTemp)
    {   // parse the active list
        if(GetState(pObjTemp, HIDE))    // an object is still not hidden
            return (0);                 // go back without changing state
        pObjTemp = pObjTemp->pNxtObj;   // go to next object
    }

    // all objects are hidden we can now remove the list	
    GOLFree();                          // remove the list of pull down menu
    GOLSetList(pListSaved);             // set active list back to saved list

    // redraw objects that were overlapped by pulldown menu
    GOLRedrawRec(pDwnLeft, pDwnTop, pDwnRight, pDwnBottom);
    // reset the window so it will not be redrawn
    ClrState(GOLFindObject(ID_WINDOW1), WND_DRAW);

    // must reset the pressed button, this code is more compact than searching
    // which one of the three is pressed.
    pObjTemp = GOLFindObject(ID_BUTTON_MO);
    ClrState(pObjTemp, BTN_PRESSED);
    pObjTemp = GOLFindObject(ID_BUTTON_YR);
    ClrState(pObjTemp, BTN_PRESSED);
    pObjTemp = GOLFindObject(ID_BUTTON_DY);
    ClrState(pObjTemp, BTN_PRESSED);

    updateDateTimeEb();                 // Update the edit boxes to reflect the new dates
    return (1);                         // go back normally 	
}

/* this selects the new month or day or year from the list box*/
WORD MsgSetDate(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    LISTBOX     *pLb;
    SLIDER      *pSld;
    OBJ_HEADER  *pObjHide;
    LISTITEM    *pItemSel;

    pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);
    pSld = (SLIDER *)GOLFindObject(ID_SLIDER1);

    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:
            if(pMsg->uiEvent == EVENT_MOVE)
            {
                pMsg->uiEvent = EVENT_PRESS;    // change event for listbox

                // Process message by default
                LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

                // Set new list box position
                SldSetPos(pSld, LbGetCount(pLb) - LbGetFocusedItem(pLb) - 1);
                SetState(pSld, SLD_DRAW_THUMB);
                pMsg->uiEvent = EVENT_MOVE;     // restore event for listbox
            }
            else if(pMsg->uiEvent == EVENT_PRESS)
            {

                // call the message default processing of the list box to select the item
                LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);
            }
            else if(pMsg->uiEvent == EVENT_RELEASE)
            {
                pObjHide = GOLGetList();        // hide all the objects in the current list
                while(pObjHide)
                {                               // set all objects with HIDE state
                    SetState(pObjHide, HIDE);
                    pObjHide = pObjHide->pNxtObj;
                }

                pItemSel = LbGetSel(pLb, NULL); // get the selected item
                if(pItemSel != NULL)            // process only when an item was selected
                    UpdateRTCCDates(pLb);       // update the RTCC values
                screenState = HIDE_DATE_PDMENU; // go to hide state
            }

            // The message was processed
            return (0);

        case ID_SLIDER1:

			if((objMsg == SLD_MSG_INC) || (objMsg == SLD_MSG_DEC)) 			
            {
	            // Process message by default
	            SldMsgDefault(objMsg, (SLIDER *)pObj, pMsg);
	
	            // Set new list box position
	            if(LbGetFocusedItem(pLb) != LbGetCount(pLb) - SldGetPos(pSld))
	            {
	                LbSetFocusedItem(pLb, LbGetCount(pLb) - SldGetPos(pSld));
	                SetState(pLb, LB_DRAW_ITEMS);
	            }
			}
            // The message was processed
            return (0);

        case ID_BUTTON_DATE_UP:                 // slider button up was pressed
            if(objMsg == BTN_MSG_RELEASED)
            {
                LbSetFocusedItem(pLb, LbGetFocusedItem(pLb) - 1);
                SetState(pLb, LB_DRAW_ITEMS);
                SldSetPos(pSld, SldGetPos(pSld) + 1);
                SetState(pSld, SLD_DRAW_THUMB);
            }

            return (1);

        case ID_BUTTON_DATE_DN:                 // slider button down was pressed
            if(objMsg == BTN_MSG_RELEASED)
            {
                LbSetFocusedItem(pLb, LbGetFocusedItem(pLb) + 1);
                SetState(pLb, LB_DRAW_ITEMS);
                SldSetPos(pSld, SldGetPos(pSld) - 1);
                SetState(pSld, SLD_DRAW_THUMB);
            }

            return (1);
    }

    return (1);
}

// Processes messages for date and time screen
WORD MsgDateTime(WORD objMsg, OBJ_HEADER *pObj)
{
    char    hour = 0, minute = 0, second = 0;
    WORD    btnID;
    WORD    ebID = 0, i;

    switch(btnID = GetObjID(pObj))
    {
        // ignore all touch screen messages to all edit boxes
        // since we do not want the caret to show up
        case ID_EB_MONTH:
        case ID_EB_DAY:
        case ID_EB_YEAR:
        case ID_EB_HOUR:  
        case ID_EB_MINUTE:
        case ID_EB_SECOND:
            return 0;

        case ID_BUTTON4:                    // exit setting of date and time
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = prevState;    // goto last state screen
                return (1);
            }

            break;

        //////////////////////////////////////////////////////////////////////////
        // 		Date update: this will create the pulldown menu
        		//////////////////////////////////////////////////////////////////////////
        case ID_BUTTON_MO:                  // month is selected show the pull down menu
            if(objMsg == BTN_MSG_PRESSED)
            {
                DateItemID = ID_EB_MONTH;
                return (1);
            }

            break;

        case ID_BUTTON_DY:                  // month is selected show the pull down menu
            if(objMsg == BTN_MSG_PRESSED)
            {
                DateItemID = ID_EB_DAY;
                return (1);
            }

            break;

        case ID_BUTTON_YR:                  // month is selected show the pull down menu
            if(objMsg == BTN_MSG_PRESSED)
            {
                DateItemID = ID_EB_YEAR;
                return (1);
            }

            break;

        //////////////////////////////////////////////////////////////////////////
        // 		Time update through the '+' and '-' buttons
        		//////////////////////////////////////////////////////////////////////////
        case ID_BUTTONHR_P:                 // increment hour value
            if(objMsg == BTN_MSG_PRESSED)
            {
                hour = +1;
                ebID = ID_EB_HOUR;
                i = 4;
            }

            break;

        case ID_BUTTONHR_M:                 // decrement hour value
            if(objMsg == BTN_MSG_PRESSED)
            {
                hour = -1;
                ebID = ID_EB_HOUR;
                i = 4;
            }

            break;

        case ID_BUTTONMN_P:                 // increment minute value
            if(objMsg == BTN_MSG_PRESSED)
            {
                minute = +1;
                ebID = ID_EB_MINUTE;
                i = 7;
            }

            break;

        case ID_BUTTONMN_M:                 // decrement minute value
            if(objMsg == BTN_MSG_PRESSED)
            {
                minute = -1;
                ebID = ID_EB_MINUTE;
                i = 7;
            }

            break;

        case ID_BUTTONSC_P:                 // increment seconds value
            if(objMsg == BTN_MSG_PRESSED)
            {
                second = +1;
                ebID = ID_EB_SECOND;
                i = 10;
            }

            break;

        case ID_BUTTONSC_M:                 // decrement seconds value
            if(objMsg == BTN_MSG_PRESSED)
            {
                second = -1;
                ebID = ID_EB_SECOND;
                i = 10;
            }

            break;

        default:
            ebID = 0;
            break;
    }

    if(ebID)
    {   // check if time was modified
        mRTCCOff();
        RTCCSetBinHour(mRTCCGetBinHour() + hour);   // update RTCC registers
        RTCCSetBinMin(mRTCCGetBinMin() + minute);
        RTCCSetBinSec(mRTCCGetBinSec() + second);
        mRTCCSet();             // copy the new values to the RTCC registers

        // update the edit box that was changed. This is done to immediately update
        // the values on the edit box. The normal update by updateDateTimeEb() is done
        // for every second. This may be too long to wait when the increment or decrement
        // was successfully performed.
        RTCCProcessEvents();    // update string values
        updateDateTimeEb();
    }

    return (1);
}

//void UpdateRTCCDates(XCHAR *pText, LISTBOX* pLb) {
void UpdateRTCCDates(LISTBOX *pLb)
{
    static char value;
    char        ch;
    XCHAR       *pItemText;

    // get the selected item
    pItemText = LbGetSel(pLb, pLb->pItemList)->pText;
    switch(DateItemID)
    {
        default:

        case ID_EB_MONTH:
            value = LbGetFocusedItem(pLb) + 1;  // get the selected month item +1 is needed since

            // RTCC stores months from index 1 as january
            RTCCSetBinMonth(value);             // set the month value in RTCC module
            RTCCCalculateWeekDay();             // calculate the weekday from this new value
            break;

        case ID_EB_YEAR:
        case ID_EB_DAY:
            ch = *pItemText++;                  // get the first character
            value = (ch - 48) * 10;             // get the value of the first character
            ch = *pItemText;                    // get the next character
            value = value + (ch - 48);          // add the value of the second character to the first one
            if(DateItemID == ID_EB_DAY)
                RTCCSetBinDay(value);           // set the day value in the RTCC module
            else
            {
                RTCCSetBinYear(value);          // set the day value in the RTCC module
            }

            RTCCCalculateWeekDay();             // calculate the weekday from this new value
            break;
    }

    mRTCCSet(); // copy the new values to the RTCC registers
}

/*********************************************************************************/

//			END OF DATE AND TIME SETTING

/*********************************************************************************/

/*********************************************************************************/

/*********************************************************************************/

/*********************************************************************************/
#define MAXMETER1VALUE  150
#define MAXMETER2VALUE  30

#define MINMETER1VALUE  0
#define MINMETER2VALUE  0

// Creates meter screen
void CreateMeter(void)
{
    #define MTR_ORIGIN_X    ((GetMaxX() - 250 + 1) / 2)
    #define MTR_ORIGIN_Y    (((40 + GetMaxY() - 180 + 1) / 2) - 10)

    METER   *pMtr;

    GOLFree();                  // free memory for the objects in the previous linked list and start new list
    CreatePage(MeterStr);       // CreatePage("Meter");
    pMtr = MtrCreate
        (
            ID_METER1,
            MTR_ORIGIN_X + 0,
            MTR_ORIGIN_Y + 0,
            MTR_ORIGIN_X + 120,
            MTR_ORIGIN_Y + 130, // set dimension
            MTR_DRAW | MTR_RING | MTR_ACCURACY, // draw meter object with ring scale & accuracy set
            MINMETER1VALUE,                     // set initial value
            MINMETER1VALUE,                     // meter minimum value
            MAXMETER1VALUE,                     // meter maximum value
            NULL,
            NULL,
            Meter1Str,                          // "METER1"
            meterScheme
        );                      // alternative GOL scheme       			

    // set scaling values
    MtrSetScaleColors(pMtr, BRIGHTGREEN, BRIGHTGREEN, BRIGHTGREEN, BRIGHTGREEN, BRIGHTYELLOW, BRIGHTRED);

    // set title and values fonts
    MtrSetTitleFont(pMtr, (void *) &GOLMediumFont);
    MtrSetValueFont(pMtr, (void *) &GOLMediumFont);

    pMtr = MtrCreate
        (
            ID_METER2,
            MTR_ORIGIN_X + 130,
            MTR_ORIGIN_Y + 0,
            MTR_ORIGIN_X + 250,
            MTR_ORIGIN_Y + 130, // set dimension
            MTR_DRAW,           // draw normal meter object
            MINMETER2VALUE,     // set initial value
            MINMETER2VALUE,     // set minimum value
            MAXMETER2VALUE,     // set maximum value
            NULL,
            NULL,
            Meter2Str,          // "METER2"
            meterScheme
        );                      // alternative GOL scheme

    // set title and values fonts
    MtrSetTitleFont(pMtr, (void *) &GOLMediumFont);
    MtrSetValueFont(pMtr, (void *) &GOLMediumFont);

    BtnCreate
    (
        ID_BUTTON1,             // button ID
        MTR_ORIGIN_X + 20,
        MTR_ORIGIN_Y + 165,     // left, top corner	
        MTR_ORIGIN_X + 100,
        MTR_ORIGIN_Y + 195,     // right, bottom corner
        0,                      // draw rectangular button	
        BTN_DRAW | BTN_TOGGLE,  // will be dislayed after creation
        NULL,                   // no bitmap	
        DecelStr,               // decelerate
        altScheme
    );                          // use alternate scheme
    BtnCreate
    (
        ID_BUTTON2,             // button ID
        MTR_ORIGIN_X + 150,
        MTR_ORIGIN_Y + 165,     // left, top corner	
        MTR_ORIGIN_X + 230,
        MTR_ORIGIN_Y + 195,     // right, bottom corner
        0,                      // draw rectangular button	
        BTN_DRAW | BTN_TOGGLE,  // will be dislayed after creation
        NULL,                   // no bitmap	
        DecelStr,               // decelerate
        altScheme
    );                          // use alternate scheme
    DmCreate
    (
        ID_DIGITALMETER1,
        MTR_ORIGIN_X + 20,
        MTR_ORIGIN_Y + 140,     // left, top corner	
        MTR_ORIGIN_X + 100,
        MTR_ORIGIN_Y + 160,     // right, bottom corner
        DM_DRAW | DM_CENTER_ALIGN | DM_FRAME,
        0,
        3,
        1,
        alt5Scheme
    );

    DmCreate
    (
        ID_DIGITALMETER2,
        MTR_ORIGIN_X + 150,
        MTR_ORIGIN_Y + 140,     // left, top corner	
        MTR_ORIGIN_X + 230,
        MTR_ORIGIN_Y + 160,     // right, bottom corner
        DM_DRAW | DM_CENTER_ALIGN | DM_FRAME,
        0,
        2,
        0,
        alt5Scheme
    );
}

// Processes messages for meter screen
WORD MsgMeter(WORD objMsg, OBJ_HEADER *pObj)
{
    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_DIAL;              // goto dial screen
            }

            return (1);                                 // process by default

        case ID_BUTTON1:
        case ID_BUTTON2:
            if(objMsg == BTN_MSG_PRESSED)
            {
                BtnSetText((BUTTON *)pObj, AccelStr);   // set text to accelerate	
            }

            if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetText((BUTTON *)pObj, DecelStr);   // set text to decelerate
            }

            return (1);                         // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_EDITBOX;   // goto edit box screen
            }

            return (1);                         // process by default

        default:
            return (1);                         // process by default
    }
}

// this updates the meters on the screen
void UpdateMeter(void)
{
    OBJ_HEADER  *pObj, *pDigMtrObj1, *pDigMtrObj2;
    METER       *pMtrObj;
    WORD        i, value;

    pDigMtrObj1 = GOLFindObject(ID_DIGITALMETER1);
    pDigMtrObj2 = GOLFindObject(ID_DIGITALMETER2);

    /* Meter 1 is controlled by ID_BUTTON1
  	   Meter 2 is controlled by ID_BUTTON2
	   Both meters goes from 0 to maximum when their respective 
	   buttons are pressed.	Both button have a toggle property. So
	   when pressed it will remain to be pressed until the meter has reached
	   the maximum value. The only difference is that ID_BUTTON1 is controlled
	   by the code while ID_BUTTON2 is manually controlled.
	*/
    for(i = ID_BUTTON1; i < ID_BUTTON2 + 1; i++)
    {
        pObj = GOLFindObject(i);

        if(i == ID_BUTTON1)
        {
            pMtrObj = (METER *)GOLFindObject(ID_METER1);    // get meter 1 pointer
            value = MtrGetVal(pMtrObj);                     // get current value
            if(value == MINMETER1VALUE)
            {   // when zero turn on ID_BUTTON1
                BtnSetText((BUTTON *)pObj, AccelStr);   // set text to accelerate
                SetState(pObj, BTN_PRESSED | BTN_DRAW);
            }

            if(value == MAXMETER1VALUE)
            {   // when maximum turn off ID_BUTTON1
                BtnSetText((BUTTON *)pObj, DecelStr);       // set text to decelerate
                ClrState(pObj, BTN_PRESSED);
                SetState(pObj, BTN_DRAW);
            }

            if(GetState(pObj, BTN_PRESSED))                 // increment meter value when button
                value += 2;                                 // is pressed
            else
                value -= 2;
            MtrSetVal(pMtrObj, value);                      // decrement meter value when button
            DmSetValue((DIGITALMETER *)pDigMtrObj1, value);
            SetState(pDigMtrObj1, DM_DRAW);
            SetState(pMtrObj, MTR_DRAW_UPDATE);             // is released
        }
        else
        {
            pMtrObj = (METER *)GOLFindObject(ID_METER2);    // get meter 2 pointer
            value = MtrGetVal(pMtrObj);                     // get meter current value
            if(GetState(pObj, BTN_PRESSED))
            {   // check if ID_BUTTON2 is pressed
                MtrSetVal(pMtrObj, value + 1);      // increment value of meter
                SetState(pMtrObj, MTR_DRAW_UPDATE); // redraw meter
                DmSetValue((DIGITALMETER *)pDigMtrObj2, value + 1);
                SetState(pDigMtrObj2, DM_UPDATE);
            }
            else
            {
                if(value > 0)
                {   // if ID_BUTTON2 is released
                    MtrSetVal(pMtrObj, value - 1);      // decrement meter if not yet zero
                    SetState(pMtrObj, MTR_DRAW_UPDATE); // redraw meter
                    DmSetValue((DIGITALMETER *)pDigMtrObj2, value - 1);
                    SetState(pDigMtrObj2, DM_UPDATE);
                }
            }

            if(value == MAXMETER2VALUE)
            {   // check if meter 2 reached max value
                ClrState(pObj, BTN_PRESSED);            // release ID_BUTTON2 when max is reached
                BtnSetText((BUTTON *)pObj, DecelStr);   // set text to decelerate
                SetState(pObj, BTN_DRAW);               // redraw button
            }
        }
    }
}

// Creates dial screen
void CreateDial(void)
{
    #define DIAL_ORIGIN_X   ((GetMaxX() - 247 + 1) / 2)
    #define DIAL_ORIGIN_Y   ((40 + GetMaxY() - 143 + 1) / 2)

    GOLFree();                      // free memory for the objects in the previous linked list and start new list
    #if !(defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__) || defined(__PIC24FJ256DA210__) || defined(GFX_PICTAIL_LCC))
    TRISA = 0xFF80;                 // set IO pins (0:6) to output mode,

    // pin 7 is used as side button S5 switch for press and release
    LATA = 0x0001;                  // light only one LED
    #endif
    CreatePage(DialStr);            // CreatePage("Dial");
    RdiaCreate
    (
        ID_ROUNDDIAL,               // ID
        DIAL_ORIGIN_X + 55,
        DIAL_ORIGIN_Y + 72,
        55,                         // dimensions
        RDIA_DRAW,                  // draw after creation
        1,                          // resolution
        400,                        // initial value
        700,                        // maximum value
        altScheme
    );                              // use alternative scheme
    MtrCreate
    (
        ID_METER1,
        DIAL_ORIGIN_X + 117,
        DIAL_ORIGIN_Y + 0,
        DIAL_ORIGIN_X + 247,
        DIAL_ORIGIN_Y + 143,        // set dimension
        MTR_DRAW,                   // draw object after creation
        400,                        // set initial value
        0,                          // set minimum value
        700,                        // set maximum value
        (void *) &GOLFontDefault,   // set title font
        (void *) &GOLMediumFont,    // set value font
        ValueLabelStr,              // "VALUE"
        meterScheme
    );                              // alternative GOL scheme

    // set scaling values
    MtrSetScaleColors
    (
        ((METER *)GOLFindObject(ID_METER1)),
        BRIGHTGREEN,
        BRIGHTGREEN,
        BRIGHTGREEN,
        BRIGHTBLUE,
        BRIGHTYELLOW,
        BRIGHTRED
    );
}

// Processes messages for dial screen
WORD MsgDial(WORD objMsg, OBJ_HEADER *pObj)
{
    OBJ_HEADER  *pMtrObj;
    SHORT       dialVal;

    switch(GetObjID(pObj))
    {
        case ID_ROUNDDIAL:
            dialVal = RdiaGetVal((ROUNDDIAL *)pObj);
            pMtrObj = GOLFindObject(ID_METER1);
            MtrSetVal((METER *)pMtrObj, dialVal);
            SetState(pMtrObj, MTR_DRAW_UPDATE);
            if(objMsg == RD_MSG_CLOCKWISE)
                if(dialVal == 700)
                    return (0);
            if(objMsg == RD_MSG_CTR_CLOCKWISE)
                if(dialVal == 0)
                    return (0);

            dialVal = (dialVal - 1) % 70;       // -1 is used to avoid 70 which is also 0 after mod.
            #if !(defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__) || defined(__PIC24FJ256DA210__) || defined(GFX_PICTAIL_LCC))
            if(dialVal <= 10)
                LATA = 0x0001;                  // light LED 0
            else if(dialVal <= 20)
                LATA = 0x0002;                  // light LED 1
            else if(dialVal <= 30)
                LATA = 0x0004;                  // light LED 2
            else if(dialVal <= 40)
                LATA = 0x0008;                  // light LED 3
            else if(dialVal <= 50)
                LATA = 0x0010;                  // light LED 4
            else if(dialVal <= 60)
                LATA = 0x0020;                  // light LED 5
            else if(dialVal <= 70)
                LATA = 0x0040;                  // light LED 6
            #endif
            break;                              // process by default

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                #if !(defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__) || defined(__PIC24FJ256DA210__) || defined(GFX_PICTAIL_LCC))
                LATA = 0xFF00;                  // turn off all LED
                TRISA = 0xFFFF;                 // set IO pin to input mode
                #endif
                screenState = CREATE_PICTURE;   // goto picture control screen
            }

            break;                              // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                #if !(defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__) || defined(__PIC24FJ256DA210__) || defined(GFX_PICTAIL_LCC))
                LATA = 0xFF00;                  // turn off all LED
                TRISA = 0xFFFF;                 // set IO pin to input mode
                #endif
                screenState = CREATE_METER;     // goto meter screen
            }

            break;                              // process by default

        default:
            break;                              // process by default
    }

    return (1);
}

/* */
void CreatePicture(void)
{
    #define PIC_ORIGIN_X    ((GetMaxX() - 190 + 1) / 2)
    #define PIC_ORIGIN_Y    ((40 + GetMaxY() - 170 + 1) / 2)

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list
    CreatePage(PictureStr);                         // CreatePage("Picture control");
    pPicture = PictCreate
        (
            ID_PICTURE1,                            // ID
            PIC_ORIGIN_X + 0,
            PIC_ORIGIN_Y + 0,
            PIC_ORIGIN_X + 155,
            PIC_ORIGIN_Y + 120,                     // dimension
            PICT_DRAW | PICT_FRAME,                 // will be dislayed, has frame
            1,                                      // scale factor is x1
            (void *) &Engine1,                      // bitmap
            altScheme
        );                                          // default GOL scheme
    pSlider = SldCreate
        (
            ID_SLIDER1,                             // ID
            PIC_ORIGIN_X + 160,
            PIC_ORIGIN_Y + 0,
            PIC_ORIGIN_X + 190,
            PIC_ORIGIN_Y + 120,                     // dimension
            SLD_DRAW | SLD_DISABLED | SLD_VERTICAL, // draw with disabled state
            50,             // range
            1,              // page
            25,             // pos
            altScheme
        );                  // alternative GOL scheme
    CbCreate
    (
        ID_CHECKBOX1,       // ID
        PIC_ORIGIN_X - 20,
        PIC_ORIGIN_Y + 130,
        PIC_ORIGIN_X + 80,
        PIC_ORIGIN_Y + 170, // dimension
        CB_DRAW,            // will be dislayed after creation
        ScaleStr,           // "Scale"
        altScheme
    );                      // alternative GOL scheme
    CbCreate
    (
        ID_CHECKBOX2,       // ID
        PIC_ORIGIN_X + 90,
        PIC_ORIGIN_Y + 130,
        PIC_ORIGIN_X + 220,
        PIC_ORIGIN_Y + 170, // dimension
        CB_DRAW,            // will be dislayed after creation
        AnimateStr,         // "Animate"
        altScheme
    );                      // alternative GOL scheme
}

/* */
WORD MsgPicture(WORD objMsg, OBJ_HEADER *pObj)
{
    switch(GetObjID(pObj))
    {
        case ID_CHECKBOX1:
            if(objMsg == CB_MSG_CHECKED)
            {                               // set new scale factor
                PictSetScale(pPicture, 2);  // x2
            }
            else
            {
                PictSetScale(pPicture, 1);  // x1
            }

            SetState(pPicture, PICT_DRAW);  // set redraw state
            return (1);                     // process by default

        case ID_CHECKBOX2:
            if(objMsg == CB_MSG_CHECKED)
            {                               // switch on/off animation
                ClrState(pSlider, SLD_DISABLED);            // enable slider
                SetState(pSlider, SLD_DRAW);                // redraw slider to show new state 	
                animate = 1;
            }
            else
            {
                SetState(pSlider, SLD_DISABLED | SLD_DRAW); // disable the slider and redraw
                animate = 0;
            }

            return (1);                             // process by default

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_CUSTOMCONTROL; // goto cutom control screen
                animate = 0;                        // switch off animation
            }

            return (1);                             // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_DIAL;          // goto dial screen
                animate = 0;                        // switch off animation
            }

            return (1);                             // process by default

        default:
            return (1);                             // process by default
    }
}

// Creates custom control screen
void CreateCustomControl(void)
{
    GOLFree();                                      // free memory for the objects in the previous linked list and start new list
    TransparentColorDisable();
    CreatePage(CustomStr);                          // CreatePage("Custom control");
    CcCreate
    (
        ID_CUSTOM1,                                 // ID
        CC_ORIGIN_X + 30,
        CC_ORIGIN_Y + 0,
        CC_ORIGIN_X + 150,
        CC_ORIGIN_Y + 120,                          // dimension
        CC_DRAW,                                    // will be dislayed after creation
        alt3Scheme
    );                                              // use alternate 3 scheme
    RbCreate
    (
        ID_RADIOBUTTON1,                            // ID
        CC_ORIGIN_X - 20,
        CC_ORIGIN_Y + 130,
        CC_ORIGIN_X + 80,
        CC_ORIGIN_Y + 150,                          // dimension
        RB_DRAW | RB_GROUP,                         // will be dislayed and focused after creation
        // first button in the group
        HeavyLoadStr,  // "Heavy Load"
        alt2Scheme
    );                          // alternative GOL scheme
    RbCreate
    (
        ID_RADIOBUTTON2,        // ID
        CC_ORIGIN_X - 20,
        CC_ORIGIN_Y + 155,
        CC_ORIGIN_X + 80,
        CC_ORIGIN_Y + 175,      // dimension
        RB_DRAW | RB_CHECKED,   // will be dislayed and checked after creation
        NormalLoadStr,          // "Normal Load"
        alt2Scheme
    );                          // alternative GOL scheme
    RbCreate
    (
        ID_RADIOBUTTON3,        // ID
        CC_ORIGIN_X + 100,
        CC_ORIGIN_Y + 130,
        CC_ORIGIN_X + 200,
        CC_ORIGIN_Y + 150,      // dimension
        RB_DRAW,                // will be dislayed after creation
        LightLoadStr,           // "Light Load"
        alt2Scheme
    );                          // alternative GOL scheme
}

// Processes messages for custom control screen
WORD MsgCustomControl(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    RADIOBUTTON *pRb;
    CUSTOM      *pCc;

    switch(GetObjID(pObj))
    {
        case ID_RADIOBUTTON1:   // set load to high
            pCc = (CUSTOM *)GOLFindObject(ID_CUSTOM1);  // get pointer to custom control object
            CcSetPos(pCc,   CC_ORIGIN_Y + (pCc->hdr.bottom - pCc->hdr.top)/6);

            SetState(pCc, CC_DRAW_BAR);                 // set to redraw the level
            return (1);         // process by default

        case ID_RADIOBUTTON2:   // set load to normal
            pCc = (CUSTOM *)GOLFindObject(ID_CUSTOM1);  // get pointer to custom control object
            CcSetPos(pCc,   CC_ORIGIN_Y + ((pCc->hdr.bottom - pCc->hdr.top)/6)*3);
            SetState(pCc, CC_DRAW_BAR);                 // set to redraw the level
            return (1);         // process by default

        case ID_RADIOBUTTON3:   // set load to light
            pCc = (CUSTOM *)GOLFindObject(ID_CUSTOM1);  // get pointer to custom control object
            CcSetPos(pCc,   CC_ORIGIN_Y + ((pCc->hdr.bottom - pCc->hdr.top)/6)*5);
            SetState(pCc, CC_DRAW_BAR);                 // set to redraw the level
            return (1);     // process by default

        case ID_CUSTOM1:    // level is changed
            if(objMsg == CC_MSG_SELECTED)
            {
                if(pMsg->param2 <= 110)
                {
                    pRb = (RADIOBUTTON *)GOLFindObject(ID_RADIOBUTTON1);    // get pointer heavy load setting
                    RbSetCheck(pRb, ID_RADIOBUTTON1);                       // set radio button for heavy to be checked
                    SetState(pRb, RB_DRAW_CHECK);
                }
                else if(pMsg->param2 <= 140)
                {
                    pRb = (RADIOBUTTON *)GOLFindObject(ID_RADIOBUTTON2);    // get pointer heavy load setting
                    RbSetCheck(pRb, ID_RADIOBUTTON2);                       // set radio button for normal to be checked
                    SetState(pRb, RB_DRAW_CHECK);
                }
                else
                {
                    pRb = (RADIOBUTTON *)GOLFindObject(ID_RADIOBUTTON3);    // get pointer heavy load setting
                    RbSetCheck(pRb, ID_RADIOBUTTON3);                       // set radio button for light to be checked
                    SetState(pRb, RB_DRAW_CHECK);
                }
            }

            return (1);

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_SIGNATURE;                             // goto signature screen
            }

            return (1);                         // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_PICTURE;   // goto picture screen
            }

            return (1);                         // process by default

        default:
            return (1);                         // process by default
    }
}

// dimensions for signature box
#define SIG_ORIGIN_X        ((GetMaxX() - 260 + 1) / 2)
#define SIG_ORIGIN_Y        ((40 + GetMaxY() - 170 + 1) / 2)
#define SIG_PANEL_LEFT      35
#define SIG_PANEL_RIGHT     GetMaxX() - 35
#define SIG_PANEL_TOP       50
#define SIG_PANEL_BOTTOM    GetMaxY() - 10

// Creates signature screen
void CreateSignature(void)
{
    GOLFree();  // free memory for the objects in the previous linked list and start new list
    CreatePage(SignatureStr);
}

// Draws box for signature
WORD PanelSignature(void)
{
    #define SIG_STATE_SET   0
    #define SIG_STATE_DRAW  1

    static BYTE state = SIG_STATE_SET;

    if(state == SIG_STATE_SET)
    {                           // set data for panel drawing
        GOLPanelDraw
        (
            SIG_PANEL_LEFT,
            SIG_PANEL_TOP,
            SIG_PANEL_RIGHT,
            SIG_PANEL_BOTTOM,
            0,
            WHITE,
            altScheme->EmbossLtColor,
            altScheme->EmbossDkColor,
            NULL,
            GOL_EMBOSS_SIZE
        );

        state = SIG_STATE_DRAW; // change state
    }

    if(!GOLPanelDrawTsk())
    {
        return (0);             // drawing is not completed
    }
    else
    {
        state = SIG_STATE_SET;  // set state to initial
        return (1);             // drawing is done
    }
}

// Processes messages for signature screen
WORD MsgSignature(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    static SHORT    prevX = -1;     // previous point
    static SHORT    prevY = -1;
    SHORT           x, y;           // current point
    switch(GetObjID(pObj))
    {
        case ID_WINDOW1:

            // get touched position
            x = pMsg->param1;
            y = pMsg->param2;

            // Check if it falls in the signature box
            if
            (
                ((SIG_PANEL_LEFT + GOL_EMBOSS_SIZE) < x) &&
                ((SIG_PANEL_RIGHT - GOL_EMBOSS_SIZE) > x) &&
                ((SIG_PANEL_TOP + GOL_EMBOSS_SIZE) < y) &&
                ((SIG_PANEL_BOTTOM - GOL_EMBOSS_SIZE) > y)
            )
            {
                SetColor(BRIGHTRED);

                // if previous position is not valid or event is PRESS
                if((pMsg->uiEvent == EVENT_PRESS) || (prevX < 0))
                {
                    PutPixel(x, y); // draw pixel
                }
                else
                {
                    WAIT_UNTIL_FINISH(Line(prevX, prevY, x, y));    // connect with thick line previous and current points
                    WAIT_UNTIL_FINISH(Line(prevX - 1, prevY, x - 1, y));
                    WAIT_UNTIL_FINISH(Line(prevX + 1, prevY, x + 1, y));
                    WAIT_UNTIL_FINISH(Line(prevX, prevY - 1, x, y - 1));
                    WAIT_UNTIL_FINISH(Line(prevX, prevY + 1, x, y + 1));
                }

                prevX = x;
                prevY = y;                          // store position
            }
            else
            {

                // reset if outside of box
                prevX = -1;
            }

            return (1);                             // process by default

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_POT;           // goto potentiometer graph screen
                animate = 0;
            }

            return (1);                             // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_CUSTOMCONTROL; // goto custom control screen
                animate = 0;
            }

            return (1);                             // process by default

        default:
            return (1);                             // process by default
    }
}

// dimensions for potentiometer graph area
#define POT_ORIGIN_X        ((GetMaxX() - 260 + 1) / 2)
#define POT_ORIGIN_Y        ((40 + GetMaxY() - 185 + 1) / 2)
#define POT_PANEL_LEFT      POT_ORIGIN_X
#define POT_PANEL_RIGHT     POT_ORIGIN_X + 260
#define POT_PANEL_TOP       POT_ORIGIN_Y
#define POT_PANEL_BOTTOM    POT_ORIGIN_Y + 160

// Graph area borders
#define POTGR_LEFT      (POT_PANEL_LEFT + GOL_EMBOSS_SIZE)
#define POTGR_RIGHT     (POT_PANEL_RIGHT - GOL_EMBOSS_SIZE)
#define POTGR_TOP       (POT_PANEL_TOP + GOL_EMBOSS_SIZE)
#define POTGR_BOTTOM    (POT_PANEL_BOTTOM - GOL_EMBOSS_SIZE)

volatile SHORT  buffer[256];
volatile SHORT  counter = 0;
volatile SHORT  inc = 10;

// Creates ntr screen
void CreatePotentiometer(void)
{
    GOLFree();  // free memory for the objects in the previous linked list and start new list
    CreatePage(PlottingStr);
}

// Draws box for potentiometer graph
WORD PanelPotentiometer(void)
{
    #define POTPNL_STATE_SET    0
    #define POTPNL_STATE_DRAW   1
    #define POTPNL_STATE_TEXT   2

    static BYTE state = POTPNL_STATE_SET;

    if(state == POTPNL_STATE_SET)
    {                               // set data for panel
        GOLPanelDraw
        (
            POT_PANEL_LEFT,
            POT_PANEL_TOP,
            POT_PANEL_RIGHT,
            POT_PANEL_BOTTOM,
            0,
            WHITE,
            altScheme->EmbossDkColor,
            altScheme->EmbossLtColor,
            NULL,
            GOL_EMBOSS_SIZE
        );

        state = POTPNL_STATE_DRAW;  // change state
    }

    if(state == POTPNL_STATE_DRAW)
    {
        if(!GOLPanelDrawTsk())
        {                           // draw box for graph
            return (0);             // drawing is not completed
        }
        else
        {
            state = POTPNL_STATE_TEXT;          // change to initial state
            SetFont((void *) &GOLMediumFont);
            SetColor(BRIGHTBLUE);
            MoveTo((GetMaxX() - GetTextWidth(UseR6PotStr, (void *) &GOLMediumFont)) >> 1, POT_ORIGIN_Y + 165);
        }
    }

    if(state == POTPNL_STATE_TEXT)
    {
        WAIT_UNTIL_FINISH(OutText(UseR6PotStr)) // "Using R6 potentiometer"))
        return (0);
        state = POTPNL_STATE_SET;               // change to initial state
        return (1);                             // drawing is done
    }

    return (1);
}

// Add sample from potentiometer ADC channel into temporary buffer

// Returns zero if samples number in the buffer is less than defined by parameter
WORD GetPotSamples(WORD number)
{
#ifdef ADC_POT
    static BYTE     counter = 0;
    volatile SHORT  temp=0;

    temp = ADCGetPot() >> 3;
    if((temp + POTGR_TOP) > POTGR_BOTTOM)
        temp = POTGR_BOTTOM - POTGR_TOP;

    tempBuffer[counter++] = temp;
    if(counter >= number)
    {
        counter = 0;
        return (1);
    }
#endif
    return (0);
}

// Draws graph
void GraphPotentiometer(void)
{
    SHORT           x, y;
    SHORT           sy, ey;
    SHORT           *ptr;
    SHORT           counter;
    static SHORT    pos;

    // remove graph
    SetColor(WHITE);
    ptr = potBuffer + pos;
    sy = *ptr++;
    for(x = POTGR_RIGHT; x >= POTGR_LEFT; x--)
    {
        if(ptr == (potBuffer + POT_BUFFER_SIZE))
            ptr = potBuffer;
        ey = *ptr++;
        if(ey > sy)
        {
            for(y = sy + POTGR_TOP; y < ey + POTGR_TOP + 1; y++)
                PutPixel(x, y);
        }
        else
        {
            for(y = ey + POTGR_TOP; y < sy + POTGR_TOP + 1; y++)
                PutPixel(x, y);
        }

        sy = ey;
    }

    // draw grid
    SetColor(LIGHTGRAY);
    for(x = POTGR_LEFT + ((POTGR_RIGHT - POTGR_LEFT) >> 3); x < POTGR_RIGHT; x += (POTGR_RIGHT - POTGR_LEFT) >> 3)
        WAIT_UNTIL_FINISH(Bar(x, POTGR_TOP, x, POTGR_BOTTOM));

    for(y = POTGR_TOP + ((POTGR_BOTTOM - POTGR_TOP) >> 3); y < POTGR_BOTTOM; y += (POTGR_BOTTOM - POTGR_TOP) >> 3)
        WAIT_UNTIL_FINISH(Bar(POTGR_LEFT, y, POTGR_RIGHT, y));

    pos -= POT_MOVE_DELTA;
    if(pos < 0)
        pos = POT_BUFFER_SIZE - 1;

    // copy new data from temporary buffer
    ptr = potBuffer + pos;
    for(counter = POT_MOVE_DELTA - 1; counter >= 0; counter--)
    {
        *ptr++ = tempBuffer[counter];
        if(ptr == (potBuffer + POT_BUFFER_SIZE))
            ptr = potBuffer;
    }

    // draw graph
    SetColor(BRIGHTRED);
    ptr = potBuffer + pos;
    sy = *ptr++;
    for(x = POTGR_RIGHT; x >= POTGR_LEFT; x--)
    {
        if(ptr == (potBuffer + POT_BUFFER_SIZE))
            ptr = potBuffer;
        ey = *ptr++;
        if(ey > sy)
        {
            for(y = sy + POTGR_TOP; y < ey + POTGR_TOP + 1; y++)
                PutPixel(x, y);
        }
        else
        {
            for(y = ey + POTGR_TOP; y < sy + POTGR_TOP + 1; y++)
                PutPixel(x, y);
        }

        sy = ey;
    }
}

// Processes messages for ECG screen
WORD MsgPotentiometer(WORD objMsg, OBJ_HEADER *pObj)
{
    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_ECG;       // goto ECG screen
            }

            return (1);                         // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_SIGNATURE; // goto signature screen
            }

            return (1);                         // process by default

        default:
            return (1);                         // process by default
    }
}

// dimensions for ECG graph area
#define ECG_ORIGIN_X        ((GetMaxX() - 262 + 1) / 2)
#define ECG_ORIGIN_Y        ((40 + GetMaxY() - 190 + 1) / 2)
#define ECG_PANEL_LEFT      ECG_ORIGIN_X
#define ECG_PANEL_RIGHT     ECG_ORIGIN_X + 262
#define ECG_PANEL_TOP       ECG_ORIGIN_Y
#define ECG_PANEL_BOTTOM    ECG_ORIGIN_Y + 165

// Graph area borders
#define GR_LEFT     ECG_PANEL_LEFT + GOL_EMBOSS_SIZE
#define GR_RIGHT    ECG_PANEL_RIGHT - GOL_EMBOSS_SIZE
#define GR_TOP      ECG_PANEL_TOP + GOL_EMBOSS_SIZE
#define GR_BOTTOM   ECG_PANEL_BOTTOM - GOL_EMBOSS_SIZE

// Creates ECG screen
void CreateECG(void)
{
    GOLFree();  // free memory for the objects in the previous linked list and start new list
    CreatePage(EcgStr);
}

// Add sample from FLASH (can be any source) into temporary buffer

// Returns zero if samples number in the buffer is less than defined by parameter
WORD GetECGSamples(WORD number)
{
    static const SHORT  *pointer = ecgPoints;
    static BYTE         counter = 0;

    if(pointer == (ecgPoints + sizeof(ecgPoints) / sizeof(SHORT)))
        pointer = ecgPoints;

    tempBuffer[counter++] = *pointer++;

    if(counter >= number)
    {
        counter = 0;
        return (1);
    }

    return (0);
}

// Draws box for ECG
WORD PanelECG(void)
{
    #define ECG_STATE_SET       0
    #define ECG_STATE_DRAW      1
    #define ECG_STATE_VERLINE   2
    #define ECG_STATE_HORLINE   3
    #define ECG_STATE_TEXT      4

    static BYTE     state = ECG_STATE_SET;
    static SHORT    pos;

    switch(state)
    {
        case ECG_STATE_SET:

            // set parameters for panel
            GOLPanelDraw
            (
                ECG_PANEL_LEFT,
                ECG_PANEL_TOP,
                ECG_PANEL_RIGHT,
                ECG_PANEL_BOTTOM,
                0,
                WHITE,
                altScheme->EmbossDkColor,
                altScheme->EmbossLtColor,
                NULL,
                GOL_EMBOSS_SIZE
            );

            state = ECG_STATE_DRAW;     // change state

        case ECG_STATE_DRAW:
            if(!GOLPanelDrawTsk())      // draw panel for graph
                return (0);             // drawing is not completed
            SetColor(LIGHTGRAY);
            SetLineType(DOTTED_LINE);
            pos = GR_LEFT + ((GR_RIGHT - GR_LEFT) >> 3);
            state = ECG_STATE_VERLINE;  // change state

        case ECG_STATE_VERLINE:
            while(pos < GR_RIGHT)
            {                           // draw vertical grid lines
                if(!Line(pos, GR_TOP, pos, GR_BOTTOM))
                    return (0);
                pos += (GR_RIGHT - GR_LEFT) >> 3;
            }

            pos = GR_TOP + ((GR_BOTTOM - GR_TOP) >> 3);
            state = ECG_STATE_HORLINE;  // change state

        case ECG_STATE_HORLINE:
            while(pos < GR_BOTTOM)
            {                           // draw vertical grid lines
                if(!Line(GR_LEFT, pos, GR_RIGHT, pos))
                    return (0);
                pos += (GR_BOTTOM - GR_TOP) >> 3;
            }

            SetLineType(SOLID_LINE);
            SetFont((void *) &GOLMediumFont);

            SetColor(BRIGHTBLUE);

            MoveTo((GetMaxX() - GetTextWidth(ScanModeStr, (void *) &GOLMediumFont)) >> 1, ECG_ORIGIN_Y + 170);
            state = ECG_STATE_TEXT;     // change to text display state

        case ECG_STATE_TEXT:
            WAIT_UNTIL_FINISH(OutText(ScanModeStr)) // WAIT_UNTIL_FINISH(OutText("Scan mode"))
            return (0);
            state = ECG_STATE_SET;                  // change to initial state
            return (1);                             // drawing is done
    }

    return (1);
}

// Draws graph
void GraphECG(void)
{
    SHORT           x, y;
    static SHORT    sy = 0;
    static SHORT    tsy = 0;
    SHORT           ey;
    static SHORT    *ptr = ecgBuffer;
    static SHORT    pos = 0;
    SHORT           counter;
    SHORT           *pTemp;
    SHORT           temp;

    // remove graph
    SetColor(WHITE);

    pTemp = ptr;
    temp = pos;

    for(x = 0; x < ECG_WINDOW_SIZE; x++)
    {
        ey = *ptr++;
        pos++;

        if(ey > sy)
        {
            for(y = sy + GR_TOP; y < ey + GR_TOP + 1; y++)
                PutPixel(GR_LEFT + pos, y);
        }
        else
        {
            for(y = ey + GR_TOP; y < sy + GR_TOP + 1; y++)
                PutPixel(GR_LEFT + pos, y);
        }

        if(ptr == (ecgBuffer + ECG_BUFFER_SIZE))
        {
            ptr = ecgBuffer;
            pos = 0;
        }

        sy = ey;
    }

    // copy new data from temporary buffer
    ptr = pTemp;
    pos = temp;

    for(counter = 0; counter < ECG_WINDOW_SIZE; counter++)
    {
        *ptr++ = tempBuffer[counter];
        pos++;
        if(ptr == (ecgBuffer + ECG_BUFFER_SIZE))
        {
            ptr = ecgBuffer;
            pos = 0;
        }
    }

    // draw graph
    SetColor(BRIGHTRED);

    ptr = pTemp;
    pos = temp;

    for(x = 0; x < ECG_WINDOW_SIZE; x++)
    {
        ey = *ptr++;
        pos++;

        if(ey > tsy)
        {
            for(y = tsy + GR_TOP; y < ey + GR_TOP + 1; y++)
                PutPixel(GR_LEFT + pos, y);
        }
        else
        {
            for(y = ey + GR_TOP; y < tsy + GR_TOP + 1; y++)
                PutPixel(GR_LEFT + pos, y);
        }

        if(ptr == (ecgBuffer + ECG_BUFFER_SIZE))
        {
            ptr = ecgBuffer;
            pos = 0;
        }

        tsy = ey;
    }

    // draw grid
    SetColor(LIGHTGRAY);
    SetLineType(DOTTED_LINE);
    for(x = GR_LEFT + ((GR_RIGHT - GR_LEFT) >> 3); x < GR_RIGHT; x += (GR_RIGHT - GR_LEFT) >> 3)
    {
        if((x >= GR_LEFT + temp) && (x <= GR_LEFT + ECG_WINDOW_SIZE + temp))
            WAIT_UNTIL_FINISH(Line(x, GR_TOP, x, GR_BOTTOM));
    }

    for(y = GR_TOP + ((GR_BOTTOM - GR_TOP) >> 3); y < GR_BOTTOM; y += (GR_BOTTOM - GR_TOP) >> 3)
        WAIT_UNTIL_FINISH(Line(GR_LEFT + temp, y, temp + GR_LEFT + ECG_WINDOW_SIZE, y));
    SetLineType(SOLID_LINE);
}

// Processes messages for ECG screen
WORD MsgECG(WORD objMsg, OBJ_HEADER *pObj)
{
    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_BUTTONS;   // goto buttons screen
            }

            return (1);                         // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_POT;       // goto potentiometer screen
            }

            return (1);                         // process by default

        default:
            return (1);                         // process by default
    }
}

// Output text message on screen and stop execution
void ErrorTrap(XCHAR *message)
{
    SetColor(BLACK);
    ClearDevice();
    SetFont((void *) &FONTDEFAULT);
    SetColor(WHITE);
    WAIT_UNTIL_FINISH(OutTextXY(0, 0, message));
    while(1);
}

/*********************************************************************
* Function: Timer4 ISR
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: increments tick counter. Tick is approx. 1 ms.
*
* Note: none
*
********************************************************************/
#ifdef __PIC32MX__
    #define __T4_ISR    __ISR(_TIMER_4_VECTOR, ipl7)
#else
    #define __T4_ISR    __attribute__((interrupt, shadow, auto_psv))
#endif

/* */
void __T4_ISR _T4Interrupt(void)
{
    tick++;

    TMR4 = 0;
    // Clear flag
    #ifdef __PIC32MX__
    mT4ClearIntFlag();
    #else
    IFS1bits.T4IF = 0;
    #endif
    
    TouchDetectPosition();    
}

/*********************************************************************
* Function: void TickInit(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: sets tick timer
*
* Note: none
*
********************************************************************/

/*********************************************************************
 * Section: Tick Delay
 *********************************************************************/
#define SAMPLE_PERIOD       500 // us
#define TICK_PERIOD			(GetPeripheralClock() * SAMPLE_PERIOD) / 4000000

/* */
void TickInit(void)
{
    // Initialize Timer4
    #ifdef __PIC32MX__
    OpenTimer4(T4_ON | T4_PS_1_8, TICK_PERIOD);
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_7);
    #else
    TMR4 = 0;
    PR4 = TICK_PERIOD;
    IFS1bits.T4IF = 0;  //Clear flag
    IEC1bits.T4IE = 1;  //Enable interrupt
    T4CONbits.TON = 1;  //Run timer
    #endif
}

/////////////////////////////////////////////////////////////////////////////
// Function: InitializeBoard()
// Input: none
// Output: none
// Overview: Initializes the hardware components including the PIC device
//           used.
/////////////////////////////////////////////////////////////////////////////
void InitializeBoard(void)
{

    #if defined (PIC24FJ256DA210_DEV_BOARD) && defined(USE_KEYBOARD)
    
     ANSA = 0x0000;
     ANSB = 0x0020;		// RB5 as potentiometer input
     ANSC = 0x0010;		// RC4 as touch screen X+, RC14 as external source of secondary oscillator
     ANSD = 0x0000;
     ANSE = 0x0000;		// RE9 used as S2
     ANSF = 0x0000;
     ANSG = 0x0080;		// RG8 used as S1, RG7 as touch screen Y+
        
    #else
        /////////////////////////////////////////////////////////////////////////////
        // ADC Explorer 16 Development Board Errata (work around 2)
        // RB15 should be output
        /////////////////////////////////////////////////////////////////////////////
        #ifndef MEB_BOARD
            LATBbits.LATB15 = 0;
            TRISBbits.TRISB15 = 0;
        #endif
    #endif


    #if defined(__dsPIC33F__) || defined(__PIC24H__)

        // Configure Oscillator to operate the device at 40Mhz
        // Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
        // Fosc= 8M*40(2*2)=80Mhz for 8M input clock
        PLLFBD = 38;                    // M=40
        CLKDIVbits.PLLPOST = 0;         // N1=2
        CLKDIVbits.PLLPRE = 0;          // N2=2
        OSCTUN = 0;                     // Tune FRC oscillator, if FRC is used
    
        // Disable Watch Dog Timer
        RCONbits.SWDTEN = 0;
    
        // Clock switching to incorporate PLL
        __builtin_write_OSCCONH(0x03);  // Initiate Clock Switch to Primary
    
        // Oscillator with PLL (NOSC=0b011)
        __builtin_write_OSCCONL(0x01);  // Start clock switching
        while(OSCCONbits.COSC != 0b011);
    
        // Wait for Clock switch to occur	
        // Wait for PLL to lock
        while(OSCCONbits.LOCK != 1)
        { };
        
        // Set PMD0 pin functionality to digital
        AD1PCFGL = AD1PCFGL | 0x1000;

        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
            AD1PCFGLbits.PCFG6 = 1;
            AD1PCFGLbits.PCFG7 = 1;
            AD1PCFGLbits.PCFG8 = 1;
        #endif

    #endif // #if defined(__dsPIC33F__) || defined(__PIC24H__)
    

    #if defined (EXPLORER_16)
/************************************************************************
* For Explorer 16 RD12 is connected to EEPROM chip select.
* To prevent a conflict between this EEPROM and SST25 flash
* the chip select of the EEPROM SPI should be pulled up.
************************************************************************/
        // Set IOs directions for EEPROM SPI
        MCHP25LC256_CS_LAT = 1;			    // set initial CS value to 1 (not asserted)
    	MCHP25LC256_CS_TRIS = 0;			// set CS pin to output
	#endif // #if defined (EXPLORER_16)


     //The following are PIC device specific settings for the SPI channel
     //used. 
    
     //Set IOs directions for SST25 SPI
        #if defined (GFX_PICTAIL_V3) || defined (MEB_BOARD) || defined (GFX_PICTAIL_LCC)   
        SST25_CS_LAT = 1;
        SST25_CS_TRIS = 0;
        #ifndef __PIC32MX__
            SST25_SCK_TRIS = 0;
            SST25_SDO_TRIS = 0;
            SST25_SDI_TRIS = 1;
            #if defined(__PIC24FJ256GB210__)
            	SST25_SDI_ANS = 0;
    	    #endif
        #endif
    
    #elif defined (PIC24FJ256DA210_DEV_BOARD)
        SST25_CS_LAT = 1;
        SST25_CS_TRIS = 0;
    
        // Set the pins to be digital 
    	SST25_SDI_ANS = 0;
        SST25_SDO_ANS = 0;

        SST25_SCK_TRIS = 0;
        SST25_SDO_TRIS = 0;
        SST25_SDI_TRIS = 1;
        
	    
	#endif

    // set the peripheral pin select for the PSI channel used
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        AD1PCFGL = 0xFFFF;
        RPOR9bits.RP18R = 11;                   // assign RP18 for SCK2
        RPOR8bits.RP16R = 10;                   // assign RP16 for SDO2
        RPINR22bits.SDI2R = 17;                 // assign RP17 for SDI2	
    #elif defined(__PIC24FJ256GB110__) || defined(__PIC24FJ256GA110__) || defined (__PIC24FJ256GB210__)
        __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
        RPOR10bits.RP21R = 11;                  // assign RP21 for SCK2
        RPOR9bits.RP19R = 10;                   // assign RP19 for SDO2
        RPINR22bits.SDI2R = 26;                 // assign RP26 for SDI2
        __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    #elif defined(__PIC24FJ256DA210__)

        __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    	#if (SST25_SPI_CHANNEL == 1)
    	    RPOR1bits.RP2R = 8;                 // assign RP2 for SCK1
    	    RPOR0bits.RP1R = 7;                 // assign RP1 for SDO1
    	    RPINR20bits.SDI1R = 0;              // assign RP0 for SDI1
        #elif (SST25_SPI_CHANNEL == 2)
            RPOR1bits.RP2R = 11;                // assign RP2 for SCK2
    	    RPOR0bits.RP1R = 10;                // assign RP1 for SDO2
    	    RPINR22bits.SDI2R = 0;              // assign RP0 for SDI2
    	#endif

        __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    #endif

    // initialize the Flash Memory driver
    SST25Init((void*)&SPI_Init_Data);
    
    // initialize the timer that manages the tick counter
    TickInit();                     	

    // change RC4 to a digital input for the PEN_INT signal (JP10 2-3)
    #if defined(__PIC24FJ256DA210__) && defined(USE_TOUCHSCREEN_AR1020)
    ANSCbits.ANSC4 = 0;
    #endif
    
    // initialize the components for Resistive Touch Screen
    TouchInit( NVMWrite, NVMRead, NVMSectorErase, TOUCH_INIT_VALUES);   	

    HardwareButtonInit();           	// Initialize the hardware buttons

    #if !(defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__) || defined(GFX_PICTAIL_LCC))
    RTCCInit();                         // Setup the RTCC
    RTCCProcessEvents();
    #endif
}    
