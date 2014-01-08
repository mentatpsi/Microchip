/*********************************************************************
 *
 *	E-Paper Demo PIP screen
 *
 *********************************************************************
 * FileName:        pip_screen.c
 * Dependencies:    None
 * Processor:       PIC24, dsPIC, PIC32
 * Compiler:        Microchip C30 and C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (c) 2011 Microchip Technology Inc.  All rights 
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
 * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product ("Device") which is 
 *      integrated into Licensee�s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and 
 *      ENC28J60.h ported to a non-Microchip device used in 
 *      conjunction with a Microchip ethernet controller for the 
 *      sole purpose of interfacing with the ethernet controller. 
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT 
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL 
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF 
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS 
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT 
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 ********************************************************************/

/*****************************************************************************
 * SECTION: Includes
 *****************************************************************************/
#include <math.h>
#include "MainDemo.h"
#include "Graphics/Graphics.h"
#include "gfx_font_resources.h"
#include "ID.h"
#include "Graphics/ListBox.h"
#include "pip_screen.h"


/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define LB_ORIGIN_START_X   ((GetMaxX() - 260 + 1) / 2)
#define LB_ORIGIN_START_Y   ((40 + GetMaxY() - 192 + 1) / 2)	
#define LB_WIDTH            (240)    
#define LB_HEIGHT           (142)
#define LB_ORIGIN_END_X     (LB_ORIGIN_START_X + LB_WIDTH)
#define LB_ORIGIN_END_Y     (LB_ORIGIN_START_Y + LB_HEIGHT)
/*****************************************************************************
 * SECTION: Externs
 *****************************************************************************/
extern volatile WORD tick;
extern WORD g_UPDATE_FLAGS;

/*****************************************************************************
 * SECTION: Constants
 *****************************************************************************/
static const XCHAR ListboxLstStr[] = "Show cursor \nMove cursor \nMove PIP window \nExit\n";
static const XCHAR UpArrowStr[]    = "up";
static const XCHAR DownArrowStr[]  = "dn";

/*****************************************************************************
 * SECTION: Varibles
 *****************************************************************************/
static GOL_SCHEME      *altScheme;                                 // alternative style scheme
typedef enum 
{ 
    PIP_START, 
    PIP_SELECT, 
    PIP_SHOW_CURSOR, 
    PIP_MOVE_CURSOR, 
    PIP_MOVE, PIP_EXIT 
} PIPACTION;

PIPACTION g_PipAction = PIP_START;

/*****************************************************************************
 * void CreateListBoxScheme()
 *****************************************************************************/	
static GOL_SCHEME * CreateListBoxScheme( void )
{
	altScheme = GOLCreateScheme();      // create alternative 1 style scheme
	
	if ( altScheme )
	{
		altScheme->Color0               = GRAY013;
	    altScheme->Color1               = GRAY007;
	    altScheme->EmbossDkColor        = GRAY001;
	    altScheme->EmbossLtColor        = GRAY013;
	    altScheme->ColorDisabled        = GRAY010;
	    altScheme->TextColor1           = BLACK;
	    altScheme->TextColor0           = GRAY002;
	    altScheme->TextColorDisabled    = GRAY012;
	
	    altScheme->pFont = (void*)&DroidSans_12;
 	} 
 	
 	return altScheme;  
}

/*****************************************************************************
 * static void DestroyListBoxScheme( void )
 *****************************************************************************/	
static void DestroyListBoxScheme( void )
{
	if ( altScheme )
	{
		free( altScheme );
		altScheme = NULL;
	}	
}	

/*****************************************************************************
 * void CreateListBox()
 *****************************************************************************/	
static BOOL CreateListBox( void )
{
    LISTBOX *pLb;
    
    DelayMs(50);
    
    if ( CreateListBoxScheme() )
    {
		GFX_DRIVER_CreateLayer( GFX_PIP_LAYER, 0, LB_ORIGIN_START_X, LB_ORIGIN_START_Y, LB_ORIGIN_END_X, LB_ORIGIN_END_Y );
	    GFX_DRIVER_ShowLayer(GFX_PIP_LAYER); 

        GFX_DRIVER_ActivateLayer( GFX_PIP_LAYER );
	        	
	    pLb = LbCreate
	    (
	         ID_LBS_LISTBOX1,                           // ID
	         0, 										// Start X
	         0,                                         // Start Y
	         LB_WIDTH-30,                               // End X
	         LB_HEIGHT,                                 // End Y dimension
	         LB_DRAW /*| LB_FOCUSED*/,                  // will be dislayed after creation
	         (XCHAR*)ListboxLstStr, //pMyItemList,
	         altScheme                                  // use alternate scheme
	    );                                          
	        
	    SHORT count = LbGetCount(pLb);
	        
	    SldCreate
	    (
	        ID_LBS_SLIDER1,                             // ID
	        LB_WIDTH-30,                                // Start X
	        32,                                         // Start Y
	        LB_WIDTH,                                   // End X
	        110,                                        // End Y dimension
	        SLD_DRAW | SLD_SCROLLBAR | SLD_VERTICAL,    // vertical, will be dislayed after creation
	        count - 1,                                  // range
	        1,                                          // page
	        count - 1,                                  // pos
	        altScheme                                   // use alternate scheme
	    );                              
	    BtnCreate
	    (
	        ID_LBS_BUTTON1,                             // ID
	        LB_WIDTH-30,                                // Start X
	        0,                                          // Start Y
	        LB_WIDTH,                                   // End X 
	        32,                                         // End Y 
	        0,                                          // dimension (no radius)
	        BTN_DRAW,                                   // will be dislayed after creation
	        NULL,                                       // no bitmap
	        (XCHAR *)UpArrowStr,                        // Up Arrow
	        altScheme                                   // use alternate scheme
	    );                              
	    BtnCreate
	    (
	        ID_LBS_BUTTON2,                             // ID
	        LB_WIDTH-30,                                        // Start X
	        110,                                        // Start Y 
	        LB_WIDTH,                                        // End X
	        142,                                        // End Y 
	        0,                                          // dimension (no radius)
	        BTN_DRAW,                                   // will be dislayed after creation
	        NULL,                                       // no bitmap
	        (XCHAR *)DownArrowStr,                      // Down Arrow
	        altScheme                                   // use alternate scheme
	    );
	    
	    
		WORD y = GetTextHeight((void *) altScheme->pFont)/2;
		GFX_DRIVER_MoveLayer( GFX_CURSOR_LAYER, LB_ORIGIN_START_X + 180, LB_ORIGIN_START_Y + y/2 ); 
		
		return TRUE;
	}
	
	return FALSE;
    
}

/*****************************************************************************
 * void DeleteListBox()
 *****************************************************************************/	
static void DeleteListBox( void )
{ 
	GFX_DRIVER_ActivateLayer( GFX_MAIN_LAYER );
	GFX_DRIVER_HideLayer( GFX_PIP_LAYER );
	GFX_DRIVER_HideLayer( GFX_CURSOR_LAYER );
	
	GOLDeleteObjectByID(ID_LBS_BUTTON2);  
	GOLDeleteObjectByID(ID_LBS_BUTTON1);  
	GOLDeleteObjectByID(ID_LBS_SLIDER1);  
	GOLDeleteObjectByID(ID_LBS_LISTBOX1);  
	
	DestroyListBoxScheme();	
}

/*****************************************************************************
 * void UpdateListBox()
 *****************************************************************************/		
static void UpdateListBox( BOOL up, BOOL down )
{
	LISTBOX *  pLb = (LISTBOX *)GOLFindObject(ID_LBS_LISTBOX1);
	SHORT    count = LbGetCount(pLb);
	SHORT  CurItem = LbGetFocusedItem(pLb);	
	SLIDER  * pSld = (SLIDER *)GOLFindObject(ID_LBS_SLIDER1);
	SHORT      pos = SldGetPos(pSld);
	WORD y;
	
	if( up )
	{
	    LbSetFocusedItem(pLb, CurItem - 1);
	    SetState(pLb, LB_DRAW_ITEMS);
	    if ( CurItem - 1 < count )
	      SldSetPos(pSld, pos + 1);
	    SetState(pSld, SLD_DRAW_THUMB);
	    
	    if ( CurItem )
	    {	    
	    	y = ( CurItem - 1 ) * GetTextHeight((void *) altScheme->pFont);
	    	y += GetTextHeight((void *) altScheme->pFont)/2;
	    	GFX_DRIVER_MoveLayer( GFX_CURSOR_LAYER, LB_ORIGIN_START_X + 180, LB_ORIGIN_START_Y + y );
	    }
	}
	
	if( down )
	{
	    LbSetFocusedItem(pLb, CurItem + 1);
	    SetState(pLb, LB_DRAW_ITEMS);
	    if ( count - CurItem - 1 > 0 )
	      SldSetPos(pSld, pos - 1);	    
	    SetState(pSld, SLD_DRAW_THUMB);
	    
	    if ( CurItem + 1 < count )
	    {
	    	y = ( CurItem + 1 ) * GetTextHeight((void *) altScheme->pFont);
	    	y += GetTextHeight((void *) altScheme->pFont)/2;
	    	GFX_DRIVER_MoveLayer( GFX_CURSOR_LAYER, LB_ORIGIN_START_X + 180, LB_ORIGIN_START_Y + y );
	    }
	}
    	
}


/*****************************************************************************
 * void ListBoxSelected(void)
 *****************************************************************************/	
static WORD ListBoxSelected( void )	
{
	LISTBOX * pLb = (LISTBOX *)GOLFindObject(ID_LBS_LISTBOX1);    
    SHORT      ix = LbGetFocusedItem(pLb);
    
    switch (ix)
    {
    	default:
    	case 0:
    		return PIP_SHOW_CURSOR;
    		
    	case 1:
    		return PIP_MOVE_CURSOR;
    		
    	case 2:
    		return PIP_MOVE;
    
    	case 3:
    		return PIP_EXIT;
    }	
}

/*****************************************************************************
 * static BOOL MoveCursor( void )
 *****************************************************************************/	
static BOOL MoveCursor( void )
{
	static WORD rad;
	WORD OriginY = LB_ORIGIN_START_Y + 1 * GetTextHeight((void *)altScheme->pFont) + GetTextHeight((void *)altScheme->pFont)/2;
	WORD OriginX = LB_ORIGIN_START_X + 180;
	WORD x, y;
	BOOL IsRun = TRUE;

	if ( tick > 1000 )
	{	
		if ( rad <= 628 )
		{
			x = OriginX - 64 + cos((double)rad/100) * 64;
			y = OriginY + sin((double)rad/100) * 64;

			rad += 20;
			GFX_DRIVER_MoveLayer( GFX_CURSOR_LAYER, x, y );	
			
			if ( rad <= 628 )
				g_UPDATE_FLAGS = GFX_UPDATE_AS_IT_DRAWS | GFX_WAIT_IMAGE_UPDATED;
			
		}
		else
		{
			rad = 0;
			x = OriginX;
			y = OriginY; 	
		
			IsRun = FALSE; // stop running
			GFX_DRIVER_MoveLayer( GFX_CURSOR_LAYER, x, y );	
		}
		
		tick = 0;	
	}

	return IsRun;		
	
}	
/*****************************************************************************
 * static BOOL MovePip( void )
 *****************************************************************************/	
static BOOL MovePip( void )
{	
	WORD x, y;
	static WORD PosCnt = 0;
	BOOL IsRun = TRUE;

		if ( tick == 0 || PosCnt == 0 )
		{
			tick = 1;
			switch ( PosCnt )
			{
				default:
				PosCnt = 0;
				case 0:
					PosCnt++;
					if ( g_AutoMode )
					{
						x = LB_ORIGIN_START_X;
						y = LB_ORIGIN_START_Y;
						break;
					}
					
				case 1:
					x = 5;
					y = 5;
					PosCnt++;
					break;					
				
				case 2:
					x = 90;
					y = 50;
					PosCnt++;
					break;				
				
				case 3:
					x = 0;
					y = 170;
					PosCnt++;
					break;				
				
				case 4:
					x = 90;
					y = 120;
					PosCnt++;
					break;	
					
				case 5:
					x = LB_ORIGIN_START_X;
					y = LB_ORIGIN_START_Y;
					PosCnt = 0;
					IsRun = FALSE; // stop running
					break;								
			}
			
			GFX_DRIVER_MoveLayer( GFX_PIP_LAYER, x, y );		
		}
	
	return IsRun;		
	
}

/*****************************************************************************
 * BOOL RunPip()
 *****************************************************************************/		
BOOL RunPip( void )	
{
	static BOOL ShowCursor;
	
	switch ( g_PipAction )
	{
		case PIP_START:
			ShowCursor = FALSE;
			CreateListBox();
			if ( g_AutoMode )
				g_PipAction = PIP_MOVE;	
			else	
				g_PipAction = PIP_SELECT;
			return TRUE;
		
		
		case PIP_SELECT:
	        if(btnS3 == HW_BUTTON_PRESS)
	        {
	            while(btnS3 == HW_BUTTON_PRESS);	        
	            UpdateListBox( TRUE, FALSE );            
	        }    
	
	        if(btnS1 == HW_BUTTON_PRESS)
	        {
	            while(btnS1 == HW_BUTTON_PRESS);            
	            UpdateListBox( FALSE, TRUE );
	        }  
	        
	        if(btnS2 == HW_BUTTON_PRESS)
	        {
	            while(btnS2 == HW_BUTTON_PRESS);	        
	            g_PipAction = ListBoxSelected();
	        } 		
			return TRUE;
		
		case PIP_SHOW_CURSOR:
			ShowCursor = ! ShowCursor;
			if ( ShowCursor )
				GFX_DRIVER_ShowLayer( GFX_CURSOR_LAYER );
			else
				GFX_DRIVER_HideLayer( GFX_CURSOR_LAYER );			
			g_PipAction = PIP_SELECT;
			return TRUE;		
			
		case PIP_MOVE_CURSOR:
			if ( ! MoveCursor() )
					g_PipAction = PIP_SELECT;
			
			return TRUE;
			
		case PIP_MOVE:
			if ( ! MovePip() )
			{
				if ( g_AutoMode  )
					g_PipAction = PIP_EXIT;
				else
					g_PipAction = PIP_SELECT;
			}
			return TRUE;						
		
		default:
		case PIP_EXIT:
			DeleteListBox();
			g_PipAction = PIP_START;
			return FALSE;
	}	
	
	//return TRUE;	
}


/*****************************************************************************
 * void LoadPipScreen(void)
 *****************************************************************************/
void LoadPipScreen(void)
{
    SHORT x = 45, y = 100;

    SetFont((void *) &DroidSans_24);
   	OutTextXY(x, y, (XCHAR *)"   No image redraw");
   	y += GetTextHeight((void *)&DroidSans_24) + 10;
   	OutTextXY(x, y, (XCHAR *)"under PIP is required!");
}
