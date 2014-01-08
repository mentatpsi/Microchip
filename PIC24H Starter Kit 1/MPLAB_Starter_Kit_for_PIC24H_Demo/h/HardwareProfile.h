/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		HardwareProfile.h
* Dependencies:    		Header (.h) files if applicable, see below
* Processor:       		PIC24HJ128GP504
* Compiler:        		MPLAB® C30 v3.11b or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
************************************************************************/

/******************************************************************************

HardwareProfile.h

This file defines all of the hardware-specific information for the project.

******************************************************************************/

#ifndef _HARDWARE_PROFILE_H_
#define _HARDWARE_PROFILE_H_

#define USE_GFX_PMP
#define USE_8BIT_PMP
#define GO_FAST

/*********************************************************************
* Overview: Display controller selection.
*                                          
********************************************************************/
#define USE_GFX_DISPLAY_CONTROLLER_SH1101A

/*********************************************************************
* Overview: Horizontal and vertical display resolution
*                  (from the glass datasheet).
*********************************************************************/
#define DISP_HOR_RESOLUTION				128
#define DISP_VER_RESOLUTION				64
/*********************************************************************
* Overview: Image orientation (can be 0, 90, 180, 270 degrees).
*********************************************************************/
#define DISP_ORIENTATION				0

#if defined( __C30__ ) || defined __XC16__
    #ifdef USE_FRC
        #define GetSystemClock()            	32000000UL
        #define GetPeripheralClock()        	(GetSystemClock())
        #define GetInstructionClock()       	(GetSystemClock() / 2)
        #define MILLISECONDS_PER_TICK       	10
        #define TIMER_PRESCALER             	TIMER_PRESCALER_8 
        #define TIMER_PERIOD                	20000
    #else
        #ifdef GO_FAST
            #define GetSystemClock()            79227500L
            #define GetPeripheralClock()        (GetSystemClock())
            #define GetInstructionClock()       (GetSystemClock() / 2)
            #define MILLISECONDS_PER_TICK       10
            #define TIMER_PRESCALER             TIMER_PRESCALER_8
            //#define TIMER_PERIOD                40000
        #else
            #define GetSystemClock()            32000000UL
            #define GetPeripheralClock()        (GetSystemClock())
            #define GetInstructionClock()       (GetSystemClock() / 2)
            #define MILLISECONDS_PER_TICK       10
            #define TIMER_PRESCALER             TIMER_PRESCALER_8   // 8MHz: TIMER_PRESCALER_1
            #define TIMER_PERIOD                20000               // 10ms=20000, 1ms=2000
        #endif
    #endif
#elif defined( __PIC32MX__)
    #define USB_A0_SILICON_WORK_AROUND
    //#define RUN_AT_48MHZ
    //#define RUN_AT_24MHZ
    #define RUN_AT_60MHZ
    // Clock values
    #define MILLISECONDS_PER_TICK       10                  // -0.000% error
    #define TIMER_PRESCALER             TIMER_PRESCALER_8   // At 60MHz
    #define TIMER_PERIOD                37500               // At 60MHz
#endif


//#define USE_USB_PLL

#if defined(__PIC32MX__)
    #if defined(RUN_AT_48MHZ)
        #define FCY_SPEED               48000000
        #define PERIPHERAL_BUS_SPEED    48000000
    #elif defined(RUN_AT_24MHZ)
        #define FCY_SPEED               24000000
        #define PERIPHERAL_BUS_SPEED    24000000
    #elif defined(RUN_AT_60MHZ)
        #define FCY_SPEED               60000000
        #define PERIPHERAL_BUS_SPEED    30000000    // Will be divided down
    #else
        #error Choose a speed
    #endif
#endif


// Define the baud rate constants
#if defined(__C30__) || defined __XC16__
    #define BAUDRATE2       57600UL
    #define BRG_DIV2        4
    #define BRGH2           1
#elif defined (__PIC32MX__)
    #define BAUDRATE2       115200ul
    #define BRG_DIV2        4
    #define BRGH2           1
#endif


// Define the RTCC default initialization.
#define RTCC_DEFAULT_DAY        13        // 13
#define RTCC_DEFAULT_MONTH      11        // November
#define RTCC_DEFAULT_YEAR       07        // 2007
#define RTCC_DEFAULT_WEEKDAY    02        // Tuesday
#define RTCC_DEFAULT_HOUR       10        // 10:10:01
#define RTCC_DEFAULT_MINUTE     10
#define RTCC_DEFAULT_SECOND     01

#if defined(__PIC24F__)
    #include <p24fxxxx.h>
//    #include <uart2.h>
#elif defined(__PIC24H__)
    #include <p24hxxxx.h>
//    #include <uart2.h>
#else
    #include <p32xxxx.h>
    #include <plib.h>
//    #include <uart2.h>
#endif


/** TRIS ***********************************************************/
#define INPUT_PIN           1
#define OUTPUT_PIN          0

/*********************************************************************
* BACKLIGHT CONTROL (these are dummy macros to satisfy common file gfxpmp.h)
*********************************************************************/
#define DisplayBacklightConfig()      
#define DisplayBacklightOn()        
#define DisplayBacklightOff()          

/*********************************************************************
* EPMP TIMING PARAMETERS
*********************************************************************/
#define PMP_DATA_SETUP_TIME         (0)    
#define PMP_DATA_WAIT_TIME          (45)  // minimum pulse width requirement of CS controlled RD/WR access in SH1101A is 100 ns
                                          // but since we have one cycle in setup and one cycle hold (minimum) we can estimate
                                          // the wait time. 
#define PMP_DATA_HOLD_TIME          (15)  // based on SH1101A data hold requirement  

/*********************************************************************
* IOS FOR THE DISPLAY CONTROLLER
*********************************************************************/
// Definitions for reset pin
#define DisplayResetConfig()        TRISAbits.TRISA1 = 0    
#define DisplayResetEnable()        LATAbits.LATA1 = 0
#define DisplayResetDisable()       LATAbits.LATA1 = 1

// Definitions for RS pin
#define DisplayCmdDataConfig()		TRISCbits.TRISC7 = 0;
#define DisplaySetCommand()         LATCbits.LATC7 = 0
#define DisplaySetData()            LATCbits.LATC7 = 1

// Definitions for CS pin
#define DisplayConfig()             TRISBbits.TRISB15 = 0             
#define DisplayEnable()             LATBbits.LATB15 = 0
#define DisplayDisable()            LATBbits.LATB15 = 1

#endif

