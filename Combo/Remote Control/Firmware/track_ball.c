/*****************************************************************************
 *
 *              Track Ball 
 *
 *****************************************************************************
 * FileName:        track_ball.c
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
#include "track_ball.h"

/*****************************************************************************
 * Section:  Structure
 *****************************************************************************/

typedef struct
{
    BYTE left:  1;
    BYTE right: 1;
    BYTE up:    1;
    BYTE down:  1;
}TRACK_BALL_PINS;

TRACK_BALL_PINS         pinState;
TRACK_BALL_DIRECTION    trackBallDirection;
/*****************************************************************************
 * 
 *****************************************************************************/
void TrackBallInit(void)
{
    TurnJBLEDOff();
    SetJBLEDDirection();

    SetJBButtonDirection();

    SetJBLeftDirection(); 
    SetJBRightDirection();
    SetJBUpDirection();   
    SetJBDownDirection(); 

    JBLeftInterrruptConfig();
    JBRightInterruptConfig();
    JBUpInterruptConfig();   
    JBDownInterruptConfig();

    JBIntDisable();
    
    pinState.left = GetJBLeftPin();
    pinState.right = GetJBRightPin();
    pinState.up = GetJBUpPin();
    pinState.down = GetJBDownPin();
   
    trackBallDirection = TRACK_BALL_IDLE;

    JBIntEnable();
    JBInterruptConfig();     

}
/*****************************************************************************
 * 
 *****************************************************************************/
void JBTaskHandler(void)
{
    trackBallDirection = TRACK_BALL_IDLE;

    if(pinState.left != GetJBLeftPin())
        trackBallDirection |= TRACK_BALL_DIRECTION_LEFT;

    if(pinState.right != GetJBRightPin())
        trackBallDirection |= TRACK_BALL_DIRECTION_RIGHT;

    if(pinState.up != GetJBUpPin())
        trackBallDirection |= TRACK_BALL_DIRECTION_UP;

    if(pinState.down != GetJBDownPin())
        trackBallDirection |= TRACK_BALL_DIRECTION_DOWN;

    pinState.left = GetJBLeftPin();
    pinState.right = GetJBRightPin();
    pinState.up = GetJBUpPin();
    pinState.down = GetJBDownPin();
}
/*****************************************************************************
 * 
 *****************************************************************************/
TRACK_BALL_DIRECTION GetTrackBallDirection(void)
{
    return trackBallDirection;
}

