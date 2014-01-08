/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Slider
 *****************************************************************************
 * FileName:        Slider.c
 * Dependencies:    None 
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07	    Version 1.0 release
 * 04/29/10	    - Added OBJ_MSG_PASSIVE message to detect event on the slider 
 *                but with no action.
 *				- fixed the swapped message translation in the 
 *                SldTranslateMsg() when keyboard event is detected.
 * 02/23/11     Removed references to BLACK and WHITE colors. Replaced
 *              with emboss dark and light colors. These are used
 *              in rendering the thumb path.
 * 04/20/11     Fixed KEYBOARD bug on object ID and GOL_MSG param1 comparison.
 * 07/29/11     Fixed state transition when hiding the slider.
 * 05/10/12     Added Alpha Blending Support
 *****************************************************************************/
#include "Graphics/Graphics.h"

#ifdef USE_SLIDER

/* Internal Functions */
SHORT   SldSetThumbSize(SLIDER *pSld, SHORT high, SHORT low);
void    SldGetMinMaxPos(SLIDER *pSld, WORD *minPos, WORD *maxPos);
WORD    SldGetWidth(SLIDER *pSld);
WORD    SldGetHeight(SLIDER *pSld);

/*********************************************************************
* Function: SLIDER *SldCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*							  SHORT bottom, WORD state, SHORT range, 
*							  SHORT page, SHORT pos, GOL_SCHEME *pScheme)
*
* Notes: Creates a SLIDER object and adds it to the current active list.
*        If the creation is successful, the pointer to the created Object 
*        is returned. If not successful, NULL is returned.
*
********************************************************************/
SLIDER *SldCreate
(
    WORD        ID,
    SHORT       left,
    SHORT       top,
    SHORT       right,
    SHORT       bottom,
    WORD        state,
    WORD        range,
    WORD        page,
    WORD        pos,
    GOL_SCHEME  *pScheme
)
{
    SLIDER  *pSld = NULL;

    pSld = (SLIDER *)GFX_malloc(sizeof(SLIDER));
    if(pSld == NULL)
        return (pSld);

    pSld->hdr.ID        = ID;                           // unique id assigned for referencing
    pSld->hdr.pNxtObj   = NULL;
    pSld->hdr.type      = OBJ_SLIDER;                   // set object type
    pSld->hdr.left      = left;                         // left and right should be equal when oriented vertically
    pSld->hdr.top       = top;                          // top and bottom should be equal when oriented horizontally
    pSld->hdr.right     = right;
    pSld->hdr.bottom    = bottom;
    pSld->hdr.state     = state;
    pSld->hdr.DrawObj      = SldDraw;     				// draw function
    pSld->hdr.MsgObj         = SldTranslateMsg;       	// message function
    pSld->hdr.MsgDefaultObj  = SldMsgDefault;  			// default message function
    pSld->hdr.FreeObj  = NULL;  						// default free function

    // Parameters in the user defined range system (pos, page and range)
    pSld->range = range;                            // range of the slider movement (always measured from 0 to range)

    // 0 refers to pSld->minPos and
    // range refers to pSld->maxpos where: minPos and maxPos are
    // the coordinate equivalent of 0 and range value
    pSld->page = page;                              // set the resolution
    pSld->pos = pos;                                // set the initial position

    // calculate the thumb width and height
    pSld->thWidth = SldGetWidth(pSld);
    pSld->thHeight = SldGetHeight(pSld);

    // Set the color scheme to be used
    if(pScheme == NULL)
        pSld->hdr.pGolScheme = _pDefaultGolScheme;  // use default scheme
    else
        pSld->hdr.pGolScheme = (GOL_SCHEME *)pScheme;   // user defined scheme

    GOLAddObject((OBJ_HEADER *)pSld);                   // add the new object to the current list
    return (pSld);
}

/*********************************************************************
* Function: SHORT SldSetThumbSize(SLIDER *pSld, SHORT high, SHORT low)
*
* Notes: An INTERNAL function used to compute for the width or 
*        height of the thumb. This function is created to save
*        code size. This function is called only to dynamically
*        compute for the thumb size. Used only when slider is 
* 		 type Scrollbar. Parameter are defined as:
*		 pSld - pointer to the object
*        high - higher value to be used
*        low  - lower value to be used
*
********************************************************************/
SHORT SldSetThumbSize(SLIDER *pSld, SHORT high, SHORT low)
{
    WORD    temp;

    temp = (pSld->range / pSld->page);
    temp = (high - low) / temp;

    // when size is less than half of emboss size, set the
    // size to half the emboss size. This is to make sure
    // thumb will always have a size.
    if(temp < (GOL_EMBOSS_SIZE << 1))
        temp = (GOL_EMBOSS_SIZE << 1);

    return (SHORT) temp;
}

/*********************************************************************
* Function: WORD SldGetWidth(SLIDER *pSld)
*
* Notes: An INTERNAL function that computes for the width  
*        of the thumb. This function is created to save
*        code size. This function is called only to dynamically
*        compute for the thumb size. 
*
********************************************************************/
WORD SldGetWidth(SLIDER *pSld)
{
    WORD    temp;

    /*
		Calculating the width is dependent on the mode type.
		If type Scrollbar, width is dependent on the ratio of the
		page/range = width/max-min (see SetThumbSize())
		if type is Slider, width is dependent on height*3/8
		
		When horizontal width is dynamic, height is contant.
	
	*/
    if(GetState(pSld, SLD_VERTICAL))
    {
        temp = pSld->hdr.right - pSld->hdr.left;
        if(GetState(pSld, SLD_SCROLLBAR))
        {
            temp = temp - (GOL_EMBOSS_SIZE << 1);
        }
        else
        {
            temp = temp - (GOL_EMBOSS_SIZE << 1) - 2;
        }
    }
    else
    {
        if(GetState(pSld, SLD_SCROLLBAR))
        {
            temp = SldSetThumbSize(pSld, pSld->hdr.right, pSld->hdr.left);
        }
        else
        {
            temp = (((pSld->hdr.bottom - pSld->hdr.top) - (GOL_EMBOSS_SIZE << 1) - 2) * 3) >> 3;
        }
    }

    // to avoid calculations of dividing by two, we store half the width value
    return (temp >> 1);
}

/*********************************************************************
* Function: WORD SldGetHeight(SLIDER *pSld)
*
* Notes: An INTERNAL function that computes for the height  
*        of the thumb. This function is created to save
*        code size. This function is called only to dynamically
*        compute for the thumb size.
*
********************************************************************/
WORD SldGetHeight(SLIDER *pSld)
{
    WORD    temp;

    /*
		Calculating the height is dependent on the mode type.
		If type Scrollbar, width is dependent on the ratio of the
		page/range = width/max-min (see SetThumbSize())
		if type is Slider, width is dependent on width*3/8
	
		When vertical height is dynamic, width is contant.
	*/
    if(GetState(pSld, SLD_VERTICAL))
    {
        if(GetState(pSld, SLD_SCROLLBAR))
        {
            temp = SldSetThumbSize(pSld, pSld->hdr.bottom, pSld->hdr.top);
        }
        else
        {
            temp = (((pSld->hdr.right - pSld->hdr.left) - (GOL_EMBOSS_SIZE << 1) - 2) * 3) >> 3;
        }
    }
    else
    {
        temp = pSld->hdr.bottom - pSld->hdr.top;
        if(GetState(pSld, SLD_SCROLLBAR))
        {
            temp = temp - (GOL_EMBOSS_SIZE << 1);
        }
        else
        {
            temp = temp - (GOL_EMBOSS_SIZE << 1) - 2;
        }
    }

    // to avoid calculations of dividing by two, we store half the height value
    return (temp >> 1);
}

/*********************************************************************
* Function: void SldGetMinMaxPos(SLIDER *pSld, WORD *min, WORD *max)
*
* Notes:  An INTERNAL function that computes for the minimum
*         and maximum pixel position in the screen. This function is 
*         created to save code size. Used to define the minimum 
*         & maximum position of the thumb when sliding. Parameters
*		  used are defined as:
*		  pSld - pointer to the object
*         min  - pointer to the minimum variable
*         max  - pointer to the maximum variable
*
********************************************************************/
void SldGetMinMaxPos(SLIDER *pSld, WORD *min, WORD *max)
{
    WORD    temp;

    // calculate maximum and minimum position	
    if(GetState(pSld, SLD_VERTICAL))
    {
        temp = pSld->thHeight + GOL_EMBOSS_SIZE;
        *min = pSld->hdr.top + temp;
        *max = pSld->hdr.bottom - temp;
    }
    else
    {
        temp = pSld->thWidth + GOL_EMBOSS_SIZE;
        *min = pSld->hdr.left + temp;
        *max = pSld->hdr.right - temp;
    }

    // for aestetics.
    if(!GetState(pSld, SLD_SCROLLBAR))
    {
        *min = *min + 2;
        *max = *max - 2;
    }
}

/*********************************************************************
* Function: void SldSetRange(SLIDER *pSld, SHORT newRange)
*
* Notes:  Sets the new range value of the slider or scrollbar.
*         Object must be redrawn after this function is called to 
*         reflect the changes to the object.
*
********************************************************************/
void SldSetRange(SLIDER *pSld, SHORT newRange)
{
    WORD        newPos;
    DWORD_VAL   dTemp;

    // this checks for limits of the range (minimum is 2)
    if(newRange <= 2)
        newRange = 2;

    if((WORD) newRange > (WORD) 0x7FFF)
        newRange = 0x7FFF;

    dTemp.Val = newRange * pSld->pos;
    dTemp.Val = dTemp.Val / pSld->range;

    // get new range
    newPos = dTemp.w[0];

    // set the new range
    pSld->range = newRange;

    // now check the page, adjust when necessary
    // page maximum limit is range/2, minimum is 1
    if(pSld->page > ((pSld->range) >> 1))
    {
        if(!((pSld->range) >> 1))
            pSld->page = 1;
        else
            pSld->page = (pSld->range) >> 1;
    }

    // calculate new thumb width and height
    pSld->thWidth = SldGetWidth(pSld);
    pSld->thHeight = SldGetHeight(pSld);
    SldSetPos(pSld, newPos);
}

/*********************************************************************
* Function: void SldSetPage(SLIDER *pSld, WORD newPage) 
*
* Notes: Sets the new page value of the slider or scrollbar.
*        The page maximum limit is range/2, minimum is 1
*
********************************************************************/
void SldSetPage(SLIDER *pSld, WORD newPage)
{
    if(newPage < 1)
        newPage = 1;
    else if(newPage > ((pSld->range) >> 1))
        newPage = (pSld->range) >> 1;
    pSld->page = newPage;

    // calculate new thumb width and height
    pSld->thWidth = SldGetWidth(pSld);
    pSld->thHeight = SldGetHeight(pSld);
}

/*********************************************************************
* Function: SldSetPos(SLIDER *pSld, SHORT newPos)
*
* Notes: Sets the thumb to the new position. Checking is first 
*        preformed if the new position is within the range (0 to range)
*        of the slider. Object must be redrawn after this function is called to 
*        reflect the changes to the object.
*
********************************************************************/
void SldSetPos(SLIDER *pSld, SHORT newPos)
{
    WORD        minPos, maxPos, relPos;
    DWORD_VAL   dTemp;

    // get minimum and maximum positions
    SldGetMinMaxPos(pSld, &minPos, &maxPos);
    dTemp.Val = 0;

        #ifndef SLD_INVERT_VERTICAL

    // check if the new value is still in range
    if(newPos <= 0)
    {
        pSld->pos = 0;                                  // set to zero in range domain
        if(GetState(pSld, SLD_VERTICAL))
        {                                               // min and max in vertical is inverted
            pSld->currPos = maxPos;                     // minimum position is the bottom position in
        }                                               // coordinate domain
        else
            pSld->currPos = minPos;                     // minimum is left most position in coordinate domain
    }
    else if(newPos >= pSld->range)
    {
        pSld->pos = pSld->range;                        // set to maximum value in range domain
        if(GetState(pSld, SLD_VERTICAL))
        {                                               // min and max in vertical is inverted
            pSld->currPos = minPos;                     // maximum position is the top position in
        }                                               // coordinate domain
        else
            pSld->currPos = maxPos;                     // maximum is right most position in coordinate domain
    }
    else
    {
        pSld->pos = newPos;                             // get new position in range domain
        dTemp.w[1] = newPos;
        dTemp.Val = dTemp.Val / pSld->range;
        dTemp.Val = (maxPos - minPos) * dTemp.Val;

        // set current position in coordinate domain
        relPos = dTemp.w[1] + minPos;

        if(GetState(pSld, SLD_VERTICAL))
        {                                               // test if we need to transform min and max position
            pSld->currPos = maxPos - (relPos - minPos); // min and max position is swapped in coordinate domain
        }
        else
            pSld->currPos = relPos;                     // use position
    }

        #else

    // check if the new value is still in range
    if(newPos <= 0)
    {
        pSld->pos = 0;              // set to zero in range domain
        pSld->currPos = minPos;     // set to minimum in coordinate domain
    }
    else if(newPos >= pSld->range)
    {
        pSld->pos = pSld->range;    // set to maximum value in range domain
        pSld->currPos = maxPos;     // set to minimum in coordinate domain
    }
    else
    {
        pSld->pos = newPos;         // get new position in range domain
        dTemp.w[1] = newPos;
        dTemp.Val = dTemp.Val / pSld->range;
        dTemp.Val = (maxPos - minPos) * dTemp.Val;

        // set current position in coordinate domain
        pSld->currPos = dTemp.w[1] + minPos;
    }

        #endif // ifndef SLD_INVERT_VERTICAL
}

/*********************************************************************
* Function: void SldMsgDefault(WORD translatedMsg, void *pObj, 
*							   GOL_MSG* pMsg)
*
* Notes: This the default operation to change the state of the button.
*		 Called inside GOLMsg() when GOLMsgCallback() returns a 1.
*
********************************************************************/
void SldMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
{
    SLIDER *pSld;

    pSld = (SLIDER *)pObj;

        #ifdef USE_TOUCHSCREEN

    WORD        newPos, minPos, maxPos;
    DWORD_VAL   dTemp;

            #ifdef USE_FOCUS
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {
        if(!GetState(pSld, SLD_FOCUSED))
        {
            GOLSetFocus((OBJ_HEADER *)pSld);
        }
    }

            #endif // USE_FOCUS

	// if message was passive do not do anything
	if (translatedMsg == OBJ_MSG_PASSIVE)
		return;
		
    // get the min and max positions
    SldGetMinMaxPos(pSld, &minPos, &maxPos);

    if(pMsg->type == TYPE_TOUCHSCREEN)
    {
        if((translatedMsg == SLD_MSG_DEC) || (translatedMsg == SLD_MSG_INC))
        {

            // newPos in this context is used in the coordinate domain
            if(!GetState(pSld, SLD_VERTICAL))
            {                               // check if Horizontal or Vertical orientation
                if(pMsg->param1 <= minPos)
                {                           // Horizontal orientation: test x position	
                    newPos = minPos;        // beyond minimum, use min position
                }
                else if(pMsg->param1 >= maxPos)
                {
                    newPos = maxPos;        // beyond maximum, use max position
                }
                else
                {
                    newPos = pMsg->param1;  // within range: use x position given
                }
            }
            else
            {
                if(pMsg->param2 <= minPos)
                {                           // Vertical orientation: test y position
                    newPos = minPos;        // beyond minimum, use min position
                }
                else if(pMsg->param2 >= maxPos)
                {
                    newPos = maxPos;        // beyond maximum, use max position
                }
                else
                {
                    newPos = pMsg->param2;  // within range: use y position given
                }
            }

            if(newPos != pSld->currPos)
            {                               // check if we need to redraw thumb
                // yes redraw is needed, translate newPos into range domain
                // first get new position in range domain
                dTemp.Val = (DWORD) (newPos - minPos) * (DWORD) pSld->range;
                dTemp.Val = dTemp.Val / (maxPos - minPos);
                newPos = dTemp.w[0];

                        #ifndef SLD_INVERT_VERTICAL
                if(GetState(pSld, SLD_VERTICAL))
                {                           // check if we need to swap min and max in vertical
                    newPos = pSld->range - newPos;  // min and max is swapped in vertical orientation
                }

                        #endif
                SldSetPos(pSld, newPos);            // set to new position	
                SetState(pSld, SLD_DRAW_THUMB);     // redraw the thumb only
            }
            else
                return;
        }
        else
            return;
    }

        #endif // USE_TOUCHSCREEN
        #ifdef USE_KEYBOARD
    if(pMsg->type == TYPE_KEYBOARD)
    {                           // for keyboard
        if(translatedMsg == SLD_MSG_INC)
        {
            SldIncPos(pSld);    // increment is requested
        }
        else
        {
            SldDecPos(pSld);    // decrement is requested
        }

        SetState(pSld, SLD_DRAW_THUMB); // redraw the thumb only
    }

        #endif // USE_KEYBOARD
}

/*********************************************************************
* Function: WORD SldTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Notes: Evaluates the message if the object will be affected by the 
*		 message or not.
*
********************************************************************/
WORD SldTranslateMsg(void *pObj, GOL_MSG *pMsg)
{

    SLIDER *pSld;

    pSld = (SLIDER *)pObj;

    // Evaluate if the message is for the slider
    // Check if disabled first
    if(GetState(pSld, SLD_DISABLED))
        return (OBJ_MSG_INVALID);

        #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {

        // Check if it falls to the left or right of the center of the thumb's face
        if((pMsg->uiEvent == EVENT_PRESS) || (pMsg->uiEvent == EVENT_MOVE))
        {
            if
            (
                (pSld->hdr.left < pMsg->param1) &&
                (pSld->hdr.right > pMsg->param1) &&
                (pSld->hdr.top < pMsg->param2) &&
                (pSld->hdr.bottom > pMsg->param2)
            )
            {
                if(GetState(pSld, SLD_VERTICAL)) 
				{
                    if(pSld->currPos < pMsg->param2)
                        return (SLD_MSG_INC);
                    else
                        return (SLD_MSG_DEC);
				}
                else 
				{
                	if(pSld->currPos < pMsg->param1)
                    	return (SLD_MSG_INC);
                	else
                    	return (SLD_MSG_DEC);
				}
            }
        }   // end of if((pMsg->uiEvent == EVENT_PRESS) || (pMsg->uiEvent == EVENT_MOVE))
        
        // when the event is release emit OBJ_MSG_PASSIVE this can be used to
        // detect that the release event happened on the slider.
        if(pMsg->uiEvent == EVENT_RELEASE)
        	return OBJ_MSG_PASSIVE;
        	
        return (OBJ_MSG_INVALID);
    }       // end of if(pMsg->type == TYPE_TOUCHSCREEN
        #endif
        #ifdef USE_KEYBOARD
    if(pMsg->type == TYPE_KEYBOARD)
    {
        if((WORD)pMsg->param1 == pSld->hdr.ID)
        {
            if(pMsg->uiEvent == EVENT_KEYSCAN)
            {
                if((pMsg->param2 == SCAN_RIGHT_PRESSED) || (pMsg->param2 == SCAN_UP_PRESSED))
                {
                    return (SLD_MSG_INC);
                }

                if((pMsg->param2 == SCAN_LEFT_PRESSED) || (pMsg->param2 == SCAN_DOWN_PRESSED))
                {
                    return (SLD_MSG_DEC);
                }    
            }
        }
    }

        #endif
    return (OBJ_MSG_INVALID);
}

/*********************************************************************
* Function: WORD SldDraw(void *pObj)
*
* Notes: This is the state machine to draw the slider or scrollbar.
*
********************************************************************/
WORD SldDraw(void *pObj)
{
    typedef enum
    {
        SLD_STATE_IDLE,
        SLD_STATE_HIDE,
        SLD_STATE_PANEL,
        SLD_STATE_THUMBPATH1,
        SLD_STATE_THUMBPATH2,
        SLD_STATE_CLEARTHUMB,
        SLD_STATE_REDRAWPATH1,
        SLD_STATE_REDRAWPATH2,
        SLD_STATE_THUMB,
        SLD_STATE_THUMBPANEL,
        SLD_STATE_FOCUS
    } SLD_DRAW_STATES;

    static GFX_COLOR colorTemp = 0;

    static SLD_DRAW_STATES state = SLD_STATE_IDLE;
    static WORD left, top, right, bottom;
    static WORD midPoint, thWidth, thHeight;
    static WORD minPos, maxPos;
    SLIDER *pSld;

    pSld = (SLIDER *)pObj;

	#ifdef USE_ALPHABLEND
	if(pSld->hdr.pGolScheme->AlphaValue != 100) 						     
			CopyPageWindow(_GFXActivePage, 
					   _GFXBackgroundPage, 
                       pSld->hdr.left, pSld->hdr.top,pSld->hdr.left, pSld->hdr.top,
					   pSld->hdr.right - pSld->hdr.left, 
					   pSld->hdr.bottom - pSld->hdr.top);
	#endif

	while(1)
	{
		if(IsDeviceBusy())
			return (0);

		switch(state)
		{
			case SLD_STATE_IDLE:
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
            GFX_DRIVER_SetupDrawUpdate( pSld->hdr.left,
                                        pSld->hdr.top,
                                        pSld->hdr.right,
                                        pSld->hdr.bottom);
#endif
				if(GetState(pSld, SLD_HIDE))
				{
					SetColor(pSld->hdr.pGolScheme->CommonBkColor);      // set to common BK Color
					state = SLD_STATE_HIDE;
                    // no break here so it falls through to SLD_STATE_HIDE
				}
                else 
                {    
    				if(!GetState(pSld, SLD_DISABLED))
    				{
    					colorTemp = pSld->hdr.pGolScheme->Color0;           // select enabled color
    				}
    				else
    				{
    					colorTemp = pSld->hdr.pGolScheme->ColorDisabled;    // select disabled color
    				}
    
    				SldGetMinMaxPos(pSld, &minPos, &maxPos);
    
    				midPoint = GetState(pSld, SLD_VERTICAL) ? (pSld->hdr.left + pSld->hdr.right) >> 1 : (pSld->hdr.top + pSld->hdr.bottom) >> 1;
    
    				// calculate the thumb width and height Actually gets the half value
    				// (see calculation of width and height) SldGetWidth() and SldGetHeight()
    				thWidth = pSld->thWidth;                                // gets half the width
    				thHeight = pSld->thHeight;                              // gets half the height
    				SetLineThickness(NORMAL_LINE);
    				SetLineType(SOLID_LINE);
    				if(GetState(pSld, SLD_DRAW))
    				{   // draw the panel for the slider	
    					// modify the color setting if scroll bar mode or slider mode
    					GOLPanelDraw
    					(
    						pSld->hdr.left,
    						pSld->hdr.top,
    						pSld->hdr.right,
    						pSld->hdr.bottom,
    						0,
    						colorTemp,
    						(GetState(pSld, SLD_SCROLLBAR)) ? pSld->hdr.pGolScheme->EmbossDkColor : pSld->hdr.pGolScheme->EmbossLtColor,
    						(GetState(pSld, SLD_SCROLLBAR)) ? pSld->hdr.pGolScheme->EmbossLtColor : pSld->hdr.pGolScheme->EmbossDkColor,
    						NULL,
    						GOL_EMBOSS_SIZE
    					);
    
    					// initialize current and previous position
    					SldSetPos(pSld, pSld->pos);
    					pSld->prevPos = pSld->currPos;
    
    					state = SLD_STATE_PANEL;
    					break;
    				}
    				else 
    				{   // we do not need to draw the whole object
    					state = SLD_STATE_CLEARTHUMB;   // go to thumb drawing
    					break;
    				}
                }

			case SLD_STATE_HIDE:
				if (!Bar(pSld->hdr.left, pSld->hdr.top, pSld->hdr.right, pSld->hdr.bottom))
					return (0);
				state = SLD_STATE_IDLE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pSld->hdr.left,
                                                pSld->hdr.top,
                                                pSld->hdr.right,
                                                pSld->hdr.bottom);
#endif
#ifdef USE_ALPHABLEND
                if(pSld->hdr.pGolScheme->AlphaValue != 100) 
                {
		           AlphaBlendWindow(_GFXActivePage, pSld->hdr.left, pSld->hdr.top,
						            _GFXBackgroundPage, pSld->hdr.left, pSld->hdr.top,
						            _GFXActivePage, pSld->hdr.left, pSld->hdr.top,
					                pSld->hdr.right - pSld->hdr.left, 
					                pSld->hdr.bottom - pSld->hdr.top,  	
					                pSld->hdr.pGolScheme->AlphaValue);
                }
 #endif
				return (1);
				
			case SLD_STATE_PANEL:
				if(!GOLPanelDrawTsk())          // draw the panel of the slider
					return (0);
				if(GetState(pSld, SLD_SCROLLBAR))
				{                               // check if slider or scroll bar
					state = SLD_STATE_THUMB;    // scrollbar: go directly to thumb drawing
					break;                      // thumb path is not drawn in scrollbar
				}
				else
				{
					state = SLD_STATE_THUMBPATH1;   // slider: draw thumb path next
				}

			case SLD_STATE_THUMBPATH1:
				SetColor(pSld->hdr.pGolScheme->EmbossDkColor);
				if(!GetState(pSld, SLD_VERTICAL))
				{
					if(!Line(minPos, midPoint, maxPos, midPoint))
						return (0);
				}
				else
				{
					if(!Line(midPoint, minPos, midPoint, maxPos))
						return (0);
				}

				state = SLD_STATE_THUMBPATH2;

			case SLD_STATE_THUMBPATH2:
				SetColor(pSld->hdr.pGolScheme->EmbossLtColor);
				if(!GetState(pSld, SLD_VERTICAL))
				{
					if(!Line(minPos, midPoint + 1, maxPos, midPoint + 1))
						return (0);
				}
				else
				{
					if(!Line(midPoint + 1, minPos, midPoint + 1, maxPos))
						return (0);
				}

				if(GetState(pSld, SLD_DRAW))
				{                               // if drawing the whole slider
					state = SLD_STATE_THUMB;    // go straight to drawing the thumb
					break; 
				}
				else
					// if just drawing the thumb
					state = SLD_STATE_CLEARTHUMB;   // go to state to remove current position

			case SLD_STATE_CLEARTHUMB:              // this removes the current thumb
				if(IsDeviceBusy()) 
				    return (0);

				if(!GetState(pSld, SLD_DRAW_THUMB))
				{                               // SLD_DRAW_THUMB is only set when
					state = SLD_STATE_FOCUS;    // object type is SLIDER
					break; 
				}

				SetColor(colorTemp);

				// Remove the current thumb by drawing a bar with background color
				if(!GetState(pSld, SLD_VERTICAL))
				{
					if(!Bar(pSld->prevPos - thWidth, midPoint - thHeight, pSld->prevPos + thWidth, midPoint + thHeight))
						return (0);
				}
				else
				{
					if(!Bar(midPoint - thWidth, pSld->prevPos - thHeight, midPoint + thWidth, pSld->prevPos + thHeight))
						return (0);
				}

				if(!GetState(pSld, SLD_SCROLLBAR))
				{                               // check if slider or scroll bar
					state = SLD_STATE_REDRAWPATH1;
				}
				else
				{
					state = SLD_STATE_THUMB;    // go directly to thumb drawing
					break;                      // thumb path is not drawn in scrollbar
				}

			case SLD_STATE_REDRAWPATH1:         // redraws the lines that it covered
				SetColor(pSld->hdr.pGolScheme->EmbossDkColor);

				// Check if the redraw area exceeds the actual dimension. This will
				// adjust the redrawing area to just within the parameters
				if(!GetState(pSld, SLD_VERTICAL))
				{
					if(minPos + thWidth > pSld->prevPos)
						left = minPos;
					else
						left = pSld->prevPos - thWidth;

					if(maxPos - thWidth < pSld->prevPos)
						right = maxPos;
					else
						right = pSld->prevPos + thWidth;

					if(!Line(left, midPoint, right, midPoint))
						return (0);
				}
				else
				{
					if(minPos + thHeight > pSld->prevPos)
						top = minPos;
					else
						top = pSld->prevPos - thHeight;
					
					if(maxPos - thHeight < pSld->prevPos)
						bottom = maxPos;
					else
						bottom = pSld->prevPos + thHeight;
					
					if(!Line(midPoint, top, midPoint, bottom))
						return (0);
				}

				state = SLD_STATE_REDRAWPATH2;

			case SLD_STATE_REDRAWPATH2:
				SetColor(pSld->hdr.pGolScheme->EmbossLtColor);
				if(!GetState(pSld, SLD_VERTICAL))
				{
					if(!Line(left, midPoint + 1, right, midPoint + 1))
						return (0);
				}
				else
				{
					if(!Line(midPoint + 1, top, midPoint + 1, bottom))
						return (0);
				}

				state = SLD_STATE_THUMB;

			case SLD_STATE_THUMB:
				if(IsDeviceBusy()) 
				    return (0);
				if(!GetState(pSld, SLD_VERTICAL))
				{                               // Draw the slider thumb based on the
					// current position
					left = pSld->currPos - thWidth;
					top = midPoint - thHeight;
					right = pSld->currPos + thWidth;
					bottom = midPoint + thHeight;
				}
				else
				{
					left = midPoint - thWidth;
					top = pSld->currPos - thHeight;
					right = midPoint + thWidth;
					bottom = pSld->currPos + thHeight;
				}

				GOLPanelDraw
				(
					left,
					top,
					right,
					bottom,
					0,                          // set the parameters of the thumb	
					colorTemp,
					pSld->hdr.pGolScheme->EmbossLtColor,
					pSld->hdr.pGolScheme->EmbossDkColor,
					NULL,
					(GOL_EMBOSS_SIZE - 1) ? GOL_EMBOSS_SIZE - 1 : 1
				);

				state = SLD_STATE_THUMBPANEL;

			case SLD_STATE_THUMBPANEL:
				if(!GOLPanelDrawTsk())          // draw the panel of the thumb
					return (0);

				pSld->prevPos = pSld->currPos;  // record the current position as previous
				if(GetState(pSld, SLD_SCROLLBAR))
				{                               // check if scroll bar focus is not used
					state = SLD_STATE_IDLE;     // go back to idle state
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pSld->hdr.left,
                                                pSld->hdr.top,
                                                pSld->hdr.right,
                                                pSld->hdr.bottom);
#endif
#ifdef USE_ALPHABLEND
                if(pSld->hdr.pGolScheme->AlphaValue != 100) 
                {
		           AlphaBlendWindow(_GFXActivePage, pSld->hdr.left, pSld->hdr.top,
						    _GFXBackgroundPage, pSld->hdr.left, pSld->hdr.top,
						    _GFXActivePage, pSld->hdr.left, pSld->hdr.top,
					          pSld->hdr.right - pSld->hdr.left, 
					          pSld->hdr.bottom - pSld->hdr.top,  	
					          pSld->hdr.pGolScheme->AlphaValue);

                }
 #endif
					return (1);
				}

				if(!GetState(pSld, SLD_DRAW_FOCUS))
				{
					state = SLD_STATE_IDLE;
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
                GFX_DRIVER_CompleteDrawUpdate(   pSld->hdr.left,
                                                pSld->hdr.top,
                                                pSld->hdr.right,
                                                pSld->hdr.bottom);
#endif
#ifdef USE_ALPHABLEND
                if(pSld->hdr.pGolScheme->AlphaValue != 100) 
                {
		           AlphaBlendWindow(_GFXActivePage, pSld->hdr.left, pSld->hdr.top,
						    _GFXBackgroundPage, pSld->hdr.left, pSld->hdr.top,
						    _GFXActivePage, pSld->hdr.left, pSld->hdr.top,
					          pSld->hdr.right - pSld->hdr.left, 
					          pSld->hdr.bottom - pSld->hdr.top,  	
					          pSld->hdr.pGolScheme->AlphaValue);

                }
 #endif
					return (1);
				}

				state = SLD_STATE_FOCUS;

			case SLD_STATE_FOCUS:
				if(!GetState(pSld, SLD_SCROLLBAR))
				{                               // do not draw focus when in scroll bar mode
					SetLineType(FOCUS_LINE);
					if(GetState(pSld, SLD_FOCUSED))
					{
						SetColor(pSld->hdr.pGolScheme->TextColor0); // draw the focus box
					}
					else
					{
						SetColor(colorTemp);                        // remove the focus box, colorTemp
					}

					if
					(
						!Rectangle
							(
								pSld->hdr.left + GOL_EMBOSS_SIZE,
								pSld->hdr.top + GOL_EMBOSS_SIZE,
								pSld->hdr.right - GOL_EMBOSS_SIZE,
								pSld->hdr.bottom - GOL_EMBOSS_SIZE
							)
					) return (0);

					SetLineType(SOLID_LINE);                        // reset line type
				}

				state = SLD_STATE_IDLE;                             // set state to idle
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH
            GFX_DRIVER_CompleteDrawUpdate(   pSld->hdr.left,
                                            pSld->hdr.top,
                                            pSld->hdr.right,
                                            pSld->hdr.bottom);
#endif
#ifdef USE_ALPHABLEND
                if(pSld->hdr.pGolScheme->AlphaValue != 100) 
                {
		           AlphaBlendWindow(_GFXActivePage, pSld->hdr.left, pSld->hdr.top,
						    _GFXBackgroundPage, pSld->hdr.left, pSld->hdr.top,
						    _GFXActivePage, pSld->hdr.left, pSld->hdr.top,
					          pSld->hdr.right - pSld->hdr.left, 
					          pSld->hdr.bottom - pSld->hdr.top,  	
					          pSld->hdr.pGolScheme->AlphaValue);

                }
 #endif
				return (1); // return as done
		}
    } // end of while(1)
}

#endif // USE_SLIDER
