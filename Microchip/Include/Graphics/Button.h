/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Button
 *****************************************************************************
 * FileName:        Button.h
 * Dependencies:    None 
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
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
 * 11/12/07	Version 1.0 release
 *****************************************************************************/
#ifndef _BUTTON_H
    #define _BUTTON_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define BTN_FOCUSED     0x0001  // Bit for focus state.
    #define BTN_DISABLED    0x0002  // Bit for disabled state.
    #define BTN_PRESSED     0x0004  // Bit for press state.
    #define BTN_TOGGLE      0x0008  // Bit to indicate button will have a toggle behavior.
    #define BTN_TEXTRIGHT   0x0010  // Bit to indicate text is right aligned.
    #define BTN_TEXTLEFT    0x0020  // Bit to indicate text is left aligned.
    #define BTN_TEXTBOTTOM  0x0040  // Bit to indicate text is top aligned.
    #define BTN_TEXTTOP     0x0080  // Bit to indicate text is bottom aligned.
    #define BTN_TWOTONE     0x0100  // Bit to indicate the button is a two tone type.
    #define BTN_NOPANEL     0x0200  // Bit to indicate the button will be drawn without a panel (for faster drawing when the button image used is larger than the button panel).

// Note that if bits[7:4] are all zero text is centered.
    #define BTN_DRAW_FOCUS  0x2000  // Bit to indicate focus must be redrawn.
    #define BTN_DRAW        0x4000  // Bit to indicate button must be redrawn.
    #define BTN_HIDE        0x8000  // Bit to indicate button must be removed from screen.
    #define BTN_REMOVE      0x8000

/*********************************************************************
* Overview: Defines the parameters required for a button Object.
* 			The following relationships of the parameters determines
*			the general shape of the button:
* 			1. Width is determined by right - left.
*			2. Height is determined by top - bottom.
*			3. Radius - specifies if the button will have a rounded 
*						edge. If zero then the button will have 
*						sharp (cornered) edge.
*			4. If 2*radius = height = width, the button is a circular button.
*
*********************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;        // Generic header for all Objects (see OBJ_HEADER).
    SHORT       radius;     // Radius for rounded buttons.
    SHORT       textWidth;  // Computed text width, done at creation.
    SHORT       textHeight; // Computed text height, done at creation.
    XCHAR       *pText;     // Pointer to the text used.
    void        *pBitmap;   // Pointer to bitmap used.
    #ifdef USE_ALPHABLEND_LITE
    GFX_COLOR    previousAlphaColor;
    #endif
} BUTTON;

/*********************************************************************
* Macros:  BtnSetBitmap(pB, pBitmap)
*
* Overview: This macro sets the bitmap used in the object. 
*			The size of the bitmap must match the face of the button.
*
* PreCondition: none
*
* Input: pB - Pointer to the object.
*        pBitmap - Pointer to the bitmap to be used.
*
* Output: none
*
* Example:
*   <CODE> 
*	extern BITMAP_FLASH myIcon;
*	BUTTON *pButton;
*
*		BtnSetBitmap(pButton , &myIcon);
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
    #define BtnSetBitmap(pB, pBtmap)    ((BUTTON *)pB)->pBitmap = pBtmap

/*********************************************************************
* Macros:  BtnGetBitmap(pB)
*
* Overview: This macro returns the location of the currently 
*			used bitmap for the object.
*
* PreCondition: none
*
* Input: pB - Pointer to the object.
*
* Output: Returns the pointer to the current bitmap used.
*
* Example:
*   <CODE> 
*	BUTTON *pButton;
*	BITMAP_FLASH *pUsedBitmap;
*
*		pUsedbitmap = BtnGetBitmap(pButton);
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
    #define BtnGetBitmap(pB)    ((BUTTON *)pB)->pBitmap

/*********************************************************************
* Macros:  BtnGetText(pB)
*
* Overview: This macro returns the address of the current 
*			text string used for the object.
*
* PreCondition: none
*
* Input: pB - Pointer to the object.
*
* Output: Returns pointer to the text string being used.
*
* Example:
*   <CODE> 
*	XCHAR *pChar;
*	BUTTON Button[2];
*	
*	pChar = BtnGetText(Button[0]);
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
    #define BtnGetText(pB)  ((BUTTON *)pB)->pText

/*********************************************************************
* Function: BtnSetText(BUTTON *pB, XCHAR *pText)
*
* Overview: This function sets the string used for the object.
*
* PreCondition: none
*
* Input: pB - The pointer to the object whose text will be modified.
*        pText - Pointer to the text that will be used.
*
* Output: none
*
* Example:
*   <CODE> 
*	XCHAR Label0[] = “ON”;
*	XCHAR Label1[] = “OFF”;
*	BUTTON Button[2];
*		
*		BtnSetText(Button[0], Label0);
*		BtnSetText(Button[1], Label1);
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
void BtnSetText(BUTTON * pB, XCHAR * pText);

/*********************************************************************
* Function: BUTTON *BtnCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*                             SHORT bottom, SHORT radius, void *pBitmap, XCHAR *pText, 
*                             GOL_SCHEME *pScheme)
*
* Overview: This function creates a BUTTON object with the parameters given. 
*			It automatically attaches the new object into a global linked list of 
*			objects and returns the address of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the object.
* 		 top - Top most position of the object. 
*		 right - Right most position of the object.
*		 bottom - Bottom most position of the object.
*		 radius - Radius of the rounded edge.
*        state - Sets the initial state of the object.
*        pBitmap - Pointer to the bitmap used on the face of the button
*                  dimension of the bitmap must match the dimension of the 
*				   button.
*        pText - Pointer to the text of the button.
*        pScheme - Pointer to the style scheme used.
*
* Output: Returns the pointer to the object created.
*
* Example:
*   <CODE> 
*	GOL_SCHEME *pScheme;
*	BUTTON *buttons[3];
*	WORD state;
*	
*		pScheme = GOLCreateScheme();
*		state = BTN_DRAW;
*
*		buttons[0] = BtnCreate(1,20,64,50,118,0, state, NULL, "ON", pScheme);
*		// check if button 0 is created
*		if (buttons[0] == NULL)		
*		   return 0;
*
*		buttons[1] = BtnCreate(2,52,64,82,118,0, state, NULL, "OFF", pScheme);
*		// check if button 1 is created
*		if (buttons[1] == NULL)
*		   return 0;
*
*		buttons[2] = BtnCreate(3,84,64,114,118,0, state, NULL, "HI", pScheme);
*		// check if button 2 is created
*		if (buttons[2] == NULL)
*		   return 0;
*
*		return 1;
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
BUTTON  *BtnCreate
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
        );

/*********************************************************************
* Function: BtnTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. The table below enumerates the translated 
*			messages for each event of the touch screen and keyboard inputs.
*
*	<TABLE>
*    	Translated Message     Input Source	    Set/Clear State Bit	        Description
*     	##################     ############	    #######################     ##################################################################################################################################################################
*     	BTN_MSG_PRESSED        Touch Screen     EVENT_PRESS, EVENT_MOVE    	If events occurs and the x,y position falls in the face of the button while the button is not pressed.
*     					       Keyboard         EVENT_KEYSCAN  				If event occurs and parameter1 passed matches the object’s ID and parameter 2 passed matches SCAN_CR_PRESSED or SCAN_SPACE_PRESSED while the button is not pressed.
*		BTN_MSG_STILLPRESSED   Touch Screen     EVENT_STILLPRESS			If event occurs and the x,y position does not change from the previous press position in the face of the button.  				
*		BTN_MSG_RELEASED	   Touch Screen     EVENT_RELEASE  				If the event occurs and the x,y position falls in the face of the button while the button is pressed.     					      			   
*							   Keyboard         EVENT_KEYSCAN  				If event occurs and parameter1 passed matches the object’s ID and parameter 2 passed matches SCAN_CR_RELEASED or SCAN_SPACE_RELEASED while the button is pressed.     					      			   
*		BTN_MSG_CANCELPRESS	   Touch Screen     EVENT_MOVE  				If the event occurs outside the face of the button and the button is currently pressed.     					      			   
*		OBJ_MSG_INVALID		   Any		        Any			  				If the message did not affect the object.							 
*	</TABLE>
*
* PreCondition: none
*
* Input: pObj  - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		  - BTN_MSG_PRESSED – Button is pressed
*         - BTN_MSG_RELEASED – Button is released
*         - BTN_MSG_CANCELPRESS – Button will be released, user cancels press action on the button
*         - OBJ_MSG_INVALID – Button is not affected
*
* Example:
*   <CODE> 
*	void MyGOLMsg(GOL_MSG *pMsg){
*
*		OBJ_HEADER *pCurrentObj;
*		WORD objMsg;
*			
*		if(pMsg->uiEvent == EVENT_INVALID)
*			return;
*		pCurrentObj = GOLGetList();
*		
*		while(pCurrentObj != NULL){
*			// If the object must be redrawn
*			// It cannot accept message
*			if(!IsObjUpdated(pCurrentObj)){
*               translatedMsg = pCurrentObj->MsgObj(pCurrentObj, pMsg);
*
*                if(translatedMsg != OBJ_MSG_INVALID)
*                {
*                    if(GOLMsgCallback(translatedMsg, pCurrentObj, pMsg))
*                        if(pCurrentObj->MsgDefaultObj)
*                            pCurrentObj->MsgDefaultObj(translatedMsg, pCurrentObj, pMsg);
*                }
*
*			}
*		}
*		pCurrentObj = pCurrentObj->pNxtObj;
*	}
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
WORD    BtnTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: BtnMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
* Overview: This function performs the actual state change 
*			based on the translated message given. The following state changes 
*			are supported:
*	<TABLE>
*    	Translated Message   Input Source	Set/Clear State Bit		Description
*     	##################   ############	######     				###########
*     	BTN_MSG_PRESSED      Touch Screen,	Set BTN_PRESSED  		Button will be redrawn in the pressed state.
*     					      Keyboard	     				
*		BTN_MSG_RELEASED	 Touch Screen,	Clear BTN_PRESSED		Button will be redrawn in the unpressed state.
*							  Keyboard        				
*		BTN_MSG_CANCELPRESS	 Touch Screen,	Clear BTN_PRESSED		Button will be redrawn in the unpressed state.
*							         				
*	</TABLE>
*
* PreCondition: none
*
* Input: translatedMsg - The translated message.
*        pObj          - The pointer to the object whose state will be modified.
*        pMsg          - The pointer to the GOL message.
*        
* Output: none
*
* Example:
*	See BtnTranslateMsg() example.
*
* Side Effects: none
*
********************************************************************/
void    BtnMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD BtnDraw(void *pObj)
*
* Overview: This function renders the object on the screen using 
* 			the current parameter settings. Location of the object is 
*			determined by the left, top, right and bottom parameters. 
*			The colors used are dependent on the state of the object. 
*			The font used is determined by the style scheme set.
*			
*			The text on the face of the button is drawn on top of 
*			the bitmap. Text is always rendered centered on the face 
*			of the button.
*			
*			When rendering objects of the same type, each object 
*			must be rendered completely before the rendering of the 
*			next object is started. This is to avoid incomplete 
*			object rendering.
*
* PreCondition: Object must be created before this function is called.
*
* Input: pB - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Example:
*   <CODE> 
*	void MyGOLDraw(){
*		static OBJ_HEADER *pCurrentObj = NULL;
*		int done;
*		
*		// There are no objects
*		if(GOLGetList() == NULL)
*			return;
*
* 		// If it's last object jump to head
*		if(pCurrentObj == NULL)
*			pCurrentObj = GOLGetList();
*
*		done = 0;
*
*		// this only process Button and Window
*		while(pCurrentObj != NULL){
*			// check if object state indicates redrawing
*			done = pCurrentObj->draw(pCurrentObj);
*
*				if(done){
*					// reset only the state if drawing was finished
*					pCurrentObj->state = 0;
*				}else{
*					// done processing the list
*					return;
*				}
*			}
*			// go to next object
*			pCurrentObj = pCurrentObj->pNxtObj;
*		}
*	}
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
WORD BtnDraw(void *pObj);
#endif // _BUTTON_H
