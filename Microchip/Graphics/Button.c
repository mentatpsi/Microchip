/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Button
 *****************************************************************************
 * FileName:        Button.c
 * Dependencies:    Button.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright � 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
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
 * Date        	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07		Version 1.0 release
 * 06/26/09		Added message ID BTN_MSG_STILLPRESSED 
 *				to signify that a continuous touch on 
 *				the button through touch screen.
 * 06/29/09		Added multi-line text support on buttons
 *				must set USE_BUTTON_MULTI_LINE in 
 *				GraphicsConfig.h file.
 * 10/04/10     Added BTN_NOPANEL property state bit to make the buttons with
 *              bitmaps that is of the same size or larger than the button
 *              dimension to skip drawing of the panel. 
 * 04/20/11     Fixed KEYBOARD bug on object ID and GOL_MSG param1 comparison.
 * 06/11/12     Added Alpha Blending features.
 *****************************************************************************/
#include "Graphics/Graphics.h"

#if defined(USE_BUTTON) || defined(USE_BUTTON_MULTI_LINE)

/*********************************************************************
* Function: BUTTON  *BtnCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, SHORT radius, void *pBitmap, XCHAR *pText, 
*                              GOL_SCHEME *pScheme)
*
*
* Notes: Creates a BUTTON object and adds it to the current active list.
*        If the creation is successful, the pointer to the created Object 
*        is returned. If not successful, NULL is returned.
*
********************************************************************/
BUTTON *BtnCreate
(
    WORD        ID,
    SHORT       left,
    SHORT       top,
    SHORT       right,
    SHORT       bottom,
    SHORT       radius,
    WORD        state,
    void        *pBitmap,
    XCHAR       *pText,
    GOL_SCHEME  *pScheme
)
{
    BUTTON  *pB = NULL;
    pB = (BUTTON *)GFX_malloc(sizeof(BUTTON));
    if(pB == NULL)
        return (NULL);

    pB->hdr.ID = ID;                        // unique id assigned for referencing
    pB->hdr.pNxtObj = NULL;                 // initialize pointer to NULL
    pB->hdr.type = OBJ_BUTTON;              // set object type
    pB->hdr.left = left;                    // left position
    pB->hdr.top = top;                      // top position
    pB->hdr.right = right;                  // right position
    pB->hdr.bottom = bottom;                // bottom position
    pB->radius = radius;                    // radius
    pB->pBitmap = pBitmap;                  // location of bitmap
    pB->pText = pText;                      // location of the text
    pB->hdr.state = state;                  // state
    pB->hdr.DrawObj = BtnDraw;              // draw function
    pB->hdr.MsgObj = BtnTranslateMsg;       // message function
    pB->hdr.MsgDefaultObj = BtnMsgDefault;  // default message function
    pB->hdr.FreeObj = NULL;  				// free function

    // Set the color scheme to be used
    if(pScheme == NULL)
        pB->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pB->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    pB->textWidth = 0;
    pB->textHeight = 0;
    if(pB->pText != NULL)
    {
	    BtnSetText(pB, pText);
    }

#ifdef USE_ALPHABLEND_LITE
    pB->previousAlphaColor = BLACK;
#endif

#ifdef USE_ALPHABLEND                //Store the background image
if(pB->hdr.pGolScheme->AlphaValue != 100) 
{
    CopyPageWindow(_GFXActivePage, 
		       _GFXBackgroundPage,
                   pB->hdr.left, pB->hdr.top,pB->hdr.left, pB->hdr.top, 
		       pB->hdr.right - pB->hdr.left, 
		       pB->hdr.bottom - pB->hdr.top);	             									            
}
#endif


    GOLAddObject((OBJ_HEADER *)pB);

#ifdef USE_FOCUS
// focus and alpha blend cannot be used together    
#ifndef USE_ALPHABLEND_LITE
    if(GetState(pB, BTN_FOCUSED))
        GOLSetFocus((OBJ_HEADER *)pB);
#endif
#endif
    return (pB);
}

/*********************************************************************
* Function: BtnSetText(BUTTON *pB, XCHAR *pText)
*
*
* Notes: Sets the text used in the button.
*
********************************************************************/
void BtnSetText(BUTTON *pB, XCHAR *pText)
{
    #ifdef USE_BUTTON_MULTI_LINE
    	int     width = 0, chCtr = 0, lineCtr = 1;
    	XCHAR   ch, *pParser;
    #endif
    
    pB->pText = pText;

	#ifdef USE_BUTTON_MULTI_LINE

        // calculate width and height taking into account the multiple lines of text
        pParser = pB->pText;
        ch = *pText;

        // calculate the width (taken from the longest line)
        while(1)
        {
            if((ch == 0x000A) || (ch == 0x0000))
            {
                if(width < GetTextWidth(pParser, pB->hdr.pGolScheme->pFont))
                {
                    width = GetTextWidth(pParser, pB->hdr.pGolScheme->pFont);
                }

                if(ch == 0x000A)
                {
                    pParser = pText + chCtr + 1;
                    lineCtr++;
                }
                else
                {
                    break;
                }
            }

            chCtr++;
            ch = *(pText + chCtr);
        }

        pB->textWidth = width;
        pB->textHeight = GetTextHeight(pB->hdr.pGolScheme->pFont) * lineCtr;
	#else
		pB->textWidth = GetTextWidth(pText, pB->hdr.pGolScheme->pFont);
    	pB->textHeight = GetTextHeight(pB->hdr.pGolScheme->pFont);
    #endif // #ifdef USE_BUTTON_MULTI_LINE
}

/*********************************************************************
* Function: BtnMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
*
* Notes: This the default operation to change the state of the button.
*		 Called inside GOLMsg() when GOLMsgCallback() returns a 1.
*
********************************************************************/
void BtnMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
{

    BUTTON *pB;

    pB = (BUTTON *)pObj;

#ifdef USE_FOCUS  
    #ifndef USE_ALPHABLEND_LITE
            #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {
        if(!GetState(pB, BTN_FOCUSED))
        {
            GOLSetFocus((OBJ_HEADER *)pB);
        }
    }

            #endif
    #endif
#endif
    switch(translatedMsg)
    {
        case BTN_MSG_PRESSED:
            SetState(pB, BTN_PRESSED | BTN_DRAW);   // set pressed and redraw
            break;

        case BTN_MSG_RELEASED:
        case BTN_MSG_CANCELPRESS:
            ClrState(pB, BTN_PRESSED);              // reset pressed
            SetState(pB, BTN_DRAW);                 // redraw
            break;

        default:

            // catch all for button messages added by users and
            // behavior defined by users in message callback
            break;
    }
}

/*********************************************************************
* Function: WORD BtnTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
*
* Notes: Evaluates the message if the object will be affected by the 
*		 message or not.
*
********************************************************************/
WORD BtnTranslateMsg(void *pObj, GOL_MSG *pMsg)
{
    BUTTON *pB;

    pB = (BUTTON *)pObj;


    // Evaluate if the message is for the button
    // Check if disabled first
    if(GetState(pB, BTN_DISABLED))
        return (OBJ_MSG_INVALID);

        #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {

        // Check if it falls in the button's face
        if
        (
            (pB->hdr.left < pMsg->param1) &&
            (pB->hdr.right > pMsg->param1) &&
            (pB->hdr.top < pMsg->param2) &&
            (pB->hdr.bottom > pMsg->param2)
        )
        {
            if(GetState(pB, BTN_TOGGLE))
            {
                if(pMsg->uiEvent == EVENT_RELEASE)
                {
                    if(GetState(pB, BTN_PRESSED))
                        return (BTN_MSG_RELEASED);
                    else
                        return (BTN_MSG_PRESSED);
                }
            }
            else
            {
                if(pMsg->uiEvent == EVENT_RELEASE)
                    return (BTN_MSG_RELEASED);
                if(pMsg->uiEvent == EVENT_STILLPRESS)
                {
                    if(GetState(pB, BTN_PRESSED))
                        return (BTN_MSG_STILLPRESSED);
                }

                if(!GetState(pB, BTN_PRESSED))
                    return (BTN_MSG_PRESSED);
            }
        }
        else
        {
            if(!GetState(pB, BTN_TOGGLE))
            {
                if((pMsg->uiEvent == EVENT_MOVE) && (GetState(pB, BTN_PRESSED)))
                    return (BTN_MSG_CANCELPRESS);
            }
        }

        return (OBJ_MSG_INVALID);
    }

        #endif
        #ifdef USE_KEYBOARD
    if(pMsg->type == TYPE_KEYBOARD)
    {
        if((WORD)pMsg->param1 == pB->hdr.ID)
        {
            if(pMsg->uiEvent == EVENT_KEYSCAN)
            {
                if(GetState(pB, BTN_TOGGLE))
                {
                    if((pMsg->param2 == SCAN_SPACE_RELEASED) || (pMsg->param2 == SCAN_CR_RELEASED))
                    {
                        if(GetState(pB, BTN_PRESSED))
                            return (BTN_MSG_RELEASED);
                        else
                            return (BTN_MSG_PRESSED);
                    }
                }
                else
                {
                    if((pMsg->param2 == SCAN_SPACE_PRESSED) || (pMsg->param2 == SCAN_CR_PRESSED))
                    {
                        return (BTN_MSG_PRESSED);
                    }

                    if((pMsg->param2 == SCAN_SPACE_RELEASED) || (pMsg->param2 == SCAN_CR_RELEASED))
                    {
                        return (BTN_MSG_RELEASED);
                    }
                }

                if((pMsg->param2 == SCAN_SPACE_PRESSED) || (pMsg->param2 == SCAN_CRA_PRESSED))
                {
                    return (BTN_MSG_PRESSED);
                }

                if((pMsg->param2 == SCAN_SPACE_RELEASED) || (pMsg->param2 == SCAN_CRA_RELEASED))
                {
                    return (BTN_MSG_RELEASED);
                }
            }
        }

        return (OBJ_MSG_INVALID);
    }

        #endif
    return (OBJ_MSG_INVALID);
}
/*********************************************************************
* Button draw states
********************************************************************/
typedef enum
{
    REMOVE,
    RNDBUTTON_DRAW,
    #ifdef USE_BUTTON_MULTI_LINE
    CHECK_TEXT_DRAW,
    #else
    TEXT_DRAW,
    #endif
    TEXT_DRAW_RUN,
    FOCUS_DRAW,
} BTN_DRAW_STATES;
/*********************************************************************
* Function: inline WORD __attribute__((always_inline)) DrawButtonFocus(BUTTON *button, SHORT radius, BTN_DRAW_STATES *current_state)
********************************************************************/
inline WORD __attribute__((always_inline)) DrawButtonFocus(BUTTON *button, SHORT radius, BTN_DRAW_STATES *current_state)
{
    
    *current_state = FOCUS_DRAW;

#ifdef USE_NONBLOCKING_CONFIG
    if(IsDeviceBusy()) 
        return (0);
#endif

    if(GetState(button, BTN_FOCUSED))
    {
        SetLineType(FOCUS_LINE);
        if(GetState(button, BTN_PRESSED))
        {
            SetColor(button->hdr.pGolScheme->TextColor1);
        }
        else
        {
            SetColor(button->hdr.pGolScheme->TextColor0);
        }

        // check if the object has rounded corners or not
        if(!button->radius)
        {
            if
            (
                !Rectangle
                    (
                        button->hdr.left + GOL_EMBOSS_SIZE + 2,
                        button->hdr.top + GOL_EMBOSS_SIZE + 2,
                        button->hdr.right - GOL_EMBOSS_SIZE - 2,
                        button->hdr.bottom - GOL_EMBOSS_SIZE - 2
                    )
            )
            {
                return (0);
            }
        }
        else
        {

            // original center is still the same, but radius is reduced
            if
            (
                !Bevel
                    (
                        button->hdr.left + radius,
                        button->hdr.top + radius,
                        button->hdr.right - radius,
                        button->hdr.bottom - radius,
                        radius - 2 - GOL_EMBOSS_SIZE
                    )
            )
            {
                return (0);
            }
        }

        SetLineType(SOLID_LINE);
    }

	#ifdef USE_ALPHABLEND
    if(button->hdr.pGolScheme->AlphaValue != 100) 
    {
		AlphaBlendWindow(_GFXActivePage, button->hdr.left, button->hdr.top,
						 _GFXBackgroundPage, button->hdr.left, button->hdr.top,
						 _GFXActivePage, button->hdr.left, button->hdr.top,
					     button->hdr.right - button->hdr.left, 
					     button->hdr.bottom - button->hdr.top,  	
					     button->hdr.pGolScheme->AlphaValue);			
	}  
	#endif

    *current_state = REMOVE;
    return 1;
}
/*********************************************************************
* Function: inline void __attribute__((always_inline)) SetButtonTextPosition(BUTTON *button, XCHAR *pCurLine, SHORT lineCtr)
********************************************************************/
#ifdef USE_BUTTON_MULTI_LINE
inline void __attribute__((always_inline)) SetButtonTextPosition(BUTTON *button, XCHAR *pCurLine, SHORT lineCtr)
{
    WORD xText, yText;
    SHORT textWidth;

    SetFont(button->hdr.pGolScheme->pFont);
    textWidth = GetTextWidth(pCurLine, button->hdr.pGolScheme->pFont);

    // check text alignment
    if(GetState(button, BTN_TEXTRIGHT))
    {
        xText = button->hdr.right - (textWidth + GOL_EMBOSS_SIZE + 2);
    }
    else if(GetState(button, BTN_TEXTLEFT))
    {
        xText = button->hdr.left + GOL_EMBOSS_SIZE + 2;
    }
    else
    {

        // centered	text in x direction
        xText = (button->hdr.left + button->hdr.right - textWidth) >> 1;
    }

    if(GetState(button, BTN_TEXTTOP))
    {
        yText = button->hdr.top + GOL_EMBOSS_SIZE + (lineCtr * GetTextHeight(button->hdr.pGolScheme->pFont));
    }
    else if(GetState(button, BTN_TEXTBOTTOM))
    {
        yText = button->hdr.bottom - (GOL_EMBOSS_SIZE + button->textHeight) + (lineCtr * GetTextHeight(button->hdr.pGolScheme->pFont));
    }
    else
    {

        // centered	text in y direction
        yText = ((button->hdr.bottom + button->hdr.top - button->textHeight) >> 1) + (lineCtr * GetTextHeight(button->hdr.pGolScheme->pFont));
    }

    MoveTo(xText, yText);

}
#endif
/*********************************************************************
* Function: WORD BtnDraw(void *pObj)
*
*
* Notes: This is the state machine to draw the button.
*
********************************************************************/
WORD BtnDraw(void *pObj)
{

    static BTN_DRAW_STATES state = REMOVE;
    static SHORT width, height, radius;

    #ifdef USE_BUTTON_MULTI_LINE
    static SHORT charCtr = 0, lineCtr = 0;
    static XCHAR *pCurLine = NULL;
    XCHAR ch = 0;
    #else
    WORD xText, yText;
    #endif
    GFX_COLOR embossLtClr, embossDkClr;
    static GFX_COLOR faceClr;
    BUTTON *pB;
    
    pB = (BUTTON *)pObj;

    if(IsDeviceBusy())
        return (0);

    switch(state)
    {
        case REMOVE:
            if(IsDeviceBusy())
                return (0);

#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
            GFX_DRIVER_SetupDrawUpdate( pB->hdr.left,
                                        pB->hdr.top,
                                        pB->hdr.right,
                                        pB->hdr.bottom);
#endif
            if(GetState(pB, BTN_HIDE))
            {                       // Hide the button (remove from screen)

				#ifdef USE_ALPHABLEND
				if(pB->hdr.pGolScheme->AlphaValue != 100)
           		{
					  
					CopyPageWindow(_GFXBackgroundPage, 
							   _GFXActivePage,
                               pB->hdr.left, pB->hdr.top,pB->hdr.left, pB->hdr.top, 
							   pB->hdr.right - pB->hdr.left, 
							   pB->hdr.bottom - pB->hdr.top);
	           	}	           	
	           	else
				#endif

                SetColor(pB->hdr.pGolScheme->CommonBkColor);
                if(!Bar(pB->hdr.left, pB->hdr.top, pB->hdr.right, pB->hdr.bottom))
                {
                    return (0);
                }

#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pB->hdr.left,
                                                pB->hdr.top,
                                                pB->hdr.right,
                                                pB->hdr.bottom);
#endif
                return (1);
            }

            /* Note: that width and height adjustment considers the following assumptions:
					1. if circular width = height = radius*2
					2. if vertical capsule width = radius*2
					3. if horizontal capsule height = radius*2
					4. radius must be less than or equal to width if height is greater than width
					5. radius must be less than or equal to height if width is greater than height
					6. if button is cornered, radius must be zero
			*/
            radius = pB->radius;    // get radius
            width = (pB->hdr.right - pB->hdr.left) - (radius * 2);  // get width
            height = (pB->hdr.bottom - pB->hdr.top) - (radius * 2); // get height

			if(!GetState(pB, BTN_NOPANEL))
			{
                if(!GetState(pB, BTN_DISABLED))
                {
                    if(GetState(pB, BTN_PRESSED))
                    {
                        embossDkClr = pB->hdr.pGolScheme->EmbossLtColor;
                        embossLtClr = pB->hdr.pGolScheme->EmbossDkColor;
                        faceClr = pB->hdr.pGolScheme->Color1;
                    }
                    else
                    {
                        embossLtClr = pB->hdr.pGolScheme->EmbossLtColor;
                        embossDkClr = pB->hdr.pGolScheme->EmbossDkColor;
                        faceClr = pB->hdr.pGolScheme->Color0;
                    }
                }
                else
                {
                    embossLtClr = pB->hdr.pGolScheme->EmbossLtColor;
                    embossDkClr = pB->hdr.pGolScheme->EmbossDkColor;
                    faceClr = pB->hdr.pGolScheme->ColorDisabled;
                }

			    #ifdef USE_ALPHABLEND
			    if(pB->hdr.pGolScheme->AlphaValue != 100) 
                {
                      CopyPageWindow(_GFXBackgroundPage, 
							   _GFXActivePage,
                               pB->hdr.left, pB->hdr.top,pB->hdr.left, pB->hdr.top, 
							   pB->hdr.right - pB->hdr.left, 
							   pB->hdr.bottom - pB->hdr.top);	             									            
			    }
			    #endif

                SetLineThickness(NORMAL_LINE);
                SetLineType(SOLID_LINE);

                #ifdef USE_GRADIENT
                SetGOLPanelGradient(pB->hdr.pGolScheme);                
                #endif

                #ifdef USE_ALPHABLEND_LITE
                SetPrevAlphaColor(pB->previousAlphaColor); 
                SetGOLPanelAlpha(pB->hdr.pGolScheme->AlphaValue);
                #endif

                GOLPanelDraw
                (
                    pB->hdr.left + radius,
                    pB->hdr.top + radius,
                    pB->hdr.right - radius,
                    pB->hdr.bottom - radius,
                    radius,
                    faceClr,
                    embossLtClr,
                    embossDkClr,
                    pB->pBitmap,
                    GOL_EMBOSS_SIZE
                );
	  		}         	
            state = RNDBUTTON_DRAW;

        case RNDBUTTON_DRAW:
			if (GetState(pB, BTN_NOPANEL))
            {
	        	// check if there is an image to be drawn
	        	if (pB->pBitmap != NULL)
	        	{
                	if
                    (
                        !PutImage
                            (
                                ((pB->hdr.right + pB->hdr.left - GetImageWidth((void *)pB->pBitmap)) >> 1) + 1,
                                ((pB->hdr.top + pB->hdr.bottom - GetImageHeight((void *)pB->pBitmap)) >> 1) + 1,
                                pB->pBitmap,IMAGE_NORMAL
                            )
                    )
                    {
                        return (0);
                    }
          		}  
    		}
    		else
    		{      
               	if (GetState(pB, BTN_TWOTONE))
	        	{
	            	if(!GOLTwoTonePanelDrawTsk())
	            	{
	                	return (0);
	            	}
	        	}	
	        	else
	        	{
		            if(!GOLPanelDrawTsk())
		            {
		                return (0);
		            }
                    //while(!GOLPanelDrawTsk());    									
				}

                #ifdef USE_ALPHABLEND_LITE
                pB->previousAlphaColor = faceClr;
                #endif
			}

            #ifdef USE_BUTTON_MULTI_LINE
            state = CHECK_TEXT_DRAW;
            #else
            state = TEXT_DRAW;
            #endif

        #ifdef USE_BUTTON_MULTI_LINE
        case CHECK_TEXT_DRAW:
            if(pB->pText != NULL)
            {
                if(!GetState(pB, BTN_DISABLED))
                {
                    if(GetState(pB, BTN_PRESSED))
                    {
                        SetColor(pB->hdr.pGolScheme->TextColor1);
                    }
                    else
                    {
                        SetColor(pB->hdr.pGolScheme->TextColor0);
                    }
                }
                else
                {
                    SetColor(pB->hdr.pGolScheme->TextColorDisabled);
                }

                pCurLine = pB->pText;
                lineCtr = 0;
                charCtr = 0;
                SetButtonTextPosition(pB, pCurLine, lineCtr);
                state = TEXT_DRAW_RUN;
            }
            else
            {
                if(DrawButtonFocus(pB, radius, &state))
                {
                    #ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                        GFX_DRIVER_CompleteDrawUpdate(   pB->hdr.left,
                                                        pB->hdr.top,
                                                        pB->hdr.right,
                                                        pB->hdr.bottom);
                    #endif
                    return 1;
                }
                return 0;
            }

        case TEXT_DRAW_RUN:
            ch = *(pCurLine + charCtr);

            // output one character at time until a newline character or a NULL character is sampled
            while(0x0000 != ch)
            {
                if(!OutChar(ch))
                    return (0);
                // render the character
                charCtr++;                          // update to next character
                ch = *(pCurLine + charCtr);

                if(ch == 0x000A)
                {                                       // new line character
                    pCurLine = pCurLine + charCtr + 1;  // go to first char of next line
                    lineCtr++;                          // update line counter
                    charCtr = 0;                        // reset char counter
                    SetButtonTextPosition(pB, pCurLine, lineCtr);
                    ch = *(pCurLine + charCtr);
                }
            }

            SetClip(CLIP_DISABLE);              // remove clipping
            state = FOCUS_DRAW;                 // go back to IDLE state

        #else

        case TEXT_DRAW:
            if(pB->pText != NULL)
            {
                if(!GetState(pB, BTN_DISABLED))
                {
                    if(GetState(pB, BTN_PRESSED))
                    {
                        SetColor(pB->hdr.pGolScheme->TextColor1);
                    }
                    else
                    {
                        SetColor(pB->hdr.pGolScheme->TextColor0);
                    }
                }
                else
                {
                    SetColor(pB->hdr.pGolScheme->TextColorDisabled);
                }

                SetFont(pB->hdr.pGolScheme->pFont);

                // check text alignment
                if(GetState(pB, BTN_TEXTRIGHT))
                {
                    xText = pB->hdr.right - (pB->textWidth + GOL_EMBOSS_SIZE + 2);
                }
                else if(GetState(pB, BTN_TEXTLEFT))
                {
                    xText = pB->hdr.left + GOL_EMBOSS_SIZE + 2;
                }
                else
                {

                    // centered	text in x direction
                    xText = (pB->hdr.left + pB->hdr.right - pB->textWidth) >> 1;
                }

                if(GetState(pB, BTN_TEXTTOP))
                {
                    yText = pB->hdr.top + GOL_EMBOSS_SIZE + 2;
                }
                else if(GetState(pB, BTN_TEXTBOTTOM))
                {
                    yText = pB->hdr.bottom - (pB->textHeight + GOL_EMBOSS_SIZE);
                }
                else
                {

                    // centered	text in y direction
                    yText = (pB->hdr.bottom + pB->hdr.top - pB->textHeight) >> 1;
                }

                MoveTo(xText, yText);
                state = TEXT_DRAW_RUN;
            }
            else
            {
                if(DrawButtonFocus(pB, radius, &state))
                {
                    #ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                        GFX_DRIVER_CompleteDrawUpdate(   pB->hdr.left,
                                                        pB->hdr.top,
                                                        pB->hdr.right,
                                                        pB->hdr.bottom);
                    #endif
                    return 1;
                }
                return 0;
            }

        case TEXT_DRAW_RUN:
            if(!OutText(pB->pText))
                return (0);
            state = FOCUS_DRAW;
                #endif // #ifdef USE_BUTTON_MULTI_LINE

        case FOCUS_DRAW:
            if(DrawButtonFocus(pB, radius, &state))
            {
                #ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                    GFX_DRIVER_CompleteDrawUpdate(   pB->hdr.left,
                                                    pB->hdr.top,
                                                    pB->hdr.right,
                                                    pB->hdr.bottom);
                #endif
                return 1;
            }
            return 0;
    }

#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
    GFX_DRIVER_CompleteDrawUpdate(   pB->hdr.left,
                                    pB->hdr.top,
                                    pB->hdr.right,
                                    pB->hdr.bottom);
#endif
    return (1);
}

#endif //#if defined (USE_BUTTON) || defined (USE_BUTTON_MULTI_LINE)
