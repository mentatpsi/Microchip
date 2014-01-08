/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  TextEntry
 *****************************************************************************
 * FileName:        Textentry.c
 * Dependencies:    Textentry.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 Version 3.00, C32
 * Linker:          MPLAB LINK30, LINK32
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 10/24/08	    ...
 * 07/01/09	    Updated for 2D accelerated primitive support.
 * 04/15/10	    Corrected TeSetBuffer() issue on string max size.
 * 08/04/11     Modified widget draw states to correct flow of rendering.
 *              Added check on the Bar() calls in cases when the non-blocking
 *              is used.
 * 12/02/11     Fix memory leak issue when TeCreateKeyMembers fails to allocate
 *              memory for all keys.
 * 12/13/11     Fix issue when displaying string with length equal to max size.
 * 03/05/12     - Added Gradient feature.
 *              - Enabled use of rounded buttons. User can now specify the rounded
 *                edge radius in the GraphicsConfing.h file. 
*                 (See TE_ROUNDEDBUTTON_RADIUS macro for deatils).
 *****************************************************************************/
#include "Graphics/Graphics.h"

#ifdef USE_TEXTENTRY

/*********************************************************************
* Function: TEXTENTRY *TeCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state
*					SHORT horizontalKeys, SHORT verticalKeys, XCHAR *pText[], 
*					void *pBuffer, WORD bufferLength, void *pDisplayFont, 
*					GOL_SCHEME *pScheme)
*
*
* Notes: 
*
********************************************************************/
TEXTENTRY *TeCreate
(
    WORD        ID,
    SHORT       left,
    SHORT       top,
    SHORT       right,
    SHORT       bottom,
    WORD        state,
    SHORT       horizontalKeys,
    SHORT       verticalKeys,
    XCHAR       *pText[],
    void        *pBuffer,
    WORD        bufferLength,
    void        *pDisplayFont,
    GOL_SCHEME  *pScheme
)
{
    TEXTENTRY   *pTe = NULL;                    //Text entry
    pTe = (TEXTENTRY *)GFX_malloc(sizeof(TEXTENTRY));
    if(pTe == NULL)
        return (NULL);

    pTe->hdr.ID = ID;
    pTe->hdr.pNxtObj = NULL;
    pTe->hdr.type = OBJ_TEXTENTRY;              // set object type
    pTe->hdr.left = left;                       // left parameter of the text-entry
    pTe->hdr.top = top;                         // top parameter of the text-entry
    pTe->hdr.right = right;                     // right parameter of the text-entry
    pTe->hdr.bottom = bottom;                   // bottom parameter of the text-entry
    pTe->hdr.state = state;                     // State of the Text-Entry
    pTe->horizontalKeys = horizontalKeys;       // number of horizontal keys
    pTe->verticalKeys = verticalKeys;           // number of vertical keys
    pTe->CurrentLength = 0;                     // current length of text
    pTe->pHeadOfList = NULL;
    TeSetBuffer(pTe, pBuffer, bufferLength);    // set the text to be displayed buffer length is also initialized in this call
    pTe->pActiveKey = NULL;
    pTe->hdr.DrawObj = TeDraw;					// draw function
    pTe->hdr.MsgObj = TeTranslateMsg;			// message function
    pTe->hdr.MsgDefaultObj = TeMsgDefault;		// default message function
    pTe->hdr.FreeObj = TeDelKeyMembers;			// free function

    // Set the color scheme to be used
    if(pScheme == NULL)
        pTe->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pTe->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    // Set the font to be used
    if(pDisplayFont == NULL)
        pTe->pDisplayFont = (void *) &FONTDEFAULT;
    else
        pTe->pDisplayFont = pDisplayFont;

    //check if either values of horizontal keys and vertical keys are equal to zero
    if((pTe->horizontalKeys != 0) || (pTe->verticalKeys != 0))
    {

        //create the key members, return null if not successful
        if(TeCreateKeyMembers(pTe, pText) == NULL)
        {
            TeDelKeyMembers(pTe);
            GFX_free(pTe);
            return (NULL);
        }
    }

    //Add this new widget object to the GOL list	
    GOLAddObject((OBJ_HEADER *)pTe);
    return (pTe);
}   //end TeCreate()

/*********************************************************************
* Function: WORD TeDraw(void *pObj)
*
* Notes: This function draws the keys with their appropriate text
*
********************************************************************/
WORD TeDraw(void *pObj)
{
    static GFX_COLOR    faceClr, embossLtClr, embossDkClr;
    static WORD         xText, yText;
    static XCHAR        XcharTmp;
    static KEYMEMBER    *pKeyTemp = NULL;

    static WORD         CountOfKeys = 0;
    static WORD         counter = 0;
    static XCHAR        hideChar[2] = {0x2A, 0x00};
    
    GFX_COLOR           color1, color2;

    typedef enum
    {
        TE_START,
        TE_HIDE_WIDGET,
        TE_DRAW_PANEL,
        TE_INIT_DRAW_EDITBOX,
        TE_DRAW_EDITBOX,
        TE_DRAW_KEY_INIT,
        TE_DRAW_KEY_SET_PANEL,
        TE_DRAW_KEY_DRAW_PANEL,
        TE_DRAW_KEY_TEXT,
        TE_DRAW_KEY_UPDATE,
        TE_UPDATE_STRING_INIT,
        TE_UPDATE_STRING,
        TE_WAIT_ERASE_EBOX_AREA,
        TE_UPDATE_CHARACTERS,
    } TE_DRAW_STATES;

    static TE_DRAW_STATES state = TE_START;
    TEXTENTRY *pTe;

    pTe = (TEXTENTRY *)pObj;

	while(1)
	{
		if(IsDeviceBusy())
			return (0);

		switch(state)
		{
			case TE_START:

				if(GetState(pTe, TE_HIDE))
				{
					SetColor(pTe->hdr.pGolScheme->CommonBkColor);
					state = TE_HIDE_WIDGET;
					// no break here so it falls through to the TE_HIDE_WIDGET state.
				}
				else
				{
					if(GetState(pTe, TE_DRAW))
					{
                        if (TE_ROUNDEDBUTTON_RADIUS == 0)
                        {
                            color1 = pTe->hdr.pGolScheme->EmbossDkColor;
                            color2 = pTe->hdr.pGolScheme->EmbossLtColor;
                        }
                        else
                        {                    
                            color1 = pTe->hdr.pGolScheme->Color0;
                            color2 = pTe->hdr.pGolScheme->Color0;
                        }

						/************DRAW THE WIDGET PANEL*****************************/
						GOLPanelDraw
						(
							pTe->hdr.left,
							pTe->hdr.top,
							pTe->hdr.right,
							pTe->hdr.bottom,
							0,
							pTe->hdr.pGolScheme->Color0,        //face color of panel
							color1, //emboss dark color
							color2, //emboss light color
							NULL,
							GOL_EMBOSS_SIZE
						);
						state = TE_DRAW_PANEL;
						break;
					}

					// update the keys (if TE_UPDATE_TEXT is also set it will also be redrawn)
					// at the states after the keys are updated
					else if(GetState(pTe, TE_UPDATE_KEY))
					{
						state = TE_DRAW_KEY_INIT;
						break;
					}

					// check if updating only the text displayed
					else if(GetState(pTe, TE_UPDATE_TEXT))
					{
						state = TE_UPDATE_STRING_INIT;
						break;
					}
				}

			/*hide the widget*/
			case TE_HIDE_WIDGET:
			    // this state only gets entered if IsDeviceBusy() immediately after while(1) returns a 0.
				if (!Bar(pTe->hdr.left, pTe->hdr.top, pTe->hdr.right, pTe->hdr.bottom))
                    return (0);
                else 
                {
    				state = TE_START;
	    			return (1);
                }

			/*Draw the widget of the Text-Entry*/
			case TE_DRAW_PANEL:
				if(!GOLPanelDrawTsk()) 
					return (0);
				state = TE_INIT_DRAW_EDITBOX;

			case TE_INIT_DRAW_EDITBOX:

				//Draw the editbox
				GOLPanelDraw
				(
					pTe->hdr.left,
					pTe->hdr.top,
					pTe->hdr.right,
					pTe->hdr.top + GetTextHeight(pTe->pDisplayFont) + GOL_EMBOSS_SIZE,
					0,
					pTe->hdr.pGolScheme->Color1; ,
					pTe->hdr.pGolScheme->EmbossDkColor,
					pTe->hdr.pGolScheme->EmbossLtColor,
					NULL,
					GOL_EMBOSS_SIZE
				);

				state = TE_DRAW_EDITBOX;

			case TE_DRAW_EDITBOX:
				if(!GOLPanelDrawTsk())
					return (0);
				state = TE_DRAW_KEY_INIT;

			/* ********************************************************************* */
			/*                  Update the keys                                      */
			/* ********************************************************************* */
			case TE_DRAW_KEY_INIT:
				embossLtClr = pTe->hdr.pGolScheme->EmbossLtColor;
				embossDkClr = pTe->hdr.pGolScheme->EmbossDkColor;
				faceClr = pTe->hdr.pGolScheme->Color0;

				// if the active key update flag is set, only one needs to be redrawn
				if((GetState(pTe, TE_DRAW) != TE_DRAW) && (pTe->pActiveKey->update == TRUE))
				{
					CountOfKeys = (pTe->horizontalKeys * pTe->verticalKeys) -
					1;
					pKeyTemp = pTe->pActiveKey;
				}
				else
				{
					CountOfKeys = 0;
					pKeyTemp = pTe->pHeadOfList;
				}

				state = TE_DRAW_KEY_SET_PANEL;

			case TE_DRAW_KEY_SET_PANEL:
				if(CountOfKeys < (pTe->horizontalKeys * pTe->verticalKeys))
				{

					// check if we need to draw the panel
					if(GetState(pTe, TE_DRAW) != TE_DRAW)
					{
						if(pKeyTemp->update == TRUE)
						{

							// set the colors needed
							if(GetState(pTe, TE_KEY_PRESSED))
							{
								embossLtClr = pTe->hdr.pGolScheme->EmbossDkColor;
								embossDkClr = pTe->hdr.pGolScheme->EmbossLtColor;
								faceClr = pTe->hdr.pGolScheme->Color1;
							}
							else
							{
								embossLtClr = pTe->hdr.pGolScheme->EmbossLtColor;
								embossDkClr = pTe->hdr.pGolScheme->EmbossDkColor;
								faceClr = pTe->hdr.pGolScheme->Color0;
							}
						}
						else
						{
							state = TE_DRAW_KEY_UPDATE;
							break;
						}
					}

					if(GetState(pTe, TE_DISABLED) == TE_DISABLED)
					{
						faceClr = SetColor(pTe->hdr.pGolScheme->ColorDisabled);
					}

#ifdef USE_GRADIENT
                    // set the gradient parameters
                    SetGOLPanelGradient(pTe->hdr.pGolScheme);                
#endif

					// set up the panel
					GOLPanelDraw
					(
						pKeyTemp->left+TE_ROUNDEDBUTTON_RADIUS,
						pKeyTemp->top+TE_ROUNDEDBUTTON_RADIUS,
						pKeyTemp->right-TE_ROUNDEDBUTTON_RADIUS,
						pKeyTemp->bottom-TE_ROUNDEDBUTTON_RADIUS,
						TE_ROUNDEDBUTTON_RADIUS,
						faceClr,
						embossLtClr,
						embossDkClr,
						NULL,
						GOL_EMBOSS_SIZE
					);

					state = TE_DRAW_KEY_DRAW_PANEL;
				}
				else
				{
					state = TE_UPDATE_STRING_INIT;
					break;
				}

			case TE_DRAW_KEY_DRAW_PANEL:
				if(!GOLPanelDrawTsk())
					return (0);

				// reset the update flag since the key panel is already redrawn   	
				pKeyTemp->update = FALSE;

				//set the text coordinates of the drawn key
				xText = ((pKeyTemp->left) + (pKeyTemp->right) - (pKeyTemp->textWidth)) >> 1;
				yText = ((pKeyTemp->bottom) + (pKeyTemp->top) - (pKeyTemp->textHeight)) >> 1;

				//set color of text
				// if the object is disabled, draw the disabled colors
				if(GetState(pTe, TE_DISABLED) == TE_DISABLED)
				{
					SetColor(pTe->hdr.pGolScheme->TextColorDisabled);
				}
				else
				{
					if((GetState(pTe, TE_DRAW) != TE_DRAW) && (GetState(pTe, TE_KEY_PRESSED)) == TE_KEY_PRESSED)
					{
						SetColor(pTe->hdr.pGolScheme->TextColor1);
					}
					else
					{
						SetColor(pTe->hdr.pGolScheme->TextColor0);
					}
				}

				//output the text
				MoveTo(xText, yText);

				// set the font to be used
				SetFont(pTe->hdr.pGolScheme->pFont);

				state = TE_DRAW_KEY_TEXT;

			case TE_DRAW_KEY_TEXT:
				if(!OutText(pKeyTemp->pKeyName))
					return (0);
				state = TE_DRAW_KEY_UPDATE;

			case TE_DRAW_KEY_UPDATE:

				// update loop variables
				CountOfKeys++;
				pKeyTemp = pKeyTemp->pNextKey;

				state = TE_DRAW_KEY_SET_PANEL;
				break;

			/* ********************************************************************* */
			/*                  Update the displayed string                          */
			/* ********************************************************************* */
			case TE_UPDATE_STRING_INIT:

				// check if there are characters to remove
				if(pTe->pActiveKey != NULL)
				{
					if(pTe->pActiveKey->command == TE_DELETE_COM)
					{
						if(pTe->CurrentLength == 0)
						{
							state = TE_START;
							return (1);
						}
					}
				}
				else
				{

					// check if text indeed needs to be updated
					if((pTe->CurrentLength == pTe->outputLenMax) && (GetState(pTe, TE_UPDATE_TEXT)))
					{
						state = TE_START;
						return (1);
					}
				}

				//set the clipping region
				SetClipRgn
				(
					pTe->hdr.left + GOL_EMBOSS_SIZE,
					pTe->hdr.top + GOL_EMBOSS_SIZE,
					pTe->hdr.right - GOL_EMBOSS_SIZE,
					pTe->hdr.top + GOL_EMBOSS_SIZE + GetTextHeight(pTe->pDisplayFont)
				);

				SetClip(1);     //set the clipping
				if(GetState(pTe, TE_DRAW))
				{

					// update only the displayed text
					// position the string rendering on the right position
					if(GetState(pTe, TE_ECHO_HIDE))
					{

						// fill the area with '*' character so we use the width of this character
						MoveTo
						(
							pTe->hdr.right - 4 - GOL_EMBOSS_SIZE - (GetTextWidth(hideChar, pTe->pDisplayFont) * pTe->CurrentLength),
							pTe->hdr.top + GOL_EMBOSS_SIZE
						);
					}
					else
					{
						MoveTo
						(
							pTe->hdr.right - 4 - GOL_EMBOSS_SIZE - GetTextWidth(pTe->pTeOutput, pTe->pDisplayFont),
							pTe->hdr.top + GOL_EMBOSS_SIZE
						);
					}
				}
				else if(GetState(pTe, TE_UPDATE_TEXT))
				{

					// erase the current text by drawing a bar over the edit box area
					SetColor(pTe->hdr.pGolScheme->Color1);

					// we have to make sure we finish the Bar() first before we continue.
					state = TE_WAIT_ERASE_EBOX_AREA;
					break;
				}
				else
				{
					SetClip(0); //reset the clipping
					state = TE_START;
					return (1);
				}

				counter = 0;
				state = TE_UPDATE_STRING;
				break;

			case TE_WAIT_ERASE_EBOX_AREA:
                if (!Bar
					(
						pTe->hdr.left + GOL_EMBOSS_SIZE,
						pTe->hdr.top + GOL_EMBOSS_SIZE,
						pTe->hdr.right - GOL_EMBOSS_SIZE,
						pTe->hdr.top + GOL_EMBOSS_SIZE + GetTextHeight(pTe->pDisplayFont)
					))
                    return 0;

				// check if the command given is delete a character
				if(pTe->pActiveKey->command == TE_DELETE_COM)
				{
					*(pTe->pTeOutput + (--pTe->CurrentLength)) = 0;
				}

				// position the cursor to the start of string rendering
				// notice that we need to remove the characters first before we position the cursor when
				// deleting characters
				if(GetState(pTe, TE_ECHO_HIDE))
				{

					// fill the area with '*' character so we use the width of this character
					MoveTo
					(
						pTe->hdr.right - 4 - GOL_EMBOSS_SIZE - (GetTextWidth(hideChar, pTe->pDisplayFont) * (pTe->CurrentLength)),
						pTe->hdr.top + GOL_EMBOSS_SIZE
					);
				}
				else
				{
					MoveTo
					(
						pTe->hdr.right - 4 - GOL_EMBOSS_SIZE - GetTextWidth(pTe->pTeOutput, pTe->pDisplayFont),
						pTe->hdr.top + GOL_EMBOSS_SIZE
					);
				}

				counter = 0;
				state = TE_UPDATE_STRING;
                // add a break here to force a check of IsDeviceBusy() so when last Bar() function is still 
                // ongoing it will wait for it to finish.
                break;

			case TE_UPDATE_STRING:

				//output the text
				SetColor(pTe->hdr.pGolScheme->TextColor1);
				SetFont(pTe->pDisplayFont);

				// this is manually doing the OutText() function but with the capability to replace the
				// characters to the * character when hide echo is enabled.							
				XcharTmp = *((pTe->pTeOutput) + counter);
				if(XcharTmp < (XCHAR)15)
				{

					// update is done time to return to start and exit with success
					SetClip(0); //reset the clipping								
					state = TE_START;
					return (1);
				}
				else
				{
					if(GetState(pTe, TE_ECHO_HIDE))
						OutChar(0x2A);
					else
						OutChar(XcharTmp);
					state = TE_UPDATE_CHARACTERS;
				}

			case TE_UPDATE_CHARACTERS:
				if(IsDeviceBusy()) return (0);
				counter++;
				state = TE_UPDATE_STRING;
				break;
		} //end switch
	} // end of while(1)
} //end TeDraw()

/*********************************************************************
* Function: TeTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Notes: Function to check which key was pressed/released
*
********************************************************************/
WORD TeTranslateMsg(void *pObj, GOL_MSG *pMsg)
{
    SHORT       NumberOfKeys, param1, param2;
    KEYMEMBER   *pKeyTemp = NULL;
    TEXTENTRY *pTe;

    pTe = (TEXTENTRY *)pObj;

    // Check if disabled first
    if(GetState(pTe, TE_DISABLED))
        return (OBJ_MSG_INVALID);

        #ifdef USE_TOUCHSCREEN

    //find the total number of keys
    NumberOfKeys = (pTe->horizontalKeys) * (pTe->verticalKeys);
    param1 = pMsg->param1;
    param2 = pMsg->param2;

    if((pMsg->type == TYPE_TOUCHSCREEN))
    {

        // Check if it falls in the panel of the TextEntry
        if
        (
            (pTe->hdr.left < pMsg->param1) &&
            (pTe->hdr.right > pMsg->param1) &&
            (pTe->hdr.top + (GetTextHeight(pTe->pDisplayFont) + (GOL_EMBOSS_SIZE << 1)) < pMsg->param2) &&
            (pTe->hdr.bottom > pMsg->param2)
        )
        {

            /* If it fell inside the TextEntry panel, go through the link list and check which one was pressed
			   At this point the touch screen event is either EVENT_MOVE or EVENT_PRESS.   
			*/

            //point to the head of the link list
            pKeyTemp = pTe->pHeadOfList;

            while(pKeyTemp != NULL)
            {
                if
                (
                    (pKeyTemp->left < param1) &&
                    (pKeyTemp->right > param1) &&
                    (pKeyTemp->top < param2) &&
                    (pKeyTemp->bottom > param2)
                )
                {
                    if(pMsg->uiEvent == EVENT_RELEASE)
                    {
                        pTe->pActiveKey = pKeyTemp;
                        pKeyTemp->update = TRUE;

                        if(pTe->pActiveKey->state == TE_KEY_PRESSED)
                        {
                            if(pKeyTemp->command == 0)
                                return (TE_MSG_ADD_CHAR);

                            //command for a TE_DELETE_COM key
                            if(pKeyTemp->command == TE_DELETE_COM)
                                return (TE_MSG_DELETE);

                            //command for a TE_SPACE_COM key 0x20	
                            if(pKeyTemp->command == TE_SPACE_COM)
                                return (TE_MSG_SPACE);

                            //command for a TE_ENTER_COM key        	
                            if(pKeyTemp->command == TE_ENTER_COM)
                                return (TE_MSG_ENTER);
                        }

                        // this is a catch all backup
                        return (TE_MSG_RELEASED);
                    }
                    else
                    {

                        // to shift the press to another key make sure that there are no other
                        // keys currently pressed. If there is one it must be released first.
                        // check if there are previously pressed keys
                        if(GetState(pTe, TE_KEY_PRESSED))
                        {

                            // there is a key being pressed.
                            if(pKeyTemp->index != pTe->pActiveKey->index)
                            {

                                // release the currently pressed key first
                                pTe->pActiveKey->update = TRUE;
                                return (TE_MSG_RELEASED);
                            }
                        }
                        else
                        {

                            // check if the active key is not pressed
                            // if not, set to press since the current touch event
                            // is either move or press
                            // check if there is an active key already set
                            // if none, set the current key as active and return a pressed mesage
                            if(pTe->pActiveKey == NULL)
                            {
                                pTe->pActiveKey = pKeyTemp;
                                pKeyTemp->update = TRUE;
                                return (TE_MSG_PRESSED);
                            }

                            if(pTe->pActiveKey->state != TE_KEY_PRESSED)
                            {
                                pTe->pActiveKey = pKeyTemp;
                                pKeyTemp->update = TRUE;
                                return (TE_MSG_PRESSED);
                            }
                            else
                            {
                                return (OBJ_MSG_INVALID);
                            }
                        }
                    }
                }
                else
                {

                    // if the key is in the pressed state and current touch is not here
                    // then it has to be redrawn
                    if(pKeyTemp->state == TE_KEY_PRESSED)
                    {
                        pTe->pActiveKey = pKeyTemp;
                        pKeyTemp->update = TRUE;
                        return (TE_MSG_RELEASED);
                    }
                }

                //access the next link list
                pKeyTemp = pKeyTemp->pNextKey;
            }   //end while
        }
        else
        {
            if((pMsg->uiEvent == EVENT_MOVE) && (GetState(pTe, TE_KEY_PRESSED)))
            {
                pTe->pActiveKey->update = TRUE;
                return (TE_MSG_RELEASED);
            }
        }
    }

    return (OBJ_MSG_INVALID);
        #endif // USE_TOUCHSCREEN		
}               //end TeTranslateMsg()

/*********************************************************************
* Function: TeMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
*
* Notes: This the default operation to change the state of the key.
*		 Called inside GOLMsg() when GOLMsgCallback() returns a 1.
*
********************************************************************/
void TeMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
{
    TEXTENTRY *pTe;

    pTe = (TEXTENTRY *)pObj;

    switch(translatedMsg)
    {
        case TE_MSG_DELETE:
            SetState(pTe, TE_UPDATE_KEY | TE_UPDATE_TEXT);
            break;

        case TE_MSG_SPACE:
            TeSpaceChar(pTe);
            SetState(pTe, TE_UPDATE_KEY | TE_UPDATE_TEXT);
            break;

        case TE_MSG_ENTER:
            SetState(pTe, TE_UPDATE_KEY);
            break;

        case TE_MSG_ADD_CHAR:
            TeAddChar(pTe);
            SetState(pTe, TE_UPDATE_KEY | TE_UPDATE_TEXT);
            break;

        case TE_MSG_PRESSED:
            (pTe->pActiveKey)->state = TE_KEY_PRESSED;
            SetState(pTe, TE_KEY_PRESSED | TE_UPDATE_KEY);
            return;

        case TE_MSG_RELEASED:
            (pTe->pActiveKey)->state = 0;
            ClrState(pTe, TE_KEY_PRESSED);  // reset pressed
            SetState(pTe, TE_UPDATE_KEY);   // redraw
            return;
    }

    if(pTe->pActiveKey != NULL)
        (pTe->pActiveKey)->state = 0;
    ClrState(pTe, TE_KEY_PRESSED);
}

/*********************************************************************
* Function: void TeClearBuffer(TEXTENTRY *pTe)
*
* Notes: This function will clear the edibox and the buffer.
*		 You must set the drawing state bit TE_UPDATE_TEXT 
*		 to update the TEXTENTRY on the screen.
*
********************************************************************/
void TeClearBuffer(TEXTENTRY *pTe)
{
    WORD    i;

    //clear the buffer
    for(i = 0; i < (pTe->outputLenMax); i++)
    {
        *(pTe->pTeOutput + i) = 0;
    }

    pTe->CurrentLength = 0;
}

/*********************************************************************
* Function: void TeSetBuffer(TEXTENTRY *pTe, XCHAR *pText, WORD size)
*
* Notes: This function will replace the currently used buffer.
*		 MaxSize defines the length of the buffer. Buffer must be
*		 a NULL terminated string.
*
********************************************************************/
void TeSetBuffer(TEXTENTRY *pTe, XCHAR *pText, WORD MaxSize)
{
    WORD    count = 0;
    XCHAR   *pTemp;

    pTemp = pText;

    while(*pTemp != 0)
    {
        if(count >= MaxSize)
            break;
        pTemp++;
        count++;
    }

    // terminate the string
    *pTemp = 0;

    pTe->CurrentLength = count;
    pTe->outputLenMax = MaxSize-1;
    pTe->pTeOutput = pText;
}

/*********************************************************************
* Function: BOOL TeIsKeyPressed(TEXTENTRY *pTe,WORD index)
*
* Notes: This function will check if the key was pressed. If no 
*		 key was pressed it will return FALSE.
*
********************************************************************/
BOOL TeIsKeyPressed(TEXTENTRY *pTe, WORD index)
{
    KEYMEMBER   *pTemp;

    pTemp = pTe->pHeadOfList;

    //search the key using the given index
    while(index != pTemp->index)
    {

        // catch all check
        if(pTemp == NULL)
            return (FALSE);
        pTemp = pTemp->pNextKey;
    }

    if(pTemp->state == TE_KEY_PRESSED)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/*********************************************************************
* Function: BOOL TeSetKeyCommand(TEXTENTRY *pTe,WORD index,WORD command)
*
* Notes: This function will assign a command to a particular key. 
*		 Returns TRUE if sucessful and FALSE if not.
*
********************************************************************/
BOOL TeSetKeyCommand(TEXTENTRY *pTe, WORD index, WORD command)
{
    KEYMEMBER   *pTemp;

    pTemp = pTe->pHeadOfList;

    //search the key using the given index
    while(index != pTemp->index)
    {

        // catch all check
        if(pTemp == NULL)
            return (FALSE);
        pTemp = pTemp->pNextKey;
    }

    pTemp->command = command;
    return (TRUE);
}

/*********************************************************************
* Function: TeGetKeyCommand(pTe, index)  
*				
* Notes: This function will return the currently used command by a key 
*		 with the given index.
*
********************************************************************/
WORD TeGetKeyCommand(TEXTENTRY *pTe, WORD index)
{
    KEYMEMBER   *pTemp;

    pTemp = pTe->pHeadOfList;

    //search the key using the given index
    while(index != pTemp->index)
    {

        // catch all check
        if(pTemp == NULL)
            return (0);
        pTemp = pTemp->pNextKey;
    }

    return (pTemp->command);
}

/*********************************************************************
* Function: BOOL TeSetKeyText(TEXTENTRY *pTe,WORD index, XCHAR *pText)
*
* Notes: This function will set the string associated with the key 
*		 with the new string pText. The key to be modified is determined 
*        by the index. Returns TRUE if sucessful and FALSE if not.
*
********************************************************************/
BOOL TeSetKeyText(TEXTENTRY *pTe, WORD index, XCHAR *pText)
{
    KEYMEMBER   *pTemp;

    pTemp = pTe->pHeadOfList;

    //search the key using the given index
    while(index != pTemp->index)
    {
        // catch all check
        if(pTemp == NULL)
            return (FALSE);
        pTemp = pTemp->pNextKey;
    }

	// Set the the text 
    pTemp->pKeyName = pText;

    return (TRUE);
}


/*********************************************************************
* Function: KEYMEMBER *TeCreateKeyMembers(TEXTENTRY *pTe,XCHAR *pText[])
*
* Notes: This function will create the members of the list
*
********************************************************************/
KEYMEMBER *TeCreateKeyMembers(TEXTENTRY *pTe, XCHAR *pText[])
{
    SHORT       NumberOfKeys, width, height;
    SHORT       keyTop, keyLeft;
    WORD        rowcount, colcount;
    WORD        index = 0;

    KEYMEMBER   *pKl = NULL;    //link list
    KEYMEMBER   *pTail = NULL;

    // determine starting positions of the keys
    keyTop = pTe->hdr.top + GetTextHeight(pTe->pDisplayFont) + (GOL_EMBOSS_SIZE << 1);
    keyLeft = pTe->hdr.left;    

    //calculate the total number of keys, and width and height of each key
    NumberOfKeys = pTe->horizontalKeys * pTe->verticalKeys;
    width = (pTe->hdr.right - keyLeft + 1) / pTe->horizontalKeys;
    height = (pTe->hdr.bottom - keyTop + 1) / pTe->verticalKeys;

    /*create the list and calculate the coordinates of each bottom, and the textwidth/textheight of each font*/

    //Add a list for each key
    for(colcount = 0; colcount < pTe->verticalKeys; colcount++)
    {
        for(rowcount = 0; rowcount < pTe->horizontalKeys; rowcount++)
        {

            //get storage for new entry
            pKl = (KEYMEMBER *)GFX_malloc(sizeof(KEYMEMBER));
            if(pKl == NULL)
			{
				TeDelKeyMembers(pTe);
                return (NULL);
			}	
            if(pTe->pHeadOfList == NULL)
                pTe->pHeadOfList = pKl;
            if(pTail == NULL)
            {
                pTail = pKl;
            }
            else
            {
                pTail->pNextKey = pKl;
                pTail = pTail->pNextKey;
            }

            //set the index for the new list
            pKl->index = index;

            // set update flag to off
            pKl->update = FALSE;

            //calculate the x-y coordinate for each key
            pKl->left 	= keyLeft + (rowcount * width);
            pKl->top 	= keyTop  + (colcount * height);
            pKl->right 	= keyLeft + ((rowcount + 1) * width);
            pKl->bottom = keyTop  + ((colcount + 1) * height);

            //Add the text to the list and increase the index
            pKl->pKeyName = pText[index++];

            //set the COMMAND to NULL for all keys
            pKl->command = 0;

            //calculate the textwidth, textheight
            pKl->textWidth = 0;
            pKl->textHeight = 0;
            if(pKl->pKeyName != NULL)
            {

                // Calculate the text width & height
                pKl->textWidth = GetTextWidth(pKl->pKeyName, pTe->hdr.pGolScheme->pFont);
                pKl->textHeight = GetTextHeight(pTe->hdr.pGolScheme->pFont);
            } //end if
        } //end for	
    } //end for	

    pTail->pNextKey = NULL;

    return (pKl);
}

/*********************************************************************
* Function: void TeDelKeyMembers(void *pObj)
*
* Notes: This function will delete the members of the list
********************************************************************/
void TeDelKeyMembers(void *pObj)
{
    KEYMEMBER   *pCurItem;
    KEYMEMBER   *pItem;
    TEXTENTRY *pTe;

    pTe = (TEXTENTRY *)pObj;

    pCurItem = pTe->pHeadOfList;

    while(pCurItem != NULL)
    {
        pItem = pCurItem;
        pCurItem = pCurItem->pNextKey;
        GFX_free(pItem);
    }

    pTe->pHeadOfList = NULL;
}

/*********************************************************************
* Function: void TeSpaceChar(TEXTENTRY *pTe)
*
* Notes: This function will add a space to the buffer/editbox
********************************************************************/
void TeSpaceChar(TEXTENTRY *pTe)
{

    //first determine if the array has not overflown
    if((pTe->CurrentLength) < pTe->outputLenMax)
    {
        *(pTe->pTeOutput + (pTe->CurrentLength)) = 0x20;
        *(pTe->pTeOutput + (pTe->CurrentLength) + 1) = 0;
    }   //end if	
    (pTe->CurrentLength)++;
}

/*********************************************************************
* Function: void TeAddChar(TEXTENTRY *pTe)
*
* Notes: This function will add a character to the buffer/editbox
********************************************************************/
void TeAddChar(TEXTENTRY *pTe)
{
    XCHAR   *pPoint;

    //first determine if the array has not overflown
    if((pTe->CurrentLength) < pTe->outputLenMax)
    {
        pPoint = (pTe->pActiveKey)->pKeyName;
        while(*(pPoint) != 0)
        {
            *(pTe->pTeOutput + (pTe->CurrentLength)) = *(pPoint)++;
        }
    }   //end if
    else
    {

        // it is full ignore the added key
        return;
    }

    (pTe->CurrentLength)++;
    // add the string terminator 
    *(pTe->pTeOutput + pTe->CurrentLength) = 0;
}

#endif // USE_TEXTENTRY
