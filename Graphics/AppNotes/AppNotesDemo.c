/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        AppNotesDemo.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright (c) 2011 Microchip Technology Inc.  All rights reserved.
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 03/17/11	    ...
 *****************************************************************************/
  
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "Graphics/Graphics.h"
#include "drv_spi.h"
#include "SST25VF016.h"   
#include "SST39LF400.h"
#include "MCHP25LC256.h"
#include "TouchScreen.h"
#include "cpld.h"
#include "AN1136Demo.h"
#include "AN1182Demo.h"
#include "AN1227Demo.h"
#include "AN1246Demo.h"

// Configuration bits
#if defined(__dsPIC33F__) || defined(__PIC24H__)
_FOSCSEL(FNOSC_PRI);
_FOSC(FCKSM_CSECMD &OSCIOFNC_OFF &POSCMD_XT);
_FWDT(FWDTEN_OFF);
#elif defined(__dsPIC33E__) 
_FOSCSEL(FNOSC_FRC);			
_FOSC(FCKSM_CSECMD & POSCMD_XT & OSCIOFNC_OFF & IOL1WAY_OFF);
_FWDT(FWDTEN_OFF);
_FPOR(FPWRT_PWR128 & BOREN_ON & ALTI2C1_ON & ALTI2C2_ON);
_FICD(ICS_PGD1 & RSTPRI_PF & JTAGEN_OFF);
#elif defined(__PIC32MX__)
    #pragma config FPLLODIV = DIV_2, FPLLMUL = MUL_20, FPLLIDIV = DIV_1, FWDTEN = OFF, FCKSM = CSECME, FPBDIV = DIV_1
    #pragma config OSCIOFNC = ON, POSCMOD = XT, FSOSCEN = ON, FNOSC = PRIPLL
    #pragma config CP = OFF, BWP = OFF, PWP = OFF
#else
    #if defined(__PIC24FJ256GB110__)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2)
_CONFIG2(0xF7FF & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_XT & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_OFF)
    #endif
    #if defined(__PIC24FJ256GA110__)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2)
_CONFIG2(IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_XT & FNOSC_PRIPLL & IOL1WAY_OFF)
    #endif
    #if defined(__PIC24FJ128GA010__)
_CONFIG2(FNOSC_PRIPLL & POSCMOD_XT) // Primary XT OSC with PLL
_CONFIG1(JTAGEN_OFF & FWDTEN_OFF)   // JTAG off, watchdog timer off
    #endif
	#if defined (__PIC24FJ256GB210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
	#endif
	#if defined (__PIC24FJ256DA210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & ALTPMP_ALTPMPEN & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
	#endif	
#endif

/////////////////////////////////////////////////////////////////////////////
//                            DEMO STATES
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CREATE_MAINMENU = 0,
    DISPLAY_MAINMENU,
    CREATE_AN1136,
    DISPLAY_AN1136,
    CREATE_AN1182,
    DISPLAY_AN1182,
    CREATE_AN1227,
    DISPLAY_AN1227,
    CREATE_AN1246,
    DISPLAY_AN1246,
} SCREEN_STATES;

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT'S IDs
/////////////////////////////////////////////////////////////////////////////
#define ID_ICON1 		    0xF010
#define ID_ICON2 		    0xF011
#define ID_ICON3 		    0xF012
#define ID_ICON4 		    0xF013
#define ID_BACK             0xF050

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT DIMENSIONS
/////////////////////////////////////////////////////////////////////////////
#define ICON_WIDTH          (72)
#define ICON_HEIGHT         (100)   
#define ICON_COLUMNSNUM     (2)
#define ICON_ROWSNUM        (2)
#define ICON_XINDENT        ((GetMaxX() - ICON_COLUMNSNUM * ICON_WIDTH) / (ICON_COLUMNSNUM + 1))
#define ICON_YINDENT        ((GetMaxY() - ICON_ROWSNUM * ICON_HEIGHT) / (ICON_ROWSNUM + 1))
#define IconTop(row)        (((row + 1) * ICON_YINDENT) + (row * ICON_HEIGHT))
#define IconBottom(row)     ((row + 1) * (ICON_YINDENT + ICON_HEIGHT))
#define IconLeft(column)    (((column + 1) * ICON_XINDENT) + (column * ICON_WIDTH))
#define IconRight(column)   ((column + 1) * (ICON_XINDENT + ICON_WIDTH))

/////////////////////////////////////////////////////////////////////////////
// SPI Channel settings
/////////////////////////////////////////////////////////////////////////////
#ifdef SPI_CHANNEL_1_ENABLE
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

#ifdef SPI_CHANNEL_2_ENABLE
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

#ifdef SPI_CHANNEL_3_ENABLE
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
#ifdef SPI_CHANNEL_4_ENABLE
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
// SPI Device Initialization Function 
/////////////////////////////////////////////////////////////////////////////
#if defined (USE_SST25VF016)
    // initialize GFX3 SST25 flash SPI
    #define FlashInit(pInitData) SST25Init((DRV_SPI_INIT_DATA*)pInitData)                    
#elif defined (USE_MCHP25LC256)
    // initialize EEPROM on Explorer 16
    #define FlashInit(pInitData) MCHP25LC256Init((DRV_SPI_INIT_DATA*)pInitData)
#endif

/////////////////////////////////////////////////////////////////////////////
//                            IMAGES USED
/////////////////////////////////////////////////////////////////////////////
// iinternal flash image
extern const IMAGE_FLASH    MCHPFolderFile_8bpp_72x72;
extern const IMAGE_FLASH    MCHPFolderEmpty_8bpp_72x72;

/////////////////////////////////////////////////////////////////////////////
//                            FONTS USED
/////////////////////////////////////////////////////////////////////////////
extern const FONT_FLASH     Gentium_16;

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void            TickInit(void);            // starts tick counter
void            InitializeBoard(void);          
WORD            CreateMainMenu();
WORD            CreateBackButton();

volatile DWORD  tick = 0, prevTick;             // tick counter,

/////////////////////////////////////////////////////////////////////////////
//                            IMAGES USED
/////////////////////////////////////////////////////////////////////////////
// see AppNotesDemoIcons.h

/////////////////////////////////////////////////////////////////////////////
//                                  MAIN
/////////////////////////////////////////////////////////////////////////////
GOL_SCHEME      *demoScheme;                // alternative style scheme
IMAGE_FLASH     *pIconPressed  = (void*) &MCHPFolderFile_8bpp_72x72;
IMAGE_FLASH     *pIconReleased = (void*) &MCHPFolderEmpty_8bpp_72x72;

// current state of main demo state mashine
SCREEN_STATES   screenState = CREATE_MAINMENU;               


XCHAR Labels[4][7] = { {'A','N','1','1','3','6',0},
                       {'A','N','1','1','8','2',0},
                       {'A','N','1','2','2','7',0},
                       {'A','N','1','2','4','6',0},
                     };

XCHAR MainMenuStr[5] = { 'M','A','I','N',0};


/* */
int main(void)
{
    GOL_MSG msg;                    // GOL message structure to interact with GOL
   
    InitializeBoard();
    
    SetColor(WHITE);
    ClearDevice();

    demoScheme = GOLCreateScheme();  // create alternative style scheme
    demoScheme->TextColor0 = BRIGHTBLUE;
    demoScheme->TextColor1 = BRIGHTRED;
    demoScheme->pFont = (void*)&Gentium_16;
    
    CreateMainMenu();
    screenState = DISPLAY_MAINMENU;    

    while(1)
    {
        if(GOLDraw())
        {                           // Draw GOL object
            if (screenState == DISPLAY_AN1227)
                AN1227ProcessKeyBoard();
            else
            {    
                TouchGetMsg(&msg);      // Get message from touch screen
                GOLMsg(&msg);           // Process message
            }    
            
        }
    }

}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD CreateBackButton()
// Input: none
// Output: Returns 0 if the screen is created and 0 if not.
// Overview: Creates the four icons to launch each of the application 
//           note demos.
/////////////////////////////////////////////////////////////////////////////
WORD CreateBackButton()
{
    BUTTON *pObj;

    pObj = BtnCreate (  ID_BACK,     // object�s ID
                        20, 10,
                        70, 35, 0,
                        BTN_DRAW,   
                        NULL,
                        MainMenuStr,  
                        demoScheme
                     );
                     
    if(pObj == NULL)
        return (0);        
        
    return 1;
    
}    

/////////////////////////////////////////////////////////////////////////////
// Function: WORD CreateMainMenu()
// Input: none
// Output: Returns 0 if the screen is created and 0 if not.
// Overview: Creates the four icons to launch each of the application 
//           note demos.
/////////////////////////////////////////////////////////////////////////////
WORD CreateMainMenu()
{
    int i;
    SHORT left = 0, top = 0, right = 0, bottom = 0;

    // loop to create the four icons
    for (i=0; i<4; i++)
    {
        switch (i)
        {
            case 0:
                top    = IconTop(0);
                bottom = IconBottom(0);
                left   = IconLeft(0);
                right  = IconRight(0);      
                break;
            case 1:
                top    = IconTop(0);
                bottom = IconBottom(0);
                left   = IconLeft(1);
                right  = IconRight(1);      
                break;
            case 2:
                top    = IconTop(1);
                bottom = IconBottom(1);
                left   = IconLeft(0);
                right  = IconRight(0);      
                break;
            case 3:
                top    = IconTop(1);
                bottom = IconBottom(1);
                left   = IconLeft(1);
                right  = IconRight(1);      
                break;
            default: break;    
        }    

        if (!BtnCreate (
                        ID_ICON1+i,     // object�s ID
                        left, top,
                        right, bottom,
                        0,              // radius if rounded edge
                        // draw the object after creation with
                        // text drawn at the bottom and no panel
                        BTN_DRAW|BTN_NOPANEL|BTN_TEXTBOTTOM,   
                        pIconReleased,  // use this bitmap 
                        Labels[i],      // use this text
                        demoScheme      // use alternative style scheme
                        )
            )
            return 0;

#if ( defined(USE_16BIT_PMP) && defined(__PIC24FJ256GB210__)) ||                                                \
    ( defined(USE_16BIT_PMP) && (defined(PIC_SK) || defined(PIC32_GP_SK) || defined(PIC32_ETH_SK) || defined(PIC32_USB_SK))) ||    \
    ( defined(__dsPIC33F__) || defined(__PIC24H__) )
 
    /* Special case: when using the following
        - 16-bit PMP on non PIC24FJ256DA210 - pins allocated for the switches are used by PMP data lines
        - 16-bit PMP with starter kit - pins allocated for the switches are used by PMP data lines
        - dsPIC and PIC24H PIMs - switches are not mapped to the PIMs
        AN1227 demo will be disabled and the the icon for that demo
        will not show on the screen.
    */
    if (i == 2)
    {
        SetState(GOLFindObject(ID_ICON1+i), DISABLED);
        BtnSetBitmap((BUTTON*)GOLFindObject(ID_ICON1+i), NULL);
        BtnSetText((BUTTON*)GOLFindObject(ID_ICON1+i), NULL);
    }
 
#endif    
    }
    return 1;
    
}    

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
// Input: objMsg - translated message for the object,
//        pObj - pointer to the object,
//        pMsg - pointer to the non-translated, raw GOL message
// Output: if the function returns non-zero the message will be processed by default
// Overview: it's a user defined function. GOLMsg() function calls it each
//           time the valid message for the object received
/////////////////////////////////////////////////////////////////////////////
WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    WORD objID;

    // check first if we need to exit the selected demo (if applicable)
    if ((objMsg == BTN_MSG_RELEASED) && (pObj->ID == ID_BACK))
    {   
        // check if the button is indeed pressed
        if (GetState(pObj, BTN_PRESSED))
        {
            screenState = CREATE_MAINMENU;
            return 1;
        }    
    }
    // process messages for demo screens
    switch(screenState)
    {
        case DISPLAY_MAINMENU:

            // In this portion, all management of the Button states are 
            // done here. This portion will always return 0 to inhibit
            // the button default message processing to take place.
                        
            objID = GetObjID(pObj);
            if ((objID == ID_ICON1) || (objID == ID_ICON2) ||
                (objID == ID_ICON3) || (objID == ID_ICON4))
            {    
                if(objMsg == BTN_MSG_PRESSED) 
                {   
                    // check if button is pressed and show pressed icon
                    BtnSetBitmap(pObj, pIconPressed);               
                    SetState(pObj, BTN_PRESSED);
                }
                else if(objMsg == BTN_MSG_RELEASED)
                {
                    // check if button is released and show release icon
                    BtnSetBitmap(pObj, pIconReleased);              
                    ClrState(pObj, BTN_PRESSED);
                    if (objID == ID_ICON1)
                        screenState = CREATE_AN1136;
                    else if (objID == ID_ICON2)
                        screenState = CREATE_AN1182;
                    else if (objID == ID_ICON3)
                        screenState = CREATE_AN1227;
                    else if (objID == ID_ICON4)
                        screenState = CREATE_AN1246;
                    else
                        // this should not happen 
                        screenState = DISPLAY_MAINMENU;    
                }
                else if(objMsg == BTN_MSG_STILLPRESSED)
                {
                    // do not do anything since it is already pressed
                    return 0;     
                }    
                else
                {
                    // check if button is released and show release icon
                    BtnSetBitmap(pObj, pIconReleased);              
                    ClrState(pObj, BTN_PRESSED);              // reset pressed
                }            
                
                // cancel all focus here since this is the main menu
                // we do not want to show the focus on the icons
                ClrState(pObj, DRAW_FOCUS | FOCUSED);
                SetState(pObj, DRAW);
                return 0;
            }
            break;

        case DISPLAY_AN1136:
            return (MsgAN1136Callback(objMsg, pObj, pMsg));

        case DISPLAY_AN1182:
            return (MsgAN1182Callback(objMsg, pObj, pMsg));

        case DISPLAY_AN1227:
            return (MsgAN1227Callback(objMsg, pObj, pMsg));

        case DISPLAY_AN1246:
            if(((TEXTENTRY *)pObj)->pActiveKey->index == EXITKEYINDEX)
            {
                if (objMsg == TE_MSG_ENTER) 
                    screenState = CREATE_MAINMENU;          // exit the AN1246 Demo
            }
            else
                return (MsgAN1246Callback(objMsg, pObj, pMsg));
        
        default:
            break;
    }        

    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLDrawCallback()
// Output: if the function returns non-zero the draw control will be passed to GOL
// Overview: it's a user defined function. GOLDraw() function calls it each
//           time when GOL objects drawing is completed. User drawing should be done here.
//           GOL will not change color, line type and clipping region settings while

//           this function returns zero.
/////////////////////////////////////////////////////////////////////////////
WORD GOLDrawCallback(void)
{
    
   switch(screenState)
    {
        case CREATE_MAINMENU:
            GOLFree();
            SetColor(WHITE);
            ClearDevice();
            
            CreateMainMenu();
            
            screenState = DISPLAY_MAINMENU;       // switch to next state
            break;
            
        case DISPLAY_MAINMENU:
            break;
            
        case CREATE_AN1136:
            GOLFree();
            
            CreateBackButton();
            CreateAN1136();
            
            screenState = DISPLAY_AN1136;         // switch to next state
            break;
            
        case DISPLAY_AN1136:
            DrawAN1136Callback();
            break;

        case CREATE_AN1182:
            GOLFree();
            
            CreateBackButton();
            CreateAN1182();
            
            screenState = DISPLAY_AN1182;         // switch to next state
            break;
            
        case DISPLAY_AN1182:
            DrawAN1182Callback();
            break;

        case CREATE_AN1227:
            GOLFree();
            
            CreateBackButton();
            CreateAN1227();
            
            screenState = DISPLAY_AN1227;         // switch to next state
            break;
            
        case DISPLAY_AN1227:
            DrawAN1227Callback();
            break;

        case CREATE_AN1246:
            GOLFree();
            CreateAN1246();
            
            screenState = DISPLAY_AN1246;         // switch to next state
            break;
            
        case DISPLAY_AN1246:
            DrawAN1246Callback();
            break;

            
        default: 
            break;    
    }    
    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: Timer3 ISR
// Input: none
// Output: none
// Overview: increments tick counter. Tick is approx. 1 ms.
/////////////////////////////////////////////////////////////////////////////
#ifdef __PIC32MX__
    #define __T3_ISR    __ISR(_TIMER_3_VECTOR, ipl4)
#else
    #define __T3_ISR    __attribute__((interrupt, shadow, auto_psv))
#endif

/* */
void __T3_ISR _T3Interrupt(void)
{
    tick++;

    TMR3 = 0; 
	// Clear flag
    #ifdef __PIC32MX__
    mT3ClearIntFlag();
    #else
    IFS0bits.T3IF = 0;
    #endif

	TouchDetectPosition();    
}

/////////////////////////////////////////////////////////////////////////////
// Function: void TickInit(void)
// Input: none
// Output: none
// Overview: Initilizes the tick timer.
/////////////////////////////////////////////////////////////////////////////

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
    OpenTimer3(T3_ON | T3_PS_1_8, TICK_PERIOD);
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_4);
    #else
    TMR3 = 0;
    PR3 = TICK_PERIOD;
    IFS0bits.T3IF = 0;  //Clear flag
    IEC0bits.T3IE = 1;  //Enable interrupt
    T3CONbits.TON = 1;  //Run timer
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

     #if defined(PIC24FJ256DA210_DEV_BOARD)
        _ANSG8 = 0; /* S1 */
        _ANSE9 = 0; /* S2 */
        _ANSB5 = 0; /* S3 */
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


   #ifdef MEB_BOARD
            CPLDInitialize();
            CPLDSetGraphicsConfiguration(GRAPHICS_HW_CONFIG);
            CPLDSetSPIFlashConfiguration(SPI_FLASH_CHANNEL);
   #endif      // #ifdef MEB_BOARD    
 
    #if defined(__dsPIC33F__) || defined(__PIC24H__) || defined(__dsPIC33E__) 

        // Configure Oscillator to operate the device at 40Mhz
        // Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
        #if defined(__dsPIC33E__) 
            //Fosc = 8M * 60/(2*2) = 120MHz for 8M input clock
            PLLFBD = 58;                // M=60         
        #else
        	// Fosc= 8M*40(2*2)=80Mhz for 8M input clock
        	PLLFBD = 38;                // M=40
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
        
        #elif defined(__dsPIC33E__)
            ANSELE = 0x00;
            ANSELDbits.ANSD6 = 0;

            // Set all touch screen related pins to Analog mode.
            ANSELBbits.ANSB11 = 1; 
        #endif
        
    #elif defined(__PIC32MX__)
        INTEnableSystemMultiVectoredInt();
        SYSTEMConfigPerformance(GetSystemClock());
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

    GOLInit();

     //The following are PIC device specific settings for the SPI channel
     //used. 
    
     //Set IOs directions for SST25 SPI
        #if defined (GFX_PICTAIL_V3) || defined (MEB_BOARD) || defined (GFX_PICTAIL_V3E)   
        SST25_CS_LAT = 1;
        SST25_CS_TRIS = 0;
        #ifndef __PIC32MX__
            SST25_SCK_TRIS = 0;
            SST25_SDO_TRIS = 0;
            SST25_SDI_TRIS = 1;
            #if defined(__PIC24FJ256GB210__) || defined(__dsPIC33E__) 
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
    FlashInit(&SPI_Init_Data);
    
    // initialize the timer that manages the tick counter
    TickInit();                     	

    // initialize the components for Resistive Touch Screen
    TouchInit(NVMWrite, NVMRead, NVMSectorErase, NULL);   	
    HardwareButtonInit();           	// Initialize the hardware buttons

}    


