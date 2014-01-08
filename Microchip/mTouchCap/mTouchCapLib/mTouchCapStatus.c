/*****************************************************************************
* FileName:         mTouchCapStatus.c
*
* Includes:
*   void mTouchCapStatus_Reset(UINT16 SliderValue)
*   void mTouchCapStatus_Capture(void)
*   void mTouchCapStatus_Restore(void)
*   void mTouchCapStatus_SetEmTouchDataReady( UINT8 FlagValue )
*   UINT16 mTouchCapStatus_Check(UINT16 *pCurrentButtonStatus,
*                            UINT16 *pCurrentButtonAsserts,
*                            UINT16 *pSliderValue)
*
* Dependencies:     ??
* Processor:        PIC32
* Compiler:         C32
* Linker:           MPLAB LINK32
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
*
* Author       Date        Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MWM          21 Jul 2011  Moved from main file to this file for simplicity
*******************************************************************************/

#include "GenericTypeDefs.h"
#include "mTouchCapStatus.h"
#include "HardwareProfile.h"
#include "mTouchCapAPI.h"
#include "mTouchCapPhy.h"
#if defined( DUMP_DIAGNOSTICS_VIA_UART )
#include  <stdio.h>
#endif
#include "mTouchCapApp_DirectKeys.h"

/** External Variables **/
// From mTouchCapAPI.c
extern volatile UINT8 EmTouchDataReady;
extern UINT8  HF_Read_Number;
extern UINT8  NumberScanChannels;
extern UINT16 ScanChannels[MAX_ADC_CHANNELS];
extern UINT16 averageData[MAX_ADC_CHANNELS];   // running average of CVD channels

// from mTouchCapPhy.c
extern UINT16 CurFiltdData[MAX_ADC_CHANNELS]; // current filtered value for all channels

// Status static storage
static UINT16 LastButtonStatus = 0;
static  INT16 LastSliderValue  = 0;
static UINT16 AvgDataCapture[MAX_ADC_CHANNELS];

// From mTouchCapPhy.c
#if defined( DUMP_DIAGNOSTICS_VIA_UART )
void SendDataBuffer(const char *buffer, UINT32 size);

#if   defined( UART_DUMP_RAW_COUNTS )
UINT16 CurRawData[  NUM_HF_READS][MAX_ADC_CHANNELS]; // Scratch storage for UART dump
#elif defined( UART_DUMP_ALL_COUNTS )
UINT16 CurRawData[3*NUM_HF_READS][MAX_ADC_CHANNELS]; // Scratch storage for UART dump
#endif

#endif//defined( DUMP_DIAGNOSTICS_VIA_UART )


void mTouchCapStatus_Reset(UINT16 SliderValue)
{
    LastButtonStatus = 0;
    LastSliderValue  = SliderValue;
}

void mTouchCapStatus_Capture(void)
{
    UINT16 iScanChan, iButton;
    for(iButton=0; iButton<NumberScanChannels; iButton++)
    {
        iScanChan = ScanChannels[iButton];
        AvgDataCapture[iScanChan] = averageData[iScanChan];
    }
}

void mTouchCapStatus_Restore(void)
{
    UINT16 iScanChan, iButton;
    for(iButton=0; iButton<NumberScanChannels; iButton++)
    {
        iScanChan = ScanChannels[iButton];
        averageData[iScanChan]  = AvgDataCapture[iScanChan];
        CurFiltdData[iScanChan] = AvgDataCapture[iScanChan];
        mTouchCapPhy_TripValCalc(iScanChan);
    }
}

void mTouchCapStatus_SetEmTouchDataReady( UINT8 FlagValue )
{
    EmTouchDataReady = FlagValue;
}

UINT16 mTouchCapStatus_Check(UINT16 *pCurrentButtonStatus,
                             UINT16 *pCurrentButtonAsserts,
                             UINT16 *pSliderValue)
{
#if defined( DUMP_DIAGNOSTICS_VIA_UART )
    char   ButtonMeasString[133];
#  if defined ( UART_DUMP_RAW_COUNTS )
    UINT16 iHF_Read;
#  endif
#endif
    UINT16 iButton,
           WeightSum,
           RawSliderValue = 0,
           WeightDeviation,
           FullScalePerButton = 256/NumberScanChannels; //256 counts = 100%
    INT16  CorrectedSliderValue = 0, SliderValue;
    UINT16 CurrentButtonMeasurements[MAX_ADC_CHANNELS],
           CurrentAveragedMeasurements[MAX_ADC_CHANNELS],
           CurrentWeights[MAX_ADC_CHANNELS];

    if(EmTouchDataReady == 1)  //This flag is set by Timer ISR when all channels have been read
    {
        //  Calculate new button values and return the measurement updates
        mTouchCapPhy_UpdateData(CurrentButtonMeasurements,
                                CurrentAveragedMeasurements,
                                CurrentWeights,
                                TRUE, FILTER_METHOD_USECHANNEL);

        // Update button and LED status and return button status for use with mTouch GUI on PC
        *pCurrentButtonStatus = mTouchCapApp_DirectKeys_Demo();

        // Check for button asserts: If buttons were pressed before, but none pressed now
        if ( *pCurrentButtonStatus == 0 && LastButtonStatus > 0 )
        {
            *pCurrentButtonAsserts = LastButtonStatus; //Buttons have fired.
        }
        else
        { //No buttons asserted
           //*pCurrentButtonAsserts = 0;
        }

        if ( *pCurrentButtonStatus > 0 ) // Something is happening on the buttons
        {
            // Calculate slider value for buttons as weighted average of weight deviation from max
            WeightSum = 0;
            RawSliderValue = 0;
            for ( iButton = 0; iButton < NumberScanChannels ; iButton++ )
            {
                WeightDeviation = 256 - CurrentWeights[iButton];
                WeightSum   += WeightDeviation;
                RawSliderValue += WeightDeviation*(iButton+1); // Buttons in natural order
              //RawSliderValue += WeightDeviation*(NumberScanChannels-iButton); // Buttons in reversed order
            }
            // Average Weight when 256 counts is full scale (100%)
            RawSliderValue = ((256*RawSliderValue)/NumberScanChannels)/WeightSum;
            // Correct for 1st button, 256 counts = 100%
            CorrectedSliderValue = (NumberScanChannels*((INT16)RawSliderValue-FullScalePerButton))/(NumberScanChannels-1);
            CorrectedSliderValue = (CorrectedSliderValue <  0 ) ?   0 : CorrectedSliderValue;
            CorrectedSliderValue = (CorrectedSliderValue > 256) ? 256 : CorrectedSliderValue;
        }
        else // Nothing is happening, put slider to sleep
        {
            //RawSliderValue = 0;
            SliderValue = LastSliderValue;
        }
        SliderValue = ( CorrectedSliderValue + ((1<<2) - 1)*LastSliderValue )>>2;
        LastSliderValue  = SliderValue;
        *pSliderValue    = (UINT16)SliderValue;

#      if defined( UART_DUMP_RAW_COUNTS )
        for ( iHF_Read = 0; iHF_Read <   NUM_HF_READS; iHF_Read++ )
#      elif defined( UART_DUMP_ALL_COUNTS )
        for ( iHF_Read = 0; iHF_Read < 3*NUM_HF_READS; iHF_Read++ )
#      endif
#      if defined( UART_DUMP_RAW_COUNTS ) || defined( UART_DUMP_ALL_COUNTS )
        {
            sprintf(ButtonMeasString,
                    "%05d,%05d,%05d,%05d,"
                    "%05d,%05d,%05d,%05d,"
                    "\r\n",
                    iHF_Read+1,*pCurrentButtonStatus,*pCurrentButtonAsserts,RawSliderValue,
                    CurRawData[iHF_Read][0],CurRawData[iHF_Read][1],CurRawData[iHF_Read][2],CurRawData[iHF_Read][3]);
            SendDataBuffer(ButtonMeasString, strlen(ButtonMeasString) );
        }
#      endif

#      if defined( DUMP_DIAGNOSTICS_VIA_UART )
        sprintf( ButtonMeasString,
                 "%05d,%05d,%05d,."
                 "%05d,%05d,%05d,%05d,"
                 "%05d,%05d,%05d,%05d,"
                 "%05d,%05d,%05d"
                 "\r\n",
                 *pCurrentButtonStatus, *pCurrentButtonAsserts, RawSliderValue,
                 CurrentButtonMeasurements[0], CurrentAveragedMeasurements[0],
                 CurrentButtonMeasurements[1], CurrentAveragedMeasurements[1],
                 CurrentButtonMeasurements[2], CurrentAveragedMeasurements[2],
                 CurrentButtonMeasurements[3], CurrentAveragedMeasurements[3],
                 RawSliderValue, CorrectedSliderValue, SliderValue );
        SendDataBuffer(ButtonMeasString, strlen(ButtonMeasString) );
#      endif//defined( DUMP_DIAGNOSTICS_VIA_UART )


        LastButtonStatus = *pCurrentButtonStatus; //Remember button status

        return 1; // Something happened
    }
    else
    {
        return 0; // Nothing happened
    } // end if(EmTouchDataReady)

}

