
/*****************************************************************************
* FileName:        mTouchCapPhy.h
* Dependencies:
* Processor:       PIC32MX
* Compiler:         C32
* Company:         Microchip Technology Incorporated
*
* Software License Agreement
*
* Copyright © 2011 Microchip Technology Inc.
* Microchip licenses this software to you solely for use with Microchip
* products, according to the terms of the accompanying click-wrap software
* license. Microchip and its licensors retain all right, title and interest in
* and to the software.  All rights reserved. This software and any accompanying
* information is for suggestion only. It shall not be deemed to modify
* Microchip’s standard warranty for its products.  It is your responsibility to
* ensure that this software meets your requirements.
*
* SOFTWARE IS PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR
* IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
* NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL
* MICROCHIP OR ITS LICENSORS BE LIABLE FOR ANY DIRECT OR INDIRECT DAMAGES OR
* EXPENSES INCLUDING BUT NOT LIMITED TO INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
* OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
* SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, OR ANY CLAIMS BY THIRD PARTIES
* (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
* The aggregate and cumulative liability of Microchip and its licensors for
* damages related to the use of the software will in no event exceed $1000 or
* the amount you paid Microchip for the software, whichever is greater.
*
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
* TERMS AND THE TERMS OF THE ACCOMPANYING CLICK-WRAP SOFTWARE LICENSE.
*
* Author                    Date          Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Naveen. M                 14 Apr 2009   Version 1.0 Initial Draft
* Sasha. M  / Naveen. M      4 May 2009   Version 1.0 Initial Draft
* Sasha. M  / Naveen. M     11 Nov 2009   Version 1.0 Release
* Sasha. M  / Nithin.       10 Apr 2010   Version 1.20 Release
* MC                        10 Aug 2010   Version 1.21 Release
* MWM                       28 Mar 2011   Added mTouchCapPhy_UpdateData
*****************************************************************************/

#ifndef _MTOUCH_CAP_PHY_H
#define _MTOUCH_CAP_PHY_H

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~    Includes    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include        "mTouchConfig.h"
#include        "HardwareProfile.h"
#include        "mTouchCapTmr.h"
#include        "mTouchCapADC.h"
#include        "GenericTypeDefs.h"

void mTouchCapApp_PortSetup(void);
void mTouchCapPhy_AdcSetup(void);
void mTouchCapPhy_TripValCalc(WORD Index);
void mTouchCapPhy_Initialize(void);
void mTouchCapPhy_StablizeChannelData(void);
void mTouchCapPhy_TickInit(void);
void mTouchCapPhy_UpdateData( UINT16 *CurrentMeasurementData,
                              UINT16 *AveragedData,
                              UINT16 *Weights,
                              BOOL    UpdateAverages,
                              UINT16  AvgFilterType );

//////////////////////////////////////////////////////////////////////
// CHANNEL SELECTION DEFINITIONS
//////////////////////////////////////////////////////////////////////

#ifdef  USE_DIRECT_KEYS
    #define DIRECTKEYS      8       // 8 direct keys
#endif

#ifdef  USE_SLIDER_2CHNL
    #define SLDR2CHL        4       // 4 slider keys enabled - only 2 are used though
#endif

#ifdef  USE_SLIDER_4CHNL
    #define SLDR4CHL        4       // 4 slider keys enabled
#endif

#ifdef  USE_MATRIX_KEYS
    #define NUMROWS         4       // number of rows 4
    #define NUMCOLS         3       // number of colons 4
#endif

#define DIGITAL             1
#define ANALOG              0

enum
{
    FILTER_METHOD_USECHANNEL=0, // Use channel filter method
    FILTER_METHOD_DONTAVERAGE,  // Average = Current Filtered Data
    FILTER_METHOD_FASTAVERAGE,
    FILTER_METHOD_SLOWAVERAGE,
    FILTER_METHOD_GATEDAVERAGE
};

enum
{
    NOT_AN_ACTIVE_CHANNEL = -1,
    KEY_NOT_PRESSED,
    KEY_PRESSED,
};

enum
{
    DECODE_METHOD_MOST_PRESSED = 0,         /* Future exapansion pending */
    DECODE_METHOD_MULTIPLE_PRESS,       /* Future exapansion pending */
    DECODE_METHOD_PRESS_ASSERT          /* Implemented */
};

#endif  // _MTOUCH_CAP_PHY_H


