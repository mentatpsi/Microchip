/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to run applications on PIC24FJ256DA10 Device Family 
 * using palettes.
 * 
 * To run this demo:
 * This demo uses only internal flash of the PIC device. To run this demo simply
 * program the generated hex file to the PIC device.
 *
 *****************************************************************************
 * FileName:        MainDemo.c
 * Dependencies:    MainDemo.h
 * Processor:       PIC24F
 * Compiler:       	MPLAB C30
 * Linker:          MPLAB LINK30
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/30/11	   ...
 *****************************************************************************/
#include "MainDemo.h"

// Configuration bits
#if defined (__PIC24FJ256DA210__)
    // difference of the two configs are the following
    // PIC24FJ256DA210_DEV_BOARD : ALTPMP_ALTPMPEN and ICS_PGx2
    // HI_REMOTE_CONTROL_BOARD   : ICS_PGx1
    #if defined (PIC24FJ256DA210_DEV_BOARD)
        _CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
        _CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
        _CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & ALTPMP_ALTPMPEN & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
    #elif defined (HI_REMOTE_CONTROL_BOARD)
        _CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
        _CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
        _CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
    #endif
#endif	

/////////////////////////////////////////////////////////////////////////////
// SPI Channel settings
/////////////////////////////////////////////////////////////////////////////
#if defined (USE_SST25VF016)
    const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
#endif    

/////////////////////////////////////////////////////////////////////////////
// Function Prototypes
/////////////////////////////////////////////////////////////////////////////
void BoardInit(void);
void TickInit(void);                                        // starts tick counter
void GraphicsInit();

/////////////////////////////////////////////////////////////////////////////
//                            DEMO STATES
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CREATE_SCREEN1 = 0,             // create screen 1 components with screen 1 palette
    DISPLAY_SCREEN1,                // display screen 1
    CREATE_SCREEN2,                 // create screen 2 components with screen 2 palette
    DISPLAY_SCREEN2,                // display screen 2
    HIDE_SCREEN,                    // state to disable the displat or turn the screen to BLACK
    SHOW_SCREEN,                    // state to enable the display
} SCREEN_STATES;

/////////////////////////////////////////////////////////////////////////////
//                       GLOBAL VARIABLES FOR DEMO
/////////////////////////////////////////////////////////////////////////////
SCREEN_STATES   screenState = HIDE_SCREEN;              // current state of demo state machine
SCREEN_STATES   targetScreenState = CREATE_SCREEN1;     // indicates which screen to show after enabling back the display
volatile DWORD  tick = 0;                               // tick counter
GOL_SCHEME      *pDemoScheme;                           // style scheme used

/* */
int main(void)
{
    // GOL message structure to interact with GOL
    GOL_MSG msg;                        

#if defined (PIC24FJ256DA210_DEV_BOARD)
    // initialize the graphics component    
    GraphicsInit();              
    // initialize the board    
    BoardInit();
#else    
    // initialize the board    
    BoardInit();
    // initialize the graphics component    
    GraphicsInit();              
#endif

    // Initialize SPI flash 
    SST25Init((DRV_SPI_INIT_DATA *)&SPI_Init_Data);

    // initialize the timer that manages the tick counter
    TickInit(); 

    // initialize the components for Resistive Touch Screen
    TouchInit(NVMWrite, NVMRead, NVMSectorErase, NULL); 

    SetBlackScreen();

    SetColor(BLACK);
    ClearDevice();

    pDemoScheme = GOLCreateScheme();
    pDemoScheme->TextColor0 = WHITE;
    pDemoScheme->TextColor1 = WHITE;

    while(1)
    {
        if(GOLDraw())
        {   
            TouchGetMsg(&msg);          // Get message from touch screen
            GOLMsg(&msg);               // Process message
        }
    }

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
void __attribute__((interrupt, shadow, auto_psv)) _T4Interrupt(void)
{
    tick++;

    TMR4 = 0;
    // Clear flag
    IFS1bits.T4IF = 0;

	TouchDetectPosition();    
}

/*********************************************************************
* Function:  void TickInit(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Initializes the tick timer.
*
* Note: none
*
********************************************************************/

/*********************************************************************
 * Section: Tick Delay
 *********************************************************************/
#define SAMPLE_PERIOD       500 // us
#define TICK_PERIOD			(GetPeripheralClock() * SAMPLE_PERIOD) / 4000000

void TickInit(void)
{
    // Initialize Timer4
    TMR4 = 0;
    PR4 = TICK_PERIOD;
    IFS1bits.T4IF = 0;  //Clear flag
    IEC1bits.T4IE = 1;  //Enable interrupt
    T4CONbits.TON = 1;  //Run timer

}


/////////////////////////////////////////////////////////////////////////////
// void BoardInit(void)
/////////////////////////////////////////////////////////////////////////////
void BoardInit(void)
{

 	ANSA = 0x0000;
	ANSB = 0x0000;
	ANSC = 0x0000;
	ANSD = 0x0000;
	ANSE = 0x0000;
	ANSF = 0x0000;
	ANSG = 0x0000;
       
    //The following are PIC device specific settings for the SPI channel
    //used. 
    SPIFlashConfigurePins();
       
    // set the peripheral pin select for the SPI channel used
#if defined (PIC24FJ256DA210_DEV_BOARD)

    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR1bits.RP2R = 11;                // assign RP2 for SCK2
  	RPOR0bits.RP1R = 10;                // assign RP1 for SDO2
   	RPINR22bits.SDI2R = 0;              // assign RP0 for SDI2
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

#elif defined (HI_REMOTE_CONTROL_BOARD)

    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    SPI2ConfigurePPS();
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    DisplayBacklightConfig();       
    DisplayBacklightOn();           
#endif

}

/*****************************************************************************
 * void GraphicsInit(void)
 *****************************************************************************/
void GraphicsInit()
{    
    // error checks when using IPU 
#if defined(USE_COMP_IPU) && defined (GFX_EPMP_CS1_BASE_ADDRESS)
    #if (GFX_DECOMPRESSED_DATA_RAM_ADDRESS < GFX_EPMP_CS1_BASE_ADDRESS)
        #error Set #define GFX_DECOMPRESSED_DATA_RAM_ADDRESS (GFX_EPMP_CS1_BASE_ADDRESS) in the hardware profile.
    #endif
    #if (GFX_COMPRESSED_DATA_RAM_ADDRESS != GFX_DECOMPRESSED_DATA_RAM_ADDRESS + GFX_DECOMPRESSED_BUFFER_SIZE)
	    #error Set #define GFX_COMPRESSED_DATA_RAM_ADDRESS (GFX_DECOMPRESSED_DATA_RAM_ADDRESS + GFX_DECOMPRESSED_BUFFER_SIZE)in the hardware profile.
    #endif    
#endif
#if defined(USE_COMP_IPU) && !defined (GFX_EPMP_CS1_BASE_ADDRESS)
    #error Cannot use IPU on the chosen hardware profile due to memory limitions. Uncomment USE_COMP_IPU in GraphicsConfig.h.
#endif
    // Initialize graphics library and crete default style scheme for GOL
    GOLInit();                              

    // set the palette color depth and initialize the palette used
    SetPaletteBpp(COLOR_DEPTH);
    SetPalette((void*)&SCR1_PALETTE, 0, SCR1_PALETTE.header.length);

	// enable the use of the palette
    EnablePalette();

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

   switch(screenState)
    {
        case CREATE_SCREEN1:
            
            GOLFree();

            SetFont((void*)&FONTDEFAULT);
            SetColor(TEXT_COLOR);
            OutTextXY(TEXT_POSX, TEXT_POSY, LABEL_TEXT);

            BtnCreate(  SCR1_BTNID, 
                        SCR1_BTNX, SCR1_BTNY, 
                        SCR1_BTNX + BTN_WIDTH, 
                        SCR1_BTNY + BTN_HEIGHT, 0,
                        BTN_DRAW | BTN_NOPANEL, 
                        SCR1_BTN_IMAGE, NULL, pDemoScheme);

            PictCreate( SCR1_PICTID, 
                        SCR1_PICTX, SCR1_PICTY, 
                        SCR1_PICTX + PICT_WIDTH, 
                        SCR1_PICTY + PICT_HEIGHT, 
                        PICT_DRAW, 1, SCR1_IMAGE, NULL);

            screenState = SHOW_SCREEN; 
            targetScreenState = DISPLAY_SCREEN1;
            break;

        case CREATE_SCREEN2:
            GOLFree();

            BtnCreate(  SCR2_BTNID, 
                        SCR2_BTNX, SCR2_BTNY, 
                        SCR2_BTNX + BTN_WIDTH, 
                        SCR2_BTNY + BTN_HEIGHT, 0,
                        BTN_DRAW | BTN_NOPANEL, 
                        SCR2_BTN_IMAGE, NULL, pDemoScheme);

            PictCreate( SCR2_PICTID, 
                        SCR2_PICTX, SCR2_PICTY, 
                        SCR2_PICTX + PICT_WIDTH, 
                        SCR2_PICTY + PICT_HEIGHT, 
                        PICT_DRAW, 1, SCR2_IMAGE, NULL);

            screenState = SHOW_SCREEN; 
            targetScreenState = DISPLAY_SCREEN2;
            break;

        case HIDE_SCREEN:
            
            // this turns on the TEST Mode in the graphics module where the data sent
            // to the screen is all BLACK color data
            SetBlackScreen();

            // RequestPaletteChange() waits for the "start of VSYNC" interrupt so when the palette is programmed
            // it is in the VSYNC period and no unwanted artifacts are seen on the screen. 
            if (targetScreenState == CREATE_SCREEN1)
                RequestPaletteChange((void*)&SCR1_PALETTE, 0, SCR1_PALETTE.header.length);
            else if (targetScreenState == CREATE_SCREEN2)
                RequestPaletteChange((void*)&SCR2_PALETTE, 0, SCR2_PALETTE.header.length);

            screenState = targetScreenState;
            break;

        case SHOW_SCREEN:
            SetNormalScreen();
            screenState = targetScreenState; 
            break;

        case DISPLAY_SCREEN1:
        case DISPLAY_SCREEN2:
            break;

        default:
            break;
    }

    return 1;
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
    // process messages for demo screens
    switch(screenState)
    {
        case DISPLAY_SCREEN1:

            if (GetObjID(pObj) == SCR1_BTNID)
            {
                if(objMsg == BTN_MSG_RELEASED)
                {
                    screenState = HIDE_SCREEN;  
                    targetScreenState = CREATE_SCREEN2;
                    BtnSetBitmap((BUTTON *)pObj, SCR1_BTN_IMAGE);
                }
                else if(objMsg == BTN_MSG_PRESSED)
                {                           
                    BtnSetBitmap((BUTTON *)pObj, SCR1_BTN_IMAGE_PRESSED);
                }
                else
                {
                    BtnSetBitmap((BUTTON *)pObj, SCR1_BTN_IMAGE);
                }
            }
            break;    

        case DISPLAY_SCREEN2:
            if (GetObjID(pObj) == SCR2_BTNID)
            {
                if(objMsg == BTN_MSG_RELEASED)
                {
                    screenState = HIDE_SCREEN;  
                    targetScreenState = CREATE_SCREEN1;
                    BtnSetBitmap((BUTTON *)pObj, SCR2_BTN_IMAGE);
                }
                else if(objMsg == BTN_MSG_PRESSED)
                {                           
                    BtnSetBitmap((BUTTON *)pObj, SCR2_BTN_IMAGE_PRESSED);
                }
                else
                {
                    BtnSetBitmap((BUTTON *)pObj, SCR2_BTN_IMAGE);
                }
            }
            break;    

        default:
            break;
    }
    return 1;
}



