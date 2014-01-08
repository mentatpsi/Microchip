/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_HardwareProfile_18FxxJ50.h
 *  Dependencies:    mTouch.h
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch CVD Framework Hardware Profile Header File
 *                   - Provides information about correct SFR addresses,
 *                     pin assignments, module availability, and any other
 *                     part-specific data.
 *                   - See the documentation for more information.
 *************************************************************************/
 /**************************************************************************
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software, and 
 * any derivatives created by any person or entity by or on your behalf, 
 * exclusively with Microchip's products in accordance with applicable
 * software license terms and conditions, a copy of which is provided for
 * your referencein accompanying documentation. Microchip and its licensors 
 * retain all ownership and intellectual property rights in the 
 * accompanying software and in all derivatives hereto. 
 * 
 * This software and any accompanying information is for suggestion only. 
 * It does not modify Microchip's standard warranty for its products. You 
 * agree that you are solely responsible for testing the software and 
 * determining its suitability. Microchip has no obligation to modify, 
 * test, certify, or support the software. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH 
 * MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY 
 * APPLICATION. 
 * 
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, 
 * TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT 
 * LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, 
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, 
 * FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, 
 * HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY 
 * OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS 
 * SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID 
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE. 
 * 
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF 
 * THESE TERMS. 
 *************************************************************************/
/** @file mTouch_HardwareProfile_18FxxJ50.h
* @brief Support for: PIC18F24J50, PIC18LF24J50, PIC18F25J50, PIC18LF25J50, PIC18F26J50, PIC18LF26J50, PIC18F44J50, PIC18LF44J50, PIC18F45J50, PIC18LF45J50, PIC18F46J50, PIC18LF46J50, PIC18F65J50, PIC18F66J50, PIC18F67J50, PIC18F85J50, PIC18F86J50, PIC18F87J50
*/
/// @cond
#if !defined(__MTOUCH_HARDWARE_18FxxJ50_H)
#define __MTOUCH_HARDWARE_18FxxJ50_H

#if !defined(_18F24J50) && !defined(_18LF24J50)     &&      !defined(__18F24J50) && !defined(__18LF24J50)
#if !defined(_18F25J50) && !defined(_18LF25J50)     &&      !defined(__18F25J50) && !defined(__18LF25J50)
#if !defined(_18F26J50) && !defined(_18LF26J50)     &&      !defined(__18F26J50) && !defined(__18LF26J50)
#if !defined(_18F44J50) && !defined(_18LF44J50)     &&      !defined(__18F44J50) && !defined(__18LF44J50)
#if !defined(_18F45J50) && !defined(_18LF45J50)     &&      !defined(__18F45J50) && !defined(__18LF45J50)
#if !defined(_18F46J50) && !defined(_18LF46J50)     &&      !defined(__18F46J50) && !defined(__18LF46J50)
#if !defined(_18F65J50)                             &&      !defined(__18F65J50)  
#if !defined(_18F66J50)                             &&      !defined(__18F66J50) 
#if !defined(_18F67J50)                             &&      !defined(__18F67J50)
#if !defined(_18F85J50)                             &&      !defined(__18F85J50)
#if !defined(_18F86J50)                             &&      !defined(__18F86J50)
#if !defined(_18F87J50)                             &&      !defined(__18F87J50)
    #error The current mTouch hardware include file does not support this PIC microcontroller.
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

#if     defined(_18F24J50)  || defined(_18LF24J50)  || defined(_18F25J50)  || defined(_18LF25J50)  || defined(_18F26J50)  || defined(_18LF26J50)    \
    ||  defined(__18F24J50) || defined(__18LF24J50) || defined(__18F25J50) || defined(__18LF25J50) || defined(__18F26J50) || defined(__18LF26J50)
    #if MTOUCH_NUMBER_SENSORS > 10
        #error The PIC18F2xJ50 is not able to support more than 10 sensors due to pinout limitations.
    #endif
#endif
#if     defined(_18F44J50)  || defined(_18LF44J50)  || defined(_18F45J50)  || defined(_18LF45J50)  || defined(_18F46J50)  || defined(_18LF46J50)    \
    ||  defined(__18F44J50) || defined(__18LF44J50) || defined(__18F45J50) || defined(__18LF45J50) || defined(__18F46J50) || defined(__18LF46J50)
    #if MTOUCH_NUMBER_SENSORS > 13
        #error The PIC18F4xJ50 is not able to support more than 13 sensors due to pinout limitations.
    #endif
#endif
#if     defined(_18F65J50)  || defined(_18F66J50)  || defined(_18F67J50)  \
    ||  defined(__18F65J50) || defined(__18F66J50) || defined(__18F67J50)
    #if MTOUCH_NUMBER_SENSORS > 8
        #error The PIC18F6xJ50 is not able to support more than 8 sensors due to pinout limitations.
    #endif
#endif
#if     defined(_18F85J50)  || defined(_18F86J50)  || defined(_18F87J50)  \
    ||  defined(__18F85J50) || defined(__18F86J50) || defined(__18F87J50)
    #if MTOUCH_NUMBER_SENSORS > 12
        #error The PIC18F4xJ50 is not able to support more than 12 sensors due to pinout limitations.
    #endif
#endif

//===========================================================
//  Device Configuration
//===========================================================


//===========================================================
//  Communications Configuration
//===========================================================
#define PIC_UART_AVAILABLE      
#define PIC_UART_BAUD_BITS                  16



// #if (MTOUCH_COMM_TYPE == MTOUCH_COMM_RS232_1WAY_ASCII) && !defined(MTOUCH_COMM_ASCII_PIC18TXREG)
    // #error The PIC18FxxJ50 must be told which pin to use as the TX2 output. In the mTouch configuration file, please define a new value "MTOUCH_COMM_ASCII_PIC18TXREG" with the Peripheral Pin Select Output Register SFR name in which to load the 'TX2' value. Ex:  #define MTOUCH_COMM_ASCII_PIC18TXREG  RPOR2
// #endif

// #define MTOUCH_PIC18_UNLOCK_PPS()   EECON2      = 0x55; \
                                    // EECON2      = 0xAA; \
                                    // PPSCONbits.IOLOCK = 0;

// #define MTOUCH_PIC18_LOCK_PPS()     EECON2      = 0x55; \
                                    // EECON2      = 0xAA; \
                                    // PPSCONbits.IOLOCK = 1;

// #define MTOUCH_PIC18_PPS_TX2        5

// #if (MTOUCH_COMM_TYPE == MTOUCH_COMM_RS232_1WAY_ASCII)
    // #if (_XTAL_FREQ == 8000000)
        // #define MTOUCH_DEBUG_COMM_INIT()                        \
            // do {                                                \
                // MTOUCH_PIC18_UNLOCK_PPS();                      \
                // MTOUCH_COMM_ASCII_PIC18TXREG = MTOUCH_PIC18_PPS_TX2;    \
                // MTOUCH_PIC18_LOCK_PPS();                        \
                // TXSTA2bits.BRGH = 1;                            \
                // SPBRG2          = 51;                           \
                // TXSTA2bits.TXEN = 1;                            \
                // RCSTA2bits.SPEN = 1;                            \
            // } while (0)
    // #elif (_XTAL_FREQ == 4000000)
        // #define MTOUCH_DEBUG_COMM_INIT()                        \
            // do {                                                \
                // MTOUCH_PIC18_UNLOCK_PPS();                      \
                // MTOUCH_COMM_ASCII_PIC18TXREG = MTOUCH_PIC18_PPS_TX2;    \
                // MTOUCH_PIC18_LOCK_PPS();                        \
                // TXSTA2bits.BRGH = 1;                            \
                // SPBRG2          = 25;                           \
                // TXSTA2bits.TXEN = 1;                            \
                // RCSTA2bits.SPEN = 1;                            \
            // } while (0)
    // #else
        // #error The mTouch Framework is only designed to run communications at 8 or 4 MHz. Please either initialize the UART manually or choose one of these Fosc values.
    // #endif
// #endif
    
//===========================================================
//  Interrupt Control Macros
//===========================================================
#define PIC_TIMER0_AVAILABLE
#define PIC_TIMER2_AVAILABLE
#define PIC_TIMER4_AVAILABLE


//===========================================================
//  ADC Configuration
//===========================================================
#define PIC_ADC_BITS                10
#define PIC_ADC_UNIMP_AVAILABLE

#define PIC_ADC_ACQT                ADCON1bits.ACQT
#define PIC_ADC_ADCS                ADCON1bits.ADCS
#define PIC_ADC_ADFM                ADCON1bits.ADFM
#define PIC_ADC_ADON                ADCON0bits.ADON

#define PIC_ADC_ADCS_FOSC2          0b000
#define PIC_ADC_ADCS_FOSC4          0b100
#define PIC_ADC_ADCS_FOSC8          0b001
#define PIC_ADC_ADCS_FOSC16         0b101
#define PIC_ADC_ADCS_FOSC32         0b010
#define PIC_ADC_ADCS_FOSC64         0b110
#define PIC_ADC_ADCS_FRC            0b111

#define PIC_ADC_ADFM_RIGHT          1
#define PIC_ADC_ADFM_LEFT           0

#if _XTAL_FREQ < 2860000
    #define PIC_ADC_SETCLK()    PIC_ADC_ACQT = 0; PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC2
    #define PIC_ADC_TAD         1
#elif _XTAL_FREQ < 5710000
    #define PIC_ADC_SETCLK()    PIC_ADC_ACQT = 0; PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC4
    #define PIC_ADC_TAD         1
#elif _XTAL_FREQ < 11430000
    #define PIC_ADC_SETCLK()    PIC_ADC_ACQT = 0; PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC8
    #define PIC_ADC_TAD         1
#elif _XTAL_FREQ < 22860000
    #define PIC_ADC_SETCLK()    PIC_ADC_ACQT = 0; PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC16
    #define PIC_ADC_TAD         1
#elif _XTAL_FREQ < 45710000
    #define PIC_ADC_SETCLK()    PIC_ADC_ACQT = 0; PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC32
    #define PIC_ADC_TAD         1
#elif _XTAL_FREQ < 48000000
    #define PIC_ADC_SETCLK()    PIC_ADC_ACQT = 0; PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC64
    #define PIC_ADC_TAD         1
#else
    #error No predefined ADC clock values for the chosen _XTAL_FREQ.
#endif



//===========================================================
//  DAC Configuration
//===========================================================
// NOT AVAILABLE

//===========================================================
//  Software-enabled Watchdog Timer Configuration
//===========================================================
#define PIC_SWDTEN_AVAILABLE
#define PIC_SWDTEN_OFF()                WDTCONbits.SWDTEN = 0
#define PIC_SWDTEN_ON()                 WDTCONbits.SWDTEN = 1


//===========================================================
//  Analog Port Mapping
//  - Right Justification
//  - VDD/VSS Reference
//  - GO/DONE cleared
//  - ADON enabled
//===========================================================
#if defined(_18F24J50)  || defined(_18LF24J50)  || \
    defined(_18F25J50)  || defined(_18LF25J50)  || \
    defined(_18F26J50)  || defined(_18LF26J50)  || \
    defined(_18F44J50)  || defined(_18LF44J50)  || \
    defined(_18F45J50)  || defined(_18LF45J50)  || \
    defined(_18F46J50)  || defined(_18LF46J50)  || \
    defined(__18F24J50) || defined(__18LF24J50) || \
    defined(__18F25J50) || defined(__18LF25J50) || \
    defined(__18F26J50) || defined(__18LF26J50) || \
    defined(__18F44J50) || defined(__18LF44J50) || \
    defined(__18F45J50) || defined(__18LF45J50) || \
    defined(__18F46J50) || defined(__18LF46J50)
    
    #define MTOUCH_AD_AN0           0x01
    #define MTOUCH_AD_AN1           0x05
    #define MTOUCH_AD_AN2           0x09
    #define MTOUCH_AD_AN3           0x0D
    #define MTOUCH_AD_AN4           0x11

    #if defined(_18F44J50)  || defined(_18LF44J50)  || \
        defined(_18F45J50)  || defined(_18LF45J50)  || \
        defined(_18F46J50)  || defined(_18LF46J50)  || \
        defined(__18F44J50) || defined(__18LF44J50) || \
        defined(__18F45J50) || defined(__18LF45J50) || \
        defined(__18F46J50) || defined(__18LF46J50) 
        
        #define MTOUCH_AD_AN5       0x15
        #define MTOUCH_AD_AN6       0x19
        #define MTOUCH_AD_AN7       0x1D
    #endif

    #define MTOUCH_AD_AN8           0x21
    #define MTOUCH_AD_AN9           0x25
    #define MTOUCH_AD_AN10          0x29
    #define MTOUCH_AD_AN11          0x2D
    #define MTOUCH_AD_AN12          0x31
    #define MTOUCH_AD_ISO_AND_GO    0x37
    #define MTOUCH_AD_ISO_NOGO      0x35
#else
    #define MTOUCH_AD_AN0           0x01
    #define MTOUCH_AD_AN1           0x05
    #define MTOUCH_AD_AN2           0x09
    #define MTOUCH_AD_AN3           0x0D
    #define MTOUCH_AD_AN4           0x11
    #define MTOUCH_AD_AN7           0x1D
    #define MTOUCH_AD_AN10          0x29
    #define MTOUCH_AD_AN11          0x2D

    #if defined(_18F85J50)  || defined(_18F86J50)  || \
        defined(_18F87J50)  || defined(__18F85J50) || \
        defined(__18F86J50) || defined(__18F87J50)
        
        #define MTOUCH_AD_AN12      0x31
        #define MTOUCH_AD_AN13      0x35
        #define MTOUCH_AD_AN14      0x39
        #define MTOUCH_AD_AN15      0x3D
    #endif

    #define MTOUCH_AD_ISO_AND_GO    0x21
    #define MTOUCH_AD_ISO_NOGO      0x23
#endif


#if defined(_18F24J50)  || defined(_18LF24J50)  || \
    defined(_18F25J50)  || defined(_18LF25J50)  || \
    defined(_18F26J50)  || defined(_18LF26J50)  || \
    defined(_18F44J50)  || defined(_18LF44J50)  || \
    defined(_18F45J50)  || defined(_18LF45J50)  || \
    defined(_18F46J50)  || defined(_18LF46J50)  || \
    defined(__18F24J50) || defined(__18LF24J50) || \
    defined(__18F25J50) || defined(__18LF25J50) || \
    defined(__18F26J50) || defined(__18LF26J50) || \
    defined(__18F44J50) || defined(__18LF44J50) || \
    defined(__18F45J50) || defined(__18LF45J50) || \
    defined(__18F46J50) || defined(__18LF46J50)
    
    #define MTOUCH_PIN_AN0      0
    #define MTOUCH_PIN_AN1      1
    #define MTOUCH_PIN_AN2      2
    #define MTOUCH_PIN_AN3      3
    #define MTOUCH_PIN_AN4      5
    #define MTOUCH_PIN_AN5      0
    #define MTOUCH_PIN_AN6      1
    #define MTOUCH_PIN_AN7      2
    #define MTOUCH_PIN_AN8      2
    #define MTOUCH_PIN_AN9      3
    #define MTOUCH_PIN_AN10     1
    #define MTOUCH_PIN_AN11     2
    #define MTOUCH_PIN_AN12     0
#else
    #define MTOUCH_PIN_AN0      0
    #define MTOUCH_PIN_AN1      1
    #define MTOUCH_PIN_AN2      2
    #define MTOUCH_PIN_AN3      3
    #define MTOUCH_PIN_AN4      5
    #define	MTOUCH_PIN_AN7      2
    #define MTOUCH_PIN_AN10     5
    #define MTOUCH_PIN_AN11     6
    #define MTOUCH_PIN_AN12     4
    #define MTOUCH_PIN_AN13     5
    #define MTOUCH_PIN_AN14     6
    #define MTOUCH_PIN_AN15     7
#endif

#if defined(_18F24J50)  || defined(_18LF24J50)  || \
    defined(_18F25J50)  || defined(_18LF25J50)  || \
    defined(_18F26J50)  || defined(_18LF26J50)  || \
    defined(_18F44J50)  || defined(_18LF44J50)  || \
    defined(_18F45J50)  || defined(_18LF45J50)  || \
    defined(_18F46J50)  || defined(_18LF46J50)  || \
    defined(__18F24J50) || defined(__18LF24J50) || \
    defined(__18F25J50) || defined(__18LF25J50) || \
    defined(__18F26J50) || defined(__18LF26J50) || \
    defined(__18F44J50) || defined(__18LF44J50) || \
    defined(__18F45J50) || defined(__18LF45J50) || \
    defined(__18F46J50) || defined(__18LF46J50)
    
    #define MTOUCH_LETTER_AN0          A
    #define MTOUCH_LETTER_AN1          A
    #define MTOUCH_LETTER_AN2          A
    #define MTOUCH_LETTER_AN3          A
    #define MTOUCH_LETTER_AN4          A
    #define MTOUCH_LETTER_AN5          E
    #define MTOUCH_LETTER_AN6          E
    #define MTOUCH_LETTER_AN7          E
    #define MTOUCH_LETTER_AN8          B
    #define MTOUCH_LETTER_AN9          B
    #define MTOUCH_LETTER_AN10         B
    #define MTOUCH_LETTER_AN11         C
    #define MTOUCH_LETTER_AN12         B
#else
    #define MTOUCH_LETTER_AN0          A
    #define MTOUCH_LETTER_AN1          A
    #define MTOUCH_LETTER_AN2          A
    #define MTOUCH_LETTER_AN3          A
    #define MTOUCH_LETTER_AN4          A
    #define MTOUCH_LETTER_AN7          F
    #define MTOUCH_LETTER_AN10         F
    #define MTOUCH_LETTER_AN11         F
    #define MTOUCH_LETTER_AN12         H
    #define MTOUCH_LETTER_AN13         H
    #define MTOUCH_LETTER_AN14         H
    #define MTOUCH_LETTER_AN15         H
#endif



#endif
/// @endcond
