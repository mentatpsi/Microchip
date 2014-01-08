/*****************************************************************************
 *
 * Side buttons driver
 * (S3(RD6), S4(RD13), S5(RA7) and S6(RD7) on Explorer 16 board)
 *
 *****************************************************************************
 * FileName:        SideButtons.c
 * Dependencies:    MainDemo.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
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
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok		07/20/07	...
 *****************************************************************************/
#include "MainDemo.h"


/************************************************************************
* Function: void SideButtonsMsg(GOL_MSG* msg)
*                                                                       
* Overview: this function checks buttons state and creates GOL messages.
*           S3 and S4 are used for ARROW UP PRESSED and ARROW DOWN PRESSED
*           messages. S6 moves focus to the next control.
*           S5 creates CARRIGE RETURN PRESSED/RELEASED messages.
*                                                                       
* Input: pointer to the GOL message structure                                                          
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void SideButtonsMsg(GOL_MSG *msg)
{

    // Previous states of the buttons
    static char S3 = 1;
    static char S4 = 1;
    static char S5 = 1;
    static char S6 = 1;
    char        state;
    OBJ_HEADER  *obj;

    msg->uiEvent = EVENT_INVALID;

    /**
     * Change Focus button
     * Explorer 16 + GFX PICTail    - S6 (8 bit PMP)
     * Starter Kit + GFX PICTail    - S1 or S2 (8 bit PMP)
     * Multimedia Expansion Board   - left or right
     * DA210 Developement Board     - S2 or S3
     **/
    state = GetHWButtonFocus();
    if(S6 != state)
    {
        if(S6)
        {

            // Get pointer to the next object should be focused
            obj = GOLGetFocusNext();
            if(obj != NULL)
            {
                GOLSetFocus(obj);
                Beep();
            }
        }

        S6 = state;
        return;
    }

    /**
     * Carriage Return button
     * Explorer 16 + GFX PICTail    - S3 (8 bit PMP)
     * Explorer 16 + GFX PICTail    - S5 (16 bit PMP)
     * Starter Kit + GFX PICTail    - S0 (8 bit PMP)
     * Multimedia Expansion Board   - Fire Button
     * DA210 Developement Board     - S1
     * NOTE:    Starter Kit + GFX PICTail will switches are shared
     *          with the 16 bit PMP data bus.
     **/
    state = GetHWButtonCR();
    if(S5 != state)
    {

        // Get pointer to the focused object
        obj = GOLGetFocus();
        if(obj != NULL)
        {
            if(S5)
            {
                msg->type = TYPE_KEYBOARD;
                msg->uiEvent = EVENT_KEYSCAN;
                msg->param1 = obj->ID;
                msg->param2 = SCAN_CR_PRESSED;
            }
            else
            {
                msg->type = TYPE_KEYBOARD;
                msg->uiEvent = EVENT_KEYSCAN;
                msg->param1 = obj->ID;
                msg->param2 = SCAN_CR_RELEASED;
            }
        }

        S5 = state;
        return;
    }

    /**
     * Scan down button
     * Explorer 16 + GFX PICTail    - S4 (8 bit PMP)
     * Multimedia Expansion Board   - Down joystick Button
     **/
    state = GetHWButtonScanDown();
    if(S3 != state)
    {

        // Get pointer to the focused object
        obj = GOLGetFocus();
        if(obj != NULL)
        {
            if(S3)
            {
                msg->type = TYPE_KEYBOARD;
                msg->uiEvent = EVENT_KEYSCAN;
                msg->param1 = obj->ID;
                msg->param2 = SCAN_DOWN_PRESSED;
            }
        }

        S3 = state;
        return;
    }

    /**
     * Scan up button
     * Explorer 16 + GFX PICTail    - S5 (8 bit PMP)
     * Multimedia Expansion Board   - Up joystick Button
     **/
    state = GetHWButtonScanUp();
    if(S4 != state)
    {

        // Get pointer to the focused object
        obj = GOLGetFocus();
        if(obj != NULL)
        {
            if(S4)
            {
                msg->type = TYPE_KEYBOARD;
                msg->uiEvent = EVENT_KEYSCAN;
                msg->param1 = obj->ID;
                msg->param2 = SCAN_UP_PRESSED;
            }
        }

        S4 = state;
        return;
    }
}
