/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        GoogleMapMenu.c
 * Dependencies:    MainDemo.h, GoogleMapMenu.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software LicenseJpeg
 * Agreement
 *
 * Copyright © 2007 Microchip Technology Inc.  All rights reserved.
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 03/18/08    ...
 *****************************************************************************/
#include "GoogleStaticMapClient.h"
#include "JPEGImage.h"

// Google Static Map Client downloader state defined in GoogleStaticMapClient.c
extern GOOGLE_STATIC_MAP_STATE MapDownloaderState;


#define BUTTONINITSTATE (BTN_DRAW|BTN_TWOTONE)

/* ******************************************** */
// Local Prototypes
/* ******************************************** */
void AlternateKeys(TEXTENTRY *pTe, XCHAR *pKeyList[]);


const XCHAR zoomOutStr[] 	= {'-',0};
const XCHAR zoomInStr[] 	= {'+',0};
const XCHAR mapStr[] 		= {'M','A','P',0};
const XCHAR satStr[] 		= {'S','A','T','E','L','L','I','T','E',0};
const XCHAR terStr[] 		= {'T','E','R','R','A','I','N',0};

const XCHAR loadStr[] 		= {'P','R','E','S','E','T',0};
const XCHAR enterStr[] 		=  {'A','D','D','R',0}; 
const XCHAR doneStr[] 		= {'D','O','N','E',0};

const XCHAR savedAddresses[] = 	"Microchip Technology Inc. 2355 W.Chandler Blvd. Chandler, AZ\n" 
								"Paris, France\n" 
								"Yokohama, Japan\n" 
								"New York City, New York\n"
								"Boston, Massachusetts\n"
								"Las Vegas, Nevada\n"
								"Honolulu, HI\n"
								"McEnery Convention Center\n" 
								"White House, Washington, DC\n"
								"Grand Canyon, AZ\n"
								"Beringer Vineyards, Napa, CA\n"
								;



XCHAR 			shiftKey[] = {'s','h','f','t',0}; 
XCHAR           spaceKey[] = {'s','p','c',0};
XCHAR           delKey[] = {'d','e','l',0};
//XCHAR           enterKey[] = {'e','n','t','e','r',0};
XCHAR           clearKey[] = {'c','l','r',0}; 
XCHAR           keydash[] = {'-',0};
XCHAR           keyforwardslash[] = {'/',0};
XCHAR           keybackslash[] = {'\\',0};
XCHAR           keycolon[] = {':',0};
XCHAR           keysemicolon[] = {';',0};
XCHAR           keyleftparen[] = {'(',0};
XCHAR           keyrightparen[] = {')',0};
XCHAR           keyleftbracket[] = {'[',0};
XCHAR           keyrightbracket[] = {']',0};
XCHAR           keyleftbrace[] = {'{',0};
XCHAR           keyrightbrace[] = {'}',0};
XCHAR           keyor[] = {'|',0};
XCHAR           keytilde[] = {'~',0};
XCHAR           keydollar[] = {'$',0};
XCHAR           keyampersand[] = {'&',0};
XCHAR           keyat[] = {'@',0};
XCHAR           keyquote[] = {'"',0};
XCHAR           keyperiod[] = {'.',0};
XCHAR           keycomma[] = {',',0};
XCHAR           keyquestion[] = {'?',0};
XCHAR           keyexclaim[] = {'!',0};
XCHAR           keyunderscore[] = {'_',0};
XCHAR           keypound[] = {'#',0};
XCHAR			keypercent[] = {'%',0};
XCHAR			keyhat[] = {'^',0};
XCHAR			keystar[] = {'*',0};
XCHAR			keyequal[] = {'=',0};
XCHAR			keylessthan[] = {'<',0};
XCHAR			keygreaterthan[] = {'>',0};
//XCHAR           keysinglequote[] = {"'"};

XCHAR *pUpperKeyNames[] =		{
								"1","2","3","4","5","6","7","8","9","0",delKey,
								"Q","W","E","R","T","Y","U","I","O","P",keyforwardslash,
								shiftKey,"A","S","D","F","G","H","J","K","L",shiftKey,
								spaceKey,keycomma,"Z","X","C","V","B","N","M",keyperiod,clearKey						
								};
XCHAR *pLowerKeyNames[] =		{
								"1","2","3","4","5","6","7","8","9","0",delKey,
								"q","w","e","r","t","y","u","i","o","p",keyforwardslash,
								shiftKey,"a","s","d","f","g","h","j","k","l",shiftKey,
								spaceKey,keycomma,"z","x","c","v","b","n","m",keyperiod,clearKey
								};
XCHAR *pSymbolKeyNames[] = 		{
								"1","2","3","4","5","6","7","8","9","0",delKey,
								keyexclaim,keyat,keypound,keydollar,keypercent,keyhat,keyampersand,keystar,keyleftparen,keyrightparen,keyforwardslash,
								shiftKey,keydash,keyequal,keycolon,keyquote,keysemicolon,keyquestion,keyunderscore,keylessthan,keygreaterthan,shiftKey,
								spaceKey,keycomma,keybackslash,keyleftbracket,keyrightbracket,keyleftbrace,keyrightbrace,keyor,keytilde,keyperiod,clearKey
								};

XCHAR           			EnterCodeText[TEBUFFERSIZE] = "Microchip Technology Inc. Chandler, AZ";
WORD 						CurrentKeySetStatus;
WORD 						AlternateMenuStatus;		// used to distinguish if we are in normal menu or alternate menu

USER_INPUT_REQUEST			CurrentDownloadRequest;


/************************************************************************
 Function: CreateDemoSelection()
                                                                       
 Overview: Creates the menu of small demos each represented 
 		   by an icon.
                                                                       
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void CreateGoogleMapScreen(void)
{
	TEXTENTRY   *pTe;  
	
    GOLFree();                      		// free memory for the objects in the previous linked list and start new list
	
	SetColor(BLACK);						
    ClearDevice();							// clear the screen 


	CurrentKeySetStatus = SHOWUPPERKEYS;
	AlternateMenuStatus = NORMALMENU;

	// list box of pre-defined addresses
	LbCreate
    (
    	LIST_ID,                			// address list ID
        ADDLIST_X,
        ADDLIST_Y,              		
        ADDLIST_X+ADDLIST_WIDTH,
        ADDLIST_Y+ADDLIST_HEIGHT,
        LB_SINGLE_SEL|LB_DISABLED,   		// will not be displayed initally 
        (XCHAR *)savedAddresses,            // list of saved addresses
        pGoogleMapMenuScheme2
    );                  // use alternate scheme

    // Keyboard object to enter the address
    pTe = TeCreate
    (
		KEYBOARD_ID,                 		// object’s ID
        KEYBOARD_X,
        KEYBOARD_Y,
        KEYBOARD_X+KEYBOARD_WIDTH,
        KEYBOARD_Y+KEYBOARD_HEIGHT,              
        TE_DISABLED,                		// disable the object after creation
        KEYBOARDWIDTH,						// number of horizontal keys
        KEYBOARDHEIGHT,         			// number of vertical keys
        pUpperKeyNames,         			// pointer to the array of key names
        (XCHAR *)EnterCodeText, 			// pointer to the string that will be displayed in the object
        TEBUFFERSIZE,
        NULL,                   			// pointer to the font of the diplayed text					
        pGoogleMapMenuScheme2
	);                          

	// assign command keys
	TeSetKeyCommand(pTe, DELCHKEYINDEX, TE_DELETE_COM); //assign the 6th key to be a Delete command
    TeSetKeyCommand(pTe, SPACEKEYINDEX, TE_SPACE_COM);  //assign the 5th key to be a Space command
    TeSetKeyCommand(pTe, SHIFTMODEKEYINDEX1, TE_ENTER_COM);   //assign the Enter command to Mode key
    TeSetKeyCommand(pTe, SHIFTMODEKEYINDEX2, TE_ENTER_COM);   //assign the Enter command to Mode key
    TeSetKeyCommand(pTe, CLEARKEYINDEX, TE_ENTER_COM);  //assign the Enter command to Clear text key
//    TeSetKeyCommand(pTe, ENTERKEYINDEX, TE_ENTER_COM);  //assign the Enter command to Enter key
	
    // Zoom In Button
    BtnCreate
    (
        ZOOMIN_ID,                 			// button ID
        ZOOMIN_X,                			// button dimensions
        ZOOMIN_Y,
        ZOOMIN_X+ZOOMBUTTONWIDTH,
        ZOOMIN_Y+ZOOMBUTTONHEIGHT, 
        10,                           	
        BUTTONINITSTATE,				// draw the button
        NULL,								// no bitmap
        (XCHAR *)zoomInStr,   				// 
        pGoogleMapMenuScheme1				// style scheme used
    );       

    // Zoom In Button
    BtnCreate
    (
        ZOOMOUT_ID,                 		// button ID
        ZOOMOUT_X,                			// button dimensions
        ZOOMOUT_Y,
        ZOOMOUT_X+ZOOMBUTTONWIDTH,
        ZOOMOUT_Y+ZOOMBUTTONHEIGHT, 
        10,                           	
        BUTTONINITSTATE,				// draw the button
        NULL,								// no bitmap
        (XCHAR *)zoomOutStr,   				// 
        pGoogleMapMenuScheme1				// style scheme used
    );    
    
    SldCreate
    (
        ZOOMSLIDE_ID,                       // ID
        ZOOMSLIDE_X,						// dimension
        ZOOMSLIDE_Y,
        ZOOMSLIDE_X + ZOOMSLIDEWIDTH,
        ZOOMSLIDE_Y + ZOOMSLIDEHEIGHT,     
        SLD_DRAW|SLD_VERTICAL,                           
        20,       							// range
        1,                                 	// page
        15,									// pos
        pGoogleMapMenuScheme1				// style scheme used
    );             
    
    // Zoom In Button
    BtnCreate
    (
        MAP_ID,                 			// button ID
        MAP_X,                				// button dimensions
        MAP_Y,
        MAP_X+GOOGLEMENUWIDTH,
        MAP_Y+GOOGLEMENUHEIGHT, 
        MENU_BTN_RADIUS,                          	
        BUTTONINITSTATE,				// draw the button
        NULL,								// no bitmap
        (XCHAR *)mapStr,   					// 
        pGoogleMapMenuScheme1				// style scheme used
    );        

    BtnCreate
    (
        SAT_ID,                 			// button ID
        SAT_X,                				// button dimensions
        SAT_Y,
        SAT_X+GOOGLEMENUWIDTH,
        SAT_Y+GOOGLEMENUHEIGHT, 
        MENU_BTN_RADIUS,                          	
        BUTTONINITSTATE,				// draw the button
        NULL,								// no bitmap
        (XCHAR *)satStr,   					// 
        pGoogleMapMenuScheme1				// style scheme used
    );    
    
    BtnCreate
    (
        TER_ID,                 			// button ID
        TER_X,                				// button dimensions
        TER_Y,
        TER_X+GOOGLEMENUWIDTH,
        TER_Y+GOOGLEMENUHEIGHT, 
        MENU_BTN_RADIUS,                          	
        BUTTONINITSTATE,					// draw the button
        NULL,								// no bitmap
        (XCHAR *)terStr,   					// 
        pGoogleMapMenuScheme1				// style scheme used
    );        

    BtnCreate
    (
        ENTER_ID,                 			// button ID
        ENTER_X,                				// button dimensions
        ENTER_Y,
        ENTER_X+GOOGLEMENUWIDTH,
        ENTER_Y+GOOGLEMENUHEIGHT, 
        MENU_BTN_RADIUS,                          	
        BUTTONINITSTATE,				// draw the button
        NULL,								// no bitmap
        (XCHAR *)enterStr,   					// 
        pGoogleMapMenuScheme1				// style scheme used
    );            

    // Initialize default values and call callback first time
    CurrentDownloadRequest.vZoom = 16;
    CurrentDownloadRequest.vMapType = GOOGLE_STATIC_MAP_HYBRID;
    CurrentDownloadRequest.szCenter = (unsigned char*)"33.304546,-111.882373";
    CurrentDownloadRequest.newItemsFlag = NEWADDRESSREQUEST;
   	GoogleStaticMapUserAction(&CurrentDownloadRequest);
   	CurrentDownloadRequest.newItemsFlag = 0;
   	CurrentDownloadRequest.reqState = DECODING_MAP_IMAGE_ENABLED;

}

/************************************************************************
 Function: WORD GoogleMapDrawCallback()

 Overview: This function must be called within GOLDrawCallback() function. 
		   User drawing should be done here. GOL will not change color, line 
 		   type and clipping region settings while  this function 
 		   returns zero.

 Input: none

 Output: If the function returns non-zero the draw control will be passed 
		to GOL. GOLDraw() can proceed and re-draw objects that needs 
		to be redrawn.
************************************************************************/
WORD GoogleMapDrawCallback()
{

#ifdef __PIC24F__
	
	static DWORD time = 0;
	static int   processing = 0, prev = 0;
	
	#define STATUSDIST   (25)
	#define STATUSRADIUS (6)
	#define STATUSXSTART ((GetMaxX()>>1))
	#define STATUSYSTART ((GetMaxY()>>1))
	#define STATUSMIDPTS (STATUSDIST*(.707))
	
	typedef struct 
	{
        WORD xPos;
        WORD yPos;
 	} Animate_Pts;	
	
	Animate_Pts positions[8] = 	{ 	{STATUSXSTART,				STATUSYSTART-STATUSDIST},
									{STATUSXSTART+STATUSMIDPTS, STATUSYSTART-STATUSMIDPTS},
									{STATUSXSTART+STATUSDIST, 	STATUSYSTART},
									{STATUSXSTART+STATUSMIDPTS, STATUSYSTART+STATUSMIDPTS},
									{STATUSXSTART, 				STATUSYSTART+STATUSDIST},
									{STATUSXSTART-STATUSMIDPTS, STATUSYSTART+STATUSMIDPTS},
									{STATUSXSTART-STATUSDIST, 	STATUSYSTART},
									{STATUSXSTART-STATUSMIDPTS, STATUSYSTART-STATUSMIDPTS}
								};								
	
	// draw the processing animation, this will draw small circles in a circular array with
	// the outline of one of the circle a moving green color
	if ((CurrentDownloadRequest.reqState == DECODING_MAP_IMAGE_ENABLED) && (AlternateMenuStatus == NORMALMENU))
	{
		if(TickGet() - time >= (TICK_SECOND>>3))
	    {
	    	time = TickGet();
			
		    if (processing < 7)
		    	processing++;
		    else
		    	processing = 0;
		    
		    // draw the moving green circle
			SetColor(BRIGHTGREEN);
			while(!FillCircle(positions[processing].xPos,positions[processing].yPos,STATUSRADIUS));
			
			// refresh the previous green circle		    
			if (prev != processing)
			{
				SetColor(RGB565CONVERT(0xA5, 0xA6, 0xA5));
				while(!Circle(positions[prev].xPos,positions[prev].yPos,STATUSRADIUS));
				SetColor(BLACK);
				while(!FillCircle(positions[prev].xPos,positions[prev].yPos,STATUSRADIUS-1));
			}
			prev = processing;
		    
	    }
	}	
	else
	{
	   	processing = 0;
	   	prev = 0;
	}	

#endif //#ifdef __PIC24F__
	
	return 1;
}	

/************************************************************************
 Function: WORD GoogleMapMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the demo selection. Based on user selection
 		   A demo is enabled by switching the screen state. Icons used
 		   changes from colored version to monochrome version when pressed. 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: none
************************************************************************/
WORD GoogleMapMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
	unsigned char i;
	SLIDER      *pSld;
	LISTBOX		*pListBox;
	XCHAR 		*pSrc, *pDst;
	LISTITEM    *pItem;

    pSld = (SLIDER *)GOLFindObject(ZOOMSLIDE_ID);	

    switch(GetObjID(pObj))
    {
	    case ZOOMSLIDE_ID:
	        // Process message by default
            SldMsgDefault(objMsg, (SLIDER *)pObj, pMsg);
			i = SldGetPos(pSld);

			if((objMsg == OBJ_MSG_PASSIVE) && (pMsg->uiEvent == EVENT_RELEASE))
			{
				if(CurrentDownloadRequest.vZoom != i)
				{
					CurrentDownloadRequest.vZoom = i;
					CurrentDownloadRequest.newItemsFlag |= NEWZOOMREQUEST;
				}
			}
            break;
            
        case ZOOMOUT_ID:				// zoom in button was pressed
            if(objMsg == BTN_MSG_RELEASED)
            {
                SldDecPos(pSld);
                SetState(pSld, SLD_DRAW_THUMB);
				i = SldGetPos(pSld);
				if(CurrentDownloadRequest.vZoom != i)
				{
					CurrentDownloadRequest.vZoom = i;
					CurrentDownloadRequest.newItemsFlag |= NEWZOOMREQUEST;
				}
            }
            break;

        case ZOOMIN_ID:					// zoom out button was pressed
            if(objMsg == BTN_MSG_RELEASED)
            {
                SldIncPos(pSld);
                SetState(pSld, SLD_DRAW_THUMB);
				i = SldGetPos(pSld);
				if(CurrentDownloadRequest.vZoom != i)
				{
					CurrentDownloadRequest.vZoom = i;
					CurrentDownloadRequest.newItemsFlag |= NEWZOOMREQUEST;
				}
            }
            break;
		
		case MAP_ID:
			if(objMsg == BTN_MSG_RELEASED)
            {
				if (CurrentDownloadRequest.vMapType != GOOGLE_STATIC_MAP_ROADMAP)
				{
	            	CurrentDownloadRequest.vMapType = GOOGLE_STATIC_MAP_ROADMAP;
                	CurrentDownloadRequest.newItemsFlag |= NEWMAPTYPEREQUEST;
    			}
            }
			break;
		
		case SAT_ID:
			if(objMsg == BTN_MSG_RELEASED)
			{
				if (CurrentDownloadRequest.vMapType != GOOGLE_STATIC_MAP_HYBRID)
				{
					CurrentDownloadRequest.vMapType = GOOGLE_STATIC_MAP_HYBRID;
                	CurrentDownloadRequest.newItemsFlag |= NEWMAPTYPEREQUEST;
    			}
			}
			break;

		case TER_ID:
			if(objMsg == BTN_MSG_RELEASED)
			{
				if (AlternateMenuStatus == NORMALMENU)
	            {
					if (CurrentDownloadRequest.vMapType != GOOGLE_STATIC_MAP_TERRAIN)
					{
			            CurrentDownloadRequest.vMapType = GOOGLE_STATIC_MAP_TERRAIN;
    	    	        CurrentDownloadRequest.newItemsFlag |= NEWMAPTYPEREQUEST;
    				}
		        }
		        else // we are at ALTERNATEMENU
		        {
					pListBox = (LISTBOX *)GOLFindObject(LIST_ID);
					
			        // display the list box here
		            if (GetState(pListBox, BTN_DISABLED))
		            {
						BtnSetText((BUTTON *)GOLFindObject(TER_ID), (XCHAR *)doneStr);
			        	ClrState(pListBox, LB_DISABLED);
			        	SetState(pListBox, LB_DRAW);
				        SetState((TEXTENTRY *)GOLFindObject(KEYBOARD_ID), TE_DISABLED);
				        SetState((BUTTON *)GOLFindObject(ENTER_ID), BTN_DISABLED|BTN_HIDE);
					}
					else
					{
						pItem = LbGetSel(pListBox, NULL);
						if (pItem != NULL)
						{
							pSrc = pItem->pText;
							pDst = TeGetBuffer((TEXTENTRY *)GOLFindObject(KEYBOARD_ID));
							
							// copy the selected text to the keyboard buffer
							while (*pSrc != '\n')
							{
								*pDst++ = *pSrc++;
							}
							*pDst = 0;
						}
						
			        	SetState(pListBox, LB_DISABLED|LB_HIDE);
			        	SetState(pListBox, LB_DRAW);
				        ClrState((BUTTON *)GOLFindObject(ENTER_ID), BTN_DISABLED);
				        SetState((BUTTON *)GOLFindObject(ENTER_ID), BTN_DRAW);
				        
						// set to normal menu screen
						AlternateMenuStatus = NORMALMENU;
						
			            // enable all the objects in the screen then disable the keyboard. 
			            // Keyboard should be hidden and all objects redrawn
			            ClrState((BUTTON *)GOLFindObject(MAP_ID), 		BTN_DISABLED);
			            ClrState((BUTTON *)GOLFindObject(SAT_ID), 		BTN_DISABLED);
			            ClrState((BUTTON *)GOLFindObject(TER_ID), 		BTN_DISABLED);
	
			            SetState((BUTTON *)GOLFindObject(MAP_ID), 		BTN_DRAW);
			            SetState((BUTTON *)GOLFindObject(SAT_ID), 		BTN_DRAW);
	
			            BtnSetText((BUTTON *)GOLFindObject(TER_ID), 	(XCHAR *)terStr);
			            SetState((BUTTON *)GOLFindObject(TER_ID), 		BTN_DRAW);
	
			            ClrState((SLIDER *)GOLFindObject(ZOOMSLIDE_ID), SLD_DISABLED);
			            ClrState((BUTTON *)GOLFindObject(ZOOMIN_ID), 	BTN_DISABLED);
			            ClrState((BUTTON *)GOLFindObject(ZOOMOUT_ID), 	BTN_DISABLED);
	
			            SetState((SLIDER *)GOLFindObject(ZOOMSLIDE_ID), SLD_DRAW);
			            SetState((BUTTON *)GOLFindObject(ZOOMIN_ID), 	BTN_DRAW);
			            SetState((BUTTON *)GOLFindObject(ZOOMOUT_ID), 	BTN_DRAW);
			            
						SetState((TEXTENTRY*)GOLFindObject(KEYBOARD_ID), TE_DISABLED|TE_HIDE);
						SetState((LISTBOX *)GOLFindObject(LIST_ID), 	LB_DISABLED|LB_HIDE);
					
						BtnSetText((BUTTON *)GOLFindObject(ENTER_ID), (XCHAR *)enterStr);
						// check if the buffer is empty if it is do not overwrite the current address
						// but still indicate a new map request to force redrawing
						if (*(unsigned char*)TeGetBuffer((TEXTENTRY*)GOLFindObject(KEYBOARD_ID)) != 0)
							CurrentDownloadRequest.szCenter = (unsigned char*)TeGetBuffer((TEXTENTRY*)GOLFindObject(KEYBOARD_ID));
	            		CurrentDownloadRequest.newItemsFlag |= NEWADDRESSREQUEST;				        
					}
			    }

			}
			break;

	    case ENTER_ID:					// enter address button was pressed
            if (objMsg == BTN_MSG_RELEASED)
            {
	            if (AlternateMenuStatus == NORMALMENU)
	            {
		            // Cancel any image download operations that may be in 
		            // progress right now (so as to prevent a map from finish 
		            // downloading and starting the JPEG decoder, which 
		            // overwrite the keyboard we are about to display.)
		            GoogleStaticMapReset(&MapDownloaderState);		            
		            
					// Cancel any JPEG decoding operation that may be in 
					// progress right now (so as to prevent a map from 
					// overdrawing on top of the keyboard we are about to 
					// display)
		            JPEGInit();
		            
					// set to alternate menu screen
					AlternateMenuStatus = ALTERNATEMENU;
		            // disable all the objects in the screen except the enter button and
		            // the keyboard. All disabled objects will not be hidden
		            SetState((BUTTON *)GOLFindObject(MAP_ID), 		BTN_HIDE|BTN_DISABLED);
		            SetState((BUTTON *)GOLFindObject(SAT_ID), 		BTN_HIDE|BTN_DISABLED);
                
                    #if !defined(__PIC32MX__)
		            BtnSetText((BUTTON *)GOLFindObject(TER_ID), 	(XCHAR *)loadStr);
		            SetState((BUTTON *)GOLFindObject(TER_ID), 		BTN_DRAW);
	
		            SetState((SLIDER *)GOLFindObject(ZOOMSLIDE_ID), SLD_DISABLED);
		            SetState((BUTTON *)GOLFindObject(ZOOMIN_ID), 	BTN_DISABLED);
		            SetState((BUTTON *)GOLFindObject(ZOOMOUT_ID), 	BTN_DISABLED);
		            
					// use the key upper case set as default everytime the keyboard is opened
					//CurrentKeySetStatus = SHOWUPPERKEYS;
					//AlternateKeys((TEXTENTRY*)pObj, pUpperKeyNames);
					
					ClrState((TEXTENTRY*)GOLFindObject(KEYBOARD_ID), TE_DISABLED);
					SetState((TEXTENTRY*)GOLFindObject(KEYBOARD_ID), TE_DRAW);
					
					BtnSetText((BUTTON *)GOLFindObject(ENTER_ID), (XCHAR *)doneStr);
                    #else
                    BtnSetText((BUTTON *)GOLFindObject(TER_ID), 	(XCHAR *)doneStr);
		            SetState((BUTTON *)GOLFindObject(TER_ID), 		BTN_DRAW);

					pListBox = (LISTBOX *)GOLFindObject(LIST_ID);
					
			        // display the list box here
		            if (GetState(pListBox, BTN_DISABLED))
		            {
						BtnSetText((BUTTON *)GOLFindObject(TER_ID), (XCHAR *)doneStr);
			        	ClrState(pListBox, LB_DISABLED);
			        	SetState(pListBox, LB_DRAW);
				        SetState((TEXTENTRY *)GOLFindObject(KEYBOARD_ID), TE_DISABLED);
				        SetState((BUTTON *)GOLFindObject(ENTER_ID), BTN_DISABLED|BTN_HIDE);
					}
					else
					{
						pItem = LbGetSel(pListBox, NULL);
						if (pItem != NULL)
						{
							pSrc = pItem->pText;
							pDst = TeGetBuffer((TEXTENTRY *)GOLFindObject(KEYBOARD_ID));
							
							// copy the selected text to the keyboard buffer
							while (*pSrc != '\n')
							{
								*pDst++ = *pSrc++;
							}
							*pDst = 0;
						}
						
			        	SetState(pListBox, LB_DISABLED|LB_HIDE);
			        	SetState(pListBox, LB_DRAW);
				        ClrState((BUTTON *)GOLFindObject(ENTER_ID), BTN_DISABLED);
				        SetState((BUTTON *)GOLFindObject(ENTER_ID), BTN_DRAW);
				        
						// set to normal menu screen
						AlternateMenuStatus = NORMALMENU;
						
			            // enable all the objects in the screen then disable the keyboard. 
			            // Keyboard should be hidden and all objects redrawn
			            ClrState((BUTTON *)GOLFindObject(MAP_ID), 		BTN_DISABLED);
			            ClrState((BUTTON *)GOLFindObject(SAT_ID), 		BTN_DISABLED);
			            ClrState((BUTTON *)GOLFindObject(TER_ID), 		BTN_DISABLED);
	
			            SetState((BUTTON *)GOLFindObject(MAP_ID), 		BTN_DRAW);
			            SetState((BUTTON *)GOLFindObject(SAT_ID), 		BTN_DRAW);
	
			            BtnSetText((BUTTON *)GOLFindObject(TER_ID), 	(XCHAR *)terStr);
			            SetState((BUTTON *)GOLFindObject(TER_ID), 		BTN_DRAW);
	
			            ClrState((SLIDER *)GOLFindObject(ZOOMSLIDE_ID), SLD_DISABLED);
			            ClrState((BUTTON *)GOLFindObject(ZOOMIN_ID), 	BTN_DISABLED);
			            ClrState((BUTTON *)GOLFindObject(ZOOMOUT_ID), 	BTN_DISABLED);
	
			            SetState((SLIDER *)GOLFindObject(ZOOMSLIDE_ID), SLD_DRAW);
			            SetState((BUTTON *)GOLFindObject(ZOOMIN_ID), 	BTN_DRAW);
			            SetState((BUTTON *)GOLFindObject(ZOOMOUT_ID), 	BTN_DRAW);
			            
						SetState((TEXTENTRY*)GOLFindObject(KEYBOARD_ID), TE_DISABLED|TE_HIDE);
						SetState((LISTBOX *)GOLFindObject(LIST_ID), 	LB_DISABLED|LB_HIDE);
					
						BtnSetText((BUTTON *)GOLFindObject(ENTER_ID), (XCHAR *)enterStr);
						// check if the buffer is empty if it is do not overwrite the current address
						// but still indicate a new map request to force redrawing
						if (*(unsigned char*)TeGetBuffer((TEXTENTRY*)GOLFindObject(KEYBOARD_ID)) != 0)
							CurrentDownloadRequest.szCenter = (unsigned char*)TeGetBuffer((TEXTENTRY*)GOLFindObject(KEYBOARD_ID));
	            		CurrentDownloadRequest.newItemsFlag |= NEWADDRESSREQUEST;				        
					}
                 #endif
				} 
				else  
				{
					// set to normal menu screen
					AlternateMenuStatus = NORMALMENU;
					
		            // enable all the objects in the screen then disable the keyboard. 
		            // Keyboard should be hidden and all objects redrawn
		            ClrState((BUTTON *)GOLFindObject(MAP_ID), 		BTN_DISABLED);
		            ClrState((BUTTON *)GOLFindObject(SAT_ID), 		BTN_DISABLED);
		            ClrState((BUTTON *)GOLFindObject(TER_ID), 		BTN_DISABLED);

		            SetState((BUTTON *)GOLFindObject(MAP_ID), 		BTN_DRAW);
		            SetState((BUTTON *)GOLFindObject(SAT_ID), 		BTN_DRAW);

		            BtnSetText((BUTTON *)GOLFindObject(TER_ID), 	(XCHAR *)terStr);
		            SetState((BUTTON *)GOLFindObject(TER_ID), 		BTN_DRAW);

		            ClrState((SLIDER *)GOLFindObject(ZOOMSLIDE_ID), SLD_DISABLED);
		            ClrState((BUTTON *)GOLFindObject(ZOOMIN_ID), 	BTN_DISABLED);
		            ClrState((BUTTON *)GOLFindObject(ZOOMOUT_ID), 	BTN_DISABLED);

		            SetState((SLIDER *)GOLFindObject(ZOOMSLIDE_ID), SLD_DRAW);
		            SetState((BUTTON *)GOLFindObject(ZOOMIN_ID), 	BTN_DRAW);
		            SetState((BUTTON *)GOLFindObject(ZOOMOUT_ID), 	BTN_DRAW);
		            
					SetState((TEXTENTRY*)GOLFindObject(KEYBOARD_ID), TE_DISABLED|TE_HIDE);
					SetState((LISTBOX *)GOLFindObject(LIST_ID), 	LB_DISABLED|LB_HIDE);
				
					BtnSetText((BUTTON *)GOLFindObject(ENTER_ID), (XCHAR *)enterStr);
					
					// check if the buffer is empty if it is do not overwrite the current address
					// but still indicate a new map request to force redrawing
					if (*(unsigned char*)TeGetBuffer((TEXTENTRY*)GOLFindObject(KEYBOARD_ID)) != 0)
						CurrentDownloadRequest.szCenter = (unsigned char*)TeGetBuffer((TEXTENTRY*)GOLFindObject(KEYBOARD_ID));
            		CurrentDownloadRequest.newItemsFlag |= NEWADDRESSREQUEST;
					
				}	
				
            }           
            break;
            
        case KEYBOARD_ID:				// shift keyboard key is pressed
        	if (objMsg == TE_MSG_ENTER)
        	{	
            	// 	check first if this is for the key with mode label
            	if((((TEXTENTRY *)pObj)->pActiveKey->index == SHIFTMODEKEYINDEX1) || (((TEXTENTRY *)pObj)->pActiveKey->index == SHIFTMODEKEYINDEX2))
            	{
	            	if (CurrentKeySetStatus == SHOWUPPERKEYS)
	            	{
	            		CurrentKeySetStatus = SHOWLOWERKEYS;
			            AlternateKeys((TEXTENTRY*)pObj, pLowerKeyNames);

	            	}
	            	else if (CurrentKeySetStatus == SHOWLOWERKEYS)
	            	{
	            		CurrentKeySetStatus = SHOWSYMBOLS;
			            AlternateKeys((TEXTENTRY*)pObj, pSymbolKeyNames);
	            	}	
	            	else if (CurrentKeySetStatus == SHOWSYMBOLS)
	            	{
	            		CurrentKeySetStatus = SHOWUPPERKEYS;
			            AlternateKeys((TEXTENTRY*)pObj, pUpperKeyNames);
	            	}	
	             	
		            // if shift is pressed update the keys to show the alternates
	                SetState(pObj, TE_DRAW);
	            }
	            else if(((TEXTENTRY *)pObj)->pActiveKey->index == CLEARKEYINDEX)
	            {
	                TeClearBuffer((TEXTENTRY *)pObj);
	                SetState(pObj, TE_UPDATE_TEXT);
	            }
	        }
            break;
            
        default:
            break;
    }
    
    if(CurrentDownloadRequest.newItemsFlag)
    {
    	GoogleStaticMapUserAction(&CurrentDownloadRequest);
    	CurrentDownloadRequest.newItemsFlag = 0;
    }
    
    return (1);
}



/************************************************************************
 Function: void InitGoogleMapStyleScheme()
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void InitGoogleMapStyleScheme()
{

    pGoogleMapMenuScheme1->EmbossLtColor =  RGB565CONVERT(0xCE, 0xCF, 0xCE);
    pGoogleMapMenuScheme1->EmbossDkColor =  RGB565CONVERT(0xA5, 0xA6, 0xA5);
	pGoogleMapMenuScheme1->TextColor0 = BLACK; 
	pGoogleMapMenuScheme1->TextColor1 = WHITE; 
	pGoogleMapMenuScheme1->Color0 = BLACK;    
	pGoogleMapMenuScheme1->Color1 = BRIGHTGREEN; 
	pGoogleMapMenuScheme1->pFont = (void *) &GOLSmallFont;
	pGoogleMapMenuScheme1->CommonBkColor = BLACK;

    pGoogleMapMenuScheme2->EmbossLtColor =  RGB565CONVERT(0xCE, 0xCF, 0xCE);
    pGoogleMapMenuScheme2->EmbossDkColor =  RGB565CONVERT(0xA5, 0xA6, 0xA5);
	pGoogleMapMenuScheme2->TextColor0 = RGB565CONVERT(0x6B, 0x69, 0x6B);    
	pGoogleMapMenuScheme2->TextColor1 = WHITE; 
	pGoogleMapMenuScheme2->Color0 = BLACK;    
	pGoogleMapMenuScheme2->Color1 = RGB565CONVERT(0x52, 0x55, 0x4A); 
    pGoogleMapMenuScheme2->pFont = (void *) &GOLSmallFont;
	pGoogleMapMenuScheme2->CommonBkColor = BLACK;

}

void AlternateKeys(TEXTENTRY *pTe, XCHAR  *pKeyList[])
{
    int     index;
    
    // Use this code to initialize the keys normally
    // randomVal here is not random at all
    for(index = 0; index < (KEYBOARDWIDTH*KEYBOARDHEIGHT); index++)
    {
	    TeSetKeyText(pTe, index, pKeyList[index]);
    }

    return;
}
