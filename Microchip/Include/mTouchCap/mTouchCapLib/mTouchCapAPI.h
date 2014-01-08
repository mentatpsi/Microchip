/**************************************************************************************

* FileName:        mTouchCapAPI.h
* Dependencies:    See included files, below.
* Processor:       PIC32MX795F512H
* Compiler:        C32
* Company:         Microchip Technology, Inc.

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
* Change History:
* Author    Date         Comments
*---------------------------------------------------------------------------------------
* BDB       26-Jun-2008  Initial release
* NMS/NK    10-Feb-2009  Folder/Files restructuring
* NK        24-Apr-2009  Porting for 18F46J50 Eval Board
* MC        22-Ian-2009  Porting for PIC32MX795F512H
* MWM       06-Sep-2011  Renamed file to mTouchCapAPI.h
**************************************************************************************/
#ifndef __MTOUCHCAP_API_H
#define __MTOUCHCAP_API_H

//////////////////////////////////////////////////////////////////////
// FILE INCLUDES
//////////////////////////////////////////////////////////////////////
#include    "mTouchConfig.h"
#include    "GenericTypeDefs.h"


//////////////////////////////////////////////////////////////////////
// CVD TECHNIQUE CALIBRATION DEFINITIONS
//////////////////////////////////////////////////////////////////////

#define MATRIX_SINGLE_KEY_CH  2     // 2 channels are multiplexed to form a single Matrix key
#define FOUR_CH_SLIDER        4     // 4 channels are used in a 4 channel slider
#define TWO_CH_SLIDER         2     // 2 channels are used in a 2 channel slider

//////////////////////////////////////////////////////////////////////
// END
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ENUMS, TYPEDEFS
//////////////////////////////////////////////////////////////////////

typedef struct{

                BYTE Channel_Num;  // Channel Used by the Directkey
                WORD TripValue;    // Trip value for each channel
                WORD hystValue;    // Hystersis value
                BYTE FilterType;   //  Stores Filter for each channel
                BYTE DecodeMethod; //  Decode Method for each Channel

} DIRECTKEY;

// Structure which stores the members associated with the Matrix key.

typedef struct{

                BYTE Channel_Num[MATRIX_SINGLE_KEY_CH];  // Channel Used by the Matrixkey
                WORD TripValue;    // Trip value for each key( includes 2 channels)
                WORD hystValue;    // Hystersis value for each key( includes 2 channels)
                BYTE FilterType;   // Stores Filter for each key( includes 2 channels)
                BYTE DecodeMethod; // Decode Method for each key( includes 2 channels)

} MATRIXKEY;

// Structure which stores the members associated with the 4 channel sliders.

typedef struct{

                BYTE Channel_Num[FOUR_CH_SLIDER]; // Channel sUsed by the Sliders
                WORD TripValue;       // Trip value for each key( includes 2 channels)
                WORD hystValue;      // Hystersis value for each key( includes 2 channels)
                BYTE FilterType;    //  Stores Filter for each key( includes 2 channels)
                BYTE DecodeMethod;  //  Decode Method for each key( includes 2 channels)

} SLIDER4CH;

enum
{
    RELEASE=0,
    PRESS
};

enum
{
    DISABLE=0,
    ENABLE
};

enum
{
    OUTPUT=0,
    INPUT
};

//////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////


void mTouchCapAPI_Init(void);
void mTouchCapAPI_SetUp_Default(WORD ChannelNumber);
BYTE mTouchCapAPI_GetChannelTouchStatus(WORD ChIndex, BYTE Decode_Method);
void mTouchCapAPI_SetUpChannelDirectKey(DIRECTKEY *Button,
                                        BYTE Channel_number,
                                        WORD Trip_Value,
                                        WORD Decode_Method,
                                        WORD Filter_Method);
BYTE mTouchCapAPI_GetStatusDirectButton(DIRECTKEY *Button);
void mTouchCapAPI_SetUpChannelMatrixKey(MATRIXKEY *Button,
                                        BYTE Row_Channel_Number,
                                        BYTE Col_Channel_Number,
                                        WORD Trip_Value,
                                        WORD Decode_Method,
                                        WORD Filter_Method);
BYTE mTouchCapAPI_GetStatusMatrixButton (MATRIXKEY *Button);
void mTouchCapAPI_SetUpChannelSlider4Ch(SLIDER4CH *Slider,
                                        BYTE Slider_Channel1_Number,
                                        BYTE Slider_Channel2_Number,
                                        BYTE Slider_Channel3_Number,
                                        BYTE Slider_Channel4_Number,
                                        WORD Trip_Value,
                                        BYTE Decode_Method,
                                        BYTE Filter_Method);
SHORT mTouchCapAPI_GetStatusSlider4Ch (SLIDER4CH *Slider);


#endif    // end of #ifndef __MTOUCHCAP_API_H
