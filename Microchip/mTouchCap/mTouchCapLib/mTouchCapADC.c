/******************************************************************************
* File Name:       mTouchCapAdc.c
* Includes:
*   void mTouchCapADC_InitADC(void )
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
* Author    Date          Comments
*--------------------------------------------------------------------------------
* MC        22-Jan-2010   First release of ADCDriver.c, ADCDriver.h
*                         Porting for PIC32MX795F512H
* MWM       28 Mar 2011   Removed ADC ISR, capture now in Timer 4 ISR.
********************************************************************************/
#include "mTouchConfig.h"
#include "HardwareProfile.h"
#include "GenericTypeDefs.h"
#include "mTouchCapADC.h"
#include "mTouchCapAPI.h"

// Definition of PortPinADC matrix, assigning Port-Pin to every ADC channel.
#if   defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
      defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )

UINT16 PortPinADC[MAX_ADC_CHANNELS] = { 0xB0, 0xB1, 0xB2, 0xB3,
                                        0xB4, 0xB5, 0xB6, 0xB7,
                                        0xB8, 0xB9, 0xBA, 0xBB,
                                        0xBC, 0xBD, 0xBE, 0xBF };
#define ADC_CHAN_SELECT AD1CHS0
#define ADC_PORT_CONFIG AD1PCFGL

#elif defined( PIC32MC764F128_EXPLORER_16_BOARD ) || \
      defined( PIC32MX795_CAP_TOUCH_EVAL_BOARD )  || \
      defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )

#define ADC_CHAN_SELECT AD1CHS
#define ADC_PORT_CONFIG AD1PCFG

UINT16 PortPinADC[MAX_ADC_CHANNELS] = { 0xB0, 0xB1, 0xB2, 0xB3,
                                        0xB4, 0xB5, 0xB6, 0xB7,
                                        0xB8, 0xB9, 0xBA, 0xBB,
                                        0xBC, 0xBD, 0xBE, 0xBF };

#elif defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD ) || \
      defined( PIC32MX220_STARTER_KIT_BOARD )

#define ADC_CHAN_SELECT AD1CHS
#define ADC_PORT_CONFIG AD1PCFG

UINT16 PortPinADC[MAX_ADC_CHANNELS] = { 0xA0, 0xA1, 0xB0, 0xB1,
                                        0xB2, 0xB3, 0xC0, 0xC1,
                                        0xC2, 0xBF, 0xBE, 0xBD,
                                        0xC3 };

#endif

#if defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD )   || \
    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD ) || \
    defined( PIC32MC764F128_EXPLORER_16_BOARD )  || \
    defined( PIC32MX795_CAP_TOUCH_EVAL_BOARD )   || \
    defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )

void mTouchCapADC_InitADC(void)
{
//*****************************************************************************
// ADC Setup
//*****************************************************************************

    // configure and enable the ADC
    AD1CON1 = 0x0;
#if defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD )|| \
    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
    AD1CON1bits.AD12B = 1;
#endif//defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD )...


    /* do not scan inputs */
    /* interrupt at completion of each conversion */
    /* always use MUXA */
    AD1CON2 = 0x0;

#if defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD )|| \
    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
    /* clock derived from periph clock */
    /* Tadc = (AD1CON3<7:0>+1)*Tcy */
    /* Tadc = (    5       +1)*Tcy */
    /* Tadc = 6 * Tcy = 6 * 12.5 ns = 75 ns > 65 ns required */
    AD1CON3 = 0x0005;
#else
    /* clock derived from periph clock */
    /* Tadc = 2*(AD1CON3<7:0>+1)*Tpbus */
    /* Tadc = 2*(    2       +1)*Tpbus */
    /* Tadc = 6 * Tpbus = 6 * 12.5 ns = 75 ns > 65 ns required */
    AD1CON3 = 0x0002;
#endif//defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD )...

    /* reset value for channel select at this point */
    ADC_CHAN_SELECT = 0x0;

#if  defined(USE_DIRECT_KEYS)

# if defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )
 // Port configuration defined in HardwareProfile.h
    ADC_PORT_CONFIG =
              ~( (1<<DIRECTKEY1_CHANNEL) |
                 (1<<DIRECTKEY2_CHANNEL) |
                 (1<<DIRECTKEY3_CHANNEL) |
                 (1<<DIRECTKEY4_CHANNEL) |
                 (1<<DIRECTKEY5_CHANNEL) );
# else
 // Port configuration defined in HardwareProfile.h
    ADC_PORT_CONFIG =
              ~( (1<<DIRECTKEY1_CHANNEL) |
                 (1<<DIRECTKEY2_CHANNEL) |
                 (1<<DIRECTKEY3_CHANNEL) |
                 (1<<DIRECTKEY4_CHANNEL) |
                 (1<<DIRECTKEY5_CHANNEL) |
                 (1<<DIRECTKEY6_CHANNEL) |
                 (1<<DIRECTKEY7_CHANNEL) |
                 (1<<DIRECTKEY8_CHANNEL) );
# endif//defined( PIC32MX795_ONE_PIC_MICRO_DEMO_BOARD )

#elif defined(USE_MATRIX_KEYS)

 // Port configuration defined in HardwareProfile.h
    ADC_PORT_CONFIG =
             ~((1<<MATRIXKEY0_ROW_CHANNEL)  | (1<<MATRIXKEY0_COLUMN_CHANNEL)  |
               (1<<MATRIXKEY1_ROW_CHANNEL)  | (1<<MATRIXKEY1_COLUMN_CHANNEL)  |
               (1<<MATRIXKEY2_ROW_CHANNEL)  | (1<<MATRIXKEY2_COLUMN_CHANNEL)  |
               (1<<MATRIXKEY3_ROW_CHANNEL)  | (1<<MATRIXKEY3_COLUMN_CHANNEL)  |
               (1<<MATRIXKEY4_ROW_CHANNEL)  | (1<<MATRIXKEY4_COLUMN_CHANNEL)  |
               (1<<MATRIXKEY5_ROW_CHANNEL)  | (1<<MATRIXKEY5_COLUMN_CHANNEL)  |
               (1<<MATRIXKEY6_ROW_CHANNEL)  | (1<<MATRIXKEY6_COLUMN_CHANNEL)  |
               (1<<MATRIXKEY7_ROW_CHANNEL)  | (1<<MATRIXKEY7_COLUMN_CHANNEL)  |
               (1<<MATRIXKEY8_ROW_CHANNEL)  | (1<<MATRIXKEY8_COLUMN_CHANNEL)  |
               (1<<MATRIXKEY9_ROW_CHANNEL)  | (1<<MATRIXKEY9_COLUMN_CHANNEL)  |
               (1<<MATRIXKEY10_ROW_CHANNEL) | (1<<MATRIXKEY10_COLUMN_CHANNEL) |
               (1<<MATRIXKEY11_ROW_CHANNEL) | (1<<MATRIXKEY11_COLUMN_CHANNEL) );

#elif defined(USE_SLIDER_4CHNL)

 // Port configuration defined in HardwareProfile.h
    ADC_PORT_CONFIG =
              ~( (1<<FOUR_CH_SLIDER1_CHANNEL0) |
                 (1<<FOUR_CH_SLIDER1_CHANNEL1) |
                 (1<<FOUR_CH_SLIDER1_CHANNEL2) |
                 (1<<FOUR_CH_SLIDER1_CHANNEL3) );

#elif defined( USE_SLIDER_2CHNL )

 // Port configuration defined in HardwareProfile.h
    ADC_PORT_CONFIG =
              ~( (1<<DIRECTKEY1_CHANNEL) |
                 (1<<DIRECTKEY2_CHANNEL) );

#else
#   error("Don't recognize demo type!")
#endif

#if defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
    defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
    ADC_PORT_CONFIG &= ~(1<<REF_CHANNEL);
#endif//defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD )...

#if   defined( PIC24HJ128_CAP_TOUCH_EVAL_BOARD ) || \
      defined( DSPIC33FJ128_CAP_TOUCH_EVAL_BOARD )
    /* no channel is scanned */
    AD1CSSH = AD1CSSL = 0;
    /* DIS-ENABLE interrupt */
    IEC0bits.AD1IE = 0;
#else
    /* no channel is scanned */
    AD1CSSL = 0x0;
    /* DIS-ENABLE interrupt */
    IEC1bits.AD1IE = 0;
#endif
}

#elif defined( PIC32MX220_CAP_TOUCH_EVAL_BOARD ) || \
      defined( PIC32MX220_STARTER_KIT_BOARD )

void mTouchCapADC_InitADC(void)
{
//*****************************************************************************
// ADC Setup
//*****************************************************************************

    // configure and enable the ADC
    AD1CON1 = 0x0;

    /* do not scan inputs */
    /* interrupt at completion of each conversion */
    /* always use MUXA */
    AD1CON2 = 0x0;

    /* clock derived from periph clock */
    /* Tadc = 2*(AD1CON<7:0>+1)*Tpbus */
    /* Tadc = 2*(   1       +1)*Tpbus */
    /* Tadc = 4 * Tpbus = 4 * 25 ns = 100 ns > 65 ns required */
    AD1CON3 = 0x0001;

    /* reset value for channel select at this point */
    AD1CHS = 0x0;

#if defined( PIC32MX220_STARTER_KIT_BOARD )
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = (1<<0) | (1<<1) | (1<<2) | (1<<3); //RC0,1,2,3
#else
    ANSELA = (1<<0) | (1<<1); //RA0,1
    ANSELB = (1<<2) | (1<<3) | (1<<13) | (1<<14) | (1<<15); //RB2,3,13,14,15
    ANSELC = (1<<0) | (1<<1) | (1<<2) | (1<<3); //RC0,1,2,3
#endif

    /* no channel is scanned */
    AD1CSSL = 0x0;

    /* DIS-ENABLE interrupt */
    IEC0bits.AD1IE = 0;

}
#endif//defined( PIC32MX220_STARTER_KIT_BOARD )
