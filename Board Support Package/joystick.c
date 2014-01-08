/*******************************************************************************
  Joystick 

  Summary:
    This file contains the interface definition for the joystick controller.
    
  Description:
    This library provides a low-level abstraction of the joystick device.  It 
    can be used to simplify low-level access to the device without the necessity of 
    interacting directly with the communication module's registers, thus hiding differences from 
    one serial device variant to another.
*******************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************
FileName:       joystick.c
Dependencies:   See includes
Processor:      PIC32MX

Complier:       Microchip MPLAB C32 v1.06 or higher
Company:        Microchip Technology Inc.

Copyright © 2008-2009 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END

/********************************************************************
 Section: Includes 
********************************************************************/
#include <stdint.h>
#include <plib.h>
#include <joystick.h>

#if defined (USE_JOYSTICK_MEB)

/********************************************************************
 Section: Local Variables 
********************************************************************/
JOYSTICK_CONTROL joystickCntrl;

/********************************************************************
 Section: Code 
********************************************************************/
/********************************************************************
 Funciton: void JoyStickInitialization(void)
********************************************************************/
void JoyStickInitialize(void)
{
    // configure the input pins
    PORTSetPinsDigitalIn(IOPORT_B, (BIT_0 | BIT_1 | BIT_3 | BIT_4 | BIT_15));

    joystickCntrl.value = 0;

    // set the change notice pins
    // going to have to use the SFR names here so that I do not overide anything
    CNPUESET = _CNPUE_CNPUE2_MASK | _CNPUE_CNPUE3_MASK | _CNPUE_CNPUE5_MASK | _CNPUE_CNPUE6_MASK | _CNPUE_CNPUE12_MASK;
    CNENSET = _CNEN_CNEN2_MASK | _CNEN_CNEN3_MASK | _CNEN_CNEN5_MASK | _CNEN_CNEN6_MASK | _CNEN_CNEN12_MASK;
    CNCONSET = _CNCON_ON_MASK;

}

/********************************************************************
 Funciton: void JoyStickTask(void)
********************************************************************/
void JoyStickTask(void)
{
    joystickCntrl.value = 0;

    if(!PORTReadBits(IOPORT_B, BIT_0))
        joystickCntrl.left = 1;

    if(!PORTReadBits(IOPORT_B, BIT_1))
        joystickCntrl.up = 1;

    if(!PORTReadBits(IOPORT_B, BIT_3))
        joystickCntrl.down = 1;

    if(!PORTReadBits(IOPORT_B, BIT_4))
        joystickCntrl.right = 1;

    if(!PORTReadBits(IOPORT_B, BIT_15))
        joystickCntrl.fire = 1;
}
/********************************************************************
 Funciton:  uint8_t GetJoyStickUp(void)
********************************************************************/
uint8_t GetJoyStickUp(void)
{
    return joystickCntrl.up;
}
/********************************************************************
 Funciton:  uint8_t GetJoyStickDown(void)
********************************************************************/
uint8_t GetJoyStickDown(void)
{
    return joystickCntrl.down;
}
/********************************************************************
 Funciton:  uint8_t GetJoyStickLeft(void)
********************************************************************/
uint8_t GetJoyStickLeft(void)
{
    return joystickCntrl.left;
}
/********************************************************************
 Funciton:  uint8_t GetJoyStickRight(void)
********************************************************************/
uint8_t GetJoyStickRight(void)
{
    return joystickCntrl.right;
}
/********************************************************************
 Funciton:  uint8_t GetJoyStickFire(void)
********************************************************************/
uint8_t GetJoyStickFire(void)
{
    return joystickCntrl.fire;
}
/********************************************************************
 Funciton:  JOYSTICK_CONTROL GetJoyStick(void)
********************************************************************/
JOYSTICK_CONTROL GetJoyStick(void)
{
    return joystickCntrl;
}

/********************************************************************
 Funciton:  JOYSTICK_CONTROL GetJoyStick(void)
********************************************************************/
JOYSTICK_POSITION GetJoystickPosition(void)
{
    JOYSTICK_POSITION pos;

    pos = JOYSTICK_IDLE;

    if(joystickCntrl.left)
        pos |= JOYSTICK_LEFT;

    if(joystickCntrl.up)
        pos |= JOSTICK_UP;

    if(joystickCntrl.down)
        pos |= JOSTICK_DOWN;

    if(joystickCntrl.right)
        pos |= JOYSTICK_RIGHT;

    if(joystickCntrl.fire)
        pos |= JOYSTICK_FIRE;

    return pos;

}

#endif // #if defined (USE_JOYSTICK_MEB)