/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		Games.h
* Dependencies:    		Header (.h) files if applicable, see below
* Processor:       		PIC24HJ128GP504
* Compiler:        		MPLAB® C30 v3.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
************************************************************************/

/************************************************************************
 Header Includes													
 ************************************************************************/
#include "Main.h"
#include "Speaker.h"

/************************************************************************
 Constants BOMBER JET GAME	 												
 ************************************************************************/
#define ID_GAMES_JET	            ID_BOTTOM_LEFT
#define ID_GAMES_HOME               ID_TOP_LEFT
#define ID_GAMES_SNAKE              ID_BOTTOM_RIGHT

#define ID_GAMES_GRID               50
#define ID_SCORE                    51
#define ID_SCORE_INFO               52
#define ID_QUIT                     53

#define RANDOM_SEED                 3

#define ID_SHIP                     50
#define ID_BOMBER                   51
#define ID_EM_WAVE					52

#define SHIP_CENTER_X               (GetMaxX()/2)
#define SHIP_CENTER_Y               (GetMaxY()/2)
#define SHIP_RADIUS                 5

#define ASTEROID_SIZE_OFFSET        2
#define ASTEROID_SIZE_MULTIPLIER    3
#define ASTEROID_SIZES              3
#define ASTEROID_UPDATE_SPEEDS      3
#define ASTEROID_UPDATE_MULTIPLIER  (1000 / MILLISECONDS_PER_TICK)
#define ASTERIOD_UPDATE_OFFSET      (250 / MILLISECONDS_PER_TICK)
#define MISSILE_SIZE                2
#define NUM_ACTIVE_ASTEROIDS        2
#define NUM_ACTIVE_ALIENS	        2
#define UPDATE_MISSILE              4
#define UPDATE_MISSILE_ALIEN        8
#define QUADRANT_POSITIONS 			6
#define BJ_SCORE_MAX				1000

/************************************************************************
 Constants SNAKE GAME													
 ************************************************************************/
#define SCORE_BOTTOM                (SCORE_TOP + TEXT_HEIGHT)
#define SCORE_LEFT                  0
#define SCORE_RIGHT                 GetMaxX()
#define SCORE_TOP                   0
#define TEXT_HEIGHT                 10 

#define FIELD_PIECES                    SNAKE_COLUMNS
#define LONGEST_SNAKE                   25

#define DIRECTION_NONE                  0
#define DIRECTION_UP                    1
#define DIRECTION_RIGHT                 2
#define DIRECTION_DOWN                  3
#define DIRECTION_LEFT                  4

#define ID_SNAKE_UP                     60
#define ID_SNAKE_RIGHT                  61
#define ID_SNAKE_DOWN                   62
#define ID_SNAKE_LEFT                   63

#define SEGMENT_SIZE                    8
#define SNAKE_COLUMNS                   (GRID_MAX_COLUMNS( SNAKE_GRID_RIGHT_MAX - SNAKE_GRID_LEFT, SEGMENT_SIZE )) //((SNAKE_GRID_RIGHT-SNAKE_GRID_LEFT-1) / SEGMENT_SIZE - 1)
#define SNAKE_GRID_BOTTOM               (GRID_HEIGHT( SNAKE_ROWS, SEGMENT_SIZE ) - 1)
#define SNAKE_GRID_BOTTOM_MAX           (GetMaxY())
#define SNAKE_GRID_LEFT                 0
#define SNAKE_GRID_RIGHT                (GRID_WIDTH( SNAKE_COLUMNS, SEGMENT_SIZE ) - 1)
#define SNAKE_GRID_RIGHT_MAX            (GetMaxX())
#define SNAKE_GRID_TOP                  0
#define SNAKE_ROWS                      (GRID_MAX_ROWS( SNAKE_GRID_BOTTOM_MAX - SNAKE_GRID_TOP, SEGMENT_SIZE ))  //((SNAKE_GRID_BOTTOM - SNAKE_GRID_TOP - 1) / SEGMENT_SIZE - 1)

#define SNAKE_UPDATE_INTERVAL           (4000 / MILLISECONDS_PER_TICK)

/************************************************************************
 Data Structures BOMBER JET GAME													
 ************************************************************************/
typedef struct
{
    WORD    active      : 1;
    WORD    terminate   : 1;
} STATUS;

typedef struct
{
    STATUS  status;
    BYTE    speed;
    BYTE    speedIndex;
    BYTE    updateInterval;
    BYTE    size;
    BYTE    sizeIndex;
    SHORT   initialX;
    SHORT   initialY;
    SHORT   positionX;
    SHORT   positionY;
    SHORT   finalX;
    SHORT   finalY;
    SHORT   step;
} ASTEROID_INFO;

typedef struct
{
    STATUS  status;
    BYTE    angle;
    BYTE    updateInterval;
    SHORT   positionX;
    SHORT   positionY;
    SHORT   deltaX;
    SHORT   deltaY;
    SHORT   step;
} MISSILE_INFO;

/************************************************************************
 Data Structures BOMBER JET GAME													
 ************************************************************************/
// This structure describes one piece of the snake.
typedef struct _PIECE_INFO
{
    SHORT   column;
    SHORT   row;
    SHORT   direction;
} PIECE_INFO;

// This structure describes one cell of the field.
typedef struct _FIELD_INFO
{
    BOOL    hasFood;
    BOOL    hasBeenEaten;
} FIELD_INFO;

// This structure describes the snake.
typedef struct _SNAKE_INFO
{
    PIECE_INFO  pieces[LONGEST_SNAKE+1];
    SHORT       length;
    BOOL        addPiece;
} SNAKE_INFO;

/************************************************************************
 Function Prototypes													
 ************************************************************************/
void InitializeRandomNumberGenerator(void);
SHORT Random(SHORT max);
WORD ProcessMessageGames(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
void ShowScreenGames(void);
void ShowScreenScore(void);
void TranslateMessageScore(GOL_MSG *pMsg);
WORD ProcessMessageScore(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);

/************************************************************************
 Function Prototypes BOMBER JET GAME													
 ************************************************************************/
void DrawJet(void);
SHORT FindNextPosition(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT step, SHORT *x, SHORT *y);
WORD ProcessMessageJet(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
void ShowScreenJet(void);
void TranslateMessageJet(GOL_MSG *pMsg);

/************************************************************************
 Function Prototypes SNAKE GAME													
 ************************************************************************/
void DrawSnake(void);
WORD ProcessMessageSnake(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
void ShowScreenSnake(void);
void TranslateMessageSnake(GOL_MSG *pMsg);


