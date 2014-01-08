// HardwareProfile.h

#ifndef _HARDWARE_PROFILE_H_
#define _HARDWARE_PROFILE_H_

// Define your clock speed here

// Sample clock speed for a 16-bit processor
#if defined (__C30__) || defined __XC16__

    // Various clock values
    #define GetSystemClock()            32000000UL
    #define GetPeripheralClock()        (GetSystemClock())
    #define GetInstructionClock()       (GetSystemClock() / 2)

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
    #include <p24Fxxxx.h>
    #include <uart2.h>
#elif defined(__PIC24H__)
    #include <p24hxxxx.h>
    #include <uart2.h>
#else
    #include <p32xxxx.h>
    #include <plib.h>
    #include <uart2.h>
#endif


// Select your MDD File System interface type
// This library currently only supports a single physical interface layer

//#define USE_SD_INTERFACE_WITH_SPI       // SD-SPI.c and .h
//#define USE_CF_INTERFACE_WITH_PMP       // CF-PMP.c and .h
//#define USE_MANUAL_CF_INTERFACE         // CF-Bit transaction.c and .h
#define USE_USB_INTERFACE               // USB host MSD library



/** TRIS ***********************************************************/
#define INPUT_PIN           1
#define OUTPUT_PIN          0

#endif  

