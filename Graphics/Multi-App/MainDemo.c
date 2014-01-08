/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        MainDemo.c
 * Dependencies:    MainDemo.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software LicenseJpeg
  Agreement
 *
 * Copyright (c) 2007 Microchip Technology Inc.  All rights reserved.
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
 * 03/10/08    ...
 *****************************************************************************/
#include "MainDemo.h"
#include "MainDemoStrings.h"


// Check device if allowed
// This check is used to check the project of the demo used. 
#ifdef CHECK_PIC24F_DEVICE_ON_GMAP 
	#if !defined (__PIC24FJ128GA010__) && !defined (__PIC24FJ256GA110__)
		#error This project of Graphics Multi Application (GMAP) Demo can only run on PIC24FJ128GA010 device. Please run the other GMAP projects for other devices
	#endif	
#endif //CHECK_PIC24F_DEVICE_ON_GMAP 


// Configuration bits
#ifdef __PIC32MX__
   #if defined(_USB) 
        #pragma config UPLLEN = ON          // USB PLL Enabled
        #pragma config FPLLMUL = MUL_20     // PLL Multiplier
        #pragma config UPLLIDIV = DIV_2     // USB PLL Input Divider
        #pragma config FPLLIDIV = DIV_2     // PLL Input Divider
        #pragma config FPLLODIV = DIV_1     // PLL Output Divider
        #pragma config FPBDIV = DIV_2       // Peripheral Clock divisor
        #pragma config FWDTEN = OFF         // Watchdog Timer
        #pragma config WDTPS = PS1          // Watchdog Timer Postscale
        #pragma config FCKSM = CSDCMD       // Clock Switching & Fail Safe Clock Monitor
        #pragma config OSCIOFNC = OFF       // CLKO Enable
        #pragma config POSCMOD = HS         // Primary Oscillator
        #pragma config IESO = OFF           // Internal/External Switch-over
        #pragma config FSOSCEN = ON         // Secondary Oscillator Enable (KLO was off)
        #pragma config FNOSC = PRIPLL       // Oscillator Selection
        #pragma config CP = OFF             // Code Protect
        #pragma config BWP = OFF            // Boot Flash Write Protect
        #pragma config PWP = OFF            // Program Flash Write Protect
        #pragma config ICESEL = ICS_PGx2    // ICE/ICD Comm Channel Select
    #else
        #pragma config FPLLODIV = DIV_2, FPLLMUL = MUL_20, FPLLIDIV = DIV_1, FWDTEN = OFF, FCKSM = CSECME, FPBDIV = DIV_2
        #pragma config OSCIOFNC = ON, POSCMOD = XT, FSOSCEN = ON, FNOSC = PRIPLL
        #pragma config CP = OFF, BWP = OFF, PWP = OFF
    #endif
#else
    #if defined(__PIC24FJ256GB110__)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2)
_CONFIG2(0xF7FF & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_XT & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_OFF)
    #endif
    #if defined(__PIC24FJ128GA010__)
_CONFIG2(FNOSC_PRIPLL & POSCMOD_XT)         // Primary XT OSC with PLL
_CONFIG1(JTAGEN_OFF & FWDTEN_OFF)           // JTAG off, watchdog timer off
    #endif
    #if defined(__PIC24FJ256GA110__)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2)
_CONFIG2(IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_XT & FNOSC_PRIPLL & IOL1WAY_OFF)
    #endif
#if defined (__PIC24FJ256GB210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
	#endif
	#if defined (__PIC24FJ256DA210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_EC & WUTSEL_LEG & ALTPMP_ALTPMPEN & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
	#endif	 
	#if defined(__dsPIC33E__) || defined(__PIC24E__)
	_FOSCSEL(FNOSC_FRC);	
	_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
	_FWDT(FWDTEN_OFF);
	_FPOR(FPWRT_PWR128 & BOREN_ON & ALTI2C1_ON & ALTI2C2_ON);
	_FICD(ICS_PGD1 & RSTPRI_PF & JTAGEN_OFF);	    
	#endif          
#endif

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
#define WAIT_UNTIL_FINISH(x)    while(!x)

/************************************************************************
 Macros: SST39PMPInit()
                                                                       
 Overview: Initializes PMP in preparation of access to the external flash.
 		   PMP access timing must be set up to meet the flash device specs.		
                                                                      
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
#define SST39PMPInit()          \
    {                           \
        while(PMMODEbits.BUSY); \
        PMMODE = 0x0a49;        \
        PMAEN = 0x0003;         \
        PMCON = 0x9320;         \
    }

/************************************************************************
 Macros: LCDPMPInit()
                                                                       
 Overview: Initializes PMP in preparation for access to the display controller.
 		   PMP access timing must be set up to meet the display controller specs.		
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
#ifdef GFX_USE_DISPLAY_CONTROLLER_LGDP4531
    #define LCDPMPInit()        \
    {                           \
        while(PMMODEbits.BUSY); \
        PMMODE = 0x0204;        \
        PMAEN = 0x0000;         \
        PMCON = 0x8300;         \
    }                               // this is used for LGDP4531_R61505_S6D0129_S6D0139_SPFD5408
#endif

/************************************************************************
 Function: StartScreen()
                                                                       
 Overview: Draws the splash screen before the main menu starts.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
    void StartScreen(void);

/************************************************************************
 Function: Init_CPUClocks()
                                                                       
 Overview: Initializes the CPU clocks and initialize registers.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
    void Init_CPUClocks(void);

/************************************************************************
 Function: TickInit()
                                                                       
 Overview: Initializes the tick counter used for the demo.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
    void TickInit(void);

/************************************************************************
 Function: InitializeBoard()
                                                                       
 Overview: Initializes the board components of the demo.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
    void InitializeBoard(void);

/************************************************************************
 Function: CheckExternalFlashHex()
                                                                       
 Overview: Checks the external memory if data is valid.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
#if defined (EXPLORER_16) || defined (PIC24FJ256DA210_DEV_BOARD)
    void CheckExternalFlashHex();
#endif

/////////////////////////////////////////////////////////////////////////////
// SPI Channel settings
/////////////////////////////////////////////////////////////////////////////
#if defined (SPI_CHANNEL_1_ENABLE) || defined (SPI_CHANNEL_2_ENABLE) || defined (SPI_CHANNEL_3_ENABLE) || defined (SPI_CHANNEL_4_ENABLE)
    #if defined (USE_SST25VF016)
        #ifdef __PIC32MX
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 1, 0, 0, 1, 1, 0};
        #else    
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
        #endif
    #elif defined (USE_MCHP25LC256)       
        const DRV_SPI_INIT_DATA SPI_Init_Data = {MCHP25LC256_SPI_CHANNEL, 6, 3, 0, 1, 1, 0};
    #endif    
#endif

/////////////////////////////////////////////////////////////////////////////
//                            IMAGES USED
/////////////////////////////////////////////////////////////////////////////
// intro and mchpLogo are declared in ResourcesExternalSPIFlash.h, ResourcesInternalFlash C32.h
// or ResourcesInternalFlash C32 SK.h depending on the hardware used.

/////////////////////////////////////////////////////////////////////////////
//                       GLOBAL VARIABLES FOR DEMO
/////////////////////////////////////////////////////////////////////////////
SCREEN_STATES           screenState = CREATE_DEMOSELECTION; // current state of main demo state mashine
GOL_SCHEME              *altScheme;                         // alternative style scheme
GOL_SCHEME              *iconScheme;                        // alternative icon style scheme
GOL_SCHEME              *meterScheme;                       // style scheme used to draw the meter
GOL_SCHEME              *navScheme;                         // style scheme for the navigation
GOL_SCHEME              *gridScheme;                        // style scheme used to draw the grid
GOL_SCHEME              *graphScheme;                       // style scheme used for the graph demo
OBJ_HEADER              *pNavList;                          // pointer to navigation list
BYTE                    usbErrorCode;                       // USB error
BYTE                    usbHIDStatus;                       // HID device status
BYTE                    usbMSDStatus;                       // MSD device status
volatile DWORD          tick = 0;                           // tick counter
extern BOOL             gEnableDemoFlag;                    // flag for automatic demo mode

/////////////////////////////////////////////////////////////////////////////
//                            STRINGS USED
/////////////////////////////////////////////////////////////////////////////
const XCHAR             ErrMsgStandard[] = {'U','S','B',' ','E','r','r','o','r',0};
const XCHAR             ErrNotSupported[] = {'n','o','t',' ','s','u','p','p','o','r','t','e','d','!',0};
const XCHAR             ErrMsgFailedStr[] = {'F','a','i','l','e','d',0};
const XCHAR             ErrMsgHUBAttachedStr[] = {'H','U','B',0};
const XCHAR             ErrMsgUDAttachedStr[] = {'D','e','v','i','c','e',0};
const XCHAR             ErrMsgEnumerationStr[] = {'E','n','u','m','e','r','a','t','i','o','n',0};
const XCHAR             ErrMsgClientInitStr[] = {'C','l','i','e','n','t',' ','I','n','i','t','i','a','l','i','z','a','t','i','o','n',0};
const XCHAR             ErrMsgOutofMemoryStr[] = {'O','u','t',' ','o','f',' ','M','e','m','o','r','y',0};
const XCHAR             ErrMsgUnpecifiedErrStr[] = {'U','n','s','p','e','c','i','f','i','e','d',0};
const XCHAR             MsgTouchToProceedStr[] = {'T','o','u','c','h',' ','t','o',' ','p','r','o','c','e','e','d',0};

/////////////////////////////////////////////////////////////////////////////
//                                  MAIN
/////////////////////////////////////////////////////////////////////////////
int main(void)
{
    GOL_MSG msg;                        // GOL message structure to interact with GOL
   
   	InitializeBoard();
  
    RTCCInit();                         // Setup the RTCC
    RTCCProcessEvents();

    #if defined(ENABLE_SD_MSD_DEMO)

    // after calibration is done permanently disable the chip select of
    // the Explorer 16 EEPROM
        #if defined(__PIC24FJ256DA210__)
        #elif defined(__PIC24FJ256GB110__)  || defined(__PIC24FJ256GB210__)

    // This PIM has RD12 rerouted to RA15
    LATGbits.LATG0 = 1;
    TRISGbits.TRISG0 = 0;
        #else
    LATDbits.LATD12 = 1;
    TRISDbits.TRISD12 = 0;
        #endif
    #endif
    #if defined(ENABLE_USB_MSD_DEMO)
        USBInitialize(0);
    #endif
    #ifdef ENABLE_SD_MSD_DEMO

    int temp;
        #if defined(__PIC24FJ256GB110__) || defined(__PIC24FJ256GA110__) || defined(__PIC24FJ256DA210__) || defined(__PIC24FJ256GB210__)
    Configure_SDSPI_PPS();
        #endif
    MDD_SDSPI_InitIO();
    if(MDD_MediaDetect())
    {

        // check if SD Device is connected, but will not wait forever.
        temp = 500;
        while(temp)
        {
            if(FSInit())
                break;
            temp--;
        }
    }

    #endif
    #ifdef ENABLE_DEMO_MODE
    InitDemoMode();
    #endif

    StartScreen();                      // Show intro screen and wait for touch

    // create the the style schemes
    navScheme = GOLCreateScheme();
    altScheme = GOLCreateScheme();
    meterScheme = GOLCreateScheme();
    iconScheme = GOLCreateScheme();
    gridScheme = GOLCreateScheme();
    graphScheme = GOLCreateScheme();

    InitECGStyleScheme(altScheme);
    InitGamesStyleScheme(gridScheme);
    InitDemoSelectStyleScheme(iconScheme);
    InitAnimationStyleScheme(meterScheme);
    InitGraphDemoStyleScheme(graphScheme);
    
    while(1)
    {
        #if defined(ENABLE_USB_MSD_DEMO)

        // call background task to update USB states (check if device was attached or not
        USBTasks();
        #endif
        if(GOLDraw())
        {                               // Draw GOL objects
            // Drawing is finished, we can now process new message
            TouchGetMsg(&msg);          // Get message from touch screen
            #ifdef ENABLE_DEMO_MODE

            // when running in demo mode enabled fake the messages to perform
            // demo on selected screens
            if(gEnableDemoFlag == TRUE)
            {
                UpdateDemoMode(&msg);   // update the next step of the automatic demo
            }

            #endif
            GOLMsg(&msg);               // Process message

            // When USE_FOCUS is set, the Demo can be controlled using
            // Explorer 16 buttons. See SideButtons.h for details.
            #ifdef USE_FOCUS
            SideButtonsMsg(&msg);
            GOLMsg(&msg);               // Process message
            #endif

            // FOR DEBUG ONLY (holding down S6 of explorer 16 will pause the screen)
            //while(PORTDbits.RD7 == 0);
        }
    }
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
    if(objMsg == BTN_MSG_PRESSED)
    {
        Beep();
    }

    // Process messages for demo screens. Each of the
    // demo screens will have its own corresponding message callback
    // functions.
    switch(screenState)
    {
        case DISPLAY_TIMEDATE:
            return (TimeDateDemoMsgCallback(objMsg, pObj, pMsg));

        case DISPLAY_MULTIFONT:
            return (MultiFontDemoMsgCallback(objMsg, pObj, pMsg));

        case DISPLAY_RGB:
            return (RGBDemoMsgCallback(objMsg, pObj, pMsg));

        case DISPLAY_SNAKE:
            return (ProcessMessageSnake(objMsg, pObj, pMsg));

        case DISPLAY_SNAKE_SCORE:
            return (ProcessMessageSnake(objMsg, pObj, pMsg));
            return (1);

        case DISPLAY_SCALE:
            return (MsgDrawTool(objMsg, pObj, pMsg));

        case DISPLAY_ANIMATION:
            return (MsgAnimation(objMsg, pObj, pMsg));

        case DISPLAY_DEMOSELECTION:
            return (MsgDemoSelection(objMsg, pObj, pMsg));

        case DISPLAY_ECG:
            return (MsgECG(objMsg, pObj, pMsg));

        case DISPLAY_SLIDESHOW:
            return (SlideShowDemoMsgCallback(objMsg, pObj, pMsg));
            #ifdef ENABLE_USB_MSD_DEMO

        case DISPLAY_JPEGDEMO:
            return (JPEGMsgCallback(objMsg, pObj, pMsg));   // see JPEGDemo.c for this implementation
            #endif
            #ifdef ENABLE_SD_MSD_DEMO

        case DISPLAY_SDCARDDEMO:
            return (JPEGMsgCallback(objMsg, pObj, pMsg));   // see SDCardDemo.c for this implementation
            #endif

        case DISPLAY_GRAPHSHOW:
            return (GraphMsgCallback(objMsg, pObj, pMsg));

        default:
            return (1); // process message by default
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
    static DWORD    prevTick = 0;                   // keeps previous value of tick

    switch(screenState)
    {
        case CREATE_DEMOSELECTION:
            CreateDemoSelection();                  // create demo selection screen
            screenState = DISPLAY_DEMOSELECTION;    // switch to next state
            return (1);                             // draw objects created

        case DISPLAY_DEMOSELECTION:
            CheckDemoStatus();
            return (1);                             // redraw objects if needed

        case CREATE_TIMEDATE:
            CreateTimeDateDemo();
            screenState = DISPLAY_TIMEDATE;         // switch to next state
            return (1);

        case DISPLAY_TIMEDATE:
            TimeDateDemoDrawCallback();
            return (1);

        case CREATE_MULTIFONT:
            CreateMultiFontsDemo();
            screenState = DISPLAY_MULTIFONT;        // switch to next state
            return (1);

        case DISPLAY_MULTIFONT:
            MultiFontDemoDrawCallback(tick);
            return (1);

        case CREATE_RGB:
            CreateRGBDemo();                        // create RGB demo
            screenState = DISPLAY_RGB;              // switch to next state
            return (1);

        case DISPLAY_RGB:
            RGBDemoDrawCallback();                  // RGB demo draw callback
            return (1);

        case CREATE_SLIDESHOW:
            CreateSlideShowDemo();                  // create slide show demo
            screenState = DISPLAY_SLIDESHOW;        // switch to next state
            return (1);

            #ifdef ENABLE_USB_MSD_DEMO

        case CREATE_JPEGDEMO:
            if(CreateJPEGDemo() == 1)               // create JPEG demo (see JPEGDemo.c)
                screenState = DISPLAY_JPEGDEMO;     // switch to next state
            else
                screenState = CREATE_DEMOSELECTION; // go back to demo selection
            return (1);

        case DISPLAY_JPEGDEMO:
            JPEGDrawCallback();                     // see JPEGDemo.c
            return (1);
            #endif
            #ifdef ENABLE_SD_MSD_DEMO

        case CREATE_SDCARDDEMO:
            if(CreateJPEGDemo() == 1)               // create JPEG demo (see SDCardDemo.c)
                screenState = DISPLAY_SDCARDDEMO;   // switch to next state
            else
                screenState = CREATE_DEMOSELECTION; // go back to demo selection
            return (1);

        case DISPLAY_SDCARDDEMO:
            JPEGDrawCallback();                     // see SDCardDemo.c
            return (1);
            #endif

        case CREATE_ANIMATION:
            CreateAnimation();
            screenState = DISPLAY_ANIMATION;        // switch to next state
            return (1);

        case DISPLAY_ANIMATION:
            NextAnimation(tick);
            return (1);

        case CREATE_SCALE:
            CreateDrawTool();
            screenState = DISPLAY_SCALE;            // switch to next state
            return (1);

        case DISPLAY_SCALE:
            return (1);

        case CREATE_SNAKE:
            ShowScreenSnake();
            screenState = DISPLAY_SNAKE;            // switch to next state
            return (1);

        case DISPLAY_SNAKE:
            DrawSnake(tick);
            return (1);

        case CREATE_SNAKE_SCORE:
            ShowScreenScore();
            prevTick = tick;
            while((tick - prevTick) < 1000);

            // delay the switching of state so that score can be read
            screenState = DISPLAY_SNAKE_SCORE;      // switch to next state
            return (1);

        case DISPLAY_SNAKE_SCORE:
            return (1);

        case CREATE_ECG:
            CreateECG();                            // create window
            screenState = BOX_DRAW_ECG;             // switch to next state
            return (1);                             // draw objects created

        case BOX_DRAW_ECG:
            if(0 == PanelECG())                     // draw box for ECG graph
                return (0);                         // drawing is not completed, don't pass

            // drawing control to GOL, try it again
            screenState = DISPLAY_ECG;              // switch to next state
            return (1);                             // pass drawing control to GOL, redraw objects if needed

        case DISPLAY_ECG:
            UpdateECG(tick);
            return (1);                             // redraw objects if needed

        case CREATE_GRAPHSHOW:
            CreateGraphDemo();
            screenState = DISPLAY_GRAPHSHOW;        // switch to next state
            return (1);

        case DISPLAY_GRAPHSHOW:
            UpdateGraphDemo(tick);
            return (1);

        case CREATE_GAME:
        case DISPLAY_GAME:
        case DISPLAY_SLIDESHOW:
        case DEMO_MODE:
        default:
            break;
    }

    return (1); // release drawing control to GOL
}

/************************************************************************
 Function: void StartScreen()

 Overview: Shows the splash screen for introduction.
 
 Input: none

 Output: none
 ************************************************************************/
// Shows intro screen and waits for touch
#define SS_ORIGIN_X    ((GetMaxX()-316+1)/2)
#define SS_ORIGIN_Y    ((GetMaxY()-140+1)/2)
void StartScreen(void)
{
    SHORT               counter;
    static const XCHAR  text[] = {'W','e','l','c','o','m','e','!',0};
    WORD                i, j, k;
    WORD                ytemp, xtemp, srRes = 0x0001;

    SetColor(WHITE);
    ClearDevice();

    WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &mchpLogo, IMAGE_NORMAL));
    WAIT_UNTIL_FINISH(PutImage(SS_ORIGIN_X,SS_ORIGIN_Y, (void *) &intro, IMAGE_X2));
    
    for(counter = 0; counter < (GetMaxX()+1) - 32; counter++)
    {   // move Microchip icon
        #ifdef __PIC24FJ256DA210__
            if(counter == 0)
            {
                WAIT_UNTIL_FINISH(PutImage(counter, GetMaxY() - 34, (void *) &mchpIcon0, IMAGE_NORMAL));
            }
            else
            {
				DWORD srcOffset, dstOffset;
                DWORD x = counter;
                DWORD y = GetMaxY() - 34;
                WORD  width, height;

                #if (DISP_ORIENTATION == 0)
                {
                    srcOffset = ((DWORD)(y * DISP_HOR_RESOLUTION) + (x - 1));
                    dstOffset = ((DWORD)(y * DISP_HOR_RESOLUTION) + x);
                    width = GetImageWidth((void *) &mchpIcon0);
                    height = GetImageHeight((void *) &mchpIcon0);
				}
                #elif (DISP_ORIENTATION == 90)
                {
                    DWORD t = x;
                    x = y;
                    y = GetMaxX() - t - GetImageWidth((void *) &mchpIcon0);
                    srcOffset = (((DWORD)(y + 1) * DISP_HOR_RESOLUTION) + x);
                    dstOffset = ((DWORD)(y * DISP_HOR_RESOLUTION) + x);
                    height = GetImageWidth((void *) &mchpIcon0);
                    width = GetImageHeight((void *) &mchpIcon0);
				}
				#endif
				WAIT_UNTIL_FINISH(CopyBlock(GFX_DISPLAY_BUFFER_START_ADDRESS, GFX_DISPLAY_BUFFER_START_ADDRESS, \
											srcOffset, dstOffset, width, height));
				DelayMs(1);
            }
        #else
            WAIT_UNTIL_FINISH(PutImage(counter, GetMaxY() - 34, (void *) &mchpIcon0, IMAGE_NORMAL));
        #endif
    }

    SetColor(BRIGHTRED);
    SetFont((void *) &FONTDEFAULT);

    MoveTo((GetMaxX() - GetTextWidth((XCHAR *)text, (void *) &FONTDEFAULT)) >> 1, SS_ORIGIN_Y+122);
    WAIT_UNTIL_FINISH(OutText((XCHAR *)text));

    DelayMs(1200);

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
                PutPixel(SS_ORIGIN_X+xtemp + (j * 40), ytemp + (k * 30));
        }
    }
}

/************************************************************************
 Function: void CreateCtrlButtons(XCHAR *pTextA, XCHAR *pTextB, 
 								  XCHAR *pTextC, XCHAR *pTextD)

 Overview: Creates the navigation and control buttons of the menu.
 
 Input: pTextA - pointer to button A label.
		pTextB - pointer to button B label.
		pTextC - pointer to button C label.
		pTextB - pointer to button D label.

 Output: none
************************************************************************/
void CreateCtrlButtons(XCHAR *pTextA, XCHAR *pTextB, XCHAR *pTextC, XCHAR *pTextD)
{
    WORD    state;
    BUTTON  *pObj;


    state = BTN_DRAW;
    if(pTextA == NULL)
        state = BTN_DRAW | BTN_DISABLED;
    pObj = BtnCreate
    (
        ID_BUTTON_A,
        CtrlBtnLeft(0),
        CtrlBtnTop(),
        CtrlBtnRight(0),
        CtrlBtnBottom(),
        0,
        state,
        NULL,
        pTextA,
        altScheme
    );
    if (pObj == NULL)
        ShowError((XCHAR *)MemoryErrorStr);

    state = BTN_DRAW;
    if(pTextB == NULL)
        state = BTN_DRAW | BTN_DISABLED;
    pObj = BtnCreate
    (
        ID_BUTTON_B,
        CtrlBtnLeft(1),
        CtrlBtnTop(),
        CtrlBtnRight(1),
        CtrlBtnBottom(),
        0,
        state,
        NULL,
        pTextB,
        altScheme
    );
    if (pObj == NULL)
        ShowError((XCHAR *)MemoryErrorStr);

    state = BTN_DRAW;
    if(pTextC == NULL)
        state = BTN_DRAW | BTN_DISABLED;
    pObj = BtnCreate
    (
        ID_BUTTON_C,
        CtrlBtnLeft(2),
        CtrlBtnTop(),
        CtrlBtnRight(2),
        CtrlBtnBottom(),
        0,
        state,
        NULL,
        pTextC,
        altScheme
    );
    if (pObj == NULL)
        ShowError((XCHAR *)MemoryErrorStr);

    state = BTN_DRAW;
    if(pTextD == NULL)
        state = BTN_DRAW | BTN_DISABLED;
    pObj = BtnCreate
    (
        ID_BUTTON_D,
        CtrlBtnLeft(3),
        CtrlBtnTop(),
        CtrlBtnRight(3),
        CtrlBtnBottom(),
        0,
        state,
        NULL,
        pTextD,
        altScheme
    );
    if (pObj == NULL)
        ShowError((XCHAR *)MemoryErrorStr);
}

///////////////////////////////////////////////////////////////////////////////
////                            USB Related Functions
///////////////////////////////////////////////////////////////////////////////
#if defined(ENABLE_USB_MSD_DEMO)

/****************************************************************************
  Function:
    BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event,
                    void *data, DWORD size )

  Description:
    This routine handles USB events sent from the USB Embedded Host stack.

  Precondition:
    None

  Parameters:
    BYTE address    - Address of the USB device generating the event
    USB_EVENT event - Event that has occurred
    void *data      - Pointer to the data associated with the event
    DWORD size      - Size of the data pointed to by *data

  Return Values:
    TRUE    - The event was handled successfully
    FALSE   - The even was not handled successfully

  Remarks:
    We will default to returning TRUE for unknown events, and let operation
    proceed.  Other applications may wish to return FALSE, since we are not
    really handling the event.
  ***************************************************************************/
BOOL USB_ApplicationEventHandler(BYTE address, USB_EVENT event, void *data, DWORD size)
{
    WORD    yPos, TextHeight;

    switch(event)
    {
        case EVENT_VBUS_REQUEST_POWER:

            // We will let everything attach.
            return (TRUE);

        case EVENT_VBUS_RELEASE_POWER:

            // We are not monitoring power allocation, so we have
            // nothing to update.
            return (TRUE);

        case EVENT_HUB_ATTACH:
        case EVENT_UNSUPPORTED_DEVICE:
        case EVENT_CANNOT_ENUMERATE:
        case EVENT_CLIENT_INIT_ERROR:
        case EVENT_OUT_OF_MEMORY:
        case EVENT_UNSPECIFIED_ERROR:   // This should never occur
            usbErrorCode = USBHostDeviceStatus(1);

            // Shut down the USB.
            //USBHostShutdown();
            break;

        default:
            return (FALSE);
    }

    // USB error messages will only appear when in the Menu Screens.
    if(screenState != DISPLAY_DEMOSELECTION)
        return (TRUE);

    // go back to demo selection when exiting this function.
    screenState = CREATE_DEMOSELECTION;

    // The following code displays the different USB errors that can
    // occur. Example, inserting an unsupported device or device
    // does not enumerate.
    // clear the screen
    SetColor(WHITE);
    ClearDevice();

    // set up the font to display the error messages
    SetFont((void *) &GOLFontDefault);
    SetColor(BRIGHTBLUE);
    TextHeight = GetTextHeight((void *) &GOLFontDefault);
    yPos = TextHeight * 2;

    // output the standard USB error string
    MoveTo((GetMaxX() - GetTextWidth((XCHAR *)ErrMsgStandard, (void *) &GOLFontDefault)) >> 1, yPos);
    WAIT_UNTIL_FINISH(OutText((XCHAR *)ErrMsgStandard));
    yPos += TextHeight;

    switch(event)
    {
        case EVENT_HUB_ATTACH:
            MoveTo((GetMaxX() - GetTextWidth((XCHAR *)ErrMsgHUBAttachedStr, (void *) &GOLFontDefault)) >> 1, yPos);
            WAIT_UNTIL_FINISH(OutText((XCHAR *)ErrMsgHUBAttachedStr));
            yPos += TextHeight;
            MoveTo((GetMaxX() - GetTextWidth((XCHAR *)ErrNotSupported, (void *) &GOLFontDefault)) >> 1, yPos);
            WAIT_UNTIL_FINISH(OutText((XCHAR *)ErrNotSupported));
            break;

        case EVENT_UNSUPPORTED_DEVICE:
            MoveTo((GetMaxX() - GetTextWidth((XCHAR *)ErrMsgUDAttachedStr, (void *) &GOLFontDefault)) >> 1, yPos);
            WAIT_UNTIL_FINISH(OutText((XCHAR *)ErrMsgUDAttachedStr));
            yPos += TextHeight;
            MoveTo((GetMaxX() - GetTextWidth((XCHAR *)ErrNotSupported, (void *) &GOLFontDefault)) >> 1, yPos);
            WAIT_UNTIL_FINISH(OutText((XCHAR *)ErrNotSupported));
            break;

        case EVENT_CANNOT_ENUMERATE:
            MoveTo((GetMaxX() - GetTextWidth((XCHAR *)ErrMsgEnumerationStr, (void *) &GOLFontDefault)) >> 1, yPos);
            WAIT_UNTIL_FINISH(OutText((XCHAR *)ErrMsgEnumerationStr));
            yPos += TextHeight;
            MoveTo((GetMaxX() - GetTextWidth((XCHAR *)ErrMsgFailedStr, (void *) &GOLFontDefault)) >> 1, yPos);
            WAIT_UNTIL_FINISH(OutText((XCHAR *)ErrMsgFailedStr));
            break;

        case EVENT_CLIENT_INIT_ERROR:
            MoveTo((GetMaxX() - GetTextWidth((XCHAR *)ErrMsgClientInitStr, (void *) &GOLFontDefault)) >> 1, yPos);
            WAIT_UNTIL_FINISH(OutText((XCHAR *)ErrMsgClientInitStr));
            yPos += TextHeight;
            MoveTo((GetMaxX() - GetTextWidth((XCHAR *)ErrMsgFailedStr, (void *) &GOLFontDefault)) >> 1, yPos);
            WAIT_UNTIL_FINISH(OutText((XCHAR *)ErrMsgFailedStr));
            break;

        case EVENT_OUT_OF_MEMORY:
            MoveTo((GetMaxX() - GetTextWidth((XCHAR *)ErrMsgOutofMemoryStr, (void *) &GOLFontDefault)) >> 1, yPos);
            WAIT_UNTIL_FINISH(OutText((XCHAR *)ErrMsgOutofMemoryStr));
            break;

        case EVENT_UNSPECIFIED_ERROR:
            MoveTo((GetMaxX() - GetTextWidth((XCHAR *)ErrMsgUnpecifiedErrStr, (void *) &GOLFontDefault)) >> 1, yPos);
            WAIT_UNTIL_FINISH(OutText((XCHAR *)ErrMsgUnpecifiedErrStr));
            break;

        default:
            return (TRUE);
    }

    yPos += TextHeight;
    MoveTo((GetMaxX() - GetTextWidth((XCHAR *)MsgTouchToProceedStr, (void *) &GOLFontDefault)) >> 1, yPos);
    WAIT_UNTIL_FINISH(OutText((XCHAR *)MsgTouchToProceedStr));

    // wait for touch
    while(TouchGetX() == -1);

    return (TRUE);
}

#endif // defined(ENABLE_USB_MSD_DEMO)

/*********************************************************************************/

/*********************************************************************************/

/*********************************************************************************/

/*********************************************************************
* Function: Init_CPUClocks()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Initialize the CPU settings, clocks and registers for interrupts.
*
* Note: none
*
********************************************************************/
void Init_CPUClocks(void)
{
    #if defined(__C30__) || defined(__XC16__)
        #if defined(ENABLE_USB_MSD_DEMO)

    // set up USB clock source
    OSCCON = 0x3302;        // Enable secondary oscillator
        #endif
    #elif defined(__PIC32MX__)
    {
        int value;

        // this also enables the cache
        value = SYSTEMConfigPerformance(GetSystemClock());
        mOSCSetPBDIV(OSC_PB_DIV_2);

        INTEnableSystemMultiVectoredInt();

        value = OSCCON;
        while(!(value & 0x00000020))
        {
            value = OSCCON; // Wait for PLL lock to stabilize
        }
        #ifdef MEB_BOARD
        CPLDInitialize();
        CPLDSetGraphicsConfiguration(GRAPHICS_HW_CONFIG);
        CPLDSetSPIFlashConfiguration(SPI_FLASH_CHANNEL);
        #endif
    }

    AD1PCFG = 0xFFFF;       // Set analog pins to digital.
    TRISF = 0x00;
    #else
        #error Cannot initialize
    #endif
}

/*********************************************************************
* Function: Timer3 ISR
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
    #define __T3_ISR    __ISR(_TIMER_3_VECTOR, ipl4)
#else
    #define __T3_ISR    __attribute__((interrupt, shadow, auto_psv))
#endif

/* */
#define TICK_PRESCALER	16
int tickscaler;

void __T3_ISR _T3Interrupt(void)
{
    TMR3 = 0;
	if (tickscaler > TICK_PRESCALER)
	{
    	tickscaler = 0;
    	tick++;
 	} 
 	else
    	tickscaler++;
 	  	

    TouchDetectPosition();
    
    // Clear flag
    #ifdef __PIC32MX__
    mT3ClearIntFlag();
    #else
    IFS0bits.T3IF = 0;
    #endif
    
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
#ifdef __PIC32MX__
// for a system clock of 80 MHz
    #define TICK_PERIOD 		(GetSystemClock()/300000) 
#else
// for a system clock of 32 MHz
	#define SAMPLE_PERIOD       200 
	#define TICK_PERIOD			(GetPeripheralClock() * SAMPLE_PERIOD) / 4000000
#endif

/* */
void TickInit(void)
{

    // Initialize Timer4
    #ifdef __PIC32MX__
    OpenTimer3(T3_ON | T3_PS_1_8, TICK_PERIOD);  // enable timer to run for approximately 100 us
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_1); // Enable interrupt
    #else
    TMR3 = 0;
    PR3 = TICK_PERIOD;							//(for approximately 100 us)
    IFS0bits.T3IF = 0;  						//Clear flag
    IEC0bits.T3IE = 1;  						//Enable interrupt
    T3CONbits.TON = 1;  						//Run timer
    #endif
    
    tickscaler = 0;

}

/*********************************************************************
* Function: InitializeBoard()
*
* Input: none
*
* Output: none
*
* Overview: Initializes the hardware components including the PIC device
*           used.
*
*********************************************************************/
void InitializeBoard(void)
{
    #if defined (PIC24FJ256DA210_DEV_BOARD)
    //All pins to digital except those that are used as analog signals
        ANSA = 0x0000;
	    ANSB = 0x0020;		// RB5 as potentiometer input,
	    ANSC = 0x0010;		// RC4 as touch screen X+, RC14 as Rx
	    ANSD = 0x0000;
	    ANSE = 0x0000;      // RE9 is S2
	    ANSF = 0x0000;
	    ANSG = 0x0080;		// RG7 as touch screen Y+, RG8 is S1
	#elif defined (EXPLORER_16)
	
	    /////////////////////////////////////////////////////////////////////////////
	    // ADC Explorer 16 Development Board Errata (work around 2)
	    // RB15 should be output
	    /////////////////////////////////////////////////////////////////////////////
        #ifndef MEB_BOARD
        LATBbits.LATB15 = 0;
        TRISBbits.TRISB15 = 0;
        #endif
		     
    #else
		#if defined (__PIC24FJ256GB210__)
			ANSA = 0x0000;
		    ANSB = 0x002C;		// RB3 & RB2 are touch screen inputs RB5 as potentiometer input
		    ANSC = 0x0000;		
		    ANSD = 0x0000;
		    ANSE = 0x0000;
		    ANSF = 0x0000;
		    ANSG = 0x0000;		
		#endif

    #endif
	

    ///////////////////////////////////////////////
  #if !defined(__PIC32MX__)
    Init_CPUClocks();
  #endif
  
   #ifdef MEB_BOARD
       CPLDInitialize();
       CPLDSetGraphicsConfiguration(GRAPHICS_HW_CONFIG);
       CPLDSetSPIFlashConfiguration(SPI_FLASH_CHANNEL);
   #endif // #ifdef MEB_BOARD

   #if defined(__dsPIC33F__) || defined(__PIC24H__) || defined(__dsPIC33E__) || defined(__PIC24E__)
		// Configure Oscillator to operate the device at 40Mhz
        // Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
		#if defined(__dsPIC33E__) || defined(__PIC24E__)
			//Fosc = 8M * 60/(2*2) = 120MHz for 8M input clock
			PLLFBD = 58;    			// M=60         
		#else
             // Fosc= 8M*40(2*2)=80Mhz for 8M input clock
          	PLLFBD = 38;                    // M=40
        #endif
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
        
        #if defined(__dsPIC33F__) || defined(__PIC24H__)
	        // Set PMD0 pin functionality to digital
	        AD1PCFGL = AD1PCFGL | 0x1000;
	
	        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
	            AD1PCFGLbits.PCFG6 = 1;
	            AD1PCFGLbits.PCFG7 = 1;
	            AD1PCFGLbits.PCFG8 = 1;
	        #endif
	        
	    #elif defined(__dsPIC33E__) || defined(__PIC24E__)
            ANSELE = 0x00;
            ANSELDbits.ANSD6 = 0;

		    // Set all touch screen related pins to Analog mode.
	        ANSELBbits.ANSB11 = 1; 	        
	        #ifdef MEB_BOARD
	        	// Configure auxiliary PLL for USB.
		        // Configuring the auxiliary PLL, since the primary
	    		// oscillator provides the source clock to the auxiliary
	    		// PLL, the auxiliary oscillator is disabled. Note that
	    		// the AUX PLL is enabled. The input 8MHz clock is divided
	    		// by 2, multiplied by 24 and then divided by 2. Wait till 
	    		// the AUX PLL locks.	
			    ACLKCON3 = 0x24C1;   
			    ACLKDIV3 = 0x7;
			    
			    ACLKCON3bits.ENAPLL = 1;
			    while(ACLKCON3bits.APLLCK != 1); 
			    // Power USB device.
			    ANSELBbits.ANSB5 = 0;
			    TRISBbits.TRISB5 = 0;
			    LATBbits.LATB5 = 1;	
		    #endif
        #endif        
        
    #elif defined(__PIC32MX__)
        INTEnableSystemMultiVectoredInt();
        SYSTEMConfigPerformance(GetSystemClock());
    #endif // #if defined(__dsPIC33F__) || defined(__PIC24H__) || defined(__dsPIC33E__) || defined(__PIC24E__)

    #ifdef MEB_BOARD
        CPLDInitialize();
        CPLDSetGraphicsConfiguration(GRAPHICS_HW_CONFIG);
        CPLDSetSPIFlashConfiguration(SPI_FLASH_CHANNEL);
    #endif // #ifdef MEB_BOARD
   

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

    GOLInit();                      		// Initialize graphics library and create default style scheme for GOL

     //The following are PIC device specific settings for the SPI channel
     //used. 
    
     //Set IOs directions for SST25 SPI
        #if defined (GFX_PICTAIL_V3) || defined (MEB_BOARD) || defined (GFX_PICTAIL_V3E) || defined (GFX_PICTAIL_LCC)  
	        #if defined SST25_CS_ANS
		    SST25_CS_ANS = 0; // Change port to digital from analog.
		    #endif   
        SST25_CS_LAT = 1;
        SST25_CS_TRIS = 0;
        #ifndef __PIC32MX__
            SST25_SCK_TRIS = 0;
            SST25_SDO_TRIS = 0;
            SST25_SDI_TRIS = 1;
            #if defined(__PIC24FJ256GB210__) || defined(__dsPIC33E__) || defined(__PIC24E__)
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

    // set the peripheral pin select for the SPI channel used
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
    FLASHInit((DRV_SPI_INIT_DATA*)&SPI_Init_Data);
    
    // initialize the timer that manages the tick counter
    TickInit();                     	

    // Initialize the hardware buttons
    HardwareButtonInit();            

    // Programming the flash is not possible when using the PIC32 STK since the hardware 
    // does not support it (no serial port) so we skip this check if using the STKs
    #if defined (EXPLORER_16) || defined (PIC24FJ256DA210_DEV_BOARD)
        /**
         * To force a flash chip program
         * Explorer 16 + GFX PICTail    - S3 (8 bit PMP)
         * Explorer 16 + GFX PICTail    - S5 (16 bit PMP)
         * DA210 Developement Board     - S1
         **/
        // check if data in external flash is valid
        CheckExternalFlashHex();

	#endif	//defined (EXPLORER_16) || defined (PIC24FJ256DA210_DEV_BOARD)

    // initialize the components for Resistive Touch Screen
    TouchInit(NVMWrite, NVMRead, NVMSectorErase, NULL);   	


}    

/*********************************************************************
* Function: void CheckExternalFlashHex()
*
* PreCondition: none
*
* Input:  none
*
* Output: none
*
* Side Effects: none
*
* Overview: This function checks the data in the external flash is valid.
*
* Note: none
*
********************************************************************/
#if defined (EXPLORER_16) || defined (PIC24FJ256DA210_DEV_BOARD)
void CheckExternalFlashHex()
{
    typedef struct 
    {
        UINT32 mchpSignature;
        UINT32 mchpCRCData;
    } CRC_CHECK; 

    CRC_CHECK externalCRC, expectedCRC;   
    WORD textHeight;
    void *pFont;
    XCHAR *pStr = NULL;
    BOOL  setProgram = FALSE;
    
    XCHAR   msgStr1[] = {'P','r','o','g','r','a','m',' ','E','x','t','e','r','n','a','l',' ','D','a','t','a',0};
    XCHAR   msgStr2[] = {'E','x','t','e','r','n','a','l',' ','d','a','t','a',' ','i','n','v','a','l','i','d','.',0};
    XCHAR   msgStr3[] = {'P','l','e','a','s','e',' ','s','e','n','d',' ','d','a','t','a',' ','u','s','i','n','g',0};
    XCHAR   msgStr4[] = {'"','E','x','t','e','r','n','a','l',' ','M','e','m','o','r','y',0};
    XCHAR   msgStr5[] = {'P','r','o','g','r','a','m','m','e','r','"',' ','u','t','i','l','i','t','y',0};
    XCHAR   msgStr6[] = {'N','o','w',' ','w','a','i','t','i','n','g',' ','f','o','r',' ','d','a','t','a',0};
#ifdef USE_COMM_PKT_MEDIA_USB
    XCHAR   msgStr7[] = {'v','i','a',' ','U','S','B','.','.','.',0};
#else
    XCHAR   msgStr7[] = {'v','i','a',' ','U','A','R','T','.','.','.',0};
#endif

    pFont = (void*) &GOLFontDefault;
    SetFont(pFont);
    textHeight = GetTextHeight(pFont);

    // check if the CRC matches the data stored in the external flash memory
    expectedCRC.mchpCRCData = GRC_CRC32_EXTERNAL_MARKER;
    expectedCRC.mchpSignature = 0x5048434D;                // this is "MCHP"

    // check if programming is prompted     
    if(GetHWButtonProgram() == HW_BUTTON_PRESS)
    {
        pStr = msgStr1;
        setProgram = TRUE;
	} 

    if (setProgram == FALSE)
    {
        ReadArray(GRC_CRC32_EXTERNAL_ADDR, (BYTE *)&externalCRC, 8);
    
        if  ((expectedCRC.mchpCRCData != externalCRC.mchpCRCData) || \
             (expectedCRC.mchpSignature != externalCRC.mchpSignature))
        {
            // expected and read CRC does not match, proceed to programming flash first
            // run the flash programming 
            pStr = msgStr2;
            setProgram = TRUE;
        }
    }
    
    if (setProgram == TRUE)
    {
        SetColor(BLACK);
        ClearDevice();
        SetColor(WHITE);
        OutTextXY(10,10                 , pStr);
        OutTextXY(10,10 + (textHeight*2), msgStr3);
        OutTextXY(10,10 + (textHeight*3), msgStr4);
        OutTextXY(10,10 + (textHeight*4), msgStr5);
        OutTextXY(10,10 + (textHeight*5), msgStr6);
        OutTextXY(10,10 + (textHeight*6), msgStr7);

        // Call the external flash programming routine
        ProgramFlash();

        // check if UART is still busy sending replies to the host
        while(U2STAbits.TRMT);
        DelayMs(10);

        // Force Reset to force the checking of the flash memory if programming was a success
        Reset();
    }
}	
#endif

/*********************************************************************
* Function: void ShowError(XCHAR *pStr)
*
* PreCondition: none
*
* Input:  pStr - pointer to the error message
*
* Output: none
*
* Side Effects: none
*
* Overview: This function shows the equivalent blue screen of death.
*
* Note: none
*
********************************************************************/
void ShowError(XCHAR *pStr)
{
    SetColor(BLUE);
    ClearDevice();
    SetColor(WHITE);
    SetFont((void*)&FONTDEFAULT);
    OutTextXY(0,0,pStr);
    while(1);
}


/*********************************************************************
* Function: WORD ExternalMemoryCallback(EXTDATA* memory, LONG offset, WORD nCount, void* buffer)
*
* PreCondition: none
*
* Input:  memory - pointer to the bitmap or font external memory structures
*                  (FONT_EXTERNAL or BITMAP_EXTERNAL)
*         offset - data offset
*         nCount - number of bytes to be transferred to the buffer
*         buffer - pointer to the buffer
*
* Output: number of bytes were transferred.
*
* Side Effects: none
*
* Overview: this function must be implemented in application. Graphics Library calls it
*           each time the data from external memory is required. The application must copy 
*           required data to the buffer provided.
*
* Note: none
*
********************************************************************/

// If there are several memories in the system they can be selected by IDs.
// In this demo ID for memory chip installed on Graphics PICTail board is assumed to be 0.
#define SST39_MEMORY    0

/* */

WORD ExternalMemoryCallback(IMAGE_EXTERNAL *memory, LONG offset, WORD nCount, void *buffer)
{
    if(memory->ID == SST39_MEMORY)
    {

        // Read data requested into buffer provided
        #if defined (GFX_PICTAIL_V3) || defined (PIC24FJ256DA210_DEV_BOARD) || defined (MEB_BOARD) || defined (GFX_PICTAIL_LCC)
        SST25ReadArray(memory->address + offset, // address to read from
        (BYTE *)buffer, nCount);
        #else
        SST39PMPInit();
        SST39ReadArray(memory->address + offset, // address to read from
        (BYTE *)buffer, nCount);
        LCDPMPInit();
        #endif
    }

    return (nCount);
}
