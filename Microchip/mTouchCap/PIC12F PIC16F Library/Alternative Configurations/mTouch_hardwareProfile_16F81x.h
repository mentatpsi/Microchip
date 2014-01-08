/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_HardwareProfile_16F81x.h
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
/** @file mTouch_HardwareProfile_16F81x.h
* @brief Support for: PIC16F818, PIC16F819
*/
/// @cond
#if !defined(__MTOUCH_HARDWARE_16F81X_H)
#define __MTOUCH_HARDWARE_16F81X_H

#if !defined(_16F818)
#if !defined(_16F819)
    #error The current mTouch hardware include file does not support this PIC microcontroller.
#endif
#endif

#if MTOUCH_NUMBER_SENSORS > 5
    #error The current mTouch microcontroller is not able to support more than 5 sensors on the mTouch Framework due to pinout limitations.
#endif
#if MTOUCH_NUMBER_OF_SLEEP_SENSORS > 5
    #error The current mTouch microcontroller is not able to support more than 5 sleep-mode sensors due to pinout limitations.
#endif

//===========================================================
//  Device Configuration
//===========================================================
#define MTOUCH_USE_PORT


//===========================================================
//  Communications Configuration
//===========================================================
// NO UART AVAILABLE

#define PIC_TIMER0_AVAILABLE
#define PIC_TIMER1_AVAILABLE
#define PIC_TIMER2_AVAILABLE

//===========================================================
//  ADC Configuration
//===========================================================
#define PIC_ADC_BITS                10
#define PIC_ADC_UNIMP_AVAILABLE

#define PIC_ADC_ADCS                ADCON0bits.ADCS
#define PIC_ADC_ADCS2               ADCON1bits.ADCS2
#define PIC_ADC_ADFM                ADCON1bits.ADFM
#define PIC_ADC_ADON                ADCON0bits.ADON

#define PIC_ADC_ADCS_FOSC2          0b00
#define PIC_ADC_ADCS_FOSC4          0b00
#define PIC_ADC_ADCS_FOSC8          0b01
#define PIC_ADC_ADCS_FOSC16         0b01
#define PIC_ADC_ADCS_FOSC32         0b10
#define PIC_ADC_ADCS_FOSC64         0b10
#define PIC_ADC_ADCS_FRC            0b11

#if     _XTAL_FREQ == 8000000 
    #define PIC_ADC_SETCLK()    PIC_ADC_ADCS2 = 1; PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC16
    #define PIC_ADC_TAD         2 
    #define PIC_ADC_CLK_MASK    0x40
#elif   _XTAL_FREQ == 4000000 
    #define PIC_ADC_SETCLK()    PIC_ADC_ADCS2 = 0; PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC8
    #define PIC_ADC_TAD         2 
    #define PIC_ADC_CLK_MASK    0x40
#elif   _XTAL_FREQ == 1000000 
    #define PIC_ADC_SETCLK()    PIC_ADC_ADCS2 = 0; PIC_ADC_ADCS = PIC_ADC_ADCS_FOSC2
    #define PIC_ADC_TAD         2 
    #define PIC_ADC_CLK_MASK    0x00
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
// NOT AVAILABLE



//===========================================================
//  Analog Port Mapping
//  - Right Justification
//  - VDD/VSS Reference
//  - GO/DONE cleared
//  - ADON enabled
//===========================================================
#define MTOUCH_AD_AN0           0x01 | PIC_ADC_CLK_MASK
#define MTOUCH_AD_AN1           0x09 | PIC_ADC_CLK_MASK
#define MTOUCH_AD_AN2           0x11 | PIC_ADC_CLK_MASK
#define MTOUCH_AD_AN3           0x19 | PIC_ADC_CLK_MASK
#define MTOUCH_AD_AN4           0x21 | PIC_ADC_CLK_MASK
#define MTOUCH_AD_ISO_NOGO      0x29 | PIC_ADC_CLK_MASK
#define MTOUCH_AD_ISO_AND_GO    0x2D | PIC_ADC_CLK_MASK

#define MTOUCH_PIN_AN0          0
#define MTOUCH_PIN_AN1          1
#define MTOUCH_PIN_AN2          2
#define MTOUCH_PIN_AN3          3
#define MTOUCH_PIN_AN4          4

#define MTOUCH_LETTER_AN0       A
#define MTOUCH_LETTER_AN1       A
#define MTOUCH_LETTER_AN2       A
#define MTOUCH_LETTER_AN3       A
#define MTOUCH_LETTER_AN4       A


#endif
/// @endcond
