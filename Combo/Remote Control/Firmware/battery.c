/*****************************************************************************
 *
 *              Battery level detection
 *
 *****************************************************************************
 * FileName:        battery.c
 * Dependencies:
 * Processor:       PIC24F
 * Compiler:        see Release Notes
 * Linker:          see Release Notes
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2007-2009 Microchip Technology Inc.  All rights reserved.
 *
 * Microchip licenses to you the right to use, modify, copy and distribute 
 * Software only when embedded on a Microchip microcontroller or digital 
 * signal controller and used with a Microchip radio frequency transceiver, 
 * which are integrated into your product or third party product (pursuant 
 * to the terms in the accompanying license agreement).   
 *
 * You should refer to the license agreement accompanying this Software for 
 * additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY 
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
 * PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
 * LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
 * CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
 * DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
 * ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
 * LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
 * TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
 * NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *****************************************************************************
 *
 *****************************************************************************/
#include "compiler.h"
#include "HardwareProfile.h"
#include "battery.h"

enum _BATTERY_STATES_
{
   BAT_MEAS_ADCREF,
   BAT_MEAS_VOLTAGE

} BATTERY_STATES;

volatile UINT16 batteryVoltageMv = 0;
volatile UINT16 adcRefVoltageMv = 0;
volatile UINT8  batteryState = BAT_MEAS_ADCREF;

void Battery_ADCInit(void)
{
    // Initialize ADC
    AD1CON1 = 0;                  // Reset
    AD1CON2 = 0;                  // AVdd, AVss, int every conversion, MUXA only
    AD1CON3 = 0x0101;             // 1 Tad auto-sample, Tad = 2*Tcy
    AD1CON1 = 0x08000;            // Turn on, single conversion required for mTouch	
}

SHORT BatteryDetectLevel(void)
{
    switch(batteryState)
    {
        case BAT_MEAS_ADCREF:
            AD1CHS = BAT_BANDGAP_CHANNEL; // switch to band gap channel    
            AD1CON1bits.SAMP = 1;
            
            // needs delay to charge up capacitor to measure the battery voltage
            // accurately (at least 1 A/D sample cycle). If the delay is less than 1 A/D cycle
            // the DONE bit may not set (See A/D Family Reference Manual for details)
            Nop();Nop();Nop();Nop();Nop();
            Nop();Nop();Nop();Nop();Nop();
            Nop();Nop();Nop();Nop();Nop();
            Nop();Nop();Nop();Nop();Nop();

            AD1CON1bits.SAMP = 0;
	        while(AD1CON1bits.DONE == 0);
            adcRefVoltageMv = (UINT16)((UINT32)BAT_BANDGAP_MV*1024/ADC1BUF0);
           
            AD1CHS = BAT_LEVEL_CHANNEL; // switch to band gap channel    
            AD1CON1bits.SAMP = 1; // start sampling (takes about 60uS)

            batteryState = BAT_MEAS_VOLTAGE;
            return 0;


        case BAT_MEAS_VOLTAGE:
            AD1CON1bits.SAMP = 0;
	        while(AD1CON1bits.DONE == 0);
            batteryVoltageMv = (UINT16)((UINT32)adcRefVoltageMv*ADC1BUF0/(1024*2)); // voltage of one AAA battery

            batteryState = BAT_MEAS_ADCREF;
            return 1;

       default:
            batteryState = BAT_MEAS_ADCREF;
            return 1;
    }
}

UINT16 GetBatteryLevel(void)
{
INT16 batLevel;

    batLevel = batteryVoltageMv - BAT_FULLY_DISCHARGED;

    if(batLevel < 0)
    {
        batLevel = 0;
    }
    if(batLevel > GetBatteryRange())
    {
       batLevel = GetBatteryRange();
    }

    // reversing the value since the progress bar used to display the battery level
    // operatates in the opposite direction. So as long as the text is not displayed 
    // (the percentage value, we can mimic the operation where the battery level is full
    // when the value of the progress bar is zero.
    batLevel = GetBatteryRange() - batLevel;

    return (UINT16) batLevel;
}

UINT16 GetBatteryRange(void)
{
    return (BAT_FULLY_CHARGED-BAT_FULLY_DISCHARGED);
}
