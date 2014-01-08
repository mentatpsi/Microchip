/*****************************************************************************
 * Microchip Graphics Library Demo Application
 *****************************************************************************
 * FileName:        RemoteFavoritesInterface.c
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

#define FAVORITECOUNT 5
XCHAR *pFavList[FAVORITECOUNT] =    {
                        "Cartoons",
                        "Movies",
                        "News",
                        "Sports",
                        "Weather"
                        };                                    
XCHAR* pMyItemList;
 

/************************************************************************
 Function: CreateFavoritesScreen()
                                                                       
 Overview: Creates the favorites screen.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateFavoritesScreen(void)
{
    WORD state = BTN_DRAW | BTN_TOGGLE;

    GOLFree();                      // free memory for the objects in the previous linked list and start new list
	
    HideScreenRendering();

	CreateCommonObjects();

    BtnCreate
    (
        ID_FAV_1,                 
        FAVLeft(0),                
        FAVTop(0),
        FAVRight(0),
        FAVBottom(0),
        0,                          
        state | BTN_PRESSED,
        NULL, 
        pFavList[0],
        pFavMenuScheme
    );                              

    BtnCreate
    (
        ID_FAV_2,                 
        FAVLeft(0),                
        FAVTop(1),
        FAVRight(0),
        FAVBottom(1),
        0,                          
        state,
        NULL, 
        pFavList[1],
        pFavMenuScheme
    );                              

    BtnCreate
    (
        ID_FAV_3,                 
        FAVLeft(0),                
        FAVTop(2),
        FAVRight(0),
        FAVBottom(2),
        0,                          
        state,
        NULL, 
        pFavList[2],
        pFavMenuScheme
    );                              

    BtnCreate
    (
        ID_FAV_4,                 
        FAVLeft(0),                
        FAVTop(3),
        FAVRight(0),
        FAVBottom(3),
        0,                          
        state,
        NULL, 
        pFavList[3],
        pFavMenuScheme
    );                              

    BtnCreate
    (
        ID_FAV_5,     
        FAVLeft(0),                
        FAVTop(4),
        FAVRight(0),
        FAVBottom(4),
        0,                          
        state,
        NULL, 
        pFavList[4],
        pFavMenuScheme
    );                              

 
    state = BTN_DRAW;

    BtnCreate
    (
        ID_FAV_UP,                 
        KPLeft(0),                
        KPTop(3),
        KPRight(0),
        KPBottom(3),
        0,                          
        state, 
        NULL, 
        "Up",
        pFavMenuScheme
    );                     

    BtnCreate
    (
        ID_FAV_DOWN,                 
        KPLeft(1),                
        KPTop(3),
        KPRight(1),
        KPBottom(3),
        0,                          
        state, 
        NULL, 
        "Down",
        pFavMenuScheme
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
        pFavMenuScheme
    );                     
        
	
}	

/************************************************************************
 Function: WORD MsgFavoritesCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the play disc screen 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD MsgFavoritesCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    WORD objID, FavItems;

    objID = GetObjID(pObj); 

    switch(objID)
    {
        case ID_FAV_1:
        case ID_FAV_2:
        case ID_FAV_3:
        case ID_FAV_4:
        case ID_FAV_5:
            if ((objMsg == BTN_MSG_PRESSED) || (objMsg == BTN_MSG_RELEASED))
            {
                // toggle the button that is pressed
                for (FavItems = 0; FavItems < FAV_ROWSNUM; FavItems++)
                {
                    if ((ID_FAV_1 + FavItems) != objID)
                    {
                        if (GetState(GOLFindObject(ID_FAV_1 + FavItems), BTN_PRESSED) == BTN_PRESSED)
                        {
                            ClrState(GOLFindObject(ID_FAV_1 + FavItems), BTN_PRESSED);
                            SetState(GOLFindObject(ID_FAV_1 + FavItems), BTN_DRAW);
                        }
                    }    
                }    
            }
            break;

        case ID_FAV_UP:

            if(objMsg == BTN_MSG_PRESSED)
            {           
                // toggle the button that is pressed and press the next button
                for (FavItems = 0; FavItems < FAV_ROWSNUM; FavItems++)
                {
                    if (GetState(GOLFindObject(ID_FAV_1 + FavItems), BTN_PRESSED) == BTN_PRESSED)
                    {
                        if (ID_FAV_1 + FavItems - 1 >= ID_FAV_1)
                        {
                            SetState(GOLFindObject(ID_FAV_1 + FavItems - 1), BTN_PRESSED);
                            SetState(GOLFindObject(ID_FAV_1 + FavItems - 1), BTN_DRAW);
                            ClrState(GOLFindObject(ID_FAV_1 + FavItems), BTN_PRESSED);
                            SetState(GOLFindObject(ID_FAV_1 + FavItems), BTN_DRAW);
                            FavItems = FAV_ROWSNUM;
                        }
                    }   
                }    
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                // do nothing 
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                // do nothing 
            }
            break;

        case ID_FAV_DOWN:

            if(objMsg == BTN_MSG_PRESSED)
            {
                // find the button that is pressed 
                for (FavItems = 0; FavItems < FAV_ROWSNUM; FavItems++)
                {
                    if (GetState(GOLFindObject(ID_FAV_1 + FavItems), BTN_PRESSED) == BTN_PRESSED)
                    {
                        if (ID_FAV_1 + FavItems + 1 < ID_FAV_1 + FAV_ROWSNUM)
                        {
                            SetState(GOLFindObject(ID_FAV_1 + FavItems + 1), BTN_PRESSED);
                            SetState(GOLFindObject(ID_FAV_1 + FavItems + 1), BTN_DRAW);
                            ClrState(GOLFindObject(ID_FAV_1 + FavItems), BTN_PRESSED);
                            SetState(GOLFindObject(ID_FAV_1 + FavItems), BTN_DRAW);
                            FavItems = FAV_ROWSNUM;
                        }
                    }   
                }    
            }
            else if(objMsg == BTN_MSG_CANCELPRESS)
            {
                // do nothing 
            }
            else if(objMsg == BTN_MSG_RELEASED)
            {
                // do nothing 
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

