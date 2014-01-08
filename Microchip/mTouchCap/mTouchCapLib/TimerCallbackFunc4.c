/******************************************************************************
* File Name:       Timer4CallbackFunc4.c
* Includes:
*   void TimerCallbackFunc4(void)
*
* Dependencies:    None
* Processor:       PIC32MX220F032
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
*
* Author    Date    Comments
*-------------------------------------------------------------------------
* BDB    26-Jun-2008 First release
* NK     24-Apr-2009 Porting for 18F46J50 Eval Board
* MC     22-Ian-2010 First release of TMRDriver.c, TMRDriver.h
*                    Porting for PIC32MX795F512H
* MWM    06-Sep-2011 Moved Callback to separate file.
******************************************************************************/
#include "mTouchConfig.h"
#include "p32xxxx.h"

// Check that part has CTMU module
#if !defined( _CTMU_IRQ )
#   error('This part doesn''t have a CTMU module!')
#endif

// From  mTouchCapAPI.c
extern  UINT8 HF_Read_Number;          //# of current HF scan
extern UINT16 ScanChannels[MAX_ADC_CHANNELS];
extern  UINT8 NumberScanChannels;                   // store the index of the channels that has to be scanned
extern volatile UINT8 EmTouchDataReady;      //global indicating the reading of all channels finished
extern  UINT8 button_set_number;       //# of current button in scan

// from mTouchCapPhy.c
extern UINT32 ButtonCumulativeSum[MAX_ADC_CHANNELS]; // to compute average
#if   defined( UART_DUMP_RAW_COUNTS )
extern UINT16 CurRawData[  NUM_HF_READS][MAX_ADC_CHANNELS]; // Scratch storage for UART dump
#elif defined( UART_DUMP_ALL_COUNTS )
extern UINT16 CurRawData[3*NUM_HF_READS][MAX_ADC_CHANNELS]; // Scratch storage for UART dump
#endif

// From mTouchCapADC.c
extern UINT16 PortPinADC[MAX_ADC_CHANNELS];

// From mTouchCapTmr.c
extern volatile BOOL IgnoreCurrentDataFlag; // Flag set by application to signal when mTouch
                                            // should ignore data being measured.

// Locals
#if defined( __32MX220F032D__ ) || defined( __32MX250F128D__ )
static UINT16 OldRawData[MAX_ADC_CHANNELS] = { 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                                               0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                                               0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                                               0x7FFF }; // raw value from previous set of ADC scans
#else
static UINT16 OldRawData[MAX_ADC_CHANNELS] = { 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                                               0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                                               0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                                               0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF }; // raw value from previous set of ADC scans
#endif//defined( __32MX220F032D__ ) || defined( __32MX250F128D__ )

/*
    This version of the call back function uses the CTMU module.
 */
void TimerCallbackFunc4(void)
{
    UINT16 Vpos, Vmeas, Vupdate;
    UINT16 iChan, PortPinChan, iPort, iPin, iPinBit;

    iChan = ScanChannels[button_set_number];
    PortPinChan = PortPinADC[iChan];
    iPort   = (PortPinChan>>4) & 0x0F;
    iPin    =  PortPinChan & 0x0F;
    iPinBit = 1<<iPin;

//*****************************************************************************
// Measure Vpos
//*****************************************************************************
    AD1CHSbits.CH0SA = iChan; // Switch ADC to sensor pin

    switch ( iPort )
    {
        case 0xA:
            TRISASET = iPinBit;// Sensor pin tristated, now input
            break;
        case 0xB:
            TRISBSET = iPinBit;// Sensor pin tristated, now input
            break;
        case 0xC:
            TRISCSET = iPinBit;// Sensor pin tristated, now input
            break;
    }
    ChargeDelay();


#if defined( TICKLE_LED1 )
  //if ( button_set_number == 3 )
//  PMADDRSET = 1<<5;
//  LATCSET = 1<<8;           // Turn on LED1 on Pin RPC8
    LATCSET = 1<<5;           // Turn on LED1 on Pin RPC5
#endif//defined( TICKLE_LED1 )
    asm("di"); // Disable interrupts

    AD1CON1bits.SAMP = 1;     // Start ADC sampling
    CTMUCONbits.IDISSEN = 1;  // Ground charge pump

    // Four ChargeDelay's seem necessary for stable behavior
    ChargeDelay();            // Wait as button discharges
    ChargeDelay();            // Wait as button discharges
    ChargeDelay();            // Wait as button discharges
    ChargeDelay();            // Wait as button discharges

    CTMUCONbits.IDISSEN = 0;  // Unground charge pump

    ChargeDelay();
    CTMUCONbits.EDG1STAT = 1; // Start charging button

    CTMU_CHARGE_DELAYS;

    CTMUCONbits.EDG1STAT = 0; // Stop charging button
    AD1CON1bits.SAMP = 0;     // Start A/D conversion
#if defined( TICKLE_LED1 )
//  PMADDRCLR = 1<<5;
//  LATCCLR = 1<<8;           // Turn off LED1 on Pin RPC8
    LATCCLR = 1<<5;           // Turn off LED1 on Pin RPC5
#endif//defined( TICKLE_LED1 )

    //Wait for ADC conversion complete
    //Enable interrupts while we wait for ADC to complete
    asm("ei");
    while ( !AD1CON1bits.DONE )
    {
        //Do Nothing
    }
    AD1CON1bits.DONE = 0;
    CTMUCONbits.IDISSEN = 1;  // Ground charge pump
    switch ( iPort )
    {
        case 0xA:
            TRISACLR = iPinBit;// Sensor pin now output to discharge button
            break;
        case 0xB:
            TRISBCLR = iPinBit;// Sensor pin now output to discharge button
            break;
        case 0xC:
            TRISCCLR = iPinBit;// Sensor pin now output to discharge button
            break;
    }

//Read new Vpos
    Vpos = ReadADC10(ReadActiveBufferADC10())<<6;

#if defined(LIMIT_SLEW_RATE)
    if ( IgnoreCurrentDataFlag == TRUE )
    {
        Vpos = 0;
        Vmeas = OldRawData[button_set_number];
    }
    else
    {
        Vmeas = Vpos;
    }

#   define VMEAS_DELTA (1<<(6))
    if     (Vmeas > OldRawData[button_set_number])
    {
       OldRawData[button_set_number] += VMEAS_DELTA;
    }
    else if(Vmeas < OldRawData[button_set_number])
    {
       OldRawData[button_set_number] -= VMEAS_DELTA;
    }
    Vupdate = OldRawData[button_set_number];

#else
    if ( IgnoreCurrentDataFlag == TRUE )
    {
        Vmeas = Vpos = 0;
        Vupdate = OldRawData[button_set_number];
    }
    else
    {
        OldRawData[button_set_number] = Vupdate = Vpos;
    }

#endif//defined(LIMIT_SLEW_RATE)

    ButtonCumulativeSum[button_set_number] += Vupdate;

#if defined( UART_DUMP_RAW_COUNTS )
    CurRawData[  HF_Read_Number  ][button_set_number] = Vmeas;
#elif defined( UART_DUMP_ALL_COUNTS )
    CurRawData[3*HF_Read_Number+0][button_set_number] = Vpos;
    CurRawData[3*HF_Read_Number+1][button_set_number] = 0; // No Vneg
    CurRawData[3*HF_Read_Number+2][button_set_number] = Vmeas;
#endif//defined( UART_DUMP_RAW_COUNTS )

    //Next Measurement??
    button_set_number++; //Increment the button number to move on to the next button
    if(button_set_number == NumberScanChannels) //if scanned all the buttons
    {
        HF_Read_Number++;      //Start the next set of HF Scans
        button_set_number = 0; //Start over at the first button
        if ( HF_Read_Number < NUM_HF_READS ) // Not done with scans
        {
            EmTouchDataReady = FALSE;
        }
        else  //All HF scans done, compute average, signal we're done
        {
            EmTouchDataReady = TRUE; // We're done with complete scan
            HF_Read_Number = 0;  // Start next set of HF scans
        }
    }
    else
    {
        EmTouchDataReady = FALSE;
    }

    return; // we're done!
}
