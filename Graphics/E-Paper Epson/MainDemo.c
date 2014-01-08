/*********************************************************************
 *
 *	Main demo for the E-Paper Demo
 *
 *********************************************************************
 * FileName:        MainDemo
 * Dependencies:    None
 * Processor:       PIC24, dsPIC, PIC32
 * Compiler:        Microchip C30 and C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (c) 2011 Microchip Technology Inc.  All rights 
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
 * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product ("Device") which is 
 *      integrated into Licensee�s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and 
 *      ENC28J60.h ported to a non-Microchip device used in 
 *      conjunction with a Microchip ethernet controller for the 
 *      sole purpose of interfacing with the ethernet controller. 
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT 
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL 
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF 
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS 
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT 
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 ********************************************************************/
/*****************************************
 * Includes
 *****************************************/
#include "MainDemo.h"
#include "ID.h"
#include "screens.h"
#include "drv_spi.h"
#include "GraphicsConfig.h"
#include "gfx_font_resources.h"
#include "SST25VF064.h"
#include "ExternalMemory.h"
#include "FlashProgrammer.h"

#ifdef USE_BITMAP_EXTERNAL
#include "external_images.h"
#else
#error This demo uses external images and needs to have USE_BITAMP_EXTERNAL defined in GraphicsConfig.h
#endif


/*****************************************
 * Configuration Bits
 *****************************************/
#if defined(__dsPIC33F__) || defined(__PIC24H__)
_FOSCSEL(FNOSC_PRI);
_FOSC(FCKSM_CSECMD &OSCIOFNC_OFF &POSCMD_XT);
_FWDT(FWDTEN_OFF);
#elif defined(__PIC32MX__)
    #pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF, FCKSM = CSECME, FPBDIV = DIV_1
    #pragma config OSCIOFNC = ON, POSCMOD = XT, FSOSCEN = ON, FNOSC = PRIPLL
    #pragma config CP = OFF, BWP = OFF, PWP = OFF
#else
    #if defined(__PIC24FJ256GB110__)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2)
_CONFIG2(0xF7FF & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_OFF)
    #endif
    #if defined(__PIC24FJ256GA110__)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2)
_CONFIG2(IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & IOL1WAY_OFF)
    #endif
    #if defined(__PIC24FJ128GA010__)
_CONFIG2(FNOSC_PRIPLL & POSCMOD_XT) // Primary XT OSC with PLL
_CONFIG1(JTAGEN_OFF & FWDTEN_OFF)   // JTAG off, watchdog timer off
    #endif
	#if defined (__PIC24FJ256GB210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_HS & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
	#endif
	#if defined (__PIC24FJ256DA210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_HS & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & ALTPMP_ALTPMPEN & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
	#endif	
#endif

/////////////////////////////////////////////////////////////////////////////
// SPI FLASH Settings
/////////////////////////////////////////////////////////////////////////////
#if defined (USE_SST25VF064)
    #ifdef __PIC32MX
        const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL,     1, 0, 0, 1, 1, 0};
    #else    
        const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL,     3,  6, 0, 1, 1, 0};
    #endif
#endif    

/*****************************************
 * Variables
 *****************************************/
volatile BOOL g_AutoMode = TRUE;
volatile WORD AutoModeCnt;
volatile HW_BUTTON_STATE btnS1, btnS2, btnS3;
WORD g_UPDATE_FLAGS = GFX_UPDATE_NO_FLASH | GFX_WAIT_IMAGE_DISPLAYED;

/*****************************************
 * Externs
 *****************************************/
extern DEMO_SCREENS demoScreens;
extern BOOL GFX_DRIVER_IsUpdateRequested(void);
/* 
The definition of ONE_CYCLE_DRAWING should be visible in
MainDemo.c and tick.c files, and so can be defined in MainDemo.h 
*/
#if !defined( ONE_CYCLE_DRAWING	)
extern BOOL g_UpdateNow;
#endif

/*****************************************
 * Function Prototypes
 *****************************************/
void CheckExternalFlashHex(void);

/*****************************************
 * void InitializeHardware(void)	
 *****************************************/
void InitializeHardware(void)	
{
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
    
    #elif defined(__PIC32MX__)
    SYSTEMConfig(GetSystemClock(), SYS_CFG_ALL);
    #ifdef MEB_BOARD
    CPLDInitialize();
    CPLDSetGraphicsConfiguration(GRAPHICS_HW_CONFIG);
    CPLDSetSPIFlashConfiguration(SPI_FLASH_CHANNEL);
    #endif
    #endif

    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
    AD1PCFGL = 0xffff;
    #endif
    
    #if defined (__PIC24FJ256GB210__)        
    // Map UART2
    __builtin_write_OSCCONL(OSCCON & 0xbf);    
	// Configure Input Functions (Table 10-2))
	// Assign U2RX To Pin RP10
	RPINR19bits.U2RXR = 10;	

	// Assign U2CTS To Pin RP32
	RPINR19bits.U2CTSR = 32;

	// Configure Output Functions (Table 10-4)
	// Assign U2TX To Pin RP17
	RPOR8bits.RP17R = 5;
	
	// Assign U2RTS To Pin RP31
	RPOR15bits.RP31R = 6;
    __builtin_write_OSCCONL (OSCCON | 0x40);
    #endif


	#if ( USE_SPI_CHANNEL == 1 )
		#if defined (__PIC24FJ256GB210__)
			__builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
			// Configure SPI1 PPS pins (ENC28J60/ENCX24J600/MRF24WB0M or other PICtail Plus cards)
			RPOR0bits.RP0R = 8;		// Assign RP0 to SCK1 (output)
			RPOR7bits.RP15R = 7;	// Assign RP15 to SDO1 (output)
			//RPOR6bits.RP13R = 9;	// Assign RP15 to SDO1 (output)
			RPINR20bits.SDI1R = 23;	// Assign RP23 to SDI1 (input)
			__builtin_write_OSCCONL(OSCCON | 0x40); // lock   PP
		#endif
	#endif 
	
    /////////////////////////////////////////////////////////////////////////////
    // ADC Explorer 16 Development Board Errata (work around 2)
    // RB15 should be output
    /////////////////////////////////////////////////////////////////////////////
    #ifndef MEB_BOARD
        LATBbits.LATB15 = 0;
        TRISBbits.TRISB15 = 0;
    #endif


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
	#if defined (USE_SST25VF064)	    
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
	#endif

    // set the peripheral pin select for the SPI channel used
    #if defined(__PIC24FJ256GB110__) || defined(__PIC24FJ256GA110__) || defined (__PIC24FJ256GB210__)
        __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
        RPOR10bits.RP21R = 11;                  // assign RP21 for SCK2
        RPOR9bits.RP19R = 10;                   // assign RP19 for SDO2
        RPINR22bits.SDI2R = 26;                 // assign RP26 for SDI2
        __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    #endif

	#if defined (USE_SST25VF064)
    	SST25Init((DRV_SPI_INIT_DATA *)&SPI_Init_Data);
    #endif
}
/*****************************************
 *int main (void)
 *****************************************/
int main (void)
{
    InitializeHardware();
    HardwareButtonInit();
    InitAllLEDs();
    
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH     
	GFX_DRIVER_AutoUpdPart();		// Turn on widget auto update, partial update for less flashing
#endif
    
    InitTick();
	GOLInit();

    SetColor(WHITE);
    ClearDevice();

	// Set proper display rotation
#if(DISP_ORIENTATION == 90)
    GFX_DRIVER_InitRotmode(ROTATE_90);
#else
#error "This PICTail display orientation must be 90."
#endif
       
	// make sure that the correct hex file is loaded
    CheckExternalFlashHex();

	// Create cursor in GFX_CURSOR_LAYER with Alpha Color = 0xA
	GFX_DRIVER_CreateLayer( GFX_CURSOR_LAYER, GFX_LAYER_TRANS_EN | 0xA, GetX(), GetY(), GetX() + 31, GetY() + 31 );
	GFX_DRIVER_ActivateLayer( GFX_CURSOR_LAYER );
	PutImage(0, 0, (void *)&mouse_cursor_icon_270, IMAGE_NORMAL); 
	GFX_DRIVER_ActivateLayer( GFX_MAIN_LAYER );
	
	// Start demo screen 
	demoScreens = DEMO_INTRO_SCREEN_CREATE;
	
    while(1)
	{
        GOLDraw();
#ifndef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
	#if defined( ONE_CYCLE_DRAWING	) 
		// The screen drawing starts and completes during one while(1) cycle loop in main().
		// This Demo is one cycle drawing application.
        if ( GFX_DRIVER_IsUpdateRequested() || (g_UPDATE_FLAGS == GFX_UPDATE_AS_IT_DRAWS) )
        {
	    	GFX_DRIVER_UpdateEpd( g_UPDATE_FLAGS, 0, 0, GetMaxX(), GetMaxY() ); 
	    	g_UPDATE_FLAGS = GFX_UPDATE_NO_FLASH | GFX_WAIT_IMAGE_DISPLAYED;   
	    }
	#else
		// This way can be used when drawing may take few or more cycles of while(1) loop in main().
		// See "tick.c" file for details.
        if ( g_UpdateNow || (g_UPDATE_FLAGS == GFX_UPDATE_AS_IT_DRAWS) )
        {
	        g_UpdateNow = 0;
	    	GFX_DRIVER_UpdateEpd( g_UPDATE_FLAGS, 0, 0, GetMaxX(), GetMaxY() ); 
	    	g_UPDATE_FLAGS = GFX_UPDATE_NO_FLASH | GFX_WAIT_IMAGE_DISPLAYED;   
	    }	
	#endif
#endif		 
	}	

    return (-1);
}

/*****************************************
 * void CreateMainPage(void)
 *****************************************/
void CreateMainDemoPage(void)
{
	WndCreate
	(
	    ID_WINDOW1,                 // ID
	    0,
	    0,
	    GetMaxX(),
	    GetMaxY(),                  // dimension
	    WND_DRAW,                   // will be dislayed after creation
	    NULL,                       // icon
	    NULL,                       // set text
	    NULL                        // default GOL scheme
	);                              
	
	BtnCreate
	(
	    ID_BUTTON_BACK,             // button ID
	    0,
	    40,                         // left, top corner	
	    NAV_BTN_WIDTH,
	    GetMaxY(),
	    0,                          // right, bottom corner (with radius = 0)
	    BTN_DRAW,                   // will be dislayed after creation
	    NULL,                       // no bitmap	
	    (XCHAR *)"<",               // LEFT arrow as text
	    NULL                        // default GOL scheme
	);                              
	
	BtnCreate
	(
	    ID_BUTTON_NEXT,             // button ID
	    GetMaxX() - NAV_BTN_WIDTH,
	    40,
	    GetMaxX(),
	    GetMaxY(),
	    0,                          // dimension (with radius = 0)
	    BTN_DRAW,                   // will be dislayed and disabled after creation
	    NULL,                       // no bitmap
	    (XCHAR *)">",               // RIGHT arrow as text
	    NULL                        // default GOL scheme
	);                          	 
	 
} 
/*****************************************
 * void PrepareDemoPage(XCHAR *pText)
 *****************************************/
void PrepareDemoPage(XCHAR *pText)
{
    SetColor(WHITE);   
    Bar(26, 40, GetMaxX()-26, GetMaxY()-1);
    
    if ( pText )
    {		    		    
	    WINDOW * pMainWin  = (WINDOW*)GOLFindObject(ID_WINDOW1);
	    WndSetText(pMainWin, pText);
	    SetState(pMainWin, WND_DRAW_TITLE);
        WndDraw(pMainWin);
    } 
}
/***************************************************************
 * WORD ExternalMemoryCallback(IMAGE_EXTERNAL *memory, LONG offset, WORD nCount, void *buffer)
 ***************************************************************/
WORD ExternalMemoryCallback(IMAGE_EXTERNAL *memory, LONG offset, WORD nCount, void *buffer)
{
	// Read data requested into buffer provided 
	#if defined (USE_SST25VF064)  
    	SST25ReadArray(memory->address + offset, (BYTE *)buffer, nCount);
    #else
        #error "Please make sure that you have USE_SST25VF064 defined in the Hardware Profile header file."
    #endif

    return (nCount);
}
/***************************************************************
 * void CheckExternalFlashHex(void)
 ***************************************************************/
void CheckExternalFlashHex(void)
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
    XCHAR   msgStr7[] = {'v','i','a',' ','U','A','R','T','.','.','.',0};

    pFont = (void*) &FONTDEFAULT;
    SetFont(pFont);
    textHeight = GetTextHeight(pFont);

    // check if the CRC matches the data stored in the external flash memory
    expectedCRC.mchpCRCData = GRC_CRC32_EXTERNAL_MARKER;
    expectedCRC.mchpSignature = 0x5048434D;                // this is "MCHP"

    // check if programming is prompted     
    if(btnS2 == HW_BUTTON_PRESS)
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
        SetColor(WHITE);
        ClearDevice();
        SetColor(BLACK);
        OutTextXY(10,10                 , pStr);
        OutTextXY(10,10 + (textHeight*2), msgStr3);
        OutTextXY(10,10 + (textHeight*3), msgStr4);
        OutTextXY(10,10 + (textHeight*4), msgStr5);
        OutTextXY(10,10 + (textHeight*5), msgStr6);
        OutTextXY(10,10 + (textHeight*6), msgStr7);

#ifndef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
	#if defined( ONE_CYCLE_DRAWING	) 
		// The screen drawing starts and completes during one while(1) cycle loop in main().
		// This Demo is one cycle drawing application.
        if ( GFX_DRIVER_IsUpdateRequested() || (g_UPDATE_FLAGS == GFX_UPDATE_AS_IT_DRAWS) )
        {
	    	GFX_DRIVER_UpdateEpd( g_UPDATE_FLAGS, 0, 0, GetMaxX(), GetMaxY() ); 
	    	g_UPDATE_FLAGS = GFX_UPDATE_NO_FLASH | GFX_WAIT_IMAGE_DISPLAYED;   
	    }
	#else
		// This way can be used when drawing may take few or more cycles of while(1) loop in main().
		// See "tick.c" file for details.
        if ( g_UpdateNow || (g_UPDATE_FLAGS == GFX_UPDATE_AS_IT_DRAWS) )
        {
	        g_UpdateNow = 0;
	    	GFX_DRIVER_UpdateEpd( g_UPDATE_FLAGS, 0, 0, GetMaxX(), GetMaxY() ); 
	    	g_UPDATE_FLAGS = GFX_UPDATE_NO_FLASH | GFX_WAIT_IMAGE_DISPLAYED;   
	    }	
	#endif
#endif		 
        // Call the external flash programming routine
        ProgramFlash();

        // check if UART is still busy sending replies to the host
        while(U2STAbits.TRMT);
        DelayMs(10);

        // Force Reset to force the checking of the flash memory if programming was a success
        Reset();
    }
}	
