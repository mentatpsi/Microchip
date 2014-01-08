// HardwareProfile.h

#ifndef _HARDWARE_PROFILE_H_
#define _HARDWARE_PROFILE_H_


// ******************* CPU Speed defintions ************************************
//  This section is required by some of the peripheral libraries and software
//  libraries in order to know what the speed of the processor is to properly
//  configure the hardware modules to run at the proper speeds
// *****************************************************************************
#if defined( __C30__ ) || defined __XC16__

    // Various clock values

    #if defined (__dsPIC33EP512MU810__)||defined(__PIC24EP512GU810__)
        #define GetSystemClock()            40000000UL
        #define GetInstructionClock()       GetSystemClock()
    #else
        #define GetSystemClock()            32000000UL
        #define GetInstructionClock()       (GetSystemClock() / 2)
    #endif
    #define GetPeripheralClock()        (GetSystemClock())

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
        #define GetPeripheralClock()        30000000UL  // Will be divided down
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #else
        #error Choose a speed
    #endif        

#endif

// ******************* MDD File System Required Definitions ********************
// Select your MDD File System interface type
// This library currently only supports a single physical interface layer
// In this example we are going to use the USB so we only need the USB definition
// *****************************************************************************
#define USE_USB_INTERFACE               // USB host MSD library


// ******************* Debugging interface hardware settings *******************
//  This section is not required by any of the libraries.  This is a
//  demo specific implmentation to assist in debugging.  
// *****************************************************************************
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
#elif defined (__dsPIC33EP512MU810__)
    #include <p33Exxxx.h>
#elif defined (__PIC24EP512GU810__)
    #include <p24Exxxx.h>
#else
    #include <p32xxxx.h>
    #include <plib.h>
    #include <uart2.h>
#endif

#endif  

