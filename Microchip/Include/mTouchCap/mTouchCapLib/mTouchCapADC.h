/******************************************************************************
* FileName:        mTouchCap_Adc.h
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
* TERMS AND THE TERMS OF THE ACCOMPANYING CLICK-WRAP SOFTWARE LICENSE
*
*.Change History:
* Author     Date          Comments
*--------------------------------------------------------------------------------
* MC        22-Jan-2010   First release of ADCDriver.c, ADCDriver.h
*                         Porting for PIC32MX795F512H
* MWM       28 Mar 2011   Removed ADC ISR, capture now in Timer 4 ISR.
******************************************************************************/

#ifndef MTOUCHCAPADC_H
#define MTOUCHCAPADC_H

#define ADC_CHANNEL_SELECT      AD1CHS

/************************************************************************
Macro       : Set_Adc_Enable_State
Overview    : This function enables or disable the ADC module depending upon the value.
Parameters  : None
Remarks     : None.
**************************************************************************/
#define Set_Adc_Enable_State(Value)          (AD1CON1bits.ADON = Value)

/* initialization of ADC driver */
void mTouchCapADC_InitADC(void);

enum
{
    CHANNEL_AN0 = 0,
    CHANNEL_AN1    ,
    CHANNEL_AN2    ,
    CHANNEL_AN3    ,
    CHANNEL_AN4    ,
    CHANNEL_AN5    ,
    CHANNEL_AN6    ,
    CHANNEL_AN7    ,
    CHANNEL_AN8    ,
    CHANNEL_AN9    ,
    CHANNEL_AN10   ,
    CHANNEL_AN11   ,
    CHANNEL_AN12   ,
    CHANNEL_AN13   ,
    CHANNEL_AN14   ,
    CHANNEL_AN15
};


#endif
