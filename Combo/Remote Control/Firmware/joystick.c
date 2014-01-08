/*****************************************************************************
 *
 *              JOYSTICK
 *
 *****************************************************************************
 * FileName:        joystick.c
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
#include "compiler.h"
#include "HardwareProfile.h"
#include "joystick.h"
#include "RF4CEController.h"
#include "led.h"

/*****************************************************************************
 * Section:  Structure
 *****************************************************************************/

volatile JOYSTICK_DIRECTION    joystickDirection;
/*****************************************************************************
 * 
 *****************************************************************************/
void JoystickInit(void)
{
    SetJKButtonDirection();

    SetJKLeftDirection(); 
    SetJKRightDirection();
    SetJKUpDirection();   
    SetJKDownDirection(); 
    SetJKCenterDirection();

    JKLeftInterrruptConfig();
    JKRightInterruptConfig();
    JKUpInterruptConfig();   
    JKDownInterruptConfig();
    JKCenterInterruptConfig();

    JKIntDisable();
      
    joystickDirection = JOYSTICK_IDLE;

    JKIntEnable();
}
/*****************************************************************************
 * 
 *****************************************************************************/
void JKTaskHandler(void)
{
    joystickDirection = JOYSTICK_IDLE;

    if(GetJKLeftPin() == 0)
        joystickDirection |= JOYSTICK_DIRECTION_LEFT;

    if(GetJKRightPin() == 0)
        joystickDirection |= JOYSTICK_DIRECTION_RIGHT;

    if(GetJKUpPin() == 0)
        joystickDirection |= JOYSTICK_DIRECTION_UP;

    if(GetJKDownPin() == 0)
        joystickDirection |= JOYSTICK_DIRECTION_DOWN;

    if(GetJKCenterPin() == 0)
        joystickDirection |= JOYSTICK_DIRECTION_CENTER;
}
/*****************************************************************************
 * 
 *****************************************************************************/
JOYSTICK_DIRECTION GetJoystickDirection(void)
{
    return joystickDirection;
}
/*****************************************************************************
 * 
 *****************************************************************************/
void JoystickDecode(void)
{
           if(GetJoystickDirection() == JOYSTICK_DIRECTION_CENTER){
                 LEDPulse(GREEN1);
                 RF4CEControllerSelect();
                 joystickDirection &= ~JOYSTICK_DIRECTION_CENTER;
           }else if(GetJoystickDirection() == JOYSTICK_DIRECTION_UP){
                 LEDPulse(GREEN1);
                 RF4CEControllerUp();
                 joystickDirection &= ~JOYSTICK_DIRECTION_UP;
           }else if(GetJoystickDirection() == JOYSTICK_DIRECTION_DOWN){
                 LEDPulse(GREEN1);
                 RF4CEControllerDown();
                 joystickDirection &= ~JOYSTICK_DIRECTION_DOWN;
           }else if(GetJoystickDirection() == JOYSTICK_DIRECTION_LEFT){
                 LEDPulse(GREEN1);
                 RF4CEControllerLeft();
                 joystickDirection &= ~JOYSTICK_DIRECTION_LEFT;
           }else if(GetJoystickDirection() == JOYSTICK_DIRECTION_RIGHT){
                 LEDPulse(GREEN1);
                 RF4CEControllerRight();
                 joystickDirection &= ~JOYSTICK_DIRECTION_RIGHT;
           }else{
               
           }
}
