/********************************************************************
 FileName:     	HardwareProfile.h
 Dependencies:	See INCLUDES section
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board, 
 				PIC18F87J50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing this
 				file (HardwareProfile.h).
 Complier:  	Microchip C18 (for PIC18) or C30 (for PIC24)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
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

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style

********************************************************************/

#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

#if defined(__PIC32MX__)
    #include <p32xxxx.h>
    #include <plib.h>
    #include <uart2.h>
#endif

#define IOPORT_BIT_15                       (1 << 15)
#define IOPORT_BIT_14                       (1 << 14)
#define IOPORT_BIT_13                       (1 << 13)
#define IOPORT_BIT_12                       (1 << 12)
#define IOPORT_BIT_11                       (1 << 11)
#define IOPORT_BIT_10                       (1 << 10)
#define IOPORT_BIT_9                        (1 << 9)
#define IOPORT_BIT_8                        (1 << 8)
#define IOPORT_BIT_7                        (1 << 7)
#define IOPORT_BIT_6                        (1 << 6)
#define IOPORT_BIT_5                        (1 << 5)
#define IOPORT_BIT_4                        (1 << 4)
#define IOPORT_BIT_3                        (1 << 3)
#define IOPORT_BIT_2                        (1 << 2)
#define IOPORT_BIT_1                        (1 << 1)
#define IOPORT_BIT_0                        (1 << 0)

// Define the baud rate constants
#define BAUDRATE2       115200//57600 //19200
#define BRG_DIV2        4 //16
#define BRGH2           1 //0

 #define mLED_9              LATAbits.LATA6
 #define mLED_10             LATAbits.LATA7
 #define mLED_9_Off()        mLED_9  = 0;
 #define mLED_10_Off()       mLED_10 = 0;

#if defined (__C30__) || defined __XC16__
    // Various clock values
    #define GetSystemClock()            32000000UL
    #define GetPeripheralClock()        (GetSystemClock())
    #define GetInstructionClock()       (GetSystemClock()/2)
#elif defined( __PIC32MX__)
    #define USB_A0_SILICON_WORK_AROUND
    //#define RUN_AT_48MHZ
    //#define RUN_AT_24MHZ
    #define RUN_AT_60MHZ

    // Various clock values
    #if defined(RUN_AT_48MHZ)
        #define GetSystemClock()            48000000UL
        #define GetPeripheralClock()        48000000UL
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #elif defined(RUN_AT_24MHZ)
        #define GetSystemClock()            24000000UL
        #define GetPeripheralClock()        24000000UL
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #elif defined(RUN_AT_60MHZ)    
        #define GetSystemClock()            60000000UL
        #define GetPeripheralClock()        60000000UL  // Will be divided down
        #define GetInstructionClock()       GetSystemClock()
    #else
        #error Choose a speed
    #endif        
#endif


#if !defined(DEMO_BOARD)
    #if defined(__C30__) || defined __XC16__
        #if defined(__PIC24FJ256GB110__)
            #define DEMO_BOARD PIC24FJ256GB110_PIM
            #define EXPLORER_16
            #define PIC24FJ256GB110_PIM
            #define CLOCK_FREQ 32000000
        #endif
    #endif

    #if defined(__PIC32MX__)
            #define DEMO_BOARD PIC32MX460512L_PIM
            #define EXPLORER_16
            #define PIC32MX460512L_PIM
            #define CLOCK_FREQ 60000000UL
             // Clock values
            #define MILLISECONDS_PER_TICK       10                  // -0.000% error
            #define TIMER_PRESCALER             TIMER_PRESCALER_8   // At 60MHz
            #define TIMER_PERIOD                37500               // At 60MHz
            #define FCY_SPEED                   GetSystemClock()
    #endif

    #if defined(__18CXX)
        #if defined(__18F4550)
            #define DEMO_BOARD PICDEM_FS_USB
            #define PICDEM_FS_USB
            #define CLOCK_FREQ 48000000
        #elif defined(__18F87J50)
            #define DEMO_BOARD PIC18F87J50_PIM
            #define PIC18F87J50_PIM
            #define CLOCK_FREQ 48000000
        #elif defined(__18F14K50)
            #define DEMO_BOARD PIC18F14K50_DEMO_BOARD
            #define PIC18F14K50_DEMO_BOARD
            #define CLOCK_FREQ 48000000
        #endif
    #endif
#endif

#if !defined(DEMO_BOARD)
    #error "Demo board not defined.  Either define DEMO_BOARD for a custom board or select the correct processor for the demo board."
#endif

//#define ENABLE_CONSOLE

/** TRIS ***********************************************************/
#define INPUT_PIN           1
#define OUTPUT_PIN          0

/** USB ************************************************************/

#if defined(PIC24FJ256GB110_PIM)
	//#define USE_SELF_POWER_SENSE_IO
	//#define USE_USB_BUS_SENSE_IO

    #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input

    #define USB_BUS_SENSE    USBOTGGetSessionStatus()

    #define tris_self_power     TRISAbits.TRISA2    // Input

    #define self_power          1

    #define VBUS_Off            LATGbits.LATG12 = 0

    #define VBUS_On             LATGbits.LATG12 = 1

    #define VBUS_Status      PORTGbits.RG12      

    #define PGOOD                  PORTGbits.RG13
    
    /** LED ************************************************************/
    
    #define mLED_1              LATAbits.LATA0
    #define mLED_2              LATAbits.LATA1
       
    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_2_On()         mLED_2 = 1;
   
    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_2_Off()        mLED_2 = 0;
    
    
    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    #define mLED_2_Toggle()     mLED_2 = !mLED_2;
  
    
    /** SWITCH *********************************************************/
    #define mInitSwitch2()      TRISDbits.TRISD6=1;
    #define mInitSwitch3()      TRISDbits.TRISD7=1;
    #define mInitAllSwitches()  mInitSwitch2();mInitSwitch3();
    #define sw2                 PORTDbits.RD6
    #define sw3                 PORTDbits.RD7
    
    /** POT ************************************************************/
    #define mInitPOT()  {AD1PCFGLbits.PCFG5 = 0;    AD1CON2bits.VCFG = 0x0;    AD1CON3bits.ADCS = 0xFF;    AD1CON1bits.SSRC = 0x0;    AD1CON3bits.SAMC = 0b10000;    AD1CON1bits.FORM = 0b00;    AD1CON2bits.SMPI = 0x0;    AD1CON1bits.ADON = 1;}
    
    /** SPI : Chip Select Lines ****************************************/
    #define tris_cs_temp_sensor TRISBbits.TRISB2    // Output
    #define cs_temp_sensor      LATBbits.LATB2
    
    #define tris_cs_sdmmc       TRISBbits.TRISB3    // Output
    #define cs_sdmmc            LATBbits.LATB3
    
    /** SD/MMC *********************************************************/
    #define TRIS_CARD_DETECT    TRISBbits.TRISB4    // Input
    #define CARD_DETECT         PORTBbits.RB4
    
    #define TRIS_WRITE_DETECT   TRISAbits.TRISA4    // Input
    #define WRITE_DETECT        PORTAbits.RA4

     /** LED ************************************************************/

    #define mInitAllLEDs()  {TRISA = 0; LATA = 0;}
    
    #define mLED_3              LATAbits.LATA0
    #define mLED_4              LATAbits.LATA1
    #define mLED_5              LATAbits.LATA2
    #define mLED_6              LATAbits.LATA3
    #define mLED_7              LATAbits.LATA4
    #define mLED_8              LATAbits.LATA5
    #define mLED_9              LATAbits.LATA6
    #define mLED_10             LATAbits.LATA7

#elif defined(PIC32MX460512L_PIM)
    #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input

    #define USB_BUS_SENSE    USBOTGGetSessionStatus()

    #define tris_self_power     TRISAbits.TRISA2    // Input

    #define self_power          1

    #define VBUS_Off            U1OTGCONbits.VBUSON = 0

    #define VBUS_On             U1OTGCONbits.VBUSON = 1

    #define VBUS_Status      U1OTGCONbits.VBUSON     

    #define PGOOD                  PORTBbits.RB0//PORTGbits.RG13

    #define mInitAllLEDs()  {TRISA = 0; LATA = 0;}
    
    /** LED ************************************************************/    
    #define mLED_1              LATAbits.LATA6
    #define mLED_2              LATAbits.LATA7
    #define mLED_3              LATAbits.LATA0
    #define mLED_4              LATAbits.LATA1
    #define mLED_5              LATAbits.LATA2
    #define mLED_6              LATAbits.LATA3
    #define mLED_7              LATAbits.LATA4
    #define mLED_8              LATAbits.LATA5
    #define mLED_9              LATAbits.LATA6
    #define mLED_10            LATAbits.LATA7
    
    
    /** SWITCH *********************************************************/
    #define mInitSwitch2()      TRISDbits.TRISD6=1;
    #define mInitSwitch3()      TRISDbits.TRISD7=1;
    #define mInitAllSwitches()  mInitSwitch2();mInitSwitch3();
    #define sw2                 PORTDbits.RD6
    #define sw3                 PORTDbits.RD7
    
    /** POT ************************************************************/
    #define mInitPOT()  {AD1PCFGbits.PCFG5 = 0;    AD1CON2bits.VCFG = 0x0;    AD1CON3bits.ADCS = 0xFF;    AD1CON1bits.SSRC = 0x0;    AD1CON3bits.SAMC = 0x10;    AD1CON1bits.FORM = 0x0;    AD1CON2bits.SMPI = 0x0;    AD1CON1bits.ADON = 1;}

    
    /** SPI : Chip Select Lines ****************************************/
    #define tris_cs_temp_sensor TRISBbits.TRISB2    // Output
    #define cs_temp_sensor      LATBbits.LATB2
    
    #define tris_cs_sdmmc       TRISBbits.TRISB3    // Output
    #define cs_sdmmc            LATBbits.LATB3
    
    /** SD/MMC *********************************************************/
    #define TRIS_CARD_DETECT    TRISBbits.TRISB4    // Input
    #define CARD_DETECT         PORTBbits.RB4
    
    #define TRIS_WRITE_DETECT   TRISAbits.TRISA4    // Input
    #define WRITE_DETECT        PORTAbits.RA4

#endif
    

    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_2_On()         mLED_2 = 1; 
    #define mLED_3_On()         mLED_3  = 1;
    #define mLED_4_On()         mLED_4  = 1;
    #define mLED_5_On()         mLED_5  = 1;
    #define mLED_6_On()         mLED_6  = 1;
    #define mLED_7_On()         mLED_7  = 1;
    #define mLED_8_On()         mLED_8  = 1;
    #define mLED_9_On()         mLED_9  = 1;
    #define mLED_10_On()        mLED_10 = 1;

    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_2_Off()        mLED_2 = 0;
    #define mLED_3_Off()        mLED_3  = 0;
    #define mLED_4_Off()        mLED_4  = 0;
    #define mLED_5_Off()        mLED_5  = 0;
    #define mLED_6_Off()        mLED_6  = 0;
    #define mLED_7_Off()        mLED_7  = 0;
    #define mLED_8_Off()        mLED_8  = 0;
    #define mLED_9_Off()        mLED_9  = 0;
    #define mLED_10_Off()       mLED_10 = 0;

    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    #define mLED_2_Toggle()     mLED_2 = !mLED_2;
    #define mLED_3_Toggle()     mLED_3  = !mLED_3;
    #define mLED_4_Toggle()     mLED_4  = !mLED_4;
    #define mLED_5_Toggle()     mLED_5  = !mLED_5;
    #define mLED_6_Toggle()     mLED_6  = !mLED_6;
    #define mLED_7_Toggle()     mLED_7  = !mLED_7;
    #define mLED_8_Toggle()     mLED_8  = !mLED_8;
    #define mLED_9_Toggle()     mLED_9  = !mLED_9;
    #define mLED_10_Toggle()    mLED_10 = !mLED_10;


	// LCD Module I/O pins
	#define LCD_DATA0_TRIS		(TRISEbits.TRISE0)		// Multiplexed with LED6
	#define LCD_DATA0_IO		(LATEbits.LATE0)
	#define LCD_DATA1_TRIS		(TRISEbits.TRISE1)
	#define LCD_DATA1_IO		(LATEbits.LATE1)
	#define LCD_DATA2_TRIS		(TRISEbits.TRISE2)
	#define LCD_DATA2_IO		(LATEbits.LATE2)
	#define LCD_DATA3_TRIS		(TRISEbits.TRISE3)		// Multiplexed with LED3
	#define LCD_DATA3_IO		(LATEbits.LATE3)
	#define LCD_DATA4_TRIS		(TRISEbits.TRISE4)		// Multiplexed with LED2
	#define LCD_DATA4_IO		(LATEbits.LATE4)
	#define LCD_DATA5_TRIS		(TRISEbits.TRISE5)
	#define LCD_DATA5_IO		(LATEbits.LATE5)
	#define LCD_DATA6_TRIS		(TRISEbits.TRISE6)
	#define LCD_DATA6_IO		(LATEbits.LATE6)
	#define LCD_DATA7_TRIS		(TRISEbits.TRISE7)
	#define LCD_DATA7_IO		(LATEbits.LATE7)
	#define LCD_RD_WR_TRIS		(TRISDbits.TRISD5)
	#define LCD_RD_WR_IO		(LATDbits.LATD5)
	#define LCD_RS_TRIS			(TRISBbits.TRISB15)
	#define LCD_RS_IO			(LATBbits.LATB15)
	#define LCD_E_TRIS			(TRISDbits.TRISD4)
	#define LCD_E_IO			(LATDbits.LATD4)

   

	
#endif  //HARDWARE_PROFILE_H
