
/*****************************************************************************
* FileName:         mTouchCapApp_DirectKeys.c
* Includes:
*   void mTouchCapApp_DirectKeys_Demo(void)
*   void mTouchCapApp_DirectKeys_Create(void)
*
* Dependencies:
* Processor:        PIC32
* Compiler:         C32
* Company:          Microchip Technology Incorporated
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
* Author                Date           Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* BDB                   26 Jun 2008    First Version for CTMU EVAL Board
* SB                    22 Oct 2008    Updates to First Version
* Naveen. M             14 Apr 2009    Version 0.1 Initial Draft
* Sasha. M  / Naveen. M 4 May 2009     Version 0.2 Updates
* Sasha. M  / Naveen. M 11 Nov 2009    Version 1.0 Release
* Sasha. M  / Nithin.   10 April 2010  Version 1.20 Release
* MWM                      March 2011
*   Changed mTouchCapApp_DirectKeys_Demo to support multi-touch
*   Simplified mTouchCapApp_DirectKeys_Create
*****************************************************************************/


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~    Includes    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include    "mTouchConfig.h"
#include    "HardwareProfile.h"
#include    "mTouchCapAPI.h"
#include    "mTouchCapLED.h"
#include    "mTouchCapPhy.h"

DIRECTKEY DirectKey1;
DIRECTKEY DirectKey2;
DIRECTKEY DirectKey3;
DIRECTKEY DirectKey4;
#if     !defined( PIC32MX220_STARTER_KIT_BOARD )
DIRECTKEY DirectKey5;
# if     !defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
DIRECTKEY DirectKey6;
DIRECTKEY DirectKey7;
DIRECTKEY DirectKey8;
# endif//!defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
#endif//!defined( PIC32MX220_STARTER_KIT_BOARD )

/********************************************************************
 * Function         :    void mTouchCapApp_DirectKeys_Demo(void)
 *
 * PreCondition     :    None
 *
 * Input            :    None
 *
 * Output           :    None
 *
 * Side Effects     :    None
 *
 * Overview         :   This function will demonstrate the usage of the API's
 *                      that is associated with the Direct key Demo Board
 *
 * Note         :
 *******************************************************************/
UINT16 mTouchCapApp_DirectKeys_Demo(void)
{
    UINT16 ButtonStatus = 0;


    if(KEY_PRESSED ==
         mTouchCapAPI_GetChannelTouchStatus(DirectKey1.Channel_Num,
                                            DirectKey1.DecodeMethod) )
    {
#      if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
        mTouchCapLED_SetLED(LED_1);
#      elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
        mTouchCapLED_SetLED(LED_1);
#      else
        mTouchCapLED_SetLED(LED_8);
#      endif
        ButtonStatus += 1;
    }

    if(KEY_PRESSED ==
         mTouchCapAPI_GetChannelTouchStatus(DirectKey2.Channel_Num,
                                            DirectKey2.DecodeMethod) )
    {
#      if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
        mTouchCapLED_SetLED(LED_2);
#      elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
        mTouchCapLED_SetLED(LED_2);
#      else
        mTouchCapLED_SetLED(LED_9);
#      endif
        ButtonStatus += 2;
    }

    if(KEY_PRESSED ==
         mTouchCapAPI_GetChannelTouchStatus(DirectKey3.Channel_Num,
                                            DirectKey3.DecodeMethod) )
    {
#      if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
        mTouchCapLED_SetLED(LED_3);
#      elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
        mTouchCapLED_SetLED(LED_3);
#      else
        mTouchCapLED_SetLED(LED_10);
#      endif
        ButtonStatus += 4;
    }

    if(KEY_PRESSED ==
         mTouchCapAPI_GetChannelTouchStatus(DirectKey4.Channel_Num,
                                            DirectKey4.DecodeMethod) )
    {
#      if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
        mTouchCapLED_SetLED(LED_4);
#      elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
        mTouchCapLED_SetLED(LED_4);
#      else
        mTouchCapLED_SetLED(LED_11);
#      endif
        ButtonStatus += 8;
    }

    // Starter PIC32MX220F032D kit board has only 4 buttons
#if !defined( PIC32MX220_STARTER_KIT_BOARD )
    if(KEY_PRESSED ==
         mTouchCapAPI_GetChannelTouchStatus(DirectKey5.Channel_Num,
                                            DirectKey5.DecodeMethod) )
    {
#      if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
        mTouchCapLED_SetLED(LED_5);
#      elif defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
        mTouchCapLED_SetLED(LED_5);
#      else
        mTouchCapLED_SetLED(LED_12);
#      endif
        ButtonStatus += 16;
    }

    // One Pic Microcontroller Platform Demo Board has only 5 buttons
# if !defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
    if(KEY_PRESSED ==
         mTouchCapAPI_GetChannelTouchStatus(DirectKey6.Channel_Num,
                                            DirectKey6.DecodeMethod) )
    {
#      if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
        mTouchCapLED_SetLED(LED_6);
#      else
        mTouchCapLED_SetLED(LED_13);
#      endif
        ButtonStatus += 32;
    }

    if(KEY_PRESSED ==
         mTouchCapAPI_GetChannelTouchStatus(DirectKey7.Channel_Num,
                                            DirectKey7.DecodeMethod) )
    {
#      if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
        mTouchCapLED_SetLED(LED_7);
#      else
        mTouchCapLED_SetLED(LED_14);
#      endif
        ButtonStatus += 64;
    }

    if(KEY_PRESSED ==
         mTouchCapAPI_GetChannelTouchStatus(DirectKey8.Channel_Num,
                                            DirectKey8.DecodeMethod) )
    {
#      if defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD )
        mTouchCapLED_SetLED(LED_8);
#      else
        mTouchCapLED_SetLED(LED_15);
#      endif
        ButtonStatus += 128;
    }
# endif//!defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
#endif//!defined( PIC32MX220_STARTER_KIT_BOARD )

    if ( ButtonStatus == 0 )
    {
        mTouchCapLED_SetLED(LED_NIL);
    }

    return ButtonStatus;
}

/********************************************************************
 * Function         :     void mTouchCapApp_DirectKeys_Create(void)
 *
 * PreCondition     :    None
 *
 * Input            :    None
 *
 * Output           :    None
 *
 * Side Effects     :    None
 *
 * Overview         :   This function fills the direct keys conf struct
 *
 * Note         :
 *******************************************************************/
 #define SETUP_DIRECT_KEY(AddrDirectKey,DirectKeyChannelNumber) \
           mTouchCapAPI_SetUpChannelDirectKey((AddrDirectKey), \
                                              (DirectKeyChannelNumber), \
                                              DEFAULT_TRIP_VALUE, \
                                              DEFAULT_DECODE_METHOD, \
                                              DEFAULT_FILTER_METHOD);
 void mTouchCapApp_DirectKeys_Create(void)
 {
     SETUP_DIRECT_KEY(&DirectKey1,DIRECTKEY1_CHANNEL);
     SETUP_DIRECT_KEY(&DirectKey2,DIRECTKEY2_CHANNEL);
     SETUP_DIRECT_KEY(&DirectKey3,DIRECTKEY3_CHANNEL);
     SETUP_DIRECT_KEY(&DirectKey4,DIRECTKEY4_CHANNEL);
#if !defined( PIC32MX220_STARTER_KIT_BOARD )
     SETUP_DIRECT_KEY(&DirectKey5,DIRECTKEY5_CHANNEL);
#if !defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
     SETUP_DIRECT_KEY(&DirectKey6,DIRECTKEY6_CHANNEL);
     SETUP_DIRECT_KEY(&DirectKey7,DIRECTKEY7_CHANNEL);
     SETUP_DIRECT_KEY(&DirectKey8,DIRECTKEY8_CHANNEL);
#endif//!defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
#endif//!defined( PIC32MX220_STARTER_KIT_BOARD )
}

