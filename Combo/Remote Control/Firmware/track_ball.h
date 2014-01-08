/*****************************************************************************
 *
 *              HardwareProfile.h -- Hardware Configuration 
 *
 *****************************************************************************
 * FileName:        HardwareProfile.h
 * Dependencies:
 * Processor:       PIC18F
 * Compiler:        C18 v3.20 or higher
 * Linker:          MPLINK 3.40 or higher
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
 * File Description:
 *
 *   This file provides configuration and basic hardware functionality 
 *   based on chosen hardware demo boards.
 *
 * Change History:
 *  Rev         Date         Description
 *  0.1         2/17/2009    Initial revision
 *  2.0         04/24/2009   Modified for MiApp interface
 *  2.1         06/20/2009   Add LCD support
 *  1.0-1.1.2   12/10/2009   Modified for RF4CE
 *****************************************************************************/
#ifndef _TRACK_BALL_HEADER_FILE
#define _TRACK_BALL_HEADER_FILE

typedef enum
{
    TRACK_BALL_DIRECTION_LEFT = 0x01,
    TRACK_BALL_DIRECTION_RIGHT = 0x02,
    TRACK_BALL_DIRECTION_UP = 0x04,
    TRACK_BALL_DIRECTION_DOWN = 0x08,
    TRACK_BALL_IDLE = 0
}TRACK_BALL_DIRECTION;

void TrackBallInit(void);
void JBTaskHandler(void);
TRACK_BALL_DIRECTION GetTrackBallDirection(void);

#endif

