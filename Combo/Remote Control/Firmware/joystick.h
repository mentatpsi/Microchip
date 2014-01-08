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
#ifndef _JOYSTICK_HEADER_FILE
#define _JOYSTICK_HEADER_FILE

typedef enum
{
    JOYSTICK_DIRECTION_LEFT = 0x01,
    JOYSTICK_DIRECTION_RIGHT = 0x02,
    JOYSTICK_DIRECTION_UP = 0x04,
    JOYSTICK_DIRECTION_DOWN = 0x08,
    JOYSTICK_DIRECTION_CENTER = 0x10,
    JOYSTICK_IDLE = 0
}JOYSTICK_DIRECTION;

void JoystickInit(void);
void JKTaskHandler(void);
JOYSTICK_DIRECTION GetJoystickDirection(void);
void JoystickDecode(void);

#endif

