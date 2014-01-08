/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_HardwareProfile_16F182x.h
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
/** @file mTouch_HardwareProfile_16F182x.h
* @brief Support for: PIC12F1822, PIC16F1823, PIC16F1824, PIC16F1825, PIC16F1826, PIC16F1827, PIC16F1828, PIC16F1829
*/
/// @cond
#if !defined(__MTOUCH_HARDWARE_16F182X_H)
#define __MTOUCH_HARDWARE_16F182X_H

#if !defined(_16F1829) && !defined(_16LF1829)
#if !defined(_16F1828) && !defined(_16LF1828)
#if !defined(_16F1827) && !defined(_16LF1827)
#if !defined(_16F1826) && !defined(_16LF1826)
#if !defined(_16F1825) && !defined(_16LF1825)
#if !defined(_16F1824) && !defined(_16LF1824)
#if !defined(_16F1823) && !defined(_16LF1823)
#if !defined(_12F1822) && !defined(_12LF1822)
    #error The current mTouch hardware include file does not support this PIC microcontroller.
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

#if defined(_12F1822) || defined(_12LF1822)
    #if MTOUCH_NUMBER_SENSORS > 4
        #error The PIC12F/LF1822 is not able to support more than 4 sensors due to pinout limitations.
    #endif
#endif
#if defined(_16F1823) || defined(_16LF1823)
    #if MTOUCH_NUMBER_SENSORS > 9
        #error The PIC16F/LF1823 is not able to support more than 7 sensors due to memory constraints.
    #endif
#endif
#if defined(_16F1823) || defined(_16LF1823)
    #if MTOUCH_NUMBER_SENSORS > 8
        #error The PIC16F/LF1823 is not able to support more than 8 sensors due to pinout limitations.
    #endif
#endif
#if defined(_16F1824) || defined(_16LF1824)
    #if MTOUCH_NUMBER_SENSORS > 8
        #error The PIC16F/LF1824 is not able to support more than 8 sensors due to pinout limitations.
    #endif
#endif
#if defined(_16F1825) || defined(_16LF1825)
    #if MTOUCH_NUMBER_SENSORS > 8
        #error The PIC16F/LF1825 is not able to support more than 8 sensors due to pinout limitations.
    #endif
#endif
#if defined(_16F1826) || defined(_16LF1826)
    #if MTOUCH_NUMBER_SENSORS > 12
        #error The PIC16F/LF1826 is not able to support more than 12 sensors due to pinout limitations.
    #endif
#endif
#if defined(_16F1827) || defined(_16LF1827)
    #if MTOUCH_NUMBER_SENSORS > 12
        #error The PIC16F/LF1827 is not able to support more than 12 sensors due to pinout limitations.
    #endif
#endif
#if defined(_16F1828) || defined(_16LF1828)
    #if MTOUCH_NUMBER_SENSORS > 12
        #error The PIC16F/LF1828 is not able to support more than 12 sensors due to pinout limitations.
    #endif
#endif
#if defined(_16F1829) || defined(_16LF1829)
    #if MTOUCH_NUMBER_SENSORS > 12
        #error The PIC16F/LF1829 is not able to support more than 12 sensors due to pinout limitations.
    #endif
#endif

#if ((MTOUCH_COMM_TYPE == MTOUCH_COMM_RS232_1WAY_ASCII) && !defined(MTOUCH_COMM_ASCII_SOFT_ENABLED)) || (MTOUCH_COMM_TYPE == MTOUCH_COMM_RS232_2WAY_BINARY)
#if !defined(APFCON_INITIALIZED)
    #warning The mTouch comm module does not automatically set the APFCON# register. The TX pin may be selectable. This warning can be removed by defining the precompiler variable 'APFCON_INITIALIZED' in the mTouch_config.h file - verifying that you've initialized this register.
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

#define PIC_TIMER0_AVAILABLE
#define PIC_TIMER1_AVAILABLE
#define PIC_TIMER2_AVAILABLE
#if !defined(_16F1823) && !defined(_16LF1823) && !defined(_12F1822) && !defined(_12LF1822)      
    #define PIC_TIMER4_AVAILABLE
    #define PIC_TIMER6_AVAILABLE
#endif

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
#define PIC_DAC_AVAILABLE
#define PIC_DACOUT_AVAILABLE

#if defined(_16F1827) || defined(_16LF1827) || defined(_16F1826) || defined(_16LF1826)
    #define PIC_DACOUT_TRIS_C               TRISA2
    #define PIC_DACOUT_LAT_C                LATA2
    #define PIC_DACOUT_LAT_ASM              _LATA
    #define PIC_DACOUT_PIN                  2
#else
    #define PIC_DACOUT_TRIS_C               TRISA0
    #define PIC_DACOUT_LAT_C                LATA0
    #define PIC_DACOUT_LAT_ASM              _LATA
    #define PIC_DACOUT_PIN                  0
#endif

#define PIC_DACCON0_VDD                 0xC0
#define PIC_DACCON1_VDD                 0x1F
#define PIC_DACCON0_VSS                 0x80
#define PIC_DACCON1_VSS                 0x00

#define PIC_DACOUT_DACCON0_VSS          0xA0
#define PIC_DACOUT_DACCON0_1_3RD_VDD    0xA0
#define PIC_DACOUT_DACCON0_HALF_VDD     0xA0
#define PIC_DACOUT_DACCON0_2_3RD_VDD    0xE0
#define PIC_DACOUT_DACCON0_VDD          0xE0

#define PIC_DACOUT_DACCON1_VSS          0x00
#define PIC_DACOUT_DACCON1_1_3RD_VDD    0x0B
#define PIC_DACOUT_DACCON1_HALF_VDD     0x10
#define PIC_DACOUT_DACCON1_2_3RD_VDD    0x16
#define PIC_DACOUT_DACCON1_VDD          0x1F

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
#define MTOUCH_AD_AN0                   0x01
#define MTOUCH_AD_AN1                   0x05
#define MTOUCH_AD_AN2                   0x09
#define MTOUCH_AD_AN3                   0x0D

#if !defined(_12F1822) && !defined(_12LF1822)
    #define MTOUCH_AD_AN4               0x11
    #define MTOUCH_AD_AN5               0x15
    #define MTOUCH_AD_AN6               0x19
    #define MTOUCH_AD_AN7               0x1D
#endif

#if defined(_16F1826) || defined(_16LF1826) || \
    defined(_16F1827) || defined(_16LF1827) || \
    defined(_16F1828) || defined(_16LF1828) || \
    defined(_16F1829) || defined(_16LF1829)
    
    #define MTOUCH_AD_AN8               0x21
    #define MTOUCH_AD_AN9               0x25
    #define MTOUCH_AD_AN10              0x29
    #define MTOUCH_AD_AN11              0x2D
#endif

#define MTOUCH_AD_FVR_AND_GO            0x7F 
#define MTOUCH_AD_DAC_AND_GO            0x7B 
#define MTOUCH_AD_DAC_NOGO              0x79 
#define MTOUCH_AD_ISO_AND_GO            0x43 
#define MTOUCH_AD_ISO_NOGO              0x41



#if defined(_16F1826) || defined(_16LF1826) || \
    defined(_16F1827) || defined(_16LF1827)
    
    #define MTOUCH_PIN_AN0              0
    #define MTOUCH_PIN_AN1              1
    #define MTOUCH_PIN_AN2              2
    #define MTOUCH_PIN_AN3              3
    #define MTOUCH_PIN_AN4              4
    #define MTOUCH_PIN_AN5              6
    #define MTOUCH_PIN_AN6              7
    #define MTOUCH_PIN_AN7              5
    #define MTOUCH_PIN_AN8              4
    #define MTOUCH_PIN_AN9              3
    #define MTOUCH_PIN_AN10             2
    #define MTOUCH_PIN_AN11             1
#else
    #define MTOUCH_PIN_AN0              0
    #define MTOUCH_PIN_AN1              1
    #define MTOUCH_PIN_AN2              2
    #define MTOUCH_PIN_AN3              4
    #define MTOUCH_PIN_AN4              0
    #define MTOUCH_PIN_AN5              1
    #define MTOUCH_PIN_AN6              2
    #define MTOUCH_PIN_AN7              3
    #define MTOUCH_PIN_AN8              6
    #define MTOUCH_PIN_AN9              7
    #define MTOUCH_PIN_AN10             4
    #define MTOUCH_PIN_AN11             5
#endif


#if defined(_16F1826) || defined(_16LF1826) || \
    defined(_16F1827) || defined(_16LF1827)
    
    #define MTOUCH_LETTER_AN0           A
    #define MTOUCH_LETTER_AN1           A
    #define MTOUCH_LETTER_AN2           A
    #define MTOUCH_LETTER_AN3           A
    #define MTOUCH_LETTER_AN4           A
    #define MTOUCH_LETTER_AN5           B
    #define MTOUCH_LETTER_AN6           B
    #define MTOUCH_LETTER_AN7           B
    #define MTOUCH_LETTER_AN8           B
    #define MTOUCH_LETTER_AN9           B
    #define MTOUCH_LETTER_AN10          B
    #define MTOUCH_LETTER_AN11          B
#else
    #define MTOUCH_LETTER_AN0           A
    #define MTOUCH_LETTER_AN1           A
    #define MTOUCH_LETTER_AN2           A
    #define MTOUCH_LETTER_AN3           A
    #define MTOUCH_LETTER_AN4           C
    #define MTOUCH_LETTER_AN5           C
    #define MTOUCH_LETTER_AN6           C
    #define MTOUCH_LETTER_AN7           C
    #define MTOUCH_LETTER_AN8           C
    #define MTOUCH_LETTER_AN9           C
    #define MTOUCH_LETTER_AN10          B
    #define MTOUCH_LETTER_AN11          B
#endif


#endif
/// @endcond

