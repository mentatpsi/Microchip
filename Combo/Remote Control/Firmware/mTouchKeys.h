
/*****************************************************************************
 * FileName:        mTouch_Keys.h
 * Dependencies:
 * Processor:       PIC18, PIC24
 * Compiler:       	C18, C30
 * Linker:          MPLINK,MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2009 Microchip Technology Inc.  
 * Microchip licenses this software to you solely for use with Microchip products, according to the terms of the  
 * accompanying click-wrap software license.  Microchip and its licensors retain all right, title and interest in 
 * and to the software.  All rights reserved. This software and any accompanying information is for suggestion only.  
 * It shall not be deemed to modify Microchip’s standard warranty for its products.  It is your responsibility to ensure 
 * that this software meets your requirements.

 * SOFTWARE IS PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS LICENSORS BE LIABLE 
 * FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO INCIDENTAL, 
 * SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, 
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, OR ANY CLAIMS BY THIRD PARTIES 
 * (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.  
 * The aggregate and cumulative liability of Microchip and its licensors for damages related to the use of the software 
 * will in no event exceed $1000 or the amount you paid Microchip for the software, whichever is greater.

 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS AND 
 * THE TERMS OF THE ACCOMPANYING CLICK-WRAP SOFTWARE LICENSE.
 * 
 *
 * Author               		Date        		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *****************************************************************************/

#ifndef _MTOUCH_DIRECT_KEYS_H
#define _MTOUCH_DIRECT_KEYS_H


// Sensors thresholds for CTMU
#define SENSOR_PRESS_CTMU_THRESHOLD        40
#define SENSOR_RELEASE_CTMU_THRESHOLD      20

// Keys numbers
#define VOL_PLUS	                       0
#define VOL_MINUS	                       1
#define CH_PLUS                            2
#define CH_MINUS                           3
#define UP                                 4
#define DOWN                               5
#define LEFT                               6
#define RIGHT                              7
#define OK                                 8
#define MUTE                               9
#define BACK                               10


void mTouch_CreateKeys(void);
void mTouch_ProcessKeys(void);


#endif	// _MTOUCH_DIRECT_KEYS_H


