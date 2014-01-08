/******************************************************************************
* File Name:       TimerCallbackFunc2.c
* Includes:
*   void TimerCallbackFunc2(void)
*
* Dependencies:    None
* Processor:       PIC32MX795F512H
* Compiler:        C32
* Company:         Microchip Technology, Inc.
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

// Check if I/O pads on device supports better algorithm
#if defined(__32MX534F064H__) || \
    defined(__32MX534F064L__) || \
    defined(__32MX564F064H__) || \
    defined(__32MX564F064L__) || \
    defined(__32MX564F128H__) || \
    defined(__32MX564F128L__) || \
    defined(__32MX664F064H__) || \
    defined(__32MX664F064L__) || \
    defined(__32MX664F128H__) || \
    defined(__32MX664F128L__) || \
    defined(__32MX764F128H__) || \
    defined(__32MX764F128L__)
#   warning('You can use TimerCallbackFunc1.')
#endif

#if defined( _CTMU_IRQ )
#   warning("Devices has CTMU, use TimerCallbackFunc4 instead!")
#endif


  #define ChargeDelay()  {} // No delay
//#define ChargeDelay()  asm("NOP") // 12.5 ns
//#define ChargeDelay()  asm("NOP");asm("NOP") // 25 ns
//#define ChargeDelay()  asm("NOP");asm("NOP");asm("NOP");asm("NOP") // 50 ns

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

// FromTouchCapADC.c
extern UINT16 PortPinADC[MAX_ADC_CHANNELS];

// From mTouchCapTmr.c
extern volatile BOOL IgnoreCurrentDataFlag; // Flag set by application to signal when mTouch
                                            // should ignore data being measured.

// Locals
#if defined( __32MX220F032D__ ) || defined( __32MX250F128D__ )
static UINT16 OldRawData[MAX_ADC_CHANNELS] = { 0x1FFF, 0x1FFF, 0x1FFF, 0x1FFF,
                                               0x1FFF, 0x1FFF, 0x1FFF, 0x1FFF,
                                               0x1FFF, 0x1FFF, 0x1FFF, 0x1FFF,
                                               0x1FFF }; // raw value from previous set of ADC scans
#else
static UINT16 OldRawData[MAX_ADC_CHANNELS] = { 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                                               0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                                               0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                                               0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF }; // raw value from previous set of ADC scans
#endif//defined( __32MX220F032D__ ) || defined( __32MX250F128D__ )

/*
    Timer ISR callback function for CVD measurements.
    Requires external pull-up resistors to Vdd for all buttons.
    For use on devices that don't support TimerCallbackFunc1
    or have a CTMU (TimerCallbackFunc4).
 */
void TimerCallbackFunc2(void)
{
    UINT16 Vpos, Vneg, Vmeas, Vupdate;
    UINT16 iChan, iChanBit;
    UINT16 iRefChan, iRefBit;

    iRefChan = REF_CHANNEL;
    iRefBit = 1<<iRefChan;

    iChan = ScanChannels[button_set_number];
    iChanBit = 1<<iChan;

//*****************************************************************************
// Measure Vneg
//*****************************************************************************
#if defined( TICKLE_LED1 )
    LATFSET = 1<<0; // Use pin 5 on J2 (RF0)
#endif// defined( TICKLE_LED1 )

    asm("di"); // disable interrupts

    AD1CON1bits.SAMP = 1; //Start Chold charging Process
    ChargeDelay();
    LATBCLR = iRefBit;    //Ref pin now low
    ChargeDelay();
    AD1CHSbits.CH0SA = iRefChan; //Switch ADC to grounded reference pin
    ChargeDelay();        //Wait as ADC SAH cap discharges and sensor cap charges

    TRISBSET = iChanBit; //Tri-state sensor port
    LATBCLR  = iChanBit; //Sensor port low

#if defined(CHARGE_DISCHARGE_CYCLE_1)
    AD1CHSbits.CH0SA = iChan;    //Switch ADC to sensor pin
    ChargeDelay();               //Wait as ADC SAH charges
    AD1CHSbits.CH0SA = iRefChan; //Switch ADC to grounded reference pin
    ChargeDelay();               //Wait as ADC SAH discharges
#endif//defined(CHARGE_DISCHARGE_CYCLE_1)

#if defined(CHARGE_DISCHARGE_CYCLE_2)
    AD1CHSbits.CH0SA = iChan;    //Switch ADC to sensor pin
    ChargeDelay();               //Wait as ADC SAH charges
    AD1CHSbits.CH0SA = iRefChan; //Switch ADC to grounded reference pin
    ChargeDelay();               //Wait as ADC SAH discharges
#endif//defined(CHARGE_DISCHARGE_CYCLE_2)

#if defined(CHARGE_DISCHARGE_CYCLE_3)
    AD1CHSbits.CH0SA = iChan;    //Switch ADC to sensor pin
    ChargeDelay();               //Wait as ADC SAH charges
    AD1CHSbits.CH0SA = iRefChan; //Switch ADC to grounded reference pin
    ChargeDelay();               //Wait as ADC SAH discharges
#endif//defined(CHARGE_DISCHARGE_CYCLE_3)

#if defined(CHARGE_DISCHARGE_CYCLE_4)
    AD1CHSbits.CH0SA = iChan;    //Switch ADC to sensor pin
    ChargeDelay();               //Wait as ADC SAH charges
    AD1CHSbits.CH0SA = iRefChan; //Switch ADC to grounded reference pin
    ChargeDelay();               //Wait as ADC SAH discharges
#endif//defined(CHARGE_DISCHARGE_CYCLE_4)

    AD1CHSbits.CH0SA = iChan; //Switch ADC to sensor pin
    ChargeDelay();            //Wait as ADC SAH charges
    AD1CON1bits.SAMP = 0;     //Start A/D conversion

#if defined( TICKLE_LED1 )
    LATFCLR = 1<<0; // Use pin 5 on J2 (RF0)
#endif// defined( TICKLE_LED1 )

    asm("ei"); //re-enable interrupts
    //Capture Voltage
    while ( !AD1CON1bits.DONE ) //Wait for ADC conversion complete
    {
        // Do Nothing
    }
    AD1CON1bits.DONE = 0;
    Vneg = ReadADC10(ReadActiveBufferADC10())<<5; //Read new Vneg

//*****************************************************************************
// Measure Vpos
//*****************************************************************************
    //Be sure ADC is in known state
    TRISBCLR = iRefBit;          //Make ref pin output
    AD1CHSbits.CH0SA = iRefChan; //Switch ADC to ref pin
    AD1CON1bits.SAMP = 1;        //Start Chold charging Process
    ChargeDelay();

#if defined( TICKLE_LED1 )
    LATFSET = 1<<0; // Use pin 5 on J2 (RF0)
#endif// defined( TICKLE_LED1 )

    asm("di"); // disable interrupts

    //Setup: SAH cap high, sensor cap low
    LATBCLR  = iChanBit; //Sensor pin low
    TRISBCLR = iChanBit; //Clear tri-state on sensor pin

    //Charging the ADC's sample and hold (SAH) capacitor
    LATBSET  = iRefBit; //Make ref pin high
    TRISBCLR = iRefBit; //Make ref pin output

#if defined( CHARGE_DISCHARGE_CYCLE_1 )
    AD1CHSbits.CH0SA = iRefChan; //Switch ADC to ref pin
    ChargeDelay();               //Wait as ADC SAH cap charges
    TRISBSET = iChanBit;         //Tristate sensor pad (make it input)
    AD1CHSbits.CH0SA = iChan;    //Switch ADC to sensor pin
    ChargeDelay();                //Wait as ADC SAH cap discharges to sensor cap
#endif//defined( CHARGE_DISCHARGE_CYCLE_1 )

#if defined( CHARGE_DISCHARGE_CYCLE_2 )
    AD1CHSbits.CH0SA = iRefChan; //Switch ADC to ref pin
    ChargeDelay();               //Wait as ADC SAH cap charges
    AD1CHSbits.CH0SA = iChan;    //Switch ADC to sensor pin
    ChargeDelay();               //Wait as ADC SAH cap discharges to sensor cap
#endif//defined( CHARGE_DISCHARGE_CYCLE_2 )

#if defined( CHARGE_DISCHARGE_CYCLE_3 )
    AD1CHSbits.CH0SA = iRefChan; //Switch ADC to ref pin
    ChargeDelay();               //Wait as ADC SAH cap charges
    AD1CHSbits.CH0SA = iChan;    //Switch ADC to sensor pin
    ChargeDelay();               //Wait as ADC SAH cap discharges to sensor cap
#endif//defined( CHARGE_DISCHARGE_CYCLE_3 )

#if defined( CHARGE_DISCHARGE_CYCLE_4 )
    AD1CHSbits.CH0SA = iRefChan; //Switch ADC to ref pin
    ChargeDelay();               //Wait as ADC SAH cap charges
    AD1CHSbits.CH0SA = iChan;    //Switch ADC to sensor pin
    ChargeDelay();               //Wait as ADC SAH cap discharges to sensor cap
#endif//defined( CHARGE_DISCHARGE_CYCLE_4 )

    //Capture Voltage
    AD1CON1bits.SAMP = 0; //Start A/D conversion

#if defined( TICKLE_LED1 )
    LATFCLR = 1<<0; // Use pin 5 on J2 (RF0)
#endif// defined( TICKLE_LED1 )

    asm("ei"); //re-enable interrupts

    while ( !AD1CON1bits.DONE ) //Wait for ADC conversion complete
    {
        //Do Nothing
    }
    AD1CON1bits.DONE = 0;
    Vpos = ReadADC10(ReadActiveBufferADC10())<<5; //Read new Vpos

#if defined(USE_DIFFERENTIAL_MEASUREMENTS)
    Vmeas = Vpos + 32768 - Vneg; //Calculate new Vmeas
#else  //single ended
    Vmeas = Vpos;                //Single ended measurements
#endif //defined USE_DIFFERENTIAL_MEASUREMENTS


#if defined(LIMIT_SLEW_RATE)
    if ( IgnoreCurrentDataFlag == TRUE )
    {
        Vpos = Vneg = 0;
        Vmeas = OldRawData[button_set_number];
    }

# if defined(USE_DIFFERENTIAL_MEASUREMENTS)
#   define VMEAS_DELTA (1<<(5))
# else
#   define VMEAS_DELTA (1<<(6))
# endif//defined(USE_DIFFERENTIAL_MEASUREMENTS)
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
        Vmeas = Vpos = Vneg = 0;
        Vupdate = OldRawData[button_set_number];
    }
    else
    {
        OldRawData[button_set_number] = Vupdate = Vmeas;
    }

#endif//defined(LIMIT_SLEW_RATE)

    ButtonCumulativeSum[button_set_number] += Vupdate;

#if defined( UART_DUMP_RAW_COUNTS )
    CurRawData[  HF_Read_Number  ][button_set_number] = Vmeas;
#elif defined( UART_DUMP_ALL_COUNTS )
    CurRawData[3*HF_Read_Number+0][button_set_number] = Vpos;
    CurRawData[3*HF_Read_Number+1][button_set_number] = Vneg;
    CurRawData[3*HF_Read_Number+2][button_set_number] = Vmeas;
#endif//defined( UART_DUMP_RAW_COUNTS )

    //Measurement teardown
    LATBCLR  = iChanBit; //Make sensor port low to discharge sensor pad

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
