/*****************************************************************************
 * FileName:        ListBox_screen.c
 * Dependencies:    See Includes Section
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 *
 * Copyright � 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
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
 *****************************************************************************/

/*****************************************************************************
 * SECTION: Includes
 *****************************************************************************/
#include "MainDemo.h"
#include "Graphics/Graphics.h"
#include "gfx_font_resources.h"
#include "ID.h"
#include "Graphics/ListBox.h"
#include "screens.h"

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
 * SECTION: Extern
 *****************************************************************************/
extern volatile WORD tick;


/*****************************************************************************
 * SECTION: Constants
 *****************************************************************************/
static const XCHAR ListboxLstStr[] = "Intro \nFonts \nForeign Fonts \nPicture \nWidgets \nChart \nQR \nEkg \nPip";
static const XCHAR UpArrowStr[]    = "up"; 
static const XCHAR DownArrowStr[]  = "dn"; 

/*****************************************************************************
 * SECTION: Variables
 *****************************************************************************/
static GOL_SCHEME      *altScheme;                                 // alternative style scheme

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

void CreateListBox(void)
{
    LISTBOX *pLb;

    DelayMs(50);
    
    if ( CreateListBoxScheme() )
    {
		GFX_DRIVER_CreateLayer( GFX_PIP_LAYER, 0, LB_ORIGIN_START_X, LB_ORIGIN_START_Y, LB_ORIGIN_END_X, LB_ORIGIN_END_Y );
	    GFX_DRIVER_ShowLayer(GFX_PIP_LAYER); 
	    GFX_DRIVER_ShowLayer( GFX_CURSOR_LAYER );
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
	}
    
}

/*****************************************************************************
 * void DeleteListBox()
 *****************************************************************************/	
void DeleteListBox( void )
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
void UpdateListBox( BOOL up, BOOL down )
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
WORD ListBoxSelected( void )	
{
	LISTBOX * pLb = (LISTBOX *)GOLFindObject(ID_LBS_LISTBOX1);    
    SHORT      ix = LbGetFocusedItem(pLb);
    
    switch (ix)
    {
    	default:
    	case 0:
    		return DEMO_INTRO_SCREEN_CREATE;
    		
    	case 1:
    		return DEMO_FONT_SCREEN_CREATE;
    		
    	case 2:
    		return DEMO_FOREIGN_FONT_SCREEN_CREATE;
    
    	case 3:
    		return DEMO_PICTURE_SCREEN_CREATE;
    
    	case 4:
    		return DEMO_WIDGETS_SCREEN_CREATE;
    
    	case 5:
    		return DEMO_CHART_SCREEN_CREATE;
    		
    	case 6:
    		return DEMO_QR_SCREEN_CREATE;
    		
    	case 7:
    		return DEMO_EKG_SCREEN_CREATE;
    		
    	case 8:
    		return DEMO_PIP_SCREEN_CREATE;
    }	
}

