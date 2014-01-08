// HardwareProfile.h

#ifndef _HARDWARE_PROFILE_H_
#define _HARDWARE_PROFILE_H_

#if defined( __C30__ ) || defined __XC16__

    #if defined (__dsPIC33EP512MU810__)||defined(__PIC24EP512GU810__)
    
        #define GetSystemClock()            40000000UL
        #define GetPeripheralClock()        (GetSystemClock())
        #define GetInstructionClock()       (GetSystemClock())

    #else
        // Various clock values
        #define GetSystemClock()            32000000UL
        #define GetPeripheralClock()        (GetSystemClock() / 2)
        #define GetInstructionClock()       (GetSystemClock() / 2)
    
    #endif

    // Clock values
    #define MILLISECONDS_PER_TICK       10
    #define TIMER_PRESCALER             TIMER_PRESCALER_8   // 8MHz: TIMER_PRESCALER_1
    #define TIMER_PERIOD                20000                // 10ms=20000, 1ms=2000

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

#endif  

