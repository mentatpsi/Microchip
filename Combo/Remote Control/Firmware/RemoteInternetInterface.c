/*****************************************************************************
 * Microchip Graphics Library Demo Application
 *****************************************************************************
 * FileName:        RemoteInternetInterface.c
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
 * 11/05/10    ...
 *****************************************************************************/
#include "GraphicsUserInterface.h"

/************************************************************************
 Function: CreateInternetScreen()
                                                                       
 Overview: Creates the favorites screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateInternetScreen(void)
{
	static XCHAR InternetModeStr[] = "You are now\nin Internet Mode";
	static XCHAR InternetInstrStr[] = "Use your TV/PC to browse the\ninternet and use the joystick\non this remote as the mouse\npointer.\n\nTo return to the main menu\npress on the Home icon.";
	
    GOLFree();                      // free memory for the objects in the previous linked list and start new list
	
    HideScreenRendering();

	CreateCommonObjects();

	SetColor(REMOTE_GRAY);
    while(!Bar(0, TOP_MARGIN, GetMaxX(), GetMaxY()));

	while(!PutImage(LEFT_MARGIN, TOP_MARGIN + 5,(void *) &INTICON_GLOBE,1));

	StCreate
    (
        ID_INT_MODE,             
        INTER_MODELeft,
        INTER_MODETop,                        
        INTER_MODERight,
        INTER_MODEBottom,
        ST_DRAW, 
        InternetModeStr,                            
        pRemoteScheme
    );               

	StCreate
    (
        ID_INT_INTR,             
        INTER_INSTRLeft,
        INTER_INSTRTop,                        
        INTER_INSTRRight,
        INTER_INSTRBottom,
        ST_DRAW, 
        InternetInstrStr,
        pRemoteScheme
    );               

    BtnCreate
    (
        ID_HOME,                 
        KPLeft(2),                
        KPTop(3),
        KPRight(2),
        KPBottom(3),
        0,                          
        BTN_DRAW,
        (void *) &ICON_HOME,            
        NULL,
        pRemoteScheme 
    );         
        
	
}	

/************************************************************************
 Function: WORD MsgInternetCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the play disc screen 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgInternetCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{

    switch(GetObjID(pObj))
    {
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

	return 1;
}

