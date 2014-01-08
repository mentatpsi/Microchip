/******************************************************************************

    Interactive Graphical Demonstrations

    (Games!)

Description:
    This file contains the interactive graphical demonstrations (games!) that
    are in the PIC24F Starter Kit demonstration firmware.  These games
    demonstrate the processing capabilities of the PIC24F.

    ShuBox
    Shu is a snake.  Help him find the mice in his feeding box.  With every
    mouse eaten, Shu gets bigger.  Don't let him crash into a wall, or tie
    himself into a knot!

    Shapelet
    Try to remove all the objects from the field.  Objects can be removed only
    if the object has at least one adjacent identical object.  Use the up, down,
    left, and right arrows to move the focused object.  Touch the center
    touchpad to highlight what objects will be removed by selecting the focused
    object.  Touch the center touchpad on a highlighted object to remove all
    highlighted objects.  The game ends when all objects are removed or when
    no more objects can be removed.  Or you can abort the game by selecting a
    blank cell.

    Blaster
    Blast the asteroids before they hit you.  Use the left and right arrows
    to spin your ship, and the center button to fire.  Asteroids come in three
    shapes and three speeds.  The smaller, faster, and more distant the
    asteroid, the more points you score for hitting it.  The game ends when
    you get hit.  Or you can abort the game by touching the up arrow touchpad.

Summary:
    This file contains the interactive graphical demonstrations (games!) that
    are in the PIC24F Starter Kit demonstration firmware.

Remarks:
    Blaster requires some modification of the Round Dial control to handle
    keyboard input.  We need to define the amount of rotation for each
    touchpad press, and be able to obtain the current rotational position.

    For Blaster, we use simple circles to represent the bullets and asteroids.
    This makes the impact calculations more consistent.  The code can be
    modified to use images instead to provide a more custom look and feel.

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
#include "PIC24F Starter Kit.h"

//******************************************************************************
//******************************************************************************
// General Games Support
//******************************************************************************
//******************************************************************************

//******************************************************************************
// Constants
//******************************************************************************

#define ID_GAMES_SNAKE              ID_TOP_LEFT
#define ID_GAMES_SHAPELET           ID_BOTTOM_LEFT
#define ID_GAMES_HOME               ID_TOP_RIGHT
#define ID_GAMES_BLASTER            ID_BOTTOM_RIGHT

#define ID_GAMES_GRID               50
#define ID_SCORE                    51
#define ID_SCORE_INFO               52
#define ID_QUIT                     53

// This macro is used to initialize the random number generator.  If you want
// a predictable sequence, set it to a constant value.  If you want something
// more unpredictable, set it to a running timer.
#define RANDOM_SEED                 3


//******************************************************************************
// Global Variables
//******************************************************************************

GRID *                  gamesGrid;
STATICTEXT *            pScore;
SHORT                   score;
XCHAR                   scoreText[20];
XCHAR                   scoreInfo[26];


//******************************************************************************
// Internal Prototypes
//******************************************************************************

void    InitializeRandomNumberGenerator( void );
SHORT   Random( SHORT max );


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

void InitializeRandomNumberGenerator( void )
{
    static BOOL initialized;

    if (!initialized)
    {
        srand( RANDOM_SEED );
        initialized = TRUE;
    }
}


/****************************************************************************
  Function:
    WORD ProcessMessageGames( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

  Description:
    This function processes the messages for the game selection screen.

  Precondition:
    Call ShowScreenGames() prior to using this function to display the
    correct screen.

  Parameters:
    WORD        translatedMsg   - The translated control-level message
    OBJ_HEADER* pObj            - Object to which the message applies
    GOL_MSG*    pMsg            - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    The global screenState is updated to proceed to the user's selection.
  ***************************************************************************/

WORD ProcessMessageGames( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    WORD    buttonID;

    buttonID = GetObjID(pObj);

    switch (buttonID)
    {
        case ID_GAMES_SNAKE:
            if (translatedMsg == BTN_MSG_RELEASED)
            {
                screenState = SCREEN_DISPLAY_SNAKE;
            }
            break;

        case ID_GAMES_SHAPELET:
            if (translatedMsg == BTN_MSG_RELEASED)
            {
                screenState = SCREEN_DISPLAY_SHAPELET;
            }
            break;

        case ID_GAMES_HOME:
            if (translatedMsg == BTN_MSG_RELEASED)
            {
                screenState = SCREEN_DISPLAY_MAIN;
            }
            break;

        case ID_GAMES_BLASTER:
            if (translatedMsg == BTN_MSG_RELEASED)
            {
                screenState = SCREEN_DISPLAY_BLASTER;
            }
            break;

    }

    // Keep the current unimplemented button "pressed"
    SetState( (BUTTON*)pObj, BTN_PRESSED | BTN_DRAW );

    return 0;   // We do not need to call the default handler.
}


/****************************************************************************
  Function:
    WORD ProcessMessageScore( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

  Description:
    This function processes the messages for the game score display screen.
    When the user touches any button after that time, we return to the main
    menu.

  Precondition:
    Call ShowScreenScore() prior to using this function to display the
    correct screen.

  Parameters:
    WORD        translatedMsg   - The translated control-level message
    OBJ_HEADER* pObj            - Object to which the message applies
    GOL_MSG*    pMsg            - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    The global screenState is updated to send the user back to the main
    screen.
  ***************************************************************************/

WORD ProcessMessageScore( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    screenState = SCREEN_DISPLAY_MAIN;

    return 0;
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

SHORT Random( SHORT max )
{
    SHORT num = 0;

    num = rand() & 0xFF;    // We only need a byte...
    while (num >= (max+1))
    {
        num -= (max+1);
    }

    return num;
}


/****************************************************************************
  Function:
    void ShowScreenGames( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the screen that allows the user to select a game.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced.  Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/


void ShowScreenGames( void )
{
    strcpy( pageInformation.title, "Games" );

    strcpy( pageInformation.buttonTextTopLeft,      "Shu Box" );
    strcpy( pageInformation.buttonTextBottomLeft,   "Shapelet" );
    strcpy( pageInformation.buttonTextTopRight,     "Home" );
    strcpy( pageInformation.buttonTextBottomRight,  "Blaster" );
	strcpy( pageInformation.buttonTextThirdrow, 	"" );

    pageInformation.buttonBitmapTopLeft        = NULL;
    pageInformation.buttonBitmapBottomLeft     = NULL;
    pageInformation.buttonBitmapTopRight       = NULL;
    pageInformation.buttonBitmapBottomRight    = NULL;

    pageInformation.instructions[0] = 0;

    ShowScreenGeneric();
}


/****************************************************************************
  Function:
    void ShowScreenScore( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the screen that shows the score of the completed game.

  Precondition:
    The following global variables must be set prior to calling this function:
                    * pageInformation.title
                    * score
                    * scoreInfo

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced.  Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenScore( void )
{
    InitializeScreen();

    sprintf( scoreText, "Score: %d", score );

    StCreate(
                ID_SCORE,                       // ID
                0,                              // left
                20,                             // top
                GetMaxX(),                      // right
                20+GetTextHeight( (void *)&FONTDEFAULT ),  // bottom
                ST_DRAW,
                scoreText,
                NULL );

    StCreate(
                ID_SCORE_INFO,                  // ID
                0,                              // left
                32,                             // top
                GetMaxX(),                      // right
                32+GetTextHeight( (void *)&FONTDEFAULT ),  // bottom
                ST_DRAW,
                scoreInfo,
                NULL );

    strcpy( pageInformation.instructions, "Touch a key to continue..." );
    StCreate(
                ID_INSTRUCTIONS,
                0, 47, 127, 47+GetTextHeight( (void *)&FONTDEFAULT )-1,
                ST_DRAW,
                pageInformation.instructions,
                NULL );

    // Create a hidden control to receive button messages.  Otherwise, no
    // messages will be sent through the system.
    BtnCreate(
                        ID_QUIT,
                        GetMaxX()-4, 0, GetMaxX()-2, 2, 0,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );
}


/****************************************************************************
  Function:
    void TranslateMessageScore( GOL_MSG *pMsg )

  Description:
    This routine translates the touchpad message for the score display screen.

  Precondition:
    None

  Parameters:
    GOL_MSG *pMsg   - The current system message

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/

void TranslateMessageScore( GOL_MSG *pMsg )
{
    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD))
    {
        pMsg->param1 = ID_QUIT;
        pMsg->param2 = SCAN_CR_PRESSED;
    }
}

//******************************************************************************
//******************************************************************************
// Blaster
//******************************************************************************
//******************************************************************************

//******************************************************************************
// Constants
//******************************************************************************

#define ID_SHIP                     50
#define ID_BLASTER                  51

#define SHIP_CENTER_X               (GetMaxX()/2)
#define SHIP_CENTER_Y               (GetMaxY()/2)
#define SHIP_RADIUS                 5

#define ASTEROID_SIZE_OFFSET        2
#define ASTEROID_SIZE_MULTIPLIER    3
#define ASTEROID_SIZES              3

#define ASTEROID_UPDATE_SPEEDS      3
#define ASTEROID_UPDATE_MULTIPLIER  (500 / MILLISECONDS_PER_TICK)
#define ASTERIOD_UPDATE_OFFSET      (250 / MILLISECONDS_PER_TICK)

#define BLASTER_MAX                 23  // 15 degrees per tick
#define BLASTER_RESOLUTION          1
#define BULLET_SIZE                 2
#define NUM_ACTIVE_ASTEROIDS        3
#define UPDATE_BULLET               1


//******************************************************************************
// Data Structures
//******************************************************************************

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
} BULLET_INFO;


//******************************************************************************
// Game Global Variables
//******************************************************************************

ASTEROID_INFO       asteroid[NUM_ACTIVE_ASTEROIDS];
BULLET_INFO         bullet;
ROUNDDIAL *         pShip;
SHORT               scoreSize[ASTEROID_SIZES]          = { 3, 2, 1 };
SHORT               scoreSpeed[ASTEROID_UPDATE_SPEEDS] = { 3, 2, 1 };


//******************************************************************************
// Local Function Prototypes
//******************************************************************************


SHORT FindNextPosition( SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT step, SHORT *x, SHORT *y );

/****************************************************************************
  Function:
    void DrawBlaster( void )

  Description:
    This function updates the screen during the Blaster game.  We update on
    one tick intervals.  First, we check to see if our bullet hit any
    asteroids.  If so, we remove the asteroid.  Then we see if any asteroids
    hit each other.  If so, both asteroids are removed.  Then we see if an
    asteroid hit our ship.  If so, we're dead, and the game is over.  If our
    ship is still intact, move the bullet if one has been shot, and move any
    asteroids that are present.  If we need to add asteroids to the field,
    do that, too.

  Precondition:
    The variables bullet and asteroid must be valid.

  Parameters:
    None

  Returns:
    None

  Remarks:
    To slow the game, either change the update test (previousTick != tick) or
    the update intervals (UPDATE_BULLET for the bullet,
    ASTEROID_UPDATE_MULTIPLIER and ASTERIOD_UPDATE_OFFSET for the asteroids.)
  ***************************************************************************/

void DrawBlaster( void )
{
    if (previousTick != tick)
    {
        int     i;
        int     j;

        //----------------------------------------------------------------------
        // Check for bullet hits

        if (bullet.status.active)
        {
            for (i=0; i<NUM_ACTIVE_ASTEROIDS; i++)
            {
                if (asteroid[i].status.active)
                {
                    if ((abs( asteroid[i].positionX - bullet.positionX) < (asteroid[i].size + BULLET_SIZE)) &&
                        (abs( asteroid[i].positionY - bullet.positionY) < (asteroid[i].size + BULLET_SIZE)))
                    {
                        // Hit!  Adjust the score, and terminate the asteroid and bullet.  The farther
                        // the bullet travelled, the bigger our score.
                        score +=    scoreSpeed[asteroid[i].speedIndex] +
                                    scoreSize[asteroid[i].sizeIndex] +
                                    bullet.step;
                        asteroid[i].status.terminate    = TRUE;
                        bullet.status.terminate         = TRUE;
                    }
                }
            }
        }

        //----------------------------------------------------------------------
        // Check for asteroid collisions

        for (i=0; i<NUM_ACTIVE_ASTEROIDS-1; i++)
        {
            if (asteroid[i].status.active)
            {
                for (j=i+1; j<NUM_ACTIVE_ASTEROIDS; j++)
                {
                    if (asteroid[j].status.active)
                    {
                        if ((abs( asteroid[i].positionX - asteroid[j].positionX) < (asteroid[i].size + asteroid[j].size)) &&
                            (abs( asteroid[i].positionY - asteroid[j].positionY) < (asteroid[i].size + asteroid[j].size)))
                        {
                            // Hit!  Adjust the score, and terminate the asteroids.  Since the
                            // asteroids collided on their own, we only give half the usual score,
                            // and no bonus for distance.
                            score += (scoreSpeed[asteroid[i].speedIndex] + scoreSize[asteroid[i].sizeIndex]) / 2;
                            score += (scoreSpeed[asteroid[j].speedIndex] + scoreSize[asteroid[j].sizeIndex]) / 2;
                            asteroid[i].status.terminate    = TRUE;
                            asteroid[j].status.terminate    = TRUE;
                        }
                    }
                }
            }
        }

        //----------------------------------------------------------------------
        // Check for ship impact

        for (i=0; i<NUM_ACTIVE_ASTEROIDS; i++)
        {
            if (asteroid[i].status.active)
            {
                if ((abs( asteroid[i].positionX - SHIP_CENTER_X) < (asteroid[i].size + SHIP_RADIUS)) &&
                    (abs( asteroid[i].positionY - SHIP_CENTER_Y) < (asteroid[i].size + SHIP_RADIUS)))
                {
                    // Hit!  We're dead.  Adjust the score, and set the screen state to
                    // end the game.
                    strcpy( pageInformation.title, "Blaster" );
                    strcpy( scoreInfo, "You have been hit!" );
                    screenState = SCREEN_DISPLAY_BLASTER_SCORE;
                }
            }
        }

        //----------------------------------------------------------------------
        // Move the bullet if it is present.

        if (bullet.status.active)
        {
            if (--bullet.updateInterval == 0)
            {
                // Reset the interval
                bullet.updateInterval = UPDATE_BULLET;

                // Erase the old bullet
                SetColor( pShip->hdr.pGolScheme->CommonBkColor );
                Bar( bullet.positionX-BULLET_SIZE, bullet.positionY-BULLET_SIZE, bullet.positionX+BULLET_SIZE, bullet.positionY+BULLET_SIZE );

                if (bullet.status.terminate)
                {
                    bullet.status.active = FALSE;
                }
                else
                {
                    // Move the bullet.  We are moving from (SHIP_CENTER_X, SHIP_CENTER_Y) to (SHIP_CENTER_X+deltaX, SHIP_CENTER_Y+deltaY)
                    bullet.step = FindNextPosition(
                                        SHIP_CENTER_X, SHIP_CENTER_Y,
                                        SHIP_CENTER_X + bullet.deltaX, SHIP_CENTER_Y + bullet.deltaY,
                                        bullet.step,
                                        &bullet.positionX,
                                        &bullet.positionY );

                    // If it is still on the screen, draw the new bullet.  Otherwise, the pilot can fire again.
                    if ((bullet.positionX < BULLET_SIZE) || ( bullet.positionX > (GetMaxX()-BULLET_SIZE)) ||
                        (bullet.positionY < BULLET_SIZE) || ( bullet.positionY > (GetMaxY()-BULLET_SIZE)))
                    {
                        bullet.status.active = FALSE;
                    }
                    else
                    {
                        //PutImage( bullet.positionX, bullet.positionY, &bullet, IMAGE_NORMAL );
                        SetColor( pShip->hdr.pGolScheme->TextColor0 );
                        Circle( bullet.positionX, bullet.positionY, BULLET_SIZE );

                        // Redraw the ship, in case our bullet messed it up
                        SetState( pShip, RDIA_DRAW );
                    }
                }
            }
        }

        //----------------------------------------------------------------------
        // Move the asteriods.  Create new ones if needed.

        for (i=0; i<NUM_ACTIVE_ASTEROIDS; i++)
        {
            if (!asteroid[i].status.active)
            {
                SHORT   angle;
                SHORT   startAxis;

                // Create a new asteroid
                asteroid[i].speedIndex          = Random( ASTEROID_UPDATE_SPEEDS-1 );
                asteroid[i].speed               = asteroid[i].speedIndex * ASTEROID_UPDATE_MULTIPLIER + ASTERIOD_UPDATE_OFFSET;
                asteroid[i].sizeIndex           = Random( ASTEROID_SIZES-1 );
                asteroid[i].size                = asteroid[i].sizeIndex * ASTEROID_SIZE_MULTIPLIER + ASTEROID_SIZE_OFFSET;

                asteroid[i].updateInterval      = asteroid[i].speed;

                angle                           = Random( (RDIA_QUADRANT_POSITIONS*2)-1 );
                startAxis                       = Random( 4-1 );
                switch( startAxis )
                {
                    case 0:
                        // We can use quadrant 0 and 3 angles.
                        angle -= RDIA_QUADRANT_POSITIONS;
                        if (angle < 0) angle += RDIA_QUADRANT_POSITIONS*4;
                        asteroid[i].initialX    = asteroid[i].size;
                        asteroid[i].initialY    = Random( GetMaxY()-1 - 2*asteroid[i].size ) + asteroid[i].size;
                        asteroid[i].finalX      = 2*(asteroid[i].initialX + RdiaCosine( angle ));
                        asteroid[i].finalY      = 2*(asteroid[i].initialY + RdiaSine( angle ));
                        break;
                    case 1:
                        // We can use quadrant 1 and 2 angles.
                        angle += RDIA_QUADRANT_POSITIONS;
                        asteroid[i].initialX    = GetMaxX() - asteroid[i].size;
                        asteroid[i].initialY    = Random( GetMaxY()-1 - 2*asteroid[i].size ) + asteroid[i].size;
                        asteroid[i].finalX      = 2*(asteroid[i].initialX + RdiaCosine( angle ));
                        asteroid[i].finalY      = 2*(asteroid[i].initialY + RdiaSine( angle ));
                        break;
                    case 2:
                        // We can use quadrant 0 and 1 angles.
                        // angle needs no adjustment
                        asteroid[i].initialX    = Random( GetMaxX()-1 - 2*asteroid[i].size ) + asteroid[i].size;
                        asteroid[i].initialY    = asteroid[i].size;
                        asteroid[i].finalX      = 2*(asteroid[i].initialX + RdiaCosine( angle ));
                        asteroid[i].finalY      = 2*(asteroid[i].initialY + RdiaSine( angle ));
                        break;
                    case 3:
                        // We can use quadrant 2 and 3 angles.
                        angle += RDIA_QUADRANT_POSITIONS*2;
                        asteroid[i].initialX    = Random( GetMaxX()-1 - 2*asteroid[i].size ) + asteroid[i].size;
                        asteroid[i].initialY    = GetMaxY() - asteroid[i].size;
                        asteroid[i].finalX      = 2*(asteroid[i].initialX + RdiaCosine( angle ));
                        asteroid[i].finalY      = 2*(asteroid[i].initialY + RdiaSine( angle ));
                        break;
                }

                asteroid[i].positionX           = asteroid[i].initialX;
                asteroid[i].positionY           = asteroid[i].initialY;

                asteroid[i].step                = 0;
                asteroid[i].status.active       = TRUE;
                asteroid[i].status.terminate    = FALSE;
            }
            else
            {
                if (--asteroid[i].updateInterval == 0)
                {
                    // Reset the interval
                    asteroid[i].updateInterval = asteroid[i].speed;

                    // Erase the old bullet
                    SetColor( pShip->hdr.pGolScheme->CommonBkColor );
                    Bar( asteroid[i].positionX-asteroid[i].size, asteroid[i].positionY-asteroid[i].size, asteroid[i].positionX+asteroid[i].size, asteroid[i].positionY+asteroid[i].size );

                    if (asteroid[i].status.terminate)
                    {
                        asteroid[i].status.active = FALSE;
                    }
                    else
                    {
                        // Move the bullet.  We are moving from (SHIP_CENTER_X, SHIP_CENTER_Y) to (SHIP_CENTER_X+deltaX, SHIP_CENTER_Y+deltaY)
                        asteroid[i].step = FindNextPosition(
                                            asteroid[i].initialX, asteroid[i].initialY,
                                            asteroid[i].finalX, asteroid[i].finalY,
                                            asteroid[i].step,
                                            &asteroid[i].positionX,
                                            &asteroid[i].positionY );

                        // If it is still on the screen, draw the new bullet.  Otherwise, the pilot can fire again.
                        if ((asteroid[i].positionX < asteroid[i].size) || ( asteroid[i].positionX > (GetMaxX()-asteroid[i].size)) ||
                            (asteroid[i].positionY < asteroid[i].size) || ( asteroid[i].positionY > (GetMaxY()-asteroid[i].size)))
                        {
                            asteroid[i].status.active = FALSE;
                        }
                        else
                        {
                            //PutImage( bullet.positionX, bullet.positionY, &bullet, IMAGE_NORMAL );
                            SetColor( pShip->hdr.pGolScheme->TextColor0 );
                            FillCircle( asteroid[i].positionX, asteroid[i].positionY, asteroid[i].size );

                            // We don't need to redraw the ship in case the asteroid messed it up, because then the pilot loses!
                            //SetState( pShip, RDIA_DRAW );
                        }
                    }
                }
            }
        }

        previousTick = tick;
    }
}


/****************************************************************************
  Function:
    SHORT FindNextPosition( SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT step, SHORT *x, SHORT *y )

  Description:
    This routine finds the position of the next step as an object is moved
    from point (x1,y1) to (x2,y2).

  Precondition:
    None

  Parameters:
    SHORT x1    - Starting x coordinate
    SHORT y1    - Starting y coordinate
    SHORT x2    - Final x coordinate
    SHORT y2    - Final y coordinate
    SHORT step  - Current step
    SHORT *x    - Calculated x position
    SHORT *y    - Calculated y position

  Returns:
    SHORT   - The next step to request.

  Remarks:
    The algorithm for this function is taken from the graphics library and
    its line draw function.  A step parameter has been added to allow a user
    to find interrum positions along the line, such as a bullet along a
    trajectory.
  ***************************************************************************/

SHORT FindNextPosition( SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT step, SHORT *x, SHORT *y )
{
    SHORT currentStep;
    SHORT deltaX, deltaY;
    SHORT error;
    SHORT stepErrorLT, stepErrorGE;
    SHORT stepX, stepY;
    SHORT steep;
    SHORT temp;

    currentStep = 0;

     if(x1==x2)
     {
        if(y1>y2)
        {
            temp = y1; y1 = y2; y2 = temp;
        }
        for(temp=y1; (temp<y2+1) && (currentStep<step); temp++, currentStep++) {}
        *x = x1;
        *y = temp;
        return step+1;
    }

    if(y1==y2)
    {
        if(x1>x2)
        {
            temp = x1; x1 = x2; x2 = temp;
        }
        for(temp=x1; (temp<x2+1) && (currentStep<step); temp++, currentStep++) {}
        *x = temp;
        *y = y1;
        return step+1;
    }

    stepX= 0;
    deltaX = x2-x1;
    if(deltaX < 0)
    {
        deltaX= -deltaX;
        --stepX;
    }
    else
    {
        ++stepX;
    }

    stepY= 0;
    deltaY = y2-y1;
    if(deltaY < 0)
    {
        deltaY= -deltaY;
        --stepY;
    }
    else
    {
        ++stepY;
    }

    steep= 0;
    if(deltaX < deltaY)
    {
        ++steep;
        temp = deltaX;  deltaX = deltaY;  deltaY = temp;
        temp = x1;  x1 = y1;  y1 = temp;
        temp = stepX;  stepX = stepY;  stepY = temp;
        *x = y1;
        *y = x1;
    }
    else
    {
        *x = x1;
        *y = y1;
    }

    // If the current error greater or equal zero
    stepErrorGE= deltaX<<1;
    // If the current error less than zero
    stepErrorLT= deltaY<<1;
    // Error for the first pixel
    error= stepErrorLT-deltaX;

    currentStep ++;

    while((--deltaX >= 0) && (currentStep < step))
    {
        currentStep ++;

        if(error >= 0)
        {
            y1+= stepY;
            error-= stepErrorGE;
        }
        x1+= stepX;
        error+= stepErrorLT;

        if(steep)
        {
            *x = y1;
            *y = x1;
        }
        else
        {
            *x = x1;
            *y = y1;
        }
   }

   return step+1;
}


/****************************************************************************
  Function:
    WORD ProcessMessageBlaster( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

  Description:
    This function processes the messages for the Asteriods screen. It allows
    the user to spin the ship and fire bullets.

  Precondition:
    Call ShowScreenBlaster() prior to using this function to display the
    correct screen.

  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    The global screenState is updated if the user quits.
  ***************************************************************************/

WORD ProcessMessageBlaster( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    WORD    controlID;

    controlID = GetObjID(pObj);

    switch (controlID)
    {
        case ID_QUIT:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                strcpy( pageInformation.title, "Blaster" );
                strcpy( scoreInfo, "Mission aborted." );
                screenState = SCREEN_DISPLAY_BLASTER_SCORE;
            }
            break;

        case ID_SHIP:
            // Perform default processing to turn the ship.
            return 1;

        case ID_BLASTER:
            // Fire!
            if (!bullet.status.active)
            {
                bullet.status.active    = TRUE;
                bullet.status.terminate = FALSE;
                bullet.angle            = RdiaGetVal(pShip);
                bullet.positionX        = SHIP_CENTER_X-(BULLET_SIZE/2);
                bullet.positionY        = SHIP_CENTER_Y-(BULLET_SIZE/2);

                bullet.updateInterval   = 1;
                bullet.deltaX           = RdiaCosine( bullet.angle );
                bullet.deltaY           = RdiaSine( bullet.angle );
                bullet.step             = 0;
            }
            break;
    }
    return 0;
}


/****************************************************************************
  Function:
    void ShowScreenBlaster( void )

  Description:
    This function draws the screen for the Blaster game.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    Buttons for the fire and exit functions are hidden.

    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.

  ***************************************************************************/

void ShowScreenBlaster( void )
{
    int     i;

    GOLFree();
    SetColor(BLACK);        // set color to BLACK
    ClearDevice();          // set screen to all BLACK

    // Create the hidden exit button
    BtnCreate(
                        ID_QUIT,
                        GetMaxX()-4, 0, GetMaxX()-2, 2, 0,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    // Create the hidden blaster trigger button
    BtnCreate(
                        ID_BLASTER,
                        GetMaxX()-6, 0, GetMaxX()-4, 2, 0,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    // Create the ship.  Yeah, it's just a dial.  So use your imagination...
    pShip = RdiaCreate(
                        ID_SHIP,
                        SHIP_CENTER_X,
                        SHIP_CENTER_Y,
                        SHIP_RADIUS,
                        RDIA_DRAW,
                        BLASTER_RESOLUTION,
                        0,  // Initial value
                        BLASTER_MAX,
                        NULL );

    InitializeRandomNumberGenerator();

    bullet.status.active    = FALSE;
    bullet.status.terminate = FALSE;
    previousTick            = tick;
    score                  = 0;
    for (i=0; i<NUM_ACTIVE_ASTEROIDS; i++)
    {
        asteroid[i].status.active       = FALSE;
        asteroid[i].status.terminate    = FALSE;
    }
}


/****************************************************************************
  Function:
    void TranslateMessageBlaster( GOL_MSG *pMsg )

  Description:
    This routine translates the raw touchpad messages into messages for the
    controls on the Blaster screen.

  Precondition:
    ShowScreenBlaster() must be called to place the proper controls on the
    screen.

  Parameters:
    GOL_MSG* pMsg       - The original system message information

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void TranslateMessageBlaster( GOL_MSG *pMsg )
{
    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_TOUCH_PAD))
    {
        if ((pMsg->param2 == SCAN_LEFT_PRESSED) ||
            (pMsg->param2 == SCAN_RIGHT_PRESSED) ||
            (pMsg->param2 == SCAN_LEFT_RELEASED) ||
            (pMsg->param2 == SCAN_RIGHT_RELEASED))
        {
            pMsg->param1 = ID_SHIP;
        }
        else if ((pMsg->param2 == SCAN_CR_PRESSED) ||
                 (pMsg->param2 == SCAN_CR_RELEASED))
        {
            pMsg->param1 = ID_BLASTER;
        }
        else if (pMsg->param2 == SCAN_UP_PRESSED)
        {
            pMsg->param1 = ID_QUIT;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else if (pMsg->param2 == SCAN_UP_RELEASED)
        {
            pMsg->param1 = ID_QUIT;
            pMsg->param2 = SCAN_CR_RELEASED;
        }
    }
}

//******************************************************************************
//******************************************************************************
// Shapelet
//******************************************************************************
//******************************************************************************

//******************************************************************************
// Constants
//******************************************************************************

#define BALL_SIZE                   8
#define BUBBLE_COLUMNS              (BUBBLE_COLUMNS_CALC > BUBBLE_COLUMNS_MAX ? BUBBLE_COLUMNS_MAX : BUBBLE_COLUMNS_CALC)
#define BUBBLE_COLUMNS_CALC         ((BUBBLE_GRID_RIGHT-BUBBLE_GRID_LEFT-1) / BALL_SIZE - 1)
#define BUBBLE_COLUMNS_MAX          12
#define BUBBLE_GRID_BOTTOM          GetMaxY()
#define BUBBLE_GRID_LEFT            0
#define BUBBLE_GRID_RIGHT           (GetMaxX())
#ifdef SHOW_SCORE
    #define BUBBLE_GRID_TOP         TEXT_HEIGHT
#else
    #define BUBBLE_GRID_TOP         0
#endif
#define BUBBLE_ROWS                 ((BUBBLE_GRID_BOTTOM - BUBBLE_GRID_TOP - 1) / BALL_SIZE - 1)
#define BUBBLE_ROWS_MIN             3
#define BLANK_BALL                  (NUM_BALL_TYPES + 1)
#define MARKING_BALL                (NUM_BALL_TYPES)
#define MAX_BALLS_REMOVED           9
#define NUM_BALL_TYPES              4
#define SCORE_BOTTOM                (SCORE_TOP + TEXT_HEIGHT)
#define SCORE_LEFT                  0
#define SCORE_RIGHT                 GetMaxX()
#define SCORE_TOP                   0
#define TEXT_HEIGHT                 10 //GetTextHeight( &FONTDEFAULT )


//******************************************************************************
// Global Variables
//******************************************************************************

extern IMAGE_FLASH  ballCircle;
extern IMAGE_FLASH  ballCross;
extern IMAGE_FLASH  ballMarking;
extern IMAGE_FLASH  ballSquare;
extern IMAGE_FLASH  ballTriangle;
void *              balls[NUM_BALL_TYPES+2];
SHORT               ballsRemoved;
SHORT               columnsRemoved;
SHORT               lastSelectedColumn;
SHORT               lastSelectedRow;
SHORT               lastBall;
SHORT               lastBallCount;
WORD                scoreTable[MAX_BALLS_REMOVED] = { 0, 1, 2, 4, 8, 16, 32, 64, 128 };
BOOL                selectionMarked;


//******************************************************************************
// Local Function Prototypes
//******************************************************************************

void    ClearSelection( void );
BOOL    CompactHorizontal( SHORT column );
BOOL    CompactVertical( SHORT column );
SHORT   FindBallNumber( SHORT column, SHORT row );
void    MarkAllNeighbors( SHORT column, SHORT row, SHORT oldBall, SHORT newBall );
BOOL    MovesRemaining( void );


/****************************************************************************
  Function:
    BOOL CompactHorizontal( SHORT column )

  Description:
    This function checks to see if any columns are now blank.  If there
    are blank columns, the columns are shifted from left to right to
    compact the field.  The routine returns an indication as to whether or
    not columns were eliminated.  It should be called until no columns are
    eliminated.

  Precondition:
    Shapelet game information must be valid.

  Parameters:
    None

  Return Values:
    TRUE    - A column was eliminated
    FALSE   - No columns were eliminated

  Remarks:
    None
  ***************************************************************************/

BOOL CompactHorizontal( SHORT column )
{
    SHORT   ball;
    SHORT   blank;
    WORD    cellType;
    SHORT   i;
    void *  pBall;

    // Find first blank column.  It is blank if the row BUBBLE_ROWS-1 ball is blank.
    for (blank=BUBBLE_COLUMNS-1; (blank >= 0) && (FindBallNumber( blank, BUBBLE_ROWS-1 ) != BLANK_BALL); blank--);
    if (blank >= 0)
    {
        // Find first ball above the blank
        for (ball=blank-1; (ball >= 0) && (FindBallNumber( ball, BUBBLE_ROWS-1 ) == BLANK_BALL); ball--);
        if (ball >= 0)
        {
            columnsRemoved++;
            while (ball >= 0)
            {
                // Move one column over.
                for (i=BUBBLE_ROWS-1; i>=0; i--)
                {
                    pBall = GridGetCell( gamesGrid, ball, i, &cellType );
                    GridSetCell( gamesGrid, blank, i, GRIDITEM_IS_BITMAP|GRIDITEM_DRAW, pBall );
                }
                blank--;
                ball--;
            }
            while (blank >= 0)
            {
                // Fill the rest of the columns with blanks.
                for (i=BUBBLE_ROWS-1; i>=0; i--)
                {
                    GridSetCell( gamesGrid, blank, i, GRIDITEM_IS_BITMAP|GRIDITEM_DRAW, NULL );
                }
                blank--;
            }
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}


/****************************************************************************
  Function:
    BOOL CompactVertical( SHORT column )

  Description:
    This routine compacts the indicated column, eliminating blank spaces
    left by removed shapes.

  Precondition:
    None

  Parameters:
    SHORT column    - The column to compact.

  Return Values:
    TRUE    - The column was compacted
    FALSE   - The column was not compacted

  Remarks:
    None
  ***************************************************************************/

BOOL CompactVertical( SHORT column )
{
    SHORT   ball;
    SHORT   blank;
    WORD    cellType;
    void    *pBall;

    // Find first blank
    for (blank=BUBBLE_ROWS-1; (blank >= 0) && (FindBallNumber( column, blank ) != BLANK_BALL); blank--);
    if (blank >= 0)
    {
        // Find first ball above the blank
        for (ball=blank-1; (ball >= 0) && (FindBallNumber( column, ball ) == BLANK_BALL); ball--);
        if (ball >= 0)
        {
            // Move the non-blank balls down.
            while (ball >= 0)
            {
                pBall = GridGetCell( gamesGrid, column, ball, &cellType );
                GridSetCell( gamesGrid, column, blank, GRIDITEM_IS_BITMAP|GRIDITEM_DRAW, pBall );
                ball--;
                blank--;
            }
            // Fill the rest with blanks.
            while (blank >= 0)
            {
                GridSetCell( gamesGrid, column, blank, GRIDITEM_IS_BITMAP|GRIDITEM_DRAW, NULL );
                blank--;
            }
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}


/****************************************************************************
  Function:
    SHORT FindBallNumber( SHORT column, SHORT row )

  Description:
    This routine returns the number of the ball in the indicated cell.  It
    does it by comparing the bitmap pointer in the cell to the various ball
    bitmaps.  If there is no match, the cell is assumed to be empty.

  Precondition:
    None

  Parameters:
    SHORT column    - The cell's column location
    SHORT row       - The cell's row location

  Returns:
    SHORT - The ball number of the current cell, or BLANK_BALL if the cell
    does not contain a ball.

  Remarks:
    None
  ***************************************************************************/

SHORT FindBallNumber( SHORT column, SHORT row )
{
    SHORT   ball;
    WORD    cellType;
    void    *pBall;

    pBall = GridGetCell( gamesGrid, column, row, &cellType );
    for (ball=0; ball<(NUM_BALL_TYPES+1); ball++)
    {
        if ((int)pBall == (int)balls[ball])
        {
            break;
        }
    }
    if (ball<(NUM_BALL_TYPES+1))
    {
        return ball;
    }
    else
    {
        return BLANK_BALL;
    }
}


/****************************************************************************
  Function:
    void MarkAllNeighbors( SHORT column, SHORT row,
                SHORT oldBall, SHORT newBall )

  Description:
    This routine marks the current ball with the indicated new ball.  It
    then checks the ball's four neighbors to see if they match the old ball
    and should be marked as well.

    Note that we use this routine in three places.  When the user first
    selects a cell, we use it to show the selected region by setting all
    matching neighboring balls to a marking ball.  If the user next selects
    a cell that does not have a marking ball in it, this routine is called to
    revert all marked balls to their original ball type.  If the user next
    selects a cell that does have a marking ball in it, this routine is
    called to set all marked balls to blank.  Then the field should be
    compacted.

  Precondition:
    None

  Parameters:
    SHORT column    - The column location of the current ball (cell)
    SHORT row       - The row location of the current ball (cell)
    SHORT oldBall   - The ball type that we are trying to match
    SHORT newBall   - The new ball that we set the cell to

  Returns:
    None

  Remarks:
    This routine is recursive.  We can do this because the architecture and
    the C compiler support it.
  ***************************************************************************/

void MarkAllNeighbors( SHORT column, SHORT row, SHORT oldBall, SHORT newBall )
{
    WORD    cellType;

    ballsRemoved++;
    GridSetCell( gamesGrid, column, row, GRIDITEM_IS_BITMAP|GRIDITEM_DRAW, balls[newBall] );

    // Check Straight neighbors
    if (((column-1) >= 0)              && ((int)GridGetCell( gamesGrid, column-1, row, &cellType ) == (int)balls[oldBall]))
        MarkAllNeighbors( column-1, row, oldBall, newBall );
    if (((column+1) <  BUBBLE_COLUMNS) && ((int)GridGetCell( gamesGrid, column+1, row, &cellType ) == (int)balls[oldBall]))
        MarkAllNeighbors( column+1, row, oldBall, newBall );
    if (((row-1)    >= 0)              && ((int)GridGetCell( gamesGrid, column, row-1, &cellType ) == (int)balls[oldBall]))
        MarkAllNeighbors( column, row-1, oldBall, newBall );
    if (((row+1)    <  BUBBLE_ROWS)    && ((int)GridGetCell( gamesGrid, column, row+1, &cellType ) == (int)balls[oldBall]))
        MarkAllNeighbors( column, row+1, oldBall, newBall );
}

/****************************************************************************
  Function:
    BOOL MovesRemaining( void )

  Description:
    This routine checks to see if any more groups are available for
    removal.  If not, the game is over.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    TRUE    - The user still has balls that can be removed
    FALSE   - No more balls can be removed.

  Remarks:
    We can stop as soon as we find one pair of balls that match.
  ***************************************************************************/

BOOL MovesRemaining( void )
{
    WORD    cellType;
    WORD    column;
    WORD    currentBall;
    WORD    row;

    for (column = 0; column < BUBBLE_COLUMNS; column++)
    {
        for (row = 0; row < BUBBLE_ROWS; row++)
        {
            currentBall = (WORD)GridGetCell( gamesGrid, column, row, &cellType );

            if (currentBall != (WORD)balls[BLANK_BALL])
            {
                if (((column-1) >= 0)              && ((int)GridGetCell( gamesGrid, column-1, row, &cellType ) == currentBall))
                    return TRUE;
                if (((column+1) <  BUBBLE_COLUMNS) && ((int)GridGetCell( gamesGrid, column+1, row, &cellType ) == currentBall))
                    return TRUE;
                if (((row-1)    >= 0)              && ((int)GridGetCell( gamesGrid, column, row-1, &cellType ) == currentBall))
                    return TRUE;
                if (((row+1)    <  BUBBLE_ROWS)    && ((int)GridGetCell( gamesGrid, column, row+1, &cellType ) == currentBall))
                    return TRUE;
            }
        }
    }

    return FALSE;
}


/****************************************************************************
  Function:
    WORD ProcessMessageShapelet( WORD translatedMsg, OBJ_HEADER* pObj,
                GOL_MSG* pMsg )

  Description:
    This function processes the messages for the Shapelet game.  Users can
    move to a different bubble, select/deselect a group for deletion, and
    delete the selected group.

  Precondition:
    Call ShowScreenShapelet() prior to using this function to display the
    correct screen.

  Parameters:
    WORD translatedMsg  The translated control-level message
    OBJ_HEADER* pObj    Object to which the message applies
    GOL_MSG* pMsg       The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    None
  ***************************************************************************/

WORD ProcessMessageShapelet( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    if (GetObjID( pObj ) == ID_GAMES_GRID)
    {
        #if defined USE_KEYBOARD
        if (translatedMsg == GRID_MSG_ITEM_SELECTED)
        #elif defined USE_TOUCHSCREEN
        if ((translatedMsg == GRID_MSG_TOUCHED) && (pMsg->uiEvent == EVENT_RELEASE))
        #endif
        {
            WORD    ballType;
            SHORT   columnSelected;
            SHORT   rowSelected;

            // Find the item that was selected.
            #if defined USE_TOUCHSCREEN
                // pMsg->param1 = column, pMsg->param2 = row
                columnSelected  = (pMsg->param1 - gamesGrid->left) / (gamesGrid->cellWidth+1);
                rowSelected     = (pMsg->param2 - gamesGrid->top)  / (gamesGrid->cellHeight+1);
            #elif defined USE_KEYBOARD
                columnSelected  = GridGetFocusX( gamesGrid );
                rowSelected     = GridGetFocusY( gamesGrid );
            #endif

            // See if the selected cell has a valid ball.
            ballType = FindBallNumber( columnSelected, rowSelected );
            if (ballType == BLANK_BALL)
            {
                // The user is aborting the game.
                strcpy( pageInformation.title, "Shapelet" );
                strcpy( scoreInfo, "Game aborted." );
                screenState = SCREEN_DISPLAY_SHAPELET_SCORE;
            }
            else if ((ballType == MARKING_BALL) && (lastBallCount > 1))
            {
                SHORT column;

                // The user is selecting part of the original selection.  Remove it from the field.
                ballsRemoved = 0;
                columnsRemoved = 0;

                MarkAllNeighbors( columnSelected, rowSelected, MARKING_BALL, BLANK_BALL );
                selectionMarked = FALSE;

                // Compact vertical.
                for (column = BUBBLE_COLUMNS-1; column >= 0; column--)
                {
                    // Compact each column until no more compaction occurs.
                    while (CompactVertical( column ));

                }

                // Compact horizontal.
                while (CompactHorizontal(0));

                if (ballsRemoved)
                {
                    SetState( gamesGrid, GRID_DRAW_ITEMS );
                }

                if (ballsRemoved < MAX_BALLS_REMOVED)
                {
                    score += scoreTable[ballsRemoved-1] + 100*columnsRemoved;
                }
                else
                {
                    score += scoreTable[MAX_BALLS_REMOVED-1] + 100*(ballsRemoved-MAX_BALLS_REMOVED+1) + 100*columnsRemoved;
                }

                #ifdef SHOW_SCORE
                    sprintf( scoreText, "Score: %d", score );
                    SetState( pScore, ST_DRAW );
                #endif

                // Check for end of game.
                if ((int)GridGetCell( gamesGrid, BUBBLE_COLUMNS-1, BUBBLE_ROWS-1, &ballType ) == (int)balls[BLANK_BALL])
                {
                    score += 1000;
                    strcpy( pageInformation.title, "Shapelet" );
                    strcpy( scoreInfo, "Congratulations! You won!" );
                    screenState = SCREEN_DISPLAY_SHAPELET_SCORE;
                }
                else if (!MovesRemaining())
                {
                    strcpy( pageInformation.title, "Shapelet" );
                    strcpy( scoreInfo, "No more moves." );
                    screenState = SCREEN_DISPLAY_SHAPELET_SCORE;
                }

                lastSelectedColumn  = -1;
                lastSelectedRow     = -1;
                lastBall            = BLANK_BALL;
                lastBallCount       = 0;
            }
            else
            {
                // The user is unselecting the old selection, and perhaps selecting a new section.
                // First, unmark the current selection.
                if (lastBall != BLANK_BALL)
                {
                    MarkAllNeighbors( lastSelectedColumn, lastSelectedRow, MARKING_BALL, lastBall );
                }

                // Redraw the affected items with the next draw.
                SetState( gamesGrid, GRID_DRAW_ITEMS );

                // If the user did not select a ball, or he's unmarking a selection of 1, then we're done.
                if ((ballType == BLANK_BALL) || (lastBallCount == 1))
                {
                    // Save the selection information.
                    lastSelectedColumn  = -1;
                    lastSelectedRow     = -1;
                    lastBall            = BLANK_BALL;
                    lastBallCount       = 0;

                    return 0;
                }

                // Now mark the new balls
                ballsRemoved = 0;
                MarkAllNeighbors( columnSelected, rowSelected, ballType, MARKING_BALL );

                // Save the selection information.
                lastSelectedColumn  = columnSelected;
                lastSelectedRow     = rowSelected;
                lastBall            = ballType;
                lastBallCount       = ballsRemoved;
            }

            return 0;   // Don't call the default handler.
        }
        else
        {
            // The focus moved.  Allow the call to the default handler to show the new focus.
            return 1;
        }
    }
    return 0;  // Don't call the default handler.
}


/****************************************************************************
  Function:
    void ShowScreenShapelet( void )

  Description:
    This routine creates the screen for the Shapelet game.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenShapelet( void )
{
    SHORT   i;
    SHORT   j;
    SHORT   k;
    SHORT   r;

    GOLFree();
    SetColor(BLACK);        // set color to BLACK
    ClearDevice();          // set screen to all BLACK

    InitializeRandomNumberGenerator();

    balls[0]            = (void *) &ballCircle;
    balls[1]            = (void *) &ballSquare;
    balls[2]            = (void *) &ballCross;
    balls[3]            = (void *) &ballTriangle;
    balls[MARKING_BALL] = (void *) &ballMarking;
    balls[BLANK_BALL]   = (void *) NULL;

    #ifdef SHOW_SCORE
        strcpy( scoreText, "Score" );
        pScore = StCreate(
                        ID_SCORE,           // ID
                        SCORE_LEFT,         // left
                        SCORE_TOP,          // top
                        SCORE_RIGHT,        // right
                        SCORE_BOTTOM,       // bottom
                        ST_DRAW,
                        scoreText,
                        NULL );
    #endif

    gamesGrid = GridCreate(
                    ID_GAMES_GRID,                      // ID
                    BUBBLE_GRID_LEFT,                   // left
                    BUBBLE_GRID_TOP,                    // top
                    BUBBLE_GRID_RIGHT,                  // right
                    BUBBLE_GRID_BOTTOM,                 // bottom
                    GRID_DRAW_ALL | GRID_SHOW_FOCUS,    // state
                    BUBBLE_COLUMNS,     // numColumns
                    BUBBLE_ROWS,        // numRows
                    BALL_SIZE,          // cellWidth
                    BALL_SIZE,          // cellHeight
                    NULL );             // scheme )

    if (!gamesGrid)
    {
        screenState = SCREEN_DISPLAY_MAIN;
        return;
    }

    // Balls are set to NULL by default.  Now populate the grid.
    for(j=0; j<BUBBLE_COLUMNS; j++)
    {
        k = Random(BUBBLE_ROWS - BUBBLE_ROWS_MIN) + BUBBLE_ROWS_MIN;
        for (i=0; i<k; i++)
        {
            r = Random(NUM_BALL_TYPES-1);
            // The cells are"upside down".  Row 0 is at the top.
            GridSetCell( gamesGrid, BUBBLE_COLUMNS-1-j, BUBBLE_ROWS-1-i, GRIDITEM_IS_BITMAP, (void *)balls[r] );
        }
    }

    GridSetFocus( gamesGrid, 0, BUBBLE_ROWS-1 );

    ballsRemoved        = 0;
    score              = 0;
    lastSelectedColumn  = -1;
    lastSelectedRow     = -1;
    lastBall            = BLANK_BALL;
    lastBallCount       = 0;
}


/****************************************************************************
  Function:
    void TranslateMessageShapelet( GOL_MSG *pMsg )

  Description:
    If the message is valid, this function translates the raw touchpad message
    pMsg->param1 to the proper message for the appropriate control.

  Precondition:
    None

  Parameters:
    GOL_MSG* pMsg       - The original system message information

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void TranslateMessageShapelet( GOL_MSG *pMsg )
{
    // If the message came from the keypad, assign it to the grid.
    if (pMsg->param1 == ID_TOUCH_PAD)
    {
        pMsg->param1 = ID_GAMES_GRID;
    }
}


//******************************************************************************
//******************************************************************************
// Snake
//******************************************************************************
//******************************************************************************

//******************************************************************************
// Constants
//******************************************************************************

#define FIELD_PIECES                    SNAKE_COLUMNS
#define LONGEST_SNAKE                   100

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

#define SNAKE_UPDATE_INTERVAL           (500 / MILLISECONDS_PER_TICK)


//******************************************************************************
// Data Structures
//******************************************************************************

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


//******************************************************************************
// Global Variables
//******************************************************************************

SHORT               defaultDirection;
FIELD_INFO          field[SNAKE_COLUMNS][SNAKE_ROWS];
SNAKE_INFO          snake;
extern IMAGE_FLASH  snakeHead;
extern IMAGE_FLASH  snakeBody;
extern IMAGE_FLASH  snakeFood;


#define SNAKE_HEAD  snake.pieces[0]


/****************************************************************************
  Function:
    void DrawSnake( void )

  Description:
    This routine draws the snake as it moves on the field.  First, we
    check if the snake has crashed into a wall.  If the snake has just eaten
    a mouse, he grows by one segment.  We'll do this by moving the head but
    not the tail, making the snake one segment longer.  Then we check to see
    if the snake has hit itself.  Finally, we see if the snake has eaten
    another mouse.  If so, we set the flags for the next pass and put a new
    mouse in the box.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void DrawSnake( void )
{
    SHORT i;
    SHORT j;
    SHORT oldTail;

    if ((tick - previousTick) > SNAKE_UPDATE_INTERVAL)
    {
        // Move the snake
        SetState( gamesGrid, GRID_DRAW_ITEMS );

        // Check for a wall
        if (((SNAKE_HEAD.direction == DIRECTION_UP)    && (SNAKE_HEAD.row == 0)) ||
            ((SNAKE_HEAD.direction == DIRECTION_RIGHT) && (SNAKE_HEAD.column == SNAKE_COLUMNS-1)) ||
            ((SNAKE_HEAD.direction == DIRECTION_DOWN)  && (SNAKE_HEAD.row == SNAKE_ROWS-1)) ||
            ((SNAKE_HEAD.direction == DIRECTION_LEFT)  && (SNAKE_HEAD.column == 0)))
        {
            strcpy( pageInformation.title, "Shu Box" );
            strcpy( scoreInfo, "Snake Crashed!" );
            screenState = SCREEN_DISPLAY_SNAKE_SCORE;
            return;
        }
        score ++;

        // Draw the snake on the field.  We record each segment in memory, but we only
        // have to draw the head, the second segment, and possibly the space that the
        // tail just left.

        oldTail = snake.length-1;
        if (field[snake.pieces[oldTail].column][snake.pieces[oldTail].row].hasBeenEaten)
        {
            // Clear the eaten flag
            field[snake.pieces[oldTail].column][snake.pieces[oldTail].row].hasBeenEaten = FALSE;

            // Add a new piece at the tail.
            snake.pieces[snake.length] = snake.pieces[oldTail];
            GridSetCell( gamesGrid, snake.pieces[snake.length].column, snake.pieces[snake.length].row, GRIDITEM_IS_BITMAP|GRIDITEM_DRAW, (void *)&snakeBody );
            snake.length++;
            score += 10;
            if (snake.length == LONGEST_SNAKE)
            {
                score += 1000;
                strcpy( pageInformation.title, "Shu Box" );
                strcpy( scoreInfo, "Shu is full grown!" );
                screenState = SCREEN_DISPLAY_SNAKE_SCORE;
                return;
            }
        }
        else
        {
            // Erase the tail.  If there is food, mark it.  Otherwise, clear it.
            if (field[snake.pieces[oldTail].column][snake.pieces[oldTail].row].hasFood)
            {
                GridSetCell( gamesGrid, snake.pieces[oldTail].column, snake.pieces[oldTail].row, GRIDITEM_IS_BITMAP|GRIDITEM_DRAW, (void *)&snakeFood );
            }
            else
            {
                GridSetCell( gamesGrid, snake.pieces[oldTail].column, snake.pieces[oldTail].row, GRIDITEM_IS_BITMAP|GRIDITEM_DRAW, NULL );
            }
        }

        for (i=oldTail; i>=0; i--)
        {
            switch (snake.pieces[i].direction)
            {
                case DIRECTION_UP:
                    snake.pieces[i].row--;
                    break;
                case DIRECTION_RIGHT:
                    snake.pieces[i].column++;
                    break;
                case DIRECTION_DOWN:
                    snake.pieces[i].row++;
                    break;
                case DIRECTION_LEFT:
                    snake.pieces[i].column--;
                    break;
            }
            if (i == 0)
            {
                // This is the head.  Don't update the direction.  Draw the head.
                GridSetCell( gamesGrid, SNAKE_HEAD.column, SNAKE_HEAD.row, GRIDITEM_IS_BITMAP|GRIDITEM_DRAW, (void *)&snakeHead );
            }
            else
            {
                if (i == 1)
                {
                    // Change the head to a segment
                    GridSetCell( gamesGrid, SNAKE_HEAD.column, SNAKE_HEAD.row, GRIDITEM_IS_BITMAP|GRIDITEM_DRAW, (void *)&snakeBody );
                }

                // Update the next direction for the piece.
                snake.pieces[i].direction = snake.pieces[i-1].direction;
            }
        }

        // Check for the snake hitting itself
        for (i=0; i<snake.length-2; i++)
        {
            for (j=i+1; j<snake.length-1; j++)
            {
                if ((snake.pieces[i].column == snake.pieces[j].column) &&
                    (snake.pieces[i].row    == snake.pieces[j].row))
                {
                    strcpy( pageInformation.title, "Shu Box" );
                    strcpy( scoreInfo, "Snake in a Knot!" );
                    screenState = SCREEN_DISPLAY_SNAKE_SCORE;
                    return;
                }
            }
        }

        // See if the snake is eating food.
        if (field[SNAKE_HEAD.column][SNAKE_HEAD.row].hasFood)
        {
            field[SNAKE_HEAD.column][SNAKE_HEAD.row].hasFood = FALSE;
            field[SNAKE_HEAD.column][SNAKE_HEAD.row].hasBeenEaten = TRUE;

            // Put another piece of food on.  We'll let it be anywhere that doesn't
            // already have food.
            do
            {
                i = Random(SNAKE_COLUMNS);
                j = Random(SNAKE_ROWS);
            } while (field[i][j].hasFood);

            // Mark and draw the food.  Don't draw it if the snake is there!
            field[i][j].hasFood = TRUE;
            if (!GridGetCell( gamesGrid, i, j, (WORD *)&oldTail ))
            {
                GridSetCell( gamesGrid, i, j, GRIDITEM_IS_BITMAP|GRIDITEM_DRAW, (void *)&snakeFood );
            }
        }

        previousTick = tick;
    }
}


/****************************************************************************
  Function:
    WORD ProcessMessageSnake( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

  Description:
    This function processes the messages for the Snake game, to change the
    snake's direction.  We will be nice to the player - if he selects the
    direction opposite to what he is traveling, we will ignore the message
    rather than instantly tie the snake into a knot.

  Precondition:
    Call ShowScreenSnake() prior to using this function to display the
    correct screen.

  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information

  Returns:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    None
  ***************************************************************************/

WORD ProcessMessageSnake( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    WORD    buttonID;

    buttonID = GetObjID(pObj);

    switch (buttonID)
    {
        case ID_SNAKE_UP:
            if (SNAKE_HEAD.direction != DIRECTION_DOWN)
            {
                SNAKE_HEAD.direction = DIRECTION_UP;
            }
            break;

        case ID_SNAKE_DOWN:
            if (SNAKE_HEAD.direction != DIRECTION_UP)
            {
                SNAKE_HEAD.direction = DIRECTION_DOWN;
            }
            break;

        case ID_SNAKE_LEFT:
            if (SNAKE_HEAD.direction != DIRECTION_RIGHT)
            {
                SNAKE_HEAD.direction = DIRECTION_LEFT;
            }
            break;

        case ID_SNAKE_RIGHT:
            if (SNAKE_HEAD.direction != DIRECTION_LEFT)
            {
                SNAKE_HEAD.direction = DIRECTION_RIGHT;
            }
            break;
    }

    return 0;
}


/****************************************************************************
  Function:
    void ShowScreenSnake( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the screen for the Shu Box (Snake) game.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenSnake( void )
{
    SHORT   i;
    SHORT   j;
    SHORT   k;

    GOLFree();
    SetColor(BLACK);        // set color to BLACK
    ClearDevice();          // set screen to all BLACK

    // Create hidden buttons to receive direction messages.

    BtnCreate(          ID_SNAKE_UP,
                        GetMaxX()-8, 0, GetMaxX()-6, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(          ID_SNAKE_RIGHT,
                        GetMaxX()-6, 0, GetMaxX()-4, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(          ID_SNAKE_DOWN,
                        GetMaxX()-4, 0, GetMaxX()-2, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(          ID_SNAKE_LEFT,
                        GetMaxX()-4, 0, GetMaxX()-2, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    // Create the game grid.

    InitializeRandomNumberGenerator();

    balls[0]            = (void *) &ballCircle;
    balls[1]            = (void *) &ballSquare;
    balls[2]            = (void *) &ballCross;
    balls[3]            = (void *) &ballTriangle;
    balls[MARKING_BALL] = (void *) &ballMarking;
    balls[BLANK_BALL]   = (void *) NULL;

    gamesGrid = GridCreate(
                    ID_GAMES_GRID,                          // ID
                    SNAKE_GRID_LEFT,                        // left
                    SNAKE_GRID_TOP,                         // top
                    SNAKE_GRID_RIGHT,                       // right
                    SNAKE_GRID_BOTTOM,                      // bottom
                    GRID_DRAW_ALL | GRID_SHOW_BORDER_ONLY,  // state
                    SNAKE_COLUMNS,                          // numColumns
                    SNAKE_ROWS,                             // numRows
                    SEGMENT_SIZE,                           // cellWidth
                    SEGMENT_SIZE,                           // cellHeight
                    NULL );                                 // scheme )

    if (!gamesGrid)
    {
        screenState = SCREEN_DISPLAY_MAIN;
        return;
    }

    // Initialize the field and the grid.

    for (i=0; i<SNAKE_COLUMNS; i++)
    {
        for (j=0; j<SNAKE_ROWS; j++)
        {
            field[i][j].hasFood         = FALSE;
            field[i][j].hasBeenEaten    = FALSE;
            GridSetCell( gamesGrid, i, j, GRIDITEM_IS_BITMAP, NULL );
        }
    }

    // Initialize the snake.

    SNAKE_HEAD.column       = 0;
    SNAKE_HEAD.row          = 0;
    SNAKE_HEAD.direction    = DIRECTION_RIGHT;
    snake.length            = 1;

    // Place random food

    for (k=0; k<FIELD_PIECES; k++)
    {
        // Find coordinates for food
        i = Random( SNAKE_COLUMNS-1 );
        j = Random( SNAKE_ROWS-1 );

        // Mark and draw the food
        field[i][j].hasFood     = TRUE;
        GridSetCell( gamesGrid, i, j, GRIDITEM_IS_BITMAP, (void *)&snakeFood );
    }

    // Draw the snake on the field
    GridSetCell( gamesGrid, SNAKE_HEAD.column, SNAKE_HEAD.row, GRIDITEM_IS_BITMAP, (void *)&snakeHead );

    score               = 0;
    previousTick        = tick;
}


/****************************************************************************
  Function:
    void TranslateMessageSnake( GOL_MSG *pMsg )

  Description:
    If the message is valid, this function translates pMsg->param1 and
    pMsg->param2 to the proper button to control the snake.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void TranslateMessageSnake( GOL_MSG *pMsg )
{
    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_TOUCH_PAD))
    {
        if (pMsg->param2 == SCAN_UP_PRESSED)
        {
            pMsg->param1 = ID_SNAKE_UP;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else if (pMsg->param2 == SCAN_RIGHT_PRESSED)
        {
            pMsg->param1 = ID_SNAKE_RIGHT;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else if (pMsg->param2 == SCAN_DOWN_PRESSED)
        {
            pMsg->param1 = ID_SNAKE_DOWN;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else if (pMsg->param2 == SCAN_LEFT_PRESSED)
        {
            pMsg->param1 = ID_SNAKE_LEFT;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
    }
}
