/*****************************************************************************
 *  Module for Microchip Graphics Library 
 *  Grid control
 *****************************************************************************
 * FileName:        Grid.h
 * Dependencies:    none
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 02/29/08    ...
 * 04/16/10	   Added Grid Item as text.
 *****************************************************************************/
#ifndef _GRID_H_
    #define _GRID_H_

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

//******************************************************************************
// Grid Object State Definitions
//******************************************************************************
    #define GRID_FOCUSED                0x0001  // Bit for focused state
    #define GRID_DISABLED               0x0002  // Bit for disabled state
    #define GRID_SHOW_LINES             0x0004  // Display grid lines
    #define GRID_SHOW_FOCUS             0x0008  // Highlight the focused cell.
    #define GRID_SHOW_BORDER_ONLY       0x0010  // Draw only the outside border of the grid.
    #define GRID_SHOW_SEPARATORS_ONLY   0x0020  // Draw only the lines between cells (like Tic-tac-toe)
    #define GRID_DRAW_ITEMS             0x1000  // Bit to indicate that at least one item must be redrawn, but not the entire grid.
    #define GRID_DRAW_ALL               0x4000  // Bit to indicate whole edit box must be redrawn
    #define GRID_HIDE                   0x8000  // Bit to remove object from screen

//******************************************************************************
// Grid Item State Definitions
//******************************************************************************
	#define GRIDITEM_SELECTED           0x0001  // The cell is selected.
    #define GRIDITEM_IS_TEXT            0x0000  // The grid item is a text string.
    #define GRIDITEM_IS_BITMAP          0x0008  // The grid item is a bitmap.
    #define GRIDITEM_TEXTRIGHT          0x0010  // Text in the cell is right aligned.
    #define GRIDITEM_TEXTLEFT           0x0020  // Text in the cell is left aligned.
    #define GRIDITEM_TEXTBOTTOM         0x0040  // Bit to indicate text is top aligned.
    #define GRIDITEM_TEXTTOP            0x0080  // Bit to indicate text is bottom aligned.
    #define GRIDITEM_DRAW               0x0100  // Draw this cell


	#define GRID_TYPE_MASK              (0x0C)
    #define GRID_SUCCESS                0x0000	// Status of a successful GridSetCell() operation.
    #define GRID_OUT_OF_BOUNDS          0x0001  // Status of an out of bounds cell GridSetCell() operation.

    #define GRID_MAX_COLUMNS(rw, cw)    ((rw - (GOL_EMBOSS_SIZE * 2) + 1) / (cw + 1))
    #define GRID_MAX_ROWS(rh, ch)       ((rh - (GOL_EMBOSS_SIZE * 2) + 1) / (ch + 1))
    #define GRID_WIDTH(c, cw)           ((GOL_EMBOSS_SIZE * 2) + (c * (cw + 1)) - 1)
    #define GRID_HEIGHT(r, ch)          ((GOL_EMBOSS_SIZE * 2) + (r * (ch + 1)) - 1)

/*********************************************************************
* Overview: Defines the grid item.
* 
*********************************************************************/
typedef struct
{
    void    *data;								// Indicates if the Grid Item is type GRIDITEM_IS_TEXT or GRIDITEM_IS_BITMAP
    WORD    status;								// indicates the status of the Grid Item
} GRIDITEM;

/*********************************************************************
* Overview: Defines the parameters required for a grid Object.
* 			Clipping is not supported in grid object. 
* 
*********************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;                            // Generic header for all Objects (see OBJ_HEADER).
    SHORT       numColumns;                     // Number of columns drawn for the Grid.
    SHORT       numRows;                        // Number of rows drawn for the Grid.
    SHORT       cellHeight;                     // The height of each cell in pixels.
    SHORT       cellWidth;                      // The width of each cell in pixels.
    SHORT       focusX;                         // The x position of the cell to be focused.
    SHORT       focusY;                         // The y position of the cell to be focused.
    GRIDITEM    *gridObjects;                   // The pointer to grid items 
} GRID;

/*********************************************************************
* Function: GRID *GridCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, 
*			                 WORD state, SHORT numColumns, SHORT numRows, 
*							 SHORT cellWidth, SHORT cellHeight, GOL_SCHEME *pScheme)
*
* Overview: This function creates a GRID object with the parameters 
*			given. It automatically attaches the new object into a global 
*			linked list of objects and returns the address of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the Object.
* 		 top - Top most position of the Object. 
*		 right - Right most position of the Object.
*		 bottom - Bottom most position of the object.
*		 state - Sets the initial state of the object.
*		 numColumns - Sets the number of columns for the grid.
*		 numRows - Sets the number of rows for the grid.
*		 cellWidth - Sets the width of each cell of the grid.
*		 cellHeight - Sets the height of each cell of the grid.
*		 pScheme - Pointer to the style scheme used for the object.
*				   Set to NULL if default style scheme is used.
*
* Output: Returns the pointer to the object created.
*
* Side Effects: none
*
********************************************************************/
GRID    *GridCreate
        (
            WORD        ID,
            SHORT       left,
            SHORT       top,
            SHORT       right,
            SHORT       bottom,
            WORD        state,
            SHORT       numColumns,
            SHORT       numRows,
            SHORT       cellWidth,
            SHORT       cellHeight,
            GOL_SCHEME  *pScheme
        );

/*********************************************************************
* Function: GridClearCellState(GRID *pGrid, SHORT column, SHORT row, WORD state)
*
* Overview: This function clears the state of the cell (or Grid Item) specified 
*			by the column and row.
*
* PreCondition: none
*
* Input: pGrid - Pointer to the object.
*		 column - column index of the cell
*		 row - row index of the cell
*		 atate - specifies the state to be cleared. See Grid Item State.
*
* Output: none.
*
* Side Effects: none
*
********************************************************************/
void    GridClearCellState(GRID *pGrid, SHORT column, SHORT row, WORD state);


/*********************************************************************
* Function: WORD GridDraw(void *pObj)
*
* Overview: This function renders the object on the screen using 
*			the current parameter settings. Location of the object 
*			is determined by the left, top, right and bottom parameters. 
*			The colors used are dependent on the state of the object. 
*			The font used is determined by the style scheme set.
*			
*			When rendering objects of the same type, each object must 
*			be rendered completely before the rendering of the next object 
*			is started. This is to avoid incomplete object rendering.
*
* PreCondition: Object must be created before this function is called.
*
* Input: pGb - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing 
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Side Effects: none
*
********************************************************************/
WORD GridDraw(void *pObj);

/*********************************************************************
* Function: GridFreeItems(void *pObj)
*
* Overview: This function removes all grid items for the given Grid
*			and frees the memory used.
*			
* PreCondition: Object must be created before this function is called.
*
* Input: pGrid - The pointer to the Grid object.
*        
* Output: none.
*
* Side Effects: none
*
********************************************************************/
void    GridFreeItems(void *pObj);

/*********************************************************************
* Function: *GridGetCell(GRID *pGrid, SHORT column, SHORT row, WORD *cellType)
*
* Overview: This function removes all grid items for the given Grid
*			and frees the memory used.
*			
* PreCondition: Object must be created before this function is called.
*
* Input: pGrid - The pointer to the Grid object.
*		 column - the column index of the cell
*		 row - the row index of the cell
*		 cellType - pointer that will receive the type of grid item 
*				    or cell (GRIDITEM_IS_TEXT or GRIDITEM_IS_BITMAP).
*        
* Output: Returns a pointer to the grid item or cell data.
*
* Side Effects: none
*
********************************************************************/
void    *GridGetCell(GRID *pGrid, SHORT column, SHORT row, WORD *cellType);

/*********************************************************************
* Macros:  GridGetFocusX(pGrid)
*
* Overview: This macro returns the x position of the focused cell.
*
* PreCondition: none
*
* Input: pGrid - Pointer to the object.
*
* Output: Returns the x position of the focused cell.
*
* Side Effects: none
*
********************************************************************/
#define GridGetFocusX(pGrid)    pGrid->focusX

/*********************************************************************
* Macros:  GridGetFocusY(pGrid)
*
* Overview: This macro returns the y position of the focused cell.
*
* PreCondition: none
*
* Input: pGrid - Pointer to the object.
*
* Output: Returns the y position of the focused cell.
*
* Side Effects: none
*
********************************************************************/
#define GridGetFocusY(pGrid)    pGrid->focusY

/*********************************************************************
* Function: GridMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
*
* Overview: This function performs the actual state change 
*			based on the translated message given. The following state changes 
*			are supported:
*	<TABLE>
*    	Translated Message		Input Source	Set/Clear State Bit		Description
*     	##################		############	######     				###########
*     	GRID_MSG_TOUCHED		Touch Screen	none  					Grid will have no state change because of this event.
*		GRID_MSG_ITEM_SELECTED	Keyboard		Set GRIDITEM_SELECTED,  Grid Item selected will be redrawn.
*												GRID_DRAW_ITEMS		
*		GRID_MSG_UP				Keyboard		Set GRIDITEM_DRAW,		Grid Item above the currently focused item will be redrawn.
*								        		GRID_DRAW_ITEMS
*		GRID_MSG_DOWN			Keyboard		Set GRIDITEM_DRAW,		Grid Item below the currently focused item will be redrawn.
*								        		GRID_DRAW_ITEMS
*		GRID_MSG_LEFT			Keyboard		Set GRIDITEM_DRAW,		Grid Item to the left of the currently focused item will be redrawn.
*								        		GRID_DRAW_ITEMS
*		GRID_MSG_RIGHT			Keyboard		Set GRIDITEM_DRAW,		Grid Item to the right of the currently focused item will be redrawn.
*								        		GRID_DRAW_ITEMS							         				
*	</TABLE>
*
* PreCondition: none
*
* Input: translatedMsg - The translated message.
*        pGrid         - The pointer to the object whose state will be modified.
*        pMsg          - The pointer to the GOL message.
*        
* Output: none
*
* Side Effects: none
*
********************************************************************/
void    GridMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: GridTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. The table below enumerates the translated 
*			messages for each event of the touch screen and keyboard inputs.
*
*	<TABLE>
*    	Translated Message       Input Source	    Set/Clear State Bit	        Description
*     	##################       ############	    #######################     ##################################################################################################################################################################
*     	GRID_MSG_TOUCHED         Touch Screen       none    					If any touch events occurs and the x,y position falls in the face of the grid.
*		GRID_MSG_ITEM_SELECTED   Keyboard			EVENT_KEYSCAN				If event occurs and parameter1 passed matches the object’s ID and parameter 2 passed matches SCAN_SPACE_PRESSED or SCAN_CR_PRESSED.  				
*		GRID_MSG_UP				 Keyboard           EVENT_KEYSCAN  				If event occurs and parameter1 passed matches the object’s ID and parameter 2 passed matches SCAN_UP_PRESSED.      					      			   
*		GRID_MSG_DOWN			 Keyboard           EVENT_KEYSCAN  				If event occurs and parameter1 passed matches the object’s ID and parameter 2 passed matches SCAN_DOWN_PRESSED.      					      			   
*		GRID_MSG_LEFT			 Keyboard           EVENT_KEYSCAN  				If event occurs and parameter1 passed matches the object’s ID and parameter 2 passed matches SCAN_LEFT_PRESSED.      					      			   
*		GRID_MSG_RIGHT			 Keyboard           EVENT_KEYSCAN  				If event occurs and parameter1 passed matches the object’s ID and parameter 2 passed matches SCAN_RIGHT_PRESSED.      					      			   
*		OBJ_MSG_INVALID		     Any		        Any			  				If the message did not affect the object.							 
*	</TABLE>
*
* PreCondition: none
*
* Input: pGrid - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		  - GRID_MSG_TOUCHED - when the grid object is touched.
*		  - GRID_MSG_ITEM_SELECTED – when key scan SCAN_SPACE_PRESSED or SCAN_CR_PRESSED are detected.
*		  - GRID_MSG_UP – when key scan SCAN_UP_PRESSED is detected.
*		  - GRID_MSG_DOWN – when key scan SCAN_DOWN_PRESSED is detected.
*		  - GRID_MSG_LEFT – when key scan SCAN_LEFT_PRESSED is detected.
*		  - GRID_MSG_RIGHT – when key scan SCAN_RIGHT_PRESSED is detected.
*         - OBJ_MSG_INVALID – Button is not affected
*
*
* Side Effects: none
*
********************************************************************/
WORD    GridTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: WORD    GridSetCell(GRID *pGrid, SHORT column, SHORT row, WORD state, void *data)
*
* Overview: This function sets the Grid Item state and data.
*			
* PreCondition: Object must be created before this function is called.
*
* Input: pGrid - The pointer to the Grid object.
*		 column - the column index of the cell
*		 row - the row index of the cell
*		 state - sets the state of the Grid Item specified.
*		 data - pointer to the data used for the Grid Item.
*        
* Output: Returns the status of the operation
*		  - GRID_SUCCESS - if the set succeeded
*		  - GRID_OUT_OF_BOUNDS - if the row and column given results in an out of bounds location.
*
* Side Effects: none
*
********************************************************************/
WORD    GridSetCell(GRID *pGrid, SHORT column, SHORT row, WORD state, void *data);

/*********************************************************************
* Function: GridSetCellState(GRID *pGrid, SHORT column, SHORT row, WORD state)
*
* Overview: This function sets the state of the Grid Item or cell.
*			
* PreCondition: Object must be created before this function is called.
*
* Input: pGrid - The pointer to the Grid object.
*		 column - the column index of the cell
*		 row - the row index of the cell
*		 state - sets the state of the Grid Item specified.
*        
* Output: none.
*
* Side Effects: none
*
********************************************************************/
void    GridSetCellState(GRID *pGrid, SHORT column, SHORT row, WORD state);

/*********************************************************************
* Function: GridSetFocus(GRID *pGrid, SHORT column, SHORT row)
*
* Overview: This function sets the focus of the specified Grid Item or cell.
*			
* PreCondition: Object must be created before this function is called.
*
* Input: pGrid - The pointer to the Grid object.
*		 column - the column index of the cell
*		 row - the row index of the cell
*        
* Output: none.
*
* Side Effects: none
*
********************************************************************/
void    GridSetFocus(GRID *pGrid, SHORT column, SHORT row);

#endif
