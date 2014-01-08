/*****************************************************************************
 * Microchip Graphics Library Demo Application
 *****************************************************************************
 * FileName:        RemotePlayDiscInterface.c
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
#include "RF4CEController.h"

BOOL playPause = TRUE;


/************************************************************************
 Function: CreatePlayDiscScreen()
                                                                       
 Overview: Creates the play disc screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreatePlayDiscScreen(void)
{
	#define BUTTONSPACING	35
	#define SETHOMESPACING	5
    
    static XCHAR DiscMenuStr[] = "Disc\nMenu";
    static XCHAR DiscSetupStr[] = "Setup";

    GOLFree();                      // free memory for the objects in the previous linked list and start new list
	playPause = TRUE;

    HideScreenRendering();

	CreateCommonObjects();

	SetColor(REMOTE_GRAY);
    while(!Bar(0, TOP_MARGIN, GetMaxX(), GetMaxY()));

    BtnCreate
    (
        ID_SLOW_RW,                 
        PDLeft(0)+BUTTONSPACING,                
        PDTop(0),
        PDRight(0)+BUTTONSPACING,
        PDBottom(0),
        0,                          
        BTN_DRAW | BTN_NOPANEL,
        (void *) &PDICON_FAST_RW,            
        NULL,
        NULL
    );                              

    BtnCreate
    (
        ID_SLOW_FW,                 
        PDLeft(2)-BUTTONSPACING,                
        PDTop(0),
        PDRight(2)-BUTTONSPACING,
        PDBottom(0),
        0,                          
        BTN_DRAW | BTN_NOPANEL,
        (void *) &PDICON_FAST_FW,            
        NULL,
        NULL
    );                              

   BtnCreate
    (
        ID_FAST_RW,                 
        PDLeft(0),                
        PDTop(1),
        PDRight(0),
        PDBottom(1),
        0,                          
        BTN_DRAW | BTN_NOPANEL,
        (void *) &PDICON_SLOW_RW,            
        NULL,
        NULL
    );                              

    BtnCreate
    (
        ID_PLAY_PAUSE,                 
        PDLeft(1),                
        PDTop(1),
        PDRight(1),
        PDBottom(1),
        0,                          
        BTN_DRAW | BTN_NOPANEL,
        (void *) &PDICON_PLAY_PAUSE,            
        NULL,
        NULL
    );                              

   BtnCreate
    (
        ID_FAST_FW,                 
        PDLeft(2),                
        PDTop(1),
        PDRight(2),
        PDBottom(1),
        0,                          
        BTN_DRAW | BTN_NOPANEL,
        (void *) &PDICON_SLOW_FW,            
        NULL,
        NULL
    );                              

    BtnCreate
    (
        ID_STOP,                 
        PDLeft(0)+BUTTONSPACING,                
        PDTop(2),
        PDRight(0)+BUTTONSPACING,
        PDBottom(2),
        0,                          
        BTN_DRAW | BTN_NOPANEL,
        (void *) &PDICON_STOP,            
        NULL,
        NULL
    );                              

   BtnCreate
    (
        ID_PLAY_INC,                 
        PDLeft(2)-BUTTONSPACING,                
        PDTop(2),
        PDRight(2)-BUTTONSPACING,
        PDBottom(2),
        0,                          
        BTN_DRAW | BTN_NOPANEL,
        (void *) &PDICON_PLAY_INC,            
        NULL,
        NULL
    );                              

    BtnCreate
    (
        ID_SETUP_PLAYER,                 
        KPLeft(0),                
        KPTop(3),
        KPRight(0),
        KPBottom(3),
        0,                          
        BTN_DRAW,
        NULL,           
        DiscSetupStr,
        pWatchDiscStyleScheme
    );                              

    BtnCreate
    (
        ID_DISC_MENU,                 
        KPLeft(1),                
        KPTop(3),
        KPRight(1),
        KPBottom(3),
        0,                          
        BTN_DRAW,
        NULL, 
        DiscMenuStr,
        pWatchDiscStyleScheme
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
        pWatchDiscStyleScheme
    );                     
	
}	

/************************************************************************
 Function: WORD MsgPlayDiscCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the play disc screen 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgPlayDiscCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{

    switch(GetObjID(pObj))
    {
        case ID_FAST_RW:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &PDICON_SLOW_RW_PRESS);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_SLOW_RW);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_SLOW_RW);
                RF4CEControllerRewind();
            }
            break;

        case ID_FAST_FW:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &PDICON_SLOW_FW_PRESS);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_SLOW_FW);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_SLOW_FW);
                RF4CEControllerFastForward();
            }
            break;

        case ID_SLOW_RW:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &PDICON_FAST_RW_PRESS);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_FAST_RW);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_FAST_RW);
                RF4CEControllerBackward();
            }
            break;

        case ID_SLOW_FW:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &PDICON_FAST_FW_PRESS);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_FAST_FW);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_FAST_FW);
                RF4CEControllerForward();
            }
            break;

		case ID_PLAY_PAUSE:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &PDICON_PLAY_PAUSE_PRESS);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_PLAY_PAUSE);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_PLAY_PAUSE);
                
                if(playPause)
                {
                    playPause = FALSE;
                    RF4CEControllerPlay();
                }else
                {
                    playPause = TRUE;
                    RF4CEControllerPause();
                }
            }
            break;

		case ID_STOP:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &PDICON_STOP_PRESS);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_STOP);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_STOP);
                RF4CEControllerStop();
            }
            break;

		case ID_PLAY_INC:
            if(objMsg == BTN_MSG_PRESSED)
            {           // change face picture
                BtnSetBitmap(pObj, (void *) &PDICON_PLAY_INC_PRESS);
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_PLAY_INC);
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                BtnSetBitmap(pObj, (void *) &PDICON_PLAY_INC);
                RF4CEControllerEject();
            }
            break;

		case ID_SETUP_PLAYER:
            if(objMsg == BTN_MSG_RELEASED)
            {
                RF4CEControllerSetupMenu();
            }
            break;

		case ID_DISC_MENU:
            if(objMsg == BTN_MSG_RELEASED)
            {
                RF4CEControllerRootMenu();
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

	return 1;
}

