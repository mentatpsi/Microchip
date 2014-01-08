/******************************************************************************

    Interactive Graphical Demonstrations

    (Games!)


*******************************************************************************/

//DOM-IGNORE-BEGIN

/******************************************************************************

* File Name:       Games.c
* Dependencies:    None
* Processor:       PIC24FJ256GB106
* Compiler:        C30 v2.01
* Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the “Company”) for its PICmicro® Microcontroller is intended and
supplied to you, the Company’s customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
software is owned by the Company and/or its supplier, and is
protected under applicable copyright laws. All rights are reserved.
Any use in violation of the foregoing restrictions may subject the
user to criminal sanctions under applicable laws, as well as to
civil liability for the breach of the terms and conditions of this
license.

THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

Author          Date    Comments
--------------------------------------------------------------------------------
KO          14-Feb-2008 First release

*******************************************************************************/

//DOM-IGNORE-END
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "MainDemo.h"

//******************************************************************************
//******************************************************************************
// General Games Support
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Fonts Used
//******************************************************************************
#define GAMEBIGFONT     GOLFontDefault
#define GAMESMALLFONT   GOLFontDefault

extern const FONT_FLASH GAMEBIGFONT;
extern const FONT_FLASH GAMESMALLFONT;

//******************************************************************************
// Strings used
//******************************************************************************
extern XCHAR            LeftArrowStr[];
extern XCHAR            RightArrowStr[];
extern XCHAR            UpArrowStr[];
extern XCHAR            DownArrowStr[];
extern XCHAR            NewGameStr[];
extern XCHAR            ExitStr[];

//******************************************************************************
// Constants
//******************************************************************************
#define ID_SCORE        51
#define ID_SCORE_INFO   52
#define ID_QUIT         53

#define RANDOM_SEED     3

//******************************************************************************
// Global Variables
//******************************************************************************
GRID        *gamesGrid;
STATICTEXT  *pScore;
SHORT       score;
XCHAR       scoreText[20] = {'S','C','O','R','E',':',0};
XCHAR       scoreInfo[26];

XCHAR       *pScoreText;
XCHAR       *pScoreInfo;

extern void Int2Str(XCHAR *pStr, WORD value, SHORT charCount);

//******************************************************************************
// Internal Prototypes
//******************************************************************************
void        InitializeRandomNumberGenerator(void);
SHORT       Random(SHORT max);

#define WAIT_UNTIL_FINISH(x)    while(!x)

    /****************************************************************************
  Function:
    void InitializeRandomNumberGenerator( void )

  Description:
    This function initializes the random number generator if it has not
    already been initialized.  The initialization method can be changed by
    changing the macro RANDOM_SEED.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    In its simplest form, RANDOM_SEED can be a constant.  To make things more
    interesting, RANDOM_SEED can be defined as part of a timer value.
  ***************************************************************************/
    void InitializeRandomNumberGenerator(void)
{
    static BOOL initialized;

    if(!initialized)
    {
        srand(RANDOM_SEED);
        initialized = TRUE;
    }
}

/****************************************************************************
  Function:
    SHORT Random( SHORT max )

  Description:
    This routine returns a random integer between 0 and max inclusive.

  Precondition:
    The random number generator should be initialized before calling this
    function.

  Parameters:
    SHORT max   - The maximum integer value to return.

  Returns:
    SHORT - A random number between 0 and max inclusive.

  Remarks:
    None
  ***************************************************************************/
SHORT Random(SHORT max)
{
    SHORT   num = 0;

    num = rand() & 0xFF;    // We only need a byte...
    while(num >= (max + 1))
    {
        num -= (max + 1);
    }

    return (num);
}

//******************************************************************************
//******************************************************************************
// Snake
//******************************************************************************
//******************************************************************************
#define BLANK_BALL      (NUM_BALL_TYPES + 1)
#define MARKING_BALL    (NUM_BALL_TYPES)
#define NUM_BALL_TYPES  4

#define ballCircle      head
#define ballSquare      body
#define ballCross       food
#define ballTriangle    food
#define ballMarking     food

void                        *balls[NUM_BALL_TYPES + 2];

#define DIRECTION_NONE          0
#define DIRECTION_UP            1
#define DIRECTION_RIGHT         2
#define DIRECTION_DOWN          3
#define DIRECTION_LEFT          4

#define ID_SNAKE_UP             60
#define ID_SNAKE_RIGHT          61
#define ID_SNAKE_DOWN           62
#define ID_SNAKE_LEFT           63

#define SEGMENT_SIZE            24

#define SNAKE_GRID_WIDTH        (GRID_WIDTH(GRID_MAX_COLUMNS(GetMaxX(), SEGMENT_SIZE), SEGMENT_SIZE) - 2 * GOL_EMBOSS_SIZE + 1)
#define SNAKE_GRID_HEIGHT       (GRID_HEIGHT(GRID_MAX_ROWS(CtrlBtnTop(), SEGMENT_SIZE), SEGMENT_SIZE) - 2 * GOL_EMBOSS_SIZE + 1)
#define SNAKE_GRID_TOP          ((CtrlBtnTop() - SNAKE_GRID_HEIGHT) / 2)
#define SNAKE_GRID_BOTTOM       (SNAKE_GRID_TOP + SNAKE_GRID_HEIGHT)
#define SNAKE_GRID_LEFT         ((GetMaxX() - SNAKE_GRID_WIDTH) / 2)
#define SNAKE_GRID_RIGHT        (SNAKE_GRID_LEFT + SNAKE_GRID_WIDTH)
#define SNAKE_COLUMNS           ((SNAKE_GRID_RIGHT - SNAKE_GRID_LEFT) / SEGMENT_SIZE)
#define SNAKE_ROWS              ((SNAKE_GRID_BOTTOM - SNAKE_GRID_TOP) / SEGMENT_SIZE)
#define FIELD_PIECES            SNAKE_COLUMNS
#define LONGEST_SNAKE           100

#define SNAKE_UPDATE_INTERVAL   (1000)

typedef struct _PIECE_INFO
{
    SHORT   column;
    SHORT   row;
    SHORT   direction;
} PIECE_INFO;

typedef struct _FIELD_INFO
{
    BOOL    hasFood;
    BOOL    notFood;
    BOOL    hasBeenEaten;
} FIELD_INFO;

typedef struct _SNAKE_INFO
{
    PIECE_INFO  pieces[LONGEST_SNAKE + 1];
    SHORT       length;
    BOOL        addPiece;
} SNAKE_INFO;

SHORT                       defaultDirection;
FIELD_INFO                  field[SNAKE_COLUMNS][SNAKE_ROWS];
SNAKE_INFO                  snake;

#define snakeHead   head
#define snakeBody   body
#define snakeFood   food

#define SNAKE_HEAD  snake.pieces[0]

const XCHAR                 CrashedStr[] = {'Y','o','u',' ','C','r','a','s','h','e','d','!',0};
const XCHAR                 ShuFullStr[] = {'Y','o','u',' ','a','r','e',' ','f','u','l','l','y',' ','g','r','o','w','n','!',0};
const XCHAR                 InAKnotStr[] = {'Y','o','u',' ','a','r','e',' ','i','n',' ','a',' ','k','n','o','t','!',0};
const XCHAR                 NotFoodStr[] = {'X',0};

/************************************************************************
 Function: void DrawSnake(DWORD tick)
                                                                       
 Overview: This function draws the snake for every determined interval.
           If the tick count does not exceed the interval from the last
           update, it will do nothing.
 		                                                                          
 Input: tick - the counter for the elapsed tick time.
                                                                       
 Output: none
************************************************************************/
void DrawSnake(DWORD tick)
{
    SHORT           i;
    SHORT           j;
    SHORT           oldTail;
    SHORT           scoreLen;
    SHORT           scoreTemp;
    static DWORD    previousTick = 0;

    if((tick - previousTick) > SNAKE_UPDATE_INTERVAL)
    {

        // Move the snake
        SetState(gamesGrid, GRID_DRAW_ITEMS);

        // Check for a wall
        if
        (
            ((SNAKE_HEAD.direction == DIRECTION_UP) && (SNAKE_HEAD.row == 0)) ||
            ((SNAKE_HEAD.direction == DIRECTION_RIGHT) && (SNAKE_HEAD.column == SNAKE_COLUMNS - 1)) ||
            ((SNAKE_HEAD.direction == DIRECTION_DOWN) && (SNAKE_HEAD.row == SNAKE_ROWS - 1)) ||
            ((SNAKE_HEAD.direction == DIRECTION_LEFT) && (SNAKE_HEAD.column == 0))
        )
        {
            pScoreText = &scoreText[7];
            scoreTemp = score;
            scoreLen = 0;
            while(scoreTemp > 0)
            {
                scoreTemp /= (10);
                scoreLen++;
            }

            Int2Str(pScoreText, score, scoreLen);
            pScoreInfo = (XCHAR *)CrashedStr;
            screenState = CREATE_SNAKE_SCORE;
            return;
        }

        score++;

        // Draw the snake on the field.  We record each segment in memory, but we only
        // have to draw the head, the second segment, and possibly the space that the
        // tail just left.
        oldTail = snake.length - 1;
        if(field[snake.pieces[oldTail].column][snake.pieces[oldTail].row].hasBeenEaten)
        {

            // Clear the eaten flag
            field[snake.pieces[oldTail].column][snake.pieces[oldTail].row].hasBeenEaten = FALSE;

            // Add a new piece at the tail.
            snake.pieces[snake.length] = snake.pieces[oldTail];
            GridSetCell
            (
                gamesGrid,
                snake.pieces[snake.length].column,
                snake.pieces[snake.length].row,
                GRIDITEM_IS_BITMAP | GRIDITEM_DRAW,
                (void *) &snakeBody
            );
            snake.length++;
            score += 10;
            if(snake.length == LONGEST_SNAKE)
            {
                score += 1000;
                pScoreText = &scoreText[7];
                scoreTemp = score;
                scoreLen = 0;
                while(scoreTemp > 0)
                {
                    scoreTemp /= (10);
                    scoreLen++;
                }

                Int2Str(pScoreText, score, scoreLen);
                pScoreInfo = (XCHAR *)ShuFullStr;
                screenState = CREATE_SNAKE_SCORE;
                return;
            }
        }
        else
        {

            // Erase the tail.  If there is food or not food, mark it.  Otherwise, clear it.
            if((field[snake.pieces[oldTail].column][snake.pieces[oldTail].row].hasFood) || 
               (field[snake.pieces[oldTail].column][snake.pieces[oldTail].row].notFood))
            {
                GridSetCell
                (
                    gamesGrid,
                    snake.pieces[oldTail].column,
                    snake.pieces[oldTail].row,
                    GRIDITEM_IS_BITMAP | GRIDITEM_DRAW,
                    (void *) &snakeFood
                );
            }
            else
            {
                GridSetCell
                (
                    gamesGrid,
                    snake.pieces[oldTail].column,
                    snake.pieces[oldTail].row,
                    GRIDITEM_IS_BITMAP | GRIDITEM_DRAW,
                    NULL
                );
            }
        }

        for(i = oldTail; i >= 0; i--)
        {
            switch(snake.pieces[i].direction)
            {
                case DIRECTION_UP:      snake.pieces[i].row--; break;
                case DIRECTION_RIGHT:   snake.pieces[i].column++; break;
                case DIRECTION_DOWN:    snake.pieces[i].row++; break;
                case DIRECTION_LEFT:    snake.pieces[i].column--; break;
            }

            if(i == 0)
            {

                // This is the head.  Don't update the direction.  Draw the head.
                GridSetCell
                (
                    gamesGrid,
                    SNAKE_HEAD.column,
                    SNAKE_HEAD.row,
                    GRIDITEM_IS_BITMAP | GRIDITEM_DRAW,
                    (void *) &snakeHead
                );
            }
            else
            {
                if(i == 1)
                {

                    // Change the head to a segment
                    GridSetCell
                    (
                        gamesGrid,
                        SNAKE_HEAD.column,
                        SNAKE_HEAD.row,
                        GRIDITEM_IS_BITMAP | GRIDITEM_DRAW,
                        (void *) &snakeBody
                    );
                }

                // Update the next direction for the piece.
                snake.pieces[i].direction = snake.pieces[i - 1].direction;
            }
        }

        // Check for the snake hitting itself
        for(i = 0; i < snake.length - 2; i++)
        {
            for(j = i + 1; j < snake.length - 1; j++)
            {
                if((snake.pieces[i].column == snake.pieces[j].column) && (snake.pieces[i].row == snake.pieces[j].row))
                {
                    pScoreText = &scoreText[7];
                    scoreTemp = score;
                    scoreLen = 0;
                    while(scoreTemp > 0)
                    {
                        scoreTemp /= (10);
                        scoreLen++;
                    }

                    Int2Str(pScoreText, score, scoreLen);
                    pScoreInfo = (XCHAR *)InAKnotStr;
                    screenState = CREATE_SNAKE_SCORE;
                    return;
                }
            }
        }

        // See if the snake is eating food.
        if	((field[SNAKE_HEAD.column][SNAKE_HEAD.row].hasFood == TRUE) || 
        	 (field[SNAKE_HEAD.column][SNAKE_HEAD.row].notFood == TRUE))
        {
	        // if eating poison subtract score
	        if (field[SNAKE_HEAD.column][SNAKE_HEAD.row].notFood == TRUE)
	        {
	            field[SNAKE_HEAD.column][SNAKE_HEAD.row].notFood = FALSE;
	        	score -= 10;
	        }	
			else
			{
            	field[SNAKE_HEAD.column][SNAKE_HEAD.row].hasFood = FALSE;
            	field[SNAKE_HEAD.column][SNAKE_HEAD.row].hasBeenEaten = TRUE;
   			}         

            // Put another piece of food on.  We'll let it be anywhere that doesn't
            // already have food.
            do
            {
                i = Random(SNAKE_COLUMNS);
                j = Random(SNAKE_ROWS);
            } while(field[i][j].hasFood);

            // Mark and draw the food or not food.  Don't draw it if the snake is there!
            if ((i + j)%3 != 0)
            {
            	field[i][j].hasFood = TRUE;
            	if(!GridGetCell(gamesGrid, i, j, (WORD *) &oldTail))
            	{
                	GridSetCell(gamesGrid, i, j, GRIDITEM_IS_BITMAP | GRIDITEM_DRAW, (void *) &snakeFood);
            	}
            }
            else
            {
            	field[i][j].notFood = TRUE;
            	if(!GridGetCell(gamesGrid, i, j, (WORD *) &oldTail))
            	{
                	GridSetCell(gamesGrid, i, j, GRIDITEM_IS_TEXT | GRIDITEM_DRAW, (void *) &NotFoodStr);
            	}
	        } 	
        }

        previousTick = tick;
    }
}

/*******************************************************************************
Function:       WORD ProcessMessageSnake( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

Precondition:   Call ShowScreenSnake() prior to using this function to display
                the correct screen.

Overview:       This function processes the messages for the Snake game, to
                change the snake's direction.  We will be nice to the player -
                if he selects the direction opposite to what he is traveling,
                we will ignore the message rather than instantly tie the snake
                into a knot.

Input:          WORD translatedMsg  The translated control-level message
                OBJ_HEADER* pObj    Object to which the message applies
                GOL_MSG* pMsg       The original system message information

Output:         None

Return Values:  0   Do not call the default message handler.
                1   Call the default message handler.

Side Effects:   None

Remarks:        None
*******************************************************************************/
WORD ProcessMessageSnake(WORD translatedMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    WORD    buttonID;

    buttonID = GetObjID(pObj);

    if(screenState == DISPLAY_SNAKE_SCORE)
    {
        if(translatedMsg == BTN_MSG_RELEASED)
        {

            // do not process if user moved the touch away from the button
            // returning 1 wil update the button
            if(pMsg->uiEvent == EVENT_MOVE)
                return (1);
            switch(buttonID)
            {
                case ID_BUTTON_A:   screenState = CREATE_DEMOSELECTION; break;
                case ID_BUTTON_D:   screenState = CREATE_SNAKE; break;
            }
        }
    }
    else
    {
        if(translatedMsg == BTN_MSG_PRESSED)
        {
            switch(buttonID)
            {
                case ID_BUTTON_C:
                    if(SNAKE_HEAD.direction != DIRECTION_DOWN)
                    {
                        SNAKE_HEAD.direction = DIRECTION_UP;
                    }

                    break;

                case ID_BUTTON_D:
                    if(SNAKE_HEAD.direction != DIRECTION_UP)
                    {
                        SNAKE_HEAD.direction = DIRECTION_DOWN;
                    }

                    break;

                case ID_BUTTON_A:
                    if(SNAKE_HEAD.direction != DIRECTION_RIGHT)
                    {
                        SNAKE_HEAD.direction = DIRECTION_LEFT;
                    }

                    break;

                case ID_BUTTON_B:
                    if(SNAKE_HEAD.direction != DIRECTION_LEFT)
                    {
                        SNAKE_HEAD.direction = DIRECTION_RIGHT;
                    }

                    break;
            }
        }
    }

    return (1);
}

/************************************************************************
 Function: void ShowScreenSnake(void)
                                                                       
 Overview: This function creates the game screen.
 		                                                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
void ShowScreenSnake(void)
{
    SHORT   i;
    SHORT   j;
    SHORT   k;

    GOLFree();
    SetColor(BLACK);                            // set color to BLACK
    ClearDevice();                              // set screen to all BLACK
    InitializeRandomNumberGenerator();

    balls[0] = (void *) &ballCircle;
    balls[1] = (void *) &ballSquare;
    balls[2] = (void *) &ballCross;
    balls[3] = (void *) &ballTriangle;
    balls[MARKING_BALL] = (void *) &ballMarking;
    balls[BLANK_BALL] = (void *)NULL;

    gamesGrid = GridCreate
        (
            ID_GAMES_GRID,                      // ID
            SNAKE_GRID_LEFT,                    // left
            SNAKE_GRID_TOP,                     // top
            SNAKE_GRID_RIGHT,                   // right
            SNAKE_GRID_BOTTOM,                  // bottom
            GRID_DRAW_ALL | GRID_SHOW_LINES,    // state
            SNAKE_COLUMNS,                      // numColumns
            SNAKE_ROWS,                         // numRows
            SEGMENT_SIZE,                       // cellWidth
            SEGMENT_SIZE,                       // cellHeight
            gridScheme
        );                              // scheme )
    if(!gamesGrid)
    {
        screenState = CREATE_DEMOSELECTION;
        return;
    }

    //gamesGrid->pGolScheme->CommonBkColor = 0xD7FF;
    CreateCtrlButtons(LeftArrowStr, RightArrowStr, UpArrowStr, DownArrowStr);

    // Initialize the field and the grid.
    for(i = 0; i < SNAKE_COLUMNS; i++)
    {
        for(j = 0; j < SNAKE_ROWS; j++)
        {
            field[i][j].hasFood = FALSE;
            field[i][j].hasBeenEaten = FALSE;
            GridSetCell(gamesGrid, i, j, GRIDITEM_IS_BITMAP, NULL);
        }
    }

    // Initialize the snake.
    SNAKE_HEAD.column = 0;
    SNAKE_HEAD.row = 0;
    SNAKE_HEAD.direction = DIRECTION_RIGHT;
    snake.length = 1;

    // Place random food
    for(k = 0; k < FIELD_PIECES; k++)
    {

        // Find coordinates for food
        i = Random(SNAKE_COLUMNS - 1);
        j = Random(SNAKE_ROWS - 1);

        if ((i + j)%3 != 0)
        {
        	// Mark and draw the food
        	field[i][j].hasFood = TRUE;
        	GridSetCell(gamesGrid, i, j, GRIDITEM_IS_BITMAP, (void *) &snakeFood);
        }
        else
        {
        	// Mark and draw the food
        	field[i][j].notFood = TRUE;
	        GridSetCell(gamesGrid, i, j, GRIDITEM_IS_TEXT, (void *) &NotFoodStr);
        }
    }

    // Draw the snake on the field
    GridSetCell(gamesGrid, SNAKE_HEAD.column, SNAKE_HEAD.row, GRIDITEM_IS_BITMAP, (void *) &snakeHead);
    score = 0;

    // initialize the score holder
    i = 7;
    while(i <= 19)
    {
        scoreText[i] = 0;
        i++;
    }
}

#define TEXT_SHADOW_OFFSET  1
#define TEXT_COLOR_BG       (RGB565CONVERT(0x4C, 0x8E, 0xFF))
#define TEXT_COLOR_FG       BRIGHTRED   //(RGB565CONVERT(0xFF, 0xBB, 0x4C))

/************************************************************************
 Function: void ShowScreenScore(void)
                                                                       
 Overview: This function shows the score after the game is terminated.
 		                                                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
void ShowScreenScore(void)
{
    XCHAR   gameOverStr[] = {'G','a','m','e',' ','O','v','e','r','!',0};
    WORD    textWidth, textHeight;

    GOLFree();

    // change the meaning of the buttons and show the score
    CreateCtrlButtons(ExitStr, NULL, NULL, NewGameStr);

    textWidth = GetTextWidth(gameOverStr, (void *) &GAMEBIGFONT);
    textHeight = GetTextHeight((void *) &GAMEBIGFONT);

    SetColor(TEXT_COLOR_BG);
    WAIT_UNTIL_FINISH(OutTextXY((GetMaxX() - textWidth) >> 1, ((GetMaxY() - CTRLBTN_HEIGHT) >> 1) - textHeight, gameOverStr));
    SetColor(TEXT_COLOR_FG);
    WAIT_UNTIL_FINISH
    (
        OutTextXY
            (
                ((GetMaxX() - textWidth) >> 1) + TEXT_SHADOW_OFFSET,
                (((GetMaxY() - CTRLBTN_HEIGHT) >> 1) - textHeight) + TEXT_SHADOW_OFFSET,
                gameOverStr
            )
    );

    textWidth = GetTextWidth(pScoreInfo, (void *) &GAMESMALLFONT);

    SetColor(TEXT_COLOR_BG);
    WAIT_UNTIL_FINISH(OutTextXY((GetMaxX() - textWidth) >> 1, (GetMaxY() - CTRLBTN_HEIGHT) >> 1, pScoreInfo));
    SetColor(TEXT_COLOR_FG);
    WAIT_UNTIL_FINISH
    (
        OutTextXY
            (
                ((GetMaxX() - textWidth) >> 1) + TEXT_SHADOW_OFFSET,
                ((GetMaxY() - CTRLBTN_HEIGHT) >> 1) + TEXT_SHADOW_OFFSET,
                pScoreInfo
            )
    );

    textHeight = GetTextHeight((void *) &GAMESMALLFONT);
    textWidth = GetTextWidth(scoreText, (void *) &GAMESMALLFONT);

    SetColor(TEXT_COLOR_BG);
    WAIT_UNTIL_FINISH(OutTextXY((GetMaxX() - textWidth) >> 1, ((GetMaxY() - CTRLBTN_HEIGHT) >> 1) + textHeight, scoreText));
    SetColor(TEXT_COLOR_FG);
    WAIT_UNTIL_FINISH
    (
        OutTextXY
            (
                ((GetMaxX() - textWidth) >> 1) + TEXT_SHADOW_OFFSET,
                (((GetMaxY() - CTRLBTN_HEIGHT) >> 1) + textHeight) + TEXT_SHADOW_OFFSET,
                scoreText
            )
    );
}

/************************************************************************
 Function: void InitGamesStyleScheme(GOL_SCHEME *pScheme)
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void InitGamesStyleScheme(GOL_SCHEME *pScheme)
{
    gridScheme->CommonBkColor = WHITE;  //RGB565CONVERT(0xFD, 0xEE, 0xCA);
}
