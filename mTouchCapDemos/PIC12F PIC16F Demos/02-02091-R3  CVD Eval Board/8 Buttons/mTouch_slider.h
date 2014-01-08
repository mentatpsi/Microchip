/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_slider.h
 *
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework Slider Include File
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
/** @file   mTouch_slider.h
*   @brief  Internal framework header file for generating the framework's
*           slider code. 
*/


#if defined(MTOUCH_NUMBER_OF_SLIDERS)
#if (MTOUCH_NUMBER_OF_SLIDERS > 0)

    #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
        #define MTOUCH_SLIDER_STRUCTINIT(i)     { __3paste(MTOUCH_NUMBER_SLIDER, i, _SENSORS) , __paste(MTOUCH_SLIDERS_ISWHEEL, i) }
    #else
        #define MTOUCH_SLIDER_STRUCTINIT(i)     { __3paste(MTOUCH_NUMBER_SLIDER, i, _SENSORS) }
    #endif
    
    #define MTOUCH_SLIDERS_ISWHEEL0     0
    #define MTOUCH_SLIDERS_ISWHEEL1     0
    #define MTOUCH_SLIDERS_ISWHEEL2     0
    #define MTOUCH_SLIDERS_ISWHEEL3     0
    #define MTOUCH_SLIDERS_ISWHEEL4     0
    #define MTOUCH_SLIDERS_ISWHEEL5     0
    #define MTOUCH_SLIDERS_ISWHEEL6     0
    #define MTOUCH_SLIDERS_ISWHEEL7     0
    #define MTOUCH_SLIDERS_ISWHEEL8     0
    #define MTOUCH_SLIDERS_ISWHEEL9     0
    #if defined(MTOUCH_NUMBER_SLIDER0_SENSORS)
        #if (MTOUCH_NUMBER_SLIDER0_SENSORS > 1)
            #if defined(MTOUCH_SLIDER0_WHEEL_ENABLE)
                #if !defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                #define MTOUCH_SLIDERS_WHEELS_IMPLEMENTED
                #endif
                #undef MTOUCH_SLIDERS_ISWHEEL0
                #define MTOUCH_SLIDERS_ISWHEEL0     1
            #endif        
            #if     MTOUCH_NUMBER_SLIDER0_SENSORS == 2
            #define MTOUCH_SLIDERINDEX_INIT0 {MTOUCH_SLIDER0_SENSOR0,MTOUCH_SLIDER0_SENSOR1}
            #elif   MTOUCH_NUMBER_SLIDER0_SENSORS == 3
            #define MTOUCH_SLIDERINDEX_INIT0 {MTOUCH_SLIDER0_SENSOR0,MTOUCH_SLIDER0_SENSOR1,MTOUCH_SLIDER0_SENSOR2}
                #if !defined(MTOUCH_SLIDERS_DIVIDE2_USED)
                #define MTOUCH_SLIDERS_DIVIDE2_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER0_SENSORS == 4
            #define MTOUCH_SLIDERINDEX_INIT0 {MTOUCH_SLIDER0_SENSOR0,MTOUCH_SLIDER0_SENSOR1,MTOUCH_SLIDER0_SENSOR2,MTOUCH_SLIDER0_SENSOR3}
                #if !defined(MTOUCH_SLIDERS_DIVIDE3_USED)
                #define MTOUCH_SLIDERS_DIVIDE3_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER0_SENSORS == 5
            #define MTOUCH_SLIDERINDEX_INIT0 {MTOUCH_SLIDER0_SENSOR0,MTOUCH_SLIDER0_SENSOR1,MTOUCH_SLIDER0_SENSOR2,MTOUCH_SLIDER0_SENSOR3,MTOUCH_SLIDER0_SENSOR4}
                #if !defined(MTOUCH_SLIDERS_DIVIDE4_USED)
                #define MTOUCH_SLIDERS_DIVIDE4_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER0_SENSORS == 6
            #define MTOUCH_SLIDERINDEX_INIT0 {MTOUCH_SLIDER0_SENSOR0,MTOUCH_SLIDER0_SENSOR1,MTOUCH_SLIDER0_SENSOR2,MTOUCH_SLIDER0_SENSOR3,MTOUCH_SLIDER0_SENSOR4,MTOUCH_SLIDER0_SENSOR5}
                #if !defined(MTOUCH_SLIDERS_DIVIDE5_USED)
                #define MTOUCH_SLIDERS_DIVIDE5_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER0_SENSORS == 7
            #define MTOUCH_SLIDERINDEX_INIT0 {MTOUCH_SLIDER0_SENSOR0,MTOUCH_SLIDER0_SENSOR1,MTOUCH_SLIDER0_SENSOR2,MTOUCH_SLIDER0_SENSOR3,MTOUCH_SLIDER0_SENSOR4,MTOUCH_SLIDER0_SENSOR5,MTOUCH_SLIDER0_SENSOR6}
                #if !defined(MTOUCH_SLIDERS_DIVIDE6_USED)
                #define MTOUCH_SLIDERS_DIVIDE6_USED
                #endif                                                                                                                                                                                              
            #elif   MTOUCH_NUMBER_SLIDER0_SENSORS == 8
            #define MTOUCH_SLIDERINDEX_INIT0 {MTOUCH_SLIDER0_SENSOR0,MTOUCH_SLIDER0_SENSOR1,MTOUCH_SLIDER0_SENSOR2,MTOUCH_SLIDER0_SENSOR3,MTOUCH_SLIDER0_SENSOR4,MTOUCH_SLIDER0_SENSOR5,MTOUCH_SLIDER0_SENSOR6,MTOUCH_SLIDER0_SENSOR7}
                #if !defined(MTOUCH_SLIDERS_DIVIDE7_USED)
                #define MTOUCH_SLIDERS_DIVIDE7_USED
                #endif
            #endif
        
            #if !defined(MTOUCH_SLIDER0_SENSOR0)
                #error Please define MTOUCH_SLIDER0_SENSOR0 to one of the sensor definitions. ex: MTOUCH_SENSOR0
            #endif
            #if !defined(MTOUCH_SLIDER0_SENSOR1)
                #error Please define MTOUCH_SLIDER0_SENSOR1 to one of the sensor definitions. ex: MTOUCH_SENSOR1
            #endif
            
            #if (MTOUCH_NUMBER_SLIDER0_SENSORS > 2)
            #if !defined(MTOUCH_SLIDER0_SENSOR2)
                #error Please define MTOUCH_SLIDER0_SENSOR2 to one of the sensor definitions. ex: MTOUCH_SENSOR2
            #endif   
            #endif
            #if (MTOUCH_NUMBER_SLIDER0_SENSORS > 3)
            #if !defined(MTOUCH_SLIDER0_SENSOR3)
                #error Please define MTOUCH_SLIDER0_SENSOR3 to one of the sensor definitions. ex: MTOUCH_SENSOR3
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER0_SENSORS > 4)
            #if !defined(MTOUCH_SLIDER0_SENSOR4)
                #error Please define MTOUCH_SLIDER0_SENSOR4 to one of the sensor definitions. ex: MTOUCH_SENSOR4
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER0_SENSORS > 5)
            #if !defined(MTOUCH_SLIDER0_SENSOR5)
                #error Please define MTOUCH_SLIDER0_SENSOR5 to one of the sensor definitions. ex: MTOUCH_SENSOR5
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER0_SENSORS > 6)
            #if !defined(MTOUCH_SLIDER0_SENSOR6)
                #error Please define MTOUCH_SLIDER0_SENSOR6 to one of the sensor definitions. ex: MTOUCH_SENSOR6
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER0_SENSORS > 7)
            #if !defined(MTOUCH_SLIDER0_SENSOR7)
                #error Please define MTOUCH_SLIDER0_SENSOR7 to one of the sensor definitions. ex: MTOUCH_SENSOR7
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER0_SENSORS > 8)
                #error Sliders can only consist of a maximum of 8 sensors. Please redefine MTOUCH_NUMBER_SLIDER0_SENSORS.
            #endif
        #else
            #error Please redefine MTOUCH_NUMBER_SLIDER0_SENSORS to a value of 2 or greater to implement a slider.
        #endif
    #else
        #error Please define MTOUCH_NUMBER_SLIDER0_SENSORS in the configuration file to equal the number of sensors used to implement Slider0.
    #endif
    
    #if (MTOUCH_NUMBER_OF_SLIDERS > 1)
    #if defined(MTOUCH_NUMBER_SLIDER1_SENSORS)
        #if (MTOUCH_NUMBER_SLIDER1_SENSORS > 1)
            #if defined(MTOUCH_SLIDER1_WHEEL_ENABLE)
                #if !defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                #define MTOUCH_SLIDERS_WHEELS_IMPLEMENTED
                #endif
                #undef MTOUCH_SLIDERS_ISWHEEL1
                #define MTOUCH_SLIDERS_ISWHEEL1     1
            #endif        
            #if     MTOUCH_NUMBER_SLIDER1_SENSORS == 2
            #define MTOUCH_SLIDERINDEX_INIT1 {MTOUCH_SLIDER1_SENSOR0,MTOUCH_SLIDER1_SENSOR1}
            #elif   MTOUCH_NUMBER_SLIDER1_SENSORS == 3
            #define MTOUCH_SLIDERINDEX_INIT1 {MTOUCH_SLIDER1_SENSOR0,MTOUCH_SLIDER1_SENSOR1,MTOUCH_SLIDER1_SENSOR2}
                #if !defined(MTOUCH_SLIDERS_DIVIDE2_USED)
                #define MTOUCH_SLIDERS_DIVIDE2_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER1_SENSORS == 4
            #define MTOUCH_SLIDERINDEX_INIT1 {MTOUCH_SLIDER1_SENSOR0,MTOUCH_SLIDER1_SENSOR1,MTOUCH_SLIDER1_SENSOR2,MTOUCH_SLIDER1_SENSOR3}
                #if !defined(MTOUCH_SLIDERS_DIVIDE3_USED)
                #define MTOUCH_SLIDERS_DIVIDE3_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER1_SENSORS == 5
            #define MTOUCH_SLIDERINDEX_INIT1 {MTOUCH_SLIDER1_SENSOR0,MTOUCH_SLIDER1_SENSOR1,MTOUCH_SLIDER1_SENSOR2,MTOUCH_SLIDER1_SENSOR3,MTOUCH_SLIDER1_SENSOR4}
                #if !defined(MTOUCH_SLIDERS_DIVIDE4_USED)
                #define MTOUCH_SLIDERS_DIVIDE4_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER1_SENSORS == 6
            #define MTOUCH_SLIDERINDEX_INIT1 {MTOUCH_SLIDER1_SENSOR0,MTOUCH_SLIDER1_SENSOR1,MTOUCH_SLIDER1_SENSOR2,MTOUCH_SLIDER1_SENSOR3,MTOUCH_SLIDER1_SENSOR4,MTOUCH_SLIDER1_SENSOR5}
                #if !defined(MTOUCH_SLIDERS_DIVIDE5_USED)
                #define MTOUCH_SLIDERS_DIVIDE5_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER1_SENSORS == 7
            #define MTOUCH_SLIDERINDEX_INIT1 {MTOUCH_SLIDER1_SENSOR0,MTOUCH_SLIDER1_SENSOR1,MTOUCH_SLIDER1_SENSOR2,MTOUCH_SLIDER1_SENSOR3,MTOUCH_SLIDER1_SENSOR4,MTOUCH_SLIDER1_SENSOR5,MTOUCH_SLIDER1_SENSOR6}
                #if !defined(MTOUCH_SLIDERS_DIVIDE6_USED)
                #define MTOUCH_SLIDERS_DIVIDE6_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER1_SENSORS == 8
            #define MTOUCH_SLIDERINDEX_INIT1 {MTOUCH_SLIDER1_SENSOR0,MTOUCH_SLIDER1_SENSOR1,MTOUCH_SLIDER1_SENSOR2,MTOUCH_SLIDER1_SENSOR3,MTOUCH_SLIDER1_SENSOR4,MTOUCH_SLIDER1_SENSOR5,MTOUCH_SLIDER1_SENSOR6,MTOUCH_SLIDER1_SENSOR7}
                #if !defined(MTOUCH_SLIDERS_DIVIDE7_USED)
                #define MTOUCH_SLIDERS_DIVIDE7_USED
                #endif
            #endif
        
            #if !defined(MTOUCH_SLIDER1_SENSOR0)
                #error Please define MTOUCH_SLIDER1_SENSOR0 to one of the sensor definitions. ex: MTOUCH_SENSOR0
            #endif
            #if !defined(MTOUCH_SLIDER1_SENSOR1)
                #error Please define MTOUCH_SLIDER1_SENSOR1 to one of the sensor definitions. ex: MTOUCH_SENSOR1
            #endif
            
            #if (MTOUCH_NUMBER_SLIDER1_SENSORS > 2)
            #if !defined(MTOUCH_SLIDER1_SENSOR2)
                #error Please define MTOUCH_SLIDER1_SENSOR2 to one of the sensor definitions. ex: MTOUCH_SENSOR2
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER1_SENSORS > 3)
            #if !defined(MTOUCH_SLIDER1_SENSOR3)
                #error Please define MTOUCH_SLIDER1_SENSOR3 to one of the sensor definitions. ex: MTOUCH_SENSOR3
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER1_SENSORS > 4)
            #if !defined(MTOUCH_SLIDER1_SENSOR4)
                #error Please define MTOUCH_SLIDER1_SENSOR4 to one of the sensor definitions. ex: MTOUCH_SENSOR4
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER1_SENSORS > 5)
            #if !defined(MTOUCH_SLIDER1_SENSOR5)
                #error Please define MTOUCH_SLIDER1_SENSOR5 to one of the sensor definitions. ex: MTOUCH_SENSOR5
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER1_SENSORS > 6)
            #if !defined(MTOUCH_SLIDER1_SENSOR6)
                #error Please define MTOUCH_SLIDER1_SENSOR6 to one of the sensor definitions. ex: MTOUCH_SENSOR6
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER1_SENSORS > 7)
            #if !defined(MTOUCH_SLIDER1_SENSOR7)
                #error Please define MTOUCH_SLIDER1_SENSOR7 to one of the sensor definitions. ex: MTOUCH_SENSOR7
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER1_SENSORS > 8)
                #error Sliders can only consist of a maximum of 8 sensors. Please redefine MTOUCH_NUMBER_SLIDER1_SENSORS.
            #endif
        #else
            #error Please redefine MTOUCH_NUMBER_SLIDER1_SENSORS to a value of 2 or greater to implement a slider.
        #endif
    #else
        #error Please define MTOUCH_NUMBER_SLIDER1_SENSORS in the configuration file to equal the number of sensors used to implement Slider1.
    #endif
    #endif
    
    #if (MTOUCH_NUMBER_OF_SLIDERS > 2)
    #if defined(MTOUCH_NUMBER_SLIDER2_SENSORS)
        #if (MTOUCH_NUMBER_SLIDER2_SENSORS > 1)
            #if defined(MTOUCH_SLIDER2_WHEEL_ENABLE)
                #if !defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                #define MTOUCH_SLIDERS_WHEELS_IMPLEMENTED
                #endif
                #undef MTOUCH_SLIDERS_ISWHEEL2
                #define MTOUCH_SLIDERS_ISWHEEL2     1
            #endif        
            #if     MTOUCH_NUMBER_SLIDER2_SENSORS == 2
            #define MTOUCH_SLIDERINDEX_INIT2 {MTOUCH_SLIDER2_SENSOR0,MTOUCH_SLIDER2_SENSOR1}
            #elif   MTOUCH_NUMBER_SLIDER2_SENSORS == 3
            #define MTOUCH_SLIDERINDEX_INIT2 {MTOUCH_SLIDER2_SENSOR0,MTOUCH_SLIDER2_SENSOR1,MTOUCH_SLIDER2_SENSOR2}
                #if !defined(MTOUCH_SLIDERS_DIVIDE2_USED)
                #define MTOUCH_SLIDERS_DIVIDE2_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER2_SENSORS == 4
            #define MTOUCH_SLIDERINDEX_INIT2 {MTOUCH_SLIDER2_SENSOR0,MTOUCH_SLIDER2_SENSOR1,MTOUCH_SLIDER2_SENSOR2,MTOUCH_SLIDER2_SENSOR3}
                #if !defined(MTOUCH_SLIDERS_DIVIDE3_USED)
                #define MTOUCH_SLIDERS_DIVIDE3_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER2_SENSORS == 5
            #define MTOUCH_SLIDERINDEX_INIT2 {MTOUCH_SLIDER2_SENSOR0,MTOUCH_SLIDER2_SENSOR1,MTOUCH_SLIDER2_SENSOR2,MTOUCH_SLIDER2_SENSOR3,MTOUCH_SLIDER2_SENSOR4}
                #if !defined(MTOUCH_SLIDERS_DIVIDE4_USED)
                #define MTOUCH_SLIDERS_DIVIDE4_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER2_SENSORS == 6
            #define MTOUCH_SLIDERINDEX_INIT2 {MTOUCH_SLIDER2_SENSOR0,MTOUCH_SLIDER2_SENSOR1,MTOUCH_SLIDER2_SENSOR2,MTOUCH_SLIDER2_SENSOR3,MTOUCH_SLIDER2_SENSOR4,MTOUCH_SLIDER2_SENSOR5}
                #if !defined(MTOUCH_SLIDERS_DIVIDE5_USED)
                #define MTOUCH_SLIDERS_DIVIDE5_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER2_SENSORS == 7
            #define MTOUCH_SLIDERINDEX_INIT2 {MTOUCH_SLIDER2_SENSOR0,MTOUCH_SLIDER2_SENSOR1,MTOUCH_SLIDER2_SENSOR2,MTOUCH_SLIDER2_SENSOR3,MTOUCH_SLIDER2_SENSOR4,MTOUCH_SLIDER2_SENSOR5,MTOUCH_SLIDER2_SENSOR6}
                #if !defined(MTOUCH_SLIDERS_DIVIDE6_USED)
                #define MTOUCH_SLIDERS_DIVIDE6_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER2_SENSORS == 8
            #define MTOUCH_SLIDERINDEX_INIT2 {MTOUCH_SLIDER2_SENSOR0,MTOUCH_SLIDER2_SENSOR1,MTOUCH_SLIDER2_SENSOR2,MTOUCH_SLIDER2_SENSOR3,MTOUCH_SLIDER2_SENSOR4,MTOUCH_SLIDER2_SENSOR5,MTOUCH_SLIDER2_SENSOR6,MTOUCH_SLIDER2_SENSOR7}
                #if !defined(MTOUCH_SLIDERS_DIVIDE7_USED)
                #define MTOUCH_SLIDERS_DIVIDE7_USED
                #endif
            #endif
        
            #if !defined(MTOUCH_SLIDER2_SENSOR0)
                #error Please define MTOUCH_SLIDER2_SENSOR0 to one of the sensor definitions. ex: MTOUCH_SENSOR0
            #endif
            #if !defined(MTOUCH_SLIDER2_SENSOR1)
                #error Please define MTOUCH_SLIDER2_SENSOR1 to one of the sensor definitions. ex: MTOUCH_SENSOR1
            #endif
            
            #if (MTOUCH_NUMBER_SLIDER2_SENSORS > 2)
            #if !defined(MTOUCH_SLIDER2_SENSOR2)
                #error Please define MTOUCH_SLIDER2_SENSOR2 to one of the sensor definitions. ex: MTOUCH_SENSOR2
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER2_SENSORS > 3)
            #if !defined(MTOUCH_SLIDER2_SENSOR3)
                #error Please define MTOUCH_SLIDER2_SENSOR3 to one of the sensor definitions. ex: MTOUCH_SENSOR3
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER2_SENSORS > 4)
            #if !defined(MTOUCH_SLIDER2_SENSOR4)
                #error Please define MTOUCH_SLIDER2_SENSOR4 to one of the sensor definitions. ex: MTOUCH_SENSOR4
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER2_SENSORS > 5)
            #if !defined(MTOUCH_SLIDER2_SENSOR5)
                #error Please define MTOUCH_SLIDER2_SENSOR5 to one of the sensor definitions. ex: MTOUCH_SENSOR5
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER2_SENSORS > 6)
            #if !defined(MTOUCH_SLIDER2_SENSOR6)
                #error Please define MTOUCH_SLIDER2_SENSOR6 to one of the sensor definitions. ex: MTOUCH_SENSOR6
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER2_SENSORS > 7)
            #if !defined(MTOUCH_SLIDER2_SENSOR7)
                #error Please define MTOUCH_SLIDER2_SENSOR7 to one of the sensor definitions. ex: MTOUCH_SENSOR7
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER2_SENSORS > 8)
                #error Sliders can only consist of a maximum of 8 sensors. Please redefine MTOUCH_NUMBER_SLIDER2_SENSORS.
            #endif
        #else
            #error Please redefine MTOUCH_NUMBER_SLIDER2_SENSORS to a value of 2 or greater to implement a slider.
        #endif
    #else
        #error Please define MTOUCH_NUMBER_SLIDER2_SENSORS in the configuration file to equal the number of sensors used to implement Slider2.
    #endif
    #endif
    
    #if (MTOUCH_NUMBER_OF_SLIDERS > 3)
    #if defined(MTOUCH_NUMBER_SLIDER3_SENSORS)
        #if (MTOUCH_NUMBER_SLIDER3_SENSORS > 1)
            #if defined(MTOUCH_SLIDER3_WHEEL_ENABLE)
                #if !defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                #define MTOUCH_SLIDERS_WHEELS_IMPLEMENTED
                #endif
                #undef MTOUCH_SLIDERS_ISWHEEL3
                #define MTOUCH_SLIDERS_ISWHEEL3     1
            #endif        
            #if     MTOUCH_NUMBER_SLIDER3_SENSORS == 2
            #define MTOUCH_SLIDERINDEX_INIT3 {MTOUCH_SLIDER3_SENSOR0,MTOUCH_SLIDER3_SENSOR1}
            #elif   MTOUCH_NUMBER_SLIDER3_SENSORS == 3
            #define MTOUCH_SLIDERINDEX_INIT3 {MTOUCH_SLIDER3_SENSOR0,MTOUCH_SLIDER3_SENSOR1,MTOUCH_SLIDER3_SENSOR2}
                #if !defined(MTOUCH_SLIDERS_DIVIDE2_USED)
                #define MTOUCH_SLIDERS_DIVIDE2_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER3_SENSORS == 4
            #define MTOUCH_SLIDERINDEX_INIT3 {MTOUCH_SLIDER3_SENSOR0,MTOUCH_SLIDER3_SENSOR1,MTOUCH_SLIDER3_SENSOR2,MTOUCH_SLIDER3_SENSOR3}
                #if !defined(MTOUCH_SLIDERS_DIVIDE3_USED)
                #define MTOUCH_SLIDERS_DIVIDE3_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER3_SENSORS == 5
            #define MTOUCH_SLIDERINDEX_INIT3 {MTOUCH_SLIDER3_SENSOR0,MTOUCH_SLIDER3_SENSOR1,MTOUCH_SLIDER3_SENSOR2,MTOUCH_SLIDER3_SENSOR3,MTOUCH_SLIDER3_SENSOR4}
                #if !defined(MTOUCH_SLIDERS_DIVIDE4_USED)
                #define MTOUCH_SLIDERS_DIVIDE4_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER3_SENSORS == 6
            #define MTOUCH_SLIDERINDEX_INIT3 {MTOUCH_SLIDER3_SENSOR0,MTOUCH_SLIDER3_SENSOR1,MTOUCH_SLIDER3_SENSOR2,MTOUCH_SLIDER3_SENSOR3,MTOUCH_SLIDER3_SENSOR4,MTOUCH_SLIDER3_SENSOR5}
                #if !defined(MTOUCH_SLIDERS_DIVIDE5_USED)
                #define MTOUCH_SLIDERS_DIVIDE5_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER3_SENSORS == 7
            #define MTOUCH_SLIDERINDEX_INIT3 {MTOUCH_SLIDER3_SENSOR0,MTOUCH_SLIDER3_SENSOR1,MTOUCH_SLIDER3_SENSOR2,MTOUCH_SLIDER3_SENSOR3,MTOUCH_SLIDER3_SENSOR4,MTOUCH_SLIDER3_SENSOR5,MTOUCH_SLIDER3_SENSOR6}
                #if !defined(MTOUCH_SLIDERS_DIVIDE6_USED)
                #define MTOUCH_SLIDERS_DIVIDE6_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER3_SENSORS == 8
            #define MTOUCH_SLIDERINDEX_INIT3 {MTOUCH_SLIDER3_SENSOR0,MTOUCH_SLIDER3_SENSOR1,MTOUCH_SLIDER3_SENSOR2,MTOUCH_SLIDER3_SENSOR3,MTOUCH_SLIDER3_SENSOR4,MTOUCH_SLIDER3_SENSOR5,MTOUCH_SLIDER3_SENSOR6,MTOUCH_SLIDER3_SENSOR7}
                #if !defined(MTOUCH_SLIDERS_DIVIDE7_USED)
                #define MTOUCH_SLIDERS_DIVIDE7_USED
                #endif
            #endif
            
            #if !defined(MTOUCH_SLIDER3_SENSOR0)
                #error Please define MTOUCH_SLIDER3_SENSOR0 to one of the sensor definitions. ex: MTOUCH_SENSOR0
            #endif
            #if !defined(MTOUCH_SLIDER3_SENSOR1)
                #error Please define MTOUCH_SLIDER3_SENSOR1 to one of the sensor definitions. ex: MTOUCH_SENSOR1
            #endif
            
            #if (MTOUCH_NUMBER_SLIDER3_SENSORS > 2)
            #if !defined(MTOUCH_SLIDER3_SENSOR2)
                #error Please define MTOUCH_SLIDER3_SENSOR2 to one of the sensor definitions. ex: MTOUCH_SENSOR2
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER3_SENSORS > 3)
            #if !defined(MTOUCH_SLIDER3_SENSOR3)
                #error Please define MTOUCH_SLIDER3_SENSOR3 to one of the sensor definitions. ex: MTOUCH_SENSOR3
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER3_SENSORS > 4)
            #if !defined(MTOUCH_SLIDER3_SENSOR4)
                #error Please define MTOUCH_SLIDER3_SENSOR4 to one of the sensor definitions. ex: MTOUCH_SENSOR4
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER3_SENSORS > 5)
            #if !defined(MTOUCH_SLIDER3_SENSOR5)
                #error Please define MTOUCH_SLIDER3_SENSOR5 to one of the sensor definitions. ex: MTOUCH_SENSOR5
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER3_SENSORS > 6)
            #if !defined(MTOUCH_SLIDER3_SENSOR6)
                #error Please define MTOUCH_SLIDER3_SENSOR6 to one of the sensor definitions. ex: MTOUCH_SENSOR6
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER3_SENSORS > 7)
            #if !defined(MTOUCH_SLIDER3_SENSOR7)
                #error Please define MTOUCH_SLIDER3_SENSOR7 to one of the sensor definitions. ex: MTOUCH_SENSOR7
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER3_SENSORS > 8)
                #error Sliders can only consist of a maximum of 8 sensors. Please redefine MTOUCH_NUMBER_SLIDER3_SENSORS.
            #endif
        #else
            #error Please redefine MTOUCH_NUMBER_SLIDER3_SENSORS to a value of 2 or greater to implement a slider.
        #endif
    #else
        #error Please define MTOUCH_NUMBER_SLIDER3_SENSORS in the configuration file to equal the number of sensors used to implement Slider3.
    #endif
    #endif
    
    #if (MTOUCH_NUMBER_OF_SLIDERS > 4)
    #if defined(MTOUCH_NUMBER_SLIDER4_SENSORS)
        #if (MTOUCH_NUMBER_SLIDER4_SENSORS > 1)
            #if defined(MTOUCH_SLIDER4_WHEEL_ENABLE)
                #if !defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                #define MTOUCH_SLIDERS_WHEELS_IMPLEMENTED
                #endif
                #undef MTOUCH_SLIDERS_ISWHEEL4
                #define MTOUCH_SLIDERS_ISWHEEL4     1
            #endif        
            #if     MTOUCH_NUMBER_SLIDER4_SENSORS == 2
            #define MTOUCH_SLIDERINDEX_INIT4 {MTOUCH_SLIDER4_SENSOR0,MTOUCH_SLIDER4_SENSOR1}
            #elif   MTOUCH_NUMBER_SLIDER4_SENSORS == 3
            #define MTOUCH_SLIDERINDEX_INIT4 {MTOUCH_SLIDER4_SENSOR0,MTOUCH_SLIDER4_SENSOR1,MTOUCH_SLIDER4_SENSOR2}
                #if !defined(MTOUCH_SLIDERS_DIVIDE2_USED)
                #define MTOUCH_SLIDERS_DIVIDE2_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER4_SENSORS == 4
            #define MTOUCH_SLIDERINDEX_INIT4 {MTOUCH_SLIDER4_SENSOR0,MTOUCH_SLIDER4_SENSOR1,MTOUCH_SLIDER4_SENSOR2,MTOUCH_SLIDER4_SENSOR3}
                #if !defined(MTOUCH_SLIDERS_DIVIDE3_USED)
                #define MTOUCH_SLIDERS_DIVIDE3_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER4_SENSORS == 5
            #define MTOUCH_SLIDERINDEX_INIT4 {MTOUCH_SLIDER4_SENSOR0,MTOUCH_SLIDER4_SENSOR1,MTOUCH_SLIDER4_SENSOR2,MTOUCH_SLIDER4_SENSOR3,MTOUCH_SLIDER4_SENSOR4}
                #if !defined(MTOUCH_SLIDERS_DIVIDE4_USED)
                #define MTOUCH_SLIDERS_DIVIDE4_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER4_SENSORS == 6
            #define MTOUCH_SLIDERINDEX_INIT4 {MTOUCH_SLIDER4_SENSOR0,MTOUCH_SLIDER4_SENSOR1,MTOUCH_SLIDER4_SENSOR2,MTOUCH_SLIDER4_SENSOR3,MTOUCH_SLIDER4_SENSOR4,MTOUCH_SLIDER4_SENSOR5}
                #if !defined(MTOUCH_SLIDERS_DIVIDE5_USED)
                #define MTOUCH_SLIDERS_DIVIDE5_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER4_SENSORS == 7
            #define MTOUCH_SLIDERINDEX_INIT4 {MTOUCH_SLIDER4_SENSOR0,MTOUCH_SLIDER4_SENSOR1,MTOUCH_SLIDER4_SENSOR2,MTOUCH_SLIDER4_SENSOR3,MTOUCH_SLIDER4_SENSOR4,MTOUCH_SLIDER4_SENSOR5,MTOUCH_SLIDER4_SENSOR6}
                #if !defined(MTOUCH_SLIDERS_DIVIDE6_USED)
                #define MTOUCH_SLIDERS_DIVIDE6_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER4_SENSORS == 8
            #define MTOUCH_SLIDERINDEX_INIT4 {MTOUCH_SLIDER4_SENSOR0,MTOUCH_SLIDER4_SENSOR1,MTOUCH_SLIDER4_SENSOR2,MTOUCH_SLIDER4_SENSOR3,MTOUCH_SLIDER4_SENSOR4,MTOUCH_SLIDER4_SENSOR5,MTOUCH_SLIDER4_SENSOR6,MTOUCH_SLIDER4_SENSOR7}
                #if !defined(MTOUCH_SLIDERS_DIVIDE7_USED)
                #define MTOUCH_SLIDERS_DIVIDE7_USED
                #endif
            #endif
            
            #if !defined(MTOUCH_SLIDER4_SENSOR0)
                #error Please define MTOUCH_SLIDER4_SENSOR0 to one of the sensor definitions. ex: MTOUCH_SENSOR0
            #endif
            #if !defined(MTOUCH_SLIDER4_SENSOR1)
                #error Please define MTOUCH_SLIDER4_SENSOR1 to one of the sensor definitions. ex: MTOUCH_SENSOR1
            #endif
            
            #if (MTOUCH_NUMBER_SLIDER4_SENSORS > 2)
            #if !defined(MTOUCH_SLIDER4_SENSOR2)
                #error Please define MTOUCH_SLIDER4_SENSOR2 to one of the sensor definitions. ex: MTOUCH_SENSOR2
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER4_SENSORS > 3)
            #if !defined(MTOUCH_SLIDER4_SENSOR3)
                #error Please define MTOUCH_SLIDER4_SENSOR3 to one of the sensor definitions. ex: MTOUCH_SENSOR3
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER4_SENSORS > 4)
            #if !defined(MTOUCH_SLIDER4_SENSOR4)
                #error Please define MTOUCH_SLIDER4_SENSOR4 to one of the sensor definitions. ex: MTOUCH_SENSOR4
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER4_SENSORS > 5)
            #if !defined(MTOUCH_SLIDER4_SENSOR5)
                #error Please define MTOUCH_SLIDER4_SENSOR5 to one of the sensor definitions. ex: MTOUCH_SENSOR5
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER4_SENSORS > 6)
            #if !defined(MTOUCH_SLIDER4_SENSOR6)
                #error Please define MTOUCH_SLIDER4_SENSOR6 to one of the sensor definitions. ex: MTOUCH_SENSOR6
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER4_SENSORS > 7)
            #if !defined(MTOUCH_SLIDER4_SENSOR7)
                #error Please define MTOUCH_SLIDER4_SENSOR7 to one of the sensor definitions. ex: MTOUCH_SENSOR7
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER4_SENSORS > 8)
                #error Sliders can only consist of a maximum of 8 sensors. Please redefine MTOUCH_NUMBER_SLIDER4_SENSORS.
            #endif
        #else
            #error Please redefine MTOUCH_NUMBER_SLIDER4_SENSORS to a value of 2 or greater to implement a slider.
        #endif
    #else
        #error Please define MTOUCH_NUMBER_SLIDER4_SENSORS in the configuration file to equal the number of sensors used to implement slider4.
    #endif
    #endif
    
    #if (MTOUCH_NUMBER_OF_SLIDERS > 5)
    #if defined(MTOUCH_NUMBER_SLIDER5_SENSORS)
        #if (MTOUCH_NUMBER_SLIDER5_SENSORS > 1)
            #if defined(MTOUCH_SLIDER5_WHEEL_ENABLE)
                #if !defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                #define MTOUCH_SLIDERS_WHEELS_IMPLEMENTED
                #endif
                #undef MTOUCH_SLIDERS_ISWHEEL5
                #define MTOUCH_SLIDERS_ISWHEEL5     1
            #endif        
            #if     MTOUCH_NUMBER_SLIDER5_SENSORS == 2
            #define MTOUCH_SLIDERINDEX_INIT5 {MTOUCH_SLIDER5_SENSOR0,MTOUCH_SLIDER5_SENSOR1}
            #elif   MTOUCH_NUMBER_SLIDER5_SENSORS == 3
            #define MTOUCH_SLIDERINDEX_INIT5 {MTOUCH_SLIDER5_SENSOR0,MTOUCH_SLIDER5_SENSOR1,MTOUCH_SLIDER5_SENSOR2}
                #if !defined(MTOUCH_SLIDERS_DIVIDE2_USED)
                #define MTOUCH_SLIDERS_DIVIDE2_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER5_SENSORS == 4
            #define MTOUCH_SLIDERINDEX_INIT5 {MTOUCH_SLIDER5_SENSOR0,MTOUCH_SLIDER5_SENSOR1,MTOUCH_SLIDER5_SENSOR2,MTOUCH_SLIDER5_SENSOR3}
                #if !defined(MTOUCH_SLIDERS_DIVIDE3_USED)
                #define MTOUCH_SLIDERS_DIVIDE3_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER5_SENSORS == 5
            #define MTOUCH_SLIDERINDEX_INIT5 {MTOUCH_SLIDER5_SENSOR0,MTOUCH_SLIDER5_SENSOR1,MTOUCH_SLIDER5_SENSOR2,MTOUCH_SLIDER5_SENSOR3,MTOUCH_SLIDER5_SENSOR4}
                #if !defined(MTOUCH_SLIDERS_DIVIDE4_USED)
                #define MTOUCH_SLIDERS_DIVIDE4_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER5_SENSORS == 6
            #define MTOUCH_SLIDERINDEX_INIT5 {MTOUCH_SLIDER5_SENSOR0,MTOUCH_SLIDER5_SENSOR1,MTOUCH_SLIDER5_SENSOR2,MTOUCH_SLIDER5_SENSOR3,MTOUCH_SLIDER5_SENSOR4,MTOUCH_SLIDER5_SENSOR5}
                #if !defined(MTOUCH_SLIDERS_DIVIDE5_USED)
                #define MTOUCH_SLIDERS_DIVIDE5_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER5_SENSORS == 7
            #define MTOUCH_SLIDERINDEX_INIT5 {MTOUCH_SLIDER5_SENSOR0,MTOUCH_SLIDER5_SENSOR1,MTOUCH_SLIDER5_SENSOR2,MTOUCH_SLIDER5_SENSOR3,MTOUCH_SLIDER5_SENSOR4,MTOUCH_SLIDER5_SENSOR5,MTOUCH_SLIDER5_SENSOR6}
                #if !defined(MTOUCH_SLIDERS_DIVIDE6_USED)
                #define MTOUCH_SLIDERS_DIVIDE6_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER5_SENSORS == 8
            #define MTOUCH_SLIDERINDEX_INIT5 {MTOUCH_SLIDER5_SENSOR0,MTOUCH_SLIDER5_SENSOR1,MTOUCH_SLIDER5_SENSOR2,MTOUCH_SLIDER5_SENSOR3,MTOUCH_SLIDER5_SENSOR4,MTOUCH_SLIDER5_SENSOR5,MTOUCH_SLIDER5_SENSOR6,MTOUCH_SLIDER5_SENSOR7}
                #if !defined(MTOUCH_SLIDERS_DIVIDE7_USED)
                #define MTOUCH_SLIDERS_DIVIDE7_USED
                #endif
            #endif
            
            #if !defined(MTOUCH_SLIDER5_SENSOR0)
                #error Please define MTOUCH_SLIDER5_SENSOR0 to one of the sensor definitions. ex: MTOUCH_SENSOR0
            #endif
            #if !defined(MTOUCH_SLIDER5_SENSOR1)
                #error Please define MTOUCH_SLIDER5_SENSOR1 to one of the sensor definitions. ex: MTOUCH_SENSOR1
            #endif
            
            #if (MTOUCH_NUMBER_SLIDER5_SENSORS > 2)
            #if !defined(MTOUCH_SLIDER5_SENSOR2)
                #error Please define MTOUCH_SLIDER5_SENSOR2 to one of the sensor definitions. ex: MTOUCH_SENSOR2
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER5_SENSORS > 3)
            #if !defined(MTOUCH_SLIDER5_SENSOR3)
                #error Please define MTOUCH_SLIDER5_SENSOR3 to one of the sensor definitions. ex: MTOUCH_SENSOR3
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER5_SENSORS > 4)
            #if !defined(MTOUCH_SLIDER5_SENSOR4)
                #error Please define MTOUCH_SLIDER5_SENSOR4 to one of the sensor definitions. ex: MTOUCH_SENSOR4
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER5_SENSORS > 5)
            #if !defined(MTOUCH_SLIDER5_SENSOR5)
                #error Please define MTOUCH_SLIDER5_SENSOR5 to one of the sensor definitions. ex: MTOUCH_SENSOR5
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER5_SENSORS > 6)
            #if !defined(MTOUCH_SLIDER5_SENSOR6)
                #error Please define MTOUCH_SLIDER5_SENSOR6 to one of the sensor definitions. ex: MTOUCH_SENSOR6
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER5_SENSORS > 7)
            #if !defined(MTOUCH_SLIDER5_SENSOR7)
                #error Please define MTOUCH_SLIDER5_SENSOR7 to one of the sensor definitions. ex: MTOUCH_SENSOR7
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER5_SENSORS > 8)
                #error Sliders can only consist of a maximum of 8 sensors. Please redefine MTOUCH_NUMBER_SLIDER5_SENSORS.
            #endif
        #else
            #error Please redefine MTOUCH_NUMBER_SLIDER5_SENSORS to a value of 2 or greater to implement a slider.
        #endif
    #else
        #error Please define MTOUCH_NUMBER_SLIDER5_SENSORS in the configuration file to equal the number of sensors used to implement Slider5.
    #endif
    #endif
    
    #if (MTOUCH_NUMBER_OF_SLIDERS > 6)
    #if defined(MTOUCH_NUMBER_SLIDER6_SENSORS)
        #if (MTOUCH_NUMBER_SLIDER6_SENSORS > 1)
            #if defined(MTOUCH_SLIDER6_WHEEL_ENABLE)
                #if !defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                #define MTOUCH_SLIDERS_WHEELS_IMPLEMENTED
                #endif
                #undef MTOUCH_SLIDERS_ISWHEEL6
                #define MTOUCH_SLIDERS_ISWHEEL6     1
            #endif        
            #if     MTOUCH_NUMBER_SLIDER6_SENSORS == 2
            #define MTOUCH_SLIDERINDEX_INIT6 {MTOUCH_SLIDER6_SENSOR0,MTOUCH_SLIDER6_SENSOR1}
            #elif   MTOUCH_NUMBER_SLIDER6_SENSORS == 3
            #define MTOUCH_SLIDERINDEX_INIT6 {MTOUCH_SLIDER6_SENSOR0,MTOUCH_SLIDER6_SENSOR1,MTOUCH_SLIDER6_SENSOR2}
                #if !defined(MTOUCH_SLIDERS_DIVIDE2_USED)
                #define MTOUCH_SLIDERS_DIVIDE2_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER6_SENSORS == 4
            #define MTOUCH_SLIDERINDEX_INIT6 {MTOUCH_SLIDER6_SENSOR0,MTOUCH_SLIDER6_SENSOR1,MTOUCH_SLIDER6_SENSOR2,MTOUCH_SLIDER6_SENSOR3}
                #if !defined(MTOUCH_SLIDERS_DIVIDE3_USED)
                #define MTOUCH_SLIDERS_DIVIDE3_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER6_SENSORS == 5
            #define MTOUCH_SLIDERINDEX_INIT6 {MTOUCH_SLIDER6_SENSOR0,MTOUCH_SLIDER6_SENSOR1,MTOUCH_SLIDER6_SENSOR2,MTOUCH_SLIDER6_SENSOR3,MTOUCH_SLIDER6_SENSOR4}
                #if !defined(MTOUCH_SLIDERS_DIVIDE4_USED)
                #define MTOUCH_SLIDERS_DIVIDE4_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER6_SENSORS == 6
            #define MTOUCH_SLIDERINDEX_INIT6 {MTOUCH_SLIDER6_SENSOR0,MTOUCH_SLIDER6_SENSOR1,MTOUCH_SLIDER6_SENSOR2,MTOUCH_SLIDER6_SENSOR3,MTOUCH_SLIDER6_SENSOR4,MTOUCH_SLIDER6_SENSOR5}
                #if !defined(MTOUCH_SLIDERS_DIVIDE5_USED)
                #define MTOUCH_SLIDERS_DIVIDE5_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER6_SENSORS == 7
            #define MTOUCH_SLIDERINDEX_INIT6 {MTOUCH_SLIDER6_SENSOR0,MTOUCH_SLIDER6_SENSOR1,MTOUCH_SLIDER6_SENSOR2,MTOUCH_SLIDER6_SENSOR3,MTOUCH_SLIDER6_SENSOR4,MTOUCH_SLIDER6_SENSOR5,MTOUCH_SLIDER6_SENSOR6}
                #if !defined(MTOUCH_SLIDERS_DIVIDE6_USED)
                #define MTOUCH_SLIDERS_DIVIDE6_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER6_SENSORS == 8
            #define MTOUCH_SLIDERINDEX_INIT6 {MTOUCH_SLIDER6_SENSOR0,MTOUCH_SLIDER6_SENSOR1,MTOUCH_SLIDER6_SENSOR2,MTOUCH_SLIDER6_SENSOR3,MTOUCH_SLIDER6_SENSOR4,MTOUCH_SLIDER6_SENSOR5,MTOUCH_SLIDER6_SENSOR6,MTOUCH_SLIDER6_SENSOR7}
                #if !defined(MTOUCH_SLIDERS_DIVIDE7_USED)
                #define MTOUCH_SLIDERS_DIVIDE7_USED
                #endif
            #endif
            
            #if !defined(MTOUCH_SLIDER6_SENSOR0)
                #error Please define MTOUCH_SLIDER6_SENSOR0 to one of the sensor definitions. ex: MTOUCH_SENSOR0
            #endif
            #if !defined(MTOUCH_SLIDER6_SENSOR1)
                #error Please define MTOUCH_SLIDER6_SENSOR1 to one of the sensor definitions. ex: MTOUCH_SENSOR1
            #endif
            
            #if (MTOUCH_NUMBER_SLIDER6_SENSORS > 2)
            #if !defined(MTOUCH_SLIDER6_SENSOR2)
                #error Please define MTOUCH_SLIDER6_SENSOR2 to one of the sensor definitions. ex: MTOUCH_SENSOR2
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER6_SENSORS > 3)
            #if !defined(MTOUCH_SLIDER6_SENSOR3)
                #error Please define MTOUCH_SLIDER6_SENSOR3 to one of the sensor definitions. ex: MTOUCH_SENSOR3
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER6_SENSORS > 4)
            #if !defined(MTOUCH_SLIDER6_SENSOR4)
                #error Please define MTOUCH_SLIDER6_SENSOR4 to one of the sensor definitions. ex: MTOUCH_SENSOR4
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER6_SENSORS > 5)
            #if !defined(MTOUCH_SLIDER6_SENSOR5)
                #error Please define MTOUCH_SLIDER6_SENSOR5 to one of the sensor definitions. ex: MTOUCH_SENSOR5
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER6_SENSORS > 6)
            #if !defined(MTOUCH_SLIDER6_SENSOR6)
                #error Please define MTOUCH_SLIDER6_SENSOR6 to one of the sensor definitions. ex: MTOUCH_SENSOR6
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER6_SENSORS > 7)
            #if !defined(MTOUCH_SLIDER6_SENSOR7)
                #error Please define MTOUCH_SLIDER6_SENSOR7 to one of the sensor definitions. ex: MTOUCH_SENSOR7
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER6_SENSORS > 8)
                #error Sliders can only consist of a maximum of 8 sensors. Please redefine MTOUCH_NUMBER_SLIDER6_SENSORS.
            #endif
        #else
            #error Please redefine MTOUCH_NUMBER_SLIDER6_SENSORS to a value of 2 or greater to implement a slider.
        #endif
    #else
        #error Please define MTOUCH_NUMBER_SLIDER6_SENSORS in the configuration file to equal the number of sensors used to implement Slider6.
    #endif
    #endif
    
    #if (MTOUCH_NUMBER_OF_SLIDERS > 7)
    #if defined(MTOUCH_NUMBER_SLIDER7_SENSORS)
        #if (MTOUCH_NUMBER_SLIDER7_SENSORS > 1)
            #if defined(MTOUCH_SLIDER7_WHEEL_ENABLE)
                #if !defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                #define MTOUCH_SLIDERS_WHEELS_IMPLEMENTED
                #endif
                #undef MTOUCH_SLIDERS_ISWHEEL7
                #define MTOUCH_SLIDERS_ISWHEEL7     1
            #endif        
            #if     MTOUCH_NUMBER_SLIDER7_SENSORS == 2
            #define MTOUCH_SLIDERINDEX_INIT7 {MTOUCH_SLIDER7_SENSOR0,MTOUCH_SLIDER7_SENSOR1}
            #elif   MTOUCH_NUMBER_SLIDER7_SENSORS == 3
            #define MTOUCH_SLIDERINDEX_INIT7 {MTOUCH_SLIDER7_SENSOR0,MTOUCH_SLIDER7_SENSOR1,MTOUCH_SLIDER7_SENSOR2}
                #if !defined(MTOUCH_SLIDERS_DIVIDE2_USED)
                #define MTOUCH_SLIDERS_DIVIDE2_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER7_SENSORS == 4
            #define MTOUCH_SLIDERINDEX_INIT7 {MTOUCH_SLIDER7_SENSOR0,MTOUCH_SLIDER7_SENSOR1,MTOUCH_SLIDER7_SENSOR2,MTOUCH_SLIDER7_SENSOR3}
                #if !defined(MTOUCH_SLIDERS_DIVIDE3_USED)
                #define MTOUCH_SLIDERS_DIVIDE3_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER7_SENSORS == 5
            #define MTOUCH_SLIDERINDEX_INIT7 {MTOUCH_SLIDER7_SENSOR0,MTOUCH_SLIDER7_SENSOR1,MTOUCH_SLIDER7_SENSOR2,MTOUCH_SLIDER7_SENSOR3,MTOUCH_SLIDER7_SENSOR4}
                #if !defined(MTOUCH_SLIDERS_DIVIDE4_USED)
                #define MTOUCH_SLIDERS_DIVIDE4_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER7_SENSORS == 6
            #define MTOUCH_SLIDERINDEX_INIT7 {MTOUCH_SLIDER7_SENSOR0,MTOUCH_SLIDER7_SENSOR1,MTOUCH_SLIDER7_SENSOR2,MTOUCH_SLIDER7_SENSOR3,MTOUCH_SLIDER7_SENSOR4,MTOUCH_SLIDER7_SENSOR5}
                #if !defined(MTOUCH_SLIDERS_DIVIDE5_USED)
                #define MTOUCH_SLIDERS_DIVIDE5_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER7_SENSORS == 7
            #define MTOUCH_SLIDERINDEX_INIT7 {MTOUCH_SLIDER7_SENSOR0,MTOUCH_SLIDER7_SENSOR1,MTOUCH_SLIDER7_SENSOR2,MTOUCH_SLIDER7_SENSOR3,MTOUCH_SLIDER7_SENSOR4,MTOUCH_SLIDER7_SENSOR5,MTOUCH_SLIDER7_SENSOR6}
                #if !defined(MTOUCH_SLIDERS_DIVIDE6_USED)
                #define MTOUCH_SLIDERS_DIVIDE6_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER7_SENSORS == 8
            #define MTOUCH_SLIDERINDEX_INIT7 {MTOUCH_SLIDER7_SENSOR0,MTOUCH_SLIDER7_SENSOR1,MTOUCH_SLIDER7_SENSOR2,MTOUCH_SLIDER7_SENSOR3,MTOUCH_SLIDER7_SENSOR4,MTOUCH_SLIDER7_SENSOR5,MTOUCH_SLIDER7_SENSOR6,MTOUCH_SLIDER7_SENSOR7}
                #if !defined(MTOUCH_SLIDERS_DIVIDE7_USED)
                #define MTOUCH_SLIDERS_DIVIDE7_USED
                #endif
            #endif
            
            #if !defined(MTOUCH_SLIDER7_SENSOR0)
                #error Please define MTOUCH_SLIDER7_SENSOR0 to one of the sensor definitions. ex: MTOUCH_SENSOR0
            #endif
            #if !defined(MTOUCH_SLIDER7_SENSOR1)
                #error Please define MTOUCH_SLIDER7_SENSOR1 to one of the sensor definitions. ex: MTOUCH_SENSOR1
            #endif
            
            #if (MTOUCH_NUMBER_SLIDER7_SENSORS > 2)
            #if !defined(MTOUCH_SLIDER7_SENSOR2)
                #error Please define MTOUCH_SLIDER7_SENSOR2 to one of the sensor definitions. ex: MTOUCH_SENSOR2
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER7_SENSORS > 3)
            #if !defined(MTOUCH_SLIDER7_SENSOR3)
                #error Please define MTOUCH_SLIDER7_SENSOR3 to one of the sensor definitions. ex: MTOUCH_SENSOR3
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER7_SENSORS > 4)
            #if !defined(MTOUCH_SLIDER7_SENSOR4)
                #error Please define MTOUCH_SLIDER7_SENSOR4 to one of the sensor definitions. ex: MTOUCH_SENSOR4
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER7_SENSORS > 5)
            #if !defined(MTOUCH_SLIDER7_SENSOR5)
                #error Please define MTOUCH_SLIDER7_SENSOR5 to one of the sensor definitions. ex: MTOUCH_SENSOR5
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER7_SENSORS > 6)
            #if !defined(MTOUCH_SLIDER7_SENSOR6)
                #error Please define MTOUCH_SLIDER7_SENSOR6 to one of the sensor definitions. ex: MTOUCH_SENSOR6
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER7_SENSORS > 7)
            #if !defined(MTOUCH_SLIDER7_SENSOR7)
                #error Please define MTOUCH_SLIDER7_SENSOR7 to one of the sensor definitions. ex: MTOUCH_SENSOR7
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER7_SENSORS > 8)
                #error Sliders can only consist of a maximum of 8 sensors. Please redefine MTOUCH_NUMBER_SLIDER7_SENSORS.
            #endif
        #else
            #error Please redefine MTOUCH_NUMBER_SLIDER7_SENSORS to a value of 2 or greater to implement a slider.
        #endif
    #else
        #error Please define MTOUCH_NUMBER_SLIDER7_SENSORS in the configuration file to equal the number of sensors used to implement Slider7.
    #endif
    #endif
    
    #if (MTOUCH_NUMBER_OF_SLIDERS > 8)
    #if defined(MTOUCH_NUMBER_SLIDER8_SENSORS)
        #if (MTOUCH_NUMBER_SLIDER8_SENSORS > 1)
            #if defined(MTOUCH_SLIDER8_WHEEL_ENABLE)
                #if !defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                #define MTOUCH_SLIDERS_WHEELS_IMPLEMENTED
                #endif
                #undef MTOUCH_SLIDERS_ISWHEEL8
                #define MTOUCH_SLIDERS_ISWHEEL8     1
            #endif        
            #if     MTOUCH_NUMBER_SLIDER8_SENSORS == 2
            #define MTOUCH_SLIDERINDEX_INIT8 {MTOUCH_SLIDER8_SENSOR0,MTOUCH_SLIDER8_SENSOR1}
            #elif   MTOUCH_NUMBER_SLIDER8_SENSORS == 3
            #define MTOUCH_SLIDERINDEX_INIT8 {MTOUCH_SLIDER8_SENSOR0,MTOUCH_SLIDER8_SENSOR1,MTOUCH_SLIDER8_SENSOR2}
                #if !defined(MTOUCH_SLIDERS_DIVIDE2_USED)
                #define MTOUCH_SLIDERS_DIVIDE2_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER8_SENSORS == 4
            #define MTOUCH_SLIDERINDEX_INIT8 {MTOUCH_SLIDER8_SENSOR0,MTOUCH_SLIDER8_SENSOR1,MTOUCH_SLIDER8_SENSOR2,MTOUCH_SLIDER8_SENSOR3}
                #if !defined(MTOUCH_SLIDERS_DIVIDE3_USED)
                #define MTOUCH_SLIDERS_DIVIDE3_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER8_SENSORS == 5
            #define MTOUCH_SLIDERINDEX_INIT8 {MTOUCH_SLIDER8_SENSOR0,MTOUCH_SLIDER8_SENSOR1,MTOUCH_SLIDER8_SENSOR2,MTOUCH_SLIDER8_SENSOR3,MTOUCH_SLIDER8_SENSOR4}
                #if !defined(MTOUCH_SLIDERS_DIVIDE4_USED)
                #define MTOUCH_SLIDERS_DIVIDE4_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER8_SENSORS == 6
            #define MTOUCH_SLIDERINDEX_INIT8 {MTOUCH_SLIDER8_SENSOR0,MTOUCH_SLIDER8_SENSOR1,MTOUCH_SLIDER8_SENSOR2,MTOUCH_SLIDER8_SENSOR3,MTOUCH_SLIDER8_SENSOR4,MTOUCH_SLIDER8_SENSOR5}
                #if !defined(MTOUCH_SLIDERS_DIVIDE5_USED)
                #define MTOUCH_SLIDERS_DIVIDE5_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER8_SENSORS == 7
            #define MTOUCH_SLIDERINDEX_INIT8 {MTOUCH_SLIDER8_SENSOR0,MTOUCH_SLIDER8_SENSOR1,MTOUCH_SLIDER8_SENSOR2,MTOUCH_SLIDER8_SENSOR3,MTOUCH_SLIDER8_SENSOR4,MTOUCH_SLIDER8_SENSOR5,MTOUCH_SLIDER8_SENSOR6}
                #if !defined(MTOUCH_SLIDERS_DIVIDE6_USED)
                #define MTOUCH_SLIDERS_DIVIDE6_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER8_SENSORS == 8
            #define MTOUCH_SLIDERINDEX_INIT8 {MTOUCH_SLIDER8_SENSOR0,MTOUCH_SLIDER8_SENSOR1,MTOUCH_SLIDER8_SENSOR2,MTOUCH_SLIDER8_SENSOR3,MTOUCH_SLIDER8_SENSOR4,MTOUCH_SLIDER8_SENSOR5,MTOUCH_SLIDER8_SENSOR6,MTOUCH_SLIDER8_SENSOR7}
                #if !defined(MTOUCH_SLIDERS_DIVIDE7_USED)
                #define MTOUCH_SLIDERS_DIVIDE7_USED
                #endif
            #endif
            
            #if !defined(MTOUCH_SLIDER8_SENSOR0)
                #error Please define MTOUCH_SLIDER8_SENSOR0 to one of the sensor definitions. ex: MTOUCH_SENSOR0
            #endif
            #if !defined(MTOUCH_SLIDER8_SENSOR1)
                #error Please define MTOUCH_SLIDER8_SENSOR1 to one of the sensor definitions. ex: MTOUCH_SENSOR1
            #endif
            
            #if (MTOUCH_NUMBER_SLIDER8_SENSORS > 2)
            #if !defined(MTOUCH_SLIDER8_SENSOR2)
                #error Please define MTOUCH_SLIDER8_SENSOR2 to one of the sensor definitions. ex: MTOUCH_SENSOR2
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER8_SENSORS > 3)
            #if !defined(MTOUCH_SLIDER8_SENSOR3)
                #error Please define MTOUCH_SLIDER8_SENSOR3 to one of the sensor definitions. ex: MTOUCH_SENSOR3
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER8_SENSORS > 4)
            #if !defined(MTOUCH_SLIDER8_SENSOR4)
                #error Please define MTOUCH_SLIDER8_SENSOR4 to one of the sensor definitions. ex: MTOUCH_SENSOR4
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER8_SENSORS > 5)
            #if !defined(MTOUCH_SLIDER8_SENSOR5)
                #error Please define MTOUCH_SLIDER8_SENSOR5 to one of the sensor definitions. ex: MTOUCH_SENSOR5
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER8_SENSORS > 6)
            #if !defined(MTOUCH_SLIDER8_SENSOR6)
                #error Please define MTOUCH_SLIDER8_SENSOR6 to one of the sensor definitions. ex: MTOUCH_SENSOR6
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER8_SENSORS > 7)
            #if !defined(MTOUCH_SLIDER8_SENSOR7)
                #error Please define MTOUCH_SLIDER8_SENSOR7 to one of the sensor definitions. ex: MTOUCH_SENSOR7
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER8_SENSORS > 8)
                #error Sliders can only consist of a maximum of 8 sensors. Please redefine MTOUCH_NUMBER_SLIDER8_SENSORS.
            #endif
        #else
            #error Please redefine MTOUCH_NUMBER_SLIDER8_SENSORS to a value of 2 or greater to implement a slider.
        #endif
    #else
        #error Please define MTOUCH_NUMBER_SLIDER8_SENSORS in the configuration file to equal the number of sensors used to implement Slider8.
    #endif
    #endif
    
    #if (MTOUCH_NUMBER_OF_SLIDERS > 9)
    #if defined(MTOUCH_NUMBER_SLIDER9_SENSORS)
        #if (MTOUCH_NUMBER_SLIDER9_SENSORS > 1)
            #if defined(MTOUCH_SLIDER9_WHEEL_ENABLE)
                #if !defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                #define MTOUCH_SLIDERS_WHEELS_IMPLEMENTED
                #endif
                #undef MTOUCH_SLIDERS_ISWHEEL9
                #define MTOUCH_SLIDERS_ISWHEEL9     1
            #endif        
            #if     MTOUCH_NUMBER_SLIDER9_SENSORS == 2
            #define MTOUCH_SLIDERINDEX_INIT9 {MTOUCH_SLIDER9_SENSOR0,MTOUCH_SLIDER9_SENSOR1}
            #elif   MTOUCH_NUMBER_SLIDER9_SENSORS == 3
            #define MTOUCH_SLIDERINDEX_INIT9 {MTOUCH_SLIDER9_SENSOR0,MTOUCH_SLIDER9_SENSOR1,MTOUCH_SLIDER9_SENSOR2}
                #if !defined(MTOUCH_SLIDERS_DIVIDE2_USED)
                #define MTOUCH_SLIDERS_DIVIDE2_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER9_SENSORS == 4
            #define MTOUCH_SLIDERINDEX_INIT9 {MTOUCH_SLIDER9_SENSOR0,MTOUCH_SLIDER9_SENSOR1,MTOUCH_SLIDER9_SENSOR2,MTOUCH_SLIDER9_SENSOR3}
                #if !defined(MTOUCH_SLIDERS_DIVIDE3_USED)
                #define MTOUCH_SLIDERS_DIVIDE3_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER9_SENSORS == 5
            #define MTOUCH_SLIDERINDEX_INIT9 {MTOUCH_SLIDER9_SENSOR0,MTOUCH_SLIDER9_SENSOR1,MTOUCH_SLIDER9_SENSOR2,MTOUCH_SLIDER9_SENSOR3,MTOUCH_SLIDER9_SENSOR4}
                #if !defined(MTOUCH_SLIDERS_DIVIDE4_USED)
                #define MTOUCH_SLIDERS_DIVIDE4_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER9_SENSORS == 6
            #define MTOUCH_SLIDERINDEX_INIT9 {MTOUCH_SLIDER9_SENSOR0,MTOUCH_SLIDER9_SENSOR1,MTOUCH_SLIDER9_SENSOR2,MTOUCH_SLIDER9_SENSOR3,MTOUCH_SLIDER9_SENSOR4,MTOUCH_SLIDER9_SENSOR5}
                #if !defined(MTOUCH_SLIDERS_DIVIDE5_USED)
                #define MTOUCH_SLIDERS_DIVIDE5_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER9_SENSORS == 7
            #define MTOUCH_SLIDERINDEX_INIT9 {MTOUCH_SLIDER9_SENSOR0,MTOUCH_SLIDER9_SENSOR1,MTOUCH_SLIDER9_SENSOR2,MTOUCH_SLIDER9_SENSOR3,MTOUCH_SLIDER9_SENSOR4,MTOUCH_SLIDER9_SENSOR5,MTOUCH_SLIDER9_SENSOR6}
                #if !defined(MTOUCH_SLIDERS_DIVIDE6_USED)
                #define MTOUCH_SLIDERS_DIVIDE6_USED
                #endif
            #elif   MTOUCH_NUMBER_SLIDER9_SENSORS == 8
            #define MTOUCH_SLIDERINDEX_INIT9 {MTOUCH_SLIDER9_SENSOR0,MTOUCH_SLIDER9_SENSOR1,MTOUCH_SLIDER9_SENSOR2,MTOUCH_SLIDER9_SENSOR3,MTOUCH_SLIDER9_SENSOR4,MTOUCH_SLIDER9_SENSOR5,MTOUCH_SLIDER9_SENSOR6,MTOUCH_SLIDER9_SENSOR7}
                #if !defined(MTOUCH_SLIDERS_DIVIDE7_USED)
                #define MTOUCH_SLIDERS_DIVIDE7_USED
                #endif
            #endif
            
            #if !defined(MTOUCH_SLIDER9_SENSOR0)
                #error Please define MTOUCH_SLIDER9_SENSOR0 to one of the sensor definitions. ex: MTOUCH_SENSOR0
            #endif
            #if !defined(MTOUCH_SLIDER9_SENSOR1)
                #error Please define MTOUCH_SLIDER9_SENSOR1 to one of the sensor definitions. ex: MTOUCH_SENSOR1
            #endif
            
            #if (MTOUCH_NUMBER_SLIDER9_SENSORS > 2)
            #if !defined(MTOUCH_SLIDER9_SENSOR2)
                #error Please define MTOUCH_SLIDER9_SENSOR2 to one of the sensor definitions. ex: MTOUCH_SENSOR2
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER9_SENSORS > 3)
            #if !defined(MTOUCH_SLIDER9_SENSOR3)
                #error Please define MTOUCH_SLIDER9_SENSOR3 to one of the sensor definitions. ex: MTOUCH_SENSOR3
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER9_SENSORS > 4)
            #if !defined(MTOUCH_SLIDER9_SENSOR4)
                #error Please define MTOUCH_SLIDER9_SENSOR4 to one of the sensor definitions. ex: MTOUCH_SENSOR4
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER9_SENSORS > 5)
            #if !defined(MTOUCH_SLIDER9_SENSOR5)
                #error Please define MTOUCH_SLIDER9_SENSOR5 to one of the sensor definitions. ex: MTOUCH_SENSOR5
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER9_SENSORS > 6)
            #if !defined(MTOUCH_SLIDER9_SENSOR6)
                #error Please define MTOUCH_SLIDER9_SENSOR6 to one of the sensor definitions. ex: MTOUCH_SENSOR6
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER9_SENSORS > 7)
            #if !defined(MTOUCH_SLIDER9_SENSOR7)
                #error Please define MTOUCH_SLIDER9_SENSOR7 to one of the sensor definitions. ex: MTOUCH_SENSOR7
            #endif
            #endif
            #if (MTOUCH_NUMBER_SLIDER9_SENSORS > 8)
                #error Sliders can only consist of a maximum of 8 sensors. Please redefine MTOUCH_NUMBER_SLIDER9_SENSORS.
            #endif
        #else
            #error Please redefine MTOUCH_NUMBER_SLIDER9_SENSORS to a value of 2 or greater to implement a slider.
        #endif
    #else
        #error Please define MTOUCH_NUMBER_SLIDER9_SENSORS in the configuration file to equal the number of sensors used to implement Slider9.
    #endif
    #endif
    
    #if (MTOUCH_NUMBER_OF_SLIDERS > 10)
        #error The mTouch Framework is only able to support up to 10 sliders at this time. Please redefine MTOUCH_NUMBER_OF_SLIDERS.
    #endif
    
    #if     (MTOUCH_NUMBER_OF_SLIDERS == 1)
        #define MTOUCH_SLIDER_STRUCT_INIT { MTOUCH_SLIDER_STRUCTINIT(0) }
        #define MTOUCH_SLIDERINDEX_INIT {MTOUCH_SLIDERINDEX_INIT0}
        #define MTOUCH_SLIDER_SENSORCOUNT_INIT {MTOUCH_NUMBER_SLIDER0_SENSORS}
        #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
        #define MTOUCH_SLIDER_ISWHEEL_INIT {MTOUCH_SLIDERS_ISWHEEL0}
        #endif
    #elif   (MTOUCH_NUMBER_OF_SLIDERS == 2)
        #define MTOUCH_SLIDER_STRUCT_INIT { MTOUCH_SLIDER_STRUCTINIT(0), MTOUCH_SLIDER_STRUCTINIT(1) }
        #define MTOUCH_SLIDERINDEX_INIT {MTOUCH_SLIDERINDEX_INIT0,MTOUCH_SLIDERINDEX_INIT1}
        #define MTOUCH_SLIDER_SENSORCOUNT_INIT {MTOUCH_NUMBER_SLIDER0_SENSORS,MTOUCH_NUMBER_SLIDER1_SENSORS}
        #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
        #define MTOUCH_SLIDER_ISWHEEL_INIT {MTOUCH_SLIDERS_ISWHEEL0,MTOUCH_SLIDERS_ISWHEEL1}
        #endif
    #elif   (MTOUCH_NUMBER_OF_SLIDERS == 3)
        #define MTOUCH_SLIDER_STRUCT_INIT { MTOUCH_SLIDER_STRUCTINIT(0), MTOUCH_SLIDER_STRUCTINIT(1), MTOUCH_SLIDER_STRUCTINIT(2) }
        #define MTOUCH_SLIDERINDEX_INIT {MTOUCH_SLIDERINDEX_INIT0,MTOUCH_SLIDERINDEX_INIT1,MTOUCH_SLIDERINDEX_INIT2}
        #define MTOUCH_SLIDER_SENSORCOUNT_INIT {MTOUCH_NUMBER_SLIDER0_SENSORS,MTOUCH_NUMBER_SLIDER1_SENSORS,MTOUCH_NUMBER_SLIDER2_SENSORS}
        #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
        #define MTOUCH_SLIDER_ISWHEEL_INIT {MTOUCH_SLIDERS_ISWHEEL0,MTOUCH_SLIDERS_ISWHEEL1,MTOUCH_SLIDERS_ISWHEEL2}
        #endif
    #elif   (MTOUCH_NUMBER_OF_SLIDERS == 4)
        #define MTOUCH_SLIDER_STRUCT_INIT { MTOUCH_SLIDER_STRUCTINIT(0), MTOUCH_SLIDER_STRUCTINIT(1), MTOUCH_SLIDER_STRUCTINIT(2), MTOUCH_SLIDER_STRUCTINIT(3) }
        #define MTOUCH_SLIDERINDEX_INIT {MTOUCH_SLIDERINDEX_INIT0,MTOUCH_SLIDERINDEX_INIT1,MTOUCH_SLIDERINDEX_INIT2,MTOUCH_SLIDERINDEX_INIT3}
        #define MTOUCH_SLIDER_SENSORCOUNT_INIT {MTOUCH_NUMBER_SLIDER0_SENSORS,MTOUCH_NUMBER_SLIDER1_SENSORS,MTOUCH_NUMBER_SLIDER2_SENSORS,MTOUCH_NUMBER_SLIDER3_SENSORS}
        #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
        #define MTOUCH_SLIDER_ISWHEEL_INIT {MTOUCH_SLIDERS_ISWHEEL0,MTOUCH_SLIDERS_ISWHEEL1,MTOUCH_SLIDERS_ISWHEEL2,MTOUCH_SLIDERS_ISWHEEL3}
        #endif
    #elif   (MTOUCH_NUMBER_OF_SLIDERS == 5)
        #define MTOUCH_SLIDER_STRUCT_INIT { MTOUCH_SLIDER_STRUCTINIT(0), MTOUCH_SLIDER_STRUCTINIT(1), MTOUCH_SLIDER_STRUCTINIT(2), MTOUCH_SLIDER_STRUCTINIT(3), MTOUCH_SLIDER_STRUCTINIT(4) }
        #define MTOUCH_SLIDERINDEX_INIT {MTOUCH_SLIDERINDEX_INIT0,MTOUCH_SLIDERINDEX_INIT1,MTOUCH_SLIDERINDEX_INIT2,MTOUCH_SLIDERINDEX_INIT3,MTOUCH_SLIDERINDEX_INIT4}
        #define MTOUCH_SLIDER_SENSORCOUNT_INIT {MTOUCH_NUMBER_SLIDER0_SENSORS,MTOUCH_NUMBER_SLIDER1_SENSORS,MTOUCH_NUMBER_SLIDER2_SENSORS,MTOUCH_NUMBER_SLIDER3_SENSORS,MTOUCH_NUMBER_SLIDER4_SENSORS}
        #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
        #define MTOUCH_SLIDER_ISWHEEL_INIT {MTOUCH_SLIDERS_ISWHEEL0,MTOUCH_SLIDERS_ISWHEEL1,MTOUCH_SLIDERS_ISWHEEL2,MTOUCH_SLIDERS_ISWHEEL3,MTOUCH_SLIDERS_ISWHEEL4}
        #endif
    #elif   (MTOUCH_NUMBER_OF_SLIDERS == 6)
        #define MTOUCH_SLIDER_STRUCT_INIT { MTOUCH_SLIDER_STRUCTINIT(0), MTOUCH_SLIDER_STRUCTINIT(1), MTOUCH_SLIDER_STRUCTINIT(2), MTOUCH_SLIDER_STRUCTINIT(3), MTOUCH_SLIDER_STRUCTINIT(4), MTOUCH_SLIDER_STRUCTINIT(5) }
        #define MTOUCH_SLIDERINDEX_INIT {MTOUCH_SLIDERINDEX_INIT0,MTOUCH_SLIDERINDEX_INIT1,MTOUCH_SLIDERINDEX_INIT2,MTOUCH_SLIDERINDEX_INIT3,MTOUCH_SLIDERINDEX_INIT4,MTOUCH_SLIDERINDEX_INIT5}
        #define MTOUCH_SLIDER_SENSORCOUNT_INIT {MTOUCH_NUMBER_SLIDER0_SENSORS,MTOUCH_NUMBER_SLIDER1_SENSORS,MTOUCH_NUMBER_SLIDER2_SENSORS,MTOUCH_NUMBER_SLIDER3_SENSORS,MTOUCH_NUMBER_SLIDER4_SENSORS,MTOUCH_NUMBER_SLIDER5_SENSORS}
        #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
        #define MTOUCH_SLIDER_ISWHEEL_INIT {MTOUCH_SLIDERS_ISWHEEL0,MTOUCH_SLIDERS_ISWHEEL1,MTOUCH_SLIDERS_ISWHEEL2,MTOUCH_SLIDERS_ISWHEEL3,MTOUCH_SLIDERS_ISWHEEL4,MTOUCH_SLIDERS_ISWHEEL5}
        #endif
    #elif   (MTOUCH_NUMBER_OF_SLIDERS == 7)
        #define MTOUCH_SLIDER_STRUCT_INIT { MTOUCH_SLIDER_STRUCTINIT(0), MTOUCH_SLIDER_STRUCTINIT(1), MTOUCH_SLIDER_STRUCTINIT(2), MTOUCH_SLIDER_STRUCTINIT(3), MTOUCH_SLIDER_STRUCTINIT(4), MTOUCH_SLIDER_STRUCTINIT(5), MTOUCH_SLIDER_STRUCTINIT(6) }
        #define MTOUCH_SLIDERINDEX_INIT {MTOUCH_SLIDERINDEX_INIT0,MTOUCH_SLIDERINDEX_INIT1,MTOUCH_SLIDERINDEX_INIT2,MTOUCH_SLIDERINDEX_INIT3,MTOUCH_SLIDERINDEX_INIT4,MTOUCH_SLIDERINDEX_INIT5,MTOUCH_SLIDERINDEX_INIT6}
        #define MTOUCH_SLIDER_SENSORCOUNT_INIT {MTOUCH_NUMBER_SLIDER0_SENSORS,MTOUCH_NUMBER_SLIDER1_SENSORS,MTOUCH_NUMBER_SLIDER2_SENSORS,MTOUCH_NUMBER_SLIDER3_SENSORS,MTOUCH_NUMBER_SLIDER4_SENSORS,MTOUCH_NUMBER_SLIDER5_SENSORS,MTOUCH_NUMBER_SLIDER6_SENSORS}
        #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
        #define MTOUCH_SLIDER_ISWHEEL_INIT {MTOUCH_SLIDERS_ISWHEEL0,MTOUCH_SLIDERS_ISWHEEL1,MTOUCH_SLIDERS_ISWHEEL2,MTOUCH_SLIDERS_ISWHEEL3,MTOUCH_SLIDERS_ISWHEEL4,MTOUCH_SLIDERS_ISWHEEL5,MTOUCH_SLIDERS_ISWHEEL6}
        #endif
    #elif   (MTOUCH_NUMBER_OF_SLIDERS == 8)
        #define MTOUCH_SLIDER_STRUCT_INIT { MTOUCH_SLIDER_STRUCTINIT(0), MTOUCH_SLIDER_STRUCTINIT(1), MTOUCH_SLIDER_STRUCTINIT(2), MTOUCH_SLIDER_STRUCTINIT(3), MTOUCH_SLIDER_STRUCTINIT(4), MTOUCH_SLIDER_STRUCTINIT(5), MTOUCH_SLIDER_STRUCTINIT(6), MTOUCH_SLIDER_STRUCTINIT(7) }
        #define MTOUCH_SLIDERINDEX_INIT {MTOUCH_SLIDERINDEX_INIT0,MTOUCH_SLIDERINDEX_INIT1,MTOUCH_SLIDERINDEX_INIT2,MTOUCH_SLIDERINDEX_INIT3,MTOUCH_SLIDERINDEX_INIT4,MTOUCH_SLIDERINDEX_INIT5,MTOUCH_SLIDERINDEX_INIT6,MTOUCH_SLIDERINDEX_INIT7}
        #define MTOUCH_SLIDER_SENSORCOUNT_INIT {MTOUCH_NUMBER_SLIDER0_SENSORS,MTOUCH_NUMBER_SLIDER1_SENSORS,MTOUCH_NUMBER_SLIDER2_SENSORS,MTOUCH_NUMBER_SLIDER3_SENSORS,MTOUCH_NUMBER_SLIDER4_SENSORS,MTOUCH_NUMBER_SLIDER5_SENSORS,MTOUCH_NUMBER_SLIDER6_SENSORS,MTOUCH_NUMBER_SLIDER7_SENSORS}
        #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
        #define MTOUCH_SLIDER_ISWHEEL_INIT {MTOUCH_SLIDERS_ISWHEEL0,MTOUCH_SLIDERS_ISWHEEL1,MTOUCH_SLIDERS_ISWHEEL2,MTOUCH_SLIDERS_ISWHEEL3,MTOUCH_SLIDERS_ISWHEEL4,MTOUCH_SLIDERS_ISWHEEL5,MTOUCH_SLIDERS_ISWHEEL6,MTOUCH_SLIDERS_ISWHEEL7}
        #endif
    #elif   (MTOUCH_NUMBER_OF_SLIDERS == 9)
        #define MTOUCH_SLIDER_STRUCT_INIT { MTOUCH_SLIDER_STRUCTINIT(0), MTOUCH_SLIDER_STRUCTINIT(1), MTOUCH_SLIDER_STRUCTINIT(2), MTOUCH_SLIDER_STRUCTINIT(3), MTOUCH_SLIDER_STRUCTINIT(4), MTOUCH_SLIDER_STRUCTINIT(5), MTOUCH_SLIDER_STRUCTINIT(6), MTOUCH_SLIDER_STRUCTINIT(7), MTOUCH_SLIDER_STRUCTINIT(8) }
        #define MTOUCH_SLIDERINDEX_INIT {MTOUCH_SLIDERINDEX_INIT0,MTOUCH_SLIDERINDEX_INIT1,MTOUCH_SLIDERINDEX_INIT2,MTOUCH_SLIDERINDEX_INIT3,MTOUCH_SLIDERINDEX_INIT4,MTOUCH_SLIDERINDEX_INIT5,MTOUCH_SLIDERINDEX_INIT6,MTOUCH_SLIDERINDEX_INIT7,MTOUCH_SLIDERINDEX_INIT8}
        #define MTOUCH_SLIDER_SENSORCOUNT_INIT {MTOUCH_NUMBER_SLIDER0_SENSORS,MTOUCH_NUMBER_SLIDER1_SENSORS,MTOUCH_NUMBER_SLIDER2_SENSORS,MTOUCH_NUMBER_SLIDER3_SENSORS,MTOUCH_NUMBER_SLIDER4_SENSORS,MTOUCH_NUMBER_SLIDER5_SENSORS,MTOUCH_NUMBER_SLIDER6_SENSORS,MTOUCH_NUMBER_SLIDER7_SENSORS,MTOUCH_NUMBER_SLIDER8_SENSORS}
        #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
        #define MTOUCH_SLIDER_ISWHEEL_INIT {MTOUCH_SLIDERS_ISWHEEL0,MTOUCH_SLIDERS_ISWHEEL1,MTOUCH_SLIDERS_ISWHEEL2,MTOUCH_SLIDERS_ISWHEEL3,MTOUCH_SLIDERS_ISWHEEL4,MTOUCH_SLIDERS_ISWHEEL5,MTOUCH_SLIDERS_ISWHEEL6,MTOUCH_SLIDERS_ISWHEEL7,MTOUCH_SLIDERS_ISWHEEL8}
        #endif
    #elif   (MTOUCH_NUMBER_OF_SLIDERS == 10)
        #define MTOUCH_SLIDER_STRUCT_INIT { MTOUCH_SLIDER_STRUCTINIT(0), MTOUCH_SLIDER_STRUCTINIT(1), MTOUCH_SLIDER_STRUCTINIT(2), MTOUCH_SLIDER_STRUCTINIT(3), MTOUCH_SLIDER_STRUCTINIT(4), MTOUCH_SLIDER_STRUCTINIT(5), MTOUCH_SLIDER_STRUCTINIT(6), MTOUCH_SLIDER_STRUCTINIT(7), MTOUCH_SLIDER_STRUCTINIT(8), MTOUCH_SLIDER_STRUCTINIT(9) }
        #define MTOUCH_SLIDERINDEX_INIT {MTOUCH_SLIDERINDEX_INIT0,MTOUCH_SLIDERINDEX_INIT1,MTOUCH_SLIDERINDEX_INIT2,MTOUCH_SLIDERINDEX_INIT3,MTOUCH_SLIDERINDEX_INIT4,MTOUCH_SLIDERINDEX_INIT5,MTOUCH_SLIDERINDEX_INIT6,MTOUCH_SLIDERINDEX_INIT7,MTOUCH_SLIDERINDEX_INIT8,MTOUCH_SLIDERINDEX_INIT9}
        #define MTOUCH_SLIDER_SENSORCOUNT_INIT {MTOUCH_NUMBER_SLIDER0_SENSORS,MTOUCH_NUMBER_SLIDER1_SENSORS,MTOUCH_NUMBER_SLIDER2_SENSORS,MTOUCH_NUMBER_SLIDER3_SENSORS,MTOUCH_NUMBER_SLIDER4_SENSORS,MTOUCH_NUMBER_SLIDER5_SENSORS,MTOUCH_NUMBER_SLIDER6_SENSORS,MTOUCH_NUMBER_SLIDER7_SENSORS,MTOUCH_NUMBER_SLIDER8_SENSORS,MTOUCH_NUMBER_SLIDER9_SENSORS}
        #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
        #define MTOUCH_SLIDER_ISWHEEL_INIT {MTOUCH_SLIDERS_ISWHEEL0,MTOUCH_SLIDERS_ISWHEEL1,MTOUCH_SLIDERS_ISWHEEL2,MTOUCH_SLIDERS_ISWHEEL3,MTOUCH_SLIDERS_ISWHEEL4,MTOUCH_SLIDERS_ISWHEEL5,MTOUCH_SLIDERS_ISWHEEL6,MTOUCH_SLIDERS_ISWHEEL7,MTOUCH_SLIDERS_ISWHEEL8,MTOUCH_SLIDERS_ISWHEEL9}
        #endif
    #endif
    
    #define MTOUCH_SLIDERS_MAX_SENSORS  MTOUCH_NUMBER_SLIDER0_SENSORS
    #if (MTOUCH_NUMBER_OF_SLIDERS > 1)
        #if  MTOUCH_SLIDERS_MAX_SENSORS < MTOUCH_NUMBER_SLIDER1_SENSORS
            #undef MTOUCH_SLIDERS_MAX_SENSORS
            #define MTOUCH_SLIDERS_MAX_SENSORS MTOUCH_NUMBER_SLIDER1_SENSORS
        #endif
    #endif
    #if (MTOUCH_NUMBER_OF_SLIDERS > 2)
        #if  MTOUCH_SLIDERS_MAX_SENSORS < MTOUCH_NUMBER_SLIDER2_SENSORS
            #undef MTOUCH_SLIDERS_MAX_SENSORS
            #define MTOUCH_SLIDERS_MAX_SENSORS MTOUCH_NUMBER_SLIDER2_SENSORS
        #endif
    #endif
    #if (MTOUCH_NUMBER_OF_SLIDERS > 3)
        #if  MTOUCH_SLIDERS_MAX_SENSORS < MTOUCH_NUMBER_SLIDER3_SENSORS
            #undef MTOUCH_SLIDERS_MAX_SENSORS
            #define MTOUCH_SLIDERS_MAX_SENSORS MTOUCH_NUMBER_SLIDER3_SENSORS
        #endif
    #endif
    #if (MTOUCH_NUMBER_OF_SLIDERS > 4)
        #if  MTOUCH_SLIDERS_MAX_SENSORS < MTOUCH_NUMBER_SLIDER4_SENSORS
            #undef MTOUCH_SLIDERS_MAX_SENSORS
            #define MTOUCH_SLIDERS_MAX_SENSORS MTOUCH_NUMBER_SLIDER4_SENSORS
        #endif
    #endif
    #if (MTOUCH_NUMBER_OF_SLIDERS > 5)
        #if  MTOUCH_SLIDERS_MAX_SENSORS < MTOUCH_NUMBER_SLIDER5_SENSORS
            #undef MTOUCH_SLIDERS_MAX_SENSORS
            #define MTOUCH_SLIDERS_MAX_SENSORS MTOUCH_NUMBER_SLIDER5_SENSORS
        #endif
    #endif
    #if (MTOUCH_NUMBER_OF_SLIDERS > 6)
        #if  MTOUCH_SLIDERS_MAX_SENSORS < MTOUCH_NUMBER_SLIDER6_SENSORS
            #undef MTOUCH_SLIDERS_MAX_SENSORS
            #define MTOUCH_SLIDERS_MAX_SENSORS MTOUCH_NUMBER_SLIDER6_SENSORS
        #endif
    #endif
    #if (MTOUCH_NUMBER_OF_SLIDERS > 7)
        #if  MTOUCH_SLIDERS_MAX_SENSORS < MTOUCH_NUMBER_SLIDER7_SENSORS
            #undef MTOUCH_SLIDERS_MAX_SENSORS
            #define MTOUCH_SLIDERS_MAX_SENSORS MTOUCH_NUMBER_SLIDER7_SENSORS
        #endif
    #endif
    #if (MTOUCH_NUMBER_OF_SLIDERS > 8)
        #if  MTOUCH_SLIDERS_MAX_SENSORS < MTOUCH_NUMBER_SLIDER8_SENSORS
            #undef MTOUCH_SLIDERS_MAX_SENSORS
            #define MTOUCH_SLIDERS_MAX_SENSORS MTOUCH_NUMBER_SLIDER8_SENSORS
        #endif
    #endif
    #if (MTOUCH_NUMBER_OF_SLIDERS > 9)
        #if  MTOUCH_SLIDERS_MAX_SENSORS < MTOUCH_NUMBER_SLIDER9_SENSORS
            #undef MTOUCH_SLIDERS_MAX_SENSORS
            #define MTOUCH_SLIDERS_MAX_SENSORS MTOUCH_NUMBER_SLIDER9_SENSORS
        #endif
    #endif
   
    #define MTOUCH_SLIDER_DIVIDE_BY_1       
   
    #define MTOUCH_SLIDER_DIVIDE_BY_2       newSlider = (uint8_t)(newSlider >> 1)

    #define MTOUCH_SLIDER_DIVIDE_BY_3       divisionResult   = (uint8_t)(newSlider >> 2);       \
                                            divisionResult  += (uint8_t)(newSlider >> 4);       \
                                            divisionResult  += (uint8_t)(newSlider >> 6);       \
                                            divisionResult  += (uint8_t)(newSlider >> 7);       \
                                            if (divisionResult > 0xFF) { newSlider = 0xFF; }    \
                                            else                       { newSlider = (uint8_t)divisionResult; }
                                                                
    #define MTOUCH_SLIDER_DIVIDE_BY_4       newSlider = (uint8_t)(newSlider >> 2)

    #define MTOUCH_SLIDER_DIVIDE_BY_5       divisionResult   = (uint8_t)(newSlider >> 3);       \
                                            divisionResult  += (uint8_t)(newSlider >> 4);       \
                                            divisionResult  += (uint8_t)(newSlider >> 6);       \
                                            if (divisionResult > 0xFF) { newSlider = 0xFF; }    \
                                            else                       { newSlider = (uint8_t)divisionResult; }
                                                                
    #define MTOUCH_SLIDER_DIVIDE_BY_6       divisionResult   = (uint8_t)(newSlider >> 3);       \
                                            divisionResult  += (uint8_t)(newSlider >> 5);       \
                                            divisionResult  += (uint8_t)(newSlider >> 7);       \
                                            if (divisionResult > 0xFF) { newSlider = 0xFF; }    \
                                            else                       { newSlider = (uint8_t)divisionResult; }

    #define MTOUCH_SLIDER_DIVIDE_BY_7       divisionResult   = (uint8_t)(newSlider >> 3);       \
                                            divisionResult  += (uint8_t)(newSlider >> 6);       \
                                            if (divisionResult > 0xFF) { newSlider = 0xFF; }    \
                                            else                       { newSlider = (uint8_t)divisionResult; }
                                                                
    #define MTOUCH_SLIDER_DIVIDE(denom)     __paste4(MTOUCH_SLIDER_DIVIDE_BY_, denom)

   
    #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
        enum    mTouch_wheelDirection
                {
                    NOSCROLLING,
                    CLOCKWISE,              //scroll in clockwise direction
                    COUNTERCLOCKWISE        //scroll in counter-clockwise direction
                };

        typedef union
        {
            uint8_t statusMask;
            
            struct
            {
                unsigned direction : 2;     // 00:no scrolling, 01: clockwise, 10 :counter-clockwise
                unsigned     speed : 6;     // 0 no scroll| 1: slow 32: fast
            };
        } mTouch_wheelStatus;

        mTouch_wheelStatus wheelStatus;


        #define  SPEEDLOGWINDOW         32
        
         int8_t  scrollingStep[SPEEDLOGWINDOW];
        uint8_t  lastSlidervalue                = 0;
        int16_t  lastSpeed                      = 0;
    #endif

#endif
#endif