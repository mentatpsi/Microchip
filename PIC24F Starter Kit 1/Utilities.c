/******************************************************************************

    PIC24F Starter Kit Utilities

Description:
    This file contains utilities for the PIC24F Starter Kit.

    Set Date/Time
    The main screen for the PIC24F Starter Kit demonstration program displays
    the current date and time.  If the initial date and time are incorrect,
    use this utility to set it.  The currently selected parameter is indicated
    by a thicker outline.  Select the different parameter by using the left
    and right touchpads.  Adjust the selected parameter by using the up and
    down touchpads.  The RTCC is udated with each change.  Touch the center
    touchpad to exit.

    Calibrate
    If you are using the Starter Kit in a noisy environment, you can change
    the capacitive touch trip points so they are more tailored to your
    environment.  Simply follow the directions on the screen.  The trip
    points are reset to their defaults upon power up.

    Board Test
    Designed as part of the in-house production test, run the OLED and the
    RGB LED through a brief sequence of tests and check for USB device mode
    connection.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

* File Name:       Utilities.c
* Dependencies:    See the #include files below
* Processor:       PIC24FJ256GB1
* Compiler:        C30 v2.01
* Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the “Company”) for its PICmicro® Microcontroller is intended and
supplied to you, the Company’s customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
software is owned by the Company and/or its supplier, and is
protected under applicable copyright laws. All rights are reserved.
Any use in violation of the foregoing restrictions may subject the
user to criminal sanctions under applicable laws, as well as to
civil liability for the breach of the terms and conditions of this
license.

THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

Author          Date    Comments
--------------------------------------------------------------------------------
KO          14-Feb-2008 First release

*******************************************************************************/
//DOM-IGNORE-END

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "PIC24F Starter Kit.h"


//******************************************************************************
//******************************************************************************
// Constants and Enumerations
//******************************************************************************
//******************************************************************************

#define ID_UTILITIES_RTCC           ID_TOP_LEFT
#define ID_UTILITIES_CTMU           ID_BOTTOM_LEFT
#define ID_UTILITIES_HOME           ID_TOP_RIGHT
#define ID_UTILITIES_BOARD_TEST     ID_BOTTOM_RIGHT

#define ID_RTCC_BUTTON_NEXT         10
#define ID_RTCC_BUTTON_PREVIOUS     11
#define ID_RTCC_BUTTON_HOME         12

#define CONTROL_SET_MONTH           0
#define CONTROL_SET_DAY             1
#define CONTROL_SET_YEAR            2
#define CONTROL_SET_HOUR            3
#define CONTROL_SET_MINUTE          4
#define CONTROL_SET_MAX             4

#define ID_START                    50
#define ID_MONTH                    (ID_START +  0)
#define ID_MONTH_PLUS               (ID_START +  1)
#define ID_MONTH_MINUS              (ID_START +  2)
#define ID_DAY                      (ID_START +  3)
#define ID_DAY_PLUS                 (ID_START +  4)
#define ID_DAY_MINUS                (ID_START +  5)
#define ID_YEAR                     (ID_START +  6)
#define ID_YEAR_PLUS                (ID_START +  7)
#define ID_YEAR_MINUS               (ID_START +  8)
#define ID_HOUR                     (ID_START +  9)
#define ID_HOUR_PLUS                (ID_START + 10)
#define ID_HOUR_MINUS               (ID_START + 11)
#define ID_MINUTE                   (ID_START + 12)
#define ID_MINUTE_PLUS              (ID_START + 13)
#define ID_MINUTE_MINUS             (ID_START + 14)

#define CURRENT_EDITBOX             (ID_START + currentControlSet * 3)
#define CURRENT_PLUS                (ID_START + currentControlSet * 3 + 1)
#define CURRENT_MINUS               (ID_START + currentControlSet * 3 + 2)

#define BUTTON_WIDTH                10
#define CONTROL_SPACING_X           5
#define CONTROL_SPACING_Y           4
#define LINE1_START_Y               (LOCATION_MIN_Y + 2)
#define LINE1_END_Y                 (LINE1_START_Y + textHeight )
#define LINE2_START_Y               (LINE1_END_Y + CONTROL_SPACING_Y)
#define LINE2_END_Y                 (LINE2_START_Y + textHeight )

#define MONTH_START_X               1
#define MONTH_END_X                 (MONTH_START_X + textWidth)
#define MONTH_START_Y               LINE1_START_Y
#define MONTH_END_Y                 LINE1_END_Y

#define MONTH_BUTTON_START_X        (MONTH_END_X + 1 )
#define MONTH_BUTTON_END_X          (MONTH_BUTTON_START_X + BUTTON_WIDTH)

#define DAY_START_X                 43 //(MONTH_BUTTON_END_X + CONTROL_SPACING_X)
#define DAY_END_X                   (DAY_START_X + textWidth)
#define DAY_START_Y                 LINE1_START_Y
#define DAY_END_Y                   LINE1_END_Y

#define DAY_BUTTON_START_X          (DAY_END_X + 1)
#define DAY_BUTTON_END_X            (DAY_BUTTON_START_X + BUTTON_WIDTH)

#define YEAR_START_X                80 //(DAY_BUTTON_END_X + CONTROL_SPACING_X)
#define YEAR_END_X                  (YEAR_START_X + textWidth)
#define YEAR_START_Y                LINE1_START_Y
#define YEAR_END_Y                  LINE1_END_Y

#define YEAR_BUTTON_START_X         (YEAR_END_X + 1)
#define YEAR_BUTTON_END_X           (YEAR_BUTTON_START_X + BUTTON_WIDTH)

#define HOUR_START_X                20
#define HOUR_END_X                  (HOUR_START_X + textWidth )
#define HOUR_START_Y                LINE2_START_Y
#define HOUR_END_Y                  LINE2_END_Y

#define HOUR_BUTTON_START_X         (HOUR_END_X + 1)
#define HOUR_BUTTON_END_X           (HOUR_BUTTON_START_X + BUTTON_WIDTH)

#define MINUTE_START_X              (HOUR_BUTTON_END_X + CONTROL_SPACING_X)
#define MINUTE_END_X                (MINUTE_START_X + textWidth)
#define MINUTE_START_Y              LINE2_START_Y
#define MINUTE_END_Y                LINE2_END_Y

#define MINUTE_BUTTON_START_X       (MINUTE_END_X + 1)
#define MINUTE_BUTTON_END_X         (MINUTE_BUTTON_START_X + BUTTON_WIDTH)


// CTMU Calibration Constants

#define CTMU_USER_LAG               (2000 / MILLISECONDS_PER_TICK)
#define CTMU_READ_TIME              (2000 / MILLISECONDS_PER_TICK)
#define CTMU_NUM_READINGS           256
#if defined TOUCHPAD_DIRECT_TOUCH
	#define DEFAULT_TRIP_VALUE          0x1000
#endif

#if defined TOUCHPAD_USE_OVERLAY
	#define DEFAULT_TRIP_VALUE          125
#endif
#define TRIP_MINIMUM                50


// CTMU Calibration States

typedef enum _CTMU_STATES
{
    CTMU_INITIALIZE,
    CTMU_SHOW_PRESS_SCREEN,
    CTMU_PRESS_WAIT,
    CTMU_READ_PRESS,
    CTMU_SHOW_RELEASE_SCREEN,
    CTMU_RELEASE_WAIT,
    CTMU_READ_RELEASE,
    CTMU_SHOW_COMPLETE,
    CTMU_COMPLETE_WAIT
} CTMU_STATES;


//******************************************************************************
//******************************************************************************
// Global Variables
//******************************************************************************
//******************************************************************************

// CTMU Calibration Variables
CTMU_STATES             calibrationState;
WORD                    countCTMU;
WORD                    currentTouchpad;
extern unsigned int     hyst   [NUM_TOUCHPADS];       //hysterisis for TOUCHPAD
extern unsigned int     rawCTMU[NUM_TOUCHPADS];       //raw A/D value
DWORD                   stateTick;
DWORD                   sumCTMUPressed[NUM_TOUCHPADS];
DWORD                   sumCTMUReleased[NUM_TOUCHPADS];
extern unsigned int     trip   [NUM_TOUCHPADS];       //trip point for TOUCHPAD


// Set Date/Time Variables

SHORT                   currentControlSet;
BYTE                    daysPerMonth[12] = {   31, 28, 31, 30, 31, 30,
                                               31, 31, 30, 31, 30, 31 };
XCHAR                   monthNames[12][3]  = {
                                                { 'J', 'a', 'n', },
                                                { 'F', 'e', 'b', },
                                                { 'M', 'a', 'r', },
                                                { 'A', 'p', 'r', },
                                                { 'M', 'a', 'y', },
                                                { 'J', 'u', 'n', },
                                                { 'J', 'u', 'l', },
                                                { 'A', 'u', 'g', },
                                                { 'S', 'e', 'p', },
                                                { 'O', 'c', 't', },
                                                { 'N', 'o', 'v', },
                                                { 'D', 'e', 'c'  } };

//*****************************************************************************
//******************************************************************************
// Function Prototypes
//******************************************************************************
//******************************************************************************

void UpdateDateTime( void );


/****************************************************************************
  Function:
    void CalibrateCTMU( void )

  Description:
    This function calibrates the CTMU for a particular environment.  It
    instructs the user to hold and release particular touchpads while it
    captures readings, and uses those readings to calculate the trip points
    and hysteresis of the individual touchpads.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    This routine is not implemented as efficiently as it could be as far as
    RAM usage.  This is intentional, so we can view the differences between
    the touchpads.
  ***************************************************************************/

void CalibrateCTMU( void )
{
    unsigned int    calculatedTrip;
    int             i;

    switch( calibrationState )
    {
        case CTMU_INITIALIZE:
            for (i=0; i<NUM_TOUCHPADS; i++)
            {
                sumCTMUPressed[i]         = 0;
                sumCTMUReleased[i]        = 0;
            }
            // Fall through...

        case CTMU_SHOW_PRESS_SCREEN:
            sprintf( pageInformation.instructions, "Hold touchpad %d", currentTouchpad+1 );
            SetState( (STATICTEXT *)GOLFindObject( ID_INSTRUCTIONS ), ST_DRAW );
            calibrationState ++;
            countCTMU       = 0;
            previousTick    = tick;
            stateTick       = tick;
            break;

        case CTMU_PRESS_WAIT:
            if ((tick - stateTick) >= CTMU_USER_LAG)
            {
                stateTick = tick;
                calibrationState ++;
            }
            break;

        case CTMU_READ_PRESS:
            if (tick != previousTick)
            {
                sumCTMUPressed[currentTouchpad] += rawCTMU[currentTouchpad];
                countCTMU ++;
                previousTick = tick;
            }

            if (countCTMU == CTMU_NUM_READINGS)
            {
                stateTick = tick;
                calibrationState ++;
            }
            break;

        case CTMU_SHOW_RELEASE_SCREEN:
            strcpy( pageInformation.instructions, "Release all touchpads" );
            SetState( (STATICTEXT *)GOLFindObject( ID_INSTRUCTIONS ), ST_DRAW );
            calibrationState ++;
            countCTMU       = 0;
            previousTick    = tick;
            stateTick       = tick;
            break;

        case CTMU_RELEASE_WAIT:
            if ((tick - stateTick) >= CTMU_USER_LAG)
            {
                stateTick = tick;
                calibrationState ++;
            }
            break;

        case CTMU_READ_RELEASE:
            if (tick != previousTick)
            {
                sumCTMUReleased[currentTouchpad] += rawCTMU[currentTouchpad];
                countCTMU ++;
                previousTick = tick;
            }

            if (countCTMU == CTMU_NUM_READINGS)
            {
#if defined TOUCHPAD_DIRECT_TOUCH
                // Calculate and store the trip and hysteresis for a button press.
                // Trip value is 2/3 of unpressed - pressed
                calculatedTrip = ((sumCTMUReleased[currentTouchpad] - sumCTMUPressed[currentTouchpad]) / CTMU_NUM_READINGS /3)*2;
#else

                // Calculate and store the trip and hysteresis for a button press.
                // Trip value is 9/10 of unpressed - pressed
                calculatedTrip = ((sumCTMUReleased[currentTouchpad] - sumCTMUPressed[currentTouchpad]) / CTMU_NUM_READINGS /16)*15;
#endif

                //Test for case where buttons are not pressed or incorrectly pressed during calibration
                if(calculatedTrip  < TRIP_MINIMUM)
                {
                    trip[currentTouchpad] = DEFAULT_TRIP_VALUE;
                }
                else
                {
                    trip[currentTouchpad] = calculatedTrip;

                }
#if defined TOUCHPAD_DIRECT_TOUCH
                hyst[currentTouchpad] = trip[currentTouchpad] / 4;
#else
                hyst[currentTouchpad] = trip[currentTouchpad] / 8;
#endif
                stateTick = tick;
                currentTouchpad ++;
                if (currentTouchpad >= NUM_TOUCHPADS)
                {
                    // We are done with calibration
                    calibrationState ++;
                }
                else
                {
                    // Calibrate the next touchpad.
                    calibrationState = CTMU_SHOW_PRESS_SCREEN;
                }
            }
            break;

        case CTMU_SHOW_COMPLETE:
            strcpy( pageInformation.instructions, "Calibration complete!" );
            SetState( (STATICTEXT *)GOLFindObject( ID_INSTRUCTIONS ), ST_DRAW );
            calibrationState ++;
            stateTick = tick;
            break;

        case CTMU_COMPLETE_WAIT:
            if ((tick - stateTick) >= CTMU_USER_LAG)
            {
                screenState = SCREEN_DISPLAY_MAIN;
            }
            break;
    }
}


/****************************************************************************
  Function:
    void IndicateFocus( SHORT lineColor )

  Description:
    This routine is used to either place or remove a focus indication
    around a control, as determined by the color of the line to be drawn
    around the control group.

  Precondition:
    ShowScreenTime() must be called prior to calling this routine so the
    correct controls are available.

  Parameters:
    SHORT lineColor - The color of the line to draw around the control set.

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void IndicateFocus( SHORT lineColor )
{
    EDITBOX *currentEditbox;
    BUTTON  *currentMinus;

    currentEditbox  = (EDITBOX *)GOLFindObject( CURRENT_EDITBOX );
    currentMinus    = (BUTTON *) GOLFindObject( CURRENT_MINUS );

    SetColor( lineColor );
    SetLineType( SOLID_LINE );

    Rectangle(  currentEditbox->hdr.left-1, currentEditbox->hdr.top-1,
                currentMinus->hdr.right+1, currentMinus->hdr.bottom+1 );
}


/****************************************************************************
  Function:
    WORD ProcessMessageCTMU( WORD translatedMsg, OBJ_HEADER* pObj,
                    GOL_MSG* pMsg )

  Description:
    This routine processes messages for the CTMU calibration screen.  For
    this screen, all messages are ignored.

  Precondition:
    Call ShowScreenCTMUs() prior to using this function to display the
    correct screen.

  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    None
  ***************************************************************************/

WORD ProcessMessageCTMU( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    // All messages are ignored, and no default processing is required.
    return 0;
}


/****************************************************************************
  Function:
    WORD ProcessMessageTime( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

  Description:
    This function processes the messages for the time and date screen.  It
    allows the user to set the current time and date of the RTCC module.

  Precondition:
    Call ShowScreenTime() prior to using this function to display the correct
    screen.

  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    None
  ***************************************************************************/

WORD ProcessMessageTime( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    signed char change;
    WORD        controlID;
    signed char day;
    signed char dayMax;
    signed char hour;
    signed char minute;
    signed char month;
    signed char year;


    controlID = GetObjID(pObj);
    switch (controlID)
    {
       case ID_RTCC_BUTTON_NEXT:
            // Show the old control set as unselected.
            IndicateFocus( BLACK );

            // Set the next control as active.
            currentControlSet ++;
            if (currentControlSet > CONTROL_SET_MAX)
            {
                currentControlSet = 0;
            }

            // Show the new control set as selected.
            IndicateFocus( WHITE );

            return 0;   // Hidden button
            break;

       case ID_RTCC_BUTTON_PREVIOUS:
            // Show the old control set as unselected.
            IndicateFocus( BLACK );

            // Set the next control as active.
            currentControlSet --;
            if (currentControlSet < 0 )
            {
                currentControlSet = CONTROL_SET_MAX;
            }

            // Show the new control set as selected.
            IndicateFocus( WHITE );

            return 0;   // Hidden button
            break;

        case ID_RTCC_BUTTON_HOME:
            screenState = SCREEN_DISPLAY_MAIN;
            return 0;   // Hidden button
            break;

        case ID_DAY_PLUS:
        case ID_MONTH_PLUS:
        case ID_YEAR_PLUS:
        case ID_HOUR_PLUS:
        case ID_MINUTE_PLUS:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                change = +1;
            }
            else
            {
                return 1;
            }
            break;

        case ID_DAY_MINUS:
        case ID_MONTH_MINUS:
        case ID_YEAR_MINUS:
        case ID_HOUR_MINUS:
        case ID_MINUTE_MINUS:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                change = -1;
            }
            else
            {
                return 1;
            }
            break;

        default:
            return 0;
    }

    // Update the selected field.

    RTCCProcessEvents();
    hour    = RTCCGetBinHour();
    minute  = RTCCGetBinMin();
    day     = RTCCGetBinDay();
    month   = RTCCGetBinMonth();
    year    = RTCCGetBinYear();

    dayMax  = daysPerMonth[month-1];

    // February has one day more for a leap year, unless it is on the thousands
    if ((month == 2) && ((year % 4) == 0) && (year != 0))
    {
        dayMax ++;
    }

    // Change the appropriate setting.  Allow the controls to roll over.
    switch (currentControlSet)
    {
        case CONTROL_SET_DAY:
            day += change;
            if (day < 1)
            {
                day = dayMax;
            }
            if (day > dayMax)
            {
                day = 1;
            }
            break;

        case CONTROL_SET_MONTH:
            month += change;
            if (month < 1)
            {
                month = 12;
            }
            if (month > 12)
            {
                month = 1;
            }
            break;

        case CONTROL_SET_YEAR:
            year += change;
            if (year < 0)
            {
                year = 99;
            }
            if (year > 99)
            {
                year = 0;
            }
            break;

        case CONTROL_SET_HOUR:
            hour += change;
            if (hour < 0)
            {
                hour = 23;
            }
            if (hour > 23)
            {
                hour = 0;
            }
            break;

        case CONTROL_SET_MINUTE:
            minute += change;
            if (minute < 0)
            {
                minute = 59;
            }
            if (minute > 59)
            {
                minute = 0;
            }
            break;

        default:
            return 0;
    }

    RTCCOff();
    RTCCSetBinHour( hour );
    RTCCSetBinMin( minute );
    RTCCSetBinSec( 0 );
    RTCCSetBinMonth( month );
    RTCCSetBinYear( year );
    RTCCSetBinDay( day );
    RTCCCalculateWeekDay();
    RTCCSet();                      // Copy the new values to the RTCC registers
    UpdateDateTime();               // Update the display.

    return 1;   // Call the default handler to show the button state.
}


/****************************************************************************
  Function:
    WORD ProcessMessageUtilities( WORD translatedMsg, OBJ_HEADER* pObj,
                    GOL_MSG* pMsg )

  Description:
    This function processes the messages for the utility selection screen.

  Precondition:
    None

  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    None
  ***************************************************************************/

WORD ProcessMessageUtilities( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    WORD    buttonID;

    buttonID = GetObjID(pObj);

    switch (buttonID)
    {
        case ID_UTILITIES_RTCC:
            if (translatedMsg == BTN_MSG_RELEASED)
            {
                screenState = SCREEN_DISPLAY_TIME;
            }
            break;

        case ID_UTILITIES_CTMU:
            if (translatedMsg == BTN_MSG_RELEASED)
            {
                screenState = SCREEN_DISPLAY_CTMU;
            }
            break;

        case ID_UTILITIES_BOARD_TEST:
            if (translatedMsg == BTN_MSG_RELEASED)
            {
                screenState = SCREEN_BOARD_TEST;;
            }
            break;

        case ID_UTILITIES_HOME:
            if (translatedMsg == BTN_MSG_RELEASED)
            {
                screenState = SCREEN_DISPLAY_MAIN;
            }
            break;

    }

    // Keep the current unimplemented button "pressed"
    SetState( (BUTTON*)pObj, BTN_PRESSED | BTN_DRAW );

    return 0;   // We do not need to call the default handler.
}


/****************************************************************************
  Function:
    void ShowScreenCTMU( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the screen that allows the user to calibrate the CTMU for detecting
    touchpad touches.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenCTMU( void )
{

    strcpy( pageInformation.title, "Calibrate Touchpads" );
    pageInformation.instructions[0] = 0;

    InitializeScreen();

    StCreate(
                ID_INSTRUCTIONS,
                0, 30, 127, 30+GetTextHeight( (void *)&FONTDEFAULT )-1,
                ST_DRAW,
                pageInformation.instructions,
                NULL );

    // Initialize calibration variables.
    calibrationState    = CTMU_INITIALIZE;
    currentTouchpad     = 0;
}


/****************************************************************************
  Function:
    void ShowScreenTime( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the screen that allows the user to set the current date and time of the
    RTCC module.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenTime( void )
{
    WORD    textHeight;
    WORD    textWidth;

    strcpy( pageInformation.title, "Set Date/Time" );

    InitializeScreen();

    RTCCProcessEvents();

    // Draw the hidden controls.

    BtnCreate(
                        ID_RTCC_BUTTON_HOME,
                        GetMaxX()-8, 0, GetMaxX()-6, 4, 0,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(
                        ID_RTCC_BUTTON_NEXT,
                        GetMaxX()-6, 0, GetMaxX()-4, 4, 0,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(
                        ID_RTCC_BUTTON_PREVIOUS,
                        GetMaxX()-4, 0, GetMaxX()-2, 4, 0,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    // -------------------------------------------------------------------------
    // Create Month, Day and Year Edit Boxes and Buttons

    textWidth  = GetTextWidth( "Mmm", (void *)&FONTDEFAULT );
    textHeight = GetTextHeight( (void *)&FONTDEFAULT ) + 2*GOL_EMBOSS_SIZE;

    // Create the month components

    EbCreate(
                ID_MONTH,
                MONTH_START_X,
                MONTH_START_Y,
                MONTH_END_X,
                MONTH_END_Y,
                EB_DRAW,                    // will be dislayed after creation
                &_date_str[0],              // use the month string value
                3,                          // max characters is 3
                NULL );                     // default GOL scheme

    BtnCreate(
                ID_MONTH_PLUS,
                MONTH_BUTTON_START_X,
                MONTH_START_Y,
                MONTH_BUTTON_END_X,
                MONTH_START_Y + (textHeight >> 1),
                0,                          // draw rectangular button
                BTN_DRAW,                   // will be dislayed after creation
                NULL,                       // no bitmap
                "+",                        // DOWN arrow as text
                NULL );                     // use navigation scheme

    BtnCreate(  ID_MONTH_MINUS,
                MONTH_BUTTON_START_X,
                MONTH_START_Y + (textHeight >> 1),
                MONTH_BUTTON_END_X,
                MONTH_END_Y,
                0,                          // draw rectangular button
                BTN_DRAW,                   // will be dislayed after creation
                NULL,                       // no bitmap
                "-",                        // DOWN arrow as text
                NULL );                     // use navigation scheme

    // Create the day components

    textWidth  = GetTextWidth( "MM", (void *)&FONTDEFAULT );

    EbCreate(
                ID_DAY,
                DAY_START_X,
                DAY_START_Y,
                DAY_END_X,
                DAY_END_Y,
                EB_DRAW,                    // will be dislayed after creation
                &_date_str[4],              // use the day string value
                2,                          // max characters is 3
                NULL );                     // default GOL scheme

    BtnCreate(
                ID_DAY_PLUS,
                DAY_BUTTON_START_X,
                DAY_START_Y,
                DAY_BUTTON_END_X,
                DAY_START_Y + (textHeight >> 1),
                0,                          // draw rectangular button
                BTN_DRAW,                   // will be dislayed after creation
                NULL,                       // no bitmap
                "+",                        // DOWN arrow as text
                NULL );                     // use navigation scheme

    BtnCreate(  ID_DAY_MINUS,
                DAY_BUTTON_START_X,
                DAY_START_Y + (textHeight >> 1),
                DAY_BUTTON_END_X,
                DAY_END_Y,
                0,                          // draw rectangular button
                BTN_DRAW,                   // will be dislayed after creation
                NULL,                       // no bitmap
                "-",                        // DOWN arrow as text
                NULL );                     // use navigation scheme

    // Create the year components

    textWidth  = GetTextWidth( "MM00", (void *)&FONTDEFAULT );

    EbCreate(
                ID_YEAR,
                YEAR_START_X,
                YEAR_START_Y,
                YEAR_END_X,
                YEAR_END_Y,
                EB_DRAW,                // will be dislayed after creation
                &_date_str[8],          // use the year string value
                4,                      // max characters is 4
                NULL );                 // default GOL scheme

    BtnCreate(
                ID_YEAR_PLUS,
                YEAR_BUTTON_START_X,
                YEAR_START_Y,
                YEAR_BUTTON_END_X,
                YEAR_START_Y + (textHeight >> 1),
                0,                          // draw rectangular button
                BTN_DRAW,                   // will be dislayed after creation
                NULL,                       // no bitmap
                "+",                        // DOWN arrow as text
                NULL );                     // use navigation scheme

    BtnCreate(  ID_YEAR_MINUS,
                YEAR_BUTTON_START_X,
                YEAR_START_Y + (textHeight >> 1),
                YEAR_BUTTON_END_X,
                YEAR_END_Y,
                0,                          // draw rectangular button
                BTN_DRAW,                   // will be dislayed after creation
                NULL,                       // no bitmap
                "-",                        // DOWN arrow as text
                NULL );                     // use navigation scheme

    // -------------------------------------------------------------------------
    // Create the Hour and Minute Edit Boxes and Buttons

    textWidth  = GetTextWidth( "MM", (void *)&FONTDEFAULT ) + (GOL_EMBOSS_SIZE<<1);

    // Create the hour components

    EbCreate(
                ID_HOUR,
                HOUR_START_X,
                HOUR_START_Y,
                HOUR_END_X,
                HOUR_END_Y,
                EB_DRAW,
                &_time_str[4],
                2,
                NULL );

    BtnCreate(
                ID_HOUR_PLUS,
                HOUR_BUTTON_START_X,
                HOUR_START_Y,
                HOUR_BUTTON_END_X,
                HOUR_START_Y + (textHeight >> 1),
                0,
                BTN_DRAW,
                NULL,
                "+",
                NULL );

    BtnCreate(  ID_HOUR_MINUS,
                HOUR_BUTTON_START_X,
                HOUR_START_Y + (textHeight >> 1),
                HOUR_BUTTON_END_X,
                HOUR_END_Y,
                0,
                BTN_DRAW,
                NULL,
                "-",
                NULL );

    // Create the minute components

    EbCreate(
                ID_MINUTE,
                MINUTE_START_X,
                MINUTE_START_Y,
                MINUTE_END_X,
                MINUTE_END_Y,
                EB_DRAW,                // will be dislayed after creation
                &_time_str[7],          // use minute string value
                2,                      // max characters is 2
                NULL );                 // default GOL scheme

    BtnCreate(
                ID_MINUTE_PLUS,
                MINUTE_BUTTON_START_X,
                MINUTE_START_Y,
                MINUTE_BUTTON_END_X,
                MINUTE_START_Y + (textHeight >> 1),
                0,                          // draw rectangular button
                BTN_DRAW,                   // will be dislayed after creation
                NULL,                       // no bitmap
                "+",                        // DOWN arrow as text
                NULL );                     // use navigation scheme

    BtnCreate(  ID_MINUTE_MINUS,
                MINUTE_BUTTON_START_X,
                MINUTE_START_Y + (textHeight >> 1),
                MINUTE_BUTTON_END_X,
                MINUTE_END_Y,
                0,                          // draw rectangular button
                BTN_DRAW,                   // will be dislayed after creation
                NULL,                       // no bitmap
                "-",                        // DOWN arrow as text
                NULL );                     // use navigation scheme


    currentControlSet = CONTROL_SET_MONTH;
    IndicateFocus( WHITE );

}


/****************************************************************************
  Function:
    void ShowScreenUtilities( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the screen to allow the user to select which utility to execute.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced.  Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenUtilities( void )
{
    strcpy( pageInformation.title, "Utilities" );

    strcpy( pageInformation.buttonTextTopLeft, "Date/Time" );
    strcpy( pageInformation.buttonTextBottomLeft, "Calibrate" );
    strcpy( pageInformation.buttonTextTopRight, "Home" );
    strcpy( pageInformation.buttonTextBottomRight, "Board Test" );
	strcpy( pageInformation.buttonTextThirdrow, 	"" );

    pageInformation.buttonBitmapTopLeft        = NULL;
    pageInformation.buttonBitmapBottomLeft     = NULL;
    pageInformation.buttonBitmapTopRight       = NULL;
    pageInformation.buttonBitmapBottomRight    = NULL;

    strcpy( pageInformation.instructions, VERSION_STRING );

    ShowScreenGeneric( );
}


/****************************************************************************
  Function:
    void TranslateMessageCTMU( GOL_MSG * pMsg )

  Description:
    This routine translates messages during CTMU calibration.  During
    calibration, we ignore all messages.  We change the message to invalid so
    it is not processed.

  Precondition:
    None

  Parameters:
    GOL_MSG* pMsg       - The original system message information

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void TranslateMessageCTMU( GOL_MSG * pMsg )
{
    // All messages are ignored during calibration.
    pMsg->uiEvent = EVENT_INVALID;
}


/****************************************************************************
  Function:
    void TranslateMessageTime( GOL_MSG * pMsg )

  Description:
    If the message is valid, this function translates the raw touchpad message
    pMsg->param1 to the proper message for the appropriate control.

  Precondition:
    ShowScreenTime() must be the last screen display routine called before
    this function executes.

  Parameters:
    GOL_MSG* pMsg       - The original system message information

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void TranslateMessageTime( GOL_MSG * pMsg )
{
    // If the message is valid, we need to translate
    // pMsg->param1 to the proper control ID depending
    // on what control set is selected.

    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_TOUCH_PAD))
    {
        if ((pMsg->param2 == SCAN_UP_PRESSED) ||
            (pMsg->param2 == SCAN_UP_RELEASED))
        {
            switch (currentControlSet)
            {
                case CONTROL_SET_MONTH:
                    pMsg->param1 = ID_MONTH_PLUS;
                    break;
                case CONTROL_SET_DAY:
                    pMsg->param1 = ID_DAY_PLUS;
                    break;
                case CONTROL_SET_YEAR:
                    pMsg->param1 = ID_YEAR_PLUS;
                    break;
                case CONTROL_SET_HOUR:
                    pMsg->param1 = ID_HOUR_PLUS;
                    break;
                case CONTROL_SET_MINUTE:
                    pMsg->param1 = ID_MINUTE_PLUS;
                    break;
            }

            if (pMsg->param2 == SCAN_UP_PRESSED)
            {
                pMsg->param2 = SCAN_CR_PRESSED;
            }
            else
            {
                pMsg->param2 = SCAN_CR_RELEASED;
            }
        }
        else if ((pMsg->param2 == SCAN_DOWN_PRESSED) ||
                 (pMsg->param2 == SCAN_DOWN_RELEASED))
        {
            switch (currentControlSet)
            {
                case CONTROL_SET_MONTH:
                    pMsg->param1 = ID_MONTH_MINUS;
                    break;
                case CONTROL_SET_DAY:
                    pMsg->param1 = ID_DAY_MINUS;
                    break;
                case CONTROL_SET_YEAR:
                    pMsg->param1 = ID_YEAR_MINUS;
                    break;
                case CONTROL_SET_HOUR:
                    pMsg->param1 = ID_HOUR_MINUS;
                    break;
                case CONTROL_SET_MINUTE:
                    pMsg->param1 = ID_MINUTE_MINUS;
                    break;
            }

            if (pMsg->param2 == SCAN_DOWN_PRESSED)
            {
                pMsg->param2 = SCAN_CR_PRESSED;
            }
            else
            {
                pMsg->param2 = SCAN_CR_RELEASED;
            }
        }
        else if (pMsg->param2 == SCAN_RIGHT_PRESSED)
        {
            pMsg->param1 = ID_RTCC_BUTTON_NEXT;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else if (pMsg->param2 == SCAN_LEFT_PRESSED)
        {
            pMsg->param1 = ID_RTCC_BUTTON_PREVIOUS;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else if (pMsg->param2 == SCAN_CR_PRESSED)
        {
            pMsg->param1 = ID_RTCC_BUTTON_HOME;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
    }
}

/****************************************************************************
  Function:
    void UpdateDateTime( void )

  Description:
    This function updates the displayed time and date to match the value
    currently in the RTCC.

  Precondition:
    ShowScreenTime() must be called before calling this routine to display
    the correct controls.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void UpdateDateTime( void )
{
    OBJ_HEADER* obj;

    RTCCProcessEvents();                // update the time and date

    // update time edit boxes
    obj = GOLFindObject(ID_HOUR);
    EbSetText((EDITBOX*)obj, &_time_str[4]);
    SetState(obj, EB_DRAW);
    obj = GOLFindObject(ID_MINUTE);
    EbSetText((EDITBOX*)obj, &_time_str[7]);
    SetState(obj, EB_DRAW);

    // update date edit boxes
    obj = GOLFindObject(ID_MONTH);
    EbSetText((EDITBOX*)obj, &_date_str[0]);
    SetState(obj, EB_DRAW);
    obj = GOLFindObject(ID_DAY);
    EbSetText((EDITBOX*)obj, &_date_str[4]);
    SetState(obj, EB_DRAW);
    obj = GOLFindObject(ID_YEAR);
    EbSetText((EDITBOX*)obj, &_date_str[8]);
    SetState(obj, EB_DRAW);
}


