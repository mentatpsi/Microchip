/******************************************************************************
* File Name:       mTouchCap_CvdAPI.c
* Includes:
*   void mTouchCapAPI_Init(void)
*   void mTouchCapAPI_SetUp_Default(WORD ChannelNum)
*   void mTouchCapAPI_SetUpChannelDirectKey(DIRECTKEY *Button,
*                                           BYTE Channel_number,
*                                           WORD Trip_Value,
*                                           WORD Decode_Method,
*                                           WORD Filter_Method)
*   BYTE mTouchCapAPI_GetStatusMatrixButton(MATRIXKEY *Button)
*   void mTouchCapAPI_SetUpChannelSlider4Ch(SLIDER4CH *Slider,
*                                           BYTE Slider_Channel1_Number,
*                                           BYTE Slider_Channel2_Number,
*                                           BYTE Slider_Channel3_Number,
*                                           BYTE Slider_Channel4_Number,
*                                           WORD Trip_Value,
*                                           BYTE Decode_Method,
*                                           BYTE Filter_Method)
*   SHORT mTouchCapAPI_GetStatusSlider4Ch (SLIDER4CH *Slider)
*   static void LoadChannelNumber (BYTE Channel_Number_Passed)
*   BYTE mTouchCapAPI_GetChannelTouchStatus(WORD ChIndex, BYTE Decode_Method)
*
* Dependencies:    None
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
* Author          Date    Comments
*------------------------------------------------------------------------------
* BDB          26-Jun-2008 First release
* SB           22-Oct-2008
* NK           24-Apr-2009 Porting for 18F46J50 Eval Board
* MC           22-Jan-2010 Porting for PIC32MX795F512H
* MWM             Mar-2011 Reogranized code, added more comments, Added decode
*                          method to mTouchCapAPI_GetChannelTouchStatus
******************************************************************************/

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
#include "mTouchConfig.h"
#include "GenericTypeDefs.h"
#include "mTouchCapAPI.h"
#include "mTouchCapPhy.h"
#include "mTouchCapTmr.h"

/* Variables only used locally ***********************************************/
static UINT16 pressedCount[MAX_ADC_CHANNELS];  //Storage for count of pressed value for each channel
static UINT16 unpressedCount[MAX_ADC_CHANNELS];//Storage for count of unpressed value for each channel

/* Global variables referenced elsewhere *************************************/
UINT8  Decode_Method_Used = DECODE_METHOD_MOST_PRESSED;
UINT16 ScanChannels[MAX_ADC_CHANNELS]={0xFF,0xFF,0xFF,0xFF,
                                       0xFF,0xFF,0xFF,0xFF,
                                       0xFF,0xFF,0xFF,0xFF,
                                       0xFF};
UINT16 tripValue[MAX_ADC_CHANNELS];
UINT16 hystValue[MAX_ADC_CHANNELS];
UINT16 averageData[MAX_ADC_CHANNELS];   // running average of CVD channels

UINT8  NumberScanChannels = 0; // store the index of the channels that has to be scanned
UINT8  button_set_number = 0; //# of current button in scan
UINT8  HF_Read_Number    = 0; //# of High Frequency reads so far,= 0,1 ... NUM_HF_READS - 1

UINT8 volatile EmTouchDataReady;        //global indicating the reading of all channels finished

/* External References ********************************************************/
// From mTouchCapPhy.c
extern UINT16 CurFiltdData[MAX_ADC_CHANNELS];  // current filtered value for all channels
extern UINT8  chFilterType[MAX_ADC_CHANNELS];  // Channel filter type for all channels
extern UINT16 ButtonWeight[MAX_ADC_CHANNELS]; // measurement of button presses
extern UINT16 MinButtonWeight;
extern INT8   iScanChanMinWeight;

static void LoadChannelNumber (BYTE Channel_Number_Passed);

/******************************************************************************
 * Function:       void mTouchCapAPI_Init(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is used to initialize the default
 *                  settings of ADC and other related modules.
 *
 * Note:            None
 *****************************************************************************/
void mTouchCapAPI_Init(void)
{
    WORD Index=0;

    // init main ports, ADC ...
    mTouchCapPhy_Initialize();

    //initialize the Timer4
    mTouchCapPhy_TickInit();

    /***************************************************************************
    --------------- Demo of API "mTouchCapAPI_SetUp_Defaul" -----------------------
    Initialize the ADC module to defaults using the API "mTouchCapAPI_SetUp_Defaul".
    ***************************************************************************/
    for (Index=0; Index < NumberScanChannels; Index++)
    {
        mTouchCapAPI_SetUp_Default(Index);
    }

    mTouchCapPhy_StablizeChannelData();

}


/******************************************************************************
 * Function:        mTouchCapAPI_SetUp_Default(WORD ChannelNum)
 *
 * PreCondition:    None
 *
 * Input:           Channel Number
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Performs Initializations for internal variables.
 *
 * Note:            None
 *****************************************************************************/
void mTouchCapAPI_SetUp_Default(WORD ChannelNum)
{
    CurFiltdData[ChannelNum]  = 32767;  // init to zero the current raw data
    averageData[ChannelNum] = 32767; // init to zero the average data value
    tripValue[ChannelNum] = DEFAULT_TRIP_VALUE;  //default trip value for all the channels
}


/********************************************************************
* Function: BYTE mTouchCapAPI_SetUpChannelDirectKey(DIRECTKEY *Button,
*                                                   CHAR Channel_number,
*                                                   WORD Trip_Value,
*                                                   WORD Decode_Method,
*                                                   WORD Filter_Method)
*
* PreCondition: None
*
* Input:           Button : Object of the Direct key structure
*                  Channel number : channel number of the object Button
*                                   associated with corresponding direct key.
*                  Trip Value: Default Trip value for the channel specified
*                              by the channel number.
*                  Filter_Method: The filter method associated with the Direct Key.
*                      FILTER_METHOD_SLOWAVERAGE=0,
*                      FILTER_METHOD_GATEDAVERAGE,
*                      FILTER_METHOD_FASTAVERAGE
*
*                  Decode_Method : The Decode method associated with the Direct Key.
*                      DECODE_METHOD_MOST_PRESSED,
*                      DECODE_METHOD_MULTIPLE_PRESS,
*                      DECODE_METHOD_PRESS_AND_RELEASE,
*                      DECODE_METHOD_PRESS_ASSERT
*
* Output:          SetUpStatus - Status of the Direct key(TRUE or FALSE).
*
* Side Effects :    None
*
* Overview:    This API will setup the channel associated with the Direct key .
*              The channel number, filter type and decode method are stored in
*              the structure associated with the Direct Key.
*
* Note         :
*******************************************************************/
#if defined( USE_DIRECT_KEYS ) | defined ( USE_SLIDER_2CHNL )
void mTouchCapAPI_SetUpChannelDirectKey(DIRECTKEY *Button,
                                        BYTE Channel_number,
                                        WORD Trip_Value,
                                        WORD Decode_Method,
                                        WORD Filter_Method)
{
    Button->Channel_Num = Channel_number;  // Stores Channel Number
    Button->FilterType = Filter_Method;
    chFilterType[Channel_number] = Filter_Method;
    Button->DecodeMethod = Decode_Method;

    LoadChannelNumber(Channel_number);
}
#endif//defined( USE_DIRECT_KEYS )

/********************************************************************
* Function: BYTE mTouchCapAPI_SetUpChannelMatrixKey(MATRIXKEY *Button,
*                                                   BYTE Row_Channel_number,
*                                                   BYTE Col_Channel_number,
*                                                   WORD Trip_Value,
*                                                   WORD Decode_Method,
*                                                   WORD Filter_Method)
*
* PreCondition :   None
*
* Input        :   Button - Object of the Matrix key structure
*                  Row_Channel_number - channel number of the Row associated with corresponding matrix key.
*                  Col_Channel_number - channel number of the Column associated with corresponding matrix key.
*                  Trip Value - Default trip value for the channel associated with corresponding matrix key
*                  Filter_Method - The filter method associated with the corresponding Matrix key
*                  Decode_Method - The Decode method associated with the corresponding Matrix key
*
* Output       :   SetUpStatus - Status of the Matrix key(TRUE or FALSE).
*
* Side Effects :   None
*
* Overview     :   This API will setup the channels of the Row and Column associated with the Matrix key.
*                  The channel number of the Row and Column, filter type and decode method are stored in the structure associated with
*                  the corresponding channel.
*
*
* Note         :
*******************************************************************/
#if defined( USE_MATRIX_KEYS )
void mTouchCapAPI_SetUpChannelMatrixKey(MATRIXKEY *Button,
                                        BYTE Row_Channel_Number,
                                        BYTE Col_Channel_Number,
                                        WORD Trip_Value,
                                        WORD Decode_Method,
                                        WORD Filter_Method)
{
    Button->Channel_Num[0] = Row_Channel_Number;  // Stores Channel Number of the Row associated with the Matrix key
    Button->Channel_Num[1] = Col_Channel_Number;  // Stores Channel Number of the Column asscoiated with the Matrix key

    Button->FilterType = Filter_Method;   //Store the Filter type which will be common for both the multiplexed channel(Row and col)
    chFilterType [Row_Channel_Number] = Filter_Method;
    chFilterType [Col_Channel_Number] = Filter_Method;
    Button->DecodeMethod = Decode_Method; //Store the Decode method which will be common for both the multiplexed channel(Row and col)

    //store the channels numbers in the global array for all the Matrix Key channels

    //Store the Row and column channel numbers in the global array which will be used by the ISR
    // for reading the ADC value associated with the channels which are multiplxed to form a single key
    LoadChannelNumber(Row_Channel_Number); LoadChannelNumber(Col_Channel_Number);
}
#endif//defined( USE_MATRIX_KEYS )

/********************************************************************
* Function:    BYTE mTouchCapAPI_GetStatusMatrixButton (MATRIXKEY *Button)
*
* PreCondition: The Channels of the Row and Column associatd with the Matrix
*               key should have been already set up
*
* Input:     Button - Object of the Structure associated with the Matrix Key
*
* Output:    The Touch Status of the particular key.
*               0 = KEY_NOT_PRESSED
*               1 = KEY_PRESSED
*
* Side Effects:    None
*
* Overview:    This API will provide the status of the Matrix key passed which
*              will be used by the application to perform the related task.
*
* Note         :
*******************************************************************/
#if defined( USE_MATRIX_KEYS )
BYTE mTouchCapAPI_GetStatusMatrixButton (MATRIXKEY *Button)
{
    BYTE MatrixKey_Status = KEY_NOT_PRESSED;
    BYTE RowFlag = KEY_NOT_PRESSED;
    BYTE ColFlag = KEY_NOT_PRESSED;

    /* Check if the channel used as Row for corresponding Matrix key is pressed */
    //Now check to see if the requested row is pressed

    if( KEY_PRESSED == mTouchCapAPI_GetChannelTouchStatus((Button->Channel_Num[0]),(Button->DecodeMethod) ))
    {
        RowFlag=KEY_PRESSED;    // Used for determining ROW match
    }
    else
    {
        //Channel requested is not detected as pressed.
        RowFlag= KEY_NOT_PRESSED;
    }

    //Now check to see if the channel used as column for corresponding Matrix key is pressed
    if( KEY_PRESSED == mTouchCapAPI_GetChannelTouchStatus((Button->Channel_Num[1]),(Button->DecodeMethod) ))
    {
         ColFlag=KEY_PRESSED;   // Used for determining COL match
    }
    else
    {
        //Channel requested is not detected as pressed.
        ColFlag= KEY_NOT_PRESSED;
    }

    // check if both the row and column corresponding to the matrix key is pressed


    if( (ColFlag == KEY_PRESSED) && (RowFlag == KEY_PRESSED) )
    {
        MatrixKey_Status= KEY_PRESSED;      //matrix key is pressed
    }
    else
    {
        //If both channels are not pressed, the key is not pressed
        MatrixKey_Status= KEY_NOT_PRESSED; //key is not pressed
    }

    return MatrixKey_Status;
}

#endif//defined( USE_MATRIX_KEYS )


/******************************************************************************
* Function: BYTE mTouchCapAPI_SetUpChannelSlider4Ch(Slider4Ch *Slider,
*                                                   BYTE Slider_Channel1_Number,
*                                                   BYTE Slider_Channel2_Number,
*                                                   BYTE Slider_Channel3_Number,
*                                                   BYTE Slider_Channel4_Number,
*                                                   WORD Trip_Value,
*                                                   BYTE Decode_Method,
*                                                   BYTE Filter_Method)
*
* PreCondition: None
*
* Input:
*  Slider : object of the 4-channel Slider structure
*  Channel number : channel # of the object Slider associated with 4-channel slider.
*  Trip Value: Trip value for the channels associated with the 4-channel slider.
*  Filter_Method: The filter method for the 4-channel slider.
*  Decode_Method:The Decode method for the 4-channel slider.
*
* Output:   SetUpStatus - Status of the 4-channel slider(TRUE or FALSE).
*
* Side Effects :    None
*
* Overview:
*  This API will setup the 4 channels associated with the 4-channel Slider.
*  The 4 channel numbers, filter type and decode method are stored in the
*  structure associated with the corresponding 4-Channel Slider
*
* Note         :
******************************************************************************/
#if defined(USE_SLIDER_4CHNL)
void mTouchCapAPI_SetUpChannelSlider4Ch(SLIDER4CH *Slider,
                                        BYTE Slider_Channel1_Number,
                                        BYTE Slider_Channel2_Number,
                                        BYTE Slider_Channel3_Number,
                                        BYTE Slider_Channel4_Number,
                                        WORD Trip_Value,
                                        BYTE Decode_Method,
                                        BYTE Filter_Method)
{
    Slider->Channel_Num[0] = Slider_Channel1_Number;  // Stores first Channel Number of the 4-channel slider
    Slider->Channel_Num[1] = Slider_Channel2_Number;  // Stores Second Channel Number of the 4-channel slider
    Slider->Channel_Num[2] = Slider_Channel3_Number;  // Stores third  Channel Number of the 4-channel slider
    Slider->Channel_Num[3] = Slider_Channel4_Number;  // Stores fourth Channel Number of the 4-channel slider

    Slider->FilterType = Filter_Method;   //Store the Filter type which will be common for all the channels in the 4 channel slider

    chFilterType [Slider_Channel1_Number] = Filter_Method;
    chFilterType [Slider_Channel2_Number] = Filter_Method;
    chFilterType [Slider_Channel3_Number] = Filter_Method;
    chFilterType [Slider_Channel4_Number] = Filter_Method;


    Slider->DecodeMethod = Decode_Method; //Store the Decode method which will be common for all the channels in the 4 channel slider

    //store the channels numbers in the global array which are associated with the 4-channel slider

    //Store the Channel 1,Channel 2,Channel 3 and Channel4  numbers of the 4-channel slider in the global array which will be used by the ISR
    // for reading the ADC value associated

    LoadChannelNumber(Slider_Channel1_Number);
    LoadChannelNumber(Slider_Channel2_Number);
    LoadChannelNumber(Slider_Channel3_Number);
    LoadChannelNumber(Slider_Channel4_Number);
}
#endif//defined(USE_SLIDER_4CHNL)


/******************************************************************************
  Function       :  SHORT   mTouchCapAPI_GetStatusSlider4Ch (Slider4Ch *Slider)

  PreCondition   :   4-channel Slider setup is complete

 Input           :  Slider - Object of the 4-channel slider

 Output          :  sliderLevel gives the Slider percent level of the touch.

 Side Effects    :  None

 Overview        :
   This API gets the percentage level of a particular 4-channel slider passed.
   The output is ratio-metrically calculated from 0% to 100% proportional to
   the finger on the slider.


 *****************************************************************************/
#if defined(USE_SLIDER_4CHNL)
#include "mTouchCapApp_FourChanSlider.h"
SHORT  mTouchCapAPI_GetStatusSlider4Ch (SLIDER4CH *Slider)
{

    SHORT SliderState;
    WORD Percent_Level = 0;

    if( (KEY_PRESSED == mTouchCapAPI_GetChannelTouchStatus((Slider->Channel_Num[0]),(Slider->DecodeMethod) )) ||
        (KEY_PRESSED == mTouchCapAPI_GetChannelTouchStatus((Slider->Channel_Num[1]),(Slider->DecodeMethod) )) ||
        (KEY_PRESSED == mTouchCapAPI_GetChannelTouchStatus((Slider->Channel_Num[2]),(Slider->DecodeMethod) )) ||
        (KEY_PRESSED == mTouchCapAPI_GetChannelTouchStatus((Slider->Channel_Num[3]),(Slider->DecodeMethod) ))
      )
    {
        /* Use the 4ch slider algorithm to get the Slider Level */
        Percent_Level = mTouchCapApp_FourChannelSlider_Logic(Slider);
        SliderState = (SHORT)Percent_Level;
    }
    else
    {
        SliderState = -1; // For KEY_NOT_PRESSED
    }

      return SliderState;
 }
#endif // end of  #if defined(USE_SLIDER_4CHNL)

/******************************************************************************
* Function     :    void LoadChannelNumber (BYTE Channel_Number_Passed)
*
* PreCondition :    None
*
* Input        :    Channel Number
*
* Output       :    None
*
* Side Effects :    None
*
* Overview     :
* This function will load the active Touchkey Channel number in the global
* array ScanChannels[NumberScanChannels] and will also store the total channels
* used in the application.
*
* Note         :
******************************************************************************/
static void LoadChannelNumber (BYTE Channel_Number_Passed)
{
 BYTE index;
 BYTE result = FALSE;

    for(index = 0; index<NumberScanChannels; index++)
    {
       if(ScanChannels[index] == Channel_Number_Passed)
       {
         result = TRUE;
       }
    }

    if(!result)
    {
        NumberScanChannels++;
        NumberScanChannels = NumberScanChannels < MAX_ADC_CHANNELS ? NumberScanChannels : MAX_ADC_CHANNELS;
        ScanChannels[NumberScanChannels-1] = Channel_Number_Passed;
    }

}


/******************************************************************************
* Function: BYTE mTouchCapAPI_GetChannelTouchStatus(WORD ChIndex,
*                                                   BYTE Decode_Method)
*
* PreCondition:    None
*
* Input:    ChIndex -The Channel number.
*           Decode_Method-The type of Decode Method associated with that channel
*             DECODE_METHOD_MOST_PRESSED,
*             ECODE_METHOD_MULTIPLE_PRESS,
*             ECODE_METHOD_PRESS_AND_RELEASE,
*             ECODE_METHOD_PRESS_ASSERT
*
* Output: TouchStatus-Whether the key associated with the Channel is pressed or not
*           KEY_NOT_PRESSED,
*           KEY_PRESSED
*
* Side Effects: None
*
* Overview :
*  This API will determine if the channel which is associated with a particular
*  key is touched or not. It will output the pressed or unpressed status of the
*  channel based on the decode method which is associated with the channel.
*
* Note             :
******************************************************************************/
BYTE mTouchCapAPI_GetChannelTouchStatus(WORD ChIndex, BYTE Decode_Method)
{
    BYTE TouchStatus = KEY_NOT_PRESSED;

    /* Check for valid channel number being passed */
    if (ChIndex < MAX_ADC_CHANNELS )
    {
        switch (Decode_Method)
        {
            case DECODE_METHOD_MOST_PRESSED:

                if (CurFiltdData[ChIndex] < (averageData[ChIndex] - tripValue[ChIndex]))
                {
                    unpressedCount[ChIndex] = 0;
                    pressedCount[ChIndex] += 1;
                    // Single-Button Only (Lockout other keys option)
                    if(pressedCount[ChIndex] > DEBOUNCECOUNT)
                    {
                        Decode_Method_Used=DECODE_METHOD_MOST_PRESSED;
                        TouchStatus = ChIndex == iScanChanMinWeight ? KEY_PRESSED : KEY_NOT_PRESSED;
                    }
                }
                else
                {
                    pressedCount[ChIndex] = 0;
                    TouchStatus = KEY_NOT_PRESSED;
                }

                if (CurFiltdData[ChIndex] > (averageData[ChIndex] - tripValue[ChIndex] + hystValue[ChIndex]))
                {   // Is scaledBigValue above { average - (trip + hysteresis) } ? .. Yes
                    // Button is up.
                    unpressedCount[ChIndex] += 1;
                    if (unpressedCount[ChIndex] > DEBOUNCECOUNT)
                    {
                        TouchStatus = KEY_NOT_PRESSED;
                    }
                }
                else
                {
                    unpressedCount[ChIndex] = 0;
                }

                break;


            case DECODE_METHOD_MULTIPLE_PRESS:
            case DECODE_METHOD_PRESS_ASSERT:
                if (CurFiltdData[ChIndex] < (averageData[ChIndex] - tripValue[ChIndex]))
                {
                    unpressedCount[ChIndex] = 0;
                    pressedCount[ChIndex] += 1;
                    // Single-Button Only (Lockout other keys option)
                    if(pressedCount[ChIndex] > DEBOUNCECOUNT)
                    {
                       TouchStatus = KEY_PRESSED;
                       Decode_Method_Used = DECODE_METHOD_PRESS_ASSERT;
                    }
                }
                else
                {
                    pressedCount[ChIndex] = 0;
                    TouchStatus = KEY_NOT_PRESSED;
                }

                if (CurFiltdData[ChIndex] > (averageData[ChIndex] - tripValue[ChIndex] + hystValue[ChIndex]))
                {       // Is scaledBigValue above { average - (trip + hysteresis) } ? .. Yes
                        // Button is up.
                    unpressedCount[ChIndex] = unpressedCount[ChIndex] + 1;
                    if (unpressedCount[ChIndex] > DEBOUNCECOUNT)
                    {
                        TouchStatus = KEY_NOT_PRESSED;
                    }

                }
                else
                {
                    unpressedCount[ChIndex] = 0;
                }

                break;

            default:
                TouchStatus = KEY_NOT_PRESSED;
                break;

        }
    }

    return TouchStatus;
}
