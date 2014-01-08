/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_modes.h
 *  Dependencies:    
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework Scanning Modes Header File
 *************************************************************************/
 /***********************************************************************
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
/** @file   mTouch_modes.h
*   @brief  Internal framework header file implementing scanning mode functionality
*/
#ifndef __MTOUCH_MODES_H
#define __MTOUCH_MODES_H

#if !defined(MTOUCH_NUM_MODES)
#define MTOUCH_NUM_MODES    0
#endif

#if MTOUCH_NUM_MODES > 1

    #define MTOUCH_MODE_SENSOR(modeIndex, orderIndex)           __4paste(MTOUCH_MODE, modeIndex, _SENSOR, orderIndex)
    #define MTOUCH_MODE_SENSOR_ENTRY(modeIndex, orderIndex)     MTOUCH_MODE_SENSOR(modeIndex, orderIndex)
    #define MTOUCH_MODE_SENSOR_END(modeIndex)                   , MTOUCH_NUMBER_SENSORS
    
    // Constant Memory Values
    #if     MTOUCH_NUM_MODES == 2
    #define MTOUCH_MODE_VAR_INIT                { &mTouch_mode0[0], &mTouch_mode1[0] }
    #elif   MTOUCH_NUM_MODES == 3
    #define MTOUCH_MODE_VAR_INIT                { &mTouch_mode0[0], &mTouch_mode1[0], &mTouch_mode2[0] }
    #elif   MTOUCH_NUM_MODES == 4
    #define MTOUCH_MODE_VAR_INIT                { &mTouch_mode0[0], &mTouch_mode1[0], &mTouch_mode2[0], &mTouch_mode3[0] }
    #elif   MTOUCH_NUM_MODES == 5
    #define MTOUCH_MODE_VAR_INIT                { &mTouch_mode0[0], &mTouch_mode1[0], &mTouch_mode2[0], &mTouch_mode3[0], &mTouch_mode4[0] }
    #elif   MTOUCH_NUM_MODES == 6
    #define MTOUCH_MODE_VAR_INIT                { &mTouch_mode0[0], &mTouch_mode1[0], &mTouch_mode2[0], &mTouch_mode3[0], &mTouch_mode4[0], &mTouch_mode5[0] }
    #elif   MTOUCH_NUM_MODES == 7
    #define MTOUCH_MODE_VAR_INIT                { &mTouch_mode0[0], &mTouch_mode1[0], &mTouch_mode2[0], &mTouch_mode3[0], &mTouch_mode4[0], &mTouch_mode5[0], &mTouch_mode6[0] }
    #elif   MTOUCH_NUM_MODES == 8
    #define MTOUCH_MODE_VAR_INIT                { &mTouch_mode0[0], &mTouch_mode1[0], &mTouch_mode2[0], &mTouch_mode3[0], &mTouch_mode4[0], &mTouch_mode5[0], &mTouch_mode6[0], &mTouch_mode7[0] }
    #endif
    
    #if MTOUCH_NUM_MODES > 1
        #if !defined(MTOUCH_MODE0_NUM_SENSORS)
            #error MTOUCH_NUM_MODES is greater than 1, but MTOUCH_MODE0_NUM_SENSORS is not defined.
        #endif
        #if !defined(MTOUCH_MODE1_NUM_SENSORS)
            #error MTOUCH_NUM_MODES is greater than 1, but MTOUCH_MODE1_NUM_SENSORS is not defined.
        #endif
    #endif
    #if MTOUCH_NUM_MODES > 2
        #if !defined(MTOUCH_MODE2_NUM_SENSORS)
            #error MTOUCH_NUM_MODES is greater than 2, but MTOUCH_MODE2_NUM_SENSORS is not defined.
        #endif
    #endif    
    #if MTOUCH_NUM_MODES > 3
        #if !defined(MTOUCH_MODE3_NUM_SENSORS)
            #error MTOUCH_NUM_MODES is greater than 3, but MTOUCH_MODE3_NUM_SENSORS is not defined.
        #endif
    #endif    
    #if MTOUCH_NUM_MODES > 4
        #if !defined(MTOUCH_MODE4_NUM_SENSORS)
            #error MTOUCH_NUM_MODES is greater than 4, but MTOUCH_MODE4_NUM_SENSORS is not defined.
        #endif
    #endif    
    #if MTOUCH_NUM_MODES > 5
        #if !defined(MTOUCH_MODE5_NUM_SENSORS)
            #error MTOUCH_NUM_MODES is greater than 5, but MTOUCH_MODE5_NUM_SENSORS is not defined.
        #endif
    #endif    
    #if MTOUCH_NUM_MODES > 6
        #if !defined(MTOUCH_MODE6_NUM_SENSORS)
            #error MTOUCH_NUM_MODES is greater than 6, but MTOUCH_MODE6_NUM_SENSORS is not defined.
        #endif
    #endif    
    #if MTOUCH_NUM_MODES > 7
        #if !defined(MTOUCH_MODE7_NUM_SENSORS)
            #error MTOUCH_NUM_MODES is greater than 7, but MTOUCH_MODE7_NUM_SENSORS is not defined.
        #endif
    #endif
    
    
    //
    //
    //
    //
    //      MODE 0
    //
    //
    //
    //
    //
    

    #if MTOUCH_MODE0_NUM_SENSORS > 0
        #if !defined(MTOUCH_MODE0_SENSOR0)
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 0, but MTOUCH_MODE0_SENSOR0 is not defined.
        #endif
    #else
        #error MTOUCH_MODE0_NUM_SENSORS must be greater than 0.
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS >  1  
        #if !defined(MTOUCH_MODE0_SENSOR1)       
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 1, but MTOUCH_MODE0_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS >  2 
        #if !defined(MTOUCH_MODE0_SENSOR2)       
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 2, but MTOUCH_MODE0_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS >  3  
        #if !defined(MTOUCH_MODE0_SENSOR3)       
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 3, but MTOUCH_MODE0_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS >  4  
        #if !defined(MTOUCH_MODE0_SENSOR4)       
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 4, but MTOUCH_MODE0_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS >  5 
        #if !defined(MTOUCH_MODE0_SENSOR5)       
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 5, but MTOUCH_MODE0_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS >  6  
        #if !defined(MTOUCH_MODE0_SENSOR6)       
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 6, but MTOUCH_MODE0_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS >  7 
        #if !defined(MTOUCH_MODE0_SENSOR7)       
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 7, but MTOUCH_MODE0_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS >  8  
        #if !defined(MTOUCH_MODE0_SENSOR8)       
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 8, but MTOUCH_MODE0_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS >  9  
        #if !defined(MTOUCH_MODE0_SENSOR9)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 9, but MTOUCH_MODE0_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 10  
        #if !defined(MTOUCH_MODE0_SENSOR0)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 0, but MTOUCH_MODE0_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 11  
        #if !defined(MTOUCH_MODE0_SENSOR1)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 1, but MTOUCH_MODE0_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 12  
        #if !defined(MTOUCH_MODE0_SENSOR2)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 2, but MTOUCH_MODE0_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 13  
        #if !defined(MTOUCH_MODE0_SENSOR3)       
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 3, but MTOUCH_MODE0_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 14  
        #if !defined(MTOUCH_MODE0_SENSOR4)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 4, but MTOUCH_MODE0_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 15  
        #if !defined(MTOUCH_MODE0_SENSOR5)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 5, but MTOUCH_MODE0_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 16 
        #if !defined(MTOUCH_MODE0_SENSOR6)
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 6, but MTOUCH_MODE0_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 17
        #if !defined(MTOUCH_MODE0_SENSOR7)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 7, but MTOUCH_MODE0_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 18 
        #if !defined(MTOUCH_MODE0_SENSOR8)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 8, but MTOUCH_MODE0_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 19 
        #if !defined(MTOUCH_MODE0_SENSOR9)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 9, but MTOUCH_MODE0_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 20 
        #if !defined(MTOUCH_MODE0_SENSOR0)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 0, but MTOUCH_MODE0_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 21   
        #if !defined(MTOUCH_MODE0_SENSOR1)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 1, but MTOUCH_MODE0_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 22 
        #if !defined(MTOUCH_MODE0_SENSOR2)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 2, but MTOUCH_MODE0_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 23   
        #if !defined(MTOUCH_MODE0_SENSOR3)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 3, but MTOUCH_MODE0_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 24   
        #if !defined(MTOUCH_MODE0_SENSOR4)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 4, but MTOUCH_MODE0_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 25 
        #if !defined(MTOUCH_MODE0_SENSOR5)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 5, but MTOUCH_MODE0_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 26  
        #if !defined(MTOUCH_MODE0_SENSOR6)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 6, but MTOUCH_MODE0_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 27   
        #if !defined(MTOUCH_MODE0_SENSOR7)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 7, but MTOUCH_MODE0_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 28   
        #if !defined(MTOUCH_MODE0_SENSOR8)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 8, but MTOUCH_MODE0_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS > 29   
        #if !defined(MTOUCH_MODE0_SENSOR9)        
        #error MTOUCH_MODE0_NUM_SENSORS is greater than 9, but MTOUCH_MODE0_SENSOR9 is not defined.     
        #endif    
    #endif
    
    
    #define MTOUCH_MODE0_ORDER0_INIT              MTOUCH_MODE_SENSOR(0, 0)
    #define MTOUCH_MODE0_ORDER1_INIT            , MTOUCH_MODE_SENSOR(0, 1)
    #define MTOUCH_MODE0_ORDER2_INIT            , MTOUCH_MODE_SENSOR(0, 2)
    #define MTOUCH_MODE0_ORDER3_INIT            , MTOUCH_MODE_SENSOR(0, 3)
    #define MTOUCH_MODE0_ORDER4_INIT            , MTOUCH_MODE_SENSOR(0, 4)
    #define MTOUCH_MODE0_ORDER5_INIT            , MTOUCH_MODE_SENSOR(0, 5)
    #define MTOUCH_MODE0_ORDER6_INIT            , MTOUCH_MODE_SENSOR(0, 6)
    #define MTOUCH_MODE0_ORDER7_INIT            , MTOUCH_MODE_SENSOR(0, 7)
    #define MTOUCH_MODE0_ORDER8_INIT            , MTOUCH_MODE_SENSOR(0, 8)
    #define MTOUCH_MODE0_ORDER9_INIT            , MTOUCH_MODE_SENSOR(0, 9)
    #define MTOUCH_MODE0_ORDER10_INIT           , MTOUCH_MODE_SENSOR(0, 10)
    #define MTOUCH_MODE0_ORDER11_INIT           , MTOUCH_MODE_SENSOR(0, 11)
    #define MTOUCH_MODE0_ORDER12_INIT           , MTOUCH_MODE_SENSOR(0, 12)
    #define MTOUCH_MODE0_ORDER13_INIT           , MTOUCH_MODE_SENSOR(0, 13)
    #define MTOUCH_MODE0_ORDER14_INIT           , MTOUCH_MODE_SENSOR(0, 14)
    #define MTOUCH_MODE0_ORDER15_INIT           , MTOUCH_MODE_SENSOR(0, 15)
    #define MTOUCH_MODE0_ORDER16_INIT           , MTOUCH_MODE_SENSOR(0, 16)
    #define MTOUCH_MODE0_ORDER17_INIT           , MTOUCH_MODE_SENSOR(0, 17)
    #define MTOUCH_MODE0_ORDER18_INIT           , MTOUCH_MODE_SENSOR(0, 18)
    #define MTOUCH_MODE0_ORDER19_INIT           , MTOUCH_MODE_SENSOR(0, 19)
    #define MTOUCH_MODE0_ORDER20_INIT           , MTOUCH_MODE_SENSOR(0, 20)
    #define MTOUCH_MODE0_ORDER21_INIT           , MTOUCH_MODE_SENSOR(0, 21)
    #define MTOUCH_MODE0_ORDER22_INIT           , MTOUCH_MODE_SENSOR(0, 22)
    #define MTOUCH_MODE0_ORDER23_INIT           , MTOUCH_MODE_SENSOR(0, 23)
    #define MTOUCH_MODE0_ORDER24_INIT           , MTOUCH_MODE_SENSOR(0, 24)
    #define MTOUCH_MODE0_ORDER25_INIT           , MTOUCH_MODE_SENSOR(0, 25)
    #define MTOUCH_MODE0_ORDER26_INIT           , MTOUCH_MODE_SENSOR(0, 26)
    #define MTOUCH_MODE0_ORDER27_INIT           , MTOUCH_MODE_SENSOR(0, 27)
    #define MTOUCH_MODE0_ORDER28_INIT           , MTOUCH_MODE_SENSOR(0, 28)
    #define MTOUCH_MODE0_ORDER29_INIT           , MTOUCH_MODE_SENSOR(0, 29)
   
    #if MTOUCH_MODE0_NUM_SENSORS < 30
        #undef  MTOUCH_MODE0_ORDER29_INIT
        #define MTOUCH_MODE0_ORDER29_INIT
    #endif    
    #if MTOUCH_MODE0_NUM_SENSORS < 29
        #undef  MTOUCH_MODE0_ORDER28_INIT
        #define MTOUCH_MODE0_ORDER28_INIT
    #endif    
    #if MTOUCH_MODE0_NUM_SENSORS < 28
        #undef  MTOUCH_MODE0_ORDER27_INIT
        #define MTOUCH_MODE0_ORDER27_INIT
    #endif    
    #if MTOUCH_MODE0_NUM_SENSORS < 27
        #undef  MTOUCH_MODE0_ORDER26_INIT
        #define MTOUCH_MODE0_ORDER26_INIT
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 26        
        #undef  MTOUCH_MODE0_ORDER25_INIT          
        #define MTOUCH_MODE0_ORDER25_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 25        
        #undef  MTOUCH_MODE0_ORDER24_INIT          
        #define MTOUCH_MODE0_ORDER24_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 24        
        #undef  MTOUCH_MODE0_ORDER23_INIT          
        #define MTOUCH_MODE0_ORDER23_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 23        
        #undef  MTOUCH_MODE0_ORDER22_INIT          
        #define MTOUCH_MODE0_ORDER22_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 22        
        #undef  MTOUCH_MODE0_ORDER21_INIT          
        #define MTOUCH_MODE0_ORDER21_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 21        
        #undef  MTOUCH_MODE0_ORDER20_INIT          
        #define MTOUCH_MODE0_ORDER20_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 20        
        #undef  MTOUCH_MODE0_ORDER19_INIT          
        #define MTOUCH_MODE0_ORDER19_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 19        
        #undef  MTOUCH_MODE0_ORDER18_INIT          
        #define MTOUCH_MODE0_ORDER18_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 18        
        #undef  MTOUCH_MODE0_ORDER17_INIT          
        #define MTOUCH_MODE0_ORDER17_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 17        
        #undef  MTOUCH_MODE0_ORDER16_INIT          
        #define MTOUCH_MODE0_ORDER16_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 16        
        #undef  MTOUCH_MODE0_ORDER15_INIT          
        #define MTOUCH_MODE0_ORDER15_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 15        
        #undef  MTOUCH_MODE0_ORDER14_INIT          
        #define MTOUCH_MODE0_ORDER14_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 14        
        #undef  MTOUCH_MODE0_ORDER13_INIT          
        #define MTOUCH_MODE0_ORDER13_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 13        
        #undef  MTOUCH_MODE0_ORDER12_INIT          
        #define MTOUCH_MODE0_ORDER12_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 12        
        #undef  MTOUCH_MODE0_ORDER11_INIT          
        #define MTOUCH_MODE0_ORDER11_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 11        
        #undef  MTOUCH_MODE0_ORDER10_INIT          
        #define MTOUCH_MODE0_ORDER10_INIT       
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS < 10        
        #undef  MTOUCH_MODE0_ORDER9_INIT           
        #define MTOUCH_MODE0_ORDER9_INIT        
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS <  9        
        #undef  MTOUCH_MODE0_ORDER8_INIT           
        #define MTOUCH_MODE0_ORDER8_INIT        
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS <  8        
        #undef  MTOUCH_MODE0_ORDER7_INIT           
        #define MTOUCH_MODE0_ORDER7_INIT        
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS <  7        
        #undef  MTOUCH_MODE0_ORDER6_INIT           
        #define MTOUCH_MODE0_ORDER6_INIT        
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS <  6        
        #undef  MTOUCH_MODE0_ORDER5_INIT           
        #define MTOUCH_MODE0_ORDER5_INIT        
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS <  5        
        #undef  MTOUCH_MODE0_ORDER4_INIT           
        #define MTOUCH_MODE0_ORDER4_INIT        
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS <  4        
        #undef  MTOUCH_MODE0_ORDER3_INIT           
        #define MTOUCH_MODE0_ORDER3_INIT        
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS <  3        
        #undef  MTOUCH_MODE0_ORDER2_INIT           
        #define MTOUCH_MODE0_ORDER2_INIT        
    #endif
    #if MTOUCH_MODE0_NUM_SENSORS <  2        
        #undef  MTOUCH_MODE0_ORDER1_INIT           
        #define MTOUCH_MODE0_ORDER1_INIT        
    #endif
   
    #if MTOUCH_MODE0_NUM_SENSORS > 0
    #define MTOUCH_MODE0_VAR_INIT               {                             \
                                                    MTOUCH_MODE0_ORDER0_INIT  \
                                                    MTOUCH_MODE0_ORDER1_INIT  \
                                                    MTOUCH_MODE0_ORDER2_INIT  \
                                                    MTOUCH_MODE0_ORDER3_INIT  \
                                                    MTOUCH_MODE0_ORDER4_INIT  \
                                                    MTOUCH_MODE0_ORDER5_INIT  \
                                                    MTOUCH_MODE0_ORDER6_INIT  \
                                                    MTOUCH_MODE0_ORDER7_INIT  \
                                                    MTOUCH_MODE0_ORDER8_INIT  \
                                                    MTOUCH_MODE0_ORDER9_INIT  \
                                                    MTOUCH_MODE0_ORDER10_INIT \
                                                    MTOUCH_MODE0_ORDER11_INIT \
                                                    MTOUCH_MODE0_ORDER12_INIT \
                                                    MTOUCH_MODE0_ORDER13_INIT \
                                                    MTOUCH_MODE0_ORDER14_INIT \
                                                    MTOUCH_MODE0_ORDER15_INIT \
                                                    MTOUCH_MODE0_ORDER16_INIT \
                                                    MTOUCH_MODE0_ORDER17_INIT \
                                                    MTOUCH_MODE0_ORDER18_INIT \
                                                    MTOUCH_MODE0_ORDER19_INIT \
                                                    MTOUCH_MODE0_ORDER20_INIT \
                                                    MTOUCH_MODE0_ORDER21_INIT \
                                                    MTOUCH_MODE0_ORDER22_INIT \
                                                    MTOUCH_MODE0_ORDER23_INIT \
                                                    MTOUCH_MODE0_ORDER24_INIT \
                                                    MTOUCH_MODE0_ORDER25_INIT \
                                                    MTOUCH_MODE0_ORDER26_INIT \
                                                    MTOUCH_MODE0_ORDER27_INIT \
                                                    MTOUCH_MODE0_ORDER28_INIT \
                                                    MTOUCH_MODE0_ORDER29_INIT \
                                                    MTOUCH_MODE_SENSOR_END(0) \
                                                }    
    #endif
    
    //
    //
    //
    //
    //      MODE 1
    //
    //
    //
    //
    //

    #if MTOUCH_MODE1_NUM_SENSORS > 0
        #if !defined(MTOUCH_MODE1_SENSOR0)
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 0, but MTOUCH_MODE1_SENSOR0 is not defined.
        #endif
    #else
        #error MTOUCH_MODE1_NUM_SENSORS must be greater than 0.
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS >  1  
        #if !defined(MTOUCH_MODE1_SENSOR1)       
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 1, but MTOUCH_MODE1_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS >  2 
        #if !defined(MTOUCH_MODE1_SENSOR2)       
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 2, but MTOUCH_MODE1_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS >  3  
        #if !defined(MTOUCH_MODE1_SENSOR3)       
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 3, but MTOUCH_MODE1_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS >  4  
        #if !defined(MTOUCH_MODE1_SENSOR4)       
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 4, but MTOUCH_MODE1_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS >  5 
        #if !defined(MTOUCH_MODE1_SENSOR5)       
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 5, but MTOUCH_MODE1_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS >  6  
        #if !defined(MTOUCH_MODE1_SENSOR6)       
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 6, but MTOUCH_MODE1_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS >  7 
        #if !defined(MTOUCH_MODE1_SENSOR7)       
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 7, but MTOUCH_MODE1_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS >  8  
        #if !defined(MTOUCH_MODE1_SENSOR8)       
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 8, but MTOUCH_MODE1_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS >  9  
        #if !defined(MTOUCH_MODE1_SENSOR9)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 9, but MTOUCH_MODE1_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 10  
        #if !defined(MTOUCH_MODE1_SENSOR0)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 0, but MTOUCH_MODE1_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 11  
        #if !defined(MTOUCH_MODE1_SENSOR1)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 1, but MTOUCH_MODE1_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 12  
        #if !defined(MTOUCH_MODE1_SENSOR2)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 2, but MTOUCH_MODE1_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 13  
        #if !defined(MTOUCH_MODE1_SENSOR3)       
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 3, but MTOUCH_MODE1_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 14  
        #if !defined(MTOUCH_MODE1_SENSOR4)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 4, but MTOUCH_MODE1_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 15  
        #if !defined(MTOUCH_MODE1_SENSOR5)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 5, but MTOUCH_MODE1_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 16 
        #if !defined(MTOUCH_MODE1_SENSOR6)
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 6, but MTOUCH_MODE1_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 17
        #if !defined(MTOUCH_MODE1_SENSOR7)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 7, but MTOUCH_MODE1_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 18 
        #if !defined(MTOUCH_MODE1_SENSOR8)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 8, but MTOUCH_MODE1_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 19 
        #if !defined(MTOUCH_MODE1_SENSOR9)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 9, but MTOUCH_MODE1_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 20 
        #if !defined(MTOUCH_MODE1_SENSOR0)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 0, but MTOUCH_MODE1_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 21   
        #if !defined(MTOUCH_MODE1_SENSOR1)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 1, but MTOUCH_MODE1_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 22 
        #if !defined(MTOUCH_MODE1_SENSOR2)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 2, but MTOUCH_MODE1_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 23   
        #if !defined(MTOUCH_MODE1_SENSOR3)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 3, but MTOUCH_MODE1_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 24   
        #if !defined(MTOUCH_MODE1_SENSOR4)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 4, but MTOUCH_MODE1_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 25 
        #if !defined(MTOUCH_MODE1_SENSOR5)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 5, but MTOUCH_MODE1_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 26  
        #if !defined(MTOUCH_MODE1_SENSOR6)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 6, but MTOUCH_MODE1_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 27   
        #if !defined(MTOUCH_MODE1_SENSOR7)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 7, but MTOUCH_MODE1_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 28   
        #if !defined(MTOUCH_MODE1_SENSOR8)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 8, but MTOUCH_MODE1_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS > 29   
        #if !defined(MTOUCH_MODE1_SENSOR9)        
        #error MTOUCH_MODE1_NUM_SENSORS is greater than 9, but MTOUCH_MODE1_SENSOR9 is not defined.     
        #endif    
    #endif
    
    
    #define MTOUCH_MODE1_ORDER0_INIT              MTOUCH_MODE_SENSOR(1, 0)
    #define MTOUCH_MODE1_ORDER1_INIT            , MTOUCH_MODE_SENSOR(1, 1)
    #define MTOUCH_MODE1_ORDER2_INIT            , MTOUCH_MODE_SENSOR(1, 2)
    #define MTOUCH_MODE1_ORDER3_INIT            , MTOUCH_MODE_SENSOR(1, 3)
    #define MTOUCH_MODE1_ORDER4_INIT            , MTOUCH_MODE_SENSOR(1, 4)
    #define MTOUCH_MODE1_ORDER5_INIT            , MTOUCH_MODE_SENSOR(1, 5)
    #define MTOUCH_MODE1_ORDER6_INIT            , MTOUCH_MODE_SENSOR(1, 6)
    #define MTOUCH_MODE1_ORDER7_INIT            , MTOUCH_MODE_SENSOR(1, 7)
    #define MTOUCH_MODE1_ORDER8_INIT            , MTOUCH_MODE_SENSOR(1, 8)
    #define MTOUCH_MODE1_ORDER9_INIT            , MTOUCH_MODE_SENSOR(1, 9)
    #define MTOUCH_MODE1_ORDER10_INIT           , MTOUCH_MODE_SENSOR(1, 10)
    #define MTOUCH_MODE1_ORDER11_INIT           , MTOUCH_MODE_SENSOR(1, 11)
    #define MTOUCH_MODE1_ORDER12_INIT           , MTOUCH_MODE_SENSOR(1, 12)
    #define MTOUCH_MODE1_ORDER13_INIT           , MTOUCH_MODE_SENSOR(1, 13)
    #define MTOUCH_MODE1_ORDER14_INIT           , MTOUCH_MODE_SENSOR(1, 14)
    #define MTOUCH_MODE1_ORDER15_INIT           , MTOUCH_MODE_SENSOR(1, 15)
    #define MTOUCH_MODE1_ORDER16_INIT           , MTOUCH_MODE_SENSOR(1, 16)
    #define MTOUCH_MODE1_ORDER17_INIT           , MTOUCH_MODE_SENSOR(1, 17)
    #define MTOUCH_MODE1_ORDER18_INIT           , MTOUCH_MODE_SENSOR(1, 18)
    #define MTOUCH_MODE1_ORDER19_INIT           , MTOUCH_MODE_SENSOR(1, 19)
    #define MTOUCH_MODE1_ORDER20_INIT           , MTOUCH_MODE_SENSOR(1, 20)
    #define MTOUCH_MODE1_ORDER21_INIT           , MTOUCH_MODE_SENSOR(1, 21)
    #define MTOUCH_MODE1_ORDER22_INIT           , MTOUCH_MODE_SENSOR(1, 22)
    #define MTOUCH_MODE1_ORDER23_INIT           , MTOUCH_MODE_SENSOR(1, 23)
    #define MTOUCH_MODE1_ORDER24_INIT           , MTOUCH_MODE_SENSOR(1, 24)
    #define MTOUCH_MODE1_ORDER25_INIT           , MTOUCH_MODE_SENSOR(1, 25)
    #define MTOUCH_MODE1_ORDER26_INIT           , MTOUCH_MODE_SENSOR(1, 26)
    #define MTOUCH_MODE1_ORDER27_INIT           , MTOUCH_MODE_SENSOR(1, 27)
    #define MTOUCH_MODE1_ORDER28_INIT           , MTOUCH_MODE_SENSOR(1, 28)
    #define MTOUCH_MODE1_ORDER29_INIT           , MTOUCH_MODE_SENSOR(1, 29)
   
    #if MTOUCH_MODE1_NUM_SENSORS < 30
        #undef  MTOUCH_MODE1_ORDER29_INIT
        #define MTOUCH_MODE1_ORDER29_INIT
    #endif    
    #if MTOUCH_MODE1_NUM_SENSORS < 29
        #undef  MTOUCH_MODE1_ORDER28_INIT
        #define MTOUCH_MODE1_ORDER28_INIT
    #endif    
    #if MTOUCH_MODE1_NUM_SENSORS < 28
        #undef  MTOUCH_MODE1_ORDER27_INIT
        #define MTOUCH_MODE1_ORDER27_INIT
    #endif    
    #if MTOUCH_MODE1_NUM_SENSORS < 27
        #undef  MTOUCH_MODE1_ORDER26_INIT
        #define MTOUCH_MODE1_ORDER26_INIT
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 26        
        #undef  MTOUCH_MODE1_ORDER25_INIT          
        #define MTOUCH_MODE1_ORDER25_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 25        
        #undef  MTOUCH_MODE1_ORDER24_INIT          
        #define MTOUCH_MODE1_ORDER24_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 24        
        #undef  MTOUCH_MODE1_ORDER23_INIT          
        #define MTOUCH_MODE1_ORDER23_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 23        
        #undef  MTOUCH_MODE1_ORDER22_INIT          
        #define MTOUCH_MODE1_ORDER22_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 22        
        #undef  MTOUCH_MODE1_ORDER21_INIT          
        #define MTOUCH_MODE1_ORDER21_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 21        
        #undef  MTOUCH_MODE1_ORDER20_INIT          
        #define MTOUCH_MODE1_ORDER20_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 20        
        #undef  MTOUCH_MODE1_ORDER19_INIT          
        #define MTOUCH_MODE1_ORDER19_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 19        
        #undef  MTOUCH_MODE1_ORDER18_INIT          
        #define MTOUCH_MODE1_ORDER18_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 18        
        #undef  MTOUCH_MODE1_ORDER17_INIT          
        #define MTOUCH_MODE1_ORDER17_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 17        
        #undef  MTOUCH_MODE1_ORDER16_INIT          
        #define MTOUCH_MODE1_ORDER16_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 16        
        #undef  MTOUCH_MODE1_ORDER15_INIT          
        #define MTOUCH_MODE1_ORDER15_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 15        
        #undef  MTOUCH_MODE1_ORDER14_INIT          
        #define MTOUCH_MODE1_ORDER14_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 14        
        #undef  MTOUCH_MODE1_ORDER13_INIT          
        #define MTOUCH_MODE1_ORDER13_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 13        
        #undef  MTOUCH_MODE1_ORDER12_INIT          
        #define MTOUCH_MODE1_ORDER12_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 12        
        #undef  MTOUCH_MODE1_ORDER11_INIT          
        #define MTOUCH_MODE1_ORDER11_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 11        
        #undef  MTOUCH_MODE1_ORDER10_INIT          
        #define MTOUCH_MODE1_ORDER10_INIT       
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS < 10        
        #undef  MTOUCH_MODE1_ORDER9_INIT           
        #define MTOUCH_MODE1_ORDER9_INIT        
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS <  9        
        #undef  MTOUCH_MODE1_ORDER8_INIT           
        #define MTOUCH_MODE1_ORDER8_INIT        
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS <  8        
        #undef  MTOUCH_MODE1_ORDER7_INIT           
        #define MTOUCH_MODE1_ORDER7_INIT        
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS <  7        
        #undef  MTOUCH_MODE1_ORDER6_INIT           
        #define MTOUCH_MODE1_ORDER6_INIT        
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS <  6        
        #undef  MTOUCH_MODE1_ORDER5_INIT           
        #define MTOUCH_MODE1_ORDER5_INIT        
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS <  5        
        #undef  MTOUCH_MODE1_ORDER4_INIT           
        #define MTOUCH_MODE1_ORDER4_INIT        
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS <  4        
        #undef  MTOUCH_MODE1_ORDER3_INIT           
        #define MTOUCH_MODE1_ORDER3_INIT        
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS <  3        
        #undef  MTOUCH_MODE1_ORDER2_INIT           
        #define MTOUCH_MODE1_ORDER2_INIT        
    #endif
    #if MTOUCH_MODE1_NUM_SENSORS <  2        
        #undef  MTOUCH_MODE1_ORDER1_INIT           
        #define MTOUCH_MODE1_ORDER1_INIT        
    #endif
   
    #if MTOUCH_MODE1_NUM_SENSORS > 0
    #define MTOUCH_MODE1_VAR_INIT               {                             \
                                                    MTOUCH_MODE1_ORDER0_INIT  \
                                                    MTOUCH_MODE1_ORDER1_INIT  \
                                                    MTOUCH_MODE1_ORDER2_INIT  \
                                                    MTOUCH_MODE1_ORDER3_INIT  \
                                                    MTOUCH_MODE1_ORDER4_INIT  \
                                                    MTOUCH_MODE1_ORDER5_INIT  \
                                                    MTOUCH_MODE1_ORDER6_INIT  \
                                                    MTOUCH_MODE1_ORDER7_INIT  \
                                                    MTOUCH_MODE1_ORDER8_INIT  \
                                                    MTOUCH_MODE1_ORDER9_INIT  \
                                                    MTOUCH_MODE1_ORDER10_INIT \
                                                    MTOUCH_MODE1_ORDER11_INIT \
                                                    MTOUCH_MODE1_ORDER12_INIT \
                                                    MTOUCH_MODE1_ORDER13_INIT \
                                                    MTOUCH_MODE1_ORDER14_INIT \
                                                    MTOUCH_MODE1_ORDER15_INIT \
                                                    MTOUCH_MODE1_ORDER16_INIT \
                                                    MTOUCH_MODE1_ORDER17_INIT \
                                                    MTOUCH_MODE1_ORDER18_INIT \
                                                    MTOUCH_MODE1_ORDER19_INIT \
                                                    MTOUCH_MODE1_ORDER20_INIT \
                                                    MTOUCH_MODE1_ORDER21_INIT \
                                                    MTOUCH_MODE1_ORDER22_INIT \
                                                    MTOUCH_MODE1_ORDER23_INIT \
                                                    MTOUCH_MODE1_ORDER24_INIT \
                                                    MTOUCH_MODE1_ORDER25_INIT \
                                                    MTOUCH_MODE1_ORDER26_INIT \
                                                    MTOUCH_MODE1_ORDER27_INIT \
                                                    MTOUCH_MODE1_ORDER28_INIT \
                                                    MTOUCH_MODE1_ORDER29_INIT \
                                                    MTOUCH_MODE_SENSOR_END(0) \
                                                }    
    #endif

    //
    //
    //
    //
    //      MODE 2
    //
    //
    //
    //
    //
    #if MTOUCH_NUM_MODES > 2
    #if MTOUCH_MODE2_NUM_SENSORS > 0
        #if !defined(MTOUCH_MODE2_SENSOR0)
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 0, but MTOUCH_MODE2_SENSOR0 is not defined.
        #endif
    #else
        #error MTOUCH_MODE2_NUM_SENSORS must be greater than 0.
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS >  1  
        #if !defined(MTOUCH_MODE2_SENSOR1)       
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 1, but MTOUCH_MODE2_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS >  2 
        #if !defined(MTOUCH_MODE2_SENSOR2)       
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 2, but MTOUCH_MODE2_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS >  3  
        #if !defined(MTOUCH_MODE2_SENSOR3)       
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 3, but MTOUCH_MODE2_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS >  4  
        #if !defined(MTOUCH_MODE2_SENSOR4)       
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 4, but MTOUCH_MODE2_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS >  5 
        #if !defined(MTOUCH_MODE2_SENSOR5)       
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 5, but MTOUCH_MODE2_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS >  6  
        #if !defined(MTOUCH_MODE2_SENSOR6)       
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 6, but MTOUCH_MODE2_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS >  7 
        #if !defined(MTOUCH_MODE2_SENSOR7)       
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 7, but MTOUCH_MODE2_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS >  8  
        #if !defined(MTOUCH_MODE2_SENSOR8)       
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 8, but MTOUCH_MODE2_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS >  9  
        #if !defined(MTOUCH_MODE2_SENSOR9)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 9, but MTOUCH_MODE2_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 10  
        #if !defined(MTOUCH_MODE2_SENSOR0)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 0, but MTOUCH_MODE2_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 11  
        #if !defined(MTOUCH_MODE2_SENSOR1)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 1, but MTOUCH_MODE2_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 12  
        #if !defined(MTOUCH_MODE2_SENSOR2)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 2, but MTOUCH_MODE2_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 13  
        #if !defined(MTOUCH_MODE2_SENSOR3)       
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 3, but MTOUCH_MODE2_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 14  
        #if !defined(MTOUCH_MODE2_SENSOR4)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 4, but MTOUCH_MODE2_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 15  
        #if !defined(MTOUCH_MODE2_SENSOR5)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 5, but MTOUCH_MODE2_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 16 
        #if !defined(MTOUCH_MODE2_SENSOR6)
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 6, but MTOUCH_MODE2_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 17
        #if !defined(MTOUCH_MODE2_SENSOR7)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 7, but MTOUCH_MODE2_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 18 
        #if !defined(MTOUCH_MODE2_SENSOR8)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 8, but MTOUCH_MODE2_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 19 
        #if !defined(MTOUCH_MODE2_SENSOR9)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 9, but MTOUCH_MODE2_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 20 
        #if !defined(MTOUCH_MODE2_SENSOR0)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 0, but MTOUCH_MODE2_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 21   
        #if !defined(MTOUCH_MODE2_SENSOR1)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 1, but MTOUCH_MODE2_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 22 
        #if !defined(MTOUCH_MODE2_SENSOR2)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 2, but MTOUCH_MODE2_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 23   
        #if !defined(MTOUCH_MODE2_SENSOR3)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 3, but MTOUCH_MODE2_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 24   
        #if !defined(MTOUCH_MODE2_SENSOR4)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 4, but MTOUCH_MODE2_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 25 
        #if !defined(MTOUCH_MODE2_SENSOR5)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 5, but MTOUCH_MODE2_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 26  
        #if !defined(MTOUCH_MODE2_SENSOR6)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 6, but MTOUCH_MODE2_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 27   
        #if !defined(MTOUCH_MODE2_SENSOR7)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 7, but MTOUCH_MODE2_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 28   
        #if !defined(MTOUCH_MODE2_SENSOR8)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 8, but MTOUCH_MODE2_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS > 29   
        #if !defined(MTOUCH_MODE2_SENSOR9)        
        #error MTOUCH_MODE2_NUM_SENSORS is greater than 9, but MTOUCH_MODE2_SENSOR9 is not defined.     
        #endif    
    #endif
    
    
    #define MTOUCH_MODE2_ORDER0_INIT              MTOUCH_MODE_SENSOR(2, 0)
    #define MTOUCH_MODE2_ORDER1_INIT            , MTOUCH_MODE_SENSOR(2, 1)
    #define MTOUCH_MODE2_ORDER2_INIT            , MTOUCH_MODE_SENSOR(2, 2)
    #define MTOUCH_MODE2_ORDER3_INIT            , MTOUCH_MODE_SENSOR(2, 3)
    #define MTOUCH_MODE2_ORDER4_INIT            , MTOUCH_MODE_SENSOR(2, 4)
    #define MTOUCH_MODE2_ORDER5_INIT            , MTOUCH_MODE_SENSOR(2, 5)
    #define MTOUCH_MODE2_ORDER6_INIT            , MTOUCH_MODE_SENSOR(2, 6)
    #define MTOUCH_MODE2_ORDER7_INIT            , MTOUCH_MODE_SENSOR(2, 7)
    #define MTOUCH_MODE2_ORDER8_INIT            , MTOUCH_MODE_SENSOR(2, 8)
    #define MTOUCH_MODE2_ORDER9_INIT            , MTOUCH_MODE_SENSOR(2, 9)
    #define MTOUCH_MODE2_ORDER10_INIT           , MTOUCH_MODE_SENSOR(2, 10)
    #define MTOUCH_MODE2_ORDER11_INIT           , MTOUCH_MODE_SENSOR(2, 11)
    #define MTOUCH_MODE2_ORDER12_INIT           , MTOUCH_MODE_SENSOR(2, 12)
    #define MTOUCH_MODE2_ORDER13_INIT           , MTOUCH_MODE_SENSOR(2, 13)
    #define MTOUCH_MODE2_ORDER14_INIT           , MTOUCH_MODE_SENSOR(2, 14)
    #define MTOUCH_MODE2_ORDER15_INIT           , MTOUCH_MODE_SENSOR(2, 15)
    #define MTOUCH_MODE2_ORDER16_INIT           , MTOUCH_MODE_SENSOR(2, 16)
    #define MTOUCH_MODE2_ORDER17_INIT           , MTOUCH_MODE_SENSOR(2, 17)
    #define MTOUCH_MODE2_ORDER18_INIT           , MTOUCH_MODE_SENSOR(2, 18)
    #define MTOUCH_MODE2_ORDER19_INIT           , MTOUCH_MODE_SENSOR(2, 19)
    #define MTOUCH_MODE2_ORDER20_INIT           , MTOUCH_MODE_SENSOR(2, 20)
    #define MTOUCH_MODE2_ORDER21_INIT           , MTOUCH_MODE_SENSOR(2, 21)
    #define MTOUCH_MODE2_ORDER22_INIT           , MTOUCH_MODE_SENSOR(2, 22)
    #define MTOUCH_MODE2_ORDER23_INIT           , MTOUCH_MODE_SENSOR(2, 23)
    #define MTOUCH_MODE2_ORDER24_INIT           , MTOUCH_MODE_SENSOR(2, 24)
    #define MTOUCH_MODE2_ORDER25_INIT           , MTOUCH_MODE_SENSOR(2, 25)
    #define MTOUCH_MODE2_ORDER26_INIT           , MTOUCH_MODE_SENSOR(2, 26)
    #define MTOUCH_MODE2_ORDER27_INIT           , MTOUCH_MODE_SENSOR(2, 27)
    #define MTOUCH_MODE2_ORDER28_INIT           , MTOUCH_MODE_SENSOR(2, 28)
    #define MTOUCH_MODE2_ORDER29_INIT           , MTOUCH_MODE_SENSOR(2, 29)
   
    #if MTOUCH_MODE2_NUM_SENSORS < 30
        #undef  MTOUCH_MODE2_ORDER29_INIT
        #define MTOUCH_MODE2_ORDER29_INIT
    #endif    
    #if MTOUCH_MODE2_NUM_SENSORS < 29
        #undef  MTOUCH_MODE2_ORDER28_INIT
        #define MTOUCH_MODE2_ORDER28_INIT
    #endif    
    #if MTOUCH_MODE2_NUM_SENSORS < 28
        #undef  MTOUCH_MODE2_ORDER27_INIT
        #define MTOUCH_MODE2_ORDER27_INIT
    #endif    
    #if MTOUCH_MODE2_NUM_SENSORS < 27
        #undef  MTOUCH_MODE2_ORDER26_INIT
        #define MTOUCH_MODE2_ORDER26_INIT
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 26        
        #undef  MTOUCH_MODE2_ORDER25_INIT          
        #define MTOUCH_MODE2_ORDER25_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 25        
        #undef  MTOUCH_MODE2_ORDER24_INIT          
        #define MTOUCH_MODE2_ORDER24_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 24        
        #undef  MTOUCH_MODE2_ORDER23_INIT          
        #define MTOUCH_MODE2_ORDER23_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 23        
        #undef  MTOUCH_MODE2_ORDER22_INIT          
        #define MTOUCH_MODE2_ORDER22_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 22        
        #undef  MTOUCH_MODE2_ORDER21_INIT          
        #define MTOUCH_MODE2_ORDER21_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 21        
        #undef  MTOUCH_MODE2_ORDER20_INIT          
        #define MTOUCH_MODE2_ORDER20_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 20        
        #undef  MTOUCH_MODE2_ORDER19_INIT          
        #define MTOUCH_MODE2_ORDER19_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 19        
        #undef  MTOUCH_MODE2_ORDER18_INIT          
        #define MTOUCH_MODE2_ORDER18_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 18        
        #undef  MTOUCH_MODE2_ORDER17_INIT          
        #define MTOUCH_MODE2_ORDER17_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 17        
        #undef  MTOUCH_MODE2_ORDER16_INIT          
        #define MTOUCH_MODE2_ORDER16_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 16        
        #undef  MTOUCH_MODE2_ORDER15_INIT          
        #define MTOUCH_MODE2_ORDER15_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 15        
        #undef  MTOUCH_MODE2_ORDER14_INIT          
        #define MTOUCH_MODE2_ORDER14_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 14        
        #undef  MTOUCH_MODE2_ORDER13_INIT          
        #define MTOUCH_MODE2_ORDER13_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 13        
        #undef  MTOUCH_MODE2_ORDER12_INIT          
        #define MTOUCH_MODE2_ORDER12_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 12        
        #undef  MTOUCH_MODE2_ORDER11_INIT          
        #define MTOUCH_MODE2_ORDER11_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 11        
        #undef  MTOUCH_MODE2_ORDER10_INIT          
        #define MTOUCH_MODE2_ORDER10_INIT       
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS < 10        
        #undef  MTOUCH_MODE2_ORDER9_INIT           
        #define MTOUCH_MODE2_ORDER9_INIT        
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS <  9        
        #undef  MTOUCH_MODE2_ORDER8_INIT           
        #define MTOUCH_MODE2_ORDER8_INIT        
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS <  8        
        #undef  MTOUCH_MODE2_ORDER7_INIT           
        #define MTOUCH_MODE2_ORDER7_INIT        
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS <  7        
        #undef  MTOUCH_MODE2_ORDER6_INIT           
        #define MTOUCH_MODE2_ORDER6_INIT        
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS <  6        
        #undef  MTOUCH_MODE2_ORDER5_INIT           
        #define MTOUCH_MODE2_ORDER5_INIT        
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS <  5        
        #undef  MTOUCH_MODE2_ORDER4_INIT           
        #define MTOUCH_MODE2_ORDER4_INIT        
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS <  4        
        #undef  MTOUCH_MODE2_ORDER3_INIT           
        #define MTOUCH_MODE2_ORDER3_INIT        
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS <  3        
        #undef  MTOUCH_MODE2_ORDER2_INIT           
        #define MTOUCH_MODE2_ORDER2_INIT        
    #endif
    #if MTOUCH_MODE2_NUM_SENSORS <  2        
        #undef  MTOUCH_MODE2_ORDER1_INIT           
        #define MTOUCH_MODE2_ORDER1_INIT        
    #endif
   
    #if MTOUCH_MODE2_NUM_SENSORS > 0
    #define MTOUCH_MODE2_VAR_INIT               {                             \
                                                    MTOUCH_MODE2_ORDER0_INIT  \
                                                    MTOUCH_MODE2_ORDER1_INIT  \
                                                    MTOUCH_MODE2_ORDER2_INIT  \
                                                    MTOUCH_MODE2_ORDER3_INIT  \
                                                    MTOUCH_MODE2_ORDER4_INIT  \
                                                    MTOUCH_MODE2_ORDER5_INIT  \
                                                    MTOUCH_MODE2_ORDER6_INIT  \
                                                    MTOUCH_MODE2_ORDER7_INIT  \
                                                    MTOUCH_MODE2_ORDER8_INIT  \
                                                    MTOUCH_MODE2_ORDER9_INIT  \
                                                    MTOUCH_MODE2_ORDER10_INIT \
                                                    MTOUCH_MODE2_ORDER11_INIT \
                                                    MTOUCH_MODE2_ORDER12_INIT \
                                                    MTOUCH_MODE2_ORDER13_INIT \
                                                    MTOUCH_MODE2_ORDER14_INIT \
                                                    MTOUCH_MODE2_ORDER15_INIT \
                                                    MTOUCH_MODE2_ORDER16_INIT \
                                                    MTOUCH_MODE2_ORDER17_INIT \
                                                    MTOUCH_MODE2_ORDER18_INIT \
                                                    MTOUCH_MODE2_ORDER19_INIT \
                                                    MTOUCH_MODE2_ORDER20_INIT \
                                                    MTOUCH_MODE2_ORDER21_INIT \
                                                    MTOUCH_MODE2_ORDER22_INIT \
                                                    MTOUCH_MODE2_ORDER23_INIT \
                                                    MTOUCH_MODE2_ORDER24_INIT \
                                                    MTOUCH_MODE2_ORDER25_INIT \
                                                    MTOUCH_MODE2_ORDER26_INIT \
                                                    MTOUCH_MODE2_ORDER27_INIT \
                                                    MTOUCH_MODE2_ORDER28_INIT \
                                                    MTOUCH_MODE2_ORDER29_INIT \
                                                    MTOUCH_MODE_SENSOR_END(0) \
                                                }    
    #endif
    #endif
    
    //
    //
    //
    //
    //      MODE 3
    //
    //
    //
    //
    //
    #if MTOUCH_NUM_MODES > 3
    #if MTOUCH_MODE3_NUM_SENSORS > 0
        #if !defined(MTOUCH_MODE3_SENSOR0)
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 0, but MTOUCH_MODE3_SENSOR0 is not defined.
        #endif
    #else
        #error MTOUCH_MODE3_NUM_SENSORS must be greater than 0.
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS >  1  
        #if !defined(MTOUCH_MODE3_SENSOR1)       
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 1, but MTOUCH_MODE3_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS >  2 
        #if !defined(MTOUCH_MODE3_SENSOR2)       
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 2, but MTOUCH_MODE3_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS >  3  
        #if !defined(MTOUCH_MODE3_SENSOR3)       
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 3, but MTOUCH_MODE3_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS >  4  
        #if !defined(MTOUCH_MODE3_SENSOR4)       
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 4, but MTOUCH_MODE3_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS >  5 
        #if !defined(MTOUCH_MODE3_SENSOR5)       
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 5, but MTOUCH_MODE3_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS >  6  
        #if !defined(MTOUCH_MODE3_SENSOR6)       
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 6, but MTOUCH_MODE3_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS >  7 
        #if !defined(MTOUCH_MODE3_SENSOR7)       
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 7, but MTOUCH_MODE3_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS >  8  
        #if !defined(MTOUCH_MODE3_SENSOR8)       
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 8, but MTOUCH_MODE3_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS >  9  
        #if !defined(MTOUCH_MODE3_SENSOR9)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 9, but MTOUCH_MODE3_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 10  
        #if !defined(MTOUCH_MODE3_SENSOR0)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 0, but MTOUCH_MODE3_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 11  
        #if !defined(MTOUCH_MODE3_SENSOR1)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 1, but MTOUCH_MODE3_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 12  
        #if !defined(MTOUCH_MODE3_SENSOR2)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 2, but MTOUCH_MODE3_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 13  
        #if !defined(MTOUCH_MODE3_SENSOR3)       
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 3, but MTOUCH_MODE3_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 14  
        #if !defined(MTOUCH_MODE3_SENSOR4)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 4, but MTOUCH_MODE3_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 15  
        #if !defined(MTOUCH_MODE3_SENSOR5)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 5, but MTOUCH_MODE3_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 16 
        #if !defined(MTOUCH_MODE3_SENSOR6)
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 6, but MTOUCH_MODE3_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 17
        #if !defined(MTOUCH_MODE3_SENSOR7)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 7, but MTOUCH_MODE3_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 18 
        #if !defined(MTOUCH_MODE3_SENSOR8)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 8, but MTOUCH_MODE3_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 19 
        #if !defined(MTOUCH_MODE3_SENSOR9)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 9, but MTOUCH_MODE3_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 20 
        #if !defined(MTOUCH_MODE3_SENSOR0)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 0, but MTOUCH_MODE3_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 21   
        #if !defined(MTOUCH_MODE3_SENSOR1)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 1, but MTOUCH_MODE3_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 22 
        #if !defined(MTOUCH_MODE3_SENSOR2)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 2, but MTOUCH_MODE3_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 23   
        #if !defined(MTOUCH_MODE3_SENSOR3)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 3, but MTOUCH_MODE3_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 24   
        #if !defined(MTOUCH_MODE3_SENSOR4)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 4, but MTOUCH_MODE3_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 25 
        #if !defined(MTOUCH_MODE3_SENSOR5)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 5, but MTOUCH_MODE3_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 26  
        #if !defined(MTOUCH_MODE3_SENSOR6)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 6, but MTOUCH_MODE3_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 27   
        #if !defined(MTOUCH_MODE3_SENSOR7)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 7, but MTOUCH_MODE3_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 28   
        #if !defined(MTOUCH_MODE3_SENSOR8)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 8, but MTOUCH_MODE3_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS > 29   
        #if !defined(MTOUCH_MODE3_SENSOR9)        
        #error MTOUCH_MODE3_NUM_SENSORS is greater than 9, but MTOUCH_MODE3_SENSOR9 is not defined.     
        #endif    
    #endif
    
    
    #define MTOUCH_MODE3_ORDER0_INIT              MTOUCH_MODE_SENSOR(3, 0)
    #define MTOUCH_MODE3_ORDER1_INIT            , MTOUCH_MODE_SENSOR(3, 1)
    #define MTOUCH_MODE3_ORDER2_INIT            , MTOUCH_MODE_SENSOR(3, 2)
    #define MTOUCH_MODE3_ORDER3_INIT            , MTOUCH_MODE_SENSOR(3, 3)
    #define MTOUCH_MODE3_ORDER4_INIT            , MTOUCH_MODE_SENSOR(3, 4)
    #define MTOUCH_MODE3_ORDER5_INIT            , MTOUCH_MODE_SENSOR(3, 5)
    #define MTOUCH_MODE3_ORDER6_INIT            , MTOUCH_MODE_SENSOR(3, 6)
    #define MTOUCH_MODE3_ORDER7_INIT            , MTOUCH_MODE_SENSOR(3, 7)
    #define MTOUCH_MODE3_ORDER8_INIT            , MTOUCH_MODE_SENSOR(3, 8)
    #define MTOUCH_MODE3_ORDER9_INIT            , MTOUCH_MODE_SENSOR(3, 9)
    #define MTOUCH_MODE3_ORDER10_INIT           , MTOUCH_MODE_SENSOR(3, 10)
    #define MTOUCH_MODE3_ORDER11_INIT           , MTOUCH_MODE_SENSOR(3, 11)
    #define MTOUCH_MODE3_ORDER12_INIT           , MTOUCH_MODE_SENSOR(3, 12)
    #define MTOUCH_MODE3_ORDER13_INIT           , MTOUCH_MODE_SENSOR(3, 13)
    #define MTOUCH_MODE3_ORDER14_INIT           , MTOUCH_MODE_SENSOR(3, 14)
    #define MTOUCH_MODE3_ORDER15_INIT           , MTOUCH_MODE_SENSOR(3, 15)
    #define MTOUCH_MODE3_ORDER16_INIT           , MTOUCH_MODE_SENSOR(3, 16)
    #define MTOUCH_MODE3_ORDER17_INIT           , MTOUCH_MODE_SENSOR(3, 17)
    #define MTOUCH_MODE3_ORDER18_INIT           , MTOUCH_MODE_SENSOR(3, 18)
    #define MTOUCH_MODE3_ORDER19_INIT           , MTOUCH_MODE_SENSOR(3, 19)
    #define MTOUCH_MODE3_ORDER20_INIT           , MTOUCH_MODE_SENSOR(3, 20)
    #define MTOUCH_MODE3_ORDER21_INIT           , MTOUCH_MODE_SENSOR(3, 21)
    #define MTOUCH_MODE3_ORDER22_INIT           , MTOUCH_MODE_SENSOR(3, 22)
    #define MTOUCH_MODE3_ORDER23_INIT           , MTOUCH_MODE_SENSOR(3, 23)
    #define MTOUCH_MODE3_ORDER24_INIT           , MTOUCH_MODE_SENSOR(3, 24)
    #define MTOUCH_MODE3_ORDER25_INIT           , MTOUCH_MODE_SENSOR(3, 25)
    #define MTOUCH_MODE3_ORDER26_INIT           , MTOUCH_MODE_SENSOR(3, 26)
    #define MTOUCH_MODE3_ORDER27_INIT           , MTOUCH_MODE_SENSOR(3, 27)
    #define MTOUCH_MODE3_ORDER28_INIT           , MTOUCH_MODE_SENSOR(3, 28)
    #define MTOUCH_MODE3_ORDER29_INIT           , MTOUCH_MODE_SENSOR(3, 29)
   
    #if MTOUCH_MODE3_NUM_SENSORS < 30
        #undef  MTOUCH_MODE3_ORDER29_INIT
        #define MTOUCH_MODE3_ORDER29_INIT
    #endif    
    #if MTOUCH_MODE3_NUM_SENSORS < 29
        #undef  MTOUCH_MODE3_ORDER28_INIT
        #define MTOUCH_MODE3_ORDER28_INIT
    #endif    
    #if MTOUCH_MODE3_NUM_SENSORS < 28
        #undef  MTOUCH_MODE3_ORDER27_INIT
        #define MTOUCH_MODE3_ORDER27_INIT
    #endif    
    #if MTOUCH_MODE3_NUM_SENSORS < 27
        #undef  MTOUCH_MODE3_ORDER26_INIT
        #define MTOUCH_MODE3_ORDER26_INIT
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 26        
        #undef  MTOUCH_MODE3_ORDER25_INIT          
        #define MTOUCH_MODE3_ORDER25_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 25        
        #undef  MTOUCH_MODE3_ORDER24_INIT          
        #define MTOUCH_MODE3_ORDER24_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 24        
        #undef  MTOUCH_MODE3_ORDER23_INIT          
        #define MTOUCH_MODE3_ORDER23_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 23        
        #undef  MTOUCH_MODE3_ORDER22_INIT          
        #define MTOUCH_MODE3_ORDER22_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 22        
        #undef  MTOUCH_MODE3_ORDER21_INIT          
        #define MTOUCH_MODE3_ORDER21_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 21        
        #undef  MTOUCH_MODE3_ORDER20_INIT          
        #define MTOUCH_MODE3_ORDER20_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 20        
        #undef  MTOUCH_MODE3_ORDER19_INIT          
        #define MTOUCH_MODE3_ORDER19_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 19        
        #undef  MTOUCH_MODE3_ORDER18_INIT          
        #define MTOUCH_MODE3_ORDER18_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 18        
        #undef  MTOUCH_MODE3_ORDER17_INIT          
        #define MTOUCH_MODE3_ORDER17_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 17        
        #undef  MTOUCH_MODE3_ORDER16_INIT          
        #define MTOUCH_MODE3_ORDER16_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 16        
        #undef  MTOUCH_MODE3_ORDER15_INIT          
        #define MTOUCH_MODE3_ORDER15_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 15        
        #undef  MTOUCH_MODE3_ORDER14_INIT          
        #define MTOUCH_MODE3_ORDER14_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 14        
        #undef  MTOUCH_MODE3_ORDER13_INIT          
        #define MTOUCH_MODE3_ORDER13_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 13        
        #undef  MTOUCH_MODE3_ORDER12_INIT          
        #define MTOUCH_MODE3_ORDER12_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 12        
        #undef  MTOUCH_MODE3_ORDER11_INIT          
        #define MTOUCH_MODE3_ORDER11_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 11        
        #undef  MTOUCH_MODE3_ORDER10_INIT          
        #define MTOUCH_MODE3_ORDER10_INIT       
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS < 10        
        #undef  MTOUCH_MODE3_ORDER9_INIT           
        #define MTOUCH_MODE3_ORDER9_INIT        
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS <  9        
        #undef  MTOUCH_MODE3_ORDER8_INIT           
        #define MTOUCH_MODE3_ORDER8_INIT        
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS <  8        
        #undef  MTOUCH_MODE3_ORDER7_INIT           
        #define MTOUCH_MODE3_ORDER7_INIT        
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS <  7        
        #undef  MTOUCH_MODE3_ORDER6_INIT           
        #define MTOUCH_MODE3_ORDER6_INIT        
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS <  6        
        #undef  MTOUCH_MODE3_ORDER5_INIT           
        #define MTOUCH_MODE3_ORDER5_INIT        
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS <  5        
        #undef  MTOUCH_MODE3_ORDER4_INIT           
        #define MTOUCH_MODE3_ORDER4_INIT        
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS <  4        
        #undef  MTOUCH_MODE3_ORDER3_INIT           
        #define MTOUCH_MODE3_ORDER3_INIT        
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS <  3        
        #undef  MTOUCH_MODE3_ORDER2_INIT           
        #define MTOUCH_MODE3_ORDER2_INIT        
    #endif
    #if MTOUCH_MODE3_NUM_SENSORS <  2        
        #undef  MTOUCH_MODE3_ORDER1_INIT           
        #define MTOUCH_MODE3_ORDER1_INIT        
    #endif
   
    #if MTOUCH_MODE3_NUM_SENSORS > 0
    #define MTOUCH_MODE3_VAR_INIT               {                             \
                                                    MTOUCH_MODE3_ORDER0_INIT  \
                                                    MTOUCH_MODE3_ORDER1_INIT  \
                                                    MTOUCH_MODE3_ORDER2_INIT  \
                                                    MTOUCH_MODE3_ORDER3_INIT  \
                                                    MTOUCH_MODE3_ORDER4_INIT  \
                                                    MTOUCH_MODE3_ORDER5_INIT  \
                                                    MTOUCH_MODE3_ORDER6_INIT  \
                                                    MTOUCH_MODE3_ORDER7_INIT  \
                                                    MTOUCH_MODE3_ORDER8_INIT  \
                                                    MTOUCH_MODE3_ORDER9_INIT  \
                                                    MTOUCH_MODE3_ORDER10_INIT \
                                                    MTOUCH_MODE3_ORDER11_INIT \
                                                    MTOUCH_MODE3_ORDER12_INIT \
                                                    MTOUCH_MODE3_ORDER13_INIT \
                                                    MTOUCH_MODE3_ORDER14_INIT \
                                                    MTOUCH_MODE3_ORDER15_INIT \
                                                    MTOUCH_MODE3_ORDER16_INIT \
                                                    MTOUCH_MODE3_ORDER17_INIT \
                                                    MTOUCH_MODE3_ORDER18_INIT \
                                                    MTOUCH_MODE3_ORDER19_INIT \
                                                    MTOUCH_MODE3_ORDER20_INIT \
                                                    MTOUCH_MODE3_ORDER21_INIT \
                                                    MTOUCH_MODE3_ORDER22_INIT \
                                                    MTOUCH_MODE3_ORDER23_INIT \
                                                    MTOUCH_MODE3_ORDER24_INIT \
                                                    MTOUCH_MODE3_ORDER25_INIT \
                                                    MTOUCH_MODE3_ORDER26_INIT \
                                                    MTOUCH_MODE3_ORDER27_INIT \
                                                    MTOUCH_MODE3_ORDER28_INIT \
                                                    MTOUCH_MODE3_ORDER29_INIT \
                                                    MTOUCH_MODE_SENSOR_END(0) \
                                                }    
    #endif
    #endif
    
    //
    //
    //
    //
    //      MODE 4
    //
    //
    //
    //
    //
    #if MTOUCH_NUM_MODES > 4
    #if MTOUCH_MODE4_NUM_SENSORS > 0
        #if !defined(MTOUCH_MODE4_SENSOR0)
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 0, but MTOUCH_MODE4_SENSOR0 is not defined.
        #endif
    #else
        #error MTOUCH_MODE4_NUM_SENSORS must be greater than 0.
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS >  1  
        #if !defined(MTOUCH_MODE4_SENSOR1)       
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 1, but MTOUCH_MODE4_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS >  2 
        #if !defined(MTOUCH_MODE4_SENSOR2)       
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 2, but MTOUCH_MODE4_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS >  3  
        #if !defined(MTOUCH_MODE4_SENSOR3)       
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 3, but MTOUCH_MODE4_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS >  4  
        #if !defined(MTOUCH_MODE4_SENSOR4)       
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 4, but MTOUCH_MODE4_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS >  5 
        #if !defined(MTOUCH_MODE4_SENSOR5)       
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 5, but MTOUCH_MODE4_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS >  6  
        #if !defined(MTOUCH_MODE4_SENSOR6)       
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 6, but MTOUCH_MODE4_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS >  7 
        #if !defined(MTOUCH_MODE4_SENSOR7)       
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 7, but MTOUCH_MODE4_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS >  8  
        #if !defined(MTOUCH_MODE4_SENSOR8)       
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 8, but MTOUCH_MODE4_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS >  9  
        #if !defined(MTOUCH_MODE4_SENSOR9)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 9, but MTOUCH_MODE4_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 10  
        #if !defined(MTOUCH_MODE4_SENSOR0)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 0, but MTOUCH_MODE4_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 11  
        #if !defined(MTOUCH_MODE4_SENSOR1)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 1, but MTOUCH_MODE4_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 12  
        #if !defined(MTOUCH_MODE4_SENSOR2)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 2, but MTOUCH_MODE4_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 13  
        #if !defined(MTOUCH_MODE4_SENSOR3)       
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 3, but MTOUCH_MODE4_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 14  
        #if !defined(MTOUCH_MODE4_SENSOR4)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 4, but MTOUCH_MODE4_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 15  
        #if !defined(MTOUCH_MODE4_SENSOR5)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 5, but MTOUCH_MODE4_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 16 
        #if !defined(MTOUCH_MODE4_SENSOR6)
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 6, but MTOUCH_MODE4_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 17
        #if !defined(MTOUCH_MODE4_SENSOR7)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 7, but MTOUCH_MODE4_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 18 
        #if !defined(MTOUCH_MODE4_SENSOR8)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 8, but MTOUCH_MODE4_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 19 
        #if !defined(MTOUCH_MODE4_SENSOR9)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 9, but MTOUCH_MODE4_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 20 
        #if !defined(MTOUCH_MODE4_SENSOR0)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 0, but MTOUCH_MODE4_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 21   
        #if !defined(MTOUCH_MODE4_SENSOR1)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 1, but MTOUCH_MODE4_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 22 
        #if !defined(MTOUCH_MODE4_SENSOR2)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 2, but MTOUCH_MODE4_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 23   
        #if !defined(MTOUCH_MODE4_SENSOR3)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 3, but MTOUCH_MODE4_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 24   
        #if !defined(MTOUCH_MODE4_SENSOR4)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 4, but MTOUCH_MODE4_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 25 
        #if !defined(MTOUCH_MODE4_SENSOR5)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 5, but MTOUCH_MODE4_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 26  
        #if !defined(MTOUCH_MODE4_SENSOR6)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 6, but MTOUCH_MODE4_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 27   
        #if !defined(MTOUCH_MODE4_SENSOR7)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 7, but MTOUCH_MODE4_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 28   
        #if !defined(MTOUCH_MODE4_SENSOR8)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 8, but MTOUCH_MODE4_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS > 29   
        #if !defined(MTOUCH_MODE4_SENSOR9)        
        #error MTOUCH_MODE4_NUM_SENSORS is greater than 9, but MTOUCH_MODE4_SENSOR9 is not defined.     
        #endif    
    #endif
    
    
    #define MTOUCH_MODE4_ORDER0_INIT              MTOUCH_MODE_SENSOR(4, 0)
    #define MTOUCH_MODE4_ORDER1_INIT            , MTOUCH_MODE_SENSOR(4, 1)
    #define MTOUCH_MODE4_ORDER2_INIT            , MTOUCH_MODE_SENSOR(4, 2)
    #define MTOUCH_MODE4_ORDER3_INIT            , MTOUCH_MODE_SENSOR(4, 3)
    #define MTOUCH_MODE4_ORDER4_INIT            , MTOUCH_MODE_SENSOR(4, 4)
    #define MTOUCH_MODE4_ORDER5_INIT            , MTOUCH_MODE_SENSOR(4, 5)
    #define MTOUCH_MODE4_ORDER6_INIT            , MTOUCH_MODE_SENSOR(4, 6)
    #define MTOUCH_MODE4_ORDER7_INIT            , MTOUCH_MODE_SENSOR(4, 7)
    #define MTOUCH_MODE4_ORDER8_INIT            , MTOUCH_MODE_SENSOR(4, 8)
    #define MTOUCH_MODE4_ORDER9_INIT            , MTOUCH_MODE_SENSOR(4, 9)
    #define MTOUCH_MODE4_ORDER10_INIT           , MTOUCH_MODE_SENSOR(4, 10)
    #define MTOUCH_MODE4_ORDER11_INIT           , MTOUCH_MODE_SENSOR(4, 11)
    #define MTOUCH_MODE4_ORDER12_INIT           , MTOUCH_MODE_SENSOR(4, 12)
    #define MTOUCH_MODE4_ORDER13_INIT           , MTOUCH_MODE_SENSOR(4, 13)
    #define MTOUCH_MODE4_ORDER14_INIT           , MTOUCH_MODE_SENSOR(4, 14)
    #define MTOUCH_MODE4_ORDER15_INIT           , MTOUCH_MODE_SENSOR(4, 15)
    #define MTOUCH_MODE4_ORDER16_INIT           , MTOUCH_MODE_SENSOR(4, 16)
    #define MTOUCH_MODE4_ORDER17_INIT           , MTOUCH_MODE_SENSOR(4, 17)
    #define MTOUCH_MODE4_ORDER18_INIT           , MTOUCH_MODE_SENSOR(4, 18)
    #define MTOUCH_MODE4_ORDER19_INIT           , MTOUCH_MODE_SENSOR(4, 19)
    #define MTOUCH_MODE4_ORDER20_INIT           , MTOUCH_MODE_SENSOR(4, 20)
    #define MTOUCH_MODE4_ORDER21_INIT           , MTOUCH_MODE_SENSOR(4, 21)
    #define MTOUCH_MODE4_ORDER22_INIT           , MTOUCH_MODE_SENSOR(4, 22)
    #define MTOUCH_MODE4_ORDER23_INIT           , MTOUCH_MODE_SENSOR(4, 23)
    #define MTOUCH_MODE4_ORDER24_INIT           , MTOUCH_MODE_SENSOR(4, 24)
    #define MTOUCH_MODE4_ORDER25_INIT           , MTOUCH_MODE_SENSOR(4, 25)
    #define MTOUCH_MODE4_ORDER26_INIT           , MTOUCH_MODE_SENSOR(4, 26)
    #define MTOUCH_MODE4_ORDER27_INIT           , MTOUCH_MODE_SENSOR(4, 27)
    #define MTOUCH_MODE4_ORDER28_INIT           , MTOUCH_MODE_SENSOR(4, 28)
    #define MTOUCH_MODE4_ORDER29_INIT           , MTOUCH_MODE_SENSOR(4, 29)
   
    #if MTOUCH_MODE4_NUM_SENSORS < 30
        #undef  MTOUCH_MODE4_ORDER29_INIT
        #define MTOUCH_MODE4_ORDER29_INIT
    #endif    
    #if MTOUCH_MODE4_NUM_SENSORS < 29
        #undef  MTOUCH_MODE4_ORDER28_INIT
        #define MTOUCH_MODE4_ORDER28_INIT
    #endif    
    #if MTOUCH_MODE4_NUM_SENSORS < 28
        #undef  MTOUCH_MODE4_ORDER27_INIT
        #define MTOUCH_MODE4_ORDER27_INIT
    #endif    
    #if MTOUCH_MODE4_NUM_SENSORS < 27
        #undef  MTOUCH_MODE4_ORDER26_INIT
        #define MTOUCH_MODE4_ORDER26_INIT
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 26        
        #undef  MTOUCH_MODE4_ORDER25_INIT          
        #define MTOUCH_MODE4_ORDER25_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 25        
        #undef  MTOUCH_MODE4_ORDER24_INIT          
        #define MTOUCH_MODE4_ORDER24_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 24        
        #undef  MTOUCH_MODE4_ORDER23_INIT          
        #define MTOUCH_MODE4_ORDER23_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 23        
        #undef  MTOUCH_MODE4_ORDER22_INIT          
        #define MTOUCH_MODE4_ORDER22_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 22        
        #undef  MTOUCH_MODE4_ORDER21_INIT          
        #define MTOUCH_MODE4_ORDER21_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 21        
        #undef  MTOUCH_MODE4_ORDER20_INIT          
        #define MTOUCH_MODE4_ORDER20_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 20        
        #undef  MTOUCH_MODE4_ORDER19_INIT          
        #define MTOUCH_MODE4_ORDER19_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 19        
        #undef  MTOUCH_MODE4_ORDER18_INIT          
        #define MTOUCH_MODE4_ORDER18_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 18        
        #undef  MTOUCH_MODE4_ORDER17_INIT          
        #define MTOUCH_MODE4_ORDER17_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 17        
        #undef  MTOUCH_MODE4_ORDER16_INIT          
        #define MTOUCH_MODE4_ORDER16_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 16        
        #undef  MTOUCH_MODE4_ORDER15_INIT          
        #define MTOUCH_MODE4_ORDER15_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 15        
        #undef  MTOUCH_MODE4_ORDER14_INIT          
        #define MTOUCH_MODE4_ORDER14_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 14        
        #undef  MTOUCH_MODE4_ORDER13_INIT          
        #define MTOUCH_MODE4_ORDER13_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 13        
        #undef  MTOUCH_MODE4_ORDER12_INIT          
        #define MTOUCH_MODE4_ORDER12_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 12        
        #undef  MTOUCH_MODE4_ORDER11_INIT          
        #define MTOUCH_MODE4_ORDER11_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 11        
        #undef  MTOUCH_MODE4_ORDER10_INIT          
        #define MTOUCH_MODE4_ORDER10_INIT       
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS < 10        
        #undef  MTOUCH_MODE4_ORDER9_INIT           
        #define MTOUCH_MODE4_ORDER9_INIT        
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS <  9        
        #undef  MTOUCH_MODE4_ORDER8_INIT           
        #define MTOUCH_MODE4_ORDER8_INIT        
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS <  8        
        #undef  MTOUCH_MODE4_ORDER7_INIT           
        #define MTOUCH_MODE4_ORDER7_INIT        
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS <  7        
        #undef  MTOUCH_MODE4_ORDER6_INIT           
        #define MTOUCH_MODE4_ORDER6_INIT        
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS <  6        
        #undef  MTOUCH_MODE4_ORDER5_INIT           
        #define MTOUCH_MODE4_ORDER5_INIT        
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS <  5        
        #undef  MTOUCH_MODE4_ORDER4_INIT           
        #define MTOUCH_MODE4_ORDER4_INIT        
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS <  4        
        #undef  MTOUCH_MODE4_ORDER3_INIT           
        #define MTOUCH_MODE4_ORDER3_INIT        
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS <  3        
        #undef  MTOUCH_MODE4_ORDER2_INIT           
        #define MTOUCH_MODE4_ORDER2_INIT        
    #endif
    #if MTOUCH_MODE4_NUM_SENSORS <  2        
        #undef  MTOUCH_MODE4_ORDER1_INIT           
        #define MTOUCH_MODE4_ORDER1_INIT        
    #endif
   
    #if MTOUCH_MODE4_NUM_SENSORS > 0
    #define MTOUCH_MODE4_VAR_INIT               {                             \
                                                    MTOUCH_MODE4_ORDER0_INIT  \
                                                    MTOUCH_MODE4_ORDER1_INIT  \
                                                    MTOUCH_MODE4_ORDER2_INIT  \
                                                    MTOUCH_MODE4_ORDER3_INIT  \
                                                    MTOUCH_MODE4_ORDER4_INIT  \
                                                    MTOUCH_MODE4_ORDER5_INIT  \
                                                    MTOUCH_MODE4_ORDER6_INIT  \
                                                    MTOUCH_MODE4_ORDER7_INIT  \
                                                    MTOUCH_MODE4_ORDER8_INIT  \
                                                    MTOUCH_MODE4_ORDER9_INIT  \
                                                    MTOUCH_MODE4_ORDER10_INIT \
                                                    MTOUCH_MODE4_ORDER11_INIT \
                                                    MTOUCH_MODE4_ORDER12_INIT \
                                                    MTOUCH_MODE4_ORDER13_INIT \
                                                    MTOUCH_MODE4_ORDER14_INIT \
                                                    MTOUCH_MODE4_ORDER15_INIT \
                                                    MTOUCH_MODE4_ORDER16_INIT \
                                                    MTOUCH_MODE4_ORDER17_INIT \
                                                    MTOUCH_MODE4_ORDER18_INIT \
                                                    MTOUCH_MODE4_ORDER19_INIT \
                                                    MTOUCH_MODE4_ORDER20_INIT \
                                                    MTOUCH_MODE4_ORDER21_INIT \
                                                    MTOUCH_MODE4_ORDER22_INIT \
                                                    MTOUCH_MODE4_ORDER23_INIT \
                                                    MTOUCH_MODE4_ORDER24_INIT \
                                                    MTOUCH_MODE4_ORDER25_INIT \
                                                    MTOUCH_MODE4_ORDER26_INIT \
                                                    MTOUCH_MODE4_ORDER27_INIT \
                                                    MTOUCH_MODE4_ORDER28_INIT \
                                                    MTOUCH_MODE4_ORDER29_INIT \
                                                    MTOUCH_MODE_SENSOR_END(0) \
                                                }    
    #endif
    #endif
    
    //
    //
    //
    //
    //      MODE 5
    //
    //
    //
    //
    //
    #if MTOUCH_NUM_MODES > 5
    #if MTOUCH_MODE5_NUM_SENSORS > 0
        #if !defined(MTOUCH_MODE5_SENSOR0)
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 0, but MTOUCH_MODE5_SENSOR0 is not defined.
        #endif
    #else
        #error MTOUCH_MODE5_NUM_SENSORS must be greater than 0.
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS >  1  
        #if !defined(MTOUCH_MODE5_SENSOR1)       
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 1, but MTOUCH_MODE5_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS >  2 
        #if !defined(MTOUCH_MODE5_SENSOR2)       
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 2, but MTOUCH_MODE5_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS >  3  
        #if !defined(MTOUCH_MODE5_SENSOR3)       
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 3, but MTOUCH_MODE5_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS >  4  
        #if !defined(MTOUCH_MODE5_SENSOR4)       
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 4, but MTOUCH_MODE5_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS >  5 
        #if !defined(MTOUCH_MODE5_SENSOR5)       
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 5, but MTOUCH_MODE5_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS >  6  
        #if !defined(MTOUCH_MODE5_SENSOR6)       
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 6, but MTOUCH_MODE5_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS >  7 
        #if !defined(MTOUCH_MODE5_SENSOR7)       
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 7, but MTOUCH_MODE5_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS >  8  
        #if !defined(MTOUCH_MODE5_SENSOR8)       
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 8, but MTOUCH_MODE5_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS >  9  
        #if !defined(MTOUCH_MODE5_SENSOR9)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 9, but MTOUCH_MODE5_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 10  
        #if !defined(MTOUCH_MODE5_SENSOR0)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 0, but MTOUCH_MODE5_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 11  
        #if !defined(MTOUCH_MODE5_SENSOR1)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 1, but MTOUCH_MODE5_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 12  
        #if !defined(MTOUCH_MODE5_SENSOR2)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 2, but MTOUCH_MODE5_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 13  
        #if !defined(MTOUCH_MODE5_SENSOR3)       
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 3, but MTOUCH_MODE5_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 14  
        #if !defined(MTOUCH_MODE5_SENSOR4)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 4, but MTOUCH_MODE5_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 15  
        #if !defined(MTOUCH_MODE5_SENSOR5)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 5, but MTOUCH_MODE5_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 16 
        #if !defined(MTOUCH_MODE5_SENSOR6)
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 6, but MTOUCH_MODE5_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 17
        #if !defined(MTOUCH_MODE5_SENSOR7)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 7, but MTOUCH_MODE5_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 18 
        #if !defined(MTOUCH_MODE5_SENSOR8)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 8, but MTOUCH_MODE5_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 19 
        #if !defined(MTOUCH_MODE5_SENSOR9)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 9, but MTOUCH_MODE5_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 20 
        #if !defined(MTOUCH_MODE5_SENSOR0)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 0, but MTOUCH_MODE5_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 21   
        #if !defined(MTOUCH_MODE5_SENSOR1)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 1, but MTOUCH_MODE5_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 22 
        #if !defined(MTOUCH_MODE5_SENSOR2)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 2, but MTOUCH_MODE5_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 23   
        #if !defined(MTOUCH_MODE5_SENSOR3)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 3, but MTOUCH_MODE5_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 24   
        #if !defined(MTOUCH_MODE5_SENSOR4)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 4, but MTOUCH_MODE5_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 25 
        #if !defined(MTOUCH_MODE5_SENSOR5)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 5, but MTOUCH_MODE5_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 26  
        #if !defined(MTOUCH_MODE5_SENSOR6)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 6, but MTOUCH_MODE5_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 27   
        #if !defined(MTOUCH_MODE5_SENSOR7)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 7, but MTOUCH_MODE5_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 28   
        #if !defined(MTOUCH_MODE5_SENSOR8)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 8, but MTOUCH_MODE5_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS > 29   
        #if !defined(MTOUCH_MODE5_SENSOR9)        
        #error MTOUCH_MODE5_NUM_SENSORS is greater than 9, but MTOUCH_MODE5_SENSOR9 is not defined.     
        #endif    
    #endif
    
    
    #define MTOUCH_MODE5_ORDER0_INIT              MTOUCH_MODE_SENSOR(5, 0)
    #define MTOUCH_MODE5_ORDER1_INIT            , MTOUCH_MODE_SENSOR(5, 1)
    #define MTOUCH_MODE5_ORDER2_INIT            , MTOUCH_MODE_SENSOR(5, 2)
    #define MTOUCH_MODE5_ORDER3_INIT            , MTOUCH_MODE_SENSOR(5, 3)
    #define MTOUCH_MODE5_ORDER4_INIT            , MTOUCH_MODE_SENSOR(5, 4)
    #define MTOUCH_MODE5_ORDER5_INIT            , MTOUCH_MODE_SENSOR(5, 5)
    #define MTOUCH_MODE5_ORDER6_INIT            , MTOUCH_MODE_SENSOR(5, 6)
    #define MTOUCH_MODE5_ORDER7_INIT            , MTOUCH_MODE_SENSOR(5, 7)
    #define MTOUCH_MODE5_ORDER8_INIT            , MTOUCH_MODE_SENSOR(5, 8)
    #define MTOUCH_MODE5_ORDER9_INIT            , MTOUCH_MODE_SENSOR(5, 9)
    #define MTOUCH_MODE5_ORDER10_INIT           , MTOUCH_MODE_SENSOR(5, 10)
    #define MTOUCH_MODE5_ORDER11_INIT           , MTOUCH_MODE_SENSOR(5, 11)
    #define MTOUCH_MODE5_ORDER12_INIT           , MTOUCH_MODE_SENSOR(5, 12)
    #define MTOUCH_MODE5_ORDER13_INIT           , MTOUCH_MODE_SENSOR(5, 13)
    #define MTOUCH_MODE5_ORDER14_INIT           , MTOUCH_MODE_SENSOR(5, 14)
    #define MTOUCH_MODE5_ORDER15_INIT           , MTOUCH_MODE_SENSOR(5, 15)
    #define MTOUCH_MODE5_ORDER16_INIT           , MTOUCH_MODE_SENSOR(5, 16)
    #define MTOUCH_MODE5_ORDER17_INIT           , MTOUCH_MODE_SENSOR(5, 17)
    #define MTOUCH_MODE5_ORDER18_INIT           , MTOUCH_MODE_SENSOR(5, 18)
    #define MTOUCH_MODE5_ORDER19_INIT           , MTOUCH_MODE_SENSOR(5, 19)
    #define MTOUCH_MODE5_ORDER20_INIT           , MTOUCH_MODE_SENSOR(5, 20)
    #define MTOUCH_MODE5_ORDER21_INIT           , MTOUCH_MODE_SENSOR(5, 21)
    #define MTOUCH_MODE5_ORDER22_INIT           , MTOUCH_MODE_SENSOR(5, 22)
    #define MTOUCH_MODE5_ORDER23_INIT           , MTOUCH_MODE_SENSOR(5, 23)
    #define MTOUCH_MODE5_ORDER24_INIT           , MTOUCH_MODE_SENSOR(5, 24)
    #define MTOUCH_MODE5_ORDER25_INIT           , MTOUCH_MODE_SENSOR(5, 25)
    #define MTOUCH_MODE5_ORDER26_INIT           , MTOUCH_MODE_SENSOR(5, 26)
    #define MTOUCH_MODE5_ORDER27_INIT           , MTOUCH_MODE_SENSOR(5, 27)
    #define MTOUCH_MODE5_ORDER28_INIT           , MTOUCH_MODE_SENSOR(5, 28)
    #define MTOUCH_MODE5_ORDER29_INIT           , MTOUCH_MODE_SENSOR(5, 29)
   
    #if MTOUCH_MODE5_NUM_SENSORS < 30
        #undef  MTOUCH_MODE5_ORDER29_INIT
        #define MTOUCH_MODE5_ORDER29_INIT
    #endif    
    #if MTOUCH_MODE5_NUM_SENSORS < 29
        #undef  MTOUCH_MODE5_ORDER28_INIT
        #define MTOUCH_MODE5_ORDER28_INIT
    #endif    
    #if MTOUCH_MODE5_NUM_SENSORS < 28
        #undef  MTOUCH_MODE5_ORDER27_INIT
        #define MTOUCH_MODE5_ORDER27_INIT
    #endif    
    #if MTOUCH_MODE5_NUM_SENSORS < 27
        #undef  MTOUCH_MODE5_ORDER26_INIT
        #define MTOUCH_MODE5_ORDER26_INIT
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 26        
        #undef  MTOUCH_MODE5_ORDER25_INIT          
        #define MTOUCH_MODE5_ORDER25_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 25        
        #undef  MTOUCH_MODE5_ORDER24_INIT          
        #define MTOUCH_MODE5_ORDER24_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 24        
        #undef  MTOUCH_MODE5_ORDER23_INIT          
        #define MTOUCH_MODE5_ORDER23_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 23        
        #undef  MTOUCH_MODE5_ORDER22_INIT          
        #define MTOUCH_MODE5_ORDER22_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 22        
        #undef  MTOUCH_MODE5_ORDER21_INIT          
        #define MTOUCH_MODE5_ORDER21_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 21        
        #undef  MTOUCH_MODE5_ORDER20_INIT          
        #define MTOUCH_MODE5_ORDER20_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 20        
        #undef  MTOUCH_MODE5_ORDER19_INIT          
        #define MTOUCH_MODE5_ORDER19_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 19        
        #undef  MTOUCH_MODE5_ORDER18_INIT          
        #define MTOUCH_MODE5_ORDER18_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 18        
        #undef  MTOUCH_MODE5_ORDER17_INIT          
        #define MTOUCH_MODE5_ORDER17_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 17        
        #undef  MTOUCH_MODE5_ORDER16_INIT          
        #define MTOUCH_MODE5_ORDER16_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 16        
        #undef  MTOUCH_MODE5_ORDER15_INIT          
        #define MTOUCH_MODE5_ORDER15_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 15        
        #undef  MTOUCH_MODE5_ORDER14_INIT          
        #define MTOUCH_MODE5_ORDER14_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 14        
        #undef  MTOUCH_MODE5_ORDER13_INIT          
        #define MTOUCH_MODE5_ORDER13_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 13        
        #undef  MTOUCH_MODE5_ORDER12_INIT          
        #define MTOUCH_MODE5_ORDER12_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 12        
        #undef  MTOUCH_MODE5_ORDER11_INIT          
        #define MTOUCH_MODE5_ORDER11_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 11        
        #undef  MTOUCH_MODE5_ORDER10_INIT          
        #define MTOUCH_MODE5_ORDER10_INIT       
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS < 10        
        #undef  MTOUCH_MODE5_ORDER9_INIT           
        #define MTOUCH_MODE5_ORDER9_INIT        
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS <  9        
        #undef  MTOUCH_MODE5_ORDER8_INIT           
        #define MTOUCH_MODE5_ORDER8_INIT        
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS <  8        
        #undef  MTOUCH_MODE5_ORDER7_INIT           
        #define MTOUCH_MODE5_ORDER7_INIT        
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS <  7        
        #undef  MTOUCH_MODE5_ORDER6_INIT           
        #define MTOUCH_MODE5_ORDER6_INIT        
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS <  6        
        #undef  MTOUCH_MODE5_ORDER5_INIT           
        #define MTOUCH_MODE5_ORDER5_INIT        
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS <  5        
        #undef  MTOUCH_MODE5_ORDER4_INIT           
        #define MTOUCH_MODE5_ORDER4_INIT        
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS <  4        
        #undef  MTOUCH_MODE5_ORDER3_INIT           
        #define MTOUCH_MODE5_ORDER3_INIT        
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS <  3        
        #undef  MTOUCH_MODE5_ORDER2_INIT           
        #define MTOUCH_MODE5_ORDER2_INIT        
    #endif
    #if MTOUCH_MODE5_NUM_SENSORS <  2        
        #undef  MTOUCH_MODE5_ORDER1_INIT           
        #define MTOUCH_MODE5_ORDER1_INIT        
    #endif
   
    #if MTOUCH_MODE5_NUM_SENSORS > 0
    #define MTOUCH_MODE5_VAR_INIT               {                             \
                                                    MTOUCH_MODE5_ORDER0_INIT  \
                                                    MTOUCH_MODE5_ORDER1_INIT  \
                                                    MTOUCH_MODE5_ORDER2_INIT  \
                                                    MTOUCH_MODE5_ORDER3_INIT  \
                                                    MTOUCH_MODE5_ORDER4_INIT  \
                                                    MTOUCH_MODE5_ORDER5_INIT  \
                                                    MTOUCH_MODE5_ORDER6_INIT  \
                                                    MTOUCH_MODE5_ORDER7_INIT  \
                                                    MTOUCH_MODE5_ORDER8_INIT  \
                                                    MTOUCH_MODE5_ORDER9_INIT  \
                                                    MTOUCH_MODE5_ORDER10_INIT \
                                                    MTOUCH_MODE5_ORDER11_INIT \
                                                    MTOUCH_MODE5_ORDER12_INIT \
                                                    MTOUCH_MODE5_ORDER13_INIT \
                                                    MTOUCH_MODE5_ORDER14_INIT \
                                                    MTOUCH_MODE5_ORDER15_INIT \
                                                    MTOUCH_MODE5_ORDER16_INIT \
                                                    MTOUCH_MODE5_ORDER17_INIT \
                                                    MTOUCH_MODE5_ORDER18_INIT \
                                                    MTOUCH_MODE5_ORDER19_INIT \
                                                    MTOUCH_MODE5_ORDER20_INIT \
                                                    MTOUCH_MODE5_ORDER21_INIT \
                                                    MTOUCH_MODE5_ORDER22_INIT \
                                                    MTOUCH_MODE5_ORDER23_INIT \
                                                    MTOUCH_MODE5_ORDER24_INIT \
                                                    MTOUCH_MODE5_ORDER25_INIT \
                                                    MTOUCH_MODE5_ORDER26_INIT \
                                                    MTOUCH_MODE5_ORDER27_INIT \
                                                    MTOUCH_MODE5_ORDER28_INIT \
                                                    MTOUCH_MODE5_ORDER29_INIT \
                                                    MTOUCH_MODE_SENSOR_END(0) \
                                                }    
    #endif
    #endif
    
    //
    //
    //
    //
    //      MODE 6
    //
    //
    //
    //
    //
    #if MTOUCH_NUM_MODES > 6
    #if MTOUCH_MODE6_NUM_SENSORS > 0
        #if !defined(MTOUCH_MODE6_SENSOR0)
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 0, but MTOUCH_MODE6_SENSOR0 is not defined.
        #endif
    #else
        #error MTOUCH_MODE6_NUM_SENSORS must be greater than 0.
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS >  1  
        #if !defined(MTOUCH_MODE6_SENSOR1)       
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 1, but MTOUCH_MODE6_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS >  2 
        #if !defined(MTOUCH_MODE6_SENSOR2)       
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 2, but MTOUCH_MODE6_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS >  3  
        #if !defined(MTOUCH_MODE6_SENSOR3)       
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 3, but MTOUCH_MODE6_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS >  4  
        #if !defined(MTOUCH_MODE6_SENSOR4)       
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 4, but MTOUCH_MODE6_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS >  5 
        #if !defined(MTOUCH_MODE6_SENSOR5)       
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 5, but MTOUCH_MODE6_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS >  6  
        #if !defined(MTOUCH_MODE6_SENSOR6)       
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 6, but MTOUCH_MODE6_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS >  7 
        #if !defined(MTOUCH_MODE6_SENSOR7)       
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 7, but MTOUCH_MODE6_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS >  8  
        #if !defined(MTOUCH_MODE6_SENSOR8)       
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 8, but MTOUCH_MODE6_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS >  9  
        #if !defined(MTOUCH_MODE6_SENSOR9)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 9, but MTOUCH_MODE6_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 10  
        #if !defined(MTOUCH_MODE6_SENSOR0)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 0, but MTOUCH_MODE6_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 11  
        #if !defined(MTOUCH_MODE6_SENSOR1)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 1, but MTOUCH_MODE6_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 12  
        #if !defined(MTOUCH_MODE6_SENSOR2)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 2, but MTOUCH_MODE6_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 13  
        #if !defined(MTOUCH_MODE6_SENSOR3)       
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 3, but MTOUCH_MODE6_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 14  
        #if !defined(MTOUCH_MODE6_SENSOR4)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 4, but MTOUCH_MODE6_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 15  
        #if !defined(MTOUCH_MODE6_SENSOR5)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 5, but MTOUCH_MODE6_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 16 
        #if !defined(MTOUCH_MODE6_SENSOR6)
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 6, but MTOUCH_MODE6_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 17
        #if !defined(MTOUCH_MODE6_SENSOR7)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 7, but MTOUCH_MODE6_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 18 
        #if !defined(MTOUCH_MODE6_SENSOR8)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 8, but MTOUCH_MODE6_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 19 
        #if !defined(MTOUCH_MODE6_SENSOR9)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 9, but MTOUCH_MODE6_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 20 
        #if !defined(MTOUCH_MODE6_SENSOR0)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 0, but MTOUCH_MODE6_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 21   
        #if !defined(MTOUCH_MODE6_SENSOR1)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 1, but MTOUCH_MODE6_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 22 
        #if !defined(MTOUCH_MODE6_SENSOR2)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 2, but MTOUCH_MODE6_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 23   
        #if !defined(MTOUCH_MODE6_SENSOR3)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 3, but MTOUCH_MODE6_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 24   
        #if !defined(MTOUCH_MODE6_SENSOR4)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 4, but MTOUCH_MODE6_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 25 
        #if !defined(MTOUCH_MODE6_SENSOR5)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 5, but MTOUCH_MODE6_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 26  
        #if !defined(MTOUCH_MODE6_SENSOR6)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 6, but MTOUCH_MODE6_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 27   
        #if !defined(MTOUCH_MODE6_SENSOR7)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 7, but MTOUCH_MODE6_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 28   
        #if !defined(MTOUCH_MODE6_SENSOR8)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 8, but MTOUCH_MODE6_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS > 29   
        #if !defined(MTOUCH_MODE6_SENSOR9)        
        #error MTOUCH_MODE6_NUM_SENSORS is greater than 9, but MTOUCH_MODE6_SENSOR9 is not defined.     
        #endif    
    #endif
    
    
    #define MTOUCH_MODE6_ORDER0_INIT              MTOUCH_MODE_SENSOR(6, 0)
    #define MTOUCH_MODE6_ORDER1_INIT            , MTOUCH_MODE_SENSOR(6, 1)
    #define MTOUCH_MODE6_ORDER2_INIT            , MTOUCH_MODE_SENSOR(6, 2)
    #define MTOUCH_MODE6_ORDER3_INIT            , MTOUCH_MODE_SENSOR(6, 3)
    #define MTOUCH_MODE6_ORDER4_INIT            , MTOUCH_MODE_SENSOR(6, 4)
    #define MTOUCH_MODE6_ORDER5_INIT            , MTOUCH_MODE_SENSOR(6, 5)
    #define MTOUCH_MODE6_ORDER6_INIT            , MTOUCH_MODE_SENSOR(6, 6)
    #define MTOUCH_MODE6_ORDER7_INIT            , MTOUCH_MODE_SENSOR(6, 7)
    #define MTOUCH_MODE6_ORDER8_INIT            , MTOUCH_MODE_SENSOR(6, 8)
    #define MTOUCH_MODE6_ORDER9_INIT            , MTOUCH_MODE_SENSOR(6, 9)
    #define MTOUCH_MODE6_ORDER10_INIT           , MTOUCH_MODE_SENSOR(6, 10)
    #define MTOUCH_MODE6_ORDER11_INIT           , MTOUCH_MODE_SENSOR(6, 11)
    #define MTOUCH_MODE6_ORDER12_INIT           , MTOUCH_MODE_SENSOR(6, 12)
    #define MTOUCH_MODE6_ORDER13_INIT           , MTOUCH_MODE_SENSOR(6, 13)
    #define MTOUCH_MODE6_ORDER14_INIT           , MTOUCH_MODE_SENSOR(6, 14)
    #define MTOUCH_MODE6_ORDER15_INIT           , MTOUCH_MODE_SENSOR(6, 15)
    #define MTOUCH_MODE6_ORDER16_INIT           , MTOUCH_MODE_SENSOR(6, 16)
    #define MTOUCH_MODE6_ORDER17_INIT           , MTOUCH_MODE_SENSOR(6, 17)
    #define MTOUCH_MODE6_ORDER18_INIT           , MTOUCH_MODE_SENSOR(6, 18)
    #define MTOUCH_MODE6_ORDER19_INIT           , MTOUCH_MODE_SENSOR(6, 19)
    #define MTOUCH_MODE6_ORDER20_INIT           , MTOUCH_MODE_SENSOR(6, 20)
    #define MTOUCH_MODE6_ORDER21_INIT           , MTOUCH_MODE_SENSOR(6, 21)
    #define MTOUCH_MODE6_ORDER22_INIT           , MTOUCH_MODE_SENSOR(6, 22)
    #define MTOUCH_MODE6_ORDER23_INIT           , MTOUCH_MODE_SENSOR(6, 23)
    #define MTOUCH_MODE6_ORDER24_INIT           , MTOUCH_MODE_SENSOR(6, 24)
    #define MTOUCH_MODE6_ORDER25_INIT           , MTOUCH_MODE_SENSOR(6, 25)
    #define MTOUCH_MODE6_ORDER26_INIT           , MTOUCH_MODE_SENSOR(6, 26)
    #define MTOUCH_MODE6_ORDER27_INIT           , MTOUCH_MODE_SENSOR(6, 27)
    #define MTOUCH_MODE6_ORDER28_INIT           , MTOUCH_MODE_SENSOR(6, 28)
    #define MTOUCH_MODE6_ORDER29_INIT           , MTOUCH_MODE_SENSOR(6, 29)
   
    #if MTOUCH_MODE6_NUM_SENSORS < 30
        #undef  MTOUCH_MODE6_ORDER29_INIT
        #define MTOUCH_MODE6_ORDER29_INIT
    #endif    
    #if MTOUCH_MODE6_NUM_SENSORS < 29
        #undef  MTOUCH_MODE6_ORDER28_INIT
        #define MTOUCH_MODE6_ORDER28_INIT
    #endif    
    #if MTOUCH_MODE6_NUM_SENSORS < 28
        #undef  MTOUCH_MODE6_ORDER27_INIT
        #define MTOUCH_MODE6_ORDER27_INIT
    #endif    
    #if MTOUCH_MODE6_NUM_SENSORS < 27
        #undef  MTOUCH_MODE6_ORDER26_INIT
        #define MTOUCH_MODE6_ORDER26_INIT
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 26        
        #undef  MTOUCH_MODE6_ORDER25_INIT          
        #define MTOUCH_MODE6_ORDER25_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 25        
        #undef  MTOUCH_MODE6_ORDER24_INIT          
        #define MTOUCH_MODE6_ORDER24_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 24        
        #undef  MTOUCH_MODE6_ORDER23_INIT          
        #define MTOUCH_MODE6_ORDER23_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 23        
        #undef  MTOUCH_MODE6_ORDER22_INIT          
        #define MTOUCH_MODE6_ORDER22_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 22        
        #undef  MTOUCH_MODE6_ORDER21_INIT          
        #define MTOUCH_MODE6_ORDER21_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 21        
        #undef  MTOUCH_MODE6_ORDER20_INIT          
        #define MTOUCH_MODE6_ORDER20_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 20        
        #undef  MTOUCH_MODE6_ORDER19_INIT          
        #define MTOUCH_MODE6_ORDER19_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 19        
        #undef  MTOUCH_MODE6_ORDER18_INIT          
        #define MTOUCH_MODE6_ORDER18_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 18        
        #undef  MTOUCH_MODE6_ORDER17_INIT          
        #define MTOUCH_MODE6_ORDER17_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 17        
        #undef  MTOUCH_MODE6_ORDER16_INIT          
        #define MTOUCH_MODE6_ORDER16_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 16        
        #undef  MTOUCH_MODE6_ORDER15_INIT          
        #define MTOUCH_MODE6_ORDER15_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 15        
        #undef  MTOUCH_MODE6_ORDER14_INIT          
        #define MTOUCH_MODE6_ORDER14_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 14        
        #undef  MTOUCH_MODE6_ORDER13_INIT          
        #define MTOUCH_MODE6_ORDER13_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 13        
        #undef  MTOUCH_MODE6_ORDER12_INIT          
        #define MTOUCH_MODE6_ORDER12_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 12        
        #undef  MTOUCH_MODE6_ORDER11_INIT          
        #define MTOUCH_MODE6_ORDER11_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 11        
        #undef  MTOUCH_MODE6_ORDER10_INIT          
        #define MTOUCH_MODE6_ORDER10_INIT       
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS < 10        
        #undef  MTOUCH_MODE6_ORDER9_INIT           
        #define MTOUCH_MODE6_ORDER9_INIT        
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS <  9        
        #undef  MTOUCH_MODE6_ORDER8_INIT           
        #define MTOUCH_MODE6_ORDER8_INIT        
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS <  8        
        #undef  MTOUCH_MODE6_ORDER7_INIT           
        #define MTOUCH_MODE6_ORDER7_INIT        
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS <  7        
        #undef  MTOUCH_MODE6_ORDER6_INIT           
        #define MTOUCH_MODE6_ORDER6_INIT        
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS <  6        
        #undef  MTOUCH_MODE6_ORDER5_INIT           
        #define MTOUCH_MODE6_ORDER5_INIT        
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS <  5        
        #undef  MTOUCH_MODE6_ORDER4_INIT           
        #define MTOUCH_MODE6_ORDER4_INIT        
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS <  4        
        #undef  MTOUCH_MODE6_ORDER3_INIT           
        #define MTOUCH_MODE6_ORDER3_INIT        
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS <  3        
        #undef  MTOUCH_MODE6_ORDER2_INIT           
        #define MTOUCH_MODE6_ORDER2_INIT        
    #endif
    #if MTOUCH_MODE6_NUM_SENSORS <  2        
        #undef  MTOUCH_MODE6_ORDER1_INIT           
        #define MTOUCH_MODE6_ORDER1_INIT        
    #endif
   
    #if MTOUCH_MODE6_NUM_SENSORS > 0
    #define MTOUCH_MODE6_VAR_INIT               {                             \
                                                    MTOUCH_MODE6_ORDER0_INIT  \
                                                    MTOUCH_MODE6_ORDER1_INIT  \
                                                    MTOUCH_MODE6_ORDER2_INIT  \
                                                    MTOUCH_MODE6_ORDER3_INIT  \
                                                    MTOUCH_MODE6_ORDER4_INIT  \
                                                    MTOUCH_MODE6_ORDER5_INIT  \
                                                    MTOUCH_MODE6_ORDER6_INIT  \
                                                    MTOUCH_MODE6_ORDER7_INIT  \
                                                    MTOUCH_MODE6_ORDER8_INIT  \
                                                    MTOUCH_MODE6_ORDER9_INIT  \
                                                    MTOUCH_MODE6_ORDER10_INIT \
                                                    MTOUCH_MODE6_ORDER11_INIT \
                                                    MTOUCH_MODE6_ORDER12_INIT \
                                                    MTOUCH_MODE6_ORDER13_INIT \
                                                    MTOUCH_MODE6_ORDER14_INIT \
                                                    MTOUCH_MODE6_ORDER15_INIT \
                                                    MTOUCH_MODE6_ORDER16_INIT \
                                                    MTOUCH_MODE6_ORDER17_INIT \
                                                    MTOUCH_MODE6_ORDER18_INIT \
                                                    MTOUCH_MODE6_ORDER19_INIT \
                                                    MTOUCH_MODE6_ORDER20_INIT \
                                                    MTOUCH_MODE6_ORDER21_INIT \
                                                    MTOUCH_MODE6_ORDER22_INIT \
                                                    MTOUCH_MODE6_ORDER23_INIT \
                                                    MTOUCH_MODE6_ORDER24_INIT \
                                                    MTOUCH_MODE6_ORDER25_INIT \
                                                    MTOUCH_MODE6_ORDER26_INIT \
                                                    MTOUCH_MODE6_ORDER27_INIT \
                                                    MTOUCH_MODE6_ORDER28_INIT \
                                                    MTOUCH_MODE6_ORDER29_INIT \
                                                    MTOUCH_MODE_SENSOR_END(0) \
                                                }    
    #endif
    #endif
    
    //
    //
    //
    //
    //      MODE 7
    //
    //
    //
    //
    //
    #if MTOUCH_NUM_MODES > 7
    #if MTOUCH_MODE7_NUM_SENSORS > 0
        #if !defined(MTOUCH_MODE7_SENSOR0)
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 0, but MTOUCH_MODE7_SENSOR0 is not defined.
        #endif
    #else
        #error MTOUCH_MODE7_NUM_SENSORS must be greater than 0.
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS >  1  
        #if !defined(MTOUCH_MODE7_SENSOR1)       
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 1, but MTOUCH_MODE7_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS >  2 
        #if !defined(MTOUCH_MODE7_SENSOR2)       
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 2, but MTOUCH_MODE7_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS >  3  
        #if !defined(MTOUCH_MODE7_SENSOR3)       
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 3, but MTOUCH_MODE7_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS >  4  
        #if !defined(MTOUCH_MODE7_SENSOR4)       
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 4, but MTOUCH_MODE7_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS >  5 
        #if !defined(MTOUCH_MODE7_SENSOR5)       
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 5, but MTOUCH_MODE7_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS >  6  
        #if !defined(MTOUCH_MODE7_SENSOR6)       
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 6, but MTOUCH_MODE7_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS >  7 
        #if !defined(MTOUCH_MODE7_SENSOR7)       
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 7, but MTOUCH_MODE7_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS >  8  
        #if !defined(MTOUCH_MODE7_SENSOR8)       
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 8, but MTOUCH_MODE7_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS >  9  
        #if !defined(MTOUCH_MODE7_SENSOR9)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 9, but MTOUCH_MODE7_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 10  
        #if !defined(MTOUCH_MODE7_SENSOR0)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 0, but MTOUCH_MODE7_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 11  
        #if !defined(MTOUCH_MODE7_SENSOR1)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 1, but MTOUCH_MODE7_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 12  
        #if !defined(MTOUCH_MODE7_SENSOR2)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 2, but MTOUCH_MODE7_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 13  
        #if !defined(MTOUCH_MODE7_SENSOR3)       
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 3, but MTOUCH_MODE7_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 14  
        #if !defined(MTOUCH_MODE7_SENSOR4)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 4, but MTOUCH_MODE7_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 15  
        #if !defined(MTOUCH_MODE7_SENSOR5)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 5, but MTOUCH_MODE7_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 16 
        #if !defined(MTOUCH_MODE7_SENSOR6)
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 6, but MTOUCH_MODE7_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 17
        #if !defined(MTOUCH_MODE7_SENSOR7)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 7, but MTOUCH_MODE7_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 18 
        #if !defined(MTOUCH_MODE7_SENSOR8)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 8, but MTOUCH_MODE7_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 19 
        #if !defined(MTOUCH_MODE7_SENSOR9)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 9, but MTOUCH_MODE7_SENSOR9 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 20 
        #if !defined(MTOUCH_MODE7_SENSOR0)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 0, but MTOUCH_MODE7_SENSOR0 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 21   
        #if !defined(MTOUCH_MODE7_SENSOR1)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 1, but MTOUCH_MODE7_SENSOR1 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 22 
        #if !defined(MTOUCH_MODE7_SENSOR2)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 2, but MTOUCH_MODE7_SENSOR2 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 23   
        #if !defined(MTOUCH_MODE7_SENSOR3)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 3, but MTOUCH_MODE7_SENSOR3 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 24   
        #if !defined(MTOUCH_MODE7_SENSOR4)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 4, but MTOUCH_MODE7_SENSOR4 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 25 
        #if !defined(MTOUCH_MODE7_SENSOR5)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 5, but MTOUCH_MODE7_SENSOR5 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 26  
        #if !defined(MTOUCH_MODE7_SENSOR6)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 6, but MTOUCH_MODE7_SENSOR6 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 27   
        #if !defined(MTOUCH_MODE7_SENSOR7)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 7, but MTOUCH_MODE7_SENSOR7 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 28   
        #if !defined(MTOUCH_MODE7_SENSOR8)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 8, but MTOUCH_MODE7_SENSOR8 is not defined.     
        #endif    
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS > 29   
        #if !defined(MTOUCH_MODE7_SENSOR9)        
        #error MTOUCH_MODE7_NUM_SENSORS is greater than 9, but MTOUCH_MODE7_SENSOR9 is not defined.     
        #endif    
    #endif
    
    
    #define MTOUCH_MODE7_ORDER0_INIT              MTOUCH_MODE_SENSOR(7, 0)
    #define MTOUCH_MODE7_ORDER1_INIT            , MTOUCH_MODE_SENSOR(7, 1)
    #define MTOUCH_MODE7_ORDER2_INIT            , MTOUCH_MODE_SENSOR(7, 2)
    #define MTOUCH_MODE7_ORDER3_INIT            , MTOUCH_MODE_SENSOR(7, 3)
    #define MTOUCH_MODE7_ORDER4_INIT            , MTOUCH_MODE_SENSOR(7, 4)
    #define MTOUCH_MODE7_ORDER5_INIT            , MTOUCH_MODE_SENSOR(7, 5)
    #define MTOUCH_MODE7_ORDER6_INIT            , MTOUCH_MODE_SENSOR(7, 6)
    #define MTOUCH_MODE7_ORDER7_INIT            , MTOUCH_MODE_SENSOR(7, 7)
    #define MTOUCH_MODE7_ORDER8_INIT            , MTOUCH_MODE_SENSOR(7, 8)
    #define MTOUCH_MODE7_ORDER9_INIT            , MTOUCH_MODE_SENSOR(7, 9)
    #define MTOUCH_MODE7_ORDER10_INIT           , MTOUCH_MODE_SENSOR(7, 10)
    #define MTOUCH_MODE7_ORDER11_INIT           , MTOUCH_MODE_SENSOR(7, 11)
    #define MTOUCH_MODE7_ORDER12_INIT           , MTOUCH_MODE_SENSOR(7, 12)
    #define MTOUCH_MODE7_ORDER13_INIT           , MTOUCH_MODE_SENSOR(7, 13)
    #define MTOUCH_MODE7_ORDER14_INIT           , MTOUCH_MODE_SENSOR(7, 14)
    #define MTOUCH_MODE7_ORDER15_INIT           , MTOUCH_MODE_SENSOR(7, 15)
    #define MTOUCH_MODE7_ORDER16_INIT           , MTOUCH_MODE_SENSOR(7, 16)
    #define MTOUCH_MODE7_ORDER17_INIT           , MTOUCH_MODE_SENSOR(7, 17)
    #define MTOUCH_MODE7_ORDER18_INIT           , MTOUCH_MODE_SENSOR(7, 18)
    #define MTOUCH_MODE7_ORDER19_INIT           , MTOUCH_MODE_SENSOR(7, 19)
    #define MTOUCH_MODE7_ORDER20_INIT           , MTOUCH_MODE_SENSOR(7, 20)
    #define MTOUCH_MODE7_ORDER21_INIT           , MTOUCH_MODE_SENSOR(7, 21)
    #define MTOUCH_MODE7_ORDER22_INIT           , MTOUCH_MODE_SENSOR(7, 22)
    #define MTOUCH_MODE7_ORDER23_INIT           , MTOUCH_MODE_SENSOR(7, 23)
    #define MTOUCH_MODE7_ORDER24_INIT           , MTOUCH_MODE_SENSOR(7, 24)
    #define MTOUCH_MODE7_ORDER25_INIT           , MTOUCH_MODE_SENSOR(7, 25)
    #define MTOUCH_MODE7_ORDER26_INIT           , MTOUCH_MODE_SENSOR(7, 26)
    #define MTOUCH_MODE7_ORDER27_INIT           , MTOUCH_MODE_SENSOR(7, 27)
    #define MTOUCH_MODE7_ORDER28_INIT           , MTOUCH_MODE_SENSOR(7, 28)
    #define MTOUCH_MODE7_ORDER29_INIT           , MTOUCH_MODE_SENSOR(7, 29)
   
    #if MTOUCH_MODE7_NUM_SENSORS < 30
        #undef  MTOUCH_MODE7_ORDER29_INIT
        #define MTOUCH_MODE7_ORDER29_INIT
    #endif    
    #if MTOUCH_MODE7_NUM_SENSORS < 29
        #undef  MTOUCH_MODE7_ORDER28_INIT
        #define MTOUCH_MODE7_ORDER28_INIT
    #endif    
    #if MTOUCH_MODE7_NUM_SENSORS < 28
        #undef  MTOUCH_MODE7_ORDER27_INIT
        #define MTOUCH_MODE7_ORDER27_INIT
    #endif    
    #if MTOUCH_MODE7_NUM_SENSORS < 27
        #undef  MTOUCH_MODE7_ORDER26_INIT
        #define MTOUCH_MODE7_ORDER26_INIT
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 26        
        #undef  MTOUCH_MODE7_ORDER25_INIT          
        #define MTOUCH_MODE7_ORDER25_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 25        
        #undef  MTOUCH_MODE7_ORDER24_INIT          
        #define MTOUCH_MODE7_ORDER24_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 24        
        #undef  MTOUCH_MODE7_ORDER23_INIT          
        #define MTOUCH_MODE7_ORDER23_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 23        
        #undef  MTOUCH_MODE7_ORDER22_INIT          
        #define MTOUCH_MODE7_ORDER22_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 22        
        #undef  MTOUCH_MODE7_ORDER21_INIT          
        #define MTOUCH_MODE7_ORDER21_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 21        
        #undef  MTOUCH_MODE7_ORDER20_INIT          
        #define MTOUCH_MODE7_ORDER20_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 20        
        #undef  MTOUCH_MODE7_ORDER19_INIT          
        #define MTOUCH_MODE7_ORDER19_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 19        
        #undef  MTOUCH_MODE7_ORDER18_INIT          
        #define MTOUCH_MODE7_ORDER18_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 18        
        #undef  MTOUCH_MODE7_ORDER17_INIT          
        #define MTOUCH_MODE7_ORDER17_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 17        
        #undef  MTOUCH_MODE7_ORDER16_INIT          
        #define MTOUCH_MODE7_ORDER16_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 16        
        #undef  MTOUCH_MODE7_ORDER15_INIT          
        #define MTOUCH_MODE7_ORDER15_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 15        
        #undef  MTOUCH_MODE7_ORDER14_INIT          
        #define MTOUCH_MODE7_ORDER14_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 14        
        #undef  MTOUCH_MODE7_ORDER13_INIT          
        #define MTOUCH_MODE7_ORDER13_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 13        
        #undef  MTOUCH_MODE7_ORDER12_INIT          
        #define MTOUCH_MODE7_ORDER12_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 12        
        #undef  MTOUCH_MODE7_ORDER11_INIT          
        #define MTOUCH_MODE7_ORDER11_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 11        
        #undef  MTOUCH_MODE7_ORDER10_INIT          
        #define MTOUCH_MODE7_ORDER10_INIT       
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS < 10        
        #undef  MTOUCH_MODE7_ORDER9_INIT           
        #define MTOUCH_MODE7_ORDER9_INIT        
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS <  9        
        #undef  MTOUCH_MODE7_ORDER8_INIT           
        #define MTOUCH_MODE7_ORDER8_INIT        
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS <  8        
        #undef  MTOUCH_MODE7_ORDER7_INIT           
        #define MTOUCH_MODE7_ORDER7_INIT        
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS <  7        
        #undef  MTOUCH_MODE7_ORDER6_INIT           
        #define MTOUCH_MODE7_ORDER6_INIT        
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS <  6        
        #undef  MTOUCH_MODE7_ORDER5_INIT           
        #define MTOUCH_MODE7_ORDER5_INIT        
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS <  5        
        #undef  MTOUCH_MODE7_ORDER4_INIT           
        #define MTOUCH_MODE7_ORDER4_INIT        
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS <  4        
        #undef  MTOUCH_MODE7_ORDER3_INIT           
        #define MTOUCH_MODE7_ORDER3_INIT        
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS <  3        
        #undef  MTOUCH_MODE7_ORDER2_INIT           
        #define MTOUCH_MODE7_ORDER2_INIT        
    #endif
    #if MTOUCH_MODE7_NUM_SENSORS <  2        
        #undef  MTOUCH_MODE7_ORDER1_INIT           
        #define MTOUCH_MODE7_ORDER1_INIT        
    #endif
   
    #if MTOUCH_MODE7_NUM_SENSORS > 0
    #define MTOUCH_MODE7_VAR_INIT               {                             \
                                                    MTOUCH_MODE7_ORDER0_INIT  \
                                                    MTOUCH_MODE7_ORDER1_INIT  \
                                                    MTOUCH_MODE7_ORDER2_INIT  \
                                                    MTOUCH_MODE7_ORDER3_INIT  \
                                                    MTOUCH_MODE7_ORDER4_INIT  \
                                                    MTOUCH_MODE7_ORDER5_INIT  \
                                                    MTOUCH_MODE7_ORDER6_INIT  \
                                                    MTOUCH_MODE7_ORDER7_INIT  \
                                                    MTOUCH_MODE7_ORDER8_INIT  \
                                                    MTOUCH_MODE7_ORDER9_INIT  \
                                                    MTOUCH_MODE7_ORDER10_INIT \
                                                    MTOUCH_MODE7_ORDER11_INIT \
                                                    MTOUCH_MODE7_ORDER12_INIT \
                                                    MTOUCH_MODE7_ORDER13_INIT \
                                                    MTOUCH_MODE7_ORDER14_INIT \
                                                    MTOUCH_MODE7_ORDER15_INIT \
                                                    MTOUCH_MODE7_ORDER16_INIT \
                                                    MTOUCH_MODE7_ORDER17_INIT \
                                                    MTOUCH_MODE7_ORDER18_INIT \
                                                    MTOUCH_MODE7_ORDER19_INIT \
                                                    MTOUCH_MODE7_ORDER20_INIT \
                                                    MTOUCH_MODE7_ORDER21_INIT \
                                                    MTOUCH_MODE7_ORDER22_INIT \
                                                    MTOUCH_MODE7_ORDER23_INIT \
                                                    MTOUCH_MODE7_ORDER24_INIT \
                                                    MTOUCH_MODE7_ORDER25_INIT \
                                                    MTOUCH_MODE7_ORDER26_INIT \
                                                    MTOUCH_MODE7_ORDER27_INIT \
                                                    MTOUCH_MODE7_ORDER28_INIT \
                                                    MTOUCH_MODE7_ORDER29_INIT \
                                                    MTOUCH_MODE_SENSOR_END(0) \
                                                }    
    #endif
    #endif
    
#endif
#endif
