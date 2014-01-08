/*****************************************************************************
 *
 *                  LEDs
 *
 *****************************************************************************
 * FileName:        led.c
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

/*****************************************************************************
 * Section:  Includes
 *****************************************************************************/
#include "led.h"

volatile LED_STATE  ledState[NUMBER_OF_LEDS];
volatile UINT16     ledCounter[NUMBER_OF_LEDS];

/*****************************************************************************
 * 
 *****************************************************************************/
void LEDInit(void)
{
BYTE i;

   SetLeftGreenOff();
   SetRightGreenOff();
   SetRedOff();

   SetLeftGreenLEDDirection();
   SetRightGreenLEDDirection();
   SetRedLEDDirection();

   for(i=0; i<NUMBER_OF_LEDS; i++)
   {
       ledState[i] = LED_IDLE;
       ledCounter[i] = LED_TOGGLE_PERIOD;
   }
}

/*****************************************************************************
 * 
 *****************************************************************************/
void LEDResume(void)
{
   SetLeftGreenLEDDirection();
   SetRightGreenLEDDirection();
   SetRedLEDDirection();
}

/*****************************************************************************
 * 
 *****************************************************************************/
void LEDStop(void)
{
   CrearLeftGreenLEDDirection();
   CrearRightGreenLEDDirection();
   CrearRedLEDDirection();
}

/*****************************************************************************
 * 
 *****************************************************************************/
void LEDOn(BYTE led)
{
	switch(led)
    {
       case GREEN1:
           SetLeftGreenOn();
           break;
       case GREEN2:
           SetRightGreenOn();
           break;
       case RED1:
           SetRedOn();
           break;
    }
}
/*****************************************************************************
 * 
 *****************************************************************************/
void LEDOff(BYTE led)
{
    ledState[led] = LED_IDLE;
	switch(led)
    {
       case GREEN1:
           SetLeftGreenOff();
           break;
       case GREEN2:
           SetRightGreenOff();
           break;
       case RED1:
           SetRedOff();
           break;
    }
}
/*****************************************************************************
 * 
 *****************************************************************************/
void LEDToggle(BYTE led)
{
	switch(led)
    {
       case GREEN1:
           SetLeftGreenToggle();
           break;
       case GREEN2:
           SetRightGreenToggle();
           break;
       case RED1:
           SetRedToggle();
           break;
    }
}
/*****************************************************************************
 * 
 *****************************************************************************/
void LEDBlink(BYTE led)
{
    ledCounter[led] = LED_TOGGLE_PERIOD;
    ledState[led] = LED_TOGGLE;
}
/*****************************************************************************
 * 
 *****************************************************************************/
void LEDPulse(BYTE led)
{
    ledCounter[led] = LED_TOGGLE_PERIOD;
    ledState[led] = LED_PULSE;
    LEDOn(led);
}

/*****************************************************************************
 * 
 *****************************************************************************/
void LEDTaskHandler(void)
{
BYTE i;

    for(i=0; i<NUMBER_OF_LEDS; i++)
    {
        switch(ledState[i])
        {
             case LED_IDLE:
                 break;
             case LED_PULSE:
                 if(0 == ledCounter[i]--)
                 {
                     LEDOff(i); 
                 }
                 break;
             case LED_TOGGLE:
                 if(0 == ledCounter[i]--)
                 {
                     LEDToggle(i);
                     ledCounter[i] = LED_TOGGLE_PERIOD;
                 }
                 break;
        }
    }
}
