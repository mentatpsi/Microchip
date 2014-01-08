/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_proximity.h
 *
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework Proximity Sensor Include File
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
/** @file   mTouch_proximity.h
*   @brief  Internal framework header file implementing proximity sensor functionality
*/

#ifndef __MTOUCH_PROXIMITY_H
#define __MTOUCH_PROXIMITY_H

#if defined(MTOUCH_PROXIMITY_ENABLED)

    
    #define MTOUCH_PROXINDEX_ARRAY_INIT     {                               \
                                                MTOUCH_SENSOR0_PROXINDEX    \
                                                MTOUCH_SENSOR1_PROXINDEX    \
                                                MTOUCH_SENSOR2_PROXINDEX    \
                                                MTOUCH_SENSOR3_PROXINDEX    \
                                                MTOUCH_SENSOR4_PROXINDEX    \
                                                MTOUCH_SENSOR5_PROXINDEX    \
                                                MTOUCH_SENSOR6_PROXINDEX    \
                                                MTOUCH_SENSOR7_PROXINDEX    \
                                                MTOUCH_SENSOR8_PROXINDEX    \
                                                MTOUCH_SENSOR9_PROXINDEX    \
                                                MTOUCH_SENSOR10_PROXINDEX   \
                                                MTOUCH_SENSOR11_PROXINDEX   \
                                                MTOUCH_SENSOR12_PROXINDEX   \
                                                MTOUCH_SENSOR13_PROXINDEX   \
                                                MTOUCH_SENSOR14_PROXINDEX   \
                                                MTOUCH_SENSOR15_PROXINDEX   \
                                                MTOUCH_SENSOR16_PROXINDEX   \
                                                MTOUCH_SENSOR17_PROXINDEX   \
                                                MTOUCH_SENSOR18_PROXINDEX   \
                                                MTOUCH_SENSOR19_PROXINDEX   \
                                                MTOUCH_SENSOR20_PROXINDEX   \
                                                MTOUCH_SENSOR21_PROXINDEX   \
                                                MTOUCH_SENSOR22_PROXINDEX   \
                                                MTOUCH_SENSOR23_PROXINDEX   \
                                                MTOUCH_SENSOR24_PROXINDEX   \
                                                MTOUCH_SENSOR25_PROXINDEX   \
                                                MTOUCH_SENSOR26_PROXINDEX   \
                                                MTOUCH_SENSOR27_PROXINDEX   \
                                                MTOUCH_SENSOR28_PROXINDEX   \
                                                MTOUCH_SENSOR29_PROXINDEX   \
                                            }


    void            mTouch_ProxInit             (void);
    uint8_t         mTouch_ProxStateMachine     (void);
    
#endif
#endif