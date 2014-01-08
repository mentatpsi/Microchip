/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        DemoSelection.c
 * Dependencies:    MainDemo.h, DemoSelection.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software LicenseJpeg
 * Agreement
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 03/10/08    ...
 *****************************************************************************/
#include "DemoSelection.h"

// strings used
extern XCHAR                LeftArrowStr[];
extern XCHAR                RightArrowStr[];
extern XCHAR                UpArrowStr[];
extern XCHAR                DownArrowStr[];
extern XCHAR                DemoStr[];

extern BYTE                 usbMSDStatus;
BYTE                        sdCardStatus;

// global tick counter
extern DWORD    tick;
DWORD           demoTick;

/************************************************************************
 Function: void InitDemoSelectStyleScheme(GOL_SCHEME *pScheme)
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void InitDemoSelectStyleScheme(GOL_SCHEME *pDemoScheme)
{
    pDemoScheme->EmbossDkColor = SCREEN_BACKGROUND_COLOR;
    pDemoScheme->EmbossLtColor = SCREEN_BACKGROUND_COLOR;
    pDemoScheme->pFont = (void *) &GOLSmallFont;
    pDemoScheme->TextColor0 = BLACK;
    pDemoScheme->ColorDisabled = SCREEN_BACKGROUND_COLOR;
    pDemoScheme->Color0 = SCREEN_BACKGROUND_COLOR;
    pDemoScheme->Color1 = SCREEN_BACKGROUND_COLOR;
}

/************************************************************************
 Function: WORD GOLDrawCallback()

 Overview: This function must be implemented by user. GOLDraw() function 
 		   calls it each time when GOL objects drawing is completed. User 
 		   drawing should be done here. GOL will not change color, line 
 		   type and clipping region settings while  this function 
 		   returns zero.

 Input: none

 Output: If the function returns non-zero the draw control will be passed 
		to GOL. GOLDraw() can proceed and re-draw objects that needs 
		to be redrawn.
************************************************************************/
WORD GOLDrawCallback(void)
{
	
	switch(screenState)
    {
        case CREATE_GOOGLEMAP:
            CreateGoogleMapScreen();                  	// create google map screen and menu
            screenState = DISPLAY_GOOGLEMAP;    		// switch to next state
            return (1);                             	// draw objects created

        case DISPLAY_GOOGLEMAP:
        	GoogleMapDrawCallback();
            return (1);                             // redraw objects if needed

        default:
            break;
    }

    return (1); // release drawing control to GOL

}


/************************************************************************
 Function: WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)

 Overview: This function must be implemented by user. GOLMsg() function 
 		   calls it each time the valid message for the object received.
           
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 

 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
	
    switch(screenState)
    {
        case DISPLAY_GOOGLEMAP:
            return (GoogleMapMsgCallback(objMsg, pObj, pMsg));

        default:
            return (1); // process message by default
    }	
	
}	

