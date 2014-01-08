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
 *****************************************************************************/
/*****************************************************************************
 * Section: Includes
 *****************************************************************************/
#include "MainDemo.h"
#include "Graphics/AnalogClock.h"
#include "gfx_schemes.h"
#include "gfx_strings.h"
#include "gfx_screens.h"
#include "gol_ids.h"
#include "timer_tick.h"
#include "FlashProgrammer.h"
#include "TouchScreen.h"
#include "JPEGImage.h"

/*****************************************************************************
 * Section: Configuration bits
 *****************************************************************************/
#if defined(__32MX360F512L__)
#pragma config FPLLODIV = DIV_2, FPLLMUL = MUL_20, FPLLIDIV = DIV_1, FWDTEN = OFF, FCKSM = CSECME, FPBDIV = DIV_1
#pragma config OSCIOFNC = ON, POSCMOD = XT, FSOSCEN = ON, FNOSC = PRIPLL
#pragma config CP = OFF, BWP = OFF, PWP = OFF
#elif defined(__32MX460F512L__) || defined(__32MX795F512L__)
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
#endif
#if defined (__PIC24FJ256GB210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
#endif


/////////////////////////////////////////////////////////////////////////////
// SPI Settings
/////////////////////////////////////////////////////////////////////////////
#if defined (USE_SST25VF016)
    #ifdef __PIC32MX
        const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL,     1, 0, 0, 1, 1, 0};
        
        #ifdef USE_TOUCHSCREEN_AR1020
            const DRV_SPI_INIT_DATA ar1020SpiInit = {AR1020_SPI_CHANNEL,    44, 0, 0, 0, 0, 0};
        #endif
    #else    
        const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL,     3,  6, 0, 1, 1, 0};
        
        #ifdef USE_TOUCHSCREEN_AR1020
            const DRV_SPI_INIT_DATA ar1020SpiInit = {AR1020_SPI_CHANNEL,    2,  3, 0, 0, 0, 0};        
        #endif
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
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////

void  TickInit(void);
void  InitializeBoard(void);  
void  SPI1TouchInit(void);
/////////////////////////////////////////////////////////////////////////////
//                       GLOBAL VARIABLES FOR DEMO
/////////////////////////////////////////////////////////////////////////////
SCREEN_STATES   screenState = CREATE_BACKGROUND;			// current state of main demo state mashine

/////////////////////////////////////////////////////////////////////////////

//                                  MAIN
/////////////////////////////////////////////////////////////////////////////
int main(void)
{
    GOL_MSG msg;                        // GOL message structure to interact with GOL
   
    InitializeBoard();

#ifdef __PIC32MX__
    ImageDecoderInit();
    JPEGInit();                  // Initialize JPEG
#endif
    
    SetColor(WHITE);
    ClearDevice();

    GFX_SchemeInit();

    while(1)
    {
        if(GOLDraw())
        {                               // Draw GOL objects
            // Drawing is finished, we can now process new message
            TouchGetMsg(&msg);          // Get message from touch screen           
            GOLMsg(&msg);               // Process message
        }   	
    }
}

/***************************************************************
 * WORD ExternalMemoryCallback(IMAGE_EXTERNAL *memory, LONG offset, WORD nCount, void *buffer)
 ***************************************************************/
WORD ExternalMemoryCallback(IMAGE_EXTERNAL *memory, LONG offset, WORD nCount, void *buffer)
{
    // Read data requested into buffer provided
    #if defined (GFX_PICTAIL_V3) || defined (PIC24FJ256DA210_DEV_BOARD) || defined (GFX_PICTAIL_V3E)
    SST25ReadArray(memory->address + offset, // address to read from
    (BYTE *)buffer, nCount);
    #else
    SST39PMPInit();
    SST39ReadArray(memory->address + offset, // address to read from
    (BYTE *)buffer, nCount);
    LCDPMPInit();
    #endif

    return (nCount);
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
        /////////////////////////////////////////////////////////////////////////////
        // ADC Explorer 16 Development Board Errata (work around 2)
        // RB15 should be output
        /////////////////////////////////////////////////////////////////////////////
            LATBbits.LATB15 = 0;
            TRISBbits.TRISB15 = 0;

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


     //The following are PIC device specific settings for the SPI channel
     //used. 
    
     //Set IOs directions for SST25 SPI   
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

    /////////////////////////////////////////////////////////////////////////////
    //                  DRIVER SPECIFIC INITIALIZATION DATA
    /////////////////////////////////////////////////////////////////////////////

	GOLInit();                      // Initialize graphics library and create default style scheme for GOL

    // initialize GFX3 SST25 flash SPI
    SST25Init((void*) &SPI_Init_Data);                    
   
    TickInit();                     	

	
#if defined(SPI_CHANNEL_1_ENABLE) && defined(__PIC32MX__)          //SPI1 shares pins with the PMP CS. Not recommended for use
    SPI1TouchInit();

/****
 * AR1020 Touch Screen Controller
 *
 * Because the AR1020 needs to use the SPI channel to retrieve data and 
 * SPI channel 1 shares the PMP CS pin, this combination is not supported.
 * The user will receive a compile time error to avoid run time errors 
 * with this combination.
 ****/
#ifdef USE_TOUCHSCREEN_AR1020
#error "The combination of AR1020 touch screen controller and SPI 1 channel is not supported.  Please use SPI channel 2 with the AR1020 controller, or resistive touch."
#endif

#else	
    // initialize the components for Resistive Touch Screen
    TouchInit(NVMWrite, NVMRead, NVMSectorErase, TOUCH_INIT_VALUES);  
#endif	

    HardwareButtonInit();           	// Initialize the hardware buttons

}   


#if defined(SPI_CHANNEL_1_ENABLE) && defined(__PIC32MX__)          //SPI1 shares pins with the PMP CS. Not recommended for use
//////////////////////// LOCAL PROTOTYPES ////////////////////////////
extern void TouchHardwareInit(void *initValues);
extern void TouchStoreCalibration(void);
extern void	TouchLoadCalibration(void);
extern void TouchCheckForCalibration(void);
//////////////////////////////////////////////////////////////////////

void SPI1TouchInit(void)
{

    extern NVM_READ_FUNC           pCalDataRead;                  // function pointer to data read
    extern NVM_WRITE_FUNC          pCalDataWrite;                 // function pointer to data write
    extern NVM_SECTORERASE_FUNC    pCalDataSectorErase;           // function pointer to data sector erase

    TouchHardwareInit(NULL);

	// assign the addresses of the callback functions
	// if these are NULL, the TouchLoadCalibration()
	// and TouchStoreCalibration() will skip the actual
	// read and/or writes.
	pCalDataWrite = NVMWrite;
	pCalDataSectorErase = NVMSectorErase;
	pCalDataRead = NVMRead;
    
		// check if version of library is correct
		// MCHP_TOUCHSCREEN_RESISTIVE_VERSION should be defined in the hardware profile.
		if (pCalDataRead(ADDRESS_RESISTIVE_TOUCH_VERSION) !=  0xF100)
		{
            SPI1CONbits.ON=0;
			// not calibrated yet, perform the calibration 
			TouchCalibration();
            SPI1CONbits.ON=1;
			// store the calibration data
			TouchStoreCalibration();
		}
		else
		{
            SPI1CONbits.ON=0;
			// check if user wants to run calibration 
			TouchCheckForCalibration();
		}

		// load the calibration values, to check if the store worked
		TouchLoadCalibration();
        SPI1CONbits.ON=0;
}
#endif
