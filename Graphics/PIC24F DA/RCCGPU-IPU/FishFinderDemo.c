/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        FishFinderDemo.c
 * Dependencies:    FishFinderDemo.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 01/11/10	    ...
 * 05/05/10	    Ported to Graphics Library Version 2.10
 *****************************************************************************/
#include "MainDemo.h"

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT'S IDs
/////////////////////////////////////////////////////////////////////////////
#define ID_FISHDEPTHMETER    0xFF10							// depth meter ID    
#define ID_FISHTEMPMETER     0xFF20							// temp meter ID    
#define ID_FISHSTATICTEXT1   0xFF30
#define ID_FISHSTATICTEXT2   0xFF31
#define ID_FISHMENUBTN1  	 0xFF40
#define ID_FISHMENUBTN2   	 0xFF41
#define ID_FISHLISTBOX		 0xFF50

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT DIMENSIONS DEFINES
/////////////////////////////////////////////////////////////////////////////

#define MENUHEIGHT			CTRLBTN_HEIGHT
#define SCALEWIDTH			28
#define SCALELINE_X_POS		(GetMaxX()-(SCALEWIDTH-20))
#define SCALETEXT_X_POS		(GetMaxX()-(SCALEWIDTH-1))

#define MV_AREA_START_X	  	0
#define MV_AREA_START_Y	  	0
#define MV_AREA_WIDTH	  	(GetMaxX()-SCALEWIDTH)
#define MV_AREA_END_X	  	(MV_AREA_START_X + MV_AREA_WIDTH)

#define TEXTAREA_WIDTH    	105
#define TEXTAREA_HEIGHT   	50
#define TEXTAREA_START_X  	MV_AREA_START_X
#define TEXTAREA_START_Y  	MV_AREA_START_Y	     
#define TEXTAREA_END_X    	(TEXTAREA_START_X + TEXTAREA_WIDTH - 1)
#define TEXTAREA_END_Y    	(TEXTAREA_START_Y + TEXTAREA_HEIGHT - 1)

// static text dimension
#define DMETER_WIDTH    	80
#define DMETER_HEIGHT   	28
#define DMETER_START_X  	TEXTAREA_START_X
#define DMETER_START_Y  	TEXTAREA_START_Y+1     
#define DMETER_END_X    	(DMETER_START_X + DMETER_WIDTH - 1)
#define DMETER_END_Y    	(DMETER_START_Y + DMETER_HEIGHT - 1)

// list box dimension
#define LBOX_WIDTH    		(180)
#define LBOX_HEIGHT   		(155)
#define LBOX_START_X  		(((GetMaxX()+LBOX_WIDTH)>>1) - LBOX_WIDTH)
#define LBOX_START_Y  		(((GetMaxY()+LBOX_HEIGHT)>>1) - LBOX_HEIGHT)     
#define LBOX_END_X    		(LBOX_START_X + LBOX_WIDTH - 1)
#define LBOX_END_Y    		(LBOX_START_Y + LBOX_HEIGHT - 1)

#define MOVEDELTA			1    
#define MOVELEFT            0
#define MOVERIGHT           1

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void CreateDisplayObjects();					// creates the objects on the screen
void CreateScale();								// creates the depth scale 
void InitButtonPress();							// initialize the inputs
void GetInputData();							// creates the sonar inputs
void DrawInputData();							// draw the input data
void ScrollFishArea(SHORT left, SHORT top, SHORT right, SHORT bottom, SHORT delta);

/////////////////////////////////////////////////////////////////////////////
//                            PALETTE USED
/////////////////////////////////////////////////////////////////////////////
#ifdef USE_PALETTE
extern const PALETTE_FLASH _ColorPalette;
#endif

/////////////////////////////////////////////////////////////////////////////
//                            FONTS USED
/////////////////////////////////////////////////////////////////////////////
// see DemoFlashResource.c (h)

/////////////////////////////////////////////////////////////////////////////
//                       GLOBAL VARIABLES FOR DEMO
/////////////////////////////////////////////////////////////////////////////
extern volatile DWORD  tick;                        // tick counter

extern XCHAR           ExitStr[];
extern XCHAR           MenuStr[];
extern XCHAR           HelpStr[];

GOL_SCHEME      *navScheme1, *navScheme2;           		// style scheme 
GOL_SCHEME      *textScheme;								// text style scheme 
DIGITALMETER    *pDm, *pDmTemp;

WORD 			_pauseOn = 0;								// flag that turns on/off the scrolling
WORD 			_moveAreaEndY;								// defines the y coordinate position of the shifting area
WORD 			_floor = GetMaxY()-CTRLBTN_HEIGHT-10;		// current depth of water 
WORD 			_prevFloor = GetMaxY()-CTRLBTN_HEIGHT-10;	// previous water depth
WORD 			_redrawScale;								// flag that indicates if scale is to be redrawn
WORD 			_inputBufferArray[(DISP_VER_RESOLUTION*MOVEDELTA*COLOR_DEPTH)/16];	// input data buffer

// strings for the list box menu
const XCHAR OptionItems[] =
{ 'O','p','t','i','o','n','s','\n',
  'S','e','t','t','i','n','g','s','\n',
  'S','e','n','s','i','t','i','v','i','t','y','\n',
  'D','e','p','t','h',' ','R','a','n','g','e','\n',
  'S','a','m','p','l','i','n','g',' ','S','p','e','e','d',0
};


/////////////////////////////////////////////////////////////////////////////
//                      CREATE FISHFINDER DEMO
/////////////////////////////////////////////////////////////////////////////
WORD CreateFishFinderDemo(void)
{
	
	static XCHAR depthLabelStr[] = {'f','t',0};
	static XCHAR TempLabelStr[]  = {' ','F',0};

    // Free memory for the objects in the previous linked list and start new list to display
    // the files seen on the media
    GOLFree();

#if defined (USE_PALETTE)
	#if (COLOR_DEPTH == 8)
	    SetPaletteBpp(8);
    	SetPalette((void*)&_ColorPalette, 0, 256);
    	EnablePalette();
	#endif
#endif
	
    // set screen to SCREEN_BACKGROUND_COLOR
    SetColor(SCREEN_BACKGROUND_COLOR);
    ClearDevice();
  
	srand(tick);

	_redrawScale = 1;
	_moveAreaEndY = GetMaxY()-CTRLBTN_HEIGHT-1; //GetMaxY(); 
	
	navScheme1 = GOLCreateScheme();
	navScheme1->CommonBkColor = WHITE;
	navScheme1->TextColor0    = BRIGHTBLUE; 
    navScheme1->pFont= (void*)&monofont;
    
	navScheme2 = GOLCreateScheme();
	navScheme2->CommonBkColor = WHITE;
	navScheme2->TextColor0    = BRIGHTBLUE; 
    navScheme2->pFont= (void*)&monofontsmall;
    
	textScheme = GOLCreateScheme();
	textScheme->CommonBkColor = WHITE;
	textScheme->TextColor0    = BRIGHTBLUE; 
	textScheme->TextColor1    = WHITE; 
	textScheme->Color0		  = WHITE;

   LbCreate
    (
        ID_FISHLISTBOX,                		// ID
        LBOX_START_X,
        LBOX_START_Y,              		// dimensions
        LBOX_END_X,
        LBOX_END_Y,
        LB_SINGLE_SEL | LB_DISABLED,   	// will be created with single select and focused
        (XCHAR *)OptionItems,           // list of demo screens
        altScheme
    );                  // use alternate scheme    

    pDm = DmCreate
    (
        ID_FISHDEPTHMETER,
        DMETER_START_X,
        DMETER_START_Y,     
        DMETER_END_X,
        DMETER_END_Y,
        DM_DRAW | DM_CENTER_ALIGN,
        0,
        3,
        1,
        navScheme1
    );

  	StCreate
    (
        ID_FISHSTATICTEXT1,     
        DMETER_END_X, 
        DMETER_START_Y, 
        TEXTAREA_END_X, 
        DMETER_END_Y,
        ST_DRAW | ST_CENTER_ALIGN,   
        depthLabelStr,                            
        textScheme
    );       

    pDmTemp = DmCreate
    (
        ID_FISHTEMPMETER,
        DMETER_START_X, 
        DMETER_END_Y, 
        (TEXTAREA_END_X >> 1)+(TEXTAREA_END_X >> 3), 
        TEXTAREA_END_Y,
        DM_DRAW | DM_CENTER_ALIGN,
        0,
        3,
        1,
        navScheme2
    );

  	StCreate
    (
        ID_FISHSTATICTEXT2,     
        (TEXTAREA_END_X >> 1)+(TEXTAREA_END_X >> 3) + 1, 
        DMETER_END_Y, 
        TEXTAREA_END_X, 
        TEXTAREA_END_Y,
        ST_DRAW,   
        TempLabelStr,                            
        textScheme
    );       
    
    CreateCtrlButtons(ExitStr, NULL, NULL, MenuStr);

  	CreateScale();
	InitButtonPress();						// initialize the buttons

	// successful creation of the JPEG demo screen
    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD FishFinderMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
// Input: objMsg - translated message for the object,
//        pObj - pointer to the object,
//        pMsg - pointer to the non-translated, raw GOL message
// Output: if the function returns non-zero the message will be processed by default
// Overview: it's a user defined function. GOLMsg() function calls it each

//           time the valid message for the object received
/////////////////////////////////////////////////////////////////////////////
WORD FishFinderMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
	LISTBOX      *pLb;
	WORD 		objectID;

	objectID = GetObjID(pObj);

	switch(objectID)
	{
        case ID_BUTTON_A:
            if(objMsg == BTN_MSG_RELEASED)
            {   // check if button is pressed
                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);
                free(navScheme1);
                free(navScheme2);
                free(textScheme);

                // go to menu screen
                screenState = CREATE_MAINMENU;
            }
            break;

		case ID_BUTTON_D:
			
			if (objMsg == BTN_MSG_PRESSED)
			{
			 	pLb = (LISTBOX*)GOLFindObject(ID_FISHLISTBOX);
				if (GetState((void*)pLb, DISABLED) == DISABLED) 
				{
					// show the list box
				 	ClrState(pLb, DISABLED);
				 	SetState(pLb, DRAW);
				 	_pauseOn = 1;
				 }		
			 	else if (GetState(pLb, DISABLED) != DISABLED) 
				{
					// redraw objects overwritten by the list box
				 	GOLRedrawRec(pLb->hdr.left, pLb->hdr.top, pLb->hdr.right, pLb->hdr.bottom);
					// disable and hide the list box
				 	SetState(pLb, DISABLED|HIDE);
				 	//_pauseOn = 0;
			 	}		
			} 	
			break;	
		default:
			break;	
	}		

    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD FishFinderDrawCallback()
// Output: if the function returns non-zero the draw control will be passed to GOL
// Overview: it's a user defined function. GOLDraw() function calls it each
//           time when GOL objects drawing is completed. User drawing should be done here.
//           GOL will not change color, line type and clipping region settings while

//           this function returns zero.
/////////////////////////////////////////////////////////////////////////////
#define FISHSCROLLDELAY	2
WORD FishFinderDrawCallback(void)
{
	static DWORD 	prevTime = 0;
	//DWORD  delay;
	
	//delay = ADCGetPot() >> 2;

	
	// we dont want to be stuck here for the delay so we use the tick instead to count the delay
	//if((tick - prevTime) < delay /*FISHSCROLLDELAY*/)
	if((tick - prevTime) < FISHSCROLLDELAY)
	{
		return 1;
	}	
    prevTime = tick;                                // reset tick timer
	
	// check if we need to redraw the scale
	if (_redrawScale)
	{
		// clear out the menu buttons
		SetColor(WHITE);
		Bar(0,GetMaxY()-(MENUHEIGHT-1),GetMaxX()-SCALEWIDTH,GetMaxY()); 
  		CreateScale();
 	} 		
 	
 	if(_pauseOn)
 	{
		// skip the scrolling if the list box is not yet hidden	
	 	//if (GetState(GOLFindObject(ID_FISHLISTBOX), DISABLED|HIDE) == (DISABLED|HIDE)) 
	 	if (GetState(GOLFindObject(ID_FISHLISTBOX), DISABLED) == (DISABLED)) 
	 	{
			_pauseOn = 0;
	 	}
	}
	else	
		if (GetHWButtonLeft() == 1) 						// Button S1 also pauses the scrolling
		{	
			// Update the screen with new data
			GetInputData();
			DrawInputData();
		}	

    return (1);
}

void InitButtonPress()
{
	// RB5 is used as an analog input to emulate temp. (using POT on DA210 Dev Board)

	// make the pins digital inputs
	ANSGbits.ANSG8 = 0;  // S1 
	ANSEbits.ANSE9 = 0;  // S2 
	
	TRISGbits.TRISG8 = 1;	// used as an input from button S1
	TRISEbits.TRISE9 = 0;	// used as an output to measure idle time 
	TRISBbits.TRISB5 = 1;   // used as an analog input from the POTENTIOMETER 
	
}	

void GetInputData()
{
	static  SHORT randomFish;
			SHORT sign; 

			SHORT y,count;
			WORD  randomPixel;
			
#if (COLOR_DEPTH == 16)
			WORD *pSamples;
#elif (COLOR_DEPTH == 8)
			BYTE *pSamples;
#endif

	// initialize the input buffer pointer
	#if (COLOR_DEPTH == 16)
		pSamples = _inputBufferArray;
	#elif (COLOR_DEPTH == 8)
		pSamples = (BYTE*)_inputBufferArray;
	#endif

	// randomly generate the floor level first
	sign  = rand()%16;
	if (sign < 8)
		sign = (-1);
	else
		sign = 1;	
	_prevFloor = _floor;	
	_floor = _floor + ((sign)*(rand()%3)); 
	if (_floor > _moveAreaEndY-5)
		_floor = _moveAreaEndY-5;
	if (_floor <= 10)
		_floor = 10;

	// update the depth meter
	DmSetValue(pDm, _floor);
	SetState(pDm, DM_UPDATE);          

	// update the temp
	DmSetValue(pDmTemp, ADCGetPot() >> 2);
	SetState(pDmTemp, DM_UPDATE);          

	for(count=0;count<((_moveAreaEndY+1)*MOVEDELTA);count++)
	{

#if defined (GFX_USE_DISPLAY_PANEL_PH480272T_005_I11Q)
		y = (count/MOVEDELTA)%(_moveAreaEndY+1);
#elif defined (GFX_USE_DISPLAY_PANEL_TFT_G240320LTSW_118W_E)	
		y = count%(_moveAreaEndY);
#endif
		if ((y > (_floor-3)) && (y < (_floor+3)))
			randomPixel = 0;				// assign black
		else if (y < _floor)				// depth reaches the floor
		{	
			randomFish = rand()%50;
			if (randomFish < 5) 
#if (COLOR_DEPTH == 16)
   				randomPixel = RGB565CONVERT(0, (rand()%50), 0xFF); 
#elif (COLOR_DEPTH == 8)			
   				randomPixel = RGB332CONVERT(0, (rand()%50), 0xFF); 
#endif   				
			else	
  		 		randomPixel = WHITE;		// assign white 
  		} 			
   		else 
   		{	
			// randomize colors below the floor
			if (y < (_floor + (((_moveAreaEndY+1) - _floor) >> 1)))
#if (COLOR_DEPTH == 16)
				randomPixel = RGB565CONVERT(0xFF, ((rand()%56) + 200), 0x00); 
#elif (COLOR_DEPTH == 8)			
				randomPixel = (rand()%16) + 218;
#endif   				
			else	
#if (COLOR_DEPTH == 16)
				randomPixel = RGB565CONVERT(0xFF, ((rand()%256) - 200), 0x00); 
#elif (COLOR_DEPTH == 8)			
				randomPixel = (rand()%16) + 236;
#endif   				
   		}	
		*pSamples++ = randomPixel;

	} 	// 	for(count=0;count<((_moveAreaEndY+1)*MOVEDELTA);count++)
}	

void DrawInputData()
{
	DWORD address;

	/* ********************************************* */
	// Update the floor marker
	/* ********************************************* */
	// erase the floor marker			
	SetColor(WHITE);
	while(!Bar(SCALELINE_X_POS+4,_prevFloor-1,SCALELINE_X_POS+9,_prevFloor+1));  	

	GFX_WaitForCommandQueue(16);
	while(GFX_IsPuGpuBusy());

	// draw the floor marker
	SetColor(BRIGHTBLUE);
	while(!Bar(SCALELINE_X_POS+4,_floor-1,SCALELINE_X_POS+9,_floor+1));  	

	GFX_WaitForCommandQueue(16);
	while(GFX_IsPuGpuBusy());

	/* ********************************************* */
	// scroll the data to the right of the text area
	/* ********************************************* */
#if defined (GFX_USE_DISPLAY_PANEL_PH480272T_005_I11Q)
	Scroll(TEXTAREA_END_X+MOVEDELTA, 0, MV_AREA_END_X, TEXTAREA_END_Y, MOVEDELTA, MOVELEFT);
#elif defined (GFX_USE_DISPLAY_PANEL_TFT_G240320LTSW_118W_E)	
	ScrollFishArea(TEXTAREA_END_X+MOVEDELTA, 0, MV_AREA_END_X, TEXTAREA_END_Y, MOVEDELTA);
#endif
	// make sure the scrolling is done	
	GFX_WaitForCommandQueue(16);
	while(GFX_IsPuGpuBusy());

	/* ********************************************* */
	// scroll the rest of the data 
	/* ********************************************* */
#if defined (GFX_USE_DISPLAY_PANEL_PH480272T_005_I11Q)
	Scroll(MOVEDELTA, TEXTAREA_END_Y+1, MV_AREA_END_X, _moveAreaEndY, MOVEDELTA, MOVELEFT);
#elif defined (GFX_USE_DISPLAY_PANEL_TFT_G240320LTSW_118W_E)	
	ScrollFishArea(MOVEDELTA, TEXTAREA_END_Y+1, MV_AREA_END_X, _moveAreaEndY, MOVEDELTA);
#endif	

	// make sure the scrolling is done	
	GFX_WaitForCommandQueue(16);
	while(GFX_IsPuGpuBusy());

	// copy the new data to the buffer
#if defined (GFX_USE_DISPLAY_PANEL_PH480272T_005_I11Q)
	address = MV_AREA_END_X - (MOVEDELTA - 1);
#elif defined (GFX_USE_DISPLAY_PANEL_TFT_G240320LTSW_118W_E)	
	address = ((GetMaxY()+1)*SCALEWIDTH)+1;
#endif
			
	/* ********************************************* */
	// Move the new data into the display buffer 
	/* ********************************************* */
#if defined (GFX_USE_DISPLAY_PANEL_PH480272T_005_I11Q)
	ROPBlock((DWORD)_inputBufferArray, GFX_DISPLAY_BUFFER_START_ADDRESS, 0, address, 
			  RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS, RCC_COPY, RCC_ROP_C, 0, MOVEDELTA, _moveAreaEndY);
#elif defined (GFX_USE_DISPLAY_PANEL_TFT_G240320LTSW_118W_E)	
	ROPBlock((DWORD)(WORD)_inputBufferArray, GFX_DISPLAY_BUFFER_START_ADDRESS, 0, address, 
			  RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS, RCC_COPY, RCC_ROP_C, 0, _moveAreaEndY, MOVEDELTA);
#endif

	GFX_WaitForCommandQueue(16);
	while(GFX_IsPuGpuBusy());

	
}	

void ScrollFishArea(SHORT left, SHORT top, SHORT right, SHORT bottom, SHORT delta)
{
	WORD j, temp;
	
	temp = (right-left+1)%delta;
	Scroll(left, top, left+(temp-1), bottom, temp, MOVELEFT);
		
	for (j = left+temp; j <= right; j += delta)
	{
		Scroll(j, top, j+(delta-1), bottom, delta, MOVELEFT);

		// make sure the scrolling is done	
		while(GFX_IsPuGpuBusy());
	}
}	 

void CreateScale() 
{
	// this draws the scale on the right side of the screen
	WORD 	scaleValueDelta, i, bIndex, Value;	
	XCHAR 	scaleTxt[3], *pTextStart = NULL;

	// draw the depth labels
	scaleValueDelta = (_moveAreaEndY)/5;
	
	SetFont((void*)&GOLMediumFont);
	
	SetColor(WHITE);
	while(!Bar(GetMaxX()-SCALEWIDTH+1, 0, GetMaxX(), _moveAreaEndY));
	
	SetColor(BRIGHTBLUE);
	while(!Bevel(SCALELINE_X_POS+1, 5, SCALELINE_X_POS+1, _moveAreaEndY-5, 2));

	for (i=5;i<_moveAreaEndY-5;i++) 
	{
		// draw the scales
		if ((i%10) == 0)
			while(!Bar(SCALELINE_X_POS,i,SCALELINE_X_POS+3,i));  	
	}

	for (i=0;i<=4;i++) 
	{
		Value = (scaleValueDelta*(i)/10);	
	    for(bIndex = 0; bIndex < 2; bIndex++)
	    {
		    
    	    scaleTxt[2 - bIndex - 1] = '0' + (Value % 10);
    	    
    	    if (Value < 10) 
	    	{
  	       		pTextStart = &scaleTxt[2 - bIndex - 1];
    	    	break;
    	 	}   	
       		Value /= 10;
    	}
    	scaleTxt[2] = '\0';
		while(!OutTextXY(SCALELINE_X_POS-3-GetTextWidth(pTextStart, (void*)&GOLSmallFont), scaleValueDelta*i, pTextStart));
		
	}	
	_redrawScale = 0;
	
}	



