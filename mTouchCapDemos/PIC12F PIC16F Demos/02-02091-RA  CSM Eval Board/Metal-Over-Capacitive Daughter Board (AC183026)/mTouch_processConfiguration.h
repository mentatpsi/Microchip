/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_processConfiguration.h
 *
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework Include File
 *                   - No application code should be implemented in this
 *                     or any other mTouch framework file. This will allow
 *                     for better customer support and easier upgrades to
 *                     later firmware versions. Use the main.c and user-
 *                     generated files to implement your application.
 *                   - See the documentation located in the docs/ folder
 *                     for a more information about how the framework is
 *                     implemented.
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
/** @file   mTouch_processConfiguration.h
*   @brief  Internal framework header file performing many pre-compiler 
*           checks and generating predefined values for macro use.
*
* Also, performs setup and error checking operations at compile time.
*/
#ifndef __MTOUCH_PROCESSCONFIGURATION_H
#define __MTOUCH_PROCESSCONFIGURATION_H

/// @cond

// COPIED FROM stdint.h DUE TO THESE BEING MISSING (BUT STILL SUPPORTED) FROM THE PIC18 COMPILER

#ifndef int8_t
typedef signed char int8_t;
#define int8_t int8_t
#define INT8_MIN (-128)
#define INT8_MAX (127)
#endif
#ifndef int16_t
typedef signed int int16_t;
#define int16_t int16_t
#define INT16_MIN (-32768)
#define INT16_MAX (32767)
#endif
#ifndef int24_t
typedef signed short long int int24_t;
#define int24_t int24_t
#define INT24_MIN (-8388608L)
#define INT24_MAX (8388607L)
#endif
#ifndef int32_t
typedef signed long int int32_t;
#define int32_t int32_t
#define INT32_MIN (-2147483648L)
#define INT32_MAX (2147483647L)
#endif
#ifndef uint8_t
typedef unsigned char uint8_t;
#define uint8_t uint8_t
#define UINT8_MAX (255)
#endif
#ifndef uint16_t
typedef unsigned int uint16_t;
#define uint16_t uint16_t
#define UINT16_MAX (65535U)
#endif
#ifndef uint24_t
typedef unsigned short long int uint24_t;
#define uint24_t uint24_t
#define UINT24_MAX (16777216UL)
#endif
#ifndef uint32_t
typedef unsigned long int uint32_t;
#define uint32_t uint32_t
#define UINT32_MAX (4294967295UL)
#endif


#if defined(HI_TECH_C)
#pragma warning disable 1395    // Disables the following message from appearing at compile time:
                                //
                                //  Advisory[1395] notable code sequence candidate suitable 
                                //                 for compiler validation suite detected
                                //
                                // TIP: The compiler will display this message if it encounters a 
                                //      specific code sequence that results in compiler templates 
                                //      being used in a unique way -- this is difficult to explain 
                                //      if you are not familiar with the compiler internals. It 
                                //      does not mean there is a bug in the generated code, but 
                                //      that the code sequence is less tested. If you run across
                                //      this advisory, sending in the code to the compiler team
                                //      enables them to create new test suite cases that will more 
                                //      rigorously test that particular situation.
                                //      The code that is currently generating this advisory in the
                                //      mTouch Framework has already been submitted to Hi-Tech.
#endif

//****************************************************************************************************
//  _   _      _                   __  __                          
// | | | | ___| |_ __   ___ _ __  |  \/  | __ _  ___ _ __ ___  ___ 
// | |_| |/ _ \ | '_ \ / _ \ '__| | |\/| |/ _` |/ __| '__/ _ \/ __|
// |  _  |  __/ | |_) |  __/ |    | |  | | (_| | (__| | | (_) \__ \
// |_| |_|\___|_| .__/ \___|_|    |_|  |_|\__,_|\___|_|  \___/|___/
//              |_|  
//****************************************************************************************************

#define     __paste(a,b)                    __pasteX(a,b)               // Use this macro to concatonate 2 strings in the pre-compiler
#define     __pasteX(a,b)                   a##b                        // Do not use directly. Use __paste(a,b) instead.
#define     __paste2(a,b)                   __paste2X(a,b)              
#define     __paste2X(a,b)                  a##b                        
#define     __paste3(a,b)                   __paste3X(a,b)              
#define     __paste3X(a,b)                  a##b                        
#define     __paste4(a,b)                   __paste4X(a,b)              
#define     __paste4X(a,b)                  a##b                        
#define     __paste5(a,b)                   __paste5X(a,b)              
#define     __paste5X(a,b)                  a##b                        

#define     __3paste(a,b,c)                 __3pasteX(a,b,c)            // Use this macro to concatonate 3 strings in the pre-compiler
#define     __4paste(a,b,c,d)               __4pasteX(a,b,c,d)          // Use this macro to concatonate 4 strings in the pre-compiler
#define     __5paste(a,b,c,d,e)             __5pasteX(a,b,c,d,e)        // Use this macro to concatonate 5 strings in the pre-compiler
#define     __3pasteX(a,b,c)                a##b##c                     // Do not use directly. Use __paste3(a,b,c) instead.
#define     __4pasteX(a,b,c,d)              a##b##c##d                  // Do not use directly. Use __paste4(a,b,c,d) instead.
#define     __5pasteX(a,b,c,d,e)            a##b##c##d##e               // Do not use directly. Use __paste5(a,b,c,d,e) instead.


//****************************************************************************************************
//   ____                      _ _              ____                            _   _ _     _ _ _ _         
//  / ___|___  _ __ ___  _ __ (_) | ___ _ __   / ___|___  _ __ ___  _ __   __ _| |_(_) |__ (_) (_) |_ _   _ 
// | |   / _ \| '_ ` _ \| '_ \| | |/ _ \ '__| | |   / _ \| '_ ` _ \| '_ \ / _` | __| | '_ \| | | | __| | | |
// | |__| (_) | | | | | | |_) | | |  __/ |    | |__| (_) | | | | | | |_) | (_| | |_| | |_) | | | | |_| |_| |
//  \____\___/|_| |_| |_| .__/|_|_|\___|_|     \____\___/|_| |_| |_| .__/ \__,_|\__|_|_.__/|_|_|_|\__|\__, |
//                      |_|                                        |_|                                |___/ 
//****************************************************************************************************
#if defined(__PICC18__)
    #if (_HTC_VER_MAJOR_ < 9) || ((_HTC_VER_MAJOR_ == 9) && (_HTC_VER_MINOR_ < 80))
    #error The mTouch Framework is not compatible with your version of the compiler. Please update to Hi-Tech PICC-18 9.80 or later - currently available on microchip.com
    #endif
#endif


//****************************************************************************************************
//  _   _               _                          ____             __ _ _           
// | | | | __ _ _ __ __| |_      ____ _ _ __ ___  |  _ \ _ __ ___  / _(_) | ___  ___ 
// | |_| |/ _` | '__/ _` \ \ /\ / / _` | '__/ _ \ | |_) | '__/ _ \| |_| | |/ _ \/ __|
// |  _  | (_| | | | (_| |\ V  V / (_| | | |  __/ |  __/| | | (_) |  _| | |  __/\__ \
// |_| |_|\__,_|_|  \__,_| \_/\_/ \__,_|_|  \___| |_|   |_|  \___/|_| |_|_|\___||___/ 
//
//****************************************************************************************************

// Include the hardware profile header file that corresponds to the currently selected PIC microcontroller.
#if     defined(_16F1827) || defined(_16LF1827) || defined(_16F1826) || defined(_16LF1826) || defined(_16F1823) || defined(_16LF1823) || defined(_12F1822) || defined(_12LF1822) || defined(_16F1824) || defined(_16LF1824) || defined(_16F1825) || defined(_16LF1825) || defined(_16F1828) || defined(_16LF1828) || defined(_16F1829) || defined(_16LF1829)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F182x.h"
    #define _16F182x
#elif   defined(_12F1840) || defined(_12LF1840)
    #include "Alternative Configurations/mTouch_hardwareProfile_12F184x.h"
    #define _12F184x
#elif   defined(_16F1847) || defined(_16LF1847)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F184x.h"
    #define _16F184x
#elif   defined(_16F1933) || defined(_16LF1933) || defined(_16F1934) || defined(_16LF1934) || defined(_16F1936) || defined(_16LF1936) || defined(_16F1937) || defined(_16LF1937) || defined(_16F1938) || defined(_16LF1938) || defined(_16F1939) || defined(_16LF1939)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F193x.h"
    #define _16F193x
#elif   defined(_16F1946) || defined(_16LF1946) || defined(_16F1947) || defined(_16LF1947) 
    #include "Alternative Configurations/mTouch_hardwareProfile_16F194x.h"
    #define _16F194x
#elif   defined(_16LF1902) || defined(_16LF1903) || defined(_16LF1904) || defined(_16LF1906) || defined(_16LF1907) 
    #include "Alternative Configurations/mTouch_hardwareProfile_16LF190x.h"
    #define _16LF190x
#elif   defined(_12F1552) || defined(_12LF1552)
    #include "Alternative Configurations/mTouch_hardwareProfile_12F1552.h"
    #define _12F1552
#elif   defined(_16F1526) || defined(_16LF1526) || defined(_16F1527) || defined(_16LF1527) 
    #include "Alternative Configurations/mTouch_hardwareProfile_16F152x.h"
    #define _16F152x
#elif   defined(_16F1516) || defined(_16LF1516) || defined(_16F1517) || defined(_16LF1517) || defined(_16F1518) || defined(_16LF1518) || defined(_16F1519) || defined(_16LF1519)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F151x.h" 
    #define _16F151x
#elif   defined(_16F1512) || defined(_16LF1512) || defined(_16F1513) || defined(_16LF1513)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F1512-3.h" 
    #define _16F15123
#elif   defined(_12F1501) || defined(_12LF1501) || defined(_16F1503) || defined(_16LF1503) || defined(_16F1507) || defined(_16LF1507) || defined(_16F1509) || defined(_16LF1509)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F150x.h" 
    #define _16F150x
#elif   defined(_16F913) || defined(_16F914) || defined(_16F916) || defined(_16F917) || defined(_16F946)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F91x.h"
#elif   defined(_16F882) || defined(_16F883) || defined(_16F884) || defined(_16F886) || defined(_16F887)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F88x.h"
#elif   defined(_16F818) || defined(_16F819)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F81x.h"
#elif   defined(_16F785) || defined(_16HV785)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F785.h"
#elif   defined(_16F720) || defined(_16LF720) || defined(_16F721) || defined(_16LF721) || defined(_16F722) || defined(_16LF722) || defined(_16F722A) || defined(_16LF722A) || defined(_16F723) || defined(_16LF723) || defined(_16F723A) || defined(_16LF723A) || defined(_16F724) || defined(_16LF724) || defined(_16F726) || defined(_16LF726) || defined(_16F727) || defined(_16LF727)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F72x.h"
#elif   defined(_12F615) || defined(_12HV615) || defined(_12F617)
    #include "Alternative Configurations/mTouch_hardwareProfile_12F61x.h"
    #define _12F61x
#elif   defined(_16F616) || defined(_16HV616)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F61x.h"
    #define _16F61x
#elif   defined(_16F677) || defined(_16F685) || defined(_16F687) || defined(_16F688) || defined(_16F689) || defined(_16F690)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F68x.h"
    #define _16F68x
#elif   defined(_16F684)
    #include "Alternative Configurations/mTouch_hardwareProfile_16F684.h"
    #define _16F68x
#elif   defined(_12F683)
    #include "Alternative Configurations/mTouch_hardwareProfile_12F683.h"
#elif   defined(_18F24J50)  || defined(_18LF24J50)  || defined(_18F25J50)  || defined(_18LF25J50)  || defined(_18F26J50)  || defined(_18LF26J50)  || defined(_18F44J50)  || defined(_18LF44J50)  || defined(_18F45J50)  || defined(_18LF45J50)  || defined(_18F46J50)  || defined(_18LF46J50)  || defined(_18F65J50)  || defined(_18F66J50)  || defined(_18F67J50)  || defined(_18F85J50)  || defined(_18F86J50)  || defined(_18F87J50) \
     || defined(__18F24J50) || defined(__18LF24J50) || defined(__18F25J50) || defined(__18LF25J50) || defined(__18F26J50) || defined(__18LF26J50) || defined(__18F44J50) || defined(__18LF44J50) || defined(__18F45J50) || defined(__18LF45J50) || defined(__18F46J50) || defined(__18LF46J50) || defined(__18F65J50) || defined(__18F66J50) || defined(__18F67J50) || defined(__18F85J50) || defined(__18F86J50) || defined(__18F87J50)
    #include "Alternative Configurations/mTouch_hardwareProfile_18FxxJ50.h"
    #define _18FxxJ50
#else
    #error The currently chosen processor is not yet supported by the mTouch Framework.
#endif 


#if !defined(PIC_ADC_ADFM_RIGHT)
#define PIC_ADC_ADFM_RIGHT          1
#endif
#if !defined(PIC_ADC_ADFM_LEFT)
#define PIC_ADC_ADFM_LEFT           0
#endif

    #if defined(MTOUCH_USE_GPIO)
        #define MTOUCH_PORT_C_SENSOR(index)     GP
        #define MTOUCH_PORT_ASM_SENSOR(index)   _GP
        #define MTOUCH_LAT_C_SENSOR(index)      GPIO
        #define MTOUCH_LAT_ASM_SENSOR(index)    _GPIO  
        #define MTOUCH_LETTER_SENSOR0           IO
        #define MTOUCH_LETTER_SENSOR1           IO
        #define MTOUCH_LETTER_SENSOR2           IO
        #define MTOUCH_LETTER_SENSOR3           IO
        #define MTOUCH_LETTER_SENSOR4           IO
        #define MTOUCH_LETTER_SENSOR5           IO
        #define MTOUCH_LETTER_SENSOR6           IO
        #define MTOUCH_LETTER_SENSOR7           IO
    #else
        #define MTOUCH_LETTER_SENSOR0           __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR0)
        #define MTOUCH_LETTER_SENSOR1           __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR1)
        #define MTOUCH_LETTER_SENSOR2           __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR2)
        #define MTOUCH_LETTER_SENSOR3           __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR3)
        #define MTOUCH_LETTER_SENSOR4           __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR4)
        #define MTOUCH_LETTER_SENSOR5           __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR5)
        #define MTOUCH_LETTER_SENSOR6           __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR6)
        #define MTOUCH_LETTER_SENSOR7           __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR7)
        #define MTOUCH_LETTER_SENSOR8           __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR8)
        #define MTOUCH_LETTER_SENSOR9           __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR9)
        #define MTOUCH_LETTER_SENSOR10          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR10)
        #define MTOUCH_LETTER_SENSOR11          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR11)
        #define MTOUCH_LETTER_SENSOR12          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR12)
        #define MTOUCH_LETTER_SENSOR13          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR13)
        #define MTOUCH_LETTER_SENSOR14          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR14)
        #define MTOUCH_LETTER_SENSOR15          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR15)
        #define MTOUCH_LETTER_SENSOR16          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR16)
        #define MTOUCH_LETTER_SENSOR17          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR17)
        #define MTOUCH_LETTER_SENSOR18          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR18)
        #define MTOUCH_LETTER_SENSOR19          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR19)
        #define MTOUCH_LETTER_SENSOR20          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR20)
        #define MTOUCH_LETTER_SENSOR21          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR21)
        #define MTOUCH_LETTER_SENSOR22          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR22)
        #define MTOUCH_LETTER_SENSOR23          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR23)
        #define MTOUCH_LETTER_SENSOR24          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR24)
        #define MTOUCH_LETTER_SENSOR25          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR25)
        #define MTOUCH_LETTER_SENSOR26          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR26)
        #define MTOUCH_LETTER_SENSOR27          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR27)
        #define MTOUCH_LETTER_SENSOR28          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR28)
        #define MTOUCH_LETTER_SENSOR29          __paste5(MTOUCH_LETTER_,MTOUCH_SENSOR29)
        
        #define MTOUCH_PORT_C_SENSOR0            __paste4(R,MTOUCH_LETTER_SENSOR0)
        #define MTOUCH_PORT_C_SENSOR1            __paste4(R,MTOUCH_LETTER_SENSOR1)
        #define MTOUCH_PORT_C_SENSOR2            __paste4(R,MTOUCH_LETTER_SENSOR2)
        #define MTOUCH_PORT_C_SENSOR3            __paste4(R,MTOUCH_LETTER_SENSOR3)
        #define MTOUCH_PORT_C_SENSOR4            __paste4(R,MTOUCH_LETTER_SENSOR4)
        #define MTOUCH_PORT_C_SENSOR5            __paste4(R,MTOUCH_LETTER_SENSOR5)
        #define MTOUCH_PORT_C_SENSOR6            __paste4(R,MTOUCH_LETTER_SENSOR6)
        #define MTOUCH_PORT_C_SENSOR7            __paste4(R,MTOUCH_LETTER_SENSOR7)
        #define MTOUCH_PORT_C_SENSOR8            __paste4(R,MTOUCH_LETTER_SENSOR8)
        #define MTOUCH_PORT_C_SENSOR9            __paste4(R,MTOUCH_LETTER_SENSOR9)
        #define MTOUCH_PORT_C_SENSOR10           __paste4(R,MTOUCH_LETTER_SENSOR10)
        #define MTOUCH_PORT_C_SENSOR11           __paste4(R,MTOUCH_LETTER_SENSOR11)
        #define MTOUCH_PORT_C_SENSOR12           __paste4(R,MTOUCH_LETTER_SENSOR12)
        #define MTOUCH_PORT_C_SENSOR13           __paste4(R,MTOUCH_LETTER_SENSOR13)
        #define MTOUCH_PORT_C_SENSOR14           __paste4(R,MTOUCH_LETTER_SENSOR14)
        #define MTOUCH_PORT_C_SENSOR15           __paste4(R,MTOUCH_LETTER_SENSOR15)
        #define MTOUCH_PORT_C_SENSOR16           __paste4(R,MTOUCH_LETTER_SENSOR16)
        #define MTOUCH_PORT_C_SENSOR17           __paste4(R,MTOUCH_LETTER_SENSOR17)
        #define MTOUCH_PORT_C_SENSOR18           __paste4(R,MTOUCH_LETTER_SENSOR18)
        #define MTOUCH_PORT_C_SENSOR19           __paste4(R,MTOUCH_LETTER_SENSOR19)
        #define MTOUCH_PORT_C_SENSOR20           __paste4(R,MTOUCH_LETTER_SENSOR20)
        #define MTOUCH_PORT_C_SENSOR21           __paste4(R,MTOUCH_LETTER_SENSOR21)
        #define MTOUCH_PORT_C_SENSOR22           __paste4(R,MTOUCH_LETTER_SENSOR22)
        #define MTOUCH_PORT_C_SENSOR23           __paste4(R,MTOUCH_LETTER_SENSOR23)
        #define MTOUCH_PORT_C_SENSOR24           __paste4(R,MTOUCH_LETTER_SENSOR24)
        #define MTOUCH_PORT_C_SENSOR25           __paste4(R,MTOUCH_LETTER_SENSOR25)
        #define MTOUCH_PORT_C_SENSOR26           __paste4(R,MTOUCH_LETTER_SENSOR26)
        #define MTOUCH_PORT_C_SENSOR27           __paste4(R,MTOUCH_LETTER_SENSOR27)
        #define MTOUCH_PORT_C_SENSOR28           __paste4(R,MTOUCH_LETTER_SENSOR28)
        #define MTOUCH_PORT_C_SENSOR29           __paste4(R,MTOUCH_LETTER_SENSOR29)
        #define MTOUCH_PORT_C_SENSOR(index)      __paste3(MTOUCH_PORT_C_SENSOR, index)

        #define MTOUCH_PORT_ASM_SENSOR0          __paste4(R,MTOUCH_LETTER_SENSOR0)
        #define MTOUCH_PORT_ASM_SENSOR1          __paste4(R,MTOUCH_LETTER_SENSOR1)
        #define MTOUCH_PORT_ASM_SENSOR2          __paste4(R,MTOUCH_LETTER_SENSOR2)
        #define MTOUCH_PORT_ASM_SENSOR3          __paste4(R,MTOUCH_LETTER_SENSOR3)
        #define MTOUCH_PORT_ASM_SENSOR4          __paste4(R,MTOUCH_LETTER_SENSOR4)
        #define MTOUCH_PORT_ASM_SENSOR5          __paste4(R,MTOUCH_LETTER_SENSOR5)
        #define MTOUCH_PORT_ASM_SENSOR6          __paste4(R,MTOUCH_LETTER_SENSOR6)
        #define MTOUCH_PORT_ASM_SENSOR7          __paste4(R,MTOUCH_LETTER_SENSOR7)
        #define MTOUCH_PORT_ASM_SENSOR8          __paste4(R,MTOUCH_LETTER_SENSOR8)
        #define MTOUCH_PORT_ASM_SENSOR9          __paste4(R,MTOUCH_LETTER_SENSOR9)
        #define MTOUCH_PORT_ASM_SENSOR10         __paste4(R,MTOUCH_LETTER_SENSOR10)
        #define MTOUCH_PORT_ASM_SENSOR11         __paste4(R,MTOUCH_LETTER_SENSOR11)
        #define MTOUCH_PORT_ASM_SENSOR12         __paste4(R,MTOUCH_LETTER_SENSOR12)
        #define MTOUCH_PORT_ASM_SENSOR13         __paste4(R,MTOUCH_LETTER_SENSOR13)
        #define MTOUCH_PORT_ASM_SENSOR14         __paste4(R,MTOUCH_LETTER_SENSOR14)
        #define MTOUCH_PORT_ASM_SENSOR15         __paste4(R,MTOUCH_LETTER_SENSOR15)
        #define MTOUCH_PORT_ASM_SENSOR16         __paste4(R,MTOUCH_LETTER_SENSOR16)
        #define MTOUCH_PORT_ASM_SENSOR17         __paste4(R,MTOUCH_LETTER_SENSOR17)
        #define MTOUCH_PORT_ASM_SENSOR18         __paste4(R,MTOUCH_LETTER_SENSOR18)
        #define MTOUCH_PORT_ASM_SENSOR19         __paste4(R,MTOUCH_LETTER_SENSOR19)
        #define MTOUCH_PORT_ASM_SENSOR20         __paste4(R,MTOUCH_LETTER_SENSOR20)
        #define MTOUCH_PORT_ASM_SENSOR21         __paste4(R,MTOUCH_LETTER_SENSOR21)
        #define MTOUCH_PORT_ASM_SENSOR22         __paste4(R,MTOUCH_LETTER_SENSOR22)
        #define MTOUCH_PORT_ASM_SENSOR23         __paste4(R,MTOUCH_LETTER_SENSOR23)
        #define MTOUCH_PORT_ASM_SENSOR24         __paste4(R,MTOUCH_LETTER_SENSOR24)
        #define MTOUCH_PORT_ASM_SENSOR25         __paste4(R,MTOUCH_LETTER_SENSOR25)
        #define MTOUCH_PORT_ASM_SENSOR26         __paste4(R,MTOUCH_LETTER_SENSOR26)
        #define MTOUCH_PORT_ASM_SENSOR27         __paste4(R,MTOUCH_LETTER_SENSOR27)
        #define MTOUCH_PORT_ASM_SENSOR28         __paste4(R,MTOUCH_LETTER_SENSOR28)
        #define MTOUCH_PORT_ASM_SENSOR29         __paste4(R,MTOUCH_LETTER_SENSOR29)
        #define MTOUCH_PORT_ASM_SENSOR(index)    __paste3(MTOUCH_PORT_ASM_SENSOR, index)
    #endif
    
        
    
    #if defined(MTOUCH_USE_PORT)
        #define MTOUCH_LAT_C_SENSOR0            __paste4(PORT,MTOUCH_LETTER_SENSOR0)
        #define MTOUCH_LAT_C_SENSOR1            __paste4(PORT,MTOUCH_LETTER_SENSOR1)
        #define MTOUCH_LAT_C_SENSOR2            __paste4(PORT,MTOUCH_LETTER_SENSOR2)
        #define MTOUCH_LAT_C_SENSOR3            __paste4(PORT,MTOUCH_LETTER_SENSOR3)
        #define MTOUCH_LAT_C_SENSOR4            __paste4(PORT,MTOUCH_LETTER_SENSOR4)
        #define MTOUCH_LAT_C_SENSOR5            __paste4(PORT,MTOUCH_LETTER_SENSOR5)
        #define MTOUCH_LAT_C_SENSOR6            __paste4(PORT,MTOUCH_LETTER_SENSOR6)
        #define MTOUCH_LAT_C_SENSOR7            __paste4(PORT,MTOUCH_LETTER_SENSOR7)
        #define MTOUCH_LAT_C_SENSOR8            __paste4(PORT,MTOUCH_LETTER_SENSOR8)
        #define MTOUCH_LAT_C_SENSOR9            __paste4(PORT,MTOUCH_LETTER_SENSOR9)
        #define MTOUCH_LAT_C_SENSOR10           __paste4(PORT,MTOUCH_LETTER_SENSOR10)
        #define MTOUCH_LAT_C_SENSOR11           __paste4(PORT,MTOUCH_LETTER_SENSOR11)
        #define MTOUCH_LAT_C_SENSOR12           __paste4(PORT,MTOUCH_LETTER_SENSOR12)
        #define MTOUCH_LAT_C_SENSOR13           __paste4(PORT,MTOUCH_LETTER_SENSOR13)
        #define MTOUCH_LAT_C_SENSOR14           __paste4(PORT,MTOUCH_LETTER_SENSOR14)
        #define MTOUCH_LAT_C_SENSOR15           __paste4(PORT,MTOUCH_LETTER_SENSOR15)
        #define MTOUCH_LAT_C_SENSOR16           __paste4(PORT,MTOUCH_LETTER_SENSOR16)
        #define MTOUCH_LAT_C_SENSOR17           __paste4(PORT,MTOUCH_LETTER_SENSOR17)
        #define MTOUCH_LAT_C_SENSOR18           __paste4(PORT,MTOUCH_LETTER_SENSOR18)
        #define MTOUCH_LAT_C_SENSOR19           __paste4(PORT,MTOUCH_LETTER_SENSOR19)
        #define MTOUCH_LAT_C_SENSOR20           __paste4(PORT,MTOUCH_LETTER_SENSOR20)
        #define MTOUCH_LAT_C_SENSOR21           __paste4(PORT,MTOUCH_LETTER_SENSOR21)
        #define MTOUCH_LAT_C_SENSOR22           __paste4(PORT,MTOUCH_LETTER_SENSOR22)
        #define MTOUCH_LAT_C_SENSOR23           __paste4(PORT,MTOUCH_LETTER_SENSOR23)
        #define MTOUCH_LAT_C_SENSOR24           __paste4(PORT,MTOUCH_LETTER_SENSOR24)
        #define MTOUCH_LAT_C_SENSOR25           __paste4(PORT,MTOUCH_LETTER_SENSOR25)
        #define MTOUCH_LAT_C_SENSOR26           __paste4(PORT,MTOUCH_LETTER_SENSOR26)
        #define MTOUCH_LAT_C_SENSOR27           __paste4(PORT,MTOUCH_LETTER_SENSOR27)
        #define MTOUCH_LAT_C_SENSOR28           __paste4(PORT,MTOUCH_LETTER_SENSOR28)
        #define MTOUCH_LAT_C_SENSOR29           __paste4(PORT,MTOUCH_LETTER_SENSOR29)
        #define MTOUCH_LAT_C_SENSOR(index)      __paste3(MTOUCH_LAT_C_SENSOR, index)

        #define MTOUCH_LAT_ASM_SENSOR0          __paste4(PORT,MTOUCH_LETTER_SENSOR0)
        #define MTOUCH_LAT_ASM_SENSOR1          __paste4(PORT,MTOUCH_LETTER_SENSOR1)
        #define MTOUCH_LAT_ASM_SENSOR2          __paste4(PORT,MTOUCH_LETTER_SENSOR2)
        #define MTOUCH_LAT_ASM_SENSOR3          __paste4(PORT,MTOUCH_LETTER_SENSOR3)
        #define MTOUCH_LAT_ASM_SENSOR4          __paste4(PORT,MTOUCH_LETTER_SENSOR4)
        #define MTOUCH_LAT_ASM_SENSOR5          __paste4(PORT,MTOUCH_LETTER_SENSOR5)
        #define MTOUCH_LAT_ASM_SENSOR6          __paste4(PORT,MTOUCH_LETTER_SENSOR6)
        #define MTOUCH_LAT_ASM_SENSOR7          __paste4(PORT,MTOUCH_LETTER_SENSOR7)
        #define MTOUCH_LAT_ASM_SENSOR8          __paste4(PORT,MTOUCH_LETTER_SENSOR8)
        #define MTOUCH_LAT_ASM_SENSOR9          __paste4(PORT,MTOUCH_LETTER_SENSOR9)
        #define MTOUCH_LAT_ASM_SENSOR10         __paste4(PORT,MTOUCH_LETTER_SENSOR10)
        #define MTOUCH_LAT_ASM_SENSOR11         __paste4(PORT,MTOUCH_LETTER_SENSOR11)
        #define MTOUCH_LAT_ASM_SENSOR12         __paste4(PORT,MTOUCH_LETTER_SENSOR12)
        #define MTOUCH_LAT_ASM_SENSOR13         __paste4(PORT,MTOUCH_LETTER_SENSOR13)
        #define MTOUCH_LAT_ASM_SENSOR14         __paste4(PORT,MTOUCH_LETTER_SENSOR14)
        #define MTOUCH_LAT_ASM_SENSOR15         __paste4(PORT,MTOUCH_LETTER_SENSOR15)
        #define MTOUCH_LAT_ASM_SENSOR16         __paste4(PORT,MTOUCH_LETTER_SENSOR16)
        #define MTOUCH_LAT_ASM_SENSOR17         __paste4(PORT,MTOUCH_LETTER_SENSOR17)
        #define MTOUCH_LAT_ASM_SENSOR18         __paste4(PORT,MTOUCH_LETTER_SENSOR18)
        #define MTOUCH_LAT_ASM_SENSOR19         __paste4(PORT,MTOUCH_LETTER_SENSOR19)
        #define MTOUCH_LAT_ASM_SENSOR20         __paste4(PORT,MTOUCH_LETTER_SENSOR20)
        #define MTOUCH_LAT_ASM_SENSOR21         __paste4(PORT,MTOUCH_LETTER_SENSOR21)
        #define MTOUCH_LAT_ASM_SENSOR22         __paste4(PORT,MTOUCH_LETTER_SENSOR22)
        #define MTOUCH_LAT_ASM_SENSOR23         __paste4(PORT,MTOUCH_LETTER_SENSOR23)
        #define MTOUCH_LAT_ASM_SENSOR24         __paste4(PORT,MTOUCH_LETTER_SENSOR24)
        #define MTOUCH_LAT_ASM_SENSOR25         __paste4(PORT,MTOUCH_LETTER_SENSOR25)
        #define MTOUCH_LAT_ASM_SENSOR26         __paste4(PORT,MTOUCH_LETTER_SENSOR26)
        #define MTOUCH_LAT_ASM_SENSOR27         __paste4(PORT,MTOUCH_LETTER_SENSOR27)
        #define MTOUCH_LAT_ASM_SENSOR28         __paste4(PORT,MTOUCH_LETTER_SENSOR28)
        #define MTOUCH_LAT_ASM_SENSOR29         __paste4(PORT,MTOUCH_LETTER_SENSOR29)
        #define MTOUCH_LAT_ASM_SENSOR(index)    __paste3(MTOUCH_LAT_ASM_SENSOR, index)
    #elif !defined(MTOUCH_USE_GPIO)
        #define MTOUCH_LAT_C_SENSOR0            __paste4(LAT,MTOUCH_LETTER_SENSOR0)
        #define MTOUCH_LAT_C_SENSOR1            __paste4(LAT,MTOUCH_LETTER_SENSOR1)
        #define MTOUCH_LAT_C_SENSOR2            __paste4(LAT,MTOUCH_LETTER_SENSOR2)
        #define MTOUCH_LAT_C_SENSOR3            __paste4(LAT,MTOUCH_LETTER_SENSOR3)
        #define MTOUCH_LAT_C_SENSOR4            __paste4(LAT,MTOUCH_LETTER_SENSOR4)
        #define MTOUCH_LAT_C_SENSOR5            __paste4(LAT,MTOUCH_LETTER_SENSOR5)
        #define MTOUCH_LAT_C_SENSOR6            __paste4(LAT,MTOUCH_LETTER_SENSOR6)
        #define MTOUCH_LAT_C_SENSOR7            __paste4(LAT,MTOUCH_LETTER_SENSOR7)
        #define MTOUCH_LAT_C_SENSOR8            __paste4(LAT,MTOUCH_LETTER_SENSOR8)
        #define MTOUCH_LAT_C_SENSOR9            __paste4(LAT,MTOUCH_LETTER_SENSOR9)
        #define MTOUCH_LAT_C_SENSOR10           __paste4(LAT,MTOUCH_LETTER_SENSOR10)
        #define MTOUCH_LAT_C_SENSOR11           __paste4(LAT,MTOUCH_LETTER_SENSOR11)
        #define MTOUCH_LAT_C_SENSOR12           __paste4(LAT,MTOUCH_LETTER_SENSOR12)
        #define MTOUCH_LAT_C_SENSOR13           __paste4(LAT,MTOUCH_LETTER_SENSOR13)
        #define MTOUCH_LAT_C_SENSOR14           __paste4(LAT,MTOUCH_LETTER_SENSOR14)
        #define MTOUCH_LAT_C_SENSOR15           __paste4(LAT,MTOUCH_LETTER_SENSOR15)
        #define MTOUCH_LAT_C_SENSOR16           __paste4(LAT,MTOUCH_LETTER_SENSOR16)
        #define MTOUCH_LAT_C_SENSOR17           __paste4(LAT,MTOUCH_LETTER_SENSOR17)
        #define MTOUCH_LAT_C_SENSOR18           __paste4(LAT,MTOUCH_LETTER_SENSOR18)
        #define MTOUCH_LAT_C_SENSOR19           __paste4(LAT,MTOUCH_LETTER_SENSOR19)
        #define MTOUCH_LAT_C_SENSOR20           __paste4(LAT,MTOUCH_LETTER_SENSOR20)
        #define MTOUCH_LAT_C_SENSOR21           __paste4(LAT,MTOUCH_LETTER_SENSOR21)
        #define MTOUCH_LAT_C_SENSOR22           __paste4(LAT,MTOUCH_LETTER_SENSOR22)
        #define MTOUCH_LAT_C_SENSOR23           __paste4(LAT,MTOUCH_LETTER_SENSOR23)
        #define MTOUCH_LAT_C_SENSOR24           __paste4(LAT,MTOUCH_LETTER_SENSOR24)
        #define MTOUCH_LAT_C_SENSOR25           __paste4(LAT,MTOUCH_LETTER_SENSOR25)
        #define MTOUCH_LAT_C_SENSOR26           __paste4(LAT,MTOUCH_LETTER_SENSOR26)
        #define MTOUCH_LAT_C_SENSOR27           __paste4(LAT,MTOUCH_LETTER_SENSOR27)
        #define MTOUCH_LAT_C_SENSOR28           __paste4(LAT,MTOUCH_LETTER_SENSOR28)
        #define MTOUCH_LAT_C_SENSOR29           __paste4(LAT,MTOUCH_LETTER_SENSOR29)
        #define MTOUCH_LAT_C_SENSOR(index)      __paste3(MTOUCH_LAT_C_SENSOR, index)

        #define MTOUCH_LAT_ASM_SENSOR0          __paste4(LAT,MTOUCH_LETTER_SENSOR0)
        #define MTOUCH_LAT_ASM_SENSOR1          __paste4(LAT,MTOUCH_LETTER_SENSOR1)
        #define MTOUCH_LAT_ASM_SENSOR2          __paste4(LAT,MTOUCH_LETTER_SENSOR2)
        #define MTOUCH_LAT_ASM_SENSOR3          __paste4(LAT,MTOUCH_LETTER_SENSOR3)
        #define MTOUCH_LAT_ASM_SENSOR4          __paste4(LAT,MTOUCH_LETTER_SENSOR4)
        #define MTOUCH_LAT_ASM_SENSOR5          __paste4(LAT,MTOUCH_LETTER_SENSOR5)
        #define MTOUCH_LAT_ASM_SENSOR6          __paste4(LAT,MTOUCH_LETTER_SENSOR6)
        #define MTOUCH_LAT_ASM_SENSOR7          __paste4(LAT,MTOUCH_LETTER_SENSOR7)
        #define MTOUCH_LAT_ASM_SENSOR8          __paste4(LAT,MTOUCH_LETTER_SENSOR8)
        #define MTOUCH_LAT_ASM_SENSOR9          __paste4(LAT,MTOUCH_LETTER_SENSOR9)
        #define MTOUCH_LAT_ASM_SENSOR10         __paste4(LAT,MTOUCH_LETTER_SENSOR10)
        #define MTOUCH_LAT_ASM_SENSOR11         __paste4(LAT,MTOUCH_LETTER_SENSOR11)
        #define MTOUCH_LAT_ASM_SENSOR12         __paste4(LAT,MTOUCH_LETTER_SENSOR12)
        #define MTOUCH_LAT_ASM_SENSOR13         __paste4(LAT,MTOUCH_LETTER_SENSOR13)
        #define MTOUCH_LAT_ASM_SENSOR14         __paste4(LAT,MTOUCH_LETTER_SENSOR14)
        #define MTOUCH_LAT_ASM_SENSOR15         __paste4(LAT,MTOUCH_LETTER_SENSOR15)
        #define MTOUCH_LAT_ASM_SENSOR16         __paste4(LAT,MTOUCH_LETTER_SENSOR16)
        #define MTOUCH_LAT_ASM_SENSOR17         __paste4(LAT,MTOUCH_LETTER_SENSOR17)
        #define MTOUCH_LAT_ASM_SENSOR18         __paste4(LAT,MTOUCH_LETTER_SENSOR18)
        #define MTOUCH_LAT_ASM_SENSOR19         __paste4(LAT,MTOUCH_LETTER_SENSOR19)
        #define MTOUCH_LAT_ASM_SENSOR20         __paste4(LAT,MTOUCH_LETTER_SENSOR20)
        #define MTOUCH_LAT_ASM_SENSOR21         __paste4(LAT,MTOUCH_LETTER_SENSOR21)
        #define MTOUCH_LAT_ASM_SENSOR22         __paste4(LAT,MTOUCH_LETTER_SENSOR22)
        #define MTOUCH_LAT_ASM_SENSOR23         __paste4(LAT,MTOUCH_LETTER_SENSOR23)
        #define MTOUCH_LAT_ASM_SENSOR24         __paste4(LAT,MTOUCH_LETTER_SENSOR24)
        #define MTOUCH_LAT_ASM_SENSOR25         __paste4(LAT,MTOUCH_LETTER_SENSOR25)
        #define MTOUCH_LAT_ASM_SENSOR26         __paste4(LAT,MTOUCH_LETTER_SENSOR26)
        #define MTOUCH_LAT_ASM_SENSOR27         __paste4(LAT,MTOUCH_LETTER_SENSOR27)
        #define MTOUCH_LAT_ASM_SENSOR28         __paste4(LAT,MTOUCH_LETTER_SENSOR28)
        #define MTOUCH_LAT_ASM_SENSOR29         __paste4(LAT,MTOUCH_LETTER_SENSOR29)
        #define MTOUCH_LAT_ASM_SENSOR(index)    __paste3(MTOUCH_LAT_ASM_SENSOR, index)
    #endif

    #define MTOUCH_TRIS_C_SENSOR0           __paste4(TRIS,MTOUCH_LETTER_SENSOR0)
    #define MTOUCH_TRIS_C_SENSOR1           __paste4(TRIS,MTOUCH_LETTER_SENSOR1)
    #define MTOUCH_TRIS_C_SENSOR2           __paste4(TRIS,MTOUCH_LETTER_SENSOR2)
    #define MTOUCH_TRIS_C_SENSOR3           __paste4(TRIS,MTOUCH_LETTER_SENSOR3)
    #define MTOUCH_TRIS_C_SENSOR4           __paste4(TRIS,MTOUCH_LETTER_SENSOR4)
    #define MTOUCH_TRIS_C_SENSOR5           __paste4(TRIS,MTOUCH_LETTER_SENSOR5)
    #define MTOUCH_TRIS_C_SENSOR6           __paste4(TRIS,MTOUCH_LETTER_SENSOR6)
    #define MTOUCH_TRIS_C_SENSOR7           __paste4(TRIS,MTOUCH_LETTER_SENSOR7)
    #define MTOUCH_TRIS_C_SENSOR8           __paste4(TRIS,MTOUCH_LETTER_SENSOR8)
    #define MTOUCH_TRIS_C_SENSOR9           __paste4(TRIS,MTOUCH_LETTER_SENSOR9)
    #define MTOUCH_TRIS_C_SENSOR10          __paste4(TRIS,MTOUCH_LETTER_SENSOR10)
    #define MTOUCH_TRIS_C_SENSOR11          __paste4(TRIS,MTOUCH_LETTER_SENSOR11)
    #define MTOUCH_TRIS_C_SENSOR12          __paste4(TRIS,MTOUCH_LETTER_SENSOR12)
    #define MTOUCH_TRIS_C_SENSOR13          __paste4(TRIS,MTOUCH_LETTER_SENSOR13)
    #define MTOUCH_TRIS_C_SENSOR14          __paste4(TRIS,MTOUCH_LETTER_SENSOR14)
    #define MTOUCH_TRIS_C_SENSOR15          __paste4(TRIS,MTOUCH_LETTER_SENSOR15)
    #define MTOUCH_TRIS_C_SENSOR16          __paste4(TRIS,MTOUCH_LETTER_SENSOR16)
    #define MTOUCH_TRIS_C_SENSOR17          __paste4(TRIS,MTOUCH_LETTER_SENSOR17)
    #define MTOUCH_TRIS_C_SENSOR18          __paste4(TRIS,MTOUCH_LETTER_SENSOR18)
    #define MTOUCH_TRIS_C_SENSOR19          __paste4(TRIS,MTOUCH_LETTER_SENSOR19)
    #define MTOUCH_TRIS_C_SENSOR20          __paste4(TRIS,MTOUCH_LETTER_SENSOR20)
    #define MTOUCH_TRIS_C_SENSOR21          __paste4(TRIS,MTOUCH_LETTER_SENSOR21)
    #define MTOUCH_TRIS_C_SENSOR22          __paste4(TRIS,MTOUCH_LETTER_SENSOR22)
    #define MTOUCH_TRIS_C_SENSOR23          __paste4(TRIS,MTOUCH_LETTER_SENSOR23)
    #define MTOUCH_TRIS_C_SENSOR24          __paste4(TRIS,MTOUCH_LETTER_SENSOR24)
    #define MTOUCH_TRIS_C_SENSOR25          __paste4(TRIS,MTOUCH_LETTER_SENSOR25)
    #define MTOUCH_TRIS_C_SENSOR26          __paste4(TRIS,MTOUCH_LETTER_SENSOR26)
    #define MTOUCH_TRIS_C_SENSOR27          __paste4(TRIS,MTOUCH_LETTER_SENSOR27)
    #define MTOUCH_TRIS_C_SENSOR28          __paste4(TRIS,MTOUCH_LETTER_SENSOR28)
    #define MTOUCH_TRIS_C_SENSOR29          __paste4(TRIS,MTOUCH_LETTER_SENSOR29)
    #define MTOUCH_TRIS_C_SENSOR(index)     __paste3(MTOUCH_TRIS_C_SENSOR, index)

    #define MTOUCH_TRIS_ASM_SENSOR0         __paste4(TRIS,MTOUCH_LETTER_SENSOR0)
    #define MTOUCH_TRIS_ASM_SENSOR1         __paste4(TRIS,MTOUCH_LETTER_SENSOR1)
    #define MTOUCH_TRIS_ASM_SENSOR2         __paste4(TRIS,MTOUCH_LETTER_SENSOR2)
    #define MTOUCH_TRIS_ASM_SENSOR3         __paste4(TRIS,MTOUCH_LETTER_SENSOR3)
    #define MTOUCH_TRIS_ASM_SENSOR4         __paste4(TRIS,MTOUCH_LETTER_SENSOR4)
    #define MTOUCH_TRIS_ASM_SENSOR5         __paste4(TRIS,MTOUCH_LETTER_SENSOR5)
    #define MTOUCH_TRIS_ASM_SENSOR6         __paste4(TRIS,MTOUCH_LETTER_SENSOR6)
    #define MTOUCH_TRIS_ASM_SENSOR7         __paste4(TRIS,MTOUCH_LETTER_SENSOR7)
    #define MTOUCH_TRIS_ASM_SENSOR8         __paste4(TRIS,MTOUCH_LETTER_SENSOR8)
    #define MTOUCH_TRIS_ASM_SENSOR9         __paste4(TRIS,MTOUCH_LETTER_SENSOR9)
    #define MTOUCH_TRIS_ASM_SENSOR10        __paste4(TRIS,MTOUCH_LETTER_SENSOR10)
    #define MTOUCH_TRIS_ASM_SENSOR11        __paste4(TRIS,MTOUCH_LETTER_SENSOR11)
    #define MTOUCH_TRIS_ASM_SENSOR12        __paste4(TRIS,MTOUCH_LETTER_SENSOR12)
    #define MTOUCH_TRIS_ASM_SENSOR13        __paste4(TRIS,MTOUCH_LETTER_SENSOR13)
    #define MTOUCH_TRIS_ASM_SENSOR14        __paste4(TRIS,MTOUCH_LETTER_SENSOR14)
    #define MTOUCH_TRIS_ASM_SENSOR15        __paste4(TRIS,MTOUCH_LETTER_SENSOR15)
    #define MTOUCH_TRIS_ASM_SENSOR16        __paste4(TRIS,MTOUCH_LETTER_SENSOR16)
    #define MTOUCH_TRIS_ASM_SENSOR17        __paste4(TRIS,MTOUCH_LETTER_SENSOR17)
    #define MTOUCH_TRIS_ASM_SENSOR18        __paste4(TRIS,MTOUCH_LETTER_SENSOR18)
    #define MTOUCH_TRIS_ASM_SENSOR19        __paste4(TRIS,MTOUCH_LETTER_SENSOR19)
    #define MTOUCH_TRIS_ASM_SENSOR20        __paste4(TRIS,MTOUCH_LETTER_SENSOR20)
    #define MTOUCH_TRIS_ASM_SENSOR21        __paste4(TRIS,MTOUCH_LETTER_SENSOR21)
    #define MTOUCH_TRIS_ASM_SENSOR22        __paste4(TRIS,MTOUCH_LETTER_SENSOR22)
    #define MTOUCH_TRIS_ASM_SENSOR23        __paste4(TRIS,MTOUCH_LETTER_SENSOR23)
    #define MTOUCH_TRIS_ASM_SENSOR24        __paste4(TRIS,MTOUCH_LETTER_SENSOR24)
    #define MTOUCH_TRIS_ASM_SENSOR25        __paste4(TRIS,MTOUCH_LETTER_SENSOR25)
    #define MTOUCH_TRIS_ASM_SENSOR26        __paste4(TRIS,MTOUCH_LETTER_SENSOR26)
    #define MTOUCH_TRIS_ASM_SENSOR27        __paste4(TRIS,MTOUCH_LETTER_SENSOR27)
    #define MTOUCH_TRIS_ASM_SENSOR28        __paste4(TRIS,MTOUCH_LETTER_SENSOR28)
    #define MTOUCH_TRIS_ASM_SENSOR29        __paste4(TRIS,MTOUCH_LETTER_SENSOR29)
    #define MTOUCH_TRIS_ASM_SENSOR(index)   __paste3(MTOUCH_TRIS_ASM_SENSOR, index)

    #define MTOUCH_PIN_SENSOR0              __paste4(MTOUCH_PIN_,MTOUCH_SENSOR0)
    #define MTOUCH_PIN_SENSOR1              __paste4(MTOUCH_PIN_,MTOUCH_SENSOR1)
    #define MTOUCH_PIN_SENSOR2              __paste4(MTOUCH_PIN_,MTOUCH_SENSOR2)
    #define MTOUCH_PIN_SENSOR3              __paste4(MTOUCH_PIN_,MTOUCH_SENSOR3)
    #define MTOUCH_PIN_SENSOR4              __paste4(MTOUCH_PIN_,MTOUCH_SENSOR4)
    #define MTOUCH_PIN_SENSOR5              __paste4(MTOUCH_PIN_,MTOUCH_SENSOR5)
    #define MTOUCH_PIN_SENSOR6              __paste4(MTOUCH_PIN_,MTOUCH_SENSOR6)
    #define MTOUCH_PIN_SENSOR7              __paste4(MTOUCH_PIN_,MTOUCH_SENSOR7)
    #define MTOUCH_PIN_SENSOR8              __paste4(MTOUCH_PIN_,MTOUCH_SENSOR8)
    #define MTOUCH_PIN_SENSOR9              __paste4(MTOUCH_PIN_,MTOUCH_SENSOR9)
    #define MTOUCH_PIN_SENSOR10             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR10)
    #define MTOUCH_PIN_SENSOR11             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR11)
    #define MTOUCH_PIN_SENSOR12             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR12)
    #define MTOUCH_PIN_SENSOR13             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR13)
    #define MTOUCH_PIN_SENSOR14             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR14)
    #define MTOUCH_PIN_SENSOR15             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR15)
    #define MTOUCH_PIN_SENSOR16             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR16)
    #define MTOUCH_PIN_SENSOR17             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR17)
    #define MTOUCH_PIN_SENSOR18             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR18)
    #define MTOUCH_PIN_SENSOR19             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR19)
    #define MTOUCH_PIN_SENSOR20             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR20)
    #define MTOUCH_PIN_SENSOR21             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR21)
    #define MTOUCH_PIN_SENSOR22             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR22)
    #define MTOUCH_PIN_SENSOR23             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR23)
    #define MTOUCH_PIN_SENSOR24             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR24)
    #define MTOUCH_PIN_SENSOR25             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR25)
    #define MTOUCH_PIN_SENSOR26             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR26)
    #define MTOUCH_PIN_SENSOR27             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR27)
    #define MTOUCH_PIN_SENSOR28             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR28)
    #define MTOUCH_PIN_SENSOR29             __paste4(MTOUCH_PIN_,MTOUCH_SENSOR29)
    #define MTOUCH_PIN_REFERENCE            __paste4(MTOUCH_PIN_,CVD_REFERENCE)
    #define MTOUCH_PIN_SENSOR(index)        __paste3(MTOUCH_PIN_SENSOR, index)
        
    #define MTOUCH_SELECT_SENSOR0	        __paste4(MTOUCH_AD_, MTOUCH_SENSOR0)
    #define MTOUCH_SELECT_SENSOR1	        __paste4(MTOUCH_AD_, MTOUCH_SENSOR1)
    #define MTOUCH_SELECT_SENSOR2	        __paste4(MTOUCH_AD_, MTOUCH_SENSOR2)
    #define MTOUCH_SELECT_SENSOR3	        __paste4(MTOUCH_AD_, MTOUCH_SENSOR3)
    #define MTOUCH_SELECT_SENSOR4	        __paste4(MTOUCH_AD_, MTOUCH_SENSOR4)
    #define MTOUCH_SELECT_SENSOR5	        __paste4(MTOUCH_AD_, MTOUCH_SENSOR5)
    #define MTOUCH_SELECT_SENSOR6	        __paste4(MTOUCH_AD_, MTOUCH_SENSOR6)
    #define MTOUCH_SELECT_SENSOR7	        __paste4(MTOUCH_AD_, MTOUCH_SENSOR7)
    #define MTOUCH_SELECT_SENSOR8	        __paste4(MTOUCH_AD_, MTOUCH_SENSOR8)
    #define MTOUCH_SELECT_SENSOR9	        __paste4(MTOUCH_AD_, MTOUCH_SENSOR9)
    #define MTOUCH_SELECT_SENSOR10          __paste4(MTOUCH_AD_, MTOUCH_SENSOR10)
    #define MTOUCH_SELECT_SENSOR11          __paste4(MTOUCH_AD_, MTOUCH_SENSOR11)
    #define MTOUCH_SELECT_SENSOR12          __paste4(MTOUCH_AD_, MTOUCH_SENSOR12)
    #define MTOUCH_SELECT_SENSOR13          __paste4(MTOUCH_AD_, MTOUCH_SENSOR13)
    #define MTOUCH_SELECT_SENSOR14          __paste4(MTOUCH_AD_, MTOUCH_SENSOR14)
    #define MTOUCH_SELECT_SENSOR15          __paste4(MTOUCH_AD_, MTOUCH_SENSOR15)
    #define MTOUCH_SELECT_SENSOR16          __paste4(MTOUCH_AD_, MTOUCH_SENSOR16)
    #define MTOUCH_SELECT_SENSOR17          __paste4(MTOUCH_AD_, MTOUCH_SENSOR17)
    #define MTOUCH_SELECT_SENSOR18          __paste4(MTOUCH_AD_, MTOUCH_SENSOR18)
    #define MTOUCH_SELECT_SENSOR19          __paste4(MTOUCH_AD_, MTOUCH_SENSOR19)
    #define MTOUCH_SELECT_SENSOR20          __paste4(MTOUCH_AD_, MTOUCH_SENSOR20)
    #define MTOUCH_SELECT_SENSOR21          __paste4(MTOUCH_AD_, MTOUCH_SENSOR21)
    #define MTOUCH_SELECT_SENSOR22          __paste4(MTOUCH_AD_, MTOUCH_SENSOR22)
    #define MTOUCH_SELECT_SENSOR23          __paste4(MTOUCH_AD_, MTOUCH_SENSOR23)
    #define MTOUCH_SELECT_SENSOR24          __paste4(MTOUCH_AD_, MTOUCH_SENSOR24)
    #define MTOUCH_SELECT_SENSOR25          __paste4(MTOUCH_AD_, MTOUCH_SENSOR25)
    #define MTOUCH_SELECT_SENSOR26          __paste4(MTOUCH_AD_, MTOUCH_SENSOR26)
    #define MTOUCH_SELECT_SENSOR27          __paste4(MTOUCH_AD_, MTOUCH_SENSOR27)
    #define MTOUCH_SELECT_SENSOR28          __paste4(MTOUCH_AD_, MTOUCH_SENSOR28)
    #define MTOUCH_SELECT_SENSOR29          __paste4(MTOUCH_AD_, MTOUCH_SENSOR29)
    #define MTOUCH_SELECT_FVR_AND_GO        __paste4(MTOUCH_AD_, FVR_AND_GO)
    #define MTOUCH_SELECT_DAC_AND_GO        __paste4(MTOUCH_AD_, DAC_AND_GO)
    #define MTOUCH_SELECT_DAC_NOGO          __paste4(MTOUCH_AD_, DAC_NOGO)
    #define MTOUCH_SELECT_ISO_AND_GO        __paste4(MTOUCH_AD_, ISO_AND_GO) 
    #define MTOUCH_SELECT_ISO_NOGO          __paste4(MTOUCH_AD_, ISO_NOGO) 
    #define MTOUCH_SELECT_REFERENCE         __paste4(MTOUCH_AD_, CVD_REFERENCE)
    #define MTOUCH_SELECT_SENSOR(index)     __paste3(MTOUCH_SELECT_SENSOR, index)
    
    
//****************************************************************************************************
// __     __    _ _     _       _          ____             __ _                       _   _             
// \ \   / /_ _| (_) __| | __ _| |_ ___   / ___|___  _ __  / _(_) __ _ _   _ _ __ __ _| |_(_) ___  _ __  
//  \ \ / / _` | | |/ _` |/ _` | __/ _ \ | |   / _ \| '_ \| |_| |/ _` | | | | '__/ _` | __| |/ _ \| '_ \ 
//   \ V / (_| | | | (_| | (_| | ||  __/ | |__| (_) | | | |  _| | (_| | |_| | | | (_| | |_| | (_) | | | |
//    \_/ \__,_|_|_|\__,_|\__,_|\__\___|  \____\___/|_| |_|_| |_|\__, |\__,_|_|  \__,_|\__|_|\___/|_| |_|
//                                                               |___/    
//****************************************************************************************************
#if defined(MTOUCH_BOARD_REVISION) && defined(MTOUCH_SOFTWARE_REVISION)
    #define MTOUCH_FIRMWARE_VERSION    ((MTOUCH_BOARD_REVISION << 8) | MTOUCH_SOFTWARE_REVISION)
#endif

#if defined(MTOUCH_NUMBER_SENSORS) && (MTOUCH_NUMBER_SENSORS > 0)
    #if !defined(MTOUCH_INTEGRATION_TYPE)
        #define MTOUCH_INTEGRATION_TYPE     MTOUCH_CONTROLS_ISR
        #warning MTOUCH_INTEGRATION_TYPE not defined. The framework will attempt to control 100% of the ISR, by default.
    #else
        #if (MTOUCH_INTEGRATION_TYPE != MTOUCH_CONTROLS_ISR) && (MTOUCH_INTEGRATION_TYPE != MTOUCH_CALLED_FROM_ISR) && (MTOUCH_INTEGRATION_TYPE != MTOUCH_CALLED_FROM_MAINLOOP)
        #undef MTOUCH_INTEGRATION_TYPE
        #define MTOUCH_INTEGRATION_TYPE     MTOUCH_CONTROLS_ISR
        #warning MTOUCH_INTEGRATION_TYPE defined to an invalid value. Defaulting to MTOUCH_CONTROLS_ISR. The framework will attempt to control 100% of the ISR.
        #endif
    #endif
#else
    #error You must have at least one mTouch sensor enabled. Please use mTouch_config.h to reconfigure the mTouch framework's sensors.
#endif


#if !defined(MTOUCH_RELEASE_THRESH_FACTOR)
    #warning MTOUCH_RELEASE_THRESH_FACTOR must be defined in the configuration file. Setting to '0.5'.
    #define MTOUCH_RELEASE_THRESH_FACTOR 0.5
#endif
#if defined(MTOUCH_BASELINE_WEIGHT)
    #if ((MTOUCH_BASELINE_WEIGHT != 1) && (MTOUCH_BASELINE_WEIGHT != 2) && (MTOUCH_BASELINE_WEIGHT != 3) && (MTOUCH_BASELINE_WEIGHT != 4))
        #warning Invalid value for MTOUCH_BASELINE_WEIGHT. Setting to default value of 3.
        #undef  MTOUCH_BASELINE_WEIGHT
        #define MTOUCH_BASELINE_WEIGHT 3
    #endif
#endif

#if !defined(MTOUCH_POWER_UP_SAMPLES)
    #warning MTOUCH_POWER_UP_SAMPLES must be defined in the configuration file. Setting to '50'.
    #define MTOUCH_POWER_UP_SAMPLES 50
#else
    #if MTOUCH_POWER_UP_SAMPLES > 65535
        #warning MTOUCH_POWER_UP_SAMPLES exceeds the maximum limit of 65535. Setting to '65535'.
        #undef MTOUCH_POWER_UP_SAMPLES
        #define MTOUCH_POWER_UP_SAMPLES 65535
    #endif
    #if MTOUCH_POWER_UP_SAMPLES < 1
        #warning MTOUCH_POWER_UP_SAMPLES must be at least '1'. Setting to '1'.
        #undef MTOUCH_POWER_UP_SAMPLES
        #define MTOUCH_POWER_UP_SAMPLES 1
    #endif
#endif
#if defined (MTOUCH_BUTTON_TIMEOUT)
    #if MTOUCH_BUTTON_TIMEOUT > 65535
        #warning MTOUCH_BUTTON_TIMEOUT exceeds the maximum limit of 65535. Setting to '65535'.
        #undef MTOUCH_BUTTON_TIMEOUT
        #define MTOUCH_BUTTON_TIMEOUT 65535
    #endif
#endif
#if defined(MTOUCH_BUTTON_TIMEOUT)
    #if MTOUCH_BUTTON_TIMEOUT > MTOUCH_POWER_UP_SAMPLES
    #define CVD_TIMER_MAX  MTOUCH_BUTTON_TIMEOUT
    #else
    #define CVD_TIMER_MAX  MTOUCH_POWER_UP_SAMPLES
    #endif
#else
#define CVD_TIMER_MAX  MTOUCH_POWER_UP_SAMPLES
#endif
#if CVD_TIMER_MAX > 0
    #if CVD_TIMER_MAX > 1
        #if CVD_TIMER_MAX > 3
            #if CVD_TIMER_MAX > 7
                #if CVD_TIMER_MAX > 15
                    #if CVD_TIMER_MAX > 31
                        #if CVD_TIMER_MAX > 63
                            #if CVD_TIMER_MAX > 127
                                #if CVD_TIMER_MAX > 255
                                    #if CVD_TIMER_MAX > 511
                                        #if CVD_TIMER_MAX > 1023
                                            #if CVD_TIMER_MAX > 2047
                                                #if CVD_TIMER_MAX > 4096
                                                    #if CVD_TIMER_MAX > 8192
                                                        #if CVD_TIMER_MAX > 16384
                                                            #if CVD_TIMER_MAX > 32768
                                                                #define CVD_STATE_TIMER_BITS 16
                                                            #else
                                                            #define CVD_STATE_TIMER_BITS 15
                                                            #endif
                                                        #else
                                                        #define CVD_STATE_TIMER_BITS 14
                                                        #endif
                                                    #else
                                                    #define CVD_STATE_TIMER_BITS 13
                                                    #endif
                                                #else
                                                #define CVD_STATE_TIMER_BITS 12
                                                #endif
                                            #else
                                            #define CVD_STATE_TIMER_BITS 11
                                            #endif
                                        #else
                                        #define CVD_STATE_TIMER_BITS 10
                                        #endif
                                    #else
                                    #define CVD_STATE_TIMER_BITS 9
                                    #endif
                                #else
                                #define CVD_STATE_TIMER_BITS 8
                                #endif
                            #else
                            #define CVD_STATE_TIMER_BITS 7
                            #endif
                        #else
                        #define CVD_STATE_TIMER_BITS 6
                        #endif
                    #else
                    #define CVD_STATE_TIMER_BITS 5
                    #endif
                #else
                #define CVD_STATE_TIMER_BITS 4
                #endif
            #else
            #define CVD_STATE_TIMER_BITS 3
            #endif
        #else
        #define CVD_STATE_TIMER_BITS 2
        #endif
    #else
    #define CVD_STATE_TIMER_BITS 1
    #endif
#else
#define CVD_STATE_TIMER_BITS 0
#endif
#if ( (CVD_STATE_TIMER_BITS > 8) || defined(MTOUCH_EEPROM_ENABLED) )
    #define MTOUCH_STATE_TIMER()               uint16_t timer
#else
    #define MTOUCH_STATE_TIMER()               __paste(unsigned timer  :,CVD_STATE_TIMER_BITS)
#endif

#if !defined(MTOUCH_DEBOUNCE_RELEASE)
    #define MTOUCH_DEBOUNCE_RELEASE 0
#else
    #if MTOUCH_DEBOUNCE_RELEASE > 65535
        #warning MTOUCH_DEBOUNCE_RELEASE exceeds the maximum limit of 65535. Setting to '65535'.
        #undef MTOUCH_DEBOUNCE_RELEASE
        #define MTOUCH_DEBOUNCE_RELEASE 65535
    #endif
    #if MTOUCH_DEBOUNCE_RELEASE < 0
        #warning MTOUCH_DEBOUNCE_RELEASE must be at least '0'. Setting to '0'.
        #undef MTOUCH_DEBOUNCE_RELEASE
        #define MTOUCH_DEBOUNCE_RELEASE 0
    #endif
#endif
#if !defined(MTOUCH_DEBOUNCE_PRESS)
    #define MTOUCH_DEBOUNCE_PRESS 0
#else
    #if MTOUCH_DEBOUNCE_PRESS > 65535
        #warning MTOUCH_DEBOUNCE_PRESS exceeds the maximum limit of 65535. Setting to '65535'.
        #undef MTOUCH_DEBOUNCE_PRESS
        #define MTOUCH_DEBOUNCE_PRESS 65535
    #endif
    #if MTOUCH_DEBOUNCE_PRESS < 0
        #warning MTOUCH_DEBOUNCE_PRESS must be at least '0'. Setting to '0'.
        #undef MTOUCH_DEBOUNCE_PRESS
        #define MTOUCH_DEBOUNCE_PRESS 0
    #endif
#endif
#if MTOUCH_DEBOUNCE_PRESS > MTOUCH_DEBOUNCE_RELEASE
    #define CVD_DEBOUNCE_MAX    MTOUCH_DEBOUNCE_PRESS
#else
    #define CVD_DEBOUNCE_MAX    MTOUCH_DEBOUNCE_RELEASE
#endif
#if CVD_DEBOUNCE_MAX > 0
    #if CVD_DEBOUNCE_MAX > 1
        #if CVD_DEBOUNCE_MAX > 3
            #if CVD_DEBOUNCE_MAX > 7
                #if CVD_DEBOUNCE_MAX > 15
                    #if CVD_DEBOUNCE_MAX > 31
                        #if CVD_DEBOUNCE_MAX > 63
                            #if CVD_DEBOUNCE_MAX > 127
                                #if CVD_DEBOUNCE_MAX > 255
                                    #if CVD_DEBOUNCE_MAX > 511
                                        #if CVD_DEBOUNCE_MAX > 1023
                                            #if CVD_DEBOUNCE_MAX > 2047
                                                #if CVD_DEBOUNCE_MAX > 4096
                                                    #if CVD_DEBOUNCE_MAX > 8192
                                                        #if CVD_DEBOUNCE_MAX > 16384
                                                            #if CVD_DEBOUNCE_MAX > 32768
                                                                #define CVD_STATE_DEBOUNCE_BITS 16
                                                            #else
                                                            #define CVD_STATE_DEBOUNCE_BITS 15
                                                            #endif
                                                        #else
                                                        #define CVD_STATE_DEBOUNCE_BITS 14
                                                        #endif
                                                    #else
                                                    #define CVD_STATE_DEBOUNCE_BITS 13
                                                    #endif
                                                #else
                                                #define CVD_STATE_DEBOUNCE_BITS 12
                                                #endif
                                            #else
                                            #define CVD_STATE_DEBOUNCE_BITS 11
                                            #endif
                                        #else
                                        #define CVD_STATE_DEBOUNCE_BITS 10
                                        #endif
                                    #else
                                    #define CVD_STATE_DEBOUNCE_BITS 9
                                    #endif
                                #else
                                #define CVD_STATE_DEBOUNCE_BITS 8
                                #endif
                            #else
                            #define CVD_STATE_DEBOUNCE_BITS 7
                            #endif
                        #else
                        #define CVD_STATE_DEBOUNCE_BITS 6
                        #endif
                    #else
                    #define CVD_STATE_DEBOUNCE_BITS 5
                    #endif
                #else
                #define CVD_STATE_DEBOUNCE_BITS 4
                #endif
            #else
            #define CVD_STATE_DEBOUNCE_BITS 3
            #endif
        #else
        #define CVD_STATE_DEBOUNCE_BITS 2
        #endif
    #else
    #define CVD_STATE_DEBOUNCE_BITS 1
    #endif
#else
#define CVD_STATE_DEBOUNCE_BITS 0
#endif
#if (CVD_STATE_DEBOUNCE_BITS > 8) || defined(MTOUCH_EEPROM_ENABLED)
    #define MTOUCH_STATE_DEBOUNCE()               uint16_t debounce
#elif CVD_STATE_DEBOUNCE_BITS != 0
    #define MTOUCH_STATE_DEBOUNCE()               __paste(unsigned debounce  :,CVD_STATE_DEBOUNCE_BITS)
#else
    #define MTOUCH_DEBOUNCE_IS_ZERO
#endif

#if !defined(MTOUCH_SAMPLES_PER_SCAN)
    #warning MTOUCH_SAMPLES_PER_SCAN must be defined in the configuration file. Defaulting to 20ms timing.
    #define MTOUCH_SAMPLES_PER_SCAN CVD_20ms_Timing
#endif
#if !defined(MTOUCH_SAMPLES_PER_SCAN)
    #warning MTOUCH_SAMPLES_PER_SCAN must be defined in the configuration file. Defaulting to 30 samples.
    #define MTOUCH_SAMPLES_PER_SCAN 30
#endif
#if MTOUCH_SAMPLES_PER_SCAN > 0
    #if MTOUCH_SAMPLES_PER_SCAN > 1
        #if MTOUCH_SAMPLES_PER_SCAN > 3
            #if MTOUCH_SAMPLES_PER_SCAN > 7
                #if MTOUCH_SAMPLES_PER_SCAN > 15
                    #if MTOUCH_SAMPLES_PER_SCAN > 31
                        #if MTOUCH_SAMPLES_PER_SCAN > 63
                            #if MTOUCH_SAMPLES_PER_SCAN > 127
                                #if MTOUCH_SAMPLES_PER_SCAN > 255
                                    #if MTOUCH_SAMPLES_PER_SCAN > 511
                                        #if MTOUCH_SAMPLES_PER_SCAN > 1023
                                            #if MTOUCH_SAMPLES_PER_SCAN > 2047
                                                #if MTOUCH_SAMPLES_PER_SCAN > 4096
                                                    #if MTOUCH_SAMPLES_PER_SCAN > 8192
                                                        #if MTOUCH_SAMPLES_PER_SCAN > 16384
                                                            #if MTOUCH_SAMPLES_PER_SCAN > 32768
                                                                #define CVD_STATE_SPS_BITS 16
                                                            #else
                                                            #define CVD_STATE_SPS_BITS 15
                                                            #endif
                                                        #else
                                                        #define CVD_STATE_SPS_BITS 14
                                                        #endif
                                                    #else
                                                    #define CVD_STATE_SPS_BITS 13
                                                    #endif
                                                #else
                                                #define CVD_STATE_SPS_BITS 12
                                                #endif
                                            #else
                                            #define CVD_STATE_SPS_BITS 11
                                            #endif
                                        #else
                                        #define CVD_STATE_SPS_BITS 10
                                        #endif
                                    #else
                                    #define CVD_STATE_SPS_BITS 9
                                    #endif
                                #else
                                #define CVD_STATE_SPS_BITS 8
                                #endif
                            #else
                            #define CVD_STATE_SPS_BITS 7
                            #endif
                        #else
                        #define CVD_STATE_SPS_BITS 6
                        #endif
                    #else
                    #define CVD_STATE_SPS_BITS 5
                    #endif
                #else
                #define CVD_STATE_SPS_BITS 4
                #endif
            #else
            #define CVD_STATE_SPS_BITS 3
            #endif
        #else
        #define CVD_STATE_SPS_BITS 2
        #endif
    #else
    #define CVD_STATE_SPS_BITS 1
    #endif
#else
#define CVD_STATE_SPS_BITS 0
#endif
#if (CVD_STATE_SPS_BITS > 8) || defined(MTOUCH_EEPROM_ENABLED)
    #define MTOUCH_STATE_SAMPLE_COUNTER()               uint16_t sampleCounter
#else
    #define MTOUCH_STATE_SAMPLE_COUNTER()               __paste(unsigned sampleCounter  :,CVD_STATE_SPS_BITS)
#endif

#if (defined(MTOUCH_JITTER_BITS) && (MTOUCH_JITTER_BITS != 0)) || defined(MTOUCH_EEPROM_ENABLED)
    #define MTOUCH_JITTER_ENABLE
#else
    #if defined(MTOUCH_JITTER_ENABLE)
    #undef MTOUCH_JITTER_ENABLE
    #endif
#endif

#if defined(MTOUCH_JITTER_ENABLE)
    #define MTOUCH_JITTER_MASK0         0x00
    #define MTOUCH_JITTER_MASK1         0x01
    #define MTOUCH_JITTER_MASK2         0x03
    #define MTOUCH_JITTER_MASK3         0x07
    #define MTOUCH_JITTER_MASK4         0x0F
    #define MTOUCH_JITTER_MASK5         0x1F
    #define MTOUCH_JITTER_MASK6         0x3F
    #define MTOUCH_JITTER_MASK7         0x7F
    #define MTOUCH_JITTER_MASK8         0xFF
    
    #define MTOUCH_JITTER_MASK          __paste4(MTOUCH_JITTER_MASK,MTOUCH_JITTER_BITS)
    #define MTOUCH_JITTER_MASK_ARRAY    { MTOUCH_JITTER_MASK0, MTOUCH_JITTER_MASK1, MTOUCH_JITTER_MASK2, MTOUCH_JITTER_MASK3, MTOUCH_JITTER_MASK4, MTOUCH_JITTER_MASK5, MTOUCH_JITTER_MASK6, MTOUCH_JITTER_MASK7, MTOUCH_JITTER_MASK8 } 
#endif

#if !defined(_XTAL_FREQ)
    #error _XTAL_FREQ is not defined in the configuration file. Framework must be told how fast Fosc is running.
#endif
#if !defined(MTOUCH_SENSOR0)
    #error There is no MTOUCH_SENSOR0 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
#endif
#if !defined(MTOUCH_NEGATIVE_CAPACITANCE)
    #define MTOUCH_NEGATIVE_CAPACITANCE 0
#endif
#if (MTOUCH_NEGATIVE_CAPACITANCE != 0) && (MTOUCH_NEGATIVE_CAPACITANCE != 1) && (MTOUCH_NEGATIVE_CAPACITANCE != 2)
    #warning MTOUCH_NEGATIVE_CAPACITANCE is set to an invalid value. Defaulting to 0.
    #undef MTOUCH_NEGATIVE_CAPACITANCE
    #define MTOUCH_NEGATIVE_CAPACITANCE 0
#endif
#if MTOUCH_BASELINE_RATE > 0
    #if MTOUCH_BASELINE_RATE > 1
        #if MTOUCH_BASELINE_RATE > 3
            #if MTOUCH_BASELINE_RATE > 7
                #if MTOUCH_BASELINE_RATE > 15
                    #if MTOUCH_BASELINE_RATE > 31
                        #if MTOUCH_BASELINE_RATE > 63
                            #if MTOUCH_BASELINE_RATE > 127
                                #if MTOUCH_BASELINE_RATE > 255
                                    #if MTOUCH_BASELINE_RATE > 511
                                        #if MTOUCH_BASELINE_RATE > 1023
                                            #if MTOUCH_BASELINE_RATE > 2047
                                                #if MTOUCH_BASELINE_RATE > 4096
                                                    #if MTOUCH_BASELINE_RATE > 8192
                                                        #if MTOUCH_BASELINE_RATE > 16384
                                                            #if MTOUCH_BASELINE_RATE > 32768
                                                                #define CVD_STATE_BLCOUNT_BITS 16
                                                            #else
                                                            #define CVD_STATE_BLCOUNT_BITS 15
                                                            #endif
                                                        #else
                                                        #define CVD_STATE_BLCOUNT_BITS 14
                                                        #endif
                                                    #else
                                                    #define CVD_STATE_BLCOUNT_BITS 13
                                                    #endif
                                                #else
                                                #define CVD_STATE_BLCOUNT_BITS 12
                                                #endif
                                            #else
                                            #define CVD_STATE_BLCOUNT_BITS 11
                                            #endif
                                        #else
                                        #define CVD_STATE_BLCOUNT_BITS 10
                                        #endif
                                    #else
                                    #define CVD_STATE_BLCOUNT_BITS 9
                                    #endif
                                #else
                                #define CVD_STATE_BLCOUNT_BITS 8
                                #endif
                            #else
                            #define CVD_STATE_BLCOUNT_BITS 7
                            #endif
                        #else
                        #define CVD_STATE_BLCOUNT_BITS 6
                        #endif
                    #else
                    #define CVD_STATE_BLCOUNT_BITS 5
                    #endif
                #else
                #define CVD_STATE_BLCOUNT_BITS 4
                #endif
            #else
            #define CVD_STATE_BLCOUNT_BITS 3
            #endif
        #else
        #define CVD_STATE_BLCOUNT_BITS 2
        #endif
    #else
    #define CVD_STATE_BLCOUNT_BITS 1
    #endif
#else
#define CVD_STATE_BLCOUNT_BITS 0
#endif
#if (CVD_STATE_BLCOUNT_BITS > 8) || defined(MTOUCH_EEPROM_ENABLED)
    #define MTOUCH_STATE_BASELINE_COUNT()         uint16_t baselineCount
    #define MTOUCH_BLCOUNT_16BITS_REQUIRED
#else
    #define MTOUCH_STATE_BASELINE_COUNT()         __paste(unsigned baselineCount  :,CVD_STATE_BLCOUNT_BITS)
#endif


#define MTOUCH_EXIT_SENSOR0()   EXIT_LAST_SENSOR(0)
#if MTOUCH_NUMBER_SENSORS > 1
    #if !defined(MTOUCH_SENSOR1)
        #error There is no MTOUCH_SENSOR1 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR0()
    #define MTOUCH_EXIT_SENSOR0()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR1()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 2
    #if !defined(MTOUCH_SENSOR2)
        #error There is no MTOUCH_SENSOR2 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR1()
    #define MTOUCH_EXIT_SENSOR1()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR2()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 3
    #if !defined(MTOUCH_SENSOR3)
        #error There is no MTOUCH_SENSOR3 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR2()
    #define MTOUCH_EXIT_SENSOR2()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR3()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 4
    #if !defined(MTOUCH_SENSOR4)
        #error There is no MTOUCH_SENSOR4 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR3()
    #define MTOUCH_EXIT_SENSOR3()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR4()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 5
    #if !defined(MTOUCH_SENSOR5)
        #error There is no MTOUCH_SENSOR5 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR4()
    #define MTOUCH_EXIT_SENSOR4()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR5()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 6
    #if !defined(MTOUCH_SENSOR6)
        #error There is no MTOUCH_SENSOR6 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR5()
    #define MTOUCH_EXIT_SENSOR5()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR6()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 7
    #if !defined(MTOUCH_SENSOR7)
        #error There is no MTOUCH_SENSOR7 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR6()
    #define MTOUCH_EXIT_SENSOR6()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR7()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 8
    #if !defined(MTOUCH_SENSOR8)
        #error There is no MTOUCH_SENSOR8 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR7()
    #define MTOUCH_EXIT_SENSOR7()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR8()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 9
    #if !defined(MTOUCH_SENSOR9)
        #error There is no MTOUCH_SENSOR9 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR8()
    #define MTOUCH_EXIT_SENSOR8()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR9()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 10
    #if !defined(MTOUCH_SENSOR10)
        #error There is no MTOUCH_SENSOR10 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR9()
    #define MTOUCH_EXIT_SENSOR9()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR10()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 11
    #if !defined(MTOUCH_SENSOR11)
        #error There is no MTOUCH_SENSOR11 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR10()
    #define MTOUCH_EXIT_SENSOR10()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR11()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 12
    #if !defined(MTOUCH_SENSOR12)
        #error There is no MTOUCH_SENSOR12 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR11()
    #define MTOUCH_EXIT_SENSOR11()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR12()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 13
    #if !defined(MTOUCH_SENSOR13)
        #error There is no MTOUCH_SENSOR13 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR12()
    #define MTOUCH_EXIT_SENSOR12()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR13()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 14
    #if !defined(MTOUCH_SENSOR14)
        #error There is no MTOUCH_SENSOR14 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR13()
    #define MTOUCH_EXIT_SENSOR13()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR14()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 15
    #if !defined(MTOUCH_SENSOR15)
        #error There is no MTOUCH_SENSOR15 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR14()
    #define MTOUCH_EXIT_SENSOR14()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR15()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 16
    #if !defined(MTOUCH_SENSOR16)
        #error There is no MTOUCH_SENSOR16 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR15()
    #define MTOUCH_EXIT_SENSOR15()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR16()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 17
    #if !defined(MTOUCH_SENSOR17)
        #error There is no MTOUCH_SENSOR17 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR16()
    #define MTOUCH_EXIT_SENSOR16()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR17()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 18
    #if !defined(MTOUCH_SENSOR18)
        #error There is no MTOUCH_SENSOR18 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR17()
    #define MTOUCH_EXIT_SENSOR17()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR18()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 19
    #if !defined(MTOUCH_SENSOR19)
        #error There is no MTOUCH_SENSOR19 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR18()
    #define MTOUCH_EXIT_SENSOR18()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR19()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 20
    #if !defined(MTOUCH_SENSOR20)
        #error There is no MTOUCH_SENSOR20 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR19()
    #define MTOUCH_EXIT_SENSOR19()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR20()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 21
    #if !defined(MTOUCH_SENSOR21)
        #error There is no MTOUCH_SENSOR21 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR20()
    #define MTOUCH_EXIT_SENSOR20()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR21()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 22
    #if !defined(MTOUCH_SENSOR22)
        #error There is no MTOUCH_SENSOR22 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR21()
    #define MTOUCH_EXIT_SENSOR21()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR22()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 23
    #if !defined(MTOUCH_SENSOR23)
        #error There is no MTOUCH_SENSOR23 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR22()
    #define MTOUCH_EXIT_SENSOR22()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR23()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 24
    #if !defined(MTOUCH_SENSOR24)
        #error There is no MTOUCH_SENSOR24 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR23()
    #define MTOUCH_EXIT_SENSOR23()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR24()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 25
    #if !defined(MTOUCH_SENSOR25)
        #error There is no MTOUCH_SENSOR25 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR24()
    #define MTOUCH_EXIT_SENSOR24()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR25()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 26
    #if !defined(MTOUCH_SENSOR26)
        #error There is no MTOUCH_SENSOR26 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR25()
    #define MTOUCH_EXIT_SENSOR25()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR26()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 27
    #if !defined(MTOUCH_SENSOR27)
        #error There is no MTOUCH_SENSOR27 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR26()
    #define MTOUCH_EXIT_SENSOR26()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR27()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 28
    #if !defined(MTOUCH_SENSOR28)
        #error There is no MTOUCH_SENSOR28 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR27()
    #define MTOUCH_EXIT_SENSOR27()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR28()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 29
    #if !defined(MTOUCH_SENSOR29)
        #error There is no MTOUCH_SENSOR29 definition in the configuration file. Framework unable to associate the sensor with an analog pin.
    #endif
    #undef  MTOUCH_EXIT_SENSOR28()
    #define MTOUCH_EXIT_SENSOR28()   EXIT_SENSOR(mTouch_currentSensor)
    #define MTOUCH_EXIT_SENSOR29()   EXIT_LAST_SENSOR(mTouch_currentSensor)
#endif
#if MTOUCH_NUMBER_SENSORS > 30
    #error This version of the mTouch CVD Framework does not support more than 30 sensors at this time.
#endif

#if !defined(THRESHOLD_PRESS_SENSOR0)
    #if MTOUCH_NUMBER_SENSORS > 0
        #error Required definition, THRESHOLD_PRESS_SENSOR0, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR0 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR1)
    #if MTOUCH_NUMBER_SENSORS > 1
        #error Required definition, THRESHOLD_PRESS_SENSOR1, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR1 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR2)
    #if MTOUCH_NUMBER_SENSORS > 2
        #error Required definition, THRESHOLD_PRESS_SENSOR2, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR2 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR3)
    #if MTOUCH_NUMBER_SENSORS > 3
        #error Required definition, THRESHOLD_PRESS_SENSOR3, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR3 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR4)
    #if MTOUCH_NUMBER_SENSORS > 4
        #error Required definition, THRESHOLD_PRESS_SENSOR4, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR4 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR5)
    #if MTOUCH_NUMBER_SENSORS > 5
        #error Required definition, THRESHOLD_PRESS_SENSOR5, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR5 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR6)
    #if MTOUCH_NUMBER_SENSORS > 6
        #error Required definition, THRESHOLD_PRESS_SENSOR6, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR6 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR7)
    #if MTOUCH_NUMBER_SENSORS > 7
        #error Required definition, THRESHOLD_PRESS_SENSOR7, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR7 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR8)
    #if MTOUCH_NUMBER_SENSORS > 8
        #error Required definition, THRESHOLD_PRESS_SENSOR8, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR8 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR9)
    #if MTOUCH_NUMBER_SENSORS > 9
        #error Required definition, THRESHOLD_PRESS_SENSOR9, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR9 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR10)
    #if MTOUCH_NUMBER_SENSORS > 10
        #error Required definition, THRESHOLD_PRESS_SENSOR10, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR10 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR11)
    #if MTOUCH_NUMBER_SENSORS > 11
        #error Required definition, THRESHOLD_PRESS_SENSOR11, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR11 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR12)
    #if MTOUCH_NUMBER_SENSORS > 12
        #error Required definition, THRESHOLD_PRESS_SENSOR12, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR12 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR13)
    #if MTOUCH_NUMBER_SENSORS > 13
        #error Required definition, THRESHOLD_PRESS_SENSOR13, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR13 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR14)
    #if MTOUCH_NUMBER_SENSORS > 14
        #error Required definition, THRESHOLD_PRESS_SENSOR14, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR14 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR15)
    #if MTOUCH_NUMBER_SENSORS > 15
        #error Required definition, THRESHOLD_PRESS_SENSOR15, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR15 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR16)
    #if MTOUCH_NUMBER_SENSORS > 16
        #error Required definition, THRESHOLD_PRESS_SENSOR16, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR16 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR17)
    #if MTOUCH_NUMBER_SENSORS > 17
        #error Required definition, THRESHOLD_PRESS_SENSOR17, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR17 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR18)
    #if MTOUCH_NUMBER_SENSORS > 18
        #error Required definition, THRESHOLD_PRESS_SENSOR18, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR18 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR19)
    #if MTOUCH_NUMBER_SENSORS > 19
        #error Required definition, THRESHOLD_PRESS_SENSOR19, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR19 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR20)
    #if MTOUCH_NUMBER_SENSORS > 20
        #error Required definition, THRESHOLD_PRESS_SENSOR20, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR20 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR21)
    #if MTOUCH_NUMBER_SENSORS > 21
        #error Required definition, THRESHOLD_PRESS_SENSOR21, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR21 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR22)
    #if MTOUCH_NUMBER_SENSORS > 22
        #error Required definition, THRESHOLD_PRESS_SENSOR22, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR22 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR23)
    #if MTOUCH_NUMBER_SENSORS > 23
        #error Required definition, THRESHOLD_PRESS_SENSOR23, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR23 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR24)
    #if MTOUCH_NUMBER_SENSORS > 24
        #error Required definition, THRESHOLD_PRESS_SENSOR24, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR24 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR25)
    #if MTOUCH_NUMBER_SENSORS > 25
        #error Required definition, THRESHOLD_PRESS_SENSOR25, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR25 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR26)
    #if MTOUCH_NUMBER_SENSORS > 26
        #error Required definition, THRESHOLD_PRESS_SENSOR26, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR26 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR27)
    #if MTOUCH_NUMBER_SENSORS > 27
        #error Required definition, THRESHOLD_PRESS_SENSOR27, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR27 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR28)
    #if MTOUCH_NUMBER_SENSORS > 28
        #error Required definition, THRESHOLD_PRESS_SENSOR28, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR28 0
    #endif
#endif
#if !defined(THRESHOLD_PRESS_SENSOR29)
    #if MTOUCH_NUMBER_SENSORS > 29
        #error Required definition, THRESHOLD_PRESS_SENSOR29, is not defined in the configuration file.
    #else
        #define THRESHOLD_PRESS_SENSOR29 0
    #endif
#endif

#define THRESHOLD_RELEASE_SENSOR0       (uint16_t)((float) THRESHOLD_PRESS_SENSOR0  * MTOUCH_RELEASE_THRESH_FACTOR)                                             
#define THRESHOLD_RELEASE_SENSOR1       (uint16_t)((float) THRESHOLD_PRESS_SENSOR1  * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR2       (uint16_t)((float) THRESHOLD_PRESS_SENSOR2  * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR3       (uint16_t)((float) THRESHOLD_PRESS_SENSOR3  * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR4       (uint16_t)((float) THRESHOLD_PRESS_SENSOR4  * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR5       (uint16_t)((float) THRESHOLD_PRESS_SENSOR5  * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR6       (uint16_t)((float) THRESHOLD_PRESS_SENSOR6  * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR7       (uint16_t)((float) THRESHOLD_PRESS_SENSOR7  * MTOUCH_RELEASE_THRESH_FACTOR)  
#define THRESHOLD_RELEASE_SENSOR8       (uint16_t)((float) THRESHOLD_PRESS_SENSOR8  * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR9       (uint16_t)((float) THRESHOLD_PRESS_SENSOR9  * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR10      (uint16_t)((float) THRESHOLD_PRESS_SENSOR10 * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR11      (uint16_t)((float) THRESHOLD_PRESS_SENSOR11 * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR12      (uint16_t)((float) THRESHOLD_PRESS_SENSOR12 * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR13      (uint16_t)((float) THRESHOLD_PRESS_SENSOR13 * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR14      (uint16_t)((float) THRESHOLD_PRESS_SENSOR14 * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR15      (uint16_t)((float) THRESHOLD_PRESS_SENSOR15 * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR16      (uint16_t)((float) THRESHOLD_PRESS_SENSOR16 * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR17      (uint16_t)((float) THRESHOLD_PRESS_SENSOR17 * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR18      (uint16_t)((float) THRESHOLD_PRESS_SENSOR18 * MTOUCH_RELEASE_THRESH_FACTOR) 
#define THRESHOLD_RELEASE_SENSOR19      (uint16_t)((float) THRESHOLD_PRESS_SENSOR19 * MTOUCH_RELEASE_THRESH_FACTOR)
#define THRESHOLD_RELEASE_SENSOR20      (uint16_t)((float) THRESHOLD_PRESS_SENSOR20 * MTOUCH_RELEASE_THRESH_FACTOR)
#define THRESHOLD_RELEASE_SENSOR21      (uint16_t)((float) THRESHOLD_PRESS_SENSOR21 * MTOUCH_RELEASE_THRESH_FACTOR)
#define THRESHOLD_RELEASE_SENSOR22      (uint16_t)((float) THRESHOLD_PRESS_SENSOR22 * MTOUCH_RELEASE_THRESH_FACTOR)
#define THRESHOLD_RELEASE_SENSOR23      (uint16_t)((float) THRESHOLD_PRESS_SENSOR23 * MTOUCH_RELEASE_THRESH_FACTOR)
#define THRESHOLD_RELEASE_SENSOR24      (uint16_t)((float) THRESHOLD_PRESS_SENSOR24 * MTOUCH_RELEASE_THRESH_FACTOR)
#define THRESHOLD_RELEASE_SENSOR25      (uint16_t)((float) THRESHOLD_PRESS_SENSOR25 * MTOUCH_RELEASE_THRESH_FACTOR)
#define THRESHOLD_RELEASE_SENSOR26      (uint16_t)((float) THRESHOLD_PRESS_SENSOR26 * MTOUCH_RELEASE_THRESH_FACTOR)
#define THRESHOLD_RELEASE_SENSOR27      (uint16_t)((float) THRESHOLD_PRESS_SENSOR27 * MTOUCH_RELEASE_THRESH_FACTOR)
#define THRESHOLD_RELEASE_SENSOR28      (uint16_t)((float) THRESHOLD_PRESS_SENSOR28 * MTOUCH_RELEASE_THRESH_FACTOR)
#define THRESHOLD_RELEASE_SENSOR29      (uint16_t)((float) THRESHOLD_PRESS_SENSOR29 * MTOUCH_RELEASE_THRESH_FACTOR)

#if defined(MTOUCH_DYNAMIC_SENSORS)
    #if !defined(MTOUCH_DYNAMIC_THRESHOLDS)
    #define MTOUCH_DYNAMIC_THRESHOLDS
    #endif
#endif

#if MTOUCH_NUMBER_SENSORS == 1
    #define MTOUCH_INDEXMINUSONE_0 0
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0")    
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0}
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
        #undef MTOUCH_UNIQUE_OVERSAMPLE_ENABLE
        #warning Disabling MTOUCH_UNIQUE_OVERSAMPLE_ENABLE because only one sensor is enabled.
    #endif    
#endif
#if MTOUCH_NUMBER_SENSORS == 2
    #define MTOUCH_INDEXMINUSONE_0 1
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR0}
#endif
#if MTOUCH_NUMBER_SENSORS == 3
    #define MTOUCH_INDEXMINUSONE_0 2
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2}
#endif
#if MTOUCH_NUMBER_SENSORS == 4
    #define MTOUCH_INDEXMINUSONE_0 3
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3}
#endif
#if MTOUCH_NUMBER_SENSORS == 5
    #define MTOUCH_INDEXMINUSONE_0 4
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4}
#endif
#if MTOUCH_NUMBER_SENSORS == 6
    #define MTOUCH_INDEXMINUSONE_0 5
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5}
#endif
#if MTOUCH_NUMBER_SENSORS == 7
    #define MTOUCH_INDEXMINUSONE_0 6
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6}
#endif
#if MTOUCH_NUMBER_SENSORS == 8
    #define MTOUCH_INDEXMINUSONE_0 7
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7}
#endif
#if MTOUCH_NUMBER_SENSORS == 9
    #define MTOUCH_INDEXMINUSONE_0 8
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8}
#endif
#if MTOUCH_NUMBER_SENSORS == 10
    #define MTOUCH_INDEXMINUSONE_0 9
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9}
#endif
#if MTOUCH_NUMBER_SENSORS == 11
    #define MTOUCH_INDEXMINUSONE_0 10
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10}
#endif
#if MTOUCH_NUMBER_SENSORS == 12
    #define MTOUCH_INDEXMINUSONE_0 11
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11}
#endif
#if MTOUCH_NUMBER_SENSORS == 13
    #define MTOUCH_INDEXMINUSONE_0 12
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12}
#endif
#if MTOUCH_NUMBER_SENSORS == 14
    #define MTOUCH_INDEXMINUSONE_0 13
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13}
#endif
#if MTOUCH_NUMBER_SENSORS == 15
    #define MTOUCH_INDEXMINUSONE_0 14
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14}
#endif
#if MTOUCH_NUMBER_SENSORS == 16
    #define MTOUCH_INDEXMINUSONE_0 15
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15}
#endif
#if MTOUCH_NUMBER_SENSORS == 17
    #define MTOUCH_INDEXMINUSONE_0 16
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16}
#endif
#if MTOUCH_NUMBER_SENSORS == 18
    #define MTOUCH_INDEXMINUSONE_0 17
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17}
#endif
#if MTOUCH_NUMBER_SENSORS == 19
    #define MTOUCH_INDEXMINUSONE_0 18
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17, sensor_18")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17,THRESHOLD_PRESS_SENSOR18}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17,THRESHOLD_RELEASE_SENSOR18}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17, MTOUCH_OVERSAMPLE18 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17, MTOUCH_SELECT_SENSOR18}
#endif
#if MTOUCH_NUMBER_SENSORS == 20
    #define MTOUCH_INDEXMINUSONE_0 19
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17, sensor_18, sensor_19")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17,THRESHOLD_PRESS_SENSOR18,THRESHOLD_PRESS_SENSOR19}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17,THRESHOLD_RELEASE_SENSOR18,THRESHOLD_RELEASE_SENSOR19}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17, MTOUCH_OVERSAMPLE18, MTOUCH_OVERSAMPLE19 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17, MTOUCH_SELECT_SENSOR18, MTOUCH_SELECT_SENSOR19}
#endif
#if MTOUCH_NUMBER_SENSORS == 21
    #define MTOUCH_INDEXMINUSONE_0 20
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17, sensor_18, sensor_19, sensor_20")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17,THRESHOLD_PRESS_SENSOR18,THRESHOLD_PRESS_SENSOR19,THRESHOLD_PRESS_SENSOR20}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17,THRESHOLD_RELEASE_SENSOR18,THRESHOLD_RELEASE_SENSOR19,THRESHOLD_RELEASE_SENSOR20}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17, MTOUCH_OVERSAMPLE18, MTOUCH_OVERSAMPLE19, MTOUCH_OVERSAMPLE20 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17, MTOUCH_SELECT_SENSOR18, MTOUCH_SELECT_SENSOR19, MTOUCH_SELECT_SENSOR20}
#endif
#if MTOUCH_NUMBER_SENSORS == 22
    #define MTOUCH_INDEXMINUSONE_0 21
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17, sensor_18, sensor_19, sensor_20, sensor_21")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17,THRESHOLD_PRESS_SENSOR18,THRESHOLD_PRESS_SENSOR19,THRESHOLD_PRESS_SENSOR20,THRESHOLD_PRESS_SENSOR21}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17,THRESHOLD_RELEASE_SENSOR18,THRESHOLD_RELEASE_SENSOR19,THRESHOLD_RELEASE_SENSOR20,THRESHOLD_RELEASE_SENSOR21}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17, MTOUCH_OVERSAMPLE18, MTOUCH_OVERSAMPLE19, MTOUCH_OVERSAMPLE20, MTOUCH_OVERSAMPLE21 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17, MTOUCH_SELECT_SENSOR18, MTOUCH_SELECT_SENSOR19, MTOUCH_SELECT_SENSOR20, MTOUCH_SELECT_SENSOR21}
#endif
#if MTOUCH_NUMBER_SENSORS == 23
    #define MTOUCH_INDEXMINUSONE_0 22
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17, sensor_18, sensor_19, sensor_20, sensor_21, sensor_22")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17,THRESHOLD_PRESS_SENSOR18,THRESHOLD_PRESS_SENSOR19,THRESHOLD_PRESS_SENSOR20,THRESHOLD_PRESS_SENSOR21,THRESHOLD_PRESS_SENSOR22}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17,THRESHOLD_RELEASE_SENSOR18,THRESHOLD_RELEASE_SENSOR19,THRESHOLD_RELEASE_SENSOR20,THRESHOLD_RELEASE_SENSOR21,THRESHOLD_RELEASE_SENSOR22}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17, MTOUCH_OVERSAMPLE18, MTOUCH_OVERSAMPLE19, MTOUCH_OVERSAMPLE20, MTOUCH_OVERSAMPLE21, MTOUCH_OVERSAMPLE22 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17, MTOUCH_SELECT_SENSOR18, MTOUCH_SELECT_SENSOR19, MTOUCH_SELECT_SENSOR20, MTOUCH_SELECT_SENSOR21, MTOUCH_SELECT_SENSOR22}
#endif
#if MTOUCH_NUMBER_SENSORS == 24
    #define MTOUCH_INDEXMINUSONE_0 23
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17, sensor_18, sensor_19, sensor_20, sensor_21, sensor_22, sensor_23")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17,THRESHOLD_PRESS_SENSOR18,THRESHOLD_PRESS_SENSOR19,THRESHOLD_PRESS_SENSOR20,THRESHOLD_PRESS_SENSOR21,THRESHOLD_PRESS_SENSOR22,THRESHOLD_PRESS_SENSOR23}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17,THRESHOLD_RELEASE_SENSOR18,THRESHOLD_RELEASE_SENSOR19,THRESHOLD_RELEASE_SENSOR20,THRESHOLD_RELEASE_SENSOR21,THRESHOLD_RELEASE_SENSOR22,THRESHOLD_RELEASE_SENSOR23}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17, MTOUCH_OVERSAMPLE18, MTOUCH_OVERSAMPLE19, MTOUCH_OVERSAMPLE20, MTOUCH_OVERSAMPLE21, MTOUCH_OVERSAMPLE22, MTOUCH_OVERSAMPLE23 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17, MTOUCH_SELECT_SENSOR18, MTOUCH_SELECT_SENSOR19, MTOUCH_SELECT_SENSOR20, MTOUCH_SELECT_SENSOR21, MTOUCH_SELECT_SENSOR22, MTOUCH_SELECT_SENSOR23}
#endif
#if MTOUCH_NUMBER_SENSORS == 25
    #define MTOUCH_INDEXMINUSONE_0 24
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17, sensor_18, sensor_19, sensor_20, sensor_21, sensor_22, sensor_23, sensor_24")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17,THRESHOLD_PRESS_SENSOR18,THRESHOLD_PRESS_SENSOR19,THRESHOLD_PRESS_SENSOR20,THRESHOLD_PRESS_SENSOR21,THRESHOLD_PRESS_SENSOR22,THRESHOLD_PRESS_SENSOR23,THRESHOLD_PRESS_SENSOR24}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17,THRESHOLD_RELEASE_SENSOR18,THRESHOLD_RELEASE_SENSOR19,THRESHOLD_RELEASE_SENSOR20,THRESHOLD_RELEASE_SENSOR21,THRESHOLD_RELEASE_SENSOR22,THRESHOLD_RELEASE_SENSOR23,THRESHOLD_RELEASE_SENSOR24}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17, MTOUCH_OVERSAMPLE18, MTOUCH_OVERSAMPLE19, MTOUCH_OVERSAMPLE20, MTOUCH_OVERSAMPLE21, MTOUCH_OVERSAMPLE22, MTOUCH_OVERSAMPLE23, MTOUCH_OVERSAMPLE24 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17, MTOUCH_SELECT_SENSOR18, MTOUCH_SELECT_SENSOR19, MTOUCH_SELECT_SENSOR20, MTOUCH_SELECT_SENSOR21, MTOUCH_SELECT_SENSOR22, MTOUCH_SELECT_SENSOR23, MTOUCH_SELECT_SENSOR24}
#endif
#if MTOUCH_NUMBER_SENSORS == 26
    #define MTOUCH_INDEXMINUSONE_0 25
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17, sensor_18, sensor_19, sensor_20, sensor_21, sensor_22, sensor_23, sensor_24, sensor_25")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17,THRESHOLD_PRESS_SENSOR18,THRESHOLD_PRESS_SENSOR19,THRESHOLD_PRESS_SENSOR20,THRESHOLD_PRESS_SENSOR21,THRESHOLD_PRESS_SENSOR22,THRESHOLD_PRESS_SENSOR23,THRESHOLD_PRESS_SENSOR24,THRESHOLD_PRESS_SENSOR25}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17,THRESHOLD_RELEASE_SENSOR18,THRESHOLD_RELEASE_SENSOR19,THRESHOLD_RELEASE_SENSOR20,THRESHOLD_RELEASE_SENSOR21,THRESHOLD_RELEASE_SENSOR22,THRESHOLD_RELEASE_SENSOR23,THRESHOLD_RELEASE_SENSOR24,THRESHOLD_RELEASE_SENSOR25}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17, MTOUCH_OVERSAMPLE18, MTOUCH_OVERSAMPLE19, MTOUCH_OVERSAMPLE20, MTOUCH_OVERSAMPLE21, MTOUCH_OVERSAMPLE22, MTOUCH_OVERSAMPLE23, MTOUCH_OVERSAMPLE24, MTOUCH_OVERSAMPLE25 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17, MTOUCH_SELECT_SENSOR18, MTOUCH_SELECT_SENSOR19, MTOUCH_SELECT_SENSOR20, MTOUCH_SELECT_SENSOR21, MTOUCH_SELECT_SENSOR22, MTOUCH_SELECT_SENSOR23, MTOUCH_SELECT_SENSOR24, MTOUCH_SELECT_SENSOR25}
#endif
#if MTOUCH_NUMBER_SENSORS == 27
    #define MTOUCH_INDEXMINUSONE_0 26
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17, sensor_18, sensor_19, sensor_20, sensor_21, sensor_22, sensor_23, sensor_24, sensor_25, sensor_26")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17,THRESHOLD_PRESS_SENSOR18,THRESHOLD_PRESS_SENSOR19,THRESHOLD_PRESS_SENSOR20,THRESHOLD_PRESS_SENSOR21,THRESHOLD_PRESS_SENSOR22,THRESHOLD_PRESS_SENSOR23,THRESHOLD_PRESS_SENSOR24,THRESHOLD_PRESS_SENSOR25,THRESHOLD_PRESS_SENSOR26}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17,THRESHOLD_RELEASE_SENSOR18,THRESHOLD_RELEASE_SENSOR19,THRESHOLD_RELEASE_SENSOR20,THRESHOLD_RELEASE_SENSOR21,THRESHOLD_RELEASE_SENSOR22,THRESHOLD_RELEASE_SENSOR23,THRESHOLD_RELEASE_SENSOR24,THRESHOLD_RELEASE_SENSOR25,THRESHOLD_RELEASE_SENSOR26}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17, MTOUCH_OVERSAMPLE18, MTOUCH_OVERSAMPLE19, MTOUCH_OVERSAMPLE20, MTOUCH_OVERSAMPLE21, MTOUCH_OVERSAMPLE22, MTOUCH_OVERSAMPLE23, MTOUCH_OVERSAMPLE24, MTOUCH_OVERSAMPLE25, MTOUCH_OVERSAMPLE26 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17, MTOUCH_SELECT_SENSOR18, MTOUCH_SELECT_SENSOR19, MTOUCH_SELECT_SENSOR20, MTOUCH_SELECT_SENSOR21, MTOUCH_SELECT_SENSOR22, MTOUCH_SELECT_SENSOR23, MTOUCH_SELECT_SENSOR24, MTOUCH_SELECT_SENSOR25, MTOUCH_SELECT_SENSOR26}
#endif
#if MTOUCH_NUMBER_SENSORS == 28
    #define MTOUCH_INDEXMINUSONE_0 27
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17, sensor_18, sensor_19, sensor_20, sensor_21, sensor_22, sensor_23, sensor_24, sensor_25, sensor_26, sensor_27")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17,THRESHOLD_PRESS_SENSOR18,THRESHOLD_PRESS_SENSOR19,THRESHOLD_PRESS_SENSOR20,THRESHOLD_PRESS_SENSOR21,THRESHOLD_PRESS_SENSOR22,THRESHOLD_PRESS_SENSOR23,THRESHOLD_PRESS_SENSOR24,THRESHOLD_PRESS_SENSOR25,THRESHOLD_PRESS_SENSOR26,THRESHOLD_PRESS_SENSOR27}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17,THRESHOLD_RELEASE_SENSOR18,THRESHOLD_RELEASE_SENSOR19,THRESHOLD_RELEASE_SENSOR20,THRESHOLD_RELEASE_SENSOR21,THRESHOLD_RELEASE_SENSOR22,THRESHOLD_RELEASE_SENSOR23,THRESHOLD_RELEASE_SENSOR24,THRESHOLD_RELEASE_SENSOR25,THRESHOLD_RELEASE_SENSOR26,THRESHOLD_RELEASE_SENSOR27}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17, MTOUCH_OVERSAMPLE18, MTOUCH_OVERSAMPLE19, MTOUCH_OVERSAMPLE20, MTOUCH_OVERSAMPLE21, MTOUCH_OVERSAMPLE22, MTOUCH_OVERSAMPLE23, MTOUCH_OVERSAMPLE24, MTOUCH_OVERSAMPLE25, MTOUCH_OVERSAMPLE26, MTOUCH_OVERSAMPLE27 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17, MTOUCH_SELECT_SENSOR18, MTOUCH_SELECT_SENSOR19, MTOUCH_SELECT_SENSOR20, MTOUCH_SELECT_SENSOR21, MTOUCH_SELECT_SENSOR22, MTOUCH_SELECT_SENSOR23, MTOUCH_SELECT_SENSOR24, MTOUCH_SELECT_SENSOR25, MTOUCH_SELECT_SENSOR26, MTOUCH_SELECT_SENSOR27}
#endif
#if MTOUCH_NUMBER_SENSORS == 29
    #define MTOUCH_INDEXMINUSONE_0 28
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17, sensor_18, sensor_19, sensor_20, sensor_21, sensor_22, sensor_23, sensor_24, sensor_25, sensor_26, sensor_27, sensor_28")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17,THRESHOLD_PRESS_SENSOR18,THRESHOLD_PRESS_SENSOR19,THRESHOLD_PRESS_SENSOR20,THRESHOLD_PRESS_SENSOR21,THRESHOLD_PRESS_SENSOR22,THRESHOLD_PRESS_SENSOR23,THRESHOLD_PRESS_SENSOR24,THRESHOLD_PRESS_SENSOR25,THRESHOLD_PRESS_SENSOR26,THRESHOLD_PRESS_SENSOR27,THRESHOLD_PRESS_SENSOR28}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17,THRESHOLD_RELEASE_SENSOR18,THRESHOLD_RELEASE_SENSOR19,THRESHOLD_RELEASE_SENSOR20,THRESHOLD_RELEASE_SENSOR21,THRESHOLD_RELEASE_SENSOR22,THRESHOLD_RELEASE_SENSOR23,THRESHOLD_RELEASE_SENSOR24,THRESHOLD_RELEASE_SENSOR25,THRESHOLD_RELEASE_SENSOR26,THRESHOLD_RELEASE_SENSOR27,THRESHOLD_RELEASE_SENSOR28}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17, MTOUCH_OVERSAMPLE18, MTOUCH_OVERSAMPLE19, MTOUCH_OVERSAMPLE20, MTOUCH_OVERSAMPLE21, MTOUCH_OVERSAMPLE22, MTOUCH_OVERSAMPLE23, MTOUCH_OVERSAMPLE24, MTOUCH_OVERSAMPLE25, MTOUCH_OVERSAMPLE26, MTOUCH_OVERSAMPLE27, MTOUCH_OVERSAMPLE28 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17, MTOUCH_SELECT_SENSOR18, MTOUCH_SELECT_SENSOR19, MTOUCH_SELECT_SENSOR20, MTOUCH_SELECT_SENSOR21, MTOUCH_SELECT_SENSOR22, MTOUCH_SELECT_SENSOR23, MTOUCH_SELECT_SENSOR24, MTOUCH_SELECT_SENSOR25, MTOUCH_SELECT_SENSOR26, MTOUCH_SELECT_SENSOR27, MTOUCH_SELECT_SENSOR28}
#endif
#if MTOUCH_NUMBER_SENSORS == 30
    #define MTOUCH_INDEXMINUSONE_0 29
    #define MTOUCH_ASM_MACRO_GLOBAL_SCAN_LABELS() asm("global sensor_0, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, sensor_9, sensor_10, sensor_11, sensor_12, sensor_13, sensor_14, sensor_15, sensor_16, sensor_17, sensor_18, sensor_19, sensor_20, sensor_21, sensor_22, sensor_23, sensor_24, sensor_25, sensor_26, sensor_27, sensor_28, sensor_29")
    #define PRESS_THRESHOLD_INIT {THRESHOLD_PRESS_SENSOR0,THRESHOLD_PRESS_SENSOR1,THRESHOLD_PRESS_SENSOR2,THRESHOLD_PRESS_SENSOR3,THRESHOLD_PRESS_SENSOR4,THRESHOLD_PRESS_SENSOR5,THRESHOLD_PRESS_SENSOR6,THRESHOLD_PRESS_SENSOR7,THRESHOLD_PRESS_SENSOR8,THRESHOLD_PRESS_SENSOR9,THRESHOLD_PRESS_SENSOR10,THRESHOLD_PRESS_SENSOR11,THRESHOLD_PRESS_SENSOR12,THRESHOLD_PRESS_SENSOR13,THRESHOLD_PRESS_SENSOR14,THRESHOLD_PRESS_SENSOR15,THRESHOLD_PRESS_SENSOR16,THRESHOLD_PRESS_SENSOR17,THRESHOLD_PRESS_SENSOR18,THRESHOLD_PRESS_SENSOR19,THRESHOLD_PRESS_SENSOR20,THRESHOLD_PRESS_SENSOR21,THRESHOLD_PRESS_SENSOR22,THRESHOLD_PRESS_SENSOR23,THRESHOLD_PRESS_SENSOR24,THRESHOLD_PRESS_SENSOR25,THRESHOLD_PRESS_SENSOR26,THRESHOLD_PRESS_SENSOR27,THRESHOLD_PRESS_SENSOR28,THRESHOLD_PRESS_SENSOR29}
    #define RELEASE_THRESHOLD_INIT {THRESHOLD_RELEASE_SENSOR0,THRESHOLD_RELEASE_SENSOR1,THRESHOLD_RELEASE_SENSOR2,THRESHOLD_RELEASE_SENSOR3,THRESHOLD_RELEASE_SENSOR4,THRESHOLD_RELEASE_SENSOR5,THRESHOLD_RELEASE_SENSOR6,THRESHOLD_RELEASE_SENSOR7,THRESHOLD_RELEASE_SENSOR8,THRESHOLD_RELEASE_SENSOR9,THRESHOLD_RELEASE_SENSOR10,THRESHOLD_RELEASE_SENSOR11,THRESHOLD_RELEASE_SENSOR12,THRESHOLD_RELEASE_SENSOR13,THRESHOLD_RELEASE_SENSOR14,THRESHOLD_RELEASE_SENSOR15,THRESHOLD_RELEASE_SENSOR16,THRESHOLD_RELEASE_SENSOR17,THRESHOLD_RELEASE_SENSOR18,THRESHOLD_RELEASE_SENSOR19,THRESHOLD_RELEASE_SENSOR20,THRESHOLD_RELEASE_SENSOR21,THRESHOLD_RELEASE_SENSOR22,THRESHOLD_RELEASE_SENSOR23,THRESHOLD_RELEASE_SENSOR24,THRESHOLD_RELEASE_SENSOR25,THRESHOLD_RELEASE_SENSOR26,THRESHOLD_RELEASE_SENSOR27,THRESHOLD_RELEASE_SENSOR28,THRESHOLD_RELEASE_SENSOR29}
    #if defined(MTOUCH_UNIQUE_OVERSAMPLE_ENABLE)
    #define MTOUCH_UNIQUE_OVERSAMPLE_INIT   { MTOUCH_OVERSAMPLE0, MTOUCH_OVERSAMPLE1, MTOUCH_OVERSAMPLE2, MTOUCH_OVERSAMPLE3, MTOUCH_OVERSAMPLE4, MTOUCH_OVERSAMPLE5, MTOUCH_OVERSAMPLE6, MTOUCH_OVERSAMPLE7, MTOUCH_OVERSAMPLE8, MTOUCH_OVERSAMPLE9, MTOUCH_OVERSAMPLE10, MTOUCH_OVERSAMPLE11, MTOUCH_OVERSAMPLE12, MTOUCH_OVERSAMPLE13, MTOUCH_OVERSAMPLE14, MTOUCH_OVERSAMPLE15, MTOUCH_OVERSAMPLE16, MTOUCH_OVERSAMPLE17, MTOUCH_OVERSAMPLE18, MTOUCH_OVERSAMPLE19, MTOUCH_OVERSAMPLE20, MTOUCH_OVERSAMPLE21, MTOUCH_OVERSAMPLE22, MTOUCH_OVERSAMPLE23, MTOUCH_OVERSAMPLE24, MTOUCH_OVERSAMPLE25, MTOUCH_OVERSAMPLE26, MTOUCH_OVERSAMPLE27, MTOUCH_OVERSAMPLE28, MTOUCH_OVERSAMPLE29 }
    #endif
    #define MTOUCH_SENSOR_CHANNEL_INIT  {MTOUCH_SELECT_SENSOR0, MTOUCH_SELECT_SENSOR1, MTOUCH_SELECT_SENSOR2, MTOUCH_SELECT_SENSOR3, MTOUCH_SELECT_SENSOR4, MTOUCH_SELECT_SENSOR5, MTOUCH_SELECT_SENSOR6, MTOUCH_SELECT_SENSOR7, MTOUCH_SELECT_SENSOR8, MTOUCH_SELECT_SENSOR9, MTOUCH_SELECT_SENSOR10, MTOUCH_SELECT_SENSOR11, MTOUCH_SELECT_SENSOR12, MTOUCH_SELECT_SENSOR13, MTOUCH_SELECT_SENSOR14, MTOUCH_SELECT_SENSOR15, MTOUCH_SELECT_SENSOR16, MTOUCH_SELECT_SENSOR17, MTOUCH_SELECT_SENSOR18, MTOUCH_SELECT_SENSOR19, MTOUCH_SELECT_SENSOR20, MTOUCH_SELECT_SENSOR21, MTOUCH_SELECT_SENSOR22, MTOUCH_SELECT_SENSOR23, MTOUCH_SELECT_SENSOR24, MTOUCH_SELECT_SENSOR25, MTOUCH_SELECT_SENSOR26, MTOUCH_SELECT_SENSOR27, MTOUCH_SELECT_SENSOR28, MTOUCH_SELECT_SENSOR29}
#endif

#if defined(MTOUCH_EEPROM_ENABLED)
    #if !defined(MTOUCH_EEPROM_START_ADR)
        #warning MTOUCH_EEPROM_START_ADR is not defined. Defaulting to 0x00 as the start address.
        #define MTOUCH_EEPROM_START_ADR     0x00
    #endif
    #include "mTouch_eeprom.h"                 // Stores select configuration options in EEPROM for live updating
#endif

#define NOP0    
#define NOP1     asm("nop");
#define NOP2     NOP1;  NOP1;
#define NOP3     NOP2;  NOP1;
#define NOP4     NOP3;  NOP1;
#define NOP5     NOP4;  NOP1;
#define NOP6     NOP5;  NOP1;
#define NOP7     NOP6;  NOP1;
#define NOP8     NOP7;  NOP1;
#define NOP9     NOP8;  NOP1;
#define NOP10    NOP9;  NOP1;
#define NOP11   NOP10;  NOP1;
#define NOP12   NOP11;  NOP1;
#define NOP13   NOP12;  NOP1;
#define NOP14   NOP13;  NOP1;
#define NOP15   NOP14;  NOP1;
#define NOP16   NOP15;  NOP1;
#define NOP17   NOP16;  NOP1;
#define NOP18   NOP17;  NOP1;
#define NOP19   NOP18;  NOP1;
#define NOP20   NOP19;  NOP1;
#define NOP21   NOP20;  NOP1;
#define NOP22   NOP21;  NOP1;
#define NOP23   NOP22;  NOP1;
#define NOP24   NOP23;  NOP1;
#define NOP25   NOP24;  NOP1;
#define NOP26   NOP25;  NOP1;
#define NOP27   NOP26;  NOP1;
#define NOP28   NOP27;  NOP1;
#define NOP29   NOP28;  NOP1;
#define NOP30   NOP29;  NOP1;
#define NOP31   NOP30;  NOP1;
#define NOP32   NOP31;  NOP1;
#define NOP33   NOP32;  NOP1;
#define NOP34   NOP33;  NOP1;
#define NOP35   NOP34;  NOP1;
#define NOP36   NOP35;  NOP1;
#define NOP37   NOP36;  NOP1;
#define NOP38   NOP37;  NOP1;
#define NOP39   NOP38;  NOP1;
#define NOP40   NOP39;  NOP1;
#define NOP41   NOP40;  NOP1;
#define NOP42   NOP41;  NOP1;
#define NOP43   NOP42;  NOP1;
#define NOP44   NOP43;  NOP1;
#define NOP45   NOP44;  NOP1;
#define NOP46   NOP45;  NOP1;
#define NOP47   NOP46;  NOP1;
#define NOP48   NOP47;  NOP1;
#define NOP49   NOP48;  NOP1;
#define NOP50   NOP49;  NOP1;
#define NOP51   NOP50;  NOP1;
#define NOP52   NOP50;  NOP2;
#define NOP53   NOP50;  NOP3;
#define NOP54   NOP50;  NOP4;
#define NOP55   NOP50;  NOP5;
#define NOP56   NOP50;  NOP6;
#define NOP57   NOP50;  NOP7;
#define NOP58   NOP50;  NOP8;
#define NOP59   NOP50;  NOP9;
#define NOP60   NOP50; NOP10;
#define NOP61   NOP50; NOP10; NOP1;
#define NOP62   NOP50; NOP11; NOP1;
#define NOP63   NOP50; NOP12; NOP1;
#define NOP64   NOP50; NOP13; NOP1;
#define NOP65   NOP50; NOP14; NOP1;
#define NOP66   NOP50; NOP15; NOP1;
#define NOP67   NOP50; NOP16; NOP1;
#define NOP68   NOP50; NOP17; NOP1;
#define NOP69   NOP50; NOP18; NOP1;
#define NOP70   NOP50; NOP19; NOP1;
#define NOP71   NOP50; NOP20; NOP1;
#define NOP72   NOP50; NOP21; NOP1;
#define NOP73   NOP50; NOP22; NOP1;
#define NOP74   NOP50; NOP23; NOP1;
#define NOP75   NOP50; NOP24; NOP1;
#define NOP76   NOP50; NOP25; NOP1;
#define NOP77   NOP50; NOP26; NOP1;
#define NOP78   NOP50; NOP27; NOP1;
#define NOP79   NOP50; NOP28; NOP1;
#define NOP80   NOP50; NOP29; NOP1;
#define NOP81   NOP50; NOP30; NOP1;
#define NOP82   NOP50; NOP31; NOP1;
#define NOP83   NOP50; NOP32; NOP1;
#define NOP84   NOP50; NOP33; NOP1;
#define NOP85   NOP50; NOP34; NOP1;
#define NOP86   NOP50; NOP35; NOP1;
#define NOP87   NOP50; NOP36; NOP1;
#define NOP88   NOP50; NOP37; NOP1;
#define NOP89   NOP50; NOP38; NOP1;
#define NOP90   NOP50; NOP39; NOP1;
#define NOP91   NOP50; NOP40; NOP1;
#define NOP92   NOP50; NOP41; NOP1;
#define NOP93   NOP50; NOP42; NOP1;
#define NOP94   NOP50; NOP43; NOP1;
#define NOP95   NOP50; NOP44; NOP1;
#define NOP96   NOP50; NOP45; NOP1;
#define NOP97   NOP50; NOP46; NOP1;
#define NOP98   NOP50; NOP47; NOP1;
#define NOP99   NOP50; NOP48; NOP1;
#define NOP100  NOP50; NOP50;

#define _NOP_DELAY(value)       __paste(NOP,value);

#if !defined(CVD_CHOLD_CHARGE_DELAY)
    #define CVD_CHOLD_CHARGE_DELAY 0
#endif
#if !defined(CVD_SETTLING_DELAY)
    #define CVD_SETTLING_DELAY 0
#endif
#if !defined(CVD_PRE_GODONE_DELAY)
    #define CVD_PRE_GODONE_DELAY 0
#endif

#if (CVD_CHOLD_CHARGE_DELAY - 2 - 3 * ((CVD_CHOLD_CHARGE_DELAY-2)/3)) == 1 
    #define CVD_DELAY_CHOLD_REMAINDER()   _NOP_DELAY(1);
#elif (CVD_CHOLD_CHARGE_DELAY - 2 - 3 * ((CVD_CHOLD_CHARGE_DELAY-2)/3)) == 2
    #define CVD_DELAY_CHOLD_REMAINDER()   _NOP_DELAY(2);
#else
    #define CVD_DELAY_CHOLD_REMAINDER()
#endif

#if defined(_12F615) || defined(_12HV615) || defined(_PIC18)
    #define CVD_DELAY_CHOLD()  _NOP_DELAY(CVD_CHOLD_CHARGE_DELAY);
#else
    #if CVD_CHOLD_CHARGE_DELAY > 4
        #define CVD_DELAY_VARIABLE_REQUIRED
        #define CVD_DELAY_CHOLD()                                               \
            do {                                                                \
                asm("BANKSEL    _mTouch_delayCount"                         );  \
                asm("movlw  "   ___mkstr(CVD_CHOLD_CHARGE_DELAY-2)      "/3");  \
                asm("movwf  "   ___mkstr(NOBANK(_mTouch_delayCount))        );  \
                asm("decfsz "   ___mkstr(NOBANK(_mTouch_delayCount))   ", F");  \
                asm("goto $-1");                                                \
                CVD_DELAY_CHOLD_REMAINDER();                                    \
            } while(0)
    #else
        #define CVD_DELAY_CHOLD()   _NOP_DELAY(CVD_CHOLD_CHARGE_DELAY);
    #endif
#endif 

#if (CVD_SETTLING_DELAY - 2 - 3 * ((CVD_SETTLING_DELAY-2)/3)) == 1
    #define CVD_DELAY_SETTLE_REMAINDER()   _NOP_DELAY(1);
#elif (CVD_SETTLING_DELAY - 2 - 3 * ((CVD_SETTLING_DELAY-2)/3)) == 2
    #define CVD_DELAY_SETTLE_REMAINDER()   _NOP_DELAY(2);
#else
    #define CVD_DELAY_SETTLE_REMAINDER()
#endif

#if defined(_12F615) || defined(_12HV615) || defined(_PIC18)
    #define CVD_DELAY_SETTLE()  _NOP_DELAY(CVD_SETTLING_DELAY);
#else
    #if CVD_SETTLING_DELAY > 4
        #if !defined(CVD_DELAY_VARIABLE_REQUIRED)
            #define CVD_DELAY_VARIABLE_REQUIRED
        #endif
        
        #define CVD_DELAY_SETTLE()                                              \
            do {                                                                \
                asm("BANKSEL    _mTouch_delayCount"                         );  \
                asm("movlw  "   ___mkstr(CVD_SETTLING_DELAY-2)         "/3" );  \
                asm("movwf  "   ___mkstr(NOBANK(_mTouch_delayCount))        );  \
                asm("decfsz "   ___mkstr(NOBANK(_mTouch_delayCount))   ", F");  \
                asm("goto       $-1"                                        );  \
                CVD_DELAY_SETTLE_REMAINDER();                                   \
            } while(0)
    #else
        #define CVD_DELAY_SETTLE()  _NOP_DELAY(CVD_SETTLING_DELAY);
    #endif       
#endif

#if (CVD_UNIMPLEMENTED_AVAILABLE == 1)
    #define CVD_GO_DONE_DELAY()     asm("BANKSEL "  ___mkstr(_ADCON0));                         \
                                    asm("movlw   "  ___mkstr(MTOUCH_SELECT_ISO_NOGO | 0x02));   \
                                    asm("movwf   "  ___mkstr(NOBANK(_ADCON0)));  
#else
    #if !defined(PIC_ADC_TAD)
        #warning The PIC_ADC_TAD value in your devices hardware profile has not been defined. Defaulting to '1' us.
        #define PIC_ADC_TAD 1
    #endif
    #if PIC_ADC_TAD == 1
        #if _XTAL_FREQ == 32000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(3);
        #elif _XTAL_FREQ == 16000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(1);
        #elif _XTAL_FREQ == 8000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(0);
        #elif _XTAL_FREQ == 4000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(0);
        #elif _XTAL_FREQ == 2000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(0);
        #elif _XTAL_FREQ == 1000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(0);
        #endif
    #elif PIC_ADC_TAD == 2
        #if _XTAL_FREQ == 32000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(7);
        #elif _XTAL_FREQ == 16000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(3);
        #elif _XTAL_FREQ == 8000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(1);
        #elif _XTAL_FREQ == 4000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(0);
        #elif _XTAL_FREQ == 2000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(0);
        #elif _XTAL_FREQ == 1000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(0);
        #endif
    #elif PIC_ADC_TAD == 4
        #if _XTAL_FREQ == 32000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(15);
        #elif _XTAL_FREQ == 16000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(7);
        #elif _XTAL_FREQ == 8000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(3);
        #elif _XTAL_FREQ == 4000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(1);
        #elif _XTAL_FREQ == 2000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(0);
        #elif _XTAL_FREQ == 1000000
            #define CVD_GO_DONE_DELAY()     _NOP_DELAY(0);
        #endif
    #endif
#endif

#if defined(MTOUCH_NUMBER_OF_SLIDERS)
#if (MTOUCH_NUMBER_OF_SLIDERS > 0)
    #define MTOUCH_SLIDERS_ENABLED
    #include "mTouch_slider.h"
#endif
#endif

#define mTouch_GetAccumulator(dataPointer)          ((uint24_t)(dataPointer->accumulator.v) & 0xFFFFF)
#define mTouch_GetResult(dataPointer)               (uint16_t)(dataPointer->result.v >> 4)
#define mTouch_SetAccumulator(dataPointer, value)                                   \
    do                                                                              \
    {                                                                               \
        mTouch_ClearAccumulator(dataPointer)                                        \
        dataPointer->accumulator.v |= (uint16_t)(value & 0xFFF);                    \
    } while(0);
#define mTouch_SetResult(dataPointer, value)                                        \
    do                                                                              \
    {                                                                               \
        dataPointer->result.v &= 0x000F;                                            \
        dataPointer->result.v |= (uint16_t)(value << 4);                            \
    } while(0);
#define mTouch_IncResult(dataPointer)                                               \
    do                                                                              \
    {                                                                               \
        dataPointer->result.v += 0x10;                                              \
        result++;                                                                   \
    } while(0);
#define mTouch_DecResult(dataPointer)                                               \
    do                                                                              \
    {                                                                               \
        dataPointer->result.v -= 0x10;                                              \
        result--;                                                                   \
    } while(0);
#define mTouch_UpdateAccumulator(dataPointer)                                       \
    do                                                                              \
    {                                                                               \
        dataPointer->accumulator.v += mTouch_GetResult(dataPointer);                \
    } while(0);
#define mTouch_ClearAccumulator(dataPointer)                                        \
    do                                                                              \
    {                                                                               \
        dataPointer->accumulator.v &= 0xF00000;                                     \
    } while(0);

#define MTOUCH_INDEXMINUSONE_1      0
#define MTOUCH_INDEXMINUSONE_2      1
#define MTOUCH_INDEXMINUSONE_3      2
#define MTOUCH_INDEXMINUSONE_4      3
#define MTOUCH_INDEXMINUSONE_5      4
#define MTOUCH_INDEXMINUSONE_6      5
#define MTOUCH_INDEXMINUSONE_7      6
#define MTOUCH_INDEXMINUSONE_8      7
#define MTOUCH_INDEXMINUSONE_9      8
#define MTOUCH_INDEXMINUSONE_10     9
#define MTOUCH_INDEXMINUSONE_11     10
#define MTOUCH_INDEXMINUSONE_12     11
#define MTOUCH_INDEXMINUSONE_13     12
#define MTOUCH_INDEXMINUSONE_14     13
#define MTOUCH_INDEXMINUSONE_15     14
#define MTOUCH_INDEXMINUSONE_16     15
#define MTOUCH_INDEXMINUSONE_17     16
#define MTOUCH_INDEXMINUSONE_18     17
#define MTOUCH_INDEXMINUSONE_19     18
#define MTOUCH_INDEXMINUSONE_20     19
#define MTOUCH_INDEXMINUSONE_21     20
#define MTOUCH_INDEXMINUSONE_22     21
#define MTOUCH_INDEXMINUSONE_23     22
#define MTOUCH_INDEXMINUSONE_24     23
#define MTOUCH_INDEXMINUSONE_25     24
#define MTOUCH_INDEXMINUSONE_26     25
#define MTOUCH_INDEXMINUSONE_27     26
#define MTOUCH_INDEXMINUSONE_28     27   
#define MTOUCH_INDEXMINUSONE_29     28


#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR0)  
#define CVD_REFSENSOR_0  CVD_REFERENCE_SENSOR0  
#else    
#define CVD_REFSENSOR_0  MTOUCH_INDEXMINUSONE_0    
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR1)   
#define CVD_REFSENSOR_1  CVD_REFERENCE_SENSOR1   
#else   
#define CVD_REFSENSOR_1  MTOUCH_INDEXMINUSONE_1    
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR2)  
#define CVD_REFSENSOR_2  CVD_REFERENCE_SENSOR2   
#else    
#define CVD_REFSENSOR_2  MTOUCH_INDEXMINUSONE_2    
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR3)  
#define CVD_REFSENSOR_3  CVD_REFERENCE_SENSOR3  
#else    
#define CVD_REFSENSOR_3  MTOUCH_INDEXMINUSONE_3    
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR4)  
#define CVD_REFSENSOR_4  CVD_REFERENCE_SENSOR4  
#else   
#define CVD_REFSENSOR_4  MTOUCH_INDEXMINUSONE_4   
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR5)   
#define CVD_REFSENSOR_5  CVD_REFERENCE_SENSOR5   
#else   
#define CVD_REFSENSOR_5  MTOUCH_INDEXMINUSONE_5   
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR6)  
#define CVD_REFSENSOR_6  CVD_REFERENCE_SENSOR6   
#else   
#define CVD_REFSENSOR_6  MTOUCH_INDEXMINUSONE_6    
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR7)   
#define CVD_REFSENSOR_7  CVD_REFERENCE_SENSOR7   
#else    
#define CVD_REFSENSOR_7  MTOUCH_INDEXMINUSONE_7    
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR8)  
#define CVD_REFSENSOR_8  CVD_REFERENCE_SENSOR8   
#else    
#define CVD_REFSENSOR_8  MTOUCH_INDEXMINUSONE_8    
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR9)  
#define CVD_REFSENSOR_9  CVD_REFERENCE_SENSOR9   
#else    
#define CVD_REFSENSOR_9  MTOUCH_INDEXMINUSONE_9    
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR10) 
#define CVD_REFSENSOR_10  CVD_REFERENCE_SENSOR10 
#else    
#define CVD_REFSENSOR_10  MTOUCH_INDEXMINUSONE_10  
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR11) 
#define CVD_REFSENSOR_11  CVD_REFERENCE_SENSOR11 
#else    
#define CVD_REFSENSOR_11  MTOUCH_INDEXMINUSONE_11  
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR12) 
#define CVD_REFSENSOR_12  CVD_REFERENCE_SENSOR12  
#else      
#define CVD_REFSENSOR_12  MTOUCH_INDEXMINUSONE_12  
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR13) 
#define CVD_REFSENSOR_13  CVD_REFERENCE_SENSOR13  
#else      
#define CVD_REFSENSOR_13  MTOUCH_INDEXMINUSONE_13  
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR14) 
#define CVD_REFSENSOR_14  CVD_REFERENCE_SENSOR14  
#else      
#define CVD_REFSENSOR_14  MTOUCH_INDEXMINUSONE_14  
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR15) 
#define CVD_REFSENSOR_15  CVD_REFERENCE_SENSOR15   
#else     
#define CVD_REFSENSOR_15  MTOUCH_INDEXMINUSONE_15  
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR16) 
#define CVD_REFSENSOR_16  CVD_REFERENCE_SENSOR16    
#else     
#define CVD_REFSENSOR_16  MTOUCH_INDEXMINUSONE_16  
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR17)
#define CVD_REFSENSOR_17  CVD_REFERENCE_SENSOR17 
#else     
#define CVD_REFSENSOR_17  MTOUCH_INDEXMINUSONE_17  
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR18) 
#define CVD_REFSENSOR_18  CVD_REFERENCE_SENSOR18   
#else    
#define CVD_REFSENSOR_18  MTOUCH_INDEXMINUSONE_18  
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR19) 
#define CVD_REFSENSOR_19  CVD_REFERENCE_SENSOR19 
#else   
#define CVD_REFSENSOR_19  MTOUCH_INDEXMINUSONE_19 
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR20) 
#define CVD_REFSENSOR_20  CVD_REFERENCE_SENSOR20  
#else   
#define CVD_REFSENSOR_20  MTOUCH_INDEXMINUSONE_20 
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR21) 
#define CVD_REFSENSOR_21  CVD_REFERENCE_SENSOR21  
#else   
#define CVD_REFSENSOR_21  MTOUCH_INDEXMINUSONE_21 
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR22)  
#define CVD_REFSENSOR_22  CVD_REFERENCE_SENSOR22  
#else   
#define CVD_REFSENSOR_22  MTOUCH_INDEXMINUSONE_22 
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR23)  
#define CVD_REFSENSOR_23  CVD_REFERENCE_SENSOR23  
#else    
#define CVD_REFSENSOR_23  MTOUCH_INDEXMINUSONE_23 
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR24) 
#define CVD_REFSENSOR_24  CVD_REFERENCE_SENSOR24  
#else    
#define CVD_REFSENSOR_24  MTOUCH_INDEXMINUSONE_24  
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR25)  
#define CVD_REFSENSOR_25  CVD_REFERENCE_SENSOR25  
#else    
#define CVD_REFSENSOR_25  MTOUCH_INDEXMINUSONE_25  
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR26)  
#define CVD_REFSENSOR_26  CVD_REFERENCE_SENSOR26  
#else    
#define CVD_REFSENSOR_26  MTOUCH_INDEXMINUSONE_26  
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR27)  
#define CVD_REFSENSOR_27  CVD_REFERENCE_SENSOR27  
#else    
#define CVD_REFSENSOR_27  MTOUCH_INDEXMINUSONE_27 
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR28)   
#define CVD_REFSENSOR_28  CVD_REFERENCE_SENSOR28  
#else    
#define CVD_REFSENSOR_28  MTOUCH_INDEXMINUSONE_28 
#endif
#if defined(MTOUCH_UNIQUE_REFERENCE_OPTIONS) && defined(CVD_REFERENCE_SENSOR29)  
#define CVD_REFSENSOR_29  CVD_REFERENCE_SENSOR29   
#else    
#define CVD_REFSENSOR_29  MTOUCH_INDEXMINUSONE_29   
#endif

#if defined(MTOUCH_UNIQUE_SCAN_OPTIONS)
#if defined(CVD_GUARD_IO_0_ENABLED)
    #define 
#endif
#endif


#if defined(CVD_FORCE_REF_DAC) && !defined(PIC_DACOUT_AVAILABLE)
    #error Your currently chosen processor does not have a DACOUT pin available. Please undefine CVD_FORCE_REF_DAC in the configuration file.
#endif
#if defined(CVD_GUARD_DACOUT_ENABLED) && !defined(PIC_DACOUT_AVAILABLE)
    #error Your currently chosen processor does not have a DACOUT pin available. Please undefine CVD_GUARD_DACOUT_ENABLED in the configuration file.
#endif
#if defined(CVD_MUTUAL_ENABLED) && defined(CVD_MUTUAL_EXCLUSIVE_ENABLED)
    #error Both CVD_MUTUAL_ENABLED and CVD_MUTUAL_EXCLUSIVE_ENABLED are defined. Please comment out one of them in the configuration file.
#endif
#if defined(CVD_GUARD_DACOUT_ENABLED) && defined(CVD_GUARD_IO_ENABLED)
    #error Both CVD_GUARD_IO_ENABLED and CVD_GUARD_DACOUT_ENABLED are defined. Please undefine one of them in the configuration file.
#endif

#if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CONTROLS_ISR) && defined(MCOMM_ENABLED) && defined(MCOMM_TWO_WAY_ENABLED)
    #error mComm 2-way communications are not compatible with MTOUCH_INTEGRATION_TYPE == MTOUCH_CONTROLS_ISR, please redefine MTOUCH_INTEGRATION_TYPE to MTOUCH_CALLED_FROM_ISR or MTOUCH_CALLED_FROM_MAINLOOP. An example ISR function to handle the mComm and mTouch interrupts is available in the example main.c file.
#endif

#if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CONTROLS_ISR) || (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_ISR)
    #if !defined(MTOUCH_ISR_TIMER)
        #define MTOUCH_ISR_TIMER 0
        #warning MTOUCH_ISR_TIMER is not defined in the configuration file. Defaulting to TMR0.
    #endif  
    
    #if MTOUCH_ISR_TIMER == 0
        #if !defined(PIC_TIMER0_AVAILABLE)
            #error Timer0 does not exist for the currently selected microcontroller. Please redefine MTOUCH_ISR_TIMER to an available timer.
        #else
            #if defined(_PIC18)
                #define MTOUCH_ISR_TMRxIF       TMR0IF
                #define MTOUCH_ISR_TMRxIE       TMR0IE
                #define MTOUCH_ISR_TMR_ASM      TMR0L
                #define MTOUCH_ISR_TMR_C        TMR0L
            #else
                #define MTOUCH_ISR_TMRxIF       TMR0IF
                #define MTOUCH_ISR_TMRxIE       TMR0IE
                #define MTOUCH_ISR_TMR_ASM      TMR0
                #define MTOUCH_ISR_TMR_C        TMR0
            #endif
        #endif
    #elif MTOUCH_ISR_TIMER == 1
        #error The mTouch framework is not compatible with TMR1. Please redefine MTOUCH_ISR_TIMER to an 8-bit timer.
    #elif MTOUCH_ISR_TIMER == 3
        #error The mTouch framework is not compatible with TMR3. Please redefine MTOUCH_ISR_TIMER to an 8-bit timer.
    #elif MTOUCH_ISR_TIMER == 5
        #error The mTouch framework is not compatible with TMR5. Please redefine MTOUCH_ISR_TIMER to an 8-bit timer.
    #elif MTOUCH_ISR_TIMER == 7
        #error The mTouch framework is not compatible with TMR7. Please redefine MTOUCH_ISR_TIMER to an 8-bit timer.
    #elif MTOUCH_ISR_TIMER == 9
        #error The mTouch framework is not compatible with TMR9. Please redefine MTOUCH_ISR_TIMER to an 8-bit timer.
    #elif MTOUCH_ISR_TIMER == 2
        #if !defined(PIC_TIMER2_AVAILABLE)
            #error Timer2 does not exist for the currently selected microcontroller. Please redefine MTOUCH_ISR_TIMER to an available timer.
        #else
            #define MTOUCH_ISR_TMRxIF       TMR2IF
            #define MTOUCH_ISR_TMRxIE       TMR2IE
            #define MTOUCH_ISR_TMR_ASM      TMR2
            #define MTOUCH_ISR_TMR_C        TMR2
            #define MTOUCH_ISR_TMR_PEIE_REQUIRED
        #endif
    #elif MTOUCH_ISR_TIMER == 4
        #if !defined(PIC_TIMER4_AVAILABLE)
            #error Timer4 does not exist for the currently selected microcontroller. Please redefine MTOUCH_ISR_TIMER to an available timer.
        #else
            #define MTOUCH_ISR_TMRxIF       TMR4IF
            #define MTOUCH_ISR_TMRxIE       TMR4IE
            #define MTOUCH_ISR_TMR_ASM      TMR4
            #define MTOUCH_ISR_TMR_C        TMR4
            #define MTOUCH_ISR_TMR_PEIE_REQUIRED
        #endif
    #elif MTOUCH_ISR_TIMER == 6
        #if !defined(PIC_TIMER6_AVAILABLE)
            #error Timer6 does not exist for the currently selected microcontroller. Please redefine MTOUCH_ISR_TIMER to an available timer.
        #else
            #define MTOUCH_ISR_TMRxIF       TMR6IF
            #define MTOUCH_ISR_TMRxIE       TMR6IE
            #define MTOUCH_ISR_TMR_ASM      TMR6
            #define MTOUCH_ISR_TMR_C        TMR6
            #define MTOUCH_ISR_TMR_PEIE_REQUIRED
        #endif
    #elif MTOUCH_ISR_TIMER == 8
        #if !defined(PIC_TIMER8_AVAILABLE)
            #error Timer8 does not exist for the currently selected microcontroller. Please redefine MTOUCH_ISR_TIMER to an available timer.
        #else
            #define MTOUCH_ISR_TMRxIF       TMR8IF
            #define MTOUCH_ISR_TMRxIE       TMR8IE
            #define MTOUCH_ISR_TMR_ASM      TMR8
            #define MTOUCH_ISR_TMR_C        TMR8
            #define MTOUCH_ISR_TMR_PEIE_REQUIRED
        #endif
    #elif MTOUCH_ISR_TIMER == 10
        #if !defined(PIC_TIMER10_AVAILABLE)
            #error Timer10 does not exist for the currently selected microcontroller. Please redefine MTOUCH_ISR_TIMER to an available timer.
        #else
            #define MTOUCH_ISR_TMRxIF       TMR10IF
            #define MTOUCH_ISR_TMRxIE       TMR10IE
            #define MTOUCH_ISR_TMR_ASM      TMR10
            #define MTOUCH_ISR_TMR_C        TMR10
            #define MTOUCH_ISR_TMR_PEIE_REQUIRED
        #endif
    #endif
#endif

#if defined(MTOUCH_MOST_PRESSED_ONLY) && (MTOUCH_NUMBER_SENSORS < 2)
    #warning The mTouch Framework's most-pressed algorithm has been enabled but there are not 2 active sensors. Undefining MTOUCH_MOST_PRESSED_ONLY.
    #undef MTOUCH_MOST_PRESSED_ONLY
#endif



    
// Include the macro library header file that corresponds to the currently selected PIC microcontroller's core.
#if defined(_PIC14E)
    // Enhanced Mid-range
    #include "mTouchCVD_macroLibrary_PIC16F1.h"
#elif defined(_PIC14)
    // Normal Mid-range
    #include "mTouchCVD_macroLibrary_PIC16F.h"
#elif defined(_PIC18) || defined(__18CXX)
    // PIC18F
    #include "mTouchCVD_macroLibrary_PIC18F.h"
#else
    #error The currently chosen processor core does not have a matching macroLibrary file associated with it.
#endif    

    #if defined(MTOUCH_EEPROM_ENABLED)

        #define MTOUCH_pDEBOUNCE_VALUE              (((uint16_t)(MTOUCH_EEPROM_read(MTOUCH_EEPROM_pDEB_ADRH))       << 8) + MTOUCH_EEPROM_read(MTOUCH_EEPROM_pDEB_ADRL))
        #define MTOUCH_rDEBOUNCE_VALUE              (((uint16_t)(MTOUCH_EEPROM_read(MTOUCH_EEPROM_rDEB_ADRH))       << 8) + MTOUCH_EEPROM_read(MTOUCH_EEPROM_rDEB_ADRL))
        #define MTOUCH_SCALING_VALUE                MTOUCH_EEPROM_read(MTOUCH_EEPROM_SCALE_ADR)
        #define MTOUCH_BASELINE_WEIGHT_VALUE        MTOUCH_EEPROM_read(MTOUCH_EEPROM_BLWEIGHT_ADR)
        #define MTOUCH_NEGCAP_VALUE                 MTOUCH_EEPROM_read(MTOUCH_EEPROM_NEGCAP_ADR)
        #define MTOUCH_JITTER_VALUE                 MTOUCH_EEPROM_read(MTOUCH_EEPROM_JITTER_ADR)
        #define MTOUCH_SpS_VALUE                    (((uint16_t)(MTOUCH_EEPROM_read(MTOUCH_EEPROM_SpS_ADRH))        << 8) + MTOUCH_EEPROM_read(MTOUCH_EEPROM_SpS_ADRL))
        #define MTOUCH_pTIMEOUT_VALUE               (((uint16_t)(MTOUCH_EEPROM_read(MTOUCH_EEPROM_pTIMEOUT_ADRH))   << 8) + MTOUCH_EEPROM_read(MTOUCH_EEPROM_pTIMEOUT_ADRL))
        #define MTOUCH_BASELINE_RATE_VALUE          (((uint16_t)(MTOUCH_EEPROM_read(MTOUCH_EEPROM_BLRATE_ADRH))     << 8) + MTOUCH_EEPROM_read(MTOUCH_EEPROM_BLRATE_ADRL))
        #define MTOUCH_DECIMATION_MAX_VALUE         (((uint16_t)(MTOUCH_EEPROM_read(MTOUCH_EEPROM_DECSTEP_ADRH))    << 8) + MTOUCH_EEPROM_read(MTOUCH_EEPROM_DECSTEP_ADRL))

        #define mTouch_GetPressThreshold(i)         MTOUCH_EEPROM_pTHRESHOLD(i)
        #define mTouch_GetReleaseThreshold(i)       MTOUCH_EEPROM_rTHRESHOLD(i)

        
    #else
        
        #define mTouch_GetPressThreshold(i)         mTouch_pressThreshold[i]  
        #define mTouch_GetReleaseThreshold(i)       mTouch_releaseThreshold[i]  
        
        #define MTOUCH_pDEBOUNCE_VALUE              MTOUCH_DEBOUNCE_PRESS
        #define MTOUCH_rDEBOUNCE_VALUE              MTOUCH_DEBOUNCE_RELEASE
        #define MTOUCH_pTIMEOUT_VALUE               MTOUCH_BUTTON_TIMEOUT
        #define MTOUCH_SpS_VALUE                    MTOUCH_SAMPLES_PER_SCAN
        #define MTOUCH_SCALING_VALUE                MTOUCH_SCALING
        #define MTOUCH_BASELINE_WEIGHT_VALUE        MTOUCH_BASELINE_WEIGHT
        #define MTOUCH_BASELINE_RATE_VALUE          MTOUCH_BASELINE_RATE
        #define MTOUCH_NEGCAP_VALUE                 MTOUCH_NEGATIVE_CAPACITANCE
        #define MTOUCH_DECIMATION_MAX_VALUE         MTOUCH_DECIMATION_MAX_STEP

        
    #endif      // end: if !defined(MTOUCH_EEPROM_ENABLED)

    #if defined(MTOUCH_MATRIX_ENABLED)
        
        // Make sure everything is defined that needs to be...
        #if !defined(MTOUCH_MATRIX_ROW_START)       || \
            !defined(MTOUCH_MATRIX_ROW_END)         || \
            !defined(MTOUCH_MATRIX_COLUMN_START)    || \
            !defined(MTOUCH_MATRIX_COLUMN_END)
            
            #error You have enabled the mTouch matrix feature but have not defined all of the necessary parameters. Please make sure mTouch_config.h contains definitions for MTOUCH_MATRIX_ROW_START, MTOUCH_MATRIX_ROW_END, MTOUCH_MATRIX_COLUMN_START, and MTOUCH_MATRIX_COLUMN_END.
            
        #endif

        #define MTOUCH_MATRIX_ROW_COUNT         (MTOUCH_MATRIX_ROW_END-MTOUCH_MATRIX_ROW_START+1)
        #define MTOUCH_MATRIX_COLUMN_COUNT      (MTOUCH_MATRIX_COLUMN_END-MTOUCH_MATRIX_COLUMN_START+1)
        
        // Find the full matrix block consisting of all rows and columns.
        #if MTOUCH_MATRIX_ROW_START < MTOUCH_MATRIX_COLUMN_START
            #define MTOUCH_MATRIX_START MTOUCH_MATRIX_ROW_START
            #define MTOUCH_MATRIX_END   MTOUCH_MATRIX_COLUMN_END
            #define MTOUCH_MATRIX_ROWS_FIRST
            
            #if (MTOUCH_MATRIX_ROW_END + 1) != MTOUCH_MATRIX_COLUMN_START
                #define MTOUCH_MATRIX_NOTCONTIGUOUS
            #endif
        #else
            #define MTOUCH_MATRIX_START MTOUCH_MATRIX_COLUMN_START
            #define MTOUCH_MATRIX_END   MTOUCH_MATRIX_ROW_END
            #define MTOUCH_MATRIX_COLUMNS_FIRST  
            
            #if (MTOUCH_MATRIX_COLUMN_END + 1) != MTOUCH_MATRIX_ROW_START
                #define MTOUCH_MATRIX_NOTCONTIGUOUS
            #endif
        #endif
        
        #if defined(MTOUCH_MATRIX_NOTCONTIGUOUS)
            #error The mTouch matrix indexes for rows and columns are not contiguous. Please rearrange the sensor definitions in mTouch_config.h to place the row and column sensor indexes next to each other. Ex: Rows 0-1, Columns 2-3. Or Columns 0-4, Rows 5-10.
        #endif
        
        // Find the non-matrix sensor block
        #if MTOUCH_MATRIX_START == 0
        
            #if MTOUCH_MATRIX_END == (MTOUCH_NUMBER_SENSOR - 1)
                #define MTOUCH_MATRIX_SENSORS_ONLY
            #else
                #define MTOUCH_SENSOR_START MTOUCH_MATRIX_END+1
                #define MTOUCH_SENSOR_END   MTOUCH_NUMBER_SENSORS-1
            #endif
        
            #define MTOUCH_MATRIX_DEFS_AT_START
        #else
            #define MTOUCH_SENSOR_START 0
            #define MTOUCH_SENSOR_END   MTOUCH_MATRIX_START-1
            
            #if (MTOUCH_MATRIX_END + 1) != MTOUCH_NUMBER_SENSORS
                #error The mTouch matrix sensors implementing the rows and columns must be either the very first sensors (in terms of index) or the very last sensors. Right now, the non-matrix sensors are split on either side of the matrix sensors. Please rearrange the sensors in mTouch_config.h.
            #endif
            
            #define MTOUCH_MATRIX_DEFS_AT_END
        #endif
        
        #if !defined(MTOUCH_MOST_PRESSED_THRESHOLD)
            #error Please define MTOUCH_MOST_PRESSED_THRESHOLD in the mTouch_config.h file - it is required for the matrix implementation so that the rows and columns may be run through a most-pressed algorithm to avoid issues with crosstalk.
        #endif
        
        // Used for the result value
        #if MTOUCH_MATRIX_ROW_COUNT == 2
            #define MTOUCH_MATRIX_ROW_OFFSET    1
            #define MTOUCH_MATRIX_ROW_MASK      0x01
        #elif MTOUCH_MATRIX_ROW_COUNT < 5
            #define MTOUCH_MATRIX_ROW_OFFSET    2
            #define MTOUCH_MATRIX_ROW_MASK      0x03
        #elif MTOUCH_MATRIX_ROW_COUNT < 9
            #define MTOUCH_MATRIX_ROW_OFFSET    3
            #define MTOUCH_MATRIX_ROW_MASK      0x07
        #elif MTOUCH_MATRIX_ROW_COUNT < 17
            #define MTOUCH_MATRIX_ROW_OFFSET    4
            #define MTOUCH_MATRIX_ROW_MASK      0x0F
        #else
            #error The mTouch Framework's matrix output logic does not currently support more than 16 rows or columns.    
        #endif
    #else
    
        // If no matrix is being implemented...
        #define MTOUCH_SENSOR_START         0
        #define MTOUCH_SENSOR_END           MTOUCH_NUMBER_SENSORS-1
        
    #endif
    
    #if   defined(PIC_ADC_HCVD_AVAILABLE)
        #define MTOUCH_ADC_INIT()           mTouch_HCVD_Init();
    #else
        #if   (PIC_ADC_BITS ==  8)
            #define MTOUCH_ADC_INIT()       PIC_ADC_SETCLK();
        #elif (PIC_ADC_BITS == 10)
            #define MTOUCH_ADC_INIT()       PIC_ADC_SETCLK();   PIC_ADC_ADFM = PIC_ADC_ADFM_RIGHT;
        #elif (PIC_ADC_BITS == 12)
            #define MTOUCH_ADC_INIT()       PIC_ADC_SETCLK();   PIC_ADC_ADFM = PIC_ADC_ADFM_RIGHT;
        #endif
    #endif
    #if (MTOUCH_NUMBER_SENSORS <= 8)
    #define MTOUCH_STATEMASK_BYTES  1
    #elif (MTOUCH_NUMBER_SENSORS <= 16)
    #define MTOUCH_STATEMASK_BYTES  2
    #elif (MTOUCH_NUMBER_SENSORS <= 24)
    #define MTOUCH_STATEMASK_BYTES  3
    #elif (MTOUCH_NUMBER_SENSORS <= 32)
    #define MTOUCH_STATEMASK_BYTES  4
    #endif
    
    #if defined(MTOUCH_SLIDERS_ENABLED) || defined(MTOUCH_MOST_PRESSED_ONLY) || defined(MTOUCH_MATRIX_ENABLED)
        #define MTOUCH_REQUIRES_DELTA_ARRAY
    #endif
    
    #if MTOUCH_NUMBER_PROXIMITY > 0
        #define MTOUCH_PROXIMITY_ENABLED
    #endif

    #define MTOUCH_STATEMACHINE_0       &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_1       , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_2       , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_3       , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_4       , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_5       , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_6       , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_7       , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_8       , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_9       , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_10      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_11      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_12      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_13      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_14      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_15      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_16      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_17      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_18      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_19      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_20      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_21      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_22      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_23      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_24      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_25      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_26      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_27      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_28      , &mTouch_ButtonStateMachine
    #define MTOUCH_STATEMACHINE_29      , &mTouch_ButtonStateMachine
    
    #define MTOUCH_SENSOR0_PROXINDEX    MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR1_PROXINDEX    , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR2_PROXINDEX    , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR3_PROXINDEX    , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR4_PROXINDEX    , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR5_PROXINDEX    , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR6_PROXINDEX    , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR7_PROXINDEX    , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR8_PROXINDEX    , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR9_PROXINDEX    , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR10_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR11_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR12_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR13_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR14_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR15_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR16_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR17_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR18_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR19_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR20_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR21_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR22_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR23_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR24_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR25_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR26_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR27_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR28_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    #define MTOUCH_SENSOR29_PROXINDEX   , MTOUCH_NUMBER_SENSORS
    
    #if MTOUCH_NUMBER_SENSORS > 0
    #if defined(MTOUCH_SENSOR0_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_0
        #undef  MTOUCH_SENSOR0_PROXINDEX
        #define MTOUCH_STATEMACHINE_0       &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR0_PROXINDEX    MTOUCH_SENSOR0_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR0_PROXINDEX
        #define MTOUCH_SENSOR0_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_0
        #define MTOUCH_STATEMACHINE_0
    #endif
    #if MTOUCH_NUMBER_SENSORS > 1
    #if defined(MTOUCH_SENSOR1_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_1
        #undef  MTOUCH_SENSOR1_PROXINDEX
        #define MTOUCH_STATEMACHINE_1       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR1_PROXINDEX    , MTOUCH_SENSOR1_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR1_PROXINDEX
        #define MTOUCH_SENSOR1_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_1
        #define MTOUCH_STATEMACHINE_1
    #endif
    #if MTOUCH_NUMBER_SENSORS > 2
    #if defined(MTOUCH_SENSOR2_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_2
        #undef  MTOUCH_SENSOR2_PROXINDEX
        #define MTOUCH_STATEMACHINE_2       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR2_PROXINDEX    , MTOUCH_SENSOR2_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR2_PROXINDEX
        #define MTOUCH_SENSOR2_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_2
        #define MTOUCH_STATEMACHINE_2
    #endif
    #if MTOUCH_NUMBER_SENSORS > 3
    #if defined(MTOUCH_SENSOR3_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_3
        #undef  MTOUCH_SENSOR3_PROXINDEX
        #define MTOUCH_STATEMACHINE_3       , &mTouch_ProxStateMachine       
        #define MTOUCH_SENSOR3_PROXINDEX    , MTOUCH_SENSOR3_IS_PROX   
    #endif
    #else
        #undef  MTOUCH_SENSOR3_PROXINDEX
        #define MTOUCH_SENSOR3_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_3
        #define MTOUCH_STATEMACHINE_3
    #endif
    #if MTOUCH_NUMBER_SENSORS > 4
    #if defined(MTOUCH_SENSOR4_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_4
        #undef  MTOUCH_SENSOR4_PROXINDEX
        #define MTOUCH_STATEMACHINE_4       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR4_PROXINDEX    , MTOUCH_SENSOR4_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR4_PROXINDEX
        #define MTOUCH_SENSOR4_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_4
        #define MTOUCH_STATEMACHINE_4
    #endif
    #if MTOUCH_NUMBER_SENSORS > 5
    #if defined(MTOUCH_SENSOR5_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_5
        #undef  MTOUCH_SENSOR5_PROXINDEX
        #define MTOUCH_STATEMACHINE_5       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR5_PROXINDEX    , MTOUCH_SENSOR5_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR5_PROXINDEX
        #define MTOUCH_SENSOR5_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_5
        #define MTOUCH_STATEMACHINE_5
    #endif
    #if MTOUCH_NUMBER_SENSORS > 6
    #if defined(MTOUCH_SENSOR6_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_6
        #undef  MTOUCH_SENSOR6_PROXINDEX
        #define MTOUCH_STATEMACHINE_6       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR6_PROXINDEX    , MTOUCH_SENSOR6_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR6_PROXINDEX
        #define MTOUCH_SENSOR6_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_6
        #define MTOUCH_STATEMACHINE_6
    #endif
    #if MTOUCH_NUMBER_SENSORS > 7
    #if defined(MTOUCH_SENSOR7_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_7
        #undef  MTOUCH_SENSOR7_PROXINDEX
        #define MTOUCH_STATEMACHINE_7       , &mTouch_ProxStateMachine       
        #define MTOUCH_SENSOR7_PROXINDEX    , MTOUCH_SENSOR7_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR7_PROXINDEX
        #define MTOUCH_SENSOR7_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_7
        #define MTOUCH_STATEMACHINE_7
    #endif
    #if MTOUCH_NUMBER_SENSORS > 8
    #if defined(MTOUCH_SENSOR8_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_8
        #undef  MTOUCH_SENSOR8_PROXINDEX
        #define MTOUCH_STATEMACHINE_8       , &mTouch_ProxStateMachine       
        #define MTOUCH_SENSOR8_PROXINDEX    , MTOUCH_SENSOR8_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR8_PROXINDEX
        #define MTOUCH_SENSOR8_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_8
        #define MTOUCH_STATEMACHINE_8
    #endif
    #if MTOUCH_NUMBER_SENSORS > 9
    #if defined(MTOUCH_SENSOR9_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_9
        #undef  MTOUCH_SENSOR9_PROXINDEX
        #define MTOUCH_STATEMACHINE_9       , &mTouch_ProxStateMachine          
        #define MTOUCH_SENSOR9_PROXINDEX    , MTOUCH_SENSOR9_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR9_PROXINDEX
        #define MTOUCH_SENSOR9_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_9
        #define MTOUCH_STATEMACHINE_9
    #endif
    #if MTOUCH_NUMBER_SENSORS > 10
    #if defined(MTOUCH_SENSOR10_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_10
        #undef  MTOUCH_SENSOR10_PROXINDEX
        #define MTOUCH_STATEMACHINE_10       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR10_PROXINDEX    , MTOUCH_SENSOR10_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR10_PROXINDEX
        #define MTOUCH_SENSOR10_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_10
        #define MTOUCH_STATEMACHINE_10
    #endif
    #if MTOUCH_NUMBER_SENSORS > 11
    #if defined(MTOUCH_SENSOR11_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_11
        #undef  MTOUCH_SENSOR11_PROXINDEX
        #define MTOUCH_STATEMACHINE_11       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR11_PROXINDEX    , MTOUCH_SENSOR11_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR11_PROXINDEX
        #define MTOUCH_SENSOR11_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_11
        #define MTOUCH_STATEMACHINE_11
    #endif
    #if MTOUCH_NUMBER_SENSORS > 12
    #if defined(MTOUCH_SENSOR12_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_12
        #undef  MTOUCH_SENSOR12_PROXINDEX
        #define MTOUCH_STATEMACHINE_12       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR12_PROXINDEX    , MTOUCH_SENSOR12_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR12_PROXINDEX
        #define MTOUCH_SENSOR12_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_12
        #define MTOUCH_STATEMACHINE_12
    #endif
    #if MTOUCH_NUMBER_SENSORS > 13
    #if defined(MTOUCH_SENSOR13_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_13
        #undef  MTOUCH_SENSOR13_PROXINDEX
        #define MTOUCH_STATEMACHINE_13       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR13_PROXINDEX    , MTOUCH_SENSOR13_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR13_PROXINDEX
        #define MTOUCH_SENSOR13_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_13
        #define MTOUCH_STATEMACHINE_13
    #endif
    #if MTOUCH_NUMBER_SENSORS > 14
    #if defined(MTOUCH_SENSOR14_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_14
        #undef  MTOUCH_SENSOR14_PROXINDEX
        #define MTOUCH_STATEMACHINE_14       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR14_PROXINDEX    , MTOUCH_SENSOR14_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR14_PROXINDEX
        #define MTOUCH_SENSOR14_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_14
        #define MTOUCH_STATEMACHINE_14
    #endif
    #if MTOUCH_NUMBER_SENSORS > 15
    #if defined(MTOUCH_SENSOR15_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_15
        #undef  MTOUCH_SENSOR15_PROXINDEX
        #define MTOUCH_STATEMACHINE_15       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR15_PROXINDEX    , MTOUCH_SENSOR15_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR15_PROXINDEX
        #define MTOUCH_SENSOR15_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_15
        #define MTOUCH_STATEMACHINE_15
    #endif
    #if MTOUCH_NUMBER_SENSORS > 16
    #if defined(MTOUCH_SENSOR16_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_16
        #undef  MTOUCH_SENSOR16_PROXINDEX
        #define MTOUCH_STATEMACHINE_16       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR16_PROXINDEX    , MTOUCH_SENSOR16_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR16_PROXINDEX
        #define MTOUCH_SENSOR16_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_16
        #define MTOUCH_STATEMACHINE_16
    #endif
    #if MTOUCH_NUMBER_SENSORS > 17
    #if defined(MTOUCH_SENSOR17_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_17
        #undef  MTOUCH_SENSOR17_PROXINDEX
        #define MTOUCH_STATEMACHINE_17       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR17_PROXINDEX    , MTOUCH_SENSOR17_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR17_PROXINDEX
        #define MTOUCH_SENSOR17_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_17
        #define MTOUCH_STATEMACHINE_17
    #endif
    #if MTOUCH_NUMBER_SENSORS > 18
    #if defined(MTOUCH_SENSOR18_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_18
        #undef  MTOUCH_SENSOR18_PROXINDEX
        #define MTOUCH_STATEMACHINE_18       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR18_PROXINDEX    , MTOUCH_SENSOR18_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR18_PROXINDEX
        #define MTOUCH_SENSOR18_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_18
        #define MTOUCH_STATEMACHINE_18
    #endif
    #if MTOUCH_NUMBER_SENSORS > 19
    #if defined(MTOUCH_SENSOR19_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_19
        #undef  MTOUCH_SENSOR19_PROXINDEX
        #define MTOUCH_STATEMACHINE_19       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR19_PROXINDEX    , MTOUCH_SENSOR19_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR19_PROXINDEX
        #define MTOUCH_SENSOR19_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_19
        #define MTOUCH_STATEMACHINE_19
    #endif
    #if MTOUCH_NUMBER_SENSORS > 20
    #if defined(MTOUCH_SENSOR20_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_20
        #undef  MTOUCH_SENSOR20_PROXINDEX
        #define MTOUCH_STATEMACHINE_20       , &mTouch_ProxStateMachine       
        #define MTOUCH_SENSOR20_PROXINDEX    , MTOUCH_SENSOR20_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR20_PROXINDEX
        #define MTOUCH_SENSOR20_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_20
        #define MTOUCH_STATEMACHINE_20
    #endif
    #if MTOUCH_NUMBER_SENSORS > 21
    #if defined(MTOUCH_SENSOR21_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_21
        #undef  MTOUCH_SENSOR21_PROXINDEX
        #define MTOUCH_STATEMACHINE_21       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR21_PROXINDEX    , MTOUCH_SENSOR21_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR21_PROXINDEX
        #define MTOUCH_SENSOR21_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_21
        #define MTOUCH_STATEMACHINE_21
    #endif
    #if MTOUCH_NUMBER_SENSORS > 22
    #if defined(MTOUCH_SENSOR22_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_22
        #undef  MTOUCH_SENSOR22_PROXINDEX
        #define MTOUCH_STATEMACHINE_22       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR22_PROXINDEX    , MTOUCH_SENSOR22_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR22_PROXINDEX
        #define MTOUCH_SENSOR22_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_22
        #define MTOUCH_STATEMACHINE_22
    #endif
    #if MTOUCH_NUMBER_SENSORS > 23
    #if defined(MTOUCH_SENSOR23_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_23
        #undef  MTOUCH_SENSOR23_PROXINDEX
        #define MTOUCH_STATEMACHINE_23       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR23_PROXINDEX    , MTOUCH_SENSOR23_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR23_PROXINDEX
        #define MTOUCH_SENSOR23_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_23
        #define MTOUCH_STATEMACHINE_23
    #endif
    #if MTOUCH_NUMBER_SENSORS > 24
    #if defined(MTOUCH_SENSOR24_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_24
        #undef  MTOUCH_SENSOR24_PROXINDEX
        #define MTOUCH_STATEMACHINE_24       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR24_PROXINDEX    , MTOUCH_SENSOR24_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR24_PROXINDEX
        #define MTOUCH_SENSOR24_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_24
        #define MTOUCH_STATEMACHINE_24
    #endif
    #if MTOUCH_NUMBER_SENSORS > 25
    #if defined(MTOUCH_SENSOR25_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_25
        #undef  MTOUCH_SENSOR25_PROXINDEX
        #define MTOUCH_STATEMACHINE_25       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR25_PROXINDEX    , MTOUCH_SENSOR25_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR25_PROXINDEX
        #define MTOUCH_SENSOR25_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_25
        #define MTOUCH_STATEMACHINE_25
    #endif
    #if MTOUCH_NUMBER_SENSORS > 26
    #if defined(MTOUCH_SENSOR26_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_26
        #undef  MTOUCH_SENSOR26_PROXINDEX
        #define MTOUCH_STATEMACHINE_26       , &mTouch_ProxStateMachine         
        #define MTOUCH_SENSOR26_PROXINDEX    , MTOUCH_SENSOR26_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR26_PROXINDEX
        #define MTOUCH_SENSOR26_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_26
        #define MTOUCH_STATEMACHINE_26
    #endif
    #if MTOUCH_NUMBER_SENSORS > 27
    #if defined(MTOUCH_SENSOR27_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_27
        #undef  MTOUCH_SENSOR27_PROXINDEX
        #define MTOUCH_STATEMACHINE_27       , &mTouch_ProxStateMachine         
        #define MTOUCH_SENSOR27_PROXINDEX    , MTOUCH_SENSOR27_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR27_PROXINDEX
        #define MTOUCH_SENSOR27_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_27
        #define MTOUCH_STATEMACHINE_27
    #endif
    #if MTOUCH_NUMBER_SENSORS > 28
    #if defined(MTOUCH_SENSOR28_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_28
        #undef  MTOUCH_SENSOR28_PROXINDEX
        #define MTOUCH_STATEMACHINE_28       , &mTouch_ProxStateMachine       
        #define MTOUCH_SENSOR28_PROXINDEX    , MTOUCH_SENSOR28_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR28_PROXINDEX
        #define MTOUCH_SENSOR28_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_28
        #define MTOUCH_STATEMACHINE_28
    #endif
    #if MTOUCH_NUMBER_SENSORS > 29
    #if defined(MTOUCH_SENSOR29_IS_PROX) && defined(MTOUCH_PROXIMITY_ENABLED)
        #undef  MTOUCH_STATEMACHINE_29
        #undef  MTOUCH_SENSOR29_PROXINDEX
        #define MTOUCH_STATEMACHINE_29       , &mTouch_ProxStateMachine        
        #define MTOUCH_SENSOR29_PROXINDEX    , MTOUCH_SENSOR29_IS_PROX    
    #endif
    #else
        #undef  MTOUCH_SENSOR29_PROXINDEX
        #define MTOUCH_SENSOR29_PROXINDEX
        #undef  MTOUCH_STATEMACHINE_29       
        #define MTOUCH_STATEMACHINE_29       
    #endif


    
    #if defined(MTOUCH_EEPROM_ENABLED)
        #define MTOUCH_CONFIG_EEPROM    0x01
    #else
        #define MTOUCH_CONFIG_EEPROM    0x00
    #endif

    #define MTOUCH_CVD_OR_CTMU      0x00

    #if defined(MTOUCH_EEPROM_STORED_IN_RAM)
        #define MTOUCH_CONFIG_EERAMSTORAGE  0x80
    #else
        #define MTOUCH_CONFIG_EERAMSTORAGE  0x00
    #endif
    
    #define MTOUCH_CONFIGURATION_BYTE   (MTOUCH_CVD_OR_CTMU | MTOUCH_CONFIG_EEPROM | MTOUCH_CONFIG_EERAMSTORAGE)
    
    
#endif
/// @endcond
