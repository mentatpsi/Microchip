/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        RemoteWatchTVInterface.c
 * Dependencies:    GraphicsUserInterface.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30 V3.00
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software LicenseJpeg
 * Agreement
 *
 * Copyright © 2010 Microchip Technology Inc.  All rights reserved.
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 09/21/11    ...
 *****************************************************************************/
#include "GraphicsUserInterface.h"

WORD    channel;

/************************************************************************
 Function: CreateWatchTVScreen()
                                                                       
 Overview: Creates the TV channel selection screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateWatchTVScreen(void)
{
    WORD state;
	
    GOLFree();                      // free memory for the objects in the previous linked list and start new list

    HideScreenRendering();

	CreateCommonObjects();
    
    state = BTN_DRAW;

    BtnCreate
    (
        ID_KP_1,                 
        KPLeft(0),                
        KPTop(0),
        KPRight(0),
        KPBottom(0),
        0,                          
        state, 
        NULL, 
        "1",
        pTVRemoteStyleScheme
    );                              

    BtnCreate
    (
        ID_KP_2,                 
        KPLeft(1),                
        KPTop(0),
        KPRight(1),
        KPBottom(0),
        0,                          
        state, 
        NULL,            
        "2",
        pTVRemoteStyleScheme
    );                              

    BtnCreate
    (
        ID_KP_3,                 
        KPLeft(2),                
        KPTop(0),
        KPRight(2),
        KPBottom(0),
        0,                          
        state, 
        NULL, 
        "3",
        pTVRemoteStyleScheme
    );                              

    BtnCreate
    (
        ID_KP_4,                 
        KPLeft(0),                
        KPTop(1),
        KPRight(0),
        KPBottom(1),
        0,                          
        state, 
        NULL, 
        "4",
        pTVRemoteStyleScheme
    );                              

    BtnCreate
    (
        ID_KP_5,                 
        KPLeft(1),                
        KPTop(1),
        KPRight(1),
        KPBottom(1),
        0,                          
        state, 
        NULL, 
        "5",
        pTVRemoteStyleScheme
    );                              

    BtnCreate
    (
        ID_KP_6,                 
        KPLeft(2),                
        KPTop(1),
        KPRight(2),
        KPBottom(1),
        0,                          
        state, 
        NULL, 
        "6",
        pTVRemoteStyleScheme
    );                     

    BtnCreate
    (
        ID_KP_7,                 
        KPLeft(0),                
        KPTop(2),
        KPRight(0),
        KPBottom(2),
        0,                          
        state, 
        NULL, 
        "7",
        pTVRemoteStyleScheme
    );                              

    BtnCreate
    (
        ID_KP_8,                 
        KPLeft(1),                
        KPTop(2),
        KPRight(1),
        KPBottom(2),
        0,                          
        state, 
        NULL, 
        "8",
        pTVRemoteStyleScheme
    );                              

    BtnCreate
    (
        ID_KP_9,                 
        KPLeft(2),                
        KPTop(2),
        KPRight(2),
        KPBottom(2),
        0,                          
        state, 
        NULL, 
        "9",
        pTVRemoteStyleScheme
    );                     

    BtnCreate
    (
        ID_KP_FAV,                 
        KPLeft(0),                
        KPTop(3),
        KPRight(0),
        KPBottom(3),
        0,                          
        state, 
        (void *) &KPICON_FAV,            
        NULL,
        pTVRemoteStyleScheme
    );                              

    BtnCreate
    (
        ID_KP_0,                 
        KPLeft(1),                
        KPTop(3),
        KPRight(1),
        KPBottom(3),
        0,                          
        state, 
        NULL,             
        "0",
        pTVRemoteStyleScheme
    );                              

    BtnCreate
    (
        ID_HOME,                 
        KPLeft(2),                
        KPTop(3),
        KPRight(2),
        KPBottom(3),
        0,                          
        state, 
        (void *) &ICON_HOME,            
        NULL,
        pTVRemoteStyleScheme
    );                     
}

/************************************************************************
 Function: WORD MsgWatchTVCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the watch TV menu
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgWatchTVCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
	WORD pressedKey = 0xFFFF; 
	
    switch(GetObjID(pObj))
    {
        case ID_KP_0:
            if(objMsg == BTN_MSG_RELEASED)
            {
                pressedKey = ID_KP_0;
            }
            break;

        case ID_KP_1:
            if(objMsg == BTN_MSG_RELEASED)
            {
                pressedKey = ID_KP_1;
            }
            break;

        case ID_KP_2:
            if(objMsg == BTN_MSG_RELEASED)
            {
                pressedKey = ID_KP_2;
            }
            break;

        case ID_KP_3:
            if(objMsg == BTN_MSG_RELEASED)
            {
                pressedKey = ID_KP_3;
            }
            break;

        case ID_KP_4:
            if(objMsg == BTN_MSG_RELEASED)
            {
                pressedKey = ID_KP_4;
            }
            break;

        case ID_KP_5:
            if(objMsg == BTN_MSG_RELEASED)
            {
                pressedKey = ID_KP_5;
            }
            break;

        case ID_KP_6:
            if(objMsg == BTN_MSG_RELEASED)
            {
                pressedKey = ID_KP_6;
            }
            break;

        case ID_KP_7:
            if(objMsg == BTN_MSG_RELEASED)
            {
                pressedKey = ID_KP_7;
            }
            break;

        case ID_KP_8:
            if(objMsg == BTN_MSG_RELEASED)
            {
                pressedKey = ID_KP_8;
            }
            break;

        case ID_KP_9:
            if(objMsg == BTN_MSG_RELEASED)
            {
                pressedKey = ID_KP_9;
            }
            break;

        case ID_KP_FAV:
            if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &KPICON_FAV);
                if(channel)
                {
                    //*** To Do:  add later RF4CEControllerChannelChange(channel);
                    channel = 0;
                }
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &KPICON_FAV);
            }
            else if (objMsg == BTN_MSG_PRESSED)
            {
                BtnSetBitmap(pObj, (void *) &KPICON_PRESS_FAV);
            }
            break;

        case ID_HOME:

           if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &ICON_PRESS_HOME);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &ICON_HOME);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &ICON_HOME);
                screenState = CREATE_HOME;
            }
            break;


    	default:
    		break;        
	}
	
    if(pressedKey != 0xFFFF)
    {
        RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_NUMBER_0_CMD + pressedKey);
        channel *= 10;
        channel += pressedKey;
    }

	return 1;
}	

