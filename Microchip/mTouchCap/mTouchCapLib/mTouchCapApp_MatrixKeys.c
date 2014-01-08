
/*****************************************************************************
* FileName:        mTouchCap_MatrixKeys.c
* Includes:
*   void   mTouchCapApp_MatrixKeys_Create(void)
*   UINT16 mTouchCapApp_MatrixKeys_Demo(void)
*
* Dependencies:
* Processor:       PIC32
* Compiler:         C32
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
* Author                 Date           Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* BDB                    26 Jun 2008    First Version for CTMU EVAL Board
* SB                     22 Oct 2008    Updates to First Version
* Naveen. M              14 Apr 2009    Version 0.1 Initial Draft
* Sasha. M  / Naveen. M   4 May 2009    Version 0.2 Updates
* Bruce Bohn             10 Nov 2009    Version 0.3 Updates
* Sasha. M  / Naveen. M  11 Nov 2009    Version 1.0 Release
* Sasha. M  / Nithin.    10 April 2010  Version 1.20 Release
* MWM                       March 2011
*   Simplified mTouchCapApp_MatrixKeys_Create
*   Changed mTouchCapApp_MatrixKeys_Demo to support multitouch
*****************************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~    Includes    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include    "mTouchConfig.h"
#include    "HardwareProfile.h"
#include    "mTouchCapApp_MatrixKeys.h"
#include    "mTouchCapAPI.h"
#include    "mTouchCapLED.h"
#include    "mTouchCapPhy.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~    Variables   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// From  mTouchCapAPI.c
extern BYTE Decode_Method_Used;


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~    Function Prototypes  ~~~~~~~~~~~~~~~~~~~~~~~~~~     */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~Instantiate or create an objects for MATRIXKEY structure*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

MATRIXKEY Matrix_Key0;
MATRIXKEY Matrix_Key1;
MATRIXKEY Matrix_Key2;
MATRIXKEY Matrix_Key3;
MATRIXKEY Matrix_Key4;
MATRIXKEY Matrix_Key5;
MATRIXKEY Matrix_Key6;
MATRIXKEY Matrix_Key7;
MATRIXKEY Matrix_Key8;
MATRIXKEY Matrix_Key9;
MATRIXKEY Matrix_Key10;
MATRIXKEY Matrix_Key11;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~    Function Definitions  ~~~~~~~~~~~~~~~~~~~~~~~~~~~   */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/********************************************************************
 * Function         :    void mTouchCapApp_MatrixKeys_Create(void)
 *
 * PreCondition     :    None
 *
 * Input            :    None
 *
 * Output           :    None
 *
 * Side Effects     :    None
 *
 * Overview         :   This function will setup the channels and other related properties
 *                      that are associated with the corresonding Direct key.
 *
 * Note         :
 *******************************************************************/
#define SETUP_MATRIX_KEY(AddrMatrixKey,MatrixKeyRowChan, MatrixKeyColChan) \
            mTouchCapAPI_SetUpChannelMatrixKey((AddrMatrixKey), \
                                               (MatrixKeyRowChan), \
                                               (MatrixKeyColChan), \
                                               DEFAULT_TRIP_VALUE, \
                                               DEFAULT_DECODE_METHOD, \
                                               DEFAULT_FILTER_METHOD)
void mTouchCapApp_MatrixKeys_Create(void)
{

/***************************************************************************
--------------- Demo of API "mTouchCapAPI_SetUpChannelMatrixKey" -----------------------
set up the channels of the matrix keys using API "mTouchCapAPI_GetStatusMatrixButton" whose Row and Column
channel numbers is mentioned. .
***************************************************************************/
    //objects of Matrix key structure

    SETUP_MATRIX_KEY( &Matrix_Key0, MATRIXKEY0_ROW_CHANNEL,  MATRIXKEY0_COLUMN_CHANNEL );
    SETUP_MATRIX_KEY( &Matrix_Key1, MATRIXKEY1_ROW_CHANNEL,  MATRIXKEY1_COLUMN_CHANNEL );
    SETUP_MATRIX_KEY( &Matrix_Key2, MATRIXKEY2_ROW_CHANNEL,  MATRIXKEY2_COLUMN_CHANNEL );
    SETUP_MATRIX_KEY( &Matrix_Key3, MATRIXKEY3_ROW_CHANNEL,  MATRIXKEY3_COLUMN_CHANNEL );
    SETUP_MATRIX_KEY( &Matrix_Key4, MATRIXKEY4_ROW_CHANNEL,  MATRIXKEY4_COLUMN_CHANNEL );
    SETUP_MATRIX_KEY( &Matrix_Key5, MATRIXKEY5_ROW_CHANNEL,  MATRIXKEY5_COLUMN_CHANNEL );
    SETUP_MATRIX_KEY( &Matrix_Key6, MATRIXKEY6_ROW_CHANNEL,  MATRIXKEY6_COLUMN_CHANNEL );
    SETUP_MATRIX_KEY( &Matrix_Key7, MATRIXKEY7_ROW_CHANNEL,  MATRIXKEY7_COLUMN_CHANNEL );
    SETUP_MATRIX_KEY( &Matrix_Key8, MATRIXKEY8_ROW_CHANNEL,  MATRIXKEY8_COLUMN_CHANNEL );
    SETUP_MATRIX_KEY( &Matrix_Key9, MATRIXKEY9_ROW_CHANNEL,  MATRIXKEY9_COLUMN_CHANNEL );
    SETUP_MATRIX_KEY( &Matrix_Key10,MATRIXKEY10_ROW_CHANNEL,MATRIXKEY10_COLUMN_CHANNEL );
    SETUP_MATRIX_KEY( &Matrix_Key11,MATRIXKEY11_ROW_CHANNEL,MATRIXKEY11_COLUMN_CHANNEL );
}


/********************************************************************
 * Function         :    void mTouchCapApp_MatrixKeys_Demo(void)
 *
 * PreCondition     :    None
 *
 * Input            :    None
 *
 * Output           :    None
 *
 * Side Effects     :    None
 *
 * Overview         :  Demonstrates the APIs of Matrix keys.
 *
 * Note             :
 *******************************************************************/
UINT16 mTouchCapApp_MatrixKeys_Demo(void)
{
    UINT16 ButtonStatus = 0;

    if ( KEY_PRESSED == mTouchCapAPI_GetStatusMatrixButton(&Matrix_Key0))
    {
        mTouchCapLED_SetLED(LED_1);
        ButtonStatus += 1<<0;
    }

    if ( KEY_PRESSED == mTouchCapAPI_GetStatusMatrixButton(&Matrix_Key1))
    {
        mTouchCapLED_SetLED(LED_2);
        ButtonStatus += 1<<1;
    }

    if ( KEY_PRESSED == mTouchCapAPI_GetStatusMatrixButton(&Matrix_Key2))
    {
        mTouchCapLED_SetLED(LED_3);
        ButtonStatus += 1<<2;
    }
            /* Check if the Matrix key is pressed l */
    if ( KEY_PRESSED == mTouchCapAPI_GetStatusMatrixButton(&Matrix_Key3))
    {
        mTouchCapLED_SetLED(LED_4);
        ButtonStatus += 1<<3;
    }

    if ( KEY_PRESSED == mTouchCapAPI_GetStatusMatrixButton(&Matrix_Key4))
    {
        mTouchCapLED_SetLED(LED_5);
        ButtonStatus += 1<<4;
    }

    if ( KEY_PRESSED == mTouchCapAPI_GetStatusMatrixButton(&Matrix_Key5))
    {
        mTouchCapLED_SetLED(LED_6);
        ButtonStatus += 1<<5;
    }

    if ( KEY_PRESSED == mTouchCapAPI_GetStatusMatrixButton(&Matrix_Key6))
    {
        mTouchCapLED_SetLED(LED_7);
        ButtonStatus += 1<<6;
    }

    if ( KEY_PRESSED == mTouchCapAPI_GetStatusMatrixButton(&Matrix_Key7))
    {
        mTouchCapLED_SetLED(LED_8);
        ButtonStatus += 1<<7;
    }

    if ( KEY_PRESSED == mTouchCapAPI_GetStatusMatrixButton(&Matrix_Key8))
    {
        mTouchCapLED_SetLED(LED_9);
        ButtonStatus += 1<<8;
    }

    if ( KEY_PRESSED == mTouchCapAPI_GetStatusMatrixButton(&Matrix_Key9))
    {
        mTouchCapLED_SetLED(LED_10);
        ButtonStatus += 1<<9;
    }

    if ( KEY_PRESSED == mTouchCapAPI_GetStatusMatrixButton(&Matrix_Key10))
    {
        mTouchCapLED_SetLED(LED_11);
        ButtonStatus += 1<<10;
    }

    if ( KEY_PRESSED == mTouchCapAPI_GetStatusMatrixButton(&Matrix_Key11))
    {
        mTouchCapLED_SetLED(LED_12);
        ButtonStatus += 1<<11;
    }

    if(ButtonStatus == 0)
    {
        mTouchCapLED_SetLED(LED_NIL);
    }

    return ButtonStatus;

}
