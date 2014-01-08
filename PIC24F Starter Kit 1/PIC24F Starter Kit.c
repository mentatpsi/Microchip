/******************************************************************************
   
PIC24F Starter Kit 1

The PIC24F Starter Kit firmware provides several examples of some of the 
features of Microchip's PIC24F microcontrollers.  The highlighted features 
include:

    - USB Embedded Host and USB Device capability - Your application can
        utilize USB devices, or can become a USB device, by utilizing one
        of the PIC24FJ256GB1 microcontrollers.
    - CTMU - Use capacitive touch sensing to obtain user input, instead of
        switches that can wear out.
    - Peripheral Pin Select - Optimized your output pins by redirecting,
        optimizing, separating, and even overlaying digital pin connections.
    - Parallel Master Port - Communicate easily with sophisticated devices,
        such as graphics displays.
    - Real Time Clock-Calendar - Simplify date and time tracking.
    - Nine independent Output Compare modules - Increase the number and
        complexity of the external devices controlled by your application.

In addition, check out the horsepower of the PIC24F:

    - Plenty of processing power - provide a real-time graphics display and
        monitor user input while capturing time-dependent data and storing it
        to a USB flash drive.
    - Plenty of room for growth - the entire demo (including USB Embedded Host,
        File System, and Graphics libraries), written in C and compiled
        with no optimizations, requires less than half of the available
        program and data memory of the PIC24FJ256GB106.

The structure of this demo matches the layout of the demo.  This file contains
the main line function and coordinates the graphics message and draw
processing.  The other source files are:
    Demos.c     Contains the demonstrations that are accessed via the
                Demos menu selection. These include controlling the RGB LED,
                displaying real-time graphical data, and capturing time
                sensitive data while displaying it graphically and saving it
                to a USB flash drive.
    Flash.c     Contains the demonstrations that are accessed via the
                Flash menu selection.  This includes the USB flash drive
                directory explorer.
    Games.c     Contains the demonstrations that are accessed via the
                Games menu selection.
    Utilities.c Contains the demonstrations that are accessed via the
                Utilities menu selection.  These include setting the current
                time and date, and calibrating the CTMU.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

* File Name:       PIC24F Starter Kit.c
* Dependencies:    None
* Processor:       PIC24FJ256GB1
* Compiler:        C30 v3.02c
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
// Configuration Bits
//******************************************************************************
//******************************************************************************

#define PLL_96MHZ_OFF   0xFFFF
#define PLL_96MHZ_ON    0xF7FF


#if defined( __PIC24FJ256GB110__ ) || defined( __PIC24FJ256GB106__ )
    #ifdef USE_FRC
        // Using the FRC (8MHz), no clock from the PIC18F67J50.
        // NOTE: USB operation is not guaranteed
        _CONFIG2(IESO_OFF & PLL_96MHZ_ON & PLLDIV_DIV2 & FNOSC_FRCPLL & POSCMOD_NONE) // Primary osc disabled, FRC OSC with PLL, USBPLL /2
        _CONFIG1(JTAGEN_OFF & ICS_PGx2 & FWDTEN_OFF)        // JTAG off, watchdog timer off
    #else
        // Using the 12MHz clock provided by the PIC18F67J50
        _CONFIG2(IESO_OFF & PLL_96MHZ_ON & PLLDIV_DIV3 & FNOSC_PRIPLL & POSCMOD_HS)   // Primary HS OSC with PLL, USBPLL /3
        _CONFIG1(JTAGEN_OFF & ICS_PGx2 & FWDTEN_OFF)        // JTAG off, watchdog timer off
    #endif
#else
    #error This code is designed for the PIC24FJ256GB1
#endif


//******************************************************************************
//******************************************************************************
// Constants
//******************************************************************************
//******************************************************************************

#define ID_MAIN_FLASH               ID_TOP_LEFT
#define ID_MAIN_DEMOS               ID_BOTTOM_LEFT
#define ID_MAIN_UTILITIES           ID_TOP_RIGHT
#define ID_MAIN_GAMES               ID_BOTTOM_RIGHT

#define MESSAGE_DEAD_TIME           (1000/MILLISECONDS_PER_TICK)


//******************************************************************************
//******************************************************************************
// Global Variables
//******************************************************************************
//******************************************************************************

DWORD               displayChangeTime;
extern IMAGE_FLASH  iconMicrochip;
extern IMAGE_FLASH  intro;
extern IMAGE_FLASH  introDevice;
PAGE_INFORMATION    pageInformation;
char                previousSecond;
DWORD               previousTick    = 0;
GOL_SCHEME          *pScheme;
SCREEN_STATES       screenState     = SCREEN_BOARD_TEST;
volatile DWORD      tick            = 0;


//*****************************************************************************
//******************************************************************************
// Function Prototypes
//******************************************************************************
//******************************************************************************

WORD GOLDrawCallback( void );
WORD GOLMsgCallback( WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
void PerformBoardTest( void );
WORD ProcessMessageMain( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
void ShowScreenMain( void );
void TickInit(void);
void TranslateTouchpad( GOL_MSG* pMsg );


//******************************************************************************
//******************************************************************************
// Main Application
//******************************************************************************
//******************************************************************************

int main( void )
{
    GOL_MSG msg;                                    // GOL message structure to interact with GOL

    RGBMapColorPins();

    TRISBbits.TRISB1 = 0; // Debug
    LATBbits.LATB1   = 0;

    #ifdef USE_FRC
        OSCCON = 0x1102;    // Enable secondary oscillator, use FRC oscillator
        CLKDIV = 0x0000;    // FRC post-scaler (1:1), USB postscaler (1:1), CPU postscaler (1:1)
    #else
        #ifdef GO_SLOW
            OSCCON = 0x3302;    // Enable secondary oscillator, use HS oscillator
            CLKDIV = 0x0080;    // (not needed - FRC post-scaler (1:1)), USB postscaler (4:1), CPU postscaler (1:1)
        #else
            OSCCON = 0x3302;    // Enable secondary oscillator, use HS oscillator
            CLKDIV = 0x0000;    // (not needed - FRC post-scaler (1:1)), USB postscaler (1:1), CPU postscaler (1:1)
        #endif
    #endif
    RCON = 0;

    // Initialize USB Embedded Host
    USBInitialize( 0 );

    // Initialize the RTCC
    RTCCInit();

    // Initialize the graphics library
    GOLInit();

    // Initialize the touch pads.
    CTMUInit();

    // Initialize the reference timer.
    TickInit();


    while (1)
    {
        if (GOLDraw())                          // Draw the screen
        {
            TouchSenseButtonsMsg( &msg );       // Get a raw touchpad message
            if ((msg.uiEvent != EVENT_INVALID) && ((tick - displayChangeTime) > MESSAGE_DEAD_TIME))
            {
                TranslateTouchpad( &msg );      // Translate the raw message
                GOLMsg( &msg );                 // Process the message
            }
        }
    }
}


/****************************************************************************
  Function:
    WORD GOLDrawCallback( void )

  Description:
    This callback is performed at the beginning of the GOLDraw() function.
    Processing that needs to be done on a regular basis and is not triggered
    by a message should be done here.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    All graphic control adding and removing must be done from this function,
    not from the message callback.
  ***************************************************************************/

WORD GOLDrawCallback( void )
{
	unsigned char           i;
    OBJ_HEADER              *pObject;
	extern unsigned int     trip[NUM_TOUCHPADS];

    switch (screenState)
    {
        case SCREEN_BOARD_TEST:
            PerformBoardTest();
            break;
            
        case SCREEN_DISPLAY_MAIN:
            ShowScreenMain();
            displayChangeTime   = tick;
            screenState         = SCREEN_MAIN;
            break;

        case SCREEN_MAIN:
            RTCCProcessEvents();

            if (previousSecond != _time_str[11])
            {
                previousSecond = _time_str[11];
                strcpy( pageInformation.instructions, _date_str );
                strcat( pageInformation.instructions, "  " );
                strcat( pageInformation.instructions, _time_str );

                pObject = GOLFindObject( ID_INSTRUCTIONS );
                StSetText( (STATICTEXT *)pObject, pageInformation.instructions );
                SetState( pObject, ST_DRAW );
            }
            break;

        case SCREEN_DISPLAY_FLASH:
            ShowScreenFlash();
            displayChangeTime   = tick;
            screenState         = SCREEN_FLASH;
            break;

        case SCREEN_FLASH:
            FlashMonitorMedia();
            break;

        case SCREEN_DISPLAY_FLASH_ERROR:
            ShowScreenFlashError();
            displayChangeTime   = tick;
            screenState         = SCREEN_FLASH_ERROR;
            break;

        case SCREEN_FLASH_ERROR:
            break;

        case SCREEN_DISPLAY_GAMES:
            ShowScreenGames();
            displayChangeTime   = tick;
            screenState         = SCREEN_GAMES;
            break;

        case SCREEN_GAMES:
            break;

        case SCREEN_DISPLAY_SNAKE:
            ShowScreenSnake();
            displayChangeTime   = tick;
            screenState         = SCREEN_SNAKE;
            break;

        case SCREEN_SNAKE:
            DrawSnake();
            break;

        case SCREEN_DISPLAY_SNAKE_SCORE:
            ShowScreenScore();
            displayChangeTime   = tick;
            screenState         = SCREEN_SNAKE_SCORE;
            break;

        case SCREEN_SNAKE_SCORE:
            break;

        case SCREEN_DISPLAY_BLASTER:
            ShowScreenBlaster();
            // Set the touchpad sensitivity for quicker game play.
			for (i=0; i<NUM_TOUCHPADS; i++) 
			{
    			trip[i]=TRIP_VALUE_BLASTER;
            } 			
            displayChangeTime   = tick;
            screenState         = SCREEN_BLASTER;
            break;

        case SCREEN_BLASTER:
            DrawBlaster();
            break;

        case SCREEN_DISPLAY_BLASTER_SCORE:
            ShowScreenScore();
            // Reset the touchpad sensitivity to normal.
			for(i=0; i<NUM_TOUCHPADS; i++) 
			{
    			trip[i]=TRIP_VALUE;
            } 			
            displayChangeTime   = tick;
            screenState         = SCREEN_BLASTER_SCORE;
            break;

        case SCREEN_BLASTER_SCORE:
            break;

        case SCREEN_DISPLAY_SHAPELET:
            ShowScreenShapelet();
            displayChangeTime   = tick;
            screenState         = SCREEN_SHAPELET;
            break;

        case SCREEN_SHAPELET:
            break;

        case SCREEN_DISPLAY_SHAPELET_SCORE:
            ShowScreenScore();
            displayChangeTime   = tick;
            screenState         = SCREEN_SHAPELET_SCORE;
            break;

        case SCREEN_SHAPELET_SCORE:
            break;

        case SCREEN_DISPLAY_DEMOS:
            ShowScreenDemos();
            displayChangeTime   = tick;
            screenState         = SCREEN_DEMOS;
            break;

        case SCREEN_DEMOS:
            break;

        case SCREEN_DISPLAY_RGB:
            ShowScreenRGB();
            displayChangeTime   = tick;
            screenState         = SCREEN_RGB;
            break;

        case SCREEN_RGB:
            break;

        case SCREEN_DISPLAY_GRAPH:
            ShowScreenGraph();
            displayChangeTime   = tick;
            screenState         = SCREEN_GRAPH;
            break;

        case SCREEN_GRAPH:
            UpdateGraph();
            break;

        case SCREEN_DISPLAY_CAPTURE_MEDIA:
            ShowScreenCaptureMedia();
            displayChangeTime   = tick;
            screenState         = SCREEN_CAPTURE_MEDIA;
            break;

        case SCREEN_CAPTURE_MEDIA:
            CaptureMonitorMedia();
            break;

        case SCREEN_DISPLAY_CAPTURE:
            ShowScreenCapture();
            displayChangeTime   = tick;
            screenState         = SCREEN_CAPTURE;
            break;

        case SCREEN_CAPTURE:
            UpdateGraph();
            CaptureMonitorMedia();
            break;

        case SCREEN_DISPLAY_UTILITIES:
            ShowScreenUtilities();
            displayChangeTime   = tick;
            screenState         = SCREEN_UTILITIES;
            break;

        case SCREEN_UTILITIES:
            break;

        case SCREEN_DISPLAY_TIME:
            ShowScreenTime();
            displayChangeTime   = tick;
            screenState         = SCREEN_TIME;
            break;

        case SCREEN_TIME:
            break;

        case SCREEN_DISPLAY_CTMU:
            ShowScreenCTMU();
            displayChangeTime   = tick;
            screenState         = SCREEN_CTMU;
            break;

        case SCREEN_CTMU:
            CalibrateCTMU();
            break;

		case SCREEN_DISPLAY_BAR_DEMO:
			ShowScreenBarDemo();
			displayChangeTime	= tick;
			screenState			= SCREEN_BAR_DEMO;
			break;

		case SCREEN_BAR_DEMO:
			DrawBarDemo();
			break;
    }

    return 1;   // Callback complete
}


/****************************************************************************
  Function:
    WORD GOLMsgCallback( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

  Description:
    This callback is executed when a graphics message is being processed.
    Based on the currently displayed screen, we determine the next operation.

  Precondition:
    None

  Parameters:
    WORD translatedMsg  - The message as translated by the processing
                            routine for the object type.
    OBJ_HEADER* pObj    - Pointer to the graphics object header.
    GOL_MSG* pMsg       - Original graphics message.

  Return Values:
    0 - Do not call the default message processing routine for the object.
    1 - Call the default message processing routine for the object.

  Remarks:
    This function is called before the default processing.  If default
    processing is required before other processing, it must be done
    explicitly in this function, and 0 must be returned.  If default
    processing can be performed after this function, simply return 1.
  ***************************************************************************/

WORD GOLMsgCallback( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    switch( screenState )
    {
        case SCREEN_BOARD_TEST:
            // No messages are processed in this state.
            break;
            
        case SCREEN_DISPLAY_MAIN:
            // This state exists for the draw callback only.
            break;

        case SCREEN_MAIN:
            return ProcessMessageMain( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_FLASH:
            // This state exists for the draw callback only.
            break;

        case SCREEN_FLASH:
            return ProcessMessageFlash( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_FLASH_ERROR:
            // This state exists for the draw callback only.
            break;

        case SCREEN_FLASH_ERROR:
            return ProcessMessageFlashError( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_GAMES:
            // This state exists for the draw callback only.
            break;

        case SCREEN_GAMES:
            return ProcessMessageGames( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_SNAKE:
            // This state exists for the draw callback only.
            break;

        case SCREEN_SNAKE:
            return ProcessMessageSnake( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_SNAKE_SCORE:
            // This state exists for the draw callback only.
            break;

        case SCREEN_SNAKE_SCORE:
            return ProcessMessageScore( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_BLASTER:
            // This state exists for the draw callback only.
            break;

        case SCREEN_BLASTER:
            return ProcessMessageBlaster( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_BLASTER_SCORE:
            // This state exists for the draw callback only.
            break;

        case SCREEN_BLASTER_SCORE:
            return ProcessMessageScore( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_SHAPELET:
            // This state exists for the draw callback only.
            break;

        case SCREEN_SHAPELET:
            return ProcessMessageShapelet( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_SHAPELET_SCORE:
            // This state exists for the draw callback only.
            break;

        case SCREEN_SHAPELET_SCORE:
            return ProcessMessageScore( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_DEMOS:
            // This state exists for the draw callback only.
            break;

        case SCREEN_DEMOS:
            return ProcessMessageDemos( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_RGB:
            // This state exists for the draw callback only.
            break;

        case SCREEN_RGB:
            return ProcessMessageRGB( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_GRAPH:
            // This state exists for the draw callback only.
            break;

        case SCREEN_GRAPH:
            return ProcessMessageGraph( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_CAPTURE_MEDIA:
            // This state exists for the draw callback only.
            break;

        case SCREEN_CAPTURE_MEDIA:
            return ProcessMessageCapture( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_CAPTURE:
            // This state exists for the draw callback only.
            break;

        case SCREEN_CAPTURE:
            return ProcessMessageCapture( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_UTILITIES:
            // This state exists for the draw callback only.
            break;

        case SCREEN_UTILITIES:
            return ProcessMessageUtilities( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_TIME:
            // This state exists for the draw callback only.
            break;

        case SCREEN_TIME:
            return ProcessMessageTime( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_CTMU:
            // This state exists for the draw callback only.
            break;

        case SCREEN_CTMU:
            return ProcessMessageCTMU( translatedMsg, pObj, pMsg );
            break;

		case SCREEN_DISPLAY_BAR_DEMO:
			break;

		case SCREEN_BAR_DEMO:
			//return ProcessMessageBarDemo( translatedMsg, pObj, pMsg );
			break;

        default:
            break;
    }

    return 1;
}


/****************************************************************************
  Function:
    void InitializeScreen( void )

  Description:
    This function erases the current list of graphics objects, clears the
    screen, puts the Microchip icon in the upper left corner of the screen,
    and displays a title at the top of the screen.  The title is obtained
    from the global pageInformation structure.

  Precondition:
    GOLInit() must be called prior to calling this function.

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced.  Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void InitializeScreen( void )
{
    GOLFree();
    SetColor(BLACK);        // set color to BLACK
    ClearDevice();          // set screen to all BLACK

    PictCreate(     ID_ICON,                    // ID
                    0,0,15,15,                  // dimension
                    PICT_DRAW,                  // will be dislayed, has frame
                    1,                          // scale factor is x1
                    &iconMicrochip,             // bitmap
                    NULL );                     // default GOL scheme

    StCreate(       ID_TITLE,
                    16, 0, GetMaxX()-11, GetTextHeight( (void *)&FONTDEFAULT )-1,
                    ST_DRAW,
                    pageInformation.title,
                    NULL );

    previousTick = tick; 
}


/****************************************************************************
  Function:
    void PerformBoardTest( void )

  Description:
    This function conducts the board test when the board is powered on.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void PerformBoardTest( void )
{
    static int  elapsedSeconds = 0;
    static void *picture;
    
    
    RTCCProcessEvents();

    if (previousSecond != _time_str[11])
    {
        switch ( elapsedSeconds )
        {
            case 0:
                // We will test to see if we are plugged into the PC as a
                // device (on the right side of the board) or if the debugger
                // side is plugged in (on the left side of the board).  If
                // we are plugged in as a device, we will receive SOF tokens,
                // which sets the SOF interrupt.
                picture = &intro;
             
                // Turn on the USB module in device mode.  First, disable all
                // USB interrupts, since we are just doing a local test.  Then
                // enable the module in default device mode and turn on the 
                // power.  We will let the module take care of pulling up the
                // correct lines.  Note that the interrupt flags in U1IR are
                // cleared by writing a "1" to each flag.
                U1IE                    = 0;
                U1IR                    = 0xFF;
                U1CONbits.USBEN         = 1;
                U1PWRCbits.USBPWR       = 1;
            
                // Fall through
                
            case 2:
                GOLFree();
                SetColor( WHITE );
                ClearDevice();          
                break;
            
            case 1:
            case 3:
                GOLFree();
                SetColor( BLACK );
                ClearDevice();          
                break;

            case 4:
                // See if we are plugged into the PC as a device.  If so, we
                // will display a slightly different picture.
                if (U1IRbits.SOFIF)
                {
                    // We are receiving SOF tokens, so we must be plugged 
                    // into a host.
                    picture = &introDevice;
                }

                // Disable and power off the USB module.
                U1CONbits.USBEN         = 1;
                U1PWRCbits.USBPWR       = 0;
                    
                // Display the selected graphic.
                PictCreate(     ID_ICON,                    // ID
                                0,0,GetMaxX(),GetMaxY(),    // dimension
                                PICT_DRAW,                  // will be dislayed, has frame
                                1,                          // scale factor is x1
                                picture,                    // bitmap
                                NULL );                     // default GOL scheme
                break;
                
            case 5:
                RGBTurnOnLED( RGB_SATURATION_MINIMUM, RGB_SATURATION_MINIMUM, RGB_SATURATION_MINIMUM );
                break;

            case 6: 
                // Red
                RGBSetRed( RGB_SATURATION_MAXIMUM );
                break;
                
            case 7:
                // Yellow
                RGBSetGreen( RGB_SATURATION_MAXIMUM );
                break;
                
            case 8:
                // Green
                RGBSetRed( RGB_SATURATION_MINIMUM );
                break;
                
            case 9:
                // Aqua
                RGBSetBlue( RGB_SATURATION_MAXIMUM );
                break;
                
            case 10:
                // Blue
                RGBSetGreen( RGB_SATURATION_MINIMUM );
                break;
                
            case 11:
                // Fuschia            
                RGBSetRed( RGB_SATURATION_MAXIMUM );
                break;
                
            case 12:
                // White
                RGBSetGreen( RGB_SATURATION_MAXIMUM );
                break;
                
            case 13:
                // Pause
                break;
                
            case 14:
                RGBTurnOffLED();
                screenState    = SCREEN_DISPLAY_MAIN;    
                previousSecond = ' ';
                elapsedSeconds = 0;
                return;
                break;
                            
        }
                   
        previousSecond = _time_str[11];
        elapsedSeconds ++;
    }
}

    
/****************************************************************************
  Function:
    WORD ProcessMessageMain( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

  Description:
    This function processes the messages for the main introduction screen.
    It allows the user to select a demo, utility or game.

  Precondition:
    Call ShowScreenMain() prior to using this function to display the correct
    screen.

  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    The global screenState is updated to proceed to the user's selection.
  ***************************************************************************/

WORD ProcessMessageMain( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    WORD    controlID;

//    BtnMsgDefault( translatedMsg, (BUTTON*)pObj, pMsg );
    if (translatedMsg == BTN_MSG_PRESSED)
    {
        controlID = GetObjID(pObj);

        switch (controlID)
        {
            case ID_MAIN_FLASH:
                screenState = SCREEN_DISPLAY_FLASH;
                break;

            case ID_MAIN_GAMES:
                screenState = SCREEN_DISPLAY_GAMES;
                break;

            case ID_MAIN_DEMOS:
                screenState = SCREEN_DISPLAY_DEMOS;
                break;

            case ID_MAIN_UTILITIES:
                screenState = SCREEN_DISPLAY_UTILITIES;
                break;
        }
    }
    return 0;   // We already called the default handler.
}


/****************************************************************************
  Function:
    void ShowScreenGeneric( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    a screen of a generic format.  The screen consists of:
            * the Microchip icon
            * a one line title
            * four buttons
            * one line of instructions

    The information for the screen contents is obtained from the global
    pageInformation structure.

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

void ShowScreenGeneric( void )
{
    InitializeScreen();

    BtnCreate(      ID_TOP_LEFT,                // ID
                    0,LOCATION_MIN_Y,62,30,2,   // dimension and radius
                    BTN_DRAW|BTN_PRESSED,       // will be displayed after creation
                    pageInformation.buttonBitmapTopLeft, // bitmap
                    pageInformation.buttonTextTopLeft,   // text
                    NULL );                     // Default scheme

    BtnCreate(      ID_BOTTOM_LEFT,             // ID
                    0,32,62,46,2,               // dimension and radius
                    BTN_DRAW,                   // will be displayed after creation
                    pageInformation.buttonBitmapBottomLeft, // bitmap
                    pageInformation.buttonTextBottomLeft,   // text
                    NULL );                     // Default scheme

    BtnCreate(      ID_TOP_RIGHT,               // ID
                    64,LOCATION_MIN_Y,125,30,2, // dimension and radius
                    BTN_DRAW,                   // will be displayed after creation
                    pageInformation.buttonBitmapTopRight, // bitmap
                    pageInformation.buttonTextTopRight,   // text
                    NULL );                     // Default scheme

    BtnCreate(      ID_BOTTOM_RIGHT,            // ID
                    64,32,125,46,2,             // dimension and radius
                    BTN_DRAW,                   // will be displayed after creation
                    pageInformation.buttonBitmapBottomRight,
                    pageInformation.buttonTextBottomRight,
                    NULL );                     // Default scheme

	
	if(!strcmp(pageInformation.buttonTextThirdrow,"Cap Touch"))
	BtnCreate(		ID_THIRDROW,
					0,48,62,62,2,
					BTN_DRAW,
					pageInformation.buttonBitmapThirdrow,
					pageInformation.buttonTextThirdrow,
					NULL );	else 

    StCreate(       ID_INSTRUCTIONS,
                    0, 47, 127, 47+GetTextHeight( (void *)&FONTDEFAULT )-1,
                    ST_DRAW,
                    pageInformation.instructions,
                    NULL );
}


/****************************************************************************
  Function:
    void ShowScreenMain( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the main menu screen.

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

void ShowScreenMain( void )
{
    RTCCProcessEvents();

    strcpy( pageInformation.title, "PIC24F Starter Kit" );

    strcpy( pageInformation.buttonTextTopLeft, "Flash Drive" );
    strcpy( pageInformation.buttonTextBottomLeft, "Demos" );
    strcpy( pageInformation.buttonTextTopRight, "Utilities" );
    strcpy( pageInformation.buttonTextBottomRight, "Games" );
	strcpy( pageInformation.buttonTextThirdrow, 	"" );

    pageInformation.buttonBitmapTopLeft        = NULL;
    pageInformation.buttonBitmapBottomLeft     = NULL;
    pageInformation.buttonBitmapTopRight       = NULL;
    pageInformation.buttonBitmapBottomRight    = NULL;

    strcpy( pageInformation.instructions, _date_str );
    strcat( pageInformation.instructions, "  " );
    strcat( pageInformation.instructions, _time_str );

    ShowScreenGeneric( );
}


/****************************************************************************
  Function:
    void TickInit( void )

  Description:
    This function sets up Timer 4 to generate an interrupt every 10 ms.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    The timer period and prescaler values are defined in HardwareProfile.h,
    since they are dependent on the oscillator speed.
  ***************************************************************************/

#define STOP_TIMER_IN_IDLE_MODE     0x2000
#define TIMER_SOURCE_INTERNAL       0x0000
#define TIMER_ON                    0x8000
#define GATED_TIME_DISABLED         0x0000
#define TIMER_16BIT_MODE            0x0000
#define TIMER_PRESCALER_1           0x0000
#define TIMER_PRESCALER_8           0x0010
#define TIMER_PRESCALER_64          0x0020
#define TIMER_PRESCALER_256         0x0030
#define TIMER_INTERRUPT_PRIORITY    0x1000

void TickInit( void )
{
    TMR4 = 0;
    PR4 = TIMER_PERIOD;
    T4CON = TIMER_ON | STOP_TIMER_IN_IDLE_MODE | TIMER_SOURCE_INTERNAL |
            GATED_TIME_DISABLED | TIMER_16BIT_MODE | TIMER_PRESCALER;

    IFS1bits.T4IF = 0;              //Clear flag
    IEC1bits.T4IE = 1;              //Enable interrupt
    T4CONbits.TON = 1;              //Run timer
}


/****************************************************************************
  Function:
    void TranslateMessageGeneric( GOL_MSG* msg )

  Description:
    This function translates the keypad message into the appropriate message
    for a generic, 4-button menu screen.  It can be used as the message
    processing function for any generic screen.

  Precondition:
    None

  Parameters:
    GOL_MSG * msg    - The current message information

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void TranslateMessageGeneric( GOL_MSG * pMsg )
{

    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_TOUCH_PAD))
    {
        switch (pMsg->param2)
        {
            case SCAN_UP_PRESSED:
                if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_PRESSED ))
                {
                    ClrState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ),     BTN_PRESSED | BTN_DRAW );
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED ))
                {
                    ClrState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ),    BTN_PRESSED | BTN_DRAW );
                }
				else if (GetState( (BUTTON *)GOLFindObject( ID_THIRDROW ), BTN_PRESSED ) && !strcmp(pageInformation.title,"Demonstrations"))
				{
					ClrState( (BUTTON *)GOLFindObject( ID_THIRDROW ),    BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_THIRDROW ),    BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ), BTN_PRESSED | BTN_DRAW );
				}
                break;

            case SCAN_DOWN_PRESSED:
                if (GetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ), BTN_PRESSED ))
                {
                    ClrState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ),     BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ),     BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_PRESSED | BTN_DRAW );
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ), BTN_PRESSED ))
                {
                    ClrState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ),    BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ),    BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED | BTN_DRAW );
                }
				else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ), BTN_PRESSED ) && !strcmp(pageInformation.title,"Demonstrations"))
				{
					ClrState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),    BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),    BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_THIRDROW ), BTN_PRESSED | BTN_DRAW );
				}
                break;

            case SCAN_RIGHT_PRESSED:
                if (GetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ), BTN_PRESSED ))
                {
                    ClrState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ),     BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ),     BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ),    BTN_PRESSED | BTN_DRAW );
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ), BTN_PRESSED ))
                {
                    ClrState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED | BTN_DRAW );
                }
                break;

            case SCAN_LEFT_PRESSED:
                if (GetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ), BTN_PRESSED ))
                {
                    ClrState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ),    BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ),    BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ),     BTN_PRESSED | BTN_DRAW );
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED ))
                {
                    ClrState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_PRESSED |  BTN_DRAW );
                }
                break;

            case SCAN_UP_RELEASED:
                break;

            case SCAN_DOWN_RELEASED:
                break;

            case SCAN_RIGHT_RELEASED:
                break;

            case SCAN_LEFT_RELEASED:
                break;

            case SCAN_CR_PRESSED:
                if (GetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_TOP_LEFT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_BOTTOM_LEFT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_TOP_RIGHT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_BOTTOM_RIGHT;
                }
				else if (GetState( (BUTTON *)GOLFindObject( ID_THIRDROW ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_THIRDROW;
                }
                break;

            case SCAN_CR_RELEASED:
                if (GetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_TOP_LEFT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_BOTTOM_LEFT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_TOP_RIGHT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_BOTTOM_RIGHT;
                }
				else if (GetState( (BUTTON *)GOLFindObject( ID_THIRDROW ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_THIRDROW;
                }
                break;

        }
    }
}


/****************************************************************************
  Function:
    void TranslateTouchpad( GOL_MSG* msg )

  Description:
    This function translates the keypad message into the appropriate message
    for the screen, based on what screen is currently displayed.

  Precondition:
    None

  Parameters:
    GOL_MSG* msg    - The current message information

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void TranslateTouchpad( GOL_MSG* pMsg )
{
    switch( screenState )
    {
        case SCREEN_BOARD_TEST:
            // Ignore all touchpad messages.
            pMsg->uiEvent = EVENT_INVALID;
            break;
        case SCREEN_DISPLAY_MAIN:
            // No translation needed.
            break;
        case SCREEN_MAIN:
            TranslateMessageGeneric( pMsg );
            break;
        case SCREEN_DISPLAY_FLASH:
            // No translation needed.
            break;
        case SCREEN_FLASH:
            TranslateMessageFlash( pMsg );
            break;
        case SCREEN_DISPLAY_FLASH_ERROR:
            // No translation needed.
            break;
        case SCREEN_FLASH_ERROR:
            TranslateMessageFlashError( pMsg );
            break;
        case SCREEN_DISPLAY_GAMES:
            // No translation needed.
            break;
        case SCREEN_GAMES:
            TranslateMessageGeneric( pMsg );
            break;
        case SCREEN_DISPLAY_SNAKE:
            // No translation needed.
            break;
        case SCREEN_SNAKE:
            TranslateMessageSnake( pMsg );
            break;
        case SCREEN_DISPLAY_SNAKE_SCORE:
            // No translation needed.
            break;
        case SCREEN_SNAKE_SCORE:
            TranslateMessageScore( pMsg );
            break;
        case SCREEN_DISPLAY_BLASTER:
            // No translation needed.
            break;
        case SCREEN_BLASTER:
            TranslateMessageBlaster( pMsg );
            break;
        case SCREEN_DISPLAY_BLASTER_SCORE:
            // No translation needed.
            break;
        case SCREEN_BLASTER_SCORE:
            TranslateMessageScore( pMsg );
            break;
        case SCREEN_DISPLAY_SHAPELET:
            // No translation needed.
            break;
        case SCREEN_SHAPELET:
            TranslateMessageShapelet( pMsg );
            break;
        case SCREEN_DISPLAY_SHAPELET_SCORE:
            // No translation needed.
            break;
        case SCREEN_SHAPELET_SCORE:
            TranslateMessageScore( pMsg );
            break;
        case SCREEN_DISPLAY_DEMOS:
            // No translation needed.
            break;
        case SCREEN_DEMOS:
            TranslateMessageGeneric( pMsg );
            break;
        case SCREEN_DISPLAY_RGB:
            // No translation needed.
            break;
        case SCREEN_RGB:
            TranslateMessageRGB( pMsg );
            break;
        case SCREEN_DISPLAY_GRAPH:
            // No translation needed.
            break;
        case SCREEN_GRAPH:
            TranslateMessageGraph( pMsg );
            break;
        case SCREEN_DISPLAY_CAPTURE_MEDIA:
            // No translation needed.
            break;
        case SCREEN_CAPTURE_MEDIA:
            TranslateMessageCapture( pMsg );
            break;
        case SCREEN_DISPLAY_CAPTURE:
            // No translation needed.
            break;
        case SCREEN_CAPTURE:
            TranslateMessageCapture( pMsg );
            break;
        case SCREEN_DISPLAY_UTILITIES:
            // No translation needed.
            break;
        case SCREEN_UTILITIES:
            TranslateMessageGeneric( pMsg );
            break;
        case SCREEN_DISPLAY_TIME:
            // No translation needed.
            break;
        case SCREEN_TIME:
            TranslateMessageTime( pMsg );
            break;
        case SCREEN_DISPLAY_CTMU:
            // No translation needed.
            break;
        case SCREEN_CTMU:
            TranslateMessageCTMU( pMsg );
            break;
		case SCREEN_DISPLAY_BAR_DEMO:
			break;
		case SCREEN_BAR_DEMO:
			//TranslateMessageBarDemo( pMsg );
			break;
    }
}

/****************************************************************************
  Function:
    void __attribute__((interrupt, shadow, auto_psv)) _T4Interrupt(void)

  Description:
    This function updates the tick count and calls ReadCTMU() to monitor the
    touchpads.

  Precondition:
    Timer 4 and the Timer 4 interrupt must be enabled in order for
                    this function to execute.  CTMUInit() must be called before
                Timer 4 and the Timer 4 interrupt are enabled.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
void __attribute__((interrupt, shadow, auto_psv)) _T4Interrupt(void)
{
    // Clear flag
    IFS1bits.T4IF = 0;
    tick++;

    ReadCTMU();
}


/*******************************************************************************
Function:       void __attribute__((__interrupt__, auto_psv)) _XXX(void)

These are just here to catch any spurious interrupts that we see during
debugging.

*******************************************************************************/
void __attribute__((interrupt, auto_psv)) _DefaultInterrupt(void)
{
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}

void __attribute__((interrupt, auto_psv)) _OscillatorFail(void)
{
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
void __attribute__((interrupt, auto_psv)) _AddressError(void)
{
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
void __attribute__((interrupt, auto_psv)) _StackError(void)
{
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
void __attribute__((interrupt, auto_psv)) _MathError(void)
{
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}

