/*********************************************************************
 * Microchip Graphic Library Demo Application
 * The header file joins all header files used in the project.
 *********************************************************************
 * FileName:        MainDemo.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC24/PIC30/PIC32
 * Compiler:        C30/C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright © 2007 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Paolo Tamayo			09/15/2007  ...
 ********************************************************************/
#ifndef __DEMOSTEPS_H__
    #define __DEMOSTEPS_H__

//******************************************************************************

//******************************************************************************

// Data Structures

//******************************************************************************

//******************************************************************************

// Demo event structure
typedef struct
{
    BYTE    type;       // Type of input device.
    BYTE    uiEvent;    // The generic events for input device.
    SHORT   param1;     // Parameter 1 meaning is dependent on the type of input device.
    SHORT   param2;     // Parameter 2 meaning is dependent on the type of input device.
    SHORT   time;
} DEMO_MSG;

// time delay definitions
    #define QUICKDELAY      100
    #define FASTDELAY       10
    #define HALFSECDELAY    500
    #define FIFTHSECDELAY   200

    #define ONESECDELAY     (1000)
    #define TWOSECDELAY     (ONESECDELAY * 2)
    #define THREESECDELAY   (ONESECDELAY * 3)
    #define FOURSECDELAY    (ONESECDELAY * 4)
    #define FIVESECDELAY    (ONESECDELAY * 5)
    #define TWENTYSECDELAY  (ONESECDELAY * 20)

/*********************************************************************
* Function:  void UpdateDemoMode(GOL_MSG *pMsg)
*
* Overview: This function updates the demo steps when automatic demo
*			mode is enabled.
*
* PreCondition: none
*
* Input: pMsg - Pointer to the message from the user. Also modified
*				to become the pre-defined events in the demo.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void    UpdateDemoMode(GOL_MSG *pMsg);

/*********************************************************************
* Function:  void InitDemoMode(void)
*
* Overview: This function initializes the automatic demo mode.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void    InitDemoMode(void);
#endif // __DEMOSTEPS_H__
