/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		Orientation.c
* Description:			This file contains the main entry function which
* Dependencies:    		Header (.h) files if applicable, see below
* Processor:       		PIC24HJ128GP504
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
#include "Orientation.h"

/************************************************************************
 Variables													
 ************************************************************************/
SHORT 						polyPointsXP[16] = {40, 20, 55, 25, 55, 20, 65, 30, 55, 40, 55, 35, 40, 40, 40, 20};
SHORT 						polyPointsXN[16] = {110, 20, 95, 25, 95, 20, 85, 30, 95, 40, 95, 35, 110, 40, 110, 20};
SHORT 						polyPointsYP[16] = {63, 50, 68, 40, 63, 40, 73, 30, 83, 40, 78, 40, 83, 50, 63, 50};
SHORT 						polyPointsYN[16] = {63, 14, 68, 24, 63, 24, 73, 34, 83, 24, 78, 24, 83, 14, 63, 14};
int 						radiusZP = 30;
int 						radiusZN = 0;
int 						swingCounter = 0;
int 						dirX1 = 0, dirY1 = 0, dirZ1 = 0;
int 						countPortrait = ORIENTATION_COUNT, countLandscape = ORIENTATION_COUNT, countPlane = ORIENTATION_COUNT;
int 						prevdirX = 0, prevdirY = 0, prevdirZ = 0;

extern DWORD            	previousTick;
extern DWORD               	tick;
extern SCREEN_STATES       	screenState;
extern PAGE_INFORMATION    	pageInformation;
extern int 					orientationFlag;
extern int 					enableSoundFlag;
extern IMAGE_FLASH 			iconMicrochip;
extern int 					maxXValue;
extern int 					maxYValue;
extern int 					maxZValue;

/****************************************************************************
  Function:
    void ShowScreenOrientation( void )
  Description:
    This function erases the currently displayed screen and replaces it with
    the Orientation screen.  
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
void ShowScreenOrientation(void)
{
    strcpy( pageInformation.title, "Orientation" );
    strcpy( pageInformation.instructionsX, "X: 0g");
    strcpy( pageInformation.instructionsY, "Y: 0g");
    strcpy( pageInformation.instructionsZ, "Z: 0g");
    InitializeOrientationScreen();

    // Create the hidden controls to receive switch messages.

    BtnCreate(          ID_GRAPH_HOME,
                        GetMaxX()+1, 0, GetMaxX()+1, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    // Draw the initial grid.

    SetColor( WHITE );
    SetLineType( DOTTED_LINE );

    previousTick    = tick;
}

/****************************************************************************
  Function:
    void InitializeOrientationScreen( void )
  Description:
    This function initializes the GOL parameters for Orientation screen.
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
void InitializeOrientationScreen(void)
{   
    GOLFree();
    SetColor(BLACK);        // set color to BLACK
    ClearDevice();          // set screen to all BLACK

    StCreate(       ID_INSTRUCTIONSX,
                    0, (GRAPH_GRID_HEIGHT/6)-5, GetMaxX()/4+10, (GRAPH_GRID_HEIGHT/6)-5+GetTextHeight( (void *)&FONTDEFAULT )-1,
                    ST_DRAW,
                    pageInformation.instructionsX,
                    NULL );
               
    StCreate(       ID_INSTRUCTIONSY,
                    0, 3*(GRAPH_GRID_HEIGHT/6)-5, GetMaxX()/4+10, 3*(GRAPH_GRID_HEIGHT/6)-5+GetTextHeight( (void *)&FONTDEFAULT )-1,
                    ST_DRAW,
                    pageInformation.instructionsY,
                    NULL );    
               
    StCreate(       ID_INSTRUCTIONSZ,
                    0, 5*(GRAPH_GRID_HEIGHT/6)-5, GetMaxX()/4+10, 5*(GRAPH_GRID_HEIGHT/6)-5+GetTextHeight( (void *)&FONTDEFAULT )-1,
                    ST_DRAW,
                    pageInformation.instructionsZ,
                    NULL ); 
                    
    previousTick = tick; 
}

/****************************************************************************
  Function:
    void TranslateMessageOrientation( GOL_MSG *pMsg )
  Description:
    If the message is valid, this function translates the raw message
    pMsg->param1 to the proper message for the appropriate control.
  Precondition:
    ShowScreenOrientation() must be the last screen display routine called before
    this function executes.
  Parameters:
    GOL_MSG* pMsg - The original system message information
  Returns:
    None
  Remarks:
    None
  ***************************************************************************/
void TranslateMessageOrientation(GOL_MSG *pMsg)
{
    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_ACCELEROMETER_PAD))
    {
        if (pMsg->param2 == SCAN_CR_PRESSED)
        {
            pMsg->param1 = ID_GRAPH_HOME;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else if (pMsg->param2 == SCAN_CRA_PRESSED)
        {
            pMsg->param1 = ID_GRAPH_HOME;
            pMsg->param2 = SCAN_CRA_PRESSED;
        }       
    }
}

/****************************************************************************
  Function:
    WORD ProcessMessageOrientation( WORD translatedMsg, OBJ_HEADER* pObj,
                    		  GOL_MSG* pMsg )
  Description:
    This function processes the switch press message.
  Precondition:
    Call ShowScreenOrientation() prior to using this function to display the
    correct screen.
  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information
  Return Values:
    0 - Do not call the default message handler.
    1 - Call the default message handler.
  Remarks:
    None
  ***************************************************************************/
WORD ProcessMessageOrientation(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
    WORD    controlID;

    controlID = GetObjID(pObj);

    switch (controlID)
    {
        case ID_GRAPH_HOME:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
		        if(enableSoundFlag)
		        {
					if(!speakerBusy())
						speakerActivate(SPEECH_ADDR_SELECT, SPEECH_SIZE_SELECT);  
				} 
	            AccelerometerInit();
                screenState = SCREEN_DISPLAY_MAIN;
            }
            break;

        default:
            break;
    }

    return 0;
}

/****************************************************************************
  Function:
    void UpdateOrientation( void )
  Description:
    This routine updates the Orientation screen based on orientation of the board.
  Precondition:
    None
  Parameters:
    None
  Returns:
    None
  Remarks:
    None 
  ***************************************************************************/
void UpdateOrientation(void)
{
	OBJ_HEADER      *pObject;
				
	if(orientationFlag == 1)
	{
        pObject = GOLFindObject( ID_INSTRUCTIONSX );	            
        strcpy( pageInformation.instructionsX, "X: " );                
		appendStringXYZ(pageInformation.instructionsX, maxXValue);
		StSetText( (STATICTEXT *)pObject, pageInformation.instructionsX );
		SetState( pObject, ST_DRAW );

		pObject = GOLFindObject( ID_INSTRUCTIONSY );
		strcpy( pageInformation.instructionsY, "Y: " );
		appendStringXYZ(pageInformation.instructionsY, maxYValue);
		StSetText( (STATICTEXT *)pObject, pageInformation.instructionsY );		
		SetState( pObject, ST_DRAW );		

		pObject = GOLFindObject( ID_INSTRUCTIONSZ );
		strcpy( pageInformation.instructionsZ, "Z: " );
		appendStringXYZ(pageInformation.instructionsZ, maxZValue);
		StSetText( (STATICTEXT *)pObject, pageInformation.instructionsZ );									
              SetState( pObject, ST_DRAW );

		if(swingCounter > 10)
			swingCounter = 0;
		if(dirX1)
		{
			DrawArrowX(dirX1);
			if(abs(dirX1) == 1 && (prevdirY == 1 || prevdirZ == 1))
			{
				swingCounter = 0;
				RestoreX(1, polyPointsXP);
				RestoreX(-1, polyPointsXN);
				prevdirY = prevdirZ = 0;
				dirY1 = dirZ1 = 0;
			}	
			
		}	
		else if(dirY1)
		{
			DrawArrowY(dirY1);
			if(abs(dirY1) == 1 && (prevdirX == 1 || prevdirZ == 1))
			{
				swingCounter = 0;
				RestoreY(1, polyPointsYP);
				RestoreY(-1, polyPointsYN);	
				prevdirX = prevdirZ = 0;
				dirX1 = dirZ1 = 0;
			}
				
		}	
		else if(dirZ1)
		{
			DrawArrowZ(dirZ1);
			if(abs(dirZ1) == 1 && (prevdirX == 1 || prevdirY == 1))
			{
				swingCounter = 0;
				RestoreZ(1);
				RestoreZ(-1);
				prevdirX = prevdirY = 0;
				dirX1 = dirY1 = 0;
			}	
			
		}
		
		// Save the direction of acceleration
		prevdirX = abs(dirX1);
		prevdirY = abs(dirY1);
		prevdirZ = abs(dirZ1);
		
		dirX1 = dirY1 = dirZ1 = 0;
		orientationFlag = 0;
	}							

}

/****************************************************************************
  Function:
    void DrawArrowX(int vdirX)
  Description:
    This routine updates the Orientation screen based on orientation 
    of the board in X direction.
  Precondition:
    None
  Parameters:
    vdirX - Movement indicator in X direction.
  Returns:
    None
  Remarks:
    None 
  ***************************************************************************/
void DrawArrowX(int vdirX)
{

		SHORT* ppolyPoints = NULL;
		int i = 0;
		int factor = 0;
		int swing = 10;
		int skip = 2;
		int points = 16;
				
		if(countPortrait++ > ORIENTATION_COUNT)
		{
			countPortrait = 0;
			if(enableSoundFlag)
				if(!speakerBusy())
					speakerActivate(SPEECH_ADDR_PORTRAIT, SPEECH_SIZE_PORTRAIT);
		}
		if(vdirX == 1)
		{
			ppolyPoints = polyPointsXP;
			factor = 5;
			swing = 10;
			i = 0;
			skip = 2;
			points = 16;
		}
		else if(vdirX == -1)
		{
			ppolyPoints = polyPointsXN;
			factor = -5;
			swing = 10;			
			i = 0;
			skip = 2;
			points = 16;
		}	
		
				
		SetLineType( SOLID_LINE );
	    SetColor( WHITE  );	
	    DrawPoly(points/2, ppolyPoints);
	    Delay(50);
	   	SetColor( BLACK  );	
	    DrawPoly(points/2, ppolyPoints);

		for(; i < points; i+=skip)            
			*(ppolyPoints+i) += factor;
			
		if(swingCounter == swing)
	    {
		    swingCounter = 0;
		    	RestoreX(vdirX, ppolyPoints);
		}
		swingCounter++;	
}

/****************************************************************************
  Function:
    void DrawArrowY(int vdirY)
  Description:
    This routine updates the Orientation screen based on orientation 
    of the board in Y direction.
  Precondition:
    None
  Parameters:
    vdirX - Movement indicator in Y direction.
  Returns:
    None
  Remarks:
    None 
  ***************************************************************************/
void DrawArrowY(int vdirY)
{
		SHORT* ppolyPoints = NULL;
		int i = 1;
		int factor = 0;
		int swing = 5;
		int skip = 2;
		int points = 16;
		
		if(countLandscape++ > ORIENTATION_COUNT)
		{
			countLandscape = 0;
			if(enableSoundFlag)
				if(!speakerBusy())
					speakerActivate(SPEECH_ADDR_LANDSCAPE, SPEECH_SIZE_LANDSCAPE);
		}

					
		if(vdirY == 1)		
		{
			ppolyPoints = polyPointsYP;
			factor = -5;
		}
		else if(vdirY == -1)
		{
			ppolyPoints = polyPointsYN;
			factor = 5;
		}
			
		SetLineType( SOLID_LINE );
	    SetColor( WHITE  );	
	    DrawPoly(points/2, ppolyPoints);
	    Delay(50);
	   	SetColor( BLACK  );	
	    DrawPoly(points/2, ppolyPoints);
					
		for(; i < points; i+=skip)            
			*(ppolyPoints+i) += factor;
			
		if(swingCounter == swing)
	    {
		    swingCounter = 0;
		    RestoreY(vdirY,ppolyPoints);
		}
		swingCounter++;	
}

/****************************************************************************
  Function:
    void DrawArrowZ(int vdirZ)
  Description:
    This routine updates the Orientation screen based on orientation 
    of the board in Z direction.
  Precondition:
    None
  Parameters:
    vdirX - Movement indicator in Z direction.
  Returns:
    None
  Remarks:
    None 
  ***************************************************************************/
void DrawArrowZ(int vdirZ)
{
		int factor = 0;
		int swing = 5;
		int* radiusZ = NULL;
		
		if(countPlane++ > ORIENTATION_COUNT)
		{
			countPlane = 0;
			if(enableSoundFlag)
				if(!speakerBusy())
					speakerActivate(SPEECH_ADDR_PLANE, SPEECH_SIZE_PLANE);
		}
		
		if(vdirZ == 1)		
		{
			factor = -5;
			radiusZ = &radiusZP;			
		}
		else if(vdirZ == -1)
		{
			factor = 5;		
			radiusZ = &radiusZN;
		}
				
		SetLineType( SOLID_LINE );
	    SetColor( WHITE  );	
	    Circle(76, 32, *radiusZ);
	    Delay(50);
	   	SetColor( BLACK  );	
    	Circle(76, 32, *radiusZ);
	    *radiusZ += factor;
	    			
		if(swingCounter == swing)
	    {
		    swingCounter = 0;
		    	RestoreZ(vdirZ);

		}
		swingCounter++;	
}

/****************************************************************************
  Function:
    void RestoreX(int vdirX, SHORT* polyPoints)
  Description:
    This routine restores the initial position polygon array for X direction.
  Precondition:
    None
  Parameters:
    vdirX - Movement indicator in X direction.
    polyPoints - Address of the polygon array.
  Returns:
    None
  Remarks:
    None 
  ***************************************************************************/
void RestoreX(int vdirX, SHORT* polyPoints)
{
	if(vdirX == 1)
	{
		*(polyPoints) = 40;
	    *(polyPoints+1) = 20;
	    *(polyPoints+2) = 55;
	    *(polyPoints+3) = 25;
	    *(polyPoints+4) = 55;
	    *(polyPoints+5) = 20;
	    *(polyPoints+6) = 65;
	    *(polyPoints+7) = 30;
	    *(polyPoints+8) = 55;
	    *(polyPoints+9) = 40;
	    *(polyPoints+10) = 55;
	    *(polyPoints+11) = 35;
	    *(polyPoints+12) = 40;
	    *(polyPoints+13) = 40;
	    *(polyPoints+14) = 40;
	    *(polyPoints+15) = 20;
	}
	else if(vdirX == -1)
	{
		*(polyPoints) = 110;
	    *(polyPoints+1) = 20;
	    *(polyPoints+2) = 95;
	    *(polyPoints+3) = 25;
	    *(polyPoints+4) = 95;
	    *(polyPoints+5) = 20;
	    *(polyPoints+6) = 85;
	    *(polyPoints+7) = 30;
	    *(polyPoints+8) = 95;
	    *(polyPoints+9) = 40;
	    *(polyPoints+10) = 95;
	    *(polyPoints+11) = 35;
	    *(polyPoints+12) = 110;
	    *(polyPoints+13) = 40;
	    *(polyPoints+14) = 110;
	    *(polyPoints+15) = 20;
	}	
}

/****************************************************************************
  Function:
    void RestoreY(int vdirY, SHORT* polyPoints)
  Description:
    This routine restores the initial position polygon array for Y direction.
  Precondition:
    None
  Parameters:
    vdirY - Movement indicator in Y direction.
    polyPoints - Address of the polygon array.
  Returns:
    None
  Remarks:
    None 
  ***************************************************************************/
void RestoreY(int vdirY, SHORT* polyPoints)
{
	if(vdirY == 1)
	{
		*(polyPoints) = 63;
	    *(polyPoints+1) = 50;
	    *(polyPoints+2) = 68;
	    *(polyPoints+3) = 40;
	    *(polyPoints+4) = 63;
	    *(polyPoints+5) = 40;
	    *(polyPoints+6) = 73;
	    *(polyPoints+7) = 30;
	    *(polyPoints+8) = 83;
	    *(polyPoints+9) = 40;
	    *(polyPoints+10) = 78;
	    *(polyPoints+11) = 40;
	    *(polyPoints+12) = 83;
	    *(polyPoints+13) = 50;
	    *(polyPoints+14) = 63;
	    *(polyPoints+15) = 50;
	}
	else if(vdirY == -1)	
	{
		*(polyPoints) = 63;
	    *(polyPoints+1) = 14;
	    *(polyPoints+2) = 68;
	    *(polyPoints+3) = 24;
	    *(polyPoints+4) = 63;
	    *(polyPoints+5) = 24;
	    *(polyPoints+6) = 73;
	    *(polyPoints+7) = 34;
	    *(polyPoints+8) = 83;
	    *(polyPoints+9) = 24;
	    *(polyPoints+10) = 78;
	    *(polyPoints+11) = 24;
	    *(polyPoints+12) = 83;
	    *(polyPoints+13) = 14;
	    *(polyPoints+14) = 63;
	    *(polyPoints+15) = 14;
	}
}

/****************************************************************************
  Function:
    void RestoreZ(int vdirZ, SHORT* polyPoints)
  Description:
    This routine restores the initial position polygon array for Z direction.
  Precondition:
    None
  Parameters:
    vdirX - Movement indicator in Z direction.
    polyPoints - Address of the polygon array.
  Returns:
    None
  Remarks:
    None 
  ***************************************************************************/
void RestoreZ(int vdirZ)
{
	if(vdirZ == 1)
		radiusZP = 30;
	else if(vdirZ == -1)
		radiusZN = 0;
}

/****************************************************************************
  Function:
    int direction1X(int acc)
  Description:
    This routine detects acceleration in X direction.
  Precondition:
    None
  Parameters:
	acc - Movement value in X direction.
  Returns:
    None
  Remarks:
    None 
  ***************************************************************************/
int direction1X(int acc)
{
	if(acc < 570 && acc > 470)
		acc = 0;
	else if(acc >= 570)
		acc = +1;
	else if(acc <= 470)
		acc = -1;

	return acc;
}	

/****************************************************************************
  Function:
    int direction1Y(int acc)
  Description:
    This routine detects acceleration in Y direction.
  Precondition:
    None
  Parameters:
	acc - Movement value in Y direction.
  Returns:
    None
  Remarks:
    None 
  ***************************************************************************/
int direction1Y(int acc)
{
	if(acc < 570 && acc > 470)
		acc = 0;
	else if(acc >= 570)
		acc = +1;
	else if(acc <= 470)
		acc = -1;

	return acc;
}	

/****************************************************************************
  Function:
    int direction1Z(int acc)
  Description:
    This routine detects acceleration in Z direction.
  Precondition:
    None
  Parameters:
	acc - Movement value in Z direction.
  Returns:
    None
  Remarks:
    None 
  ***************************************************************************/
int direction1Z(int acc)
{
	if(acc < 570 && acc > 470)
		acc = 0;
	else if(acc >= 570)
		acc = +1;
	else if(acc <= 470)
		acc = -1;

	return acc;
}

/****************************************************************************
  Function:
    void appendStringXYZ(XCHAR* mainString, unsigned int value)
  Description:
    This routine appends the acceleration information string.
  Precondition:
    None
  Parameters:
	mainString - String to be appended by the acceleration information.
	value - Measurement of acceleration.
  Returns:
    None
  Remarks:
    None 
  ***************************************************************************/
void appendStringXYZ(XCHAR* mainString, unsigned int value)
{
	if(value>702)
		value = 702;
	else if(value<=436)
		value = 437;
		
	if(value>516 && value<=524)
		strcat( mainString, " 0.0g" );
	else if(value>524 && value<=532)
		strcat( mainString, "+0.1g" );
	else if(value>532 && value<=540)
		strcat( mainString, "+0.2g" );			
	else if(value>540 && value<=548)
		strcat( mainString, "+0.3g" );
	else if(value>548 && value<=556)
		strcat( mainString, "+0.4g" );
	else if(value>556 && value<=564)
		strcat( mainString, "+0.5g" );
	else if(value>564 && value<=572)
		strcat( mainString, "+0.6g" );
	else if(value>580 && value<=588)
		strcat( mainString, "+0.7g" );
	else if(value>588 && value<=596)
		strcat( mainString, "+0.8g" );
	else if(value>596 && value<=604)
		strcat( mainString, "+0.9g" );
	else if(value>604 && value<=702)
		strcat( mainString, "+1.0g" );
	else if(value>508 && value<=516)
		strcat( mainString, "-0.1g" );
	else if(value>500 && value<=508)
		strcat( mainString, "-0.2g" );			
	else if(value>492 && value<=500)
		strcat( mainString, "-0.3g" );
	else if(value>484 && value<=492)
		strcat( mainString, "-0.4g" );		
	else if(value>476 && value<=484)
		strcat( mainString, "-0.5g" );
	else if(value>468 && value<=476)
		strcat( mainString, "-0.6g" );		
	else if(value>460 && value<=468)
		strcat( mainString, "-0.7g" );		
	else if(value>452 && value<=460)
		strcat( mainString, "-0.8g" );	
	else if(value>444 && value<=452)
		strcat( mainString, "-0.9g" );
	else if(value>436 && value<=444)
		strcat( mainString, "-1.0g" );	
}

