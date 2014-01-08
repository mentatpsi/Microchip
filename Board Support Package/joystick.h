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
FileName:       joystick.h
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

#ifndef JOYSTICK_H_FILE_
#define JOYSTICK_H_FILE_

/********************************************************************
 Section: Includes 
********************************************************************/
#include <stdint.h>

/******************************************************************************************
  Summary:
    Joystick position.

  Description:
    The current position of the joystick.


  Example:
    <code>
    if(GetJoystickPosition() == JOYSTICK_LEFT)
    {
        // do something.....
    }
    </code>
  
  Remarks:
    None.
  *****************************************************************************************/
typedef enum
{
    // Joystick is in the idle position.
    JOYSTICK_IDLE,                  
    // Joystick is pressed to the left position.
    JOYSTICK_LEFT       /*DOM-IGNORE-BEGIN*/= 0x01/*DOM-IGNORE-END*/,
    // Joystick is pressed to the right position.
    JOYSTICK_RIGHT      /*DOM-IGNORE-BEGIN*/= 0x02/*DOM-IGNORE-END*/,
    // Joystick is pressed to the up position.
    JOSTICK_UP          /*DOM-IGNORE-BEGIN*/= 0x04/*DOM-IGNORE-END*/,
    // Joystick is pressed to the down position.
    JOSTICK_DOWN        /*DOM-IGNORE-BEGIN*/= 0x08/*DOM-IGNORE-END*/,
    // Joystick fire button is pressed.
    JOYSTICK_FIRE       /*DOM-IGNORE-BEGIN*/= 0x80/*DOM-IGNORE-END*/,

    // Joystick is pressed to the left position and the fire button is pressed.
    JOYSTICK_LEFT_FIRE  /*DOM-IGNORE-BEGIN*/= JOYSTICK_FIRE | JOYSTICK_LEFT/*DOM-IGNORE-END*/,
    // Joystick is pressed to the right position and the fire button is pressed.
    JOYSTICK_RIGHT_FIRE /*DOM-IGNORE-BEGIN*/= JOYSTICK_FIRE | JOYSTICK_RIGHT/*DOM-IGNORE-END*/,
    // Joystick is pressed to the up position and the fire button is pressed.
    JOYSTICK_UP_FIRE    /*DOM-IGNORE-BEGIN*/= JOYSTICK_FIRE | JOSTICK_UP/*DOM-IGNORE-END*/,
    // Joystick is pressed to the down position and the fire button is pressed.
    JOYSTICK_DOWN_FIRE  /*DOM-IGNORE-BEGIN*/= JOYSTICK_FIRE | JOSTICK_DOWN/*DOM-IGNORE-END*/,

}JOYSTICK_POSITION;

/******************************************************************************************
  Summary:
    Joystick position structure.

  Description:
    The structure of the current position of the joystick.


  Example:
    <code>
    JOYSTIRCK_CNTRL pos;

    pos = GetJoystickPosition();

    if(pos.left)
    {
        // do something.....
    }
    </code>
  
  Remarks:
    None.
  *****************************************************************************************/
typedef union
{
    struct
    {
        uint8_t up: 1;      // Joystick up position.
        uint8_t down: 1;    // Joystick down position.
        uint8_t right: 1;   // Joystick right position.
        uint8_t left: 1;    // Joystick left position.
        uint8_t fire: 1;    // Joystick fire position.
    };

    uint8_t value;
}JOYSTICK_CONTROL;

/**************************************************************************************
  Function:
    void JoyStickInitialize(void)
    
  Summary:
    Initializes the control I/O, uses the change notice pull-ups and enables them.

  Description:
    This routine configures the control I/O and change notice pull-ups.
  Conditions:
    None.
  Input:
    None.
  Return:
    None.
  Example:
    <code>
    // Initialize the joystick
    JoyStickInitialize();

    // joystick is configured and ready to go
    </code>
  Remarks:
    None.
  **************************************************************************************/
void JoyStickInitialize(void);

/**************************************************************************************
  Function:
    void JoyStickTask(void)
    
  Summary:
    Checks the joystick position.

  Description:
    This routine checks the current joystick position and records it.
  Conditions:
    The initialization routine, JoyStickInitialize, must be called.
  Input:
    None.
  Return:
    None.
  Example:
    Joystick pollling 
    <code>
    // Initialize the joystick
    JoyStickInitialize();

    // joystick is configured and ready to go
    JoysStickTask();    // ...polling

    //..... do something
    </code>

    Joystick interrupt
    <code>
    __ISR(_CHANGE_NOTICE_VECTOR, ipl2) SystemChangeNoticeIntHandler(void)
    {
        // handle any changes by the joystick
        JoyStickTask();
        
        // handle any other change notice interrupts

        INTClearFlag(INT_CN);
    }
    </code>
  Remarks:
    This routine can be used as polling or as part of an interrupt routine.
  **************************************************************************************/
void JoyStickTask(void);

/**************************************************************************************
  Function:
    uint8_t GetJoyStickUp(void)
    
  Summary:
    Returns if the joystick is in the "up" position.

  Description:
    This routine returns if the joystick is in the "up" position.
  Conditions:
    The initialization routine, JoyStickInitialize, must be called.
  Input:
    None.
  Return:
    0 - the joystick is not in the "up" position.
    non-zero - the joystick is in the "up" position. 
  Example:
    <code>
    // Initialize the joystick
    JoyStickInitialize();

    // joystick is configured and ready to go
    while(1)
    (
        JoystickTask();

        if(GetJoystickUp())
        {
            // so something because joystick in the up position
        }
    }
    </code>
  Remarks:
    None.
  **************************************************************************************/
uint8_t GetJoyStickUp(void);

/**************************************************************************************
  Function:
    uint8_t GetJoyStickDown(void)
    
  Summary:
    Returns if the joystick is in the "down" position.

  Description:
    This routine returns if the joystick is in the "down" position.
  Conditions:
    The initialization routine, JoyStickInitialize, must be called.
  Input:
    None.
  Return:
    0 - the joystick is not in the "down" position.
    non-zero - the joystick is in the "down" position. 
  Example:
    <code>
    // Initialize the joystick
    JoyStickInitialize();

    // joystick is configured and ready to go
    while(1)
    (
        JoystickTask();

        if(GetJoyStickDown())
        {
            // so something because joystick in the down position
        }
    }
    </code>
  Remarks:
    None.
  **************************************************************************************/
uint8_t GetJoyStickDown(void);

/**************************************************************************************
  Function:
    uint8_t GetJoyStickLeft(void)
    
  Summary:
    Returns if the joystick is in the "left" position.

  Description:
    This routine returns if the joystick is in the "left" position.
  Conditions:
    The initialization routine, JoyStickInitialize, must be called.
  Input:
    None.
  Return:
    0 - the joystick is not in the "left" position.
    non-zero - the joystick is in the "left" position. 
  Example:
    <code>
    // Initialize the joystick
    JoyStickInitialize();

    // joystick is configured and ready to go
    while(1)
    (
        JoystickTask();

        if(GetJoyStickLeft())
        {
            // so something because joystick in the left position
        }
    }
    </code>
  Remarks:
    None.
  **************************************************************************************/
uint8_t GetJoyStickLeft(void);

/**************************************************************************************
  Function:
    uint8_t GetJoyStickRight(void)
    
  Summary:
    Returns if the joystick is in the "right" position.

  Description:
    This routine returns if the joystick is in the "right" position.
  Conditions:
    The initialization routine, JoyStickInitialize, must be called.
  Input:
    None.
  Return:
    0 - the joystick is not in the "right" position.
    non-zero - the joystick is in the "right" position. 
  Example:
    <code>
    // Initialize the joystick
    JoyStickInitialize();

    // joystick is configured and ready to go
    while(1)
    (
        JoystickTask();

        if(GetJoyStickRight())
        {
            // so something because joystick in the right position
        }
    }
    </code>
  Remarks:
    None.
  **************************************************************************************/
uint8_t GetJoyStickRight(void);


/**************************************************************************************
  Function:
    uint8_t GetJoyStickFire(void)
    
  Summary:
    Returns if the joystick is in the "fire" position.

  Description:
    This routine returns if the joystick is in the "fire" position.
  Conditions:
    The initialization routine, JoyStickInitialize, must be called.
  Input:
    None.
  Return:
    0 - the joystick is not in the "fire" position.
    non-zero - the joystick is in the "fire" position. 
  Example:
    <code>
    // Initialize the joystick
    JoyStickInitialize();

    // joystick is configured and ready to go
    while(1)
    (
        JoystickTask();

        if(GetJoyStickFire())
        {
            // so something because joystick in the fire position
        }
    }
    </code>
  Remarks:
    None.
  **************************************************************************************/
uint8_t GetJoyStickFire(void);

/**************************************************************************************
  Function:
    JOYSTICK_CONTROL GetJoyStick(void)
    
  Summary:
    Returns the joystick's position.

  Description:
    This routine returns the joystick's current position.
  Conditions:
    The initialization routine, JoyStickInitialize, must be called.
  Input:
    None.
  Return:
    A structure, JOYSTICK_CONTROL, that has the current joystick position.
  Example:
    <code>
    // Initialize the joystick
    JoyStickInitialize();

    // joystick is configured and ready to go
    while(1)
    (
        JOYSTICK_CONTROL pos;
        
        JoystickTask();

        pos = GetJoyStick();

        if(pos.fire)
        {
            // so something because joystick in the fire position
        }
    }
    </code>
  Remarks:
    None.
  **************************************************************************************/
JOYSTICK_CONTROL GetJoyStick(void);

/**************************************************************************************
  Function:
    JOYSTICK_POSITION GetJoystickPosition(void)
    
  Summary:
    Returns the joystick's position.

  Description:
    This routine returns the joystick's current position.
  Conditions:
    The initialization routine, JoyStickInitialize, must be called.
  Input:
    None.
  Return:
    An enumeration, JOYSTICK_POSITION, that has the current joystick position.
  Example:
    <code>
    // Initialize the joystick
    JoyStickInitialize();

    // joystick is configured and ready to go
    while(1)
    (
        
        JoystickTask();

        if(GetJoystickPosition() & JOYSTICK_FIRE)
        {
            // so something because joystick in the fire position
        }
    }
    </code>
  Remarks:
    None.
  **************************************************************************************/
JOYSTICK_POSITION GetJoystickPosition(void);

#endif