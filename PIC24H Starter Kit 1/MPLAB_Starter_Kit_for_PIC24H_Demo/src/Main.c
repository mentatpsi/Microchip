/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		Main.c
* Description:			This file contains the main entry function which
* Dependencies:    		Header (.h) files if applicable, see below
* Processor:       		PIC24FJ128GP504
* Compiler:        		MPLAB® C30 v3.11b or higher
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
#include "Main.h"

/************************************************************************
 Configuration Bits													
 ************************************************************************/
_FOSCSEL(FNOSC_FRC);			// FRC Oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON  & POSCMD_NONE); 
								// Clock Switching is enabled and Fail Safe Clock Monitor is disabled
								// OSC2 Pin Function: OSC2 is Clock Output
								// Primary Oscillator Mode: Disabled
_FWDT(FWDTEN_OFF);             	// Watchdog Timer enabled/disabled by user software
								// (LPRC can be disabled by clearing SWDTEN bit in RCON register
_FICD(ICS_PGD1 & JTAGEN_OFF);	// Communication Channel: PGC1/EMUC1 and PGD1/EMUD1
								// JTAG disabled

/************************************************************************
 Global Variables														 
 ************************************************************************/
int 			 				enableSoundFlag = 1;
GOL_SCHEME          			*pScheme;
DWORD               			displayChangeTime;
PAGE_INFORMATION    			pageInformation;
SCREEN_STATES       			screenState = SCREEN_START;
volatile DWORD      			tick = 0;
DWORD               			previousTick = 0;
int 							slowFlag = 0;
static void 					*picture;
extern IMAGE_FLASH 			    iconMicrochip;
extern IMAGE_FLASH 			    intro;
extern IMAGE_FLASH 			    introDevice;

/************************************************************************
 Main																
 ************************************************************************/
int main( void )
{
	GOL_MSG msg;                                // Instance of GOL message structure 
												// to interact with Graphics Object layer
	    
	// Configure the Device Clock
	// Configure FRC to operate the device at 40MIPS
	// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
	// Fosc= 7.37M*43/(2*2)=79.2275Mhz for 40MIPS input clock
	PLLFBD = 41;								// M=43
	CLKDIVbits.PLLPOST = 0;						// N1=2
	CLKDIVbits.PLLPRE = 0;						// N2=2
	OSCTUN = 0;									// Tune FRC oscillator, if FRC is used
	RCONbits.SWDTEN = 0;						// Disable Watch Dog Timer

	// Clock switch to incorporate PLL
	__builtin_write_OSCCONH(0x01);				// Initiate Clock Switch to
												// FRC with PLL (NOSC=0b001)
	__builtin_write_OSCCONL(0x01);				// Start clock switching
	while(OSCCONbits.COSC != 0b001);			// Wait for Clock switch to occur
	while(OSCCONbits.LOCK!=1);					// Wait for PLL to lock
	
	// Starter Kit initialization delay
	Delay(2000);
	
	// Initialize Speaker and Switches
	PIC24HSKInit();
       		
    // Initialize the OLED module and Microchip Graphics Library
    GOLInit();     	
	picture = &introDevice;

    while (1)
    {
        if (GOLDraw())                          // Draw the screen
        {
            AccelerometerMsg(&msg); 	      	// Get the raw accelerometer message
            if ((msg.uiEvent != EVENT_INVALID) && ((tick - displayChangeTime) > MESSAGE_DEAD_TIME))
            {
                TranslateAccelerometer(&msg);   // Translate the raw accelerometer message
                GOLMsg(&msg);                   // Process the message through to Graphics Object layer 
            }
        }
    }
}

/****************************************************************************
  Function:
    WORD GOLDrawCallback( void )
  Description:
    This callback is performed at the beginning of the GOLDraw() function.
    Processing that needs to be done on a regular basis and is not triggered
    by a message should be done here.
  Precondition:
    None
  Parameters:
    None
  Returns:
    None
  Remarks:
    All graphic control adding and removing must be done from this function,
    not from the message callback.
  ***************************************************************************/
WORD GOLDrawCallback( void )
{
	
    switch (screenState)
    {
	    // Display Start-up screen and play Welcome message
        case SCREEN_START:
            // Display the start up screen
		    PictCreate(     ID_ICON,                    // ID
		                    0,0,GetMaxX(),GetMaxY(),    // dimension
		                    PICT_DRAW,                  // will be displayed, has frame
		                    1,                          // scale factor is x1
		                    picture,                    // bitmap
		                    NULL );                     // default GOL scheme
        	screenState     = SCREEN_START_DELAY;
            break;
            
        case SCREEN_START_DELAY:
        	if(enableSoundFlag)
        	{
        		while(speakerBusy());
        		// Speaker: Welcome to MPLAB Starter Kit for PIC24H MCU Demonstration
        		speakerActivate(SPEECH_ADDR_WELCOME, SPEECH_SIZE_WELCOME);		
        		Delay(6000);
        	}
        	// Initialize the reference timer
    		TickInit();
			// Initialize the accelerometer
    		AccelerometerInit();
        	screenState     = SCREEN_DISPLAY_MAIN;
        	break;
        
        // Display Main screen                
        case SCREEN_DISPLAY_MAIN:
            ShowScreenMain();            
            screenState         = SCREEN_MAIN;
            displayChangeTime   = tick;
            break;
               	
        case SCREEN_MAIN:
            break;
        
        // Display Accelerometer Tri-axial Outputs graph	            
        case SCREEN_DISPLAY_GRAPH:
            ShowScreenGraph();
            screenState         = SCREEN_GRAPH;
            displayChangeTime   = tick;
            break;

        case SCREEN_GRAPH:
        	AccelerometerStop();			
            UpdateGraph();
            break;
		
		// Display connect External Sensor information
		case SCREEN_DISPLAY_ES_INFO:
		  	ShowScreenESInfo();
		  	screenState 		= SCREEN_ES_INFO;
		  	displayChangeTime   = tick;
		  	break;

		case SCREEN_ES_INFO:
			Delay(2000);
		  	screenState = SCREEN_DISPLAY_ES_GRAPH;
		  	break;
		
		// Display External Sensor output graph   			
        case SCREEN_DISPLAY_ES_GRAPH:
        	AccelerometerStop();
            ShowScreenESGraph();                      
            initESADC();
            screenState         = SCREEN_ES_GRAPH;
            displayChangeTime   = tick;
            break;

        case SCREEN_ES_GRAPH:
	        UpdateESGraph();
            break;
        
        // Display Orientation screen     
        case SCREEN_DISPLAY_ORIENTATION:
            ShowScreenOrientation();
            screenState         = SCREEN_ORIENTATION;
            displayChangeTime   = tick;
            break;

        case SCREEN_ORIENTATION:
            UpdateOrientation();
            break;
 
 		// Display Games screen
        case SCREEN_DISPLAY_GAMES:
            ShowScreenGames();
            screenState         = SCREEN_GAMES;
            displayChangeTime   = tick;
            break;

        case SCREEN_GAMES:
            break;
        
        // Display Bomber Jet screen                         
        case SCREEN_DISPLAY_JET:
            ShowScreenJet();
            AccelerometerStop();
        	AccelerometerInit();    
         	if(enableSoundFlag)
         	{
	         	while(speakerBusy());
				speakerActivate(SPEECH_ADDR_START, SPEECH_SIZE_START); 
				while(speakerBusy());
			}
            screenState         = SCREEN_JET;
            displayChangeTime   = tick;
            break;

        case SCREEN_JET:    	
            DrawJet();
            break;
        
        // Display Snake screen    
        case SCREEN_DISPLAY_SNAKE:
            ShowScreenSnake();
            AccelerometerStop();
        	AccelerometerInit();   
        	if(enableSoundFlag)
         	{
	         	while(speakerBusy());
				speakerActivate(SPEECH_ADDR_START, SPEECH_SIZE_START); 
				while(speakerBusy());
			} 
            screenState         = SCREEN_SNAKE;
            displayChangeTime   = tick;
            break;

        case SCREEN_SNAKE:    	
            DrawSnake();
            break;
            
        // Display Score screen    
        case SCREEN_DISPLAY_SCORE:
            ShowScreenScore();
            if(enableSoundFlag)
            {
				while(speakerBusy());
				speakerActivate(SPEECH_ADDR_GAME, SPEECH_SIZE_GAME); 
				while(speakerBusy());
				speakerActivate(SPEECH_ADDR_OVER, SPEECH_SIZE_OVER); 
			}
            displayChangeTime   = tick;
            screenState         = SCREEN_SCORE;			
            break;

        case SCREEN_SCORE:
            break;
    }

    return 1;   // Callback complete
}

/****************************************************************************
  Function:
    WORD GOLMsgCallback( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
  Description:
    This callback is executed when a graphics message is being processed.
    Based on the currently displayed screen, we determine the next operation.
  Precondition:
    None
  Parameters:
    WORD translatedMsg  - The message as translated by the processing
                            routine for the object type.
    OBJ_HEADER* pObj    - Pointer to the graphics object header.
    GOL_MSG* pMsg       - Original graphics message.
  Return Values:
    0 - Do not call the default message processing routine for the object.
    1 - Call the default message processing routine for the object.
  Remarks:
    This function is called before the default processing.  If default
    processing is required before other processing, it must be done
    explicitly in this function, and 0 must be returned.  If default
    processing can be performed after this function, simply return 1.
  ***************************************************************************/
WORD GOLMsgCallback( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    switch( screenState )
    {
	    case SCREEN_START:
            // No messages are processed in this state.
            break;
 
        case SCREEN_START_DELAY:
			// No messages are processed in this state.
        	break;
        	           
        case SCREEN_DISPLAY_MAIN:
            // This state exists for the draw callback only.
            break;

        case SCREEN_MAIN:
            return ProcessMessageMain( translatedMsg, pObj, pMsg );
            break;
                       
        case SCREEN_DISPLAY_GRAPH:
            // This state exists for the draw callback only.
            break;

        case SCREEN_GRAPH:
            return ProcessMessageGraph( translatedMsg, pObj, pMsg );
            break;

		case SCREEN_DISPLAY_ES_INFO:
			// This state exists for the draw callback only.
		  	break;

		case SCREEN_ES_INFO:
			// This state exists for the draw callback only.
		  	break;
		  	
        case SCREEN_DISPLAY_ES_GRAPH:
            // This state exists for the draw callback only.
            break;
                        
        case SCREEN_ES_GRAPH:        	        
            return ProcessMessageESGraph( translatedMsg, pObj, pMsg );
            break; 
            
        case SCREEN_DISPLAY_ORIENTATION:
            // This state exists for the draw callback only.
            break;
                        
        case SCREEN_ORIENTATION:
            return ProcessMessageOrientation( translatedMsg, pObj, pMsg );
            break; 
 
         case SCREEN_DISPLAY_GAMES:
            // This state exists for the draw callback only.
            break;

        case SCREEN_GAMES:
            return ProcessMessageGames( translatedMsg, pObj, pMsg );
            break;
                       
        case SCREEN_DISPLAY_SNAKE:
            // This state exists for the draw callback only.
            break;

        case SCREEN_SNAKE:
            return ProcessMessageSnake( translatedMsg, pObj, pMsg );
            break;
            
        case SCREEN_DISPLAY_JET:
            // This state exists for the draw callback only.
            break;

        case SCREEN_JET:
            return ProcessMessageJet( translatedMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_SCORE:
            // This state exists for the draw callback only.
            break;

        case SCREEN_SCORE:
            return ProcessMessageScore( translatedMsg, pObj, pMsg );
            break;
                                                       
        default:
            break;
    }

    return 1;
}

/****************************************************************************
  Function:
    void ShowScreenMain( void )
  Description:
    This function erases the currently displayed screen and replaces it with
    the main menu screen.
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
void ShowScreenMain( void )
{
    strcpy( pageInformation.title, "PIC24H Starter Kit" );

    strcpy( pageInformation.buttonTextTopLeft, "Acc. Graph" );
    strcpy( pageInformation.buttonTextTopRight, "Ext. Sensor" );
    strcpy( pageInformation.buttonTextBottomLeft, "Orientation" );
    strcpy( pageInformation.buttonTextBottomRight, "Games" );

    pageInformation.buttonBitmapTopLeft        = NULL;
    pageInformation.buttonBitmapTopRight       = NULL;
    pageInformation.buttonBitmapBottomLeft     = NULL;
    pageInformation.buttonBitmapBottomRight    = NULL;

    ShowScreenGeneric( );
}

/****************************************************************************
  Function:
    WORD ProcessMessageMain( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
  Description:
    This function processes the messages for the main introduction screen.
    It allows the user to select Accelerometer Graph, External Sensor Graph, 
    Orientation Display or Games.
  Precondition:
    Call ShowScreenMain() prior to using this function to display the correct
    screen.
  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information
  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.
  Remarks:
    The global screenState is updated to proceed to the user's selection.
  ***************************************************************************/
WORD ProcessMessageMain( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    WORD    controlID;

    if (translatedMsg == BTN_MSG_PRESSED)
    {
        if(enableSoundFlag)
        {
			if(!speakerBusy())
				speakerActivate(SPEECH_ADDR_SELECT, SPEECH_SIZE_SELECT);  
		} 
								
        controlID = GetObjID(pObj);

        switch (controlID)
        {
            case ID_MAIN_GRAPH:
	            screenState = SCREEN_DISPLAY_GRAPH;
	            break;
                
            case ID_MAIN_GAMES:
                screenState = SCREEN_DISPLAY_GAMES;
                break;

            case ID_MAIN_ORIENTATION:
	            screenState = SCREEN_DISPLAY_ORIENTATION;
                break;

            case ID_MAIN_EXTSENSOR:
	            screenState = SCREEN_DISPLAY_ES_INFO;
                break;
        }
    }
    return 0;  
    
}

/****************************************************************************
  Function:
    void ShowScreenGeneric( void )
  Description:
    This function erases the currently displayed screen and replaces it with
    a screen of a generic format.  The screen consists of:
            * the Microchip icon
            * a one line title
            * four buttons
            * one line of instructions
    The information for the screen contents is obtained from the global
    pageInformation structure.
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
void ShowScreenGeneric( void )
{
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

    BtnCreate(      ID_TOP_RIGHT,               // ID
                    64,LOCATION_MIN_Y,125,38,2, // dimension and radius
                    BTN_DRAW,                   // will be displayed after creation
                    pageInformation.buttonBitmapTopRight, // bitmap
                    pageInformation.buttonTextTopRight,   // text
                    NULL );                     // Default scheme

    BtnCreate(      ID_BOTTOM_RIGHT,            // ID
                    64,40,125,62,2,             // dimension and radius
                    BTN_DRAW,                   // will be displayed after creation
                    pageInformation.buttonBitmapBottomRight,
                    pageInformation.buttonTextBottomRight,
                    NULL );                     // Default scheme

}

/****************************************************************************
  Function:
    void InitializeScreen( void )
  Description:
    This function erases the current list of graphics objects, clears the
    screen, puts the Microchip icon in the upper left corner of the screen,
    and displays a title at the top of the screen.  The title is obtained
    from the global pageInformation structure.
  Precondition:
    GOLInit() must be called prior to calling this function.
  Parameters:
    None
  Returns:
    None
  Remarks:
    The global list of graphics objects is destroyed and replaced.  Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/
void InitializeScreen( void )
{
    GOLFree();
    SetColor(BLACK);        // set color to BLACK
    ClearDevice();          // set screen to all BLACK

    PictCreate(     ID_ICON,                    // ID
                    0,0,15,15,                  // dimension
                    PICT_DRAW,                  // will be displayed, has frame
                    1,                          // scale factor is x1
                    &iconMicrochip,             // bitmap
                    NULL );                     // default GOL scheme

    StCreate(       ID_TITLE,
                    16, 0, GetMaxX()-11, GetTextHeight( (void *)&FONTDEFAULT )-1,
                    ST_DRAW,
                    pageInformation.title,
                    NULL );
                   
    previousTick = tick; 
}
    
/****************************************************************************
  Function:
    void TranslateMessageGeneric( GOL_MSG* msg )
  Description:
    This function translates the keypad message into the appropriate message
    for a generic, 4-button menu screen.  It can be used as the message
    processing function for any generic screen.
  Precondition:
    None
  Parameters:
    GOL_MSG * msg    - The current message information
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void TranslateMessageGeneric( GOL_MSG * pMsg )
{

    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_ACCELEROMETER_PAD))
    {
        switch (pMsg->param2)
        {
            case SCAN_UP_PRESSED:
                if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_PRESSED ))
                {
                    ClrState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ),     BTN_PRESSED | BTN_DRAW );
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED ))
                {
	                if(screenState != SCREEN_GAMES)
	                {
	                    ClrState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED );
	                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_DRAW );
	                    SetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ),    BTN_PRESSED | BTN_DRAW );
                    }
                }
                break;

            case SCAN_DOWN_PRESSED:
                if (GetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ), BTN_PRESSED ))
                {
                    ClrState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ),     BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ),     BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_PRESSED | BTN_DRAW );
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ), BTN_PRESSED ))
                {
	                if(screenState != SCREEN_GAMES)
	                {
	                    ClrState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ),    BTN_PRESSED );
	                    SetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ),    BTN_DRAW );
	                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED | BTN_DRAW );
	                }
                }
                break;

            case SCAN_RIGHT_PRESSED:
                if (GetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ), BTN_PRESSED ))
                {
	                if(screenState != SCREEN_GAMES)
	                {
	                    ClrState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ),     BTN_PRESSED );
	                    SetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ),     BTN_DRAW );
	                    SetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ),    BTN_PRESSED | BTN_DRAW );
                    }
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ), BTN_PRESSED ))
                {
                    ClrState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED | BTN_DRAW );
                }
                break;

            case SCAN_LEFT_PRESSED:
                if (GetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ), BTN_PRESSED ))
                {
	                if(screenState != SCREEN_GAMES)
	                {
	                    ClrState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ),    BTN_PRESSED );
	                    SetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ),    BTN_DRAW );
	                    SetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ),     BTN_PRESSED | BTN_DRAW );
	                }
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED ))
                {
                    ClrState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_DRAW );
                    SetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ),  BTN_PRESSED |  BTN_DRAW );
                }
                break;

            case SCAN_UP_RELEASED:
                break;

            case SCAN_DOWN_RELEASED:
                break;

            case SCAN_RIGHT_RELEASED:
                break;

            case SCAN_LEFT_RELEASED:
                break;

            case SCAN_CR_PRESSED:
                if (GetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_TOP_LEFT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_BOTTOM_LEFT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ), BTN_PRESSED ))
                {
	                if(screenState != SCREEN_GAMES)
               	    	pMsg->param1 = ID_TOP_RIGHT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_BOTTOM_RIGHT;
                }
                break;

            case SCAN_CR_RELEASED:
                if (GetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_TOP_LEFT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_BOTTOM_LEFT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ), BTN_PRESSED ))
                {
	                if(screenState != SCREEN_GAMES)
                    	pMsg->param1 = ID_TOP_RIGHT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_BOTTOM_RIGHT;
                }
                break;
                
            case SCAN_CRA_PRESSED:
                if (GetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_TOP_LEFT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_BOTTOM_LEFT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ), BTN_PRESSED ))
                {
	                if(screenState != SCREEN_GAMES)
                    	pMsg->param1 = ID_TOP_RIGHT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_BOTTOM_RIGHT;
                }
                break;

            case SCAN_CRA_RELEASED:
                if (GetState( (BUTTON *)GOLFindObject( ID_TOP_LEFT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_TOP_LEFT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_LEFT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_BOTTOM_LEFT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_TOP_RIGHT ), BTN_PRESSED ))
                {
	                if(screenState != SCREEN_GAMES)
                    	pMsg->param1 = ID_TOP_RIGHT;
                }
                else if (GetState( (BUTTON *)GOLFindObject( ID_BOTTOM_RIGHT ), BTN_PRESSED ))
                {
                    pMsg->param1 = ID_BOTTOM_RIGHT;
                }
                break;
        }
    }
}

/****************************************************************************
  Function:
    void TranslateAccelerometer( GOL_MSG* msg )
  Description:
    This function translates the accelerometer message into the appropriate message
    for the screen, based on what screen is currently displayed.
  Precondition:
    None
  Parameters:
    GOL_MSG* msg    - The current message information
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void TranslateAccelerometer( GOL_MSG* pMsg )
{
    switch( screenState )
    {
        case SCREEN_START:
            // Ignore all accelerometer and switch messages.
            pMsg->uiEvent = EVENT_INVALID;
            break;

        case SCREEN_START_DELAY:
			// No translation needed.
        	break;
        	            
        case SCREEN_DISPLAY_MAIN:
            // No translation needed.
            break;
            
        case SCREEN_MAIN:
            TranslateMessageGeneric( pMsg );
            break;
            
        case SCREEN_DISPLAY_SCORE:
            // No translation needed.
            break;
            
        case SCREEN_SCORE:
            TranslateMessageScore( pMsg );
            break;
            
        case SCREEN_DISPLAY_GRAPH:
            // No translation needed.
            break;
            
        case SCREEN_GRAPH:
            TranslateMessageGraph( pMsg );
            break;

		case SCREEN_DISPLAY_ES_INFO:
			// No translation needed.
		  	break;

		case SCREEN_ES_INFO:
			// No translation needed.
		  	break;
		  	            
        case SCREEN_DISPLAY_ES_GRAPH:
            // No translation needed.
            break;
            
        case SCREEN_ES_GRAPH:
            TranslateMessageESGraph( pMsg );
            break;
            
        case SCREEN_DISPLAY_ORIENTATION:
            // No translation needed.
            break;
            
        case SCREEN_ORIENTATION:
            TranslateMessageGraph( pMsg );
            break;  
                                 
        case SCREEN_DISPLAY_GAMES:
            // No translation needed.
            break;  
                     
        case SCREEN_GAMES:
            TranslateMessageGeneric( pMsg );
            break; 
                  
        case SCREEN_DISPLAY_SNAKE:
            // No translation needed.
            break;  
                      
        case SCREEN_SNAKE:
            TranslateMessageSnake( pMsg );
            break;  
                        
        case SCREEN_DISPLAY_JET:
            // No translation needed.
            break;   
                     
        case SCREEN_JET:
            TranslateMessageJet( pMsg );
            break;          
    }
}

/****************************************************************************
  Function:
    void TickInit( void )
  Description:
    This function sets up Timer 1 to generate an interrupt every 10 ms.
  Precondition:
    None
  Parameters:
    None
  Returns:
    None
  Remarks:
    The timer period and prescaler values are defined in HardwareProfile.h,
    since they are dependent on the oscillator speed.
  ***************************************************************************/
#define STOP_TIMER_IN_IDLE_MODE     0x2000
#define TIMER_SOURCE_INTERNAL       0x0000
#define TIMER_ON                    0x8000
#define GATED_TIME_DISABLED         0x0000
#define TIMER_16BIT_MODE            0x0000
#define TIMER_PRESCALER_1           0x0000
#define TIMER_PRESCALER_8           0x0010
#define TIMER_PRESCALER_64          0x0020
#define TIMER_PRESCALER_256         0x0030
#define TIMER_INTERRUPT_PRIORITY    0x1000

#define FS_1   			10000
#define TIMER_PERIOD    (unsigned int)((GetInstructionClock()/FS_1)-1)


void TickInit( void )
{
    TMR1 = 0;
    PR1 = TIMER_PERIOD;
    
   	T1CON = TIMER_ON | STOP_TIMER_IN_IDLE_MODE | TIMER_SOURCE_INTERNAL |
       	    GATED_TIME_DISABLED | TIMER_16BIT_MODE | TIMER_PRESCALER;

    IFS0bits.T1IF = 0;              //Clear flag
    IEC0bits.T1IE = 1;              //Enable interrupt
    T1CONbits.TON = 1;              //Run timer
}

/****************************************************************************
  Function:
    void TickStop( void )
  Description:
    This function stops Timer 1.
  Precondition:
    None
  Parameters:
    None
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void TickStop(void)
{
	T1CONbits.TON = 0;              //Stop timer
}

/****************************************************************************
  Function:
    void __attribute__((interrupt, shadow, auto_psv)) _T4Interrupt(void)
  Description:
    This function updates the tick count and calls ReadAccelerometer() 
    to monitor the accelerometer tri-axial outputs.
  Precondition:
    Timer 4 and the Timer 4 interrupt must be enabled in order for
    this function to execute.  AccelerometerInit() must be called before
    Timer 4 and the Timer 4 interrupt are enabled.
  Parameters:
    None
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void __attribute__((interrupt, shadow, auto_psv)) _T1Interrupt(void)
{
    // Clear flag
    IFS0bits.T1IF = 0;
    tick++;

	// Read accelerometer data
    ReadAccelerometer();
}


