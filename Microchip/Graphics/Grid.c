/*****************************************************************************
 *  Module for Microchip Graphics Library 
 *  Grid control
 *****************************************************************************
 * FileName:        Grid.c
 * Dependencies:    string.h, Graphics.h 
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
 * 02/29/08    ...
 * 04/16/10	   Added Grid Item as text.
 * 04/20/11    Fixed KEYBOARD bug on object ID and GOL_MSG param1 comparison.
 *****************************************************************************/

/*
    Grid Object

    Grid does not support scroll bars. Size defined must fit on the screen.

*/
#include <string.h>
#include "Graphics/Graphics.h"

#ifdef USE_GRID
    #define CELL_AT(c, r)   ((c * pGrid->numRows) + r)

/* */

GRID *GridCreate
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
)
{
    GRID    *pGrid = NULL;

    if((pGrid = (GRID *)GFX_malloc(sizeof(GRID))) == NULL)
    {
        return (NULL);
    }

    if((pGrid->gridObjects = GFX_malloc(sizeof(GRIDITEM) * numColumns * numRows)) == NULL)
    {
        GFX_free(pGrid);
        return (NULL);
    }

    // Initialize grid items to default.
    memset(pGrid->gridObjects, 0, sizeof(GRIDITEM) * numColumns * numRows);

    // Initialize grid
    pGrid->hdr.ID = ID;
    pGrid->hdr.pNxtObj = NULL;
    pGrid->hdr.type = OBJ_GRID;
    pGrid->hdr.state = state;
    pGrid->hdr.left = left;
    pGrid->hdr.top = top;
    pGrid->hdr.right = right;
    pGrid->hdr.bottom = bottom;
    pGrid->numColumns = numColumns;
    pGrid->numRows = numRows;
    pGrid->cellWidth = cellWidth;
    pGrid->cellHeight = cellHeight;
    pGrid->focusX = 0;
    pGrid->focusY = 0;
    pGrid->hdr.DrawObj = GridDraw;				// draw function
    pGrid->hdr.MsgObj = GridTranslateMsg;       // message function
    pGrid->hdr.MsgDefaultObj = GridMsgDefault;  // default message function
    pGrid->hdr.FreeObj = GridFreeItems;			// free function

    // Set the color scheme to be used
    if(pScheme == NULL)
    {
        pGrid->hdr.pGolScheme = _pDefaultGolScheme;
    }
    else
    {
        pGrid->hdr.pGolScheme = (GOL_SCHEME *)pScheme;
    }

    GOLAddObject((OBJ_HEADER *)pGrid);

    return (pGrid);
}

    #define CELL_LEFT       (1 + pGrid->hdr.left + (i * (pGrid->cellWidth + 1)))
    #define CELL_TOP        (1 + pGrid->hdr.top + (j * (pGrid->cellHeight + 1)))
    #define CELL_RIGHT      (CELL_LEFT + pGrid->cellWidth - 1)
    #define CELL_BOTTOM     (CELL_TOP + pGrid->cellHeight - 1)
    #define BITMAP_SCALE    1

/* */

WORD GridDraw(void *pObj)
{
    SHORT   i;
    SHORT   j;
    SHORT   xText, yText;
    GRID *pGrid;

    pGrid = (GRID *)pObj;
    
    if
    (
        (pGrid->hdr.state & GRID_DRAW_ITEMS) ||
        (pGrid->hdr.state & GRID_DRAW_ALL) ||
        (pGrid->hdr.state & GRID_SHOW_FOCUS)
    )
    {
        if(pGrid->hdr.state & GRID_DRAW_ALL)
        {

            // Clear the entire region.
            SetColor(pGrid->hdr.pGolScheme->CommonBkColor);
            while(!Bar(pGrid->hdr.left, pGrid->hdr.top, pGrid->hdr.right, pGrid->hdr.bottom));
            
            // initialize the global cursor positions to the Grid left top position.
			_cursorX = pGrid->hdr.left;
			_cursorY = pGrid->hdr.top;

            // Draw the grid lines
            if(pGrid->hdr.state & (GRID_SHOW_LINES | GRID_SHOW_BORDER_ONLY | GRID_SHOW_SEPARATORS_ONLY))
            {
                SetLineType(SOLID_LINE);
                SetColor(pGrid->hdr.pGolScheme->EmbossLtColor);

                // Draw the outside of the box
                if(pGrid->hdr.state & (GRID_SHOW_LINES | GRID_SHOW_BORDER_ONLY))
                {
                    while(!Line(pGrid->hdr.left, pGrid->hdr.top, pGrid->hdr.right, pGrid->hdr.top));
                    LineTo(_cursorX, pGrid->hdr.bottom);
                    LineTo(pGrid->hdr.left, _cursorY);
                    LineTo(pGrid->hdr.left, pGrid->hdr.top);
                }

                // Draw the lines between each cell
                if(pGrid->hdr.state & (GRID_SHOW_LINES | GRID_SHOW_SEPARATORS_ONLY))
                {
                    for(i = 1; i < pGrid->numColumns; i++)
                    {
                        while
                        (
                            !Line
                                (
                                    pGrid->hdr.left + i * (pGrid->cellWidth + 1),
                                    pGrid->hdr.top,
                                    pGrid->hdr.left + i * (pGrid->cellWidth + 1),
                                    pGrid->hdr.top + pGrid->numRows * (pGrid->cellHeight + 1)
                                )
                        );
                    }

                    for(i = 1; i < pGrid->numRows; i++)
                    {
                        while
                        (
                            !Line
                                (
                                    pGrid->hdr.left,
                                    pGrid->hdr.top + i * (pGrid->cellHeight + 1),
                                    pGrid->hdr.right,
                                    pGrid->hdr.top + i * (pGrid->cellHeight + 1)
                                )
                        );
                    }
                }
            }
        }

        for(i = 0; i < pGrid->numColumns; i++)
        {
            for(j = 0; j < pGrid->numRows; j++)
            {
                if
                (
                    (pGrid->hdr.state & GRID_DRAW_ALL) ||
                    (
                        (pGrid->hdr.state & GRID_DRAW_ITEMS) &&
                        (pGrid->gridObjects[CELL_AT(i, j)].status & GRIDITEM_DRAW)
                    ) ||
                    ((pGrid->hdr.state & GRID_SHOW_FOCUS) && (i == pGrid->focusX) && (j == pGrid->focusY))
                )
                {

                    // Clear the cell
                    SetColor(pGrid->hdr.pGolScheme->CommonBkColor);
                    while(!Bar(CELL_LEFT, CELL_TOP, CELL_RIGHT, CELL_BOTTOM));

                    // Draw the cell
                    if((pGrid->gridObjects[CELL_AT(i, j)].status & GRID_TYPE_MASK) == GRIDITEM_IS_BITMAP)
                    {

                        // Draw the bitmap
                        if(pGrid->gridObjects[CELL_AT(i, j)].data)
                        {
                            while(!PutImage(CELL_LEFT, CELL_TOP, pGrid->gridObjects[CELL_AT(i, j)].data, BITMAP_SCALE));
                        }
                    }
                    else
                    {

                        // Draw the text
                        if(pGrid->gridObjects[CELL_AT(i, j)].data)
                        {
	                        SetFont(pGrid->hdr.pGolScheme->pFont);
	                        SetColor(pGrid->hdr.pGolScheme->TextColor0);
	                        
	                        if(GetState(pGrid, GRIDITEM_TEXTRIGHT))
            				{
	            				xText = CELL_LEFT+pGrid->cellWidth-GetTextWidth(pGrid->gridObjects[CELL_AT(i, j)].data,pGrid->hdr.pGolScheme->pFont);
	            			}
	                        else if(GetState(pGrid, GRIDITEM_TEXTLEFT))
            				{
	            				xText = CELL_LEFT;
	            			}
	            			else
	            			{
		            			xText = CELL_LEFT+(pGrid->cellWidth>>1)-(GetTextWidth(pGrid->gridObjects[CELL_AT(i, j)].data,pGrid->hdr.pGolScheme->pFont)>>1);
		            		}	
	                        if(GetState(pGrid, GRIDITEM_TEXTBOTTOM))
            				{
	            				yText = CELL_TOP+pGrid->cellHeight-GetTextHeight(pGrid->hdr.pGolScheme->pFont);
	            			}
	                        else if(GetState(pGrid, GRIDITEM_TEXTTOP))
            				{
	            				yText = CELL_TOP;
	            			}
	            			else
	            			{
		            			yText = CELL_TOP+(pGrid->cellHeight>>1)-(GetTextHeight(pGrid->hdr.pGolScheme->pFont)>>1);
		            		}		
	            				
                            while(!OutTextXY( xText, yText, pGrid->gridObjects[CELL_AT(i, j)].data));
                        }                        
                    }

                    // Draw the focus if applicable.
                    if((pGrid->hdr.state & GRID_SHOW_FOCUS) && (i == pGrid->focusX) && (j == pGrid->focusY))
                    {
                        SetColor(pGrid->hdr.pGolScheme->EmbossLtColor);
                        SetLineType(DOTTED_LINE);
                        SetLineThickness(NORMAL_LINE);
                        while(!Rectangle(CELL_LEFT, CELL_TOP, CELL_RIGHT, CELL_BOTTOM));
                    }

                    // If the cell is selected, indicate it.
                    if(pGrid->gridObjects[CELL_AT(pGrid->focusX, pGrid->focusY)].status & GRIDITEM_SELECTED)
                    {
                        SetColor(pGrid->hdr.pGolScheme->EmbossLtColor);
                        SetLineType(SOLID_LINE);
                        if(pGrid->hdr.state & GRID_SHOW_LINES)
                        {
                            SetLineThickness(THICK_LINE);
                        }
                        else
                        {
                            SetLineThickness(NORMAL_LINE);
                        }

                        while(!Rectangle(CELL_LEFT - 1, CELL_TOP - 1, CELL_RIGHT + 1, CELL_BOTTOM + 1));
                    }

                    GridClearCellState(pGrid, i, j, GRIDITEM_DRAW);
                }
            }
        }

        //pGrid->state &= ~(GRID_DRAW_ITEMS || GRID_DRAW_ALL || GRID_SHOW_FOCUS);
        pGrid->hdr.state &= ~(GRID_DRAW_ITEMS || GRID_DRAW_ALL);

        // Set line state
        SetLineType(SOLID_LINE);
    }

    return (1);
}

/* */
void GridFreeItems(void *pObj)
{
    GRID *pGrid;

    pGrid = (GRID *)pObj;

    if(pGrid && pGrid->gridObjects)
    {
        GFX_free(pGrid->gridObjects);
        pGrid->gridObjects = NULL;  // Just in case...
    }
}

/* */
WORD GridSetCell(GRID *pGrid, SHORT column, SHORT row, WORD state, void *data)
{
    if((column >= pGrid->numColumns) || (row >= pGrid->numRows))
    {
        return (GRID_OUT_OF_BOUNDS);
    }

    pGrid->gridObjects[CELL_AT(column, row)].data = data;
    pGrid->gridObjects[CELL_AT(column, row)].status = state;    // This overwrites GRIDITEM_SELECTED
    return (GRID_SUCCESS);
}

/* */
void GridClearCellState(GRID *pGrid, SHORT column, SHORT row, WORD state)
{
    if((column >= pGrid->numColumns) || (row >= pGrid->numRows))
    {
        return;
    }

    pGrid->gridObjects[CELL_AT(column, row)].status &= ~state;

    return;
}

/* */
void GridSetFocus(GRID *pGrid, SHORT column, SHORT row)
{
    if((column >= pGrid->numColumns) || (row >= pGrid->numRows))
    {
        return;
    }

    pGrid->focusX = column;
    pGrid->focusY = row;
}

/* */
void GridSetCellState(GRID *pGrid, SHORT column, SHORT row, WORD state)
{
    if((column >= pGrid->numColumns) || (row >= pGrid->numRows))
    {
        return;
    }

    pGrid->gridObjects[CELL_AT(column, row)].status |= state;

    return;
}

/* */
void *GridGetCell(GRID *pGrid, SHORT column, SHORT row, WORD *cellType)
{
    if((column >= pGrid->numColumns) || (row >= pGrid->numRows))
    {
        return (NULL);
    }

    *cellType = pGrid->gridObjects[CELL_AT(column, row)].status & GRID_TYPE_MASK;

    return (pGrid->gridObjects[CELL_AT(column, row)].data);
}

/* */
void GridMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
{
    GRID *pGrid;

    pGrid = (GRID *)pObj;

    switch(translatedMsg)
    {
        case GRID_MSG_ITEM_SELECTED:

            // Currently, only a single item can be selected.  This can be expanded later,
            // when touchscreen support is enhanced.
            pGrid->gridObjects[CELL_AT(pGrid->focusX, pGrid->focusY)].status ^= GRIDITEM_SELECTED;

            //            SetState( pGrid, GRID_SHOW_FOCUS | GRID_DRAW_ITEMS );
            SetState(pGrid, GRID_DRAW_ITEMS);
            break;

        case GRID_MSG_UP:
            if(pGrid->focusY > 0)
            {
                pGrid->gridObjects[CELL_AT(pGrid->focusX, pGrid->focusY)].status |= GRIDITEM_DRAW;
                pGrid->focusY--;

                //            SetState( pGrid, GRID_SHOW_FOCUS | GRID_DRAW_ITEMS );
                SetState(pGrid, GRID_DRAW_ITEMS);
            }

            break;

        case GRID_MSG_DOWN:
            if(pGrid->focusY < (pGrid->numRows - 1))
            {
                pGrid->gridObjects[CELL_AT(pGrid->focusX, pGrid->focusY)].status |= GRIDITEM_DRAW;
                pGrid->focusY++;

                //            SetState( pGrid, GRID_SHOW_FOCUS | GRID_DRAW_ITEMS );
                SetState(pGrid, GRID_DRAW_ITEMS);
            }

            break;

        case GRID_MSG_LEFT:
            if(pGrid->focusX > 0)
            {
                pGrid->gridObjects[CELL_AT(pGrid->focusX, pGrid->focusY)].status |= GRIDITEM_DRAW;
                pGrid->focusX--;

                //            SetState( pGrid, GRID_SHOW_FOCUS | GRID_DRAW_ITEMS );
                SetState(pGrid, GRID_DRAW_ITEMS);
            }

            break;

        case GRID_MSG_RIGHT:
            if(pGrid->focusX < (pGrid->numColumns - 1))
            {
                pGrid->gridObjects[CELL_AT(pGrid->focusX, pGrid->focusY)].status |= GRIDITEM_DRAW;
                pGrid->focusX++;

                //            SetState( pGrid, GRID_SHOW_FOCUS | GRID_DRAW_ITEMS );
                SetState(pGrid, GRID_DRAW_ITEMS);
            }

            break;
    }
}

/* */
WORD GridTranslateMsg(void *pObj, GOL_MSG *pMsg)
{

    GRID *pGrid;

    pGrid = (GRID *)pObj;

    // Evaluate if the message is for the check box
    // Check if disabled first
    if(GetState(pGrid, GRID_DISABLED))
    {
        return (OBJ_MSG_INVALID);
    }

        #ifdef USE_TOUCHSCREEN
    if(pMsg->type == TYPE_TOUCHSCREEN)
    {

        // Check if it falls in the check box borders
        if
        (
            (pGrid->hdr.left <= pMsg->param1) &&
            (pGrid->hdr.right >= pMsg->param1) &&
            (pGrid->hdr.top <= pMsg->param2) &&
            (pGrid->hdr.bottom >= pMsg->param2)
        )
        {
            return (GRID_MSG_TOUCHED);
        }

        return (OBJ_MSG_INVALID);
    }

        #endif
        #ifdef USE_KEYBOARD
    if((pMsg->uiEvent == EVENT_KEYSCAN) && (pMsg->type == TYPE_KEYBOARD) && ((WORD)pMsg->param1 == pGrid->hdr.ID))
    {
        if((pMsg->param2 == SCAN_SPACE_PRESSED) || (pMsg->param2 == SCAN_CR_PRESSED))
        {
            return (GRID_MSG_ITEM_SELECTED);
        }
        else if(pMsg->param2 == SCAN_LEFT_PRESSED)
        {
            return (GRID_MSG_LEFT);
        }
        else if(pMsg->param2 == SCAN_RIGHT_PRESSED)
        {
            return (GRID_MSG_RIGHT);
        }
        else if(pMsg->param2 == SCAN_UP_PRESSED)
        {
            return (GRID_MSG_UP);
        }
        else if(pMsg->param2 == SCAN_DOWN_PRESSED)
        {
            return (GRID_MSG_DOWN);
        }
    }

    return (OBJ_MSG_INVALID);
        #endif
    return (OBJ_MSG_INVALID);
}

#endif // USE_GRID
