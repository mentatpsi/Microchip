// HardwareProfile.h

#ifndef _HARDWARE_PROFILE_H_
#define _HARDWARE_PROFILE_H_

#if defined( __C30__ ) || defined __XC16__

    // Various clock values
    #if defined (__dsPIC33EP512MU810__)||defined(__PIC24EP512GU810__)
    #define GetSystemClock()            40000000UL
    #define GetPeripheralClock()        (GetSystemClock())
    #define GetInstructionClock()       (GetSystemClock())
    #else
    #define GetSystemClock()            32000000UL
    #define GetPeripheralClock()        (GetSystemClock())
    #define GetInstructionClock()       (GetSystemClock() / 2)
    #endif

    // Clock values
    #define MILLISECONDS_PER_TICK       10
    #define TIMER_PRESCALER             TIMER_PRESCALER_8   // 8MHz: TIMER_PRESCALER_1
    #define TIMER_PERIOD                20000                // 10ms=20000, 1ms=2000

#elif defined( __PIC32MX__)

#define PIC32MX460F512L_PIM

//    #define USB_A0_SILICON_WORK_AROUND
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
        #define GetSystemClock()            80000000UL
        #define GetPeripheralClock()        80000000UL  // Will be divided down
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #else
        #error Choose a speed
    #endif        

    // Clock values
    #define MILLISECONDS_PER_TICK       10                  // -0.000% error
    #define TIMER_PRESCALER             TIMER_PRESCALER_8   // At 60MHz
    #define TIMER_PERIOD                37500               // At 60MHz

#endif


//#define USE_USB_PLL

#if defined(__PIC32MX__)
#endif


// Define the baud rate constants
#if defined(__C30__) || defined __XC16__
    #define BAUDRATE2       57600UL
    #define BRG_DIV2        4
    #define BRGH2           1
#elif defined (__PIC32MX__)
    #define BAUDRATE2       57600UL
    #define BRG_DIV2        4 
    #define BRGH2           1 
#endif

#if defined(__PIC24F__)
    #include <p24fxxxx.h>
    #include <uart2.h>
#elif defined(__PIC24H__)
    #include <p24hxxxx.h>
    #include <uart2.h>
#elif defined(__dsPIC33EP512MU810__)
    #include <p33Exxxx.h>
    #include <uart2.h>
#elif defined(__PIC24EP512GU810__)
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
    #define mInitAllLEDs()      LATA &= 0x00; TRISA &= 0xFF00;
    
    #define mLED_1              LATAbits.LATA0
    #define mLED_2              LATAbits.LATA1
    #define mLED_3              LATAbits.LATA2
    #define mLED_4              LATAbits.LATA3
    
    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_2_On()         mLED_2 = 1;
    #define mLED_3_On()         mLED_3 = 1;
    #define mLED_4_On()         mLED_4 = 1;
    
    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_2_Off()        mLED_2 = 0;
    #define mLED_3_Off()        mLED_3 = 0;
    #define mLED_4_Off()        mLED_4 = 0;
    
    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    #define mLED_2_Toggle()     mLED_2 = !mLED_2;
    #define mLED_3_Toggle()     mLED_3 = !mLED_3;
    #define mLED_4_Toggle()     mLED_4 = !mLED_4;
    
    /** SWITCH *********************************************************/
    #define mInitSwitch3()      TRISDbits.TRISD6=1;
    #define mInitSwitch6()      TRISDbits.TRISD7=1;
    #define mInitAllSwitches()  mInitSwitch3();mInitSwitch6();
    #define sw3                 PORTDbits.RD6
    #define sw6                 PORTDbits.RD7

#elif defined(__PIC32MX__)
	#ifdef PIC32MX460F512L_PIM
		#define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
//  //    
//  //    #if defined(USE_USB_BUS_SENSE_IO)
//  //    #define USB_BUS_SENSE       PORTBbits.RB5
//  //    #else
        #define USB_BUS_SENSE       1
//  //    #endif
//  //    
        #define tris_self_power     TRISAbits.TRISA2    // Input
//  //    
//  //    #if defined(USE_SELF_POWER_SENSE_IO)
//  //    #define self_power          PORTAbits.RA2
//  //    #else
        #define self_power          1
//  //    #endif
        
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
    #elif defined(PIC32_USB_STARTER_KIT)
		/** LED **************************************/
        #define LED1                        BIT_0
        #define LED2                        BIT_1
        #define LED3                        BIT_2
        
        #define mInitAllLEDs()               (mPORTDClearBits(LED1 | LED2 | LED3), \
                                             mPORTDSetPinsDigitalOut(LED1 | LED2 | LED3) )
                                            
        #define mLED_1                      PORTDbits.RD0
        #define mLED_2                      PORTDbits.RD1
        #define mLED_3                      PORTDbits.RD2
                                            
        #define mLED_1_On()                 (LATDSET = LED1)
        #define mLED_2_On()                 (LATDSET = LED2)
        #define mLED_3_On()                 (LATDSET = LED3)
                                            
        #define mLED_1_Off()                (LATDCLR = LED1)
        #define mLED_2_Off()                (LATDCLR = LED2)
        #define mLED_3_Off()                (LATDCLR = LED3)
                                            
        #define mLED_1_Toggle()             (LATDINV = LED1)
        #define mLED_2_Toggle()             (LATDINV = LED2)
        #define mLED_3_Toggle()             (LATDINV = LED3)
        
	#endif
#endif

#endif  


