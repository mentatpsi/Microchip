/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_slider.c
 *  Dependencies:    mTouch.h
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Slider Functions
 *                   - Implements the mTouch slider initialization, and 
 *                     decoding.
 *                   - See the documentation for more information about
 *                     implementing the framework with your application.
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
/** @file   mTouch_Slider.c
*   @brief  Implements the mTouch sliders' decoding, if enabled.
*/
#include "mTouch.h"

#if defined(MTOUCH_SLIDERS_ENABLED)  
    
    #if defined(MTOUCH_SLIDER_SCALING_ENABLED)
        uint8_t   mTouch_slider   [MTOUCH_NUMBER_OF_SLIDERS];
        #else
        uint16_t  mTouch_slider   [MTOUCH_NUMBER_OF_SLIDERS];
    #endif

    #if MTOUCH_NUMBER_OF_SLIDERS > 1
        /** @struct     mTouch_SliderState
        *   @brief      Constant array storing configuration data for enabled sliders
        */
        typedef struct {

            unsigned sensorCount    :4;     ///< The number of sensors used to make this slider
                
            #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
            unsigned isWheel        :1;     ///< Boolean: Is this a wheel?
            #endif
            
        } mTouch_SliderState;  
        
        const mTouch_SliderState        mTouch_sliderState[MTOUCH_NUMBER_OF_SLIDERS] = MTOUCH_SLIDER_STRUCT_INIT;

    #endif

    const uint8_t mTouch_sliderIndex    [MTOUCH_NUMBER_OF_SLIDERS][MTOUCH_SLIDERS_MAX_SENSORS]  = MTOUCH_SLIDERINDEX_INIT;

    #if MTOUCH_NUMBER_OF_SLIDERS > 1
        #define MTOUCH_NUMSENSORINSLIDER(i)                     (uint8_t)mTouch_sliderState[i].sensorCount
        #define MTOUCH_GETSLIDERSENSORINDEX(slider, sensor)     mTouch_sliderIndex[slider][sensor]
        #define MTOUCH_ISSLIDERAWHEEL(slider)                   mTouch_sliderState[slider].isWheel
        #define MTOUCH_SLIDERVALUE(i)                           mTouch_slider[i]
    #else
        #define MTOUCH_NUMSENSORINSLIDER(i)                     MTOUCH_NUMBER_SLIDER0_SENSORS
        #define MTOUCH_GETSLIDERSENSORINDEX(slider, sensor)     mTouch_sliderIndex[0][sensor]
        #define MTOUCH_ISSLIDERAWHEEL(slider)                   MTOUCH_SLIDERS_ISWHEEL0
        #define MTOUCH_SLIDERVALUE(i)                           mTouch_slider[0]
    #endif

    
//*************************************************************************
// mTouch_SliderDecode() - Slider and Wheel Decoding Module
//*************************************************************************
void mTouch_SliderDecode(void)
{
    #if MTOUCH_NUMBER_OF_SLIDERS > 1
    for (int8_t sliderIndex = MTOUCH_NUMBER_OF_SLIDERS-1; sliderIndex >= 0; sliderIndex--)
    {
    #endif
    
        uint8_t mTouch_decode = 0;
        
        // Step 1: Determine if any of the sensors assigned to the slider are currently pressed
        #if (MTOUCH_NUMBER_OF_SLIDERS == 1) && (MTOUCH_NUMBER_SLIDER0_SENSORS == 2)
            if ((mTouch_GetButtonState(MTOUCH_SLIDER0_SENSOR0) == MTOUCH_PRESSED) ||
                (mTouch_GetButtonState(MTOUCH_SLIDER0_SENSOR1) == MTOUCH_PRESSED))  mTouch_decode = 1; 
        #elif (MTOUCH_NUMBER_OF_SLIDERS == 1) && (MTOUCH_NUMBER_SLIDER0_SENSORS == 3)
            if ((mTouch_GetButtonState(MTOUCH_SLIDER0_SENSOR0) == MTOUCH_PRESSED) ||
                (mTouch_GetButtonState(MTOUCH_SLIDER0_SENSOR1) == MTOUCH_PRESSED) ||
                (mTouch_GetButtonState(MTOUCH_SLIDER0_SENSOR2) == MTOUCH_PRESSED))  mTouch_decode = 1; 
        #else
            for (int8_t i = MTOUCH_NUMSENSORINSLIDER(sliderIndex)-1; i >= 0; i--)
            {
                if (mTouch_GetButtonState(MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, i)) == MTOUCH_PRESSED)
                {
                    mTouch_decode = 1;
                }
            }
        #endif        
        
        // Step 2: If there are, decode the location of the slider
        if (mTouch_decode)
        {
            int32_t   newSlider       = 0;
            uint8_t   maxIndex        = 0;
            
            #if MTOUCH_NUMBER_OF_SLIDERS > 1 || ((MTOUCH_NUMBER_OF_SLIDERS == 1) && (MTOUCH_NUMBER_SLIDER0_SENSORS > 2))
            uint8_t   indexMinusOne;
            uint8_t   indexPlusOne;
            #endif
         
            #if defined(MTOUCH_SLIDER_SCALING_ENABLED)
            #if defined(MTOUCH_SLIDERS_DIVIDE3_USED) || defined(MTOUCH_SLIDERS_DIVIDE5_USED) || defined(MTOUCH_SLIDERS_DIVIDE6_USED) || defined(MTOUCH_SLIDERS_DIVIDE7_USED)
            uint16_t  divisionResult;
            #endif
            #endif
         
            // Step 2a: Find the sensor on the slider which has the largest delta value...
            #if (MTOUCH_NUMBER_OF_SLIDERS == 1) && (MTOUCH_NUMBER_SLIDER0_SENSORS == 2)
                if (mTouch_deltas[MTOUCH_SLIDER0_SENSOR0] > mTouch_deltas[MTOUCH_SLIDER0_SENSOR1])
                {
                    maxIndex = 0;
                } else {
                    maxIndex = 1;
                }
            #elif (MTOUCH_NUMBER_OF_SLIDERS == 1) && (MTOUCH_NUMBER_SLIDER0_SENSORS == 3)
                if (mTouch_deltas[MTOUCH_SLIDER0_SENSOR0] > mTouch_deltas[MTOUCH_SLIDER0_SENSOR1])
                {
                    if (mTouch_deltas[MTOUCH_SLIDER0_SENSOR0] > mTouch_deltas[MTOUCH_SLIDER0_SENSOR2])
                    {
                        maxIndex = 0;
                    } else {
                        maxIndex = 2;
                    }
                } else {
                    if (mTouch_deltas[MTOUCH_SLIDER0_SENSOR1] > mTouch_deltas[MTOUCH_SLIDER0_SENSOR2])
                    {
                        maxIndex = 1;
                    } else {
                        maxIndex = 2;
                    }
                }
            #else
                for (int8_t i = MTOUCH_NUMSENSORINSLIDER(sliderIndex)-1; i >= 0; i--)
                {
                    if (mTouch_deltas[MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, i)] > newSlider)     // Use the mTouch_sliderIndex array to find all 
                    {                                                                               // the sensors assigned to the slider.
                        maxIndex    = i;
                        newSlider   = mTouch_deltas[MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, i)];
                    }
                }
            #endif
            
            
            // Step 2b: Decode the position when the largest delta is one of the sensors on the end
            if (maxIndex == 0)                                                              // If the largest delta value is on index 0...
            {   
            
                // mTouch_sliderIndex   [sliderIndex][0]    :: Stores the mTouch sensor index of this slider's 'sensor0'
                // mTouch_deltas        [i]                 :: Stores the current delta values for each mTouch sensor
                
                #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                if (MTOUCH_ISSLIDERAWHEEL(sliderIndex) == 0)
                {
                #endif
                
                    //
                    //      |           delta[0]                |
                    //      | (  -------------------  ) - 0x100 | = Slider Value
                    //      |    delta[0] + delta[1]            |  
                    //

                    #define MTOUCH_SLIDER_2SENSOR_DECODE0(deltaFirst, deltaSec)     \
                        newSlider   = (int32_t)((int32_t)(deltaFirst) << 8);        \
                        newSlider  /= (int16_t)(deltaFirst + deltaSec);             \
                        newSlider  -= 0x100;                                        \
                        if (newSlider < 0)  { newSlider = -newSlider; }
                        
                    // Far-left Slider Decode  
                    MTOUCH_SLIDER_2SENSOR_DECODE0(mTouch_deltas[MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, 0)], mTouch_deltas[MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, 1)]);
                    
                    mTouch_decode = 0;      // Finished
                    
                #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                }
                else
                {
                    indexMinusOne   = MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, MTOUCH_NUMSENSORINSLIDER(sliderIndex) - 1);
                    indexPlusOne    = MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, 1);
                    
                    mTouch_decode++;    // Perform wheel decoding
                }
                #endif

            }
            else if (maxIndex == MTOUCH_NUMSENSORINSLIDER(sliderIndex) - 1)
            {   
                #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                if (MTOUCH_ISSLIDERAWHEEL(sliderIndex) == 0)
                {
                #endif
            
                    //
                    //                  delta[last]                                    
                    //       (  ---------------------------  ) + 0x100 * sensorPosition  = Slider Value
                    //          delta[last] + delta[last-1]                              
                    //
                    #define MTOUCH_SLIDER_2SENSOR_DECODEN(deltaLast, deltaSecLast, index)   \
                        newSlider   = (int32_t)((int32_t)(deltaSecLast) << 8);              \
                        newSlider  /= (int16_t)(deltaLast + deltaSecLast);                  \
                        newSlider   = (int32_t)(index << 8) - newSlider;
                    
                    // Far-right Slider Decode
                    MTOUCH_SLIDER_2SENSOR_DECODEN(mTouch_deltas[MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, MTOUCH_NUMSENSORINSLIDER(sliderIndex) - 1)], mTouch_deltas[MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, MTOUCH_NUMSENSORINSLIDER(sliderIndex) - 2)], (MTOUCH_NUMSENSORINSLIDER(sliderIndex) - 1));
                    
                    mTouch_decode   = 0;        // Finished
                    
                #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                }
                else
                {
                    indexMinusOne   = MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, MTOUCH_NUMSENSORINSLIDER(sliderIndex) - 2);
                    indexPlusOne    = MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, 0);
                    
                    mTouch_decode++;        // Perform wheel decoding
                }
                #endif
            
            }
            //#endif
            
            // Slider and Wheel Decode - Middle Sensors
            #if MTOUCH_NUMBER_OF_SLIDERS > 1 || ((MTOUCH_NUMBER_OF_SLIDERS == 1) && (MTOUCH_NUMBER_SLIDER0_SENSORS > 2))
            if (mTouch_decode != 0)
            {   
                if (mTouch_decode == 1)
                {
                    indexMinusOne   = MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, maxIndex - 1);
                    indexPlusOne    = MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, maxIndex + 1);
                }
                
                if (mTouch_deltas[indexPlusOne] > mTouch_deltas[indexMinusOne])
                {
                
                    //
                    //                delta[i+1]                            
                    //       (  ---------------------  )  + (0x100 * i) = Slider Value
                    //          delta[i] + delta[i+1]                      
                    //
                    #define MTOUCH_SLIDER_2SENSOR_DECODERIGHT(deltaMax, deltaPlusOne, index)    \
                        newSlider   = (int32_t)((int32_t)(deltaPlusOne) << 8);                  \
                        newSlider  /= (int16_t)(deltaMax + deltaPlusOne);                       \
                        newSlider  += (int32_t)(index << 8);
                    
                    // Right neighbor bigger
                    MTOUCH_SLIDER_2SENSOR_DECODERIGHT(mTouch_deltas[MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, maxIndex)], mTouch_deltas[indexPlusOne], maxIndex);
                }
                else
                {
                    //
                    //                             delta[i-1]                       
                    //      (0x100 * i) - (  ---------------------  )  = Slider Value
                    //                       delta[i] + delta[i-1]                  
                    //
                    
                    #define MTOUCH_SLIDER_2SENSOR_DECODELEFT(deltaMax, deltaMinusOne, index)    \
                        newSlider   = (int32_t)((int32_t)(deltaMinusOne) << 8);                 \
                        newSlider  /= (int16_t)(deltaMax + deltaMinusOne);                      \
                        newSlider   = (int32_t)(index << 8) - newSlider;                                                  
                    
                    // Left neighbor bigger
                    MTOUCH_SLIDER_2SENSOR_DECODELEFT(mTouch_deltas[MTOUCH_GETSLIDERSENSORINDEX(sliderIndex, maxIndex)], mTouch_deltas[indexMinusOne], maxIndex);
                }
            }
            #endif
            
            // Scale the newSlider value based on the number of sensors in this 
            //   slider to ensure a final result between 0 and 255
            #if defined(MTOUCH_SLIDER_SCALING_ENABLED) || defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
            
                #if     (MTOUCH_NUMBER_OF_SLIDERS == 1) || \
                        (MTOUCH_NUMBER_OF_SLIDERS == 2) && (MTOUCH_NUMBER_SLIDER0_SENSORS == MTOUCH_NUMBER_SLIDER1_SENSORS) || \
                        (MTOUCH_NUMBER_OF_SLIDERS == 3) && (MTOUCH_NUMBER_SLIDER0_SENSORS == MTOUCH_NUMBER_SLIDER1_SENSORS) && (MTOUCH_NUMBER_SLIDER1_SENSORS == MTOUCH_NUMBER_SLIDER2_SENSORS)
                            // If there is only one slider -OR- if the enabled sliders all have the
                            //  same number of sensors, customize the scaling math to minimize the
                            //  number of logic checks and program instructions.
                            #if (MTOUCH_NUMBER_SLIDER0_SENSORS == 2)
                                MTOUCH_SLIDER_DIVIDE(1);
                            #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 3)
                                MTOUCH_SLIDER_DIVIDE(2);
                            #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 4)
                                MTOUCH_SLIDER_DIVIDE(3);
                            #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 5)
                                MTOUCH_SLIDER_DIVIDE(4);
                            #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 6)
                                MTOUCH_SLIDER_DIVIDE(5);
                            #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 7)
                                MTOUCH_SLIDER_DIVIDE(6);
                            #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 8)
                                MTOUCH_SLIDER_DIVIDE(7);
                            #endif
                #elif   (MTOUCH_NUMBER_OF_SLIDERS == 2) && (MTOUCH_NUMBER_SLIDER0_SENSORS != MTOUCH_NUMBER_SLIDER1_SENSORS) || \
                        (MTOUCH_NUMBER_OF_SLIDERS == 3) && (MTOUCH_NUMBER_SLIDER0_SENSORS != MTOUCH_NUMBER_SLIDER1_SENSORS) && (MTOUCH_NUMBER_SLIDER1_SENSORS == MTOUCH_NUMBER_SLIDER2_SENSORS)
                                                    // If there are only two sliders, minimize the logic overhead by performing
                                                    //  a single if check on the slider index value. 
                    if      (sliderIndex == 0   
                    {   
                        #if (MTOUCH_NUMBER_SLIDER0_SENSORS == 2)
                            MTOUCH_SLIDER_DIVIDE(1);
                        #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 3)
                            MTOUCH_SLIDER_DIVIDE(2);
                        #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 4)
                            MTOUCH_SLIDER_DIVIDE(3);
                        #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 5)
                            MTOUCH_SLIDER_DIVIDE(4);
                        #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 6)
                            MTOUCH_SLIDER_DIVIDE(5);
                        #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 7)
                            MTOUCH_SLIDER_DIVIDE(6);
                        #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 8)
                            MTOUCH_SLIDER_DIVIDE(7);
                        #endif
                    }
                    else                        
                    {   
                        #if (MTOUCH_NUMBER_SLIDER1_SENSORS == 2)
                            MTOUCH_SLIDER_DIVIDE(1);
                        #elif (MTOUCH_NUMBER_SLIDER1_SENSORS == 3)
                            MTOUCH_SLIDER_DIVIDE(2);
                        #elif (MTOUCH_NUMBER_SLIDER1_SENSORS == 4)
                            MTOUCH_SLIDER_DIVIDE(3);
                        #elif (MTOUCH_NUMBER_SLIDER1_SENSORS == 5)
                            MTOUCH_SLIDER_DIVIDE(4);
                        #elif (MTOUCH_NUMBER_SLIDER1_SENSORS == 6)
                            MTOUCH_SLIDER_DIVIDE(5);
                        #elif (MTOUCH_NUMBER_SLIDER1_SENSORS == 7)
                            MTOUCH_SLIDER_DIVIDE(6);
                        #elif (MTOUCH_NUMBER_SLIDER1_SENSORS == 8)
                            MTOUCH_SLIDER_DIVIDE(7);
                        #endif
                    }
                #elif   (MTOUCH_NUMBER_OF_SLIDERS == 3)
                                                    // If there are three sliders, minimize the logic overhead by performing
                                                    //  two if checks on the slider index value.
                    if      (sliderIndex == 0)  
                    {   
                        #if (MTOUCH_NUMBER_SLIDER0_SENSORS == 2)
                            MTOUCH_SLIDER_DIVIDE(1);
                        #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 3)
                            MTOUCH_SLIDER_DIVIDE(2);
                        #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 4)
                            MTOUCH_SLIDER_DIVIDE(3);
                        #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 5)
                            MTOUCH_SLIDER_DIVIDE(4);
                        #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 6)
                            MTOUCH_SLIDER_DIVIDE(5);
                        #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 7)
                            MTOUCH_SLIDER_DIVIDE(6);
                        #elif (MTOUCH_NUMBER_SLIDER0_SENSORS == 8)
                            MTOUCH_SLIDER_DIVIDE(7);
                        #endif
                    }
                    else if (sliderIndex == 1)  
                    {   
                        #if (MTOUCH_NUMBER_SLIDER1_SENSORS == 2)
                            MTOUCH_SLIDER_DIVIDE(1);
                        #elif (MTOUCH_NUMBER_SLIDER1_SENSORS == 3)
                            MTOUCH_SLIDER_DIVIDE(2);
                        #elif (MTOUCH_NUMBER_SLIDER1_SENSORS == 4)
                            MTOUCH_SLIDER_DIVIDE(3);
                        #elif (MTOUCH_NUMBER_SLIDER1_SENSORS == 5)
                            MTOUCH_SLIDER_DIVIDE(4);
                        #elif (MTOUCH_NUMBER_SLIDER1_SENSORS == 6)
                            MTOUCH_SLIDER_DIVIDE(5);
                        #elif (MTOUCH_NUMBER_SLIDER1_SENSORS == 7)
                            MTOUCH_SLIDER_DIVIDE(6);
                        #elif (MTOUCH_NUMBER_SLIDER1_SENSORS == 8)
                            MTOUCH_SLIDER_DIVIDE(7);
                        #endif
                    }
                    else                        
                    {   
                        #if (MTOUCH_NUMBER_SLIDER2_SENSORS == 2)
                            MTOUCH_SLIDER_DIVIDE(1);
                        #elif (MTOUCH_NUMBER_SLIDER2_SENSORS == 3)
                            MTOUCH_SLIDER_DIVIDE(2);
                        #elif (MTOUCH_NUMBER_SLIDER2_SENSORS == 4)
                            MTOUCH_SLIDER_DIVIDE(3);
                        #elif (MTOUCH_NUMBER_SLIDER2_SENSORS == 5)
                            MTOUCH_SLIDER_DIVIDE(4);
                        #elif (MTOUCH_NUMBER_SLIDER2_SENSORS == 6)
                            MTOUCH_SLIDER_DIVIDE(5);
                        #elif (MTOUCH_NUMBER_SLIDER2_SENSORS == 7)
                            MTOUCH_SLIDER_DIVIDE(6);
                        #elif (MTOUCH_NUMBER_SLIDER2_SENSORS == 8)
                            MTOUCH_SLIDER_DIVIDE(7);
                        #endif
                    }
                #else
                    // If there are four or more sliders, just implement a switch 
                    //  statement that will divide by the necessary amount.
                    switch(MTOUCH_NUMSENSORINSLIDER(sliderIndex))
                    {
                        #if defined(MTOUCH_SLIDERS_DIVIDE2_USED)            // Only implements cases that will be used.
                            case 3: MTOUCH_SLIDER_DIVIDE(2);  break;
                        #endif
                        #if defined(MTOUCH_SLIDERS_DIVIDE3_USED)
                            case 4: MTOUCH_SLIDER_DIVIDE(3);  break;
                        #endif
                        #if defined(MTOUCH_SLIDERS_DIVIDE4_USED)
                            case 5: MTOUCH_SLIDER_DIVIDE(4);  break;  
                        #endif
                        #if defined(MTOUCH_SLIDERS_DIVIDE5_USED)
                            case 6: MTOUCH_SLIDER_DIVIDE(5);  break;  
                        #endif
                        #if defined(MTOUCH_SLIDERS_DIVIDE6_USED)
                            case 7: MTOUCH_SLIDER_DIVIDE(6);  break;  
                        #endif
                        #if defined(MTOUCH_SLIDERS_DIVIDE7_USED)
                            case 8: MTOUCH_SLIDER_DIVIDE(7);  break;  
                        #endif
                            default:                            break;
                    }
                #endif
            #endif
            
            // Implement a maximum update speed for the slider. 
            // Only allow it to change by a fixed, maximum amount each decode.
            #if defined(MTOUCH_SLIDER_STEP_SIZE)
                #if MTOUCH_SLIDER_STEP_SIZE != 0
                
                    #if defined(MTOUCH_SLIDER_SCALING_ENABLED)
                        #define MTOUCH_SLIDER_EQUALS_NEWSLIDER()    MTOUCH_SLIDERVALUE(sliderIndex) = (uint8_t)(newSlider & 0xFF)   // 8-bit result
                    #else
                        #define MTOUCH_SLIDER_EQUALS_NEWSLIDER()    MTOUCH_SLIDERVALUE(sliderIndex) = (uint16_t)newSlider;          // 16-bit result
                    #endif
                
                    #if defined(MTOUCH_SLIDER_SCALING_ENABLED)
                    if (newSlider == 256) newSlider--;          // If 256, make equal to 255 to fit within 8 bits.
                    #endif
                    
                    #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
                        if ((uint16_t)(newSlider) > (uint16_t)(MTOUCH_SLIDERVALUE(sliderIndex) + MTOUCH_SLIDER_STEP_SIZE))
                        {
                                                                                                                    // If the new wheel value has increased...
                            if ((uint16_t)(newSlider) > (uint16_t)(MTOUCH_SLIDERVALUE(sliderIndex) + 0x7F))         // Check if the scrolling is just scrolling across the 0 and 255 transition
                            {                                                                                       // Special math for step size for transition point
                                if(MTOUCH_SLIDERVALUE(sliderIndex) >= MTOUCH_SLIDER_STEP_SIZE)                       
                                    MTOUCH_SLIDERVALUE(sliderIndex) - =  MTOUCH_SLIDER_STEP_SIZE;                    
                                else
                                    MTOUCH_SLIDERVALUE(sliderIndex) = 0xFF - (MTOUCH_SLIDER_STEP_SIZE - MTOUCH_SLIDERVALUE(sliderIndex));
                            }
                            else
                            {                                                                                        // The scrolling is not near transition point
                                MTOUCH_SLIDERVALUE(sliderIndex) + = MTOUCH_SLIDER_STEP_SIZE;                         // and move step is smaller than step size
                            }

                        }
                        else if ((uint16_t)(newSlider+ MTOUCH_SLIDER_STEP_SIZE) < (uint16_t)(MTOUCH_SLIDERVALUE(sliderIndex)) )   
                        {                                                                                            //If the new wheel value has increased...
                                                                                                                     // Check if the scrolling is just scrolling across the 0 and 255 transition
                            if ((uint16_t)(newSlider + 0x7F )< (uint16_t)(MTOUCH_SLIDERVALUE(sliderIndex)))          // Special math for step size for transition point
                            {                                                                                       
                                if((0xFF - MTOUCH_SLIDERVALUE(sliderIndex) > MTOUCH_SLIDER_STEP_SIZE))                       
                                    MTOUCH_SLIDERVALUE(sliderIndex) +=  MTOUCH_SLIDER_STEP_SIZE;                         
                                else
                                    MTOUCH_SLIDERVALUE(sliderIndex) = MTOUCH_SLIDER_STEP_SIZE - (0xFF -MTOUCH_SLIDERVALUE(sliderIndex) );
                            }
                            else
                            {                                                                                        // The scrolling is not near transition point
                                MTOUCH_SLIDERVALUE(sliderIndex) - = MTOUCH_SLIDER_STEP_SIZE;                         // and move step is smaller than step size
                            }
                        }
                        else
                        {
                            MTOUCH_SLIDER_EQUALS_NEWSLIDER();                                                       // mTouch_slider is near an underflow condition
                                                                                                                    //  and newSlider is not increasing its value.
                                                                                                                    // Simply storing the result as-is.
                        }
                    #else
                        if ((uint16_t)(newSlider) > (uint16_t)(MTOUCH_SLIDERVALUE(sliderIndex) + MTOUCH_SLIDER_STEP_SIZE))           
                        {                                                                                           
                            #if defined(MTOUCH_SLIDER_SCALING_ENABLED)                                              // If the new slider value has increased...
                            if (MTOUCH_SLIDERVALUE(sliderIndex) < (0xFF - MTOUCH_SLIDER_STEP_SIZE))                 // Check if the slider's value is close to 
                            {                                                                                       //  overflowing. If scaling is enabled, it
                            #endif                                                                                  //  is stored as an unsigned char.
                                MTOUCH_SLIDERVALUE(sliderIndex) += MTOUCH_SLIDER_STEP_SIZE;                         // Not going to overflow - increment by the
                            #if defined(MTOUCH_SLIDER_SCALING_ENABLED)                                              //  maximum step size.
                            } else {                                                                                // We might overflow - so just load the
                                MTOUCH_SLIDER_EQUALS_NEWSLIDER();                                                   //  8-bit value from newSlider.
                            }
                            #endif
                        }                                                                         
                        else if (MTOUCH_SLIDERVALUE(sliderIndex) > MTOUCH_SLIDER_STEP_SIZE)                         // If mTouch_slider is less than or close
                        {                                                                                           //  in value to newSlider...
                            if ((uint16_t)(newSlider) < MTOUCH_SLIDERVALUE(sliderIndex) - MTOUCH_SLIDER_STEP_SIZE)  // If newSlider is much less...
                            {                                                                                       //
                                MTOUCH_SLIDERVALUE(sliderIndex) -= MTOUCH_SLIDER_STEP_SIZE;                         //  Decrement by the maximum step size 
                            } else {                                                                                // If newSlider is about equal to mTouchSlider...
                                MTOUCH_SLIDER_EQUALS_NEWSLIDER();                                                   //  Store newSlider as our new result 
                            }                                                                               
                        }                                                                                   
                        else                                                                                
                        {   
                            MTOUCH_SLIDER_EQUALS_NEWSLIDER();                                                       // mTouch_slider is near an underflow condition
                                                                                                                    //  and newSlider is not increasing its value.
                                                                                                                    // Simply storing the result as-is.
                        } 
                    #endif
                #else                                                                           // No maximum step size defined.
                    MTOUCH_SLIDER_EQUALS_NEWSLIDER();                                           // Simply store the result as-is.
                #endif
            #else                                                                               // No maximum step size defined.
                MTOUCH_SLIDER_EQUALS_NEWSLIDER();                                               // Simply store the result as-is.
            #endif
        }
        
    #if MTOUCH_NUMBER_OF_SLIDERS > 1
    }
    #endif
    
    #if defined(MTOUCH_SLIDERS_WHEELS_IMPLEMENTED)
    for (int8_t i = SPEEDLOGWINDOW - 1; i >= 1; i--)
    {
        scrollingStep[i] = scrollingStep[i-1];                                      //shift the log data
    }
    scrollingStep[0] = mTouch_GetSlider(0) - lastSlidervalue;                       //log the last moving steps
    
    // Solve the 0,255 point problem
         if ((mTouch_GetSlider(0) - lastSlidervalue) < -100)    { scrollingStep[0]  = (int8_t)(mTouch_GetSlider(0) - lastSlidervalue + 0xFF); }
    else if ((mTouch_GetSlider(0) - lastSlidervalue) >  100)    { scrollingStep[0]  = (int8_t)(mTouch_GetSlider(0) - lastSlidervalue - 0xFF); }
    else                                                        { scrollingStep[0]  = (int8_t)(mTouch_GetSlider(0) - lastSlidervalue);        }

    int16_t  currentSpeed = 0;
    for(int8_t i = SPEEDLOGWINDOW - 1; i >= 0  ; i--)
    {
        currentSpeed += scrollingStep[i];                       // Sum up all the speed in the log window, then average
    }
    
    currentSpeed   /= SPEEDLOGWINDOW;
    currentSpeed    = (currentSpeed *3  + lastSpeed) >> 2;      // Average the speed
    lastSpeed       = currentSpeed;
    lastSlidervalue = mTouch_GetSlider(0);                      // Log the last position for next step calculation
    
         if (currentSpeed >  2) {   wheelStatus.direction   = CLOCKWISE;        }
    else if (currentSpeed < -2) {   wheelStatus.direction   = COUNTERCLOCKWISE; }
    else                        {   wheelStatus.direction   = NOSCROLLING;      }

    if (currentSpeed < 0)       {   wheelStatus.speed       = -currentSpeed;    }
    else                        {   wheelStatus.speed       =  currentSpeed;    }
    #endif
}

#endif // End - #if defined(MTOUCH_SLIDERS_ENABLED)  
