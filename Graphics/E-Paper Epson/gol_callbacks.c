/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        main_screen.c
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
#include "Graphics/Graphics.h"
#include "MainDemo.h"
#include "screens.h"
#include "ID.h"
#include "pip_screen.h"
#include "ListBox_screen.h"

/*****************************************************************************
 * SECTION: Local Prototypes
 *****************************************************************************/
void NextButtonRelease(void);
void NextButtonPress(void);
void BackButtonPress(void);
void BackButtonRelease(void);

/*****************************************************************************
 * SECTION: Externs
 *****************************************************************************/
extern WORD g_UPDATE_FLAGS;
/*****************************************************************************
 * SECTION: Variables
 *****************************************************************************/
DEMO_SCREENS demoScreens;

/************************************************************************
 Function: WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)

 Overview: This function must be implemented by user. GOLMsg() function 
 		   calls it each time the valid message for the object received.
           
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 

 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    
    
    return (1); // process message by default
}

/************************************************************************
 Function: WORD GOLDrawCallback()

 Overview: This function must be implemented by user. GOLDraw() function 
 		   calls it each time when GOL objects drawing is completed. User 
 		   drawing should be done here. GOL will not change color, line 
 		   type and clipping region settings while  this function 
 		   returns zero.

 Input: none

 Output: If the function returns non-zero the draw control will be passed 
		to GOL. GOLDraw() can proceed and re-draw objects that needs 
		to be redrawn.
************************************************************************/
WORD GOLDrawCallback(void)
{
/************************************************************************
BUTTONS
************************************************************************/ 	
    if(demoScreens & DEMO_NEXT_PRESS)
    {
        NextButtonPress();
        demoScreens &= ~DEMO_NEXT_PRESS;
        demoScreens |= DEMO_NEXT_WAIT_RELEASE; 
        demoScreens |= DEMO_NEXT_UPDATE;
        return 1;
    }

    if(demoScreens & DEMO_NEXT_UPDATE)
    {      
        demoScreens &= ~DEMO_NEXT_UPDATE;
        return 1;
    }

    if(demoScreens & DEMO_NEXT_WAIT_RELEASE)
    {
        if(btnS3 == HW_BUTTON_RELEASE)
        {
            NextButtonRelease();
            demoScreens &= ~DEMO_NEXT_WAIT_RELEASE;
            demoScreens |= DEMO_NEXT_UPDATE;
        }    

        return 1;
    }

    if(demoScreens & DEMO_BACK_PRESS)
    {
        BackButtonPress();
        demoScreens &= ~DEMO_BACK_PRESS;
        demoScreens |= DEMO_BACK_WAIT_RELEASE;
        demoScreens |= DEMO_BACK_UPDATE;
        return 1;
    }

    if(demoScreens & DEMO_BACK_UPDATE)
    {          
        demoScreens &= ~DEMO_BACK_UPDATE;
        return 1;
    }

    if(demoScreens & DEMO_BACK_WAIT_RELEASE)
    {
        if(btnS1 == HW_BUTTON_RELEASE)
        {
            BackButtonRelease();
            demoScreens &= ~DEMO_BACK_WAIT_RELEASE;
            demoScreens |= DEMO_BACK_UPDATE;
        }    

        return 1;
    }
    
/************************************************************************
SCREENS
************************************************************************/    	
	
    switch(demoScreens)
    {
    /**********************************
     * Intro
     **********************************/
    case DEMO_INTRO_SCREEN_CREATE:
    	GOLFree();
    	SetColor(WHITE);
    	ClearDevice();   	
    	LoadIntroScreen();
    	g_AutoMode = TRUE;
        demoScreens = DEMO_INTRO_SCREEN_UPDATE;
        break;

    case DEMO_INTRO_SCREEN_UPDATE:
        demoScreens = DEMO_INTRO_SCREEN_WAIT;
        AutoModeCnt = 0;
        break;

    case DEMO_INTRO_SCREEN_WAIT:
        if(btnS1 == HW_BUTTON_PRESS || btnS2 == HW_BUTTON_PRESS || btnS3 == HW_BUTTON_PRESS)
        {
    	    SetColor(WHITE);  	    
    	    ClearDevice();   
	        CreateMainDemoPage();
            demoScreens = DEMO_FONT_SCREEN_CREATE;
        }    
        break;	    
	    
    /**********************************
     * Fonts
     **********************************/
    case DEMO_FONT_SCREEN_CREATE:
    	PrepareDemoPage((XCHAR *)"Fonts");
        LoadFontScreen();
        demoScreens = DEMO_FONT_SCREEN_UPDATE;
        break;

    case DEMO_FONT_SCREEN_UPDATE:
        demoScreens = DEMO_FONT_SCREEN_WAIT;
        AutoModeCnt = 0;
        break;

    case DEMO_FONT_SCREEN_WAIT:
        if(btnS3 == HW_BUTTON_PRESS)
        {
            demoScreens = DEMO_NEXT_PRESS | DEMO_FOREIGN_FONT_SCREEN_CREATE;
        }    
          
        if(btnS1 == HW_BUTTON_PRESS)
        {
            demoScreens = DEMO_BACK_PRESS | DEMO_PIP_SCREEN_CREATE;
        }    
            
        if(btnS2 == HW_BUTTON_PRESS && !g_AutoMode)
    	{    
        	//****demoScreens = DEMO_LISTBOX_SCREEN_CREATE;
     	}   	
        
        break;

    /**********************************
     * Foreign Fonts
     **********************************/
    case DEMO_FOREIGN_FONT_SCREEN_CREATE:
    	PrepareDemoPage("Foreign Fonts");  
        LoadForeignFontScreen();
        demoScreens = DEMO_FOREIGN_FONT_SCREEN_UPDATE;
        break;

    case DEMO_FOREIGN_FONT_SCREEN_UPDATE:
        demoScreens = DEMO_FOREIGN_FONT_SCREEN_WAIT;
        AutoModeCnt = 0;
        break;

    case DEMO_FOREIGN_FONT_SCREEN_WAIT:
        if(btnS3 == HW_BUTTON_PRESS)
        {
            demoScreens = DEMO_NEXT_PRESS | DEMO_PICTURE_SCREEN_CREATE;
        }    

        if(btnS1 == HW_BUTTON_PRESS)
        {
            demoScreens = DEMO_BACK_PRESS | DEMO_FONT_SCREEN_CREATE;        
        } 
        
        if(btnS2 == HW_BUTTON_PRESS && !g_AutoMode)
    	{    
        	//****demoScreens = DEMO_LISTBOX_SCREEN_CREATE;
     	}
        break;

    /**********************************
     * Picture
     **********************************/
    case DEMO_PICTURE_SCREEN_CREATE:
        PrepareDemoPage("Picture");
        demoScreens = DEMO_PICTURE_SCREEN_UPDATE;
        break;

    case DEMO_PICTURE_SCREEN_UPDATE:
    	LoadPicture();
		g_UPDATE_FLAGS = GFX_UPDATE_HIGH_QUALITY | GFX_WAIT_IMAGE_DISPLAYED;
        demoScreens = DEMO_PICTURE_SCREEN_WAIT;
        AutoModeCnt = 0;
        break;

    case DEMO_PICTURE_SCREEN_WAIT:
        if(btnS3 == HW_BUTTON_PRESS )
        {
            demoScreens = DEMO_NEXT_PRESS | DEMO_WIDGETS_SCREEN_CREATE;
        }    

        if(btnS1 == HW_BUTTON_PRESS)
        {
            demoScreens = DEMO_BACK_PRESS | DEMO_FOREIGN_FONT_SCREEN_CREATE;
        }    
            
        if(btnS2 == HW_BUTTON_PRESS && !g_AutoMode)
    	{    
            demoScreens = DEMO_PICTURE_SCREEN_UPDATE;
        	//****demoScreens = DEMO_LISTBOX_SCREEN_CREATE;
     	}                
                  
        break;

    /**********************************
     * Widgets
     **********************************/
    case DEMO_WIDGETS_SCREEN_CREATE:
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH     
		GFX_DRIVER_AutoUpdPart();		// Turn on widget auto update, partial update for less flashing
#endif		
    	PrepareDemoPage("Widgets");
        CreateWidgets();  
        demoScreens = DEMO_WIDGETS_SCREEN_UPDATE;
        break;

    case DEMO_WIDGETS_SCREEN_UPDATE:      
        demoScreens = DEMO_WIDGETS_SCREEN_WAIT;
        AutoModeCnt = 0;
        break;

    case DEMO_WIDGETS_SCREEN_WAIT:
        if(btnS3 == HW_BUTTON_PRESS)
        {
            DeleteWidgets(); // Remove objects
            demoScreens = DEMO_NEXT_PRESS | DEMO_CHART_SCREEN_CREATE;
        }    

        if(btnS1 == HW_BUTTON_PRESS)
        {
            DeleteWidgets(); // Remove objects
            demoScreens = DEMO_BACK_PRESS | DEMO_PICTURE_SCREEN_CREATE;
        }    
            
        if(btnS2 == HW_BUTTON_PRESS && !g_AutoMode)
    	{    
        	DeleteWidgets(); // Remove objects
        	//****demoScreens = DEMO_LISTBOX_SCREEN_CREATE;
     	}    
		
        break;    

    /**********************************
     * Chart
     **********************************/
    case DEMO_CHART_SCREEN_CREATE:
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH    
		GFX_DRIVER_AutoUpdPart();		// Turn on widget auto update, partial update for less flashing
#endif		
    	PrepareDemoPage("Chart");
        CreateChart();
        demoScreens = DEMO_CHART_SCREEN_UPDATE;
        break;

    case DEMO_CHART_SCREEN_UPDATE:     
        demoScreens = DEMO_CHART_SCREEN_WAIT;
        AutoModeCnt = 0;
        break;

    case DEMO_CHART_SCREEN_WAIT:
        if(btnS3 == HW_BUTTON_PRESS)
        {
            DeleteChart(); // Remove objects
            demoScreens = DEMO_NEXT_PRESS | DEMO_QR_SCREEN_CREATE;
        }    

        if(btnS1 == HW_BUTTON_PRESS)
        {
            DeleteChart(); // Remove objects
            demoScreens = DEMO_BACK_PRESS | DEMO_WIDGETS_SCREEN_CREATE;
        }                
        
        if(btnS2 == HW_BUTTON_PRESS && !g_AutoMode)
    	{    
        	DeleteChart(); // Remove objects
        	//****demoScreens = DEMO_LISTBOX_SCREEN_CREATE;
     	} 
     	    		
        break;

    /**********************************
     * QR
     **********************************/
    case DEMO_QR_SCREEN_CREATE:
#ifdef USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH    
		GFX_DRIVER_AutoUpdPart();		// Turn on widget auto update, partial update for less flashing
#endif		
    	PrepareDemoPage((XCHAR *)"QR");
        LoadQRCode();
        demoScreens = DEMO_QR_SCREEN_UPDATE;
        break;

    case DEMO_QR_SCREEN_UPDATE:
        demoScreens = DEMO_QR_SCREEN_WAIT;
        AutoModeCnt = 0;
        break;

    case DEMO_QR_SCREEN_WAIT:
        if(btnS3 == HW_BUTTON_PRESS)
        {
            demoScreens = DEMO_NEXT_PRESS | DEMO_EKG_SCREEN_CREATE;
        }    

        if(btnS1 == HW_BUTTON_PRESS)
        {
            demoScreens = DEMO_BACK_PRESS | DEMO_CHART_SCREEN_CREATE;
        }                
        
        if(btnS2 == HW_BUTTON_PRESS && !g_AutoMode )
    	{    
            demoScreens = DEMO_QR_SCREEN_CREATE;
        	//****demoScreens = DEMO_LISTBOX_SCREEN_CREATE;
     	} 
     	    		
        break;

    /**********************************
     * EKG 
     **********************************/     
    case DEMO_EKG_SCREEN_CREATE:
    	PrepareDemoPage("EKG");
        ResetEKG();
        demoScreens = DEMO_EKG_SCREEN_UPDATE;
        AutoModeCnt = 0;
        break;

    case DEMO_EKG_SCREEN_UPDATE:
    	demoScreens = DEMO_EKG_SCREEN_WAIT;
    	break;
    	
    case DEMO_EKG_SCREEN_WAIT:
        GraphEKG();       // redraw graph
		g_UPDATE_FLAGS = GFX_UPDATE_AS_IT_DRAWS;
		
        if(btnS3 == HW_BUTTON_PRESS)
        {
            demoScreens = DEMO_NEXT_PRESS | DEMO_PIP_SCREEN_CREATE;
        }    

        if(btnS1 == HW_BUTTON_PRESS)
        {
            demoScreens = DEMO_BACK_PRESS | DEMO_QR_SCREEN_CREATE;
        }
        
        if(btnS2 == HW_BUTTON_PRESS && !g_AutoMode)
    	{    	    	
        	//***demoScreens = DEMO_LISTBOX_SCREEN_CREATE;
     	}            
        break;
        
    /**********************************
     * PIP
     **********************************/     
    case DEMO_PIP_SCREEN_CREATE:
        if ( g_AutoMode )
        {
        	PrepareDemoPage("PIP");
        	demoScreens = DEMO_PIP_SCREEN_RUN;
        }	
	    else
	    {
	    	PrepareDemoPage("PIP: push middle buton");
        	demoScreens = DEMO_PIP_SCREEN_WAIT;  
     	}   	 
            	
    	LoadPipScreen(); 
        break;   

	case DEMO_PIP_SCREEN_RUN:
		if ( ! RunPip() )				
			demoScreens = DEMO_PIP_SCREEN_WAIT;
        break;   	
	
    case DEMO_PIP_SCREEN_WAIT:	
        if(btnS3 == HW_BUTTON_PRESS)
        {
            demoScreens =DEMO_NEXT_PRESS | DEMO_FONT_SCREEN_CREATE;          
        }    

        if(btnS1 == HW_BUTTON_PRESS)
        {
            demoScreens = DEMO_BACK_PRESS | DEMO_EKG_SCREEN_CREATE;
        }  
        
        if(btnS2 == HW_BUTTON_PRESS)
        {
            demoScreens = DEMO_PIP_SCREEN_RUN;            
        }           
        break;              
        
    /**********************************
     * LIST BOX
     **********************************/     
    case DEMO_LISTBOX_SCREEN_CREATE:
        CreateListBox();                    		// create list box test screen
        demoScreens = DEMO_LISTBOX_SCREEN_UPDATE;   // switch to next state
        break;   

    case DEMO_LISTBOX_SCREEN_UPDATE:
        demoScreens = DEMO_LISTBOX_SCREEN_WAIT; 
        break;

    case DEMO_LISTBOX_SCREEN_WAIT:	
        if(btnS3 == HW_BUTTON_PRESS)
        {
            while(btnS3 == HW_BUTTON_PRESS);	        
            UpdateListBox( TRUE, FALSE ); 
            demoScreens = DEMO_LISTBOX_SCREEN_UPDATE;            
        }    

        if(btnS1 == HW_BUTTON_PRESS)
        {
            while(btnS1 == HW_BUTTON_PRESS);            
            UpdateListBox( FALSE, TRUE );
            demoScreens = DEMO_LISTBOX_SCREEN_UPDATE;
        }  
        
        if(btnS2 == HW_BUTTON_PRESS)
        {
            while(btnS2 == HW_BUTTON_PRESS);	        
            demoScreens = ListBoxSelected(); 
            DeleteListBox();
        }           
        break;            
        
    default:
        break;
    }


    return (1);
}

void NextButtonPress(void)
{
    OBJ_HEADER *buttonObj = GOLFindObject(ID_BUTTON_NEXT);
    SetState(buttonObj, BTN_PRESSED | BTN_DRAW);
}

void NextButtonRelease(void)
{
    OBJ_HEADER *buttonObj = GOLFindObject(ID_BUTTON_NEXT);
    ClrState(buttonObj, BTN_PRESSED);              // reset pressed
    SetState(buttonObj, BTN_DRAW);                 // redraw
}

void BackButtonPress(void)
{
    OBJ_HEADER *buttonObj = GOLFindObject(ID_BUTTON_BACK);
    SetState(buttonObj, BTN_PRESSED | BTN_DRAW);
}

void BackButtonRelease(void)
{
    OBJ_HEADER *buttonObj = GOLFindObject(ID_BUTTON_BACK);
    ClrState(buttonObj, BTN_PRESSED);              // reset pressed
    SetState(buttonObj, BTN_DRAW);                 // redraw
}
