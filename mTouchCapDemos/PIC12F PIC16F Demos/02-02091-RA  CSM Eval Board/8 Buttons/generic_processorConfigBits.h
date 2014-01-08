/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    (Generic)
 *  FileName:        generic_processorConfigBits.h
 *  Dependencies:    
 *  Processor:       
 *  Compiler:        HI-TECH Ver. 9.81 or later
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     Generic configuration bits for select processors.
 *************************************************************************/
/*************************************************************************
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
/** @file   generic_processorConfigBits.h
*   @brief  Generic configuration bits for framework-supported processors.
*           Provided for ease-of-development.
*/
#if !defined(_GENERIC_PROCESSORCONFIGBITS_H)
#define _GENERIC_PROCESSORCONFIGBITS_H

    // When available, these configuration values will set:
    //      Internal Oscillator:    Fastest available
    //      Watchdog Timer Software Enabled (or, if not available, OFF)
    //      Code and Memory Protection Disabled
    //      Low Voltage Programming Off                 
    //      Power-Up Timer Enabled                      
    //      Brown-out Reset Disabled
    //      MCLR Disabled
    //      Clock Out Disabled
    //      Internal/External Switchover Mode Disabled
    //      Fail-Safe Clock Monitor Disabled
    //      Write Protection Disabled
    //      4x PLL Enabled
    //      Stack Overflow/Underflow Reset Enabled
    //      Voltage Regulator Capacitor Disabled - NOTE: This is bad for noise.

    
    #if     defined(_16F1827)       || defined(_16LF1827)   || defined(_16F1826)    || defined(_16LF1826)   \
         || defined(_16F1823)       || defined(_16LF1823)   || defined(_12F1822)    || defined(_12LF1822)   \
         || defined(_16F1824)       || defined(_16LF1824)   || defined(_16F1825)    || defined(_16LF1825)   \
         || defined(_16F1828)       || defined(_16LF1828)   || defined(_16F1829)    || defined(_16LF1829)   \
         || defined(_12F1840)       || defined(_12LF1840)   || defined(_16F1847)    || defined(_16LF1847) 
        #if (_HTC_VER_MAJOR_ < 9) || ((_HTC_VER_MAJOR_ == 9) && (_HTC_VER_MINOR_ < 83))
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
            __CONFIG(WRT_OFF & PLLEN_ON & STVREN_ON & BORV_25 & LVP_OFF);
        #else
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
            __CONFIG(WRT_OFF & PLLEN_ON & STVREN_ON & BORV_HI & LVP_OFF);
        #endif
    #elif   defined(_16F1933)       || defined(_16F1934)       \
         || defined(_16F1936)       || defined(_16F1937)       \
         || defined(_16F1938)       || defined(_16F1939)       \
         || defined(_16F1946)       || defined(_16F1947)    
        #if (_HTC_VER_MAJOR_ < 9) || ((_HTC_VER_MAJOR_ == 9) && (_HTC_VER_MINOR_ < 83))
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
            __CONFIG(WRT_OFF & VCAPEN_OFF & PLLEN_ON & STVREN_ON & BORV_25 & LVP_OFF);
        #else
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
            __CONFIG(WRT_OFF & VCAPEN_OFF & PLLEN_ON & STVREN_ON & BORV_HI & LVP_OFF);
        #endif
    #elif   defined(_16LF1933)   || defined(_16LF1934)   \
         || defined(_16LF1936)   || defined(_16LF1937)   \
         || defined(_16LF1938)   || defined(_16LF1939)   \
         || defined(_16LF1946)   || defined(_16LF1947) 
        #if (_HTC_VER_MAJOR_ < 9) || ((_HTC_VER_MAJOR_ == 9) && (_HTC_VER_MINOR_ < 83))
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
            __CONFIG(WRT_OFF & PLLEN_ON & STVREN_ON & BORV_25 & LVP_OFF);
        #else
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
            __CONFIG(WRT_OFF & PLLEN_ON & STVREN_ON & BORV_HI & LVP_OFF);
        #endif
    #elif   defined(_16LF1902)      || defined(_16LF1903)   || defined(_16LF1904)   || defined(_16LF1906)   \
         || defined(_16LF1907)      || defined(_16F1503)    || defined(_16LF1503)
        #if (_HTC_VER_MAJOR_ < 9) || ((_HTC_VER_MAJOR_ == 9) && (_HTC_VER_MINOR_ < 83))
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & BOREN_OFF & CLKOUTEN_OFF);
            __CONFIG(WRT_OFF & STVREN_ON & BORV_25 & LPBOR_OFF & LVP_OFF);
        #else
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & BOREN_OFF & CLKOUTEN_OFF);
            __CONFIG(WRT_OFF & STVREN_ON & BORV_HI & LPBOR_OFF & LVP_OFF);
        #endif
    #elif   defined(_16F1526)
        #if (_HTC_VER_MAJOR_ < 9) || ((_HTC_VER_MAJOR_ == 9) && (_HTC_VER_MINOR_ < 83))
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
            __CONFIG(WRT_OFF & VCAPEN_OFF & STVREN_ON & BORV_270 & LPBOR_OFF & LVP_OFF);
        #else
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
            __CONFIG(WRT_OFF & VCAPEN_OFF & STVREN_ON & BORV_HI & LPBOR_OFF & LVP_OFF);
        #endif
    #elif   defined(_16LF1526)      || defined(_16F1527)    || defined(_16LF1527)   || defined(_16F1516)    \
         || defined(_16LF1516)      || defined(_16F1517)    || defined(_16LF1517)   || defined(_16F1518)    \
         || defined(_16LF1518)      || defined(_16F1519)    || defined(_16LF1519)
        #if (_HTC_VER_MAJOR_ < 9) || ((_HTC_VER_MAJOR_ == 9) && (_HTC_VER_MINOR_ < 83))
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
            __CONFIG(WRT_OFF & STVREN_ON & BORV_25 & LPBOR_OFF & LVP_OFF);
        #else
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
            __CONFIG(WRT_OFF & STVREN_ON & BORV_HI & LPBOR_OFF & LVP_OFF);
        #endif
    #elif   defined(_16F1507)     || defined(_16F1509)
        #if (_HTC_VER_MAJOR_ < 9) || ((_HTC_VER_MAJOR_ == 9) && (_HTC_VER_MINOR_ < 83))
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & BOREN_OFF & CLKOUTEN_OFF);
            __CONFIG(WRT_OFF & STVREN_ON & BORV_25 & LPBOREN_OFF & LVP_OFF);
        #else
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & BOREN_OFF & CLKOUTEN_OFF);
            __CONFIG(WRT_OFF & STVREN_ON & BORV_HI & BOREN_OFF & LVP_OFF);
        #endif
    #elif   defined(_16LF1507)    || defined(_16LF1509)
        #if (_HTC_VER_MAJOR_ < 9) || ((_HTC_VER_MAJOR_ == 9) && (_HTC_VER_MINOR_ < 83))
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & BOREN_OFF & CLKOUTEN_OFF);
            __CONFIG(WRT_OFF & STVREN_ON & BORV_25 & LPBOREN_OFF);
        #else
            __CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_ON & MCLRE_OFF & CP_OFF & BOREN_OFF & CLKOUTEN_OFF);
            __CONFIG(WRT_OFF & STVREN_ON & BORV_HI & LPBOREN_OFF);
        #endif
    #elif   defined(_16F785)        || defined(_16HV785)
            __CONFIG(FOSC_INTOSCIO & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & IESO_OFF & FCMEN_OFF);
    #elif   defined(_16F720)        || defined(_16LF720)    || defined(_16F721)     || defined(_16LF721)
            __CONFIG(FOSC_INTOSCIO & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & BOREN_OFF & PLLEN_ON);
            __CONFIG(WRTEN_OFF);
    #elif   defined(_16F722)        || defined(_16LF722)    || defined(_16F722A)    || defined(_16LF722A)   \
         || defined(_16F723)        || defined(_16LF723)    || defined(_16F723A)    || defined(_16LF723A)   \
         || defined(_16F724)        || defined(_16LF724)    || defined(_16F726)     || defined(_16LF726)    \
         || defined(_16F727)        || defined(_16LF727)
        #if (_HTC_VER_MAJOR_ < 9) || ((_HTC_VER_MAJOR_ == 9) && (_HTC_VER_MINOR_ < 83))
            __CONFIG(FOSC_INTOSCIO & WDTE_OFF & nPWRTE_OFF & MCLRE_OFF & CP_OFF & BOREN_OFF & BORV_25 & PLLEN_ON & DEBUG_OFF);
            __CONFIG(VCAPEN_DIS);
        #else
            __CONFIG(FOSC_INTOSCIO & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & BOREN_OFF & BORV_25 & PLLEN_ON & DEBUG_OFF);
            __CONFIG(VCAPEN_DIS);
        #endif
    #elif   defined(_12F615)        || defined(_12HV615)    || defined(_16F616)     || defined(_16HV616)
            __CONFIG(FOSC_INTOSCIO & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & IOSCFS_8MHZ & BOREN_OFF);
    #elif   defined(_12F617)
            __CONFIG(FOSC_INTOSCIO & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & IOSCFS_8MHZ & BOREN_OFF & WRT_OFF);
    #elif   defined(_16F677)        || defined(_16F685)     || defined(_16F687)     || defined(_16F689)     \
         || defined(_16F690)
            __CONFIG(FOSC_INTRCIO & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & IESO_OFF & FCMEN_OFF);
    #elif   defined(_16F688)
            __CONFIG(FOSC_INTOSCIO & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & IESO_OFF & FCMEN_OFF);
    #elif   defined(_12F683)        || defined(_16F688)     || defined(_16F684)
            __CONFIG(FOSC_INTOSCIO & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & IESO_OFF & FCMEN_OFF);
    #elif   defined(_PIC18)
        #if defined(_18F24J50)      || defined(_18F25J50)   || defined(_18F26J50)   || defined(_18F44J50)   \
         || defined(_18F45J50)      || defined(_18LF24J50)  || defined(_18LF25J50)  || defined(_18LF26J50)  \
         || defined(_18LF44J50)     || defined(_18LF45J50)  || defined(_18F46J50)   || defined(_18F26J53)   \
         || defined(_18F27J53)      || defined(_18F46J53)   || defined(_18F47J53)   || defined(_18LF26J53)  \
         || defined(_18LF27J53)     || defined(_18LF46J53)  || defined(_18LF47J53)  || defined(_18LF46J50) 
            #pragma config WDTEN = OFF, XINST = OFF, PLLDIV = 2, CPUDIV = OSC1 
            #pragma config OSC=HSPLL, DSBOREN = OFF, IOL1WAY = OFF 
        #endif
        #if defined(__18F24J11)     || defined(__18F25J11)  || defined(__18F26J11)  || defined(__18F44J11)  \
         || defined(__18F45J11)     || defined(__18F46J11)  || defined(__18LF45J11) || defined(__18LF46J11) \
         || defined(__18LF24J11)    || defined(__18LF25J11) || defined(__18LF26J11) || defined(__18LF44J11) \
         || defined(__18F26J13)     || defined(__18F27J13)  || defined(__18F46J13)  || defined(__18F47J13)  \
         || defined(__18LF26J13)    || defined(__18LF27J13) || defined(__18LF46J13) || defined(__18LF47J13) 
            #pragma config WDTEN = OFF, XINST = OFF 
            #pragma config OSC=HSPLL, DSBOREN = OFF, IOL1WAY = OFF 
        #endif
        #if defined(ADC_ADCON_LEGACY)
            #pragma config WDTEN = OFF, XINST = OFF, OSC=HSPLL 
        #endif
        #if defined(ADC_ANSEL_NEW)
            #pragma config XINST = OFF, FOSC = HSMP, BOREN = OFF
            #pragma config WDTEN = OFF, MCLRE = EXTMCLR
        #endif
        #if defined(ADC_ANCON_NEW)
            #pragma config XINST = OFF, FOSC = HS1,PLLCFG = ON, BOREN = OFF
            #pragma config WDTEN = OFF, MCLRE = ON
        #endif
    #endif


#endif