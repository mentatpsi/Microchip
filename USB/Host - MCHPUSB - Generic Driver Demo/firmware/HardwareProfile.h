// HardwareProfile.h

#ifndef _HARDWAREPROFILE_H_
#define _HARDWAREPROFILE_H_

#if defined( __PIC32MX__)
#endif

//#define USE_USB_PLL

#if defined (__C30__) || defined __XC16__
    #if defined (__dsPIC33EP512MU810__)||defined(__PIC24EP512GU810__)
        #define GetSystemClock()            40000000UL
        #define GetPeripheralClock()        (GetSystemClock())
        #define GetInstructionClock()       (GetSystemClock() / 2)
    #else
    // Various clock values
        #define GetSystemClock()            32000000UL
        #define GetPeripheralClock()        (GetSystemClock())
        #define GetInstructionClock()       (GetSystemClock() / 2)
    #endif
#elif defined( __PIC32MX__)
    //#define USB_A0_SILICON_WORK_AROUND
    //#define RUN_AT_48MHZ
    //#define RUN_AT_24MHZ
    #define RUN_AT_60MHZ

    // Various clock values
    #if defined(RUN_AT_48MHZ)
        #define GetSystemClock()            48000000UL
        #define GetPeripheralClock()        48000000UL
        #define GetInstructionClock()       (GetSystemClock() / 2) 
    #elif defined(RUN_AT_24MHZ)
        #define GetSystemClock()            24000000UL
        #define GetPeripheralClock()        24000000UL
        #define GetInstructionClock()       (GetSystemClock() / 2) 
    #elif defined(RUN_AT_60MHZ)    
        #define GetSystemClock()            60000000UL
        #define GetPeripheralClock()        60000000UL  // Will be divided down
        #define GetInstructionClock()       (GetSystemClock() / 2) 
    #else
        #error Choose a speed
    #endif        
#endif

// Define the baud rate constants
#define BAUDRATE2       57600 //19200
#define BRG_DIV2        4 //16
#define BRGH2           1 //0

#define DEMO_TIMEOUT_LIMIT  0xF000

#if defined(__PIC24F__) || defined(__PIC24H__)
    #include <p24fxxxx.h>
    #include <uart2.h>
#elif defined (__dsPIC33EP512MU810__)
#elif defined(__dsPIC33EP512MU810__)
    #include <p33Exxxx.h>
    #include <p33Exxxx.h>
    #include <uart2.h>
#elif defined (__PIC24EP512GU810__)
    #include <p24Exxxx.h>
    #include <uart2.h>
#else
    #include <p32xxxx.h>
    #include <plib.h>
    #include <uart2.h>
#endif


/** TRIS ***********************************************************/
#define INPUT_PIN           1
#define OUTPUT_PIN          0


#if defined (__C30__) || defined __XC16__
    #if defined(__PIC24FJ256GB110__) || defined (__dsPIC33EP512MU810__) || defined (__PIC24EP512GU810__) ||defined(__PIC24FJ256GB210__)
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

        #define sw3_bit             IOPORT_BIT_6
        #define sw3_port            PORTD
        #define sw6_bit             IOPORT_BIT_7
        #define sw6_port            PORTD

    #elif defined(__PIC24FJ64GB004__)
    	// LCD Module I/O pins
    	#define LCD_DATA0_TRIS		(TRISBbits.TRISB0)		// Multiplexed with LED6
    	#define LCD_DATA0_IO		(LATBbits.LATB0)
    	#define LCD_DATA1_TRIS		(TRISBbits.TRISB1)
    	#define LCD_DATA1_IO		(LATBbits.LATB1)
    	#define LCD_DATA2_TRIS		(TRISBbits.TRISB2)
    	#define LCD_DATA2_IO		(LATBbits.LATB2)
    	#define LCD_DATA3_TRIS		(TRISBbits.TRISB9)		// Multiplexed with LED3
    	#define LCD_DATA3_IO		(LATBbits.LATB9)
    	#define LCD_DATA4_TRIS		(TRISBbits.TRISB8)		// Multiplexed with LED2
    	#define LCD_DATA4_IO		(LATBbits.LATB8)
    	#define LCD_DATA5_TRIS		(TRISBbits.TRISB7)
    	#define LCD_DATA5_IO		(LATBbits.LATB7)
    	#define LCD_DATA6_TRIS		(TRISAbits.TRISA1)
    	#define LCD_DATA6_IO		(LATAbits.LATA1)
    	#define LCD_DATA7_TRIS		(TRISAbits.TRISA0)
    	#define LCD_DATA7_IO		(LATAbits.LATA0)
    	#define LCD_RD_WR_TRIS		(TRISBbits.TRISB13)
    	#define LCD_RD_WR_IO		(LATBbits.LATB13)
    	#define LCD_RS_TRIS			(TRISCbits.TRISC7)
    	#define LCD_RS_IO			(LATCbits.LATC7)
    	#define LCD_E_TRIS			(TRISBbits.TRISB3)
    	#define LCD_E_IO			(LATBbits.LATB3)
    
        /** LED ************************************************************/
        #define mInitAllLEDs()      //LATA &= 0xFD7F; TRISA &= 0xFD7F; LATB &= 0xFFF3; TRISB &= 0xFFF3;
        
        #define mLED_5              LATAbits.LATA7
        #define mLED_6              LATBbits.LATB3
        #define mLED_9              LATBbits.LATB2
        #define mLED_10             LATAbits.LATA9

        #define sw3_bit             IOPORT_BIT_10
        #define sw3_port            PORTA
        #define sw6_bit             IOPORT_BIT_9
        #define sw6_port            PORTA
    #endif
    
    #define mLED_5_On()         mLED_5  = 1;
    #define mLED_6_On()         mLED_6  = 1;
    #define mLED_9_On()         mLED_9  = 1;
    #define mLED_10_On()        mLED_10 = 1;
    

    #define mLED_5_Off()        mLED_5  = 0;
    #define mLED_6_Off()        mLED_6  = 0;
    #define mLED_9_Off()        mLED_9  = 0;
    #define mLED_10_Off()       mLED_10 = 0;
    
    #define mLED_5_Toggle()     mLED_5  = !mLED_5;
    #define mLED_6_Toggle()     mLED_6  = !mLED_6;
    #define mLED_9_Toggle()     mLED_9  = !mLED_9;
    #define mLED_10_Toggle()    mLED_10 = !mLED_10;
    
#elif defined(__PIC32MX__)
    #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
//    
//    #if defined(USE_USB_BUS_SENSE_IO)
//    #define USB_BUS_SENSE       PORTBbits.RB5
//    #else
    #define USB_BUS_SENSE       1
//    #endif
//    
    #define tris_self_power     TRISAbits.TRISA2    // Input
//    
//    #if defined(USE_SELF_POWER_SENSE_IO)
//    #define self_power          PORTAbits.RA2
//    #else
    #define self_power          1
//    #endif
    
    // External Transceiver Interface
    #define tris_usb_vpo        TRISBbits.TRISB3    // Output
    #define tris_usb_vmo        TRISBbits.TRISB2    // Output
    #define tris_usb_rcv        TRISAbits.TRISA4    // Input
    #define tris_usb_vp         TRISCbits.TRISC5    // Input
    #define tris_usb_vm         TRISCbits.TRISC4    // Input
    #define tris_usb_oe         TRISCbits.TRISC1    // Output
    
    #define tris_usb_suspnd     TRISAbits.TRISA3    // Output
    
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
    
    #define mLED_3_On()         mLED_3  = 1;
    #define mLED_4_On()         mLED_4  = 1;
    #define mLED_5_On()         mLED_5  = 1;
    #define mLED_6_On()         mLED_6  = 1;
    #define mLED_7_On()         mLED_7  = 1;
    #define mLED_8_On()         mLED_8  = 1;
    #define mLED_9_On()         mLED_9  = 1;
    #define mLED_10_On()        mLED_10 = 1;
    
    #define mLED_3_Off()        mLED_3  = 0;
    #define mLED_4_Off()        mLED_4  = 0;
    #define mLED_5_Off()        mLED_5  = 0;
    #define mLED_6_Off()        mLED_6  = 0;
    #define mLED_7_Off()        mLED_7  = 0;
    #define mLED_8_Off()        mLED_8  = 0;
    #define mLED_9_Off()        mLED_9  = 0;
    #define mLED_10_Off()       mLED_10 = 0;
    
    #define mLED_3_Toggle()     mLED_3  = !mLED_3;
    #define mLED_4_Toggle()     mLED_4  = !mLED_4;
    #define mLED_5_Toggle()     mLED_5  = !mLED_5;
    #define mLED_6_Toggle()     mLED_6  = !mLED_6;
    #define mLED_7_Toggle()     mLED_7  = !mLED_7;
    #define mLED_8_Toggle()     mLED_8  = !mLED_8;
    #define mLED_9_Toggle()     mLED_9  = !mLED_9;
    #define mLED_10_Toggle()    mLED_10 = !mLED_10;
    
    /** SWITCH *********************************************************/
    #define mInitAllSwitches()  mPORTDSetPinsDigitalIn(IOPORT_BIT_6|IOPORT_BIT_7)
    #define mInitSwitch3()      mPORTDSetPinsDigitalIn(IOPORT_BIT_7)
    #define mInitSwitch6()      mPORTDSetPinsDigitalIn(IOPORT_BIT_6)
    #define sw3                 mPORTDReadBit(IOPORT_BIT_6)
    #define sw6                 mPORTDReadBit(IOPORT_BIT_7)

    #define sw3_bit             IOPORT_BIT_6
    #define sw3_port            PORTD
    #define sw6_bit             IOPORT_BIT_7
    #define sw6_port            PORTD

    /** POT ************************************************************/
    #define mInitPOT()          TRISAbits.TRISA0=1;AD1CON0=0x01;AD1CON2=0x3C;
    
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

#endif

#endif  

