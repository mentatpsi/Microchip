/*****************************************************************************
 * Microchip Graphics Library
 * Graphics Display Designer (GDD) Template
 *****************************************************************************
 * FileName:        Main.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30/C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2010 Microchip Technology Inc.  All rights reserved.
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *****************************************************************************/
#include "Main.h"
#include "GDD_Screens.h"

// Configuration bits
#if defined(__dsPIC33F__) || defined(__PIC24H__)
_FOSCSEL(FNOSC_PRI);
_FOSC(FCKSM_CSECMD &OSCIOFNC_OFF &POSCMD_XT);
_FWDT(FWDTEN_OFF);
#elif defined(__dsPIC33E__) || defined(__PIC24E__)
_FOSCSEL(FNOSC_FRC);			
_FOSC(FCKSM_CSECMD & POSCMD_XT & OSCIOFNC_OFF & IOL1WAY_OFF);
_FWDT(FWDTEN_OFF);
_FPOR(FPWRT_PWR128 & BOREN_ON & ALTI2C1_ON & ALTI2C2_ON);
_FICD(ICS_PGD1 & RSTPRI_PF & JTAGEN_OFF);
#elif defined(__PIC32MX__)
    #pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF, FCKSM = CSECME, FPBDIV = DIV_1
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
_CONFIG3( WPFP_WPFP255 & SOSCSEL_EC & WUTSEL_LEG & ALTPMP_ALTPMPEN & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
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
#elif defined (USE_M25P80)       
    #define FlashInit(pInitData) SST25Init((DRV_SPI_INIT_DATA*)pInitData)
#endif
  


/////////////////////////////////////////////////////////////////////////////
// SPI Channel settings
/////////////////////////////////////////////////////////////////////////////
#if defined (SPI_CHANNEL_1_ENABLE) || defined (SPI_CHANNEL_2_ENABLE) || defined (SPI_CHANNEL_3_ENABLE) || defined (SPI_CHANNEL_4_ENABLE)
    #if defined (USE_SST25VF016)
        #ifdef __PIC32MX
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 1, 0, 0, 1, 1, 0};
            #ifdef USE_TOUCHSCREEN_AR1020
                const DRV_SPI_INIT_DATA ar1020SpiInit = {AR1020_SPI_CHANNEL,    44, 0, 0, 0, 0, 0};
            #endif
        #else    
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
            #ifdef USE_TOUCHSCREEN_AR1020
                const DRV_SPI_INIT_DATA ar1020SpiInit = {AR1020_SPI_CHANNEL,    2,  3, 0, 0, 0, 0};        
            #endif
        #endif
    #elif defined (USE_MCHP25LC256)       
        const DRV_SPI_INIT_DATA SPI_Init_Data = {MCHP25LC256_SPI_CHANNEL, 6, 3, 0, 1, 1, 0};    
    #elif defined (USE_M25P80)
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
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
void            TickInit(void);                 // starts tick counter
void 		InitializeBoard(void);

/* */
int main(void)
{
    GOL_MSG msg;                    // GOL message structure to interact with GOL
    
    InitializeBoard();

    GDDDemoCreateFirstScreen();

    while(1)
    {
        if(GOLDraw())               // Draw GOL object
        {
            TouchGetMsg(&msg);      // Get message from touch screen
			
            #if (NUM_GDD_SCREENS > 1)
			// GDD Readme:
			// The following line of code allows a GDD user to touch the touchscreen
			// to cycle through different static screens for viewing. This is useful as a
			// quick way to view how each screen looks on the physical target hardware.
			// This line of code should eventually be commented out for actual development.
			// Also note that widget/object names can be found in GDD_Screens.h
			if(msg.uiEvent == EVENT_RELEASE) GDDDemoNextScreen();
			#endif
			
            GOLMsg(&msg);           // Process message
        }
    }//end while
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
    WORD    objectID;

    objectID = GetObjID(pObj);

    GDDDemoGOLMsgCallback(objMsg, pObj, pMsg);
    
    // Add additional code here...

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
    GDDDemoGOLDrawCallback();

    // Add additional code here...

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

    // Initialize graphics library and create default style scheme for GOL
    GOLInit();  
    
// Set the other chip selects to a known state
#ifdef MIKRO_BOARD
    // SD Card chip select
    LATGbits.LATG9 = 1;
    TRISGbits.TRISG9 = 0;
    
    // MP3 Codac
    // reset
    LATAbits.LATA5 = 0;
    TRISAbits.TRISA5 = 0;
    // chip select
    LATAbits.LATA2 = 1;
    TRISAbits.TRISA2 = 0;
    // chip select
    LATAbits.LATA3 = 1;
    TRISAbits.TRISA3 = 0;

    AD1PCFGbits.PCFG11 = 1;
    AD1PCFGbits.PCFG10 = 1;
#endif

    //The following are PIC device specific settings for the SPI channel
    //used. 
    
    //Set IOs directions for SST25 SPI
    #if defined (GFX_PICTAIL_V3) || defined (MEB_BOARD) || defined(GFX_PICTAIL_LCC) || defined(MIKRO_BOARD) || defined(GFX_PICTAIL_V3E)
	    
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
    FlashInit(&SPI_Init_Data);
   
    // initialize the timer that manages the tick counter
    TickInit(); 
                      
    // initialize the components for Resistive Touch Screen
    TouchInit(NVMWrite, NVMRead, NVMSectorErase, TOUCH_INIT_VALUES);   
                
    HardwareButtonInit();           	// Initialize the hardware buttons

}    

