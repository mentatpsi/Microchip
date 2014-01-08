/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
*****************************************************************************
 * FileName:        SlideShowDemo.c
 * Dependencies:    SlideShowDemo.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
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
 * Paolo A. Tamayo		03/10/08	... 
 *****************************************************************************/
#include "MainDemo.h"

/////////////////////////////////////////////////////////////////////////////
//                            MACROS
/////////////////////////////////////////////////////////////////////////////
#define PIXDATAMAX  2 

/////////////////////////////////////////////////////////////////////////////
//                            IMAGES USED
/////////////////////////////////////////////////////////////////////////////
// rose and intro declared in ResourcesExternalSPIFlash.h

/////////////////////////////////////////////////////////////////////////////
//                            STRING USED
/////////////////////////////////////////////////////////////////////////////
extern XCHAR            RightArrowStr[];
extern XCHAR            LeftArrowStr[];
extern XCHAR            ExitStr[];

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void                    InitPixData(void);

/////////////////////////////////////////////////////////////////////////////

//                            STRUCTURES
/////////////////////////////////////////////////////////////////////////////
typedef struct
{
    void    *pImage;    // pointer to the image used
    void    *pPrev;     // pointer to the previous list member
    void    *pNext;     // pointer to the next list member
} PIXDATA;

/////////////////////////////////////////////////////////////////////////////
//                            GLOBALS
/////////////////////////////////////////////////////////////////////////////
// array of structures that will hold the strings and its pointers to corresponding font tables.
// this will be configured as a ringed linked list
PIXDATA     PixArray[PIXDATAMAX];

// global pointer to the linked list.
PIXDATA     *pPixData;
GOL_SCHEME  *SlideShowScheme;

/************************************************************************
 Function: void CreateSlideShowDemo(void)
                                                                       
 Overview: Creates the Slide Show demo screen. Images are taken from 
 		   flash memory.
  		                                         
 Input: none
                                                                       
 Output: none
************************************************************************/
void CreateSlideShowDemo(void)
{
    SHORT   pixWidth, pixHeight;

    GOLFree();  // free memory for the objects in the previous linked list and start new list
    SetColor(BLACK);
    ClearDevice();

    SlideShowScheme = GOLCreateScheme();

    SlideShowScheme->CommonBkColor = BLACK;

    InitPixData();
    pixWidth = GetImageWidth(pPixData->pImage);
    pixHeight = GetImageHeight(pPixData->pImage);

    PictCreate(	ID_PICTURE2, 
    			0, 10, GetMaxX(), 			// dimensions
    			GetMaxY() - 40, 					
    			PICT_DRAW,							// draw the picture when created
    			2,          						// scale factor is x2
    			(void *) &(rose), 
    			SlideShowScheme);

    // create the components of the demo
    CreateCtrlButtons(ExitStr, NULL, LeftArrowStr, RightArrowStr);
}

/************************************************************************
 Function: WORD SlideShowDemoMsgCallback(WORD objMsg, OBJ_HEADER* pObj, 
 										GOL_MSG* pMsg)

 Overview: This function is called by GOLMsg() function when in this 
 		   particular demo each time the valid message is received for the 
  		   objects in the demo.
           
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 

 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD SlideShowDemoMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    WORD    objectID;
    PICTURE *pPict;

    objectID = GetObjID(pObj);

    switch(objectID)
    {
        case ID_BUTTON_A:
            if(objMsg == BTN_MSG_RELEASED)
            {   // check if button is pressed
                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);
                free(SlideShowScheme);

                // go to menu screen
                screenState = CREATE_DEMOSELECTION;
            }

            return (1);

        case ID_BUTTON_C:
            if(objMsg == BTN_MSG_RELEASED)
            {   // check if button is pressed
                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);
                pPixData = pPixData->pPrev;

                pPict = (PICTURE *)GOLFindObject(ID_PICTURE2);  // get pointer to static text
                PictSetBitmap(pPict, pPixData->pImage);
                SetState(pPict, DRAW);                          // set redraw state
            }

            break;

        case ID_BUTTON_D:
            if(objMsg == BTN_MSG_RELEASED)
            {

                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);
                pPixData = pPixData->pNext;

                pPict = (PICTURE *)GOLFindObject(ID_PICTURE2);  // get pointer to static text
                PictSetBitmap(pPict, pPixData->pImage);
                SetState(pPict, DRAW);                          // set redraw state
            }

            break;

        default:
            return (1);
    }

    return (1);
}

/* */
void InitPixData(void)
{
    int i;

    for(i = 0; i < PIXDATAMAX; i++)
    {
        switch(i)
        {
            case 0:     PixArray[i].pImage = (void *) &rose; break;
            case 1:     PixArray[i].pImage = (void *) &intro; break;
            default:    break;
        }

        // make the list a ring list
        if(i == (PIXDATAMAX - 1))
        {
            PixArray[i].pNext = (void *) &PixArray[0];
            PixArray[i].pPrev = (void *) &PixArray[i - 1];
        }
        else if(i == 0)
        {
            PixArray[i].pNext = (void *) &PixArray[i + 1];
            PixArray[i].pPrev = (void *) &PixArray[PIXDATAMAX - 1];
        }
        else
        {
            PixArray[i].pNext = (void *) &PixArray[i + 1];
            PixArray[i].pPrev = (void *) &PixArray[i - 1];
        }
    }

    pPixData = &PixArray[0];
}
