/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
*****************************************************************************
 * FileName:        TimeDateDemo.c
 * Dependencies:    TimeDateDemo.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright � 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
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
 * Paolo A. Tamayo		03/10/08	... 
 *****************************************************************************/
#include "MainDemo.h"

/////////////////////////////////////////////////////////////////////////////
//                              MACROS
/////////////////////////////////////////////////////////////////////////////
#define ID_DAYSTXT      0xF710              // Static Text for Day ID
#define ID_WKDAYSTXT    0xF711              // Static Text for Week Day ID
#define ID_MOSTXT       0xF712              // Static Text for Month ID
#define ID_YRSTXT       0xF713              // Static Text for Year ID
#define ID_HRSTXT       0xF714              // Static Text for Hour ID
#define ID_MINSTXT      0xF715              // Static Text for Minute ID
#define ID_SECSTXT      0xF716              // Static Text for Seconds ID
#define ID_DUM1STXT     0xF720              // Dummy Static Text
#define ID_DUM2STXT     0xF721              // Dummy Static Text
#define ID_TEMP         0xFFF3

// dimensions of the objects
#define DAYXPOS CtrlBtnTop()

// timing
#if defined(__PIC32MX__)
    #define DATETIME_REFRESH    100
#else
    #define DATETIME_REFRESH    200
#endif

// text colors and backgrounds
#define SCREENBKCOLOR       BLACK           //SCREEN_BACKGROUND_COLOR
#define TIMECOLOR           BRIGHTGREEN     //WHITE
#define TIMEBACKGRND        SCREENBKCOLOR
#define TIMEFOCUSCOLOR      SCREENBKCOLOR   //RGB565CONVERT(0x4C, 0x8E, 0xFF)
#define TIMEFOCUSBACKGRND   BRIGHTGREEN     //RGB565CONVERT(0xFF, 0xBB, 0x4C)
    
    
    /////////////////////////////////////////////////////////////////////////////

//                              LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void                    UpdateTimeVars(void);

/////////////////////////////////////////////////////////////////////////////
//                              FONTS USED
/////////////////////////////////////////////////////////////////////////////
#define DATEFONT    GOLFontDefault
#define TIMEFONT    BigFonts

extern const FONT_FLASH    DATEFONT;
#if defined (EXPLORER_16) || defined (PIC24FJ256DA210_DEV_BOARD)
extern const FONT_EXTERNAL TIMEFONT;
#else
extern const FONT_FLASH TIMEFONT;
#endif

/////////////////////////////////////////////////////////////////////////////
//                              GLOBALS
/////////////////////////////////////////////////////////////////////////////
extern DWORD            tick;               // tick counter
extern XCHAR            UpArrowStr[];
extern XCHAR            DownArrowStr[];
extern XCHAR            ExitStr[];
extern XCHAR            SetStr[];

const XCHAR             Monday[] = {'M','o','n','d','a','y',0};
const XCHAR             Tuesday[] = {'T','u','e','s','d','a','y',0};
const XCHAR             Wednesday[] = {'W','e','d','n','e','s','d','a','y',0};
const XCHAR             Thursday[] = {'T','h','u','r','s','d','a','y',0};
const XCHAR             Friday[] = {'F','r','i','d','a','y',' ',' ',0};
const XCHAR             Saturday[] = {'S','a','t','u','r','d','a','y',0};
const XCHAR             Sunday[] = {'S','u','n','d','a','y',0};

const XCHAR             January[] = {'J','a','n','u','a','r','y',' ',0};
const XCHAR             February[] = {'F','e','b','r','u','a','r','y',0};
const XCHAR             March[] = {'M','a','r','c','h',0};
const XCHAR             April[] = {'A','p','r','i','l',0};
const XCHAR             May[] = {'M','a','y',0};
const XCHAR             June[] = {'J','u','n','e',0};
const XCHAR             July[] = {'J','u','l','y',0};
const XCHAR             August[] = {'A','u','g','u','s','t',0};
const XCHAR             September[] = {'S','e','p','t','e','m','b','e','r',0};
const XCHAR             October[] = {'O','c','t','o','b','e','r',0};
const XCHAR             November[] = {'N','o','v','e','m','b','e','r',0};
const XCHAR             December[] = {'D','e','c','e','m','b','e','r',0};

XCHAR                   *MoItems[13] = {NULL,
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

XCHAR                   *WkDayItems[7] = {(XCHAR*)Sunday,
    (XCHAR*)Monday,
    (XCHAR*)Tuesday,
    (XCHAR*)Wednesday,
    (XCHAR*)Thursday,
    (XCHAR*)Friday,
    (XCHAR*)Saturday};

XCHAR                   *pWkDay, *pMonth;

static XCHAR            Day[3] = {0x30,0x30,0};

static XCHAR            Hour[3] = {'0','0',0};
static XCHAR            Minute[3] = {'0','0',0};
static XCHAR            Second[3] = {'0','0',0};

// date and time style schemes
GOL_SCHEME              *WkDayScheme, *MoScheme;
GOL_SCHEME              *DayScheme, *HourScheme;
GOL_SCHEME              *MinScheme, *SecScheme, *dumScheme;

/************************************************************************
 Function: void CreateTimeDateDemo(void)
                                                                       
 Overview: Creates the Time and Date demo screen.
  		                                         
 Input: none
                                                                       
 Output: none
************************************************************************/
void CreateTimeDateDemo(void)
{
    SHORT           wkdayHeight, wkdayWidth;
    SHORT           dayHeight, dayWidth;
    SHORT           moHeight, moWidth;
    SHORT           timeHeight, timeWidth;
    SHORT           dumWidth;

    static XCHAR    colon[2] = {':',0};
    XCHAR           tmpTime[3] = {'8','8',0};
    int             i;

    GOLFree();              // free memory for the objects in the previous linked list and start new list
    SetColor(SCREENBKCOLOR);
    ClearDevice();

    WkDayScheme = GOLCreateScheme();
    MoScheme = GOLCreateScheme();
    DayScheme = GOLCreateScheme();
    HourScheme = GOLCreateScheme();
    MinScheme = GOLCreateScheme();
    SecScheme = GOLCreateScheme();
    dumScheme = GOLCreateScheme();

    WkDayScheme->CommonBkColor = SCREENBKCOLOR;
    WkDayScheme->TextColor0 = TIMECOLOR;
    WkDayScheme->pFont = (void *) &DATEFONT;

    DayScheme->CommonBkColor = SCREENBKCOLOR;
    DayScheme->TextColor0 = TIMECOLOR;
    DayScheme->pFont = (void *) &DATEFONT;

    MoScheme->CommonBkColor = SCREENBKCOLOR;
    MoScheme->TextColor0 = TIMECOLOR;
    MoScheme->pFont = (void *) &DATEFONT;

    HourScheme->CommonBkColor = SCREENBKCOLOR;
    HourScheme->TextColor0 = TIMECOLOR;
    HourScheme->pFont = (void *) &TIMEFONT;

    MinScheme->CommonBkColor = SCREENBKCOLOR;
    MinScheme->TextColor0 = TIMECOLOR;
    MinScheme->pFont = (void *) &TIMEFONT;

    SecScheme->CommonBkColor = SCREENBKCOLOR;
    SecScheme->TextColor0 = TIMECOLOR;
    SecScheme->pFont = (void *) &TIMEFONT;

    dumScheme->CommonBkColor = SCREENBKCOLOR;
    dumScheme->TextColor0 = TIMECOLOR;
    dumScheme->pFont = (void *) &TIMEFONT;

    // calculate the dimensions of the text to be used
    wkdayHeight = (GOL_EMBOSS_SIZE) + GetTextHeight((void *) &DATEFONT);
    wkdayWidth = (GOL_EMBOSS_SIZE) + GetTextWidth((XCHAR *)Wednesday, (void *) &DATEFONT);
    dayHeight = (GOL_EMBOSS_SIZE) + GetTextHeight((void *) &DATEFONT);
    dayWidth = GetTextWidth((XCHAR *)tmpTime, (void *) &DATEFONT);
    dayWidth = (GOL_EMBOSS_SIZE) + (dayWidth + (dayWidth >> 1));
    moHeight = (GOL_EMBOSS_SIZE) + GetTextHeight((void *) &DATEFONT);
    moWidth = (GOL_EMBOSS_SIZE) + GetTextWidth((XCHAR *)September, (void *) &DATEFONT);

    timeHeight = (GOL_EMBOSS_SIZE) + GetTextHeight((void *) &TIMEFONT);
    timeWidth = GetTextWidth((XCHAR *)tmpTime, (void *) &TIMEFONT);
    timeWidth = (timeWidth >> 1) + timeWidth;
    dumWidth = GetTextWidth(colon, (void *) &TIMEFONT);

    RTCCProcessEvents();    // update the global time and date strings

    // format is:
    //	_time_str[16]; // Sat 10:01:15
    //  _date_str[16]; // Sep 30, 2005
    i = 0;
    while(i < 2)
    {
        Hour[i] = _time_str[i + 4];
        Minute[i] = _time_str[i + 7];
        Second[i] = _time_str[i + 10];
        Day[i] = _date_str[i + 4];
        i++;
    }

    // terminate the Second string properly
    Second[2] = 0;

    pWkDay = WkDayItems[mRTCCGetBinWkDay()];

    // use the binary version here since the number of months is more than 9
    // (bin and dec are not the same)
    pMonth = MoItems[mRTCCGetBinMonth()];

    // create the static texts that will hold the values
    StCreate
    (
        ID_WKDAYSTXT,
        (GetMaxX() - wkdayWidth) >> 1,
        (CtrlBtnTop() >> 1) - (wkdayHeight + (timeHeight >> 1)),
        ((GetMaxX() - wkdayWidth) >> 1) + wkdayWidth,
        ((CtrlBtnTop() >> 1) - (timeHeight >> 1)),
        ST_DRAW | ST_CENTER_ALIGN,
        pWkDay,
        WkDayScheme
    );

    StCreate
    (
        ID_MOSTXT,
        (GetMaxX() >> 1) - ((moWidth + dayWidth) >> 1),
        (CtrlBtnTop() + timeHeight) >> 1,
        (GetMaxX() >> 1) - ((moWidth + dayWidth) >> 1) + moWidth,
        ((CtrlBtnTop() + timeHeight) >> 1) + moHeight,
        ST_DRAW,
        pMonth,
        MoScheme
    );

    StCreate
    (
        ID_DAYSTXT,
        (GetMaxX() >> 1) - ((moWidth + dayWidth) >> 1) + moWidth,
        (CtrlBtnTop() + timeHeight) >> 1,
        (GetMaxX() >> 1) - ((moWidth + dayWidth) >> 1) + moWidth + dayWidth,
        ((CtrlBtnTop() + timeHeight) >> 1) + moHeight,
        ST_DRAW | ST_CENTER_ALIGN,
        Day,
        DayScheme
    );

    StCreate
    (
        ID_HRSTXT,
        (GetMaxX() >> 1) - (timeWidth + dumWidth + (timeWidth >> 1)),
        (CtrlBtnTop() >> 1) - (timeHeight >> 1),
        ((GetMaxX() >> 1) - (timeWidth + dumWidth + (timeWidth >> 1))) + timeWidth,
        (CtrlBtnTop() >> 1) + (timeHeight >> 1),
        ST_DRAW | ST_CENTER_ALIGN,
        Hour,
        HourScheme
    );

    StCreate
    (
        ID_MINSTXT,
        (GetMaxX() >> 1) - (timeWidth >> 1),
        (CtrlBtnTop() >> 1) - (timeHeight >> 1),
        (GetMaxX() >> 1) - (timeWidth >> 1) + timeWidth,
        (CtrlBtnTop() >> 1) + (timeHeight >> 1),
        ST_DRAW | ST_CENTER_ALIGN,
        Minute,
        MinScheme
    );

    StCreate
    (
        ID_SECSTXT,
        (GetMaxX() >> 1) - (timeWidth >> 1) + timeWidth + dumWidth,
        (CtrlBtnTop() >> 1) - (timeHeight >> 1),
        ((GetMaxX() >> 1) - (timeWidth >> 1) + timeWidth) + timeWidth + dumWidth,
        (CtrlBtnTop() >> 1) + (timeHeight >> 1),
        ST_DRAW | ST_CENTER_ALIGN,
        Second,
        SecScheme
    );

    StCreate
    (
        ID_DUM1STXT,
        (GetMaxX() >> 1) - (dumWidth + (timeWidth >> 1)),
        (CtrlBtnTop() >> 1) - (timeHeight >> 1),
        (GetMaxX() >> 1) - (timeWidth >> 1),
        (CtrlBtnTop() >> 1) + (timeHeight >> 1),
        ST_DRAW | ST_CENTER_ALIGN,
        colon,
        dumScheme
    );

    StCreate
    (
        ID_DUM2STXT,
        (GetMaxX() >> 1) + (timeWidth >> 1),
        (CtrlBtnTop() >> 1) - (timeHeight >> 1),
        (GetMaxX() >> 1) + (dumWidth + (timeWidth >> 1)),
        (CtrlBtnTop() >> 1) + (timeHeight >> 1),
        ST_DRAW | ST_CENTER_ALIGN,
        colon,
        dumScheme
    );

    // create the components of the demo
    CreateCtrlButtons(ExitStr, SetStr, UpArrowStr, DownArrowStr);

    // change the style schemes and the behavior of the control buttons
    SetState((BUTTON *)GOLFindObject(ID_BUTTON_C), BTN_DISABLED);
    SetState((BUTTON *)GOLFindObject(ID_BUTTON_D), BTN_DISABLED);
}

/************************************************************************
 Function: WORD TimeDateDemoMsgCallback(WORD objMsg, OBJ_HEADER* pObj, 
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
WORD TimeDateDemoMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    static STATICTEXT   *pStFocus = NULL;
    STATICTEXT          *pPrevFocus;
    WORD                objectID;

    objectID = GetObjID(pObj);

    switch(objectID)
    {
        case ID_BUTTON_A:
            if(objMsg == BTN_MSG_RELEASED)
            {                               // check if button is pressed
                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);

                free(MoScheme);
                free(DayScheme);
                free(WkDayScheme);
                free(HourScheme);
                free(MinScheme);
                free(SecScheme);
                free(dumScheme);

                // Update the time and date with the current settings.
                pWkDay = WkDayItems[mRTCCGetBinWkDay()];
                StSetText((STATICTEXT *)GOLFindObject(ID_WKDAYSTXT), pWkDay);
                SetState((STATICTEXT *)GOLFindObject(ID_WKDAYSTXT), DRAW);

                StSetText((STATICTEXT *)GOLFindObject(ID_MOSTXT), NULL);

                pStFocus = NULL;

                // go to menu screen
                screenState = CREATE_DEMOSELECTION;
            }

            return (1);

        case ID_BUTTON_B:                   // set button
            if(objMsg == BTN_MSG_RELEASED)
            {                               // check if button is pressed
                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);

                if(pStFocus == NULL)
                {
                    ClrState((BUTTON *)GOLFindObject(ID_BUTTON_C), BTN_DISABLED);
                    ClrState((BUTTON *)GOLFindObject(ID_BUTTON_D), BTN_DISABLED);
                    SetState((BUTTON *)GOLFindObject(ID_BUTTON_C), BTN_DRAW);
                    SetState((BUTTON *)GOLFindObject(ID_BUTTON_D), BTN_DRAW);
                    pStFocus = (STATICTEXT *)GOLFindObject(ID_HRSTXT);
                }
                else
                {
                    pPrevFocus = pStFocus;

                    // we skip the week day setting since it will be dependent
                    // on the month and the day of the month.
                    if(GetObjID(pStFocus) == ID_HRSTXT)
                    {
                        pStFocus = (STATICTEXT *)GOLFindObject(ID_MINSTXT);
                    }
                    else if(GetObjID(pStFocus) == ID_MINSTXT)
                    {
                        pStFocus = (STATICTEXT *)GOLFindObject(ID_SECSTXT);
                    }
                    else if(GetObjID(pStFocus) == ID_SECSTXT)
                    {
                        pStFocus = (STATICTEXT *)GOLFindObject(ID_MOSTXT);
                    }
                    else if(GetObjID(pStFocus) == ID_MOSTXT)
                    {
                        pStFocus = (STATICTEXT *)GOLFindObject(ID_DAYSTXT);
                    }
                    else if(GetObjID(pStFocus) == ID_DAYSTXT)
                    {

                        // update the week day being displayed
                        pWkDay = WkDayItems[mRTCCGetBinWkDay()];
                        StSetText((STATICTEXT *)GOLFindObject(ID_WKDAYSTXT), pWkDay);
                        SetState((STATICTEXT *)GOLFindObject(ID_WKDAYSTXT), DRAW);
                        SetState((BUTTON *)GOLFindObject(ID_BUTTON_C), BTN_DISABLED);
                        SetState((BUTTON *)GOLFindObject(ID_BUTTON_D), BTN_DISABLED);
                        SetState((BUTTON *)GOLFindObject(ID_BUTTON_C), BTN_DRAW);
                        SetState((BUTTON *)GOLFindObject(ID_BUTTON_D), BTN_DRAW);
                        pStFocus = NULL;
                    }

                    // restore the previous
                    pPrevFocus->hdr.pGolScheme->CommonBkColor = TIMEBACKGRND;
                    pPrevFocus->hdr.pGolScheme->TextColor0 = TIMECOLOR;
                    SetState(pPrevFocus, DRAW);
                }

                if(pStFocus)
                {

                    // set the current to highlighted or focused colors
                    pStFocus->hdr.pGolScheme->CommonBkColor = TIMEFOCUSBACKGRND;
                    pStFocus->hdr.pGolScheme->TextColor0 = TIMEFOCUSCOLOR;
                    SetState(pStFocus, DRAW);
                }
            }

            return (1);

        case ID_BUTTON_C:                   // increment button
        case ID_BUTTON_D:                   // increment button
            if(pStFocus == NULL)
                return (0);

            if(objMsg == BTN_MSG_RELEASED)
            {                               // check if button is pressed
                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);

                #if defined(__PIC32MX__)

                // open the RTCC for writing
                mRtccWrEnable();
                #endif
                if(GetObjID(pStFocus) == ID_HRSTXT)
                {

                    // increment the current value
                    if(objectID == ID_BUTTON_C)
                        RTCCSetBinHour(mRTCCGetBinHour() + 1);
                    else
                        RTCCSetBinHour(mRTCCGetBinHour() - 1);
                    mRTCCSet();             // copy the new values to the RTCC registers
                    RTCCProcessEvents();    // update string values
                    UpdateTimeVars();
                    StSetText(pStFocus, Hour);
                }
                else if(GetObjID(pStFocus) == ID_MINSTXT)
                {

                    // increment the current value
                    if(objectID == ID_BUTTON_C)
                        RTCCSetBinMin(mRTCCGetBinMin() + 1);
                    else
                        RTCCSetBinMin(mRTCCGetBinMin() - 1);
                    mRTCCSet();             // copy the new values to the RTCC registers
                    RTCCProcessEvents();    // update string values
                    UpdateTimeVars();
                    StSetText(pStFocus, Minute);
                }
                else if(GetObjID(pStFocus) == ID_SECSTXT)
                {

                    // increment the current value
                    if(objectID == ID_BUTTON_C)
                        RTCCSetBinSec(mRTCCGetBinSec() + 1);
                    else
                        RTCCSetBinSec(mRTCCGetBinSec() - 1);
                    mRTCCSet();             // copy the new values to the RTCC registers
                    RTCCProcessEvents();    // update string values
                    UpdateTimeVars();
                    StSetText(pStFocus, Second);
                }
                else if(GetObjID(pStFocus) == ID_MOSTXT)
                {

                    // increment the current value
                    if(objectID == ID_BUTTON_C)
                        RTCCSetBinMonth(mRTCCGetBinMonth() + 1);
                    else
                        RTCCSetBinMonth(mRTCCGetBinMonth() - 1);
                    RTCCCalculateWeekDay();
                    mRTCCSet();             // copy the new values to the RTCC registers
                    RTCCProcessEvents();    // update string values
                    pMonth = MoItems[mRTCCGetBinMonth()];
                    StSetText(pStFocus, pMonth);
                }
                else if(GetObjID(pStFocus) == ID_DAYSTXT)
                {

                    // increment the current value
                    if(objectID == ID_BUTTON_C)
                        RTCCSetBinDay(mRTCCGetBinDay() + 1);
                    else
                        RTCCSetBinDay(mRTCCGetBinDay() - 1);
                    RTCCCalculateWeekDay();
                    mRTCCSet();             // copy the new values to the RTCC registers
                    RTCCProcessEvents();    // update string values
                    UpdateTimeVars();
                    StSetText(pStFocus, Day);
                }

                SetState(pStFocus, DRAW);

                #if defined(__PIC32MX__)

                // open the RTCC for writing
                mRtccWrDisable();
                #endif
            }

            return (1);

        default:
            return (1);
    }

    return (1);
}

/************************************************************************
 Function: WORD TimeDateDemoDrawCallback(void)
 
 Overview: This function is called by GOLDraw() function when the state 
 		   is in this particular demo.

 Input: none

 Output: If the function returns non-zero the draw control will be passed 
		to GOL. GOLDraw() can proceed and re-draw objects that needs 
		to be redrawn.
************************************************************************/
WORD TimeDateDemoDrawCallback(void)
{
    static DWORD    prevTick = 0;   // keeps previous value of tick	
    STATICTEXT      *pSt;
    static BYTE     prevHour, prevMin, prevSec;

    if((tick - prevTick) > DATETIME_REFRESH)
    {
        RTCCProcessEvents();        // update the global time and date strings

        // format is:
        //	_time_str[16]; // Sat 10:01:15
        //  _date_str[16]; // Sep 30, 2005
        pWkDay = WkDayItems[mRTCCGetBinWkDay()];
        pMonth = MoItems[mRTCCGetBinMonth()];
        UpdateTimeVars();

        if(prevMin != mRTCCGetBinMin())
        {
            pSt = (STATICTEXT *)GOLFindObject(ID_MINSTXT);
            StSetText(pSt, Minute);
            SetState(pSt, ST_DRAW); // redraw the time display
            prevMin = mRTCCGetBinMin();
        }

        if(prevSec != mRTCCGetBinSec())
        {
            pSt = (STATICTEXT *)GOLFindObject(ID_SECSTXT);
            StSetText(pSt, Second);
            SetState(pSt, ST_DRAW); // redraw the time display
            prevSec = mRTCCGetBinSec();
        }

        // check if the hour has changed
        if(prevHour != mRTCCGetBinHour())
        {
            pSt = (STATICTEXT *)GOLFindObject(ID_HRSTXT);
            StSetText(pSt, Hour);
            SetState(pSt, ST_DRAW); // redraw the time display
            pSt = (STATICTEXT *)GOLFindObject(ID_WKDAYSTXT);
            StSetText(pSt, pWkDay);
            SetState(pSt, ST_DRAW); // redraw the time display
            pSt = (STATICTEXT *)GOLFindObject(ID_MOSTXT);
            StSetText(pSt, pMonth);
            SetState(pSt, ST_DRAW); // redraw the time display
            pSt = (STATICTEXT *)GOLFindObject(ID_DAYSTXT);
            StSetText(pSt, Day);
            SetState(pSt, ST_DRAW); // redraw the time display
            prevHour = mRTCCGetBinHour();
        }

        prevTick = tick;
    }

    return (1);
}

/* */
void UpdateTimeVars(void)
{
    int i = 0;
    while(i < 2)
    {
        Hour[i] = _time_str[i + 4];
        Minute[i] = _time_str[i + 7];
        Second[i] = _time_str[i + 10];
        Day[i] = _date_str[i + 4];
        i++;
    }
}
