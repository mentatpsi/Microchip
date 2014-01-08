/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		Games.c
* Description:			This file contains the main entry function which
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
*
************************************************************************/

/************************************************************************
 Header Includes													
 ************************************************************************/
#include "Games.h"

/************************************************************************
 Variables													
 ************************************************************************/
GRID *                  gamesGrid;
STATICTEXT *            pScore;
SHORT                   score;
XCHAR                   scoreText[20];
XCHAR                   scoreInfo[35];

extern int 				enableSoundFlag;
extern int 				SwitchS1Flag;
extern int 				SwitchS2Flag;

// Bomber Jet Game Variables
ASTEROID_INFO       	asteroid[NUM_ACTIVE_ASTEROIDS];
ASTEROID_INFO       	aliens[NUM_ACTIVE_ALIENS];
MISSILE_INFO         	missile;
MISSILE_INFO         	alienmissile[NUM_ACTIVE_ALIENS];
int 					countalienmissile = 0;
ROUNDDIAL*		        pShip;
SHORT           	    scoreSize[ASTEROID_SIZES]          = { 3, 2, 1 };
SHORT               	scoreSpeed[ASTEROID_UPDATE_SPEEDS] = { 3, 2, 1 };
int 					leftJet=0, rightJet=0, topJet=0, downJet=0;
int 					activemissileJet = 0;
extern int 				jetX, jetY;
int 					tempjetX, tempjetY;
volatile int 					EMWaveFlag = 0, EMWaveCount = 0;
int 					prevRadius = 0;

extern IMAGE_FLASH 	iconBomberJet;

// Snake Game Variables
SHORT               	defaultDirection;
FIELD_INFO          	field[SNAKE_COLUMNS][SNAKE_ROWS];
SNAKE_INFO          	snake;
SHORT               	columnsRemoved;
SHORT               	lastSelectedColumn;
SHORT               	lastSelectedRow;
BOOL                	selectionMarked;

extern IMAGE_FLASH 		snakeHead;
extern IMAGE_FLASH 		snakeBody;
extern IMAGE_FLASH 		snakeFood;
extern DWORD            previousTick;
extern DWORD            tick;
extern SCREEN_STATES    screenState;
extern PAGE_INFORMATION pageInformation;

#define SNAKE_HEAD  snake.pieces[0]


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

    if (!initialized)
    {
        srand( RANDOM_SEED );
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
    SHORT num = 0;

    num = rand() & 0xFF;   
    while (num >= (max+1))
    {
        num -= (max+1);
    }

    return num;
}


/************************************************************************
 Games
 ************************************************************************/
 
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
void ShowScreenGames(void)
{
    strcpy( pageInformation.title, "Games" );

    strcpy( pageInformation.buttonTextTopLeft,      "Home" );
    strcpy( pageInformation.buttonTextBottomLeft,   "Bomber Jet" );
    strcpy( pageInformation.buttonTextBottomRight,  "Snake" );

    pageInformation.buttonBitmapTopLeft        = NULL;
    pageInformation.buttonBitmapBottomLeft     = NULL;
    pageInformation.buttonBitmapTopRight       = NULL;
    pageInformation.buttonBitmapBottomRight    = NULL;

    pageInformation.instructions[0] = 0;

    InitializeScreen();

    BtnCreate(      ID_TOP_LEFT,                // ID
                    0,LOCATION_MIN_Y,62,38,2,   // dimension and radius
                    BTN_DRAW|BTN_PRESSED,       // will be displayed after creation
                    pageInformation.buttonBitmapTopLeft, // bitmap
                    pageInformation.buttonTextTopLeft,   // text
                    NULL );                     // Default scheme

    BtnCreate(      ID_BOTTOM_LEFT,             // ID
                    0,40,62,62,2,               // dimension and radius
                    BTN_DRAW,                   // will be displayed after creation
                    pageInformation.buttonBitmapBottomLeft, // bitmap
                    pageInformation.buttonTextBottomLeft,   // text
                    NULL );                     // Default scheme

    BtnCreate(      ID_BOTTOM_RIGHT,            // ID
                    64,40,125,62,2,             // dimension and radius
                    BTN_DRAW,                   // will be displayed after creation
                    pageInformation.buttonBitmapBottomRight,
                    pageInformation.buttonTextBottomRight,
                    NULL );                     // Default scheme
    Delay(50);
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
WORD ProcessMessageGames(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
    WORD    buttonID;

    buttonID = GetObjID(pObj);

	if (translatedMsg == BTN_MSG_RELEASED)
          if(enableSoundFlag)
          {
				while(speakerBusy());
				speakerActivate(SPEECH_ADDR_SELECT, SPEECH_SIZE_SELECT); 
		  }  
    switch (buttonID)
    {
	    case ID_GAMES_JET:
            if (translatedMsg == BTN_MSG_RELEASED)
            {
                screenState = SCREEN_DISPLAY_JET;
            }
            break;
            
        case ID_GAMES_SNAKE:
            if (translatedMsg == BTN_MSG_RELEASED)
            {
                screenState = SCREEN_DISPLAY_SNAKE;
            }
            break;

        case ID_GAMES_HOME:
            if (translatedMsg == BTN_MSG_RELEASED)
            {
                screenState = SCREEN_DISPLAY_MAIN;
            }
            break;
    }

    // Keep the current unimplemented button pressed
    SetState( (BUTTON*)pObj, BTN_PRESSED | BTN_DRAW );

    return 0;   // We do not need to call the default handler.
}


/************************************************************************
 Bomber Jet Game													
 ************************************************************************/

/****************************************************************************
  Function:
    void ShowScreenJet( void )
  Description:
    This function draws the screen for the Bomber Jet game.
  Precondition:
    None
  Parameters:
    None
  Returns:
    None
  Remarks:
    Buttons for the fire are hidden.
    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/
void ShowScreenJet(void)
{
    int     i;

    GOLFree();
    SetColor(BLACK);        // set color to BLACK
    ClearDevice();          // set screen to all BLACK
    
    // Create the hidden blaster trigger button
    BtnCreate(
                        ID_SHIP,
                        GetMaxX()-8, 0, GetMaxX()-6, 2, 0,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );
                        
    // Create the hidden exit button
    BtnCreate(
                        ID_EM_WAVE,
                        GetMaxX()-4, 0, GetMaxX()-2, 2, 0,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );
    // Create the hidden blaster trigger button
    BtnCreate(
                        ID_BOMBER,
                        GetMaxX()-6, 0, GetMaxX()-4, 2, 0,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );
                        
    InitializeRandomNumberGenerator();
    
    PutImage(leftJet, topJet, &iconBomberJet, 1);   

    missile.status.active    = FALSE;
    missile.status.terminate = FALSE;
    previousTick            = tick;
    score                   = 0;
    for (i=0; i<NUM_ACTIVE_ASTEROIDS; i++)
    {
        asteroid[i].status.active       = FALSE;
        asteroid[i].status.terminate    = FALSE;
    }
    
    for (i=0; i<NUM_ACTIVE_ALIENS; i++)
    {
	    alienmissile[i].status.active  	 = FALSE;
	    alienmissile[i].status.terminate  = FALSE;
        aliens[i].status.active       	 = FALSE;
        aliens[i].status.terminate    	 = FALSE;
    }
}

/****************************************************************************
  Function:
    void TranslateMessageJet( GOL_MSG *pMsg )
  Description:
    This routine translates the raw accelerometer messages into messages for the
    controls on the Bomber Jet screen.
  Precondition:
    ShowScreenJet() must be called to place the proper controls on the
    screen.
  Parameters:
    GOL_MSG* pMsg       - The original system message information
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void TranslateMessageJet(GOL_MSG *pMsg)
{
    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_ACCELEROMETER_PAD))
    {
        if ((pMsg->param2 == SCAN_UP_PRESSED) ||
            (pMsg->param2 == SCAN_RIGHT_PRESSED) ||
            (pMsg->param2 == SCAN_DOWN_RELEASED) ||
            (pMsg->param2 == SCAN_LEFT_RELEASED))
        {
            pMsg->param1 = ID_SHIP;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else if ((pMsg->param2 == SCAN_CR_PRESSED) ||
                 (pMsg->param2 == SCAN_CR_RELEASED))
        {
            pMsg->param1 = ID_BOMBER;
        }
        else if ((pMsg->param2 == SCAN_CRA_PRESSED) ||
                 (pMsg->param2 == SCAN_CRA_RELEASED))
        {
            pMsg->param1 = ID_EM_WAVE;
        }
    }
}

/****************************************************************************
  Function:
    WORD ProcessMessageJet( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
  Description:
    This function processes the messages for the Bomber Jet screen. It allows
    the user to maneuver the Jet left, right, up, down and fire bullets.
  Precondition:
    Call ShowScreenJet() prior to using this function to display the
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
WORD ProcessMessageJet(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
    WORD    controlID;

    controlID = GetObjID(pObj);

    switch (controlID)
    {
        case ID_SHIP:
            // Perform  processing to move the ship
		    SetColor(BLACK);        // set color to BLACK
		    FillBevel((leftJet),(topJet),(leftJet+27),(topJet+12), 0);		    
			leftJet += jetX;
			topJet += (-jetY);				
			if(leftJet < 0) 
				leftJet = 0; 
			else if(leftJet > 128-27) 
				leftJet = 128-27;			
			if(topJet < 0) 
				topJet = 0;
			else if(topJet > 64-12) 
				topJet = 64-12;
			PutImage(leftJet, topJet, &iconBomberJet, 1);      
            break;

        case ID_BOMBER:
            // Fire the missile
            if (!missile.status.active)
            {
                missile.status.active    = TRUE;
                missile.status.terminate = FALSE;
                missile.angle            = 0;
                missile.positionX        = leftJet+27;
                missile.positionY        = topJet+6;;
                missile.updateInterval   = 1;
                missile.deltaX           = 0;
                missile.deltaY           = 0;
                missile.step             = 0;
                
			   	if(enableSoundFlag)
					if(!speakerBusy())
						speakerActivate(SPEECH_ADDR_SNAKE_EAT, SPEECH_SIZE_SNAKE_EAT);
						
				score -= 1;
            }
            break;
            
        case ID_EM_WAVE:
        	if(!EMWaveFlag)
        	{
        		tempjetX = leftJet+14;
				tempjetY = topJet+6;
			}
		    EMWaveFlag = 1;
		    score -= 1;
            break;
    }
    return 0;
}

/****************************************************************************
  Function:
    void DrawJet( void )
  Description:
    This function updates the screen during the Bomber Jet game.  Sreen is 
    updated on one tick intervals.
  Precondition:
    The variables missile and asteroid must be valid.
  Parameters:
    None
  Returns:
    None
  Remarks:
    To slow the game, either change the update test (previousTick != tick) or
    the update intervals.
  ***************************************************************************/
void DrawJet(void)
{
	int k;	

    if(EMWaveFlag)
    {  		
	    SetColor( WHITE  );	
	    Circle(tempjetX, tempjetY, 5*EMWaveCount);
	   	SetColor( BLACK  );	
	    Circle(tempjetX, tempjetY, 5*prevRadius);

	    prevRadius = EMWaveCount;	   		
   		if(EMWaveCount++ == 5)
   		{
	   		Circle(tempjetX, tempjetY, 5*prevRadius);
   			for (k=0; k<NUM_ACTIVE_ALIENS; k++)
   			{
	            if (alienmissile[k].status.active)
	            	if(abs(alienmissile[k].positionX-tempjetX) < 5*EMWaveCount && abs(alienmissile[k].positionY-tempjetY) < 5*EMWaveCount )
	            	{
	                	alienmissile[k].status.active    = FALSE;
	                	alienmissile[k].status.terminate = TRUE;
	                	SetColor(BLACK);
	                    Circle( alienmissile[k].positionX, alienmissile[k].positionY, MISSILE_SIZE );
	                }

	        }
   			for (k=0; k<NUM_ACTIVE_ASTEROIDS; k++)
   			{	        
	             if (asteroid[k].status.active)
	            	if(abs(asteroid[k].positionX-tempjetX) < 5*EMWaveCount && abs(asteroid[k].positionY-tempjetY) < 5*EMWaveCount )
	            	{
	                	asteroid[k].status.active    = FALSE;
	                	asteroid[k].status.terminate = TRUE;
	                	SetColor(BLACK);
	                    FillCircle( asteroid[k].positionX, asteroid[k].positionY, asteroid[k].size );
	                }
	        }
	       	EMWaveCount = 0;
   			EMWaveFlag = 0;         
   		}
   		PutImage(leftJet, topJet, &iconBomberJet, 1);      
   	}
   		
    activemissileJet = missile.status.active;    	
        
    if (previousTick != tick)
    {
        int     i;
        int     j;

		if(score > BJ_SCORE_MAX)
		{
		    strcpy( pageInformation.title, "Bomber Jet" );
		    strcpy( scoreInfo, "Mission accomplished!" );
		    activemissileJet = 0;
		    EMWaveFlag = 0;
		    EMWaveCount = 0;
		    countalienmissile = 0;
			leftJet=0, rightJet=0, topJet=0, downJet=0;
			prevRadius = 0;
		    SwitchS1Flag = 0;
		    SwitchS2Flag = 0;
		    screenState = SCREEN_DISPLAY_SCORE;		
		}

        // Check for Bomber Jet missile hits on asteroids and alien ships
        if (missile.status.active)
        {
            for (i=0; i<NUM_ACTIVE_ASTEROIDS; i++)
            {
                if (asteroid[i].status.active)
                {
                    if ((abs( asteroid[i].positionX - missile.positionX) < (asteroid[i].size + MISSILE_SIZE)) &&
                        (abs( asteroid[i].positionY - missile.positionY) < (asteroid[i].size + MISSILE_SIZE)))
                    {
                        score +=    (scoreSpeed[asteroid[i].speedIndex] +
                                     scoreSize[asteroid[i].sizeIndex] +
                                     missile.step)/4;                                   
                        asteroid[i].status.terminate    = TRUE;
                        missile.status.terminate        = TRUE;
					   	if(enableSoundFlag)
							if(!speakerBusy())
								speakerActivate(SPEECH_ADDR_SNAKE_MOVE, SPEECH_SIZE_SNAKE_MOVE);
                    }
                }
            }
            for (i=0; i<NUM_ACTIVE_ALIENS; i++)
            {
                if (aliens[i].status.active)
                {
                    if ((abs( aliens[i].positionX - missile.positionX) < (aliens[i].size + MISSILE_SIZE)) &&
                        (abs( aliens[i].positionY - missile.positionY) < (aliens[i].size + MISSILE_SIZE)))
                    {
                        score +=    (scoreSpeed[aliens[i].speedIndex] +
                                     scoreSize[aliens[i].sizeIndex] +
                                     missile.step)/4;
                        aliens[i].status.terminate      = TRUE;
                        missile.status.terminate        = TRUE;
					   	if(enableSoundFlag)
							if(!speakerBusy())
								speakerActivate(SPEECH_ADDR_SNAKE_MOVE, SPEECH_SIZE_SNAKE_MOVE);                       
                    }
                }
            }
        }

        for (i=0; i<NUM_ACTIVE_ASTEROIDS-1; i++)
        {
	        // Check for asteroid vs. asteroid collisions
            if (asteroid[i].status.active)
            {
                for (j=i+1; j<NUM_ACTIVE_ASTEROIDS; j++)
                {
                    if (asteroid[j].status.active)
                    {
                        if ((abs( asteroid[i].positionX - asteroid[j].positionX) < (asteroid[i].size + asteroid[j].size)) &&
                            (abs( asteroid[i].positionY - asteroid[j].positionY) < (asteroid[i].size + asteroid[j].size)))
                        {
                            asteroid[i].status.terminate    = TRUE;
                            asteroid[j].status.terminate    = TRUE;
                        }
                    }
                }
            }
            
            // Check for alien ship vs. asteroid collisions
            if (aliens[i].status.active)
            {
                for (j=i+1; j<NUM_ACTIVE_ALIENS; j++)
                {
                    if (aliens[j].status.active)
                    {
                        if ((abs( asteroid[i].positionX - aliens[j].positionX) < (asteroid[i].size + aliens[j].size)) &&
                            (abs( asteroid[i].positionY - aliens[j].positionY) < (asteroid[i].size + aliens[j].size)))
                        {
                            asteroid[i].status.terminate    = TRUE;
                            aliens[j].status.terminate    = TRUE;
                        }
                    }
                } 
        	}              	
            
        }

        // Check for Bomber Jet vs. asteroid collision
        for (i=0; i<NUM_ACTIVE_ASTEROIDS; i++)
        {
            if (asteroid[i].status.active)
            {
                if ((abs( asteroid[i].positionX - (leftJet+14)) < (asteroid[i].size+7)) &&
                    (abs( asteroid[i].positionY - (topJet+6)) < (asteroid[i].size+3)))
                {
                    // Hit!  We're dead.  Adjust the score, and set the screen state to
                    // end the game.
                    strcpy( pageInformation.title, "Bomber Jet" );
                    strcpy( scoreInfo, "Collided with an asteroid!" );
                    activemissileJet = 0;
                    EMWaveFlag = 0;
                    EMWaveCount = 0;
                    countalienmissile = 0;
					leftJet=0, rightJet=0, topJet=0, downJet=0;
					prevRadius = 0;
                    SwitchS1Flag = 0;
                    SwitchS2Flag = 0;
                    screenState = SCREEN_DISPLAY_SCORE;
                }
            }
        }     
        
 		for (k=0; k<NUM_ACTIVE_ALIENS; k++)
 		{
	 		// Check for Bomber Jet vs. alien missile collision
            if (alienmissile[k].status.active)
            {
	           	if(abs(alienmissile[k].positionX-(leftJet+14)) < MISSILE_SIZE+14 && abs(alienmissile[k].positionY-(topJet+6)) < MISSILE_SIZE+6 )
	           	{
	                    strcpy( pageInformation.title, "Bomber Jet" );
	                    strcpy( scoreInfo, "Hit by a missile!" );
	                    activemissileJet = 0;
	                    EMWaveFlag = 0;
	                    EMWaveCount = 0;
	                    countalienmissile = 0;
						leftJet=0, rightJet=0, topJet=0, downJet=0;
						prevRadius = 0;	                    
	                    SwitchS1Flag = 0;
                    	SwitchS2Flag = 0;
	                    screenState = SCREEN_DISPLAY_SCORE;
	            }
	        }
	        
	        // Check for Bomber Jet vs. alien ship collision
	        if (aliens[k].status.active)
            {
	           	if(abs(aliens[k].positionX-(leftJet+14)) < (aliens[k].size+7) && abs(aliens[k].positionY-(topJet+6)) < (aliens[k].size+3) )
	           	{
	                    strcpy( pageInformation.title, "Bomber Jet" );
	                    strcpy( scoreInfo, "Collided with an alien ship!" );
	                    activemissileJet = 0;
	                    EMWaveFlag = 0;
	                    EMWaveCount = 0;
	                    countalienmissile = 0;
						leftJet=0, rightJet=0, topJet=0, downJet=0;
						prevRadius = 0;	                    
	                    SwitchS1Flag = 0;
                    	SwitchS2Flag = 0;
	                    screenState = SCREEN_DISPLAY_SCORE;
	            }
	        }
	        

	 		// Check for asteroid vs. alien missile collision
            if (alienmissile[k].status.active)
            {
	            for (i=0; i<NUM_ACTIVE_ASTEROIDS; i++)
		           	if(abs(alienmissile[k].positionX-asteroid[i].positionX) < MISSILE_SIZE+2 && abs(alienmissile[k].positionY-asteroid[i].positionY) < MISSILE_SIZE+2)
		           	{
	                            asteroid[i].status.terminate    	= TRUE;
	                            alienmissile[k].status.terminate    = TRUE;
		            }
	        }	        
       }
	        

        // Move the Bomber Jet missile if it is present
        if (missile.status.active)
        {
            if (--missile.updateInterval == 0)
            {
                // Reset the interval
                missile.updateInterval = UPDATE_MISSILE;

                // Erase the old missile
                SetColor(BLACK);
                Bar( missile.positionX, missile.positionY, missile.positionX+MISSILE_SIZE+8, missile.positionY+MISSILE_SIZE );

                if (missile.status.terminate)
                {
                    missile.status.active = FALSE;
                }
                else
                {
                    // Move the missile.  We are moving from (SHIP_CENTER_X, SHIP_CENTER_Y) to (SHIP_CENTER_X+deltaX, SHIP_CENTER_Y+deltaY)
                    missile.step = FindNextPosition(
                                        leftJet+27, topJet+6,
                                        DISP_HOR_RESOLUTION, topJet+6,
                                        missile.step,
                                        &missile.positionX,
                                        &missile.positionY );
                                        

                    // If it is still on the screen, draw the new missile.  Otherwise, the pilot can fire again.
                    if ((missile.positionX < MISSILE_SIZE) || ( missile.positionX > (GetMaxX()-MISSILE_SIZE)) ||
                        (missile.positionY < MISSILE_SIZE) || ( missile.positionY > (GetMaxY()-MISSILE_SIZE)))
                    {
                        missile.status.active = FALSE;
                    }
                    else
                    {
                        SetColor(WHITE);
                        Rectangle(missile.positionX,missile.positionY,missile.positionX+MISSILE_SIZE+8,missile.positionY+MISSILE_SIZE);
                    }
                }
            }
        }

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
                angle                           = Random( (QUADRANT_POSITIONS*2)-1 );
                startAxis                       = Random( 4-1 );
                switch( startAxis )
                {
                  	case 0:
                    case 1:
                        // Quadrant 1 and 2 angles
                        angle += QUADRANT_POSITIONS;
                        asteroid[i].initialX    = GetMaxX() - asteroid[i].size;
                        asteroid[i].initialY    = Random( GetMaxY()-1 - 2*asteroid[i].size ) + asteroid[i].size;
                        asteroid[i].finalX      = 2*(asteroid[i].initialX + RdiaCosine( angle ));
                        asteroid[i].finalY      = 2*(asteroid[i].initialY + RdiaSine( angle ));
                        break;
                    case 2:
                        // Quadrant 0 and 1 angles
                        asteroid[i].initialX    = GetMaxX()/2 + (Random( GetMaxX()-1 - 2*asteroid[i].size ) + asteroid[i].size)/2 - 16;
                        asteroid[i].initialY    = asteroid[i].size;
                        asteroid[i].finalX      = 2*(asteroid[i].initialX + RdiaCosine( angle ));
                        asteroid[i].finalY      = 2*(asteroid[i].initialY + RdiaSine( angle ));
                        break;
                    case 3:
                        // Quadrant 2 and 3 angles
                        angle += QUADRANT_POSITIONS*2;
                        asteroid[i].initialX    = GetMaxX()/2 + (Random( GetMaxX()-1 - 2*asteroid[i].size ) + asteroid[i].size)/2 - 16;
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

                    // Erase the old missile
                    SetColor(BLACK);
                    Bar( asteroid[i].positionX-asteroid[i].size, asteroid[i].positionY-asteroid[i].size, asteroid[i].positionX+asteroid[i].size, asteroid[i].positionY+asteroid[i].size );

                    if (asteroid[i].status.terminate)
                    {
                        asteroid[i].status.active = FALSE;
                    }
                    else
                    {
                        // Move the missile
                        asteroid[i].step = FindNextPosition(
                                            asteroid[i].initialX, asteroid[i].initialY,
                                            asteroid[i].finalX, asteroid[i].finalY,
                                            asteroid[i].step,
                                            &asteroid[i].positionX,
                                            &asteroid[i].positionY );

                        // If it is still on the screen, draw the new missile.  Otherwise, the pilot can fire again.
                        if ((asteroid[i].positionX < asteroid[i].size) || ( asteroid[i].positionX > (GetMaxX()-asteroid[i].size)) ||
                            (asteroid[i].positionY < asteroid[i].size) || ( asteroid[i].positionY > (GetMaxY()-asteroid[i].size)))
                        {
                            asteroid[i].status.active = FALSE;
                        }
                        else
                        {                      
                            SetColor( pShip->hdr.pGolScheme->TextColor0 );
                            SetColor(WHITE);
                            FillCircle( asteroid[i].positionX, asteroid[i].positionY, asteroid[i].size );
                        }
                    }
                }
            }
        }

		if(countalienmissile++ == 0x0800)
		{
			for (i=0; i<NUM_ACTIVE_ALIENS; i++)
	            if (!alienmissile[i].status.active)
	            {
	                alienmissile[i].status.active    = TRUE;
	                alienmissile[i].status.terminate = FALSE;
	                alienmissile[i].angle            = 0;
	                alienmissile[i].positionX        = aliens[i].positionX-(MISSILE_SIZE/2);
	                alienmissile[i].positionY        = aliens[i].positionY-(MISSILE_SIZE/2);
	                alienmissile[i].updateInterval   = 1;
	                alienmissile[i].deltaX           = 1;
	                alienmissile[i].deltaY           = 1;
	                alienmissile[i].step             = 0;
	            }
	        countalienmissile = 0;
	     }
	     
    	// Move the alien ship missile if it is present
		for (i=0; i<NUM_ACTIVE_ALIENS; i++)
	        if (alienmissile[i].status.active)
	        {
	            if (--alienmissile[i].updateInterval == 0)
	            {
	                // Reset the interval
	                alienmissile[i].updateInterval = UPDATE_MISSILE_ALIEN;
	
	                // Erase the old missile
	                SetColor(BLACK);
	                Bar( alienmissile[i].positionX-MISSILE_SIZE, alienmissile[i].positionY-MISSILE_SIZE, alienmissile[i].positionX+MISSILE_SIZE, alienmissile[i].positionY+MISSILE_SIZE );
	
	                if (alienmissile[i].status.terminate)
	                {
	                    alienmissile[i].status.active = FALSE;
	                }
	                else
	                {
	                    // Move the missile
	                    alienmissile[i].step = FindNextPosition(
	                                        aliens[i].positionX, aliens[i].positionY,
	                                        0, GetMaxY()-aliens[i].positionY,
	                                        alienmissile[i].step,
	                                        &alienmissile[i].positionX,
	                                        &alienmissile[i].positionY );
	                    
	                    	
	                    // If it is still on the screen, draw the new missile.  Otherwise, the alien pilot can fire again.
	                    if ((alienmissile[i].positionX < MISSILE_SIZE) || ( alienmissile[i].positionX > (GetMaxX()-MISSILE_SIZE)) ||
	                        (alienmissile[i].positionY < MISSILE_SIZE) || ( alienmissile[i].positionY > (GetMaxY()-MISSILE_SIZE)))
	                    {
	                        alienmissile[i].status.active = FALSE;
	                    }
	                    else
	                    {
	                        SetColor( pShip->hdr.pGolScheme->TextColor0 );
	                        SetColor(WHITE);
	                        Circle( alienmissile[i].positionX, alienmissile[i].positionY, MISSILE_SIZE );
	                    }
	                }
	            }
	        }
        
        // Move the alien ships.  Create new ones if needed.            
        for (i=0; i<NUM_ACTIVE_ALIENS; i++)
        {
            if (!aliens[i].status.active)
            {
                SHORT   angle;
                SHORT   startAxis;

                // Create a new alien ship
                aliens[i].speedIndex          = Random( ASTEROID_UPDATE_SPEEDS-1 );
                aliens[i].speed               = aliens[i].speedIndex * ASTEROID_UPDATE_MULTIPLIER + ASTERIOD_UPDATE_OFFSET;
                aliens[i].sizeIndex           = Random( ASTEROID_SIZES-1 );
                aliens[i].size                = aliens[i].sizeIndex * ASTEROID_SIZE_MULTIPLIER + ASTEROID_SIZE_OFFSET;
                aliens[i].updateInterval      = aliens[i].speed;
                angle                         = Random( (QUADRANT_POSITIONS*2)-1 );
                startAxis                     = Random( 4-1 );
                
                switch( startAxis )
                {
	                case 0:
                    case 1:
                        // Quadrant 1 and 2 angles
                        angle += QUADRANT_POSITIONS;
                        aliens[i].initialX    = GetMaxX() - asteroid[i].size;
                        aliens[i].initialY    = Random( GetMaxY()-1 - 2*asteroid[i].size ) + asteroid[i].size;
                        aliens[i].finalX      = 2*(aliens[i].initialX + RdiaCosine( angle ));
                        aliens[i].finalY      = 2*(aliens[i].initialY + RdiaSine( angle ));
                        break;
                    case 2:
                        // Quadrant 0 and 1 angles.
                        asteroid[i].initialX    = GetMaxX()/2 + (Random( GetMaxX()-1 - 2*asteroid[i].size ) + asteroid[i].size)/2 - 16;
                        aliens[i].initialY    = asteroid[i].size;
                        aliens[i].finalX      = 2*(aliens[i].initialX + RdiaCosine( angle ));
                        aliens[i].finalY      = 2*(aliens[i].initialY + RdiaSine( angle ));
                        break;
                    case 3:
                        // Quadrant 2 and 3 angles
                        angle += QUADRANT_POSITIONS*2;
                        asteroid[i].initialX    = GetMaxX()/2 + (Random( GetMaxX()-1 - 2*asteroid[i].size ) + asteroid[i].size)/2 - 16;
                        aliens[i].initialY    = GetMaxY() - asteroid[i].size;
                        aliens[i].finalX      = 2*(aliens[i].initialX + RdiaCosine( angle ));
                        aliens[i].finalY      = 2*(aliens[i].initialY + RdiaSine( angle ));
                        break;
                }

                aliens[i].positionX           = aliens[i].initialX;
                aliens[i].positionY           = aliens[i].initialY;

                aliens[i].step                = 0;
                aliens[i].status.active       = TRUE;
                aliens[i].status.terminate    = FALSE;
            }
            else
            {
                if (--aliens[i].updateInterval == 0)
                {
                    // Reset the interval
                    aliens[i].updateInterval = aliens[i].speed;

                    // Erase the old missile
                    SetColor(BLACK);
                    Bar( aliens[i].positionX-aliens[i].size-5, aliens[i].positionY-aliens[i].size-5, aliens[i].positionX+aliens[i].size+5, aliens[i].positionY+aliens[i].size+5 );

                    if (aliens[i].status.terminate)
                    {
                        aliens[i].status.active = FALSE;
                    }
                    else
                    {
                        // Move the alien ship
                        aliens[i].step = FindNextPosition(
                                            aliens[i].initialX, aliens[i].initialY,
                                            aliens[i].finalX, aliens[i].finalY,
                                            aliens[i].step,
                                            &aliens[i].positionX,
                                            &aliens[i].positionY );

                        if ((aliens[i].positionX < aliens[i].size) || ( aliens[i].positionX > (GetMaxX()-aliens[i].size)) ||
                            (aliens[i].positionY < aliens[i].size) || ( aliens[i].positionY > (GetMaxY()-aliens[i].size)))
                        {
                            aliens[i].status.active = FALSE;
                        }
                        else
                        {                       
                            SetColor(WHITE);
                            Bevel( aliens[i].positionX-5, aliens[i].positionY-5, aliens[i].positionX+5, aliens[i].positionY+5, aliens[i].size );
                            FillCircle( aliens[i].positionX, aliens[i].positionY, aliens[i].size );
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
    to find interrum positions along the line, such as a missile along a
    trajectory.
  ***************************************************************************/
SHORT FindNextPosition(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT step, SHORT *x, SHORT *y)
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


/************************************************************************
 Snake Game													
 ************************************************************************/

/****************************************************************************
  Function:
    void ShowScreenSnake( void )
  Description:
    This function erases the currently displayed screen and replaces it with
    the screen for the Snake game.
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
void ShowScreenSnake(void)
{
    SHORT   i;
    SHORT   j;
    SHORT   k;

    GOLFree();
    SetColor(BLACK);        // set color to BLACK
    ClearDevice();          // set screen to all BLACK

    // Create hidden buttons to receive direction messages
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

    // Create the game grid
    InitializeRandomNumberGenerator();
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

    // Initialize the field and the grid  
    for (i=0; i<SNAKE_COLUMNS; i++)
    {
        for (j=0; j<SNAKE_ROWS; j++)
        {
            field[i][j].hasFood         = FALSE;
            field[i][j].hasBeenEaten    = FALSE;
            GridSetCell( gamesGrid, i, j, GRIDITEM_IS_BITMAP, NULL );
        }
    }

    // Initialize the snake   
    SNAKE_HEAD.column       = 0;
    SNAKE_HEAD.row          = 0;
    SNAKE_HEAD.direction    = DIRECTION_RIGHT;
    snake.length            = 1;

    // Place random food
    for (k=0; k<FIELD_PIECES/2; k++)
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
void TranslateMessageSnake(GOL_MSG *pMsg)
{
    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_ACCELEROMETER_PAD))
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

/****************************************************************************
  Function:
    WORD ProcessMessageSnake( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
  Description:
    This function processes the messages for the Snake game, to change the 
    snake's direction based on how the board is manuevered.  
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
WORD ProcessMessageSnake(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
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
    void DrawSnake( void )
  Description:
    This routine draws the snake as it moves on the field and eats food.  
  Precondition:
    None
  Parameters:
    None
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void DrawSnake(void)
{
    SHORT i;
    SHORT j;
    SHORT oldTail;

    if ((tick - previousTick) > SNAKE_UPDATE_INTERVAL)
    {
	   	if(enableSoundFlag)
	   	{
		    while(speakerBusy());
        	speakerActivate(SPEECH_ADDR_SNAKE_MOVE, SPEECH_SIZE_SNAKE_MOVE);		
        }
        // Move the snake
        SetState( gamesGrid, GRID_DRAW_ITEMS );

        // Check for a wall crash
        if (((SNAKE_HEAD.direction == DIRECTION_UP)    && (SNAKE_HEAD.row == 0)) ||
            ((SNAKE_HEAD.direction == DIRECTION_RIGHT) && (SNAKE_HEAD.column == SNAKE_COLUMNS-1)) ||
            ((SNAKE_HEAD.direction == DIRECTION_DOWN)  && (SNAKE_HEAD.row == SNAKE_ROWS-1)) ||
            ((SNAKE_HEAD.direction == DIRECTION_LEFT)  && (SNAKE_HEAD.column == 0)))
        {
            strcpy( pageInformation.title, "Snake" );
            strcpy( scoreInfo, "Snake Crashed!" );
            screenState = SCREEN_DISPLAY_SCORE;
            return;
        }      

        // Draw the snake on the field.  We record each segment in memory, but we only
        // have to draw the head, the second segment, and possibly the space that the
        // tail just left.

        oldTail = snake.length-1;
        if (field[snake.pieces[oldTail].column][snake.pieces[oldTail].row].hasBeenEaten)
        {	        
            // Clear the eaten flag
            field[snake.pieces[oldTail].column][snake.pieces[oldTail].row].hasBeenEaten = FALSE;
    		if(enableSoundFlag)
			{         
            	while(speakerBusy());
        		speakerActivate(SPEECH_ADDR_SNAKE_EAT, SPEECH_SIZE_SNAKE_EAT);
        	}
        	
            // Add a new piece at the tail.
            snake.pieces[snake.length] = snake.pieces[oldTail];
            GridSetCell( gamesGrid, snake.pieces[snake.length].column, snake.pieces[snake.length].row, GRIDITEM_IS_BITMAP|GRIDITEM_DRAW, (void *)&snakeBody );
            snake.length++;
            score += 10;
            if (snake.length == LONGEST_SNAKE)
            {
                score += 100;
                strcpy( pageInformation.title, "Snake" );
                strcpy( scoreInfo, "Snake is full grown!" );
                screenState = SCREEN_DISPLAY_SCORE;
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
                    strcpy( pageInformation.title, "Snake" );
                    strcpy( scoreInfo, "Snake in a Knot!" );
                    screenState = SCREEN_DISPLAY_SCORE;
                    return;
                }
            }
        }

        // See if the snake is eating food.
        if (field[SNAKE_HEAD.column][SNAKE_HEAD.row].hasFood)
        {        
            field[SNAKE_HEAD.column][SNAKE_HEAD.row].hasFood = FALSE;
            field[SNAKE_HEAD.column][SNAKE_HEAD.row].hasBeenEaten = TRUE;

            // Put another piece of food on.  Let it be anywhere that doesn't
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


/************************************************************************
 Score Screen
 ************************************************************************/

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
void ShowScreenScore(void)
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

    strcpy( pageInformation.instructions, "Press switch to continue..." );
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
    This routine translates the switch message for the score display screen.
  Precondition:
    None
  Parameters:
    GOL_MSG *pMsg   - The current system message
  Return Values:
    None
  Remarks:
    None
  ***************************************************************************/
void TranslateMessageScore(GOL_MSG *pMsg)
{
    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_ACCELEROMETER_PAD))
      	{
	      	if (pMsg->param2 == SCAN_CR_PRESSED)
		    {
		        pMsg->param1 = ID_QUIT;
		        pMsg->param2 = SCAN_CR_PRESSED;
		    }
	        else    if (pMsg->param2 == SCAN_CRA_PRESSED)
	        {
	            pMsg->param1 = ID_QUIT;
	            pMsg->param2 = SCAN_CRA_PRESSED;
        	}
        }

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
WORD ProcessMessageScore(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
	WORD    controlID;

    controlID = GetObjID(pObj);

    switch (controlID)
    {
        case ID_QUIT:
	        if(enableSoundFlag)
	        {
				while(speakerBusy());
				speakerActivate(SPEECH_ADDR_SELECT, SPEECH_SIZE_SELECT);  
			} 
	    	screenState = SCREEN_DISPLAY_GAMES;
	    	break;
	 }

    return 0;
}


