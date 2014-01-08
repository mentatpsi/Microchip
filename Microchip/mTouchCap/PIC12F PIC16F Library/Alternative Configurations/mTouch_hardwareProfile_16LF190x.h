/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_HardwareProfile_16LF190x.h
 *  Dependencies:    mTouch.h
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework Hardware Profile Header File
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
/** @file mTouch_HardwareProfile_16LF190x.h
* @brief Support for: PIC16LF1902, PIC16LF1903, PIC16LF1904, PIC16LF1906, PIC16LF1907
*/
/// @cond
#if !defined(__MTOUCH_HARDWARE_16LF190X_H)
#define __MTOUCH_HARDWARE_16LF190X_H

#if !defined(_16LF1902)
#if !defined(_16LF1903)
#if !defined(_16LF1904)
#if !defined(_16LF1906)
#if !defined(_16LF1907)
    #error The current mTouch hardware include file does not support this PIC microcontroller.
#endif
#endif
#endif
#endif
#endif

#if defined(_16LF1902)
    #if MTOUCH_NUMBER_SENSORS > 11
        #error The PIC16LF1902 is not able to support more than 11 sensors due to pinout limitations.
    #endif
#endif
#if defined(_16LF1903)
    #if MTOUCH_NUMBER_SENSORS > 11
        #error The PIC16LF1903 is not able to support more than 11 sensors due to pinout limitations.
    #endif
#endif
#if defined(_16LF1904)
    #if MTOUCH_NUMBER_SENSORS > 14
        #error The PIC16LF1904 is not able to support more than 14 sensors due to pinout limitations.
    #endif
#endif
#if defined(_16LF1906)
    #if MTOUCH_NUMBER_SENSORS > 11
        #error The PIC16LF1906 is not able to support more than 11 sensors due to pinout limitations.
    #endif
#endif
#if defined(_16LF1907)
    #if MTOUCH_NUMBER_SENSORS > 14
        #error The PIC16LF1907 is not able to support more than 14 sensors due to pinout limitations.
    #endif
#endif

//===========================================================
//  Device Configuration
//===========================================================


//===========================================================
//  Communications Configuration
//===========================================================
#if !defined(_16LF1902) && !defined(_16LF1903)
    #define PIC_UART_AVAILABLE      
    #define PIC_UART_BAUD_BITS          16
#endif
#if defined(_16LF1902) || defined(_16LF1903)
    // NOT AVAILABLE
#endif


#define PIC_TIMER0_AVAILABLE
#define PIC_TIMER1_AVAILABLE
    
//===========================================================
//  ADC Configuration
//===========================================================
#define PIC_ADC_BITS                10
#define PIC_ADC_UNIMP_AVAILABLE

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

#if     _XTAL_FREQ == 32000000 
    #define PIC_ADC_SETCLK()    PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC32
    #define PIC_ADC_TAD         1
#elif   _XTAL_FREQ == 16000000 
    #define PIC_ADC_SETCLK()    PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC16
    #define PIC_ADC_TAD         1 
#elif   _XTAL_FREQ ==  8000000 
    #define PIC_ADC_SETCLK()    PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC8
    #define PIC_ADC_TAD         1  
#elif   _XTAL_FREQ ==  4000000 
    #define PIC_ADC_SETCLK()    PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC4
    #define PIC_ADC_TAD         1 
#elif   _XTAL_FREQ ==  2000000 
    #define PIC_ADC_SETCLK()    PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC2
    #define PIC_ADC_TAD         1
#elif   _XTAL_FREQ ==  1000000 
    #define PIC_ADC_SETCLK()    PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC2
    #define PIC_ADC_TAD         2
#else
    #error No predefined ADC clock values for the chosen _XTAL_FREQ.
#endif
  
//===========================================================
//  DAC Configuration
//===========================================================
// NOT AVAILABLE
#if MTOUCH_NUMBER_SENSORS == 1
    #error Single-sensor scanning code not currently compatible with the PIC16LF190x family due to the lack of a DAC module.
#endif

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
#define MTOUCH_AD_AN0           0x01
#define MTOUCH_AD_AN1           0x05
#define MTOUCH_AD_AN2           0x09
#define MTOUCH_AD_AN3           0x0D
#define MTOUCH_AD_AN4           0x11

#if !defined(_16LF1902)
#if !defined(_16LF1903)
#if !defined(_16LF1906)
    #define MTOUCH_AD_AN5       0x15
    #define MTOUCH_AD_AN6       0x19
    #define MTOUCH_AD_AN7       0x1D
#endif
#endif
#endif

#define MTOUCH_AD_AN8           0x21
#define MTOUCH_AD_AN9           0x25
#define MTOUCH_AD_AN10          0x29
#define MTOUCH_AD_AN11          0x2D
#define MTOUCH_AD_AN12          0x31
#define MTOUCH_AD_AN13          0x35
#define MTOUCH_AD_ISO_AND_GO    0x43
#define MTOUCH_AD_ISO_NOGO      0x41


#define MTOUCH_PIN_AN0          0
#define MTOUCH_PIN_AN1          1
#define MTOUCH_PIN_AN2          2
#define MTOUCH_PIN_AN3          3
#define MTOUCH_PIN_AN4          5
#define MTOUCH_PIN_AN5          0
#define MTOUCH_PIN_AN6          1
#define MTOUCH_PIN_AN7          2
#define MTOUCH_PIN_AN8          2
#define MTOUCH_PIN_AN9          3
#define MTOUCH_PIN_AN10         1
#define MTOUCH_PIN_AN11         4
#define MTOUCH_PIN_AN12         0
#define MTOUCH_PIN_AN13         5


#define MTOUCH_LETTER_AN0       A
#define MTOUCH_LETTER_AN1       A
#define MTOUCH_LETTER_AN2       A
#define MTOUCH_LETTER_AN3       A
#define MTOUCH_LETTER_AN4       A
#define MTOUCH_LETTER_AN5       E
#define MTOUCH_LETTER_AN6       E
#define MTOUCH_LETTER_AN7       E
#define MTOUCH_LETTER_AN8       B
#define MTOUCH_LETTER_AN9       B
#define MTOUCH_LETTER_AN10      B
#define MTOUCH_LETTER_AN11      B
#define MTOUCH_LETTER_AN12      B
#define MTOUCH_LETTER_AN13      B


#endif
/// @endcond
