/*****************************************************************************
 *
 *              Power save module
 *
 *****************************************************************************
 * FileName:        sleep.c
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
#include "sleep.h"
#include "RF4CEController.h"
#include "TimeDelay.h"
#include "led.h"


void PowerSaveInit(void)
{
   // initialize PWM for LCD backlight
   OC1CON1bits.OCTSEL = 7;      // peripheral clock
   OC1CON2bits.SYNCSEL = 0x1F;  // period defined in OC1RS register
   OC1RS  = BKLT_PWM_RERIOD;    // set PWM period
   OC1R = 0;                    // set initial duty cycle to 0% (backlight on)
   OC1CON1bits.OCM = 6;         // PWM mode on

   // enable wakeup button interrupt
   IEC0bits.INT0IE = 1;
}

void PowerSaveBacklight(BOOL mode)
{
UINT8 counter;

     for(counter=0; counter<BKLT_DIMMER_STEPS_NUM; counter++)
     {
         if(mode == TRUE)
         {
            // turn off
            if(OC1R >= BKLT_PWM_RERIOD)
            { 
                OC1R = BKLT_PWM_RERIOD;
                return;
            }
	        OC1R += BKLT_PWM_RERIOD/BKLT_DIMMER_STEPS_NUM;
         }else{
            // turn on
            if(OC1R <= BKLT_PWM_RERIOD/BKLT_DIMMER_STEPS_NUM)
            { 
                OC1R = 0;
                return;
            }
	        OC1R -= BKLT_PWM_RERIOD/BKLT_DIMMER_STEPS_NUM;
         }
         DelayMs(BKLT_DIMMER_DELAY_MS);
     }
}

void PowerSaveHandler(void)
{
     // SLEEP
     RF4CE_TransceiverPowerState(POWER_STATE_SLEEP);
     PowerSaveBacklight(TRUE);
     LEDStop(); // off LEDs

     // disable interrupts
     IEC1bits.CNIE = 0;             // joystick
     IEC1bits.T4IE = 0;             // tick timer
     IEC1bits.INT1IE = 0;           // RF
     IEC6bits.GFX1IE = 0;           // GFX
     IEC5bits.USB1IE = 0;           // USB

     // clear any pending wake up interrupt and enable the 
     // external interrupt. This serves as a debiunce logic on 
     // the WAKE button.
     IFS0bits.INT0IF = 0;
     IEC0bits.INT0IE = 1;

     Sleep();

     // WAKEUP
     // enable interrupts
     IEC1bits.CNIE = 1;             // joystick
     IEC1bits.T4IE = 1;             // tick timer
     IEC1bits.INT1IE = 1;           // RF
     IEC6bits.GFX1IE = 1;           // GFX
     IEC5bits.USB1IE = 1;           // USB
     LEDResume();                   // on LEDs

     RF4CE_TransceiverPowerState(POWER_STATE_WAKEUP);
     PowerSaveBacklight(FALSE);
}
