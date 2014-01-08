/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  TextEntry
 *****************************************************************************
 * FileName:        TextEntry.h
 * Dependencies:    None 
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
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
 * 10/24/08		...
 * 03/05/12		Enabled gradient feature and rounded buttons.
 *****************************************************************************/
#ifndef _TEXTENTRY_H
    #define _TEXTENTRY_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define TE_KEY_PRESSED  0x0004  // Bit for press state of one of the keys.
    #define TE_DISABLED     0x0002  // Bit for disabled state.
    #define TE_ECHO_HIDE    0x0008  // Bit to hide the entered characters and instead echo "*" characters to the display.
    #define TE_DRAW         0x4000  // Bit to indicate object must be redrawn.
    #define TE_HIDE         0x8000  // Bit to indicate object must be removed from screen.
    #define TE_UPDATE_KEY   0x2000  // Bit to indicate redraw of a key is needed.
    #define TE_UPDATE_TEXT  0x1000  // Bit to indicate redraw of the text displayed is needed.

/*********************************************************************
* Optional COMMANDS assigned to keys 
*********************************************************************/
    #define TE_DELETE_COM   0x01    // This macro is used to assign a "delete" command on a key.
    #define TE_SPACE_COM    0x02    // This macro is used to assign an insert "space" command on a key.
    #define TE_ENTER_COM    0x03    // This macro is used to assign an "enter" (carriage return) command on a key.

// User can use this command to customize application code in the message
// callback function. Use the returned translated TE_MSG_ENTER to detect the key
// pressed was assigned the enter command. Refer to TeTranslateMsg() for details.

/*********************************************************************
* Optional settigns
*********************************************************************/
#ifndef TE_ROUNDEDBUTTON_RADIUS    
    #define TE_ROUNDEDBUTTON_RADIUS 0    // You can set this parameter in the GraphicsConfig.h
#endif


/*********************************************************************
* Overview: Defines the parameters and the strings assigned for each key. 
*********************************************************************/
typedef struct
{
    SHORT   left;       // Left position of the key
    SHORT   top;        // Top position of the key
    SHORT   right;      // Right position of the key
    SHORT   bottom;     // Bottom position of the key
    SHORT   index;      // Index of the key in the list	
    WORD    state;      // State of the key. Either Pressed (TE_KEY_PRESSED) or Released (0)
    BOOL    update;     // flag to indicate key is to be redrawn with the current state
    WORD    command;    // Command of the key. Either TE_DELETE_COM, TE_SPACE_COM or TE_ENTER_COM.
    XCHAR   *pKeyName;  // Pointer to the custom text assigned to the key. This is displayed over the face of the key.
    SHORT   textWidth;  // Computed text width, done at creation. Used to predict size and position of text on the key face.
    SHORT   textHeight; // Computed text height, done at creation. Used to predict size and position of text on the key face.
    void    *pNextKey;  // Pointer to the next key parameters.
} KEYMEMBER;

/*********************************************************************
* Overview: Defines the parameters required for a TextEntry Object.
*********************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;            // Generic header for all objects (see OBJ_HEADER).
    SHORT       horizontalKeys; // Number of horizontal keys
    SHORT       verticalKeys;   // Number of vertical keys
    XCHAR       *pTeOutput;     // Pointer to the buffer assigned by the user which holds the text shown in the editbox.

    // User creates and manages the buffer. Buffer can also be managed using the APIs provided
    // to add a character, delete the last character or clear the buffer.
    WORD        CurrentLength;  // Current length of the string in the buffer. The maximum value of this is equal to outputLenMax.

    // TextEntry object will update this parameter when adding, removing characters or clearing the buffer
    // and switching buffers.
    WORD        outputLenMax;   // Maximum expected length of output buffer pTeOutput
    KEYMEMBER   *pActiveKey;    // Pointer to the active key KEYMEMBER. This is only used by the Widget. User must not change

    // the value of this parameter directly.
    KEYMEMBER   *pHeadOfList;   // Pointer to head of the list
    void        *pDisplayFont;  // Pointer to the font used in displaying the text.
} TEXTENTRY;

/*********************************************************************
* Function: TEXTENTRY *TeCreate(WORD ID, SHORT left, SHORT top, 
*                   SHORT right, SHORT bottom, WORD state,
*					SHORT horizontalKeys, SHORT verticalKeys, XCHAR *pText[], 
*					void *pBuffer, WORD bufferLength,void *pDisplayFont, 
*					GOL_SCHEME *pScheme)
*
* Overview: This function creates a TEXTENTRY object with the parameters given. 
*	       It automatically attaches the new object into a global linked list of 
*	       objects and returns the address of the object. 
*
*	       	
* PreCondition: If the object will use customized keys, the structure CUSTOMEKEYS must be
*	            populated before calling this function.
*
* Input:  ID -		Unique user defined ID for the object instance 
*		  left-     Left most position of the object.
*		  top -		Top most position of the object.
*		  right -	Right most position of the object.
*		  bottom - 	Bottom most position of the object.
*		  state			 - state of the widget.
*		  horizontalKeys - Number of horizontal keys 
*		  verticalKeys	 - Number of vertical keys
*		  pText			 - array of pointer to the custom "text" assigned by the user.
*         pBuffer        - pointer to the buffer that holds the text to be displayed.
*		  bufferLength	 - length of the buffer assigned by the user.
*	      pDisplayFont   - pointer to the font image to be used on the editbox
*	      pScheme- Pointer to the style scheme used.
*
* Output Returns the pointer to the object created.
*
* Side Effects: none.
*
********************************************************************/
TEXTENTRY   *TeCreate
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
            );

/*********************************************************************
* Function: WORD TeDraw(void *pObj)
*
* Overview: This function renders the object on the screen using 
* 	        the current parameter settings. Location of the object is 
*	        determined by the left, top, right and bottom parameters. 
*	        The colors used are dependent on the state of the object. 
*		
*		    This widget will draw the keys using the function
*			GOLPanelDraw(). The number of keys will depend on the horizontal 
*			and vertical parameters given (horizontalKeys*verticakKeys). 
* 
* PreCondition: Object must be created before this function is called.
*
* Input: pTe- Pointer to the object to be rendered.
*
* Output: Returns the status of the drawing
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Side Effects: none.
*
********************************************************************/
WORD TeDraw(void *pObj);

/*********************************************************************
* Function:  WORD TeTranslateMsg(void *pObj, GOL_MSG *pMsg)
*				
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. If the message 
*			is valid, the keys in the Text Entry object will be 
*			scanned to detect which key was pressed. If True, the 
*			corresponding text will be displayed, the ‘text’ will 
*			also be stored in the TeOutput parameter of the object. 
*
*	<TABLE>
*    	Translated Message   Input Source  Events         				Description
*     	##################   ############  ######         				###########
*		TE_MSG_PRESS	 	 Touch Screen  EVENT_PRESS, EVENT_MOVE   	If the event occurs and the x,y position falls in the face of one of the keys of the object while the key is unpressed. 
*		TE_MSG_RELEASED	 	 Touch Screen  EVENT_MOVE  	                If the event occurs and the x,y position falls outside the face of one of the keys of the object while the key is pressed. 
*		TE_MSG_RELEASED	 	 Touch Screen  EVENT_RELEASE                If the event occurs and the x,y position falls does not falls inside any of the faces of the keys of the object. 
*		TE_MSG_ADD_CHAR	 	 Touch Screen  EVENT_RELEASE, EVENT_MOVE  	If the event occurs and the x,y position falls in the face of one of the keys of the object while the key is unpressed and the key is associated with no commands.
*		TE_MSG_DELETE	 	 Touch Screen  EVENT_RELEASE, EVENT_MOVE  	If the event occurs and the x,y position falls in the face of one of the keys of the object while the key is unpressed and the key is associated with delete command. 
*		TE_MSG_SPACE	 	 Touch Screen  EVENT_RELEASE, EVENT_MOVE  	If the event occurs and the x,y position falls in the face of one of the keys of the object while the key is unpressed and the key is associated with space command.  
*		TE_MSG_ENTER	 	 Touch Screen  EVENT_RELEASE, EVENT_MOVE  	If the event occurs and the x,y position falls in the face of one of the keys of the object while the key is unpressed and the key is associated with enter command.  
*		OBJ_MSG_INVALID		 Any		   Any			  				If the message did not affect the object.							 
*	</TABLE>
*
* PreCondition: none
*
* Input: 	pTe-	The pointer to the object where the message will be
*					evaluated to check if the message will affect the object.
*        	pMsg-   Pointer to the message struct containing the message from 
*        			the user interface.
*	
* Output: Returns the translated message depending on the received GOL message:
*		  - TE_MSG_PRESS – A key is pressed
*         - TE_MSG_RELEASED - A key was released (generic for keys with no commands or characters assigned)
*         - TE_MSG_ADD_CHAR – A key was released with character assigned
*         - TE_MSG_DELETE – A key was released with delete command assigned
*         - TE_MSG_SPACE - A key was released with space command assigned
*         - TE_MSG_ENTER - A key was released with enter command assigned
*         - OBJ_MSG_INVALID – Text Entry is not affected
*
* Side Effects: none.
*
********************************************************************/
WORD        TeTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: TeMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
* Overview: This function performs the actual state change 
*			based on the translated message given. The following state changes 
*			are supported:
*	<TABLE>
*    	Translated Message   Input Source	Set/Clear State Bit		Description
*     	##################   ############	######     				###########
*     	TE_MSG_ADD_CHAR      Touch Screen,	Set TE_UPDATE_TEXT,	    Add a character in the buffer and update the text displayed.
*												TE_UPDATE_KEY,
*                                           Clear TE_KEY_PRESSED  		
*     	TE_MSG_SPACE      	 Touch Screen,	Set TE_UPDATE_TEXT, 	Insert a space character in the buffer and update the text displayed.
*												TE_UPDATE_KEY,  		
*                                           Clear TE_KEY_PRESSED  		
*     	TE_MSG_DELETE      	 Touch Screen,	Set TE_UPDATE_TEXT, 	Delete the most recent character in the buffer and update the text displayed.
*												TE_UPDATE_KEY,  		
*                                           Clear TE_KEY_PRESSED  		
*     	TE_MSG_ENTER      	 Touch Screen,	Set TE_UPDATE_TEXT, 	User can define the use of this event in the message callback. Object will just update the key.
*												TE_UPDATE_KEY,  		
*                                           Clear TE_KEY_PRESSED  		
*		TE_MSG_RELEASED	 	 Touch Screen,	Clear TE_KEY_PRESSED	A Key in the object will be redrawn in the unpressed state.
*                                           Set Te_UPDATE_KEY
*		TE_MSG_PRESSED	 	 Touch Screen,	Set TE_KEY_PRESSED		A Key in the object will be redrawn in the pressed state.
*                                               TE_UPDATE_KEY
*							         				
*	</TABLE>
* 
* PreCondition: none
*
* Input: translatedMsg - The translated message.
*        pTe           - The pointer to the object whose state will be modified.
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
void        TeMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: void TeSetBuffer(TEXTENTRY *pTe, XCHAR *pText, WORD MaxSize)  
*				
* Overview: This function sets the buffer used to display text. If the 
*			buffer is initialized with a string, the string must be 
*			a null terminated string. If the string length is greater
*           than MaxSize, string will be truncated to MaxSize.
*           pText must point to a valid memory location with size equal 
*           to MaxSize+1. The +1 is used for the string terminator. 
*           
*
* PreCondition: none
*
* Input: 	pTe- pointer to the object
*			pText- pointer to the new text buffer to be displayed
*			maxSize - maximum length of the new buffer to be used.
* Output:  none.	
*		
* Side Effects: none.
*
********************************************************************/
void        TeSetBuffer(TEXTENTRY *pTe, XCHAR *pText, WORD MaxSize);

/*********************************************************************
* Macro: TeGetBuffer(pTe)  
*				
* Overview: This macro will return the currently used buffer in the
*			TextEntry object.
*
* PreCondition: none
*
* Input: 	pTe- pointer to the object
*	
* Output:  It will return a pointer to the buffer used.
*		
* Side Effects: none.
*
********************************************************************/
    #define TeGetBuffer(pTe)    (((TEXTENTRY *)pTe)->pTeOutput)

/*********************************************************************
* Function: void TeClearBuffer (TEXTENTRY *pTe)  
*				
* Overview: This function will clear the data in the display. You must 
*			set the drawing state bit TE_UPDATE_TEXT 
*		 	to update the TEXTENTRY on the screen.			 
*
* PreCondition: none
*
* Input: 	pTe- pointer to the object
*	
* Output:  none
*		
* Side Effects: none.
*
********************************************************************/
void        TeClearBuffer(TEXTENTRY *pTe);

/*********************************************************************
* Function: BOOL TeIsKeyPressed(TEXTENTRY *pTe, WORD index)   
*				
* Overview: This function will test if a key given by its index 
*			in the TextEntry object has been pressed.
*
* PreCondition: none
*
* Input: 	pTe- pointer to the object
*			index- index to the key in the link list
* Output:  	Returns a TRUE if the key is pressed. FALSE if key 
*			is not pressed or the given index does not exist in 
*			the list.
*		
* Side Effects: none.
*
********************************************************************/
BOOL        TeIsKeyPressed(TEXTENTRY *pTe, WORD index);

/*********************************************************************
* Function: void TeSetKeyCommand(TEXTENTRY *pTe,WORD index,WORD command)   
*				
* Overview: This function will assign a command (TE_DELETE_COM, TE_SPACE_COM 
*			or TE_ENTER_COM) to a key with the given index.
*
* PreCondition: none
*
* Input: 	pTe  - 		pointer to the object
*			index  - 	index to the key in the link list
*			command- 	command assigned for the key
*			
* Output:  	Returns TRUE if successful and FALSE if not.
*		
* Side Effects: none.
*
********************************************************************/
BOOL        TeSetKeyCommand(TEXTENTRY *pTe, WORD index, WORD command);

/*********************************************************************
* Function: TeGetKeyCommand(pTe, index)  
*				
* Overview: This function will return the currently used command by a key 
*			with the given index.
*
* PreCondition: none
*
* Input: 	pTe- pointer to the object
*			index- index to the key in the link list
*	
* Output:  It will return the command ID currently set for the key. If the 
*          given index is not in the list the function returns zero.
*          0x00 - no command is assigned or the index given does not exist.
*          0x01 - TE_DELETE_COM
*          0x02 - TE_SPACE_COM
*          0x03 - TE_ENTER_COM
*		
* Side Effects: none.
*
********************************************************************/
WORD        TeGetKeyCommand(TEXTENTRY *pTe, WORD index);

/*********************************************************************
* Function: TeSetKeyText(TEXTENTRY *pTe, WORD index, XCHAR *pText)   
*				
* Overview: This function will set the test assigned to a key with 
*			the given index.
*
* PreCondition: none
*
* Input: 	pTe  - 		pointer to the object
*			index  - 	index to the key in the link list
*			pText - 	pointer to the new string to be used
*			
* Output:  	Returns TRUE if successful and FALSE if not.
*		
* Side Effects: none.
*
********************************************************************/
BOOL TeSetKeyText(TEXTENTRY *pTe, WORD index, XCHAR *pText);

/*********************************************************************
* Function: KEYMEMBER *TeCreateKeyMembers(TEXTENTRY *pTe,XCHAR *pText[])   
*				
* Overview: This function will create the list of KEYMEMBERS that holds the 
*			information on each key. The number of keys is determined by the
*			equation (verticalKeys*horizontalKeys). The object creates the information 
*			holder for each key automatically and assigns each entry in the *pText[] 
*			array with the first entry automatically assigned to the key with an 
*			index of 1. The number of entries to *pText[] must be equal or greater
*			than (verticalKeys*horizontalKeys). The last key is assigned with an index 
*			of (verticalKeys*horizontalKeys)-1. No checking is performed on the 
*			length of *pText[] entries to match (verticalKeys*horizontalKeys).
*
* PreCondition: none
*
* Input: 	pTe  - 		pointer to the object
*			pText -		pointer to the text defined by the user
*			
* Output: Returns the pointer to the newly created KEYMEMBER list. A NULL is returned
*		  if the list is not created succesfully.
*		
* Side Effects: none.
*
********************************************************************/
KEYMEMBER   *TeCreateKeyMembers(TEXTENTRY *pTe, XCHAR *pText[]);

/*********************************************************************
* Function: void TeDelKeyMembers(void *pObj)   
*				
* Overview: This function will delete the KEYMEMBER list assigned to 
*			the object from memory. Pointer to the KEYMEMBER list is 
*			then initialized to NULL.
*
* PreCondition: none
*
* Input: 	pTe  - 	pointer to the object
*			
* Output:  none.	
*		
* Side Effects: none.
*
********************************************************************/
void        TeDelKeyMembers(void *pObj);

/*********************************************************************
* Function: void TeSpaceChar(TEXTENTRY *pTe)   
*				
* Overview: This function will insert a space character to the end of 
*			the buffer. Drawing states TE_UPDATE_TEXT or TE_DRAW must 
*			be set to see the effect of this insertion.
*
* PreCondition: none
*
* Input: 	pTe  - 	pointer to the object
*			
* Output:  none.	
*		
* Side Effects: none.
*
********************************************************************/
void        TeSpaceChar(TEXTENTRY *pTe);

/*********************************************************************
* Function: void TeAddChar(TEXTENTRY *pTe)   
*				
* Overview: This function will insert a character to the end of 
*			the buffer. The character inserted is dependent on the 
*			currently pressed key. Drawing states TE_UPDATE_TEXT or 
*			TE_DRAW must be set to see the effect of this insertion.
*
* PreCondition: none
*
* Input: 	pTe  - 	pointer to the object
*			
* Output:  	
*		
* Side Effects: none.
*
********************************************************************/
void        TeAddChar(TEXTENTRY *pTe);
#endif // _TEXTENTRY_H
